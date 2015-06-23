#include "Presentation.h"
#include "util/CDisplayMetrics.h"
#include <elastos/utility/logging/Logger.h>
#include "util/CTypedValue.h"
#include "view/WindowManagerImpl.h"
#include "R.h"

using Elastos::Utility::Logging::Logger;
using Elastos::Droid::Os::EIID_IHandler;
using Elastos::Droid::View::IGravity;
using Elastos::Droid::View::WindowManagerImpl;
using Elastos::Droid::Utility::ITypedValue;
using Elastos::Droid::Utility::CTypedValue;
using Elastos::Droid::Utility::IDisplayMetrics;
using Elastos::Droid::Utility::CDisplayMetrics;
using Elastos::Droid::Content::Res::IResourcesTheme;
using Elastos::Droid::Hardware::Display::EIID_IDisplayListener;

namespace Elastos {
namespace Droid {
namespace App {

const String Presentation::TAG("Presentation");

CAR_INTERFACE_IMPL(Presentation::PresentationDisplayListener, IDisplayListener);
CAR_INTERFACE_IMPL(Presentation::PresentationContextThemeWrapper, IContextThemeWrapper);

ECode Presentation::PresentationDisplayListener::OnDisplayAdded(
    /* [in] */ Int32 displayId)
{
    return NOERROR;
}

ECode Presentation::PresentationDisplayListener::OnDisplayRemoved(
    /* [in] */ Int32 displayId)
{
    Int32 id;
    mHost->mDisplay->GetDisplayId(&id);
    if (displayId == id) {
        mHost->HandleDisplayRemoved();
    }
    return NOERROR;
}

ECode Presentation::PresentationDisplayListener::OnDisplayChanged(
    /* [in] */ Int32 displayId)
{
    Int32 id;
    mHost->mDisplay->GetDisplayId(&id);
    if (displayId == id) {
        mHost->HandleDisplayChanged();
    }
    return NOERROR;
}

ECode Presentation::PresentationHandler::HandleMessage(
    /* [in] */ IMessage* msg)
{
    Int32 what;
    msg->GetWhat(&what);
    switch (what) {
        case IPresentation::MSG_CANCEL:
            mHost->Cancel();
            break;
    }
    return NOERROR;
}

ECode Presentation::PresentationContextThemeWrapper::GetSystemService(
    /* [in] */ const String& name,
    /* [out] */ IInterface** object)
{
    VALIDATE_NOT_NULL(object);
    if (name.Equals(IContext::WINDOW_SERVICE)) {
        *object = mDisplayWindowManager;
        REFCOUNT_ADD(*object);
        return NOERROR;
    }
    return ContextThemeWrapper::GetSystemService(name, object);
}

// IContextThemeWrapper Implementation
ECode Presentation::PresentationContextThemeWrapper::GetBaseContext(
    /* [out] */ IContext** context)
{
    return ContextThemeWrapper::GetBaseContext(context);
}

ECode Presentation::PresentationContextThemeWrapper::ApplyOverrideConfiguration(
    /* [in] */ IConfiguration* overrideConfiguration)
{
    return ContextThemeWrapper::ApplyOverrideConfiguration(overrideConfiguration);
}

ECode Presentation::PresentationContextThemeWrapper::GetAssets(
    /* [out] */ IAssetManager** assetManager)
{
    return ContextThemeWrapper::GetAssets(assetManager);
}

ECode Presentation::PresentationContextThemeWrapper::GetResources(
    /* [out] */ IResources** resources)
{
    return ContextThemeWrapper::GetResources(resources);
}

ECode Presentation::PresentationContextThemeWrapper::GetPackageManager(
    /* [out] */ IPackageManager** packageManager)
{
    return ContextThemeWrapper::GetPackageManager(packageManager);
}

ECode Presentation::PresentationContextThemeWrapper::GetContentResolver(
    /* [out] */ IContentResolver** resolver)
{
    return ContextThemeWrapper::GetContentResolver(resolver);
}

ECode Presentation::PresentationContextThemeWrapper::GetMainLooper(
    /* [out] */ ILooper** looper)
{
    return ContextThemeWrapper::GetMainLooper(looper);
}

ECode Presentation::PresentationContextThemeWrapper::GetApplicationContext(
    /* [out] */ IContext** ctx)
{
    return ContextThemeWrapper::GetApplicationContext(ctx);
}

ECode Presentation::PresentationContextThemeWrapper::RegisterComponentCallbacks(
    /* [in] */ IComponentCallbacks* componentCallback)
{
    return Context::RegisterComponentCallbacks(componentCallback);
}

ECode Presentation::PresentationContextThemeWrapper::UnregisterComponentCallbacks(
    /* [in] */ IComponentCallbacks* componentCallback)
{
    return Context::UnregisterComponentCallbacks(componentCallback);
}

ECode Presentation::PresentationContextThemeWrapper::GetText(
    /* [in] */ Int32 resId,
    /* [out] */ ICharSequence** text)
{
    return Context::GetText(resId, text);
}

ECode Presentation::PresentationContextThemeWrapper::GetString(
    /* [in] */ Int32 resId,
    /* [out] */ String* str)
{
    return Context::GetString(resId, str);
}

ECode Presentation::PresentationContextThemeWrapper::GetString(
    /* [in] */ Int32 resId,
    /* [in] */ ArrayOf<IInterface*>* formatArgs,
    /* [out] */ String* str)
{
    return ContextThemeWrapper::GetString(resId, formatArgs, str);
}

ECode Presentation::PresentationContextThemeWrapper::SetTheme(
    /* [in] */ Int32 resId)
{
    return ContextThemeWrapper::SetTheme(resId);
}

ECode Presentation::PresentationContextThemeWrapper::GetThemeResId(
    /* [out] */ Int32* resId)
{
    return ContextThemeWrapper::GetThemeResId(resId);
}

ECode Presentation::PresentationContextThemeWrapper::GetTheme(
    /* [out] */ IResourcesTheme** theme)
{
    return ContextThemeWrapper::GetTheme(theme);
}

ECode Presentation::PresentationContextThemeWrapper::ObtainStyledAttributes(
    /* [in] */ ArrayOf<Int32>* attrs,
    /* [out] */ ITypedArray** styles)
{
    return Context::ObtainStyledAttributes(attrs, styles);
}

ECode Presentation::PresentationContextThemeWrapper::ObtainStyledAttributes(
    /* [in] */ Int32 resid,
    /* [in] */ ArrayOf<Int32>* attrs,
    /* [out] */ ITypedArray** styles)
{
    return Context::ObtainStyledAttributes(resid, attrs, styles);
}

ECode Presentation::PresentationContextThemeWrapper::ObtainStyledAttributes(
    /* [in] */ IAttributeSet* set,
    /* [in] */ ArrayOf<Int32>* attrs,
    /* [out] */ ITypedArray** styles)
{
    return Context::ObtainStyledAttributes(set, attrs, styles);
}

ECode Presentation::PresentationContextThemeWrapper::ObtainStyledAttributes(
    /* [in] */ IAttributeSet* set,
    /* [in] */ ArrayOf<Int32>* attrs,
    /* [in] */ Int32 defStyleAttr,
    /* [in] */ Int32 defStyleRes,
    /* [out] */ ITypedArray** styles)
{
    return Context::ObtainStyledAttributes(set, attrs, defStyleAttr, defStyleRes,
        styles);
}

ECode Presentation::PresentationContextThemeWrapper::GetClassLoader(
    /* [out] */ IClassLoader** loader)
{
    return ContextThemeWrapper::GetClassLoader(loader);
}

ECode Presentation::PresentationContextThemeWrapper::GetPackageName(
    /* [out] */ String* packageName)
{
    return ContextThemeWrapper::GetPackageName(packageName);
}

ECode Presentation::PresentationContextThemeWrapper::GetApplicationInfo(
    /* [out] */ IApplicationInfo** info)
{
    return ContextThemeWrapper::GetApplicationInfo(info);
}

ECode Presentation::PresentationContextThemeWrapper::GetPackageResourcePath(
    /* [out] */ String* path)
{
    return ContextThemeWrapper::GetPackageResourcePath(path);
}

ECode Presentation::PresentationContextThemeWrapper::GetPackageCodePath(
    /* [out] */ String* codePath)
{
    return ContextThemeWrapper::GetPackageCodePath(codePath);
}

ECode Presentation::PresentationContextThemeWrapper::GetSharedPrefsFile(
    /* [in] */ const String& name,
    /* [out] */IFile** file)
{
    return ContextThemeWrapper::GetSharedPrefsFile(name, file);
}

ECode Presentation::PresentationContextThemeWrapper::GetSharedPreferences(
    /* [in] */ const String& name,
    /* [in] */ Int32 mode,
    /* [out] */ ISharedPreferences** prefs)
{
    return ContextThemeWrapper::GetSharedPreferences(name, mode, prefs);
}

ECode Presentation::PresentationContextThemeWrapper::OpenFileInput(
    /* [in] */ const String& name,
    /* [out] */ IFileInputStream** fileInputStream)
{
    return ContextThemeWrapper::OpenFileInput(name, fileInputStream);
}

ECode Presentation::PresentationContextThemeWrapper::OpenFileOutput(
    /* [in] */ const String& name,
    /* [in] */ Int32 mode,
    /* [out] */ IFileOutputStream** fileOutputStream)
{
    return ContextThemeWrapper::OpenFileOutput(name, mode, fileOutputStream);
}

ECode Presentation::PresentationContextThemeWrapper::DeleteFile(
    /* [in] */ const String& name,
    /* [out] */ Boolean* succeeded)
{
    return ContextThemeWrapper::DeleteFile(name, succeeded);
}

ECode Presentation::PresentationContextThemeWrapper::GetFileStreamPath(
    /* [in] */ const String& name,
    /* [out] */ IFile** file)
{
    return ContextThemeWrapper::GetFileStreamPath(name, file);
}

ECode Presentation::PresentationContextThemeWrapper::GetFilesDir(
    /* [out] */ IFile** filesDir)
{
    return ContextThemeWrapper::GetFilesDir(filesDir);
}

ECode Presentation::PresentationContextThemeWrapper::GetExternalFilesDir(
    /* [in] */ const String& type,
    /* [out] */ IFile** filesDir)
{
    return ContextThemeWrapper::GetExternalFilesDir(type, filesDir);
}

ECode Presentation::PresentationContextThemeWrapper::GetObbDir(
    /* [out] */ IFile** obbDir)
{
    return ContextThemeWrapper::GetObbDir(obbDir);
}

ECode Presentation::PresentationContextThemeWrapper::GetCacheDir(
    /* [out] */ IFile** cacheDir)
{
    return ContextThemeWrapper::GetCacheDir(cacheDir);
}

ECode Presentation::PresentationContextThemeWrapper::GetExternalCacheDir(
    /* [out] */ IFile** externalDir)
{
    return ContextThemeWrapper::GetExternalCacheDir(externalDir);
}

ECode Presentation::PresentationContextThemeWrapper::GetFileList(
    /* [out, callee] */ ArrayOf<String>** fileList)
{
    return ContextThemeWrapper::GetFileList(fileList);
}

ECode Presentation::PresentationContextThemeWrapper::GetDir(
    /* [in] */ const String& name,
    /* [in] */ Int32 mode,
    /* [out] */ IFile** dir)
{
    return ContextThemeWrapper::GetDir(name, mode, dir);
}

ECode Presentation::PresentationContextThemeWrapper::OpenOrCreateDatabase(
    /* [in] */ const String& name,
    /* [in] */ Int32 mode,
    /* [in] */ ISQLiteDatabaseCursorFactory* factory,
    /* [out] */ ISQLiteDatabase** sqliteDB)
{
    return ContextThemeWrapper::OpenOrCreateDatabase(name, mode, factory, sqliteDB);
}

ECode Presentation::PresentationContextThemeWrapper::OpenOrCreateDatabase(
    /* [in] */ const String& name,
    /* [in] */ Int32 mode,
    /* [in] */ ISQLiteDatabaseCursorFactory* factory,
    /* [in] */ IDatabaseErrorHandler* errorHandler,
    /* [out] */ ISQLiteDatabase** sqliteDB)
{
    return ContextThemeWrapper::OpenOrCreateDatabase(name, mode, factory, errorHandler, sqliteDB);
}

ECode Presentation::PresentationContextThemeWrapper::DeleteDatabase(
    /* [in] */ const String& name,
    /* [out] */ Boolean* succeeded)
{
    return ContextThemeWrapper::DeleteDatabase(name, succeeded);
}

ECode Presentation::PresentationContextThemeWrapper::GetDatabasePath(
    /* [in] */ const String& name,
    /* [out] */ IFile** path)
{
    return ContextThemeWrapper::GetDatabasePath(name, path);
}

ECode Presentation::PresentationContextThemeWrapper::GetDatabaseList(
    /* [out, callee] */ ArrayOf<String>** databaseList)
{
    return ContextThemeWrapper::GetDatabaseList(databaseList);
}

ECode Presentation::PresentationContextThemeWrapper::GetWallpaper(
    /* [out] */ IDrawable** drawable)
{
    return ContextThemeWrapper::GetWallpaper(drawable);
}

ECode Presentation::PresentationContextThemeWrapper::PeekWallpaper(
    /* [out] */ IDrawable** drawable)
{
    return ContextThemeWrapper::PeekWallpaper(drawable);
}

ECode Presentation::PresentationContextThemeWrapper::GetWallpaperDesiredMinimumWidth(
    /* [out] */ Int32* minWidth)
{
    return ContextThemeWrapper::GetWallpaperDesiredMinimumWidth(minWidth);
}

ECode Presentation::PresentationContextThemeWrapper::GetWallpaperDesiredMinimumHeight(
    /* [out] */ Int32* minHeight)
{
    return ContextThemeWrapper::GetWallpaperDesiredMinimumHeight(minHeight);
}

ECode Presentation::PresentationContextThemeWrapper::SetWallpaper(
    /* [in] */ IBitmap* bitmap)
{
    return ContextThemeWrapper::SetWallpaper(bitmap);
}

ECode Presentation::PresentationContextThemeWrapper::SetWallpaper(
    /* [in] */ IInputStream* data)
{
    return ContextThemeWrapper::SetWallpaper(data);
}

ECode Presentation::PresentationContextThemeWrapper::ClearWallpaper()
{
    return ContextThemeWrapper::ClearWallpaper();
}

ECode Presentation::PresentationContextThemeWrapper::StartActivity(
    /* [in] */ IIntent* intent)
{
    return ContextThemeWrapper::StartActivity(intent);
}

ECode Presentation::PresentationContextThemeWrapper::StartActivityAsUser(
    /* [in] */ IIntent* intent,
    /* [in] */ IUserHandle* user)
{
    return ContextThemeWrapper::StartActivityAsUser(intent, user);
}

ECode Presentation::PresentationContextThemeWrapper::StartActivity(
    /* [in] */ IIntent* intent,
    /* [in] */ IBundle* options)
{
    return ContextThemeWrapper::StartActivity(intent, options);
}

ECode Presentation::PresentationContextThemeWrapper::StartActivityAsUser(
    /* [in] */ IIntent* intent,
    /* [in] */ IBundle* options,
    /* [in] */ IUserHandle* userId)
{
    return ContextThemeWrapper::StartActivityAsUser(intent, options, userId);
}

ECode Presentation::PresentationContextThemeWrapper::StartActivities(
    /* [in] */ ArrayOf<IIntent*>* intents)
{
    return ContextThemeWrapper::StartActivities(intents);
}

ECode Presentation::PresentationContextThemeWrapper::StartActivities(
    /* [in] */ ArrayOf<IIntent*>* intents,
    /* [in] */ IBundle* options)
{
    return ContextThemeWrapper::StartActivities(intents, options);
}

ECode Presentation::PresentationContextThemeWrapper::StartActivitiesAsUser(
    /* [in] */ ArrayOf<IIntent*>* intents,
    /* [in] */ IBundle* options,
    /* [in] */ IUserHandle* userHandle)
{
    return ContextThemeWrapper::StartActivitiesAsUser(intents, options, userHandle);
}

ECode Presentation::PresentationContextThemeWrapper::StartIntentSender(
    /* [in] */ IIntentSender* intent,
    /* [in] */ IIntent* fillInIntent,
    /* [in] */ Int32 flagsMask,
    /* [in] */ Int32 flagsValues,
    /* [in] */ Int32 extraFlags)
{
    return ContextThemeWrapper::StartIntentSender(intent, fillInIntent, flagsMask, flagsValues,
        extraFlags);
}

ECode Presentation::PresentationContextThemeWrapper::StartIntentSender(
    /* [in] */ IIntentSender* intent,
    /* [in] */ IIntent* fillInIntent,
    /* [in] */ Int32 flagsMask,
    /* [in] */ Int32 flagsValues,
    /* [in] */ Int32 extraFlags,
    /* [in] */ IBundle* options)
{
    return ContextThemeWrapper::StartIntentSender(intent, fillInIntent, flagsMask, flagsValues,
        extraFlags, options);
}

ECode Presentation::PresentationContextThemeWrapper::SendBroadcast(
    /* [in] */ IIntent* intent)
{
    return ContextThemeWrapper::SendBroadcast(intent);
}

ECode Presentation::PresentationContextThemeWrapper::SendBroadcast(
    /* [in] */ IIntent* intent,
    /* [in] */ const String& receiverPermission)
{
    return ContextThemeWrapper::SendBroadcast(intent, receiverPermission);
}

ECode Presentation::PresentationContextThemeWrapper::SendOrderedBroadcast(
    /* [in] */ IIntent* intent,
    /* [in] */ const String& receiverPermission)
{
    return ContextThemeWrapper::SendOrderedBroadcast(intent, receiverPermission);
}

ECode Presentation::PresentationContextThemeWrapper::SendOrderedBroadcast(
    /* [in] */ IIntent* intent,
    /* [in] */ const String& receiverPermission,
    /* [in] */ IBroadcastReceiver* resultReceiver,
    /* [in] */ IHandler* scheduler,
    /* [in] */ Int32 initialCode,
    /* [in] */ const String& initialData,
    /* [in] */ IBundle* initialExtras)
{
    return ContextThemeWrapper::SendOrderedBroadcast(intent, receiverPermission, resultReceiver,
        scheduler, initialCode, initialData, initialExtras);
}

ECode Presentation::PresentationContextThemeWrapper::SendBroadcastAsUser(
    /* [in] */ IIntent* intent,
    /* [in] */ IUserHandle* user)
{
    return ContextThemeWrapper::SendBroadcastAsUser(intent, user);
}

ECode Presentation::PresentationContextThemeWrapper::SendBroadcastAsUser(
    /* [in] */ IIntent* intent,
    /* [in] */ IUserHandle* user,
    /* [in] */ const String& receiverPermission)
{
    return ContextThemeWrapper::SendBroadcastAsUser(intent, user, receiverPermission);
}

ECode Presentation::PresentationContextThemeWrapper::SendOrderedBroadcastAsUser(
    /* [in] */ IIntent* intent,
    /* [in] */ IUserHandle* user,
    /* [in] */ const String& receiverPermission,
    /* [in] */ IBroadcastReceiver* resultReceiver,
    /* [in] */ IHandler* scheduler,
    /* [in] */ Int32 initialCode,
    /* [in] */ const String& initialData,
    /* [in] */ IBundle* initialExtras)
{
    return ContextThemeWrapper::SendOrderedBroadcastAsUser(intent, user, receiverPermission,
        resultReceiver, scheduler, initialCode, initialData, initialExtras);
}

ECode Presentation::PresentationContextThemeWrapper::SendStickyBroadcast(
    /* [in] */ IIntent* intent)
{
    return ContextThemeWrapper::SendStickyBroadcast(intent);
}

ECode Presentation::PresentationContextThemeWrapper::SendStickyOrderedBroadcast(
    /* [in] */ IIntent* intent,
    /* [in] */ IBroadcastReceiver* resultReceiver,
    /* [in] */ IHandler* scheduler,
    /* [in] */ Int32 initialCode,
    /* [in] */ const String& initialData,
    /* [in] */ IBundle* initialExtras)
{
    return ContextThemeWrapper::SendStickyOrderedBroadcast(intent, resultReceiver, scheduler,
        initialCode, initialData, initialExtras);
}

ECode Presentation::PresentationContextThemeWrapper::RemoveStickyBroadcast(
    /* [in] */ IIntent* intent)
{
    return ContextThemeWrapper::RemoveStickyBroadcast(intent);
}

ECode Presentation::PresentationContextThemeWrapper::SendStickyBroadcastAsUser(
    /* [in] */ IIntent* intent,
    /* [in] */ IUserHandle* user)
{
    return ContextThemeWrapper::SendStickyBroadcastAsUser(intent, user);
}

ECode Presentation::PresentationContextThemeWrapper::SendStickyOrderedBroadcastAsUser(
    /* [in] */ IIntent* intent,
    /* [in] */ IUserHandle* user,
    /* [in] */ IBroadcastReceiver* resultReceiver,
    /* [in] */ IHandler* scheduler,
    /* [in] */ Int32 initialCode,
    /* [in] */ const String& initialData,
    /* [in] */ IBundle* initialExtras)
{
    return ContextThemeWrapper::SendStickyOrderedBroadcastAsUser(intent, user, resultReceiver,
        scheduler, initialCode, initialData, initialExtras);
}

ECode Presentation::PresentationContextThemeWrapper::RemoveStickyBroadcastAsUser(
    /* [in] */ IIntent* intent,
    /* [in] */ IUserHandle* user)
{
    return ContextThemeWrapper::RemoveStickyBroadcastAsUser(intent, user);
}

ECode Presentation::PresentationContextThemeWrapper::RegisterReceiver(
    /* [in] */ IBroadcastReceiver* receiver,
    /* [in] */ IIntentFilter* filter,
    /* [out] */ IIntent** stickyIntent)
{
    return ContextThemeWrapper::RegisterReceiver(receiver, filter, stickyIntent);
}

ECode Presentation::PresentationContextThemeWrapper::RegisterReceiver(
    /* [in] */ IBroadcastReceiver* receiver,
    /* [in] */ IIntentFilter* filter,
    /* [in] */ const String& broadcastPermission,
    /* [in] */ IHandler* scheduler,
    /* [out] */ IIntent** stickyIntent)
{
    return ContextThemeWrapper::RegisterReceiver(receiver, filter, broadcastPermission, scheduler,
        stickyIntent);
}

ECode Presentation::PresentationContextThemeWrapper::RegisterReceiverAsUser(
    /* [in] */ IBroadcastReceiver* receiver,
    /* [in] */ IUserHandle* user,
    /* [in] */ IIntentFilter* filter,
    /* [in] */ const String& broadcastPermission,
    /* [in] */ IHandler* scheduler,
    /* [out] */ IIntent** stickyIntent)
{
    return ContextThemeWrapper::RegisterReceiverAsUser(receiver, user, filter, broadcastPermission,
        scheduler, stickyIntent);
}

ECode Presentation::PresentationContextThemeWrapper::UnregisterReceiver(
    /* [in] */ IBroadcastReceiver* receiver)
{
    return ContextThemeWrapper::UnregisterReceiver(receiver);
}

ECode Presentation::PresentationContextThemeWrapper::StartService(
    /* [in] */ IIntent* service,
    /* [out] */ IComponentName** name)
{
    return ContextThemeWrapper::StartService(service, name);
}

ECode Presentation::PresentationContextThemeWrapper::StopService(
    /* [in] */ IIntent* service,
    /* [out] */ Boolean* succeeded)
{
    return ContextThemeWrapper::StopService(service, succeeded);
}

ECode Presentation::PresentationContextThemeWrapper::StartServiceAsUser(
    /* [in] */ IIntent* service,
    /* [in] */ IUserHandle* user,
    /* [out] */ IComponentName** name)
{
    return ContextThemeWrapper::StartServiceAsUser(service, user, name);
}

ECode Presentation::PresentationContextThemeWrapper::StopServiceAsUser(
    /* [in] */ IIntent* service,
    /* [in] */ IUserHandle* user,
    /* [out] */ Boolean* succeeded)
{
    return ContextThemeWrapper::StopServiceAsUser(service, user, succeeded);
}

ECode Presentation::PresentationContextThemeWrapper::BindService(
    /* [in] */ IIntent* service,
    /* [in] */ IServiceConnection* conn,
    /* [in] */ Int32 flags,
    /* [out] */ Boolean* succeeded)
{
    return ContextThemeWrapper::BindService(service, conn, flags, succeeded);
}

ECode Presentation::PresentationContextThemeWrapper::BindService(
    /* [in] */ IIntent* service,
    /* [in] */ IServiceConnection* conn,
    /* [in] */ Int32 flags,
    /* [in] */ Int32 userHandle,
    /* [out] */ Boolean* succeeded)
{
    return ContextThemeWrapper::BindService(service, conn, flags, userHandle, succeeded);
}

ECode Presentation::PresentationContextThemeWrapper::UnbindService(
    /* [in] */ IServiceConnection* conn)
{
    return ContextThemeWrapper::UnbindService(conn);
}

ECode Presentation::PresentationContextThemeWrapper::StartInstrumentation(
    /* [in] */ IComponentName* className,
    /* [in] */ const String& profileFile,
    /* [in] */ IBundle* arguments,
    /* [out] */ Boolean* succeeded)
{
    return ContextThemeWrapper::StartInstrumentation(className, profileFile, arguments, succeeded);
}

// ECode Presentation::PresentationContextThemeWrapper::GetSystemService(
//     /* [in] */ const String& name,
//     /* [out] */ IInterface** object)
// {
//     return ContextThemeWrapper::GetSystemService(name, object);
// }

ECode Presentation::PresentationContextThemeWrapper::CheckPermission(
    /* [in] */ const String& permission,
    /* [in] */ Int32 pid,
    /* [in] */ Int32 uid,
    /* [out] */ Int32* permissionId)
{
    return ContextThemeWrapper::CheckPermission(permission, pid, uid, permissionId);
}

ECode Presentation::PresentationContextThemeWrapper::CheckCallingPermission(
    /* [in] */ const String& permission,
    /* [out] */ Int32* permissionId)
{
    return ContextThemeWrapper::CheckCallingPermission(permission, permissionId);
}

ECode Presentation::PresentationContextThemeWrapper::CheckCallingOrSelfPermission(
    /* [in] */ const String& permission,
    /* [out] */ Int32* permissionId)
{
    return ContextThemeWrapper::CheckCallingOrSelfPermission(permission, permissionId);
}

ECode Presentation::PresentationContextThemeWrapper::EnforcePermission(
    /* [in] */ const String& permission,
    /* [in] */ Int32 pid,
    /* [in] */ Int32 uid,
    /* [in] */ const String& message)
{
    return ContextThemeWrapper::EnforcePermission(permission, pid, uid, message);
}

ECode Presentation::PresentationContextThemeWrapper::EnforceCallingPermission(
    /* [in] */ const String& permission,
    /* [in] */ const String& message)
{
    return ContextThemeWrapper::EnforceCallingPermission(permission, message);
}

ECode Presentation::PresentationContextThemeWrapper::EnforceCallingOrSelfPermission(
    /* [in] */ const String& permission,
    /* [in] */ const String& message)
{
    return ContextThemeWrapper::EnforceCallingOrSelfPermission(permission, message);
}

ECode Presentation::PresentationContextThemeWrapper::GrantUriPermission(
    /* [in] */ const String& toPackage,
    /* [in] */ IUri* uri,
    /* [in] */ Int32 modeFlags)
{
    return ContextThemeWrapper::GrantUriPermission(toPackage, uri, modeFlags);
}

ECode Presentation::PresentationContextThemeWrapper::RevokeUriPermission(
    /* [in] */ IUri* uri,
    /* [in] */ Int32 modeFlags)
{
    return ContextThemeWrapper::RevokeUriPermission(uri, modeFlags);
}

ECode Presentation::PresentationContextThemeWrapper::CheckUriPermission(
    /* [in] */ IUri* uri,
    /* [in] */ Int32 pid,
    /* [in] */ Int32 uid,
    /* [in] */ Int32 modeFlags,
    /* [out] */ Int32* permissionId)
{
    return ContextThemeWrapper::CheckUriPermission(uri, pid, uid, modeFlags, permissionId);
}

ECode Presentation::PresentationContextThemeWrapper::CheckCallingUriPermission(
    /* [in] */ IUri* uri,
    /* [in] */ Int32 modeFlags,
    /* [out] */ Int32* permissionId)
{
    return ContextThemeWrapper::CheckCallingUriPermission(uri, modeFlags, permissionId);
}

ECode Presentation::PresentationContextThemeWrapper::CheckCallingOrSelfUriPermission(
    /* [in] */ IUri* uri,
    /* [in] */ Int32 modeFlags,
    /* [out] */ Int32* permissionId)
{
    return ContextThemeWrapper::CheckCallingOrSelfUriPermission(uri, modeFlags, permissionId);
}

ECode Presentation::PresentationContextThemeWrapper::CheckUriPermission(
    /* [in] */ IUri* uri,
    /* [in] */ const String& readPermission,
    /* [in] */ const String& writePermission,
    /* [in] */ Int32 pid,
    /* [in] */ Int32 uid,
    /* [in] */ Int32 modeFlags,
    /* [out] */ Int32* permissionId)
{
    return ContextThemeWrapper::CheckUriPermission(uri, readPermission, writePermission, pid, uid,
        modeFlags, permissionId);
}

ECode Presentation::PresentationContextThemeWrapper::EnforceUriPermission(
    /* [in] */ IUri* uri,
    /* [in] */ Int32 pid,
    /* [in] */ Int32 uid,
    /* [in] */ Int32 modeFlags,
    /* [in] */ const String& message)
{
    return ContextThemeWrapper::EnforceUriPermission(uri, pid, uid, modeFlags, message);
}

ECode Presentation::PresentationContextThemeWrapper::EnforceCallingUriPermission(
    /* [in] */ IUri* uri,
    /* [in] */ Int32 modeFlags,
    /* [in] */ const String& message)
{
    return ContextThemeWrapper::EnforceCallingUriPermission(uri, modeFlags, message);
}

ECode Presentation::PresentationContextThemeWrapper::EnforceCallingOrSelfUriPermission(
    /* [in] */ IUri* uri,
    /* [in] */ Int32 modeFlags,
    /* [in] */ const String& message)
{
    return ContextThemeWrapper::EnforceCallingOrSelfUriPermission(uri, modeFlags, message);
}

ECode Presentation::PresentationContextThemeWrapper::EnforceUriPermission(
    /* [in] */ IUri* uri,
    /* [in] */ const String& readPermission,
    /* [in] */ const String& writePermission,
    /* [in] */ Int32 pid,
    /* [in] */ Int32 uid,
    /* [in] */ Int32 modeFlags,
    /* [in] */ const String& message)
{
    return ContextThemeWrapper::EnforceUriPermission(uri, readPermission, writePermission, pid,
        uid, modeFlags, message);
}

ECode Presentation::PresentationContextThemeWrapper::CreatePackageContext(
    /* [in] */ const String& packageName,
    /* [in] */ Int32 flags,
    /* [out] */ IContext** ctx)
{
    return ContextThemeWrapper::CreatePackageContext(packageName, flags, ctx);
}

ECode Presentation::PresentationContextThemeWrapper::CreatePackageContextAsUser(
    /* [in] */ const String& packageName,
    /* [in] */ Int32 flags,
    /* [in] */ IUserHandle* user,
    /* [out] */ IContext** ctx)
{
    return ContextThemeWrapper::CreatePackageContextAsUser(packageName, flags, user, ctx);
}

ECode Presentation::PresentationContextThemeWrapper::CreateConfigurationContext(
    /* [in] */ IConfiguration* overrideConfiguration,
    /* [out] */ IContext** ctx)
{
    return ContextThemeWrapper::CreateConfigurationContext(overrideConfiguration, ctx);
}

ECode Presentation::PresentationContextThemeWrapper::CreateDisplayContext(
    /* [in] */ IDisplay* display,
    /* [out] */ IContext** ctx)
{
    return ContextThemeWrapper::CreateDisplayContext(display, ctx);
}

ECode Presentation::PresentationContextThemeWrapper::GetCompatibilityInfo(
    /* [in] */ Int32 displayId,
    /* [out] */ ICompatibilityInfoHolder** infoHolder)
{
    return ContextThemeWrapper::GetCompatibilityInfo(displayId, infoHolder);
}

ECode Presentation::PresentationContextThemeWrapper::IsRestricted(
    /* [out] */ Boolean* isRestricted)
{
    return ContextThemeWrapper::IsRestricted(isRestricted);
}

Presentation::Presentation()
{
    mDisplayListener = new PresentationDisplayListener(this);
    mHandler = new PresentationHandler(this);
}

Presentation::~Presentation()
{
}

Presentation::Presentation(
    /* [in] */ IContext * outerContext,
    /* [in] */ IDisplay * display)
{
    Init(outerContext, display, 0);
}

Presentation::Presentation(
    /* [in] */ IContext * outerContext,
    /* [in] */ IDisplay * display,
    /* [in] */ Int32 theme)
{
    Init(outerContext, display, theme);
}

ECode Presentation::Init(
    /* [in] */ IContext * outerContext,
    /* [in] */ IDisplay * display,
    /* [in] */ Int32 theme)
{
    Dialog::Init(CreatePresentationContext(outerContext, display, theme), theme, FALSE);

    mDisplay = display;

    GetContext()->GetSystemService(IContext::DISPLAY_SERVICE, (IInterface**)&mDisplayManager);

    GetWindow()->SetGravity(IGravity::FILL);
    SetCanceledOnTouchOutside(FALSE);
    return NOERROR;
}

/**
 * Gets the {@link Display} that this presentation appears on.
 *
 * @return The display.
 */
ECode Presentation::GetDisplay(
    /* [out] */ IDisplay **display)
{
    *display = mDisplay;
    return NOERROR;
}

/**
 * Gets the {@link Resources} that should be used to inflate the layout of this presentation.
 * This resources object has been configured according to the metrics of the
 * display that the presentation appears on.
 *
 * @return The presentation resources object.
 */
ECode Presentation::GetResources(
    /* [out] */ IResources **resources)
{
    return GetContext()->GetResources(resources);
}

/**
 * Called by the system when the {@link Display} to which the presentation
 * is attached has been removed.
 *
 * The system automatically calls {@link #cancel} to dismiss the presentation
 * after sending this event.
 *
 * @see #getDisplay
 */
ECode Presentation::OnDisplayRemoved()
{
    return NOERROR;
}

/**
 * Called by the system when the properties of the {@link Display} to which
 * the presentation is attached have changed.
 *
 * If the display metrics have changed (for example, if the display has been
 * resized or rotated), then the system automatically calls
 * {@link #cancel} to dismiss the presentation.
 *
 * @see #getDisplay
 */
ECode Presentation::OnDisplayChanged()
{
    return NOERROR;
}

/**
 * Inherited from {@link Dialog#show}. Will throw
 * {@link android.view.WindowManager.InvalidDisplayException} if the specified secondary
 * {@link Display} can't be found.
 */
//@Override
// ECode Presentation::Show()
// {
//     return Dialog::Show();
// }

void Presentation::OnStart()
{
    Dialog::OnStart();
    mDisplayManager->RegisterDisplayListener(mDisplayListener, mHandler);

    // Since we were not watching for display changes until just now, there is a
    // chance that the display metrics have changed.  If so, we will need to
    // dismiss the presentation immediately.  This case is expected
    // to be rare but surprising, so we'll write a log message about it.
    if (!IsConfigurationStillValid()) {
        Logger::I(TAG, "Presentation is being immediately dismissed because the display metrics have changed since it was created.");
        Boolean result;
        mHandler->SendEmptyMessage(IPresentation::MSG_CANCEL, &result);
    }
}

void Presentation::OnStop()
{
    mDisplayManager->UnregisterDisplayListener(mDisplayListener);
    Dialog::OnStop();
}

void Presentation::HandleDisplayRemoved()
{
    OnDisplayRemoved();
    Cancel();
}

void Presentation::HandleDisplayChanged()
{
    OnDisplayChanged();

    // We currently do not support configuration changes for presentations
    // (although we could add that feature with a bit more work).
    // If the display metrics have changed in any way then the current configuration
    // is invalid and the application must recreate the presentation to get
    // a new context.
    if (!IsConfigurationStillValid()) {
        Cancel();
    }
}

Boolean Presentation::IsConfigurationStillValid()
{
    AutoPtr<IDisplayMetrics> dm;
    CDisplayMetrics::New((IDisplayMetrics**)&dm);
    mDisplay->GetMetrics(dm);
    AutoPtr<IResources> res;
    GetResources((IResources**)&res);
    AutoPtr<IDisplayMetrics> dm2;
    res->GetDisplayMetrics((IDisplayMetrics**)&dm2);
    Boolean result;
    dm->EqualsPhysical(dm2, &result);
    return result;
}

AutoPtr<IContext> Presentation::CreatePresentationContext(
        /* [in] */ IContext* outerContext,
        /* [in] */ IDisplay* display,
        /* [in] */ Int32 theme)
{
    if (outerContext == NULL) {
        Logger::E(TAG, "outerContext must not be null");
        return NULL;    // return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (display == NULL) {
        Logger::E(TAG, "display must not be null");
        return NULL;    // return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    AutoPtr<IContext> displayContext;
    outerContext->CreateDisplayContext(display, (IContext**)&displayContext);
    if (theme == 0) {
        AutoPtr<ITypedValue> outValue;
        CTypedValue::New((ITypedValue**)&outValue);
        AutoPtr<IResourcesTheme> currentTheme;
        displayContext->GetTheme((IResourcesTheme**)&currentTheme);
        Boolean isFounded;
        currentTheme->ResolveAttribute(R::attr::presentationTheme,
                outValue, TRUE, &isFounded);
        outValue->GetResourceId(&theme);
    }

    // Derive the display's window manager from the outer window manager.
    // We do this because the outer window manager have some extra information
    // such as the parent window, which is important if the presentation uses
    // an application window type.
    AutoPtr<IWindowManager> outerWindowManager;
    ASSERT_SUCCEEDED(outerContext->GetSystemService(IContext::WINDOW_SERVICE, (IInterface**)&outerWindowManager));
    AutoPtr<IWindowManager> displayWindowManager;
    displayWindowManager = ((WindowManagerImpl*)outerWindowManager.Get())->CreatePresentationWindowManager(display);
    AutoPtr<IContext> ctx = new PresentationContextThemeWrapper(displayContext, theme, displayWindowManager);
    return ctx;
}

}
}
}
