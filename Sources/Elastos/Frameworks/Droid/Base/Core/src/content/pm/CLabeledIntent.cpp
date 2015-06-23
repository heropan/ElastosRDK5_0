
#include "content/pm/CLabeledIntent.h"
#include "ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Content {
namespace Pm {

CLabeledIntent::CLabeledIntent()
    : mLabelRes(0)
    , mIcon(0)
{}

CLabeledIntent::~CLabeledIntent()
{}

PInterface CLabeledIntent::Probe(
    /* [in] */ REIID riid)
{
    return _CLabeledIntent::Probe(riid);
}

ECode CLabeledIntent::constructor()
{
    return NOERROR;
}

ECode CLabeledIntent::constructor(
    /* [in] */ IIntent* origIntent,
    /* [in] */ const String& sourcePackage,
    /* [in] */ Int32 labelRes,
    /* [in] */ Int32 icon)
{
    Intent::Init(origIntent);
    mSourcePackage = sourcePackage;
    mLabelRes = labelRes;
    mIcon = icon;

    return NOERROR;
}

ECode CLabeledIntent::constructor(
    /* [in] */ IIntent* origIntent,
    /* [in] */ const String& sourcePackage,
    /* [in] */ ICharSequence* nonLocalizedLabel,
    /* [in] */ Int32 icon)
{
    Intent::Init(origIntent);
    mSourcePackage = sourcePackage;
    mLabelRes = 0;
    mNonLocalizedLabel = nonLocalizedLabel;
    mIcon = icon;

    return NOERROR;
}

ECode CLabeledIntent::constructor(
    /* [in] */ const String& sourcePackage,
    /* [in] */ Int32 labelRes,
    /* [in] */ Int32 icon)
{
    mSourcePackage = sourcePackage;
    mLabelRes = labelRes;
    mIcon = icon;

    return NOERROR;
}

ECode CLabeledIntent::constructor(
    /* [in] */ const String& sourcePackage,
    /* [in] */ ICharSequence* nonLocalizedLabel,
    /* [in] */ Int32 icon)
{
    mSourcePackage = sourcePackage;
    mLabelRes = 0;
    mNonLocalizedLabel = nonLocalizedLabel;
    mIcon = icon;

    return NOERROR;
}

ECode CLabeledIntent::GetSourcePackage(
    /* [out] */ String* sourcePackage)
{
    VALIDATE_NOT_NULL(sourcePackage);
    *sourcePackage = mSourcePackage;
    return NOERROR;
}

ECode CLabeledIntent::GetLabelResource(
    /* [out] */ Int32* labelRes)
{
    VALIDATE_NOT_NULL(labelRes);
    *labelRes = mLabelRes;
    return NOERROR;
}

ECode CLabeledIntent::GetNonLocalizedLabel(
    /* [out] */ ICharSequence** label)
{
    VALIDATE_NOT_NULL(label);
    *label = mNonLocalizedLabel;
    REFCOUNT_ADD(*label);
    return NOERROR;
}

ECode CLabeledIntent::GetIconResource(
    /* [out] */ Int32* iconRes)
{
    VALIDATE_NOT_NULL(iconRes);
    *iconRes = mIcon;
    return NOERROR;
}

ECode CLabeledIntent::LoadLabel(
    /* [in] */ IPackageManager* pm,
    /* [out] */ ICharSequence** label)
{
    VALIDATE_NOT_NULL(label);
    *label = NULL;

    if (mNonLocalizedLabel != NULL) {
        *label = mNonLocalizedLabel;
        REFCOUNT_ADD(*label);
        return NOERROR;
    }

    if (mLabelRes != 0 && !mSourcePackage.IsNull()) {
        FAIL_RETURN(pm->GetText(mSourcePackage, mLabelRes, NULL, label));
    }

    return NOERROR;
}

ECode CLabeledIntent::LoadIcon(
    /* [in] */ IPackageManager* pm,
    /* [out] */ IDrawable** icon)
{
    VALIDATE_NOT_NULL(icon);
    *icon = NULL;

    if (mIcon != 0 && !mSourcePackage.IsNull()) {
        FAIL_RETURN(pm->GetDrawable(mSourcePackage, mIcon, NULL, icon));
    }

    return NOERROR;
}

ECode CLabeledIntent::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    assert(source != NULL);

    Intent::ReadFromParcel(source);
    source->ReadString(&mSourcePackage);
    source->ReadInt32(&mLabelRes);
    AutoPtr<IInterface> obj;
    source->ReadInterfacePtr((Handle32*)&obj);
    mNonLocalizedLabel = obj != NULL ? ICharSequence::Probe(obj) : NULL;
    source->ReadInt32(&mIcon);

    return NOERROR;
}

ECode CLabeledIntent::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    assert(dest != NULL);

    Intent::WriteToParcel(dest);
    dest->WriteString(mSourcePackage);
    dest->WriteInt32(mLabelRes);
    dest->WriteInterfacePtr(mNonLocalizedLabel);
    dest->WriteInt32(mIcon);

    return NOERROR;
}

ECode CLabeledIntent::Clone(
    /* [out] */ IIntent** intent)
{
    return Intent::Clone(intent);
}

ECode CLabeledIntent::CloneFilter(
    /* [out] */ IIntent** intent)
{
    VALIDATE_NOT_NULL(intent);
    return Intent::CloneFilter(intent);
}

ECode CLabeledIntent::GetAction(
    /* [out] */ String* action)
{
    VALIDATE_NOT_NULL(action);
    return Intent::GetAction(action);
}

ECode CLabeledIntent::GetData(
    /* [out] */ IUri** data)
{
    VALIDATE_NOT_NULL(data);
    return Intent::GetData(data);
}

ECode CLabeledIntent::GetDataString(
    /* [out] */ String* dataString)
{
    VALIDATE_NOT_NULL(dataString);
    return Intent::GetDataString(dataString);
}

ECode CLabeledIntent::GetScheme(
    /* [out] */ String* scheme)
{
    VALIDATE_NOT_NULL(scheme);
    return Intent::GetScheme(scheme);
}

ECode CLabeledIntent::GetType(
    /* [out] */ String* type)
{
    VALIDATE_NOT_NULL(type);
    return Intent::GetType(type);
}

ECode CLabeledIntent::ResolveType(
    /* [in] */ IContext* context,
    /* [out] */ String* type)
{
    VALIDATE_NOT_NULL(type);
    return Intent::ResolveType(context, type);
}

ECode CLabeledIntent::ResolveType(
    /* [in] */ IContentResolver* resolver,
    /* [out] */ String* type)
{
    VALIDATE_NOT_NULL(type);
    return Intent::ResolveType(resolver, type);
}

ECode CLabeledIntent::ResolveTypeIfNeeded(
    /* [in] */ IContentResolver* resolver,
    /* [out] */ String* type)
{
    VALIDATE_NOT_NULL(type);
    return Intent::ResolveTypeIfNeeded(resolver, type);
}

ECode CLabeledIntent::HasCategory(
    /* [in] */ const String& category,
    /* [out] */ Boolean* hasCategory)
{
    VALIDATE_NOT_NULL(hasCategory);
    return Intent::HasCategory(category, hasCategory);
}

ECode CLabeledIntent::GetCategories(
    /* [out, callee] */ ArrayOf<String>** categories)
{
    VALIDATE_NOT_NULL(categories);
    return Intent::GetCategories(categories);
}

ECode CLabeledIntent::GetSelector(
    /* [out] */ IIntent** intent)
{
    VALIDATE_NOT_NULL(intent);
    return Intent::GetSelector(intent);
}

ECode CLabeledIntent::GetClipData(
    /* [out] */ IClipData** clipData)
{
    VALIDATE_NOT_NULL(clipData);
    return Intent::GetClipData(clipData);
}

ECode CLabeledIntent::HasExtra(
    /* [in] */ const String& name,
    /* [out] */ Boolean* hasExtra)
{
    VALIDATE_NOT_NULL(hasExtra);
    return Intent::HasExtra(name, hasExtra);
}

ECode CLabeledIntent::HasFileDescriptors(
    /* [out] */ Boolean* hasFileDescriptors)
{
    VALIDATE_NOT_NULL(hasFileDescriptors);
    return Intent::HasFileDescriptors(hasFileDescriptors);
}

ECode CLabeledIntent::SetAllowFds(
    /* [in] */ Boolean allowFds)
{
    return Intent::SetAllowFds(allowFds);
}

ECode CLabeledIntent::GetExtra(
    /* [in] */ const String& name,
    /* [out] */ IInterface** obj)
{
    VALIDATE_NOT_NULL(obj);
    return Intent::GetExtra(name, obj);
}

ECode CLabeledIntent::GetBooleanExtra(
    /* [in] */ const String& name,
    /* [in] */ Boolean defaultValue,
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value);
    return Intent::GetBooleanExtra(name, defaultValue, value);
}

ECode CLabeledIntent::GetByteExtra(
    /* [in] */ const String& name,
    /* [in] */ Byte defaultValue,
    /* [out] */ Byte* value)
{
    VALIDATE_NOT_NULL(value);
    return Intent::GetByteExtra(name, defaultValue, value);
}

ECode CLabeledIntent::GetInt16Extra(
    /* [in] */ const String& name,
    /* [in] */ Int16 defaultValue,
    /* [out] */ Int16* value)
{
    VALIDATE_NOT_NULL(value);
    return Intent::GetInt16Extra(name, defaultValue, value);
}

ECode CLabeledIntent::GetCharExtra(
    /* [in] */ const String& name,
    /* [in] */ Char32 defaultValue,
    /* [out] */ Char32* value)
{
    VALIDATE_NOT_NULL(value);
    return Intent::GetCharExtra(name, defaultValue, value);
}

ECode CLabeledIntent::GetInt32Extra(
    /* [in] */ const String& name,
    /* [in] */ Int32 defaultValue,
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    return Intent::GetInt32Extra(name, defaultValue, value);
}

ECode CLabeledIntent::GetInt64Extra(
    /* [in] */ const String& name,
    /* [in] */ Int64 defaultValue,
    /* [out] */ Int64* value)
{
    VALIDATE_NOT_NULL(value);
    return Intent::GetInt64Extra(name, defaultValue, value);
}

ECode CLabeledIntent::GetFloatExtra(
    /* [in] */ const String& name,
    /* [in] */ Float defaultValue,
    /* [out] */ Float* value)
{
    VALIDATE_NOT_NULL(value);
    return Intent::GetFloatExtra(name, defaultValue, value);
}

ECode CLabeledIntent::GetDoubleExtra(
    /* [in] */ const String& name,
    /* [in] */ Double defaultValue,
    /* [out] */ Double* value)
{
    VALIDATE_NOT_NULL(value);
    return Intent::GetDoubleExtra(name, defaultValue, value);
}

ECode CLabeledIntent::GetStringExtra(
    /* [in] */ const String& name,
    /* [out] */ String* value)
{
    VALIDATE_NOT_NULL(value);
    return Intent::GetStringExtra(name, value);
}

ECode CLabeledIntent::GetCharSequenceExtra(
    /* [in] */ const String& name,
    /* [out] */ ICharSequence** value)
{
    VALIDATE_NOT_NULL(value);
    return Intent::GetCharSequenceExtra(name, value);
}

ECode CLabeledIntent::GetParcelableExtra(
    /* [in] */ const String& name,
    /* [out] */ IParcelable** value)
{
    VALIDATE_NOT_NULL(value);
    return Intent::GetParcelableExtra(name, value);
}

ECode CLabeledIntent::GetObjectStringMapExtra(
    /* [in] */ const String& name,
    /* [out] */ IObjectStringMap** value)
{
    VALIDATE_NOT_NULL(value);
    return Intent::GetObjectStringMapExtra(name, value);
}

ECode CLabeledIntent::GetParcelableArrayExtra(
    /* [in] */ const String& name,
    /* [out, callee] */ ArrayOf<IParcelable *>** value)
{
    VALIDATE_NOT_NULL(value);
    return Intent::GetParcelableArrayExtra(name, value);
}

ECode CLabeledIntent::GetParcelableArrayListExtra(
    /* [in] */ const String& name,
    /* [out] */ IObjectContainer** value)
{
    VALIDATE_NOT_NULL(value);
    return Intent::GetParcelableArrayListExtra(name, value);
}

ECode CLabeledIntent::GetIntegerArrayListExtra(
    /* [in] */ const String& name,
    /* [out] */ IObjectContainer** value)
{
    VALIDATE_NOT_NULL(value);
    return Intent::GetIntegerArrayListExtra(name, value);
}

ECode CLabeledIntent::GetStringArrayListExtra(
    /* [in] */ const String& name,
    /* [out] */ IObjectContainer** value)
{
    VALIDATE_NOT_NULL(value);
    return Intent::GetStringArrayListExtra(name, value);
}

ECode CLabeledIntent::GetCharSequenceArrayListExtra(
    /* [in] */ const String& name,
    /* [out] */ IObjectContainer** value)
{
    VALIDATE_NOT_NULL(value);
    return Intent::GetCharSequenceArrayListExtra(name, value);
}

ECode CLabeledIntent::GetBooleanArrayExtra(
    /* [in] */ const String& name,
    /* [out, callee] */ ArrayOf<Boolean>** value)
{
    VALIDATE_NOT_NULL(value);
    return Intent::GetBooleanArrayExtra(name, value);
}

ECode CLabeledIntent::GetByteArrayExtra(
    /* [in] */ const String& name,
    /* [out, callee] */ ArrayOf<Byte>** value)
{
    VALIDATE_NOT_NULL(value);
    return Intent::GetByteArrayExtra(name, value);
}

ECode CLabeledIntent::GetInt16ArrayExtra(
    /* [in] */ const String& name,
    /* [out, callee] */ ArrayOf<Int16>** value)
{
    VALIDATE_NOT_NULL(value);
    return Intent::GetInt16ArrayExtra(name, value);
}

ECode CLabeledIntent::GetCharArrayExtra(
    /* [in] */ const String& name,
    /* [out, callee] */ ArrayOf<Char32>** value)
{
    VALIDATE_NOT_NULL(value);
    return Intent::GetCharArrayExtra(name, value);
}

ECode CLabeledIntent::GetInt32ArrayExtra(
    /* [in] */ const String& name,
    /* [out, callee] */ ArrayOf<Int32>** value)
{
    VALIDATE_NOT_NULL(value);
    return Intent::GetInt32ArrayExtra(name, value);
}

ECode CLabeledIntent::GetInt64ArrayExtra(
    /* [in] */ const String& name,
    /* [out, callee] */ ArrayOf<Int64>** value)
{
    VALIDATE_NOT_NULL(value);
    return Intent::GetInt64ArrayExtra(name, value);
}

ECode CLabeledIntent::GetFloatArrayExtra(
    /* [in] */ const String& name,
    /* [out, callee] */ ArrayOf<Float>** value)
{
    VALIDATE_NOT_NULL(value);
    return Intent::GetFloatArrayExtra(name, value);
}

ECode CLabeledIntent::GetDoubleArrayExtra(
    /* [in] */ const String& name,
    /* [out, callee] */ ArrayOf<Double>** value)
{
    VALIDATE_NOT_NULL(value);
    return Intent::GetDoubleArrayExtra(name, value);
}

ECode CLabeledIntent::GetStringArrayExtra(
    /* [in] */ const String& name,
    /* [out, callee] */ ArrayOf<String>** value)
{
    VALIDATE_NOT_NULL(value);
    return Intent::GetStringArrayExtra(name, value);
}

ECode CLabeledIntent::GetCharSequenceArrayExtra(
    /* [in] */ const String& name,
    /* [out, callee] */ ArrayOf<ICharSequence *>** value)
{
    VALIDATE_NOT_NULL(value);
    return Intent::GetCharSequenceArrayExtra(name, value);
}

ECode CLabeledIntent::GetBundleExtra(
    /* [in] */ const String& name,
    /* [out] */ IBundle** value)
{
    VALIDATE_NOT_NULL(value);
    return Intent::GetBundleExtra(name, value);
}

ECode CLabeledIntent::GetExtra(
    /* [in] */ const String& name,
    /* [in] */ IInterface* defaultValue,
    /* [out] */ IInterface** obj)
{
    VALIDATE_NOT_NULL(obj);
    return Intent::GetExtra(name, defaultValue, obj);
}

ECode CLabeledIntent::GetExtras(
    /* [out] */ IBundle** extras)
{
    VALIDATE_NOT_NULL(extras);
    return Intent::GetExtras(extras);
}

ECode CLabeledIntent::GetFlags(
    /* [out] */ Int32* flags)
{
    VALIDATE_NOT_NULL(flags);
    return Intent::GetFlags(flags);
}

ECode CLabeledIntent::IsExcludingStopped(
    /* [out] */ Boolean* isStopped)
{
    VALIDATE_NOT_NULL(isStopped);
    return Intent::IsExcludingStopped(isStopped);
}

ECode CLabeledIntent::GetPackage(
    /* [out] */ String* packageName)
{
    VALIDATE_NOT_NULL(packageName);
    return Intent::GetPackage(packageName);
}

ECode CLabeledIntent::GetComponent(
    /* [out] */ IComponentName** componentName)
{
    VALIDATE_NOT_NULL(componentName);
    return Intent::GetComponent(componentName);
}

ECode CLabeledIntent::GetSourceBounds(
    /* [out] */ IRect** sourceBounds)
{
    VALIDATE_NOT_NULL(sourceBounds);
    return Intent::GetSourceBounds(sourceBounds);
}

ECode CLabeledIntent::ResolveActivity(
    /* [in] */ IPackageManager* pm,
    /* [out] */ IComponentName** componentName)
{
    VALIDATE_NOT_NULL(componentName);
    return Intent::ResolveActivity(pm, componentName);
}

ECode CLabeledIntent::ResolveActivityInfo(
    /* [in] */ IPackageManager* pm,
    /* [in] */ Int32 flags,
    /* [out] */ IActivityInfo** result)
{
    VALIDATE_NOT_NULL(result);
    return Intent::ResolveActivityInfo(pm, flags, result);
}

ECode CLabeledIntent::SetAction(
    /* [in] */ const String& action)
{
    return Intent::SetAction(action);
}

ECode CLabeledIntent::SetData(
    /* [in] */ IUri* data)
{
    return Intent::SetData(data);
}

ECode CLabeledIntent::SetDataAndNormalize(
    /* [in] */ IUri* data)
{
    return Intent::SetDataAndNormalize(data);
}

ECode CLabeledIntent::SetType(
    /* [in] */ const String& type)
{
    return Intent::SetType(type);
}

ECode CLabeledIntent::SetTypeAndNormalize(
    /* [in] */ const String& type)
{
    return Intent::SetTypeAndNormalize(type);
}

ECode CLabeledIntent::SetDataAndType(
    /* [in] */ IUri* data,
    /* [in] */ const String& type)
{
    return Intent::SetDataAndType(data, type);
}

ECode CLabeledIntent::SetDataAndTypeAndNormalize(
    /* [in] */ IUri* data,
    /* [in] */ const String& type)
{
    return Intent::SetDataAndTypeAndNormalize(data, type);
}

ECode CLabeledIntent::AddCategory(
    /* [in] */ const String& category)
{
    return Intent::AddCategory(category);
}

ECode CLabeledIntent::RemoveCategory(
    /* [in] */ const String& category)
{
    return Intent::RemoveCategory(category);
}

ECode CLabeledIntent::SetSelector(
    /* [in] */ IIntent* selector)
{
    return Intent::SetSelector(selector);
}

ECode CLabeledIntent::SetClipData(
    /* [in] */ IClipData* clip)
{
    return Intent::SetClipData(clip);
}

ECode CLabeledIntent::PutBooleanExtra(
    /* [in] */ const String& name,
    /* [in] */ Boolean value)
{
    return Intent::PutBooleanExtra(name, value);
}

ECode CLabeledIntent::PutByteExtra(
    /* [in] */ const String& name,
    /* [in] */ Byte value)
{
    return Intent::PutByteExtra(name, value);
}

ECode CLabeledIntent::PutCharExtra(
    /* [in] */ const String& name,
    /* [in] */ Char32 value)
{
    return Intent::PutCharExtra(name, value);
}

ECode CLabeledIntent::PutInt16Extra(
    /* [in] */ const String& name,
    /* [in] */ Int16 value)
{
    return Intent::PutInt16Extra(name, value);
}

ECode CLabeledIntent::PutInt32Extra(
    /* [in] */ const String& name,
    /* [in] */ Int32 value)
{
    return Intent::PutInt32Extra(name, value);
}

ECode CLabeledIntent::PutInt64Extra(
    /* [in] */ const String& name,
    /* [in] */ Int64 value)
{
    return Intent::PutInt64Extra(name, value);
}

ECode CLabeledIntent::PutFloatExtra(
    /* [in] */ const String& name,
    /* [in] */ Float value)
{
    return Intent::PutFloatExtra(name, value);
}

ECode CLabeledIntent::PutDoubleExtra(
    /* [in] */ const String& name,
    /* [in] */ Double value)
{
    return Intent::PutDoubleExtra(name, value);
}

ECode CLabeledIntent::PutStringExtra(
    /* [in] */ const String& name,
    /* [in] */ const String& value)
{
    return Intent::PutStringExtra(name, value);
}

ECode CLabeledIntent::PutCharSequenceExtra(
    /* [in] */ const String& name,
    /* [in] */ ICharSequence* value)
{
    return Intent::PutCharSequenceExtra(name, value);
}

ECode CLabeledIntent::PutParcelableExtra(
    /* [in] */ const String& name,
    /* [in] */ IParcelable* value)
{
    return Intent::PutParcelableExtra(name, value);
}

ECode CLabeledIntent::PutObjectStringMapExtra(
    /* [in] */ const String& name,
    /* [in] */ IObjectStringMap* value)
{
    return Intent::PutObjectStringMapExtra(name, value);
}

ECode CLabeledIntent::PutParcelableArrayExtra(
    /* [in] */ const String& name,
    /* [in] */ ArrayOf<IParcelable *>* value)
{
    return Intent::PutParcelableArrayExtra(name, value);
}

ECode CLabeledIntent::PutParcelableArrayListExtra(
    /* [in] */ const String& name,
    /* [in] */ IObjectContainer* value)
{
    return Intent::PutParcelableArrayListExtra(name, value);
}

ECode CLabeledIntent::PutIntegerArrayListExtra(
    /* [in] */ const String& name,
    /* [in] */ IObjectContainer* value)
{
    return Intent::PutIntegerArrayListExtra(name, value);
}

ECode CLabeledIntent::PutStringArrayListExtra(
    /* [in] */ const String& name,
    /* [in] */ IObjectContainer* value)
{
    return Intent::PutStringArrayListExtra(name, value);
}

ECode CLabeledIntent::PutCharSequenceArrayListExtra(
    /* [in] */ const String& name,
    /* [in] */ IObjectContainer* value)
{
    return Intent::PutCharSequenceArrayListExtra(name, value);
}

ECode CLabeledIntent::PutBooleanArrayExtra(
    /* [in] */ const String& name,
    /* [in] */ ArrayOf<Boolean>* value)
{
    return Intent::PutBooleanArrayExtra(name, value);
}

ECode CLabeledIntent::PutByteArrayExtra(
    /* [in] */ const String& name,
    /* [in] */ ArrayOf<Byte>* value)
{
    return Intent::PutByteArrayExtra(name, value);
}

ECode CLabeledIntent::PutInt16ArrayExtra(
    /* [in] */ const String& name,
    /* [in] */ ArrayOf<Int16>* value)
{
    return Intent::PutInt16ArrayExtra(name, value);
}

ECode CLabeledIntent::PutCharArrayExtra(
    /* [in] */ const String& name,
    /* [in] */ ArrayOf<Char32>* value)
{
    return Intent::PutCharArrayExtra(name, value);
}

ECode CLabeledIntent::PutInt32ArrayExtra(
    /* [in] */ const String& name,
    /* [in] */ ArrayOf<Int32>* value)
{
    return Intent::PutInt32ArrayExtra(name, value);
}

ECode CLabeledIntent::PutInt64ArrayExtra(
    /* [in] */ const String& name,
    /* [in] */ ArrayOf<Int64>* value)
{
    return Intent::PutInt64ArrayExtra(name, value);
}

ECode CLabeledIntent::PutFloatArrayExtra(
    /* [in] */ const String& name,
    /* [in] */ ArrayOf<Float>* value)
{
    return Intent::PutFloatArrayExtra(name, value);
}

ECode CLabeledIntent::PutDoubleArrayExtra(
    /* [in] */ const String& name,
    /* [in] */ ArrayOf<Double>* value)
{
    return Intent::PutDoubleArrayExtra(name, value);
}

ECode CLabeledIntent::PutStringArrayExtra(
    /* [in] */ const String& name,
    /* [in] */ ArrayOf<String>* value)
{
    return Intent::PutStringArrayExtra(name, value);
}

ECode CLabeledIntent::PutCharSequenceArrayExtra(
    /* [in] */ const String& name,
    /* [in] */ ArrayOf<ICharSequence *>* value)
{
    return Intent::PutCharSequenceArrayExtra(name, value);
}

ECode CLabeledIntent::PutBundleExtra(
    /* [in] */ const String& name,
    /* [in] */ IBundle* value)
{
    return Intent::PutBundleExtra(name, value);
}

ECode CLabeledIntent::PutExtras(
    /* [in] */ IIntent* src)
{
    return Intent::PutExtras(src);
}

ECode CLabeledIntent::PutExtras(
    /* [in] */ IBundle* extras)
{
    return Intent::PutExtras(extras);
}

ECode CLabeledIntent::ReplaceExtras(
    /* [in] */ IIntent* src)
{
    return Intent::ReplaceExtras(src);
}

ECode CLabeledIntent::ReplaceExtras(
    /* [in] */ IBundle* extras)
{
    return Intent::ReplaceExtras(extras);
}

ECode CLabeledIntent::RemoveExtra(
    /* [in] */ const String& name)
{
    return Intent::RemoveExtra(name);
}

ECode CLabeledIntent::SetFlags(
    /* [in] */ Int32 flags)
{
    return Intent::SetFlags(flags);
}

ECode CLabeledIntent::AddFlags(
    /* [in] */ Int32 flags)
{
    return Intent::AddFlags(flags);
}

ECode CLabeledIntent::SetPackage(
    /* [in] */ const String& packageName)
{
    return Intent::SetPackage(packageName);
}

ECode CLabeledIntent::SetComponent(
    /* [in] */ IComponentName* component)
{
    return Intent::SetComponent(component);
}

ECode CLabeledIntent::SetClassName(
    /* [in] */ IContext* packageContext,
    /* [in] */ const String& className)
{
    return Intent::SetClassName(packageContext, className);
}

ECode CLabeledIntent::SetClassName(
    /* [in] */ const String& packageName,
    /* [in] */ const String& className)
{
    return Intent::SetClassName(packageName, className);
}

ECode CLabeledIntent::SetClass(
    /* [in] */ IContext* packageContext,
    /* [in] */ const ClassID& clsId)
{
    return Intent::SetClass(packageContext, clsId);
}

ECode CLabeledIntent::SetSourceBounds(
    /* [in] */ IRect* r)
{
    return Intent::SetSourceBounds(r);
}

ECode CLabeledIntent::FillIn(
    /* [in] */ IIntent* other,
    /* [in] */ Int32 flags,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    return Intent::FillIn(other, flags, result);
}

ECode CLabeledIntent::FilterEquals(
    /* [in] */ IIntent* other,
    /* [out] */ Boolean* isEqual)
{
    VALIDATE_NOT_NULL(isEqual);
    return Intent::FilterEquals(other, isEqual);
}

ECode CLabeledIntent::FilterHashCode(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    return Intent::FilterHashCode(result);
}

ECode CLabeledIntent::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);
    return Intent::ToString(str);
}

ECode CLabeledIntent::ToInsecureString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);
    return Intent::ToInsecureString(str);
}

ECode CLabeledIntent::ToInsecureStringWithClip(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);
    return Intent::ToInsecureStringWithClip(str);
}

ECode CLabeledIntent::ToShortString(
    /* [in] */ Boolean secure,
    /* [in] */ Boolean comp,
    /* [in] */ Boolean extras,
    /* [in] */ Boolean clip,
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);
    return Intent::ToShortString(secure, comp, extras, clip, str);
}

ECode CLabeledIntent::ToShortString(
    /* [in] */ IStringBuilder* sb,
    /* [in] */ Boolean secure,
    /* [in] */ Boolean comp,
    /* [in] */ Boolean extras,
    /* [in] */ Boolean clip)
{
    return Intent::ToShortString(sb, secure, comp, extras, clip);
}

ECode CLabeledIntent::ToURI(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);
    return Intent::ToURI(str);
}

ECode CLabeledIntent::ToUri(
    /* [in] */ Int32 flags,
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);
    return Intent::ToUri(flags, str);
}

ECode CLabeledIntent::MigrateExtraStreamToClipData(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return Intent::MigrateExtraStreamToClipData(result);
}

}
}
}
}

