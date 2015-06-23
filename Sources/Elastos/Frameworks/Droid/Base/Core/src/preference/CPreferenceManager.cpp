
#include "CPreferenceManager.h"
#include "CPreferenceScreen.h"
#include "PreferenceInflater.h"
#include <elastos/utility/logging/Slogger.h>
#include <elastos/utility/etl/HashSet.h>
#include <elastos/core/StringBuilder.h>
#include <elastos/utility/etl/Algorithm.h>

using Elastos::Core::StringBuilder;
using Elastos::Core::CStringWrapper;
using Elastos::Utility::Etl::HashSet;
using Elastos::Utility::Logging::Slogger;
using Elastos::Droid::Content::Res::IXmlResourceParser;
using Elastos::Droid::Content::Pm::IPackageManager;
using Elastos::Droid::Content::Pm::IActivityInfo;

namespace Elastos {
namespace Droid {
namespace Preference {

const String CPreferenceManager::TAG("PreferenceManager");

CPreferenceManager::CPreferenceManager()
    : mNextId(0)
    , mNextRequestCode(0)
    , mNoCommit(FALSE)
    , mSharedPreferencesMode(0)
{}

void CPreferenceManager::Init(
    /* [in] */ IContext* context)
{
    mContext = context;
    SetSharedPreferencesName(GetDefaultSharedPreferencesName(context));
}

ECode CPreferenceManager::SetFragment(
    /* [in] */ IPreferenceFragment* fragment)
{
    mFragment = fragment;
    return NOERROR;
}

ECode CPreferenceManager::GetFragment(
    /* [out] */ IPreferenceFragment** fragment)
{
    VALIDATE_NOT_NULL(fragment)
    *fragment = mFragment;
    REFCOUNT_ADD(*fragment)
    return NOERROR;
}

AutoPtr<List<AutoPtr<IResolveInfo> > > CPreferenceManager::QueryIntentActivities(
    /* [in] */ IIntent* queryIntent)
{
    AutoPtr<List<AutoPtr<IResolveInfo> > > resolvelist = new List<AutoPtr<IResolveInfo> >();
    AutoPtr<IPackageManager> manager;
    mContext->GetPackageManager((IPackageManager**)&manager);
    AutoPtr<IObjectContainer> resolve;
    manager->QueryIntentActivities(queryIntent, IPackageManager::GET_META_DATA,
            (IObjectContainer**)&resolve);
    AutoPtr<IObjectEnumerator> ator;
    resolve->GetObjectEnumerator((IObjectEnumerator**)&ator);
    Boolean has;
    while (ator->MoveNext(&has), has) {
        AutoPtr<IInterface> obj;
        ator->Current((IInterface**)&obj);
        resolvelist->PushBack(IResolveInfo::Probe(obj));
    }
    return resolvelist;
}

ECode CPreferenceManager::InflateFromIntent(
    /* [in] */ IIntent* queryIntent,
    /* [in] */ IPreferenceScreen* _rootPreferences,
    /* [out] */ IPreferenceScreen** screen)
{
    VALIDATE_NOT_NULL(screen)

    AutoPtr<IPreference> rootPreferences = IPreference::Probe(_rootPreferences);

    AutoPtr<List<AutoPtr<IResolveInfo> > > activities = QueryIntentActivities(queryIntent);
    HashSet<String> inflatedRes;

    List<AutoPtr<IResolveInfo> >::ReverseIterator rit = activities->RBegin();
    for (; rit != activities->REnd(); ++rit) {
        AutoPtr<IActivityInfo> activityInfo;
        (*rit)->GetActivityInfo((IActivityInfo**)&activityInfo);
        AutoPtr<IBundle> metaData;
        activityInfo->GetMetaData((IBundle**)&metaData);

        Boolean contains;
        if ((metaData == NULL) || (metaData->ContainsKey(METADATA_KEY_PREFERENCES, &contains), !contains)) {
            continue;
        }

        // Need to concat the package with res ID since the same res ID
        // can be re-used across contexts
        String packageName;
        activityInfo->GetPackageName(&packageName);
        Int32 keyPreferences;
        metaData->GetInt32(METADATA_KEY_PREFERENCES, &keyPreferences);
        StringBuilder sb(packageName);
        sb += ":";
        sb += keyPreferences;
        String uniqueResId = sb.ToString();

        if (inflatedRes.Find(uniqueResId) == inflatedRes.End()) {
            inflatedRes.Insert(uniqueResId);

            AutoPtr<IContext> context;
            // try {
            ECode ec = mContext->CreatePackageContext(packageName, 0, (IContext**)&context);
            if (ec == (ECode)E_NAME_NOT_FOUND_EXCEPTION) {
                Slogger::W(TAG, "Could not create context for %s: 0x%08x", packageName.string(), ec);
                continue;
            }
            // } catch (NameNotFoundException e) {
            //     Log.w(TAG, "Could not create context for " + activityInfo.packageName + ": "
            //         + Log.getStackTraceString(e));
            //     continue;
            // }

            AutoPtr<PreferenceInflater> inflater = new PreferenceInflater(context, THIS_PROBE(IPreferenceManager));
            AutoPtr<IPackageManager> pm;
            context->GetPackageManager((IPackageManager**)&pm);
            AutoPtr<IXmlResourceParser> parser;
            activityInfo->LoadXmlMetaData(pm, METADATA_KEY_PREFERENCES, (IXmlResourceParser**)&parser);
            AutoPtr<IInterface> temp;
            inflater->Inflate(parser, rootPreferences, TRUE, (IInterface**)&temp);
            rootPreferences = IPreference::Probe(temp);
            parser->Close();
        }
    }

    rootPreferences->OnAttachedToHierarchy(this);

    *screen = IPreferenceScreen::Probe(rootPreferences);
    REFCOUNT_ADD(*screen)
    return NOERROR;
}

ECode CPreferenceManager::InflateFromResource(
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

ECode CPreferenceManager::CreatePreferenceScreen(
    /* [in] */ IContext* context,
    /* [out] */ IPreferenceScreen** screen)
{
    VALIDATE_NOT_NULL(screen)
    AutoPtr<IPreferenceScreen> preferenceScreen;
    CPreferenceScreen::New(context, NULL, (IPreferenceScreen**)&preferenceScreen);
    IPreference::Probe(preferenceScreen)->OnAttachedToHierarchy(this);
    *screen = preferenceScreen;
    REFCOUNT_ADD(*screen)
    return NOERROR;
}

ECode CPreferenceManager::GetNextId(
    /* [out] */ Int64* id)
{
    VALIDATE_NOT_NULL(id)
    Mutex::Autolock lock(mLock);
    *id = mNextId++;
    return NOERROR;
}

ECode CPreferenceManager::GetSharedPreferencesName(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name)
    *name = mSharedPreferencesName;
    return NOERROR;
}

ECode CPreferenceManager::SetSharedPreferencesName(
    /* [in] */ const String& sharedPreferencesName)
{
    mSharedPreferencesName = sharedPreferencesName;
    mSharedPreferences = NULL;
    return NOERROR;
}

ECode CPreferenceManager::GetSharedPreferencesMode(
    /* [out] */ Int32* mode)
{
    VALIDATE_NOT_NULL(mode)
    *mode = mSharedPreferencesMode;
    return NOERROR;
}

ECode CPreferenceManager::SetSharedPreferencesMode(
    /* [in] */ Int32 sharedPreferencesMode)
{
    mSharedPreferencesMode = sharedPreferencesMode;
    mSharedPreferences = NULL;
    return NOERROR;
}

ECode CPreferenceManager::GetSharedPreferences(
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

AutoPtr<ISharedPreferences> CPreferenceManager::GetDefaultSharedPreferences(
    /* [in] */ IContext* context)
{
    AutoPtr<ISharedPreferences> preferences;
    context->GetSharedPreferences(GetDefaultSharedPreferencesName(context),
            GetDefaultSharedPreferencesMode(), (ISharedPreferences**)&preferences);
    return preferences;
}

String CPreferenceManager::GetDefaultSharedPreferencesName(
        /* [in] */ IContext* context)
{
    String packageName;
    context->GetPackageName(&packageName);
    return packageName + String("_preferences");
}

Int32 CPreferenceManager::GetDefaultSharedPreferencesMode()
{
    return IContext::MODE_PRIVATE;
}

ECode CPreferenceManager::GetPreferenceScreen(
    /* [out] */ IPreferenceScreen** screen)
{
    VALIDATE_NOT_NULL(screen)
    *screen = mPreferenceScreen;
    REFCOUNT_ADD(*screen)
    return NOERROR;
}

ECode CPreferenceManager::SetPreferences(
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

ECode CPreferenceManager::FindPreference(
    /* [in] */ ICharSequence* key,
    /* [out] */ IPreference** preference)
{
    VALIDATE_NOT_NULL(preference)
    if (mPreferenceScreen == NULL) {
        *preference = NULL;
        return NOERROR;
    }
    return mPreferenceScreen->FindPreference(key, preference);
}

void CPreferenceManager::SetDefaultValues(
    /* [in] */ IContext* context,
    /* [in] */ Int32 resId,
    /* [in] */ Boolean readAgain)
{
    // Use the default shared preferences name and mode
    SetDefaultValues(context, GetDefaultSharedPreferencesName(context),
            GetDefaultSharedPreferencesMode(), resId, readAgain);
}

void CPreferenceManager::SetDefaultValues(
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
        AutoPtr<CPreferenceManager> pm;
        CPreferenceManager::NewByFriend(context, (CPreferenceManager**)&pm);
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

ECode CPreferenceManager::GetEditor(
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

ECode CPreferenceManager::ShouldCommit(
    /* [out] */ Boolean* shouldCommit)
{
    VALIDATE_NOT_NULL(shouldCommit)
    *shouldCommit = !mNoCommit;
    return NOERROR;
}

void CPreferenceManager::SetNoCommit(
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

ECode CPreferenceManager::GetActivity(
    /* [out] */ IActivity** activity)
{
    VALIDATE_NOT_NULL(activity)
    *activity = mActivity;
    REFCOUNT_ADD(*activity)
    return NOERROR;
}

ECode CPreferenceManager::GetContext(
    /* [out] */ IContext** context)
{
    VALIDATE_NOT_NULL(context)
    *context = mContext;
    REFCOUNT_ADD(*context)
    return NOERROR;
}

ECode CPreferenceManager::RegisterOnActivityResultListener(
    /* [in] */ IPreferenceManagerOnActivityResultListener* listener)
{
    Mutex::Autolock lock(mLock);

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

ECode CPreferenceManager::UnregisterOnActivityResultListener(
    /* [in] */ IPreferenceManagerOnActivityResultListener* listener)
{
    Mutex::Autolock lock(mLock);
    if (mActivityResultListeners != NULL) {
        mActivityResultListeners->Remove(listener);
    }
    return NOERROR;
}

ECode CPreferenceManager::DispatchActivityResult(
    /* [in] */ Int32 requestCode,
    /* [in] */ Int32 resultCode,
    /* [in] */ IIntent* data)
{
    AutoPtr<List< AutoPtr<IPreferenceManagerOnActivityResultListener> > > list;

    {
        Mutex::Autolock lock(mLock);
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

ECode CPreferenceManager::RegisterOnActivityStopListener(
    /* [in] */ IPreferenceManagerOnActivityStopListener* listener)
{
    Mutex::Autolock lock(mLock);
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

ECode CPreferenceManager::UnregisterOnActivityStopListener(
    /* [in] */ IPreferenceManagerOnActivityStopListener* listener)
{
    Mutex::Autolock lock(mLock);
    if (mActivityStopListeners != NULL) {
        mActivityStopListeners->Remove(listener);
    }
    return NOERROR;
}

ECode CPreferenceManager::DispatchActivityStop()
{
    AutoPtr<List<AutoPtr<IPreferenceManagerOnActivityStopListener> > > list;

    {
        Mutex::Autolock lock(mLock);
        if (mActivityStopListeners == NULL) return NOERROR;
        list = new List<AutoPtr<IPreferenceManagerOnActivityStopListener> >(*mActivityStopListeners);
    }

    List<AutoPtr<IPreferenceManagerOnActivityStopListener> >::Iterator it;
    for (it = list->Begin(); it != list->End(); ++it) {
        (*it)->OnActivityStop();
    }
    return NOERROR;
}

ECode CPreferenceManager::RegisterOnActivityDestroyListener(
    /* [in] */ IPreferenceManagerOnActivityDestroyListener* listener)
{
    Mutex::Autolock lock(mLock);
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

ECode CPreferenceManager::UnregisterOnActivityDestroyListener(
    /* [in] */ IPreferenceManagerOnActivityDestroyListener* listener)
{
    Mutex::Autolock lock(mLock);
    if (mActivityDestroyListeners != NULL) {
        mActivityDestroyListeners->Remove(listener);
    }
    return NOERROR;
}

ECode CPreferenceManager::DispatchActivityDestroy()
{
    AutoPtr<List<AutoPtr<IPreferenceManagerOnActivityDestroyListener> > > list;

    {
        Mutex::Autolock lock(mLock);
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

ECode CPreferenceManager::GetNextRequestCode(
    /* [out] */ Int32* code)
{
    VALIDATE_NOT_NULL(code)
    Mutex::Autolock lock(mLock);
    *code = mNextRequestCode++;
    return NOERROR;
}

ECode CPreferenceManager::AddPreferencesScreen(
    /* [in] */ IDialogInterface* screen)
{
    Mutex::Autolock lock(mLock);
    if (mPreferencesScreens == NULL) {
        mPreferencesScreens = new List<AutoPtr<IDialogInterface> >();
    }

    mPreferencesScreens->PushBack(screen);
    return NOERROR;
}

ECode CPreferenceManager::RemovePreferencesScreen(
    /* [in] */ IDialogInterface* screen)
{
    Mutex::Autolock lock(mLock);

    if (mPreferencesScreens == NULL) {
        return NOERROR;
    }

    mPreferencesScreens->Remove(screen);
    return NOERROR;
}

ECode CPreferenceManager::DispatchNewIntent(
    /* [in] */ IIntent* intent)
{
    DismissAllScreens();
    return NOERROR;
}

void CPreferenceManager::DismissAllScreens()
{
     // Remove any of the previously shown preferences screens
    AutoPtr<List<AutoPtr<IDialogInterface> > > screensToDismiss;

    {
        Mutex::Autolock lock(mLock);
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

ECode CPreferenceManager::SetOnPreferenceTreeClickListener(
    /* [in] */ IPreferenceManagerOnPreferenceTreeClickListener* listener)
{
    mOnPreferenceTreeClickListener = listener;
    return NOERROR;
}

ECode CPreferenceManager::GetOnPreferenceTreeClickListener(
    /* [out] */ IPreferenceManagerOnPreferenceTreeClickListener** listener)
{
    VALIDATE_NOT_NULL(listener)
    *listener = mOnPreferenceTreeClickListener;
    REFCOUNT_ADD(*listener)
    return NOERROR;
}

ECode CPreferenceManager::constructor(
    /* [in] */ IActivity* activity,
    /* [in] */ Int32 firstRequestCode)
{
    mActivity = activity;
    mNextRequestCode = firstRequestCode;

    Init(activity);
    return NOERROR;
}

ECode CPreferenceManager::constructor(
    /* [in] */ IContext* context)
{
    Init(context);
    return NOERROR;
}

}
}
}
