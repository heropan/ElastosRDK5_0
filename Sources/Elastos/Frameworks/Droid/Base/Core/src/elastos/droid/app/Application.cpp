
#include "elastos/droid/app/Application.h"
#ifdef DROID_CORE
#else
#include "Elastos.Droid.Core.h"
#endif
#include <elastos/utility/logging/Slogger.h>

using Elastos::Utility::Logging::Slogger;
using Elastos::Droid::Content::EIID_IContext;
using Elastos::Droid::Content::EIID_IContextWrapper;
using Elastos::Droid::Content::EIID_IComponentCallbacks2;

namespace Elastos {
namespace Droid {
namespace App {

const String Application::TAG("Application");

Application::Application()
{}

Application::~Application()
{}

ECode Application::Initialize()
{
    return NOERROR;
}

PInterface Application::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)(IApplication*)this;
    }
    else if (riid == EIID_IObject) {
        return (IObject*)this;
    }
    else if (riid == EIID_IContext) {
        return (IContext*)this;
    }
    else if (riid == EIID_IContextWrapper) {
        return (IContextWrapper*)this;
    }
    else if (riid == EIID_IApplication) {
        return (IApplication*)this;
    }
    else if (riid == EIID_IComponentCallbacks2) {
        return (IComponentCallbacks2*)this;
    }
    else if (riid == EIID_IWeakReferenceSource) {
        return (IWeakReferenceSource*)this;
    }
    return NULL;
}

UInt32 Application::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 Application::Release()
{
    return ElRefBase::Release();
}

ECode Application::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID)

    if (pObject == (IInterface*)(IObject*)this) {
        *pIID = EIID_IObject;
        return NOERROR;
    }
    else if (pObject == (IInterface*)(IContext*)this) {
        *pIID = EIID_IContext;
        return NOERROR;
    }
    else if (pObject == (IInterface*)(IContextWrapper*)this) {
        *pIID = EIID_IContextWrapper;
        return NOERROR;
    }
    else if (pObject == (IInterface*)(IApplication*)this) {
        *pIID = EIID_IApplication;
        return NOERROR;
    }
    else if (pObject == (IInterface*)(IComponentCallbacks2*)this) {
        *pIID = EIID_IComponentCallbacks2;
        return NOERROR;
    }
    else if (pObject == (IInterface*)(IWeakReferenceSource*)this) {
        *pIID = EIID_IWeakReferenceSource;
        return NOERROR;
    }

    return E_INVALID_ARGUMENT;
}

ECode Application::Aggregate(
    /* [in] */ AggrType aggrType,
    /* [in] */ PInterface pObject)
{
    return NOERROR;
}

ECode Application::GetDomain(
    /* [out] */ PInterface *ppObject)
{
    return NOERROR;
}

ECode Application::GetClassID(
    /* [out] */ ClassID *pCLSID)
{
    return NOERROR;
}

ECode Application::Equals(
    /* [in] */ IInterface* other,
    /* [out] */ Boolean * result)
{
    VALIDATE_NOT_NULL(result)
    *result = (IApplication::Probe(other) == THIS_PROBE(IApplication));
    return NOERROR;
}

ECode Application::GetHashCode(
    /* [out] */ Int32* hash)
{
    VALIDATE_NOT_NULL(hash)
    *hash = (Int32)THIS_PROBE(IApplication);
    return NOERROR;
}

ECode Application::ToString(
    /* [out] */ String* str)
{
    return NOERROR;
}

ECode Application::GetWeakReference(
    /* [out] */ IWeakReference** weakReference)
{
    VALIDATE_NOT_NULL(weakReference)
    *weakReference = new WeakReferenceImpl(THIS_PROBE(IInterface), CreateWeak(this));
    REFCOUNT_ADD(*weakReference)
    return NOERROR;
}

ECode Application::OnCreate()
{
    return NOERROR;
}

ECode Application::OnTerminate()
{
    return NOERROR;
}

ECode Application::OnConfigurationChanged(
    /* [in] */ IConfiguration* newConfig)
{
    AutoPtr< ArrayOf<IComponentCallbacks*> > callbacks = CollectComponentCallbacks();
    if (callbacks != NULL) {
        for (Int32 i = 0; i < callbacks->GetLength(); i++) {
            (*callbacks)[i]->OnConfigurationChanged(newConfig);
        }
    }
    return NOERROR;
}

ECode Application::OnLowMemory()
{
    AutoPtr< ArrayOf<IComponentCallbacks*> > callbacks = CollectComponentCallbacks();
    if (callbacks != NULL) {
        for (Int32 i = 0; i < callbacks->GetLength(); i++) {
            (*callbacks)[i]->OnLowMemory();
        }
    }
    return NOERROR;
}

ECode Application::OnTrimMemory(
    /* [in] */ Int32 level)
{
    AutoPtr< ArrayOf<IComponentCallbacks*> > callbacks = CollectComponentCallbacks();
    if (callbacks != NULL) {
        for (Int32 i = 0; i < callbacks->GetLength(); i++) {
            IComponentCallbacks2* cb2 = IComponentCallbacks2::Probe((*callbacks)[i]);
            if (cb2 != NULL) {
                cb2->OnTrimMemory(level);
            }
        }
    }
    return NOERROR;
}

ECode Application::RegisterComponentCallbacks(
    /* [in] */ IComponentCallbacks* componentCallback)
{
    if (componentCallback) {
        AutoLock lock(mComponentCallbacksLock);
        mComponentCallbacks.PushBack(componentCallback);
    }
    return NOERROR;
}

ECode Application::UnregisterComponentCallbacks(
    /* [in] */ IComponentCallbacks* componentCallback)
{
    if (componentCallback) {
        AutoLock lock(mComponentCallbacksLock);
        mComponentCallbacks.Remove(componentCallback);
    }
    return NOERROR;
}

ECode Application::RegisterActivityLifecycleCallbacks(
    /* [in] */ IActivityLifecycleCallbacks* cb)
{
    if (cb) {
        AutoLock lock(mActivityLifecycleCallbacksLock);
        mActivityLifecycleCallbacks.PushBack(cb);
    }
    return NOERROR;
}

ECode Application::UnregisterActivityLifecycleCallbacks(
    /* [in] */ IActivityLifecycleCallbacks* cb)
{
    if (cb) {
        AutoLock lock(mActivityLifecycleCallbacksLock);
        mActivityLifecycleCallbacks.Remove(cb);
    }
    return NOERROR;
}

ECode Application::Attach(
    /* [in] */ IContext* ctx)
{
    FAIL_RETURN(AttachBaseContext(ctx))
    // assert(0);
    // mLoadedApk = ContextImpl.getImpl(context).mPackageInfo;
    return NOERROR;
}

ECode Application::DispatchActivityCreated(
    /* [in] */ IActivity* activity,
    /* [in] */ IBundle* savedInstanceState)
{
    AutoPtr< ArrayOf<IActivityLifecycleCallbacks*> > callbacks = CollectActivityLifecycleCallbacks();
    if (callbacks != NULL) {
        for (Int32 i = 0; i < callbacks->GetLength(); i++) {
            (*callbacks)[i]->OnActivityCreated(activity, savedInstanceState);
        }
    }
    return NOERROR;
}

ECode Application::DispatchActivityStarted(
    /* [in] */ IActivity* activity)
{
    AutoPtr< ArrayOf<IActivityLifecycleCallbacks*> > callbacks = CollectActivityLifecycleCallbacks();
    if (callbacks != NULL) {
        for (Int32 i = 0; i < callbacks->GetLength(); i++) {
            (*callbacks)[i]->OnActivityStarted(activity);
        }
    }
    return NOERROR;
}

ECode Application::DispatchActivityResumed(
    /* [in] */ IActivity* activity)
{
    AutoPtr< ArrayOf<IActivityLifecycleCallbacks*> > callbacks = CollectActivityLifecycleCallbacks();
    if (callbacks != NULL) {
        for (Int32 i = 0; i < callbacks->GetLength(); i++) {
            (*callbacks)[i]->OnActivityResumed(activity);
        }
    }
    return NOERROR;
}

ECode Application::DispatchActivityPaused(
    /* [in] */ IActivity* activity)
{
    AutoPtr< ArrayOf<IActivityLifecycleCallbacks*> > callbacks = CollectActivityLifecycleCallbacks();
    if (callbacks != NULL) {
        for (Int32 i = 0; i < callbacks->GetLength(); i++) {
            (*callbacks)[i]->OnActivityPaused(activity);
        }
    }
    return NOERROR;
}

ECode Application::DispatchActivityStopped(
    /* [in] */ IActivity* activity)
{
    AutoPtr< ArrayOf<IActivityLifecycleCallbacks*> > callbacks = CollectActivityLifecycleCallbacks();
    if (callbacks != NULL) {
        for (Int32 i = 0; i < callbacks->GetLength(); i++) {
            (*callbacks)[i]->OnActivityStopped(activity);
        }
    }
    return NOERROR;
}

ECode Application::DispatchActivitySaveInstanceState(
    /* [in] */ IActivity* activity,
    /* [in] */ IBundle* outState)
{
    AutoPtr< ArrayOf<IActivityLifecycleCallbacks*> > callbacks = CollectActivityLifecycleCallbacks();
    if (callbacks != NULL) {
        for (Int32 i = 0; i < callbacks->GetLength(); i++) {
            (*callbacks)[i]->OnActivitySaveInstanceState(activity, outState);
        }
    }
    return NOERROR;
}

ECode Application::DispatchActivityDestroyed(
    /* [in] */ IActivity* activity)
{
    AutoPtr< ArrayOf<IActivityLifecycleCallbacks*> > callbacks = CollectActivityLifecycleCallbacks();
    if (callbacks != NULL) {
        for (Int32 i = 0; i < callbacks->GetLength(); i++) {
            (*callbacks)[i]->OnActivityDestroyed(activity);
        }
    }
    return NOERROR;
}

AutoPtr< ArrayOf<IComponentCallbacks*> > Application::CollectComponentCallbacks()
{
    AutoPtr< ArrayOf<IComponentCallbacks*> > callbacks;
    {
        AutoLock lock(mComponentCallbacksLock);

        Int32 size = mComponentCallbacks.GetSize();
        if (size > 0) {
            callbacks = ArrayOf<IComponentCallbacks*>::Alloc(size);
            List< AutoPtr<IComponentCallbacks> >::Iterator it;
            Int32 i;
            for (it = mComponentCallbacks.Begin(), i = 0; it != mComponentCallbacks.End(); ++it, ++i) {
                callbacks->Set(i, *it);
            }
        }
    }
    return callbacks;
}

AutoPtr< ArrayOf<IActivityLifecycleCallbacks*> > Application::CollectActivityLifecycleCallbacks()
{
    AutoPtr< ArrayOf<IActivityLifecycleCallbacks*> > callbacks;
    {
        AutoLock lock(mActivityLifecycleCallbacksLock);

        Int32 size = mActivityLifecycleCallbacks.GetSize();
        if (size > 0) {
            callbacks = ArrayOf<IActivityLifecycleCallbacks*>::Alloc(size);
            List< AutoPtr<IActivityLifecycleCallbacks> >::Iterator it;
            Int32 i;
            for (it = mActivityLifecycleCallbacks.Begin(), i = 0; it != mActivityLifecycleCallbacks.End(); ++it, ++i) {
                callbacks->Set(i, *it);
            }
        }

    }
    return callbacks;
}

ECode Application::GetBaseContext(
    /* [out] */ IContext** context)
{
    VALIDATE_NOT_NULL(context)
    *context = mBase;
    REFCOUNT_ADD(*context)
    return NOERROR;
}

ECode Application::GetAssets(
    /* [out] */ IAssetManager** assetManager)
{
    VALIDATE_NOT_NULL(assetManager)
    return mBase->GetAssets(assetManager);
}

ECode Application::GetResources(
    /* [out] */ IResources** resources)
{
    VALIDATE_NOT_NULL(resources)
    return mBase->GetResources(resources);
}

ECode Application::GetPackageManager(
    /* [out] */ IPackageManager** packageManager)
{
    VALIDATE_NOT_NULL(packageManager)
    return mBase->GetPackageManager(packageManager);
}

ECode Application::GetContentResolver(
    /* [out] */ IContentResolver** resolver)
{
    VALIDATE_NOT_NULL(resolver)
    return mBase->GetContentResolver(resolver);
}

ECode Application::GetMainLooper(
    /* [out] */ ILooper** looper)
{
    VALIDATE_NOT_NULL(looper)
    return mBase->GetMainLooper(looper);
}

ECode Application::GetApplicationContext(
    /* [out] */ IContext** ctx)
{
    VALIDATE_NOT_NULL(ctx)
    return mBase->GetApplicationContext(ctx);
}

ECode Application::GetText(
    /* [in] */ Int32 resId,
    /* [out] */ ICharSequence** text)
{
    VALIDATE_NOT_NULL(text)
    *text = NULL;
    AutoPtr<IResources> resources;
    FAIL_RETURN(GetResources((IResources**)&resources));
    return resources->GetText(resId, text);
}

ECode Application::GetString(
    /* [in] */ Int32 resId,
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str)
    *str = String(NULL);
    AutoPtr<IResources> resources;
    FAIL_RETURN(GetResources((IResources**)&resources));
    return resources->GetString(resId, str);
}

ECode Application::GetString(
    /* [in] */ Int32 resId,
    /* [in] */ ArrayOf<IInterface*>* formatArgs,
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str)
    *str = String(NULL);
    AutoPtr<IResources> resources;
    FAIL_RETURN(GetResources((IResources**)&resources));
    return resources->GetString(resId, formatArgs, str);
}

ECode Application::SetTheme(
    /* [in] */ Int32 resId)
{
    return mBase->SetTheme(resId);
}

ECode Application::GetThemeResId(
    /* [out] */ Int32* resId)
{
    VALIDATE_NOT_NULL(resId)
    return mBase->GetThemeResId(resId);
}

ECode Application::GetTheme(
    /* [out] */ IResourcesTheme** theme)
{
    VALIDATE_NOT_NULL(theme)
    return mBase->GetTheme(theme);
}

ECode Application::ObtainStyledAttributes(
    /* [in] */ ArrayOf<Int32>* attrs,
    /* [out] */ ITypedArray** styles)
{
    VALIDATE_NOT_NULL(styles)
    *styles = NULL;
    AutoPtr<IResourcesTheme> theme;
    FAIL_RETURN(GetTheme((IResourcesTheme**)&theme));
    return theme->ObtainStyledAttributes(attrs, styles);
}

ECode Application::ObtainStyledAttributes(
    /* [in] */ Int32 resid,
    /* [in] */ ArrayOf<Int32>* attrs,
    /* [out] */ ITypedArray** styles)
{
    VALIDATE_NOT_NULL(styles)
    *styles = NULL;
    AutoPtr<IResourcesTheme> theme;
    FAIL_RETURN(GetTheme((IResourcesTheme**)&theme));
    return theme->ObtainStyledAttributes(resid, attrs, styles);
}

ECode Application::ObtainStyledAttributes(
    /* [in] */ IAttributeSet* set,
    /* [in] */ ArrayOf<Int32>* attrs,
    /* [out] */ ITypedArray** styles)
{
    VALIDATE_NOT_NULL(styles)
    *styles = NULL;
    AutoPtr<IResourcesTheme> theme;
    FAIL_RETURN(GetTheme((IResourcesTheme**)&theme));
    return theme->ObtainStyledAttributes(set, attrs, 0, 0, styles);
}

ECode Application::ObtainStyledAttributes(
    /* [in] */ IAttributeSet* set,
    /* [in] */ ArrayOf<Int32>* attrs,
    /* [in] */ Int32 defStyleAttr,
    /* [in] */ Int32 defStyleRes,
    /* [out] */ ITypedArray** styles)
{
    VALIDATE_NOT_NULL(styles)
    *styles = NULL;
    AutoPtr<IResourcesTheme> theme;
    FAIL_RETURN(GetTheme((IResourcesTheme**)&theme));
    return theme->ObtainStyledAttributes(set, attrs, defStyleAttr, defStyleRes, styles);
}

ECode Application::GetClassLoader(
    /* [out] */ IClassLoader** loader)
{
    VALIDATE_NOT_NULL(loader)
    return mBase->GetClassLoader(loader);
}

ECode Application::GetPackageName(
    /* [out] */ String* packageName)
{
    VALIDATE_NOT_NULL(packageName)
    return mBase->GetPackageName(packageName);
}

ECode Application::GetApplicationInfo(
    /* [out] */ IApplicationInfo** info)
{
    VALIDATE_NOT_NULL(info)
    return mBase->GetApplicationInfo(info);
}

ECode Application::GetPackageResourcePath(
    /* [out] */ String* path)
{
    VALIDATE_NOT_NULL(path)
    return mBase->GetPackageResourcePath(path);
}

ECode Application::GetPackageCodePath(
    /* [out] */ String* codePath)
{
    VALIDATE_NOT_NULL(codePath)
    return mBase->GetPackageCodePath(codePath);
}

ECode Application::GetSharedPrefsFile(
    /* [in] */ const String& name,
    /* [out] */IFile** file)
{
    VALIDATE_NOT_NULL(file)
    return mBase->GetSharedPrefsFile(name, file);
}

ECode Application::GetSharedPreferences(
    /* [in] */ const String& name,
    /* [in] */ Int32 mode,
    /* [out] */ ISharedPreferences** prefs)
{
    VALIDATE_NOT_NULL(prefs)
    return mBase->GetSharedPreferences(name, mode, prefs);
}

ECode Application::OpenFileInput(
    /* [in] */ const String& name,
    /* [out] */ IFileInputStream** fileInputStream)
{
    VALIDATE_NOT_NULL(fileInputStream)
    return mBase->OpenFileInput(name, fileInputStream);
}

ECode Application::OpenFileOutput(
    /* [in] */ const String& name,
    /* [in] */ Int32 mode,
    /* [out] */ IFileOutputStream** fileOutputStream)
{
    VALIDATE_NOT_NULL(fileOutputStream)
    return mBase->OpenFileOutput(name, mode, fileOutputStream);
}

ECode Application::DeleteFile(
    /* [in] */ const String& name,
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded)
    return mBase->DeleteFile(name, succeeded);
}

ECode Application::GetFileStreamPath(
    /* [in] */ const String& name,
    /* [out] */ IFile** file)
{
    VALIDATE_NOT_NULL(file)
    return mBase->GetFileStreamPath(name, file);
}

ECode Application::GetFilesDir(
    /* [out] */ IFile** filesDir)
{
    VALIDATE_NOT_NULL(filesDir)
    return mBase->GetFilesDir(filesDir);
}

ECode Application::GetExternalFilesDir(
    /* [in] */ const String& type,
    /* [out] */ IFile** filesDir)
{
    VALIDATE_NOT_NULL(filesDir)
    return mBase->GetExternalFilesDir(type, filesDir);
}

ECode Application::GetObbDir(
    /* [out] */ IFile** obbDir)
{
    VALIDATE_NOT_NULL(obbDir)
    return mBase->GetObbDir(obbDir);
}

ECode Application::GetCacheDir(
    /* [out] */ IFile** cacheDir)
{
    VALIDATE_NOT_NULL(cacheDir)
    return mBase->GetCacheDir(cacheDir);
}

ECode Application::GetExternalCacheDir(
    /* [out] */ IFile** externalDir)
{
    VALIDATE_NOT_NULL(externalDir)
    return mBase->GetExternalCacheDir(externalDir);
}

ECode Application::GetFileList(
    /* [out, callee] */ ArrayOf<String>** fileList)
{
    VALIDATE_NOT_NULL(fileList)
    return mBase->GetFileList(fileList);
}

ECode Application::GetDir(
    /* [in] */ const String& name,
    /* [in] */ Int32 mode,
    /* [out] */ IFile** dir)
{
    VALIDATE_NOT_NULL(dir)
    return mBase->GetDir(name, mode, dir);
}

ECode Application::OpenOrCreateDatabase(
    /* [in] */ const String& name,
    /* [in] */ Int32 mode,
    /* [in] */ ISQLiteDatabaseCursorFactory* factory,
    /* [out] */ ISQLiteDatabase** sqliteDB)
{
    VALIDATE_NOT_NULL(sqliteDB)
    return mBase->OpenOrCreateDatabase(name, mode, factory, sqliteDB);
}

ECode Application::OpenOrCreateDatabase(
    /* [in] */ const String& name,
    /* [in] */ Int32 mode,
    /* [in] */ ISQLiteDatabaseCursorFactory* factory,
    /* [in] */ IDatabaseErrorHandler* errorHandler,
    /* [out] */ ISQLiteDatabase** sqliteDB)
{
    VALIDATE_NOT_NULL(sqliteDB)
    return mBase->OpenOrCreateDatabase(name, mode, factory, errorHandler, sqliteDB);
}

ECode Application::DeleteDatabase(
    /* [in] */ const String& name,
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded)
    return mBase->DeleteDatabase(name, succeeded);
}

ECode Application::GetDatabasePath(
    /* [in] */ const String& name,
    /* [out] */ IFile** path)
{
    VALIDATE_NOT_NULL(path)
    return mBase->GetDatabasePath(name, path);
}

ECode Application::GetDatabaseList(
    /* [out, callee] */ ArrayOf<String>** databaseList)
{
    VALIDATE_NOT_NULL(databaseList)
    return mBase->GetDatabaseList(databaseList);
}

ECode Application::GetWallpaper(
    /* [out] */ IDrawable** drawable)
{
    VALIDATE_NOT_NULL(drawable)
    return mBase->GetWallpaper(drawable);
}

ECode Application::PeekWallpaper(
    /* [out] */ IDrawable** drawable)
{
    VALIDATE_NOT_NULL(drawable)
    return mBase->PeekWallpaper(drawable);
}

ECode Application::GetWallpaperDesiredMinimumWidth(
    /* [out] */ Int32* minWidth)
{
    VALIDATE_NOT_NULL(minWidth)
    return mBase->GetWallpaperDesiredMinimumWidth(minWidth);
}

ECode Application::GetWallpaperDesiredMinimumHeight(
    /* [out] */ Int32* minHeight)
{
    VALIDATE_NOT_NULL(minHeight)
    return mBase->GetWallpaperDesiredMinimumHeight(minHeight);
}

ECode Application::SetWallpaper(
    /* [in] */ IBitmap* bitmap)
{
    return mBase->SetWallpaper(bitmap);
}

ECode Application::SetWallpaper(
    /* [in] */ IInputStream* data)
{
    return mBase->SetWallpaper(data);
}

ECode Application::ClearWallpaper()
{
    return mBase->ClearWallpaper();
}

ECode Application::StartActivity(
    /* [in] */ IIntent* intent)
{
    return mBase->StartActivity(intent);
}

ECode Application::StartActivityAsUser(
    /* [in] */ IIntent* intent,
    /* [in] */ IUserHandle* user)
{
    return mBase->StartActivityAsUser(intent, user);
}

ECode Application::StartActivity(
    /* [in] */ IIntent* intent,
    /* [in] */ IBundle* options)
{
    return mBase->StartActivity(intent, options);
}

ECode Application::StartActivityAsUser(
    /* [in] */ IIntent* intent,
    /* [in] */ IBundle* options,
    /* [in] */ IUserHandle* userId)
{
    return mBase->StartActivityAsUser(intent, options, userId);
}

ECode Application::StartActivities(
    /* [in] */ ArrayOf<IIntent*>* intents)
{
    return mBase->StartActivities(intents);
}

ECode Application::StartActivities(
    /* [in] */ ArrayOf<IIntent*>* intents,
    /* [in] */ IBundle* options)
{
    return mBase->StartActivities(intents, options);
}

ECode Application::StartActivitiesAsUser(
    /* [in] */ ArrayOf<IIntent*>* intents,
    /* [in] */ IBundle* options,
    /* [in] */ IUserHandle* userHandle)
{
    return mBase->StartActivitiesAsUser(intents, options, userHandle);
}

ECode Application::StartIntentSender(
    /* [in] */ IIntentSender* intent,
    /* [in] */ IIntent* fillInIntent,
    /* [in] */ Int32 flagsMask,
    /* [in] */ Int32 flagsValues,
    /* [in] */ Int32 extraFlags)
{
    return mBase->StartIntentSender(intent, fillInIntent, flagsMask, flagsValues, extraFlags);
}

ECode Application::StartIntentSender(
    /* [in] */ IIntentSender* intent,
    /* [in] */ IIntent* fillInIntent,
    /* [in] */ Int32 flagsMask,
    /* [in] */ Int32 flagsValues,
    /* [in] */ Int32 extraFlags,
    /* [in] */ IBundle* options)
{
    return mBase->StartIntentSender(intent, fillInIntent, flagsMask, flagsValues, extraFlags, options);
}

ECode Application::SendBroadcast(
    /* [in] */ IIntent* intent)
{
    return mBase->SendBroadcast(intent);
}

ECode Application::SendBroadcast(
    /* [in] */ IIntent* intent,
    /* [in] */ const String& receiverPermission)
{
    return mBase->SendBroadcast(intent, receiverPermission);
}

ECode Application::SendOrderedBroadcast(
    /* [in] */ IIntent* intent,
    /* [in] */ const String& receiverPermission)
{
    return mBase->SendOrderedBroadcast(intent, receiverPermission);
}

ECode Application::SendOrderedBroadcast(
    /* [in] */ IIntent* intent,
    /* [in] */ const String& receiverPermission,
    /* [in] */ IBroadcastReceiver* resultReceiver,
    /* [in] */ IHandler* scheduler,
    /* [in] */ Int32 initialCode,
    /* [in] */ const String& initialData,
    /* [in] */ IBundle* initialExtras)
{
    return mBase->SendOrderedBroadcast(intent, receiverPermission, resultReceiver, scheduler,
        initialCode, initialData, initialExtras);
}

ECode Application::SendBroadcastAsUser(
    /* [in] */ IIntent* intent,
    /* [in] */ IUserHandle* user)
{
    return mBase->SendBroadcastAsUser(intent, user);
}

ECode Application::SendBroadcastAsUser(
    /* [in] */ IIntent* intent,
    /* [in] */ IUserHandle* user,
    /* [in] */ const String& receiverPermission)
{
    return mBase->SendBroadcastAsUser(intent, user, receiverPermission);
}

ECode Application::SendOrderedBroadcastAsUser(
    /* [in] */ IIntent* intent,
    /* [in] */ IUserHandle* user,
    /* [in] */ const String& receiverPermission,
    /* [in] */ IBroadcastReceiver* resultReceiver,
    /* [in] */ IHandler* scheduler,
    /* [in] */ Int32 initialCode,
    /* [in] */ const String& initialData,
    /* [in] */ IBundle* initialExtras)
{
    return mBase->SendOrderedBroadcastAsUser(intent, user, receiverPermission, resultReceiver, scheduler,
        initialCode, initialData, initialExtras);
}

ECode Application::SendStickyBroadcast(
    /* [in] */ IIntent* intent)
{
    return mBase->SendStickyBroadcast(intent);
}

ECode Application::SendStickyOrderedBroadcast(
    /* [in] */ IIntent* intent,
    /* [in] */ IBroadcastReceiver* resultReceiver,
    /* [in] */ IHandler* scheduler,
    /* [in] */ Int32 initialCode,
    /* [in] */ const String& initialData,
    /* [in] */ IBundle* initialExtras)
{
    return mBase->SendStickyOrderedBroadcast(intent, resultReceiver, scheduler, initialCode, initialData,
        initialExtras);
}

ECode Application::RemoveStickyBroadcast(
    /* [in] */ IIntent* intent)
{
    return mBase->RemoveStickyBroadcast(intent);
}

ECode Application::SendStickyBroadcastAsUser(
    /* [in] */ IIntent* intent,
    /* [in] */ IUserHandle* user)
{
    return mBase->SendStickyBroadcastAsUser(intent, user);
}

ECode Application::SendStickyOrderedBroadcastAsUser(
    /* [in] */ IIntent* intent,
    /* [in] */ IUserHandle* user,
    /* [in] */ IBroadcastReceiver* resultReceiver,
    /* [in] */ IHandler* scheduler,
    /* [in] */ Int32 initialCode,
    /* [in] */ const String& initialData,
    /* [in] */ IBundle* initialExtras)
{
    return mBase->SendStickyOrderedBroadcastAsUser(intent, user, resultReceiver, scheduler, initialCode,
        initialData, initialExtras);
}

ECode Application::RemoveStickyBroadcastAsUser(
    /* [in] */ IIntent* intent,
    /* [in] */ IUserHandle* user)
{
    return mBase->RemoveStickyBroadcastAsUser(intent, user);
}

ECode Application::RegisterReceiver(
    /* [in] */ IBroadcastReceiver* receiver,
    /* [in] */ IIntentFilter* filter,
    /* [out] */ IIntent** stickyIntent)
{
    VALIDATE_NOT_NULL(stickyIntent)
    return mBase->RegisterReceiver(receiver, filter, stickyIntent);
}

ECode Application::RegisterReceiver(
    /* [in] */ IBroadcastReceiver* receiver,
    /* [in] */ IIntentFilter* filter,
    /* [in] */ const String& broadcastPermission,
    /* [in] */ IHandler* scheduler,
    /* [out] */ IIntent** stickyIntent)
{
    VALIDATE_NOT_NULL(stickyIntent)
    return mBase->RegisterReceiver(receiver, filter, broadcastPermission, scheduler, stickyIntent);
}

ECode Application::RegisterReceiverAsUser(
    /* [in] */ IBroadcastReceiver* receiver,
    /* [in] */ IUserHandle* user,
    /* [in] */ IIntentFilter* filter,
    /* [in] */ const String& broadcastPermission,
    /* [in] */ IHandler* scheduler,
    /* [out] */ IIntent** stickyIntent)
{
    VALIDATE_NOT_NULL(stickyIntent)
    return mBase->RegisterReceiverAsUser(receiver, user, filter, broadcastPermission, scheduler, stickyIntent);
}

ECode Application::UnregisterReceiver(
    /* [in] */ IBroadcastReceiver* receiver)
{
    return mBase->UnregisterReceiver(receiver);
}

ECode Application::StartService(
    /* [in] */ IIntent* service,
    /* [out] */ IComponentName** name)
{
    VALIDATE_NOT_NULL(name)
    return mBase->StartService(service, name);
}

ECode Application::StopService(
    /* [in] */ IIntent* service,
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded)
    return mBase->StopService(service, succeeded);
}

ECode Application::StartServiceAsUser(
    /* [in] */ IIntent* service,
    /* [in] */ IUserHandle* user,
    /* [out] */ IComponentName** name)
{
    VALIDATE_NOT_NULL(name)
    return mBase->StartServiceAsUser(service, user, name);
}

ECode Application::StopServiceAsUser(
    /* [in] */ IIntent* service,
    /* [in] */ IUserHandle* user,
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded)
    return mBase->StopServiceAsUser(service, user, succeeded);
}

ECode Application::BindService(
    /* [in] */ IIntent* service,
    /* [in] */ IServiceConnection* conn,
    /* [in] */ Int32 flags,
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded)
    return mBase->BindService(service, conn, flags, succeeded);
}

ECode Application::BindService(
    /* [in] */ IIntent* service,
    /* [in] */ IServiceConnection* conn,
    /* [in] */ Int32 flags,
    /* [in] */ Int32 userHandle,
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded)
    return mBase->BindService(service, conn, flags, userHandle, succeeded);
}

ECode Application::UnbindService(
    /* [in] */ IServiceConnection* conn)
{
    return mBase->UnbindService(conn);
}

ECode Application::StartInstrumentation(
    /* [in] */ IComponentName* className,
    /* [in] */ const String& profileFile,
    /* [in] */ IBundle* arguments,
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded)
    return mBase->StartInstrumentation(className, profileFile, arguments, succeeded);
}

ECode Application::GetSystemService(
    /* [in] */ const String& name,
    /* [out] */ IInterface** object)
{
    VALIDATE_NOT_NULL(object)
    return mBase->GetSystemService(name, object);
}

ECode Application::CheckPermission(
    /* [in] */ const String& permission,
    /* [in] */ Int32 pid,
    /* [in] */ Int32 uid,
    /* [out] */ Int32* permissionId)
{
    VALIDATE_NOT_NULL(permissionId)
    return mBase->CheckPermission(permission, pid, uid, permissionId);
}

ECode Application::CheckCallingPermission(
    /* [in] */ const String& permission,
    /* [out] */ Int32* permissionId)
{
    VALIDATE_NOT_NULL(permissionId)
    return mBase->CheckCallingPermission(permission, permissionId);
}

ECode Application::CheckCallingOrSelfPermission(
    /* [in] */ const String& permission,
    /* [out] */ Int32* permissionId)
{
    VALIDATE_NOT_NULL(permissionId)
    return mBase->CheckCallingOrSelfPermission(permission, permissionId);
}

ECode Application::EnforcePermission(
    /* [in] */ const String& permission,
    /* [in] */ Int32 pid,
    /* [in] */ Int32 uid,
    /* [in] */ const String& message)
{
    return mBase->EnforcePermission(permission, pid, uid, message);
}

ECode Application::EnforceCallingPermission(
    /* [in] */ const String& permission,
    /* [in] */ const String& message)
{
    return mBase->EnforceCallingPermission(permission, message);
}

ECode Application::EnforceCallingOrSelfPermission(
    /* [in] */ const String& permission,
    /* [in] */ const String& message)
{
    return mBase->EnforceCallingOrSelfPermission(permission, message);
}

ECode Application::GrantUriPermission(
    /* [in] */ const String& toPackage,
    /* [in] */ IUri* uri,
    /* [in] */ Int32 modeFlags)
{
    return mBase->GrantUriPermission(toPackage, uri, modeFlags);
}

ECode Application::RevokeUriPermission(
    /* [in] */ IUri* uri,
    /* [in] */ Int32 modeFlags)
{
    return mBase->RevokeUriPermission(uri, modeFlags);
}

ECode Application::CheckUriPermission(
    /* [in] */ IUri* uri,
    /* [in] */ Int32 pid,
    /* [in] */ Int32 uid,
    /* [in] */ Int32 modeFlags,
    /* [out] */ Int32* permissionId)
{
    VALIDATE_NOT_NULL(permissionId)
    return mBase->CheckUriPermission(uri, pid, uid, modeFlags, permissionId);
}

ECode Application::CheckCallingUriPermission(
    /* [in] */ IUri* uri,
    /* [in] */ Int32 modeFlags,
    /* [out] */ Int32* permissionId)
{
    VALIDATE_NOT_NULL(permissionId)
    return mBase->CheckCallingUriPermission(uri, modeFlags, permissionId);
}

ECode Application::CheckCallingOrSelfUriPermission(
    /* [in] */ IUri* uri,
    /* [in] */ Int32 modeFlags,
    /* [out] */ Int32* permissionId)
{
    VALIDATE_NOT_NULL(permissionId)
    return mBase->CheckCallingOrSelfUriPermission(uri, modeFlags, permissionId);
}

ECode Application::CheckUriPermission(
    /* [in] */ IUri* uri,
    /* [in] */ const String& readPermission,
    /* [in] */ const String& writePermission,
    /* [in] */ Int32 pid,
    /* [in] */ Int32 uid,
    /* [in] */ Int32 modeFlags,
    /* [out] */ Int32* permissionId)
{
    VALIDATE_NOT_NULL(permissionId)
    return mBase->CheckUriPermission(uri, readPermission, writePermission, pid, uid, modeFlags, permissionId);
}

ECode Application::EnforceUriPermission(
    /* [in] */ IUri* uri,
    /* [in] */ Int32 pid,
    /* [in] */ Int32 uid,
    /* [in] */ Int32 modeFlags,
    /* [in] */ const String& message)
{
    return mBase->EnforceUriPermission(uri, pid, uid, modeFlags, message);
}

ECode Application::EnforceCallingUriPermission(
    /* [in] */ IUri* uri,
    /* [in] */ Int32 modeFlags,
    /* [in] */ const String& message)
{
    return mBase->EnforceCallingUriPermission(uri, modeFlags, message);
}

ECode Application::EnforceCallingOrSelfUriPermission(
    /* [in] */ IUri* uri,
    /* [in] */ Int32 modeFlags,
    /* [in] */ const String& message)
{
    return mBase->EnforceCallingOrSelfUriPermission(uri, modeFlags, message);
}

ECode Application::EnforceUriPermission(
    /* [in] */ IUri* uri,
    /* [in] */ const String& readPermission,
    /* [in] */ const String& writePermission,
    /* [in] */ Int32 pid,
    /* [in] */ Int32 uid,
    /* [in] */ Int32 modeFlags,
    /* [in] */ const String& message)
{
    return mBase->EnforceUriPermission(uri, readPermission, writePermission, pid, uid, modeFlags, message);
}

ECode Application::CreatePackageContext(
    /* [in] */ const String& packageName,
    /* [in] */ Int32 flags,
    /* [out] */ IContext** ctx)
{
    VALIDATE_NOT_NULL(ctx)
    return mBase->CreatePackageContext(packageName, flags, ctx);
}

ECode Application::CreatePackageContextAsUser(
    /* [in] */ const String& packageName,
    /* [in] */ Int32 flags,
    /* [in] */ IUserHandle* user,
    /* [out] */ IContext** ctx)
{
    VALIDATE_NOT_NULL(ctx)
    return mBase->CreatePackageContextAsUser(packageName, flags, user, ctx);
}

ECode Application::CreateConfigurationContext(
    /* [in] */ IConfiguration* overrideConfiguration,
    /* [out] */ IContext** ctx)
{
    VALIDATE_NOT_NULL(ctx)
    return mBase->CreateConfigurationContext(overrideConfiguration, ctx);
}

ECode Application::CreateDisplayContext(
    /* [in] */ IDisplay* display,
    /* [out] */ IContext** ctx)
{
    VALIDATE_NOT_NULL(ctx)
    return mBase->CreateDisplayContext(display, ctx);
}

ECode Application::GetCompatibilityInfo(
    /* [in] */ Int32 displayId,
    /* [out] */ ICompatibilityInfoHolder** infoHolder)
{
    VALIDATE_NOT_NULL(infoHolder)
    return mBase->GetCompatibilityInfo(displayId, infoHolder);
}

ECode Application::IsRestricted(
    /* [out] */ Boolean* isRestricted)
{
    VALIDATE_NOT_NULL(isRestricted)
    return mBase->IsRestricted(isRestricted);
}

ECode Application::AttachBaseContext(
    /* [in] */ IContext* base)
{
    if (mBase != NULL) {
        Slogger::E(TAG, "Base context already set");
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    mBase = base;
    return NOERROR;
}

} // namespace App
} // namespace Droid
} // namespace Elastos
