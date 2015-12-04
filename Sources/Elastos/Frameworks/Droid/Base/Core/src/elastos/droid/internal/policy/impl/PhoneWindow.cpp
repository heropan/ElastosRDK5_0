
#include "elastos/droid/internal/policy/impl/PhoneWindow.h"
#include "elastos/droid/internal/policy/impl/CPhoneWindowRotationWatcher.h"
#include "elastos/droid/transition/CTransitionSet.h"

using Elastos::Droid::Internal::View::EIID_IRootViewSurfaceTaker;
using Elastos::Droid::Internal::View::Menu::EIID_IMenuBuilderCallback;
using Elastos::Droid::Internal::View::Menu::EIID_IMenuPresenterCallback;
using Elastos::Droid::Internal::Widget::EIID_IOnDismissedListener;
using Elastos::Droid::Internal::Widget::EIID_IOnSwipeProgressChangedListener;
using Elastos::Droid::View::EIID_IActionModeCallback;
using Elastos::Droid::Transition::CTransitionSet;
using Elastos::Core::EIID_IRunnable;

namespace Elastos {
namespace Droid {
namespace Internal {
namespace Policy {
namespace Impl {

//=====================================================================
//                   PhoneWindow::WindowManagerHolder
//=====================================================================
AutoPtr<IIWindowManager> PhoneWindow::WindowManagerHolder::sWindowManager = PhoneWindow::WindowManagerHolder::MiddleInitSwindowmanager();

AutoPtr<IIWindowManager> PhoneWindow::WindowManagerHolder::MiddleInitSwindowmanager()
{
    // ==================before translated======================
    // ->WWZ_SIGN: FUNC_CALL_START {
    // ServiceManager.getService("window")
    // ->WWZ_SIGN: FUNC_CALL_END }
    assert(0);
    AutoPtr<IIWindowManager> empty;
    return empty;
}

//=====================================================================
//               PhoneWindow::ActionMenuPresenterCallback
//=====================================================================
CAR_INTERFACE_IMPL(PhoneWindow::ActionMenuPresenterCallback, Object, IMenuPresenterCallback)

PhoneWindow::ActionMenuPresenterCallback::ActionMenuPresenterCallback(
    /* [in] */ PhoneWindow* owner)
    : mOwner(owner)
{
}

ECode PhoneWindow::ActionMenuPresenterCallback::OnOpenSubMenu(
    /* [in] */ IMenuBuilder* subMenu,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // Callback cb = getCallback();
    // if (cb != null) {
    //     cb.onMenuOpened(FEATURE_ACTION_BAR, subMenu);
    //     return true;
    // }
    // return false;
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::ActionMenuPresenterCallback::OnCloseMenu(
    /* [in] */ IMenuBuilder* menu,
    /* [in] */ Boolean allMenusAreClosing)
{
    VALIDATE_NOT_NULL(menu);
    // ==================before translated======================
    // checkCloseActionMenu(menu);
    assert(0);
    return NOERROR;
}

//=====================================================================
//          PhoneWindow::DecorView::ActionModeCallbackWrapper
//=====================================================================
CAR_INTERFACE_IMPL(PhoneWindow::DecorView, FrameLayout, IRootViewSurfaceTaker)

CAR_INTERFACE_IMPL(PhoneWindow::DecorView::ActionModeCallbackWrapper, Object, IActionModeCallback)

PhoneWindow::DecorView::ActionModeCallbackWrapper::ActionModeCallbackWrapper(
    /* [in] */ DecorView* owner,
    /* [in] */ IActionModeCallback* wrapped)
{
    // ==================before translated======================
    mOwner = owner;
    mWrapped = wrapped;
}

ECode PhoneWindow::DecorView::ActionModeCallbackWrapper::OnCreateActionMode(
    /* [in] */ IActionMode* mode,
    /* [in] */ IMenu* menu,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(mode);
    VALIDATE_NOT_NULL(menu);
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return mWrapped.onCreateActionMode(mode, menu);
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::DecorView::ActionModeCallbackWrapper::OnPrepareActionMode(
    /* [in] */ IActionMode* mode,
    /* [in] */ IMenu* menu,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(mode);
    VALIDATE_NOT_NULL(menu);
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // requestFitSystemWindows();
    // return mWrapped.onPrepareActionMode(mode, menu);
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::DecorView::ActionModeCallbackWrapper::OnActionItemClicked(
    /* [in] */ IActionMode* mode,
    /* [in] */ IMenuItem* item,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(mode);
    VALIDATE_NOT_NULL(item);
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return mWrapped.onActionItemClicked(mode, item);
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::DecorView::ActionModeCallbackWrapper::OnDestroyActionMode(
    /* [in] */ IActionMode* mode)
{
    VALIDATE_NOT_NULL(mode);
    // ==================before translated======================
    // mWrapped.onDestroyActionMode(mode);
    // if (mActionModePopup != null) {
    //     removeCallbacks(mShowActionModePopup);
    //     mActionModePopup.dismiss();
    // } else if (mActionModeView != null) {
    //     mActionModeView.setVisibility(GONE);
    // }
    // if (mActionModeView != null) {
    //     mActionModeView.removeAllViews();
    // }
    // if (getCallback() != null && !isDestroyed()) {
    //     try {
    //         getCallback().onActionModeFinished(mActionMode);
    //     } catch (AbstractMethodError ame) {
    //         // Older apps might not implement this callback method.
    //     }
    // }
    // mActionMode = null;
    // requestFitSystemWindows();
    assert(0);
    return NOERROR;
}

//=====================================================================
//                        PhoneWindow::DecorView
//=====================================================================
PhoneWindow::DecorView::DecorView(
    /* [in] */ PhoneWindow* owner,
    /* [in] */ IContext* context,
    /* [in] */ Int32 featureId)
{
    mOwner = owner;
    // ==================before translated======================
    // super(context);
    // mFeatureId = featureId;
}

ECode PhoneWindow::DecorView::SetBackgroundFallback(
    /* [in] */ Int32 resId)
{
    // ==================before translated======================
    // mBackgroundFallback.setDrawable(resId != 0 ? getContext().getDrawable(resId) : null);
    // setWillNotDraw(getBackground() == null && !mBackgroundFallback.hasFallback());
    assert(0);
    return NOERROR;
}

void PhoneWindow::DecorView::OnDraw(
    /* [in] */ ICanvas* c)
{
    // ==================before translated======================
    // super.onDraw(c);
    // mBackgroundFallback.draw(mContentRoot, c, mContentParent);
    assert(0);
}

ECode PhoneWindow::DecorView::DispatchKeyEvent(
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // final int keyCode = event.getKeyCode();
    // final int action = event.getAction();
    // final boolean isDown = action == KeyEvent.ACTION_DOWN;
    //
    // if (isDown && (event.getRepeatCount() == 0)) {
    //     // First handle chording of panel key: if a panel key is held
    //     // but not released, try to execute a shortcut in it.
    //     if ((mPanelChordingKey > 0) && (mPanelChordingKey != keyCode)) {
    //         boolean handled = dispatchKeyShortcutEvent(event);
    //         if (handled) {
    //             return true;
    //         }
    //     }
    //
    //     // If a panel is open, perform a shortcut on it without the
    //     // chorded panel key
    //     if ((mPreparedPanel != null) && mPreparedPanel.isOpen) {
    //         if (performPanelShortcut(mPreparedPanel, keyCode, event, 0)) {
    //             return true;
    //         }
    //     }
    // }
    //
    // if (!isDestroyed()) {
    //     final Callback cb = getCallback();
    //     final boolean handled = cb != null && mFeatureId < 0 ? cb.dispatchKeyEvent(event)
    //             : super.dispatchKeyEvent(event);
    //     if (handled) {
    //         return true;
    //     }
    // }
    //
    // return isDown ? PhoneWindow.this.onKeyDown(mFeatureId, event.getKeyCode(), event)
    //         : PhoneWindow.this.onKeyUp(mFeatureId, event.getKeyCode(), event);
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::DecorView::DispatchKeyShortcutEvent(
    /* [in] */ IKeyEvent* ev,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // // If the panel is already prepared, then perform the shortcut using it.
    // boolean handled;
    // if (mPreparedPanel != null) {
    //     handled = performPanelShortcut(mPreparedPanel, ev.getKeyCode(), ev,
    //             Menu.FLAG_PERFORM_NO_CLOSE);
    //     if (handled) {
    //         if (mPreparedPanel != null) {
    //             mPreparedPanel.isHandled = true;
    //         }
    //         return true;
    //     }
    // }
    //
    // // Shortcut not handled by the panel.  Dispatch to the view hierarchy.
    // final Callback cb = getCallback();
    // handled = cb != null && !isDestroyed() && mFeatureId < 0
    //         ? cb.dispatchKeyShortcutEvent(ev) : super.dispatchKeyShortcutEvent(ev);
    // if (handled) {
    //     return true;
    // }
    //
    // // If the panel is not prepared, then we may be trying to handle a shortcut key
    // // combination such as Control+C.  Temporarily prepare the panel then mark it
    // // unprepared again when finished to ensure that the panel will again be prepared
    // // the next time it is shown for real.
    // if (mPreparedPanel == null) {
    //     PanelFeatureState st = getPanelState(FEATURE_OPTIONS_PANEL, true);
    //     preparePanel(st, ev);
    //     handled = performPanelShortcut(st, ev.getKeyCode(), ev,
    //             Menu.FLAG_PERFORM_NO_CLOSE);
    //     st.isPrepared = false;
    //     if (handled) {
    //         return true;
    //     }
    // }
    // return false;
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::DecorView::DispatchTouchEvent(
    /* [in] */ IMotionEvent* ev,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // final Callback cb = getCallback();
    // return cb != null && !isDestroyed() && mFeatureId < 0 ? cb.dispatchTouchEvent(ev)
    //         : super.dispatchTouchEvent(ev);
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::DecorView::DispatchTrackballEvent(
    /* [in] */ IMotionEvent* ev,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // final Callback cb = getCallback();
    // return cb != null && !isDestroyed() && mFeatureId < 0 ? cb.dispatchTrackballEvent(ev)
    //         : super.dispatchTrackballEvent(ev);
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::DecorView::DispatchGenericMotionEvent(
    /* [in] */ IMotionEvent* ev,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // final Callback cb = getCallback();
    // return cb != null && !isDestroyed() && mFeatureId < 0 ? cb.dispatchGenericMotionEvent(ev)
    //         : super.dispatchGenericMotionEvent(ev);
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::DecorView::SuperDispatchKeyEvent(
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(event);
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // // Give priority to closing action modes if applicable.
    // if (event.getKeyCode() == KeyEvent.KEYCODE_BACK) {
    //     final int action = event.getAction();
    //     // Back cancels action modes first.
    //     if (mActionMode != null) {
    //         if (action == KeyEvent.ACTION_UP) {
    //             mActionMode.finish();
    //         }
    //         return true;
    //     }
    // }
    //
    // return super.dispatchKeyEvent(event);
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::DecorView::SuperDispatchKeyShortcutEvent(
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(event);
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return super.dispatchKeyShortcutEvent(event);
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::DecorView::SuperDispatchTouchEvent(
    /* [in] */ IMotionEvent* event,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(event);
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return super.dispatchTouchEvent(event);
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::DecorView::SuperDispatchTrackballEvent(
    /* [in] */ IMotionEvent* event,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(event);
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return super.dispatchTrackballEvent(event);
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::DecorView::SuperDispatchGenericMotionEvent(
    /* [in] */ IMotionEvent* event,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(event);
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return super.dispatchGenericMotionEvent(event);
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::DecorView::DispatchApplyWindowInsets(
    /* [in] */ IWindowInsets* insets,
    /* [out] */ IWindowInsets** result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // if (mOutsetBottom != null) {
    //     final DisplayMetrics metrics = getContext().getResources().getDisplayMetrics();
    //     int bottom = (int) mOutsetBottom.getDimension(metrics);
    //     WindowInsets newInsets = insets.replaceSystemWindowInsets(
    //             insets.getSystemWindowInsetLeft(), insets.getSystemWindowInsetTop(),
    //             insets.getSystemWindowInsetRight(), bottom);
    //     return super.dispatchApplyWindowInsets(newInsets);
    // } else {
    //     return super.dispatchApplyWindowInsets(insets);
    // }
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::DecorView::OnTouchEvent(
    /* [in] */ IMotionEvent* event,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return onInterceptTouchEvent(event);
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::DecorView::OnInterceptTouchEvent(
    /* [in] */ IMotionEvent* event,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // int action = event.getAction();
    // if (mFeatureId >= 0) {
    //     if (action == MotionEvent.ACTION_DOWN) {
    //         int x = (int)event.getX();
    //         int y = (int)event.getY();
    //         if (isOutOfBounds(x, y)) {
    //             closePanel(mFeatureId);
    //             return true;
    //         }
    //     }
    // }
    //
    // if (!SWEEP_OPEN_MENU) {
    //     return false;
    // }
    //
    // if (mFeatureId >= 0) {
    //     if (action == MotionEvent.ACTION_DOWN) {
    //         Log.i(TAG, "Watchiing!");
    //         mWatchingForMenu = true;
    //         mDownY = (int) event.getY();
    //         return false;
    //     }
    //
    //     if (!mWatchingForMenu) {
    //         return false;
    //     }
    //
    //     int y = (int)event.getY();
    //     if (action == MotionEvent.ACTION_MOVE) {
    //         if (y > (mDownY+30)) {
    //             Log.i(TAG, "Closing!");
    //             closePanel(mFeatureId);
    //             mWatchingForMenu = false;
    //             return true;
    //         }
    //     } else if (action == MotionEvent.ACTION_UP) {
    //         mWatchingForMenu = false;
    //     }
    //
    //     return false;
    // }
    //
    // //Log.i(TAG, "Intercept: action=" + action + " y=" + event.getY()
    // //        + " (in " + getHeight() + ")");
    //
    // if (action == MotionEvent.ACTION_DOWN) {
    //     int y = (int)event.getY();
    //     if (y >= (getHeight()-5) && !hasChildren()) {
    //         Log.i(TAG, "Watchiing!");
    //         mWatchingForMenu = true;
    //     }
    //     return false;
    // }
    //
    // if (!mWatchingForMenu) {
    //     return false;
    // }
    //
    // int y = (int)event.getY();
    // if (action == MotionEvent.ACTION_MOVE) {
    //     if (y < (getHeight()-30)) {
    //         Log.i(TAG, "Opening!");
    //         openPanel(FEATURE_OPTIONS_PANEL, new KeyEvent(
    //                 KeyEvent.ACTION_DOWN, KeyEvent.KEYCODE_MENU));
    //         mWatchingForMenu = false;
    //         return true;
    //     }
    // } else if (action == MotionEvent.ACTION_UP) {
    //     mWatchingForMenu = false;
    // }
    //
    // return false;
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::DecorView::SendAccessibilityEvent(
    /* [in] */ Int32 eventType)
{
    // ==================before translated======================
    // if (!AccessibilityManager.getInstance(mContext).isEnabled()) {
    //     return;
    // }
    //
    // // if we are showing a feature that should be announced and one child
    // // make this child the event source since this is the feature itself
    // // otherwise the callback will take over and announce its client
    // if ((mFeatureId == FEATURE_OPTIONS_PANEL ||
    //         mFeatureId == FEATURE_CONTEXT_MENU ||
    //         mFeatureId == FEATURE_PROGRESS ||
    //         mFeatureId == FEATURE_INDETERMINATE_PROGRESS)
    //         && getChildCount() == 1) {
    //     getChildAt(0).sendAccessibilityEvent(eventType);
    // } else {
    //     super.sendAccessibilityEvent(eventType);
    // }
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::DecorView::DispatchPopulateAccessibilityEvent(
    /* [in] */ IAccessibilityEvent* event,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // final Callback cb = getCallback();
    // if (cb != null && !isDestroyed()) {
    //     if (cb.dispatchPopulateAccessibilityEvent(event)) {
    //         return true;
    //     }
    // }
    // return super.dispatchPopulateAccessibilityEvent(event);
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::DecorView::Draw(
    /* [in] */ ICanvas* canvas)
{
    VALIDATE_NOT_NULL(canvas);
    // ==================before translated======================
    // super.draw(canvas);
    //
    // if (mMenuBackground != null) {
    //     mMenuBackground.draw(canvas);
    // }
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::DecorView::ShowContextMenuForChild(
    /* [in] */ IView* originalView,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // // Reuse the context menu builder
    // if (mContextMenu == null) {
    //     mContextMenu = new ContextMenuBuilder(getContext());
    //     mContextMenu.setCallback(mContextMenuCallback);
    // } else {
    //     mContextMenu.clearAll();
    // }
    //
    // final MenuDialogHelper helper = mContextMenu.show(originalView,
    //         originalView.getWindowToken());
    // if (helper != null) {
    //     helper.setPresenterCallback(mContextMenuCallback);
    // } else if (mContextMenuHelper != null) {
    //     // No menu to show, but if we have a menu currently showing it just became blank.
    //     // Close it.
    //     mContextMenuHelper.dismiss();
    // }
    // mContextMenuHelper = helper;
    // return helper != null;
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::DecorView::StartActionModeForChild(
    /* [in] */ IView* originalView,
    /* [in] */ IActionModeCallback* callback,
    /* [out] */ IActionMode** res)
{
    // ==================before translated======================
    // // originalView can be used here to be sure that we don't obscure
    // // relevant content with the context mode UI.
    // return startActionMode(callback);
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::DecorView::StartActionMode(
        /* [in] */ IActionModeCallback* callback,
        /* [out] */ IActionMode** res)
{
    // ==================before translated======================
    // if (mActionMode != null) {
    //     mActionMode.finish();
    // }
    //
    // final ActionMode.Callback wrappedCallback = new ActionModeCallbackWrapper(callback);
    // ActionMode mode = null;
    // if (getCallback() != null && !isDestroyed()) {
    //     try {
    //         mode = getCallback().onWindowStartingActionMode(wrappedCallback);
    //     } catch (AbstractMethodError ame) {
    //         // Older apps might not implement this callback method.
    //     }
    // }
    // if (mode != null) {
    //     mActionMode = mode;
    // } else {
    //     if (mActionModeView == null) {
    //         if (isFloating()) {
    //             // Use the action bar theme.
    //             final TypedValue outValue = new TypedValue();
    //             final Theme baseTheme = mContext.getTheme();
    //             baseTheme.resolveAttribute(R.attr.actionBarTheme, outValue, true);
    //
    //             final Context actionBarContext;
    //             if (outValue.resourceId != 0) {
    //                 final Theme actionBarTheme = mContext.getResources().newTheme();
    //                 actionBarTheme.setTo(baseTheme);
    //                 actionBarTheme.applyStyle(outValue.resourceId, true);
    //
    //                 actionBarContext = new ContextThemeWrapper(mContext, 0);
    //                 actionBarContext.getTheme().setTo(actionBarTheme);
    //             } else {
    //                 actionBarContext = mContext;
    //             }
    //
    //             mActionModeView = new ActionBarContextView(actionBarContext);
    //             mActionModePopup = new PopupWindow(actionBarContext, null,
    //                     R.attr.actionModePopupWindowStyle);
    //             mActionModePopup.setWindowLayoutType(
    //                     WindowManager.LayoutParams.TYPE_APPLICATION);
    //             mActionModePopup.setContentView(mActionModeView);
    //             mActionModePopup.setWidth(MATCH_PARENT);
    //
    //             actionBarContext.getTheme().resolveAttribute(
    //                     R.attr.actionBarSize, outValue, true);
    //             final int height = TypedValue.complexToDimensionPixelSize(outValue.data,
    //                     actionBarContext.getResources().getDisplayMetrics());
    //             mActionModeView.setContentHeight(height);
    //             mActionModePopup.setHeight(WRAP_CONTENT);
    //             mShowActionModePopup = new Runnable() {
    //                 public void run() {
    //                     mActionModePopup.showAtLocation(
    //                             mActionModeView.getApplicationWindowToken(),
    //                             Gravity.TOP | Gravity.FILL_HORIZONTAL, 0, 0);
    //                 }
    //             };
    //         } else {
    //             ViewStub stub = (ViewStub) findViewById(
    //                     R.id.action_mode_bar_stub);
    //             if (stub != null) {
    //                 mActionModeView = (ActionBarContextView) stub.inflate();
    //             }
    //         }
    //     }
    //
    //     if (mActionModeView != null) {
    //         mActionModeView.killMode();
    //         mode = new StandaloneActionMode(mActionModeView.getContext(), mActionModeView,
    //                 wrappedCallback, mActionModePopup == null);
    //         if (callback.onCreateActionMode(mode, mode.getMenu())) {
    //             mode.invalidate();
    //             mActionModeView.initForMode(mode);
    //             mActionModeView.setVisibility(View.VISIBLE);
    //             mActionMode = mode;
    //             if (mActionModePopup != null) {
    //                 post(mShowActionModePopup);
    //             }
    //             mActionModeView.sendAccessibilityEvent(
    //                     AccessibilityEvent.TYPE_WINDOW_STATE_CHANGED);
    //         } else {
    //             mActionMode = null;
    //         }
    //     }
    // }
    // if (mActionMode != null && getCallback() != null && !isDestroyed()) {
    //     try {
    //         getCallback().onActionModeStarted(mActionMode);
    //     } catch (AbstractMethodError ame) {
    //         // Older apps might not implement this callback method.
    //     }
    // }
    // return mActionMode;
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::DecorView::StartChanging()
{
    // ==================before translated======================
    // mChanging = true;
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::DecorView::FinishChanging()
{
    // ==================before translated======================
    // mChanging = false;
    // drawableChanged();
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::DecorView::SetWindowBackground(
    /* [in] */ IDrawable* drawable)
{
    VALIDATE_NOT_NULL(drawable);
    // ==================before translated======================
    // if (getBackground() != drawable) {
    //     setBackgroundDrawable(drawable);
    //     if (drawable != null) {
    //         drawable.getPadding(mBackgroundPadding);
    //     } else {
    //         mBackgroundPadding.setEmpty();
    //     }
    //     drawableChanged();
    // }
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::DecorView::SetBackgroundDrawable(
    /* [in] */ IDrawable* d)
{
    VALIDATE_NOT_NULL(d);
    // ==================before translated======================
    // super.setBackgroundDrawable(d);
    // if (getWindowToken() != null) {
    //     updateWindowResizeState();
    // }
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::DecorView::SetWindowFrame(
    /* [in] */ IDrawable* drawable)
{
    VALIDATE_NOT_NULL(drawable);
    // ==================before translated======================
    // if (getForeground() != drawable) {
    //     setForeground(drawable);
    //     if (drawable != null) {
    //         drawable.getPadding(mFramePadding);
    //     } else {
    //         mFramePadding.setEmpty();
    //     }
    //     drawableChanged();
    // }
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::DecorView::OnWindowSystemUiVisibilityChanged(
    /* [in] */ Int32 visible)
{
    // ==================before translated======================
    // updateColorViews(null /* insets */);
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::DecorView::OnApplyWindowInsets(
    /* [in] */ IWindowInsets* insets,
    /* [out] */ IWindowInsets** result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // mFrameOffsets.set(insets.getSystemWindowInsets());
    // insets = updateColorViews(insets);
    // insets = updateStatusGuard(insets);
    // updateNavigationGuard(insets);
    // if (getForeground() != null) {
    //     drawableChanged();
    // }
    // return insets;
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::DecorView::IsTransitionGroup(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return false;
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::DecorView::OnWindowFocusChanged(
    /* [in] */ Boolean hasWindowFocus)
{
    // ==================before translated======================
    // super.onWindowFocusChanged(hasWindowFocus);
    //
    // // If the user is chording a menu shortcut, release the chord since
    // // this window lost focus
    // if (!hasWindowFocus && mPanelChordingKey != 0) {
    //     closePanel(FEATURE_OPTIONS_PANEL);
    // }
    //
    // final Callback cb = getCallback();
    // if (cb != null && !isDestroyed() && mFeatureId < 0) {
    //     cb.onWindowFocusChanged(hasWindowFocus);
    // }
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::DecorView::UpdateWindowResizeState()
{
    // ==================before translated======================
    // Drawable bg = getBackground();
    // hackTurnOffWindowResizeAnim(bg == null || bg.getOpacity()
    //         != PixelFormat.OPAQUE);
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::DecorView::OnCloseSystemDialogs(
    /* [in] */ const String& reason)
{
    // ==================before translated======================
    // if (mFeatureId >= 0) {
    //     closeAllPanels();
    // }
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::DecorView::WillYouTakeTheSurface(
    /* [out] */ ISurfaceHolderCallback2** result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return mFeatureId < 0 ? mTakeSurfaceCallback : null;
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::DecorView::WillYouTakeTheInputQueue(
    /* [out] */ IInputQueueCallback** result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return mFeatureId < 0 ? mTakeInputQueueCallback : null;
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::DecorView::SetSurfaceType(
    /* [in] */ Int32 type)
{
    // ==================before translated======================
    // PhoneWindow.this.setType(type);
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::DecorView::SetSurfaceFormat(
    /* [in] */ Int32 format)
{
    // ==================before translated======================
    // PhoneWindow.this.setFormat(format);
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::DecorView::SetSurfaceKeepScreenOn(
    /* [in] */ Boolean keepOn)
{
    // ==================before translated======================
    // if (keepOn) PhoneWindow.this.addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
    // else PhoneWindow.this.clearFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
    assert(0);
    return NOERROR;
}

Boolean PhoneWindow::DecorView::SetFrame(
    /* [in] */ Int32 l,
    /* [in] */ Int32 t,
    /* [in] */ Int32 r,
    /* [in] */ Int32 b)
{
    // ==================before translated======================
    // boolean changed = super.setFrame(l, t, r, b);
    // if (changed) {
    //     final Rect drawingBounds = mDrawingBounds;
    //     getDrawingRect(drawingBounds);
    //
    //     Drawable fg = getForeground();
    //     if (fg != null) {
    //         final Rect frameOffsets = mFrameOffsets;
    //         drawingBounds.left += frameOffsets.left;
    //         drawingBounds.top += frameOffsets.top;
    //         drawingBounds.right -= frameOffsets.right;
    //         drawingBounds.bottom -= frameOffsets.bottom;
    //         fg.setBounds(drawingBounds);
    //         final Rect framePadding = mFramePadding;
    //         drawingBounds.left += framePadding.left - frameOffsets.left;
    //         drawingBounds.top += framePadding.top - frameOffsets.top;
    //         drawingBounds.right -= framePadding.right - frameOffsets.right;
    //         drawingBounds.bottom -= framePadding.bottom - frameOffsets.bottom;
    //     }
    //
    //     Drawable bg = getBackground();
    //     if (bg != null) {
    //         bg.setBounds(drawingBounds);
    //     }
    //
    //     if (SWEEP_OPEN_MENU) {
    //         if (mMenuBackground == null && mFeatureId < 0
    //                 && getAttributes().height
    //                 == WindowManager.LayoutParams.MATCH_PARENT) {
    //             mMenuBackground = getContext().getDrawable(
    //                     R.drawable.menu_background);
    //         }
    //         if (mMenuBackground != null) {
    //             mMenuBackground.setBounds(drawingBounds.left,
    //                     drawingBounds.bottom-6, drawingBounds.right,
    //                     drawingBounds.bottom+20);
    //         }
    //     }
    // }
    // return changed;
    assert(0);
    return FALSE;
}

void PhoneWindow::DecorView::OnMeasure(
    /* [in] */ Int32 widthMeasureSpec,
    /* [in] */ Int32 heightMeasureSpec)
{
    // ==================before translated======================
    // final DisplayMetrics metrics = getContext().getResources().getDisplayMetrics();
    // final boolean isPortrait = metrics.widthPixels < metrics.heightPixels;
    //
    // final int widthMode = getMode(widthMeasureSpec);
    // final int heightMode = getMode(heightMeasureSpec);
    //
    // boolean fixedWidth = false;
    // if (widthMode == AT_MOST) {
    //     final TypedValue tvw = isPortrait ? mFixedWidthMinor : mFixedWidthMajor;
    //     if (tvw != null && tvw.type != TypedValue.TYPE_NULL) {
    //         final int w;
    //         if (tvw.type == TypedValue.TYPE_DIMENSION) {
    //             w = (int) tvw.getDimension(metrics);
    //         } else if (tvw.type == TypedValue.TYPE_FRACTION) {
    //             w = (int) tvw.getFraction(metrics.widthPixels, metrics.widthPixels);
    //         } else {
    //             w = 0;
    //         }
    //
    //         if (w > 0) {
    //             final int widthSize = MeasureSpec.getSize(widthMeasureSpec);
    //             widthMeasureSpec = MeasureSpec.makeMeasureSpec(
    //                     Math.min(w, widthSize), EXACTLY);
    //             fixedWidth = true;
    //         }
    //     }
    // }
    //
    // if (heightMode == AT_MOST) {
    //     final TypedValue tvh = isPortrait ? mFixedHeightMajor : mFixedHeightMinor;
    //     if (tvh != null && tvh.type != TypedValue.TYPE_NULL) {
    //         final int h;
    //         if (tvh.type == TypedValue.TYPE_DIMENSION) {
    //             h = (int) tvh.getDimension(metrics);
    //         } else if (tvh.type == TypedValue.TYPE_FRACTION) {
    //             h = (int) tvh.getFraction(metrics.heightPixels, metrics.heightPixels);
    //         } else {
    //             h = 0;
    //         }
    //         if (h > 0) {
    //             final int heightSize = MeasureSpec.getSize(heightMeasureSpec);
    //             heightMeasureSpec = MeasureSpec.makeMeasureSpec(
    //                     Math.min(h, heightSize), EXACTLY);
    //         }
    //     }
    // }
    //
    // if (mOutsetBottom != null) {
    //     int mode = MeasureSpec.getMode(heightMeasureSpec);
    //     if (mode != MeasureSpec.UNSPECIFIED) {
    //         int outset = (int) mOutsetBottom.getDimension(metrics);
    //         int height = MeasureSpec.getSize(heightMeasureSpec);
    //         heightMeasureSpec = MeasureSpec.makeMeasureSpec(height + outset, mode);
    //     }
    // }
    //
    // super.onMeasure(widthMeasureSpec, heightMeasureSpec);
    //
    // int width = getMeasuredWidth();
    // boolean measure = false;
    //
    // widthMeasureSpec = MeasureSpec.makeMeasureSpec(width, EXACTLY);
    //
    // if (!fixedWidth && widthMode == AT_MOST) {
    //     final TypedValue tv = isPortrait ? mMinWidthMinor : mMinWidthMajor;
    //     if (tv.type != TypedValue.TYPE_NULL) {
    //         final int min;
    //         if (tv.type == TypedValue.TYPE_DIMENSION) {
    //             min = (int)tv.getDimension(metrics);
    //         } else if (tv.type == TypedValue.TYPE_FRACTION) {
    //             min = (int)tv.getFraction(metrics.widthPixels, metrics.widthPixels);
    //         } else {
    //             min = 0;
    //         }
    //
    //         if (width < min) {
    //             widthMeasureSpec = MeasureSpec.makeMeasureSpec(min, EXACTLY);
    //             measure = true;
    //         }
    //     }
    // }
    //
    // // TODO: Support height?
    //
    // if (measure) {
    //     super.onMeasure(widthMeasureSpec, heightMeasureSpec);
    // }
    assert(0);
}

ECode PhoneWindow::DecorView::OnAttachedToWindow()
{
    // ==================before translated======================
    // super.onAttachedToWindow();
    //
    // updateWindowResizeState();
    //
    // final Callback cb = getCallback();
    // if (cb != null && !isDestroyed() && mFeatureId < 0) {
    //     cb.onAttachedToWindow();
    // }
    //
    // if (mFeatureId == -1) {
    //     /*
    //      * The main window has been attached, try to restore any panels
    //      * that may have been open before. This is called in cases where
    //      * an activity is being killed for configuration change and the
    //      * menu was open. When the activity is recreated, the menu
    //      * should be shown again.
    //      */
    //     openPanelsAfterRestore();
    // }
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::DecorView::OnDetachedFromWindow()
{
    // ==================before translated======================
    // super.onDetachedFromWindow();
    //
    // final Callback cb = getCallback();
    // if (cb != null && mFeatureId < 0) {
    //     cb.onDetachedFromWindow();
    // }
    //
    // if (mDecorContentParent != null) {
    //     mDecorContentParent.dismissPopups();
    // }
    //
    // if (mActionModePopup != null) {
    //     removeCallbacks(mShowActionModePopup);
    //     if (mActionModePopup.isShowing()) {
    //         mActionModePopup.dismiss();
    //     }
    //     mActionModePopup = null;
    // }
    //
    // PanelFeatureState st = getPanelState(FEATURE_OPTIONS_PANEL, false);
    // if (st != null && st.menu != null && mFeatureId < 0) {
    //     st.menu.close();
    // }
    assert(0);
    return NOERROR;
}

Boolean PhoneWindow::DecorView::IsOutOfBounds(
    /* [in] */ Int32 x,
    /* [in] */ Int32 y)
{
    // ==================before translated======================
    // return x < -5 || y < -5 || x > (getWidth() + 5)
    //         || y > (getHeight() + 5);
    assert(0);
    return FALSE;
}

AutoPtr<IWindowInsets> PhoneWindow::DecorView::UpdateColorViews(
    /* [in] */ IWindowInsets* insets)
{
    // ==================before translated======================
    // WindowManager.LayoutParams attrs = getAttributes();
    // int sysUiVisibility = attrs.systemUiVisibility | getWindowSystemUiVisibility();
    //
    // if (!mIsFloating && ActivityManager.isHighEndGfx()) {
    //     if (insets != null) {
    //         mLastTopInset = Math.min(insets.getStableInsetTop(),
    //                 insets.getSystemWindowInsetTop());
    //         mLastBottomInset = Math.min(insets.getStableInsetBottom(),
    //                 insets.getSystemWindowInsetBottom());
    //         mLastRightInset = Math.min(insets.getStableInsetRight(),
    //                 insets.getSystemWindowInsetRight());
    //     }
    //     mStatusColorView = updateColorViewInt(mStatusColorView, sysUiVisibility,
    //             SYSTEM_UI_FLAG_FULLSCREEN, FLAG_TRANSLUCENT_STATUS,
    //             mStatusBarColor, mLastTopInset, Gravity.TOP,
    //             STATUS_BAR_BACKGROUND_TRANSITION_NAME,
    //             com.android.internal.R.id.statusBarBackground,
    //             (getAttributes().flags & FLAG_FULLSCREEN) != 0);
    //     mNavigationColorView = updateColorViewInt(mNavigationColorView, sysUiVisibility,
    //             SYSTEM_UI_FLAG_HIDE_NAVIGATION, FLAG_TRANSLUCENT_NAVIGATION,
    //             mNavigationBarColor, mLastBottomInset, Gravity.BOTTOM,
    //             NAVIGATION_BAR_BACKGROUND_TRANSITION_NAME,
    //             com.android.internal.R.id.navigationBarBackground,
    //             false /* hiddenByWindowFlag */);
    // }
    //
    // // When we expand the window with FLAG_DRAWS_SYSTEM_BAR_BACKGROUNDS, we still need
    // // to ensure that the rest of the view hierarchy doesn't notice it, unless they've
    // // explicitly asked for it.
    //
    // boolean consumingNavBar =
    //         (attrs.flags & FLAG_DRAWS_SYSTEM_BAR_BACKGROUNDS) != 0
    //                 && (sysUiVisibility & SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION) == 0
    //                 && (sysUiVisibility & SYSTEM_UI_FLAG_HIDE_NAVIGATION) == 0;
    //
    // int consumedRight = consumingNavBar ? mLastRightInset : 0;
    // int consumedBottom = consumingNavBar ? mLastBottomInset : 0;
    //
    // if (mContentRoot != null
    //         && mContentRoot.getLayoutParams() instanceof MarginLayoutParams) {
    //     MarginLayoutParams lp = (MarginLayoutParams) mContentRoot.getLayoutParams();
    //     if (lp.rightMargin != consumedRight || lp.bottomMargin != consumedBottom) {
    //         lp.rightMargin = consumedRight;
    //         lp.bottomMargin = consumedBottom;
    //         mContentRoot.setLayoutParams(lp);
    //
    //         if (insets == null) {
    //             // The insets have changed, but we're not currently in the process
    //             // of dispatching them.
    //             requestApplyInsets();
    //         }
    //     }
    //     if (insets != null) {
    //         insets = insets.replaceSystemWindowInsets(
    //                 insets.getSystemWindowInsetLeft(),
    //                 insets.getSystemWindowInsetTop(),
    //                 insets.getSystemWindowInsetRight() - consumedRight,
    //                 insets.getSystemWindowInsetBottom() - consumedBottom);
    //     }
    // }
    //
    // if (insets != null) {
    //     insets = insets.consumeStableInsets();
    // }
    // return insets;
    assert(0);
    AutoPtr<IWindowInsets> empty;
    return empty;
}

AutoPtr<IView> PhoneWindow::DecorView::UpdateColorViewInt(
    /* [in] */ IView* view,
    /* [in] */ Int32 sysUiVis,
    /* [in] */ Int32 systemUiHideFlag,
    /* [in] */ Int32 translucentFlag,
    /* [in] */ Int32 color,
    /* [in] */ Int32 height,
    /* [in] */ Int32 verticalGravity,
    /* [in] */ const String& transitionName,
    /* [in] */ Int32 id,
    /* [in] */ Boolean hiddenByWindowFlag)
{
    // ==================before translated======================
    // boolean show = height > 0 && (sysUiVis & systemUiHideFlag) == 0
    //         && !hiddenByWindowFlag
    //         && (getAttributes().flags & translucentFlag) == 0
    //         && (color & Color.BLACK) != 0
    //         && (getAttributes().flags & FLAG_DRAWS_SYSTEM_BAR_BACKGROUNDS) != 0;
    //
    // if (view == null) {
    //     if (show) {
    //         view = new View(mContext);
    //         view.setBackgroundColor(color);
    //         view.setTransitionName(transitionName);
    //         view.setId(id);
    //         addView(view, new LayoutParams(LayoutParams.MATCH_PARENT, height,
    //                 Gravity.START | verticalGravity));
    //     }
    // } else {
    //     int vis = show ? VISIBLE : INVISIBLE;
    //     view.setVisibility(vis);
    //     if (show) {
    //         LayoutParams lp = (LayoutParams) view.getLayoutParams();
    //         if (lp.height != height) {
    //             lp.height = height;
    //             view.setLayoutParams(lp);
    //         }
    //         view.setBackgroundColor(color);
    //     }
    // }
    // return view;
    assert(0);
    AutoPtr<IView> empty;
    return empty;
}

AutoPtr<IWindowInsets> PhoneWindow::DecorView::UpdateStatusGuard(
    /* [in] */ IWindowInsets* insets)
{
    // ==================before translated======================
    // boolean showStatusGuard = false;
    // // Show the status guard when the non-overlay contextual action bar is showing
    // if (mActionModeView != null) {
    //     if (mActionModeView.getLayoutParams() instanceof MarginLayoutParams) {
    //         // Insets are magic!
    //         final MarginLayoutParams mlp = (MarginLayoutParams)
    //                 mActionModeView.getLayoutParams();
    //         boolean mlpChanged = false;
    //         if (mActionModeView.isShown()) {
    //             if (mTempRect == null) {
    //                 mTempRect = new Rect();
    //             }
    //             final Rect rect = mTempRect;
    //
    //             // If the parent doesn't consume the insets, manually
    //             // apply the default system window insets.
    //             mContentParent.computeSystemWindowInsets(insets, rect);
    //             final int newMargin = rect.top == 0 ? insets.getSystemWindowInsetTop() : 0;
    //             if (mlp.topMargin != newMargin) {
    //                 mlpChanged = true;
    //                 mlp.topMargin = insets.getSystemWindowInsetTop();
    //
    //                 if (mStatusGuard == null) {
    //                     mStatusGuard = new View(mContext);
    //                     mStatusGuard.setBackgroundColor(mContext.getResources()
    //                             .getColor(R.color.input_method_navigation_guard));
    //                     addView(mStatusGuard, indexOfChild(mStatusColorView),
    //                             new LayoutParams(LayoutParams.MATCH_PARENT,
    //                                     mlp.topMargin, Gravity.START | Gravity.TOP));
    //                 } else {
    //                     final LayoutParams lp = (LayoutParams)
    //                             mStatusGuard.getLayoutParams();
    //                     if (lp.height != mlp.topMargin) {
    //                         lp.height = mlp.topMargin;
    //                         mStatusGuard.setLayoutParams(lp);
    //                     }
    //                 }
    //             }
    //
    //             // The action mode's theme may differ from the app, so
    //             // always show the status guard above it if we have one.
    //             showStatusGuard = mStatusGuard != null;
    //
    //             // We only need to consume the insets if the action
    //             // mode is overlaid on the app content (e.g. it's
    //             // sitting in a FrameLayout, see
    //             // screen_simple_overlay_action_mode.xml).
    //             final boolean nonOverlay = (getLocalFeatures()
    //                     & (1 << FEATURE_ACTION_MODE_OVERLAY)) == 0;
    //             insets = insets.consumeSystemWindowInsets(
    //                     false, nonOverlay && showStatusGuard /* top */, false, false);
    //         } else {
    //             // reset top margin
    //             if (mlp.topMargin != 0) {
    //                 mlpChanged = true;
    //                 mlp.topMargin = 0;
    //             }
    //         }
    //         if (mlpChanged) {
    //             mActionModeView.setLayoutParams(mlp);
    //         }
    //     }
    // }
    // if (mStatusGuard != null) {
    //     mStatusGuard.setVisibility(showStatusGuard ? View.VISIBLE : View.GONE);
    // }
    // return insets;
    assert(0);
    AutoPtr<IWindowInsets> empty;
    return empty;
}

void PhoneWindow::DecorView::UpdateNavigationGuard(
    /* [in] */ IWindowInsets* insets)
{
    // ==================before translated======================
    // // IMEs lay out below the nav bar, but the content view must not (for back compat)
    // if (getAttributes().type == WindowManager.LayoutParams.TYPE_INPUT_METHOD) {
    //     // prevent the content view from including the nav bar height
    //     if (mContentParent != null) {
    //         if (mContentParent.getLayoutParams() instanceof MarginLayoutParams) {
    //             MarginLayoutParams mlp =
    //                     (MarginLayoutParams) mContentParent.getLayoutParams();
    //             mlp.bottomMargin = insets.getSystemWindowInsetBottom();
    //             mContentParent.setLayoutParams(mlp);
    //         }
    //     }
    //     // position the navigation guard view, creating it if necessary
    //     if (mNavigationGuard == null) {
    //         mNavigationGuard = new View(mContext);
    //         mNavigationGuard.setBackgroundColor(mContext.getResources()
    //                 .getColor(R.color.input_method_navigation_guard));
    //         addView(mNavigationGuard, indexOfChild(mNavigationColorView), new LayoutParams(
    //                 LayoutParams.MATCH_PARENT, insets.getSystemWindowInsetBottom(),
    //                 Gravity.START | Gravity.BOTTOM));
    //     } else {
    //         LayoutParams lp = (LayoutParams) mNavigationGuard.getLayoutParams();
    //         lp.height = insets.getSystemWindowInsetBottom();
    //         mNavigationGuard.setLayoutParams(lp);
    //     }
    // }
    assert(0);
}

void PhoneWindow::DecorView::DrawableChanged()
{
    // ==================before translated======================
    // if (mChanging) {
    //     return;
    // }
    //
    // setPadding(mFramePadding.left + mBackgroundPadding.left, mFramePadding.top
    //         + mBackgroundPadding.top, mFramePadding.right + mBackgroundPadding.right,
    //         mFramePadding.bottom + mBackgroundPadding.bottom);
    // requestLayout();
    // invalidate();
    //
    // int opacity = PixelFormat.OPAQUE;
    // // Note: if there is no background, we will assume opaque. The
    // // common case seems to be that an application sets there to be
    // // no background so it can draw everything itself. For that,
    // // we would like to assume OPAQUE and let the app force it to
    // // the slower TRANSLUCENT mode if that is really what it wants.
    // Drawable bg = getBackground();
    // Drawable fg = getForeground();
    // if (bg != null) {
    //     if (fg == null) {
    //         opacity = bg.getOpacity();
    //     } else if (mFramePadding.left <= 0 && mFramePadding.top <= 0
    //             && mFramePadding.right <= 0 && mFramePadding.bottom <= 0) {
    //         // If the frame padding is zero, then we can be opaque
    //         // if either the frame -or- the background is opaque.
    //         int fop = fg.getOpacity();
    //         int bop = bg.getOpacity();
    //         if (false)
    //             Log.v(TAG, "Background opacity: " + bop + ", Frame opacity: " + fop);
    //         if (fop == PixelFormat.OPAQUE || bop == PixelFormat.OPAQUE) {
    //             opacity = PixelFormat.OPAQUE;
    //         } else if (fop == PixelFormat.UNKNOWN) {
    //             opacity = bop;
    //         } else if (bop == PixelFormat.UNKNOWN) {
    //             opacity = fop;
    //         } else {
    //             opacity = Drawable.resolveOpacity(fop, bop);
    //         }
    //     } else {
    //         // For now we have to assume translucent if there is a
    //         // frame with padding... there is no way to tell if the
    //         // frame and background together will draw all pixels.
    //         if (false)
    //             Log.v(TAG, "Padding: " + mFramePadding);
    //         opacity = PixelFormat.TRANSLUCENT;
    //     }
    // }
    //
    // if (false)
    //     Log.v(TAG, "Background: " + bg + ", Frame: " + fg);
    // if (false)
    //     Log.v(TAG, "Selected default opacity: " + opacity);
    //
    // mDefaultOpacity = opacity;
    // if (mFeatureId < 0) {
    //     setDefaultWindowFormat(opacity);
    // }
    assert(0);
}

//=====================================================================
//                  PhoneWindow::DrawableFeatureState
//=====================================================================
PhoneWindow::DrawableFeatureState::DrawableFeatureState(
    /* [in] */ Int32 _featureId)
{
    // ==================before translated======================
    // featureId = _featureId;
}

//=====================================================================
//              PhoneWindow::PanelFeatureState::SavedState
//=====================================================================
//const AutoPtr<IParcelable> AutoPtr< ::Creator<SavedState> > PhoneWindow::PanelFeatureState::SavedState::CREATOR = new InnerParcelableCreator(this);
CAR_INTERFACE_IMPL_2(PhoneWindow::PanelFeatureState::SavedState, Object, IPhoneWindowSavedState, IParcelable)

PhoneWindow::PanelFeatureState::SavedState::SavedState()
{
}

ECode PhoneWindow::PanelFeatureState::SavedState::constructor()
{
    return NOERROR;
}

ECode PhoneWindow::PanelFeatureState::SavedState::constructor(
    /* [in] */ IParcelable* superState)
{
    // savedState.featureId = source.readInt();
    // savedState.isOpen = source.readInt() == 1;
    // savedState.isInExpandedMode = source.readInt() == 1;

    // if (savedState.isOpen) {
    //     savedState.menuState = source.readBundle();
    // }
    return NOERROR;
}

ECode PhoneWindow::PanelFeatureState::SavedState::DescribeContents(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return 0;
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::PanelFeatureState::SavedState::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    VALIDATE_NOT_NULL(dest);
    // ==================before translated======================
    // dest.writeInt(featureId);
    // dest.writeInt(isOpen ? 1 : 0);
    // dest.writeInt(isInExpandedMode ? 1 : 0);
    //
    // if (isOpen) {
    //     dest.writeBundle(menuState);
    // }
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::PanelFeatureState::SavedState::ReadFromParcel(
    /* [in] */ IParcel* in)
{
    // ==================before translated======================
    // SavedState savedState = new SavedState();
    // savedState.featureId = source.readInt();
    // savedState.isOpen = source.readInt() == 1;
    // savedState.isInExpandedMode = source.readInt() == 1;
    //
    // if (savedState.isOpen) {
    //     savedState.menuState = source.readBundle();
    // }
    //
    // return savedState;
    assert(0);
    return NOERROR;
}

//=====================================================================
//                    PhoneWindow::PanelFeatureState
//=====================================================================
PhoneWindow::PanelFeatureState::PanelFeatureState(
    /* [in] */ PhoneWindow* owner,
    /* [in] */ Int32 featureId)
{
    // ==================before translated======================
    // this.featureId = featureId;
    //
    // refreshDecorView = false;
    mOwner = owner;
}

ECode PhoneWindow::PanelFeatureState::IsInListMode(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return isInExpandedMode || isCompact;
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::PanelFeatureState::HasPanelItems(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // if (shownPanelView == null) return false;
    // if (createdPanelView != null) return true;
    //
    // if (isCompact || isInExpandedMode) {
    //     return listMenuPresenter.getAdapter().getCount() > 0;
    // } else {
    //     return ((ViewGroup) shownPanelView).getChildCount() > 0;
    // }
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::PanelFeatureState::ClearMenuPresenters()
{
    // ==================before translated======================
    // if (menu != null) {
    //     menu.removeMenuPresenter(iconMenuPresenter);
    //     menu.removeMenuPresenter(listMenuPresenter);
    // }
    // iconMenuPresenter = null;
    // listMenuPresenter = null;
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::PanelFeatureState::SetStyle(
    /* [in] */ IContext* context)
{
    VALIDATE_NOT_NULL(context);
    // ==================before translated======================
    // TypedArray a = context.obtainStyledAttributes(R.styleable.Theme);
    // background = a.getResourceId(
    //         R.styleable.Theme_panelBackground, 0);
    // fullBackground = a.getResourceId(
    //         R.styleable.Theme_panelFullBackground, 0);
    // windowAnimations = a.getResourceId(
    //         R.styleable.Theme_windowAnimationStyle, 0);
    // isCompact = a.getBoolean(
    //         R.styleable.Theme_panelMenuIsCompact, false);
    // listPresenterTheme = a.getResourceId(
    //         R.styleable.Theme_panelMenuListTheme,
    //         R.style.Theme_ExpandedMenu);
    // a.recycle();
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::PanelFeatureState::SetMenu(
    /* [in] */ IMenuBuilder* menu)
{
    VALIDATE_NOT_NULL(menu);
    // ==================before translated======================
    // if (menu == this.menu) return;
    //
    // if (this.menu != null) {
    //     this.menu.removeMenuPresenter(iconMenuPresenter);
    //     this.menu.removeMenuPresenter(listMenuPresenter);
    // }
    // this.menu = menu;
    // if (menu != null) {
    //     if (iconMenuPresenter != null) menu.addMenuPresenter(iconMenuPresenter);
    //     if (listMenuPresenter != null) menu.addMenuPresenter(listMenuPresenter);
    // }
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::PanelFeatureState::GetListMenuView(
    /* [in] */ IContext* context,
    /* [in] */ IMenuPresenterCallback* cb,
    /* [out] */ IMenuView** result)
{
    VALIDATE_NOT_NULL(context);
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // if (menu == null) return null;
    //
    // if (!isCompact) {
    //     getIconMenuView(context, cb); // Need this initialized to know where our offset goes
    // }
    //
    // if (listMenuPresenter == null) {
    //     listMenuPresenter = new ListMenuPresenter(
    //             R.layout.list_menu_item_layout, listPresenterTheme);
    //     listMenuPresenter.setCallback(cb);
    //     listMenuPresenter.setId(R.id.list_menu_presenter);
    //     menu.addMenuPresenter(listMenuPresenter);
    // }
    //
    // if (iconMenuPresenter != null) {
    //     listMenuPresenter.setItemIndexOffset(
    //             iconMenuPresenter.getNumActualItemsShown());
    // }
    // MenuView result = listMenuPresenter.getMenuView(decorView);
    //
    // return result;
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::PanelFeatureState::GetIconMenuView(
    /* [in] */ IContext* context,
    /* [in] */ IMenuPresenterCallback* cb,
    /* [out] */ IMenuView** result)
{
    VALIDATE_NOT_NULL(context);
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // if (menu == null) return null;
    //
    // if (iconMenuPresenter == null) {
    //     iconMenuPresenter = new IconMenuPresenter(context);
    //     iconMenuPresenter.setCallback(cb);
    //     iconMenuPresenter.setId(R.id.icon_menu_presenter);
    //     menu.addMenuPresenter(iconMenuPresenter);
    // }
    //
    // MenuView result = iconMenuPresenter.getMenuView(decorView);
    //
    // return result;
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::PanelFeatureState::OnSaveInstanceState(
    /* [out] */ IParcelable** result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // SavedState savedState = new SavedState();
    // savedState.featureId = featureId;
    // savedState.isOpen = isOpen;
    // savedState.isInExpandedMode = isInExpandedMode;
    //
    // if (menu != null) {
    //     savedState.menuState = new Bundle();
    //     menu.savePresenterStates(savedState.menuState);
    // }
    //
    // return savedState;
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::PanelFeatureState::OnRestoreInstanceState(
    /* [in] */ IParcelable* state)
{
    VALIDATE_NOT_NULL(state);
    // ==================before translated======================
    // SavedState savedState = (SavedState) state;
    // featureId = savedState.featureId;
    // wasLastOpen = savedState.isOpen;
    // wasLastExpanded = savedState.isInExpandedMode;
    // frozenMenuState = savedState.menuState;
    //
    // /*
    //  * A LocalActivityManager keeps the same instance of this class around.
    //  * The first time the menu is being shown after restoring, the
    //  * Activity.onCreateOptionsMenu should be called. But, if it is the
    //  * same instance then menu != null and we won't call that method.
    //  * We clear any cached views here. The caller should invalidatePanelMenu.
    //  */
    // createdPanelView = null;
    // shownPanelView = null;
    // decorView = null;
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::PanelFeatureState::ApplyFrozenState()
{
    // ==================before translated======================
    // if (menu != null && frozenMenuState != null) {
    //     menu.restorePresenterStates(frozenMenuState);
    //     frozenMenuState = null;
    // }
    assert(0);
    return NOERROR;
}

//=====================================================================
//                   PhoneWindow::DialogMenuCallback
//=====================================================================
CAR_INTERFACE_IMPL_2(PhoneWindow::DialogMenuCallback, Object, IMenuBuilderCallback, IMenuPresenterCallback)

PhoneWindow::DialogMenuCallback::DialogMenuCallback(
    /* [in] */ PhoneWindow* owner,
    /* [in] */ Int32 featureId)
{
    mOwner = owner;
    // ==================before translated======================
    // mFeatureId = featureId;
}

ECode PhoneWindow::DialogMenuCallback::OnCloseMenu(
    /* [in] */ IMenuBuilder* menu,
    /* [in] */ Boolean allMenusAreClosing)
{
    VALIDATE_NOT_NULL(menu);
    // ==================before translated======================
    // if (menu.getRootMenu() != menu) {
    //     onCloseSubMenu(menu);
    // }
    //
    // if (allMenusAreClosing) {
    //     Callback callback = getCallback();
    //     if (callback != null && !isDestroyed()) {
    //         callback.onPanelClosed(mFeatureId, menu);
    //     }
    //
    //     if (menu == mContextMenu) {
    //         dismissContextMenu();
    //     }
    //
    //     // Dismiss the submenu, if it is showing
    //     if (mSubMenuHelper != null) {
    //         mSubMenuHelper.dismiss();
    //         mSubMenuHelper = null;
    //     }
    // }
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::DialogMenuCallback::OnCloseSubMenu(
    /* [in] */ IMenuBuilder* menu)
{
    VALIDATE_NOT_NULL(menu);
    // ==================before translated======================
    // Callback callback = getCallback();
    // if (callback != null && !isDestroyed()) {
    //     callback.onPanelClosed(mFeatureId, menu.getRootMenu());
    // }
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::DialogMenuCallback::OnMenuItemSelected(
    /* [in] */ IMenuBuilder* menu,
    /* [in] */ IMenuItem* item,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(menu);
    VALIDATE_NOT_NULL(item);
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // Callback callback = getCallback();
    // return (callback != null && !isDestroyed())
    //         && callback.onMenuItemSelected(mFeatureId, item);
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::DialogMenuCallback::OnMenuModeChange(
    /* [in] */ IMenuBuilder* menu)
{
    VALIDATE_NOT_NULL(menu);
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::DialogMenuCallback::OnOpenSubMenu(
    /* [in] */ IMenuBuilder* subMenu,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(subMenu);
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // if (subMenu == null) return false;
    //
    // // Set a simple callback for the submenu
    // subMenu.setCallback(this);
    //
    // // The window manager will give us a valid window token
    // mSubMenuHelper = new MenuDialogHelper(subMenu);
    // mSubMenuHelper.show(null);
    //
    // return true;
    assert(0);
    return NOERROR;
}

//=====================================================================
//                      PhoneWindow::InnerRunnable
//=====================================================================
CAR_INTERFACE_IMPL(PhoneWindow::InnerRunnable, Object, IRunnable)

PhoneWindow::InnerRunnable::InnerRunnable(
    /* [in] */ PhoneWindow* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

ECode PhoneWindow::InnerRunnable::Run()
{
    //@Override public void run() {
    //    for (int i = 0; i <= FEATURE_MAX; i++) {
    //        if ((mInvalidatePanelMenuFeatures & 1 << i) != 0) {
    //            doInvalidatePanelMenu(i);
    //        }
    //    }
    //    mInvalidatePanelMenuPosted = false;
    //    mInvalidatePanelMenuFeatures = 0;
    //}
    return NOERROR;
}

//=====================================================================
//               PhoneWindow::PanelMenuPresenterCallback
//=====================================================================
CAR_INTERFACE_IMPL(PhoneWindow::PanelMenuPresenterCallback, Object, IMenuPresenterCallback)

PhoneWindow::PanelMenuPresenterCallback::PanelMenuPresenterCallback(
    /* [in] */ PhoneWindow* owner)
    : mOwner(owner)
{
}

ECode PhoneWindow::PanelMenuPresenterCallback::OnCloseMenu(
    /* [in] */ IMenuBuilder* menu,
    /* [in] */ Boolean allMenusAreClosing)
{
    VALIDATE_NOT_NULL(menu);
    // ==================before translated======================
    // final Menu parentMenu = menu.getRootMenu();
    // final boolean isSubMenu = parentMenu != menu;
    // final PanelFeatureState panel = findMenuPanel(isSubMenu ? parentMenu : menu);
    // if (panel != null) {
    //     if (isSubMenu) {
    //         callOnPanelClosed(panel.featureId, panel, parentMenu);
    //         closePanel(panel, true);
    //     } else {
    //         // Close the panel and only do the callback if the menu is being
    //         // closed completely, not if opening a sub menu
    //         closePanel(panel, allMenusAreClosing);
    //     }
    // }
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::PanelMenuPresenterCallback::OnOpenSubMenu(
    /* [in] */ IMenuBuilder* subMenu,
    /* [out] */ Boolean* handle)
{
    VALIDATE_NOT_NULL(handle);
    // ==================before translated======================
    // if (subMenu == null && hasFeature(FEATURE_ACTION_BAR)) {
    //     Callback cb = getCallback();
    //     if (cb != null && !isDestroyed()) {
    //         cb.onMenuOpened(FEATURE_ACTION_BAR, subMenu);
    //     }
    // }
    //
    // return true;
    assert(0);
    return NOERROR;
}

//=====================================================================
//       PhoneWindow::InnerSwipeDismissLayoutOnDismissedListener1
//=====================================================================
CAR_INTERFACE_IMPL(PhoneWindow::InnerSwipeDismissLayoutOnDismissedListener1, Object, IOnDismissedListener)

PhoneWindow::InnerSwipeDismissLayoutOnDismissedListener1::InnerSwipeDismissLayoutOnDismissedListener1(
    /* [in] */ PhoneWindow* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

ECode PhoneWindow::InnerSwipeDismissLayoutOnDismissedListener1::OnDismissed(
    /* [in] */ ISwipeDismissLayout* layout)
{
    VALIDATE_NOT_NULL(layout);
    // ==================before translated======================
    // dispatchOnWindowDismissed();
    assert(0);
    return NOERROR;
}

//=====================================================================
// PhoneWindow::InnerSwipeDismissLayoutOnSwipeProgressChangedListener1
//=====================================================================
CAR_INTERFACE_IMPL(PhoneWindow::InnerSwipeDismissLayoutOnSwipeProgressChangedListener1, Object, IOnSwipeProgressChangedListener)

const Float PhoneWindow::InnerSwipeDismissLayoutOnSwipeProgressChangedListener1::ALPHA_DECREASE = 0.5f;

PhoneWindow::InnerSwipeDismissLayoutOnSwipeProgressChangedListener1::InnerSwipeDismissLayoutOnSwipeProgressChangedListener1(
    /* [in] */ PhoneWindow* owner)
    : mIsTranslucent(FALSE)
    , mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

ECode PhoneWindow::InnerSwipeDismissLayoutOnSwipeProgressChangedListener1::OnSwipeProgressChanged(
    /* [in] */ ISwipeDismissLayout* layout,
    /* [in] */ Float progress,
    /* [in] */ Float translate)
{
    VALIDATE_NOT_NULL(layout);
    // ==================before translated======================
    // WindowManager.LayoutParams newParams = getAttributes();
    // newParams.x = (int) translate;
    // newParams.alpha = 1 - (progress * ALPHA_DECREASE);
    // setAttributes(newParams);
    //
    // int flags = 0;
    // if (newParams.x == 0) {
    //     flags = FLAG_FULLSCREEN;
    // } else {
    //     flags = FLAG_LAYOUT_NO_LIMITS;
    // }
    // setFlags(flags, FLAG_FULLSCREEN | FLAG_LAYOUT_NO_LIMITS);
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::InnerSwipeDismissLayoutOnSwipeProgressChangedListener1::OnSwipeCancelled(
    /* [in] */ ISwipeDismissLayout* layout)
{
    VALIDATE_NOT_NULL(layout);
    // ==================before translated======================
    // WindowManager.LayoutParams newParams = getAttributes();
    // newParams.x = 0;
    // newParams.alpha = 1;
    // setAttributes(newParams);
    // setFlags(FLAG_FULLSCREEN, FLAG_FULLSCREEN | FLAG_LAYOUT_NO_LIMITS);
    assert(0);
    return NOERROR;
}

//=====================================================================
//                             PhoneWindow
//=====================================================================
static AutoPtr<IPhoneWindowRotationWatcher> InitDefaultRotationWatcher()
{
    AutoPtr<CPhoneWindowRotationWatcher> rotationWatcher;
    CPhoneWindowRotationWatcher::NewByFriend(NULL, (CPhoneWindowRotationWatcher**)&rotationWatcher);
    return (IPhoneWindowRotationWatcher*)rotationWatcher.Get();
}

static AutoPtr<ITransition> InitDefaultTransitionSet()
{
    AutoPtr<CTransitionSet> transitionSet;
    CTransitionSet::NewByFriend((CTransitionSet**)&transitionSet);
    //return ITransition::Probe(transitionSet);
    return (ITransition*)(transitionSet.Get());
}

CAR_INTERFACE_IMPL_2(PhoneWindow, Window, IPhoneWindow, IMenuBuilderCallback);

const Int32 PhoneWindow::FLAG_RESOURCE_SET_ICON;
const Int32 PhoneWindow::FLAG_RESOURCE_SET_LOGO;
const Int32 PhoneWindow::FLAG_RESOURCE_SET_ICON_FALLBACK;
AutoPtr<IPhoneWindowRotationWatcher> PhoneWindow::sRotationWatcher = InitDefaultRotationWatcher();
const String PhoneWindow::TAG("PhoneWindow");
const Boolean PhoneWindow::SWEEP_OPEN_MENU = false;
const Int32 PhoneWindow::DEFAULT_BACKGROUND_FADE_DURATION_MS;
const Int32 PhoneWindow::CUSTOM_TITLE_COMPATIBLE_FEATURES = DEFAULT_FEATURES |
                            (1 << FEATURE_CUSTOM_TITLE) |
                            (1 << FEATURE_CONTENT_TRANSITIONS) |
                            (1 << FEATURE_ACTIVITY_TRANSITIONS) |
                            (1 << FEATURE_ACTION_MODE_OVERLAY);

AutoPtr<ITransition> PhoneWindow::USE_DEFAULT_TRANSITION = InitDefaultTransitionSet();
const String PhoneWindow::FOCUSED_ID_TAG("android:focusedViewId");
const String PhoneWindow::VIEWS_TAG("android:views");
const String PhoneWindow::PANELS_TAG("android:Panels");
const String PhoneWindow::ACTION_BAR_TAG("android:ActionBar");

PhoneWindow::PhoneWindow()
{
}

ECode PhoneWindow::constructor(
    /* [in] */ IContext* context)
{
    // ==================before translated======================
    // super(context);
    // mLayoutInflater = LayoutInflater.from(context);
    return NOERROR;
}

ECode PhoneWindow::SetContainer(
    /* [in] */ IWindow* container)
{
    VALIDATE_NOT_NULL(container);
    // ==================before translated======================
    // super.setContainer(container);
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::RequestFeature(
    /* [in] */ Int32 featureId,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // if (mContentParent != null) {
    //     throw new AndroidRuntimeException("requestFeature() must be called before adding content");
    // }
    // final int features = getFeatures();
    // final int newFeatures = features | (1 << featureId);
    // if ((newFeatures & (1 << FEATURE_CUSTOM_TITLE)) != 0 &&
    //         (newFeatures & ~CUSTOM_TITLE_COMPATIBLE_FEATURES) != 0) {
    //     // Another feature is enabled and the user is trying to enable the custom title feature
    //     // or custom title feature is enabled and the user is trying to enable another feature
    //     throw new AndroidRuntimeException(
    //             "You cannot combine custom titles with other title features");
    // }
    // if ((features & (1 << FEATURE_NO_TITLE)) != 0 && featureId == FEATURE_ACTION_BAR) {
    //     return false; // Ignore. No title dominates.
    // }
    // if ((features & (1 << FEATURE_ACTION_BAR)) != 0 && featureId == FEATURE_NO_TITLE) {
    //     // Remove the action bar feature if we have no title. No title dominates.
    //     removeFeature(FEATURE_ACTION_BAR);
    // }
    //
    // if ((features & (1 << FEATURE_ACTION_BAR)) != 0 && featureId == FEATURE_SWIPE_TO_DISMISS) {
    //     throw new AndroidRuntimeException(
    //             "You cannot combine swipe dismissal and the action bar.");
    // }
    // if ((features & (1 << FEATURE_SWIPE_TO_DISMISS)) != 0 && featureId == FEATURE_ACTION_BAR) {
    //     throw new AndroidRuntimeException(
    //             "You cannot combine swipe dismissal and the action bar.");
    // }
    //
    // if (featureId == FEATURE_INDETERMINATE_PROGRESS &&
    //         getContext().getPackageManager().hasSystemFeature(PackageManager.FEATURE_WATCH)) {
    //     throw new AndroidRuntimeException("You cannot use indeterminate progress on a watch.");
    // }
    // return super.requestFeature(featureId);
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::SetUiOptions(
    /* [in] */ Int32 uiOptions)
{
    // ==================before translated======================
    // mUiOptions = uiOptions;
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::SetUiOptions(
    /* [in] */ Int32 uiOptions,
    /* [in] */ Int32 mask)
{
    // ==================before translated======================
    // mUiOptions = (mUiOptions & ~mask) | (uiOptions & mask);
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::GetTransitionManager(
    /* [out] */ ITransitionManager** tm)
{
    VALIDATE_NOT_NULL(tm)
    // ==================before translated======================
    // return mTransitionManager;
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::SetTransitionManager(
    /* [in] */ ITransitionManager* tm)
{
    VALIDATE_NOT_NULL(tm);
    // ==================before translated======================
    // mTransitionManager = tm;
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::GetContentScene(
    /* [out] */ IScene** scene)
{
    // ==================before translated======================
    // return mContentScene;
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::SetContentView(
    /* [in] */ Int32 layoutResID)
{
    // ==================before translated======================
    // // Note: FEATURE_CONTENT_TRANSITIONS may be set in the process of installing the window
    // // decor, when theme attributes and the like are crystalized. Do not check the feature
    // // before this happens.
    // if (mContentParent == null) {
    //     installDecor();
    // } else if (!hasFeature(FEATURE_CONTENT_TRANSITIONS)) {
    //     mContentParent.removeAllViews();
    // }
    //
    // if (hasFeature(FEATURE_CONTENT_TRANSITIONS)) {
    //     final Scene newScene = Scene.getSceneForLayout(mContentParent, layoutResID,
    //             getContext());
    //     transitionTo(newScene);
    // } else {
    //     mLayoutInflater.inflate(layoutResID, mContentParent);
    // }
    // final Callback cb = getCallback();
    // if (cb != null && !isDestroyed()) {
    //     cb.onContentChanged();
    // }
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::SetContentView(
    /* [in] */ IView* view)
{
    VALIDATE_NOT_NULL(view);
    // ==================before translated======================
    // setContentView(view, new ViewGroup.LayoutParams(MATCH_PARENT, MATCH_PARENT));
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::SetContentView(
    /* [in] */ IView* view,
    /* [in] */ IViewGroupLayoutParams* viewGroup)
{
    VALIDATE_NOT_NULL(view);
    // ==================before translated======================
    // // Note: FEATURE_CONTENT_TRANSITIONS may be set in the process of installing the window
    // // decor, when theme attributes and the like are crystalized. Do not check the feature
    // // before this happens.
    // if (mContentParent == null) {
    //     installDecor();
    // } else if (!hasFeature(FEATURE_CONTENT_TRANSITIONS)) {
    //     mContentParent.removeAllViews();
    // }
    //
    // if (hasFeature(FEATURE_CONTENT_TRANSITIONS)) {
    //     view.setLayoutParams(params);
    //     final Scene newScene = new Scene(mContentParent, view);
    //     transitionTo(newScene);
    // } else {
    //     mContentParent.addView(view, params);
    // }
    // final Callback cb = getCallback();
    // if (cb != null && !isDestroyed()) {
    //     cb.onContentChanged();
    // }
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::AddContentView(
    /* [in] */ IView* view,
    /* [in] */ IViewGroupLayoutParams* viewGroup)
{
    VALIDATE_NOT_NULL(view);
    // ==================before translated======================
    // if (mContentParent == null) {
    //     installDecor();
    // }
    // if (hasFeature(FEATURE_CONTENT_TRANSITIONS)) {
    //     // TODO Augment the scenes/transitions API to support this.
    //     Log.v(TAG, "addContentView does not support content transitions");
    // }
    // mContentParent.addView(view, params);
    // final Callback cb = getCallback();
    // if (cb != null && !isDestroyed()) {
    //     cb.onContentChanged();
    // }
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::GetCurrentFocus(
    /* [out] */ IView** view)
{
    // ==================before translated======================
    // return mDecor != null ? mDecor.findFocus() : null;
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::TakeSurface(
    /* [in] */ ISurfaceHolderCallback2* cb)
{
    // ==================before translated======================
    // mTakeSurfaceCallback = callback;
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::TakeInputQueue(
    /* [in] */ IInputQueueCallback* cb)
{
    // ==================before translated======================
    // mTakeInputQueueCallback = callback;
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::IsFloating(
    /* [out] */ Boolean* isFloating)
{
    // ==================before translated======================
    // return mIsFloating;
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::GetLayoutInflater(
    /* [out] */ ILayoutInflater** inflater)
{
    // ==================before translated======================
    // return mLayoutInflater;
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::SetTitle(
    /* [in] */ ICharSequence* title)
{
    VALIDATE_NOT_NULL(title);
    // ==================before translated======================
    // if (mTitleView != null) {
    //     mTitleView.setText(title);
    // } else if (mDecorContentParent != null) {
    //     mDecorContentParent.setWindowTitle(title);
    // }
    // mTitle = title;
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::SetTitleColor(
    /* [in] */ Int32 textColor)
{
    // ==================before translated======================
    // if (mTitleView != null) {
    //     mTitleView.setTextColor(textColor);
    // }
    // mTitleColor = textColor;
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::PreparePanel(
    /* [in] */ PanelFeatureState* st,
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(st);
    VALIDATE_NOT_NULL(event);
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // if (isDestroyed()) {
    //     return false;
    // }
    //
    // // Already prepared (isPrepared will be reset to false later)
    // if (st.isPrepared) {
    //     return true;
    // }
    //
    // if ((mPreparedPanel != null) && (mPreparedPanel != st)) {
    //     // Another Panel is prepared and possibly open, so close it
    //     closePanel(mPreparedPanel, false);
    // }
    //
    // final Callback cb = getCallback();
    //
    // if (cb != null) {
    //     st.createdPanelView = cb.onCreatePanelView(st.featureId);
    // }
    //
    // final boolean isActionBarMenu =
    //         (st.featureId == FEATURE_OPTIONS_PANEL || st.featureId == FEATURE_ACTION_BAR);
    //
    // if (isActionBarMenu && mDecorContentParent != null) {
    //     // Enforce ordering guarantees around events so that the action bar never
    //     // dispatches menu-related events before the panel is prepared.
    //     mDecorContentParent.setMenuPrepared();
    // }
    //
    // if (st.createdPanelView == null) {
    //     // Init the panel state's menu--return false if init failed
    //     if (st.menu == null || st.refreshMenuContent) {
    //         if (st.menu == null) {
    //             if (!initializePanelMenu(st) || (st.menu == null)) {
    //                 return false;
    //             }
    //         }
    //
    //         if (isActionBarMenu && mDecorContentParent != null) {
    //             if (mActionMenuPresenterCallback == null) {
    //                 mActionMenuPresenterCallback = new ActionMenuPresenterCallback();
    //             }
    //             mDecorContentParent.setMenu(st.menu, mActionMenuPresenterCallback);
    //         }
    //
    //         // Call callback, and return if it doesn't want to display menu.
    //
    //         // Creating the panel menu will involve a lot of manipulation;
    //         // don't dispatch change events to presenters until we're done.
    //         st.menu.stopDispatchingItemsChanged();
    //         if ((cb == null) || !cb.onCreatePanelMenu(st.featureId, st.menu)) {
    //             // Ditch the menu created above
    //             st.setMenu(null);
    //
    //             if (isActionBarMenu && mDecorContentParent != null) {
    //                 // Don't show it in the action bar either
    //                 mDecorContentParent.setMenu(null, mActionMenuPresenterCallback);
    //             }
    //
    //             return false;
    //         }
    //
    //         st.refreshMenuContent = false;
    //     }
    //
    //     // Callback and return if the callback does not want to show the menu
    //
    //     // Preparing the panel menu can involve a lot of manipulation;
    //     // don't dispatch change events to presenters until we're done.
    //     st.menu.stopDispatchingItemsChanged();
    //
    //     // Restore action view state before we prepare. This gives apps
    //     // an opportunity to override frozen/restored state in onPrepare.
    //     if (st.frozenActionViewState != null) {
    //         st.menu.restoreActionViewStates(st.frozenActionViewState);
    //         st.frozenActionViewState = null;
    //     }
    //
    //     if (!cb.onPreparePanel(st.featureId, st.createdPanelView, st.menu)) {
    //         if (isActionBarMenu && mDecorContentParent != null) {
    //             // The app didn't want to show the menu for now but it still exists.
    //             // Clear it out of the action bar.
    //             mDecorContentParent.setMenu(null, mActionMenuPresenterCallback);
    //         }
    //         st.menu.startDispatchingItemsChanged();
    //         return false;
    //     }
    //
    //     // Set the proper keymap
    //     KeyCharacterMap kmap = KeyCharacterMap.load(
    //             event != null ? event.getDeviceId() : KeyCharacterMap.VIRTUAL_KEYBOARD);
    //     st.qwertyMode = kmap.getKeyboardType() != KeyCharacterMap.NUMERIC;
    //     st.menu.setQwertyMode(st.qwertyMode);
    //     st.menu.startDispatchingItemsChanged();
    // }
    //
    // // Set other state
    // st.isPrepared = true;
    // st.isHandled = false;
    // mPreparedPanel = st;
    //
    // return true;
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::OnConfigurationChanged(
    /* [in] */ IConfiguration* newConfig)
{
    VALIDATE_NOT_NULL(newConfig);
    // ==================before translated======================
    // // Action bars handle their own menu state
    // if (mDecorContentParent == null) {
    //     PanelFeatureState st = getPanelState(FEATURE_OPTIONS_PANEL, false);
    //     if ((st != null) && (st.menu != null)) {
    //         if (st.isOpen) {
    //             // Freeze state
    //             final Bundle state = new Bundle();
    //             if (st.iconMenuPresenter != null) {
    //                 st.iconMenuPresenter.saveHierarchyState(state);
    //             }
    //             if (st.listMenuPresenter != null) {
    //                 st.listMenuPresenter.saveHierarchyState(state);
    //             }
    //
    //             // Remove the menu views since they need to be recreated
    //             // according to the new configuration
    //             clearMenuViews(st);
    //
    //             // Re-open the same menu
    //             reopenMenu(false);
    //
    //             // Restore state
    //             if (st.iconMenuPresenter != null) {
    //                 st.iconMenuPresenter.restoreHierarchyState(state);
    //             }
    //             if (st.listMenuPresenter != null) {
    //                 st.listMenuPresenter.restoreHierarchyState(state);
    //             }
    //
    //         } else {
    //             // Clear menu views so on next menu opening, it will use
    //             // the proper layout
    //             clearMenuViews(st);
    //         }
    //     }
    // }
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::OpenPanel(
    /* [in] */ Int32 featureId,
    /* [in] */ IKeyEvent* event)
{
    VALIDATE_NOT_NULL(event);
    // ==================before translated======================
    // if (featureId == FEATURE_OPTIONS_PANEL && mDecorContentParent != null &&
    //         mDecorContentParent.canShowOverflowMenu() &&
    //         !ViewConfiguration.get(getContext()).hasPermanentMenuKey()) {
    //     mDecorContentParent.showOverflowMenu();
    // } else {
    //     openPanel(getPanelState(featureId, true), event);
    // }
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::ClosePanel(
    /* [in] */ Int32 featureId)
{
    // ==================before translated======================
    // if (featureId == FEATURE_OPTIONS_PANEL && mDecorContentParent != null &&
    //         mDecorContentParent.canShowOverflowMenu() &&
    //         !ViewConfiguration.get(getContext()).hasPermanentMenuKey()) {
    //     mDecorContentParent.hideOverflowMenu();
    // } else if (featureId == FEATURE_CONTEXT_MENU) {
    //     closeContextMenu();
    // } else {
    //     closePanel(getPanelState(featureId, true), true);
    // }
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::ClosePanel(
    /* [in] */ PanelFeatureState* st,
    /* [in] */ Boolean doCallback)
{
    VALIDATE_NOT_NULL(st);
    // ==================before translated======================
    // // System.out.println("Close panel: isOpen=" + st.isOpen);
    // if (doCallback && st.featureId == FEATURE_OPTIONS_PANEL &&
    //         mDecorContentParent != null && mDecorContentParent.isOverflowMenuShowing()) {
    //     checkCloseActionMenu(st.menu);
    //     return;
    // }
    //
    // final ViewManager wm = getWindowManager();
    // if ((wm != null) && st.isOpen) {
    //     if (st.decorView != null) {
    //         wm.removeView(st.decorView);
    //         // Log.v(TAG, "Removing main menu from window manager.");
    //         if (st.isCompact) {
    //             sRotationWatcher.removeWindow(this);
    //         }
    //     }
    //
    //     if (doCallback) {
    //         callOnPanelClosed(st.featureId, st, null);
    //     }
    // }
    //
    // st.isPrepared = false;
    // st.isHandled = false;
    // st.isOpen = false;
    //
    // // This view is no longer shown, so null it out
    // st.shownPanelView = null;
    //
    // if (st.isInExpandedMode) {
    //     // Next time the menu opens, it should not be in expanded mode, so
    //     // force a refresh of the decor
    //     st.refreshDecorView = true;
    //     st.isInExpandedMode = false;
    // }
    //
    // if (mPreparedPanel == st) {
    //     mPreparedPanel = null;
    //     mPanelChordingKey = 0;
    // }
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::CheckCloseActionMenu(
    /* [in] */ IMenu* menu)
{
    VALIDATE_NOT_NULL(menu);
    // ==================before translated======================
    // if (mClosingActionMenu) {
    //     return;
    // }
    //
    // mClosingActionMenu = true;
    // mDecorContentParent.dismissPopups();
    // Callback cb = getCallback();
    // if (cb != null && !isDestroyed()) {
    //     cb.onPanelClosed(FEATURE_ACTION_BAR, menu);
    // }
    // mClosingActionMenu = false;
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::TogglePanel(
    /* [in] */ Int32 featureId,
    /* [in] */ IKeyEvent* event)
{
    VALIDATE_NOT_NULL(event);
    // ==================before translated======================
    // PanelFeatureState st = getPanelState(featureId, true);
    // if (st.isOpen) {
    //     closePanel(st, true);
    // } else {
    //     openPanel(st, event);
    // }
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::InvalidatePanelMenu(
    /* [in] */ Int32 featureId)
{
    // ==================before translated======================
    // mInvalidatePanelMenuFeatures |= 1 << featureId;
    //
    // if (!mInvalidatePanelMenuPosted && mDecor != null) {
    //     mDecor.postOnAnimation(mInvalidatePanelMenuRunnable);
    //     mInvalidatePanelMenuPosted = true;
    // }
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::DoPendingInvalidatePanelMenu()
{
    // ==================before translated======================
    // if (mInvalidatePanelMenuPosted) {
    //     mDecor.removeCallbacks(mInvalidatePanelMenuRunnable);
    //     mInvalidatePanelMenuRunnable.run();
    // }
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::DoInvalidatePanelMenu(
    /* [in] */ Int32 featureId)
{
    // ==================before translated======================
    // PanelFeatureState st = getPanelState(featureId, true);
    // Bundle savedActionViewStates = null;
    // if (st.menu != null) {
    //     savedActionViewStates = new Bundle();
    //     st.menu.saveActionViewStates(savedActionViewStates);
    //     if (savedActionViewStates.size() > 0) {
    //         st.frozenActionViewState = savedActionViewStates;
    //     }
    //     // This will be started again when the panel is prepared.
    //     st.menu.stopDispatchingItemsChanged();
    //     st.menu.clear();
    // }
    // st.refreshMenuContent = true;
    // st.refreshDecorView = true;
    //
    // // Prepare the options panel if we have an action bar
    // if ((featureId == FEATURE_ACTION_BAR || featureId == FEATURE_OPTIONS_PANEL)
    //         && mDecorContentParent != null) {
    //     st = getPanelState(Window.FEATURE_OPTIONS_PANEL, false);
    //     if (st != null) {
    //         st.isPrepared = false;
    //         preparePanel(st, null);
    //     }
    // }
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::OnKeyDownPanel(
    /* [in] */ Int32 featureId,
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(event);
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // final int keyCode = event.getKeyCode();
    //
    // if (event.getRepeatCount() == 0) {
    //     // The panel key was pushed, so set the chording key
    //     mPanelChordingKey = keyCode;
    //
    //     PanelFeatureState st = getPanelState(featureId, true);
    //     if (!st.isOpen) {
    //         return preparePanel(st, event);
    //     }
    // }
    //
    // return false;
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::OnKeyUpPanel(
    /* [in] */ Int32 featureId,
    /* [in] */ IKeyEvent* event)
{
    VALIDATE_NOT_NULL(event);
    // ==================before translated======================
    // // The panel key was released, so clear the chording key
    // if (mPanelChordingKey != 0) {
    //     mPanelChordingKey = 0;
    //
    //     if (event.isCanceled() || (mDecor != null && mDecor.mActionMode != null)) {
    //         return;
    //     }
    //
    //     boolean playSoundEffect = false;
    //     final PanelFeatureState st = getPanelState(featureId, true);
    //     if (featureId == FEATURE_OPTIONS_PANEL && mDecorContentParent != null &&
    //             mDecorContentParent.canShowOverflowMenu() &&
    //             !ViewConfiguration.get(getContext()).hasPermanentMenuKey()) {
    //         if (!mDecorContentParent.isOverflowMenuShowing()) {
    //             if (!isDestroyed() && preparePanel(st, event)) {
    //                 playSoundEffect = mDecorContentParent.showOverflowMenu();
    //             }
    //         } else {
    //             playSoundEffect = mDecorContentParent.hideOverflowMenu();
    //         }
    //     } else {
    //         if (st.isOpen || st.isHandled) {
    //
    //             // Play the sound effect if the user closed an open menu (and not if
    //             // they just released a menu shortcut)
    //             playSoundEffect = st.isOpen;
    //
    //             // Close menu
    //             closePanel(st, true);
    //
    //         } else if (st.isPrepared) {
    //             boolean show = true;
    //             if (st.refreshMenuContent) {
    //                 // Something may have invalidated the menu since we prepared it.
    //                 // Re-prepare it to refresh.
    //                 st.isPrepared = false;
    //                 show = preparePanel(st, event);
    //             }
    //
    //             if (show) {
    //                 // Write 'menu opened' to event log
    //                 EventLog.writeEvent(50001, 0);
    //
    //                 // Show menu
    //                 openPanel(st, event);
    //
    //                 playSoundEffect = true;
    //             }
    //         }
    //     }
    //
    //     if (playSoundEffect) {
    //         AudioManager audioManager = (AudioManager) getContext().getSystemService(
    //                 Context.AUDIO_SERVICE);
    //         if (audioManager != null) {
    //             audioManager.playSoundEffect(AudioManager.FX_KEY_CLICK);
    //         } else {
    //             Log.w(TAG, "Couldn't get audio manager");
    //         }
    //     }
    // }
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::CloseAllPanels()
{
    // ==================before translated======================
    // final ViewManager wm = getWindowManager();
    // if (wm == null) {
    //     return;
    // }
    //
    // final PanelFeatureState[] panels = mPanels;
    // final int N = panels != null ? panels.length : 0;
    // for (int i = 0; i < N; i++) {
    //     final PanelFeatureState panel = panels[i];
    //     if (panel != null) {
    //         closePanel(panel, true);
    //     }
    // }
    //
    // closeContextMenu();
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::PerformPanelShortcut(
    /* [in] */ Int32 featureId,
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event,
    /* [in] */ Int32 flags,
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded);
    // ==================before translated======================
    // return performPanelShortcut(getPanelState(featureId, true), keyCode, event, flags);
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::PerformPanelIdentifierAction(
    /* [in] */ Int32 featureId,
    /* [in] */ Int32 id,
    /* [in] */ Int32 flags,
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded);
    // ==================before translated======================
    //
    // PanelFeatureState st = getPanelState(featureId, true);
    // if (!preparePanel(st, new KeyEvent(KeyEvent.ACTION_DOWN, KeyEvent.KEYCODE_MENU))) {
    //     return false;
    // }
    // if (st.menu == null) {
    //     return false;
    // }
    //
    // boolean res = st.menu.performIdentifierAction(id, flags);
    //
    // // Only close down the menu if we don't have an action bar keeping it open.
    // if (mDecorContentParent == null) {
    //     closePanel(st, true);
    // }
    //
    // return res;
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::FindMenuPanel(
    /* [in] */ IMenu* menu,
    /* [out] */ PanelFeatureState** result)
{
    VALIDATE_NOT_NULL(menu);
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // final PanelFeatureState[] panels = mPanels;
    // final int N = panels != null ? panels.length : 0;
    // for (int i = 0; i < N; i++) {
    //     final PanelFeatureState panel = panels[i];
    //     if (panel != null && panel.menu == menu) {
    //         return panel;
    //     }
    // }
    // return null;
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::OnMenuItemSelected(
    /* [in] */ IMenuBuilder* menu,
    /* [in] */ IMenuItem* item,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(menu);
    VALIDATE_NOT_NULL(item);
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // final Callback cb = getCallback();
    // if (cb != null && !isDestroyed()) {
    //     final PanelFeatureState panel = findMenuPanel(menu.getRootMenu());
    //     if (panel != null) {
    //         return cb.onMenuItemSelected(panel.featureId, item);
    //     }
    // }
    // return false;
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::OnMenuModeChange(
    /* [in] */ IMenuBuilder* menu)
{
    VALIDATE_NOT_NULL(menu);
    // ==================before translated======================
    // reopenMenu(true);
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::OnOptionsPanelRotationChanged()
{
    // ==================before translated======================
    // final PanelFeatureState st = getPanelState(FEATURE_OPTIONS_PANEL, false);
    // if (st == null) return;
    //
    // final WindowManager.LayoutParams lp = st.decorView != null ?
    //         (WindowManager.LayoutParams) st.decorView.getLayoutParams() : null;
    // if (lp != null) {
    //     lp.gravity = getOptionsPanelGravity();
    //     final ViewManager wm = getWindowManager();
    //     if (wm != null) {
    //         wm.updateViewLayout(st.decorView, lp);
    //     }
    // }
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::PerformContextMenuIdentifierAction(
    /* [in] */ Int32 id,
    /* [in] */ Int32 flags,
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded);
    // ==================before translated======================
    // return (mContextMenu != null) ? mContextMenu.performIdentifierAction(id, flags) : false;
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::SetBackgroundDrawable(
    /* [in] */ IDrawable* drawable)
{
    VALIDATE_NOT_NULL(drawable);
    // ==================before translated======================
    // if (drawable != mBackgroundDrawable || mBackgroundResource != 0) {
    //     mBackgroundResource = 0;
    //     mBackgroundDrawable = drawable;
    //     if (mDecor != null) {
    //         mDecor.setWindowBackground(drawable);
    //     }
    //     if (mBackgroundFallbackResource != 0) {
    //         mDecor.setBackgroundFallback(drawable != null ? 0 : mBackgroundFallbackResource);
    //     }
    // }
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::SetFeatureDrawableResource(
    /* [in] */ Int32 featureId,
    /* [in] */ Int32 resId)
{
    // ==================before translated======================
    // if (resId != 0) {
    //     DrawableFeatureState st = getDrawableState(featureId, true);
    //     if (st.resid != resId) {
    //         st.resid = resId;
    //         st.uri = null;
    //         st.local = getContext().getDrawable(resId);
    //         updateDrawable(featureId, st, false);
    //     }
    // } else {
    //     setFeatureDrawable(featureId, null);
    // }
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::SetFeatureDrawableUri(
    /* [in] */ Int32 featureId,
    /* [in] */ IUri* uri)
{
    VALIDATE_NOT_NULL(uri);
    // ==================before translated======================
    // if (uri != null) {
    //     DrawableFeatureState st = getDrawableState(featureId, true);
    //     if (st.uri == null || !st.uri.equals(uri)) {
    //         st.resid = 0;
    //         st.uri = uri;
    //         st.local = loadImageURI(uri);
    //         updateDrawable(featureId, st, false);
    //     }
    // } else {
    //     setFeatureDrawable(featureId, null);
    // }
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::SetFeatureDrawable(
    /* [in] */ Int32 featureId,
    /* [in] */ IDrawable* drawable)
{
    VALIDATE_NOT_NULL(drawable);
    // ==================before translated======================
    // DrawableFeatureState st = getDrawableState(featureId, true);
    // st.resid = 0;
    // st.uri = null;
    // if (st.local != drawable) {
    //     st.local = drawable;
    //     updateDrawable(featureId, st, false);
    // }
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::SetFeatureDrawableAlpha(
    /* [in] */ Int32 featureId,
    /* [in] */ Int32 alpha)
{
    // ==================before translated======================
    // DrawableFeatureState st = getDrawableState(featureId, true);
    // if (st.alpha != alpha) {
    //     st.alpha = alpha;
    //     updateDrawable(featureId, st, false);
    // }
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::SetFeatureInt(
    /* [in] */ Int32 featureId,
    /* [in] */ Int32 value)
{
    // ==================before translated======================
    // // XXX Should do more management (as with drawable features) to
    // // deal with interactions between multiple window policies.
    // updateInt(featureId, value, false);
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::SetIcon(
    /* [in] */ Int32 resId)
{
    // ==================before translated======================
    // mIconRes = resId;
    // mResourcesSetFlags |= FLAG_RESOURCE_SET_ICON;
    // mResourcesSetFlags &= ~FLAG_RESOURCE_SET_ICON_FALLBACK;
    // if (mDecorContentParent != null) {
    //     mDecorContentParent.setIcon(resId);
    // }
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::SetDefaultIcon(
    /* [in] */ Int32 resId)
{
    // ==================before translated======================
    // if ((mResourcesSetFlags & FLAG_RESOURCE_SET_ICON) != 0) {
    //     return;
    // }
    // mIconRes = resId;
    // if (mDecorContentParent != null && (!mDecorContentParent.hasIcon() ||
    //         (mResourcesSetFlags & FLAG_RESOURCE_SET_ICON_FALLBACK) != 0)) {
    //     if (resId != 0) {
    //         mDecorContentParent.setIcon(resId);
    //         mResourcesSetFlags &= ~FLAG_RESOURCE_SET_ICON_FALLBACK;
    //     } else {
    //         mDecorContentParent.setIcon(
    //                 getContext().getPackageManager().getDefaultActivityIcon());
    //         mResourcesSetFlags |= FLAG_RESOURCE_SET_ICON_FALLBACK;
    //     }
    // }
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::SetLogo(
    /* [in] */ Int32 resId)
{
    // ==================before translated======================
    // mLogoRes = resId;
    // mResourcesSetFlags |= FLAG_RESOURCE_SET_LOGO;
    // if (mDecorContentParent != null) {
    //     mDecorContentParent.setLogo(resId);
    // }
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::SetDefaultLogo(
    /* [in] */ Int32 resId)
{
    // ==================before translated======================
    // if ((mResourcesSetFlags & FLAG_RESOURCE_SET_LOGO) != 0) {
    //     return;
    // }
    // mLogoRes = resId;
    // if (mDecorContentParent != null && !mDecorContentParent.hasLogo()) {
    //     mDecorContentParent.setLogo(resId);
    // }
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::SetLocalFocus(
    /* [in] */ Boolean hasFocus,
    /* [in] */ Boolean inTouchMode)
{
    // ==================before translated======================
    // getViewRootImpl().windowFocusChanged(hasFocus, inTouchMode);
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::InjectInputEvent(
    /* [in] */ IInputEvent* event)
{
    VALIDATE_NOT_NULL(event);
    // ==================before translated======================
    // getViewRootImpl().dispatchInputEvent(event);
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::TakeKeyEvents(
    /* [in] */ Boolean get)
{
    // ==================before translated======================
    // mDecor.setFocusable(get);
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::SuperDispatchKeyEvent(
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded);
    // ==================before translated======================
    // return mDecor.superDispatchKeyEvent(event);
    assert(0);
    return FALSE;
}

ECode PhoneWindow::SuperDispatchKeyShortcutEvent(
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded);
    // ==================before translated======================
    // return mDecor.superDispatchKeyShortcutEvent(event);
    assert(0);
    return FALSE;
}

ECode PhoneWindow::SuperDispatchTouchEvent(
    /* [in] */ IMotionEvent* event,
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded);
    // ==================before translated======================
    // return mDecor.superDispatchTouchEvent(event);
    assert(0);
    return FALSE;
}

ECode PhoneWindow::SuperDispatchTrackballEvent(
    /* [in] */ IMotionEvent* event,
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded);
    // ==================before translated======================
    // return mDecor.superDispatchTrackballEvent(event);
    assert(0);
    return FALSE;
}

ECode PhoneWindow::SuperDispatchGenericMotionEvent(
    /* [in] */ IMotionEvent* event,
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded);
    // ==================before translated======================
    // return mDecor.superDispatchGenericMotionEvent(event);
    assert(0);
    return FALSE;
}

ECode PhoneWindow::GetAudioManager(
    /* [out] */ IAudioManager** result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // if (mAudioManager == null) {
    //     mAudioManager = (AudioManager)getContext().getSystemService(Context.AUDIO_SERVICE);
    // }
    // return mAudioManager;
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::GetDecorView(
    /* [out] */ IView** view)
{
    VALIDATE_NOT_NULL(view);
    // ==================before translated======================
    // if (mDecor == null) {
    //     installDecor();
    // }
    // return mDecor;
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::PeekDecorView(
    /* [out] */ IView** view)
{
    VALIDATE_NOT_NULL(view);
    // ==================before translated======================
    // return mDecor;
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::SaveHierarchyState(
    /* [out] */ IBundle** instanceState)
{
    VALIDATE_NOT_NULL(instanceState);
    // ==================before translated======================
    // Bundle outState = new Bundle();
    // if (mContentParent == null) {
    //     return outState;
    // }
    //
    // SparseArray<Parcelable> states = new SparseArray<Parcelable>();
    // mContentParent.saveHierarchyState(states);
    // outState.putSparseParcelableArray(VIEWS_TAG, states);
    //
    // // save the focused view id
    // View focusedView = mContentParent.findFocus();
    // if (focusedView != null) {
    //     if (focusedView.getId() != View.NO_ID) {
    //         outState.putInt(FOCUSED_ID_TAG, focusedView.getId());
    //     } else {
    //         if (false) {
    //             Log.d(TAG, "couldn't save which view has focus because the focused view "
    //                     + focusedView + " has no id.");
    //         }
    //     }
    // }
    //
    // // save the panels
    // SparseArray<Parcelable> panelStates = new SparseArray<Parcelable>();
    // savePanelState(panelStates);
    // if (panelStates.size() > 0) {
    //     outState.putSparseParcelableArray(PANELS_TAG, panelStates);
    // }
    //
    // if (mDecorContentParent != null) {
    //     SparseArray<Parcelable> actionBarStates = new SparseArray<Parcelable>();
    //     mDecorContentParent.saveToolbarHierarchyState(actionBarStates);
    //     outState.putSparseParcelableArray(ACTION_BAR_TAG, actionBarStates);
    // }
    //
    // return outState;
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::RestoreHierarchyState(
    /* [in] */ IBundle* savedInstanceState)
{
    VALIDATE_NOT_NULL(savedInstanceState);
    // ==================before translated======================
    // if (mContentParent == null) {
    //     return;
    // }
    //
    // SparseArray<Parcelable> savedStates
    //         = savedInstanceState.getSparseParcelableArray(VIEWS_TAG);
    // if (savedStates != null) {
    //     mContentParent.restoreHierarchyState(savedStates);
    // }
    //
    // // restore the focused view
    // int focusedViewId = savedInstanceState.getInt(FOCUSED_ID_TAG, View.NO_ID);
    // if (focusedViewId != View.NO_ID) {
    //     View needsFocus = mContentParent.findViewById(focusedViewId);
    //     if (needsFocus != null) {
    //         needsFocus.requestFocus();
    //     } else {
    //         Log.w(TAG,
    //                 "Previously focused view reported id " + focusedViewId
    //                         + " during save, but can't be found during restore.");
    //     }
    // }
    //
    // // restore the panels
    // SparseArray<Parcelable> panelStates = savedInstanceState.getSparseParcelableArray(PANELS_TAG);
    // if (panelStates != null) {
    //     restorePanelState(panelStates);
    // }
    //
    // if (mDecorContentParent != null) {
    //     SparseArray<Parcelable> actionBarStates =
    //             savedInstanceState.getSparseParcelableArray(ACTION_BAR_TAG);
    //     if (actionBarStates != null) {
    //         doPendingInvalidatePanelMenu();
    //         mDecorContentParent.restoreToolbarHierarchyState(actionBarStates);
    //     } else {
    //         Log.w(TAG, "Missing saved instance states for action bar views! " +
    //                 "State will not be restored.");
    //     }
    // }
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::AlwaysReadCloseOnTouchAttr()
{
    // ==================before translated======================
    // mAlwaysReadCloseOnTouchAttr = true;
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::SetChildDrawable(
    /* [in] */ Int32 featureId,
    /* [in] */ IDrawable* drawable)
{
    VALIDATE_NOT_NULL(drawable);
    // ==================before translated======================
    // DrawableFeatureState st = getDrawableState(featureId, true);
    // st.child = drawable;
    // updateDrawable(featureId, st, false);
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::SetChildInt(
    /* [in] */ Int32 featureId,
    /* [in] */ Int32 value)
{
    // ==================before translated======================
    // updateInt(featureId, value, false);
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::IsShortcutKey(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* isShortcutKey)
{
    VALIDATE_NOT_NULL(isShortcutKey);
    // ==================before translated======================
    // PanelFeatureState st = getPanelState(FEATURE_OPTIONS_PANEL, true);
    // return st.menu != null && st.menu.isShortcutKey(keyCode, event);
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::SetVolumeControlStream(
    /* [in] */ Int32 streamType)
{
    // ==================before translated======================
    // mVolumeControlStreamType = streamType;
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::GetVolumeControlStream(
    /* [out] */ Int32* streamType)
{
    VALIDATE_NOT_NULL(streamType);
    // ==================before translated======================
    // return mVolumeControlStreamType;
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::SetMediaController(
    /* [in] */ IMediaController* controller)
{
    VALIDATE_NOT_NULL(controller);
    // ==================before translated======================
    // mMediaController = controller;
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::GetMediaController(
    /* [out] */ IMediaController** controller)
{
    VALIDATE_NOT_NULL(controller);
    // ==================before translated======================
    // return mMediaController;
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::SetEnterTransition(
    /* [in] */ ITransition* enterTransition)
{
    VALIDATE_NOT_NULL(enterTransition);
    // ==================before translated======================
    // mEnterTransition = enterTransition;
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::SetReturnTransition(
    /* [in] */ ITransition* transition)
{
    VALIDATE_NOT_NULL(transition);
    // ==================before translated======================
    // mReturnTransition = transition;
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::SetExitTransition(
    /* [in] */ ITransition* exitTransition)
{
    VALIDATE_NOT_NULL(exitTransition);
    // ==================before translated======================
    // mExitTransition = exitTransition;
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::SetReenterTransition(
    /* [in] */ ITransition* transition)
{
    VALIDATE_NOT_NULL(transition);
    // ==================before translated======================
    // mReenterTransition = transition;
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::SetSharedElementEnterTransition(
    /* [in] */ ITransition* sharedElementEnterTransition)
{
    VALIDATE_NOT_NULL(sharedElementEnterTransition);
    // ==================before translated======================
    // mSharedElementEnterTransition = sharedElementEnterTransition;
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::SetSharedElementReturnTransition(
    /* [in] */ ITransition* transition)
{
    VALIDATE_NOT_NULL(transition);
    // ==================before translated======================
    // mSharedElementReturnTransition = transition;
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::SetSharedElementExitTransition(
    /* [in] */ ITransition* sharedElementExitTransition)
{
    VALIDATE_NOT_NULL(sharedElementExitTransition);
    // ==================before translated======================
    // mSharedElementExitTransition = sharedElementExitTransition;
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::SetSharedElementReenterTransition(
    /* [in] */ ITransition* transition)
{
    VALIDATE_NOT_NULL(transition);
    // ==================before translated======================
    // mSharedElementReenterTransition = transition;
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::GetEnterTransition(
    /* [out] */ ITransition** transition)
{
    VALIDATE_NOT_NULL(transition);
    // ==================before translated======================
    // return mEnterTransition;
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::GetReturnTransition(
    /* [out] */ ITransition** transition)
{
    VALIDATE_NOT_NULL(transition);
    // ==================before translated======================
    // return mReturnTransition == USE_DEFAULT_TRANSITION ? getEnterTransition()
    //         : mReturnTransition;
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::GetExitTransition(
    /* [out] */ ITransition** transition)
{
    VALIDATE_NOT_NULL(transition);
    // ==================before translated======================
    // return mExitTransition;
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::GetReenterTransition(
    /* [out] */ ITransition** transition)
{
    VALIDATE_NOT_NULL(transition);
    // ==================before translated======================
    // return mReenterTransition == USE_DEFAULT_TRANSITION ? getExitTransition()
    //         : mReenterTransition;
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::GetSharedElementEnterTransition(
    /* [out] */ ITransition** transition)
{
    VALIDATE_NOT_NULL(transition);
    // ==================before translated======================
    // return mSharedElementEnterTransition;
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::GetSharedElementReturnTransition(
    /* [out] */ ITransition** transition)
{
    VALIDATE_NOT_NULL(transition);
    // ==================before translated======================
    // return mSharedElementReturnTransition == USE_DEFAULT_TRANSITION
    //         ? getSharedElementEnterTransition() : mSharedElementReturnTransition;
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::GetSharedElementExitTransition(
    /* [out] */ ITransition** transition)
{
    VALIDATE_NOT_NULL(transition);
    // ==================before translated======================
    // return mSharedElementExitTransition;
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::GetSharedElementReenterTransition(
    /* [out] */ ITransition** transition)
{
    VALIDATE_NOT_NULL(transition);
    // ==================before translated======================
    // return mSharedElementReenterTransition == USE_DEFAULT_TRANSITION
    //         ? getSharedElementExitTransition() : mSharedElementReenterTransition;
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::SetAllowEnterTransitionOverlap(
    /* [in] */ Boolean allow)
{
    // ==================before translated======================
    // mAllowEnterTransitionOverlap = allow;
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::GetAllowEnterTransitionOverlap(
    /* [out] */ Boolean* allow)
{
    VALIDATE_NOT_NULL(allow);
    // ==================before translated======================
    // return (mAllowEnterTransitionOverlap == null) ? true : mAllowEnterTransitionOverlap;
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::SetAllowReturnTransitionOverlap(
    /* [in] */ Boolean allowExitTransitionOverlap)
{
    // ==================before translated======================
    // mAllowReturnTransitionOverlap = allowExitTransitionOverlap;
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::GetAllowReturnTransitionOverlap(
    /* [out] */ Boolean* allow)
{
    VALIDATE_NOT_NULL(allow);
    // ==================before translated======================
    // return (mAllowReturnTransitionOverlap == null) ? true : mAllowReturnTransitionOverlap;
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::GetTransitionBackgroundFadeDuration(
    /* [out] */ Int64* duration)
{
    VALIDATE_NOT_NULL(duration);
    // ==================before translated======================
    // return (mBackgroundFadeDurationMillis < 0) ? DEFAULT_BACKGROUND_FADE_DURATION_MS
    //         : mBackgroundFadeDurationMillis;
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::SetTransitionBackgroundFadeDuration(
    /* [in] */ Int64 fadeDurationMillis)
{
    // ==================before translated======================
    // if (fadeDurationMillis < 0) {
    //     throw new IllegalArgumentException("negative durations are not allowed");
    // }
    // mBackgroundFadeDurationMillis = fadeDurationMillis;
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::SetSharedElementsUseOverlay(
    /* [in] */ Boolean sharedElementsUseOverlay)
{
    // ==================before translated======================
    // mSharedElementsUseOverlay = sharedElementsUseOverlay;
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::GetSharedElementsUseOverlay(
    /* [out] */ Boolean* shared)
{
    VALIDATE_NOT_NULL(shared);
    // ==================before translated======================
    // return (mSharedElementsUseOverlay == null) ? true : mSharedElementsUseOverlay;
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::SendCloseSystemWindows()
{
    // ==================before translated======================
    // PhoneWindowManager.sendCloseSystemWindows(getContext(), null);
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::SendCloseSystemWindows(
    /* [in] */ const String& reason)
{
    // ==================before translated======================
    // PhoneWindowManager.sendCloseSystemWindows(getContext(), reason);
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::GetStatusBarColor(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return mStatusBarColor;
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::SetStatusBarColor(
    /* [in] */ Int32 color)
{
    // ==================before translated======================
    // mStatusBarColor = color;
    // mForcedStatusBarColor = true;
    // if (mDecor != null) {
    //     mDecor.updateColorViews(null);
    // }
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::GetNavigationBarColor(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return mNavigationBarColor;
    assert(0);
    return NOERROR;
}

ECode PhoneWindow::SetNavigationBarColor(
    /* [in] */ Int32 color)
{
    // ==================before translated======================
    // mNavigationBarColor = color;
    // mForcedNavigationBarColor = true;
    // if (mDecor != null) {
    //     mDecor.updateColorViews(null);
    // }
    assert(0);
    return NOERROR;
}

Boolean PhoneWindow::InitializePanelMenu(
    /* [in] */ PanelFeatureState* st)
{
    // ==================before translated======================
    // Context context = getContext();
    //
    // // If we have an action bar, initialize the menu with the right theme.
    // if ((st.featureId == FEATURE_OPTIONS_PANEL || st.featureId == FEATURE_ACTION_BAR) &&
    //         mDecorContentParent != null) {
    //     final TypedValue outValue = new TypedValue();
    //     final Theme baseTheme = context.getTheme();
    //     baseTheme.resolveAttribute(R.attr.actionBarTheme, outValue, true);
    //
    //     Theme widgetTheme = null;
    //     if (outValue.resourceId != 0) {
    //         widgetTheme = context.getResources().newTheme();
    //         widgetTheme.setTo(baseTheme);
    //         widgetTheme.applyStyle(outValue.resourceId, true);
    //         widgetTheme.resolveAttribute(
    //                 R.attr.actionBarWidgetTheme, outValue, true);
    //     } else {
    //         baseTheme.resolveAttribute(
    //                 R.attr.actionBarWidgetTheme, outValue, true);
    //     }
    //
    //     if (outValue.resourceId != 0) {
    //         if (widgetTheme == null) {
    //             widgetTheme = context.getResources().newTheme();
    //             widgetTheme.setTo(baseTheme);
    //         }
    //         widgetTheme.applyStyle(outValue.resourceId, true);
    //     }
    //
    //     if (widgetTheme != null) {
    //         context = new ContextThemeWrapper(context, 0);
    //         context.getTheme().setTo(widgetTheme);
    //     }
    // }
    //
    // final MenuBuilder menu = new MenuBuilder(context);
    // menu.setCallback(this);
    // st.setMenu(menu);
    //
    // return true;
    assert(0);
    return FALSE;
}

Boolean PhoneWindow::InitializePanelDecor(
    /* [in] */ PanelFeatureState* st)
{
    // ==================before translated======================
    // st.decorView = new DecorView(getContext(), st.featureId);
    // st.gravity = Gravity.CENTER | Gravity.BOTTOM;
    // st.setStyle(getContext());
    // TypedArray a = getContext().obtainStyledAttributes(null,
    //         R.styleable.Window, 0, st.listPresenterTheme);
    // final float elevation = a.getDimension(R.styleable.Window_windowElevation, 0);
    // if (elevation != 0) {
    //     st.decorView.setElevation(elevation);
    // }
    // a.recycle();
    //
    // return true;
    assert(0);
    return FALSE;
}

Boolean PhoneWindow::InitializePanelContent(
    /* [in] */ PanelFeatureState* st)
{
    // ==================before translated======================
    // if (st.createdPanelView != null) {
    //     st.shownPanelView = st.createdPanelView;
    //     return true;
    // }
    //
    // if (st.menu == null) {
    //     return false;
    // }
    //
    // if (mPanelMenuPresenterCallback == null) {
    //     mPanelMenuPresenterCallback = new PanelMenuPresenterCallback();
    // }
    //
    // MenuView menuView = st.isInListMode()
    //         ? st.getListMenuView(getContext(), mPanelMenuPresenterCallback)
    //         : st.getIconMenuView(getContext(), mPanelMenuPresenterCallback);
    //
    // st.shownPanelView = (View) menuView;
    //
    // if (st.shownPanelView != null) {
    //     // Use the menu View's default animations if it has any
    //     final int defaultAnimations = menuView.getWindowAnimations();
    //     if (defaultAnimations != 0) {
    //         st.windowAnimations = defaultAnimations;
    //     }
    //     return true;
    // } else {
    //     return false;
    // }
    assert(0);
    return FALSE;
}

void PhoneWindow::SetFeatureDefaultDrawable(
    /* [in] */ Int32 featureId,
    /* [in] */ IDrawable* drawable)
{
    // ==================before translated======================
    // DrawableFeatureState st = getDrawableState(featureId, true);
    // if (st.def != drawable) {
    //     st.def = drawable;
    //     updateDrawable(featureId, st, false);
    // }
    assert(0);
}

void PhoneWindow::UpdateDrawable(
    /* [in] */ Int32 featureId,
    /* [in] */ Boolean fromActive)
{
    // ==================before translated======================
    // final DrawableFeatureState st = getDrawableState(featureId, false);
    // if (st != null) {
    //     updateDrawable(featureId, st, fromActive);
    // }
    assert(0);
}

void PhoneWindow::OnDrawableChanged(
    /* [in] */ Int32 featureId,
    /* [in] */ IDrawable* drawable,
    /* [in] */ Int32 alpha)
{
    // ==================before translated======================
    // ImageView view;
    // if (featureId == FEATURE_LEFT_ICON) {
    //     view = getLeftIconView();
    // } else if (featureId == FEATURE_RIGHT_ICON) {
    //     view = getRightIconView();
    // } else {
    //     return;
    // }
    //
    // if (drawable != null) {
    //     drawable.setAlpha(alpha);
    //     view.setImageDrawable(drawable);
    //     view.setVisibility(View.VISIBLE);
    // } else {
    //     view.setVisibility(View.GONE);
    // }
    assert(0);
}

void PhoneWindow::OnIntChanged(
    /* [in] */ Int32 featureId,
    /* [in] */ Int32 value)
{
    // ==================before translated======================
    // if (featureId == FEATURE_PROGRESS || featureId == FEATURE_INDETERMINATE_PROGRESS) {
    //     updateProgressBars(value);
    // } else if (featureId == FEATURE_CUSTOM_TITLE) {
    //     FrameLayout titleContainer = (FrameLayout) findViewById(R.id.title_container);
    //     if (titleContainer != null) {
    //         mLayoutInflater.inflate(value, titleContainer);
    //     }
    // }
    assert(0);
}

Boolean PhoneWindow::OnKeyDown(
    /* [in] */ Int32 featureId,
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event)
{
    // ==================before translated======================
    // /* ****************************************************************************
    //  * HOW TO DECIDE WHERE YOUR KEY HANDLING GOES.
    //  *
    //  * If your key handling must happen before the app gets a crack at the event,
    //  * it goes in PhoneWindowManager.
    //  *
    //  * If your key handling should happen in all windows, and does not depend on
    //  * the state of the current application, other than that the current
    //  * application can override the behavior by handling the event itself, it
    //  * should go in PhoneFallbackEventHandler.
    //  *
    //  * Only if your handling depends on the window, and the fact that it has
    //  * a DecorView, should it go here.
    //  * ****************************************************************************/
    //
    // final KeyEvent.DispatcherState dispatcher =
    //         mDecor != null ? mDecor.getKeyDispatcherState() : null;
    // //Log.i(TAG, "Key down: repeat=" + event.getRepeatCount()
    // //        + " flags=0x" + Integer.toHexString(event.getFlags()));
    //
    // switch (keyCode) {
    //     case KeyEvent.KEYCODE_VOLUME_UP:
    //     case KeyEvent.KEYCODE_VOLUME_DOWN: {
    //         int direction = keyCode == KeyEvent.KEYCODE_VOLUME_UP ? AudioManager.ADJUST_RAISE
    //                 : AudioManager.ADJUST_LOWER;
    //         // If we have a session send it the volume command, otherwise
    //         // use the suggested stream.
    //         if (mMediaController != null) {
    //             mMediaController.adjustVolume(direction, AudioManager.FLAG_SHOW_UI);
    //         } else {
    //             MediaSessionLegacyHelper.getHelper(getContext()).sendAdjustVolumeBy(
    //                     mVolumeControlStreamType, direction,
    //                     AudioManager.FLAG_SHOW_UI | AudioManager.FLAG_VIBRATE);
    //         }
    //         return true;
    //     }
    //     case KeyEvent.KEYCODE_VOLUME_MUTE: {
    //         getAudioManager().handleKeyDown(event, mVolumeControlStreamType);
    //         return true;
    //     }
    //     // These are all the recognized media key codes in
    //     // KeyEvent.isMediaKey()
    //     case KeyEvent.KEYCODE_MEDIA_PLAY:
    //     case KeyEvent.KEYCODE_MEDIA_PAUSE:
    //     case KeyEvent.KEYCODE_MEDIA_PLAY_PAUSE:
    //     case KeyEvent.KEYCODE_MUTE:
    //     case KeyEvent.KEYCODE_HEADSETHOOK:
    //     case KeyEvent.KEYCODE_MEDIA_STOP:
    //     case KeyEvent.KEYCODE_MEDIA_NEXT:
    //     case KeyEvent.KEYCODE_MEDIA_PREVIOUS:
    //     case KeyEvent.KEYCODE_MEDIA_REWIND:
    //     case KeyEvent.KEYCODE_MEDIA_RECORD:
    //     case KeyEvent.KEYCODE_MEDIA_FAST_FORWARD: {
    //         if (mMediaController != null) {
    //             if (mMediaController.dispatchMediaButtonEvent(event)) {
    //                 return true;
    //             }
    //         }
    //         return false;
    //     }
    //
    //     case KeyEvent.KEYCODE_MENU: {
    //         onKeyDownPanel((featureId < 0) ? FEATURE_OPTIONS_PANEL : featureId, event);
    //         return true;
    //     }
    //
    //     case KeyEvent.KEYCODE_BACK: {
    //         if (event.getRepeatCount() > 0) break;
    //         if (featureId < 0) break;
    //         // Currently don't do anything with long press.
    //         if (dispatcher != null) {
    //             dispatcher.startTracking(event, this);
    //         }
    //         return true;
    //     }
    //
    // }
    //
    // return false;
    assert(0);
    return FALSE;
}

Boolean PhoneWindow::OnKeyUp(
    /* [in] */ Int32 featureId,
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event)
{
    // ==================before translated======================
    // final KeyEvent.DispatcherState dispatcher =
    //         mDecor != null ? mDecor.getKeyDispatcherState() : null;
    // if (dispatcher != null) {
    //     dispatcher.handleUpEvent(event);
    // }
    // //Log.i(TAG, "Key up: repeat=" + event.getRepeatCount()
    // //        + " flags=0x" + Integer.toHexString(event.getFlags()));
    //
    // switch (keyCode) {
    //     case KeyEvent.KEYCODE_VOLUME_UP:
    //     case KeyEvent.KEYCODE_VOLUME_DOWN: {
    //         // If we have a session send it the volume command, otherwise
    //         // use the suggested stream.
    //         if (mMediaController != null) {
    //             mMediaController.adjustVolume(0, AudioManager.FLAG_PLAY_SOUND
    //                     | AudioManager.FLAG_VIBRATE);
    //         } else {
    //             MediaSessionLegacyHelper.getHelper(getContext()).sendAdjustVolumeBy(
    //                     mVolumeControlStreamType, 0,
    //                     AudioManager.FLAG_PLAY_SOUND | AudioManager.FLAG_VIBRATE);
    //         }
    //         return true;
    //     }
    //     case KeyEvent.KEYCODE_VOLUME_MUTE: {
    //         // Similar code is in PhoneFallbackEventHandler in case the window
    //         // doesn't have one of these.  In this case, we execute it here and
    //         // eat the event instead, because we have mVolumeControlStreamType
    //         // and they don't.
    //         getAudioManager().handleKeyUp(event, mVolumeControlStreamType);
    //         return true;
    //     }
    //     // These are all the recognized media key codes in
    //     // KeyEvent.isMediaKey()
    //     case KeyEvent.KEYCODE_MEDIA_PLAY:
    //     case KeyEvent.KEYCODE_MEDIA_PAUSE:
    //     case KeyEvent.KEYCODE_MEDIA_PLAY_PAUSE:
    //     case KeyEvent.KEYCODE_MUTE:
    //     case KeyEvent.KEYCODE_HEADSETHOOK:
    //     case KeyEvent.KEYCODE_MEDIA_STOP:
    //     case KeyEvent.KEYCODE_MEDIA_NEXT:
    //     case KeyEvent.KEYCODE_MEDIA_PREVIOUS:
    //     case KeyEvent.KEYCODE_MEDIA_REWIND:
    //     case KeyEvent.KEYCODE_MEDIA_RECORD:
    //     case KeyEvent.KEYCODE_MEDIA_FAST_FORWARD: {
    //         if (mMediaController != null) {
    //             if (mMediaController.dispatchMediaButtonEvent(event)) {
    //                 return true;
    //             }
    //         }
    //         return false;
    //     }
    //
    //     case KeyEvent.KEYCODE_MENU: {
    //         onKeyUpPanel(featureId < 0 ? FEATURE_OPTIONS_PANEL : featureId,
    //                 event);
    //         return true;
    //     }
    //
    //     case KeyEvent.KEYCODE_BACK: {
    //         if (featureId < 0) break;
    //         if (event.isTracking() && !event.isCanceled()) {
    //             if (featureId == FEATURE_OPTIONS_PANEL) {
    //                 PanelFeatureState st = getPanelState(featureId, false);
    //                 if (st != null && st.isInExpandedMode) {
    //                     // If the user is in an expanded menu and hits back, it
    //                     // should go back to the icon menu
    //                     reopenMenu(true);
    //                     return true;
    //                 }
    //             }
    //             closePanel(featureId);
    //             return true;
    //         }
    //         break;
    //     }
    //
    //     case KeyEvent.KEYCODE_SEARCH: {
    //         /*
    //          * Do this in onKeyUp since the Search key is also used for
    //          * chording quick launch shortcuts.
    //          */
    //         if (getKeyguardManager().inKeyguardRestrictedInputMode()) {
    //             break;
    //         }
    //         if (event.isTracking() && !event.isCanceled()) {
    //             launchDefaultSearch();
    //         }
    //         return true;
    //     }
    // }
    //
    // return false;
    assert(0);
    return FALSE;
}

void PhoneWindow::OnActive()
{
    assert(0);
}

AutoPtr<PhoneWindow::DecorView> PhoneWindow::GenerateDecor()
{
    // ==================before translated======================
    // return new DecorView(getContext(), -1);
    assert(0);
    AutoPtr<DecorView> empty;
    return empty;
}

void PhoneWindow::SetFeatureFromAttrs(
    /* [in] */ Int32 featureId,
    /* [in] */ ITypedArray* attrs,
    /* [in] */ Int32 drawableAttr,
    /* [in] */ Int32 alphaAttr)
{
    // ==================before translated======================
    // Drawable d = attrs.getDrawable(drawableAttr);
    // if (d != null) {
    //     requestFeature(featureId);
    //     setFeatureDefaultDrawable(featureId, d);
    // }
    // if ((getFeatures() & (1 << featureId)) != 0) {
    //     int alpha = attrs.getInt(alphaAttr, -1);
    //     if (alpha >= 0) {
    //         setFeatureDrawableAlpha(featureId, alpha);
    //     }
    // }
    assert(0);
}

AutoPtr<IViewGroup> PhoneWindow::GenerateLayout(
    /* [in] */ DecorView* decor)
{
    // ==================before translated======================
    // // Apply data from current theme.
    //
    // TypedArray a = getWindowStyle();
    //
    // if (false) {
    //     System.out.println("From style:");
    //     String s = "Attrs:";
    //     for (int i = 0; i < R.styleable.Window.length; i++) {
    //         s = s + " " + Integer.toHexString(R.styleable.Window[i]) + "="
    //                 + a.getString(i);
    //     }
    //     System.out.println(s);
    // }
    //
    // mIsFloating = a.getBoolean(R.styleable.Window_windowIsFloating, false);
    // int flagsToUpdate = (FLAG_LAYOUT_IN_SCREEN|FLAG_LAYOUT_INSET_DECOR)
    //         & (~getForcedWindowFlags());
    // if (mIsFloating) {
    //     setLayout(WRAP_CONTENT, WRAP_CONTENT);
    //     setFlags(0, flagsToUpdate);
    // } else {
    //     setFlags(FLAG_LAYOUT_IN_SCREEN|FLAG_LAYOUT_INSET_DECOR, flagsToUpdate);
    // }
    //
    // if (a.getBoolean(R.styleable.Window_windowNoTitle, false)) {
    //     requestFeature(FEATURE_NO_TITLE);
    // } else if (a.getBoolean(R.styleable.Window_windowActionBar, false)) {
    //     // Don't allow an action bar if there is no title.
    //     requestFeature(FEATURE_ACTION_BAR);
    // }
    //
    // if (a.getBoolean(R.styleable.Window_windowActionBarOverlay, false)) {
    //     requestFeature(FEATURE_ACTION_BAR_OVERLAY);
    // }
    //
    // if (a.getBoolean(R.styleable.Window_windowActionModeOverlay, false)) {
    //     requestFeature(FEATURE_ACTION_MODE_OVERLAY);
    // }
    //
    // if (a.getBoolean(R.styleable.Window_windowSwipeToDismiss, false)) {
    //     requestFeature(FEATURE_SWIPE_TO_DISMISS);
    // }
    //
    // if (a.getBoolean(R.styleable.Window_windowFullscreen, false)) {
    //     setFlags(FLAG_FULLSCREEN, FLAG_FULLSCREEN & (~getForcedWindowFlags()));
    // }
    //
    // if (a.getBoolean(R.styleable.Window_windowTranslucentStatus,
    //         false)) {
    //     setFlags(FLAG_TRANSLUCENT_STATUS, FLAG_TRANSLUCENT_STATUS
    //             & (~getForcedWindowFlags()));
    // }
    //
    // if (a.getBoolean(R.styleable.Window_windowTranslucentNavigation,
    //         false)) {
    //     setFlags(FLAG_TRANSLUCENT_NAVIGATION, FLAG_TRANSLUCENT_NAVIGATION
    //             & (~getForcedWindowFlags()));
    // }
    //
    // if (a.getBoolean(R.styleable.Window_windowOverscan, false)) {
    //     setFlags(FLAG_LAYOUT_IN_OVERSCAN, FLAG_LAYOUT_IN_OVERSCAN&(~getForcedWindowFlags()));
    // }
    //
    // if (a.getBoolean(R.styleable.Window_windowShowWallpaper, false)) {
    //     setFlags(FLAG_SHOW_WALLPAPER, FLAG_SHOW_WALLPAPER&(~getForcedWindowFlags()));
    // }
    //
    // if (a.getBoolean(R.styleable.Window_windowEnableSplitTouch,
    //         getContext().getApplicationInfo().targetSdkVersion
    //                 >= android.os.Build.VERSION_CODES.HONEYCOMB)) {
    //     setFlags(FLAG_SPLIT_TOUCH, FLAG_SPLIT_TOUCH&(~getForcedWindowFlags()));
    // }
    //
    // a.getValue(R.styleable.Window_windowMinWidthMajor, mMinWidthMajor);
    // a.getValue(R.styleable.Window_windowMinWidthMinor, mMinWidthMinor);
    // if (a.hasValue(R.styleable.Window_windowFixedWidthMajor)) {
    //     if (mFixedWidthMajor == null) mFixedWidthMajor = new TypedValue();
    //     a.getValue(R.styleable.Window_windowFixedWidthMajor,
    //             mFixedWidthMajor);
    // }
    // if (a.hasValue(R.styleable.Window_windowFixedWidthMinor)) {
    //     if (mFixedWidthMinor == null) mFixedWidthMinor = new TypedValue();
    //     a.getValue(R.styleable.Window_windowFixedWidthMinor,
    //             mFixedWidthMinor);
    // }
    // if (a.hasValue(R.styleable.Window_windowFixedHeightMajor)) {
    //     if (mFixedHeightMajor == null) mFixedHeightMajor = new TypedValue();
    //     a.getValue(R.styleable.Window_windowFixedHeightMajor,
    //             mFixedHeightMajor);
    // }
    // if (a.hasValue(R.styleable.Window_windowFixedHeightMinor)) {
    //     if (mFixedHeightMinor == null) mFixedHeightMinor = new TypedValue();
    //     a.getValue(R.styleable.Window_windowFixedHeightMinor,
    //             mFixedHeightMinor);
    // }
    // if (a.getBoolean(R.styleable.Window_windowContentTransitions, false)) {
    //     requestFeature(FEATURE_CONTENT_TRANSITIONS);
    // }
    // if (a.getBoolean(R.styleable.Window_windowActivityTransitions, false)) {
    //     requestFeature(FEATURE_ACTIVITY_TRANSITIONS);
    // }
    //
    // final WindowManager windowService = (WindowManager) getContext().getSystemService(
    //         Context.WINDOW_SERVICE);
    // if (windowService != null) {
    //     final Display display = windowService.getDefaultDisplay();
    //     final boolean shouldUseBottomOutset =
    //             display.getDisplayId() == Display.DEFAULT_DISPLAY
    //                     || (getForcedWindowFlags() & FLAG_FULLSCREEN) != 0;
    //     if (shouldUseBottomOutset && a.hasValue(R.styleable.Window_windowOutsetBottom)) {
    //         if (mOutsetBottom == null) mOutsetBottom = new TypedValue();
    //         a.getValue(R.styleable.Window_windowOutsetBottom,
    //                 mOutsetBottom);
    //     }
    // }
    //
    // final Context context = getContext();
    // final int targetSdk = context.getApplicationInfo().targetSdkVersion;
    // final boolean targetPreHoneycomb = targetSdk < android.os.Build.VERSION_CODES.HONEYCOMB;
    // final boolean targetPreIcs = targetSdk < android.os.Build.VERSION_CODES.ICE_CREAM_SANDWICH;
    // final boolean targetPreL = targetSdk < android.os.Build.VERSION_CODES.LOLLIPOP;
    // final boolean targetHcNeedsOptions = context.getResources().getBoolean(
    //         R.bool.target_honeycomb_needs_options_menu);
    // final boolean noActionBar = !hasFeature(FEATURE_ACTION_BAR) || hasFeature(FEATURE_NO_TITLE);
    //
    // if (targetPreHoneycomb || (targetPreIcs && targetHcNeedsOptions && noActionBar)) {
    //     addFlags(WindowManager.LayoutParams.FLAG_NEEDS_MENU_KEY);
    // } else {
    //     clearFlags(WindowManager.LayoutParams.FLAG_NEEDS_MENU_KEY);
    // }
    //
    // // Non-floating windows on high end devices must put up decor beneath the system bars and
    // // therefore must know about visibility changes of those.
    // if (!mIsFloating && ActivityManager.isHighEndGfx()) {
    //     if (!targetPreL && a.getBoolean(
    //             R.styleable.Window_windowDrawsSystemBarBackgrounds,
    //             false)) {
    //         setFlags(FLAG_DRAWS_SYSTEM_BAR_BACKGROUNDS,
    //                 FLAG_DRAWS_SYSTEM_BAR_BACKGROUNDS & ~getForcedWindowFlags());
    //     }
    // }
    // if (!mForcedStatusBarColor) {
    //     mStatusBarColor = a.getColor(R.styleable.Window_statusBarColor, 0xFF000000);
    // }
    // if (!mForcedNavigationBarColor) {
    //     mNavigationBarColor = a.getColor(R.styleable.Window_navigationBarColor, 0xFF000000);
    // }
    //
    // if (mAlwaysReadCloseOnTouchAttr || getContext().getApplicationInfo().targetSdkVersion
    //         >= android.os.Build.VERSION_CODES.HONEYCOMB) {
    //     if (a.getBoolean(
    //             R.styleable.Window_windowCloseOnTouchOutside,
    //             false)) {
    //         setCloseOnTouchOutsideIfNotSet(true);
    //     }
    // }
    //
    // WindowManager.LayoutParams params = getAttributes();
    //
    // if (!hasSoftInputMode()) {
    //     params.softInputMode = a.getInt(
    //             R.styleable.Window_windowSoftInputMode,
    //             params.softInputMode);
    // }
    //
    // if (a.getBoolean(R.styleable.Window_backgroundDimEnabled,
    //         mIsFloating)) {
    //     /* All dialogs should have the window dimmed */
    //     if ((getForcedWindowFlags()&WindowManager.LayoutParams.FLAG_DIM_BEHIND) == 0) {
    //         params.flags |= WindowManager.LayoutParams.FLAG_DIM_BEHIND;
    //     }
    //     if (!haveDimAmount()) {
    //         params.dimAmount = a.getFloat(
    //                 android.R.styleable.Window_backgroundDimAmount, 0.5f);
    //     }
    // }
    //
    // if (params.windowAnimations == 0) {
    //     params.windowAnimations = a.getResourceId(
    //             R.styleable.Window_windowAnimationStyle, 0);
    // }
    //
    // // The rest are only done if this window is not embedded; otherwise,
    // // the values are inherited from our container.
    // if (getContainer() == null) {
    //     if (mBackgroundDrawable == null) {
    //         if (mBackgroundResource == 0) {
    //             mBackgroundResource = a.getResourceId(
    //                     R.styleable.Window_windowBackground, 0);
    //         }
    //         if (mFrameResource == 0) {
    //             mFrameResource = a.getResourceId(R.styleable.Window_windowFrame, 0);
    //         }
    //         mBackgroundFallbackResource = a.getResourceId(
    //                 R.styleable.Window_windowBackgroundFallback, 0);
    //         if (false) {
    //             System.out.println("Background: "
    //                     + Integer.toHexString(mBackgroundResource) + " Frame: "
    //                     + Integer.toHexString(mFrameResource));
    //         }
    //     }
    //     mElevation = a.getDimension(R.styleable.Window_windowElevation, 0);
    //     mClipToOutline = a.getBoolean(R.styleable.Window_windowClipToOutline, false);
    //     mTextColor = a.getColor(R.styleable.Window_textColor, Color.TRANSPARENT);
    // }
    //
    // // Inflate the window decor.
    //
    // int layoutResource;
    // int features = getLocalFeatures();
    // // System.out.println("Features: 0x" + Integer.toHexString(features));
    // if ((features & (1 << FEATURE_SWIPE_TO_DISMISS)) != 0) {
    //     layoutResource = R.layout.screen_swipe_dismiss;
    // } else if ((features & ((1 << FEATURE_LEFT_ICON) | (1 << FEATURE_RIGHT_ICON))) != 0) {
    //     if (mIsFloating) {
    //         TypedValue res = new TypedValue();
    //         getContext().getTheme().resolveAttribute(
    //                 R.attr.dialogTitleIconsDecorLayout, res, true);
    //         layoutResource = res.resourceId;
    //     } else {
    //         layoutResource = R.layout.screen_title_icons;
    //     }
    //     // XXX Remove this once action bar supports these features.
    //     removeFeature(FEATURE_ACTION_BAR);
    //     // System.out.println("Title Icons!");
    // } else if ((features & ((1 << FEATURE_PROGRESS) | (1 << FEATURE_INDETERMINATE_PROGRESS))) != 0
    //         && (features & (1 << FEATURE_ACTION_BAR)) == 0) {
    //     // Special case for a window with only a progress bar (and title).
    //     // XXX Need to have a no-title version of embedded windows.
    //     layoutResource = R.layout.screen_progress;
    //     // System.out.println("Progress!");
    // } else if ((features & (1 << FEATURE_CUSTOM_TITLE)) != 0) {
    //     // Special case for a window with a custom title.
    //     // If the window is floating, we need a dialog layout
    //     if (mIsFloating) {
    //         TypedValue res = new TypedValue();
    //         getContext().getTheme().resolveAttribute(
    //                 R.attr.dialogCustomTitleDecorLayout, res, true);
    //         layoutResource = res.resourceId;
    //     } else {
    //         layoutResource = R.layout.screen_custom_title;
    //     }
    //     // XXX Remove this once action bar supports these features.
    //     removeFeature(FEATURE_ACTION_BAR);
    // } else if ((features & (1 << FEATURE_NO_TITLE)) == 0) {
    //     // If no other features and not embedded, only need a title.
    //     // If the window is floating, we need a dialog layout
    //     if (mIsFloating) {
    //         TypedValue res = new TypedValue();
    //         getContext().getTheme().resolveAttribute(
    //                 R.attr.dialogTitleDecorLayout, res, true);
    //         layoutResource = res.resourceId;
    //     } else if ((features & (1 << FEATURE_ACTION_BAR)) != 0) {
    //         layoutResource = a.getResourceId(
    //                 R.styleable.Window_windowActionBarFullscreenDecorLayout,
    //                 R.layout.screen_action_bar);
    //     } else {
    //         layoutResource = R.layout.screen_title;
    //     }
    //     // System.out.println("Title!");
    // } else if ((features & (1 << FEATURE_ACTION_MODE_OVERLAY)) != 0) {
    //     layoutResource = R.layout.screen_simple_overlay_action_mode;
    // } else {
    //     // Embedded, so no decoration is needed.
    //     layoutResource = R.layout.screen_simple;
    //     // System.out.println("Simple!");
    // }
    //
    // mDecor.startChanging();
    //
    // View in = mLayoutInflater.inflate(layoutResource, null);
    // decor.addView(in, new ViewGroup.LayoutParams(MATCH_PARENT, MATCH_PARENT));
    // mContentRoot = (ViewGroup) in;
    //
    // ViewGroup contentParent = (ViewGroup)findViewById(ID_ANDROID_CONTENT);
    // if (contentParent == null) {
    //     throw new RuntimeException("Window couldn't find content container view");
    // }
    //
    // if ((features & (1 << FEATURE_INDETERMINATE_PROGRESS)) != 0) {
    //     ProgressBar progress = getCircularProgressBar(false);
    //     if (progress != null) {
    //         progress.setIndeterminate(true);
    //     }
    // }
    //
    // if ((features & (1 << FEATURE_SWIPE_TO_DISMISS)) != 0) {
    //     registerSwipeCallbacks();
    // }
    //
    // // Remaining setup -- of background and title -- that only applies
    // // to top-level windows.
    // if (getContainer() == null) {
    //     final Drawable background;
    //     if (mBackgroundResource != 0) {
    //         background = getContext().getDrawable(mBackgroundResource);
    //     } else {
    //         background = mBackgroundDrawable;
    //     }
    //     mDecor.setWindowBackground(background);
    //
    //     final Drawable frame;
    //     if (mFrameResource != 0) {
    //         frame = getContext().getDrawable(mFrameResource);
    //     } else {
    //         frame = null;
    //     }
    //     mDecor.setWindowFrame(frame);
    //
    //     mDecor.setElevation(mElevation);
    //     mDecor.setClipToOutline(mClipToOutline);
    //
    //     if (mTitle != null) {
    //         setTitle(mTitle);
    //     }
    //
    //     if (mTitleColor == 0) {
    //         mTitleColor = mTextColor;
    //     }
    //     setTitleColor(mTitleColor);
    // }
    //
    // mDecor.finishChanging();
    //
    // return contentParent;
    assert(0);
    AutoPtr<IViewGroup> empty;
    return empty;
}

void PhoneWindow::DispatchWindowAttributesChanged(
    /* [in] */ IWindowManagerLayoutParams* attrs)
{
    // ==================before translated======================
    // super.dispatchWindowAttributesChanged(attrs);
    // if (mDecor != null) {
    //     mDecor.updateColorViews(null /* insets */);
    // }
    assert(0);
}

ECode PhoneWindow::DispatchOnWindowDismissed(
    /* [in] */ IOnWindowDismissedCallback* cb)
{
    return Window::DispatchOnWindowDismissed();
}

void PhoneWindow::TransitionTo(
    /* [in] */ IScene* scene)
{
    // ==================before translated======================
    // if (mContentScene == null) {
    //     scene.enter();
    // } else {
    //     mTransitionManager.transitionTo(scene);
    // }
    // mContentScene = scene;
    assert(0);
}

void PhoneWindow::ClearMenuViews(
    /* [in] */ PanelFeatureState* st)
{
    // ==================before translated======================
    // // This can be called on config changes, so we should make sure
    // // the views will be reconstructed based on the new orientation, etc.
    //
    // // Allow the callback to create a new panel view
    // st.createdPanelView = null;
    //
    // // Causes the decor view to be recreated
    // st.refreshDecorView = true;
    //
    // st.clearMenuPresenters();
    assert(0);
}

void PhoneWindow::OpenPanel(
    /* [in] */ PanelFeatureState* st,
    /* [in] */ IKeyEvent* event)
{
    // ==================before translated======================
    // // System.out.println("Open panel: isOpen=" + st.isOpen);
    //
    // // Already open, return
    // if (st.isOpen || isDestroyed()) {
    //     return;
    // }
    //
    // // Don't open an options panel for honeycomb apps on xlarge devices.
    // // (The app should be using an action bar for menu items.)
    // if (st.featureId == FEATURE_OPTIONS_PANEL) {
    //     Context context = getContext();
    //     Configuration config = context.getResources().getConfiguration();
    //     boolean isXLarge = (config.screenLayout & Configuration.SCREENLAYOUT_SIZE_MASK) ==
    //             Configuration.SCREENLAYOUT_SIZE_XLARGE;
    //     boolean isHoneycombApp = context.getApplicationInfo().targetSdkVersion >=
    //             android.os.Build.VERSION_CODES.HONEYCOMB;
    //
    //     if (isXLarge && isHoneycombApp) {
    //         return;
    //     }
    // }
    //
    // Callback cb = getCallback();
    // if ((cb != null) && (!cb.onMenuOpened(st.featureId, st.menu))) {
    //     // Callback doesn't want the menu to open, reset any state
    //     closePanel(st, true);
    //     return;
    // }
    //
    // final WindowManager wm = getWindowManager();
    // if (wm == null) {
    //     return;
    // }
    //
    // // Prepare panel (should have been done before, but just in case)
    // if (!preparePanel(st, event)) {
    //     return;
    // }
    //
    // int width = WRAP_CONTENT;
    // if (st.decorView == null || st.refreshDecorView) {
    //     if (st.decorView == null) {
    //         // Initialize the panel decor, this will populate st.decorView
    //         if (!initializePanelDecor(st) || (st.decorView == null))
    //             return;
    //     } else if (st.refreshDecorView && (st.decorView.getChildCount() > 0)) {
    //         // Decor needs refreshing, so remove its views
    //         st.decorView.removeAllViews();
    //     }
    //
    //     // This will populate st.shownPanelView
    //     if (!initializePanelContent(st) || !st.hasPanelItems()) {
    //         return;
    //     }
    //
    //     ViewGroup.LayoutParams lp = st.shownPanelView.getLayoutParams();
    //     if (lp == null) {
    //         lp = new ViewGroup.LayoutParams(WRAP_CONTENT, WRAP_CONTENT);
    //     }
    //
    //     int backgroundResId;
    //     if (lp.width == ViewGroup.LayoutParams.MATCH_PARENT) {
    //         // If the contents is fill parent for the width, set the
    //         // corresponding background
    //         backgroundResId = st.fullBackground;
    //         width = MATCH_PARENT;
    //     } else {
    //         // Otherwise, set the normal panel background
    //         backgroundResId = st.background;
    //     }
    //     st.decorView.setWindowBackground(getContext().getDrawable(
    //             backgroundResId));
    //
    //     ViewParent shownPanelParent = st.shownPanelView.getParent();
    //     if (shownPanelParent != null && shownPanelParent instanceof ViewGroup) {
    //         ((ViewGroup) shownPanelParent).removeView(st.shownPanelView);
    //     }
    //     st.decorView.addView(st.shownPanelView, lp);
    //
    //     /*
    //      * Give focus to the view, if it or one of its children does not
    //      * already have it.
    //      */
    //     if (!st.shownPanelView.hasFocus()) {
    //         st.shownPanelView.requestFocus();
    //     }
    // } else if (!st.isInListMode()) {
    //     width = MATCH_PARENT;
    // } else if (st.createdPanelView != null) {
    //     // If we already had a panel view, carry width=MATCH_PARENT through
    //     // as we did above when it was created.
    //     ViewGroup.LayoutParams lp = st.createdPanelView.getLayoutParams();
    //     if (lp != null && lp.width == ViewGroup.LayoutParams.MATCH_PARENT) {
    //         width = MATCH_PARENT;
    //     }
    // }
    //
    // st.isHandled = false;
    //
    // WindowManager.LayoutParams lp = new WindowManager.LayoutParams(
    //         width, WRAP_CONTENT,
    //         st.x, st.y, WindowManager.LayoutParams.TYPE_APPLICATION_ATTACHED_DIALOG,
    //         WindowManager.LayoutParams.FLAG_ALT_FOCUSABLE_IM
    //         | WindowManager.LayoutParams.FLAG_SPLIT_TOUCH,
    //         st.decorView.mDefaultOpacity);
    //
    // if (st.isCompact) {
    //     lp.gravity = getOptionsPanelGravity();
    //     sRotationWatcher.addWindow(this);
    // } else {
    //     lp.gravity = st.gravity;
    // }
    //
    // lp.windowAnimations = st.windowAnimations;
    //
    // wm.addView(st.decorView, lp);
    // st.isOpen = true;
    // // Log.v(TAG, "Adding main menu to window manager.");
    assert(0);
}

// synchronized
void PhoneWindow::CloseContextMenu()
{
    // ==================before translated======================
    // if (mContextMenu != null) {
    //     mContextMenu.close();
    //     dismissContextMenu();
    // }
    assert(0);
}

// synchronized
void PhoneWindow::DismissContextMenu()
{
    // ==================before translated======================
    // mContextMenu = null;
    //
    // if (mContextMenuHelper != null) {
    //     mContextMenuHelper.dismiss();
    //     mContextMenuHelper = null;
    // }
    assert(0);
}

Boolean PhoneWindow::PerformPanelShortcut(
    /* [in] */ PanelFeatureState* st,
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event,
    /* [in] */ Int32 flags)
{
    // ==================before translated======================
    // if (event.isSystem() || (st == null)) {
    //     return false;
    // }
    //
    // boolean handled = false;
    //
    // // Only try to perform menu shortcuts if preparePanel returned true (possible false
    // // return value from application not wanting to show the menu).
    // if ((st.isPrepared || preparePanel(st, event)) && st.menu != null) {
    //     // The menu is prepared now, perform the shortcut on it
    //     handled = st.menu.performShortcut(keyCode, event, flags);
    // }
    //
    // if (handled) {
    //     // Mark as handled
    //     st.isHandled = true;
    //
    //     // Only close down the menu if we don't have an action bar keeping it open.
    //     if ((flags & Menu.FLAG_PERFORM_NO_CLOSE) == 0 && mDecorContentParent == null) {
    //         closePanel(st, true);
    //     }
    // }
    //
    // return handled;
    assert(0);
    return FALSE;
}

void PhoneWindow::ReopenMenu(
    /* [in] */ Boolean toggleMenuMode)
{
    // ==================before translated======================
    // if (mDecorContentParent != null && mDecorContentParent.canShowOverflowMenu() &&
    //         (!ViewConfiguration.get(getContext()).hasPermanentMenuKey() ||
    //                 mDecorContentParent.isOverflowMenuShowPending())) {
    //     final Callback cb = getCallback();
    //     if (!mDecorContentParent.isOverflowMenuShowing() || !toggleMenuMode) {
    //         if (cb != null && !isDestroyed()) {
    //             // If we have a menu invalidation pending, do it now.
    //             if (mInvalidatePanelMenuPosted &&
    //                     (mInvalidatePanelMenuFeatures & (1 << FEATURE_OPTIONS_PANEL)) != 0) {
    //                 mDecor.removeCallbacks(mInvalidatePanelMenuRunnable);
    //                 mInvalidatePanelMenuRunnable.run();
    //             }
    //
    //             final PanelFeatureState st = getPanelState(FEATURE_OPTIONS_PANEL, true);
    //
    //             // If we don't have a menu or we're waiting for a full content refresh,
    //             // forget it. This is a lingering event that no longer matters.
    //             if (st.menu != null && !st.refreshMenuContent &&
    //                     cb.onPreparePanel(FEATURE_OPTIONS_PANEL, st.createdPanelView, st.menu)) {
    //                 cb.onMenuOpened(FEATURE_ACTION_BAR, st.menu);
    //                 mDecorContentParent.showOverflowMenu();
    //             }
    //         }
    //     } else {
    //         mDecorContentParent.hideOverflowMenu();
    //         if (cb != null && !isDestroyed()) {
    //             final PanelFeatureState st = getPanelState(FEATURE_OPTIONS_PANEL, true);
    //             cb.onPanelClosed(FEATURE_ACTION_BAR, st.menu);
    //         }
    //     }
    //     return;
    // }
    //
    // PanelFeatureState st = getPanelState(FEATURE_OPTIONS_PANEL, true);
    //
    // // Save the future expanded mode state since closePanel will reset it
    // boolean newExpandedMode = toggleMenuMode ? !st.isInExpandedMode : st.isInExpandedMode;
    //
    // st.refreshDecorView = true;
    // closePanel(st, false);
    //
    // // Set the expanded mode state
    // st.isInExpandedMode = newExpandedMode;
    //
    // openPanel(st, null);
    assert(0);
}

Int32 PhoneWindow::GetOptionsPanelGravity()
{
    // ==================before translated======================
    // try {
    //     return WindowManagerHolder.sWindowManager.getPreferredOptionsPanelGravity();
    // } catch (RemoteException ex) {
    //     Log.e(TAG, "Couldn't getOptionsPanelGravity; using default", ex);
    //     return Gravity.CENTER | Gravity.BOTTOM;
    // }
    assert(0);
    return 0;
}

void PhoneWindow::UpdateProgressBars(
    /* [in] */ Int32 value)
{
    // ==================before translated======================
    // ProgressBar circularProgressBar = getCircularProgressBar(true);
    // ProgressBar horizontalProgressBar = getHorizontalProgressBar(true);
    //
    // final int features = getLocalFeatures();
    // if (value == PROGRESS_VISIBILITY_ON) {
    //     if ((features & (1 << FEATURE_PROGRESS)) != 0) {
    //         if (horizontalProgressBar != null) {
    //             int level = horizontalProgressBar.getProgress();
    //             int visibility = (horizontalProgressBar.isIndeterminate() || level < 10000) ?
    //                     View.VISIBLE : View.INVISIBLE;
    //             horizontalProgressBar.setVisibility(visibility);
    //         } else {
    //             Log.e(TAG, "Horizontal progress bar not located in current window decor");
    //         }
    //     }
    //     if ((features & (1 << FEATURE_INDETERMINATE_PROGRESS)) != 0) {
    //         if (circularProgressBar != null) {
    //             circularProgressBar.setVisibility(View.VISIBLE);
    //         } else {
    //             Log.e(TAG, "Circular progress bar not located in current window decor");
    //         }
    //     }
    // } else if (value == PROGRESS_VISIBILITY_OFF) {
    //     if ((features & (1 << FEATURE_PROGRESS)) != 0) {
    //         if (horizontalProgressBar != null) {
    //             horizontalProgressBar.setVisibility(View.GONE);
    //         } else {
    //             Log.e(TAG, "Horizontal progress bar not located in current window decor");
    //         }
    //     }
    //     if ((features & (1 << FEATURE_INDETERMINATE_PROGRESS)) != 0) {
    //         if (circularProgressBar != null) {
    //             circularProgressBar.setVisibility(View.GONE);
    //         } else {
    //             Log.e(TAG, "Circular progress bar not located in current window decor");
    //         }
    //     }
    // } else if (value == PROGRESS_INDETERMINATE_ON) {
    //     if (horizontalProgressBar != null) {
    //         horizontalProgressBar.setIndeterminate(true);
    //     } else {
    //         Log.e(TAG, "Horizontal progress bar not located in current window decor");
    //     }
    // } else if (value == PROGRESS_INDETERMINATE_OFF) {
    //     if (horizontalProgressBar != null) {
    //         horizontalProgressBar.setIndeterminate(false);
    //     } else {
    //         Log.e(TAG, "Horizontal progress bar not located in current window decor");
    //     }
    // } else if (PROGRESS_START <= value && value <= PROGRESS_END) {
    //     // We want to set the progress value before testing for visibility
    //     // so that when the progress bar becomes visible again, it has the
    //     // correct level.
    //     if (horizontalProgressBar != null) {
    //         horizontalProgressBar.setProgress(value - PROGRESS_START);
    //     } else {
    //         Log.e(TAG, "Horizontal progress bar not located in current window decor");
    //     }
    //
    //     if (value < PROGRESS_END) {
    //         showProgressBars(horizontalProgressBar, circularProgressBar);
    //     } else {
    //         hideProgressBars(horizontalProgressBar, circularProgressBar);
    //     }
    // } else if (PROGRESS_SECONDARY_START <= value && value <= PROGRESS_SECONDARY_END) {
    //     if (horizontalProgressBar != null) {
    //         horizontalProgressBar.setSecondaryProgress(value - PROGRESS_SECONDARY_START);
    //     } else {
    //         Log.e(TAG, "Horizontal progress bar not located in current window decor");
    //     }
    //
    //     showProgressBars(horizontalProgressBar, circularProgressBar);
    // }
    assert(0);
}

void PhoneWindow::ShowProgressBars(
    /* [in] */ IProgressBar* horizontalProgressBar,
    /* [in] */ IProgressBar* spinnyProgressBar)
{
    // ==================before translated======================
    // final int features = getLocalFeatures();
    // if ((features & (1 << FEATURE_INDETERMINATE_PROGRESS)) != 0 &&
    //         spinnyProgressBar != null && spinnyProgressBar.getVisibility() == View.INVISIBLE) {
    //     spinnyProgressBar.setVisibility(View.VISIBLE);
    // }
    // // Only show the progress bars if the primary progress is not complete
    // if ((features & (1 << FEATURE_PROGRESS)) != 0 && horizontalProgressBar != null &&
    //         horizontalProgressBar.getProgress() < 10000) {
    //     horizontalProgressBar.setVisibility(View.VISIBLE);
    // }
    assert(0);
}

void PhoneWindow::HideProgressBars(
    /* [in] */ IProgressBar* horizontalProgressBar,
    /* [in] */ IProgressBar* spinnyProgressBar)
{
    // ==================before translated======================
    // final int features = getLocalFeatures();
    // Animation anim = AnimationUtils.loadAnimation(getContext(), R.anim.fade_out);
    // anim.setDuration(1000);
    // if ((features & (1 << FEATURE_INDETERMINATE_PROGRESS)) != 0 &&
    //         spinnyProgressBar != null &&
    //         spinnyProgressBar.getVisibility() == View.VISIBLE) {
    //     spinnyProgressBar.startAnimation(anim);
    //     spinnyProgressBar.setVisibility(View.INVISIBLE);
    // }
    // if ((features & (1 << FEATURE_PROGRESS)) != 0 && horizontalProgressBar != null &&
    //         horizontalProgressBar.getVisibility() == View.VISIBLE) {
    //     horizontalProgressBar.startAnimation(anim);
    //     horizontalProgressBar.setVisibility(View.INVISIBLE);
    // }
    assert(0);
}

AutoPtr<IViewRootImpl> PhoneWindow::GetViewRootImpl()
{
    // ==================before translated======================
    // if (mDecor != null) {
    //     ViewRootImpl viewRootImpl = mDecor.getViewRootImpl();
    //     if (viewRootImpl != null) {
    //         return viewRootImpl;
    //     }
    // }
    // throw new IllegalStateException("view not added");
    assert(0);
    AutoPtr<IViewRootImpl> empty;
    return empty;
}

AutoPtr<IKeyguardManager> PhoneWindow::GetKeyguardManager()
{
    // ==================before translated======================
    // if (mKeyguardManager == null) {
    //     mKeyguardManager = (KeyguardManager) getContext().getSystemService(
    //             Context.KEYGUARD_SERVICE);
    // }
    // return mKeyguardManager;
    assert(0);
    AutoPtr<IKeyguardManager> empty;
    return empty;
}

void PhoneWindow::SavePanelState(
    /* [in] */ ISparseArray* icicles)//IParcelable
{
    // ==================before translated======================
    // PanelFeatureState[] panels = mPanels;
    // if (panels == null) {
    //     return;
    // }
    //
    // for (int curFeatureId = panels.length - 1; curFeatureId >= 0; curFeatureId--) {
    //     if (panels[curFeatureId] != null) {
    //         icicles.put(curFeatureId, panels[curFeatureId].onSaveInstanceState());
    //     }
    // }
    assert(0);
}

void PhoneWindow::RestorePanelState(
    /* [in] */ ISparseArray* icicles)//IParcelable
{
    // ==================before translated======================
    // PanelFeatureState st;
    // int curFeatureId;
    // for (int i = icicles.size() - 1; i >= 0; i--) {
    //     curFeatureId = icicles.keyAt(i);
    //     st = getPanelState(curFeatureId, false /* required */);
    //     if (st == null) {
    //         // The panel must not have been required, and is currently not around, skip it
    //         continue;
    //     }
    //
    //     st.onRestoreInstanceState(icicles.get(curFeatureId));
    //     invalidatePanelMenu(curFeatureId);
    // }
    //
    // /*
    //  * Implementation note: call openPanelsAfterRestore later to actually open the
    //  * restored panels.
    //  */
    assert(0);
}

void PhoneWindow::OpenPanelsAfterRestore()
{
    // ==================before translated======================
    // PanelFeatureState[] panels = mPanels;
    //
    // if (panels == null) {
    //     return;
    // }
    //
    // PanelFeatureState st;
    // for (int i = panels.length - 1; i >= 0; i--) {
    //     st = panels[i];
    //     // We restore the panel if it was last open; we skip it if it
    //     // now is open, to avoid a race condition if the user immediately
    //     // opens it when we are resuming.
    //     if (st != null) {
    //         st.applyFrozenState();
    //         if (!st.isOpen && st.wasLastOpen) {
    //             st.isInExpandedMode = st.wasLastExpanded;
    //             openPanel(st, null);
    //         }
    //     }
    // }
    assert(0);
}

void PhoneWindow::InstallDecor()
{
    // ==================before translated======================
    // if (mDecor == null) {
    //     mDecor = generateDecor();
    //     mDecor.setDescendantFocusability(ViewGroup.FOCUS_AFTER_DESCENDANTS);
    //     mDecor.setIsRootNamespace(true);
    //     if (!mInvalidatePanelMenuPosted && mInvalidatePanelMenuFeatures != 0) {
    //         mDecor.postOnAnimation(mInvalidatePanelMenuRunnable);
    //     }
    // }
    // if (mContentParent == null) {
    //     mContentParent = generateLayout(mDecor);
    //
    //     // Set up decor part of UI to ignore fitsSystemWindows if appropriate.
    //     mDecor.makeOptionalFitsSystemWindows();
    //
    //     final DecorContentParent decorContentParent = (DecorContentParent) mDecor.findViewById(
    //             R.id.decor_content_parent);
    //
    //     if (decorContentParent != null) {
    //         mDecorContentParent = decorContentParent;
    //         mDecorContentParent.setWindowCallback(getCallback());
    //         if (mDecorContentParent.getTitle() == null) {
    //             mDecorContentParent.setWindowTitle(mTitle);
    //         }
    //
    //         final int localFeatures = getLocalFeatures();
    //         for (int i = 0; i < FEATURE_MAX; i++) {
    //             if ((localFeatures & (1 << i)) != 0) {
    //                 mDecorContentParent.initFeature(i);
    //             }
    //         }
    //
    //         mDecorContentParent.setUiOptions(mUiOptions);
    //
    //         if ((mResourcesSetFlags & FLAG_RESOURCE_SET_ICON) != 0 ||
    //                 (mIconRes != 0 && !mDecorContentParent.hasIcon())) {
    //             mDecorContentParent.setIcon(mIconRes);
    //         } else if ((mResourcesSetFlags & FLAG_RESOURCE_SET_ICON) == 0 &&
    //                 mIconRes == 0 && !mDecorContentParent.hasIcon()) {
    //             mDecorContentParent.setIcon(
    //                     getContext().getPackageManager().getDefaultActivityIcon());
    //             mResourcesSetFlags |= FLAG_RESOURCE_SET_ICON_FALLBACK;
    //         }
    //         if ((mResourcesSetFlags & FLAG_RESOURCE_SET_LOGO) != 0 ||
    //                 (mLogoRes != 0 && !mDecorContentParent.hasLogo())) {
    //             mDecorContentParent.setLogo(mLogoRes);
    //         }
    //
    //         // Invalidate if the panel menu hasn't been created before this.
    //         // Panel menu invalidation is deferred avoiding application onCreateOptionsMenu
    //         // being called in the middle of onCreate or similar.
    //         // A pending invalidation will typically be resolved before the posted message
    //         // would run normally in order to satisfy instance state restoration.
    //         PanelFeatureState st = getPanelState(FEATURE_OPTIONS_PANEL, false);
    //         if (!isDestroyed() && (st == null || st.menu == null)) {
    //             invalidatePanelMenu(FEATURE_ACTION_BAR);
    //         }
    //     } else {
    //         mTitleView = (TextView)findViewById(R.id.title);
    //         if (mTitleView != null) {
    //             mTitleView.setLayoutDirection(mDecor.getLayoutDirection());
    //             if ((getLocalFeatures() & (1 << FEATURE_NO_TITLE)) != 0) {
    //                 View titleContainer = findViewById(
    //                         R.id.title_container);
    //                 if (titleContainer != null) {
    //                     titleContainer.setVisibility(View.GONE);
    //                 } else {
    //                     mTitleView.setVisibility(View.GONE);
    //                 }
    //                 if (mContentParent instanceof FrameLayout) {
    //                     ((FrameLayout)mContentParent).setForeground(null);
    //                 }
    //             } else {
    //                 mTitleView.setText(mTitle);
    //             }
    //         }
    //     }
    //
    //     if (mDecor.getBackground() == null && mBackgroundFallbackResource != 0) {
    //         mDecor.setBackgroundFallback(mBackgroundFallbackResource);
    //     }
    //
    //     // Only inflate or create a new TransitionManager if the caller hasn't
    //     // already set a custom one.
    //     if (hasFeature(FEATURE_ACTIVITY_TRANSITIONS)) {
    //         if (mTransitionManager == null) {
    //             final int transitionRes = getWindowStyle().getResourceId(
    //                     R.styleable.Window_windowContentTransitionManager,
    //                     0);
    //             if (transitionRes != 0) {
    //                 final TransitionInflater inflater = TransitionInflater.from(getContext());
    //                 mTransitionManager = inflater.inflateTransitionManager(transitionRes,
    //                         mContentParent);
    //             } else {
    //                 mTransitionManager = new TransitionManager();
    //             }
    //         }
    //
    //         mEnterTransition = getTransition(mEnterTransition, null,
    //                 R.styleable.Window_windowEnterTransition);
    //         mReturnTransition = getTransition(mReturnTransition, USE_DEFAULT_TRANSITION,
    //                 R.styleable.Window_windowReturnTransition);
    //         mExitTransition = getTransition(mExitTransition, null,
    //                 R.styleable.Window_windowExitTransition);
    //         mReenterTransition = getTransition(mReenterTransition, USE_DEFAULT_TRANSITION,
    //                 R.styleable.Window_windowReenterTransition);
    //         mSharedElementEnterTransition = getTransition(mSharedElementEnterTransition, null,
    //                 R.styleable.Window_windowSharedElementEnterTransition);
    //         mSharedElementReturnTransition = getTransition(mSharedElementReturnTransition,
    //                 USE_DEFAULT_TRANSITION,
    //                 R.styleable.Window_windowSharedElementReturnTransition);
    //         mSharedElementExitTransition = getTransition(mSharedElementExitTransition, null,
    //                 R.styleable.Window_windowSharedElementExitTransition);
    //         mSharedElementReenterTransition = getTransition(mSharedElementReenterTransition,
    //                 USE_DEFAULT_TRANSITION,
    //                 R.styleable.Window_windowSharedElementReenterTransition);
    //         if (mAllowEnterTransitionOverlap == null) {
    //             mAllowEnterTransitionOverlap = getWindowStyle().getBoolean(
    //                     R.styleable.Window_windowAllowEnterTransitionOverlap, true);
    //         }
    //         if (mAllowReturnTransitionOverlap == null) {
    //             mAllowReturnTransitionOverlap = getWindowStyle().getBoolean(
    //                     R.styleable.Window_windowAllowReturnTransitionOverlap, true);
    //         }
    //         if (mBackgroundFadeDurationMillis < 0) {
    //             mBackgroundFadeDurationMillis = getWindowStyle().getInteger(
    //                     R.styleable.Window_windowTransitionBackgroundFadeDuration,
    //                     DEFAULT_BACKGROUND_FADE_DURATION_MS);
    //         }
    //         if (mSharedElementsUseOverlay == null) {
    //             mSharedElementsUseOverlay = getWindowStyle().getBoolean(
    //                     R.styleable.Window_windowSharedElementsUseOverlay, true);
    //         }
    //     }
    // }
    assert(0);
}

AutoPtr<ITransition> PhoneWindow::GetTransition(
    /* [in] */ ITransition* currentValue,
    /* [in] */ ITransition* defaultValue,
    /* [in] */ Int32 id)
{
    // ==================before translated======================
    // if (currentValue != defaultValue) {
    //     return currentValue;
    // }
    // int transitionId = getWindowStyle().getResourceId(id, -1);
    // Transition transition = defaultValue;
    // if (transitionId != -1 && transitionId != R.transition.no_transition) {
    //     TransitionInflater inflater = TransitionInflater.from(getContext());
    //     transition = inflater.inflateTransition(transitionId);
    //     if (transition instanceof TransitionSet &&
    //             ((TransitionSet)transition).getTransitionCount() == 0) {
    //         transition = null;
    //     }
    // }
    // return transition;
    assert(0);
    AutoPtr<ITransition> empty;
    return empty;
}

AutoPtr<IDrawable> PhoneWindow::LoadImageURI(
    /* [in] */ IUri* uri)
{
    // ==================before translated======================
    // try {
    //     return Drawable.createFromStream(
    //             getContext().getContentResolver().openInputStream(uri), null);
    // } catch (Exception e) {
    //     Log.w(TAG, "Unable to open content: " + uri);
    // }
    // return null;
    assert(0);
    AutoPtr<IDrawable> empty;
    return empty;
}

AutoPtr<PhoneWindow::DrawableFeatureState> PhoneWindow::GetDrawableState(
    /* [in] */ Int32 featureId,
    /* [in] */ Boolean required)
{
    // ==================before translated======================
    // if ((getFeatures() & (1 << featureId)) == 0) {
    //     if (!required) {
    //         return null;
    //     }
    //     throw new RuntimeException("The feature has not been requested");
    // }
    //
    // DrawableFeatureState[] ar;
    // if ((ar = mDrawables) == null || ar.length <= featureId) {
    //     DrawableFeatureState[] nar = new DrawableFeatureState[featureId + 1];
    //     if (ar != null) {
    //         System.arraycopy(ar, 0, nar, 0, ar.length);
    //     }
    //     mDrawables = ar = nar;
    // }
    //
    // DrawableFeatureState st = ar[featureId];
    // if (st == null) {
    //     ar[featureId] = st = new DrawableFeatureState(featureId);
    // }
    // return st;
    assert(0);
    AutoPtr<DrawableFeatureState> empty;
    return empty;
}

AutoPtr<PhoneWindow::PanelFeatureState> PhoneWindow::GetPanelState(
    /* [in] */ Int32 featureId,
    /* [in] */ Boolean required)
{
    // ==================before translated======================
    // return getPanelState(featureId, required, null);
    assert(0);
    AutoPtr<PanelFeatureState> empty;
    return empty;
}

AutoPtr<PhoneWindow::PanelFeatureState> PhoneWindow::GetPanelState(
    /* [in] */ Int32 featureId,
    /* [in] */ Boolean required,
    /* [in] */ PanelFeatureState* convertPanelState)
{
    // ==================before translated======================
    // if ((getFeatures() & (1 << featureId)) == 0) {
    //     if (!required) {
    //         return null;
    //     }
    //     throw new RuntimeException("The feature has not been requested");
    // }
    //
    // PanelFeatureState[] ar;
    // if ((ar = mPanels) == null || ar.length <= featureId) {
    //     PanelFeatureState[] nar = new PanelFeatureState[featureId + 1];
    //     if (ar != null) {
    //         System.arraycopy(ar, 0, nar, 0, ar.length);
    //     }
    //     mPanels = ar = nar;
    // }
    //
    // PanelFeatureState st = ar[featureId];
    // if (st == null) {
    //     ar[featureId] = st = (convertPanelState != null)
    //             ? convertPanelState
    //             : new PanelFeatureState(featureId);
    // }
    // return st;
    assert(0);
    AutoPtr<PanelFeatureState> empty;
    return empty;
}

void PhoneWindow::UpdateDrawable(
    /* [in] */ Int32 featureId,
    /* [in] */ DrawableFeatureState* st,
    /* [in] */ Boolean fromResume)
{
    // ==================before translated======================
    // // Do nothing if the decor is not yet installed... an update will
    // // need to be forced when we eventually become active.
    // if (mContentParent == null) {
    //     return;
    // }
    //
    // final int featureMask = 1 << featureId;
    //
    // if ((getFeatures() & featureMask) == 0 && !fromResume) {
    //     return;
    // }
    //
    // Drawable drawable = null;
    // if (st != null) {
    //     drawable = st.child;
    //     if (drawable == null)
    //         drawable = st.local;
    //     if (drawable == null)
    //         drawable = st.def;
    // }
    // if ((getLocalFeatures() & featureMask) == 0) {
    //     if (getContainer() != null) {
    //         if (isActive() || fromResume) {
    //             getContainer().setChildDrawable(featureId, drawable);
    //         }
    //     }
    // } else if (st != null && (st.cur != drawable || st.curAlpha != st.alpha)) {
    //     // System.out.println("Drawable changed: old=" + st.cur
    //     // + ", new=" + drawable);
    //     st.cur = drawable;
    //     st.curAlpha = st.alpha;
    //     onDrawableChanged(featureId, drawable, st.alpha);
    // }
    assert(0);
}

void PhoneWindow::UpdateInt(
    /* [in] */ Int32 featureId,
    /* [in] */ Int32 value,
    /* [in] */ Boolean fromResume)
{
    // ==================before translated======================
    //
    // // Do nothing if the decor is not yet installed... an update will
    // // need to be forced when we eventually become active.
    // if (mContentParent == null) {
    //     return;
    // }
    //
    // final int featureMask = 1 << featureId;
    //
    // if ((getFeatures() & featureMask) == 0 && !fromResume) {
    //     return;
    // }
    //
    // if ((getLocalFeatures() & featureMask) == 0) {
    //     if (getContainer() != null) {
    //         getContainer().setChildInt(featureId, value);
    //     }
    // } else {
    //     onIntChanged(featureId, value);
    // }
    assert(0);
}

AutoPtr<IImageView> PhoneWindow::GetLeftIconView()
{
    // ==================before translated======================
    // if (mLeftIconView != null) {
    //     return mLeftIconView;
    // }
    // if (mContentParent == null) {
    //     installDecor();
    // }
    // return (mLeftIconView = (ImageView)findViewById(R.id.left_icon));
    assert(0);
    AutoPtr<IImageView> empty;
    return empty;
}

AutoPtr<IProgressBar> PhoneWindow::GetCircularProgressBar(
    /* [in] */ Boolean shouldInstallDecor)
{
    // ==================before translated======================
    // if (mCircularProgressBar != null) {
    //     return mCircularProgressBar;
    // }
    // if (mContentParent == null && shouldInstallDecor) {
    //     installDecor();
    // }
    // mCircularProgressBar = (ProgressBar) findViewById(R.id.progress_circular);
    // if (mCircularProgressBar != null) {
    //     mCircularProgressBar.setVisibility(View.INVISIBLE);
    // }
    // return mCircularProgressBar;
    assert(0);
    AutoPtr<IProgressBar> empty;
    return empty;
}

AutoPtr<IProgressBar> PhoneWindow::GetHorizontalProgressBar(
    /* [in] */ Boolean shouldInstallDecor)
{
    // ==================before translated======================
    // if (mHorizontalProgressBar != null) {
    //     return mHorizontalProgressBar;
    // }
    // if (mContentParent == null && shouldInstallDecor) {
    //     installDecor();
    // }
    // mHorizontalProgressBar = (ProgressBar) findViewById(R.id.progress_horizontal);
    // if (mHorizontalProgressBar != null) {
    //     mHorizontalProgressBar.setVisibility(View.INVISIBLE);
    // }
    // return mHorizontalProgressBar;
    assert(0);
    AutoPtr<IProgressBar> empty;
    return empty;
}

AutoPtr<IImageView> PhoneWindow::GetRightIconView()
{
    // ==================before translated======================
    // if (mRightIconView != null) {
    //     return mRightIconView;
    // }
    // if (mContentParent == null) {
    //     installDecor();
    // }
    // return (mRightIconView = (ImageView)findViewById(R.id.right_icon));
    assert(0);
    AutoPtr<IImageView> empty;
    return empty;
}

void PhoneWindow::RegisterSwipeCallbacks()
{
    // ==================before translated======================
    // SwipeDismissLayout swipeDismiss =
    //         (SwipeDismissLayout) findViewById(R.id.content);
    // swipeDismiss.setOnDismissedListener(new SwipeDismissLayout.OnDismissedListener() {
    //     @Override
    //     public void onDismissed(SwipeDismissLayout layout) {
    //         dispatchOnWindowDismissed();
    //     }
    // });
    // swipeDismiss.setOnSwipeProgressChangedListener(
    //         new SwipeDismissLayout.OnSwipeProgressChangedListener() {
    //             private static final float ALPHA_DECREASE = 0.5f;
    //             private boolean mIsTranslucent = false;
    //             @Override
    //             public void onSwipeProgressChanged(
    //                     SwipeDismissLayout layout, float progress, float translate) {
    //                 WindowManager.LayoutParams newParams = getAttributes();
    //                 newParams.x = (int) translate;
    //                 newParams.alpha = 1 - (progress * ALPHA_DECREASE);
    //                 setAttributes(newParams);
    //
    //                 int flags = 0;
    //                 if (newParams.x == 0) {
    //                     flags = FLAG_FULLSCREEN;
    //                 } else {
    //                     flags = FLAG_LAYOUT_NO_LIMITS;
    //                 }
    //                 setFlags(flags, FLAG_FULLSCREEN | FLAG_LAYOUT_NO_LIMITS);
    //             }
    //
    //             @Override
    //             public void onSwipeCancelled(SwipeDismissLayout layout) {
    //                 WindowManager.LayoutParams newParams = getAttributes();
    //                 newParams.x = 0;
    //                 newParams.alpha = 1;
    //                 setAttributes(newParams);
    //                 setFlags(FLAG_FULLSCREEN, FLAG_FULLSCREEN | FLAG_LAYOUT_NO_LIMITS);
    //             }
    //         });
    assert(0);
}

void PhoneWindow::CallOnPanelClosed(
    /* [in] */ Int32 featureId,
    /* [in] */ PanelFeatureState* panel,
    /* [in] */ IMenu* menu)
{
    // ==================before translated======================
    // final Callback cb = getCallback();
    // if (cb == null)
    //     return;
    //
    // // Try to get a menu
    // if (menu == null) {
    //     // Need a panel to grab the menu, so try to get that
    //     if (panel == null) {
    //         if ((featureId >= 0) && (featureId < mPanels.length)) {
    //             panel = mPanels[featureId];
    //         }
    //     }
    //
    //     if (panel != null) {
    //         // menu still may be null, which is okay--we tried our best
    //         menu = panel.menu;
    //     }
    // }
    //
    // // If the panel is not open, do not callback
    // if ((panel != null) && (!panel.isOpen))
    //     return;
    //
    // if (!isDestroyed()) {
    //     cb.onPanelClosed(featureId, menu);
    // }
    assert(0);
}

Boolean PhoneWindow::LaunchDefaultSearch()
{
    // ==================before translated======================
    // boolean result;
    // final Callback cb = getCallback();
    // if (cb == null || isDestroyed()) {
    //     result = false;
    // } else {
    //     sendCloseSystemWindows("search");
    //     result = cb.onSearchRequested();
    // }
    // if (!result && (getContext().getResources().getConfiguration().uiMode
    //         & Configuration.UI_MODE_TYPE_MASK) == Configuration.UI_MODE_TYPE_TELEVISION) {
    //     // On TVs, if the app doesn't implement search, we want to launch assist.
    //     return ((SearchManager)getContext().getSystemService(Context.SEARCH_SERVICE))
    //             .launchAssistAction(0, null, UserHandle.myUserId());
    // }
    // return result;
    assert(0);
    return FALSE;
}

Boolean PhoneWindow::IsTranslucent()
{
    // ==================before translated======================
    // TypedArray a = getWindowStyle();
    // return a.getBoolean(a.getResourceId(
    //         R.styleable.Window_windowIsTranslucent, 0), false);
    assert(0);
    return FALSE;
}

} // namespace Impl
} // namespace Policy
} // namespace Internal
} // namespace Droid
} // namespace Elastos


