
#include "elastos/droid/app/ActivityView.h"

// using Elastos::Droid::Content::IContext;
// using Elastos::Droid::Content::IContextWrapper;
// using Elastos::Droid::Content::IIIntentSender;
// using Elastos::Droid::Content::IIntent;
// using Elastos::Droid::Content::IIntentSender;
// using Elastos::Droid::Graphics::ISurfaceTexture;
// using Elastos::Droid::Os::IBinder;
// using Elastos::Droid::Os::IRemoteException;
// using Elastos::Droid::Utility::IAttributeSet;
// using Elastos::Droid::Utility::IDisplayMetrics;
// using Elastos::Droid::View::IInputDevice;
// using Elastos::Droid::View::IInputEvent;
// using Elastos::Droid::View::IMotionEvent;
// using Elastos::Droid::View::ISurface;
// using Elastos::Droid::View::ITextureView;
// using Elastos::Droid::View::ITextureView.SurfaceTextureListener;
// using Elastos::Droid::View::IView;
// using Elastos::Droid::View::IViewGroup;
// using Elastos::Droid::View::IWindowManager;

namespace Elastos {
namespace Droid {
namespace App {

//=========================================================================
// ActivityView::ActivityViewSurfaceTextureListener
//=========================================================================
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
        if (mActivityContainer == NULL) {
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
        if (mActivityContainer == NULL)
        {
            return;
        }
        if (DEBUG) Log.d(TAG, "onSurfaceTextureSizeChanged: w=" + width + " h=" + height);
    }

    CARAPI OnSurfaceTextureDestroyed(
        /* [in] */ ISurfaceTexture* surfaceTexture,
        /* [out] */ Boolean* bval)
    {
        if (mActivityContainer == NULL) {
            return TRUE;
        }
        if (DEBUG) Log.d(TAG, "onSurfaceTextureDestroyed");
        mSurface->Release();
        mSurface = NULL;
        try {
            mActivityContainer->SetSurface(NULL, mWidth, mHeight, mMetrics.densityDpi);
        } catch (RemoteException e) {
            throw new RuntimeException(
                    "ActivityView: Unable to set surface of ActivityContainer. " + e);
        }
        return TRUE;
    }

    CARAPI OnSurfaceTextureUpdated(
        /* [in] */ ISurfaceTexture* surfaceTexture)
    {
//            Log.d(TAG, "onSurfaceTextureUpdated");
    }

};

//=========================================================================
// ActivityView::ActivityContainerCallback
//=========================================================================
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
        if (DEBUG) Logger::V(TAG, "setVisible(): container=" + container + " visible=" + visible +
                " ActivityView=" + mActivityViewWeakReference->Get());
    }

    CARAPI OnAllActivitiesComplete(
        /* [in] */ IBinder* container)
    {
        final ActivityView activityView = mActivityViewWeakReference->Get();
        if (activityView != NULL) {
            final ActivityViewCallback callback = activityView.mActivityViewCallback;
            if (callback != NULL) {
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


//=========================================================================
// ActivityView::ActivityContainerWrapper
//=========================================================================
static class ActivityContainerWrapper
    : public Object
{
public:
    ActivityContainerWrapper(
        /* [in] */ IActivityContainer* container)
    {
        mIActivityContainer = container;
        mOpened = TRUE;
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
        mIActivityContainer->SetSurface(surface, width, height, density);
    }

    CARAPI StartActivity(
        /* [in] */ IIntent* intent,
        /* [out] */ Int32* status)
    {
        try {
            return mIActivityContainer->StartActivity(intent);
        } catch (RemoteException e) {
            throw new RuntimeException("ActivityView: Unable to startActivity. " + e);
        }
    }

    CARAPI StartActivityIntentSender(
        /* [in] */ IIIntentSender* intentSender,
        /* [out] */ Int32* status)
    {
        try {
            return mIActivityContainer->StartActivityIntentSender(intentSender);
        } catch (RemoteException e) {
            throw new RuntimeException(
                    "ActivityView: Unable to startActivity from IntentSender. " + e);
        }
    }

    CARAPI CheckEmbeddedAllowed(
        /* [in] */ IIntent* intent)
    {
        try {
            mIActivityContainer->CheckEmbeddedAllowed(intent);
        } catch (RemoteException e) {
            throw new RuntimeException(
                    "ActivityView: Unable to startActivity from Intent. " + e);
        }
    }

    CARAPI CheckEmbeddedAllowedIntentSender(
        /* [in] */ IIIntentSender* intentSender)
    {
        try {
            mIActivityContainer->CheckEmbeddedAllowedIntentSender(intentSender);
        } catch (RemoteException e) {
            throw new RuntimeException(
                    "ActivityView: Unable to startActivity from IntentSender. " + e);
        }
    }

    CARAPI GetDisplayId(
        /* [out] */ Int32* status)
    {
        try {
            return mIActivityContainer->GetDisplayId();
        } catch (RemoteException e) {
            return -1;
        }
    }

    CARAPI InjectEvent(
        /* [in] */ IInputEvent* event,
        /* [out] */ Boolean* bval)
    {
        try {
            return mIActivityContainer->InjectEvent(event);
        } catch (RemoteException e) {
            return FALSE;
        }
    }

    CARAPI ReleaseSources()
    {
        synchronized (mGuard) {
            if (mOpened) {
                if (DEBUG) Logger::V(TAG, "ActivityContainerWrapper: release called");
                try {
                    mIActivityContainer->Release();
                    mGuard->Close();
                } catch (RemoteException e) {
                }
                mOpened = FALSE;
            }
        }
    }

    void Finalize()
    {
        if (DEBUG) Logger::V(TAG, "ActivityContainerWrapper: finalize called");
        try {
            if (mGuard != NULL) {
                mGuard.warnIfOpen();
                release();
            }
        } finally {
            super.finalize();
        }
    }

private:
    AutoPtr<IActivityContainer> mIActivityContainer;
    AutoPtr<ICloseGuard> mGuard;// = CloseGuard->Get();
    Boolean mOpened; // Protected by mGuard.
};

//=========================================================================
// ActivityView
//=========================================================================

static const String ActivityView::TAG("ActivityView");
static const Boolean ActivityView::DEBUG = FALSE;

CAR_INTERFACE_IMPL(ActivityView, ViewGroup, IActivityView)

ActivityView::ActivityView()
    : mWidth(0)
    , mHeight(0)
    , mLastVisibility(0)
{}

ActivityView::~ActivityView()
{}

ECode ActivityView::constructor(
    /* [in] */ IContext* context)
{
    return constructor(context, NULL);
}

ECode ActivityView::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return constructor(context, attrs, 0);
}

ECode ActivityView::constructor(
    /* [in] */ IContext* ctx,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    FAIL_RETURN(ViewGroup::constructor(ctx, attrs, defStyle))

    AutoPtr<IContext> context = ctx;
    while (IContextWrapper::Probe(context) != NULL) {
        if (IActivity::Probe(context) != NULL) {
            mActivity = IActivity::Probe(context);
            break;
        }
        AutoPtr<IContextWrapper> cw = IContextWrapper::Probe(context);
        context = NULL;
        cw->GetBaseContext((IContext**)&context);
    }
    if (mActivity == NULL) {
        Logger::E(TAG, "The ActivityView's Context is not an Activity.");
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    // try {
    AutoPtr<IBinder> token;
    mActivity->GetActivityToken((IBinder**)&token);
    AutoPtr<IActivityContainerCallback> cb = new ActivityContainerCallback(this);
    AutoPtr<IActivityContainer> ac;
    ECode ec = ActivityManagerNative::GetDefault()->CreateActivityContainer(
        token, cb, (IActivityContainer**)&ac);

    // } catch (RemoteException e)
    if (ec == (ECode)E_REMOTE_EXCEPTION) {
        Logger::E(TAG, "ActivityView: Unable to create ActivityContainer. ");
        return E_RUNTIME_EXCEPTION;
    }

    mActivityContainer = new ActivityContainerWrapper(ac);

    CTextureView::New(context, (ITextureView**)&mTextureView);
    AutoPtr<ISurfaceTextureListener> listener = new ActivityViewSurfaceTextureListener();
    mTextureView->SetSurfaceTextureListener(listener);
    AddView(mTextureView);


    AutoPtr<IInterface> obj;
    mActivity->GetSystemService(IContext::WINDOW_SERVICE, (IInterface**)&obj);
    AutoPtr<IWindowManager> wm = IWindowManager::Probe(obj);
    CDisplayMetrics::New((IDisplayMetrics**)&mMetrics);
    AutoPtr<IDisplay> display;
    wm->GetDefaultDisplay((IDisplay**)&display);
    display->GetMetrics(mMetrics);

    GetVisibility(&mLastVisibility);

    if (DEBUG) Logger::V(TAG, "ctor()");
}

ECode ActivityView::OnLayout(
    /* [in] */ Boolean changed,
    /* [in] */ Int32 l,
    /* [in] */ Int32 t,
    /* [in] */ Int32 r,
    /* [in] */ Int32 b)
{
    return mTextureView->Layout(0, 0, r - l, b - t);
}

ECode ActivityView::OnVisibilityChanged(
    /* [in] */ View changedView,
    /* [in] */ Int32 visibility)
{
    FAIL_RETURN(ViewGroup::OnVisibilityChanged(changedView, visibility))

    if (mSurface != NULL) {
        // try {
            if (visibility == IView::GONE) {
                mActivityContainer->SetSurface(NULL, mWidth, mHeight, mMetrics.densityDpi);
            } else if (mLastVisibility == IView::GONE) {
                // Don't change surface when going between IView::VISIBLE and IView::INVISIBLE.
                mActivityContainer->SetSurface(mSurface, mWidth, mHeight, mMetrics.densityDpi);
            }
        // } catch (RemoteException e) {
            Logger::E(TAG, "ActivityView: Unable to set surface of ActivityContainer. ");
            return E_RUNTIME_EXCEPTION;
        // }
    }
    mLastVisibility = visibility;
}

Boolean ActivityView::InjectInputEvent(
    /* [in] */ IInputEvent* event)
{
    return mActivityContainer != NULL && mActivityContainer->InjectEvent(event);
}

ECode ActivityView::OnTouchEvent(
    /* [in] */ IMotionEvent* event,
    /* [out] */ Boolean* bval)
{
    VALIDATE_NOT_NULL(bval)
    *bval = InjectInputEvent(event);
    if (*bval == FALSE) {
        return ViewGroup::OnTouchEvent(event, bval);
    }
    return NOERROR;
}

ECode ActivityView::OnGenericMotionEvent(
    /* [in] */ IMotionEvent* event,
    /* [out] */ Boolean* bval)
{
    VALIDATE_NOT_NULL(bval)
    *bval = FALSE;

    Boolean fromSource;
    event->IsFromSource(IInputDevice::SOURCE_CLASS_POINTER, &fromSource);
    if (fromSource) {
        if (InjectInputEvent(event)) {
            *bval = TRUE;
            return NOERROR;
        }
    }
    return ViewGroup::OnGenericMotionEvent(event, bval);
}

ECode ActivityView::OnAttachedToWindow()
{
    if (DEBUG) Logger::V(TAG, "onAttachedToWindow(): mActivityContainer=  mSurface=");
    return NOERROR;
}

ECode ActivityView::OnDetachedFromWindow()
{
    if (DEBUG) Logger::V(TAG, "onDetachedFromWindow(): mActivityContainer= mSurface=");
    return NOERROR;
}

ECode ActivityView::IsAttachedToDisplay(
    /* [out] */ Boolean* bval)
{
    VALIDATE_NOT_NULL(bval)
    *bval = mSurface != NULL;
    return NOERROR;
}

ECode ActivityView::StartActivity(
    /* [in] */ IIntent* intent)
{
    if (mActivityContainer == NULL) {
        Logger::E(TAG, "Attempt to call startActivity after release");
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    Boolean bval;
    IsAttachedToDisplay(&bval);
    if (DEBUG) Logger::V(TAG, "startActivity(): intent=%s %s attached",
            Object::ToString(intent).string(), (bval ? "" : "not"));
    if (mSurface != NULL) {
        mActivityContainer->StartActivity(intent);
    }
    else {
        mActivityContainer->CheckEmbeddedAllowed(intent);
        mQueuedIntent = intent;
        mQueuedPendingIntent = NULL;
    }
    return NOERROR;
}

ECode ActivityView::StartActivity(
    /* [in] */ IIntentSender* intentSender)
{
    if (mActivityContainer == NULL) {
        Logger::E(TAG, "Attempt to call startActivity after release");
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    Boolean bval;
    IsAttachedToDisplay(&bval);
    if (DEBUG) Logger::V(TAG, "startActivityIntentSender(): intentSender=%s %s attached",
        Object::ToString(intentSender).string(), (bval ? "" : "not"));
    AutoPtr<IIIntentSender> iIntentSender;
    intentSender->GetTarget((IIIntentSender**)&iIntentSender);
    if (mSurface != NULL) {
        mActivityContainer->StartActivityIntentSender(iIntentSender);
    }
    else {
        mActivityContainer->CheckEmbeddedAllowedIntentSender(iIntentSender);
        mQueuedPendingIntent = iIntentSender;
        mQueuedIntent = NULL;
    }
    return NOERROR;
}

ECode ActivityView::StartActivity(
    /* [in] */ IPendingIntent* pendingIntent)
{
    if (mActivityContainer == NULL) {
        throw new IllegalStateException("Attempt to call startActivity after release");
    }
    if (DEBUG) Logger::V(TAG, "startActivityPendingIntent(): PendingIntent=" + pendingIntent + " "
            + (IsAttachedToDisplay() ? "" : "not") + " attached");
    final IIntentSender iIntentSender = pendingIntent->GetTarget();
    if (mSurface != NULL) {
        mActivityContainer->StartActivityIntentSender(iIntentSender);
    } else {
        mActivityContainer->CheckEmbeddedAllowedIntentSender(iIntentSender);
        mQueuedPendingIntent = iIntentSender;
        mQueuedIntent = NULL;
    }
    return NOERROR;
}

ECode ActivityView::ReleaseResources()
{
    if (DEBUG) Logger::V(TAG, "release() mActivityContainer= mSurface=");
    if (mActivityContainer == NULL) {
        Logger::E(TAG, "Duplicate call to release");
        return NOERROR;
    }
    mActivityContainer->ReleaseSources();
    mActivityContainer = NULL;

    if (mSurface != NULL) {
        mSurface->ReleaseSources();
        mSurface = NULL;
    }

    return mTextureView->SetSurfaceTextureListener(NULL);
}

ECode ActivityView::AttachToSurfaceWhenReady()
{
    AutoPtr<ISurfaceTexture> surfaceTexture;
    mTextureView->GetSurfaceTexture((ISurfaceTexture**)&surfaceTexture);
    if (surfaceTexture == NULL || mSurface != NULL) {
        // Either not ready to attach, or already attached.
        return NOERROR;
    }

    mSurface = NULL;
    CSurface::New(surfaceTexture, (ISurface**)&mSurface);
    // try {
        mActivityContainer->SetSurface(mSurface, mWidth, mHeight, mMetrics.densityDpi);
    // } catch (RemoteException e) {
    if (ec == (ECode)E_REMOTE_EXCEPTION) {
        mSurface->ReleaseSources();
        mSurface = NULL;
        Logger::E(TAG, "ActivityView: Unable to create ActivityContainer. ");
        return E_RUNTIME_EXCEPTION;
    }

    if (DEBUG) Logger::V(TAG, "attachToSurfaceWhenReady: %s  queued intent",
        (mQueuedIntent != NULL || mQueuedPendingIntent != NULL ? "" : "no"));
    if (mQueuedIntent != NULL) {
        mActivityContainer->StartActivity(mQueuedIntent);
        mQueuedIntent = NULL;
    }
    else if (mQueuedPendingIntent != NULL) {
        mActivityContainer->StartActivityIntentSender(mQueuedPendingIntent);
        mQueuedPendingIntent = NULL;
    }
    return NOERROR;
}

ECode ActivityView::SetCallback(
    /* [in] */ IActivityViewCallback* callback)
{
    mActivityViewCallback = callback;
    return NOERROR;
}

} // namespace App
} // namespace Droid
} // namespace Elastos
