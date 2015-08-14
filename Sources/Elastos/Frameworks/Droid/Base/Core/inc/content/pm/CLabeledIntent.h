
#ifndef __ELASTOS_DROID_CONTENT_PM_CLABELEDINTENT_H__
#define __ELASTOS_DROID_CONTENT_PM_CLABELEDINTENT_H__

#include "_CLabeledIntent.h"
#include "../Intent.h"

using Elastos::Core::ICharSequence;
using Elastos::Core::IStringBuilder;

using Elastos::Droid::Content::IIntent;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::IClipData;
using Elastos::Droid::Content::IContentResolver;
using Elastos::Droid::Content::IComponentName;
using Elastos::Droid::Graphics::IRect;
using Elastos::Droid::Graphics::Drawable::IDrawable;
using Elastos::Droid::Net::IUri;
using Elastos::Droid::Os::IBundle;
using Elastos::Utility::IObjectStringMap;

namespace Elastos {
namespace Droid {
namespace Content {
namespace Pm {

/**
* A special subclass of Intent that can have a custom label/icon
* associated with it.  Primarily for use with {@link Intent#ACTION_CHOOSER}.
 */
CarClass(CLabeledIntent), public Intent
{
public:
    CLabeledIntent();

    ~CLabeledIntent();

    CARAPI_(PInterface) Probe(
            /* [in] */ REIID riid);

    CARAPI constructor();

    /**
     * Create a labeled intent from the given intent, supplying the label
     * and icon resources for it.
     *
     * @param origIntent The original Intent to copy.
     * @param sourcePackage The package in which the label and icon live.
     * @param labelRes Resource containing the label, or 0 if none.
     * @param icon Resource containing the icon, or 0 if none.
     */
    CARAPI constructor(
        /* [in] */ IIntent* origIntent,
        /* [in] */ const String& sourcePackage,
        /* [in] */ Int32 labelRes,
        /* [in] */ Int32 icon);

    /**
     * Create a labeled intent from the given intent, supplying a textual
     * label and icon resource for it.
     *
     * @param origIntent The original Intent to copy.
     * @param sourcePackage The package in which the label and icon live.
     * @param nonLocalizedLabel Concrete text to use for the label.
     * @param icon Resource containing the icon, or 0 if none.
     */
    CARAPI constructor(
        /* [in] */ IIntent* origIntent,
        /* [in] */ const String& sourcePackage,
        /* [in] */ ICharSequence* nonLocalizedLabel,
        /* [in] */ Int32 icon);

    /**
     * Create a labeled intent with no intent data but supplying the label
     * and icon resources for it.
     *
     * @param sourcePackage The package in which the label and icon live.
     * @param labelRes Resource containing the label, or 0 if none.
     * @param icon Resource containing the icon, or 0 if none.
     */
    CARAPI constructor(
        /* [in] */ const String& sourcePackage,
        /* [in] */ Int32 labelRes,
        /* [in] */ Int32 icon);

    /**
     * Create a labeled intent with no intent data but supplying a textual
     * label and icon resource for it.
     *
     * @param sourcePackage The package in which the label and icon live.
     * @param nonLocalizedLabel Concrete text to use for the label.
     * @param icon Resource containing the icon, or 0 if none.
     */
    CARAPI constructor(
        /* [in] */ const String& sourcePackage,
        /* [in] */ ICharSequence* nonLocalizedLabel,
        /* [in] */ Int32 icon);

    /**
     * Return the name of the package holding label and icon resources.
     */
    CARAPI GetSourcePackage(
        /* [out] */ String* sourcePackage);

    /**
     * Return any resource identifier that has been given for the label text.
     */
    CARAPI GetLabelResource(
        /* [out] */ Int32* labelRes);

    /**
     * Return any concrete text that has been given for the label text.
     */
    CARAPI GetNonLocalizedLabel(
        /* [out] */ ICharSequence** label);

    /**
     * Return any resource identifier that has been given for the label icon.
     */
    CARAPI GetIconResource(
        /* [out] */ Int32* iconRes);

    /**
     * Retrieve the label associated with this object.  If the object does
     * not have a label, null will be returned, in which case you will probably
     * want to load the label from the underlying resolved info for the Intent.
     */
    CARAPI LoadLabel(
        /* [in] */ IPackageManager* pm,
        /* [out] */ ICharSequence** label);

    /**
     * Retrieve the icon associated with this object.  If the object does
     * not have a icon, null will be returned, in which case you will probably
     * want to load the icon from the underlying resolved info for the Intent.
     */
    CARAPI LoadIcon(
        /* [in] */ IPackageManager* pm,
        /* [out] */ IDrawable** icon);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI Clone(
        /* [out] */ IIntent** intent);

    CARAPI CloneFilter(
        /* [out] */ IIntent** intent);

    CARAPI GetAction(
        /* [out] */ String* action);

    CARAPI GetData(
        /* [out] */ IUri** data);

    CARAPI GetDataString(
        /* [out] */ String* dataString);

    CARAPI GetScheme(
        /* [out] */ String* scheme);

    CARAPI GetType(
        /* [out] */ String* type);

    CARAPI ResolveType(
        /* [in] */ IContext* context,
        /* [out] */ String* type);

    CARAPI ResolveType(
        /* [in] */ IContentResolver* resolver,
        /* [out] */ String* type);

    CARAPI ResolveTypeIfNeeded(
        /* [in] */ IContentResolver* resolver,
        /* [out] */ String* type);

    CARAPI HasCategory(
        /* [in] */ const String& category,
        /* [out] */ Boolean* hasCategory);

    CARAPI GetCategories(
        /* [out, callee] */ ArrayOf<String>** categories);

    CARAPI GetSelector(
        /* [out] */ IIntent** intent);

    CARAPI GetClipData(
        /* [out] */ IClipData** clipData);

    CARAPI HasExtra(
        /* [in] */ const String& name,
        /* [out] */ Boolean* hasExtra);

    CARAPI HasFileDescriptors(
        /* [out] */ Boolean* hasFileDescriptors);

    CARAPI SetAllowFds(
        /* [in] */ Boolean allowFds);

    CARAPI GetExtra(
        /* [in] */ const String& name,
        /* [out] */ IInterface** obj);

    CARAPI GetBooleanExtra(
        /* [in] */ const String& name,
        /* [in] */ Boolean defaultValue,
        /* [out] */ Boolean* value);

    CARAPI GetByteExtra(
        /* [in] */ const String& name,
        /* [in] */ Byte defaultValue,
        /* [out] */ Byte* value);

    CARAPI GetInt16Extra(
        /* [in] */ const String& name,
        /* [in] */ Int16 defaultValue,
        /* [out] */ Int16* value);

    CARAPI GetCharExtra(
        /* [in] */ const String& name,
        /* [in] */ Char32 defaultValue,
        /* [out] */ Char32* value);

    CARAPI GetInt32Extra(
        /* [in] */ const String& name,
        /* [in] */ Int32 defaultValue,
        /* [out] */ Int32* value);

    CARAPI GetInt64Extra(
        /* [in] */ const String& name,
        /* [in] */ Int64 defaultValue,
        /* [out] */ Int64* value);

    CARAPI GetFloatExtra(
        /* [in] */ const String& name,
        /* [in] */ Float defaultValue,
        /* [out] */ Float* value);

    CARAPI GetDoubleExtra(
        /* [in] */ const String& name,
        /* [in] */ Double defaultValue,
        /* [out] */ Double* value);

    CARAPI GetStringExtra(
        /* [in] */ const String& name,
        /* [out] */ String* value);

    CARAPI GetCharSequenceExtra(
        /* [in] */ const String& name,
        /* [out] */ ICharSequence** value);

    CARAPI GetParcelableExtra(
        /* [in] */ const String& name,
        /* [out] */ IParcelable** value);

    CARAPI GetObjectStringMapExtra(
        /* [in] */ const String& name,
        /* [out] */ IObjectStringMap** value);

    CARAPI GetParcelableArrayExtra(
        /* [in] */ const String& name,
        /* [out, callee] */ ArrayOf<IParcelable *>** value);

    CARAPI GetParcelableArrayListExtra(
        /* [in] */ const String& name,
        /* [out] */ IObjectContainer** value);

    CARAPI GetIntegerArrayListExtra(
        /* [in] */ const String& name,
        /* [out] */ IObjectContainer** value);

    CARAPI GetStringArrayListExtra(
        /* [in] */ const String& name,
        /* [out] */ IObjectContainer** value);

    CARAPI GetCharSequenceArrayListExtra(
        /* [in] */ const String& name,
        /* [out] */ IObjectContainer** value);

    CARAPI GetBooleanArrayExtra(
        /* [in] */ const String& name,
        /* [out, callee] */ ArrayOf<Boolean>** value);

    CARAPI GetByteArrayExtra(
        /* [in] */ const String& name,
        /* [out, callee] */ ArrayOf<Byte>** value);

    CARAPI GetInt16ArrayExtra(
        /* [in] */ const String& name,
        /* [out, callee] */ ArrayOf<Int16>** value);

    CARAPI GetCharArrayExtra(
        /* [in] */ const String& name,
        /* [out, callee] */ ArrayOf<Char32>** value);

    CARAPI GetInt32ArrayExtra(
        /* [in] */ const String& name,
        /* [out, callee] */ ArrayOf<Int32>** value);

    CARAPI GetInt64ArrayExtra(
        /* [in] */ const String& name,
        /* [out, callee] */ ArrayOf<Int64>** value);

    CARAPI GetFloatArrayExtra(
        /* [in] */ const String& name,
        /* [out, callee] */ ArrayOf<Float>** value);

    CARAPI GetDoubleArrayExtra(
        /* [in] */ const String& name,
        /* [out, callee] */ ArrayOf<Double>** value);

    CARAPI GetStringArrayExtra(
        /* [in] */ const String& name,
        /* [out, callee] */ ArrayOf<String>** value);

    CARAPI GetCharSequenceArrayExtra(
        /* [in] */ const String& name,
        /* [out, callee] */ ArrayOf<ICharSequence *>** value);

    CARAPI GetBundleExtra(
        /* [in] */ const String& name,
        /* [out] */ IBundle** value);

    CARAPI GetExtra(
        /* [in] */ const String& name,
        /* [in] */ IInterface* defaultValue,
        /* [out] */ IInterface** obj);

    CARAPI GetExtras(
        /* [out] */ IBundle** extras);

    CARAPI GetFlags(
        /* [out] */ Int32* flags);

    CARAPI IsExcludingStopped(
        /* [out] */ Boolean* isStopped);

    CARAPI GetPackage(
        /* [out] */ String* packageName);

    CARAPI GetComponent(
        /* [out] */ IComponentName** componentName);

    CARAPI GetSourceBounds(
        /* [out] */ IRect** sourceBounds);

    CARAPI ResolveActivity(
        /* [in] */ IPackageManager* pm,
        /* [out] */ IComponentName** componentName);

    CARAPI ResolveActivityInfo(
        /* [in] */ IPackageManager* pm,
        /* [in] */ Int32 flags,
        /* [out] */ IActivityInfo** result);

    CARAPI SetAction(
        /* [in] */ const String& action);

    CARAPI SetData(
        /* [in] */ IUri* data);

    CARAPI SetDataAndNormalize(
        /* [in] */ IUri* data);

    CARAPI SetType(
        /* [in] */ const String& type);

    CARAPI SetTypeAndNormalize(
        /* [in] */ const String& type);

    CARAPI SetDataAndType(
        /* [in] */ IUri* data,
        /* [in] */ const String& type);

    CARAPI SetDataAndTypeAndNormalize(
        /* [in] */ IUri* data,
        /* [in] */ const String& type);

    CARAPI AddCategory(
        /* [in] */ const String& category);

    CARAPI RemoveCategory(
        /* [in] */ const String& category);

    CARAPI SetSelector(
        /* [in] */ IIntent* selector);

    CARAPI SetClipData(
        /* [in] */ IClipData* clip);

    CARAPI PutBooleanExtra(
        /* [in] */ const String& name,
        /* [in] */ Boolean value);

    CARAPI PutByteExtra(
        /* [in] */ const String& name,
        /* [in] */ Byte value);

    CARAPI PutCharExtra(
        /* [in] */ const String& name,
        /* [in] */ Char32 value);

    CARAPI PutInt16Extra(
        /* [in] */ const String& name,
        /* [in] */ Int16 value);

    CARAPI PutInt32Extra(
        /* [in] */ const String& name,
        /* [in] */ Int32 value);

    CARAPI PutInt64Extra(
        /* [in] */ const String& name,
        /* [in] */ Int64 value);

    CARAPI PutFloatExtra(
        /* [in] */ const String& name,
        /* [in] */ Float value);

    CARAPI PutDoubleExtra(
        /* [in] */ const String& name,
        /* [in] */ Double value);

    CARAPI PutStringExtra(
        /* [in] */ const String& name,
        /* [in] */ const String& value);

    CARAPI PutCharSequenceExtra(
        /* [in] */ const String& name,
        /* [in] */ ICharSequence* value);

    CARAPI PutParcelableExtra(
        /* [in] */ const String& name,
        /* [in] */ IParcelable* value);

    CARAPI PutObjectStringMapExtra(
        /* [in] */ const String& name,
        /* [in] */ IObjectStringMap* value);

    CARAPI PutParcelableArrayExtra(
        /* [in] */ const String& name,
        /* [in] */ ArrayOf<IParcelable *>* value);

    CARAPI PutParcelableArrayListExtra(
        /* [in] */ const String& name,
        /* [in] */ IObjectContainer* value);

    CARAPI PutIntegerArrayListExtra(
        /* [in] */ const String& name,
        /* [in] */ IObjectContainer* value);

    CARAPI PutStringArrayListExtra(
        /* [in] */ const String& name,
        /* [in] */ IObjectContainer* value);

    CARAPI PutCharSequenceArrayListExtra(
        /* [in] */ const String& name,
        /* [in] */ IObjectContainer* value);

    CARAPI PutBooleanArrayExtra(
        /* [in] */ const String& name,
        /* [in] */ ArrayOf<Boolean>* value);

    CARAPI PutByteArrayExtra(
        /* [in] */ const String& name,
        /* [in] */ ArrayOf<Byte>* value);

    CARAPI PutInt16ArrayExtra(
        /* [in] */ const String& name,
        /* [in] */ ArrayOf<Int16>* value);

    CARAPI PutCharArrayExtra(
        /* [in] */ const String& name,
        /* [in] */ ArrayOf<Char32>* value);

    CARAPI PutInt32ArrayExtra(
        /* [in] */ const String& name,
        /* [in] */ ArrayOf<Int32>* value);

    CARAPI PutInt64ArrayExtra(
        /* [in] */ const String& name,
        /* [in] */ ArrayOf<Int64>* value);

    CARAPI PutFloatArrayExtra(
        /* [in] */ const String& name,
        /* [in] */ ArrayOf<Float>* value);

    CARAPI PutDoubleArrayExtra(
        /* [in] */ const String& name,
        /* [in] */ ArrayOf<Double>* value);

    CARAPI PutStringArrayExtra(
        /* [in] */ const String& name,
        /* [in] */ ArrayOf<String>* value);

    CARAPI PutCharSequenceArrayExtra(
        /* [in] */ const String& name,
        /* [in] */ ArrayOf<ICharSequence *>* value);

    CARAPI PutBundleExtra(
        /* [in] */ const String& name,
        /* [in] */ IBundle* value);

    CARAPI PutExtras(
        /* [in] */ IIntent* src);

    CARAPI PutExtras(
        /* [in] */ IBundle* extras);

    CARAPI ReplaceExtras(
        /* [in] */ IIntent* src);

    CARAPI ReplaceExtras(
        /* [in] */ IBundle* extras);

    CARAPI RemoveExtra(
        /* [in] */ const String& name);

    CARAPI SetFlags(
        /* [in] */ Int32 flags);

    CARAPI AddFlags(
        /* [in] */ Int32 flags);

    CARAPI SetPackage(
        /* [in] */ const String& packageeName);

    CARAPI SetComponent(
        /* [in] */ IComponentName* component);

    CARAPI SetClassName(
        /* [in] */ IContext* packageContext,
        /* [in] */ const String& className);

    CARAPI SetClassName(
        /* [in] */ const String& packageName,
        /* [in] */ const String& className);

    CARAPI SetClass(
        /* [in] */ IContext* packageContext,
        /* [in] */ const ClassID& cls);

    CARAPI SetSourceBounds(
        /* [in] */ IRect* r);

    CARAPI FillIn(
        /* [in] */ IIntent* other,
        /* [in] */ Int32 flags,
        /* [out] */ Int32* result);

    CARAPI FilterEquals(
        /* [in] */ IIntent* other,
        /* [out] */ Boolean* isEqual);

    CARAPI FilterHashCode(
        /* [out] */ Int32* result);

    CARAPI ToString(
        /* [out] */ String* str);

    CARAPI ToInsecureString(
        /* [out] */ String* str);

    CARAPI ToInsecureStringWithClip(
        /* [out] */ String* str);

    CARAPI ToShortString(
        /* [in] */ Boolean secure,
        /* [in] */ Boolean comp,
        /* [in] */ Boolean extras,
        /* [in] */ Boolean clip,
        /* [out] */ String* str);

    CARAPI ToShortString(
        /* [in] */ IStringBuilder* b,
        /* [in] */ Boolean secure,
        /* [in] */ Boolean comp,
        /* [in] */ Boolean extras,
        /* [in] */ Boolean clip);

    CARAPI ToURI(
        /* [out] */ String* str);

    CARAPI ToUri(
        /* [in] */ Int32 flags,
        /* [out] */ String* str);

    CARAPI MigrateExtraStreamToClipData(
        /* [out] */ Boolean* result);

private:
    String mSourcePackage;

    Int32 mLabelRes;

    AutoPtr<ICharSequence> mNonLocalizedLabel;

    Int32 mIcon;

};

}
}
}
}

#endif // __ELASTOS_DROID_CONTENT_PM_CLABELEDINTENT_H__
