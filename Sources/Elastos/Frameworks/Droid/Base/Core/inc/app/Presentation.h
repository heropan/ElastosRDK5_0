#ifndef __PRESENTATION_H__
#define __PRESENTATION_H__

#include "os/HandlerBase.h"
#include "app/Dialog.h"
#include "app/DialogMacro.h"
#include "content/ContextMacro.h"
#include "view/ContextThemeWrapper.h"

using Elastos::Droid::Os::HandlerBase;
using Elastos::Droid::App::Dialog;
using Elastos::Droid::View::IDisplay;
using Elastos::Droid::View::IContextThemeWrapper;
using Elastos::Droid::View::ContextThemeWrapper;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::IIntent;
using Elastos::Droid::Content::IIntentFilter;
using Elastos::Droid::Content::IIntentSender;
using Elastos::Droid::Content::IContentResolver;
using Elastos::Droid::Content::IServiceConnection;
using Elastos::Droid::Content::IBroadcastReceiver;
using Elastos::Droid::Content::IComponentCallbacks;
using Elastos::Droid::Content::ISharedPreferences;
using Elastos::Droid::Content::Res::IResources;
using Elastos::Droid::Hardware::Display::IDisplayListener;
using Elastos::Droid::Hardware::Display::IDisplayManager;

namespace Elastos{
namespace Droid{
namespace App{

class Presentation
    : public Dialog
{
private:
    class PresentationDisplayListener
        : public IDisplayListener
        , public ElRefBase
    {
    public:
        PresentationDisplayListener(
            /* [in] */ Presentation* host)
            : mHost(host)
        {}

        CAR_INTERFACE_DECL();

        CARAPI OnDisplayAdded(
            /* [in] */ Int32 displayId);

        CARAPI OnDisplayRemoved(
            /* [in] */ Int32 displayId);

        CARAPI OnDisplayChanged(
            /* [in] */ Int32 displayId);

    private:
        Presentation* mHost;
    };

    class PresentationHandler
        : public HandlerBase
    {
    public:
        PresentationHandler(
            /* [in] */ Presentation* host)
            : mHost(host)
        {}

        CARAPI HandleMessage(
            /* [in] */ IMessage* msg);

    private:
        Presentation* mHost;
    };

    class PresentationContextThemeWrapper
        : public ElRefBase
        , public IContextThemeWrapper
        , public ContextThemeWrapper
    {
    public:
        PresentationContextThemeWrapper(
            /* [in] */ IContext* base,
            /* [in] */ Int32 theme,
            /* [in] */ IWindowManager* displayWindowManager)
            : mBase(base)
            , mTheme(theme)
            , mDisplayWindowManager(displayWindowManager)
        {}

        CARAPI GetSystemService(
            /* [in] */ const String& name,
            /* [out] */ IInterface** object);

        CAR_INTERFACE_DECL();

        // IContextThemeWrapper Declare
        CARAPI GetBaseContext(
            /* [out] */ IContext** context);

        CARAPI ApplyOverrideConfiguration(
            /* [in] */ IConfiguration* overrideConfiguration);

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

        CARAPI GetStringEx(
            /* [in] */ Int32 resId,
            /* [in] */ ArrayOf<IInterface*>* formatArgs,
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

        CARAPI ObtainStyledAttributesEx(
            /* [in] */ Int32 resid,
            /* [in] */ ArrayOf<Int32>* attrs,
            /* [out] */ ITypedArray** styles);

        CARAPI ObtainStyledAttributesEx2(
            /* [in] */ IAttributeSet* set,
            /* [in] */ ArrayOf<Int32>* attrs,
            /* [out] */ ITypedArray** styles);

        CARAPI ObtainStyledAttributesEx3(
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

        CARAPI OpenOrCreateDatabaseEx(
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

        CARAPI SetWallpaperEx(
            /* [in] */ IInputStream* data);

        CARAPI ClearWallpaper();

        CARAPI StartActivity(
            /* [in] */ IIntent* intent);

        CARAPI StartActivityAsUser(
            /* [in] */ IIntent* intent,
            /* [in] */ IUserHandle* user);

        CARAPI StartActivityEx(
            /* [in] */ IIntent* intent,
            /* [in] */ IBundle* options);

        CARAPI StartActivityAsUserEx(
            /* [in] */ IIntent* intent,
            /* [in] */ IBundle* options,
            /* [in] */ IUserHandle* userId);

        CARAPI StartActivities(
            /* [in] */ ArrayOf<IIntent*>* intents);

        CARAPI StartActivitiesEx(
            /* [in] */ ArrayOf<IIntent*>* intents,
            /* [in] */ IBundle* options);

        CARAPI StartActivitiesAsUser(
            /* [in] */ ArrayOf<IIntent*>* intents,
            /* [in] */ IBundle* options,
            /* [in] */ IUserHandle* userHandle);

        CARAPI StartIntentSender(
            /* [in] */ IIntentSender* intent,
            /* [in] */ IIntent* fillInIntent,
            /* [in] */ Int32 flagsMask,
            /* [in] */ Int32 flagsValues,
            /* [in] */ Int32 extraFlags);

        CARAPI StartIntentSenderEx(
            /* [in] */ IIntentSender* intent,
            /* [in] */ IIntent* fillInIntent,
            /* [in] */ Int32 flagsMask,
            /* [in] */ Int32 flagsValues,
            /* [in] */ Int32 extraFlags,
            /* [in] */ IBundle* options);

        CARAPI SendBroadcast(
            /* [in] */ IIntent* intent);

        CARAPI SendBroadcastEx(
            /* [in] */ IIntent* intent,
            /* [in] */ const String& receiverPermission);

        CARAPI SendOrderedBroadcast(
            /* [in] */ IIntent* intent,
            /* [in] */ const String& receiverPermission);

        CARAPI SendOrderedBroadcastEx(
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

        CARAPI SendBroadcastAsUserEx(
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

        CARAPI RegisterReceiverEx(
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

        CARAPI BindServiceEx(
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

        CARAPI CheckPermission(
            /* [in] */ const String& permission,
            /* [in] */ Int32 pid,
            /* [in] */ Int32 uid,
            /* [out] */ Int32* permissionId);

        CARAPI CheckCallingPermission(
            /* [in] */ const String& permission,
            /* [out] */ Int32* permissionId);

        CARAPI CheckCallingOrSelfPermission(
            /* [in] */ const String& permission,
            /* [out] */ Int32* permissionId);

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
            /* [out] */ Int32* permissionId);

        CARAPI CheckCallingUriPermission(
            /* [in] */ IUri* uri,
            /* [in] */ Int32 modeFlags,
            /* [out] */ Int32* permissionId);

        CARAPI CheckCallingOrSelfUriPermission(
            /* [in] */ IUri* uri,
            /* [in] */ Int32 modeFlags,
            /* [out] */ Int32* permissionId);

        CARAPI CheckUriPermissionEx(
            /* [in] */ IUri* uri,
            /* [in] */ const String& readPermission,
            /* [in] */ const String& writePermission,
            /* [in] */ Int32 pid,
            /* [in] */ Int32 uid,
            /* [in] */ Int32 modeFlags,
            /* [out] */ Int32* permissionId);

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

        CARAPI EnforceUriPermissionEx(
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

    private:
        AutoPtr<IContext> mBase;
        Int32 mTheme;
        AutoPtr<IWindowManager> mDisplayWindowManager;
    };

public:
    Presentation();

    ~Presentation();

    Presentation(
        /* [in] */ IContext * context,
        /* [in] */ IDisplay * display);

    Presentation(
        /* [in] */ IContext * context,
        /* [in] */ IDisplay * display,
        /* [in] */ Int32 theme);

    /**
     * Gets the {@link Display} that this presentation appears on.
     *
     * @return The display.
     */
    CARAPI GetDisplay(
        /* [out] */ IDisplay **display);

    /**
     * Gets the {@link Resources} that should be used to inflate the layout of this presentation.
     * This resources object has been configured according to the metrics of the
     * display that the presentation appears on.
     *
     * @return The presentation resources object.
     */
    CARAPI GetResources(
        /* [out] */ IResources **resources);

    /**
     * Called by the system when the {@link Display} to which the presentation
     * is attached has been removed.
     *
     * The system automatically calls {@link #cancel} to dismiss the presentation
     * after sending this event.
     *
     * @see #getDisplay
     */
    CARAPI OnDisplayRemoved();

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
    CARAPI OnDisplayChanged();

    /**
     * Inherited from {@link Dialog#show}. Will throw
     * {@link android.view.WindowManager.InvalidDisplayException} if the specified secondary
     * {@link Display} can't be found.
     */
    //@Override
    // CARAPI Show();

protected:
    CARAPI Init(
        /* [in] */ IContext * outerContext,
        /* [in] */ IDisplay * display,
        /* [in] */ Int32 theme);

    CARAPI_(void) OnStart();

    CARAPI_(void) OnStop();

private:
    CARAPI_(void) HandleDisplayRemoved();

    CARAPI_(void) HandleDisplayChanged();

    CARAPI_(Boolean) IsConfigurationStillValid();

    static CARAPI_(AutoPtr<IContext>) CreatePresentationContext(
        /* [in] */ IContext* outerContext,
        /* [in] */ IDisplay* display,
        /* [in] */ Int32 theme);

private:
    static const String TAG; // = "Presentation";

    AutoPtr<IDisplay> mDisplay;
    AutoPtr<IDisplayManager> mDisplayManager;
    AutoPtr<PresentationDisplayListener> mDisplayListener;
    AutoPtr<PresentationHandler> mHandler;
};

} // namespace App
} // namespace Droid
} // namespace Elastos

#endif // __PRESENTATION_H__
