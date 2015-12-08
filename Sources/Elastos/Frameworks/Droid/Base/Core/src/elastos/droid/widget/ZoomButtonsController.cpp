#include "elastos/droid/widget/ZoomButtonsController.h"
#include "elastos/droid/content/CIntentFilter.h"
#include "elastos/droid/view/View.h"
#include "elastos/droid/view/CViewConfiguration.h"
#include "elastos/droid/view/CMotionEventHelper.h"
#include "elastos/droid/view/CViewGroupLayoutParams.h"
#include "elastos/droid/view/CWindowManagerLayoutParams.h"
#include "elastos/droid/view/ViewRootImpl.h"
#include "elastos/droid/graphics/CRect.h"
#include <elastos/utility/logging/Logger.h>
#include <elastos/core/Math.h>

using Elastos::Utility::Logging::Logger;
using Elastos::Droid::Os::IBinder;
using Elastos::Droid::Content::CIntentFilter;
using Elastos::Droid::Content::Res::IResources;
using Elastos::Droid::Graphics::CRect;
using Elastos::Droid::Graphics::IPixelFormat;
using Elastos::Droid::Graphics::Drawable::EIID_IDrawableCallback;
using Elastos::Droid::View::EIID_View;
using Elastos::Droid::View::ViewRootImpl;
using Elastos::Droid::View::IMotionEvent;
using Elastos::Droid::View::IMotionEventHelper;
using Elastos::Droid::View::CMotionEventHelper;
using Elastos::Droid::View::CViewConfiguration;
using Elastos::Droid::View::ILayoutInflater;
using Elastos::Droid::View::CViewGroupLayoutParams;
using Elastos::Droid::View::IViewGroupLayoutParams;
using Elastos::Droid::View::EIID_View;
using Elastos::Droid::View::EIID_IView;
using Elastos::Droid::View::EIID_ViewGroup;
using Elastos::Droid::View::EIID_IViewGroup;
using Elastos::Droid::View::EIID_IViewParent;
using Elastos::Droid::View::EIID_IViewManager;
using Elastos::Droid::View::EIID_IKeyEventCallback;
using Elastos::Droid::View::EIID_IViewOnClickListener;
using Elastos::Droid::View::EIID_IViewOnTouchListener;
using Elastos::Droid::View::CWindowManagerLayoutParams;
using Elastos::Droid::View::IWindowManagerLayoutParams;
using Elastos::Droid::View::Accessibility::EIID_IAccessibilityEventSource;


namespace Elastos {
namespace Droid {
namespace Widget {

const String ZoomButtonsController::TAG("ZoomButtonsController");
const Int32 ZoomButtonsController::ZOOM_CONTROLS_TIMEOUT = (Int32)CViewConfiguration::GetZoomControlsTimeout();
const Int32 ZoomButtonsController::ZOOM_CONTROLS_TOUCH_PADDING;
const Int32 ZoomButtonsController::MSG_POST_CONFIGURATION_CHANGED;
const Int32 ZoomButtonsController::MSG_DISMISS_ZOOM_CONTROLS;
const Int32 ZoomButtonsController::MSG_POST_SET_VISIBLE;

IVIEW_METHODS_IMPL(ZoomButtonsController::Container, ZoomButtonsController::_Container)
IVIEWGROUP_METHODS_IMPL(ZoomButtonsController::Container, ZoomButtonsController::_Container)
IVIEWPARENT_METHODS_IMPL(ZoomButtonsController::Container, ZoomButtonsController::_Container)
IVIEWMANAGER_METHODS_IMPL(ZoomButtonsController::Container, ZoomButtonsController::_Container)
IDRAWABLECALLBACK_METHODS_IMPL(ZoomButtonsController::Container, ZoomButtonsController::_Container)
IKEYEVENTCALLBACK_METHODS_IMPL(ZoomButtonsController::Container, ZoomButtonsController::_Container)
IACCESSIBILITYEVENTSOURCE_METHODS_IMPL(ZoomButtonsController::Container, ZoomButtonsController::_Container)
IFRAMELAYOUT_METHODS_IMPL(ZoomButtonsController::Container, ZoomButtonsController::_Container)

CAR_INTERFACE_IMPL(ZoomButtonsController::MyClickListener, IViewOnClickListener)

ECode ZoomButtonsController::MyHandler::HandleMessage(
    /* [in] */ IMessage* msg)
{
    Int32 what;
    msg->GetWhat(&what);

    switch(what) {
        case ZoomButtonsController::MSG_POST_CONFIGURATION_CHANGED:
            mHost->OnPostConfigurationChanged();
            break;

        case ZoomButtonsController::MSG_DISMISS_ZOOM_CONTROLS:
            mHost->SetVisible(FALSE);
            break;

        case ZoomButtonsController::MSG_POST_SET_VISIBLE: {
            AutoPtr<IBinder> binder;
            mHost->mOwnerView->GetWindowToken((IBinder**)&binder);
            if (binder == NULL) {
                // Doh, it is still null, just ignore the set visible call
                Logger::E(ZoomButtonsController::TAG,
                    "Cannot make the zoom controller visible if the owner view is " \
                    "not attached to a window.");
            }
            else {
                mHost->SetVisible(TRUE);
            }
            break;
        }
    }

    return NOERROR;
}

ZoomButtonsController::MyClickListener::MyClickListener(
    /* [in] */ ZoomButtonsController* host,
    /* [in] */ Boolean onZoom)
    : mHost(host)
    , mOnZoom(onZoom)
{}

ECode ZoomButtonsController::MyClickListener::OnClick(
    /* [in] */ IView* v)
{
    mHost->DismissControlsDelayed(ZoomButtonsController::ZOOM_CONTROLS_TIMEOUT);
    if(mHost->mCallback)
        mHost->mCallback->OnZoom(mOnZoom);
    return NOERROR;
}

ZoomButtonsController::MyRunnable::MyRunnable(
    /* [in] */ ZoomButtonsController* host)
    : mHost(host)
{}

ECode ZoomButtonsController::MyRunnable::Run()
{
    mHost->RefreshPositioningVariables();

    if(mHost->mCallback != NULL)
    {
        return mHost->mCallback->OnVisibilityChanged(TRUE);
    }
    return NOERROR;
}

PInterface ZoomButtonsController::Container::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)(IFrameLayout*)this;
    } else if (riid == EIID_IFrameLayout) {
        return (IFrameLayout*)this;
    } else if (riid == EIID_IViewGroup) {
        return (IFrameLayout*)this;
    } else if (riid == EIID_IView) {
        return (IFrameLayout*)this;
    } else if (riid == EIID_IViewParent) {
        return (IViewParent*)this;
    } else if (riid == EIID_IViewManager) {
        return (IViewManager*)this;
    } else if (riid == EIID_IDrawableCallback) {
        return (IDrawableCallback*)this;
    } else if (riid == EIID_IKeyEventCallback) {
        return (IKeyEventCallback*)this;
    } else if (riid == EIID_IAccessibilityEventSource) {
        return (IAccessibilityEventSource*)this;
    } else if (riid == EIID_IWeakReferenceSource) {
        return (IWeakReferenceSource*)this;
    } else if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)(_Container*)this);
    } else if (riid == EIID_ViewGroup) {
        return reinterpret_cast<PInterface>((ViewGroup*)(_Container*)this);
    }
    return NULL;
}

UInt32 ZoomButtonsController::Container::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 ZoomButtonsController::Container::Release()
{
    return ElRefBase::Release();
}

ECode ZoomButtonsController::Container::GetInterfaceID(
    /* [in] */ IInterface *object,
    /* [out] */ InterfaceID *pIID)
{
    if (object == (IFrameLayout*)this) {
        *pIID = EIID_IFrameLayout;
    }
    else if (object == (IViewParent*)this) {
        *pIID = EIID_IViewParent;
    }
    else if (object == (IViewManager*)this) {
        *pIID = EIID_IViewManager;
    }
    else if (object == (IDrawableCallback*)this) {
        *pIID = EIID_IDrawableCallback;
    }
    else if (object == (IKeyEventCallback*)this) {
        *pIID = EIID_IKeyEventCallback;
    }
    else if (object == (IAccessibilityEventSource*)this) {
        *pIID = EIID_IAccessibilityEventSource;
    }
    else if (object == reinterpret_cast<PInterface>((View*)this)) {
        *pIID = EIID_View;
    }
    else if (object == reinterpret_cast<PInterface>((ViewGroup*)this)) {
        *pIID = EIID_ViewGroup;
    }
    else {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return NOERROR;
}

ECode ZoomButtonsController::Container::GetWeakReference(
    /* [out] */ IWeakReference** weakReference)
{
    VALIDATE_NOT_NULL(weakReference)
    *weakReference = new WeakReferenceImpl(Probe(EIID_IInterface), CreateWeak(this));
    REFCOUNT_ADD(*weakReference)
    return NOERROR;
}

ZoomButtonsController::MyBroadcastReceiver::MyBroadcastReceiver(
    /* [in] */ ZoomButtonsController* host)
    : mHost(host)
{}

ECode ZoomButtonsController::MyBroadcastReceiver::OnReceive(
    /* [in] */ IContext* context,
    /* [in] */ IIntent* intent)
{
    if (!mHost->mIsVisible) return NOERROR;

    mHost->mHandler->RemoveMessages(ZoomButtonsController::MSG_POST_CONFIGURATION_CHANGED);
    Boolean result;
    mHost->mHandler->SendEmptyMessage(ZoomButtonsController::MSG_POST_CONFIGURATION_CHANGED, &result);
    return NOERROR;
}

ZoomButtonsController::ZoomButtonsController()
    : mTouchPaddingScaledSq(0)
    , mAutoDismissControls(TRUE)
    , mReleaseTouchListenerOnUp(FALSE)
    , mIsVisible(FALSE)
{
    mOwnerViewRawLocation = ArrayOf<Int32>::Alloc(2);
    mContainerRawLocation = ArrayOf<Int32>::Alloc(2);
    mTouchTargetWindowLocation = ArrayOf<Int32>::Alloc(2);
    CRect::NewByFriend((CRect**)&mTempRect);
    mTempIntArray = ArrayOf<Int32>::Alloc(2);
    CIntentFilter::New(IIntent::ACTION_CONFIGURATION_CHANGED, (IIntentFilter**)&mConfigurationChangedFilter);
    mConfigurationChangedReceiver = new MyBroadcastReceiver(this);
    mHandler = new MyHandler(this);
}

ZoomButtonsController::ZoomButtonsController(
    /* [in] */ IView* ownerView)
{
    mOwnerViewRawLocation = ArrayOf<Int32>::Alloc(2);
    mContainerRawLocation = ArrayOf<Int32>::Alloc(2);
    mTouchTargetWindowLocation = ArrayOf<Int32>::Alloc(2);
    CRect::NewByFriend((CRect**)&mTempRect);
    mTempIntArray = ArrayOf<Int32>::Alloc(2);
    CIntentFilter::New(IIntent::ACTION_CONFIGURATION_CHANGED, (IIntentFilter**)&mConfigurationChangedFilter);
    mConfigurationChangedReceiver = new MyBroadcastReceiver(this);
    mHandler = new MyHandler(this);
    Init(ownerView);
}

ECode ZoomButtonsController::Init(
    /* [in] */ IView* ownerView)
{
    ownerView->GetContext((IContext**)&mContext);
    AutoPtr<IInterface> sTemp;
    mContext->GetSystemService(IContext::WINDOW_SERVICE, (IInterface**)&sTemp);
    mWindowManager = IWindowManager::Probe(sTemp);
    mOwnerView = ownerView;

    AutoPtr<IResources> resource;
    mContext->GetResources((IResources**)&resource);
    AutoPtr<IDisplayMetrics> metrics;
    resource->GetDisplayMetrics((IDisplayMetrics**)&metrics);
    Float density;
    metrics->GetDensity(&density);

    mTouchPaddingScaledSq = (Int32)(ZOOM_CONTROLS_TOUCH_PADDING * density);
    mTouchPaddingScaledSq *= mTouchPaddingScaledSq;

    mContainer = CreateContainer();
    return NOERROR;
}

ECode ZoomButtonsController::SetZoomInEnabled(
    /* [in] */ Boolean enabled)
{
   return mControls->SetIsZoomInEnabled(enabled);
}

ECode ZoomButtonsController::SetZoomOutEnabled(
    /* [in] */ Boolean enabled)
{
   return mControls->SetIsZoomOutEnabled(enabled);
}

/**
 * Sets the delay between zoom callbacks as the user holds a zoom button.
 *
 * @param speed The delay in milliseconds between zoom callbacks.
 */
ECode ZoomButtonsController::SetZoomSpeed(
        /* [in] */ Int64 speed)
{
   return mControls->SetZoomSpeed(speed);
}

AutoPtr<ZoomButtonsController::Container> ZoomButtonsController::CreateContainer()
{
    AutoPtr<IWindowManagerLayoutParams> lp;
    CWindowManagerLayoutParams::New(IWindowManagerLayoutParams::WRAP_CONTENT,
        IWindowManagerLayoutParams::WRAP_CONTENT, (IWindowManagerLayoutParams**)&lp);
    lp->SetGravity(IGravity::TOP | IGravity::START);
    lp->SetFlags(IWindowManagerLayoutParams::FLAG_NOT_TOUCHABLE |
         IWindowManagerLayoutParams::FLAG_NOT_FOCUSABLE |
         IWindowManagerLayoutParams::FLAG_LAYOUT_NO_LIMITS |
         IWindowManagerLayoutParams::FLAG_ALT_FOCUSABLE_IM);
    lp->SetHeight(IWindowManagerLayoutParams::WRAP_CONTENT);
    lp->SetWidth(IWindowManagerLayoutParams::WRAP_CONTENT);
    lp->SetType(IWindowManagerLayoutParams::TYPE_APPLICATION_PANEL);
    lp->SetFormat(IPixelFormat::TRANSLUCENT);
    lp->SetWindowAnimations(R::style::Animation_ZoomButtons);
    mContainerLayoutParams = lp;

    AutoPtr<Container> container = new Container(mContext, this);
    container->SetLayoutParams(lp);
    container->SetMeasureAllChildren(TRUE);

    AutoPtr<IInterface> sTemp;
    mContext ->GetSystemService(IContext::LAYOUT_INFLATER_SERVICE, (IInterface**)&sTemp);
    AutoPtr<ILayoutInflater> inflater = ILayoutInflater::Probe(sTemp);
    AutoPtr<IView> vTemp;
    inflater->Inflate(R::layout::zoom_container, container, (IView**)&vTemp);
    vTemp = NULL;
    container->FindViewById(R::id::zoomControls, (IView**)&vTemp);
    mControls = IZoomControls::Probe(vTemp);
    AutoPtr<IViewOnClickListener> incls = new MyClickListener(this, TRUE);
    mControls->SetOnZoomInClickListener(incls);
    AutoPtr<IViewOnClickListener> outcls = new MyClickListener(this, FALSE);
    mControls->SetOnZoomOutClickListener(outcls);
    return container;
}

ECode ZoomButtonsController::SetOnZoomListener(
    /* [in] */ IOnZoomListener* listener)
{
   mCallback = listener;
   return NOERROR;
}

ECode ZoomButtonsController::SetFocusable(
    /* [in] */ Boolean focusable)
{
    Int32 oldFlags, now;
    mContainerLayoutParams->GetFlags(&oldFlags);
    if (focusable) {
        mContainerLayoutParams->SetFlags(oldFlags & (~IWindowManagerLayoutParams::FLAG_NOT_FOCUSABLE));
    }
    else {
        mContainerLayoutParams->SetFlags(oldFlags | (IWindowManagerLayoutParams::FLAG_NOT_FOCUSABLE));
    }

    mContainerLayoutParams->GetFlags(&now);
    if ((now != oldFlags) && mIsVisible) {
        mWindowManager->UpdateViewLayout(mContainer, mContainerLayoutParams);
    }
    return NOERROR;
}

Boolean ZoomButtonsController::IsAutoDismissed()
{
    return mAutoDismissControls;
}

ECode ZoomButtonsController::SetAutoDismissed(
        /* [in] */ Boolean autoDismiss)
{
    if (mAutoDismissControls != autoDismiss)
        mAutoDismissControls = autoDismiss;
    return NOERROR;
}

Boolean ZoomButtonsController::IsVisible()
{
    return mIsVisible;
}

ECode ZoomButtonsController::SetVisible(
    /* [in] */ Boolean visible)
{
    if (visible) {
        AutoPtr<IBinder> b;
        mOwnerView->GetWindowToken((IBinder**)&b);
        if (b == NULL) {
            /*
             * We need a window token to show ourselves, maybe the owner's
             * window hasn't been created yet but it will have been by the
             * time the looper is idle, so post the setVisible(TRUE) call.
             */
            Boolean hasMsg;
            if (mHandler->HasMessages(MSG_POST_SET_VISIBLE, &hasMsg), !hasMsg) {
                Boolean result;
                return mHandler->SendEmptyMessage(MSG_POST_SET_VISIBLE, &result);
            }
            return NOERROR;
        }

        DismissControlsDelayed(ZOOM_CONTROLS_TIMEOUT);
    }

    if (mIsVisible == visible) {
        return NOERROR;
    }
    mIsVisible = visible;

    if (visible) {
        AutoPtr<IBinder> binder;
        mContainerLayoutParams->GetToken((IBinder**)&binder);
        if (binder == NULL) {
            AutoPtr<IBinder> b;
            mOwnerView->GetWindowToken((IBinder**)&b);
            mContainerLayoutParams->SetToken(b);
        }

        IViewManager::Probe(mWindowManager)->AddView(mContainer, mContainerLayoutParams);

        if (mPostedVisibleInitializer == NULL) {
            mPostedVisibleInitializer = new MyRunnable(this);
        }
        Boolean postRst;
        mHandler->Post(mPostedVisibleInitializer, &postRst);

        // Handle configuration changes when visible
        AutoPtr<IIntent> rst;
        mContext->RegisterReceiver(mConfigurationChangedReceiver, mConfigurationChangedFilter, (IIntent**)&rst);

        // Steal touches events from the owner
        mOwnerView->SetOnTouchListener(THIS_PROBE(IViewOnTouchListener));
        mReleaseTouchListenerOnUp = FALSE;

    }
    else {
        // Don't want to steal any more touches
        if (mTouchTargetView != NULL) {
            // We are still stealing the touch events for this touch
            // sequence, so release the touch listener later
            mReleaseTouchListenerOnUp = TRUE;
        }
        else {
            mOwnerView->SetOnTouchListener(NULL);
        }

        // No longer care about configuration changes
        mContext->UnregisterReceiver(mConfigurationChangedReceiver);

        IViewManager::Probe(mWindowManager)->RemoveView(mContainer);
        mHandler->RemoveCallbacks(mPostedVisibleInitializer);

        if (mCallback != NULL) {
            mCallback->OnVisibilityChanged(FALSE);
        }
    }
    return NOERROR;
}

AutoPtr<IViewGroup> ZoomButtonsController::GetContainer()
{
    return mContainer;
}

AutoPtr<IView> ZoomButtonsController::GetZoomControls()
{
    return mControls;
}

ECode ZoomButtonsController::DismissControlsDelayed(
    /* [in] */ Int32 delay)
{
    if (mAutoDismissControls) {
        mHandler->RemoveMessages(MSG_DISMISS_ZOOM_CONTROLS);
        Boolean result;
        return mHandler->SendEmptyMessageDelayed(MSG_DISMISS_ZOOM_CONTROLS, delay, &result);
    }
    return NOERROR;
}

ECode ZoomButtonsController::RefreshPositioningVariables()
{
    // if the mOwnerView is detached from window then skip.
    AutoPtr<IBinder> token;
    mOwnerView->GetWindowToken((IBinder**)&token);
    if (token == NULL) return NOERROR;

    // Position the zoom controls on the bottom of the owner view.
    Int32 ownerHeight;
    mOwnerView->GetHeight(&ownerHeight);
    Int32 ownerWidth;
    mOwnerView->GetWidth(&ownerWidth);
    // The gap between the top of the owner and the top of the container
    Int32 containerOwnerYOffset;
    mContainer->GetHeight(&containerOwnerYOffset);
    containerOwnerYOffset = ownerHeight - containerOwnerYOffset;

    // Calculate the owner view's bounds
    mOwnerView->GetLocationOnScreen(&(*mOwnerViewRawLocation)[0], &(*mOwnerViewRawLocation)[1]);
    (*mContainerRawLocation)[0] = (*mOwnerViewRawLocation)[0];
    (*mContainerRawLocation)[1] = (*mOwnerViewRawLocation)[1] + containerOwnerYOffset;

    AutoPtr<ArrayOf<Int32> > ownerViewWindowLoc = mTempIntArray;
    mOwnerView->GetLocationInWindow(&(*ownerViewWindowLoc)[0], &(*ownerViewWindowLoc)[1]);

    // lp.x and lp.y should be relative to the owner's window top-left
    mContainerLayoutParams->SetX((*ownerViewWindowLoc)[0]);
    mContainerLayoutParams->SetWidth(ownerWidth);
    mContainerLayoutParams->SetY((*ownerViewWindowLoc)[1] + containerOwnerYOffset);
    if (mIsVisible) {
        mWindowManager->UpdateViewLayout(mContainer, mContainerLayoutParams);
    }
    return NOERROR;
}

/* This will only be called when the container has focus. */
Boolean ZoomButtonsController::OnContainerKey(
    /* [in] */ IKeyEvent* event)
{
    Int32 keyCode;
    event->GetKeyCode(&keyCode);
    if (IsInterestingKey(keyCode)) {

        if (keyCode == IKeyEvent::KEYCODE_BACK) {
            Int32 action, repeatCount;
            Boolean tracking, canceled;
            if ((event->GetAction(&action), action) == IKeyEvent::ACTION_DOWN
                    && (event->GetRepeatCount(&repeatCount), repeatCount) == 0) {
                if (mOwnerView != NULL) {
                    AutoPtr<IDispatcherState> ds;
                    mOwnerView->GetKeyDispatcherState((IDispatcherState**)&ds);
                    if (ds != NULL) {
                        ds->StartTracking(event, THIS_PROBE(IInterface));
                    }
                }
                return TRUE;
            }
            else if ((event->GetAction(&action), action) == IKeyEvent::ACTION_UP
                    && (event->IsTracking(&tracking), tracking) && !(event->IsCanceled(&canceled), canceled)) {
                SetVisible(FALSE);
                return TRUE;
            }

        } else {
            DismissControlsDelayed(ZOOM_CONTROLS_TIMEOUT);
        }

        // Let the container handle the key
        return FALSE;

    } else {

        AutoPtr<ViewRootImpl> viewRoot = reinterpret_cast<Elastos::Droid::View::View*>((mOwnerView)->Probe(EIID_View))->GetViewRootImpl();
        if (viewRoot != NULL) {
            viewRoot->DispatchKey(event);
        }

        // We gave the key to the owner, don't let the container handle this key
        return TRUE;
    }
}

Boolean ZoomButtonsController::IsInterestingKey(
    /* [in] */ Int32 keyCode)
{
    switch (keyCode) {
        case IKeyEvent::KEYCODE_DPAD_CENTER:
        case IKeyEvent::KEYCODE_DPAD_UP:
        case IKeyEvent::KEYCODE_DPAD_DOWN:
        case IKeyEvent::KEYCODE_DPAD_LEFT:
        case IKeyEvent::KEYCODE_DPAD_RIGHT:
        case IKeyEvent::KEYCODE_ENTER:
        case IKeyEvent::KEYCODE_BACK:
            return TRUE;
        default:
            return FALSE;
    }
}

/**
 * @hide The ZoomButtonsController implements the OnTouchListener, but this
 *       does not need to be shown in its public API.
 */
ECode ZoomButtonsController::OnTouch(
    /* [in] */ IView* v,
    /* [in] */ IMotionEvent* event,
    /* [out] */ Boolean* result)
{
    Int32 action;
    event->GetAction(&action);

    Int32 pointerCount;
    if ((event->GetPointerCount(&pointerCount), pointerCount) > 1) {
        // ZoomButtonsController doesn't handle mutitouch. Give up control.
        *result = FALSE;
        return NOERROR;
    }

    if (mReleaseTouchListenerOnUp) {
        // The controls were dismissed but we need to throw away all events until the up
        if (action == IMotionEvent::ACTION_UP || action == IMotionEvent::ACTION_CANCEL) {
            mOwnerView->SetOnTouchListener(NULL);
            SetTouchTargetView(NULL);
            mReleaseTouchListenerOnUp = FALSE;
        }

       // Eat this event
       *result = TRUE;
       return NOERROR;
    }

    DismissControlsDelayed(ZOOM_CONTROLS_TIMEOUT);

    AutoPtr<IView> targetView = mTouchTargetView;

    switch (action) {
       case IMotionEvent::ACTION_DOWN:
            Float rawX, rawY;
            event->GetRawX(&rawX);
            event->GetRawY(&rawY);
            targetView = FindViewForTouch((Int32)rawX, (Int32)rawY);
            SetTouchTargetView(targetView);
            break;

       case IMotionEvent::ACTION_UP:
       case IMotionEvent::ACTION_CANCEL:
            SetTouchTargetView(NULL);
            break;
    }

    if (targetView != NULL) {
        // The upperleft corner of the target view in raw coordinates
        Int32 targetViewRawX = (*mContainerRawLocation)[0] + (*mTouchTargetWindowLocation)[0];
        Int32 targetViewRawY = (*mContainerRawLocation)[1] + (*mTouchTargetWindowLocation)[1];

        AutoPtr<IMotionEventHelper> helper;
        CMotionEventHelper::AcquireSingleton((IMotionEventHelper**)&helper);
        AutoPtr<IMotionEvent> containerEvent;
        helper->Obtain(event, (IMotionEvent**)&containerEvent);
        // Convert the motion event into the target view's coordinates (from
        // owner view's coordinates)
        containerEvent->OffsetLocation((*mOwnerViewRawLocation)[0] - targetViewRawX,
               (*mOwnerViewRawLocation)[1] - targetViewRawY);
        /* Disallow negative coordinates (which can occur due to
        * ZOOM_CONTROLS_TOUCH_PADDING) */
        // These are floats because we need to potentially offset away this exact amount
        Float containerX;
        containerEvent->GetX(&containerX);
        Float containerY;
        containerEvent->GetY(&containerY);
        if (containerX < 0 && containerX > -ZOOM_CONTROLS_TOUCH_PADDING) {
           containerEvent->OffsetLocation(-containerX, 0);
        }
        if (containerY < 0 && containerY > -ZOOM_CONTROLS_TOUCH_PADDING) {
           containerEvent->OffsetLocation(0, -containerY);
        }
        Boolean retValue;
        targetView->DispatchTouchEvent(containerEvent, &retValue);
        containerEvent->Recycle();
        *result = retValue;

    } else {
        *result = FALSE;
    }
    return NOERROR;
}

ECode ZoomButtonsController::SetTouchTargetView(
    /* [in] */ IView* view)
{
    mTouchTargetView = view;
    if (view != NULL) {
        view->GetLocationInWindow(&(*mTouchTargetWindowLocation)[0], &(*mTouchTargetWindowLocation)[1]);
    }
    return NOERROR;
}

AutoPtr<IView> ZoomButtonsController::FindViewForTouch(
    /* [in] */ Int32 rawX,
    /* [in] */ Int32 rawY)
{
    // Reverse order so the child drawn on top gets first dibs.
    Int32 containerCoordsX = rawX - (*mContainerRawLocation)[0];
    Int32 containerCoordsY = rawY - (*mContainerRawLocation)[1];
    AutoPtr<CRect> frame = mTempRect;

    AutoPtr<IView> closestChild = NULL;
    Int32 closestChildDistanceSq = Elastos::Core::Math::INT32_MAX_VALUE;
    Int32 i;
    mContainer->GetChildCount(&i);
    i -= 1;
    for (; i >= 0; i--) {
        AutoPtr<IView> child;
        mContainer->GetChildAt(i, (IView**)&child);
        Int32 visibility;
        child->GetVisibility(&visibility);
        if (visibility != IView::VISIBLE) {
            continue;
        }

        child->GetHitRect(frame);
        Boolean rst;
        if (frame->Contains(containerCoordsX, containerCoordsY, &rst), rst) {
            return child;
        }

        Int32 distanceX;
        if (containerCoordsX >= frame->mLeft && containerCoordsX <= frame->mRight) {
            distanceX = 0;
        }
        else {
            distanceX = Elastos::Core::Math::Min(Elastos::Core::Math::Abs(frame->mLeft - containerCoordsX),
                Elastos::Core::Math::Abs(containerCoordsX - frame->mRight));
        }
        Int32 distanceY;
        if (containerCoordsY >= frame->mTop && containerCoordsY <= frame->mBottom) {
            distanceY = 0;
        }
        else {
            distanceY = Elastos::Core::Math::Min(Elastos::Core::Math::Abs(frame->mTop - containerCoordsY),
                    Elastos::Core::Math::Abs(containerCoordsY - frame->mBottom));
        }
        Int32 distanceSq = distanceX * distanceX + distanceY * distanceY;

        if ((distanceSq < mTouchPaddingScaledSq) &&
                (distanceSq < closestChildDistanceSq)) {
            closestChild = child;
            closestChildDistanceSq = distanceSq;
        }
    }

    return closestChild;
}

ECode ZoomButtonsController::OnPostConfigurationChanged()
{
    DismissControlsDelayed(ZOOM_CONTROLS_TIMEOUT);
    RefreshPositioningVariables();
    return NOERROR;
}


}// namespace Widget
}// namespace Droid
}// namespace Elastos
