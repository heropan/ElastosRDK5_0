
#ifndef __ELASTOS_DROID_VIEW_VIEWROOTIMPL_H__
#define __ELASTOS_DROID_VIEW_VIEWROOTIMPL_H__

#include "elastos/droid/ext/frameworkext.h"
#include "view/CWindowManagerLayoutParams.h"
#include "view/View.h"
#include "view/CSurface.h"
#include "view/BaseSurfaceHolder.h"
#include "view/CInputQueue.h"
#include "view/InputEventReceiver.h"
#include "view/HardwareRenderer.h"
#include "view/CViewConfiguration.h"
#include "graphics/CRect.h"
#include "graphics/CRegion.h"
#include "os/HandlerBase.h"
#include "widget/CScroller.h"
#include <elastos/utility/etl/List.h>
#include <elastos/utility/etl/HashSet.h>

using Elastos::Utility::Etl::HashSet;
using Elastos::Core::IThread;
using Elastos::Droid::Content::IComponentCallbacks;
using Elastos::Droid::Content::IClipDescription;
using Elastos::Droid::Animation::ILayoutTransition;
using Elastos::Droid::View::InputMethod::IInputMethodManagerFinishedEventCallback;
using Elastos::Droid::View::Accessibility::IAccessibilityManager;
using Elastos::Droid::View::Accessibility::IAccessibilityStateChangeListener;
using Elastos::Droid::View::Accessibility::IAccessibilityInteractionConnection;
using Elastos::Droid::View::Accessibility::IAccessibilityInteractionConnectionCallback;
using Elastos::Droid::Utility::ITypedValue;
using Elastos::Droid::Os::HandlerBase;
using Elastos::Droid::Widget::CScroller;
using Elastos::Droid::Media::IAudioManager;

namespace Elastos {
namespace Droid {
namespace View {

extern "C" const InterfaceID EIID_ViewRootImpl;

//class Scroller;
class CViewRootImplW;
class CAccessibilityInteractionController;

class ViewRootImpl :
    public ElRefBase,
    public IViewParent,
    public View::AttachInfo::Callbacks,
    public HardwareRenderer::HardwareDrawCallbacks,
    public IWeakReferenceSource
{
    friend class View;
    friend class CWindowManagerGlobal;
    friend class CViewRootImplW;
private:
    static const char* TAG;
    static const Boolean DBG;
    static const Boolean LOCAL_LOGV;
    static const Boolean DEBUG_DRAW;
    static const Boolean DEBUG_LAYOUT;
    static const Boolean DEBUG_DIALOG;
    static const Boolean DEBUG_INPUT_RESIZE;
    static const Boolean DEBUG_ORIENTATION;
    static const Boolean DEBUG_TRACKBALL;
    static const Boolean DEBUG_IMF;
    static const Boolean DEBUG_CONFIGURATION;
    static const Boolean DEBUG_FPS;

    static const Int32 MSG_INVALIDATE;// = 1;
    static const Int32 MSG_INVALIDATE_RECT;// = 2;
    static const Int32 MSG_DIE;// = 3;
    static const Int32 MSG_RESIZED;// = 4;
    static const Int32 MSG_RESIZED_REPORT;// = 5;
    static const Int32 MSG_WINDOW_FOCUS_CHANGED;// = 6;
    static const Int32 MSG_DISPATCH_KEY;// = 7;
    static const Int32 MSG_DISPATCH_APP_VISIBILITY;// = 8;
    static const Int32 MSG_DISPATCH_GET_NEW_SURFACE;// = 9;
    static const Int32 MSG_IME_FINISHED_EVENT;// = 10;
    static const Int32 MSG_DISPATCH_KEY_FROM_IME;// = 11;
    static const Int32 MSG_FINISH_INPUT_CONNECTION;// = 12;
    static const Int32 MSG_CHECK_FOCUS;// = 13;
    static const Int32 MSG_CLOSE_SYSTEM_DIALOGS;// = 14;
    static const Int32 MSG_DISPATCH_DRAG_EVENT;// = 15;
    static const Int32 MSG_DISPATCH_DRAG_LOCATION_EVENT;// = 16;
    static const Int32 MSG_DISPATCH_SYSTEM_UI_VISIBILITY;// = 17;
    static const Int32 MSG_UPDATE_CONFIGURATION;// = 18;
    static const Int32 MSG_PROCESS_INPUT_EVENTS;// = 19;
    static const Int32 MSG_DISPATCH_SCREEN_STATE;// = 20;
    static const Int32 MSG_INVALIDATE_DISPLAY_LIST;// = 21;
    static const Int32 MSG_CLEAR_ACCESSIBILITY_FOCUS_HOST;// = 22;
    static const Int32 MSG_DISPATCH_DONE_ANIMATING;// = 23;
    static const Int32 MSG_INVALIDATE_WORLD;// = 24;
    static const Int32 MSG_WINDOW_MOVED;// = 25;

    static const Boolean USE_RENDER_THREAD;

    /**
     * Set this system property to true to force the view hierarchy to render
     * at 60 Hz. This can be used to measure the potential framerate.
     */
    static const String PROPERTY_PROFILE_RENDERING;

    static const Boolean MEASURE_LATENCY;

    /**
    * Maximum time we allow the user to roll the trackball enough to generate
    * a key event, before resetting the counters.
    */
    static const Int32 MAX_TRACKBALL_DELAY;

    // Pool of queued input events.
    static const Int32 MAX_QUEUED_INPUT_EVENT_POOL_SIZE;

    //static LatencyTimer mLt;

    static pthread_key_t sKeyRunQueues;
    static Boolean sKeyRunQueuesInited;

    static List<AutoPtr<IRunnable> > sFirstDrawHandlers;
    static Object sFirstDrawHandlersLock;

    static Boolean sFirstDrawComplete;

    static List<AutoPtr<IComponentCallbacks> > sConfigCallbacks;
    static Object sConfigCallbacksLock;

    static Boolean sUseRenderThread;
    static Boolean sRenderThreadQueried ;
    static Object sRenderThreadQueryLock;

    static AutoPtr<Elastos::Droid::View::Animation::IInterpolator> mResizeInterpolator;

private:
    class ViewRootHandler : public HandlerBase
    {
    public:
        ViewRootHandler(
            /* [in] */ ViewRootImpl* host);

        CARAPI HandleMessage(
            /* [in] */ IMessage* msg);

        CARAPI_(String) GetMessageNameImpl(
            /* [in] */ IMessage* msg);
    private:
        AutoPtr<IWeakReference> mWeakHost;
    };

    class SystemUiVisibilityInfo
        : public ElRefBase
        , public IInterface
    {
    public:
        CAR_INTERFACE_DECL()

        SystemUiVisibilityInfo()
            : mSeq(0)
            , mGlobalVisibility(0)
            , mLocalValue(0)
            , mLocalChanges(0)
        {}

        Int32 mSeq;
        Int32 mGlobalVisibility;
        Int32 mLocalValue;
        Int32 mLocalChanges;
    };

    /**
     * Represents a pending input event that is waiting in a queue.
     *
     * Input events are processed in serial order by the timestamp specified by
     * {@link InputEvent#getEventTimeNano()}.  In general, the input dispatcher delivers
     * one input event to the application at a time and waits for the application
     * to finish handling it before delivering the next one.
     *
     * However, because the application or IME can synthesize and inject multiple
     * key events at a time without going through the input dispatcher, we end up
     * needing a queue on the application's side.
     */
    class QueuedInputEvent : public ElRefBase
    {
    public:
        QueuedInputEvent()
            : mFlags(0)
        {}

        static const Int32 FLAG_DELIVER_POST_IME = 1;

        AutoPtr<QueuedInputEvent> mNext;

        AutoPtr<IInputEvent> mEvent;
        AutoPtr<InputEventReceiver> mReceiver;
        Int32 mFlags;
    };

    class TraversalRunnable : public Runnable
    {
    public:
        TraversalRunnable(
            /* [in] */ IWeakReference* host);

        CARAPI Run();

    private:
        AutoPtr<IWeakReference> mHost;
    };


    class WindowInputEventReceiver : public InputEventReceiver
    {
    public:
        WindowInputEventReceiver(
            /* [in] */ IInputChannel* inputChannel,
            /* [in] */ ILooper* looper,
            /* [in] */ IWeakReference* viewRootImpl);

        //@Override
        CARAPI OnInputEvent(
            /* [in] */ IInputEvent* event);

        CARAPI OnBatchedInputEventPending();

        CARAPI Dispose();

    private:
        AutoPtr<IWeakReference> mHost;
    };

    class ConsumeBatchedInputRunnable : public Runnable
    {
    public:
        ConsumeBatchedInputRunnable(
            /* [in] */ IWeakReference* host);

        CARAPI Run();

    private:
        AutoPtr<IWeakReference> mHost;
    };

    class InvalidateOnAnimationRunnable : public Runnable
    {
    public:
        InvalidateOnAnimationRunnable(
          /* [in] */ IWeakReference* host);

        CARAPI_(void) AddView(
            /* [in] */ IView* view);

        CARAPI_(void) AddViewRect(
            /* [in] */ View::AttachInfo::InvalidateInfo* info);

        CARAPI_(void) RemoveView(
            /* [in] */ IView* view);

        //@Override
        CARAPI Run();

    private:
        CARAPI_(void) PostIfNeededLocked();

    private:
        Boolean mPosted;
        List<AutoPtr<IView> > mViews;
        List<AutoPtr<View::AttachInfo::InvalidateInfo> > mViewRects;
        AutoPtr<ArrayOf<IView*> > mTempViews;
        AutoPtr<ArrayOf<View::AttachInfo::InvalidateInfo*> > mTempViewRects;

        Object mSelfLock;
        AutoPtr<IWeakReference> mHost;
    };


    class TakenSurfaceHolder : public BaseSurfaceHolder
    {
    public:
        TakenSurfaceHolder(
            /* [in] */ ViewRootImpl* viewRoot);

        CARAPI_(Boolean) OnAllowLockCanvas();

        CARAPI_(void) OnRelayoutContainer();

        CARAPI SetFormat(
            /* [in] */ Int32 format);

        CARAPI SetType(
            /* [in] */ Int32 type);

        CARAPI OnUpdateSurface();

        CARAPI IsCreating(
            /* [out] */ Boolean* result);

        CARAPI SetFixedSize(
            /* [in] */ Int32 width,
            /* [in] */ Int32 height);

        CARAPI SetKeepScreenOn(
            /* [in] */ Boolean screenOn);

    private:
        ViewRootImpl* mViewRoot;
    };

    class InputMethodCallback
        : public ElRefBase
        , public IInputMethodManagerFinishedEventCallback
    {
    public:
        CAR_INTERFACE_DECL()

        InputMethodCallback(
            /* [in] */ IWeakReference* viewAncestor);

        //@Override
        CARAPI FinishedEvent(
            /* [in] */ Int32 seq,
            /* [in] */ Boolean handled);

    private:
        AutoPtr<IWeakReference> mViewAncestor;
    };

    /**
     * Maintains state information for a single trackball axis, generating
     * discrete (DPAD) movements based on raw trackball motion.
     */
    class TrackballAxis : public ElRefBase
    {
        friend class ViewRootImpl;
    public:
        TrackballAxis();

        CARAPI_(void) Reset(
            /* [in] */ Int32 _step);

        /**
         * Add trackball movement into the state.  If the direction of movement
         * has been reversed, the state is reset before adding the
         * movement (so that you don't have to compensate for any previously
         * collected movement before see the result of the movement in the
         * new direction).
         *
         * @return Returns the absolute value of the amount of movement
         * collected so far.
         */
        CARAPI_(Float) Collect(
            /* [in] */ Float off,
            /* [in] */ Int64 time,
            /* [in] */ const char* axis);

        /**
         * Generate the number of discrete movement events appropriate for
         * the currently collected trackball movement.
         *
         * @param precision The minimum movement required to generate the
         * first discrete movement.
         *
         * @return Returns the number of discrete movements, either positive
         * or negative, or 0 if there is not enough trackball movement yet
         * for a discrete movement.
         */
        CARAPI_(Int32) Generate(
            /* [in] */ Float precision);

    private:
        /**
        * The maximum amount of acceleration we will apply.
        */
        static const Float MAX_ACCELERATION = 20;

        /**
        * The maximum amount of time (in milliseconds) between events in order
        * for us to consider the user to be doing fast trackball movements,
        * and thus apply an acceleration.
        */
        static const Int64 FAST_MOVE_TIME = 150;

        /**
        * Scaling factor to the time (in milliseconds) between events to how
        * much to multiple/divide the current acceleration.  When movement
        * is < FAST_MOVE_TIME this multiplies the acceleration; when >
        * FAST_MOVE_TIME it divides it.
        */
        static const Float ACCEL_MOVE_SCALING_FACTOR = (1.0f/40);

        Float mPosition;
        Float mAbsPosition;
        Float mAcceleration;
        Int64 mLastMoveTime;
        Int32 mStep;
        Int32 mDir;
        Int32 mNonAccelMovement;
    };

    class CSurfaceHolder : public ElRefBase, public ISurfaceHolder
    {
    public:
        CSurfaceHolder(
            /* [in] */ ViewRootImpl* viewRoot);

        CARAPI_(PInterface) Probe(
            /* [in] */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        // we only need a SurfaceHolder for opengl. it would be nice
        // to implement everything else though, especially the callback
        // support (opengl doesn't make use of it right now, but eventually
        // will).
        CARAPI GetSurface(
            /* [out] */ ISurface** surface);

        CARAPI IsCreating(
            /* [out] */ Boolean* result);

        CARAPI AddCallback(
            /* [in] */ ISurfaceHolderCallback* cback);

        CARAPI RemoveCallback(
            /* [in] */ ISurfaceHolderCallback* cback);

        CARAPI SetFixedSize(
            /* [in] */ Int32 width,
            /* [in] */ Int32 height);

        CARAPI SetSizeFromLayout();

        CARAPI SetFormat(
            /* [in] */ Int32 format);

        CARAPI SetType(
            /* [in] */ Int32 type);

        CARAPI SetKeepScreenOn(
            /* [in] */ Boolean screenOn);

        CARAPI LockCanvas(
            /* [out] */ ICanvas** canvas);

        CARAPI LockCanvas(
            /* [in] */ IRect* dirty,
            /* [out] */ ICanvas** canvas);

        CARAPI UnlockCanvasAndPost(
            /* [in] */ ICanvas* canvas);

        CARAPI GetSurfaceFrame(
            /* [out] */ IRect** rect);

    private:
        ViewRootImpl* mViewRoot;
    };

public:
    /**
     * The run queue is used to enqueue pending work from Views when no Handler is
     * attached.  The work is executed during the next call to performTraversals on
     * the thread.
     * @hide
     */
    class RunQueue : public ElRefBase
    {
    public:
        CARAPI_(void) Post(
            /* [in] */ IRunnable* action);

        CARAPI_(void) PostDelayed(
            /* [in] */ IRunnable* action,
            /* [in] */ Int32 delayMillis);

        CARAPI_(void) RemoveCallbacks(
            /* [in] */ IRunnable* action);

        CARAPI_(void) ExecuteActions(
            /* [in] */ IHandler* apartment);

    private:
        class HandlerAction
            : public ElRefBase
        {
        public:
            HandlerAction()
                : mDelay(0)
            {}

            AutoPtr<IRunnable> mAction;
            Int32 mDelay;
        };

    private:
        List<AutoPtr<HandlerAction> > mActions;
        Object mLock;
    };

private:
    /**
     * Class for managing the accessibility interaction connection
     * based on the global accessibility state.
     */
    class AccessibilityInteractionConnectionManager
        : public ElRefBase
        , public IAccessibilityStateChangeListener
    {
    public:
        CAR_INTERFACE_DECL()

        AccessibilityInteractionConnectionManager(
            /* [in] */ ViewRootImpl* viewRootImpl);

        CARAPI OnAccessibilityStateChanged(
            /* [in] */ Boolean enabled);

        CARAPI EnsureConnection();

        CARAPI EnsureNoConnection();

    private:
        ViewRootImpl* mViewRootImpl;
    };

    class SendWindowContentChangedAccessibilityEvent
        : public Runnable
    {
    public:
        CARAPI Run();

    public:
        AutoPtr<IView> mSource;
    };

    class RenderProfileRunnable : public Runnable
    {
    private:
        class MyRunnable : public Runnable
        {
        public:
            MyRunnable(
                /* [in] */ IWeakReference* viewRootImpl);

            CARAPI Run();

        private:
            AutoPtr<IWeakReference> mWeakHost;
        };

    public:
        RenderProfileRunnable(
            /* [in] */ IWeakReference* viewRootImpl);

        CARAPI Run();

    private:
        AutoPtr<IWeakReference> mWeakHost;
    };

public:
    /**
    * Return TRUE if child is an ancestor of parent, (or equal to the parent).
    */
    static CARAPI_(Boolean) IsViewDescendantOf(
        /* [in] */ IView* child,
        /* [in] */ IView* parent);

    static CARAPI_(void) AddFirstDrawHandler(
        /* [in] */ IRunnable* callback);

    static CARAPI_(void) AddConfigCallback(
        /* [in] */ IComponentCallbacks* callback);

    /**
    * Indicates whether we are in touch mode. Calling this method triggers an IPC
    * call and should be avoided whenever possible.
    *
    * @return True, if the device is in touch mode, FALSE otherwise.
    *
    * @hide
    */
    static CARAPI_(Boolean) IsInTouchMode();

    static CARAPI_(AutoPtr<RunQueue>) GetRunQueue();

private:
     /**
     * @return True if the application requests the use of a separate render thread,
     *         FALSE otherwise
     */
    static Boolean IsRenderThreadRequested(
        /* [in] */ IContext* context);

    /**
    * Figures out the measure spec for the root view in a window based on it's
    * layout params.
    *
    * @param windowSize
    *            The available width or height of the window
    *
    * @param rootDimension
    *            The layout params for one dimension (width or height) of the
    *            window.
    *
    * @return The measure spec to use to measure the root view.
    */
    static CARAPI_(Int32) GetRootMeasureSpec(
        /* [in] */ Int32 windowSize,
        /* [in] */ Int32 rootDimension);

    static CARAPI_(void) ForceLayout(
        /* [in] */ IView* view);

    /**
    * Find an ancestor of focused that wants focus after its descendants and is
    * focusable in touch mode.
    * @param focused The currently focused view.
    * @return An appropriate view, or NULL if no such view exists.
    */
    static CARAPI_(AutoPtr<IViewGroup>) FindAncestorToTakeFocusInTouchMode(
        /* [in] */ IView* focused);

    static CARAPI_(Int32) JoystickAxisValueToDirection(
        /* [in] */ Float value);

    /**
     * Returns TRUE if the key is used for keyboard navigation.
     * @param keyEvent The key event->
     * @return True if the key is used for keyboard navigation.
     */
    static CARAPI_(Boolean) IsNavigationKey(
        /* [in] */ IKeyEvent* keyEvent);

    /**
     * Returns TRUE if the key is used for typing.
     * @param keyEvent The key event->
     * @return True if the key is used for typing.
     */
    static CARAPI_(Boolean) IsTypingKey(
        /* [in] */ IKeyEvent* keyEvent);

    static CARAPI_(void) GetGfxInfo(
        /* [in] */ IView* view,
        /* [in] */ ArrayOf<Int32>* info);

public:
    ViewRootImpl(
        /* [in] */ IContext* ctx,
        /* [in] */ IDisplay* display);

    ~ViewRootImpl();

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI GetInterfaceID(
        /* [in] */ IInterface *pObject,
        /* [out] */ InterfaceID *pIID);

    /**
    * Call this to profile the next traversal call.
    * FIXME for perf testing only. Remove eventually
    */
    CARAPI_(void) Profile();

    CARAPI SetView(
        /* [in] */ IView* view,
        /* [in] */ IWindowManagerLayoutParams* attrs,
        /* [in] */ IView* panelParentView);

    CARAPI_(Boolean) AttachFunctor(
        /* [in] */ Int32 functor);

    CARAPI_(void) DetachFunctor(
        /* [in] */ Int32 functor);

    CARAPI_(AutoPtr<IView>) GetView();

    CARAPI RequestFitSystemWindows();

    CARAPI RequestLayout();

    //in IViewParent
    CARAPI RequestLayout();

    CARAPI IsLayoutRequested(
        /* [out] */ Boolean* result);

    //in IViewParent
    CARAPI IsLayoutRequested(
        /* [out] */ Boolean* result);

    CARAPI InvalidateChild(
        /* [in] */ IView* child,
        /* [in] */ IRect* dirty);

    CARAPI InvalidateChildInParent(
        /* [in] */ ArrayOf<Int32>* location,
        /* [in] */ IRect* dirty,
        /* [out] */ IViewParent** parent);

    CARAPI GetParent(
        /* [out] */ IViewParent** parent);

    //in IViewParent
    CARAPI GetParent(
        /* [out] */ IViewParent** parent);

    CARAPI GetChildVisibleRect(
        /* [in] */ IView* child,
        /* [in] */ IRect* r,
        /* [in] */ IPoint* offset,
        /* [out] */ Boolean* result);

    CARAPI BringChildToFront(
        /* [in] */ IView* child);

    /**
     * Add LayoutTransition to the list of transitions to be started in the next traversal.
     * This list will be cleared after the transitions on the list are start()'ed. These
     * transitionsa re added by LayoutTransition itself when it sets up animations. The setup
     * happens during the layout phase of traversal, which we want to complete before any of the
     * animations are started (because those animations may side-effect properties that layout
     * depends upon, like the bounding rectangles of the affected views). So we add the transition
     * to the list and it is started just prior to starting the drawing phase of traversal.
     *
     * @param transition The LayoutTransition to be started on the next traversal.
     *
     * @hide
     */
    CARAPI_(void) RequestTransitionStart(
        /* [in] */ ILayoutTransition* transition);

    CARAPI RequestTransparentRegion(
        /* [in] */ IView* child);

    CARAPI_(void) OnHardwarePreDraw(
        /* [in] */ IHardwareCanvas* canvas);

    CARAPI_(void) OnHardwarePostDraw(
        /* [in] */ IHardwareCanvas* canvas);

    CARAPI_(AutoPtr<IAccessibilityNodeInfo>) GetAccessibilityFocusedVirtualView();

    CARAPI RequestChildFocus(
        /* [in] */ IView* child,
        /* [in] */ IView* focused);

    CARAPI ClearChildFocus(
        /* [in] */ IView* child);

    CARAPI GetParentForAccessibility(
        /* [out] */ IViewParent** parent);

    CARAPI FocusableViewAvailable(
        /* [in] */ IView* v);

    CARAPI RecomputeViewAttributes(
        /* [in] */ IView* child);

    CARAPI_(void) HandleDispatchSystemUiVisibilityChanged(
        /* [in] */ SystemUiVisibilityInfo* args);

    CARAPI_(void) HandleDispatchDoneAnimating();

    CARAPI_(void) GetLastTouchPoint(
        /* [in] */ IPoint* outLocation);

    CARAPI_(void) SetDragFocus(
        /* [in] */ IView* newDragTarget);

    CARAPI_(AutoPtr<IAccessibilityInteractionController>) GetAccessibilityInteractionController();

    CARAPI PlaySoundEffect(
        /* [in] */ Int32 effectId);

    CARAPI PerformHapticFeedback(
        /* [in] */ Int32 effectId,
        /* [in] */ Boolean always,
        /* [out] */ Boolean* result);

    CARAPI FocusSearch(
        /* [in] */ IView* focused,
        /* [in] */ Int32 direction,
        /* [out */ IView** result);

    //in IViewParent
    CARAPI FocusSearch(
        /* [in] */ IView* focused,
        /* [in] */ Int32 direction,
        /* [out */ IView** result);

    CARAPI_(void) Debug();

    CARAPI_(void) DumpGfxInfo(
        /* [in] */ ArrayOf<Int32>* info);

    CARAPI Die(
        /* [in] */ Boolean immediate);

    CARAPI_(void) RequestUpdateConfiguration(
        /* [in] */ IConfiguration* config);

    CARAPI_(void) LoadSystemProperties();

    CARAPI_(void) DispatchFinishInputConnection(
        /* [in] */ IInputConnection* connection);

    CARAPI_(void) DispatchResized(
        /* [in] */ IRect* frame,
        /* [in] */ IRect* coveredInsets,
        /* [in] */ IRect* visibleInsets,
        /* [in] */ Boolean reportDraw,
        /* [in] */ IConfiguration* newConfig);

    CARAPI_(void) DispatchMoved(
        /* [in] */ Int32 newX,
        /* [in] */ Int32 newY);

    CARAPI_(void) DispatchInvalidateDelayed(
        /* [in] */ IView* view,
        /* [in] */ Int64 delayMilliseconds);

    CARAPI_(void) DispatchInvalidateRectDelayed(
        /* [in] */ View::AttachInfo::InvalidateInfo* info,
        /* [in] */ Int64 delayMilliseconds);

    CARAPI_(void) DispatchInvalidateOnAnimation(
        /* [in] */ IView* view);

    CARAPI_(void) DispatchInvalidateRectOnAnimation(
        /* [in] */ View::AttachInfo::InvalidateInfo* info);

    CARAPI_(void) EnqueueDisplayList(
        /* [in] */ IDisplayList* displayList);

    CARAPI_(void) DequeueDisplayList(
        /* [in] */ IDisplayList* displayList);

    CARAPI_(void) CancelInvalidate(
        /* [in] */ IView* view);

    CARAPI_(void) DispatchKey(
        /* [in] */ IKeyEvent* event);

    CARAPI_(void) DispatchKeyFromIme(
        /* [in] */ IKeyEvent* event);

    CARAPI_(void) DispatchUnhandledKey(
        /* [in] */ IKeyEvent* event);

    CARAPI_(void) DispatchAppVisibility(
        /* [in] */ Boolean visible);

    CARAPI_(void) DispatchScreenStateChange(
        /* [ijn] */ Boolean on);

    CARAPI_(void) DispatchGetNewSurface();

    CARAPI_(void) WindowFocusChanged(
        /* [in] */ Boolean hasFocus,
        /* [in] */ Boolean inTouchMode);

    CARAPI_(void) DispatchCloseSystemDialogs(
        /* [in] */ const String& reason);

    CARAPI_(void) DispatchDragEvent(
        /* [in] */ IDragEvent* event);

    CARAPI_(void) DispatchSystemUiVisibilityChanged(
        /* [in] */ Int32 seq,
        /* [in] */ Int32 globalVisibility,
        /* [in] */ Int32 localValue,
        /* [in] */ Int32 localChanges);

    CARAPI_(void) DispatchDoneAnimating();

    CARAPI_(void) DispatchCheckFocus();

    CARAPI StartActionModeForChild(
        /* [in] */ IView* originalView,
        /* [in] */ IActionModeCallback* callback,
        /* [out] */ IActionMode** actionMode);

    CARAPI ShowContextMenuForChild(
        /* [in] */ IView* originalView,
        /* [out] */ Boolean* result);

    CARAPI CreateContextMenu(
        /* [in] */ IContextMenu* menu);

    CARAPI_(void) CreateContextMenu(
        /* [in] */ IContextMenu* menu);

    CARAPI ChildDrawableStateChanged(
        /* [in] */ IView* child);

    CARAPI RequestSendAccessibilityEvent(
        /* [in] */ IView* child,
        /* [in] */ IAccessibilityEvent* event,
        /* [out] */ Boolean* res);

    CARAPI ChildAccessibilityStateChanged(
        /* [in] */ IView* child);

    CARAPI_(CARAPI) ChildHasTransientStateChanged(
        /* [in] */ IView* child,
        /* [in] */ Boolean hasTransientState);

    CARAPI RequestDisallowInterceptTouchEvent(
        /* [in] */ Boolean disallowIntercept);

    CARAPI RequestChildRectangleOnScreen(
        /* [in] */ IView* child,
        /* [in] */ IRect* rectangle,
        /* [in] */ Boolean immediate,
        /* [out] */ Boolean* result);

    CARAPI_(AutoPtr<IView>) GetAccessibilityFocusedHost();

    CARAPI GetWeakReference(
        /* [out] */ IWeakReference** weakReference);

protected:
    CARAPI_(void) DestroyHardwareResources();

    CARAPI_(void) TerminateHardwareResources();

    CARAPI_(void) DestroyHardwareLayers();

    CARAPI_(void) PushHardwareLayerUpdate(
        /* [in] */ IHardwareLayer* layer);

    CARAPI_(void) SetLayoutParams(
        /* [in] */ IWindowManagerLayoutParams* attrs,
        /* [in] */ Boolean newView);

    CARAPI_(void) HandleAppVisibility(
        /* [in] */ Boolean visible);

    CARAPI_(void) HandleGetNewSurface();

    CARAPI_(void) HandleScreenStateChange(
        /* [in] */ Boolean on);

    CARAPI_(void) Invalidate();

    CARAPI_(void) InvalidateWorld(
        /* [in] */ IView* view);

    CARAPI_(void) SetStopped(
        /* [in] */ Boolean stopped);

    CARAPI_(Int32) GetHostVisibility();

    CARAPI_(void) DisposeResizeBuffer();

    CARAPI_(void) ScheduleTraversals();

    CARAPI_(void) UnscheduleTraversals();

    CARAPI_(void) DoTraversal();

    CARAPI_(Boolean) ScrollToRectOrFocus(
        /* [in] */ IRect* rectangle,
        /* [in] */ Boolean immediate);

    CARAPI_(void) OutputDisplayList(
        /* [in] */ IView* view);

    /**
     * @see #PROPERTY_PROFILE_RENDERING
     */
    CARAPI_(void) ProfileRendering(
        /* [in] */ Boolean enabled);

    CARAPI_(void) PerformDraw();

    /**
     * Called from draw() when DEBUG_FPS is enabled
     */
    CARAPI_(void) TrackFPS();

    CARAPI_(void) InvalidateDisplayLists();

    CARAPI_(void) SetAccessibilityFocus(
        /* [in] */ IView* view,
        /* [in] */ IAccessibilityNodeInfo* node);

    CARAPI_(void) DispatchDetachedFromWindow();

    CARAPI_(void) UpdateConfiguration(
        /* [in] */ IConfiguration* config,
        /* [in] */ Boolean force);

    /**
    * Something in the current window tells us we need to change the touch mode.  For
    * example, we are not in touch mode, and the user touches the screen.
    *
    * If the touch mode has changed, tell the window manager, and handle it locally.
    *
    * @param inTouchMode Whether we want to be in touch mode.
    * @return True if the touch mode changed and focus changed was changed as a result
    */
    CARAPI_(Boolean) EnsureTouchMode(
        /* [in] */ Boolean inTouchMode);

    CARAPI_(void) HandleImeFinishedEvent(
        /* [in] */ Int32 seq,
        /* [in] */ Boolean handled);

    CARAPI_(void) SetLocalDragState(
        /* [in] */ IInterface* obj);

    CARAPI_(void) EnqueueInputEvent(
        /* [in] */ IInputEvent* event);

    CARAPI_(void) EnqueueInputEvent(
        /* [in] */ IInputEvent* event,
        /* [in] */ InputEventReceiver* receiver,
        /* [in] */ Int32 flags,
        /* [in] */ Boolean processImmediately);

    CARAPI_(void) DoProcessInputEvents();

    CARAPI_(void) ScheduleConsumeBatchedInput();

    CARAPI_(void) UnscheduleConsumeBatchedInput();

    CARAPI_(void) DoConsumeBatchedInput(
        /* [in] */ Int64 frameTimeNanos);

    CARAPI DoDie();

    CARAPI_(void) DispatchImeFinishedEvent(
        /* [in] */ Int32 seq,
        /* [in] */ Boolean handled);

    CARAPI CheckThread();

private:
    CARAPI_(void) EnableHardwareAcceleration(
        /* [in] */ IContext* context,
        /* [in] */ IWindowManagerLayoutParams* attrs);

    CARAPI_(void) ApplyKeepScreenOnFlag(
        /* [in] */ CWindowManagerLayoutParams* params);

    CARAPI_(Boolean) CollectViewAttributes();

    CARAPI_(Boolean) MeasureHierarchy(
        /* [in] */ View* host,
        /* [in] */ CWindowManagerLayoutParams* lp,
        /* [in] */ IResources* res,
        /* [in] */ Int32 desiredWindowWidth,
        /* [in] */ Int32 desiredWindowHeight);

    CARAPI_(void) PerformTraversals();

    CARAPI_(void) PerformMeasure(
        /* [in] */ Int32 childWidthMeasureSpec,
        /* [in] */ Int32 childHeightMeasureSpec);

    CARAPI_(void) PerformLayout();

    CARAPI_(void) Draw(
        /* [in] */ Boolean fullRedrawNeeded);

    /**
     * @return TRUE if drawing was succesfull, FALSE if an error occurred
     */
    CARAPI_(Boolean) DrawSoftware(
        /* [in] */ ISurface* surface,
        /* [in] */ View::AttachInfo* attachInfo,
        /* [in] */ Int32 yoff,
        /* [in] */ Boolean scalingRequired,
        /* [in] */ CRect* dirty);

    /**
     * We want to draw a highlight around the current accessibility focused.
     * Since adding a style for all possible view is not a viable option we
     * have this specialized drawing method.
     *
     * Note: We are doing this here to be able to draw the highlight for
     *       virtual views in addition to real ones.
     *
     * @param canvas The canvas on which to draw.
     */
    CARAPI_(void) DrawAccessibilityFocusedDrawableIfNeeded(
        /* [in] */ ICanvas* canvas);

    CARAPI_(AutoPtr<IDrawable>) GetAccessibilityFocusedDrawable();

    /**
    * Ensure that the touch mode for this window is set, and if it is changing,
    * take the appropriate action.
    * @param inTouchMode Whether we want to be in touch mode.
    * @return True if the touch mode changed and focus changed was changed as a result
    */
    CARAPI_(Boolean) EnsureTouchModeLocally(
        /* [in] */ Boolean inTouchMode);

    CARAPI_(Boolean) EnterTouchMode();

    CARAPI_(Boolean) LeaveTouchMode();

    CARAPI_(void) DeliverInputEvent(
        /* [in] */ QueuedInputEvent* q);

    CARAPI_(void) DeliverPointerEvent(
        /* [in] */ QueuedInputEvent* q);

    CARAPI_(void) DeliverTrackballEvent(
        /* [in] */ QueuedInputEvent* q);

    CARAPI_(void) DeliverTrackballEventPostIme(
        /* [in] */ QueuedInputEvent* q);

    CARAPI_(void) DeliverGenericMotionEvent(
        /* [in] */ QueuedInputEvent* q);

    CARAPI_(void) DeliverGenericMotionEventPostIme(
        /* [in] */ QueuedInputEvent* q);

    CARAPI_(void) UpdateJoystickDirection(
        /* [in] */ IMotionEvent* event,
        /* [in] */ Boolean synthesizeNewKeys);

    /**
    * See if the key event means we should leave touch mode (and leave touch
    * mode if so).
    * @param event The key event.
    * @return Whether this key event should be consumed (meaning the act of
    *   leaving touch mode alone is considered the event).
    */
    CARAPI_(Boolean) CheckForLeavingTouchModeAndConsume(
        /* [in] */ IKeyEvent* event);

    CARAPI_(void) DeliverKeyEvent(
        /* [in] */ QueuedInputEvent* q);

    CARAPI_(void) DeliverKeyEventPostIme(
        /* [in] */ QueuedInputEvent* q);

    CARAPI_(void) HandleDragEvent(
        /* [in] */ IDragEvent* event);

    CARAPI_(AutoPtr<IAudioManager>) GetAudioManager();

    CARAPI_(Int32) RelayoutWindow(
        /* [in] */ CWindowManagerLayoutParams* params,
        /* [in] */ Int32 viewVisibility,
        /* [in] */ Boolean insetsPending);

    CARAPI_(void) DestroyHardwareRenderer();

    CARAPI_(AutoPtr<QueuedInputEvent>) ObtainQueuedInputEvent(
        /* [in] */ IInputEvent* event,
        /* [in] */ InputEventReceiver* receiver,
        /* [in] */ Int32 flags);

    CARAPI_(void) RecycleQueuedInputEvent(
        /* [in] */ QueuedInputEvent* q);

    CARAPI_(void) ScheduleProcessInputEvents();

    CARAPI FinishInputEvent(
        /* [in] */ QueuedInputEvent* q,
        /* [in] */ Boolean handled);

    /**
     * Post a callback to send a
     * {@link AccessibilityEvent#TYPE_WINDOW_CONTENT_CHANGED} event->
     * This event is send at most once every
     * {@link ViewConfiguration#getSendRecurringAccessibilityEventsInterval()}.
     */
    CARAPI_(void) PostSendWindowContentChangedCallback(
        /* [in] */ IView* source);

    /**
     * Remove a posted callback to send a
     * {@link AccessibilityEvent#TYPE_WINDOW_CONTENT_CHANGED} event->
     */
    CARAPI_(void) RemoveSendWindowContentChangedCallback();

    CARAPI_(AutoPtr<IView>) GetCommonPredecessor(
        /* [in] */ IView* first,
        /* [in] */ IView* second);

    CARAPI_(void) HandleWindowFocusChanged(
        /* [in] */ Boolean hasWindowFocus,
        /* [in] */ Boolean inTouchMode);

    CARAPI_(void) LogView(
        /* [in] */ const String& info = String(""));

public:
    AutoPtr<IWindowSession> mWindowSession;
    AutoPtr<IDisplay> mDisplay;

    Int32 mLastTrackballTime;
    AutoPtr<TrackballAxis> mTrackballAxisX;
    AutoPtr<TrackballAxis> mTrackballAxisY;

    Int32 mLastJoystickXDirection;
    Int32 mLastJoystickYDirection;
    Int32 mLastJoystickXKeyCode;
    Int32 mLastJoystickYKeyCode;

    Int32 mTmpLocation[2];

    AutoPtr<ITypedValue> mTmpValue;

    AutoPtr<InputMethodCallback> mInputMethodCallback;

    Int32 mThreadID; // final Thread mThread;

    // final WindowLeaked mLocation;

    AutoPtr<CWindowManagerLayoutParams> mWindowAttributes;

    AutoPtr<IIWindow> mWindow;

    Int32 mTargetSdkVersion;

    Int32 mSeq;

    AutoPtr<IView> mView;
    AutoPtr<IView> mFocusedView;
    AutoPtr<IView> mRealFocusedView;  // this is not set to null in touch mode
    AutoPtr<IView> mOldFocusedView;

    AutoPtr<IView> mAccessibilityFocusedHost;
    AutoPtr<IAccessibilityNodeInfo> mAccessibilityFocusedVirtualView;

    Int32 mViewVisibility;
    Boolean mAppVisible;
    Int32 mOrigWindowType;

    // Set to true if the owner of this window is in the stopped state,
    // so the window should no longer be active.
    Boolean mStopped;

    Boolean mLastInCompatMode;

    AutoPtr<ISurfaceHolderCallback2> mSurfaceHolderCallback;
    AutoPtr<BaseSurfaceHolder> mSurfaceHolder;
    Boolean mIsCreating;
    Boolean mDrawingAllowed;

    AutoPtr<CRegion> mTransparentRegion;
    AutoPtr<CRegion> mPreviousTransparentRegion;

    Int32 mWidth;
    Int32 mHeight;
    AutoPtr<CRect> mDirty;    // will be a graphics.Region soon
    AutoPtr<CRect> mCurrentDirty;
    AutoPtr<CRect> mPreviousDirty;
    Boolean mIsAnimating;

    AutoPtr<ICompatibilityInfoTranslator> mTranslator;

    AutoPtr<View::AttachInfo> mAttachInfo;
    AutoPtr<IInputChannel> mInputChannel;
    AutoPtr<IInputQueueCallback> mInputQueueCallback;
    AutoPtr<IInputQueue> mInputQueue;
    AutoPtr<IFallbackEventHandler> mFallbackEventHandler;
    AutoPtr<IChoreographer> mChoreographer;

    AutoPtr<CRect> mTempRect; // used in the transaction to not thrash the heap.
    AutoPtr<CRect> mVisRect; // used to retrieve visible rect of focused view.

    Boolean mTraversalScheduled;
    Int32 mTraversalBarrier;
    Boolean mWillDrawSoon;
    /** Set to true while in performTraversals for detecting when die(true) is called from internal
     * callbacks such as onMeasure, onPreDraw, onDraw and deferring doDie() until later. */
    Boolean mIsInTraversal;
    Boolean mFitSystemWindowsRequested;
    Boolean mLayoutRequested;
    Boolean mFirst;
    Boolean mReportNextDraw;
    Boolean mFullRedrawNeeded;
    Boolean mNewSurfaceNeeded;
    Boolean mHasHadWindowFocus;
    Boolean mLastWasImTarget;
    Boolean mWindowsAnimating;
    Boolean mIsDrawing;
    Int32 mLastSystemUiVisibility;
    Int32 mClientWindowLayoutFlags;

    // Input event queue.
    AutoPtr<QueuedInputEvent> mFirstPendingInputEvent;
    AutoPtr<QueuedInputEvent> mCurrentInputEvent;
    Boolean mProcessInputEventsScheduled;

    Boolean mWindowAttributesChanged;
    Int32 mWindowAttributesChangesFlag;

    Boolean mAdded;
    Boolean mAddedTouchMode;

    AutoPtr<ICompatibilityInfoHolder> mCompatibilityInfo;

    // These are accessed by multiple threads.
    AutoPtr<CRect> mWinFrame; // frame given by window manager.

    AutoPtr<CRect> mPendingVisibleInsets;
    AutoPtr<CRect> mPendingContentInsets;

    AutoPtr<IInternalInsetsInfo> mLastGivenInsets;

    AutoPtr<CRect> mFitSystemWindowsInsets;

    AutoPtr<IConfiguration> mLastConfiguration;
    AutoPtr<IConfiguration> mPendingConfiguration;

    Boolean mScrollMayChange;
    Int32 mSoftInputMode;
    AutoPtr<IView> mLastScrolledFocus;
    Int32 mScrollY;
    Int32 mCurScrollY;
    AutoPtr<CScroller> mScroller;

    AutoPtr<IHardwareLayer> mResizeBuffer;
    Int64 mResizeBufferStartTime;
    Int32 mResizeBufferDuration;

    AutoPtr<CViewConfiguration> mViewConfiguration;

    /* Drag/drop */
    AutoPtr<IClipDescription> mDragDescription;
    AutoPtr<IView> mCurrentDragView;
    /*volatile*/ AutoPtr<IInterface> mLocalDragState;
    AutoPtr<IPointF> mDragPoint;
    AutoPtr<IPointF> mLastTouchPoint;

    /**
    * see {@link #playSoundEffect(Int32)}
    */
    AutoPtr<IAudioManager> mAudioManager;

    AutoPtr<IAccessibilityManager> mAccessibilityManager;

    AutoPtr<IAccessibilityInteractionController> mAccessibilityInteractionController;

    AutoPtr<AccessibilityInteractionConnectionManager> mAccessibilityInteractionConnectionManager;

    AutoPtr<SendWindowContentChangedAccessibilityEvent> mSendWindowContentChangedAccessibilityEvent;

    HashSet<AutoPtr<IView> > mTempHashSet;

    Int32 mHardwareYOffset;
    Int32 mResizeAlpha;
    AutoPtr<IPaint> mResizePaint;

    AutoPtr<TraversalRunnable> mTraversalRunnable;

    AutoPtr<WindowInputEventReceiver> mInputEventReceiver;


    AutoPtr<ConsumeBatchedInputRunnable> mConsumedBatchedInputRunnable;
    Boolean mConsumeBatchedInputScheduled;

    AutoPtr<InvalidateOnAnimationRunnable> mInvalidateOnAnimationRunnable;

protected:
    AutoPtr<InputEventConsistencyVerifier> mInputEventConsistencyVerifier;

private:
    friend class CAccessibilityInteractionController;

    AutoPtr<QueuedInputEvent> mQueuedInputEventPool;
    Int32 mQueuedInputEventPoolSize;

    // These can be accessed by any thread, must be protected with a lock.
    // Surface can never be reassigned or cleared (use Surface.clear()).
    AutoPtr<CSurface> mSurface;

    List<AutoPtr<ILayoutTransition> > mPendingTransitions;

    Boolean mProfileRendering;
    AutoPtr<IThread> mRenderProfiler;
    volatile Boolean mRenderProfilingEnabled;

    // Variables to track frames per second, enabled via DEBUG_FPS flag
    Int64 mFpsStartTime;
    Int64 mFpsPrevTime;
    Int32 mFpsNumFrames;

    List<AutoPtr<IDisplayList> > mDisplayLists;

    Int32 mDensity;
    Int32 mNoncompatDensity;

    Int32 mViewLayoutDirectionInitial;

    // FIXME for perf testing only
    Boolean mProfile;

    AutoPtr<CSurfaceHolder> mHolder;

    AutoPtr<ViewRootHandler> mHandler;
    Object mSyncLock;
};

} // namespace View
} // namespace Droid
} // namespace Elastos

template <>
struct Conversion<Elastos::Droid::View::ViewRootImpl*, IInterface*>
{
    enum { exists = TRUE, exists2Way = FALSE, sameType = FALSE };
};

#endif // __ELASTOS_DROID_VIEW_VIEWROOTIMPL_H__
