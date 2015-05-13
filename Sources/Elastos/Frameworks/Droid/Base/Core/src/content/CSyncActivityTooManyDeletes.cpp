
#include "content/CSyncActivityTooManyDeletes.h"
// #include "content/CContentResolverHelper.h"
#include "os/CBundle.h"

using Elastos::Droid::Widget::IListAdapter;

namespace Elastos {
namespace Droid {
namespace Content {

CSyncActivityTooManyDeletes::CSyncActivityTooManyDeletes()
    : mNumDeletes(0)
    , mAccount(NULL)
{}

CSyncActivityTooManyDeletes::~CSyncActivityTooManyDeletes()
{}

ECode CSyncActivityTooManyDeletes::GetAssets(
    /* [out] */ IAssetManager** assetManager)
{
    return Activity::GetAssets(assetManager);
}

ECode CSyncActivityTooManyDeletes::GetResources(
    /* [out] */ IResources** resources)
{
    return Activity::GetResources(resources);
}

ECode CSyncActivityTooManyDeletes::GetPackageManager(
    /* [out] */ IPackageManager** packageManager)
{
    return Activity::GetPackageManager(packageManager);
}

ECode CSyncActivityTooManyDeletes::GetContentResolver(
    /* [out] */ IContentResolver** resolver)
{
    return Activity::GetContentResolver(resolver);
}

ECode CSyncActivityTooManyDeletes::GetMainLooper(
    /* [out] */ ILooper** looper)
{
    return Activity::GetMainLooper(looper);
}

ECode CSyncActivityTooManyDeletes::GetApplicationContext(
    /* [out] */ IContext** ctx)
{
    return Activity::GetApplicationContext(ctx);
}

ECode CSyncActivityTooManyDeletes::RegisterComponentCallbacks(
    /* [in] */ IComponentCallbacks* componentCallback)
{
    return Activity::RegisterComponentCallbacks(componentCallback);
}

ECode CSyncActivityTooManyDeletes::UnregisterComponentCallbacks(
    /* [in] */ IComponentCallbacks* componentCallback)
{
    return Activity::UnregisterComponentCallbacks(componentCallback);
}

ECode CSyncActivityTooManyDeletes::GetText(
    /* [in] */ Int32 resId,
    /* [out] */ ICharSequence** text)
{
    return Activity::GetText(resId, text);
}

ECode CSyncActivityTooManyDeletes::GetString(
    /* [in] */ Int32 resId,
    /* [out] */ String* str)
{
    return Activity::GetString(resId, str);
}

ECode CSyncActivityTooManyDeletes::GetStringEx(
    /* [in] */ Int32 resId,
    /* [in] */ ArrayOf<IInterface *>* formatArgs,
    /* [out] */ String* str)
{
//    return Activity::GetStringEx(resId, formatArgs, str);
    return E_NOT_IMPLEMENTED;
}

ECode CSyncActivityTooManyDeletes::SetTheme(
    /* [in] */ Int32 resId)
{
    return Activity::SetTheme(resId);
}

ECode CSyncActivityTooManyDeletes::GetThemeResId(
    /* [out] */ Int32* resId)
{
    return Activity::GetThemeResId(resId);
}

ECode CSyncActivityTooManyDeletes::GetTheme(
    /* [out] */ IResourcesTheme** theme)
{
    return Activity::GetTheme(theme);
}

ECode CSyncActivityTooManyDeletes::ObtainStyledAttributes(
    /* [in] */ ArrayOf<Int32>* attrs,
    /* [out] */ ITypedArray** styles)
{
    return Activity::ObtainStyledAttributes(attrs, styles);
}

ECode CSyncActivityTooManyDeletes::ObtainStyledAttributesEx(
    /* [in] */ Int32 resid,
    /* [in] */ ArrayOf<Int32>* attrs,
    /* [out] */ ITypedArray** styles)
{
    return Activity::ObtainStyledAttributesEx(resid, attrs, styles);
}

ECode CSyncActivityTooManyDeletes::ObtainStyledAttributesEx2(
    /* [in] */ IAttributeSet* set,
    /* [in] */ ArrayOf<Int32>* attrs,
    /* [out] */ ITypedArray** styles)
{
    return Activity::ObtainStyledAttributesEx2(set, attrs, styles);
}

ECode CSyncActivityTooManyDeletes::ObtainStyledAttributesEx3(
    /* [in] */ IAttributeSet* set,
    /* [in] */ ArrayOf<Int32>* attrs,
    /* [in] */ Int32 defStyleAttr,
    /* [in] */ Int32 defStyleRes,
    /* [out] */ ITypedArray** styles)
{
    return Activity::ObtainStyledAttributesEx3(set, attrs, defStyleAttr, defStyleRes, styles);
}

ECode CSyncActivityTooManyDeletes::GetClassLoader(
        /* [out] */ IClassLoader** loader)
{
//    return Activity::GetClassLoader(loader);
    return E_NOT_IMPLEMENTED;
}

ECode CSyncActivityTooManyDeletes::GetPackageName(
    /* [out] */ String* packageName)
{
    return Activity::GetPackageName(packageName);
}

ECode CSyncActivityTooManyDeletes::GetApplicationInfo(
    /* [out] */ IApplicationInfo** info)
{
    return Activity::GetApplicationInfo(info);
}

ECode CSyncActivityTooManyDeletes::GetPackageResourcePath(
    /* [out] */ String* path)
{
    return Activity::GetPackageResourcePath(path);
}

ECode CSyncActivityTooManyDeletes::GetPackageCodePath(
    /* [out] */ String* codePath)
{
    return Activity::GetPackageCodePath(codePath);
}

ECode CSyncActivityTooManyDeletes::GetSharedPrefsFile(
    /* [in] */ const String& name,
    /* [out] */ IFile** file)
{
    return Activity::GetSharedPrefsFile(name, file);
}

ECode CSyncActivityTooManyDeletes::GetSharedPreferences(
    /* [in] */ const String& name,
    /* [in] */ Int32 mode,
    /* [out] */ ISharedPreferences** prefs)
{
    return Activity::GetSharedPreferences(name, mode, prefs);
}

ECode CSyncActivityTooManyDeletes::OpenFileInput(
    /* [in] */ const String& name,
    /* [out] */ IFileInputStream** fileInputStream)
{
    return Activity::OpenFileInput(name, fileInputStream);
}

ECode CSyncActivityTooManyDeletes::OpenFileOutput(
    /* [in] */ const String& name,
    /* [in] */ Int32 mode,
    /* [out] */ IFileOutputStream** fileOutputStream)
{
    return Activity::OpenFileOutput(name, mode, fileOutputStream);
}

ECode CSyncActivityTooManyDeletes::DeleteFile(
    /* [in] */ const String& name,
    /* [out] */ Boolean* succeeded)
{
    return Activity::DeleteFile(name, succeeded);
}

ECode CSyncActivityTooManyDeletes::GetFileStreamPath(
    /* [in] */ const String& name,
    /* [out] */ IFile** file)
{
    return Activity::GetFileStreamPath(name, file);
}

ECode CSyncActivityTooManyDeletes::GetFilesDir(
    /* [out] */ IFile** filesDir)
{
    return Activity::GetFilesDir(filesDir);
}

ECode CSyncActivityTooManyDeletes::GetExternalFilesDir(
    /* [in] */ const String& type,
    /* [out] */ IFile** filesDir)
{
    return Activity::GetExternalFilesDir(type, filesDir);
}

ECode CSyncActivityTooManyDeletes::GetObbDir(
    /* [out] */ IFile** obbDir)
{
    return Activity::GetObbDir(obbDir);
}

ECode CSyncActivityTooManyDeletes::GetCacheDir(
    /* [out] */ IFile** cacheDir)
{
    return Activity::GetCacheDir(cacheDir);
}

ECode CSyncActivityTooManyDeletes::GetExternalCacheDir(
    /* [out] */ IFile** externalDir)
{
    return Activity::GetExternalCacheDir(externalDir);
}

ECode CSyncActivityTooManyDeletes::GetFileList(
    /* [out, callee] */ ArrayOf<String>** fileList)
{
    return Activity::GetFileList(fileList);
}

ECode CSyncActivityTooManyDeletes::GetDir(
    /* [in] */ const String& name,
    /* [in] */ Int32 mode,
    /* [out] */ IFile** dir)
{
    return Activity::GetDir(name, mode, dir);
}

ECode CSyncActivityTooManyDeletes::OpenOrCreateDatabase(
    /* [in] */ const String& name,
    /* [in] */ Int32 mode,
    /* [in] */ ISQLiteDatabaseCursorFactory* factory,
    /* [out] */ ISQLiteDatabase** sqliteDB)
{
    return Activity::OpenOrCreateDatabase(name, mode, factory, sqliteDB);
}

ECode CSyncActivityTooManyDeletes::OpenOrCreateDatabaseEx(
    /* [in] */ const String& name,
    /* [in] */ Int32 mode,
    /* [in] */ ISQLiteDatabaseCursorFactory* factory,
    /* [in] */ IDatabaseErrorHandler* errorHandler,
    /* [out] */ ISQLiteDatabase** sqliteDB)
{
    return Activity::OpenOrCreateDatabaseEx(name, mode, factory, errorHandler, sqliteDB);
}

ECode CSyncActivityTooManyDeletes::DeleteDatabase(
    /* [in] */ const String& name,
    /* [out] */ Boolean* succeeded)
{
    return Activity::DeleteDatabase(name, succeeded);
}

ECode CSyncActivityTooManyDeletes::GetDatabasePath(
    /* [in] */ const String& name,
    /* [out] */ IFile** path)
{
    return Activity::GetDatabasePath(name, path);
}

ECode CSyncActivityTooManyDeletes::GetDatabaseList(
    /* [out, callee] */ ArrayOf<String>** databaseList)
{
    return Activity::GetDatabaseList(databaseList);
}

ECode CSyncActivityTooManyDeletes::GetWallpaper(
    /* [out] */ IDrawable** drawable)
{
    return Activity::GetWallpaper(drawable);
}

ECode CSyncActivityTooManyDeletes::PeekWallpaper(
    /* [out] */ IDrawable** drawable)
{
    return Activity::PeekWallpaper(drawable);
}

ECode CSyncActivityTooManyDeletes::GetWallpaperDesiredMinimumWidth(
    /* [out] */ Int32* minWidth)
{
    return Activity::GetWallpaperDesiredMinimumWidth(minWidth);
}

ECode CSyncActivityTooManyDeletes::GetWallpaperDesiredMinimumHeight(
    /* [out] */ Int32* minHeight)
{
    return Activity::GetWallpaperDesiredMinimumHeight(minHeight);
}

ECode CSyncActivityTooManyDeletes::SetWallpaper(
    /* [in] */ IBitmap* bitmap)
{
    return Activity::SetWallpaper(bitmap);
}

ECode CSyncActivityTooManyDeletes::SetWallpaperEx(
    /* [in] */ IInputStream* data)
{
    return Activity::SetWallpaperEx(data);
}

ECode CSyncActivityTooManyDeletes::ClearWallpaper()
{
    return Activity::ClearWallpaper();
}

ECode CSyncActivityTooManyDeletes::StartActivity(
    /* [in] */ IIntent* intent)
{
    return Activity::StartActivity(intent);
}

ECode CSyncActivityTooManyDeletes::StartActivityAsUser(
    /* [in] */ IIntent* intent,
    /* [in] */ IUserHandle* user)
{
    return Activity::StartActivityAsUser(intent, user);
}

ECode CSyncActivityTooManyDeletes::StartActivityEx(
    /* [in] */ IIntent* intent,
    /* [in] */ IBundle* options)
{
    return Activity::StartActivityEx(intent, options);
}

ECode CSyncActivityTooManyDeletes::StartActivityAsUserEx(
    /* [in] */ IIntent* intent,
    /* [in] */ IBundle* options,
    /* [in] */ IUserHandle* userId)
{
    return Activity::StartActivityAsUserEx(intent, options, userId);
}

ECode CSyncActivityTooManyDeletes::StartActivities(
    /* [in] */ ArrayOf<IIntent *>* intents)
{
    return Activity::StartActivities(intents);
}

ECode CSyncActivityTooManyDeletes::StartActivitiesEx(
    /* [in] */ ArrayOf<IIntent *>* intents,
    /* [in] */ IBundle* options)
{
    return Activity::StartActivitiesEx(intents, options);
}

ECode CSyncActivityTooManyDeletes::StartActivitiesAsUser(
    /* [in] */ ArrayOf<IIntent *>* intents,
    /* [in] */ IBundle* options,
    /* [in] */ IUserHandle* userHandle)
{
    return Activity::StartActivitiesAsUser(intents, options, userHandle);
}

ECode CSyncActivityTooManyDeletes::StartIntentSender(
    /* [in] */ IIntentSender* intent,
    /* [in] */ IIntent* fillInIntent,
    /* [in] */ Int32 flagsMask,
    /* [in] */ Int32 flagsValues,
    /* [in] */ Int32 extraFlags)
{
    return Activity::StartIntentSender(intent, fillInIntent, flagsMask, flagsValues, extraFlags);
}

ECode CSyncActivityTooManyDeletes::StartIntentSenderEx(
    /* [in] */ IIntentSender* intent,
    /* [in] */ IIntent* fillInIntent,
    /* [in] */ Int32 flagsMask,
    /* [in] */ Int32 flagsValues,
    /* [in] */ Int32 extraFlags,
    /* [in] */ IBundle* options)
{
    return Activity::StartIntentSenderEx(intent, fillInIntent, flagsMask, flagsValues, extraFlags, options);
}

ECode CSyncActivityTooManyDeletes::SendBroadcast(
    /* [in] */ IIntent* intent)
{
    return Activity::SendBroadcast(intent);
}

ECode CSyncActivityTooManyDeletes::SendBroadcastEx(
    /* [in] */ IIntent* intent,
    /* [in] */ const String& receiverPermission)
{
    return Activity::SendBroadcastEx(intent, receiverPermission);
}

ECode CSyncActivityTooManyDeletes::SendOrderedBroadcast(
    /* [in] */ IIntent* intent,
    /* [in] */ const String& receiverPermission)
{
    return Activity::SendOrderedBroadcast(intent, receiverPermission);
}

ECode CSyncActivityTooManyDeletes::SendOrderedBroadcastEx(
    /* [in] */ IIntent* intent,
    /* [in] */ const String& receiverPermission,
    /* [in] */ IBroadcastReceiver* resultReceiver,
    /* [in] */ IHandler* scheduler,
    /* [in] */ Int32 initialCode,
    /* [in] */ const String& initialData,
    /* [in] */ IBundle* initialExtras)
{
    return Activity::SendOrderedBroadcastEx(intent, receiverPermission, resultReceiver, scheduler,
            initialCode, initialData, initialExtras);
}

ECode CSyncActivityTooManyDeletes::SendBroadcastAsUser(
    /* [in] */ IIntent* intent,
    /* [in] */ IUserHandle* user)
{
    return Activity::SendBroadcastAsUser(intent, user);
}

ECode CSyncActivityTooManyDeletes::SendBroadcastAsUserEx(
    /* [in] */ IIntent* intent,
    /* [in] */ IUserHandle* user,
    /* [in] */ const String& receiverPermission)
{
    return Activity::SendBroadcastAsUserEx(intent, user, receiverPermission);
}

ECode CSyncActivityTooManyDeletes::SendOrderedBroadcastAsUser(
    /* [in] */ IIntent* intent,
    /* [in] */ IUserHandle* user,
    /* [in] */ const String& receiverPermission,
    /* [in] */ IBroadcastReceiver* resultReceiver,
    /* [in] */ IHandler* scheduler,
    /* [in] */ Int32 initialCode,
    /* [in] */ const String& initialData,
    /* [in] */ IBundle* initialExtras)
{
    return Activity::SendOrderedBroadcastAsUser(intent, user, receiverPermission, resultReceiver,
            scheduler, initialCode, initialData, initialExtras);
}

ECode CSyncActivityTooManyDeletes::SendStickyBroadcast(
    /* [in] */ IIntent* intent)
{
    return Activity::SendStickyBroadcast(intent);
}

ECode CSyncActivityTooManyDeletes::SendStickyOrderedBroadcast(
    /* [in] */ IIntent* intent,
    /* [in] */ IBroadcastReceiver* resultReceiver,
    /* [in] */ IHandler* scheduler,
    /* [in] */ Int32 initialCode,
    /* [in] */ const String& initialData,
    /* [in] */ IBundle* initialExtras)
{
    return Activity::SendStickyOrderedBroadcast(intent, resultReceiver, scheduler, initialCode,
            initialData, initialExtras);
}

ECode CSyncActivityTooManyDeletes::RemoveStickyBroadcast(
    /* [in] */ IIntent* intent)
{
    return Activity::RemoveStickyBroadcast(intent);
}

ECode CSyncActivityTooManyDeletes::SendStickyBroadcastAsUser(
    /* [in] */ IIntent* intent,
    /* [in] */ IUserHandle* user)
{
    return Activity::SendStickyBroadcastAsUser(intent, user);
}

ECode CSyncActivityTooManyDeletes::SendStickyOrderedBroadcastAsUser(
    /* [in] */ IIntent* intent,
    /* [in] */ IUserHandle* user,
    /* [in] */ IBroadcastReceiver* resultReceiver,
    /* [in] */ IHandler* scheduler,
    /* [in] */ Int32 initialCode,
    /* [in] */ const String& initialData,
    /* [in] */ IBundle* initialExtras)
{
    return Activity::SendStickyOrderedBroadcastAsUser(intent, user, resultReceiver, scheduler,
            initialCode, initialData, initialExtras);
}

ECode CSyncActivityTooManyDeletes::RemoveStickyBroadcastAsUser(
    /* [in] */ IIntent* intent,
    /* [in] */ IUserHandle* user)
{
    return Activity::RemoveStickyBroadcastAsUser(intent, user);
}

ECode CSyncActivityTooManyDeletes::RegisterReceiver(
    /* [in] */ IBroadcastReceiver* receiver,
    /* [in] */ IIntentFilter* filter,
    /* [out] */ IIntent** stickyIntent)
{
    return Activity::RegisterReceiver(receiver, filter, stickyIntent);
}

ECode CSyncActivityTooManyDeletes::RegisterReceiverEx(
    /* [in] */ IBroadcastReceiver* receiver,
    /* [in] */ IIntentFilter* filter,
    /* [in] */ const String& broadcastPermission,
    /* [in] */ IHandler* scheduler,
    /* [out] */ IIntent** stickyIntent)
{
    return Activity::RegisterReceiverEx(receiver, filter, broadcastPermission, scheduler, stickyIntent);
}

ECode CSyncActivityTooManyDeletes::RegisterReceiverAsUser(
    /* [in] */ IBroadcastReceiver* receiver,
    /* [in] */ IUserHandle* user,
    /* [in] */ IIntentFilter* filter,
    /* [in] */ const String& broadcastPermission,
    /* [in] */ IHandler* scheduler,
    /* [out] */ IIntent** stickyIntent)
{
    return Activity::RegisterReceiverAsUser(receiver, user, filter, broadcastPermission, scheduler, stickyIntent);
}

ECode CSyncActivityTooManyDeletes::UnregisterReceiver(
    /* [in] */ IBroadcastReceiver* receiver)
{
    return Activity::UnregisterReceiver(receiver);
}

ECode CSyncActivityTooManyDeletes::StartService(
    /* [in] */ IIntent* service,
    /* [out] */ IComponentName** name)
{
    return Activity::StartService(service, name);
}

ECode CSyncActivityTooManyDeletes::StopService(
    /* [in] */ IIntent* service,
    /* [out] */ Boolean* succeeded)
{
    return Activity::StopService(service, succeeded);
}

ECode CSyncActivityTooManyDeletes::StartServiceAsUser(
    /* [in] */ IIntent* service,
    /* [in] */ IUserHandle* user,
    /* [out] */ IComponentName** name)
{
    return Activity::StartServiceAsUser(service, user, name);
}

ECode CSyncActivityTooManyDeletes::StopServiceAsUser(
    /* [in] */ IIntent* service,
    /* [in] */ IUserHandle* user,
    /* [out] */ Boolean* succeeded)
{
    return Activity::StopServiceAsUser(service, user, succeeded);
}

ECode CSyncActivityTooManyDeletes::BindService(
    /* [in] */ IIntent* service,
    /* [in] */ IServiceConnection* conn,
    /* [in] */ Int32 flags,
    /* [out] */ Boolean* succeeded)
{
    return Activity::BindService(service, conn, flags, succeeded);
}

ECode CSyncActivityTooManyDeletes::BindServiceEx(
    /* [in] */ IIntent* service,
    /* [in] */ IServiceConnection* conn,
    /* [in] */ Int32 flags,
    /* [in] */ Int32 userHandle,
    /* [out] */ Boolean* succeeded)
{
    return Activity::BindServiceEx(service, conn, flags, userHandle, succeeded);
}

ECode CSyncActivityTooManyDeletes::UnbindService(
    /* [in] */ IServiceConnection* conn)
{
    return Activity::UnbindService(conn);
}

ECode CSyncActivityTooManyDeletes::StartInstrumentation(
    /* [in] */ IComponentName* className,
    /* [in] */ const String& profileFile,
    /* [in] */ IBundle* arguments,
    /* [out] */ Boolean* succeeded)
{
    return Activity::StartInstrumentation(className, profileFile, arguments, succeeded);
}

ECode CSyncActivityTooManyDeletes::GetSystemService(
    /* [in] */ const String& name,
    /* [out] */ IInterface** object)
{
    return Activity::GetSystemService(name, object);
}

ECode CSyncActivityTooManyDeletes::CheckPermission(
    /* [in] */ const String& permission,
    /* [in] */ Int32 pid,
    /* [in] */ Int32 uid,
    /* [out] */ Int32* result)
{
    return Activity::CheckPermission(permission, pid, uid, result);
}

ECode CSyncActivityTooManyDeletes::CheckCallingPermission(
    /* [in] */ const String& permission,
    /* [out] */ Int32* result)
{
    return Activity::CheckCallingPermission(permission, result);
}

ECode CSyncActivityTooManyDeletes::CheckCallingOrSelfPermission(
    /* [in] */ const String& permission,
    /* [out] */ Int32* result)
{
    return Activity::CheckCallingOrSelfPermission(permission, result);
}

ECode CSyncActivityTooManyDeletes::EnforcePermission(
    /* [in] */ const String& permission,
    /* [in] */ Int32 pid,
    /* [in] */ Int32 uid,
    /* [in] */ const String& message)
{
    return Activity::EnforcePermission(permission, pid, uid, message);
}

ECode CSyncActivityTooManyDeletes::EnforceCallingPermission(
    /* [in] */ const String& permission,
    /* [in] */ const String& message)
{
    return Activity::EnforceCallingPermission(permission, message);
}

ECode CSyncActivityTooManyDeletes::EnforceCallingOrSelfPermission(
    /* [in] */ const String& permission,
    /* [in] */ const String& message)
{
    return Activity::EnforceCallingOrSelfPermission(permission, message);
}

ECode CSyncActivityTooManyDeletes::GrantUriPermission(
    /* [in] */ const String& toPackage,
    /* [in] */ IUri* uri,
    /* [in] */ Int32 modeFlags)
{
    return Activity::GrantUriPermission(toPackage, uri, modeFlags);
}

ECode CSyncActivityTooManyDeletes::RevokeUriPermission(
    /* [in] */ IUri* uri,
    /* [in] */ Int32 modeFlags)
{
    return Activity::RevokeUriPermission(uri, modeFlags);
}

ECode CSyncActivityTooManyDeletes::CheckUriPermission(
    /* [in] */ IUri* uri,
    /* [in] */ Int32 pid,
    /* [in] */ Int32 uid,
    /* [in] */ Int32 modeFlags,
    /* [out] */ Int32* result)
{
    return Activity::CheckUriPermission(uri, pid, uid, modeFlags, result);
}

ECode CSyncActivityTooManyDeletes::CheckCallingUriPermission(
    /* [in] */ IUri* uri,
    /* [in] */ Int32 modeFlags,
    /* [out] */ Int32* result)
{
    return Activity::CheckCallingUriPermission(uri, modeFlags, result);
}

ECode CSyncActivityTooManyDeletes::CheckCallingOrSelfUriPermission(
    /* [in] */ IUri* uri,
    /* [in] */ Int32 modeFlags,
    /* [out] */ Int32* result)
{
    return Activity::CheckCallingOrSelfUriPermission(uri, modeFlags, result);
}

ECode CSyncActivityTooManyDeletes::CheckUriPermissionEx(
    /* [in] */ IUri* uri,
    /* [in] */ const String& readPermission,
    /* [in] */ const String& writePermission,
    /* [in] */ Int32 pid,
    /* [in] */ Int32 uid,
    /* [in] */ Int32 modeFlags,
    /* [out] */ Int32* result)
{
    return Activity::CheckUriPermissionEx(uri, readPermission, writePermission, pid, uid, modeFlags, result);
}

ECode CSyncActivityTooManyDeletes::EnforceUriPermission(
    /* [in] */ IUri* uri,
    /* [in] */ Int32 pid,
    /* [in] */ Int32 uid,
    /* [in] */ Int32 modeFlags,
    /* [in] */ const String& message)
{
    return Activity::EnforceUriPermission(uri, pid, uid, modeFlags, message);
}

ECode CSyncActivityTooManyDeletes::EnforceCallingUriPermission(
    /* [in] */ IUri* uri,
    /* [in] */ Int32 modeFlags,
    /* [in] */ const String& message)
{
    return Activity::EnforceCallingUriPermission(uri, modeFlags, message);
}

ECode CSyncActivityTooManyDeletes::EnforceCallingOrSelfUriPermission(
    /* [in] */ IUri* uri,
    /* [in] */ Int32 modeFlags,
    /* [in] */ const String& message)
{
    return Activity::EnforceCallingOrSelfUriPermission(uri, modeFlags, message);
}

ECode CSyncActivityTooManyDeletes::EnforceUriPermissionEx(
    /* [in] */ IUri* uri,
    /* [in] */ const String& readPermission,
    /* [in] */ const String& writePermission,
    /* [in] */ Int32 pid,
    /* [in] */ Int32 uid,
    /* [in] */ Int32 modeFlags,
    /* [in] */ const String& message)
{
    return Activity::EnforceUriPermissionEx(uri, readPermission, writePermission, pid, uid, modeFlags, message);
}

ECode CSyncActivityTooManyDeletes::CreatePackageContext(
    /* [in] */ const String& packageName,
    /* [in] */ Int32 flags,
    /* [out] */ IContext** ctx)
{
    return Activity::CreatePackageContext(packageName, flags, ctx);
}

ECode CSyncActivityTooManyDeletes::CreatePackageContextAsUser(
    /* [in] */ const String& packageName,
    /* [in] */ Int32 flags,
    /* [in] */ IUserHandle* user,
    /* [out] */ IContext** ctx)
{
    return Activity::CreatePackageContextAsUser(packageName, flags, user, ctx);
}

ECode CSyncActivityTooManyDeletes::CreateConfigurationContext(
    /* [in] */ IConfiguration* overrideConfiguration,
    /* [out] */ IContext** ctx)
{
    return Activity::CreateConfigurationContext(overrideConfiguration, ctx);
}

ECode CSyncActivityTooManyDeletes::CreateDisplayContext(
    /* [in] */ IDisplay* display,
    /* [out] */ IContext** ctx)
{
    return Activity::CreateDisplayContext(display, ctx);
}

ECode CSyncActivityTooManyDeletes::GetCompatibilityInfo(
    /* [in] */ Int32 displayId,
    /* [out] */ ICompatibilityInfoHolder** infoHolder)
{
    return Activity::GetCompatibilityInfo(displayId, infoHolder);
}

ECode CSyncActivityTooManyDeletes::IsRestricted(
    /* [out] */ Boolean* isRestricted)
{
    return Activity::IsRestricted(isRestricted);
}

ECode CSyncActivityTooManyDeletes::ApplyOverrideConfiguration(
    /* [in] */ IConfiguration* overrideConfiguration)
{
    return Activity::ApplyOverrideConfiguration(overrideConfiguration);
}

ECode CSyncActivityTooManyDeletes::GetBaseContext(
    /* [out] */ IContext** context)
{
    return Activity::GetBaseContext(context);
}

ECode CSyncActivityTooManyDeletes::ToString(
    /* [out] */ String* str)
{
    return Activity::ToString(str);
}

ECode CSyncActivityTooManyDeletes::GetIntent(
    /* [out] */ IIntent** intent)
{
    return Activity::GetIntent(intent);
}

ECode CSyncActivityTooManyDeletes::SetIntent(
    /* [in] */ IIntent* newIntent)
{
    return Activity::SetIntent(newIntent);
}

ECode CSyncActivityTooManyDeletes::GetApplication(
    /* [out] */ IApplication** application)
{
    return Activity::GetApplication(application);
}

ECode CSyncActivityTooManyDeletes::IsChild(
    /* [out] */ Boolean* isChild)
{
    return Activity::IsChild(isChild);
}

ECode CSyncActivityTooManyDeletes::GetParent(
    /* [out] */ IActivity** parent)
{
    return Activity::GetParent(parent);
}

ECode CSyncActivityTooManyDeletes::GetWindowManager(
    /* [out] */ IWindowManager** mgr)
{
    return Activity::GetWindowManager(mgr);
}

ECode CSyncActivityTooManyDeletes::GetWindow(
    /* [out] */ IWindow** window)
{
    return Activity::GetWindow(window);
}

ECode CSyncActivityTooManyDeletes::GetLoaderManager(
    /* [out] */ ILoaderManager** loaderManager)
{
    return Activity::GetLoaderManager(loaderManager);
}

ECode CSyncActivityTooManyDeletes::GetCurrentFocus(
    /* [out] */ IView** focus)
{
    return Activity::GetCurrentFocus(focus);
}

ECode CSyncActivityTooManyDeletes::OnCreateThumbnail(
    /* [in] */ IBitmap* outBitmap,
    /* [in] */ ICanvas* canvas,
    /* [out] */ Boolean* value)
{
    return Activity::OnCreateThumbnail(outBitmap, canvas, value);
}

ECode CSyncActivityTooManyDeletes::OnCreateDescription(
    /* [out] */ ICharSequence** description)
{
    return Activity::OnCreateDescription(description);
}

ECode CSyncActivityTooManyDeletes::OnConfigurationChanged(
    /* [in] */ IConfiguration* newConfig)
{
    return Activity::OnConfigurationChanged(newConfig);
}

ECode CSyncActivityTooManyDeletes::GetChangingConfigurations(
    /* [out] */ Int32* config)
{
    return Activity::GetChangingConfigurations(config);
}

ECode CSyncActivityTooManyDeletes::GetLastNonConfigurationInstance(
    /* [out] */ IInterface** instance)
{
    return Activity::GetLastNonConfigurationInstance(instance);
}

ECode CSyncActivityTooManyDeletes::OnRetainNonConfigurationInstance(
    /* [out] */ IInterface** instance)
{
    return Activity::OnRetainNonConfigurationInstance(instance);
}

ECode CSyncActivityTooManyDeletes::GetLastNonConfigurationChildInstances(
    /* [out] */ IObjectStringMap** data)
{
    return Activity::GetLastNonConfigurationChildInstances(data);
}

ECode CSyncActivityTooManyDeletes::OnRetainNonConfigurationChildInstances(
    /* [out] */ IObjectStringMap** objectStringMap)
{
    return Activity::OnRetainNonConfigurationChildInstances(objectStringMap);
}

ECode CSyncActivityTooManyDeletes::RetainNonConfigurationInstances(
    /* [out] */ IActivityNonConfigurationInstances** instance)
{
    return Activity::RetainNonConfigurationInstances(instance);
}

ECode CSyncActivityTooManyDeletes::OnLowMemory()
{
    return Activity::OnLowMemory();
}

ECode CSyncActivityTooManyDeletes::OnTrimMemory(
    /* [in] */ Int32 level)
{
    return Activity::OnTrimMemory(level);
}

ECode CSyncActivityTooManyDeletes::GetFragmentManager(
    /* [out] */ IFragmentManager** manager)
{
    return Activity::GetFragmentManager(manager);
}

ECode CSyncActivityTooManyDeletes::OnAttachFragment(
    /* [in] */ IFragment* fragment)
{
    return Activity::OnAttachFragment(fragment);
}

ECode CSyncActivityTooManyDeletes::ManagedQuery(
    /* [in] */ IUri* uri,
    /* [in] */ ArrayOf<String>* projection,
    /* [in] */ const String& selection,
    /* [in] */ const String& sortOrder,
    /* [out] */ ICursor** cursor)
{
    return Activity::ManagedQuery(uri, projection, selection, sortOrder, cursor);
}

ECode CSyncActivityTooManyDeletes::ManagedQueryEx(
    /* [in] */ IUri* uri,
    /* [in] */ ArrayOf<String>* projection,
    /* [in] */ const String& selection,
    /* [in] */ ArrayOf<String>* selectionArgs,
    /* [in] */ const String& sortOrder,
    /* [out] */ ICursor** cursor)
{
    return Activity::ManagedQueryEx(uri, projection, selection, selectionArgs, sortOrder, cursor);
}

ECode CSyncActivityTooManyDeletes::StartManagingCursor(
    /* [in] */ ICursor* c)
{
    return Activity::StartManagingCursor(c);
}

ECode CSyncActivityTooManyDeletes::StopManagingCursor(
    /* [in] */ ICursor* c)
{
    return Activity::StopManagingCursor(c);
}

ECode CSyncActivityTooManyDeletes::SetPersistent(
    /* [in] */ Boolean isPersistent)
{
    return Activity::SetPersistent(isPersistent);
}

ECode CSyncActivityTooManyDeletes::FindViewById(
    /* [in] */ Int32 id,
    /* [out] */ IView** view)
{
    return Activity::FindViewById(id, view);
}

ECode CSyncActivityTooManyDeletes::GetActionBar(
    /* [out] */ IActionBar** bar)
{
    return Activity::GetActionBar(bar);
}

ECode CSyncActivityTooManyDeletes::SetContentView(
    /* [in] */ Int32 layoutResID)
{
    return Activity::SetContentView(layoutResID);
}

ECode CSyncActivityTooManyDeletes::SetContentViewEx(
    /* [in] */ IView* view)
{
    return Activity::SetContentViewEx(view);
}

ECode CSyncActivityTooManyDeletes::SetContentViewEx2(
    /* [in] */ IView* view,
    /* [in] */ IViewGroupLayoutParams* params)
{
    return Activity::SetContentViewEx2(view, params);
}

ECode CSyncActivityTooManyDeletes::AddContentView(
    /* [in] */ IView* view,
    /* [in] */ IViewGroupLayoutParams* params)
{
    return Activity::AddContentView(view, params);
}

ECode CSyncActivityTooManyDeletes::SetFinishOnTouchOutside(
    /* [in] */ Boolean finish)
{
    return Activity::SetFinishOnTouchOutside(finish);
}

ECode CSyncActivityTooManyDeletes::SetDefaultKeyMode(
    /* [in] */ Int32 mode)
{
    return Activity::SetDefaultKeyMode(mode);
}

ECode CSyncActivityTooManyDeletes::OnKeyDown(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* value)
{
    return Activity::OnKeyDown(keyCode, event, value);
}

ECode CSyncActivityTooManyDeletes::OnKeyLongPress(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* value)
{
    return Activity::OnKeyLongPress(keyCode, event, value);
}

ECode CSyncActivityTooManyDeletes::OnKeyUp(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* value)
{
    return Activity::OnKeyUp(keyCode, event, value);
}

ECode CSyncActivityTooManyDeletes::OnKeyMultiple(
    /* [in] */ Int32 keyCode,
    /* [in] */ Int32 repeatCount,
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* value)
{
    return Activity::OnKeyMultiple(keyCode, repeatCount, event, value);
}

ECode CSyncActivityTooManyDeletes::OnBackPressed()
{
    return Activity::OnBackPressed();
}

ECode CSyncActivityTooManyDeletes::OnKeyShortcut(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* value)
{
    return Activity::OnKeyShortcut(keyCode, event, value);
}

ECode CSyncActivityTooManyDeletes::OnTouchEvent(
    /* [in] */ IMotionEvent* event,
    /* [out] */ Boolean* value)
{
    return Activity::OnTouchEvent(event, value);
}

ECode CSyncActivityTooManyDeletes::OnTrackballEvent(
    /* [in] */ IMotionEvent* event,
    /* [out] */ Boolean* value)
{
    return Activity::OnTrackballEvent(event, value);
}

ECode CSyncActivityTooManyDeletes::OnGenericMotionEvent(
    /* [in] */ IMotionEvent* event,
    /* [out] */ Boolean* value)
{
    return Activity::OnGenericMotionEvent(event, value);
}

ECode CSyncActivityTooManyDeletes::OnUserInteraction()
{
    return Activity::OnUserInteraction();
}

ECode CSyncActivityTooManyDeletes::OnWindowAttributesChanged(
    /* [in] */ IWindowManagerLayoutParams* params)
{
    return Activity::OnWindowAttributesChanged(params);
}

ECode CSyncActivityTooManyDeletes::OnContentChanged()
{
    return Activity::OnContentChanged();
}

ECode CSyncActivityTooManyDeletes::OnWindowFocusChanged(
    /* [in] */ Boolean hasFocus)
{
    return Activity::OnWindowFocusChanged(hasFocus);
}

ECode CSyncActivityTooManyDeletes::OnAttachedToWindow()
{
    return Activity::OnAttachedToWindow();
}

ECode CSyncActivityTooManyDeletes::OnDetachedFromWindow()
{
    return Activity::OnDetachedFromWindow();
}

ECode CSyncActivityTooManyDeletes::HasWindowFocus(
    /* [out] */ Boolean* hasFocus)
{
    return Activity::HasWindowFocus(hasFocus);
}

ECode CSyncActivityTooManyDeletes::DispatchKeyEvent(
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* res)
{
    return Activity::DispatchKeyEvent(event, res);
}

ECode CSyncActivityTooManyDeletes::DispatchKeyShortcutEvent(
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* res)
{
    return Activity::DispatchKeyShortcutEvent(event, res);
}

ECode CSyncActivityTooManyDeletes::DispatchTouchEvent(
    /* [in] */ IMotionEvent* ev,
    /* [out] */ Boolean* res)
{
    return Activity::DispatchTouchEvent(ev, res);
}

ECode CSyncActivityTooManyDeletes::DispatchTrackballEvent(
    /* [in] */ IMotionEvent* ev,
    /* [out] */ Boolean* res)
{
    return Activity::DispatchTrackballEvent(ev, res);
}

ECode CSyncActivityTooManyDeletes::DispatchGenericMotionEvent(
    /* [in] */ IMotionEvent* ev,
    /* [out] */ Boolean* res)
{
    return Activity::DispatchGenericMotionEvent(ev, res);
}

ECode CSyncActivityTooManyDeletes::DispatchPopulateAccessibilityEvent(
    /* [in] */ IAccessibilityEvent* event,
    /* [out] */ Boolean* res)
{
    return Activity::DispatchPopulateAccessibilityEvent(event, res);
}

ECode CSyncActivityTooManyDeletes::OnCreatePanelView(
    /* [in] */ Int32 featureId,
    /* [out] */ IView** view)
{
    return Activity::OnCreatePanelView(featureId, view);
}

ECode CSyncActivityTooManyDeletes::OnCreatePanelMenu(
    /* [in] */ Int32 featureId,
    /* [in] */ IMenu* menu,
    /* [out] */ Boolean* res)
{
    return Activity::OnCreatePanelMenu(featureId, menu, res);
}

ECode CSyncActivityTooManyDeletes::OnPreparePanel(
    /* [in] */ Int32 featureId,
    /* [in] */ IView* view,
    /* [in] */ IMenu* menu,
    /* [out] */ Boolean* res)
{
    return Activity::OnPreparePanel(featureId, view, menu, res);
}

ECode CSyncActivityTooManyDeletes::OnMenuOpened(
    /* [in] */ Int32 featureId,
    /* [in] */ IMenu* menu,
    /* [out] */ Boolean* opened)
{
    return Activity::OnMenuOpened(featureId, menu, opened);
}

ECode CSyncActivityTooManyDeletes::OnMenuItemSelected(
    /* [in] */ Int32 featureId,
    /* [in] */ IMenuItem* item,
    /* [out] */ Boolean* selected)
{
    return Activity::OnMenuItemSelected(featureId, item, selected);
}

ECode CSyncActivityTooManyDeletes::OnPanelClosed(
    /* [in] */ Int32 featureId,
    /* [in] */ IMenu* menu)
{
    return Activity::OnPanelClosed(featureId, menu);
}

ECode CSyncActivityTooManyDeletes::InvalidateOptionsMenu()
{
    return Activity::InvalidateOptionsMenu();
}

ECode CSyncActivityTooManyDeletes::OnCreateOptionsMenu(
    /* [in] */ IMenu* menu,
    /* [out] */ Boolean* displayed)
{
    return Activity::OnCreateOptionsMenu(menu, displayed);
}

ECode CSyncActivityTooManyDeletes::OnPrepareOptionsMenu(
    /* [in] */ IMenu* menu,
    /* [out] */ Boolean* displayed)
{
    return Activity::OnPrepareOptionsMenu(menu, displayed);
}

ECode CSyncActivityTooManyDeletes::OnOptionsItemSelected(
    /* [in] */ IMenuItem* item,
    /* [out] */ Boolean* selected)
{
    return Activity::OnOptionsItemSelected(item, selected);
}

ECode CSyncActivityTooManyDeletes::OnNavigateUp(
    /* [out] */ Boolean* result)
{
    return Activity::OnNavigateUp(result);
}

ECode CSyncActivityTooManyDeletes::OnNavigateUpFromChild(
    /* [in] */ IActivity* child,
    /* [out] */ Boolean* result)
{
    return Activity::OnNavigateUpFromChild(child, result);
}

ECode CSyncActivityTooManyDeletes::OnCreateNavigateUpTaskStack(
    /* [in] */ ITaskStackBuilder* builder)
{
    return Activity::OnCreateNavigateUpTaskStack(builder);
}

ECode CSyncActivityTooManyDeletes::OnPrepareNavigateUpTaskStack(
    /* [in] */ ITaskStackBuilder* builder)
{
    return Activity::OnPrepareNavigateUpTaskStack(builder);
}

ECode CSyncActivityTooManyDeletes::OnOptionsMenuClosed(
    /* [in] */ IMenu* menu)
{
    return Activity::OnOptionsMenuClosed(menu);
}

ECode CSyncActivityTooManyDeletes::OpenOptionsMenu()
{
    return Activity::OpenOptionsMenu();
}

ECode CSyncActivityTooManyDeletes::CloseOptionsMenu()
{
    return Activity::CloseOptionsMenu();
}

ECode CSyncActivityTooManyDeletes::OnCreateContextMenu(
    /* [in] */ IContextMenu* menu,
    /* [in] */ IView* v,
    /* [in] */ IContextMenuInfo* menuInfo)
{
    return Activity::OnCreateContextMenu(menu, v, menuInfo);
}

ECode CSyncActivityTooManyDeletes::RegisterForContextMenu(
    /* [in] */ IView* view)
{
    return Activity::RegisterForContextMenu(view);
}

ECode CSyncActivityTooManyDeletes::UnregisterForContextMenu(
    /* [in] */ IView* view)
{
    return Activity::UnregisterForContextMenu(view);
}

ECode CSyncActivityTooManyDeletes::OpenContextMenu(
    /* [in] */ IView* view)
{
    return Activity::OpenContextMenu(view);
}

ECode CSyncActivityTooManyDeletes::CloseContextMenu()
{
    return Activity::CloseContextMenu();
}

ECode CSyncActivityTooManyDeletes::OnContextItemSelected(
    /* [in] */ IMenuItem* item,
    /* [out] */ Boolean* res)
{
    return Activity::OnContextItemSelected(item, res);
}

ECode CSyncActivityTooManyDeletes::OnContextMenuClosed(
    /* [in] */ IMenu* menu)
{
    return Activity::OnContextMenuClosed(menu);
}

ECode CSyncActivityTooManyDeletes::ShowDialog(
    /* [in] */ Int32 id)
{
    return Activity::ShowDialog(id);
}

ECode CSyncActivityTooManyDeletes::ShowDialogEx(
    /* [in] */ Int32 id,
    /* [in] */ IBundle* args,
    /* [out] */ Boolean* res)
{
    return Activity::ShowDialogEx(id, args, res);
}

ECode CSyncActivityTooManyDeletes::DismissDialog(
    /* [in] */ Int32 id)
{
    return Activity::DismissDialog(id);
}

ECode CSyncActivityTooManyDeletes::RemoveDialog(
    /* [in] */ Int32 id)
{
    return Activity::RemoveDialog(id);
}

ECode CSyncActivityTooManyDeletes::OnSearchRequested(
    /* [out] */ Boolean* res)
{
    return Activity::OnSearchRequested(res);
}

ECode CSyncActivityTooManyDeletes::StartSearch(
    /* [in] */ const String& initialQuery,
    /* [in] */ Boolean selectInitialQuery,
    /* [in] */ IBundle* appSearchData,
    /* [in] */ Boolean globalSearch)
{
    return Activity::StartSearch(initialQuery, selectInitialQuery, appSearchData, globalSearch);
}

ECode CSyncActivityTooManyDeletes::TriggerSearch(
    /* [in] */ const String& query,
    /* [in] */ IBundle* appSearchData)
{
    return Activity::TriggerSearch(query, appSearchData);
}

ECode CSyncActivityTooManyDeletes::TakeKeyEvents(
    /* [in] */ Boolean get)
{
    return Activity::TakeKeyEvents(get);
}

ECode CSyncActivityTooManyDeletes::RequestWindowFeature(
    /* [in] */ Int32 featureId,
    /* [out] */ Boolean* result)
{
    return Activity::RequestWindowFeature(featureId, result);
}

ECode CSyncActivityTooManyDeletes::SetFeatureDrawableResource(
    /* [in] */ Int32 featureId,
    /* [in] */ Int32 resId)
{
    return Activity::SetFeatureDrawableResource(featureId, resId);
}

ECode CSyncActivityTooManyDeletes::SetFeatureDrawableUri(
    /* [in] */ Int32 featureId,
    /* [in] */ IUri* uri)
{
    return Activity::SetFeatureDrawableUri(featureId, uri);
}

ECode CSyncActivityTooManyDeletes::SetFeatureDrawable(
    /* [in] */ Int32 featureId,
    /* [in] */ IDrawable* drawable)
{
    return Activity::SetFeatureDrawable(featureId, drawable);
}

ECode CSyncActivityTooManyDeletes::SetFeatureDrawableAlpha(
    /* [in] */ Int32 featureId,
    /* [in] */ Int32 alpha)
{
    return Activity::SetFeatureDrawableAlpha(featureId, alpha);
}

ECode CSyncActivityTooManyDeletes::GetLayoutInflater(
    /* [out] */ ILayoutInflater** inflater)
{
    return Activity::GetLayoutInflater(inflater);
}

ECode CSyncActivityTooManyDeletes::GetMenuInflater(
    /* [out] */ IMenuInflater** flater)
{
    return Activity::GetMenuInflater(flater);
}

ECode CSyncActivityTooManyDeletes::StartActivityForResult(
    /* [in] */ IIntent* intent,
    /* [in] */ Int32 requestCode)
{
    return Activity::StartActivityForResult(intent, requestCode);
}

ECode CSyncActivityTooManyDeletes::StartActivityForResultEx(
    /* [in] */ IIntent* intent,
    /* [in] */ Int32 requestCode,
    /* [in] */ IBundle* options)
{
    return Activity::StartActivityForResultEx(intent, requestCode, options);
}

ECode CSyncActivityTooManyDeletes::StartIntentSenderForResult(
    /* [in] */ IIntentSender* intent,
    /* [in] */ Int32 requestCode,
    /* [in] */ IIntent* fillInIntent,
    /* [in] */ Int32 flagsMask,
    /* [in] */ Int32 flagsValues,
    /* [in] */ Int32 extraFlags)
{
    return Activity::StartIntentSenderForResult(intent, requestCode, fillInIntent, flagsMask, flagsValues, extraFlags);
}

ECode CSyncActivityTooManyDeletes::StartIntentSenderForResultEx(
    /* [in] */ IIntentSender* intent,
    /* [in] */ Int32 requestCode,
    /* [in] */ IIntent* fillInIntent,
    /* [in] */ Int32 flagsMask,
    /* [in] */ Int32 flagsValues,
    /* [in] */ Int32 extraFlags,
    /* [in] */ IBundle* options)
{
    return Activity::StartIntentSenderForResultEx(intent, requestCode, fillInIntent, flagsMask, flagsValues,
            extraFlags, options);
}

ECode CSyncActivityTooManyDeletes::StartActivityIfNeeded(
    /* [in] */ IIntent* intent,
    /* [in] */ Int32 requestCode,
    /* [out] */ Boolean* started)
{
    return Activity::StartActivityIfNeeded(intent, requestCode, started);
}

ECode CSyncActivityTooManyDeletes::StartActivityIfNeededEx(
    /* [in] */ IIntent* intent,
    /* [in] */ Int32 requestCode,
    /* [in] */ IBundle* options,
    /* [out] */ Boolean* started)
{
    return Activity::StartActivityIfNeededEx(intent, requestCode, options, started);
}

ECode CSyncActivityTooManyDeletes::StartNextMatchingActivity(
    /* [in] */ IIntent* intent,
    /* [out] */ Boolean* started)
{
    return Activity::StartNextMatchingActivity(intent, started);
}

ECode CSyncActivityTooManyDeletes::StartNextMatchingActivityEx(
    /* [in] */ IIntent* intent,
    /* [in] */ IBundle* options,
    /* [out] */ Boolean* started)
{
    return Activity::StartNextMatchingActivityEx(intent, options, started);
}

ECode CSyncActivityTooManyDeletes::StartActivityFromChild(
    /* [in] */ IActivity* child,
    /* [in] */ IIntent* intent,
    /* [in] */ Int32 requestCode)
{
    return Activity::StartActivityFromChild(child, intent, requestCode);
}

ECode CSyncActivityTooManyDeletes::StartActivityFromChildEx(
    /* [in] */ IActivity* child,
    /* [in] */ IIntent* intent,
    /* [in] */ Int32 requestCode,
    /* [in] */ IBundle* options)
{
    return Activity::StartActivityFromChildEx(child, intent, requestCode, options);
}

ECode CSyncActivityTooManyDeletes::StartActivityFromFragment(
    /* [in] */ IFragment* fragment,
    /* [in] */ IIntent* intent,
    /* [in] */ Int32 requestCode)
{
    return Activity::StartActivityFromFragment(fragment, intent, requestCode);
}

ECode CSyncActivityTooManyDeletes::StartActivityFromFragmentEx(
    /* [in] */ IFragment* fragment,
    /* [in] */ IIntent* intent,
    /* [in] */ Int32 requestCode,
    /* [in] */ IBundle* options)
{
    return Activity::StartActivityFromFragmentEx(fragment, intent, requestCode, options);
}

ECode CSyncActivityTooManyDeletes::StartIntentSenderFromChild(
    /* [in] */ IActivity* child,
    /* [in] */ IIntentSender* intent,
    /* [in] */ Int32 requestCode,
    /* [in] */ IIntent* fillInIntent,
    /* [in] */ Int32 flagsMask,
    /* [in] */ Int32 flagsValues,
    /* [in] */ Int32 extraFlags)
{
    return Activity::StartIntentSenderFromChild(child, intent, requestCode, fillInIntent, flagsMask, flagsValues,
            extraFlags);
}

ECode CSyncActivityTooManyDeletes::StartIntentSenderFromChildEx(
    /* [in] */ IActivity* child,
    /* [in] */ IIntentSender* intent,
    /* [in] */ Int32 requestCode,
    /* [in] */ IIntent* fillInIntent,
    /* [in] */ Int32 flagsMask,
    /* [in] */ Int32 flagsValues,
    /* [in] */ Int32 extraFlags,
    /* [in] */ IBundle* options)
{
    return Activity::StartIntentSenderFromChildEx(child, intent, requestCode, fillInIntent, flagsMask, flagsValues,
            extraFlags, options);
}

ECode CSyncActivityTooManyDeletes::OverridePendingTransition(
    /* [in] */ Int32 enterAnim,
    /* [in] */ Int32 exitAnim)
{
    return Activity::OverridePendingTransition(enterAnim, exitAnim);
}

ECode CSyncActivityTooManyDeletes::SetResult(
    /* [in] */ Int32 resultCode)
{
    return Activity::SetResult(resultCode);
}

ECode CSyncActivityTooManyDeletes::SetResultEx(
    /* [in] */ Int32 resultCode,
    /* [in] */ IIntent* data)
{
    return Activity::SetResultEx(resultCode, data);
}

ECode CSyncActivityTooManyDeletes::GetCallingPackage(
    /* [out] */ String* package)
{
    return Activity::GetCallingPackage(package);
}

ECode CSyncActivityTooManyDeletes::GetCallingActivity(
    /* [out] */ IComponentName** name)
{
    return Activity::GetCallingActivity(name);
}

ECode CSyncActivityTooManyDeletes::SetVisible(
    /* [in] */ Boolean visible)
{
    return Activity::SetVisible(visible);
}

ECode CSyncActivityTooManyDeletes::SetCalled(
    /* [in] */ Boolean called)
{
    return Activity::SetCalled(called);
}

ECode CSyncActivityTooManyDeletes::IsCalled(
    /* [out] */ Boolean* called)
{
    return Activity::IsCalled(called);
}

ECode CSyncActivityTooManyDeletes::SetFinishing(
    /* [in] */ Boolean finished)
{
    return Activity::SetFinishing(finished);
}

ECode CSyncActivityTooManyDeletes::IsFinishing(
    /* [out] */ Boolean* isFinished)
{
    return Activity::IsFinishing(isFinished);
}

ECode CSyncActivityTooManyDeletes::IsDestroyed(
    /* [out] */ Boolean* isDestroyed)
{
    return Activity::IsDestroyed(isDestroyed);
}

ECode CSyncActivityTooManyDeletes::IsChangingConfigurations(
    /* [out] */ Boolean* isChanged)
{
    return Activity::IsChangingConfigurations(isChanged);
}

ECode CSyncActivityTooManyDeletes::Recreate()
{
    return Activity::Recreate();
}

ECode CSyncActivityTooManyDeletes::Finish()
{
    return Activity::Finish();
}

ECode CSyncActivityTooManyDeletes::FinishAffinity()
{
    return Activity::FinishAffinity();
}

ECode CSyncActivityTooManyDeletes::FinishFromChild(
    /* [in] */ IActivity* child)
{
    return Activity::FinishFromChild(child);
}

ECode CSyncActivityTooManyDeletes::FinishActivity(
    /* [in] */ Int32 requestCode)
{
    return Activity::FinishActivity(requestCode);
}

ECode CSyncActivityTooManyDeletes::FinishActivityFromChild(
    /* [in] */ IActivity* child,
    /* [in] */ Int32 requestCode)
{
    return Activity::FinishActivityFromChild(child, requestCode);
}

ECode CSyncActivityTooManyDeletes::CreatePendingResult(
    /* [in] */ Int32 requestCode,
    /* [in] */ IIntent* data,
    /* [in] */ Int32 flags,
    /* [out] */ IPendingIntent** pendingIntent)
{
    return Activity::CreatePendingResult(requestCode, data, flags, pendingIntent);
}

ECode CSyncActivityTooManyDeletes::SetRequestedOrientation(
    /* [in] */ Int32 requestedOrientation)
{
    return Activity::SetRequestedOrientation(requestedOrientation);
}

ECode CSyncActivityTooManyDeletes::GetRequestedOrientation(
    /* [out] */ Int32* orientation)
{
    return Activity::GetRequestedOrientation(orientation);
}

ECode CSyncActivityTooManyDeletes::GetTaskId(
    /* [out] */ Int32* id)
{
    return Activity::GetTaskId(id);
}

ECode CSyncActivityTooManyDeletes::IsTaskRoot(
    /* [out] */ Boolean* isRoot)
{
    return Activity::IsTaskRoot(isRoot);
}

ECode CSyncActivityTooManyDeletes::MoveTaskToBack(
    /* [in] */ Boolean nonRoot,
    /* [out] */ Boolean* moved)
{
    return Activity::MoveTaskToBack(nonRoot, moved);
}

ECode CSyncActivityTooManyDeletes::GetLocalClassName(
    /* [out] */ String* className)
{
    return Activity::GetLocalClassName(className);
}

ECode CSyncActivityTooManyDeletes::GetComponentName(
    /* [out] */ IComponentName** componentName)
{
    return Activity::GetComponentName(componentName);
}

ECode CSyncActivityTooManyDeletes::GetPreferences(
    /* [in] */ Int32 mode,
    /* [out] */ ISharedPreferences** perferences)
{
    return Activity::GetPreferences(mode, perferences);
}

ECode CSyncActivityTooManyDeletes::SetTitle(
    /* [in] */ ICharSequence* title)
{
    return Activity::SetTitle(title);
}

ECode CSyncActivityTooManyDeletes::SetTitleEx(
    /* [in] */ Int32 titleId)
{
    return Activity::SetTitleEx(titleId);
}

ECode CSyncActivityTooManyDeletes::SetTitleColor(
    /* [in] */ Int32 textColor)
{
    return Activity::SetTitleColor(textColor);
}

ECode CSyncActivityTooManyDeletes::GetTitle(
    /* [out] */ ICharSequence** title)
{
    return Activity::GetTitle(title);
}

ECode CSyncActivityTooManyDeletes::GetTitleColor(
    /* [out] */ Int32* color)
{
    return Activity::GetTitleColor(color);
}

ECode CSyncActivityTooManyDeletes::OnTitleChanged(
    /* [in] */ ICharSequence* title,
    /* [in] */ Int32 color)
{
    return Activity::OnTitleChanged(title, color);
}

ECode CSyncActivityTooManyDeletes::OnChildTitleChanged(
    /* [in] */ IActivity* childActivity,
    /* [in] */ ICharSequence* title)
{
    return Activity::OnChildTitleChanged(childActivity, title);
}

ECode CSyncActivityTooManyDeletes::SetProgressBarVisibility(
    /* [in] */ Boolean visible)
{
    return Activity::SetProgressBarVisibility(visible);
}

ECode CSyncActivityTooManyDeletes::SetProgressBarIndeterminateVisibility(
    /* [in] */ Boolean visible)
{
    return Activity::SetProgressBarIndeterminateVisibility(visible);
}

ECode CSyncActivityTooManyDeletes::SetProgressBarIndeterminate(
    /* [in] */ Boolean indeterminate)
{
    return Activity::SetProgressBarIndeterminate(indeterminate);
}

ECode CSyncActivityTooManyDeletes::SetProgress(
    /* [in] */ Int32 progress)
{
    return Activity::SetProgress(progress);
}

ECode CSyncActivityTooManyDeletes::SetSecondaryProgress(
    /* [in] */ Int32 secondaryProgress)
{
    return Activity::SetSecondaryProgress(secondaryProgress);
}

ECode CSyncActivityTooManyDeletes::SetVolumeControlStream(
    /* [in] */ Int32 streamType)
{
    return Activity::SetVolumeControlStream(streamType);
}

ECode CSyncActivityTooManyDeletes::GetVolumeControlStream(
    /* [out] */ Int32* type)
{
    return Activity::GetVolumeControlStream(type);
}

ECode CSyncActivityTooManyDeletes::RunOnUiThread(
    /* [in] */ IRunnable* action)
{
    return Activity::RunOnUiThread(action);
}

ECode CSyncActivityTooManyDeletes::OnCreateView(
    /* [in] */ const String& name,
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [out] */ IView** view)
{
    return Activity::OnCreateView(name, context, attrs, view);
}

ECode CSyncActivityTooManyDeletes::OnCreateViewEx(
    /* [in] */ IView* parent,
    /* [in] */ const String& name,
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [out] */ IView** view)
{
    return Activity::OnCreateViewEx(parent, name, context, attrs, view);
}

ECode CSyncActivityTooManyDeletes::Dump(
    /* [in] */ const String& prefix,
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ IPrintWriter* writer,
    /* [in] */ ArrayOf<String>* args)
{
    return Activity::Dump(prefix, fd, writer, args);
}

ECode CSyncActivityTooManyDeletes::IsImmersive(
    /* [out] */ Boolean* isImmersive)
{
    return Activity::IsImmersive(isImmersive);
}

ECode CSyncActivityTooManyDeletes::SetImmersive(
    /* [in] */ Boolean i)
{
    return Activity::SetImmersive(i);
}

ECode CSyncActivityTooManyDeletes::StartActionMode(
    /* [in] */ IActionModeCallback* cb,
    /* [out] */ IActionMode** mode)
{
    return Activity::StartActionMode(cb, mode);
}

ECode CSyncActivityTooManyDeletes::OnWindowStartingActionMode(
    /* [in] */ IActionModeCallback* cb,
    /* [out] */ IActionMode** mode)
{
    return Activity::OnWindowStartingActionMode(cb, mode);
}

ECode CSyncActivityTooManyDeletes::OnActionModeStarted(
    /* [in] */ IActionMode* mode)
{
    return Activity::OnActionModeStarted(mode);
}

ECode CSyncActivityTooManyDeletes::OnActionModeFinished(
    /* [in] */ IActionMode* mode)
{
    return Activity::OnActionModeFinished(mode);
}

ECode CSyncActivityTooManyDeletes::ShouldUpRecreateTask(
    /* [in] */ IIntent* targetIntent,
    /* [out] */ Boolean* res)
{
    return Activity::ShouldUpRecreateTask(targetIntent, res);
}

ECode CSyncActivityTooManyDeletes::NavigateUpTo(
    /* [in] */ IIntent* upIntent,
    /* [out] */ Boolean* success)
{
    return Activity::NavigateUpTo(upIntent, success);
}

ECode CSyncActivityTooManyDeletes::NavigateUpToFromChild(
    /* [in] */ IActivity* child,
    /* [in] */ IIntent* upIntent,
    /* [out] */ Boolean* success)
{
    return Activity::NavigateUpToFromChild(child, upIntent, success);
}

ECode CSyncActivityTooManyDeletes::GetParentActivityIntent(
    /* [out] */ IIntent** intent)
{
    return Activity::GetParentActivityIntent(intent);
}

ECode CSyncActivityTooManyDeletes::Attach(
    /* [in] */ IContext* ctx,
    /* [in] */ IActivityThread* aThread,
    /* [in] */ IInstrumentation* instr,
    /* [in] */ IBinder* token,
    /* [in] */ IApplication* application,
    /* [in] */ IIntent* intent,
    /* [in] */ IActivityInfo* info,
    /* [in] */ ICharSequence* title,
    /* [in] */ IActivity* parent,
    /* [in] */ const String& id,
    /* [in] */ IInterface* lastNonConfigurationInstance,
    /* [in] */ IConfiguration* config)
{
    return Activity::Attach(ctx, aThread, instr, token, application, intent, info, title, parent, id,
            lastNonConfigurationInstance, config);
}

ECode CSyncActivityTooManyDeletes::AttachEx(
    /* [in] */ IContext* ctx,
    /* [in] */ IActivityThread* aThread,
    /* [in] */ IInstrumentation* instr,
    /* [in] */ IBinder* token,
    /* [in] */ Int32 ident,
    /* [in] */ IApplication* application,
    /* [in] */ IIntent* intent,
    /* [in] */ IActivityInfo* info,
    /* [in] */ ICharSequence* title,
    /* [in] */ IActivity* parent,
    /* [in] */ const String& id,
    /* [in] */ IInterface* lastNonConfigurationInstance,
    /* [in] */ IConfiguration* config)
{
    return Activity::AttachEx(ctx, aThread, instr, token, ident, application, intent, info, title, parent, id,
            lastNonConfigurationInstance, config);
}

ECode CSyncActivityTooManyDeletes::GetActivityToken(
    /* [out] */ IBinder** token)
{
    return Activity::GetActivityToken(token);
}

ECode CSyncActivityTooManyDeletes::IsResumed(
    /* [out] */ Boolean* isResumed)
{
    return Activity::IsResumed(isResumed);
}

ECode CSyncActivityTooManyDeletes::SetParent(
    /* [in] */ IActivity* parent)
{
    return Activity::SetParent(parent);
}

ECode CSyncActivityTooManyDeletes::GetID(
    /* [out] */ String* id)
{
    return Activity::GetID(id);
}

ECode CSyncActivityTooManyDeletes::GetToken(
    /* [out] */ IBinder** token)
{
    return Activity::GetToken(token);
}

ECode CSyncActivityTooManyDeletes::IsStartedActivity(
    /* [out] */ Boolean* isStartedActivity)
{
    return Activity::IsStartedActivity(isStartedActivity);
}

ECode CSyncActivityTooManyDeletes::GetWindowEx(
    /* [out] */ IWindow** window)
{
    return Activity::GetWindowEx(window);
}

ECode CSyncActivityTooManyDeletes::SetDecorView(
    /* [in] */ IView* decor)
{
    return Activity::SetDecorView(decor);
}

ECode CSyncActivityTooManyDeletes::GetDecorView(
    /* [out] */ IView** decor)
{
    return Activity::GetDecorView(decor);
}

ECode CSyncActivityTooManyDeletes::SetVisibleFromServer(
    /* [in] */ Boolean visible)
{
    return Activity::SetVisibleFromServer(visible);
}

ECode CSyncActivityTooManyDeletes::IsVisibleFromServer(
    /* [out] */ Boolean* visible)
{
    return Activity::IsVisibleFromServer(visible);
}

ECode CSyncActivityTooManyDeletes::IsVisibleFromClient(
    /* [out] */ Boolean* visible)
{
    return Activity::IsVisibleFromClient(visible);
}

ECode CSyncActivityTooManyDeletes::SetWindowAdded(
    /* [in] */ Boolean added)
{
    return Activity::SetWindowAdded(added);
}

ECode CSyncActivityTooManyDeletes::IsWindowAdded(
    /* [out] */ Boolean* isAdded)
{
    return Activity::IsWindowAdded(isAdded);
}

ECode CSyncActivityTooManyDeletes::GetWindowManagerEx(
    /* [out] */ IWindowManager** mgr)
{
    return Activity::GetWindowManagerEx(mgr);
}

ECode CSyncActivityTooManyDeletes::SetConfigChangeFlags(
    /* [in] */ Int32 configChangeFlags)
{
    return Activity::SetConfigChangeFlags(configChangeFlags);
}

ECode CSyncActivityTooManyDeletes::GetConfigChangeFlags(
    /* [out] */ Int32* configChangeFlags)
{
    return Activity::GetConfigChangeFlags(configChangeFlags);
}

ECode CSyncActivityTooManyDeletes::MakeVisible()
{
    return Activity::MakeVisible();
}

ECode CSyncActivityTooManyDeletes::SetStartedActivity(
    /* [in] */ Boolean started)
{
    return Activity::SetStartedActivity(started);
}

ECode CSyncActivityTooManyDeletes::PerformSaveInstanceState(
    /* [in] */ IBundle* outState)
{
    return Activity::PerformSaveInstanceState(outState);
}

ECode CSyncActivityTooManyDeletes::PerformRestoreInstanceState(
    /* [in] */ IBundle* savedInstanceState)
{
    return Activity::PerformRestoreInstanceState(savedInstanceState);
}

ECode CSyncActivityTooManyDeletes::PerformCreate(
    /* [in] */ IBundle* icicle)
{
    return Activity::PerformCreate(icicle);
}

ECode CSyncActivityTooManyDeletes::PerformStart()
{
    return Activity::PerformStart();
}

ECode CSyncActivityTooManyDeletes::PerformRestart()
{
    return Activity::PerformRestart();
}

ECode CSyncActivityTooManyDeletes::PerformResume()
{
    return Activity::PerformResume();
}

ECode CSyncActivityTooManyDeletes::PerformPause()
{
    return Activity::PerformPause();
}

ECode CSyncActivityTooManyDeletes::PerformUserLeaving()
{
    return Activity::PerformUserLeaving();
}

ECode CSyncActivityTooManyDeletes::PerformStop()
{
    return Activity::PerformStop();
}

ECode CSyncActivityTooManyDeletes::PerformDestroy()
{
    return Activity::PerformDestroy();
}

ECode CSyncActivityTooManyDeletes::OnItemClick(
    /* [in] */ IAdapterView* parent,
    /* [in] */ IView* view,
    /* [in] */ Int32 position,
    /* [in] */ Int64 id)
{
    // the constants for position correspond to the items options array in onCreate()
    if (position == 0) {
        FAIL_RETURN(StartSyncReallyDelete())
    }
    else if (position == 1) {
        FAIL_RETURN(StartSyncUndoDeletes())
    }
    FAIL_RETURN(Finish())
    return NOERROR;
}

ECode CSyncActivityTooManyDeletes::OnCreate(
    /* [in] */ IBundle* savedInstanceState)
{
    FAIL_RETURN(Activity::OnCreate(savedInstanceState))

    AutoPtr<IIntent> intent;
    AutoPtr<IBundle> extras;
    FAIL_RETURN(GetIntent((IIntent**)&intent))
    FAIL_RETURN(intent->GetExtras((IBundle**)&extras))

    if (extras == NULL) {
        FAIL_RETURN(Finish())
        return NOERROR;
    }

    FAIL_RETURN(extras->GetInt64(String("numDeletes"), &mNumDeletes))
    AutoPtr<IParcelable> parcelable;
    FAIL_RETURN(extras->GetParcelable(String("account"), (IParcelable**)&parcelable))
    mAccount = IAccount::Probe(parcelable);
    FAIL_RETURN(extras->GetString(String("authority"), &mAuthority))
    FAIL_RETURN(extras->GetString(String("provider"), &mProvider))
    AutoPtr<IResources> res;
    FAIL_RETURN(GetResources((IResources**)&res))

    // the order of these must match up with the constants for position used in onItemClick
//    CharSequence[] options = new CharSequence[]{
//            getResources().getText(R.string.sync_really_delete),
//            getResources().getText(R.string.sync_undo_deletes),
//            getResources().getText(R.string.sync_do_nothing)
//    };

    AutoPtr<IListAdapter> adapter;
//    ListAdapter adapter = new ArrayAdapter<CharSequence>(this,
//            android.R.layout.simple_list_item_1,
//            android.R.id.text1,
//            options);

//    ListView listView = new ListView(this);
//    listView.setAdapter(adapter);
//    listView.setItemsCanFocus(true);
//    listView.setOnItemClickListener(this);

//    TextView textView = new TextView(this);
//    CharSequence tooManyDeletesDescFormat =
//            getResources().getText(R.string.sync_too_many_deletes_desc);
//    textView.setText(String.format(tooManyDeletesDescFormat.toString(),
//            mNumDeletes, mProvider, mAccount.name));

//    final LinearLayout ll = new LinearLayout(this);
//    ll.setOrientation(LinearLayout.VERTICAL);
//    final LinearLayout.LayoutParams lp = new LinearLayout.LayoutParams(
//            ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.WRAP_CONTENT, 0);
//    ll.addView(textView, lp);
//    ll.addView(listView, lp);

    // TODO: consider displaying the icon of the account type
//        AuthenticatorDescription[] descs = AccountManager.get(this).getAuthenticatorTypes();
//        for (AuthenticatorDescription desc : descs) {
//            if (desc.type.equals(mAccount.type)) {
//                try {
//                    final Context authContext = createPackageContext(desc.packageName, 0);
//                    ImageView imageView = new ImageView(this);
//                    imageView.setImageDrawable(authContext.getResources().getDrawable(desc.iconId));
//                    ll.addView(imageView, lp);
//                } catch (PackageManager.NameNotFoundException e) {
//                }
//                break;
//            }
//        }

//    FAIL_RETURN(SetContentView(ll))
    return E_NOT_IMPLEMENTED;
}

ECode CSyncActivityTooManyDeletes::Initialize()
{
    return NOERROR;
}

ECode CSyncActivityTooManyDeletes::OnUserLeaveHint()
{
    return NOERROR;
}

ECode CSyncActivityTooManyDeletes::OnActivityResult(
    /* [in] */ Int32 requestCode,
    /* [in] */ Int32 resultCode,
    /* [in] */ IIntent *data)
{
    return NOERROR;
}

ECode CSyncActivityTooManyDeletes::OnSaveInstanceState(
    /* [in] */ IBundle* outState)
{
    return NOERROR;
}

ECode CSyncActivityTooManyDeletes::AttachBaseContext(
    /* [in] */ IContext* base)
{
    return NOERROR;
}

ECode CSyncActivityTooManyDeletes::OnApplyThemeResource(
    /* [in] */ IResourcesTheme* theme,
    /* [in] */ Int32 resid,
    /* [in] */ Boolean first)
{
    return NOERROR;
}

ECode CSyncActivityTooManyDeletes::OnRestoreInstanceState(
    /* [in] */ IBundle* savedInstanceState)
{
    return NOERROR;
}

AutoPtr<IDialog> CSyncActivityTooManyDeletes::CreateDialog(
    /* [in] */ Int32 dialogId,
    /* [in] */ IBundle* state,
    /* [in] */ IBundle* args)
{
    return NULL;
}

ECode CSyncActivityTooManyDeletes::OnPostCreate(
    /* [in] */ IBundle* savedInstanceState)
{
    return NOERROR;
}

ECode CSyncActivityTooManyDeletes::OnStart()
{
    return NOERROR;
}

ECode CSyncActivityTooManyDeletes::OnRestart()
{
    return NOERROR;
}

ECode CSyncActivityTooManyDeletes::OnResume()
{
    return NOERROR;
}

ECode CSyncActivityTooManyDeletes::OnPause()
{
    return NOERROR;
}

ECode CSyncActivityTooManyDeletes::OnPostResume()
{
    return NOERROR;
}

ECode CSyncActivityTooManyDeletes::OnStop()
{
    return NOERROR;
}

ECode CSyncActivityTooManyDeletes::OnDestroy()
{
    return NOERROR;
}

ECode CSyncActivityTooManyDeletes::OnNewIntent(
    /* [in] */ IIntent *intent)
{
    return NOERROR;
}

AutoPtr<IDialog> CSyncActivityTooManyDeletes::OnCreateDialog(
    /* [in] */ Int32 id)
{
    return NULL;
}

AutoPtr<IDialog> CSyncActivityTooManyDeletes::OnCreateDialog(
    /* [in] */ Int32 id,
    /* [in] */ IBundle* args)
{
    return NULL;
}

void CSyncActivityTooManyDeletes::OnPrepareDialog(
    /* [in] */ Int32 id,
    /* [in] */ IDialog* dialog)
{}

void CSyncActivityTooManyDeletes::OnPrepareDialog(
    /* [in] */ Int32 id,
    /* [in] */ IDialog* dialog,
    /* [in] */ IBundle* args)
{}

ECode CSyncActivityTooManyDeletes::StartSyncReallyDelete()
{
    AutoPtr<IBundle> extras;
    FAIL_RETURN(CBundle::New((IBundle**)&extras))
    FAIL_RETURN(extras->PutBoolean(IContentResolver::SYNC_EXTRAS_OVERRIDE_TOO_MANY_DELETIONS, TRUE))
    FAIL_RETURN(extras->PutBoolean(IContentResolver::SYNC_EXTRAS_MANUAL, TRUE))
    FAIL_RETURN(extras->PutBoolean(IContentResolver::SYNC_EXTRAS_EXPEDITED, TRUE))
    FAIL_RETURN(extras->PutBoolean(IContentResolver::SYNC_EXTRAS_UPLOAD, TRUE))
    // AutoPtr<IContentResolverHelper> resolverHelper;
    // FAIL_RETURN(CContentResolverHelper::AcquireSingleton((IContentResolverHelper**)&resolverHelper))
    // FAIL_RETURN(resolverHelper->RequestSync(mAccount, mAuthority, extras))
    return NOERROR;
}

ECode CSyncActivityTooManyDeletes::StartSyncUndoDeletes()
{
    AutoPtr<IBundle> extras;
    FAIL_RETURN(CBundle::New((IBundle**)&extras))
    FAIL_RETURN(extras->PutBoolean(IContentResolver::SYNC_EXTRAS_DISCARD_LOCAL_DELETIONS, TRUE))
    FAIL_RETURN(extras->PutBoolean(IContentResolver::SYNC_EXTRAS_MANUAL, TRUE))
    FAIL_RETURN(extras->PutBoolean(IContentResolver::SYNC_EXTRAS_EXPEDITED, TRUE))
    FAIL_RETURN(extras->PutBoolean(IContentResolver::SYNC_EXTRAS_UPLOAD, TRUE))
    // AutoPtr<IContentResolverHelper> resolverHelper;
    // FAIL_RETURN(CContentResolverHelper::AcquireSingleton((IContentResolverHelper**)&resolverHelper))
    // FAIL_RETURN(resolverHelper->RequestSync(mAccount, mAuthority, extras))
    return NOERROR;
}

}
}
}
