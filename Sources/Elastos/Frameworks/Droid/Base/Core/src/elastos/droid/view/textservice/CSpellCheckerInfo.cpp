
#include "elastos/droid/view/textservice/CSpellCheckerInfo.h"

namespace Elastos {
namespace Droid {
namespace View {
namespace Textservice {

//========================================================================================
//              CSpellCheckerInfo::
//========================================================================================
CAR_INTERFACE_IMPL_2(CSpellCheckerInfo, Object, ISpellCheckerInfo, IParcelable)

CAR_OBJECT_IMPL(CSpellCheckerInfo)

CSpellCheckerInfo::CSpellCheckerInfo()
    : mId("")
    , mLabel(0)
    , mSettingsActivityName("")
{
    CArrayList::New((IArrayList**)&mSubtypes);
}

ECode CSpellCheckerInfo::constructor()
{
    return NOERROR;
}

ECode CSpellCheckerInfo::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IResolveInfo* service)
{
    mService = service;

    CResolveInfo* info = (CResolveInfo*)(service.Get());
    AutoPtr<IServiceInfo> si = info->mServiceInfo;
    AutoPtr<CServiceInfo> _si = (CServiceInfo*)si.Get();
    AutoPtr<IComponentName> name;
    CComponentName::New(_si->mPackageName, _si->mName, (IComponentName**)&name);
    name->FlattenToShortString(&mId);

    AutoPtr<IPackageManager> pm;
    context->GetPackageManager((IPackageManager**)&pm);
    Int32 label = 0;
    String settingsActivityComponent(NULL);

    AutoPtr<IXmlResourceParser> parser;
    //try {
        si->LoadXmlMetaData(pm, ISpellCheckerSession::SERVICE_META_DATA, (IXmlResourceParser**)&parser);
        //if (parser == NULL) {
        //    throw new XmlPullParserException("No "
        //            + SpellCheckerSession.SERVICE_META_DATA + " meta-data");
        //}
        assert(parser != NULL);

        AutoPtr<IResources> res;
        pm->GetResourcesForApplication(_si->mApplicationInfo, (IResources**)&res);
        AutoPtr<IAttributeSet> attrs = Xml::AsAttributeSet(parser);
        Int32 type = 0;
        while ((IXmlPullParser::Probe(parser)->Next(&type), type) != IXmlPullParser::END_DOCUMENT
                && type != IXmlPullParser::START_TAG) {
        }

        String nodeName;
        parser->GetName(&nodeName);
        //if (!nodeName.Equals("spell-checker")) {
        //    throw new XmlPullParserException(
        //            "Meta-data does not start with spell-checker tag");
        //}
        assert(nodeName.Equals("spell-checker"));

/*        AutoPtr<ITypedArray> sa;
        res->ObtainAttributes(attrs,
                com.android.internal.R.styleable.SpellChecker, (ITypedArray**)&sa);
        sa->GetResourceId(com.android.internal.R.styleable.SpellChecker_label, 0, &label);
        sa->GetString(
                com.android.internal.R.styleable.SpellChecker_settingsActivity, &settingsActivityComponent);
        sa->Recycle();

        Int32 depth;
        parser->GetDepth(&depth);
        // Parse all subtypes
        while (((type = parser.next()) != XmlPullParser.END_TAG || parser.getDepth() > depth)
                && type != XmlPullParser.END_DOCUMENT) {
            if (type == XmlPullParser.START_TAG) {
                final String subtypeNodeName = parser.getName();
                if (!"subtype".equals(subtypeNodeName)) {
                    throw new XmlPullParserException(
                            "Meta-data in spell-checker does not start with subtype tag");
                }
                final TypedArray a = res.obtainAttributes(
                        attrs, com.android.internal.R.styleable.SpellChecker_Subtype);
                SpellCheckerSubtype subtype = new SpellCheckerSubtype(
                        a.getResourceId(com.android.internal.R.styleable
                                .SpellChecker_Subtype_label, 0),
                        a.getString(com.android.internal.R.styleable
                                .SpellChecker_Subtype_subtypeLocale),
                        a.getString(com.android.internal.R.styleable
                                .SpellChecker_Subtype_subtypeExtraValue));
                mSubtypes.add(subtype);
            }
        }*/
    //} catch (Exception e) {
    //    Slog.e(TAG, "Caught exception: " + e);
    //    throw new XmlPullParserException(
    //            "Unable to create context for: " + si.packageName);
    //} finally {
        if (parser != NULL) parser->Close();
    //}
    mLabel = label;
    mSettingsActivityName = settingsActivityComponent;

    return NOERROR;
}

ECode CSpellCheckerInfo::ReadFromParcel(
    /* [in] */ IParcel* parcel)
{
    source->ReadInt32(&mLabel);
    source->ReadString(&mId);
    source->ReadString(&mSettingsActivityName);
//    mService = ResolveInfo.CREATOR.createFromParcel(source);
//    source.readTypedList(mSubtypes, SpellCheckerSubtype.CREATOR);

    return NOERROR;
}

ECode CSpellCheckerInfo::GetId(
    /* [out] */ String* id)
{
    VALIDATE_NOT_NULL(id);
    *id = mId;
    return NOERROR;
}

ECode CSpellCheckerInfo::GetComponent(
    /* [out] */ IComponentName** component)
{
    VALIDATE_NOT_NULL(component);
    AutoPtr<IServiceInfo> serviceInfo;
    String packageName, name;
    serviceInfo = ((CResolveInfo*)(mService.Get()))->serviceInfo;
    packageName = ((CServiceInfo*)(serviceInfo.Get()))->packageName;
    name = ((CServiceInfo*)(serviceInfo.Get()))->name;
    CComponentName::New(packageName, name, component);

    return NOERROR;
}

ECode CSpellCheckerInfo::GetPackageName(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);
    *name = ((CServiceInfo*)(serviceInfo.Get()))->packageName;
    return NOERROR;
}

ECode CSpellCheckerInfo::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    dest->WriteInt32(mLabel);
    dest->WriteString(mId);
    dest->WriteString(mSettingsActivityName);
    mService->WriteToParcel(dest, flags);
//    dest.writeTypedList(mSubtypes);

    return NOERROR;
}

ECode CSpellCheckerInfo::LoadLabel(
    /* [in] */ IPackageManager* pm,
    /* [out] */ ICharSequence** lable)
{
    VALIDATE_NOT_NULL(lable);
    if (mLabel == 0 || pm == NULL)
        return CStringWrapper::New(String(""), lable);

    String packageName, applicationInfo;
    AutoPtr<IServiceInfo> serviceInfo;

    applicationInfo = ((CServiceInfo*)(serviceInfo.Get()))->applicationInfo;
    GetPackageName(&packageName);

    return pm->GetText(packageName, mLabel, applicationInfo);
}

ECode CSpellCheckerInfo::LoadIcon(
    /* [in] */ IPackageManager* pm,
    /* [out] */ IDrawable** drawable)
{
    return mService->LoadIcon(pm, drawable);
}

ECode CSpellCheckerInfo::GetServiceInfo(
    /* [out] */ IServiceInfo** info)
{
    VALIDATE_NOT_NULL(info);
    *info = ((CResolveInfo*)(mService.Get()))->mServiceInfo;
    REFCOUNT_ADD(*info);
    return NOERROR;
}

ECode CSpellCheckerInfo::GetSettingsActivity(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);
    *name = mSettingsActivityName;
    return NOERROR;
}

ECode CSpellCheckerInfo::GetSubtypeCount(
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count);
    return mSubtypes->GetSize(count);
}

ECode CSpellCheckerInfo::GetSubtypeAt(
    /* [in] */ Int32 index,
    /* [out] */ ISpellCheckerSubtype** subtype)
{
    VALIDATE_NOT_NULL(subtype);
    *subtype = (*mSubtypes)[index];
    REFCOUNT_ADD(*subtype);
    return NOERROR;
}

} // namespace Textservice
} // namespace View
} // namespace Droid
} // namespace Elastos
