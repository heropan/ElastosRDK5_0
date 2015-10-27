
#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/app/Dialog.h"
#include "elastos/droid/app/CActionBarImpl.h"
#include "elastos/droid/os/Looper.h"
#include "elastos/droid/os/Handler.h"
#include "elastos/droid/os/CBundle.h"
#include "elastos/droid/os/CHandler.h"
#include "elastos/droid/os/CMessageHelper.h"
#include "elastos/droid/R.h"
#include "elastos/droid/impl/CPolicyManager.h"
#include "elastos/droid/impl/CPolicyManager.h"
#include "elastos/droid/view/CWindowManagerLayoutParams.h"
#include "elastos/droid/view/CContextThemeWrapper.h"
#include "elastos/droid/utility/CTypedValue.h"
#include <elastos/utility/logging/Slogger.h>

using Elastos::Utility::Logging::Slogger;
using Elastos::Core::CString;
using Elastos::Droid::R;
using Elastos::Droid::Os::Looper;
using Elastos::Droid::Os::ILooper;
using Elastos::Droid::Os::CHandler;
using Elastos::Droid::Os::CBundle;
using Elastos::Droid::Os::IMessageHelper;
using Elastos::Droid::Os::CMessageHelper;
using Elastos::Droid::Content::IContextWrapper;
using Elastos::Droid::Content::IDialogInterface;
using Elastos::Droid::Content::EIID_IDialogInterface;
using Elastos::Droid::Content::Res::IResourcesTheme;
using Elastos::Droid::Internal::Policy::IPolicyManager;
using Elastos::Droid::Internal::Policy::CPolicyManager;
using Elastos::Droid::Internal::App::CActionBarImpl;
using Elastos::Droid::Utility::CTypedValue;
using Elastos::Droid::Utility::ITypedValue;
using Elastos::Droid::View::IGravity;
using Elastos::Droid::View::IWindowCallback;
using Elastos::Droid::View::EIID_IWindowCallback;
using Elastos::Droid::View::IDispatcherState;
using Elastos::Droid::View::IKeyEventCallback;
using Elastos::Droid::View::EIID_IKeyEventCallback;
using Elastos::Droid::View::IContextThemeWrapper;
using Elastos::Droid::View::CContextThemeWrapper;
using Elastos::Droid::View::CWindowManagerLayoutParams;
using Elastos::Droid::View::IWindowManagerLayoutParams;
using Elastos::Droid::View::IViewOnCreateContextMenuListener;
using Elastos::Droid::View::EIID_IViewOnCreateContextMenuListener;

namespace Elastos {
namespace Droid {
namespace App {

const Int32 Dialog::DISMISS;
const Int32 Dialog::CANCEL;
const Int32 Dialog::SHOW;

const String Dialog::TAG("Dialog");
const String Dialog::DIALOG_SHOWING_TAG("android:dialogShowing");
const String Dialog::DIALOG_HIERARCHY_TAG("android:dialogHierarchy");

//==============================================================================
//            Dialog::ListenersHandler
//==============================================================================
Dialog::ListenersHandler::ListenersHandler(
    /* [in] */ IDialog* dialog)
{
    AutoPtr<IWeakReferenceSource> wrs = IWeakReferenceSource::Probe(dialog);
    if (wrs) {
        wrs->GetWeakReference((IWeakReference**)&mDialog);
    }
}

ECode Dialog::ListenersHandler::HandleMessage(
    /* [in] */ IMessage* msg)
{
    Int32 what;
    msg->GetWhat(&what);
    AutoPtr<IInterface> obj;
    msg->GetObj((IInterface**)&obj);

    AutoPtr<IInterface> dialogObj;
    mDialog->Resolve(EIID_IDialogInterface, (IInterface**)&dialogObj);
    IDialogInterface* dialog = IDialogInterface::Probe(dialogObj);

    switch (what) {
        case Dialog::DISMISS: {
            IDialogInterfaceOnDismissListener* listener = IDialogInterfaceOnDismissListener::Probe(obj);
            if (listener) {
                listener->OnDismiss(dialog);
            }
            break;
        }
        case Dialog::CANCEL: {
            IDialogInterfaceOnCancelListener* listener = IDialogInterfaceOnCancelListener::Probe(obj);
            if (listener) {
                listener->OnCancel(dialog);
            }
            break;
        }
        case Dialog::SHOW: {
            IDialogInterfaceOnShowListener* listener = IDialogInterfaceOnShowListener::Probe(obj);
            if (listener) {
                listener->OnShow(dialog);
            }
            break;
        }
    }
    return NOERROR;
}


//==============================================================================
//            Dialog::DismissAction
//==============================================================================

Dialog::DismissAction::DismissAction(
    /* [in] */ Dialog* host)
    : mHost(host)
{}

ECode Dialog::DismissAction::Run()
{
    mHost->DismissDialog();

    return NOERROR;
}


//==============================================================================
//            Dialog
//==============================================================================

Dialog::Dialog()
    : mCancelable(TRUE)
    , mCreated(FALSE)
    , mShowing(FALSE)
    , mCanceled(FALSE)
    , mDismissAction(new DismissAction(this))
{}

Dialog::Dialog(
    /* [in] */ IContext* context)
    : mCancelable(TRUE)
    , mCreated(FALSE)
    , mShowing(FALSE)
    , mCanceled(FALSE)
    , mDismissAction(new DismissAction(this))
{
    Init(context);
}

Dialog::Dialog(
    /* [in] */ IContext* context,
    /* [in] */ Int32 theme)
    : mCancelable(TRUE)
    , mCreated(FALSE)
    , mShowing(FALSE)
    , mCanceled(FALSE)
    , mDismissAction(new DismissAction(this))
{
    Init(context, theme);
}

Dialog::Dialog(
    /* [in] */ IContext* context,
    /* [in] */ Int32 theme,
    /* [in] */ Boolean createContextThemeWrapper)
    : mCancelable(TRUE)
    , mCreated(FALSE)
    , mShowing(FALSE)
    , mCanceled(FALSE)
    , mDismissAction(new DismissAction(this))
{
    Init(context, theme, createContextThemeWrapper);
}

Dialog::Dialog(
    /* [in] */ IContext* context,
    /* [in] */ Boolean cancelable,
    /* [in] */ IDialogInterfaceOnCancelListener* cancelListener)
    : mCancelable(TRUE)
    , mCreated(FALSE)
    , mShowing(FALSE)
    , mCanceled(FALSE)
    , mDismissAction(new DismissAction(this))
{
    Slogger::V(TAG, " >> create Dialog(): %p", this);
    Init(context, cancelable, cancelListener);
}

Dialog::~Dialog()
{
    mWindow->SetCallback(NULL);
    Slogger::V(TAG, ">> destory ~Dialog(): %p", this);
}

ECode Dialog::Init(
    /* [in] */ IContext* context,
    /* [in] */ Int32 theme)
{
    return Init(context, theme, TRUE);
}

ECode Dialog::Init(
    /* [in] */ IContext* context,
    /* [in] */ Int32 theme,
    /* [in] */ Boolean createContextThemeWrapper)
{
    VALIDATE_NOT_NULL(context);

    if (createContextThemeWrapper) {
        if (theme == 0) {
            theme = GetResourceId(context, R::attr::dialogTheme);
        }
        CContextThemeWrapper::New(
                context, theme, (IContextThemeWrapper**)&mContext);
    } else {
        mContext = context;
    }

    context->GetSystemService(IContext::WINDOW_SERVICE, (IInterface**)&mWindowManager);

    AutoPtr<IPolicyManager> pm;
    CPolicyManager::AcquireSingleton((IPolicyManager**)&pm);
    pm->MakeNewWindow(mContext, (IWindow**)&mWindow);

    mWindow->SetCallback(THIS_PROBE(IWindowCallback));
    mWindow->SetOnWindowDismissedCallback(THIS_PROBE(IWindowOnWindowDismissedCallback));
    mWindow->SetWindowManager(mWindowManager, NULL, String(NULL));
    mWindow->SetGravity(IGravity::CENTER);

    mListenersHandler = new ListenersHandler(THIS_PROBE(IDialog));
    CHandler::New((IHandler**)&mHandler);
    assert(mHandler != NULL);

    return NOERROR;
}

ECode Dialog::Init(
    /* [in] */ IContext* context,
    /* [in] */ Boolean cancelable,
    /* [in] */ IDialogInterfaceOnCancelListener* cancelListener)
{
    FAIL_RETURN(Init(context));
    mCancelable = cancelable;
    SetOnCancelListener(cancelListener);

    return NOERROR;
}

Int32 Dialog::GetResourceId(
    /* [in] */ IContext* context,
    /* [in] */ Int32 attrId)
{
    AutoPtr<ITypedValue> outValue;
    CTypedValue::New((ITypedValue**)&outValue);
    AutoPtr<IResourcesTheme> resourcesTheme;
    context->GetTheme((IResourcesTheme**)&resourcesTheme);
    Boolean isFounded;
    resourcesTheme->ResolveAttribute(attrId, outValue, TRUE, &isFounded);
    Int32 resourceId;
    outValue->GetResourceId(&resourceId);
    return resourceId;
}

/**
 * Retrieve the Context this Dialog is running in.
 *
 * @return Context The Context that was supplied to the constructor.
 */
AutoPtr<IContext> Dialog::GetContext()
{
    return mContext;
}

/**
 * Retrieve the {@link ActionBar} attached to this dialog, if present.
 *
 * @return The ActionBar attached to the dialog or NULL if no ActionBar is present.
 */
AutoPtr<IActionBar> Dialog::GetActionBar()
{
    return mActionBar;
}

/**
 * Sets the Activity that owns this dialog. An example use: This Dialog will
 * use the suggested volume control stream of the Activity.
 *
 * @param activity The Activity that owns this dialog.
 */
ECode Dialog::SetOwnerActivity(
    /* [in] */ IActivity* activity)
{
    AutoPtr<IWeakReferenceSource> wrs = IWeakReferenceSource::Probe(activity);
    VALIDATE_NOT_NULL(wrs);

    mWeakOwnerActivity = NULL;
    wrs->GetWeakReference((IWeakReference**)&mWeakOwnerActivity);

    Int32 streamType;
    activity->GetVolumeControlStream(&streamType);
    return GetWindow()->SetVolumeControlStream(streamType);
}

/**
 * Returns the Activity that owns this Dialog. For example, if
 * {@link Activity#showDialog(Int32)} is used to show this Dialog, that
 * Activity will be the owner (by default). Depending on how this dialog was
 * created, this may return NULL.
 *
 * @return The Activity that owns this Dialog.
 */
AutoPtr<IActivity> Dialog::GetOwnerActivity()
{
    AutoPtr<IActivity> activity;
    if (mWeakOwnerActivity) {
        mWeakOwnerActivity->Resolve(EIID_IActivity, (IInterface**)&activity);
    }
    return activity;
}

/**
 * @return Whether the dialog is currently showing.
 */
Boolean Dialog::IsShowing()
{
    return mShowing;
}

/**
 * Forces immediate creation of the dialog.
 * <p>
 * Note that you should not override this method to perform dialog creation.
 * Rather, override {@link #onCreate(Bundle)}.
 */
ECode Dialog::Create()
{
    if (!mCreated) {
        return DispatchOnCreate(NULL);
    }
    return NOERROR;
}

/**
 * Start the dialog and display it on screen.  The window is placed in the
 * application layer and opaque.  Note that you should not override this
 * method to do initialization when the dialog is shown, instead implement
 * that in {@link #OnStart}.
 */
ECode Dialog::Show()
{
    if (mShowing) {
        if (mDecor != NULL) {
            Boolean hasFeature;
            mWindow->HasFeature(IWindow::FEATURE_ACTION_BAR, &hasFeature);
            if (hasFeature) {
                mWindow->InvalidatePanelMenu(IWindow::FEATURE_ACTION_BAR);
            }
            mDecor->SetVisibility(IView::VISIBLE);
        }
        return NOERROR;
    }

    mCanceled = FALSE;

    if (!mCreated) {
        DispatchOnCreate(NULL);
    }

    OnStart();
    mDecor = NULL;
    mWindow->GetDecorView((IView**)&mDecor);

    Boolean hasFeature;
    mWindow->HasFeature(IWindow::FEATURE_ACTION_BAR, &hasFeature);
        AutoPtr<IApplicationInfo> info;
        mContext->GetApplicationInfo((IApplicationInfo**)&info);
        mWindow.setDefaultIcon(info.icon);
        mWindow.setDefaultLogo(info.logo);
        mActionBar = new WindowDecorActionBar(this);
    }

    AutoPtr<IWindowManagerLayoutParams> l;
    mWindow->GetAttributes((IWindowManagerLayoutParams**)&l);
    Int32 inputMode;
    l->GetSoftInputMode(&inputMode);
    if ((inputMode & IWindowManagerLayoutParams::SOFT_INPUT_IS_FORWARD_NAVIGATION) == 0) {
        AutoPtr<IWindowManagerLayoutParams> nl;
        CWindowManagerLayoutParams::New((IWindowManagerLayoutParams**)&nl);
        Int32 changes;
        nl->CopyFrom(l, &changes);
        Int32 nlInputMode;
        nl->GetSoftInputMode(&nlInputMode);
        nl->SetSoftInputMode(nlInputMode | IWindowManagerLayoutParams::SOFT_INPUT_IS_FORWARD_NAVIGATION);
        l = nl;
    }

    mWindowManager->AddView(mDecor, l);
    mShowing = TRUE;

    SendShowMessage();

    return NOERROR;
}

/**
 * Hide the dialog, but do not dismiss it.
 */
ECode Dialog::Hide()
{
    if (mDecor != NULL) {
        mDecor->SetVisibility(IView::GONE);
    }

    return NOERROR;
}

/**
 * Dismiss this dialog, removing it from the screen. This method can be
 * invoked safely from any thread.  Note that you should not override this
 * method to do cleanup when the dialog is dismissed, instead implement
 * that in {@link #OnStop}.
 */
ECode Dialog::Dismiss()
{
    AutoPtr<ILooper> looper = Looper::GetMyLooper();
    AutoPtr<ILooper> handlerLooper;
    mHandler->GetLooper((ILooper**)&handlerLooper);
    if (looper == handlerLooper) {
        DismissDialog();
    }
    else {
        Boolean result;
        mHandler->Post(mDismissAction, &result);
    }

    return NOERROR;
}

void Dialog::DismissDialog()
{
    if (mDecor == NULL || !mShowing) {
        return;
    }

    Boolean destroyed;
    mWindow->IsDestroyed(&destroyed);
    if (destroyed) {
        // Slogger::E(TAG, String("Tried to dismissDialog() but the Dialog's window was already destroyed!"));
        return;
    }

    mWindowManager->RemoveViewImmediate(mDecor);
    if (mActionMode != NULL) {
        mActionMode->Finish();
    }
    mDecor = NULL;
    mWindow->CloseAllPanels();
    OnStop();
    mShowing = FALSE;

    SendDismissMessage();
}

void Dialog::SendDismissMessage()
{
    if (mDismissMessage != NULL) {
        AutoPtr<IMessageHelper> helper;
        CMessageHelper::AcquireSingleton((IMessageHelper**)&helper);
        AutoPtr<IMessage> msg;
        helper->Obtain(mDismissMessage, (IMessage**)&msg);
        // Obtain a new message so this dialog can be re-used
        msg->SendToTarget();
    }
}

void Dialog::SendShowMessage()
{
    if (mShowMessage != NULL) {
        AutoPtr<IMessageHelper> helper;
        CMessageHelper::AcquireSingleton((IMessageHelper**)&helper);
        AutoPtr<IMessage> msg;
        helper->Obtain(mShowMessage, (IMessage**)&msg);
        // Obtain a new message so this dialog can be re-used
        msg->SendToTarget();
    }
}

// internal method to make sure mcreated is set properly without requiring
// users to call through to super in onCreate
ECode Dialog::DispatchOnCreate(
    /* [in] */ IBundle* savedInstanceState)
{
    if (!mCreated) {
        OnCreate(savedInstanceState);
        mCreated = TRUE;
    }

    return NOERROR;
}

/**
 * Similar to {@link Activity#onCreate}, you should initialize your dialog
 * in this method, including calling {@link #setContentView}.
 * @param savedInstanceState If this dialog is being reinitalized after a
 *     the hosting activity was previously shut down, holds the result from
 *     the most recent call to {@link #onSaveInstanceState}, or NULL if this
 *     is the first time.
 */
void Dialog::OnCreate(
    /* [in] */ IBundle* savedInstanceState)
{
}

/**
 * Called when the dialog is starting.
 */
void Dialog::OnStart()
{
    if (mActionBar != NULL) mActionBar->SetShowHideAnimationEnabled(TRUE);
}

/**
 * Called to tell you that you're stopping.
 */
void Dialog::OnStop()
{
    if (mActionBar != NULL) mActionBar->SetShowHideAnimationEnabled(FALSE);
}

/**
 * Saves the state of the dialog into a bundle.
 *
 * The default implementation saves the state of its view hierarchy, so you'll
 * likely want to call through to super if you override this to save additional
 * state.
 * @return A bundle with the state of the dialog.
 */
AutoPtr<IBundle> Dialog::OnSaveInstanceState()
{
    AutoPtr<IBundle> bundle;
    CBundle::New((IBundle**)&bundle);
    bundle->PutBoolean(DIALOG_SHOWING_TAG, mShowing);
    if (mCreated) {
        AutoPtr<IBundle> wBundle;
        mWindow->SaveHierarchyState((IBundle**)&wBundle);
        bundle->PutBundle(DIALOG_HIERARCHY_TAG, wBundle);
    }
    return bundle;
}

/**
 * Restore the state of the dialog from a previously saved bundle.
 *
 * The default implementation restores the state of the dialog's view
 * hierarchy that was saved in the default implementation of {@link #onSaveInstanceState()},
 * so be sure to call through to super when overriding unless you want to
 * do all restoring of state yourself.
 * @param savedInstanceState The state of the dialog previously saved by
 *     {@link #onSaveInstanceState()}.
 */
ECode Dialog::OnRestoreInstanceState(
    /* [in] */ IBundle* savedInstanceState)
{
    VALIDATE_NOT_NULL(savedInstanceState);

    AutoPtr<IBundle> dialogHierarchyState;
    savedInstanceState->GetBundle(DIALOG_HIERARCHY_TAG, (IBundle**)&dialogHierarchyState);
    if (dialogHierarchyState == NULL) {
        // dialog has never been shown, or onCreated, nothing to restore.
        return NOERROR;
    }

    DispatchOnCreate(savedInstanceState);
    mWindow->RestoreHierarchyState(dialogHierarchyState);
    Boolean show;
    savedInstanceState->GetBoolean(DIALOG_SHOWING_TAG, &show);
    if (show) {
        Show();
    }

    return NOERROR;
}

/**
 * Retrieve the current Window for the activity.  This can be used to
 * directly access parts of the Window API that are not available
 * through Activity/Screen.
 *
 * @return Window The current window, or NULL if the activity is not
 *         visual.
 */
AutoPtr<IWindow> Dialog::GetWindow()
{
    return mWindow;
}

/**
 * Call {@link android.view.Window#getCurrentFocus} on the
 * Window if this Activity to return the currently focused view.
 *
 * @return View The current View with focus or NULL.
 *
 * @see #GetWindow
 * @see android.view.Window#getCurrentFocus
 */
AutoPtr<IView> Dialog::GetCurrentFocus()
{
    AutoPtr<IView> focus;
    if (mWindow != NULL) {
        mWindow->GetCurrentFocus((IView**)&focus);
    }

    return focus;
}

/**
 * Finds a view that was identified by the id attribute from the XML that
 * was processed in {@link #OnStart}.
 *
 * @param id the identifier of the view to find
 * @return The view if found or NULL otherwise.
 */
AutoPtr<IView> Dialog::FindViewById(
    /* [in] */ Int32 id)
{
    AutoPtr<IView> view;
    mWindow->FindViewById(id, (IView**)&view);

    return view;
}

/**
 * Set the screen content from a layout resource.  The resource will be
 * inflated, adding all top-level views to the screen.
 *
 * @param layoutResID Resource ID to be inflated.
 */
ECode Dialog::SetContentView(
    /* [in] */ Int32 layoutResID)
{
    return mWindow->SetContentView(layoutResID);
}

/**
 * Set the screen content to an explicit view.  This view is placed
 * directly into the screen's view hierarchy.  It can itself be a complex
 * view hierarhcy.
 *
 * @param view The desired content to display.
 */
ECode Dialog::SetContentView(
    /* [in] */ IView* view)
{
    return mWindow->SetContentView(view);
}

/**
 * Set the screen content to an explicit view.  This view is placed
 * directly into the screen's view hierarchy.  It can itself be a complex
 * view hierarhcy.
 *
 * @param view The desired content to display.
 * @param params Layout parameters for the view.
 */
ECode Dialog::SetContentView(
    /* [in] */ IView* view,
    /* [in] */ IViewGroupLayoutParams* params)
{
    return mWindow->SetContentView(view, params);
}

/**
 * Add an additional content view to the screen.  Added after any existing
 * ones in the screen -- existing views are NOT removed.
 *
 * @param view The desired content to display.
 * @param params Layout parameters for the view.
 */
ECode Dialog::AddContentView(
    /* [in] */ IView* view,
    /* [in] */ IViewGroupLayoutParams* params)
{
    return mWindow->AddContentView(view, params);
}

/**
 * Set the title text for this dialog's window.
 *
 * @param title The new text to display in the title.
 */
ECode Dialog::SetTitle(
    /* [in] */ ICharSequence* title)
{
    mWindow->SetTitle(title);
    AutoPtr<IWindowManagerLayoutParams> l;
    mWindow->GetAttributes((IWindowManagerLayoutParams**)&l);
    l->SetTitle(title);

    return NOERROR;
}

/**
 * Set the title text for this dialog's window. The text is retrieved
 * from the resources with the supplied identifier.
 *
 * @param titleId the title's text resource identifier
 */
ECode Dialog::SetTitle(
    /* [in] */ Int32 titleId)
{
    AutoPtr<ICharSequence> title;
    mContext->GetText(titleId, (ICharSequence**)&title);

    return SetTitle(title);
}

/**
 * A key was pressed down.
 *
 * <p>If the focused view didn't want this event, this method is called.
 *
 * <p>The default implementation consumed the KEYCODE_BACK to later
 * handle it in {@link #onKeyUp}.
 *
 * @see #onKeyUp
 * @see android.view.KeyEvent
 */
Boolean Dialog::OnKeyDown(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event)
{
    if (keyCode == IKeyEvent::KEYCODE_BACK) {
        event->StartTracking();
        return TRUE;
    }

    return FALSE;
}

/**
 * Default implementation of {@link KeyEvent.Callback#onKeyLongPress(Int32, KeyEvent)
 * KeyEvent.Callback.onKeyLongPress()}: always returns FALSE (doesn't handle
 * the event).
 */
Boolean Dialog::OnKeyLongPress(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event)
{
    return FALSE;
}

/**
 * A key was released.
 *
 * <p>The default implementation handles KEYCODE_BACK to close the
 * dialog.
 *
 * @see #onKeyDown
 * @see KeyEvent
 */
Boolean Dialog::OnKeyUp(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event)
{
    Boolean isTracking;
    Boolean isCanceled;
    if (keyCode == IKeyEvent::KEYCODE_BACK
        && (event->IsTracking(&isTracking), isTracking)
        && !(event->IsCanceled(&isCanceled), isCanceled)) {
        OnBackPressed();
        return TRUE;
    }
    return FALSE;
}

/**
 * Default implementation of {@link KeyEvent.Callback#onKeyMultiple(Int32, Int32, KeyEvent)
 * KeyEvent.Callback.onKeyMultiple()}: always returns FALSE (doesn't handle
 * the event).
 */
Boolean Dialog::OnKeyMultiple(
    /* [in] */ Int32 keyCode,
    /* [in] */ Int32 repeatCount,
    /* [in] */ IKeyEvent* event)
{
    return FALSE;
}

/**
 * Called when the dialog has detected the user's press of the back
 * key.  The default implementation simply cancels the dialog (only if
 * it is cancelable), but you can override this to do whatever you want.
 */
ECode Dialog::OnBackPressed()
{
    if (mCancelable) {
        Cancel();
    }

    return NOERROR;
}

/**
 * Called when a key shortcut event is not handled by any of the views in the Dialog.
 * Override this method to implement global key shortcuts for the Dialog.
 * Key shortcuts can also be implemented by setting the
 * {@link MenuItem#setShortcut(char, char) shortcut} property of menu items.
 *
 * @param keyCode The value in event.getKeyCode().
 * @param event Description of the key event.
 * @return True if the key shortcut was handled.
 */
Boolean Dialog::OnKeyShortcut(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event)
{
    return FALSE;
}

/**
 * Called when a touch screen event was not handled by any of the views
 * under it. This is most useful to process touch events that happen outside
 * of your window bounds, where there is no view to receive it.
 *
 * @param event The touch screen event being processed.
 * @return Return TRUE if you have consumed the event, FALSE if you haven't.
 *         The default implementation will cancel the dialog when a touch
 *         happens outside of the window bounds.
 */
Boolean Dialog::OnTouchEvent(
    /* [in] */ IMotionEvent* event)
{
    // Int32 action;
    Boolean result;
    mWindow->ShouldCloseOnTouch(mContext, event, &result);
    if (mCancelable && mShowing && result) {
        Cancel();
        return TRUE;
    }

    return FALSE;
}

/**
 * Called when the trackball was moved and not handled by any of the
 * views inside of the activity.  So, for example, if the trackball moves
 * while focus is on a button, you will receive a call here because
 * buttons do not normally do anything with trackball events.  The call
 * here happens <em>before</em> trackball movements are converted to
 * DPAD key events, which then get sent back to the view hierarchy, and
 * will be processed at the point for things like focus navigation.
 *
 * @param event The trackball event being processed.
 *
 * @return Return TRUE if you have consumed the event, FALSE if you haven't.
 * The default implementation always returns FALSE.
 */
Boolean Dialog::OnTrackballEvent(
    /* [in] */ IMotionEvent* event)
{
    return FALSE;
}

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
Boolean Dialog::OnGenericMotionEvent(
    /* [in] */ IMotionEvent* event)
{
    return FALSE;
}

ECode Dialog::OnWindowAttributesChanged(
    /* [in] */ IWindowManagerLayoutParams* params)
{
    if (mDecor != NULL) {
        mWindowManager->UpdateViewLayout(mDecor, params);
    }

    return NOERROR;
}

ECode Dialog::OnContentChanged()
{
    return NOERROR;
}

ECode Dialog::OnWindowFocusChanged(
    /* [in] */ Boolean hasFocus)
{
    return NOERROR;
}

ECode Dialog::OnAttachedToWindow()
{
    return NOERROR;
}

ECode Dialog::OnDetachedFromWindow()
{
    return NOERROR;
}

ECode Dialog::OnWindowDismissed()
{
    return Dismiss();
}

/**
 * Called to process key events.  You can override this to intercept all
 * key events before they are dispatched to the window.  Be sure to call
 * this implementation for key events that should be handled normally.
 *
 * @param event The key event.
 *
 * @return Boolean Return TRUE if this event was consumed.
 */
Boolean Dialog::DispatchKeyEvent(
    /* [in] */ IKeyEvent* event)
{
    Int32 keyCode;
    event->GetKeyCode(&keyCode);
    Boolean consumed;
    if ((mOnKeyListener != NULL) &&
        (mOnKeyListener->OnKey(THIS_PROBE(IDialogInterface),
            keyCode, event, &consumed), consumed)) {
        return TRUE;
    }

    Boolean res;
    mWindow->SuperDispatchKeyEvent(event, &res);
    if (res) {
        return TRUE;
    }

    AutoPtr<IDispatcherState> state;
    if (mDecor != NULL) {
        mDecor->GetKeyDispatcherState((IDispatcherState**)&state);
    }

    event->Dispatch(THIS_PROBE(IKeyEventCallback),
        state, this->Probe(EIID_IInterface), &res);

    return res;
}

/**
 * Called to process a key shortcut event.
 * You can override this to intercept all key shortcut events before they are
 * dispatched to the window.  Be sure to call this implementation for key shortcut
 * events that should be handled normally.
 *
 * @param event The key shortcut event.
 * @return True if this event was consumed.
 */
Boolean Dialog::DispatchKeyShortcutEvent(
    /* [in] */ IKeyEvent* event)
{
    Boolean result;
    mWindow->SuperDispatchKeyShortcutEvent(event, &result);
    if (result) {
        return TRUE;
    }
    Int32 keycode;
    event->GetKeyCode(&keycode);
    return OnKeyShortcut(keycode, event);
}

/**
 * Called to process touch screen events.  You can override this to
 * intercept all touch screen events before they are dispatched to the
 * window.  Be sure to call this implementation for touch screen events
 * that should be handled normally.
 *
 * @param ev The touch screen event.
 *
 * @return Boolean Return TRUE if this event was consumed.
 */
Boolean Dialog::DispatchTouchEvent(
    /* [in] */ IMotionEvent* ev)
{
    Boolean res;
    mWindow->SuperDispatchTouchEvent(ev, &res);
    if (res) {
        return TRUE;
    }

    return OnTouchEvent(ev);
}

/**
 * Called to process trackball events.  You can override this to
 * intercept all trackball events before they are dispatched to the
 * window.  Be sure to call this implementation for trackball events
 * that should be handled normally.
 *
 * @param ev The trackball event.
 *
 * @return Boolean Return TRUE if this event was consumed.
 */
Boolean Dialog::DispatchTrackballEvent(
    /* [in] */ IMotionEvent* ev)
{
    Boolean res;
    mWindow->SuperDispatchTrackballEvent(ev, &res);
    if (res) {
        return TRUE;
    }

    return OnTrackballEvent(ev);
}

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
Boolean Dialog::DispatchGenericMotionEvent(
    /* [in] */ IMotionEvent* ev)
{
    Boolean result;
    mWindow->SuperDispatchGenericMotionEvent(ev, &result);
    if (result) {
        return TRUE;
    }

    return OnGenericMotionEvent(ev);
}

Boolean Dialog::DispatchPopulateAccessibilityEvent(
    /* [in] */ IAccessibilityEvent* event)
{
    String packageName;
    mContext->GetPackageName(&packageName);
    AutoPtr<ICharSequence> pnCsq;
    CString::New(packageName, (ICharSequence**)&pnCsq);
//TODO    event.setClassName(getClass().getName());
    event->SetPackageName(pnCsq);

    AutoPtr<IWindowManagerLayoutParams> params;
    mWindow->GetAttributes((IWindowManagerLayoutParams**)&params);
    Int32 width, height;
    params->GetWidth(&width);
    params->GetHeight(&height);
    Boolean isFullScreen = (width == IViewGroupLayoutParams::MATCH_PARENT) &&
        (height == IViewGroupLayoutParams::MATCH_PARENT);
    event->SetFullScreen(isFullScreen);

    return FALSE;
}

/**
 * @see Activity#onCreatePanelView(Int32)
 */
AutoPtr<IView> Dialog::OnCreatePanelView(
    /* [in] */ Int32 featureId)
{
    return NULL;
}

/**
 * @see Activity#onCreatePanelMenu(Int32, Menu)
 */
Boolean Dialog::OnCreatePanelMenu(
    /* [in] */ Int32 featureId,
    /* [in] */ IMenu* menu)
{
    if (featureId == IWindow::FEATURE_OPTIONS_PANEL) {
        return OnCreateOptionsMenu(menu);
    }

    return FALSE;
}

/**
 * @see Activity#onPreparePanel(Int32, View, Menu)
 */
Boolean Dialog::OnPreparePanel(
    /* [in] */ Int32 featureId,
    /* [in] */ IView* view,
    /* [in] */ IMenu* menu)
{
    if (featureId == IWindow::FEATURE_OPTIONS_PANEL && menu != NULL) {
        Boolean goforit = OnPrepareOptionsMenu(menu);
        Boolean hasVisibleItems;
        return goforit && (menu->HasVisibleItems(&hasVisibleItems),
            hasVisibleItems);
    }
    return TRUE;
}

/**
 * @see Activity#onMenuOpened(Int32, Menu)
 */
Boolean Dialog::OnMenuOpened(
    /* [in] */ Int32 featureId,
    /* [in] */ IMenu* menu)
{
    if (featureId == IWindow::FEATURE_ACTION_BAR) {
        mActionBar->DispatchMenuVisibilityChanged(TRUE);
    }
    return TRUE;
}

/**
 * @see Activity#onMenuItemSelected(Int32, MenuItem)
 */
Boolean Dialog::OnMenuItemSelected(
    /* [in] */ Int32 featureId,
    /* [in] */ IMenuItem* item)
{
    return FALSE;
}

/**
 * @see Activity#onPanelClosed(Int32, Menu)
 */
ECode Dialog::OnPanelClosed(
    /* [in] */ Int32 featureId,
    /* [in] */ IMenu* menu)
{
    if (featureId == IWindow::FEATURE_ACTION_BAR) {
        mActionBar->DispatchMenuVisibilityChanged(FALSE);
    }
    return NOERROR;
}

/**
 * It is usually safe to proxy this call to the owner activity's
 * {@link Activity#onCreateOptionsMenu(Menu)} if the client desires the same
 * menu for this Dialog.
 *
 * @see Activity#onCreateOptionsMenu(Menu)
 * @see #getOwnerActivity()
 */
Boolean Dialog::OnCreateOptionsMenu(
    /* [in] */ IMenu* menu)
{
    return TRUE;
}

/**
 * It is usually safe to proxy this call to the owner activity's
 * {@link Activity#onPrepareOptionsMenu(Menu)} if the client desires the
 * same menu for this Dialog.
 *
 * @see Activity#onPrepareOptionsMenu(Menu)
 * @see #getOwnerActivity()
 */
Boolean Dialog::OnPrepareOptionsMenu(
    /* [in] */ IMenu* menu)
{
    return TRUE;
}

/**
 * @see Activity#onOptionsItemSelected(MenuItem)
 */
Boolean Dialog::OnOptionsItemSelected(
    /* [in] */ IMenuItem* item)
{
    return FALSE;
}

/**
 * @see Activity#onOptionsMenuClosed(Menu)
 */
ECode Dialog::OnOptionsMenuClosed(
    /* [in] */ IMenu* menu)
{
    return NOERROR;
}

/**
 * @see Activity#openOptionsMenu()
 */
ECode Dialog::OpenOptionsMenu()
{
    return mWindow->OpenPanel(IWindow::FEATURE_OPTIONS_PANEL, NULL);
}

/**
 * @see Activity#closeOptionsMenu()
 */
ECode Dialog::CloseOptionsMenu()
{
    return mWindow->ClosePanel(IWindow::FEATURE_OPTIONS_PANEL);
}

/**
 * @see Activity#invalidateOptionsMenu()
 */
ECode Dialog::InvalidateOptionsMenu()
{
    return mWindow->InvalidatePanelMenu(IWindow::FEATURE_OPTIONS_PANEL);
}

/**
 * @see Activity#onCreateContextMenu(ContextMenu, View, ContextMenuInfo)
 */
ECode Dialog::OnCreateContextMenu(
    /* [in] */ IContextMenu* menu,
    /* [in] */ IView* v,
    /* [in] */ IContextMenuInfo* menuInfo)
{
    return NOERROR;
}

/**
 * @see Activity#registerForContextMenu(View)
 */
ECode Dialog::RegisterForContextMenu(
    /* [in] */ IView* view)
{
    return view->SetOnCreateContextMenuListener(
        (IViewOnCreateContextMenuListener*)this->Probe(
        EIID_IViewOnCreateContextMenuListener));
}

/**
 * @see Activity#unregisterForContextMenu(View)
 */
ECode Dialog::UnregisterForContextMenu(
    /* [in] */ IView* view)
{
    return view->SetOnCreateContextMenuListener(NULL);
}

/**
 * @see Activity#openContextMenu(View)
 */
ECode Dialog::OpenContextMenu(
    /* [in] */ IView* view)
{
    Boolean res;
    return view->ShowContextMenu(&res);
}

/**
 * @see Activity#onContextItemSelected(MenuItem)
 */
Boolean Dialog::OnContextItemSelected(
    /* [in] */ IMenuItem* item)
{
    return FALSE;
}

/**
 * @see Activity#onContextMenuClosed(Menu)
 */
ECode Dialog::OnContextMenuClosed(
    /* [in] */ IMenu* menu)
{
    return NOERROR;
}

/**
 * This hook is called when the user signals the desire to start a search.
 */
Boolean Dialog::OnSearchRequested()
{
    assert(0 && "TODO");
//    final SearchManager searchManager = (SearchManager) mContext
//            .getSystemService(Context_SEARCH_SERVICE);
//
//    // associate search with owner activity
//    final ComponentName appName = getAssociatedActivity();
//    if (appName != NULL && searchManager->GetSearchableInfo(appName) != NULL) {
//        searchManager.startSearch(NULL, FALSE, appName, NULL, FALSE);
//        dismiss();
//        return TRUE;
//    } else {
//        return FALSE;
//    }
    return FALSE;
}

AutoPtr<IActionMode> Dialog::OnWindowStartingActionMode(
    /* [in] */ IActionModeCallback* callback)
{
    if (mActionBar != NULL) {
        AutoPtr<IActionMode> tmp;
        mActionBar->StartActionMode(callback, (IActionMode**)&tmp);
        return tmp.Get();
    }
    return NULL;
}

/**
 * {@inheritDoc}
 *
 * Note that if you override this method you should always call through
 * to the superclass implementation by calling super.onActionModeStarted(mode).
 */
ECode Dialog::OnActionModeStarted(
    /* [in] */ IActionMode* mode)
{
    mActionMode = mode;
    return NOERROR;
}

/**
 * {@inheritDoc}
 *
 * Note that if you override this method you should always call through
 * to the superclass implementation by calling super.onActionModeFinished(mode).
 */
ECode Dialog::OnActionModeFinished(
    /* [in] */ IActionMode* mode)
{
    if (mode == mActionMode) {
        mActionMode = NULL;
    }
    return NOERROR;
}

/**
 * @return The activity associated with this dialog, or NULL if there is no assocaited activity.
 */
AutoPtr<IComponentName> Dialog::GetAssociatedActivity()
{
    AutoPtr<IActivity> activity = GetOwnerActivity();
    AutoPtr<IContext> context = GetContext();
    while (activity == NULL && context != NULL) {
        if (IActivity::Probe(context)) {
            activity = IActivity::Probe(context);  // found it!
        }
        else {
            if (IContextWrapper::Probe(context)) {// unwrap one level
                AutoPtr<IContext> tempCtx;
                IContextWrapper::Probe(context)->GetBaseContext(
                    (IContext**)&tempCtx);
                context = tempCtx;
            }
            else {// done
                context = NULL;
            }
        }
    }
    AutoPtr<IComponentName> name;
    if (activity != NULL) {
        activity->GetComponentName((IComponentName**)&name);
    }
    return name;
}

/**
 * Request that key events come to this dialog. Use this if your
 * dialog has no views with focus, but the dialog still wants
 * a chance to process key events.
 *
 * @param get TRUE if the dialog should receive key events, FALSE otherwise
 * @see android.view.Window#takeKeyEvents
 */
ECode Dialog::TakeKeyEvents(
    /* [in] */ Boolean get)
{
    return mWindow->TakeKeyEvents(get);
}

/**
 * Enable extended window features.  This is a convenience for calling
 * {@link android.view.Window#requestFeature GetWindow().requestFeature()}.
 *
 * @param featureId The desired feature as defined in
 *                  {@link android.view.Window}.
 * @return Returns TRUE if the requested feature is supported and now
 *         enabled.
 *
 * @see android.view.Window#requestFeature
 */
Boolean Dialog::RequestWindowFeature(
    /* [in] */ Int32 featureId)
{
    Boolean res;
    GetWindow()->RequestFeature(featureId, &res);

    return res;
}

/**
 * Convenience for calling
 * {@link android.view.Window#setFeatureDrawableResource}.
 */
ECode Dialog::SetFeatureDrawableResource(
    /* [in] */ Int32 featureId,
    /* [in] */ Int32 resId)
{
    return GetWindow()->SetFeatureDrawableResource(featureId, resId);
}

/**
 * Convenience for calling
 * {@link android.view.Window#setFeatureDrawableUri}.
 */
ECode Dialog::SetFeatureDrawableUri(
    /* [in] */ Int32 featureId,
    /* [in] */ IUri* uri)
{
    return GetWindow()->SetFeatureDrawableUri(featureId, uri);
}

/**
 * Convenience for calling
 * {@link android.view.Window#setFeatureDrawable(Int32, Drawable)}.
 */
ECode Dialog::SetFeatureDrawable(
    /* [in] */ Int32 featureId,
    /* [in] */ IDrawable* drawable)
{
    return GetWindow()->SetFeatureDrawable(featureId, drawable);
}

/**
 * Convenience for calling
 * {@link android.view.Window#setFeatureDrawableAlpha}.
 */
ECode Dialog::SetFeatureDrawableAlpha(
    /* [in] */ Int32 featureId,
    /* [in] */ Int32 alpha)
{
    return GetWindow()->SetFeatureDrawableAlpha(featureId, alpha);
}

AutoPtr<ILayoutInflater> Dialog::GetLayoutInflater()
{
    AutoPtr<ILayoutInflater> layout;
    GetWindow()->GetLayoutInflater((ILayoutInflater**)&layout);

    return layout;
}

/**
 * Sets whether this dialog is cancelable with the
 * {@link KeyEvent#KEYCODE_BACK BACK} key.
 */
ECode Dialog::SetCancelable(
    /* [in] */ Boolean flag)
{
    mCancelable = flag;

    return NOERROR;
}

/**
 * Sets whether this dialog is canceled when touched outside the window's
 * bounds. If setting to TRUE, the dialog is set to be cancelable if not
 * already set.
 *
 * @param cancel Whether the dialog should be canceled when touched outside
 *            the window.
 */
ECode Dialog::SetCanceledOnTouchOutside(
    /* [in] */ Boolean cancel)
{
    if (cancel && !mCancelable) {
        mCancelable = TRUE;
    }

    mWindow->SetCloseOnTouchOutside(cancel);

    return NOERROR;
}

/**
 * Cancel the dialog.  This is essentially the same as calling {@link #dismiss()}, but it will
 * also call your {@link DialogInterface.OnCancelListener} (if registered).
 */
ECode Dialog::Cancel()
{
    if (!mCanceled && mCancelMessage != NULL) {
        mCanceled = TRUE;

        AutoPtr<IMessageHelper> helper;
        CMessageHelper::AcquireSingleton((IMessageHelper**)&helper);
        AutoPtr<IMessage> msg;
        helper->Obtain(mCancelMessage, (IMessage**)&msg);
        // Obtain a new message so this dialog can be re-used
        msg->SendToTarget();

    }

    Dismiss();

    return NOERROR;
}

/**
 * Set a listener to be invoked when the dialog is canceled.
 *
 * <p>This will only be invoked when the dialog is canceled.
 * Cancel events alone will not capture all ways that
 * the dialog might be dismissed. If the creator needs
 * to know when a dialog is dismissed in general, use
 * {@link #setOnDismissListener}.</p>
 *
 * @param listener The {@link DialogInterface.OnCancelListener} to use.
 */
ECode Dialog::SetOnCancelListener(
    /* [in] */ IDialogInterfaceOnCancelListener* listener)
{
    if (mCancelAndDismissTaken != NULL) {
        return E_ILLEGAL_STATE_EXCEPTION;
        // throw new IllegalStateException(
        //        "OnCancelListener is already taken by "
        //        + mCancelAndDismissTaken + " and can not be replaced.");
    }

    mCancelMessage = NULL;
    if (listener != NULL) {
        mListenersHandler->ObtainMessage(CANCEL, listener, (IMessage**)&mCancelMessage);
    }
    return NOERROR;
}

/**
 * Set a message to be sent when the dialog is canceled.
 * @param msg The msg to send when the dialog is canceled.
 * @see #SetOnCancelListener(android.content.DialogInterface.OnCancelListener)
 */
ECode Dialog::SetCancelMessage(
    /* [in] */ IMessage* msg)
{
   mCancelMessage = msg;
   return NOERROR;
}

/**
 * Set a listener to be invoked when the dialog is dismissed.
 * @param listener The {@link DialogInterface.OnDismissListener} to use.
 */
ECode Dialog::SetOnDismissListener(
    /* [in] */ IDialogInterfaceOnDismissListener* listener)
{
    if (mCancelAndDismissTaken != NULL) {
        return E_ILLEGAL_STATE_EXCEPTION;
        // throw new IllegalStateException(
        //        "OnDismissListener is already taken by "
        //        + mCancelAndDismissTaken + " and can not be replaced.");
    }

    mDismissMessage = NULL;
    if (listener != NULL) {
        mListenersHandler->ObtainMessage(DISMISS, listener, (IMessage**)&mDismissMessage);
    }
    return NOERROR;
}

/**
 * Sets a listener to be invoked when the dialog is shown.
 * @param listener The {@link DialogInterface.OnShowListener} to use.
 */
ECode Dialog::SetOnShowListener(
    /* [in] */ IDialogInterfaceOnShowListener* listener)
{
    mShowMessage = NULL;
    if (listener != NULL) {
        mListenersHandler->ObtainMessage(SHOW, listener, (IMessage**)&mShowMessage);
    }

    return NOERROR;
}

/**
 * Set a message to be sent when the dialog is dismissed.
 * @param msg The msg to send when the dialog is dismissed.
 */
ECode Dialog::SetDismissMessage(
    /* [in] */ IMessage* msg)
{
   mDismissMessage = msg;
   return NOERROR;
}

 /** @hide */
Boolean Dialog::TakeCancelAndDismissListeners(
    /* [in] */ const String& msg,
    /* [in] */ IDialogInterfaceOnCancelListener* cancel,
    /* [in] */ IDialogInterfaceOnDismissListener* dismiss)
{
    if (mCancelAndDismissTaken != NULL) {
        mCancelAndDismissTaken = NULL;
    }
    else if (mCancelMessage != NULL || mDismissMessage != NULL) {
        return FALSE;
    }

    SetOnCancelListener(cancel);
    SetOnDismissListener(dismiss);
    mCancelAndDismissTaken = msg;

    return TRUE;
}

/**
 * By default, this will use the owner Activity's suggested stream type.
 *
 * @see Activity#setVolumeControlStream(Int32)
 * @see #setOwnerActivity(Activity)
 */
ECode Dialog::SetVolumeControlStream(
    /* [in] */ Int32 streamType)
{
    return GetWindow()->SetVolumeControlStream(streamType);
}

/**
 * @see Activity#getVolumeControlStream()
 */
Int32 Dialog::GetVolumeControlStream()
{
    Int32 volume;
    GetWindow()->GetVolumeControlStream(&volume);

    return volume;
}

/**
 * Sets the callback that will be called if a key is dispatched to the dialog.
 */
ECode Dialog::SetOnKeyListener(
    /* [in] */ IDialogInterfaceOnKeyListener* onKeyListener)
{
    mOnKeyListener = onKeyListener;

    return NOERROR;
}

} // namespace App
} // namespace Droid
} // namespace Elastos
