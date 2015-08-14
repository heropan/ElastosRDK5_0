
#ifndef __ELASTOS_DROID_CONTENT_CSYNCACTIVITYTOOMANYDELETES_H__
#define __ELASTOS_DROID_CONTENT_CSYNCACTIVITYTOOMANYDELETES_H__

#include "_CSyncActivityTooManyDeletes.h"
#include <ext/frameworkext.h>
#include "app/Activity.h"

using namespace Elastos::Droid::App;
using namespace Elastos::Droid::Content::Res;
using namespace Elastos::Droid::Content::Pm;
using namespace Elastos::Droid::Os;
using namespace Elastos::IO;
using namespace Elastos::Droid::View;
using Elastos::Core::ICharSequence;
using Elastos::Core::IClassLoader;
using Elastos::Core::IRunnable;
using Elastos::Droid::Accounts::IAccount;
using Elastos::Droid::Database::ICursor;
using Elastos::Droid::Database::IDatabaseErrorHandler;
using Elastos::Droid::Database::Sqlite::ISQLiteDatabase;
using Elastos::Droid::Database::Sqlite::ISQLiteDatabaseCursorFactory;
using Elastos::Droid::Graphics::Drawable::IDrawable;
using Elastos::Droid::Graphics::IBitmap;
using Elastos::Droid::Graphics::ICanvas;
using Elastos::Droid::Net::IUri;
using Elastos::Droid::Utility::IAttributeSet;
using Elastos::Droid::View::Accessibility::IAccessibilityEvent;
using Elastos::Droid::Widget::IAdapterView;
using Elastos::Utility::IObjectStringMap;

namespace Elastos {
namespace Droid {
namespace Content {

class CSyncActivityTooManyDeletes : public Activity
{
public:
    CSyncActivityTooManyDeletes();

    ~CSyncActivityTooManyDeletes();

    CARAPI GetAssets(
        /* [out] */ IAssetManager** assetManager);

    CARAPI GetResources(
        /* [out] */ IResources** resources);

    CARAPI GetPackageManager(
        /* [out] */ IPackageManager** packageManager);

    CARAPI GetContentResolver(
        /* [out] */ IContentResolver** resolver);

    CARAPI GetMainLooper(
        /* [out] */ ILooper** looper);

    CARAPI GetApplicationContext(
        /* [out] */ IContext** ctx);

    CARAPI RegisterComponentCallbacks(
        /* [in] */ IComponentCallbacks* componentCallback);

    CARAPI UnregisterComponentCallbacks(
        /* [in] */ IComponentCallbacks* componentCallback);

    CARAPI GetText(
        /* [in] */ Int32 resId,
        /* [out] */ ICharSequence** text);

    CARAPI GetString(
        /* [in] */ Int32 resId,
        /* [out] */ String* str);

    CARAPI GetString(
        /* [in] */ Int32 resId,
        /* [in] */ ArrayOf<IInterface *>* formatArgs,
        /* [out] */ String* str);

    CARAPI SetTheme(
        /* [in] */ Int32 resId);

    CARAPI GetThemeResId(
        /* [out] */ Int32* resId);

    CARAPI GetTheme(
        /* [out] */ IResourcesTheme** theme);

    CARAPI ObtainStyledAttributes(
        /* [in] */ ArrayOf<Int32>* attrs,
        /* [out] */ ITypedArray** styles);

    CARAPI ObtainStyledAttributes(
        /* [in] */ Int32 resid,
        /* [in] */ ArrayOf<Int32>* attrs,
        /* [out] */ ITypedArray** styles);

    CARAPI ObtainStyledAttributes(
        /* [in] */ IAttributeSet* set,
        /* [in] */ ArrayOf<Int32>* attrs,
        /* [out] */ ITypedArray** styles);

    CARAPI ObtainStyledAttributes(
        /* [in] */ IAttributeSet* set,
        /* [in] */ ArrayOf<Int32>* attrs,
        /* [in] */ Int32 defStyleAttr,
        /* [in] */ Int32 defStyleRes,
        /* [out] */ ITypedArray** styles);

    CARAPI GetClassLoader(
        /* [out] */ IClassLoader** loader);

    CARAPI GetPackageName(
        /* [out] */ String* packageName);

    CARAPI GetApplicationInfo(
        /* [out] */ IApplicationInfo** info);

    CARAPI GetPackageResourcePath(
        /* [out] */ String* path);

    CARAPI GetPackageCodePath(
        /* [out] */ String* codePath);

    CARAPI GetSharedPrefsFile(
        /* [in] */ const String& name,
        /* [out] */ IFile** file);

    CARAPI GetSharedPreferences(
        /* [in] */ const String& name,
        /* [in] */ Int32 mode,
        /* [out] */ ISharedPreferences** prefs);

    CARAPI OpenFileInput(
        /* [in] */ const String& name,
        /* [out] */ IFileInputStream** fileInputStream);

    CARAPI OpenFileOutput(
        /* [in] */ const String& name,
        /* [in] */ Int32 mode,
        /* [out] */ IFileOutputStream** fileOutputStream);

    CARAPI DeleteFile(
        /* [in] */ const String& name,
        /* [out] */ Boolean* succeeded);

    CARAPI GetFileStreamPath(
        /* [in] */ const String& name,
        /* [out] */ IFile** file);

    CARAPI GetFilesDir(
        /* [out] */ IFile** filesDir);

    CARAPI GetExternalFilesDir(
        /* [in] */ const String& type,
        /* [out] */ IFile** filesDir);

    CARAPI GetObbDir(
        /* [out] */ IFile** obbDir);

    CARAPI GetCacheDir(
        /* [out] */ IFile** cacheDir);

    CARAPI GetExternalCacheDir(
        /* [out] */ IFile** externalDir);

    CARAPI GetFileList(
        /* [out, callee] */ ArrayOf<String>** fileList);

    CARAPI GetDir(
        /* [in] */ const String& name,
        /* [in] */ Int32 mode,
        /* [out] */ IFile** dir);

    CARAPI OpenOrCreateDatabase(
        /* [in] */ const String& name,
        /* [in] */ Int32 mode,
        /* [in] */ ISQLiteDatabaseCursorFactory* factory,
        /* [out] */ ISQLiteDatabase** sqliteDB);

    CARAPI OpenOrCreateDatabase(
        /* [in] */ const String& name,
        /* [in] */ Int32 mode,
        /* [in] */ ISQLiteDatabaseCursorFactory* factory,
        /* [in] */ IDatabaseErrorHandler* errorHandler,
        /* [out] */ ISQLiteDatabase** sqliteDB);

    CARAPI DeleteDatabase(
        /* [in] */ const String& name,
        /* [out] */ Boolean* succeeded);

    CARAPI GetDatabasePath(
        /* [in] */ const String& name,
        /* [out] */ IFile** path);

    CARAPI GetDatabaseList(
        /* [out, callee] */ ArrayOf<String>** databaseList);

    CARAPI GetWallpaper(
        /* [out] */ IDrawable** drawable);

    CARAPI PeekWallpaper(
        /* [out] */ IDrawable** drawable);

    CARAPI GetWallpaperDesiredMinimumWidth(
        /* [out] */ Int32* minWidth);

    CARAPI GetWallpaperDesiredMinimumHeight(
        /* [out] */ Int32* minHeight);

    CARAPI SetWallpaper(
        /* [in] */ IBitmap* bitmap);

    CARAPI SetWallpaper(
        /* [in] */ IInputStream* data);

    CARAPI ClearWallpaper();

    CARAPI StartActivity(
        /* [in] */ IIntent* intent);

    CARAPI StartActivityAsUser(
        /* [in] */ IIntent* intent,
        /* [in] */ IUserHandle* user);

    CARAPI StartActivity(
        /* [in] */ IIntent* intent,
        /* [in] */ IBundle* options);

    CARAPI StartActivityAsUser(
        /* [in] */ IIntent* intent,
        /* [in] */ IBundle* options,
        /* [in] */ IUserHandle* userId);

    CARAPI StartActivities(
        /* [in] */ ArrayOf<IIntent *>* intents);

    CARAPI StartActivities(
        /* [in] */ ArrayOf<IIntent *>* intents,
        /* [in] */ IBundle* options);

    CARAPI StartActivitiesAsUser(
        /* [in] */ ArrayOf<IIntent *>* intents,
        /* [in] */ IBundle* options,
        /* [in] */ IUserHandle* userHandle);

    CARAPI StartIntentSender(
        /* [in] */ IIntentSender* intent,
        /* [in] */ IIntent* fillInIntent,
        /* [in] */ Int32 flagsMask,
        /* [in] */ Int32 flagsValues,
        /* [in] */ Int32 extraFlags);

    CARAPI StartIntentSender(
        /* [in] */ IIntentSender* intent,
        /* [in] */ IIntent* fillInIntent,
        /* [in] */ Int32 flagsMask,
        /* [in] */ Int32 flagsValues,
        /* [in] */ Int32 extraFlags,
        /* [in] */ IBundle* options);

    CARAPI SendBroadcast(
        /* [in] */ IIntent* intent);

    CARAPI SendBroadcast(
        /* [in] */ IIntent* intent,
        /* [in] */ const String& receiverPermission);

    CARAPI SendOrderedBroadcast(
        /* [in] */ IIntent* intent,
        /* [in] */ const String& receiverPermission);

    CARAPI SendOrderedBroadcast(
        /* [in] */ IIntent* intent,
        /* [in] */ const String& receiverPermission,
        /* [in] */ IBroadcastReceiver* resultReceiver,
        /* [in] */ IHandler* scheduler,
        /* [in] */ Int32 initialCode,
        /* [in] */ const String& initialData,
        /* [in] */ IBundle* initialExtras);

    CARAPI SendBroadcastAsUser(
        /* [in] */ IIntent* intent,
        /* [in] */ IUserHandle* user);

    CARAPI SendBroadcastAsUser(
        /* [in] */ IIntent* intent,
        /* [in] */ IUserHandle* user,
        /* [in] */ const String& receiverPermission);

    CARAPI SendOrderedBroadcastAsUser(
        /* [in] */ IIntent* intent,
        /* [in] */ IUserHandle* user,
        /* [in] */ const String& receiverPermission,
        /* [in] */ IBroadcastReceiver* resultReceiver,
        /* [in] */ IHandler* scheduler,
        /* [in] */ Int32 initialCode,
        /* [in] */ const String& initialData,
        /* [in] */ IBundle* initialExtras);

    CARAPI SendStickyBroadcast(
        /* [in] */ IIntent* intent);

    CARAPI SendStickyOrderedBroadcast(
        /* [in] */ IIntent* intent,
        /* [in] */ IBroadcastReceiver* resultReceiver,
        /* [in] */ IHandler* scheduler,
        /* [in] */ Int32 initialCode,
        /* [in] */ const String& initialData,
        /* [in] */ IBundle* initialExtras);

    CARAPI RemoveStickyBroadcast(
        /* [in] */ IIntent* intent);

    CARAPI SendStickyBroadcastAsUser(
        /* [in] */ IIntent* intent,
        /* [in] */ IUserHandle* user);

    CARAPI SendStickyOrderedBroadcastAsUser(
        /* [in] */ IIntent* intent,
        /* [in] */ IUserHandle* user,
        /* [in] */ IBroadcastReceiver* resultReceiver,
        /* [in] */ IHandler* scheduler,
        /* [in] */ Int32 initialCode,
        /* [in] */ const String& initialData,
        /* [in] */ IBundle* initialExtras);

    CARAPI RemoveStickyBroadcastAsUser(
        /* [in] */ IIntent* intent,
        /* [in] */ IUserHandle* user);

    CARAPI RegisterReceiver(
        /* [in] */ IBroadcastReceiver* receiver,
        /* [in] */ IIntentFilter* filter,
        /* [out] */ IIntent** stickyIntent);

    CARAPI RegisterReceiver(
        /* [in] */ IBroadcastReceiver* receiver,
        /* [in] */ IIntentFilter* filter,
        /* [in] */ const String& broadcastPermission,
        /* [in] */ IHandler* scheduler,
        /* [out] */ IIntent** stickyIntent);

    CARAPI RegisterReceiverAsUser(
        /* [in] */ IBroadcastReceiver* receiver,
        /* [in] */ IUserHandle* user,
        /* [in] */ IIntentFilter* filter,
        /* [in] */ const String& broadcastPermission,
        /* [in] */ IHandler* scheduler,
        /* [out] */ IIntent** stickyIntent);

    CARAPI UnregisterReceiver(
        /* [in] */ IBroadcastReceiver* receiver);

    CARAPI StartService(
        /* [in] */ IIntent* service,
        /* [out] */ IComponentName** name);

    CARAPI StopService(
        /* [in] */ IIntent* service,
        /* [out] */ Boolean* succeeded);

    CARAPI StartServiceAsUser(
        /* [in] */ IIntent* service,
        /* [in] */ IUserHandle* user,
        /* [out] */ IComponentName** name);

    CARAPI StopServiceAsUser(
        /* [in] */ IIntent* service,
        /* [in] */ IUserHandle* user,
        /* [out] */ Boolean* succeeded);

    CARAPI BindService(
        /* [in] */ IIntent* service,
        /* [in] */ IServiceConnection* conn,
        /* [in] */ Int32 flags,
        /* [out] */ Boolean* succeeded);

    CARAPI BindService(
        /* [in] */ IIntent* service,
        /* [in] */ IServiceConnection* conn,
        /* [in] */ Int32 flags,
        /* [in] */ Int32 userHandle,
        /* [out] */ Boolean* succeeded);

    CARAPI UnbindService(
        /* [in] */ IServiceConnection* conn);

    CARAPI StartInstrumentation(
        /* [in] */ IComponentName* className,
        /* [in] */ const String& profileFile,
        /* [in] */ IBundle* arguments,
        /* [out] */ Boolean* succeeded);

    CARAPI GetSystemService(
        /* [in] */ const String& name,
        /* [out] */ IInterface** object);

    CARAPI CheckPermission(
        /* [in] */ const String& permission,
        /* [in] */ Int32 pid,
        /* [in] */ Int32 uid,
        /* [out] */ Int32* result);

    CARAPI CheckCallingPermission(
        /* [in] */ const String& permission,
        /* [out] */ Int32* result);

    CARAPI CheckCallingOrSelfPermission(
        /* [in] */ const String& permission,
        /* [out] */ Int32* result);

    CARAPI EnforcePermission(
        /* [in] */ const String& permission,
        /* [in] */ Int32 pid,
        /* [in] */ Int32 uid,
        /* [in] */ const String& message);

    CARAPI EnforceCallingPermission(
        /* [in] */ const String& permission,
        /* [in] */ const String& message);

    CARAPI EnforceCallingOrSelfPermission(
        /* [in] */ const String& permission,
        /* [in] */ const String& message);

    CARAPI GrantUriPermission(
        /* [in] */ const String& toPackage,
        /* [in] */ IUri* uri,
        /* [in] */ Int32 modeFlags);

    CARAPI RevokeUriPermission(
        /* [in] */ IUri* uri,
        /* [in] */ Int32 modeFlags);

    CARAPI CheckUriPermission(
        /* [in] */ IUri* uri,
        /* [in] */ Int32 pid,
        /* [in] */ Int32 uid,
        /* [in] */ Int32 modeFlags,
        /* [out] */ Int32* result);

    CARAPI CheckCallingUriPermission(
        /* [in] */ IUri* uri,
        /* [in] */ Int32 modeFlags,
        /* [out] */ Int32* result);

    CARAPI CheckCallingOrSelfUriPermission(
        /* [in] */ IUri* uri,
        /* [in] */ Int32 modeFlags,
        /* [out] */ Int32* result);

    CARAPI CheckUriPermission(
        /* [in] */ IUri* uri,
        /* [in] */ const String& readPermission,
        /* [in] */ const String& writePermission,
        /* [in] */ Int32 pid,
        /* [in] */ Int32 uid,
        /* [in] */ Int32 modeFlags,
        /* [out] */ Int32* result);

    CARAPI EnforceUriPermission(
        /* [in] */ IUri* uri,
        /* [in] */ Int32 pid,
        /* [in] */ Int32 uid,
        /* [in] */ Int32 modeFlags,
        /* [in] */ const String& message);

    CARAPI EnforceCallingUriPermission(
        /* [in] */ IUri* uri,
        /* [in] */ Int32 modeFlags,
        /* [in] */ const String& message);

    CARAPI EnforceCallingOrSelfUriPermission(
        /* [in] */ IUri* uri,
        /* [in] */ Int32 modeFlags,
        /* [in] */ const String& message);

    CARAPI EnforceUriPermission(
        /* [in] */ IUri* uri,
        /* [in] */ const String& readPermission,
        /* [in] */ const String& writePermission,
        /* [in] */ Int32 pid,
        /* [in] */ Int32 uid,
        /* [in] */ Int32 modeFlags,
        /* [in] */ const String& message);

    CARAPI CreatePackageContext(
        /* [in] */ const String& packageName,
        /* [in] */ Int32 flags,
        /* [out] */ IContext** ctx);

    CARAPI CreatePackageContextAsUser(
        /* [in] */ const String& packageName,
        /* [in] */ Int32 flags,
        /* [in] */ IUserHandle* user,
        /* [out] */ IContext** ctx);

    CARAPI CreateConfigurationContext(
        /* [in] */ IConfiguration* overrideConfiguration,
        /* [out] */ IContext** ctx);

    CARAPI CreateDisplayContext(
        /* [in] */ IDisplay* display,
        /* [out] */ IContext** ctx);

    CARAPI GetCompatibilityInfo(
        /* [in] */ Int32 displayId,
        /* [out] */ ICompatibilityInfoHolder** infoHolder);

    CARAPI IsRestricted(
        /* [out] */ Boolean* isRestricted);

    CARAPI ApplyOverrideConfiguration(
        /* [in] */ IConfiguration* overrideConfiguration);

    CARAPI GetBaseContext(
        /* [out] */ IContext** context);

    CARAPI ToString(
        /* [out] */ String* str);

    CARAPI GetIntent(
        /* [out] */ IIntent** intent);

    CARAPI SetIntent(
        /* [in] */ IIntent* newIntent);

    CARAPI GetApplication(
        /* [out] */ IApplication** application);

    CARAPI IsChild(
        /* [out] */ Boolean* isChild);

    CARAPI GetParent(
        /* [out] */ IActivity** parent);

    CARAPI GetWindowManager(
        /* [out] */ IWindowManager** mgr);

    CARAPI GetWindow(
        /* [out] */ IWindow** window);

    CARAPI GetLoaderManager(
        /* [out] */ ILoaderManager** loaderManager);

    CARAPI GetCurrentFocus(
        /* [out] */ IView** focus);

    CARAPI OnCreateThumbnail(
        /* [in] */ IBitmap* outBitmap,
        /* [in] */ ICanvas* canvas,
        /* [out] */ Boolean* value);

    CARAPI OnCreateDescription(
        /* [out] */ ICharSequence** description);

    CARAPI OnConfigurationChanged(
        /* [in] */ IConfiguration* newConfig);

    CARAPI GetChangingConfigurations(
        /* [out] */ Int32* config);

    CARAPI GetLastNonConfigurationInstance(
        /* [out] */ IInterface** instance);

    CARAPI OnRetainNonConfigurationInstance(
        /* [out] */ IInterface** instance);

    CARAPI GetLastNonConfigurationChildInstances(
        /* [out] */ IObjectStringMap** data);

    CARAPI OnRetainNonConfigurationChildInstances(
        /* [out] */ IObjectStringMap** objectStringMap);

    CARAPI RetainNonConfigurationInstances(
        /* [out] */ IActivityNonConfigurationInstances** instance);

    CARAPI OnLowMemory();

    CARAPI OnTrimMemory(
        /* [in] */ Int32 level);

    CARAPI GetFragmentManager(
        /* [out] */ IFragmentManager** manager);

    CARAPI OnAttachFragment(
        /* [in] */ IFragment* fragment);

    CARAPI ManagedQuery(
        /* [in] */ IUri* uri,
        /* [in] */ ArrayOf<String>* projection,
        /* [in] */ const String& selection,
        /* [in] */ const String& sortOrder,
        /* [out] */ ICursor** cursor);

    CARAPI ManagedQuery(
        /* [in] */ IUri* uri,
        /* [in] */ ArrayOf<String>* projection,
        /* [in] */ const String& selection,
        /* [in] */ ArrayOf<String>* selectionArgs,
        /* [in] */ const String& sortOrder,
        /* [out] */ ICursor** cursor);

    CARAPI StartManagingCursor(
        /* [in] */ ICursor* c);

    CARAPI StopManagingCursor(
        /* [in] */ ICursor* c);

    CARAPI SetPersistent(
        /* [in] */ Boolean isPersistent);

    CARAPI FindViewById(
        /* [in] */ Int32 id,
        /* [out] */ IView** view);

    CARAPI GetActionBar(
        /* [out] */ IActionBar** bar);

    CARAPI SetContentView(
        /* [in] */ Int32 layoutResID);

    CARAPI SetContentView(
        /* [in] */ IView* view);

    CARAPI SetContentView(
        /* [in] */ IView* view,
        /* [in] */ IViewGroupLayoutParams* params);

    CARAPI AddContentView(
        /* [in] */ IView* view,
        /* [in] */ IViewGroupLayoutParams* params);

    CARAPI SetFinishOnTouchOutside(
        /* [in] */ Boolean finish);

    CARAPI SetDefaultKeyMode(
        /* [in] */ Int32 mode);

    CARAPI OnKeyDown(
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event,
        /* [out] */ Boolean* value);

    CARAPI OnKeyLongPress(
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event,
        /* [out] */ Boolean* value);

    CARAPI OnKeyUp(
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event,
        /* [out] */ Boolean* value);

    CARAPI OnKeyMultiple(
        /* [in] */ Int32 keyCode,
        /* [in] */ Int32 repeatCount,
        /* [in] */ IKeyEvent* event,
        /* [out] */ Boolean* value);

    CARAPI OnBackPressed();

    CARAPI OnKeyShortcut(
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event,
        /* [out] */ Boolean* value);

    CARAPI OnTouchEvent(
        /* [in] */ IMotionEvent* event,
        /* [out] */ Boolean* value);

    CARAPI OnTrackballEvent(
        /* [in] */ IMotionEvent* event,
        /* [out] */ Boolean* value);

    CARAPI OnGenericMotionEvent(
        /* [in] */ IMotionEvent* event,
        /* [out] */ Boolean* value);

    CARAPI OnUserInteraction();

    CARAPI OnWindowAttributesChanged(
        /* [in] */ IWindowManagerLayoutParams* params);

    CARAPI OnContentChanged();

    CARAPI OnWindowFocusChanged(
        /* [in] */ Boolean hasFocus);

    CARAPI OnAttachedToWindow();

    CARAPI OnDetachedFromWindow();

    CARAPI HasWindowFocus(
        /* [out] */ Boolean* hasFocus);

    CARAPI DispatchKeyEvent(
        /* [in] */ IKeyEvent* event,
        /* [out] */ Boolean* res);

    CARAPI DispatchKeyShortcutEvent(
        /* [in] */ IKeyEvent* event,
        /* [out] */ Boolean* res);

    CARAPI DispatchTouchEvent(
        /* [in] */ IMotionEvent* ev,
        /* [out] */ Boolean* res);

    CARAPI DispatchTrackballEvent(
        /* [in] */ IMotionEvent* ev,
        /* [out] */ Boolean* res);

    CARAPI DispatchGenericMotionEvent(
        /* [in] */ IMotionEvent* ev,
        /* [out] */ Boolean* res);

    CARAPI DispatchPopulateAccessibilityEvent(
        /* [in] */ IAccessibilityEvent* event,
        /* [out] */ Boolean* res);

    CARAPI OnCreatePanelView(
        /* [in] */ Int32 featureId,
        /* [out] */ IView** view);

    CARAPI OnCreatePanelMenu(
        /* [in] */ Int32 featureId,
        /* [in] */ IMenu* menu,
        /* [out] */ Boolean* res);

    CARAPI OnPreparePanel(
        /* [in] */ Int32 featureId,
        /* [in] */ IView* view,
        /* [in] */ IMenu* menu,
        /* [out] */ Boolean* res);

    CARAPI OnMenuOpened(
        /* [in] */ Int32 featureId,
        /* [in] */ IMenu* menu,
        /* [out] */ Boolean* opened);

    CARAPI OnMenuItemSelected(
        /* [in] */ Int32 featureId,
        /* [in] */ IMenuItem* item,
        /* [out] */ Boolean* selected);

    CARAPI OnPanelClosed(
        /* [in] */ Int32 featureId,
        /* [in] */ IMenu* menu);

    CARAPI InvalidateOptionsMenu();

    CARAPI OnCreateOptionsMenu(
        /* [in] */ IMenu* menu,
        /* [out] */ Boolean* displayed);

    CARAPI OnPrepareOptionsMenu(
        /* [in] */ IMenu* menu,
        /* [out] */ Boolean* displayed);

    CARAPI OnOptionsItemSelected(
        /* [in] */ IMenuItem* item,
        /* [out] */ Boolean* selected);

    CARAPI OnNavigateUp(
        /* [out] */ Boolean* result);

    CARAPI OnNavigateUpFromChild(
        /* [in] */ IActivity* child,
        /* [out] */ Boolean* result);

    CARAPI OnCreateNavigateUpTaskStack(
        /* [in] */ ITaskStackBuilder* builder);

    CARAPI OnPrepareNavigateUpTaskStack(
        /* [in] */ ITaskStackBuilder* builder);

    CARAPI OnOptionsMenuClosed(
        /* [in] */ IMenu* menu);

    CARAPI OpenOptionsMenu();

    CARAPI CloseOptionsMenu();

    CARAPI OnCreateContextMenu(
        /* [in] */ IContextMenu* menu,
        /* [in] */ IView* v,
        /* [in] */ IContextMenuInfo* menuInfo);

    CARAPI RegisterForContextMenu(
        /* [in] */ IView* view);

    CARAPI UnregisterForContextMenu(
        /* [in] */ IView* view);

    CARAPI OpenContextMenu(
        /* [in] */ IView* view);

    CARAPI CloseContextMenu();

    CARAPI OnContextItemSelected(
        /* [in] */ IMenuItem* item,
        /* [out] */ Boolean* res);

    CARAPI OnContextMenuClosed(
        /* [in] */ IMenu* menu);

    CARAPI ShowDialog(
        /* [in] */ Int32 id);

    CARAPI ShowDialog(
        /* [in] */ Int32 id,
        /* [in] */ IBundle* args,
        /* [out] */ Boolean* res);

    CARAPI DismissDialog(
        /* [in] */ Int32 id);

    CARAPI RemoveDialog(
        /* [in] */ Int32 id);

    CARAPI OnSearchRequested(
        /* [out] */ Boolean* res);

    CARAPI StartSearch(
        /* [in] */ const String& initialQuery,
        /* [in] */ Boolean selectInitialQuery,
        /* [in] */ IBundle* appSearchData,
        /* [in] */ Boolean globalSearch);

    CARAPI TriggerSearch(
        /* [in] */ const String& query,
        /* [in] */ IBundle* appSearchData);

    CARAPI TakeKeyEvents(
        /* [in] */ Boolean get);

    CARAPI RequestWindowFeature(
        /* [in] */ Int32 featureId,
        /* [out] */ Boolean* result);

    CARAPI SetFeatureDrawableResource(
        /* [in] */ Int32 featureId,
        /* [in] */ Int32 resId);

    CARAPI SetFeatureDrawableUri(
        /* [in] */ Int32 featureId,
        /* [in] */ IUri* uri);

    CARAPI SetFeatureDrawable(
        /* [in] */ Int32 featureId,
        /* [in] */ IDrawable* drawable);

    CARAPI SetFeatureDrawableAlpha(
        /* [in] */ Int32 featureId,
        /* [in] */ Int32 alpha);

    CARAPI GetLayoutInflater(
        /* [out] */ ILayoutInflater** inflater);

    CARAPI GetMenuInflater(
        /* [out] */ IMenuInflater** flater);

    CARAPI StartActivityForResult(
        /* [in] */ IIntent* intent,
        /* [in] */ Int32 requestCode);

    CARAPI StartActivityForResult(
        /* [in] */ IIntent* intent,
        /* [in] */ Int32 requestCode,
        /* [in] */ IBundle* options);

    CARAPI StartIntentSenderForResult(
        /* [in] */ IIntentSender* intent,
        /* [in] */ Int32 requestCode,
        /* [in] */ IIntent* fillInIntent,
        /* [in] */ Int32 flagsMask,
        /* [in] */ Int32 flagsValues,
        /* [in] */ Int32 extraFlags);

    CARAPI StartIntentSenderForResult(
        /* [in] */ IIntentSender* intent,
        /* [in] */ Int32 requestCode,
        /* [in] */ IIntent* fillInIntent,
        /* [in] */ Int32 flagsMask,
        /* [in] */ Int32 flagsValues,
        /* [in] */ Int32 extraFlags,
        /* [in] */ IBundle* options);

    CARAPI StartActivityIfNeeded(
        /* [in] */ IIntent* intent,
        /* [in] */ Int32 requestCode,
        /* [out] */ Boolean* started);

    CARAPI StartActivityIfNeeded(
        /* [in] */ IIntent* intent,
        /* [in] */ Int32 requestCode,
        /* [in] */ IBundle* options,
        /* [out] */ Boolean* started);

    CARAPI StartNextMatchingActivity(
        /* [in] */ IIntent* intent,
        /* [out] */ Boolean* started);

    CARAPI StartNextMatchingActivity(
        /* [in] */ IIntent* intent,
        /* [in] */ IBundle* options,
        /* [out] */ Boolean* started);

    CARAPI StartActivityFromChild(
        /* [in] */ IActivity* child,
        /* [in] */ IIntent* intent,
        /* [in] */ Int32 requestCode);

    CARAPI StartActivityFromChild(
        /* [in] */ IActivity* child,
        /* [in] */ IIntent* intent,
        /* [in] */ Int32 requestCode,
        /* [in] */ IBundle* options);

    CARAPI StartActivityFromFragment(
        /* [in] */ IFragment* fragment,
        /* [in] */ IIntent* intent,
        /* [in] */ Int32 requestCode);

    CARAPI StartActivityFromFragment(
        /* [in] */ IFragment* fragment,
        /* [in] */ IIntent* intent,
        /* [in] */ Int32 requestCode,
        /* [in] */ IBundle* options);

    CARAPI StartIntentSenderFromChild(
        /* [in] */ IActivity* child,
        /* [in] */ IIntentSender* intent,
        /* [in] */ Int32 requestCode,
        /* [in] */ IIntent* fillInIntent,
        /* [in] */ Int32 flagsMask,
        /* [in] */ Int32 flagsValues,
        /* [in] */ Int32 extraFlags);

    CARAPI StartIntentSenderFromChild(
        /* [in] */ IActivity* child,
        /* [in] */ IIntentSender* intent,
        /* [in] */ Int32 requestCode,
        /* [in] */ IIntent* fillInIntent,
        /* [in] */ Int32 flagsMask,
        /* [in] */ Int32 flagsValues,
        /* [in] */ Int32 extraFlags,
        /* [in] */ IBundle* options);

    CARAPI OverridePendingTransition(
        /* [in] */ Int32 enterAnim,
        /* [in] */ Int32 exitAnim);

    CARAPI SetResult(
        /* [in] */ Int32 resultCode);

    CARAPI SetResult(
        /* [in] */ Int32 resultCode,
        /* [in] */ IIntent* data);

    CARAPI GetCallingPackage(
        /* [out] */ String* package);

    CARAPI GetCallingActivity(
        /* [out] */ IComponentName** name);

    CARAPI SetVisible(
        /* [in] */ Boolean visible);

    CARAPI SetCalled(
        /* [in] */ Boolean called);

    CARAPI IsCalled(
        /* [out] */ Boolean* called);

    CARAPI SetFinishing(
        /* [in] */ Boolean finished);

    CARAPI IsFinishing(
        /* [out] */ Boolean* isFinished);

    CARAPI IsDestroyed(
        /* [out] */ Boolean* isDestroyed);

    CARAPI IsChangingConfigurations(
        /* [out] */ Boolean* isChanged);

    CARAPI Recreate();

    CARAPI Finish();

    CARAPI FinishAffinity();

    CARAPI FinishFromChild(
        /* [in] */ IActivity* child);

    CARAPI FinishActivity(
        /* [in] */ Int32 requestCode);

    CARAPI FinishActivityFromChild(
        /* [in] */ IActivity* child,
        /* [in] */ Int32 requestCode);

    CARAPI CreatePendingResult(
        /* [in] */ Int32 requestCode,
        /* [in] */ IIntent* data,
        /* [in] */ Int32 flags,
        /* [out] */ IPendingIntent** pendingIntent);

    CARAPI SetRequestedOrientation(
        /* [in] */ Int32 requestedOrientation);

    CARAPI GetRequestedOrientation(
        /* [out] */ Int32* orientation);

    CARAPI GetTaskId(
        /* [out] */ Int32* id);

    CARAPI IsTaskRoot(
        /* [out] */ Boolean* isRoot);

    CARAPI MoveTaskToBack(
        /* [in] */ Boolean nonRoot,
        /* [out] */ Boolean* moved);

    CARAPI GetLocalClassName(
        /* [out] */ String* className);

    CARAPI GetComponentName(
        /* [out] */ IComponentName** componentName);

    CARAPI GetPreferences(
        /* [in] */ Int32 mode,
        /* [out] */ ISharedPreferences** perferences);

    CARAPI SetTitle(
        /* [in] */ ICharSequence* title);

    CARAPI SetTitle(
        /* [in] */ Int32 titleId);

    CARAPI SetTitleColor(
        /* [in] */ Int32 textColor);

    CARAPI GetTitle(
        /* [out] */ ICharSequence** title);

    CARAPI GetTitleColor(
        /* [out] */ Int32* color);

    CARAPI OnTitleChanged(
        /* [in] */ ICharSequence* title,
        /* [in] */ Int32 color);

    CARAPI OnChildTitleChanged(
        /* [in] */ IActivity* childActivity,
        /* [in] */ ICharSequence* title);

    CARAPI SetProgressBarVisibility(
        /* [in] */ Boolean visible);

    CARAPI SetProgressBarIndeterminateVisibility(
        /* [in] */ Boolean visible);

    CARAPI SetProgressBarIndeterminate(
        /* [in] */ Boolean indeterminate);

    CARAPI SetProgress(
        /* [in] */ Int32 progress);

    CARAPI SetSecondaryProgress(
        /* [in] */ Int32 secondaryProgress);

    CARAPI SetVolumeControlStream(
        /* [in] */ Int32 streamType);

    CARAPI GetVolumeControlStream(
        /* [out] */ Int32* type);

    CARAPI RunOnUiThread(
        /* [in] */ IRunnable* action);

    CARAPI OnCreateView(
        /* [in] */ const String& name,
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [out] */ IView** view);

    CARAPI OnCreateView(
        /* [in] */ IView* parent,
        /* [in] */ const String& name,
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [out] */ IView** view);

    CARAPI Dump(
        /* [in] */ const String& prefix,
        /* [in] */ IFileDescriptor* fd,
        /* [in] */ IPrintWriter* writer,
        /* [in] */ ArrayOf<String>* args);

    CARAPI IsImmersive(
        /* [out] */ Boolean* isImmersive);

    CARAPI SetImmersive(
        /* [in] */ Boolean i);

    CARAPI StartActionMode(
        /* [in] */ IActionModeCallback* cb,
        /* [out] */ IActionMode** mode);

    CARAPI OnWindowStartingActionMode(
        /* [in] */ IActionModeCallback* cb,
        /* [out] */ IActionMode** mode);

    CARAPI OnActionModeStarted(
        /* [in] */ IActionMode* mode);

    CARAPI OnActionModeFinished(
        /* [in] */ IActionMode* mode);

    CARAPI ShouldUpRecreateTask(
        /* [in] */ IIntent* targetIntent,
        /* [out] */ Boolean* res);

    CARAPI NavigateUpTo(
        /* [in] */ IIntent* upIntent,
        /* [out] */ Boolean* success);

    CARAPI NavigateUpToFromChild(
        /* [in] */ IActivity* child,
        /* [in] */ IIntent* upIntent,
        /* [out] */ Boolean* success);

    CARAPI GetParentActivityIntent(
        /* [out] */ IIntent** intent);

    CARAPI Attach(
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
        /* [in] */ IConfiguration* config);

    CARAPI Attach(
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
        /* [in] */ IConfiguration* config);

    CARAPI GetActivityToken(
        /* [out] */ IBinder** token);

    CARAPI IsResumed(
        /* [out] */ Boolean* isResumed);

    CARAPI SetParent(
        /* [in] */ IActivity* parent);

    CARAPI GetID(
        /* [out] */ String* id);

    CARAPI GetToken(
        /* [out] */ IBinder** token);

    CARAPI IsStartedActivity(
        /* [out] */ Boolean* isStartedActivity);

    CARAPI GetWindow(
        /* [out] */ IWindow** window);

    CARAPI SetDecorView(
        /* [in] */ IView* decor);

    CARAPI GetDecorView(
        /* [out] */ IView** decor);

    CARAPI SetVisibleFromServer(
        /* [in] */ Boolean visible);

    CARAPI IsVisibleFromServer(
        /* [out] */ Boolean* visible);

    CARAPI IsVisibleFromClient(
        /* [out] */ Boolean* visible);

    CARAPI SetWindowAdded(
        /* [in] */ Boolean added);

    CARAPI IsWindowAdded(
        /* [out] */ Boolean* isAdded);

    CARAPI GetWindowManager(
        /* [out] */ IWindowManager** mgr);

    CARAPI SetConfigChangeFlags(
        /* [in] */ Int32 configChangeFlags);

    CARAPI GetConfigChangeFlags(
        /* [out] */ Int32* configChangeFlags);

    CARAPI MakeVisible();

    CARAPI SetStartedActivity(
        /* [in] */ Boolean started);

    CARAPI PerformSaveInstanceState(
        /* [in] */ IBundle* outState);

    CARAPI PerformRestoreInstanceState(
        /* [in] */ IBundle* savedInstanceState);

    CARAPI PerformCreate(
        /* [in] */ IBundle* icicle);

    CARAPI PerformStart();

    CARAPI PerformRestart();

    CARAPI PerformResume();

    CARAPI PerformPause();

    CARAPI PerformUserLeaving();

    CARAPI PerformStop();

    CARAPI PerformDestroy();

    CARAPI OnItemClick(
        /* [in] */ IAdapterView* parent,
        /* [in] */ IView* view,
        /* [in] */ Int32 position,
        /* [in] */ Int64 id);

    CARAPI OnCreate(
        /* [in] */ IBundle* savedInstanceState);

    CARAPI Initialize();

    CARAPI OnUserLeaveHint();

    CARAPI OnActivityResult(
        /* [in] */ Int32 requestCode,
        /* [in] */ Int32 resultCode,
        /* [in] */ IIntent *data);

    CARAPI OnSaveInstanceState(
        /* [in] */ IBundle* outState);

    CARAPI AttachBaseContext(
        /* [in] */ IContext* base);

    CARAPI OnApplyThemeResource(
        /* [in] */ IResourcesTheme* theme,
        /* [in] */ Int32 resid,
        /* [in] */ Boolean first);

    CARAPI OnRestoreInstanceState(
        /* [in] */ IBundle* savedInstanceState);

    CARAPI_(AutoPtr<IDialog>) CreateDialog(
        /* [in] */ Int32 dialogId,
        /* [in] */ IBundle* state,
        /* [in] */ IBundle* args);

    CARAPI OnPostCreate(
        /* [in] */ IBundle* savedInstanceState);

    CARAPI OnStart();

    CARAPI OnRestart();

    CARAPI OnResume();

    CARAPI OnPause();

    CARAPI OnPostResume();

    CARAPI OnStop();

    CARAPI OnDestroy();

    CARAPI OnNewIntent(
        /* [in] */ IIntent *intent);

    CARAPI_(AutoPtr<IDialog>) OnCreateDialog(
        /* [in] */ Int32 id);

    CARAPI_(AutoPtr<IDialog>) OnCreateDialog(
        /* [in] */ Int32 id,
        /* [in] */ IBundle* args);

    CARAPI_(void) OnPrepareDialog(
        /* [in] */ Int32 id,
        /* [in] */ IDialog* dialog);

    CARAPI_(void) OnPrepareDialog(
        /* [in] */ Int32 id,
        /* [in] */ IDialog* dialog,
        /* [in] */ IBundle* args);

private:
    CARAPI StartSyncReallyDelete();

    CARAPI StartSyncUndoDeletes();

private:
    Int64 mNumDeletes;
    AutoPtr<IAccount> mAccount;
    String mAuthority;
    String mProvider;

};

}
}
}

#endif // __ELASTOS_DROID_CONTENT_CSYNCACTIVITYTOOMANYDELETES_H__
