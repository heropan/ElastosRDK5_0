
#ifndef __ELASTOS_DROID_APP_DIALOG_H__
#define __ELASTOS_DROID_APP_DIALOG_H__

#include "elastos/droid/os/Runnable.h"
#include "elastos/droid/os/HandlerBase.h"

using Elastos::Core::ICharSequence;
using Elastos::Droid::App::IActionBar;
using Elastos::Droid::App::IActivity;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::IDialogInterfaceOnCancelListener;
using Elastos::Droid::Content::IDialogInterfaceOnDismissListener;
using Elastos::Droid::Content::IDialogInterfaceOnShowListener;
using Elastos::Droid::Content::IDialogInterfaceOnKeyListener;
using Elastos::Droid::Content::IComponentName;
using Elastos::Droid::Graphics::Drawable::IDrawable;
using Elastos::Droid::Internal::App::IActionBarImpl;
using Elastos::Droid::Net::IUri;
using Elastos::Droid::Os::Runnable;
using Elastos::Droid::Os::IBundle;
using Elastos::Droid::Os::HandlerBase;
using Elastos::Droid::View::IWindow;
using Elastos::Droid::View::IMotionEvent;
using Elastos::Droid::View::IWindowManagerLayoutParams;
using Elastos::Droid::View::IMenuItem;
using Elastos::Droid::View::ILayoutInflater;
using Elastos::Droid::View::IContextMenu;
using Elastos::Droid::View::IContextMenuInfo;
using Elastos::Droid::View::IActionModeCallback;
using Elastos::Droid::View::IView;
using Elastos::Droid::View::IViewGroupLayoutParams;
using Elastos::Droid::View::IKeyEvent;
using Elastos::Droid::View::IMenu;
using Elastos::Droid::View::IActionMode;
using Elastos::Droid::View::IWindowManager;
using Elastos::Droid::View::Accessibility::IAccessibilityEvent;

namespace Elastos {
namespace Droid {
namespace App {

class Dialog
    : public Object
    , public IDialog
    , public IDialogInterface
    , public IWindowCallback
    , public IKeyEventCallback
    , public IViewOnCreateContextMenuListener
    , public IWindowOnWindowDismissedCallback
{
private:
    static const Int32 DISMISS = 0x43;
    static const Int32 CANCEL = 0x44;
    static const Int32 SHOW = 0x45;

    static const String DIALOG_SHOWING_TAG;
    static const String DIALOG_HIERARCHY_TAG;

private:
    class DismissAction : public Runnable
    {
    public:
        DismissAction(
            /* [in] */ Dialog* host);

        CARAPI Run();

    private:
        Dialog* mHost;
    };

    class ListenersHandler : public HandlerBase
    {
    public:
        ListenersHandler(
            /* [in] */ IDialog* host);

        CARAPI HandleMessage(
            /* [in] */ IMessage* msg);
    private:
        AutoPtr<IWeakReference> mDialog; //WeakReference<DialogInterface> mDialog;
    };

public:
    /**
     * Create a Dialog window that uses the default dialog frame style.
     *
     * @param context The Context the Dialog is to run it.  In particular, it
     *                uses the window manager and theme in this context to
     *                present its UI.
     */
    Dialog(
        /* [in] */ IContext* context);

    /**
     * Create a Dialog window that uses a custom dialog style.
     *
     * @param context The Context in which the Dialog should run. In particular, it
     *                uses the window manager and theme from this context to
     *                present its UI.
     * @param theme A style resource describing the theme to use for the
     * window. See <a href="{@docRoot}guide/topics/resources/available-resources.html#stylesandthemes">Style
     * and Theme Resources</a> for more information about defining and using
     * styles.  This theme is applied on top of the current theme in
     * <var>context</var>.  If 0, the default dialog theme will be used.
     */
    Dialog(
        /* [in] */ IContext* context,
        /* [in] */ Int32 theme);

    Dialog(
        /* [in] */ IContext* context,
        /* [in] */ Int32 theme,
        /* [in] */ Boolean createContextThemeWrapper);

    Dialog(
        /* [in] */ IContext* context,
        /* [in] */ Boolean cancelable,
        /* [in] */ IDialogInterfaceOnCancelListener* cancelListener);

    virtual ~Dialog();

    virtual CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid) = 0;

    CARAPI_(AutoPtr<IContext>) GetContext();

    /**
     * Retrieve the {@link ActionBar} attached to this dialog, if present.
     *
     * @return The ActionBar attached to the dialog or null if no ActionBar is present.
     */
    CARAPI_(AutoPtr<IActionBar>) GetActionBar();

    CARAPI SetOwnerActivity(
        /* [in] */ IActivity* activity);

    CARAPI_(AutoPtr<IActivity>) GetOwnerActivity();

    virtual CARAPI_(Boolean) IsShowing();

    virtual CARAPI Create();

    virtual CARAPI Show();

    virtual CARAPI Hide();

    virtual CARAPI Dismiss();

    virtual CARAPI DispatchOnCreate(
        /* [in] */ IBundle* savedInstanceState);

    virtual CARAPI_(AutoPtr<IBundle>) OnSaveInstanceState();

    virtual CARAPI OnRestoreInstanceState(
        /* [in] */ IBundle* savedInstanceState);

    virtual CARAPI_(AutoPtr<IWindow>) GetWindow();

    virtual CARAPI_(AutoPtr<IView>) GetCurrentFocus();

   /**
     * Finds a child view with the given identifier. Returns null if the
     * specified child view does not exist or the dialog has not yet been fully
     * created (for example, via {@link #show()} or {@link #create()}).
     *
     * @param id the identifier of the view to find
     * @return The view with the given id or null.
     */
    virtual CARAPI_(AutoPtr<IView>) FindViewById(
        /* [in] */ Int32 id);

    virtual CARAPI SetContentView(
        /* [in] */ Int32 layoutResID);

    virtual CARAPI SetContentView(
        /* [in] */ IView* view);

    virtual CARAPI SetContentView(
        /* [in] */ IView* view,
        /* [in] */ IViewGroupLayoutParams* params);

    virtual CARAPI AddContentView(
        /* [in] */ IView* view,
        /* [in] */ IViewGroupLayoutParams* params);

    virtual CARAPI SetTitle(
        /* [in] */ ICharSequence* title);

    virtual CARAPI SetTitle(
        /* [in] */ Int32 titleId);

    virtual CARAPI_(Boolean) OnKeyDown(
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event);

    virtual CARAPI_(Boolean) OnKeyLongPress(
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event);

    virtual CARAPI_(Boolean) OnKeyUp(
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event);

    virtual CARAPI_(Boolean) OnKeyMultiple(
        /* [in] */ Int32 keyCode,
        /* [in] */ Int32 repeatCount,
        /* [in] */ IKeyEvent* event);

    virtual CARAPI OnBackPressed();

    virtual CARAPI_(Boolean) OnKeyShortcut(
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event);

    virtual CARAPI_(Boolean) OnTouchEvent(
        /* [in] */ IMotionEvent* event);

    virtual CARAPI_(Boolean) OnTrackballEvent(
        /* [in] */ IMotionEvent* event);

    /**
     * Called when a generic motion event was not handled by any of the
     * views inside of the dialog.
     * <p>
     * Generic motion events describe joystick movements, mouse hovers, track pad
     * touches, scroll wheel movements and other input events.  The
     * {@link MotionEvent#getSource() source} of the motion event specifies
     * the class of input that was received.  Implementations of this method
     * must examine the bits in the source before processing the event.
     * The following code example shows how this is done.
     * </p><p>
     * Generic motion events with source class
     * {@link android.view.InputDevice#SOURCE_CLASS_POINTER}
     * are delivered to the view under the pointer.  All other generic motion events are
     * delivered to the focused view.
     * </p><p>
     * See {@link View#onGenericMotionEvent(MotionEvent)} for an example of how to
     * handle this event.
     * </p>
     *
     * @param event The generic motion event being processed.
     *
     * @return Return true if you have consumed the event, false if you haven't.
     * The default implementation always returns false.
     */
    virtual CARAPI_(Boolean) OnGenericMotionEvent(
        /* [in] */ IMotionEvent* event);

    virtual CARAPI OnWindowAttributesChanged(
        /* [in] */ IWindowManagerLayoutParams* params);

    virtual CARAPI OnContentChanged();

    virtual CARAPI OnWindowFocusChanged(
        /* [in] */ Boolean hasFocus);

    virtual CARAPI OnAttachedToWindow();

    virtual CARAPI OnDetachedFromWindow();

    /** @hide */
    //@Override
    CARAPI OnWindowDismissed();

    virtual CARAPI_(Boolean) DispatchKeyEvent(
        /* [in] */ IKeyEvent* event);

    /**
     * Called to process a key shortcut event.
     * You can override this to intercept all key shortcut events before they are
     * dispatched to the window.  Be sure to call this implementation for key shortcut
     * events that should be handled normally.
     *
     * @param event The key shortcut event.
     * @return True if this event was consumed.
     */
    virtual CARAPI_(Boolean) DispatchKeyShortcutEvent(
        /* [in] */ IKeyEvent* event);

    virtual CARAPI_(Boolean) DispatchTouchEvent(
        /* [in] */ IMotionEvent* ev);

    virtual CARAPI_(Boolean) DispatchTrackballEvent(
        /* [in] */ IMotionEvent* ev);

    /**
     * Called to process generic motion events.  You can override this to
     * intercept all generic motion events before they are dispatched to the
     * window.  Be sure to call this implementation for generic motion events
     * that should be handled normally.
     *
     * @param ev The generic motion event.
     *
     * @return boolean Return true if this event was consumed.
     */
    virtual CARAPI_(Boolean) DispatchGenericMotionEvent(
        /* [in] */ IMotionEvent* ev);

    virtual CARAPI_(Boolean) DispatchPopulateAccessibilityEvent(
        /* [in] */ IAccessibilityEvent* event);

    virtual CARAPI_(AutoPtr<IView>) OnCreatePanelView(
        /* [in] */ Int32 featureId);

    virtual CARAPI_(Boolean) OnCreatePanelMenu(
        /* [in] */ Int32 featureId,
        /* [in] */ IMenu* menu);

    virtual CARAPI_(Boolean) OnPreparePanel(
        /* [in] */ Int32 featureId,
        /* [in] */ IView* view,
        /* [in] */ IMenu* menu);

    virtual CARAPI_(Boolean) OnMenuOpened(
        /* [in] */ Int32 featureId,
        /* [in] */ IMenu* menu);

    virtual CARAPI_(Boolean) OnMenuItemSelected(
        /* [in] */ Int32 featureId,
        /* [in] */ IMenuItem* item);

    virtual CARAPI OnPanelClosed(
        /* [in] */ Int32 featureId,
        /* [in] */ IMenu* menu);

    virtual CARAPI_(Boolean) OnCreateOptionsMenu(
        /* [in] */ IMenu* menu);

    virtual CARAPI_(Boolean) OnPrepareOptionsMenu(
        /* [in] */ IMenu* menu);

    virtual CARAPI_(Boolean) OnOptionsItemSelected(
        /* [in] */ IMenuItem* item);

    virtual CARAPI OnOptionsMenuClosed(
        /* [in] */ IMenu* menu);

    virtual CARAPI OpenOptionsMenu();

    virtual CARAPI CloseOptionsMenu();

    /**
     * @see Activity#invalidateOptionsMenu()
     */
    virtual CARAPI InvalidateOptionsMenu();

    virtual CARAPI OnCreateContextMenu(
        /* [in] */ IContextMenu* menu,
        /* [in] */ IView* v,
        /* [in] */ IContextMenuInfo* menuInfo);

    virtual CARAPI RegisterForContextMenu(
        /* [in] */ IView* view);

    virtual CARAPI UnregisterForContextMenu(
        /* [in] */ IView* view);

    virtual CARAPI OpenContextMenu(
        /* [in] */ IView* view);

    virtual CARAPI_(Boolean) OnContextItemSelected(
        /* [in] */ IMenuItem* item);

    virtual CARAPI OnContextMenuClosed(
        /* [in] */ IMenu* menu);

    virtual CARAPI_(Boolean) OnSearchRequested();

    virtual CARAPI_(AutoPtr<IActionMode>) OnWindowStartingActionMode(
        /* [in] */ IActionModeCallback* callback);

    /**
     * {@inheritDoc}
     *
     * Note that if you override this method you should always call through
     * to the superclass implementation by calling super.onActionModeStarted(mode).
     */
    virtual CARAPI OnActionModeStarted(
        /* [in] */ IActionMode* mode);

    /**
     * {@inheritDoc}
     *
     * Note that if you override this method you should always call through
     * to the superclass implementation by calling super.onActionModeFinished(mode).
     */
    virtual CARAPI OnActionModeFinished(
        /* [in] */ IActionMode* mode);

    virtual CARAPI TakeKeyEvents(
        /* [in] */ Boolean get);

    CARAPI_(Boolean) RequestWindowFeature(
        /* [in] */ Int32 featureId);

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

    virtual CARAPI_(AutoPtr<ILayoutInflater>) GetLayoutInflater();

    virtual CARAPI SetCancelable(
        /* [in] */ Boolean flag);

    virtual CARAPI SetCanceledOnTouchOutside(
        /* [in] */ Boolean cancel);

    virtual CARAPI Cancel();

    virtual CARAPI SetOnCancelListener(
        /* [in] */ IDialogInterfaceOnCancelListener* listener);

    virtual CARAPI SetOnDismissListener(
        /* [in] */ IDialogInterfaceOnDismissListener* listener);

    virtual CARAPI SetOnShowListener(
        /* [in] */ IDialogInterfaceOnShowListener* listener);

    virtual CARAPI SetCancelMessage(
        /* [in] */ IMessage* msg);

    virtual CARAPI SetDismissMessage(
        /* [in] */ IMessage* msg);

    virtual CARAPI_(Boolean) TakeCancelAndDismissListeners(
        /* [in] */ const String& msg,
        /* [in] */ IDialogInterfaceOnCancelListener* cancel,
        /* [in] */ IDialogInterfaceOnDismissListener* dismiss);

    CARAPI SetVolumeControlStream(
        /* [in] */ Int32 streamType);

    CARAPI_(Int32) GetVolumeControlStream();

    virtual CARAPI SetOnKeyListener(
        /* [in] */ IDialogInterfaceOnKeyListener* onKeyListener);

    static CARAPI_(Int32) GetResourceId(
            /* [in] */ IContext* context,
            /* [in] */ Int32 attrId);

protected:
    Dialog();

    virtual CARAPI OnCreate(
        /* [in] */ IBundle* savedInstanceState);

    virtual CARAPI OnStart();

    virtual CARAPI OnStop();

    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ Int32 theme = 0);

    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ Int32 theme,
        /* [in] */ Boolean createContextThemeWrapper);

    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ Boolean cancelable,
        /* [in] */ IDialogInterfaceOnCancelListener* cancelListener);

private:
    CARAPI_(void) DismissDialog();

    CARAPI_(void) SendDismissMessage();

    CARAPI_(void) SendShowMessage();

    CARAPI_(AutoPtr<IComponentName>) GetAssociatedActivity();

public:
    AutoPtr<IContext> mContext;     // memory leak! IContext may hold Dialog. luo.zhaohui
    AutoPtr<IWindowManager> mWindowManager;
    AutoPtr<IWindow> mWindow;
    AutoPtr<IView> mDecor;

    /**
     * This field should be made private, so it is hidden from the SDK.
     * {@hide}
     */
    Boolean mCancelable;

private:
    const static String TAG;

    AutoPtr<IWeakReference> mWeakOwnerActivity;
    // AutoPtr<IActivity> mOwnerActivity;
    AutoPtr<IActionBar> mActionBar;

    String mCancelAndDismissTaken;
    AutoPtr<IMessage> mCancelMessage;
    AutoPtr<IMessage> mDismissMessage;
    AutoPtr<IMessage> mShowMessage;

    AutoPtr<IDialogInterfaceOnCancelListener> mCancelListener;
    AutoPtr<IDialogInterfaceOnDismissListener> mDismissListener;
    AutoPtr<IDialogInterfaceOnShowListener> mShowListener;

    AutoPtr<IDialogInterfaceOnKeyListener> mOnKeyListener;

    Boolean mCreated;
    Boolean mShowing;
    Boolean mCanceled;

    AutoPtr<IHandler> mHandler;

    AutoPtr<IHandler> mListenersHandler;

    AutoPtr<IActionMode> mActionMode;

    AutoPtr<IRunnable> mDismissAction;
};

} // namespace App
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_APP_DIALOG_H__
