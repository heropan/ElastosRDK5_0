
#ifndef __WallpaperService_H__
#define __WallpaperService_H__

#include "app/Service.h"
#include "content/BroadcastReceiver.h"
#include "view/BaseSurfaceHolder.h"
#include "view/InputEventReceiver.h"

using Elastos::Droid::App::Service;
using Elastos::Droid::Content::BroadcastReceiver;
using Elastos::Droid::Os::IHandlerCaller;
using namespace Elastos::Droid::View;

namespace Elastos {
namespace Droid {
namespace Service {
namespace Wallpaper {

/**
 * A wallpaper service is responsible for showing a live wallpaper behind
 * applications that would like to sit on top of it.  This service object
 * itself does very little -- its only purpose is to generate instances of
 * {@link Engine} as needed.  Implementing a wallpaper thus
 * involves subclassing from this, subclassing an Engine implementation,
 * and implementing {@link #onCreateEngine()} to return a new instance of
 * your engine.
 */
class WallpaperService : public Elastos::Droid::App::Service
{
    friend class CWallpaperEngineWrapper;
    friend class CWallpaperServiceEngineWindow;
public:
    /**
     * The {@link Intent} that must be declared as handled by the service.
     * To be supported, the service must also require the
     * {@link android.Manifest.permission#BIND_WALLPAPER} permission so
     * that other applications can not abuse it.
     */
    static const String SERVICE_INTERFACE;
    /**
     * Name under which a WallpaperService component publishes information
     * about itself.  This meta-data must reference an XML resource containing
     * a <code>&lt;{@link android.R.styleable#Wallpaper wallpaper}&gt;</code>
     * tag.
     */
    static const String SERVICE_META_DATA;

protected:
    static const String TAG;
    static const Boolean DEBUG;

    class WallpaperCommand;

private:
    static const Int32 MSG_UPDATE_SURFACE = 10000;
    static const Int32 MSG_VISIBILITY_CHANGED = 10010;
    static const Int32 MSG_WALLPAPER_OFFSETS = 10020;
    static const Int32 MSG_WALLPAPER_COMMAND = 10025;
    static const Int32 MSG_WINDOW_RESIZED = 10030;
    static const Int32 MSG_WINDOW_MOVED = 10035;
    static const Int32 MSG_TOUCH_EVENT = 10040;

public:

    /**
     * The actual implementation of a wallpaper.  A wallpaper service may
     * have multiple instances running (for example as a real wallpaper
     * and as a preview), each of which is represented by its own Engine
     * instance.  You must implement {@link WallpaperService#onCreateEngine()}
     * to return your concrete Engine implementation.
     */
    class Engine : public ElRefBase
    {
        friend class WallpaperService;
        friend class CWallpaperEngineWrapper;
        friend class CWallpaperServiceEngineWindow;
    protected:
        class MyBroadcastReceiver : public BroadcastReceiver
        {
        public:
            MyBroadcastReceiver(
                /* [in] */ Engine* owner);

            CARAPI OnReceive(
                /* [in] */ IContext* context,
                /* [in] */ IIntent* intent);

            CARAPI ToString(
                /* [out] */ String* info)
            {
                VALIDATE_NOT_NULL(info);
                *info = String("WallpaperService::Engine::MyBroadcastReceiver: ");
                (*info).AppendFormat("%p", this);
                return NOERROR;
            }
        private:
            Engine* mOwner;
        };

        class MySurfaceHolder : public BaseSurfaceHolder
        {
        public:
            MySurfaceHolder(
                /* [in] */ Engine* owner);

            CARAPI_(Boolean) OnAllowLockCanvas();

            CARAPI_(void) OnRelayoutContainer();

            CARAPI OnUpdateSurface();

            CARAPI IsCreating(
                /* [out] */ Boolean* isCreating);

            CARAPI SetFixedSize(
                /* [in] */ Int32 width,
                /* [in] */ Int32 height);

            CARAPI SetKeepScreenOn(
                /* [in] */ Boolean screenOn);

        private:
            Engine* mOwner;
        };

        class WallpaperInputEventReceiver : public InputEventReceiver
        {
        public:
            WallpaperInputEventReceiver(
                /* [in] */ IInputChannel* inputChannel,
                /* [in] */ ILooper* looper,
                /* [in] */ Engine* owner);

            CARAPI OnInputEvent(
                /* [in] */ IInputEvent* event);

        private:
           Engine* mOwner;
        };

    public:
        Engine(
            /* [in] */ WallpaperService* owner);

        virtual ~Engine();

        /**
         * Provides access to the surface in which this wallpaper is drawn.
         */
        virtual CARAPI_(AutoPtr<ISurfaceHolder>) GetSurfaceHolder();

        /**
         * Convenience for {@link WallpaperManager#getDesiredMinimumWidth()
         * WallpaperManager.getDesiredMinimumWidth()}, returning the width
         * that the system would like this wallpaper to run in.
         */
        virtual CARAPI_(Int32) GetDesiredMinimumWidth();

        /**
         * Convenience for {@link WallpaperManager#getDesiredMinimumHeight()
         * WallpaperManager.getDesiredMinimumHeight()}, returning the height
         * that the system would like this wallpaper to run in.
         */
        virtual CARAPI_(Int32) GetDesiredMinimumHeight();

        /**
         * Return whether the wallpaper is currently visible to the user,
         * this is the last value supplied to
         * {@link #onVisibilityChanged(boolean)}.
         */
        virtual CARAPI_(Boolean) IsVisible();

        /**
         * Returns true if this engine is running in preview mode -- that is,
         * it is being shown to the user before they select it as the actual
         * wallpaper.
         */
        virtual CARAPI_(Boolean) IsPreview();

        /**
         * Control whether this wallpaper will receive raw touch events
         * from the window manager as the user interacts with the window
         * that is currently displaying the wallpaper.  By default they
         * are turned off.  If enabled, the events will be received in
         * {@link #onTouchEvent(MotionEvent)}.
         */
        virtual CARAPI_(void) SetTouchEventsEnabled(
            /* [in] */ Boolean enabled);

        /**
         * Control whether this wallpaper will receive notifications when the wallpaper
         * has been scrolled. By default, wallpapers will receive notifications, although
         * the default static image wallpapers do not. It is a performance optimization to
         * set this to false.
         *
         * @param enabled whether the wallpaper wants to receive offset notifications
         */
        virtual CARAPI_(void) SetOffsetNotificationsEnabled(
            /* [in] */ Boolean enabled);

        /** {@hide} */
        virtual CARAPI_(void) SetFixedSizeAllowed(
            /* [in] */ Boolean  allowed);

        /**
         * Called once to initialize the engine.  After returning, the
         * engine's surface will be created by the framework.
         */
        virtual CARAPI_(void) OnCreate(
            /* [in] */ ISurfaceHolder* surfaceHolder);

        /**
         * Called right before the engine is going away.  After this the
         * surface will be destroyed and this Engine object is no longer
         * valid.
         */
        virtual CARAPI_(void) OnDestroy();

        /**
         * Called to inform you of the wallpaper becoming visible or
         * hidden.  <em>It is very important that a wallpaper only use
         * CPU while it is visible.</em>.
         */
        virtual CARAPI_(void) OnVisibilityChanged(
            /* [in] */ Boolean visible);

        /**
         * Called as the user performs touch-screen interaction with the
         * window that is currently showing this wallpaper.  Note that the
         * events you receive here are driven by the actual application the
         * user is interacting with, so if it is slow you will get fewer
         * move events.
         */
        virtual CARAPI_(void) OnTouchEvent(
            /* [in] */ IMotionEvent* event);

        /**
         * Called to inform you of the wallpaper's offsets changing
         * within its contain, corresponding to the container's
         * call to {@link WallpaperManager#setWallpaperOffsets(IBinder, float, float)
         * WallpaperManager.setWallpaperOffsets()}.
         */
        virtual CARAPI_(void) OnOffsetsChanged(
            /* [in] */ Float xOffset,
            /* [in] */ Float yOffset,
            /* [in] */ Float xOffsetStep,
            /* [in] */ Float yOffsetStep,
            /* [in] */ Int32 xPixelOffset,
            /* [in] */ Int32 yPixelOffset);

        /**
         * Process a command that was sent to the wallpaper with
         * {@link WallpaperManager#sendWallpaperCommand}.
         * The default implementation does nothing, and always returns null
         * as the result.
         *
         * @param action The name of the command to perform.  This tells you
         * what to do and how to interpret the rest of the arguments.
         * @param x Generic integer parameter.
         * @param y Generic integer parameter.
         * @param z Generic integer parameter.
         * @param extras Any additional parameters.
         * @param resultRequested If true, the caller is requesting that
         * a result, appropriate for the command, be returned back.
         * @return If returning a result, create a Bundle and place the
         * result data in to it.  Otherwise return null.
         */
        virtual CARAPI_(AutoPtr<IBundle>) OnCommand(
            /* [in] */ const String& action,
            /* [in] */ Int32 x,
            /* [in] */ Int32 y,
            /* [in] */ Int32 z,
            /* [in] */ IBundle* extras,
            /* [in] */ Boolean resultRequested);

        /**
         * Called when an application has changed the desired virtual size of
         * the wallpaper.
         */
        virtual CARAPI_(void) OnDesiredSizeChanged(
            /* [in] */ Int32 desiredWidth,
            /* [in] */ Int32 desiredHeight);

        /**
         * Convenience for {@link SurfaceHolder.Callback#surfaceChanged
         * SurfaceHolder.Callback.surfaceChanged()}.
         */
        virtual CARAPI_(void) OnSurfaceChanged(
            /* [in] */ ISurfaceHolder* holder,
            /* [in] */ Int32 format,
            /* [in] */ Int32 width,
            /* [in] */ Int32 height);

        /**
         * Convenience for {@link SurfaceHolder.Callback2#surfaceRedrawNeeded
         * SurfaceHolder.Callback.surfaceRedrawNeeded()}.
         */
        virtual CARAPI_(void) OnSurfaceRedrawNeeded(
            /* [in] */ ISurfaceHolder* holder);

        /**
         * Convenience for {@link SurfaceHolder.Callback#surfaceCreated
         * SurfaceHolder.Callback.surfaceCreated()}.
         */
        virtual CARAPI_(void) OnSurfaceCreated(
            /* [in] */ ISurfaceHolder* holder);

        /**
         * Convenience for {@link SurfaceHolder.Callback#surfaceDestroyed
         * SurfaceHolder.Callback.surfaceDestroyed()}.
         */
        virtual CARAPI_(void) OnSurfaceDestroyed(
            /* [in] */ ISurfaceHolder* holder);

    protected:
        virtual CARAPI_(void) Dump(
                /* [in] */ const String& prefix,
                /* [in] */ IFileDescriptor* fd,
                /* [in] */ IPrintWriter* out,
                /* [in] */ const ArrayOf<String>& args);

        virtual CARAPI_(void) UpdateSurface(
            /* [in] */ Boolean forceRelayout,
            /* [in] */ Boolean forceReport,
            /* [in] */ Boolean redrawNeeded);

        virtual CARAPI_(void) Attach(
            /* [in] */ IWallpaperEngineWrapper* wrapper);

        virtual CARAPI_(void) DoDesiredSizeChanged(
            /* [in] */ Int32 desiredWidth,
            /* [in] */ Int32 desiredHeight);

        virtual CARAPI_(void) DoVisibilityChanged(
            /* [in] */ Boolean visible);

        virtual CARAPI_(void) ReportVisibility();

        virtual CARAPI_(void) DoOffsetsChanged(
            /* [in] */ Boolean always);

        virtual CARAPI_(void) DoCommand(
            /* [in] */ WallpaperCommand* cmd);

        virtual CARAPI_(void) ReportSurfaceDestroyed();

        virtual CARAPI_(void) Detach();

    private:
        CARAPI_(void) DispatchPointer(
            /* [in] */ IMotionEvent* event);

    protected:
        AutoPtr<IWallpaperEngineWrapper> mIWallpaperEngine;
        // Copies from mIWallpaperEngine.
        AutoPtr<IHandlerCaller> mCaller;
        AutoPtr<IWallpaperConnection> mConnection;
        AutoPtr<IBinder> mWindowToken;

        Boolean mInitializing;
        Boolean mVisible;
        Boolean mScreenOn;
        Boolean mReportedVisible;
        Boolean mDestroyed;

        // Current window state.
        Boolean mCreated;
        Boolean mSurfaceCreated;
        Boolean mIsCreating;
        Boolean mDrawingAllowed;
        Boolean mOffsetsChanged;
        Boolean mFixedSizeAllowed;
        Int32 mWidth;
        Int32 mHeight;
        Int32 mFormat;
        Int32 mType;
        Int32 mCurWidth;
        Int32 mCurHeight;
        Int32 mWindowFlags;
        Int32 mWindowPrivateFlags;
        Int32 mCurWindowFlags;
        Int32 mCurWindowPrivateFlags;
        AutoPtr<IRect> mVisibleInsets;
        AutoPtr<IRect> mWinFrame;
        AutoPtr<IRect> mContentInsets;
        AutoPtr<IConfiguration> mConfiguration;

        AutoPtr<IWindowManagerLayoutParams> mLayout;
        AutoPtr<IWindowSession> mSession;
        AutoPtr<IInputChannel> mInputChannel;

        Mutex mLock;

        Boolean mOffsetMessageEnqueued;
        Float mPendingXOffset;
        Float mPendingYOffset;
        Float mPendingXOffsetStep;
        Float mPendingYOffsetStep;
        Boolean mPendingSync;
        AutoPtr<IMotionEvent> mPendingMove;

        AutoPtr<BroadcastReceiver> mReceiver;
        AutoPtr<BaseSurfaceHolder> mSurfaceHolder;
        AutoPtr<WallpaperInputEventReceiver> mInputEventReceiver;
        AutoPtr<IBaseIWindow> mWindow;

    private:
        WallpaperService* mOwner;
    };

protected:
    class WallpaperCommand
        : public ElRefBase
        , public IInterface
    {
    public:
        CAR_INTERFACE_DECL()

        String mAction;
        Int32 mX;
        Int32 mY;
        Int32 mZ;
        AutoPtr<IBundle> mExtras;
        Boolean mSync;
    };

public:
    CARAPI OnCreate();

    CARAPI OnDestroy();

    /**
    * Implement to return the implementation of the internal accessibility
    * service interface.  Subclasses should not override.
    */
    //@Override
    CARAPI OnBind(
        /* [in] */ IIntent* intent,
        /* [out] */ IBinder** binder);

    /**
     * This allows subclasses to change the thread that most callbacks
     * occur on.  Currently hidden because it is mostly needed for the
     * image wallpaper (which runs in the system process and doesn't want
     * to get stuck running on that seriously in use main thread).  Not
     * exposed right now because the semantics of this are not totally
     * well defined and some callbacks can still happen on the main thread).
     * @hide
     */
    CARAPI SetCallbackLooper(
        /* [in] */ ILooper* looper);

    /**
     * Must be implemented to return a new instance of the wallpaper's engine.
     * Note that multiple instances may be active at the same time, such as
     * when the wallpaper is currently set as the active wallpaper and the user
     * is in the wallpaper picker viewing a preview of it as well.
     */
    virtual CARAPI_(AutoPtr<Engine>) OnCreateEngine() = 0;

protected:
    //@Override
    CARAPI Dump(
        /* [in] */ IFileDescriptor* fd,
        /* [in] */ IPrintWriter* out,
        /* [in] */ const ArrayOf<String>& args);

private:
    AutoPtr<ILooper> mCallbackLooper;
    List<AutoPtr<Engine> > mActiveEngines;
};

} // namespace Wallpaper
} // namespace Service
} // namepsace Droid
} // namespace Elastos

#endif // __WallpaperService_H__
