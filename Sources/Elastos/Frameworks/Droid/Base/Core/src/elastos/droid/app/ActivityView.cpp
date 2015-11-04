
#include "elastos/droid/app/ActivityView.h"

// import android.content.Context;
// import android.content.ContextWrapper;
// import android.content.IIntentSender;
// import android.content.Intent;
// import android.content.IntentSender;
// import android.graphics.SurfaceTexture;
// import android.os.IBinder;
// import android.os.RemoteException;
// import android.util.AttributeSet;
// import android.util.DisplayMetrics;
// import android.util.Log;
// import android.view.InputDevice;
// import android.view.InputEvent;
// import android.view.MotionEvent;
// import android.view.Surface;
// import android.view.TextureView;
// import android.view.TextureView.SurfaceTextureListener;
// import android.view.View;
// import android.view.ViewGroup;
// import android.view.WindowManager;
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
            /* [in] */ Int32 height)
        {
            if (mActivityContainer == null) {
                return;
            }
            if (DEBUG) Log.d(TAG, "onSurfaceTextureAvailable: width=" + width + " height="
                    + height);
            mWidth = width;
            mHeight = height;
            attachToSurfaceWhenReady();
        }

        CARAPI OnSurfaceTextureSizeChanged(
            /* [in] */ ISurfaceTexture* surfaceTexture,
            /* [in] */ Int32 width,
            /* [in] */ Int32 height)
        {
            if (mActivityContainer == null)
            {
                return;
            }
            if (DEBUG) Log.d(TAG, "onSurfaceTextureSizeChanged: w=" + width + " h=" + height);
        }

        CARAPI OnSurfaceTextureDestroyed(
            /* [in] */ ISurfaceTexture* surfaceTexture,
            /* [out] */ Boolean* bval)
        {
            if (mActivityContainer == null) {
                return true;
            }
            if (DEBUG) Log.d(TAG, "onSurfaceTextureDestroyed");
            mSurface.release();
            mSurface = null;
            try {
                mActivityContainer.setSurface(null, mWidth, mHeight, mMetrics.densityDpi);
            } catch (RemoteException e) {
                throw new RuntimeException(
                        "ActivityView: Unable to set surface of ActivityContainer. " + e);
            }
            return true;
        }

        CARAPI OnSurfaceTextureUpdated(
            /* [in] */ ISurfaceTexture* surfaceTexture)
        {
//            Log.d(TAG, "onSurfaceTextureUpdated");
        }

    };

    class ActivityContainerCallback
        : public Object
        , public IActivityContainerCallback
        , public IBinder
    {
    public:
        CAR_INTERFACE_DECL()

        ActivityContainerCallback(
            /* [in] */ IActivityView* activityView)
        {
            mActivityViewWeakReference = new WeakReference<ActivityView>(activityView);
        }

        CARAPI SetVisible(
            /* [in] */ IBinder* container,
            /* [in] */ Boolean visible)
        {
            if (DEBUG) Log.v(TAG, "setVisible(): container=" + container + " visible=" + visible +
                    " ActivityView=" + mActivityViewWeakReference.get());
        }

        CARAPI OnAllActivitiesComplete(
            /* [in] */ IBinder* container)
        {
            final ActivityView activityView = mActivityViewWeakReference.get();
            if (activityView != null) {
                final ActivityViewCallback callback = activityView.mActivityViewCallback;
                if (callback != null) {
                    activityView.post(new Runnable() {
                        @Override
                        public void run() {
                            callback.onAllActivitiesComplete(activityView);
                        }
                    });
                }
            }
        }

    private:
        AutoPtr<IWeakReference> mActivityViewWeakReference;
    };

    static class ActivityContainerWrapper
        : public Object
    {
    public:
        ActivityContainerWrapper(
            /* [in] */ IActivityContainer* container)
        {
            mIActivityContainer = container;
            mOpened = true;
            mGuard.open("release");
        }

        virtual ~ActivityContainerWrapper()
        {
            Finalize();
        }

        CARAPI AttachToDisplay(
            /* [in] */ Int32 displayId)
        {
            try {
                mIActivityContainer.attachToDisplay(displayId);
            } catch (RemoteException e) {
            }
        }

        CARAPI SetSurface(
            /* [in] */ Surface surface,
            /* [in] */ Int32 width,
            /* [in] */ Int32 height,
            /* [in] */ Int32 density)
        {
            mIActivityContainer.setSurface(surface, width, height, density);
        }

        CARAPI StartActivity(
            /* [in] */ IIntent* intent,
            /* [out] */ Int32* status)
        {
            try {
                return mIActivityContainer.startActivity(intent);
            } catch (RemoteException e) {
                throw new RuntimeException("ActivityView: Unable to startActivity. " + e);
            }
        }

        CARAPI StartActivityIntentSender(
            /* [in] */ IIIntentSender* intentSender,
            /* [out] */ Int32* status)
        {
            try {
                return mIActivityContainer.startActivityIntentSender(intentSender);
            } catch (RemoteException e) {
                throw new RuntimeException(
                        "ActivityView: Unable to startActivity from IntentSender. " + e);
            }
        }

        CARAPI CheckEmbeddedAllowed(
            /* [in] */ IIntent* intent)
        {
            try {
                mIActivityContainer.checkEmbeddedAllowed(intent);
            } catch (RemoteException e) {
                throw new RuntimeException(
                        "ActivityView: Unable to startActivity from Intent. " + e);
            }
        }

        CARAPI CheckEmbeddedAllowedIntentSender(
            /* [in] */ IIIntentSender* intentSender)
        {
            try {
                mIActivityContainer.checkEmbeddedAllowedIntentSender(intentSender);
            } catch (RemoteException e) {
                throw new RuntimeException(
                        "ActivityView: Unable to startActivity from IntentSender. " + e);
            }
        }

        CARAPI GetDisplayId(
            /* [out] */ Int32* status)
        {
            try {
                return mIActivityContainer.getDisplayId();
            } catch (RemoteException e) {
                return -1;
            }
        }

        CARAPI InjectEvent(
            /* [in] */ IInputEvent* event,
            /* [out] */ Boolean* bval)
        {
            try {
                return mIActivityContainer.injectEvent(event);
            } catch (RemoteException e) {
                return false;
            }
        }

        CARAPI ReleaseSources()
        {
            synchronized (mGuard) {
                if (mOpened) {
                    if (DEBUG) Log.v(TAG, "ActivityContainerWrapper: release called");
                    try {
                        mIActivityContainer.release();
                        mGuard.close();
                    } catch (RemoteException e) {
                    }
                    mOpened = false;
                }
            }
        }

        void Finalize()
        {
            if (DEBUG) Log.v(TAG, "ActivityContainerWrapper: finalize called");
            try {
                if (mGuard != null) {
                    mGuard.warnIfOpen();
                    release();
                }
            } finally {
                super.finalize();
            }
        }

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
        /* [in] */ Context context)
    {
        this(context, null);
    }

    CARAPI constructor(
        /* [in] */ Context context,
        /* [in] */ AttributeSet attrs)
    {
        this(context, attrs, 0);
    }

    CARAPI constructor(
        /* [in] */ Context context,
        /* [in] */ AttributeSet attrs,
        /* [in] */ Int32 defStyle)
    {
        super(context, attrs, defStyle);

        while (context instanceof ContextWrapper) {
            if (context instanceof Activity) {
                mActivity = (Activity)context;
                break;
            }
            context = ((ContextWrapper)context).getBaseContext();
        }
        if (mActivity == null) {
            throw new IllegalStateException("The ActivityView's Context is not an Activity.");
        }

        try {
            mActivityContainer = new ActivityContainerWrapper(
                    ActivityManagerNative.getDefault().createActivityContainer(
                            mActivity.getActivityToken(), new ActivityContainerCallback(this)));
        } catch (RemoteException e) {
            throw new RuntimeException("ActivityView: Unable to create ActivityContainer. "
                    + e);
        }

        mTextureView = new TextureView(context);
        mTextureView.setSurfaceTextureListener(new ActivityViewSurfaceTextureListener());
        addView(mTextureView);

        WindowManager wm = (WindowManager)mActivity.getSystemService(Context.WINDOW_SERVICE);
        mMetrics = new DisplayMetrics();
        wm.getDefaultDisplay().getMetrics(mMetrics);

        mLastVisibility = getVisibility();

        if (DEBUG) Log.v(TAG, "ctor()");
    }

    protected CARAPI OnLayout(
        /* [in] */ Boolean changed,
        /* [in] */ Int32 l,
        /* [in] */ Int32 t,
        /* [in] */ Int32 r,
        /* [in] */ Int32 b)
    {
        mTextureView.layout(0, 0, r - l, b - t);
    }

    CARAPI OnVisibilityChanged(
        /* [in] */ View changedView,
        /* [in] */ Int32 visibility)
    {
        super.onVisibilityChanged(changedView, visibility);

        if (mSurface != null) {
            try {
                if (visibility == View.GONE) {
                    mActivityContainer.setSurface(null, mWidth, mHeight, mMetrics.densityDpi);
                } else if (mLastVisibility == View.GONE) {
                    // Don't change surface when going between View.VISIBLE and View.INVISIBLE.
                    mActivityContainer.setSurface(mSurface, mWidth, mHeight, mMetrics.densityDpi);
                }
            } catch (RemoteException e) {
                throw new RuntimeException(
                        "ActivityView: Unable to set surface of ActivityContainer. " + e);
            }
        }
        mLastVisibility = visibility;
    }

    private Boolean InjectInputEvent(
        /* [in] */ IInputEvent* event)
    {
        return mActivityContainer != null && mActivityContainer.injectEvent(event);
    }

    public CARAPI OnTouchEvent(
        /* [in] */ IMotionEvent* event,
        /* [out] */ Boolean* bval)
    {
        return injectInputEvent(event) || super.onTouchEvent(event);
    }

    public CARAPI OnGenericMotionEvent(
        /* [in] */ IMotionEvent* event,
        /* [out] */ Boolean* bval)
    {
        if (event.isFromSource(InputDevice.SOURCE_CLASS_POINTER)) {
            if (injectInputEvent(event)) {
                return true;
            }
        }
        return super.onGenericMotionEvent(event);
    }

    public CARAPI OnAttachedToWindow()
    {
        if (DEBUG) Log.v(TAG, "onAttachedToWindow(): mActivityContainer=" + mActivityContainer +
                " mSurface=" + mSurface);
    }

    public CARAPI OnDetachedFromWindow()
    {
        if (DEBUG) Log.v(TAG, "onDetachedFromWindow(): mActivityContainer=" + mActivityContainer +
                " mSurface=" + mSurface);
    }

    public CARAPI IsAttachedToDisplay(
        /* [out] */ Boolean* bval)
    {
        return mSurface != null;
    }

    public CARAPI StartActivity(
        /* [in] */ IIntent* intent)
    {
        if (mActivityContainer == null) {
            throw new IllegalStateException("Attempt to call startActivity after release");
        }
        if (DEBUG) Log.v(TAG, "startActivity(): intent=" + intent + " " +
                (isAttachedToDisplay() ? "" : "not") + " attached");
        if (mSurface != null) {
            mActivityContainer.startActivity(intent);
        } else {
            mActivityContainer.checkEmbeddedAllowed(intent);
            mQueuedIntent = intent;
            mQueuedPendingIntent = null;
        }
    }

    public CARAPI StartActivity(
        /* [in] */ IIntentSender* intentSender)
    {
        if (mActivityContainer == null) {
            throw new IllegalStateException("Attempt to call startActivity after release");
        }
        if (DEBUG) Log.v(TAG, "startActivityIntentSender(): intentSender=" + intentSender + " " +
                (isAttachedToDisplay() ? "" : "not") + " attached");
        final IIntentSender iIntentSender = intentSender.getTarget();
        if (mSurface != null) {
            mActivityContainer.startActivityIntentSender(iIntentSender);
        } else {
            mActivityContainer.checkEmbeddedAllowedIntentSender(iIntentSender);
            mQueuedPendingIntent = iIntentSender;
            mQueuedIntent = null;
        }
    }

    public CARAPI StartActivity(
        /* [in] */ IPendingIntent* pendingIntent)
    {
        if (mActivityContainer == null) {
            throw new IllegalStateException("Attempt to call startActivity after release");
        }
        if (DEBUG) Log.v(TAG, "startActivityPendingIntent(): PendingIntent=" + pendingIntent + " "
                + (isAttachedToDisplay() ? "" : "not") + " attached");
        final IIntentSender iIntentSender = pendingIntent.getTarget();
        if (mSurface != null) {
            mActivityContainer.startActivityIntentSender(iIntentSender);
        } else {
            mActivityContainer.checkEmbeddedAllowedIntentSender(iIntentSender);
            mQueuedPendingIntent = iIntentSender;
            mQueuedIntent = null;
        }
    }

    public CARAPI ReleaseResources()
    {
        if (DEBUG) Log.v(TAG, "release() mActivityContainer=" + mActivityContainer +
                " mSurface=" + mSurface);
        if (mActivityContainer == null) {
            Log.e(TAG, "Duplicate call to release");
            return;
        }
        mActivityContainer.release();
        mActivityContainer = null;

        if (mSurface != null) {
            mSurface.release();
            mSurface = null;
        }

        mTextureView.setSurfaceTextureListener(null);
    }

    private CARAPI AttachToSurfaceWhenReady()
    {
        final SurfaceTexture surfaceTexture = mTextureView.getSurfaceTexture();
        if (surfaceTexture == null || mSurface != null) {
            // Either not ready to attach, or already attached.
            return;
        }

        mSurface = new Surface(surfaceTexture);
        try {
            mActivityContainer.setSurface(mSurface, mWidth, mHeight, mMetrics.densityDpi);
        } catch (RemoteException e) {
            mSurface.release();
            mSurface = null;
            throw new RuntimeException("ActivityView: Unable to create ActivityContainer. " + e);
        }

        if (DEBUG) Log.v(TAG, "attachToSurfaceWhenReady: " + (mQueuedIntent != null ||
                mQueuedPendingIntent != null ? "" : "no") + " queued intent");
        if (mQueuedIntent != null) {
            mActivityContainer.startActivity(mQueuedIntent);
            mQueuedIntent = null;
        } else if (mQueuedPendingIntent != null) {
            mActivityContainer.startActivityIntentSender(mQueuedPendingIntent);
            mQueuedPendingIntent = null;
        }
    }

    /**
     * Set the callback to use to report certain state changes.
     * @param callback The callback to report events to.
     *
     * @see ActivityViewCallback
     */
    public CARAPI SetCallback(
        /* [in] */ IActivityViewCallback* callback)
    {
        mActivityViewCallback = callback;
    }

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
