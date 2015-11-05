
#include "elastos/droid/os/Build.h"
#include "elastos/droid/app/Service.h"
#include "elastos/droid/R.h"
#include "elastos/droid/content/CComponentName.h"
#include <elastos/utility/logging/Logger.h>
#include <elastos/core/StringBuilder.h>

using Elastos::Core::StringBuilder;
using Elastos::Utility::Logging::Logger;
using Elastos::Droid::R;
using Elastos::Droid::Os::Build;
using Elastos::Droid::Content::CComponentName;
using Elastos::Droid::Content::EIID_IContext;
using Elastos::Droid::Content::EIID_IContextWrapper;
using Elastos::Droid::Content::EIID_IComponentCallbacks2;

namespace Elastos {
namespace Droid {
namespace App {

static const String TAG = String("Service");

Service::Service()
    : mStartCompatibility(FALSE)
{
}

Service::~Service()
{
}

PInterface Service::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)(IService*)this;
    }
    else if (riid == EIID_IObject) {
        return (IObject*)this;
    }
    if (riid == EIID_IService) {
        return (IService*)this;
    }
    else if (riid == EIID_IContextWrapper) {
       return (IContextWrapper*)this;
    }
    else if (riid == EIID_IContext) {
       return (IContext*)this;
    }
    else if (riid == EIID_IComponentCallbacks2) {
       return (IComponentCallbacks2*)this;
    }
    else if (riid == EIID_IWeakReferenceSource) {
        return (IWeakReferenceSource*)this;
    }

    return NULL;
}

UInt32 Service::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 Service::Release()
{
    return ElRefBase::Release();
}

ECode Service::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    if (NULL == pIID) return E_INVALID_ARGUMENT;

    if (pObject == (IInterface *)(IService *)this) {
        *pIID = EIID_IService;
        return NOERROR;
    }
    else if (pObject == (IInterface *)(IObject *)this) {
        *pIID = EIID_IObject;
        return NOERROR;
    }
    else if (pObject == (IContextWrapper *)(IObject *)this) {
        *pIID = EIID_IContextWrapper;
        return NOERROR;
    }
    else if (pObject == (IContext *)(IObject *)this) {
        *pIID = EIID_IContext;
        return NOERROR;
    }
    else if (pObject == (IComponentCallbacks2 *)(IObject *)this) {
        *pIID = EIID_IComponentCallbacks2;
        return NOERROR;
    }
    else if (pObject == (IInterface *)(IWeakReferenceSource *)this) {
        *pIID = EIID_IWeakReferenceSource;
        return NOERROR;
    }

    return E_INVALID_ARGUMENT;
}

ECode Service::Aggregate(
    /* [in] */ AggrType aggrType,
    /* [in] */ PInterface pObject)
{
    return E_NOT_IMPLEMENTED;
}

ECode Service::GetDomain(
    /* [out] */ PInterface *ppObject)
{
    return E_NOT_IMPLEMENTED;
}

ECode Service::Equals(
    /* [in] */ IInterface* other,
    /* [out] */ Boolean * result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;
    VALIDATE_NOT_NULL(other);

    IService * o = IService::Probe(other);
    if (o != NULL) {
        *result = (o == THIS_PROBE(IService));
    }
    return NOERROR;
}

ECode Service::GetHashCode(
    /* [out] */ Int32* hash)
{
    VALIDATE_NOT_NULL(hash);
    *hash = (Int32)THIS_PROBE(IService);
    return NOERROR;
}

ECode Service::ToString(
    /* [out] */ String* info)
{
    VALIDATE_NOT_NULL(info);
    StringBuilder sb("Service:(");
    sb += (Int32)THIS_PROBE(IService);
    sb += ")";
    sb.ToString(info);
    return NOERROR;
}

ECode Service::Initialize()
{
    return NOERROR;
}

ECode Service::GetApplication(
    /* [out] */ IApplication** application)
{
    VALIDATE_NOT_NULL(application);

    *application = mApplication;
    REFCOUNT_ADD(*application);

    return NOERROR;
}

ECode Service::GetClassID(
    /* [out] */ ClassID *pCLSID)
{
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode Service::OnApplyThemeResource(
    /* [in] */ IResourcesTheme* theme,
    /* [in] */ Int32 resid,
    /* [in] */ Boolean first)
{
    return theme->ApplyStyle(resid, TRUE);
}

ECode Service::InitializeTheme()
{
    Boolean first = mTheme == NULL;
    if (first) {
        AutoPtr<IResources> resources;
        GetResources((IResources**)&resources);
        resources->NewTheme((IResourcesTheme**)&mTheme);
        AutoPtr<IResourcesTheme> theme;
        mBase->GetTheme((IResourcesTheme**)&theme);
        if (theme != NULL) {
            mTheme->SetTo(theme);
        }
    }

    return OnApplyThemeResource(mTheme, mThemeResource, first);
}

ECode Service::OnCreate()
{
    return NOERROR;
}

ECode Service::OnStart(
    /* [in] */ IIntent* intent,
    /* [in] */ Int32 startId)
{
    return NOERROR;
}

ECode Service::OnStartCommand(
    /* [in] */ IIntent* intent,
    /* [in] */ Int32 flags,
    /* [in] */ Int32 startId,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    OnStart(intent, startId);
    *result = mStartCompatibility ?
            IService::START_STICKY_COMPATIBILITY : IService::START_STICKY;
    return NOERROR;
}

ECode Service::OnDestroy()
{
    return NOERROR;
}

ECode Service::OnConfigurationChanged(
    /* [in] */ IConfiguration* newConfig)
{
    return NOERROR;
}

ECode Service::OnLowMemory()
{
    return NOERROR;
}

ECode Service::OnTrimMemory(
    /* [in] */ Int32 level)
{
    return NOERROR;
}

ECode Service::OnBind(
    /* [in] */ IIntent* intent,
    /* [out] */ IBinder** binder)
{
    VALIDATE_NOT_NULL(binder);
    *binder = NULL;
    return NOERROR;
}

ECode Service::OnUnbind(
    /* [in] */ IIntent* intent,
    /* [out] */ Boolean* callOnRebindLater)
{
    VALIDATE_NOT_NULL(callOnRebindLater);
    *callOnRebindLater = FALSE;
    return NOERROR;
}

ECode Service::OnRebind(
    /* [in] */ IIntent* intent)
{
    return NOERROR;
}

ECode Service::OnTaskRemoved(
    /* [in] */ IIntent* rootIntent)
{
    return NOERROR;
}

ECode Service::StopSelf()
{
    return StopSelf(-1);
}

ECode Service::StopSelf(
    /* [in] */ Int32 startId)
{
    if (mActivityManager == NULL) {
        return NOERROR;
    }

    AutoPtr<IComponentName> cn;
    FAIL_RETURN(CComponentName::New(THIS_PROBE(IContext), mClassName, (IComponentName**)&cn));

    Boolean res;
    ECode ec = mActivityManager->StopServiceToken(cn, mToken, startId, &res);
    if (ec == (ECode)E_REMOTE_EXCEPTION) {
        ec = NOERROR;
    }
    return ec;
}

ECode Service::StopSelfResult(
    /* [in] */ Int32 startId,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;
    if (mActivityManager == NULL) {
        return NOERROR;
    }

    AutoPtr<IComponentName> cn;
    FAIL_RETURN(CComponentName::New(THIS_PROBE(IContext), mClassName, (IComponentName**)&cn));

    ECode ec = mActivityManager->StopServiceToken(cn, mToken, startId, result);
    if (ec == (ECode)E_REMOTE_EXCEPTION) {
        ec = NOERROR;
    }
    return ec;
}

ECode Service::SetForeground(
    /* [in] */ Boolean isForeground)
{
    Logger::W(TAG, String("setForeground: ignoring old API call on Service"));
    return NOERROR;
}

ECode Service::StartForeground(
    /* [in] */ Int32 id,
    /* [in] */ INotification* notification)
{
    String packageName;
    GetPackageName(&packageName);
    AutoPtr<IComponentName> cn;
    FAIL_RETURN(CComponentName::New(THIS_PROBE(IContext), mClassName, (IComponentName**)&cn));

    return mActivityManager->SetServiceForeground(cn, mToken, id, notification, TRUE);
}

ECode Service::StopForeground(
    /* [in] */ Boolean removeNotification)
{
    String packageName;
    GetPackageName(&packageName);
    AutoPtr<IComponentName> cn;
    FAIL_RETURN(CComponentName::New(THIS_PROBE(IContext), mClassName, (IComponentName**)&cn));

    return mActivityManager->SetServiceForeground(cn, mToken, 0, NULL, removeNotification);
}

ECode Service::Dump(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ IPrintWriter* writer,
    /* [in] */ ArrayOf<String>* args)
{
    return writer->PrintStringln(String("nothing to dump"));
}

// ------------------ Internal API ------------------

ECode Service::Attach(
    /* [in] */ IContext* context,
    /* [in] */ IActivityThread* thread,
    /* [in] */ const String& className,
    /* [in] */ IBinder* token,
    /* [in] */ IApplication* application,
    /* [in] */ IIActivityManager* activityManager)
{
    FAIL_RETURN(AttachBaseContext(context));

    mBase = context;
    mThread = thread;            // NOTE:  unused - remove?
    mClassName = className;
    mToken = token;
    mApplication = application;
    mActivityManager = activityManager;

    Int32 targetSdkVersion;
    AutoPtr<IApplicationInfo> appInfo;
    FAIL_RETURN(GetApplicationInfo((IApplicationInfo**)&appInfo));
    FAIL_RETURN(appInfo->GetTargetSdkVersion(&targetSdkVersion));

    mStartCompatibility = targetSdkVersion < Build::VERSION_CODES::ECLAIR;
    return NOERROR;
}

ECode Service::GetClassName(
    /* [out] */ String* className)
{
    VALIDATE_NOT_NULL(className);
    *className = mClassName;
    return NOERROR;
}

//
// IContext interfaces
//
ECode Service::AttachBaseContext(
    /* [in] */ IContext* newBase)
{
    if (mBase != NULL) {
        //throw new IllegalStateException("Base context already set");
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    mBase = newBase;

    return NOERROR;
}

ECode Service::GetBaseContext(
    /* [out] */ IContext** ctx)
{
    VALIDATE_NOT_NULL(ctx);

    *ctx = mBase;
    REFCOUNT_ADD(*ctx);

    return NOERROR;
}

ECode Service::GetAssets(
    /* [out] */ IAssetManager** assetManager)
{
    return mBase->GetAssets(assetManager);
}

ECode Service::GetResources(
    /* [out] */ IResources** resources)
{
    return mBase->GetResources(resources);
}

ECode Service::GetPackageManager(
    /* [out] */ IPackageManager** packageManager)
{
    return mBase->GetPackageManager(packageManager);
}

ECode Service::GetContentResolver(
    /* [out] */ IContentResolver** resolver)
{
    return mBase->GetContentResolver(resolver);
}

ECode Service::GetMainLooper(
    /* [out] */ ILooper** looper)
{
    return mBase->GetMainLooper(looper);
}

ECode Service::GetApplicationContext(
    /* [out] */ IContext** ctx)
{
    return mBase->GetApplicationContext(ctx);
}

ECode Service::RegisterComponentCallbacks(
    /* [in] */ IComponentCallbacks* componentCallback)
{
    return mBase->RegisterComponentCallbacks(componentCallback);
}

ECode Service::UnregisterComponentCallbacks(
    /* [in] */ IComponentCallbacks* componentCallback)
{
    return mBase->UnregisterComponentCallbacks(componentCallback);
}

ECode Service::GetText(
    /* [in] */ Int32 resId,
    /* [out] */ ICharSequence** text)
{
    return mBase->GetText(resId, text);
}

ECode Service::GetString(
    /* [in] */ Int32 resId,
    /* [out] */ String* str)
{
    return mBase->GetString(resId, str);
}

ECode Service::GetString(
    /* [in] */ Int32 resId,
    /* [in] */ ArrayOf<IInterface*>* formatArgs,
    /* [out] */ String* str)
{
    return mBase->GetString(resId, formatArgs, str);
}

ECode Service::SetTheme(
    /* [in] */ Int32 resid)
{
    return mBase->SetTheme(resid);
}

ECode Service::GetThemeResId(
    /* [out] */ Int32* resId)
{
    return mBase->GetThemeResId(resId);
}

ECode Service::GetTheme(
    /* [out] */ IResourcesTheme** theme)
{
    return mBase->GetTheme(theme);
}

ECode Service::ObtainStyledAttributes(
        /* [in] */ ArrayOf<Int32>* attrs,
    /* [out] */ ITypedArray** styles)
{
    return mBase->ObtainStyledAttributes(attrs, styles);
}

ECode Service::ObtainStyledAttributes(
    /* [in] */ Int32 resid,
    /* [in] */ ArrayOf<Int32>* attrs,
    /* [out] */ ITypedArray** styles)
{
    return mBase->ObtainStyledAttributes(resid, attrs, styles);
}

ECode Service::ObtainStyledAttributes(
    /* [in] */ IAttributeSet* set,
    /* [in] */ ArrayOf<Int32>* attrs,
    /* [out] */ ITypedArray** styles)
{
    return mBase->ObtainStyledAttributes(set, attrs, styles);
}

ECode Service::ObtainStyledAttributes(
    /* [in] */ IAttributeSet* set,
    /* [in] */ ArrayOf<Int32>* attrs,
    /* [in] */ Int32 defStyleAttr,
    /* [in] */ Int32 defStyleRes,
    /* [out] */ ITypedArray** styles)
{
    return mBase->ObtainStyledAttributes(set, attrs,
            defStyleAttr, defStyleRes, styles);
}

ECode Service::GetClassLoader(
    /* [out] */ IClassLoader** loader)
{
    return mBase->GetClassLoader(loader);
}

ECode Service::GetPackageName(
    /* [out] */ String* packageName)
{
    return mBase->GetPackageName(packageName);
}

ECode Service::GetApplicationInfo(
    /* [out] */ IApplicationInfo** info)
{
    return mBase->GetApplicationInfo(info);
}

ECode Service::GetPackageResourcePath(
    /* [out] */ String* path)
{
    return mBase->GetPackageResourcePath(path);
}

ECode Service::GetPackageCodePath(
    /* [out] */ String* codePath)
{
    return mBase->GetPackageCodePath(codePath);
}

ECode Service::GetSharedPrefsFile(
    /* [in] */ const String& name,
    /* [out] */ IFile** file)
{
    return mBase->GetSharedPrefsFile(name, file);
}

ECode Service::GetSharedPreferences(
    /* [in] */ const String& name,
    /* [in] */ Int32 mode,
    /* [out] */ ISharedPreferences** prefs)
{
    return mBase->GetSharedPreferences(name, mode, prefs);
}

ECode Service::OpenFileInput(
    /* [in] */ const String& name,
    /* [out] */ IFileInputStream** fileInputStream)
{
    return mBase->OpenFileInput(name, fileInputStream);
}

ECode Service::OpenFileOutput(
    /* [in] */ const String& name,
    /* [in] */ Int32 mode,
    /* [out] */ IFileOutputStream** fileOutputStream)
{
    return mBase->OpenFileOutput(name, mode, fileOutputStream);
}

ECode Service::DeleteFile(
    /* [in] */ const String& name,
    /* [out] */ Boolean* succeeded)
{
    return mBase->DeleteFile(name, succeeded);
}

ECode Service::GetFileStreamPath(
    /* [in] */ const String& name,
    /* [out] */ IFile** file)
{
    return mBase->GetFileStreamPath(name, file);
}

ECode Service::GetFilesDir(
    /* [out] */ IFile** filesDir)
{
    return mBase->GetFilesDir(filesDir);
}

ECode Service::GetExternalFilesDir(
    /* [in] */ const String& type,
    /* [out] */ IFile** filesDir)
{
    return mBase->GetExternalFilesDir(type, filesDir);
}

ECode Service::GetObbDir(
    /* [out] */ IFile** obbDir)
{
    return mBase->GetObbDir(obbDir);
}

ECode Service::GetCacheDir(
    /* [out] */ IFile** cacheDir)
{
    return mBase->GetCacheDir(cacheDir);
}

ECode Service::GetExternalCacheDir(
    /* [out] */ IFile** externalDir)
{
    return mBase->GetExternalCacheDir(externalDir);
}

ECode Service::GetFileList(
    /* [out, callee] */ ArrayOf<String>** fileList)
{
    return mBase->GetFileList(fileList);
}

ECode Service::GetDir(
    /* [in] */ const String& name,
    /* [in] */ Int32 mode,
    /* [out] */ IFile** dir)
{
    return mBase->GetDir(name, mode, dir);
}

ECode Service::OpenOrCreateDatabase(
    /* [in] */ const String& name,
    /* [in] */ Int32 mode,
    /* [in] */ ISQLiteDatabaseCursorFactory* factory,
    /* [out] */ ISQLiteDatabase** sqliteDB)
{
    return mBase->OpenOrCreateDatabase(name, mode, factory, sqliteDB);
}

ECode Service::OpenOrCreateDatabase(
    /* [in] */ const String& name,
    /* [in] */ Int32 mode,
    /* [in] */ ISQLiteDatabaseCursorFactory* factory,
    /* [in] */ IDatabaseErrorHandler* errorHandler,
    /* [out] */ ISQLiteDatabase** sqliteDB)
{
    return mBase->OpenOrCreateDatabase(name, mode, factory, errorHandler, sqliteDB);
}

ECode Service::DeleteDatabase(
    /* [in] */ const String& name,
    /* [out] */ Boolean* succeeded)
{
    return mBase->DeleteDatabase(name, succeeded);
}

ECode Service::GetDatabasePath(
    /* [in] */ const String& name,
    /* [out] */ IFile** path)
{
    return mBase->GetDatabasePath(name, path);
}

ECode Service::GetDatabaseList(
    /* [out, callee] */ ArrayOf<String>** databaseList)
{
    return mBase->GetDatabaseList(databaseList);
}

ECode Service::GetWallpaper(
    /* [out] */ IDrawable** drawable)
{
    return mBase->GetWallpaper(drawable);
}

ECode Service::PeekWallpaper(
    /* [out] */ IDrawable** drawable)
{
    return mBase->PeekWallpaper(drawable);
}

ECode Service::GetWallpaperDesiredMinimumWidth(
    /* [out] */ Int32* minWidth)
{
    return mBase->GetWallpaperDesiredMinimumWidth(minWidth);
}

ECode Service::GetWallpaperDesiredMinimumHeight(
    /* [out] */ Int32* minHeight)
{
    return mBase->GetWallpaperDesiredMinimumHeight(minHeight);
}

ECode Service::SetWallpaper(
    /* [in] */ IBitmap* bitmap)
{
    return mBase->SetWallpaper(bitmap);
}

ECode Service::SetWallpaper(
    /* [in] */ IInputStream* data)
{
    return mBase->SetWallpaper(data);
}

ECode Service::ClearWallpaper()
{
    return mBase->ClearWallpaper();
}

ECode Service::StartActivity(
    /* [in] */ IIntent* intent)
{
    return mBase->StartActivity(intent);
}

ECode Service::StartActivity(
    /* [in] */ IIntent* intent,
    /* [in] */ IBundle* options)
{
    return mBase->StartActivity(intent, options);
}

ECode Service::StartActivityAsUser(
    /* [in] */ IIntent* intent,
    /* [in] */ IUserHandle* user)
{
    return mBase->StartActivityAsUser(intent, user);
}

ECode Service::StartActivityAsUser(
    /* [in] */ IIntent* intent,
    /* [in] */ IBundle* options,
    /* [in] */ IUserHandle* user)
{
    return mBase->StartActivityAsUser(intent, options, user);
}

ECode Service::StartActivities(
    /* [in] */ ArrayOf<IIntent*>* intents)
{
    return mBase->StartActivities(intents);
}

ECode Service::StartActivities(
    /* [in] */ ArrayOf<IIntent*>* intents,
    /* [in] */ IBundle* options)
{
    return mBase->StartActivities(intents, options);
}

ECode Service::StartActivitiesAsUser(
    /* [in] */ ArrayOf<IIntent*>* intents,
    /* [in] */ IBundle* options,
    /* [in] */ IUserHandle* userHandle)
{
    return mBase->StartActivitiesAsUser(intents, options, userHandle);
}

ECode Service::StartIntentSender(
    /* [in] */ IIntentSender* intent,
    /* [in] */ IIntent* fillInIntent,
    /* [in] */ Int32 flagsMask,
    /* [in] */ Int32 flagsValues,
    /* [in] */ Int32 extraFlags)
{
    return mBase->StartIntentSender(intent, fillInIntent, flagsMask,
        flagsValues, extraFlags);
}

ECode Service::StartIntentSender(
    /* [in] */ IIntentSender* intent,
    /* [in] */ IIntent* fillInIntent,
    /* [in] */ Int32 flagsMask,
    /* [in] */ Int32 flagsValues,
    /* [in] */ Int32 extraFlags,
    /* [in] */ IBundle* options)
{
    return mBase->StartIntentSender(intent, fillInIntent, flagsMask,
        flagsValues, extraFlags, options);
}

ECode Service::SendBroadcast(
    /* [in] */ IIntent* intent)
{
    return mBase->SendBroadcast(intent);
}

ECode Service::SendBroadcast(
    /* [in] */ IIntent* intent,
    /* [in] */ const String& receiverPermission)
{
    return mBase->SendBroadcast(intent, receiverPermission);
}

ECode Service::SendOrderedBroadcast(
    /* [in] */ IIntent* intent,
    /* [in] */ const String& receiverPermission)
{
    return mBase->SendOrderedBroadcast(intent, receiverPermission);
}

ECode Service::SendOrderedBroadcast(
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

ECode Service::SendBroadcastAsUser(
    /* [in] */ IIntent* intent,
    /* [in] */ IUserHandle* user)
{
    return mBase->SendBroadcastAsUser(intent, user);
}

ECode Service::SendBroadcastAsUser(
    /* [in] */ IIntent* intent,
    /* [in] */ IUserHandle* user,
    /* [in] */ const String& receiverPermission)
{
    return mBase->SendBroadcastAsUser(intent, user, receiverPermission);
}

ECode Service::SendOrderedBroadcastAsUser(
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

ECode Service::SendStickyBroadcast(
    /* [in] */ IIntent* intent)
{
    return mBase->SendStickyBroadcast(intent);
}

ECode Service::SendStickyOrderedBroadcast(
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

ECode Service::RemoveStickyBroadcast(
    /* [in] */ IIntent* intent)
{
    return mBase->RemoveStickyBroadcast(intent);
}

ECode Service::SendStickyBroadcastAsUser(
    /* [in] */ IIntent* intent,
    /* [in] */ IUserHandle* user)
{
    return mBase->SendStickyBroadcastAsUser(intent, user);
}

ECode Service::SendStickyOrderedBroadcastAsUser(
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

ECode Service::RemoveStickyBroadcastAsUser(
    /* [in] */ IIntent* intent,
    /* [in] */ IUserHandle* user)
{
    return mBase->RemoveStickyBroadcastAsUser(intent, user);
}

ECode Service::RegisterReceiver(
    /* [in] */ IBroadcastReceiver* receiver,
    /* [in] */ IIntentFilter* filter,
    /* [out] */ IIntent** intent)
{
    return mBase->RegisterReceiver(receiver,filter,intent);
}

ECode Service::RegisterReceiver(
    /* [in] */ IBroadcastReceiver* receiver,
    /* [in] */ IIntentFilter* filter,
    /* [in] */ const String& broadcastPermission,
    /* [in] */ IHandler* scheduler,
    /* [out] */ IIntent** intent)
{
    return mBase->RegisterReceiver(receiver,filter,broadcastPermission,scheduler, intent);
}

ECode Service::RegisterReceiverAsUser(
    /* [in] */ IBroadcastReceiver* receiver,
    /* [in] */ IUserHandle* user,
    /* [in] */ IIntentFilter* filter,
    /* [in] */ const String& broadcastPermission,
    /* [in] */ IHandler* scheduler,
    /* [out] */ IIntent** stickyIntent)
{
    //Implememt in ContextWarpper.
    return E_NOT_IMPLEMENTED;
}

ECode Service::UnregisterReceiver(
    /* [in] */ IBroadcastReceiver* receiver)
{
    return mBase->UnregisterReceiver(receiver);
}

ECode Service::StartService(
    /* [in] */ IIntent* service,
    /* [out] */ IComponentName** name)
{
    return mBase->StartService(service, name);
}

ECode Service::StopService(
    /* [in] */ IIntent* service,
    /* [out] */ Boolean* succeeded)
{
    return mBase->StopService(service, succeeded);
}

ECode Service::StartServiceAsUser(
    /* [in] */ IIntent* service,
    /* [in] */ IUserHandle* user,
    /* [out] */ IComponentName** name)
{
    return mBase->StartServiceAsUser(service, user, name);
}

ECode Service::StopServiceAsUser(
    /* [in] */ IIntent* service,
    /* [in] */ IUserHandle* user,
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded)
    return mBase->StopServiceAsUser(service, user, succeeded);
}

ECode Service::BindService(
    /* [in] */ IIntent* service,
    /* [in] */ Elastos::Droid::Content::IServiceConnection* conn,
    /* [in] */ Int32 flags,
    /* [out] */ Boolean* succeeded)
{
    return mBase->BindService(service, conn, flags, succeeded);
}

ECode Service::BindService(
    /* [in] */ IIntent* service,
    /* [in] */ Elastos::Droid::Content::IServiceConnection* conn,
    /* [in] */ Int32 flags,
    /* [in] */ Int32 userHandle,
    /* [out] */ Boolean* succeeded)
{
    return mBase->BindService(service, conn, flags, userHandle, succeeded);
}

ECode Service::UnbindService(
    /* [in] */ Elastos::Droid::Content::IServiceConnection* conn)
{
    return mBase->UnbindService(conn);
}

ECode Service::StartInstrumentation(
    /* [in] */ IComponentName* className,
    /* [in] */ const String& profileFile,
    /* [in] */ IBundle* arguments,
    /* [out] */ Boolean* succeeded)
{
    return mBase->StartInstrumentation(className, profileFile, arguments, succeeded);
}

//    @Override
ECode Service::GetSystemService(
    /* [in] */ const String&name,
    /* [out] */ IInterface** object)
{
    if (!IContext::LAYOUT_INFLATER_SERVICE.Compare(name)) {
        if (mInflater == NULL) {
            AutoPtr<ILayoutInflater> inflater;
            mBase->GetSystemService(IContext::LAYOUT_INFLATER_SERVICE, (IInterface**)&inflater);

            if (inflater == NULL) {
                return E_INVALID_ARGUMENT;
            }

            inflater->CloneInContext(
                    THIS_PROBE(IContext), (ILayoutInflater**)&mInflater);
        }
        *object = mInflater;
        REFCOUNT_ADD(*object);
        return NOERROR;
    }

    return mBase->GetSystemService(name, object);
}

ECode Service::CheckPermission(
    /* [in] */ const String& permission,
    /* [in] */ Int32 pid,
    /* [in] */ Int32 uid,
    /* [out] */ Int32 * result)
{
    return mBase->CheckPermission(permission, pid, uid, result);
}

ECode Service::CheckCallingPermission(
    /* [in] */ const String& permission,
    /* [out] */ Int32* value)
{
    return mBase->CheckCallingPermission(permission, value);
}

ECode Service::CheckCallingOrSelfPermission(
    /* [in] */ const String& permission,
    /* [out] */ Int32* perm)
{
    return mBase->CheckCallingOrSelfPermission(permission, perm);
}

ECode Service::EnforcePermission(
    /* [in] */ const String& permission,
    /* [in] */ Int32 pid,
    /* [in] */ Int32 uid,
    /* [in] */ const String& message)
{
    return mBase->EnforcePermission(permission, pid, uid, message);
}

ECode Service::EnforceCallingPermission(
    /* [in] */ const String& permission,
    /* [in] */ const String& message)
{
    return mBase->EnforceCallingPermission(permission, message);
}

ECode Service::EnforceCallingOrSelfPermission(
    /* [in] */ const String& permission,
    /* [in] */ const String& message)
{
    return mBase->EnforceCallingOrSelfPermission(permission, message);
}

ECode Service::GrantUriPermission(
    /* [in] */ const String& toCapsule,
    /* [in] */ IUri* uri,
    /* [in] */ Int32 modeFlags)
{
    return mBase->GrantUriPermission(toCapsule, uri, modeFlags);
}

ECode Service::RevokeUriPermission(
    /* [in] */ IUri* uri,
    /* [in] */ Int32 modeFlags)
{
    return mBase->RevokeUriPermission(uri, modeFlags);
}

ECode Service::CheckUriPermission(
    /* [in] */ IUri * uri,
    /* [in] */ Int32 pid,
    /* [in] */ Int32 uid,
    /* [in] */ Int32 modeFlags,
    /* [out] */ Int32 * result)
{
    return mBase->CheckUriPermission(uri, pid, uid, modeFlags, result);
}

ECode Service::CheckCallingUriPermission(
    /* [in] */ IUri* uri,
    /* [in] */ Int32 modeFlags,
    /* [out] */ Int32* permissionId)
{
    return mBase->CheckCallingUriPermission(uri, modeFlags, permissionId);
}

ECode Service::CheckCallingOrSelfUriPermission(
    /* [in] */ IUri* uri,
    /* [in] */ Int32 modeFlags,
    /* [out] */ Int32* permissionId)
{
    return mBase->CheckCallingOrSelfUriPermission(uri, modeFlags, permissionId);
}

ECode Service::CheckUriPermission(
    /* [in] */ IUri * uri,
    /* [in] */ const String& readPermission,
    /* [in] */ const String& writePermission,
    /* [in] */ Int32 pid,
    /* [in] */ Int32 uid,
    /* [in] */ Int32 modeFlags,
    /* [out] */ Int32 * result)
{
    return mBase->CheckUriPermission(uri, readPermission, writePermission,
                    pid, uid, modeFlags, result);
}

ECode Service::EnforceUriPermission(
    /* [in] */ IUri* uri,
    /* [in] */ Int32 pid,
    /* [in] */ Int32 uid,
    /* [in] */ Int32 modeFlags,
    /* [in] */ const String& message)
{
    return mBase->EnforceUriPermission(uri, pid, uid, modeFlags, message);
}

ECode Service::EnforceCallingUriPermission(
    /* [in] */ IUri* uri,
    /* [in] */ Int32 modeFlags,
    /* [in] */ const String& message)
{
    return mBase->EnforceCallingUriPermission(uri, modeFlags, message);
}

ECode Service::EnforceCallingOrSelfUriPermission(
    /* [in] */ IUri* uri,
    /* [in] */ Int32 modeFlags,
    /* [in] */ const String& message)
{
    return mBase->EnforceCallingOrSelfUriPermission(uri, modeFlags, message);
}

ECode Service::EnforceUriPermission(
    /* [in] */ IUri* uri,
    /* [in] */ const String& readPermission,
    /* [in] */ const String& writePermission,
    /* [in] */ Int32 pid,
    /* [in] */ Int32 uid,
    /* [in] */ Int32 modeFlags,
    /* [in] */ const String& message)
{
    return mBase->EnforceUriPermission(uri, readPermission, writePermission,
            pid, uid, modeFlags, message);
}

ECode Service::CreatePackageContext(
    /* [in] */ const String& packageName,
    /* [in] */ Int32 flags,
    /* [out] */ IContext** ctx)
{
    return mBase->CreatePackageContext(packageName, flags, ctx);
}

ECode Service::CreatePackageContextAsUser(
    /* [in] */ const String& packageName,
    /* [in] */ Int32 flags,
    /* [in] */ IUserHandle* user,
    /* [out] */ IContext** ctx)
{
    return mBase->CreatePackageContextAsUser(packageName, flags, user, ctx);
}

ECode Service::CreateConfigurationContext(
    /* [in] */ IConfiguration* overrideConfiguration,
    /* [out] */ IContext** ctx)
{
    return mBase->CreateConfigurationContext(overrideConfiguration, ctx);
}

ECode Service::CreateDisplayContext(
    /* [in] */ IDisplay* display,
    /* [out] */ IContext** ctx)
{
    return mBase->CreateDisplayContext(display, ctx);
}

ECode Service::GetCompatibilityInfo(
    /* [in] */ Int32 displayId,
    /* [out] */ ICompatibilityInfoHolder** infoHolder)
{
    return mBase->GetCompatibilityInfo(displayId, infoHolder);
}

ECode Service::IsRestricted(
    /* [out] */ Boolean* isRestricted)
{
    return mBase->IsRestricted(isRestricted);
}

} // App
} // Droid
} // Elastos
