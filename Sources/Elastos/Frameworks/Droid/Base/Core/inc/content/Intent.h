
#ifndef __INTENT_H__
#define __INTENT_H__

#include "ext/frameworkext.h"
#include <elastos/HashSet.h>
#include <elastos/StringBuilder.h>

using Elastos::Utility::HashSet;
using Elastos::Core::StringBuilder;
using Elastos::Core::ICharSequence;
using Elastos::Core::IStringBuilder;
using Elastos::Core::IClassLoader;
using Org::Xmlpull::V1::IXmlPullParser;
using Elastos::Droid::Net::IUri;
using Elastos::Droid::Os::IBundle;
using Elastos::Droid::Graphics::IRect;
using Elastos::Droid::Content::Pm::IPackageManager;
using Elastos::Droid::Content::Pm::IActivityInfo;
using Elastos::Droid::Content::Res::IResources;
using Elastos::Droid::Utility::IAttributeSet;
using Elastos::Utility::IObjectStringMap;

namespace Elastos {
namespace Droid {
namespace Content {

class Intent
{
public:
    Intent();

    virtual ~Intent();

    virtual CARAPI_(PInterface) Probe(
            /* [in] */ REIID riid) = 0;

    /**
     * Convenience function for creating a {@link #ACTION_CHOOSER} Intent.
     *
     * <p>Builds a new {@link #ACTION_CHOOSER} Intent that wraps the given
     * target intent, also optionally supplying a title.  If the target
     * intent has specified {@link #FLAG_GRANT_READ_URI_PERMISSION} or
     * {@link #FLAG_GRANT_WRITE_URI_PERMISSION}, then these flags will also be
     * set in the returned chooser intent, with its ClipData set appropriately:
     * either a direct reflection of {@link #getClipData()} if that is non-null,
     * or a new ClipData build from {@link #getData()}.
     *
     * @param target The Intent that the user will be selecting an activity
     * to perform.
     * @param title Optional title that will be displayed in the chooser.
     * @return Return a new Intent object that you can hand to
     * {@link Context#startActivity(Intent) Context.startActivity()} and
     * related methods.
     */
    static CARAPI_(AutoPtr<IIntent>) CreateChooser(
        /* [in] */ IIntent* target,
        /* [in] */ ICharSequence* title);

    CARAPI Clone(
        /* [out] */ IIntent** intent);

    CARAPI CloneFilter(
        /* [out] */ IIntent** intent);

    /**
     * Create an intent to launch the main (root) activity of a task.  This
     * is the Intent that is started when the application's is launched from
     * Home.  For anything else that wants to launch an application in the
     * same way, it is important that they use an Intent structured the same
     * way, and can use this function to ensure this is the case.
     *
     * <p>The returned Intent has the given Activity component as its explicit
     * component, {@link #ACTION_MAIN} as its action, and includes the
     * category {@link #CATEGORY_LAUNCHER}.  This does <em>not</em> have
     * {@link #FLAG_ACTIVITY_NEW_TASK} set, though typically you will want
     * to do that through {@link #addFlags(int)} on the returned Intent.
     *
     * @param mainActivity The main activity component that this Intent will
     * launch.
     * @return Returns a newly created Intent that can be used to launch the
     * activity as a main application entry.
     *
     * @see #setClass
     * @see #setComponent
     */
    static CARAPI_(AutoPtr<IIntent>) MakeMainActivity(
        /* [in] */ IComponentName* mainActivity);

    /**
     * Make an Intent for the main activity of an application, without
     * specifying a specific activity to run but giving a selector to find
     * the activity.  This results in a final Intent that is structured
     * the same as when the application is launched from
     * Home.  For anything else that wants to launch an application in the
     * same way, it is important that they use an Intent structured the same
     * way, and can use this function to ensure this is the case.
     *
     * <p>The returned Intent has {@link #ACTION_MAIN} as its action, and includes the
     * category {@link #CATEGORY_LAUNCHER}.  This does <em>not</em> have
     * {@link #FLAG_ACTIVITY_NEW_TASK} set, though typically you will want
     * to do that through {@link #addFlags(int)} on the returned Intent.
     *
     * @param selectorAction The action name of the Intent's selector.
     * @param selectorCategory The name of a category to add to the Intent's
     * selector.
     * @return Returns a newly created Intent that can be used to launch the
     * activity as a main application entry.
     *
     * @see #setSelector(Intent)
     */
    static CARAPI_(AutoPtr<IIntent>) MakeMainSelectorActivity(
        /* [in] */ const String& selectorAction,
        /* [in] */ const String& selectorCategory);

    /**
     * Make an Intent that can be used to re-launch an application's task
     * in its base state.  This is like {@link #makeMainActivity(ComponentName)},
     * but also sets the flags {@link #FLAG_ACTIVITY_NEW_TASK} and
     * {@link #FLAG_ACTIVITY_CLEAR_TASK}.
     *
     * @param mainActivity The activity component that is the root of the
     * task; this is the activity that has been published in the application's
     * manifest as the main launcher icon.
     *
     * @return Returns a newly created Intent that can be used to relaunch the
     * activity's task in its root state.
     */
    static CARAPI_(AutoPtr<IIntent>) MakeRestartActivityTask(
        /* [in] */ IComponentName* mainActivity);

    /**
     * Call {@link #parseUri} with 0 flags.
     * @deprecated Use {@link #parseUri} instead.
     */
    static CARAPI GetIntent(
        /* [in] */ const String& uri,
        /* [out] */ IIntent** intent);

    /**
     * Create an intent from a URI.  This URI may encode the action,
     * category, and other intent fields, if it was returned by
     * {@link #toUri}.  If the Intent was not generate by toUri(), its data
     * will be the entire URI and its action will be ACTION_VIEW.
     *
     * <p>The URI given here must not be relative -- that is, it must include
     * the scheme and full path.
     *
     * @param uri The URI to turn into an Intent.
     * @param flags Additional processing flags.  Either 0 or
     * {@link #URI_INTENT_SCHEME}.
     *
     * @return Intent The newly created Intent object.
     *
     * @throws URISyntaxException Throws URISyntaxError if the basic URI syntax
     * it bad (as parsed by the Uri class) or the Intent data within the
     * URI is invalid.
     *
     * @see #toUri
     */
    static CARAPI ParseUri(
        /* [in] */ const String& uri,
        /* [in] */ Int32 flags,
        /* [out] */ IIntent** intent);

    static CARAPI GetIntentOld(
        /* [in] */ const String& uri,
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

    CARAPI ResolveTypeEx(
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

    CARAPI SetExtrasClassLoader(
        /* [in] */ IClassLoader* loader);

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
        /* [out, callee] */ ArrayOf<IParcelable*>** value);

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
        /* [out, callee] */ ArrayOf<ICharSequence*>** value);

    CARAPI GetBundleExtra(
        /* [in] */ const String& name,
        /* [out] */ IBundle** value);

    CARAPI GetExtraEx(
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
        /* [in] */ ArrayOf<IParcelable*>* value);

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
        /* [in] */ ArrayOf<ICharSequence*>* value);

    CARAPI PutBundleExtra(
        /* [in] */ const String& name,
        /* [in] */ IBundle* value);

    CARAPI PutExtras(
        /* [in] */ IIntent* src);

    CARAPI PutExtrasEx(
        /* [in] */ IBundle* extras);

    CARAPI ReplaceExtras(
        /* [in] */ IIntent* src);

    CARAPI ReplaceExtrasEx(
        /* [in] */ IBundle* extras);

    CARAPI RemoveExtra(
        /* [in] */ const String& name);

    CARAPI SetFlags(
        /* [in] */ Int32 flags);

    CARAPI AddFlags(
        /* [in] */ Int32 flags);

    CARAPI SetPackage(
        /* [in] */ const String& packageName);

    CARAPI SetComponent(
        /* [in] */ IComponentName* component);

    CARAPI SetClassName(
        /* [in] */ IContext* packageContext,
        /* [in] */ const String& className);

    CARAPI SetClassNameEx(
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

    CARAPI ToShortStringEx(
        /* [in] */ IStringBuilder* sb,
        /* [in] */ Boolean secure,
        /* [in] */ Boolean comp,
        /* [in] */ Boolean extras,
        /* [in] */ Boolean clip);

    CARAPI ToURI(
        /* [out] */ String* str);

    CARAPI ToUri(
        /* [in] */ Int32 flags,
        /* [out] */ String* str);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    static CARAPI ParseIntent(
        /* [in] */ IResources* resources,
        /* [in] */ IXmlPullParser* parser,
        /* [in] */ IAttributeSet* attrs,
        /* [out] */ IIntent** intent);

    static CARAPI_(String) NormalizeMimeType(
        /* [in] */ const String& type);

    CARAPI MigrateExtraStreamToClipData(
        /* [out] */ Boolean* result);

protected:
    CARAPI Init();

    CARAPI Init(
        /* [in] */ IIntent* intent);

    CARAPI Init(
        /* [in] */ IIntent* intent,
        /* [in] */ Boolean all);

    CARAPI Init(
        /* [in] */ const String& action);

    CARAPI Init(
        /* [in] */ const String& action,
        /* [in] */ IUri* uri);

    CARAPI Init(
        /* [in] */ IContext* packageContext,
        /* [in] */ const ClassID& clsId);

    CARAPI Init(
        /* [in] */ const String& action,
        /* [in] */ IUri* uri,
        /* [in] */ IContext* packageContext,
        /* [in] */ IClassInfo* cls);

    CARAPI_(void) ToUriInner(
        /* [in] */ StringBuilder& uri,
        /* [in] */ const String& scheme,
        /* [in] */ Int32 flags);

private:
    static CARAPI_(AutoPtr<IClipDataItem>) MakeClipItem(
        /* [in] */ ArrayOf<IUri*>* streams,
        /* [in] */ ArrayOf<ICharSequence*>* texts,
        /* [in] */ ArrayOf<String>* htmlTexts,
        /* [in] */ Int32 which);

    CARAPI_(AutoPtr<IClassInfo>) TransformClassInfo(
        /* [in] */ const ClassID& objId);
private:
    static const String TAG;

    String mAction;
    AutoPtr<IUri> mData;
    String mType;
    String mPackage;
    AutoPtr<IComponentName> mComponent;
    Int32 mFlags;
    AutoPtr<HashSet<String> > mCategories;
    AutoPtr<IBundle> mExtras;
    AutoPtr<IRect> mSourceBounds;
    AutoPtr<IIntent> mSelector;
    AutoPtr<IClipData> mClipData;
};

} // Content
} // Droid
} // Elastos

#endif //__INTENT_H__
