
#ifndef __ELASTOS_DROID_WIDGET_ZOOMBUTTONSCONTROLLER_H__
#define __ELASTOS_DROID_WIDGET_ZOOMBUTTONSCONTROLLER_H__

#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/widget/FrameLayout.h"


#include "elastos/droid/content/BroadcastReceiver.h"
#include "elastos/droid/os/Runnable.h"
#include "elastos/droid/os/HandlerBase.h"
#include "elastos/droid/R.h"

using Elastos::Droid::R;
using Elastos::Droid::Os::Runnable;
using Elastos::Droid::Os::HandlerBase;
using Elastos::Droid::Content::IIntent;
using Elastos::Droid::Content::IIntentFilter;
using Elastos::Droid::Content::BroadcastReceiver;
using Elastos::Droid::View::IViewOnTouchListener;
using Elastos::Droid::View::IViewOnClickListener;
using Elastos::Droid::View::IMotionEvent;
using Elastos::Droid::View::IKeyEvent;
using Elastos::Droid::View::IWindowManager;
using Elastos::Droid::View::IWindowManagerLayoutParams;
using Elastos::Droid::View::IViewManager;
using Elastos::Droid::View::IViewParent;
using Elastos::Droid::View::IViewGroup;
using Elastos::Droid::View::IKeyEventCallback;
using Elastos::Droid::View::IGravity;
using Elastos::Droid::View::Accessibility::IAccessibilityEventSource;
using Elastos::Droid::Graphics::Drawable::IDrawableCallback;

namespace Elastos{
namespace Droid{
namespace Widget{

class ZoomButtonsController
{
private:
    class MyHandler : public HandlerBase
    {
    public:
        MyHandler(
            /* [in] */ ZoomButtonsController* host)
            : mHost(host)
        {}

        CARAPI HandleMessage(
            /* [in] */ IMessage* msg);
    private:
        ZoomButtonsController* mHost;
    };

    class _Container
        : public FrameLayout
    {
    public:
        _Container(
            /* [in] */ IContext* context,
            /* [in] */ ZoomButtonsController* controller)
            : FrameLayout(context)
            , mController(controller)
        {
        }

        /*
         * Need to override this to intercept the key events. Otherwise, we
         * would attach a key listener to the container but its superclass
         * ViewGroup gives it to the focused View instead of calling the key
         * listener, and so we wouldn't get the events.
         */
        virtual CARAPI_(Boolean) DispatchKeyEvent(
            /* [in] */ IKeyEvent* event)
        {
            return mController->OnContainerKey(event) ?
                    TRUE : FrameLayout::DispatchKeyEvent(event);
        }
    protected:
        ZoomButtonsController* mController;
    };

    class Container
        : public _Container
        , public IFrameLayout
        , public IViewParent
        , public IViewManager
        , public IDrawableCallback
        , public IKeyEventCallback
        , public IAccessibilityEventSource
        , public IWeakReferenceSource
        , public ElRefBase
    {
    public:
        Container(
            /* [in] */ IContext* context,
            /* [in] */ ZoomButtonsController* controller)
            : _Container(context, controller)
        {}

        IVIEW_METHODS_DECL()
        IVIEWGROUP_METHODS_DECL()
        IVIEWPARENT_METHODS_DECL()
        IVIEWMANAGER_METHODS_DECL()
        IDRAWABLECALLBACK_METHODS_DECL()
        IKEYEVENTCALLBACK_METHODS_DECL()
        IACCESSIBILITYEVENTSOURCE_METHODS_DECL()
        IFRAMELAYOUT_METHODS_DECL()

        CARAPI_(PInterface) Probe(
            /* [in] */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        CARAPI GetWeakReference(
            /* [out] */ IWeakReference** weakReference);
    };

    class MyBroadcastReceiver : public BroadcastReceiver
    {
    public:
        MyBroadcastReceiver(
            /* [in] */ ZoomButtonsController* host);

        virtual CARAPI OnReceive(
            /* [in] */ IContext* context,
            /* [in] */ IIntent* intent);

        CARAPI ToString(
            /* [out] */ String* info)
        {
            VALIDATE_NOT_NULL(info);
            *info = String("ZoomButtonsController::MyBroadcastReceiver: ");
            (*info).AppendFormat("%p", this);
            return NOERROR;
        }
    private:
        ZoomButtonsController* mHost;
    };

    class MyClickListener
        : public IViewOnClickListener
        , public ElRefBase
    {
    public:
        CAR_INTERFACE_DECL()

        MyClickListener(
            /* [in] */ ZoomButtonsController* host,
            /* [in] */ Boolean onZoom);

        CARAPI OnClick(
            /* [in] */ IView* v);
    private:
        ZoomButtonsController* mHost;
        Boolean mOnZoom;
    };

    class MyRunnable : public Runnable
    {
    public:
        MyRunnable(
            /* [in] */ ZoomButtonsController* host);

        CARAPI Run();

    private:
        ZoomButtonsController* mHost;
    };
public:

    /**
     * @hide The ZoomButtonsController implements the OnTouchListener, but this
     *       does not need to be shown in its public API.
     */
    CARAPI OnTouch(
        /* [in] */ IView* v,
        /* [in] */ IMotionEvent* event,
        /* [out] */ Boolean* result);

    /**
     * Constructor for the {@link ZoomButtonsController}.
     *
     * @param ownerView The view that is being zoomed by the zoom controls. The
     *            zoom controls will be displayed aligned with this view.
     */
    ZoomButtonsController();

    ZoomButtonsController(
        /* [in] */ IView* ownerView);

    virtual CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid) = 0;
    /**
     * Whether to enable the zoom in control.
     *
     * @param enabled Whether to enable the zoom in control.
     */
    CARAPI  SetZoomInEnabled(
        /* [in] */ Boolean enabled);

    /**
     * Whether to enable the zoom out control.
     *
     * @param enabled Whether to enable the zoom out control.
     */
    CARAPI SetZoomOutEnabled(
        /* [in] */ Boolean enabled);

    /**
     * Sets the delay between zoom callbacks as the user holds a zoom button.
     *
     * @param speed The delay in milliseconds between zoom callbacks.
     */
    CARAPI SetZoomSpeed(
        /* [in] */ Int64 speed);

    /**
     * Sets the {@link OnZoomListener} listener that receives callbacks to zoom.
     *
     * @param listener The listener that will be told to zoom.
     */
    CARAPI SetOnZoomListener(
        /* [in] */ IOnZoomListener* listener);

    /**
     * Sets whether the zoom controls should be focusable. If the controls are
     * focusable, then trackball and arrow key interactions are possible.
     * Otherwise, only touch interactions are possible.
     *
     * @param focusable Whether the zoom controls should be focusable.
     */
    CARAPI SetFocusable(
        /* [in] */ Boolean focusable);

    /**
     * Whether the zoom controls will be automatically dismissed after showing.
     *
     * @return Whether the zoom controls will be auto dismissed after showing.
     */
    Boolean IsAutoDismissed();

    /**
     * Sets whether the zoom controls will be automatically dismissed after
     * showing.
     */
    CARAPI SetAutoDismissed(
        /* [in] */ Boolean autoDismiss);

    /**
     * Whether the zoom controls are visible to the user.
     *
     * @return Whether the zoom controls are visible to the user.
     */
    Boolean IsVisible();

    /**
     * Sets whether the zoom controls should be visible to the user.
     *
     * @param visible Whether the zoom controls should be visible to the user.
     */
    CARAPI SetVisible(
        /* [in] */ Boolean visible);

    /**
     * Gets the container that is the parent of the zoom controls.
     * <p>
     * The client can add other views to this container to link them with the
     * zoom controls.
     *
     * @return The container of the zoom controls. It will be a layout that
     *         respects the gravity of a child's layout parameters.
     */
    CARAPI_(AutoPtr<IViewGroup>) GetContainer();

    /**
     * Gets the view for the zoom controls.
     *
     * @return The zoom controls view.
     */
    AutoPtr<IView> GetZoomControls();

    /* This will only be called when the container has focus. */
    Boolean OnContainerKey(
        /* [in] */ IKeyEvent* event);

protected:
    CARAPI Init(
        /* [in] */ IView* ownerView);

private:
    AutoPtr<Container> CreateContainer();

    CARAPI DismissControlsDelayed(
        /* [in] */ Int32 delay);

    CARAPI RefreshPositioningVariables();

    Boolean IsInterestingKey(
        /* [in] */ Int32 keyCode);

    CARAPI SetTouchTargetView(
        /* [in] */ IView* view);

    /**
     * Returns the View that should receive a touch at the given coordinates.
     *
     * @param rawX The raw X.
     * @param rawY The raw Y.
     * @return The view that should receive the touches, or null if there is not one.
     */
    AutoPtr<IView> FindViewForTouch(
        /* [in] */ Int32 rawX,
        /* [in] */ Int32 rawY);

    CARAPI OnPostConfigurationChanged();

private:
    static const String TAG;// = "ZoomButtonsController";

    static const Int32 ZOOM_CONTROLS_TIMEOUT;// = (Int32) ViewConfiguration.getZoomControlsTimeout();

    static const Int32 ZOOM_CONTROLS_TOUCH_PADDING = 20;

    /** When configuration changes, this is called after the UI thread is idle. */
    static const Int32 MSG_POST_CONFIGURATION_CHANGED = 2;
    /** Used to delay the zoom controller dismissal. */
    static const Int32 MSG_DISMISS_ZOOM_CONTROLS = 3;
    /**
     * If setVisible(true) is called and the owner view's window token is null,
     * we delay the setVisible(true) call until it is not null.
     */
    static const Int32 MSG_POST_SET_VISIBLE = 4;

    Int32 mTouchPaddingScaledSq;

    AutoPtr<IContext> mContext;
    AutoPtr<IWindowManager> mWindowManager;
    Boolean mAutoDismissControls;// = true;

    /**
     * The view that is being zoomed by this zoom controller.
     */
    IView* mOwnerView;

    /**
     * The location of the owner view on the screen. This is recalculated
     * each time the zoom controller is shown.
     */
    AutoPtr<ArrayOf<Int32> > mOwnerViewRawLocation;// = new Int32[2];

    /**
     * The container that is added as a window.
     */
    AutoPtr<Container> mContainer;
    AutoPtr<IWindowManagerLayoutParams> mContainerLayoutParams;
    AutoPtr<ArrayOf<Int32> > mContainerRawLocation;// = new Int32[2];

    AutoPtr<IZoomControls> mControls;

    /**
     * The view (or null) that should receive touch events. This will get set if
     * the touch down hits the container. It will be reset on the touch up.
     */
    AutoPtr<IView> mTouchTargetView;

    /**
     * The {@link #mTouchTargetView}'s location in window, set on touch down.
     */
    AutoPtr<ArrayOf<Int32> > mTouchTargetWindowLocation;// = new Int32[2];

    /**
     * If the zoom controller is dismissed but the user is still in a touch
     * interaction, we set this to true. This will ignore all touch events until
     * up/cancel, and then set the owner's touch listener to null.
     * <p>
     * Otherwise, the owner view would get mismatched events (i.e., touch move
     * even though it never got the touch down.)
     */
    Boolean mReleaseTouchListenerOnUp;

    /** Whether the container has been added to the window manager. */
    Boolean mIsVisible;

    AutoPtr<CRect> mTempRect;// = new Rect();
    AutoPtr<ArrayOf<Int32> > mTempIntArray;// = new Int32[2];

    AutoPtr<IOnZoomListener> mCallback;

    /**
     * When showing the zoom, we add the view as a new window. However, there is
     * logic that needs to know the size of the zoom which is determined after
     * it's laid out. Therefore, we must post this logic onto the UI thread so
     * it will be exceuted AFTER the layout. This is the logic.
     */
    AutoPtr<IRunnable> mPostedVisibleInitializer;

    AutoPtr<IIntentFilter> mConfigurationChangedFilter;// =  new IntentFilter(Intent.ACTION_CONFIGURATION_CHANGED);
    AutoPtr<MyBroadcastReceiver> mConfigurationChangedReceiver;
    AutoPtr<IHandler> mHandler;
};

}// namespace Widget
}// namespace Droid
}// namespace Elastos

#endif
