
#include "elastos/droid/preference/PreferenceManager.h"
// #include "elastos/droid/preference/CPreferenceScreen.h"
#include "elastos/droid/preference/PreferenceInflater.h"
#include <elastos/utility/logging/Slogger.h>
#include <elastos/utility/etl/HashSet.h>
#include <elastos/core/StringBuilder.h>
#include <elastos/utility/etl/Algorithm.h>
#include <elastos/core/AutoLock.h>

using Elastos::Core::StringBuilder;
using Elastos::Core::CString;
using Elastos::Core::AutoLock;
using Elastos::Utility::Etl::HashSet;
using Elastos::Utility::Logging::Slogger;
using Elastos::Droid::Content::Res::IXmlResourceParser;
using Elastos::Droid::Content::Pm::IPackageManager;
using Elastos::Droid::Content::Pm::IActivityInfo;
using Elastos::Utility::IList;
using Elastos::Droid::Os::IBundle;

namespace Elastos {
namespace Droid {
namespace Preference {

const String PreferenceManager::TAG("PreferenceManager");

PreferenceManager::PreferenceManager()
    : mNextId(0)
    , mNextRequestCode(0)
    , mNoCommit(FALSE)
    , mSharedPreferencesMode(0)
{}

PreferenceManager::PreferenceManager(
    /* [in] */ IActivity* activity,
    /* [in] */ Int32 firstRequestCode)
{
    mActivity = activity;
    mNextRequestCode = firstRequestCode;

    Init(IContext::Probe(activity));
}

PreferenceManager::PreferenceManager(
    /* [in] */ IContext* context)
{
    Init(context);
}

CAR_INTERFACE_IMPL(PreferenceManager, Object, IPreferenceManager)

ECode PreferenceManager::constructor(
    /* [in] */ IActivity* activity,
    /* [in] */ Int32 firstRequestCode)
{
    mActivity = activity;
    mNextRequestCode = firstRequestCode;

    Init(IContext::Probe(activity));
    return NOERROR;
}

ECode PreferenceManager::constructor(
    /* [in] */ IContext* context)
{
    Init(context);
    return NOERROR;
}

void PreferenceManager::Init(
    /* [in] */ IContext* context)
{
    mContext = context;
    SetSharedPreferencesName(GetDefaultSharedPreferencesName(context));
}

ECode PreferenceManager::SetFragment(
    /* [in] */ IPreferenceFragment* fragment)
{
    mFragment = fragment;
    return NOERROR;
}

ECode PreferenceManager::GetFragment(
    /* [out] */ IPreferenceFragment** fragment)
{
    VALIDATE_NOT_NULL(fragment)
    *fragment = mFragment;
    REFCOUNT_ADD(*fragment)
    return NOERROR;
}

AutoPtr<List<AutoPtr<IResolveInfo> > > PreferenceManager::QueryIntentActivities(
    /* [in] */ IIntent* queryIntent)
{
    AutoPtr<IPackageManager> manager;
    mContext->GetPackageManager((IPackageManager**)&manager);
    AutoPtr<IList> resolves;
    manager->QueryIntentActivities(queryIntent, IPackageManager::GET_META_DATA, (IList**)&resolves);
    // AutoPtr<List<AutoPtr<IResolveInfo> > list;
    Int32 size;
    resolves->GetSize(&size);
    for (Int32 i = 0; i < size; ++i) {
        AutoPtr<IInterface> element;
        resolves->Get(i, (IInterface**)&element);
        AutoPtr<IResolveInfo> rs = IResolveInfo::Probe(element);
        // list.PushBack(rs);
    }
    // return list;
    return NULL;
}

ECode PreferenceManager::InflateFromIntent(
    /* [in] */ IIntent* queryIntent,
    /* [in] */ IPreferenceScreen* _rootPreferences,
    /* [out] */ IPreferenceScreen** screen)
{
    // VALIDATE_NOT_NULL(screen)

    // AutoPtr<IPreference> rootPreferences = IPreference::Probe(_rootPreferences);

    // AutoPtr<List<AutoPtr<IResolveInfo> > > activities = QueryIntentActivities(queryIntent);
    // HashSet<String> inflatedRes;

    // List<AutoPtr<IResolveInfo> >::ReverseIterator rit = activities->RBegin();
    // for (; rit != activities->REnd(); ++rit) {
    //     AutoPtr<IActivityInfo> activityInfo;
    //     (*rit)->GetActivityInfo((IActivityInfo**)&activityInfo);
        // AutoPtr<IBundle> metaData;
        // activityInfo->GetMetaData((IBundle**)&metaData);

        // Boolean contains;
        // if ((metaData == NULL) || (metaData->ContainsKey(METADATA_KEY_PREFERENCES, &contains), !contains)) {
        //     continue;
        // }

        // Need to concat the package with res ID since the same res ID
        // can be re-used across contexts
        // String packageName;
        // activityInfo->GetPackageName(&packageName);
        // Int32 keyPreferences;
        // metaData->GetInt32(METADATA_KEY_PREFERENCES, &keyPreferences);
        // StringBuilder sb(packageName);
        // sb += ":";
        // sb += keyPreferences;
        // String uniqueResId = sb.ToString();

        // if (inflatedRes.Find(uniqueResId) == inflatedRes.End()) {
        //     inflatedRes.Insert(uniqueResId);

        //     AutoPtr<IContext> context;
        //     // try {
        //     ECode ec = mContext->CreatePackageContext(packageName, 0, (IContext**)&context);
        //     if (ec == (ECode)E_NAME_NOT_FOUND_EXCEPTION) {
        //         Slogger::W(TAG, "Could not create context for %s: 0x%08x", packageName.string(), ec);
        //         continue;
        //     }
            // } catch (NameNotFoundException e) {
            //     Log.w(TAG, "Could not create context for " + activityInfo.packageName + ": "
            //         + Log.getStackTraceString(e));
            //     continue;
            // }

            // AutoPtr<PreferenceInflater> inflater = new PreferenceInflater(context, THIS_PROBE(IPreferenceManager));
            // AutoPtr<IPackageManager> pm;
            // context->GetPackageManager((IPackageManager**)&pm);
            // AutoPtr<IXmlResourceParser> parser;
            // activityInfo->LoadXmlMetaData(pm, METADATA_KEY_PREFERENCES, (IXmlResourceParser**)&parser);
            // AutoPtr<IInterface> temp;
            // inflater->Inflate(parser, rootPreferences, TRUE, (IInterface**)&temp);
            // rootPreferences = IPreference::Probe(temp);
            // parser->Close();
    //     }
    // }

    // rootPreferences->OnAttachedToHierarchy(this);

    // *screen = IPreferenceScreen::Probe(rootPreferences);
    // REFCOUNT_ADD(*screen)
    return NOERROR;
}

ECode PreferenceManager::InflateFromResource(
    /* [in] */ IContext* context,
    /* [in] */ Int32 resId,
    /* [in] */ IPreferenceScreen* rootPreferences,
    /* [out] */ IPreferenceScreen** screen)
{
    VALIDATE_NOT_NULL(screen)
    // Block commits
    SetNoCommit(TRUE);

    AutoPtr<PreferenceInflater> inflater = new PreferenceInflater(context, THIS_PROBE(IPreferenceManager));
    AutoPtr<IInterface> pfObj;
    inflater->Inflate(resId, IPreference::Probe(rootPreferences), TRUE, (IInterface**)&pfObj);
    AutoPtr<IPreference> pf = IPreference::Probe(pfObj);
    pf->OnAttachedToHierarchy(THIS_PROBE(IPreferenceManager));

    // Unblock commits
    SetNoCommit(FALSE);
    *screen = IPreferenceScreen::Probe(pf);
    REFCOUNT_ADD(*screen)
    return NOERROR;
}

ECode PreferenceManager::CreatePreferenceScreen(
    /* [in] */ IContext* context,
    /* [out] */ IPreferenceScreen** screen)
{
    VALIDATE_NOT_NULL(screen)
    // AutoPtr<IPreferenceScreen> preferenceScreen;
    // CPreferenceScreen::New(context, NULL, (IPreferenceScreen**)&preferenceScreen);
    // IPreference::Probe(preferenceScreen)->OnAttachedToHierarchy(this);
    // *screen = preferenceScreen;
    // REFCOUNT_ADD(*screen)
    return NOERROR;
}

ECode PreferenceManager::GetNextId(
    /* [out] */ Int64* id)
{
    VALIDATE_NOT_NULL(id)
    AutoLock lock(mLock);
    *id = mNextId++;
    return NOERROR;
}

ECode PreferenceManager::GetSharedPreferencesName(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name)
    *name = mSharedPreferencesName;
    return NOERROR;
}

ECode PreferenceManager::SetSharedPreferencesName(
    /* [in] */ const String& sharedPreferencesName)
{
    mSharedPreferencesName = sharedPreferencesName;
    mSharedPreferences = NULL;
    return NOERROR;
}

ECode PreferenceManager::GetSharedPreferencesMode(
    /* [out] */ Int32* mode)
{
    VALIDATE_NOT_NULL(mode)
    *mode = mSharedPreferencesMode;
    return NOERROR;
}

ECode PreferenceManager::SetSharedPreferencesMode(
    /* [in] */ Int32 sharedPreferencesMode)
{
    mSharedPreferencesMode = sharedPreferencesMode;
    mSharedPreferences = NULL;
    return NOERROR;
}

ECode PreferenceManager::GetSharedPreferences(
    /* [out] */ ISharedPreferences** sp)
{
    VALIDATE_NOT_NULL(sp)

    if (mSharedPreferences == NULL) {
        mContext->GetSharedPreferences(mSharedPreferencesName,
                mSharedPreferencesMode, (ISharedPreferences**)&mSharedPreferences);
    }

    *sp = mSharedPreferences;
    REFCOUNT_ADD(*sp)
    return NOERROR;
}

AutoPtr<ISharedPreferences> PreferenceManager::GetDefaultSharedPreferences(
    /* [in] */ IContext* context)
{
    AutoPtr<ISharedPreferences> preferences;
    context->GetSharedPreferences(GetDefaultSharedPreferencesName(context),
            GetDefaultSharedPreferencesMode(), (ISharedPreferences**)&preferences);
    return preferences;
}

String PreferenceManager::GetDefaultSharedPreferencesName(
        /* [in] */ IContext* context)
{
    String packageName;
    context->GetPackageName(&packageName);
    return packageName + String("_preferences");
}

Int32 PreferenceManager::GetDefaultSharedPreferencesMode()
{
    return IContext::MODE_PRIVATE;
}

ECode PreferenceManager::GetPreferenceScreen(
    /* [out] */ IPreferenceScreen** screen)
{
    VALIDATE_NOT_NULL(screen)
    *screen = mPreferenceScreen;
    REFCOUNT_ADD(*screen)
    return NOERROR;
}

ECode PreferenceManager::SetPreferences(
    /* [in] */ IPreferenceScreen* preferenceScreen,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    if (preferenceScreen != mPreferenceScreen) {
        mPreferenceScreen = preferenceScreen;
        *result = TRUE;
        return NOERROR;
    }
    *result = FALSE;
    return NOERROR;
}

ECode PreferenceManager::FindPreference(
    /* [in] */ ICharSequence* key,
    /* [out] */ IPreference** preference)
{
    VALIDATE_NOT_NULL(preference)
    if (mPreferenceScreen == NULL) {
        *preference = NULL;
        return NOERROR;
    }
    // return mPreferenceScreen->FindPreference(key, preference);
    return NOERROR;
}

void PreferenceManager::SetDefaultValues(
    /* [in] */ IContext* context,
    /* [in] */ Int32 resId,
    /* [in] */ Boolean readAgain)
{
    // Use the default shared preferences name and mode
    SetDefaultValues(context, GetDefaultSharedPreferencesName(context),
            GetDefaultSharedPreferencesMode(), resId, readAgain);
}

void PreferenceManager::SetDefaultValues(
    /* [in] */ IContext* context,
    /* [in] */ const String& sharedPreferencesName,
    /* [in] */ Int32 sharedPreferencesMode,
    /* [in] */ Int32 resId,
    /* [in] */ Boolean readAgain)
{
    AutoPtr<ISharedPreferences> defaultValueSp;
    context->GetSharedPreferences(KEY_HAS_SET_DEFAULT_VALUES, IContext::MODE_PRIVATE, (ISharedPreferences**)&defaultValueSp);

    Boolean value;
    if (readAgain || (defaultValueSp->GetBoolean(KEY_HAS_SET_DEFAULT_VALUES, FALSE, &value), !value)) {
        AutoPtr<PreferenceManager> pm = new PreferenceManager(context);
        pm->SetSharedPreferencesName(sharedPreferencesName);
        pm->SetSharedPreferencesMode(sharedPreferencesMode);
        AutoPtr<IPreferenceScreen> screen;
        pm->InflateFromResource(context, resId, NULL, (IPreferenceScreen**)&screen);

        AutoPtr<ISharedPreferencesEditor> editor;
        defaultValueSp->Edit((ISharedPreferencesEditor**)&editor);
        editor->PutBoolean(KEY_HAS_SET_DEFAULT_VALUES, TRUE);
        // try {
        editor->Apply();
        // } catch (AbstractMethodError unused) {
        //     // The app injected its own pre-Gingerbread
        //     // SharedPreferences.Editor implementation without
        //     // an apply method.
        //     editor.commit();
        // }
    }
}

ECode PreferenceManager::GetEditor(
    /* [out] */ ISharedPreferencesEditor** editor)
{
    VALIDATE_NOT_NULL(editor)
    if (mNoCommit) {
        if (mEditor == NULL) {
            AutoPtr<ISharedPreferences> sp;
            GetSharedPreferences((ISharedPreferences**)&sp);
            sp->Edit((ISharedPreferencesEditor**)&mEditor);
        }
        *editor = mEditor;
        REFCOUNT_ADD(*editor)
        return NOERROR;
    }
    else {
        mEditor = NULL;
        AutoPtr<ISharedPreferences> share;
        GetSharedPreferences((ISharedPreferences**)&share);
        share->Edit((ISharedPreferencesEditor**)&mEditor);
        *editor = mEditor;
        REFCOUNT_ADD(*editor)
        return NOERROR;
    }
}

ECode PreferenceManager::ShouldCommit(
    /* [out] */ Boolean* shouldCommit)
{
    VALIDATE_NOT_NULL(shouldCommit)
    *shouldCommit = !mNoCommit;
    return NOERROR;
}

void PreferenceManager::SetNoCommit(
    /* [in] */ Boolean noCommit)
{
    if (!noCommit && mEditor != NULL) {
        // try {
        mEditor->Apply();
        // } catch (AbstractMethodError unused) {
        //     // The app injected its own pre-Gingerbread
        //     // SharedPreferences.Editor implementation without
        //     // an apply method.
        //     mEditor.commit();
        // }
    }
    mNoCommit = noCommit;
}

ECode PreferenceManager::GetActivity(
    /* [out] */ IActivity** activity)
{
    VALIDATE_NOT_NULL(activity)
    *activity = mActivity;
    REFCOUNT_ADD(*activity)
    return NOERROR;
}

ECode PreferenceManager::GetContext(
    /* [out] */ IContext** context)
{
    VALIDATE_NOT_NULL(context)
    *context = mContext;
    REFCOUNT_ADD(*context)
    return NOERROR;
}

ECode PreferenceManager::RegisterOnActivityResultListener(
    /* [in] */ IPreferenceManagerOnActivityResultListener* listener)
{
    AutoLock lock(mLock);

    if (mActivityResultListeners == NULL) {
        mActivityResultListeners = new List<AutoPtr<IPreferenceManagerOnActivityResultListener> >();
    }

    AutoPtr<IPreferenceManagerOnActivityResultListener> temp = listener;
    List<AutoPtr<IPreferenceManagerOnActivityResultListener> >::Iterator it
            = Find(mActivityResultListeners->Begin(), mActivityResultListeners->End(), temp);
    if (it == mActivityResultListeners->End()) {
        mActivityResultListeners->PushBack(listener);
    }

    return NOERROR;
}

ECode PreferenceManager::UnregisterOnActivityResultListener(
    /* [in] */ IPreferenceManagerOnActivityResultListener* listener)
{
    AutoLock lock(mLock);
    if (mActivityResultListeners != NULL) {
        mActivityResultListeners->Remove(listener);
    }
    return NOERROR;
}

ECode PreferenceManager::DispatchActivityResult(
    /* [in] */ Int32 requestCode,
    /* [in] */ Int32 resultCode,
    /* [in] */ IIntent* data)
{
    AutoPtr<List< AutoPtr<IPreferenceManagerOnActivityResultListener> > > list;

    {
        AutoLock lock(mLock);
        if (mActivityResultListeners == NULL) {
            return NOERROR;
        }
        list = new List< AutoPtr<IPreferenceManagerOnActivityResultListener> >(*mActivityResultListeners);
    }

    List<AutoPtr<IPreferenceManagerOnActivityResultListener> >::Iterator it;
    for (it = list->Begin(); it != list->End(); ++it) {
        Boolean result;
        (*it)->OnActivityResult(requestCode, resultCode, data, &result);
        if (result) {
            break;
        }
    }

    return NOERROR;
}

ECode PreferenceManager::RegisterOnActivityStopListener(
    /* [in] */ IPreferenceManagerOnActivityStopListener* listener)
{
    AutoLock lock(mLock);
    if (mActivityStopListeners == NULL) {
        mActivityStopListeners = new List<AutoPtr<IPreferenceManagerOnActivityStopListener> >();
    }

    AutoPtr<IPreferenceManagerOnActivityStopListener> temp = listener;
    List<AutoPtr<IPreferenceManagerOnActivityStopListener> >::Iterator it
            = Find(mActivityStopListeners->Begin(), mActivityStopListeners->End(), temp);
    if (it == mActivityStopListeners->End()) {
        mActivityStopListeners->PushBack(listener);
    }

    return NOERROR;
}

ECode PreferenceManager::UnregisterOnActivityStopListener(
    /* [in] */ IPreferenceManagerOnActivityStopListener* listener)
{
    AutoLock lock(mLock);
    if (mActivityStopListeners != NULL) {
        mActivityStopListeners->Remove(listener);
    }
    return NOERROR;
}

ECode PreferenceManager::DispatchActivityStop()
{
    AutoPtr<List<AutoPtr<IPreferenceManagerOnActivityStopListener> > > list;

    {
        AutoLock lock(mLock);
        if (mActivityStopListeners == NULL) return NOERROR;
        list = new List<AutoPtr<IPreferenceManagerOnActivityStopListener> >(*mActivityStopListeners);
    }

    List<AutoPtr<IPreferenceManagerOnActivityStopListener> >::Iterator it;
    for (it = list->Begin(); it != list->End(); ++it) {
        (*it)->OnActivityStop();
    }
    return NOERROR;
}

ECode PreferenceManager::RegisterOnActivityDestroyListener(
    /* [in] */ IPreferenceManagerOnActivityDestroyListener* listener)
{
    AutoLock lock(mLock);
    if (mActivityDestroyListeners == NULL) {
        mActivityDestroyListeners = new List<AutoPtr<IPreferenceManagerOnActivityDestroyListener> >();
    }

    AutoPtr<IPreferenceManagerOnActivityDestroyListener> temp = listener;
    List<AutoPtr<IPreferenceManagerOnActivityDestroyListener> >::Iterator it
            = Find(mActivityDestroyListeners->Begin(), mActivityDestroyListeners->End(), temp);
    if (it == mActivityDestroyListeners->End()) {
        mActivityDestroyListeners->PushBack(listener);
    }
    return NOERROR;
}

ECode PreferenceManager::UnregisterOnActivityDestroyListener(
    /* [in] */ IPreferenceManagerOnActivityDestroyListener* listener)
{
    AutoLock lock(mLock);
    if (mActivityDestroyListeners != NULL) {
        mActivityDestroyListeners->Remove(listener);
    }
    return NOERROR;
}

ECode PreferenceManager::DispatchActivityDestroy()
{
    AutoPtr<List<AutoPtr<IPreferenceManagerOnActivityDestroyListener> > > list;

    {
        AutoLock lock(mLock);
        if (mActivityDestroyListeners != NULL) {
            list = new List<AutoPtr<IPreferenceManagerOnActivityDestroyListener> >(*mActivityDestroyListeners);
        }
    }

    if (list != NULL) {
        List<AutoPtr<IPreferenceManagerOnActivityDestroyListener> >::Iterator it;
        for (it = list->Begin(); it != list->End(); ++it) {
            (*it)->OnActivityDestroy();
        }
    }

    // Dismiss any PreferenceScreens still showing
    DismissAllScreens();

    return NOERROR;
}

ECode PreferenceManager::GetNextRequestCode(
    /* [out] */ Int32* code)
{
    VALIDATE_NOT_NULL(code)
    AutoLock lock(mLock);
    *code = mNextRequestCode++;
    return NOERROR;
}

ECode PreferenceManager::AddPreferencesScreen(
    /* [in] */ IDialogInterface* screen)
{
    AutoLock lock(mLock);
    if (mPreferencesScreens == NULL) {
        mPreferencesScreens = new List<AutoPtr<IDialogInterface> >();
    }

    mPreferencesScreens->PushBack(screen);
    return NOERROR;
}

ECode PreferenceManager::RemovePreferencesScreen(
    /* [in] */ IDialogInterface* screen)
{
    AutoLock lock(mLock);

    if (mPreferencesScreens == NULL) {
        return NOERROR;
    }

    mPreferencesScreens->Remove(screen);
    return NOERROR;
}

ECode PreferenceManager::DispatchNewIntent(
    /* [in] */ IIntent* intent)
{
    DismissAllScreens();
    return NOERROR;
}

void PreferenceManager::DismissAllScreens()
{
     // Remove any of the previously shown preferences screens
    AutoPtr<List<AutoPtr<IDialogInterface> > > screensToDismiss;

    {
        AutoLock lock(mLock);
        if (mPreferencesScreens == NULL) {
            return;
        }

        screensToDismiss = new List<AutoPtr<IDialogInterface> >(*mPreferencesScreens);
        mPreferencesScreens->Clear();
    }

    List<AutoPtr<IDialogInterface> >::ReverseIterator rit;
    for (rit = screensToDismiss->RBegin(); rit != screensToDismiss->REnd(); ++rit) {
        (*rit)->Dismiss();
    }
}

ECode PreferenceManager::SetOnPreferenceTreeClickListener(
    /* [in] */ IPreferenceManagerOnPreferenceTreeClickListener* listener)
{
    mOnPreferenceTreeClickListener = listener;
    return NOERROR;
}

ECode PreferenceManager::GetOnPreferenceTreeClickListener(
    /* [out] */ IPreferenceManagerOnPreferenceTreeClickListener** listener)
{
    VALIDATE_NOT_NULL(listener)
    *listener = mOnPreferenceTreeClickListener;
    REFCOUNT_ADD(*listener)
    return NOERROR;
}

}
}
}
