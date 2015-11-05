
#ifndef __ELASTOS_DROID_APP_ACTIVITY_VIEW_H__
#define __ELASTOS_DROID_APP_ACTIVITY_VIEW_H__

#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/view/ViewGroup.h"

using Elastos::Droid::View::ViewGroup;

// using Elastos::Droid::content.Context;
// using Elastos::Droid::content.ContextWrapper;
// using Elastos::Droid::content.IIntentSender;
// using Elastos::Droid::content.Intent;
// using Elastos::Droid::content.IntentSender;
// using Elastos::Droid::graphics.SurfaceTexture;
// using Elastos::Droid::os.IBinder;
// using Elastos::Droid::os.RemoteException;
// using Elastos::Droid::util.AttributeSet;
// using Elastos::Droid::util.DisplayMetrics;
// using Elastos::Droid::util.Log;
// using Elastos::Droid::view.InputDevice;
// using Elastos::Droid::view.InputEvent;
// using Elastos::Droid::view.MotionEvent;
// using Elastos::Droid::view.Surface;
// using Elastos::Droid::view.TextureView;
// using Elastos::Droid::view.TextureView.SurfaceTextureListener;
// using Elastos::Droid::view.View;
// using Elastos::Droid::view.ViewGroup;
// using Elastos::Droid::view.WindowManager;
// import dalvik.system.CloseGuard;

// import java.lang.ref.WeakReference;

namespace Elastos {
namespace Droid {
namespace App {

/** @hide */
class ActivityView
    : public ViewGroup
    , public IActivityView
{
private:

    class ActivityViewSurfaceTextureListener
        : public Object
        , public ISurfaceTextureListener
    {
    public:
        CAR_INTERFACE_DECL()

        CARAPI OnSurfaceTextureAvailable(
            /* [in] */ ISurfaceTexture* surfaceTexture,
            /* [in] */ Int32 width,
            /* [in] */ Int32 height);

        CARAPI OnSurfaceTextureSizeChanged(
            /* [in] */ ISurfaceTexture* surfaceTexture,
            /* [in] */ Int32 width,
            /* [in] */ Int32 height);

        CARAPI OnSurfaceTextureDestroyed(
            /* [in] */ ISurfaceTexture* surfaceTexture,
            /* [out] */ Boolean* bval);

        CARAPI OnSurfaceTextureUpdated(
            /* [in] */ ISurfaceTexture* surfaceTexture);
    };

    class ActivityContainerCallback
        : public Object
        , public IActivityContainerCallback
        , public IBinder
    {
    public:
        CAR_INTERFACE_DECL()

        ActivityContainerCallback(
            /* [in] */ IActivityView* activityView);

        CARAPI SetVisible(
            /* [in] */ IBinder* container,
            /* [in] */ Boolean visible);

        CARAPI OnAllActivitiesComplete(
            /* [in] */ IBinder* container);

    private:
        AutoPtr<IWeakReference> mActivityViewWeakReference;
    };

    static class ActivityContainerWrapper
        : public Object
    {
    public:
        ActivityContainerWrapper(
            /* [in] */ IActivityContainer* container);

        virtual ~ActivityContainerWrapper();

        CARAPI AttachToDisplay(
            /* [in] */ Int32 displayId);

        CARAPI SetSurface(
            /* [in] */ Surface surface,
            /* [in] */ Int32 width,
            /* [in] */ Int32 height,
            /* [in] */ Int32 density);

        CARAPI StartActivity(
            /* [in] */ IIntent* intent,
            /* [out] */ Int32* status);

        CARAPI StartActivityIntentSender(
            /* [in] */ IIIntentSender* intentSender,
            /* [out] */ Int32* status);

        CARAPI CheckEmbeddedAllowed(
            /* [in] */ IIntent* intent);

        CARAPI CheckEmbeddedAllowedIntentSender(
            /* [in] */ IIIntentSender* intentSender);

        CARAPI GetDisplayId(
            /* [out] */ Int32* status);

        CARAPI InjectEvent(
            /* [in] */ IInputEvent* event,
            /* [out] */ Boolean* bval);

        CARAPI ReleaseSources();

        void Finalize();

    private:
        AutoPtr<IActivityContainer> mIActivityContainer;
        AutoPtr<ICloseGuard> mGuard;// = CloseGuard.get();
        Boolean mOpened; // Protected by mGuard.
    };

public:
    CAR_INTERFACE_DECL()

    ActivityView();

    virtual ~ActivityView();

    CARAPI constructor(
        /* [in] */ Context context);

    CARAPI constructor(
        /* [in] */ Context context,
        /* [in] */ AttributeSet attrs);

    CARAPI constructor(
        /* [in] */ Context context,
        /* [in] */ AttributeSet attrs,
        /* [in] */ Int32 defStyle);

    CARAPI OnVisibilityChanged(
        /* [in] */ View changedView,
        /* [in] */ Int32 visibility);

    CARAPI OnTouchEvent(
        /* [in] */ IMotionEvent* event,
        /* [out] */ Boolean* bval;

    CARAPI OnGenericMotionEvent(
        /* [in] */ IMotionEvent* event,
        /* [out] */ Boolean* bval);

    CARAPI OnAttachedToWindow();

    CARAPI OnDetachedFromWindow();

    CARAPI IsAttachedToDisplay(
        /* [out] */ Boolean* bval);

    CARAPI StartActivity(
        /* [in] */ IIntent* intent);

    CARAPI StartActivity(
        /* [in] */ IIntentSender* intentSender);

    CARAPI StartActivity(
        /* [in] */ IPendingIntent* pendingIntent);

    CARAPI ReleaseResources();

    /**
     * Set the callback to use to report certain state changes.
     * @param callback The callback to report events to.
     *
     * @see ActivityViewCallback
     */
    CARAPI SetCallback(
        /* [in] */ IActivityViewCallback* callback);

protected:
    CARAPI OnLayout(
        /* [in] */ Boolean changed,
        /* [in] */ Int32 l,
        /* [in] */ Int32 t,
        /* [in] */ Int32 r,
        /* [in] */ Int32 b);

private:
    Boolean InjectInputEvent(
        /* [in] */ IInputEvent* event);

    CARAPI AttachToSurfaceWhenReady();

private:

    static const String TAG;// = "ActivityView";
    static const Boolean DEBUG;// = false;

    AutoPtr<IDisplayMetrics> mMetrics;
    AutoPtr<ITextureView> mTextureView;
    AutoPtr<IActivityContainerWrapper> mActivityContainer;
    AutoPtr<IActivity> mActivity; // TODO weak ref ??
    Int32 mWidth;
    Int32 mHeight;
    AutoPtr<ISurface> mSurface;
    Int32 mLastVisibility;
    AutoPtr<IActivityViewCallback> mActivityViewCallback;

    // Only one IIntentSender or Intent may be queued at a time. Most recent one wins.
    AutoPtr<IIIntentSender> mQueuedPendingIntent;
    AutoPtr<IIntent> mQueuedIntent;
};


} // namespace App
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_APP_ACTIVITY_VIEW_H__
