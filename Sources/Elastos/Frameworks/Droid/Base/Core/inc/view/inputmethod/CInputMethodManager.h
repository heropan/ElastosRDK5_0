
#ifndef  __CLOCALINPUTMETHODMANAGER_H__
#define  __CLOCALINPUTMETHODMANAGER_H__

#include "ext/frameworkdef.h"
#include "_CInputMethodManager.h"
#include "view/inputmethod/CControlledInputConnectionWrapper.h"
#include "os/HandlerBase.h"
#include "os/Runnable.h"
#include <elastos/List.h>

using Elastos::Utility::IObjectMap;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Graphics::IRect;
using Elastos::Droid::Os::HandlerBase;
using Elastos::Droid::Os::Runnable;
using Elastos::Droid::Os::IBinder;
using Elastos::Droid::Os::IBundle;
using Elastos::Droid::Os::IResultReceiver;
using Elastos::Droid::Text::Style::ISuggestionSpan;
using Elastos::Droid::Internal::View::IInputBindResult;
using Elastos::Droid::Internal::View::IInputContext;
using Elastos::Droid::Internal::View::IInputMethodClient;
using Elastos::Droid::Internal::View::IIInputMethodManager;
using Elastos::Droid::Internal::View::IIInputMethodSession;

namespace Elastos {
namespace Droid {
namespace View {
namespace InputMethod {

CarClass(CInputMethodManager)
{
private:
    class MyHandler : public HandlerBase
    {
    public:
        MyHandler(
            /* [in] */ ILooper* looper,
            /* [in] */ CInputMethodManager* host)
            : HandlerBase(looper)
            , mHost(host)
        {}

        CARAPI HandleMessage(
            /* [in] */ IMessage* msg);
    private:
        CInputMethodManager* mHost;
    };

    class StartInputRunnable
        : public Runnable
    {
    public:
        StartInputRunnable(
            /* [in] */ CInputMethodManager* service)
            : mHost(service)
        {}

        CARAPI Run();
    private:
        CInputMethodManager* mHost;
    };

    class PendingEvent
        : public ElRefBase
        , public IInterface
    {
    public:
        CAR_INTERFACE_DECL()

        PendingEvent();

    public:
        AutoPtr<PendingEvent> mNext;

        Int64 mStartTime;
        Int32 mSeq;
        String mInputMethodId;
        AutoPtr<IInputMethodManagerFinishedEventCallback> mCallback;
    };

public:
    CInputMethodManager();

    //TODO: will delete. Because LocalInputMethodManager is private in java code.
    CARAPI constructor(
        /* [in] */ IIInputMethodManager* service,
        /* [in] */ ILooper* looper);

    /**
     * Retrieve the global InputMethodManager instance, creating it if it
     * doesn't already exist.
     * @hide
     */
    static CARAPI_(AutoPtr<IInputMethodManager>) GetInstance(
        /* [in] */ IContext* context);

    /**
     * Internally, the input method manager can't be context-dependent, so
     * we have this here for the places that need it.
     * @hide
     */
    static CARAPI_(AutoPtr<IInputMethodManager>) GetInstance(
        /* [in] */ ILooper* mainLooper);

    /**
     * Private optimization: retrieve the global InputMethodManager instance,
     * if it exists.
     * @hide
     */
    static CARAPI_(AutoPtr<IInputMethodManager>) PeekInstance();

    /** @hide */
    CARAPI GetClient(
        /* [out] */ IInputMethodClient** client);

    /** @hide */
    CARAPI GetInputContext(
        /* [out] */ IInputContext** context);

    CARAPI GetInputMethodList(
        /* [out] */ IObjectContainer** infos);

    CARAPI GetEnabledInputMethodList(
        /* [out] */ IObjectContainer** list);

    CARAPI GetEnabledInputMethodSubtypeList(
        /* [in] */ IInputMethodInfo* imi,
        /* [in] */ Boolean allowsImplicitlySelectedSubtypes,
        /* [out] */ IObjectContainer** infos);

    CARAPI ShowStatusIcon(
        /* [in] */ IBinder* imeToken,
        /* [in] */ const String& packageName,
        /* [in] */ Int32 iconId);

    CARAPI HideStatusIcon(
        /* [in] */ IBinder* imeToken);

    CARAPI SetImeWindowStatus(
        /* [in] */ IBinder* imeToken,
        /* [in] */ Int32 vis,
        /* [in] */ Int32 backDisposition);

    /** @hide */
    CARAPI SetFullscreenMode(
        /* [in] */ Boolean fullScreen);

    /** @hide */
    CARAPI RegisterSuggestionSpansForNotification(
        /* [in] */ ArrayOf<ISuggestionSpan*>* spans);

    /** @hide */
    CARAPI NotifySuggestionPicked(
        /* [in] */ ISuggestionSpan* span,
        /* [in] */ const String& originalString,
        /* [in] */ Int32 index);

    /**
     * Allows you to discover whether the attached input method is running
     * in fullscreen mode.  Return true if it is fullscreen, entirely covering
     * your UI, else returns false.
     */
    CARAPI IsFullscreenMode(
        /* [out] */ Boolean* isfull);

    /**
     * Return true if the given view is the currently active view for the
     * input method.
     */
    CARAPI IsActive(
        /* [in] */ IView* view,
        /* [out] */ Boolean* active);

    /**
     * Return true if any view is currently active in the input method.
     */
    CARAPI IsActiveEx(
        /* [out] */ Boolean* active);

    /**
     * Return true if the currently served view is accepting full text edits.
     * If false, it has no input connection, so can only handle raw key events.
     */
    CARAPI IsAcceptingText(
        /* [out] */ Boolean* isAccepting);

    /**
     * Reset all of the state associated with being bound to an input method.
     */
    /* package */ CARAPI_(void) ClearBindingLocked();

    /**
     * Reset all of the state associated with a served view being connected
     * to an input method
     */
    /* package */ CARAPI_(void) ClearConnectionLocked();

    /**
     * Disconnect any existing input connection, clearing the served view.
     */
    CARAPI_(void) FinishInputLocked();

    /**
     * Called from the FINISH_INPUT_CONNECTION message above.
     * @hide
     */
    CARAPI ReportFinishInputConnection(
        /* [in] */ IInputConnection* ic);

    CARAPI DisplayCompletions(
        /* [in] */ IView* view,
        /* [in] */ ArrayOf<ICompletionInfo*>* completions);

    CARAPI UpdateExtractedText(
        /* [in] */ IView* view,
        /* [in] */ Int32 token,
        /* [in] */ IExtractedText* text);

    /**
     * Synonym for {@link #showSoftInput(View, Int32, ResultReceiver)} without
     * a result receiver: explicitly request that the current input method's
     * soft input area be shown to the user, if needed.
     *
     * @param view The currently focused view, which would like to receive
     * soft keyboard input.
     * @param flags Provides additional operating flags.  Currently may be
     * 0 or have the {@link #SHOW_IMPLICIT} bit set.
     */
    CARAPI ShowSoftInput(
        /* [in] */ IView* view,
        /* [in] */ Int32 flags,
        /* [out] */ Boolean* show);

    /**
     * Explicitly request that the current input method's soft input area be
     * shown to the user, if needed.  Call this if the user interacts with
     * your view in such a way that they have expressed they would like to
     * start performing input into it.
     *
     * @param view The currently focused view, which would like to receive
     * soft keyboard input.
     * @param flags Provides additional operating flags.  Currently may be
     * 0 or have the {@link #SHOW_IMPLICIT} bit set.
     * @param resultReceiver If non-NULL, this will be called by the IME when
     * it has processed your request to tell you what it has done.  The result
     * code you receive may be either {@link #RESULT_UNCHANGED_SHOWN},
     * {@link #RESULT_UNCHANGED_HIDDEN}, {@link #RESULT_SHOWN}, or
     * {@link #RESULT_HIDDEN}.
     */
    CARAPI ShowSoftInputEx(
        /* [in] */ IView* view,
        /* [in] */ Int32 flags,
        /* [in] */ IResultReceiver* resultReceiver,
        /* [out] */ Boolean* show);

    /** @hide */
    CARAPI ShowSoftInputUnchecked(
        /* [in] */ Int32 flags,
        /* [in] */ IResultReceiver* resultReceiver);

    /**
     * Synonym for {@link #hideSoftInputFromWindow(IBinder, Int32, ResultReceiver)
     * without a result: request to hide the soft input window from the
     * context of the window that is currently accepting input.
     *
     * @param windowToken The token of the window that is making the request,
     * as returned by {@link View#getWindowToken() View.getWindowToken()}.
     * @param flags Provides additional operating flags.  Currently may be
     * 0 or have the {@link #HIDE_IMPLICIT_ONLY} bit set.
     */
    CARAPI HideSoftInputFromWindow(
        /* [in] */ IBinder* windowToken,
        /* [in] */ Int32 flags,
        /* [out] */ Boolean* hide);

    /**
     * Request to hide the soft input window from the context of the window
     * that is currently accepting input.  This should be called as a result
     * of the user doing some actually than fairly explicitly requests to
     * have the input window hidden.
     *
     * @param windowToken The token of the window that is making the request,
     * as returned by {@link View#getWindowToken() View.getWindowToken()}.
     * @param flags Provides additional operating flags.  Currently may be
     * 0 or have the {@link #HIDE_IMPLICIT_ONLY} bit set.
     * @param resultReceiver If non-NULL, this will be called by the IME when
     * it has processed your request to tell you what it has done.  The result
     * code you receive may be either {@link #RESULT_UNCHANGED_SHOWN},
     * {@link #RESULT_UNCHANGED_HIDDEN}, {@link #RESULT_SHOWN}, or
     * {@link #RESULT_HIDDEN}.
     */
    CARAPI HideSoftInputFromWindowEx(
        /* [in] */ IBinder* windowToken,
        /* [in] */ Int32 flags,
        /* [in] */ IResultReceiver* resultReceiver,
        /* [out] */ Boolean* hide);

    /**
     * This method toggles the input method window display.
     * If the input window is already displayed, it gets hidden.
     * If not the input window will be displayed.
     * @param windowToken The token of the window that is making the request,
     * as returned by {@link View#getWindowToken() View.getWindowToken()}.
     * @param showFlags Provides additional operating flags.  May be
     * 0 or have the {@link #SHOW_IMPLICIT},
     * {@link #SHOW_FORCED} bit set.
     * @param hideFlags Provides additional operating flags.  May be
     * 0 or have the {@link #HIDE_IMPLICIT_ONLY},
     * {@link #HIDE_NOT_ALWAYS} bit set.
     **/
    CARAPI ToggleSoftInputFromWindow(
        /* [in] */ IBinder* windowToken,
        /* [in] */ Int32 showFlags,
        /* [in] */ Int32 hideFlags);

    /*
     * This method toggles the input method window display.
     * If the input window is already displayed, it gets hidden.
     * If not the input window will be displayed.
     * @param showFlags Provides additional operating flags.  May be
     * 0 or have the {@link #SHOW_IMPLICIT},
     * {@link #SHOW_FORCED} bit set.
     * @param hideFlags Provides additional operating flags.  May be
     * 0 or have the {@link #HIDE_IMPLICIT_ONLY},
     * {@link #HIDE_NOT_ALWAYS} bit set.
     * @hide
     */
    CARAPI ToggleSoftInput(
        /* [in] */ Int32 showFlags,
        /* [in] */ Int32 hideFlags);

    /**
     * If the input method is currently connected to the given view,
     * restart it with its new contents.  You should call this when the text
     * within your view changes outside of the normal input method or key
     * input flow, such as when an application calls TextView.setText().
     *
     * @param view The view whose text has changed.
     */
    CARAPI RestartInput(
        /* [in] */ IView* view);

    CARAPI_(Boolean) StartInputInner(
        /* [in] */ IBinder* windowGainingFocus,
        /* [in] */ Int32 controlFlags,
        /* [in] */ Int32 softInputMode,
        /* [in] */ Int32 windowFlags);

    /**
     * When the focused window is dismissed, this method is called to finish the
     * input method started before.
     * @hide
     */
    CARAPI WindowDismissed(
        /* [in] */ IBinder* appWindowToken);

    /**
     * Call this when a view receives focus.
     * @hide
     */
    CARAPI FocusIn(
        /* [in] */ IView* view);

    CARAPI_(void) FocusInLocked(
        /* [in] */ IView* view);

    /**
     * Call this when a view loses focus.
     * @hide
     */
    CARAPI FocusOut(
        /* [in] */ IView* view);

    static CARAPI_(void) ScheduleCheckFocusLocked(
        /* [in] */ IView* view);

    /**
     * @hide
     */
    CARAPI CheckFocus();

    CARAPI_(void) CloseCurrentInput();

    /**
     * Called by ViewRoot when its window gets input focus.
     * @hide
     */
    CARAPI OnWindowFocus(
        /* [in] */ IView* rootView,
        /* [in] */ IView* focusedView,
        /* [in] */ Int32 softInputMode,
        /* [in] */ Boolean first,
        /* [in] */ Int32 windowFlags);

    /** @hide */
    CARAPI StartGettingWindowFocus(
        /* [in] */ IView* rootView);

    /**
     * Report the current selection range.
     */
    CARAPI UpdateSelection(
        /* [in] */ IView* view,
        /* [in] */ Int32 selStart,
        /* [in] */ Int32 selEnd,
        /* [in] */ Int32 candidatesStart,
        /* [in] */ Int32 candidatesEnd);

    CARAPI ViewClicked(
        /* [in] */ IView* view);

    /**
     * Returns true if the current input method wants to watch the location
     * of the input editor's cursor in its window.
     */
    CARAPI IsWatchingCursor(
        /* [in] */ IView* view,
        /* [out] */ Boolean* isWatching);

    /**
     * Report the current cursor location in its window.
     */
    CARAPI UpdateCursor(
        /* [in] */ IView* view,
        /* [in] */ Int32 left,
        /* [in] */ Int32 top,
        /* [in] */ Int32 right,
        /* [in] */ Int32 bottom);

    /**
     * Call {@link InputMethodSession#appPrivateCommand(String, Bundle)
     * InputMethodSession.appPrivateCommand()} on the current Input Method.
     * @param view Optional View that is sending the command, or NULL if
     * you want to send the command regardless of the view that is attached
     * to the input method.
     * @param action Name of the command to be performed.  This <em>must</em>
     * be a scoped name, i.e. prefixed with a package name you own, so that
     * different developers will not create conflicting commands.
     * @param data Any data to include with the command.
     */
    CARAPI SendAppPrivateCommand(
        /* [in] */ IView* view,
        /* [in] */ const String& action,
        /* [in] */ IBundle* data);

    /**
     * Force switch to a new input method component.  This can only be called
     * from the currently active input method, as validated by the given token.
     * @param token Supplies the identifying token given to an input method
     * when it was started, which allows it to perform this operation on
     * itself.
     * @param id The unique identifier for the new input method to be switched to.
     */
    CARAPI SetInputMethod(
        /* [in] */ IBinder* token,
        /* [in] */ const String& id);

    CARAPI SetInputMethodAndSubtype(
        /* [in] */ IBinder* token,
        /* [in] */ const String& id,
        /* [in] */ IInputMethodSubtype* subtype);

    /**
     * Close/hide the input method's soft input area, so the user no longer
     * sees it or can interact with it.  This can only be called
     * from the currently active input method, as validated by the given token.
     *
     * @param token Supplies the identifying token given to an input method
     * when it was started, which allows it to perform this operation on
     * itself.
     * @param flags Provides additional operating flags.  Currently may be
     * 0 or have the {@link #HIDE_IMPLICIT_ONLY},
     * {@link #HIDE_NOT_ALWAYS} bit set.
     */
    CARAPI HideSoftInputFromInputMethod(
        /* [in] */ IBinder* token,
        /* [in] */ Int32 flags);

    /**
     * Show the input method's soft input area, so the user
     * sees the input method window and can interact with it.
     * This can only be called from the currently active input method,
     * as validated by the given token.
     *
     * @param token Supplies the identifying token given to an input method
     * when it was started, which allows it to perform this operation on
     * itself.
     * @param flags Provides additional operating flags.  Currently may be
     * 0 or have the {@link #SHOW_IMPLICIT} or
     * {@link #SHOW_FORCED} bit set.
     */
    CARAPI ShowSoftInputFromInputMethod(
        /* [in] */ IBinder* token,
        /* [in] */ Int32 flags);

    /**
     * @hide
     */
    CARAPI DispatchKeyEvent(
        /* [in] */ IContext* context,
        /* [in] */ Int32 seq,
        /* [in] */ IKeyEvent* key,
        /* [in] */ IInputMethodManagerFinishedEventCallback* callback);

    CARAPI DispatchTrackballEvent(
        /* [in] */ IContext* context,
        /* [in] */ Int32 seq,
        /* [in] */ IMotionEvent* motion,
        /* [in] */ IInputMethodManagerFinishedEventCallback* callback);

    /**
     * @hide
     */
    CARAPI DispatchGenericMotionEvent(
        /* [in] */ IContext* context,
        /* [in] */ Int32 seq,
        /* [in] */ IMotionEvent* motion,
        /* [in] */ IInputMethodManagerFinishedEventCallback* callback);

    CARAPI_(void) FinishedEvent(
        /* [in] */ Int32 seq,
        /* [in] */ Boolean handled);

    CARAPI_(void) TimeoutEvent(
        /* [in] */ Int32 seq);

    CARAPI ShowInputMethodPicker();

    /**
     * Show the settings for enabling subtypes of the specified input method.
     * @param imiId An input method, whose subtypes settings will be shown. If imiId is null,
     * subtypes of all input methods will be shown.
     */
    CARAPI ShowInputMethodAndSubtypeEnabler(
        /* [in] */ const String& imiId);

    /**
     * Returns the current input method subtype. This subtype is one of the subtypes in
     * the current input method. This method returns null when the current input method doesn't
     * have any input method subtype.
     */
    CARAPI GetCurrentInputMethodSubtype(
        /* [out] */ IInputMethodSubtype** subtype);

    /**
     * Switch to a new input method subtype of the current input method.
     * @param subtype A new input method subtype to switch.
     * @return true if the current subtype was successfully switched. When the specified subtype is
     * null, this method returns false.
     */
    CARAPI SetCurrentInputMethodSubtype(
        /* [in] */ IInputMethodSubtype* subtype,
        /* [out] */ Boolean* switched);

    /**
     * Returns a map of all shortcut input method info and their subtypes.
     */
    CARAPI GetShortcutInputMethodsAndSubtypes(
        /* [out] */ IObjectMap** subtypes);

    /**
     * Force switch to the last used input method and subtype. If the last input method didn't have
     * any subtypes, the framework will simply switch to the last input method with no subtype
     * specified.
     * @param imeToken Supplies the identifying token given to an input method when it was started,
     * which allows it to perform this operation on itself.
     * @return true if the current input method and subtype was successfully switched to the last
     * used input method and subtype.
     */
    CARAPI SwitchToLastInputMethod(
        /* [in] */ IBinder* imeToken,
        /* [out] */ Boolean* switched);

    /**
     * Force switch to the next input method and subtype. If there is no IME enabled except
     * current IME and subtype, do nothing.
     * @param imeToken Supplies the identifying token given to an input method when it was started,
     * which allows it to perform this operation on itself.
     * @param onlyCurrentIme if true, the framework will find the next subtype which
     * belongs to the current IME
     * @return true if the current input method and subtype was successfully switched to the next
     * input method and subtype.
     */
    CARAPI SwitchToNextInputMethod(
        /* [in] */ IBinder* imeToken,
        /* [in] */ Boolean onlyCurrentIme,
        /* [out] */ Boolean* switched);

    /**
     * Set additional input method subtypes. Only a process which shares the same uid with the IME
     * can add additional input method subtypes to the IME.
     * Please note that a subtype's status is stored in the system.
     * For example, enabled subtypes are remembered by the framework even after they are removed
     * by using this method. If you re-add the same subtypes again,
     * they will just get enabled. If you want to avoid such conflicts, for instance, you may
     * want to create a "different" new subtype even with the same locale and mode,
     * by changing its extra value. The different subtype won't get affected by the stored past
     * status. (You may want to take a look at {@link InputMethodSubtype#hashCode()} to refer
     * to the current implementation.)
     * @param imiId Id of InputMethodInfo which additional input method subtypes will be added to.
     * @param subtypes subtypes will be added as additional subtypes of the current input method.
     */
    CARAPI SetAdditionalInputMethodSubtypes(
        /* [in] */ const String& imiId,
        /* [in] */ ArrayOf<IInputMethodSubtype*>* subtypes);

    CARAPI GetLastInputMethodSubtype(
        /* [out] */ IInputMethodSubtype** subtype);

    //For callback
    CARAPI_(void) HandleBind(
        /* [in] */ IInputBindResult* res);

    CARAPI_(void) HandleUnBind(
        /* [in] */ Int32 sequence);

    CARAPI_(void) HandleSetActive(
        /* [in] */ Boolean active);

    CARAPI_(void) HandleEventTimeout(
        /* [in] */ Int32 seq);

private:
    /**
     * Notifies the served view that the current InputConnection will no longer be used.
     */
    CARAPI_(void) NotifyInputConnectionFinished();

    CARAPI_(Boolean) CheckFocusNoStartInput(
        /* [in] */ Boolean forceNewFocus,
        /* [in] */ Boolean finishComposingText);

    CARAPI_(void) EnqueuePendingEventLocked(
        /* [in] */ Int64 startTime,
        /* [in] */ Int32 seq,
        /* [in] */ const String& inputMethodId,
        /* [in] */ IInputMethodManagerFinishedEventCallback* callback);

    CARAPI_(AutoPtr<PendingEvent>) DequeuePendingEventLocked(
        /* [in] */ Int32 seq);

    CARAPI_(AutoPtr<PendingEvent>) ObtainPendingEventLocked(
        /* [in] */ Int64 startTime,
        /* [in] */ Int32 seq,
        /* [in] */ const String& inputMethodId,
        /* [in] */ IInputMethodManagerFinishedEventCallback* callback);

    CARAPI_(void) RecyclePendingEventLocked(
        /* [in] */ PendingEvent* p);

    CARAPI_(void) ShowInputMethodPickerLocked();

public:
    static const Boolean DEBUG;
    static const String TAG;

    static Mutex sInstanceSync;
    static AutoPtr<IInputMethodManager> sInstance;

    /**
     * Timeout in milliseconds for delivering a key to an IME.
     */
    static const Int64 INPUT_METHOD_NOT_RESPONDING_TIMEOUT = 2500;

    static const Int32 MSG_DUMP;// = 1;
    static const Int32 MSG_BIND;// = 2;
    static const Int32 MSG_UNBIND;// = 3;
    static const Int32 MSG_SET_ACTIVE;// = 4;
    static const Int32 MSG_EVENT_TIMEOUT;// = 5;

    AutoPtr<IIInputMethodManager> mService;
    AutoPtr<ILooper> mMainLooper;

    // For scheduling work on the main thread.  This also serves as our
    // global lock.
    AutoPtr<IHandler> mH;
    Mutex mHLock;

    // Our generic input connection if the current target does not have its own.
    AutoPtr<IInputContext> mIInputContext;

    /**
     * True if this input method client is active, initially false.
     */
    Boolean mActive;

    /**
     * Set whenever this client becomes inactive, to know we need to reset
     * state with the IME then next time we receive focus.
     */
    Boolean mHasBeenInactive;

    /**
     * As reported by IME through InputConnection.
     */
    Boolean mFullscreenMode;

    // -----------------------------------------------------------

    /**
     * This is the root view of the overall window that currently has input
     * method focus.
     */
    AutoPtr<IView> mCurRootView;
    /**
     * This is the view that should currently be served by an input method,
     * regardless of the state of setting that up.
     */
    AutoPtr<IView> mServedView;
    /**
     * This is then next view that will be served by the input method, when
     * we get around to updating things.
     */
    AutoPtr<IView> mNextServedView;
    /**
     * This is set when we are in the process of connecting, to determine
     * when we have actually finished.
     */
    Boolean mServedConnecting;
    /**
     * This is non-NULL when we have connected the served view; it holds
     * the attributes that were last retrieved from the served view and given
     * to the input connection.
     */
    AutoPtr<IEditorInfo> mCurrentTextBoxAttribute;
    /**
     * The InputConnection that was last retrieved from the served view.
     */
    AutoPtr<IInputConnection> mServedInputConnection;
    AutoPtr<CControlledInputConnectionWrapper> mServedInputConnectionWrapper;

    /**
     * The completions that were last provided by the served view.
     */
    AutoPtr< ArrayOf<ICompletionInfo*> > mCompletions;

    // Cursor position on the screen.
    AutoPtr<IRect> mTmpCursorRect;
    AutoPtr<IRect> mCursorRect;
    Int32 mCursorSelStart;
    Int32 mCursorSelEnd;
    Int32 mCursorCandStart;
    Int32 mCursorCandEnd;

    // -----------------------------------------------------------

    /**
     * Sequence number of this binding, as returned by the server.
     */
    Int32 mBindSequence;
    /**
     * ID of the method we are bound to.
     */
    String mCurId;
    /**
     * The actual instance of the method to make calls on it.
     */
    AutoPtr<IIInputMethodSession> mCurMethod;

    AutoPtr<PendingEvent> mPendingEventPool;
    Int32 mPendingEventPoolSize;
    AutoPtr<PendingEvent> mFirstPendingEvent;

    AutoPtr<IInputMethodClient> mClient;

    AutoPtr<IInputConnection> mDummyInputConnection;

    AutoPtr<IInputMethodCallback> mInputMethodCallback;

private:
    static const Int32 MAX_PENDING_EVENT_POOL_SIZE = 4;
};

} // namespace InputMethod
} // namespace View
} // namespace Droid
} // namespace Elastos

#endif   //__CLOCALINPUTMETHODMANAGER_H__
