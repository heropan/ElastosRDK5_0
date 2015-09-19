
#include "content/CComponentName.h"
#include "content/pm/CServiceInfo.h"
#include "content/pm/CApplicationInfo.h"
#include "view/inputmethod/CInputMethodInfo.h"
#include "view/inputmethod/CInputMethodSubtype.h"
#include "utility/Xml.h"
#include "R.h"

using Elastos::Droid::R;
using Elastos::Droid::Content::CComponentName;
using Elastos::Droid::Content::Pm::CServiceInfo;
using Elastos::Droid::Content::Pm::CApplicationInfo;
using Elastos::Droid::Content::Res::ITypedArray;
using Elastos::Droid::Content::Res::IResources;
using Elastos::Droid::View::InputMethod::CInputMethodSubtype;
using Elastos::Droid::Utility::IAttributeSet;
using Elastos::Droid::Utility::Xml;

namespace Elastos {
namespace Droid {
namespace View {
namespace InputMethod {

const String CInputMethodInfo::TAG("InputMethodInfo");

CInputMethodInfo::CInputMethodInfo()
    : mIsDefaultResId(0)
    , mIsAuxIme(FALSE)
{}

ECode CInputMethodInfo::constructor()
{
    return NOERROR;
}

ECode CInputMethodInfo::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IResolveInfo* service)
{
    return Init(context, service, NULL);
}

ECode CInputMethodInfo::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IResolveInfo* service,
    /* [in] */ IObjectStringMap* additionalSubtypesMap)
{
    return Init(context, service, additionalSubtypesMap);
}

ECode CInputMethodInfo::constructor(
    /* [in] */ const String& packageName,
    /* [in] */ const String& className,
    /* [in] */ ICharSequence* label,
    /* [in] */ const String& settingsActivity)
{
    AutoPtr<CResolveInfo> ri;
    CResolveInfo::NewByFriend((CResolveInfo**)&ri);
    AutoPtr<CServiceInfo> si;
    CServiceInfo::NewByFriend((CServiceInfo**)&si);
    AutoPtr<CApplicationInfo> ai;
    CApplicationInfo::NewByFriend((CApplicationInfo**)&ai);
    ai->mPackageName = packageName;
    ai->mEnabled = TRUE;
    si->mApplicationInfo = ai.Get();
    si->mEnabled = TRUE;
    si->mPackageName = packageName;
    si->mName = className;
    si->mExported = TRUE;
    si->mNonLocalizedLabel = label;
    ri->mServiceInfo = si.Get();
    mService = ri;
    AutoPtr<CComponentName> componentName;
    CComponentName::NewByFriend(si->mPackageName,
            si->mName, (CComponentName**)&componentName);
    componentName->FlattenToShortString(&mId);
    mSettingsActivityName = settingsActivity;
    mIsDefaultResId = 0;
    mIsAuxIme = FALSE;
    return NOERROR;
}

ECode CInputMethodInfo::ReadFromParcel(
    /* [in] */ IParcel *source)
{
    source->ReadString(&mId);
    source->ReadString(&mSettingsActivityName);
    source->ReadInt32(&mIsDefaultResId);
    source->ReadBoolean(&mIsAuxIme);
    source->ReadInterfacePtr((Handle32*)&mService);

    Int32 size = 0;
    source->ReadInt32(&size);
    for (Int32 i = 0; i < size; i++) {
        AutoPtr<IInputMethodSubtype> subtype;
        source->ReadInterfacePtr((Handle32*)&subtype);
        mSubtypes.PushBack(subtype);
    }

    return NOERROR;
}

ECode CInputMethodInfo::WriteToParcel(
    /* [in] */ IParcel *dest)
{
    dest->WriteString(mId);
    dest->WriteString(mSettingsActivityName);
    dest->WriteInt32(mIsDefaultResId);
    dest->WriteBoolean(mIsAuxIme);
    dest->WriteInterfacePtr((IInterface*)(IResolveInfo*)mService.Get());

    Int32 size = mSubtypes.GetSize();
    dest->WriteInt32(size);
    List<AutoPtr<IInputMethodSubtype> >::Iterator iter = mSubtypes.Begin();
    for (; iter != mSubtypes.End(); ++iter) {
        dest->WriteInterfacePtr((*iter).Get());
    }

    return NOERROR;
}

ECode CInputMethodInfo::GetId(
    /* [out] */ String* id)
{
    VALIDATE_NOT_NULL(id);
    *id = mId;
    return NOERROR;
}

ECode CInputMethodInfo::GetPackageName(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);
    *name = ((CServiceInfo*)mService->mServiceInfo.Get())->mPackageName;
    return NOERROR;
}

ECode CInputMethodInfo::GetServiceName(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);
    *name = ((CServiceInfo*)mService->mServiceInfo.Get())->mName;
    return NOERROR;
}

ECode CInputMethodInfo::GetServiceInfo(
    /* [out] */ IServiceInfo** info)
{
    VALIDATE_NOT_NULL(info);
    *info = mService->mServiceInfo;
    REFCOUNT_ADD(*info);
    return NOERROR;
}

ECode CInputMethodInfo::GetComponent(
    /* [out] */ IComponentName** name)
{
    VALIDATE_NOT_NULL(name);
    return CComponentName::New(((CServiceInfo*)mService->mServiceInfo.Get())->mPackageName,
            ((CServiceInfo*)mService->mServiceInfo.Get())->mName, name);
}

ECode CInputMethodInfo::LoadLabel(
    /* [in] */ IPackageManager* pm,
    /* [out] */ ICharSequence** str)
{
    VALIDATE_NOT_NULL(str);
    *str = NULL;
    return mService->LoadLabel(pm, str);
}

ECode CInputMethodInfo::LoadIcon(
    /* [in] */ IPackageManager* pm,
    /* [out] */ IDrawable** drawable)
{
    VALIDATE_NOT_NULL(drawable);
    return mService->LoadIcon(pm, drawable);
}

ECode CInputMethodInfo::GetSettingsActivity(
    /* [out] */ String* settings)
{
    VALIDATE_NOT_NULL(settings);
    *settings = mSettingsActivityName;
    return NOERROR;
}

ECode CInputMethodInfo::GetSubtypeCount(
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count);
    *count = mSubtypes.GetSize();
    return NOERROR;
}

ECode CInputMethodInfo::GetSubtypeAt(
    /* [in] */ Int32 index,
    /* [out] */ IInputMethodSubtype** subtype)
{
    VALIDATE_NOT_NULL(subtype);
    *subtype = mSubtypes[index];
    REFCOUNT_ADD(*subtype);
    return NOERROR;
}

ECode CInputMethodInfo::GetIsDefaultResourceId(
    /* [out] */ Int32* id)
{
    VALIDATE_NOT_NULL(id);
    *id = mIsDefaultResId;
    return NOERROR;
}

ECode CInputMethodInfo::IsAuxiliaryIme(
    /* [in] */ Boolean* auxIme)
{
    assert(auxIme != NULL);
    *auxIme = mIsAuxIme;
    return NOERROR;
}

ECode CInputMethodInfo::GetHashCode(
    /* [out] */ Int32* code)
{
    VALIDATE_NOT_NULL(code);
    *code = mId.GetHashCode();
    return NOERROR;
}

ECode CInputMethodInfo::Equals(
    /* [in] */ IInputMethodInfo* o,
    /* [out] */ Boolean* equal)
{
    VALIDATE_NOT_NULL(equal);
    *equal = FALSE;

    if (o == this) {
        *equal = TRUE;
        return NOERROR;
    }

    if (o == NULL) {
        return NOERROR;
    }

    String id;
    o->GetId(&id);
    *equal = mId.Equals(id);
    return NOERROR;
}

ECode CInputMethodInfo::Equals(
    /* [in] */ IInterface* other,
    /* [out] */ Boolean * result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;
    VALIDATE_NOT_NULL(other);

    return Equals(IInputMethodInfo::Probe(other), result);
}

ECode CInputMethodInfo::Init(
    /* [in] */ IContext* context,
    /* [in] */ IResolveInfo* service,
    /* [in] */ IObjectStringMap* additionalSubtypesMap)
{
    mService = (CResolveInfo*)service;
    CServiceInfo* si = (CServiceInfo*)mService->mServiceInfo.Get();
    AutoPtr<CComponentName> componentName;
    CComponentName::NewByFriend(si->mPackageName, si->mName, (CComponentName**)&componentName);
    componentName->FlattenToShortString(&mId);
    mIsAuxIme = TRUE;

    AutoPtr<IPackageManager> pm;
    context->GetPackageManager((IPackageManager**)&pm);
    String settingsActivityComponent;
    Int32 isDefaultResId = 0;

    AutoPtr<IXmlResourceParser> parser;
    //try {
    si->LoadXmlMetaData(pm, IInputMethod::SERVICE_META_DATA, (IXmlResourceParser**)&parser);
    if (parser == NULL) {
        // throw new XmlPullParserException("No "
        //         + InputMethod.SERVICE_META_DATA + " meta-data");
        return E_XML_PULL_PARSER_EXCEPTION;
    }

    AutoPtr<IResources> res;
    pm->GetResourcesForApplication(si->mApplicationInfo, (IResources**)&res);

    AutoPtr<IAttributeSet> attrs = Xml::AsAttributeSet(parser);

    Int32 type = 0;
    while ((parser->Next(&type), type) != IXmlPullParser::END_DOCUMENT
            && type != IXmlPullParser::START_TAG) {
    }
    String nodeName;
    parser->GetName(&nodeName);
    if (!nodeName.Equals("input-method")) {
        // throw new XmlPullParserException(
        //         "Meta-data does not start with input-method tag");
        return E_XML_PULL_PARSER_EXCEPTION;
    }

    Int32 size = ARRAY_SIZE(R::styleable::InputMethod);
    AutoPtr<ArrayOf<Int32> > layout = ArrayOf<Int32>::Alloc(size);
    layout->Copy(R::styleable::InputMethod, size);

    AutoPtr<ITypedArray> sa;
    res->ObtainAttributes(attrs, layout, (ITypedArray**)&sa);
    sa->GetString(R::styleable::InputMethod_settingsActivity, &settingsActivityComponent);
    sa->GetResourceId(R::styleable::InputMethod_isDefault, 0, &isDefaultResId);
    sa->Recycle();

    Int32 depth = 0, tmpDepth = 0;
    parser->GetDepth(&depth);
    // Parse all subtypes
    while (((parser->Next(&type), type) != IXmlPullParser::END_TAG || (parser->GetDepth(&tmpDepth), tmpDepth) > depth)
            && type != IXmlPullParser::END_DOCUMENT) {
        if (type == IXmlPullParser::START_TAG) {
            parser->GetName(&nodeName);
            if (!nodeName.Equals("subtype")) {
                parser->Close();
                // throw new XmlPullParserException(
                //         "Meta-data in input-method does not start with subtype tag");
                return E_XML_PULL_PARSER_EXCEPTION;
            }
            size = ARRAY_SIZE(R::styleable::InputMethod_Subtype);
            layout = ArrayOf<Int32>::Alloc(size);
            layout->Copy(R::styleable::InputMethod_Subtype, size);

            AutoPtr<ITypedArray> a;
            res->ObtainAttributes(attrs, layout, (ITypedArray**)&a);
            Int32 label = 0;
            a->GetResourceId(R::styleable::InputMethod_Subtype_label, 0, &label);
            Int32 icon = 0;
            a->GetResourceId(R::styleable::InputMethod_Subtype_icon, 0, &icon);
            String imeSubtypeLocale;
            a->GetString(R::styleable::InputMethod_Subtype_imeSubtypeLocale, &imeSubtypeLocale);
            String imeSubtypeMode;
            a->GetString(R::styleable::InputMethod_Subtype_imeSubtypeMode, &imeSubtypeMode);
            String imeSubtypeExtraValue;
            a->GetString(R::styleable::InputMethod_Subtype_imeSubtypeExtraValue, &imeSubtypeExtraValue);
            Boolean isAuxiliary = FALSE;
            a->GetBoolean(R::styleable::InputMethod_Subtype_isAuxiliary, FALSE, &isAuxiliary);
            Boolean overridesImplicitlyEnabledSubtype = FALSE;
            a->GetBoolean(R::styleable::InputMethod_Subtype_overridesImplicitlyEnabledSubtype, FALSE, &overridesImplicitlyEnabledSubtype);
            Int32 subtypeId = 0;
            a->GetInt32(R::styleable::InputMethod_Subtype_subtypeId, 0 /* use Arrays.hashCode */, &subtypeId);
            AutoPtr<IInputMethodSubtype> subtype;
            CInputMethodSubtype::New(label, icon, imeSubtypeLocale, imeSubtypeMode, imeSubtypeExtraValue,
                isAuxiliary, overridesImplicitlyEnabledSubtype, subtypeId,
                (IInputMethodSubtype**)&subtype);
            Boolean auxiliary = FALSE;
            if (!(subtype->IsAuxiliary(&auxiliary), auxiliary)) {
                mIsAuxIme = FALSE;
            }
            mSubtypes.PushBack(subtype);
        }
    }
    // } catch (NameNotFoundException e) {
    //     throw new XmlPullParserException(
    //             "Unable to create context for: " + si.packageName);
    // } finally {
    //     if (parser != null) parser.close();
    // }
    if (parser != NULL) {
        parser->Close();
    }

    if (mSubtypes.IsEmpty()) {
        mIsAuxIme = FALSE;
    }
    Boolean contains;
    if (additionalSubtypesMap != NULL && (additionalSubtypesMap->ContainsKey(mId, &contains), contains)) {
        AutoPtr<IObjectContainer> additionalSubtypes;
        additionalSubtypesMap->Get(mId, (IInterface**)&additionalSubtypes);
        AutoPtr<IObjectEnumerator> subtypeEnum;
        additionalSubtypes->GetObjectEnumerator((IObjectEnumerator**)&subtypeEnum);
        Boolean hasNext = FALSE;
        while(subtypeEnum->MoveNext(&hasNext), hasNext) {
            AutoPtr<IInputMethodSubtype> subtype;
            subtypeEnum->Current((IInterface**)&subtype);
            if (Find(mSubtypes.Begin(), mSubtypes.End(), subtype) == mSubtypes.End()) {
                mSubtypes.PushBack(subtype);
            }
            else {
                // Slog.w(TAG, "Duplicated subtype definition found: "
                //         + subtype.getLocale() + ", " + subtype.getMode());
            }
        }
    }
    mSettingsActivityName = settingsActivityComponent;
    mIsDefaultResId = isDefaultResId;
    return NOERROR;
}

} // namespace InputMethod
} // namespace View
} // namespace Droid
} // namespace Elastos
