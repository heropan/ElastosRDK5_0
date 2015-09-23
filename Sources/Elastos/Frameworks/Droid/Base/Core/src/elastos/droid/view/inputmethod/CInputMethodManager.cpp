
#include "view/inputmethod/CInputMethodManager.h"
#include "graphics/CRect.h"
#include "os/ServiceManager.h"
#include "os/Looper.h"
#include "os/SystemClock.h"
#include "view/View.h"
#include "view/ViewRootImpl.h"
#include "view/CInputBindResult.h"
#include "view/inputmethod/CEditorInfo.h"
#include "view/inputmethod/CBaseInputConnection.h"
#include "view/inputmethod/CInputMethodManagerInputMethodCallback.h"
#include "view/inputmethod/CIInputMethodClient.h"
#include "view/inputmethod/CControlledInputConnectionWrapper.h"
#include <elastos/utility/logging/Logger.h>

using Elastos::Utility::Logging::Logger;
using Elastos::Utility::CObjectMap;

using Elastos::Droid::Graphics::CRect;
using Elastos::Droid::Os::Looper;
using Elastos::Droid::Os::SystemClock;
using Elastos::Droid::Os::IBinder;
using Elastos::Droid::Os::ServiceManager;
using Elastos::Droid::View::View;
using Elastos::Droid::View::ViewRootImpl;
using Elastos::Droid::View::InputMethod::CInputMethodManagerInputMethodCallback;
using Elastos::Droid::View::InputMethod::CControlledInputConnectionWrapper;

namespace Elastos {
namespace Droid {
namespace View {
namespace InputMethod {

const Boolean CInputMethodManager::DEBUG = FALSE;
const String CInputMethodManager::TAG("CInputMethodManager");

Mutex CInputMethodManager::sInstanceSync;
AutoPtr<IInputMethodManager> CInputMethodManager::sInstance;
const Int64 CInputMethodManager::INPUT_METHOD_NOT_RESPONDING_TIMEOUT;
const Int32 CInputMethodManager::MAX_PENDING_EVENT_POOL_SIZE;

const Int32 CInputMethodManager::MSG_DUMP = 1;
const Int32 CInputMethodManager::MSG_BIND = 2;
const Int32 CInputMethodManager::MSG_UNBIND = 3;
const Int32 CInputMethodManager::MSG_SET_ACTIVE = 4;
const Int32 CInputMethodManager::MSG_EVENT_TIMEOUT = 5;

ECode CInputMethodManager::MyHandler::HandleMessage(
    /* [in] */ IMessage* msg)
{
    Int32 what, arg1;
    msg->GetWhat(&what);
    msg->GetArg1(&arg1);
    AutoPtr<IInterface> obj;
    msg->GetObj((IInterface**)&obj);

    switch(what) {
        case CInputMethodManager::MSG_DUMP:
            // SomeArgs args = (SomeArgs)msg.obj;
            // try {
            //     doDump((FileDescriptor)args.arg1,
            //             (PrintWriter)args.arg2, (String[])args.arg3);
            // } catch (RuntimeException e) {
            //     ((PrintWriter)args.arg2).println("Exception: " + e);
            // }
            // synchronized (args.arg4) {
            //     ((CountDownLatch)args.arg4).countDown();
            // }
            // args.recycle();
            break;
        case CInputMethodManager::MSG_BIND: {
            IInputBindResult* result = IInputBindResult::Probe(obj);
            mHost->HandleBind(result);
            break;
        }
        case CInputMethodManager::MSG_UNBIND: {
            mHost->HandleUnBind(arg1);
            break;
        }
        case CInputMethodManager::MSG_SET_ACTIVE: {
            mHost->HandleSetActive(arg1 != 0);
            break;
        }
        case CInputMethodManager::MSG_EVENT_TIMEOUT:
            mHost->HandleEventTimeout(arg1);
            break;
    }

    return NOERROR;
}

ECode CInputMethodManager::StartInputRunnable::Run()
{
    mHost->StartInputInner(NULL, 0, 0, 0);
    return NOERROR;
}

CAR_INTERFACE_IMPL(CInputMethodManager::PendingEvent, IInterface)

CInputMethodManager::PendingEvent::PendingEvent()
    : mStartTime(0)
    , mSeq(0)
{}

CInputMethodManager::CInputMethodManager()
    : mActive(FALSE)
    , mHasBeenInactive(TRUE)
    , mFullscreenMode(FALSE)
    , mServedConnecting(FALSE)
    , mCursorSelStart(0)
    , mCursorSelEnd(0)
    , mCursorCandStart(0)
    , mCursorCandEnd(0)
    , mBindSequence(-1)
    , mPendingEventPoolSize(0)
{
    CRect::New((IRect**)&mTmpCursorRect);
    CRect::New((IRect**)&mCursorRect);
    CIInputMethodClient::New(this, (IInputMethodClient**)&mClient);
    CBaseInputConnection::New(this, FALSE, (IBaseInputConnection**)&mDummyInputConnection);
    CInputMethodManagerInputMethodCallback::New(this, (IInputMethodCallback**)&mInputMethodCallback);
}

ECode CInputMethodManager::constructor(
    /* [in] */ IIInputMethodManager* service,
    /* [in] */ ILooper* looper)
{
    mService = service;
    mMainLooper = looper;
    mH = new MyHandler(looper, this);
    CControlledInputConnectionWrapper::New(looper, mDummyInputConnection,
        this, (IInputContext**)&mIInputContext);

    if (sInstance == NULL) {
        sInstance = this;
    }
    return NOERROR;
}

AutoPtr<IInputMethodManager> CInputMethodManager::GetInstance(
    /* [in] */ IContext* context)
{
    AutoPtr<ILooper> mainLooper;
    context->GetMainLooper((ILooper**)&mainLooper);
    return GetInstance(mainLooper);
}

AutoPtr<IInputMethodManager> CInputMethodManager::GetInstance(
    /* [in] */ ILooper* mainLooper)
{
    AutoLock lock(sInstanceSync);

    if (sInstance != NULL) {
        return sInstance;
    }
    AutoPtr<IIInputMethodManager> service =
            (IIInputMethodManager*)ServiceManager::GetService(IContext::INPUT_METHOD_SERVICE).Get();
    ASSERT_SUCCEEDED(CInputMethodManager::New(service, mainLooper, (IInputMethodManager**)&sInstance));
    return sInstance;
}

AutoPtr<IInputMethodManager> CInputMethodManager::PeekInstance()
{
    return sInstance;
}

ECode CInputMethodManager::GetClient(
    /* [out] */ IInputMethodClient** client)
{
    VALIDATE_NOT_NULL(client);
    *client = mClient;
    REFCOUNT_ADD(*client);
    return NOERROR;
}

ECode CInputMethodManager::GetInputContext(
    /* [out] */ IInputContext** context)
{
    VALIDATE_NOT_NULL(context);
    *context = mIInputContext;
    REFCOUNT_ADD(*context);
    return NOERROR;
}

ECode CInputMethodManager::GetInputMethodList(
    /* [in] */ IObjectContainer** infos)
{
    VALIDATE_NOT_NULL(infos);
    // try {
    return mService->GetInputMethodList(infos);
    // } catch (RemoteException e) {
    //     throw new RuntimeException(e);
    // }
}

ECode CInputMethodManager::GetEnabledInputMethodList(
    /* [out] */ IObjectContainer** list)
{
    VALIDATE_NOT_NULL(list);
    // try {
    return mService->GetEnabledInputMethodList(list);
    // } catch (RemoteException e) {
    //     throw new RuntimeException(e);
    // }
}

ECode CInputMethodManager::GetEnabledInputMethodSubtypeList(
    /* [in] */ IInputMethodInfo* imi,
    /* [in] */ Boolean allowsImplicitlySelectedSubtypes,
    /* [out] */ IObjectContainer** infos)
{
    VALIDATE_NOT_NULL(infos);
    // try {
    return mService->GetEnabledInputMethodSubtypeList(imi, allowsImplicitlySelectedSubtypes, infos);
    // } catch (RemoteException e) {
    //     throw new RuntimeException(e);
    // }
}

ECode CInputMethodManager::ShowStatusIcon(
    /* [in] */ IBinder* imeToken,
    /* [in] */ const String& packageName,
    /* [in] */ Int32 iconId)
{
    // try {
    return mService->UpdateStatusIcon(imeToken, packageName, iconId);
    // } catch (RemoteException e) {
    //     throw new RuntimeException(e);
    // }
}

ECode CInputMethodManager::HideStatusIcon(
    /* [in] */ IBinder* imeToken)
{
    VALIDATE_NOT_NULL(imeToken);
    // try {
    return mService->UpdateStatusIcon(imeToken, String(NULL), 0);
    // } catch (RemoteException e) {
    //     throw new RuntimeException(e);
    // }
}

ECode CInputMethodManager::SetImeWindowStatus(
    /* [in] */ IBinder* imeToken,
    /* [in] */ Int32 vis,
    /* [in] */ Int32 backDisposition)
{
    //try {
    return mService->SetImeWindowStatus(imeToken, vis, backDisposition);
    //} catch (RemoteException e) {
    //    throw new RuntimeException(e);
    //}
}

ECode CInputMethodManager::SetFullscreenMode(
    /* [in] */ Boolean fullScreen)
{
    mFullscreenMode = fullScreen;
    return NOERROR;
}

ECode CInputMethodManager::RegisterSuggestionSpansForNotification(
    /* [in] */ ArrayOf<ISuggestionSpan*>* spans)
{
    //try {
    return mService->RegisterSuggestionSpansForNotification(spans);
    //} catch (RemoteException e) {
    //    throw new RuntimeException(e);
    //}
}

ECode CInputMethodManager::NotifySuggestionPicked(
    /* [in] */ ISuggestionSpan* span,
    /* [in] */ const String& originalString,
    /* [in] */ Int32 index)
{
    //try {
    Boolean result = FALSE;
    return mService->NotifySuggestionPicked(span, originalString, index, &result);
    //} catch (RemoteException e) {
    //    throw new RuntimeException(e);
    //}
}

ECode CInputMethodManager::IsFullscreenMode(
    /* [out] */ Boolean* isfull)
{
    VALIDATE_NOT_NULL(isfull);
    *isfull = mFullscreenMode;
    return NOERROR;
}

ECode CInputMethodManager::IsActive(
    /* [in] */ IView* view,
    /* [out] */ Boolean* active)
{
    VALIDATE_NOT_NULL(active);
    FAIL_RETURN(CheckFocus());
    AutoLock lock(mHLock);

    Boolean check = FALSE;
    *active = (mServedView.Get() == view ||
            (mServedView != NULL && (mServedView->CheckInputConnectionProxy(view, &check), check))) &&
            mCurrentTextBoxAttribute != NULL;
    return NOERROR;
}

ECode CInputMethodManager::IsActive(
    /* [out] */ Boolean* active)
{
    VALIDATE_NOT_NULL(active);
    FAIL_RETURN(CheckFocus());
    AutoLock lock(mHLock);

    *active = mServedView != NULL && mCurrentTextBoxAttribute != NULL;
    return NOERROR;
}

ECode CInputMethodManager::IsAcceptingText(
    /* [out] */ Boolean* isAccepting)
{
    VALIDATE_NOT_NULL(isAccepting);
    FAIL_RETURN(CheckFocus());

    *isAccepting = mServedInputConnection != NULL;
    return NOERROR;
}

void CInputMethodManager::ClearBindingLocked()
{
    ClearConnectionLocked();
    mBindSequence = -1;
    mCurId = NULL;
    mCurMethod = NULL;
}

void CInputMethodManager::ClearConnectionLocked()
{
    mCurrentTextBoxAttribute = NULL;
    mServedInputConnection = NULL;
    if (mServedInputConnectionWrapper != NULL) {
        mServedInputConnectionWrapper->Deactivate();
        mServedInputConnectionWrapper = NULL;
    }
}

void CInputMethodManager::FinishInputLocked()
{
    mCurRootView = NULL;
    mNextServedView = NULL;
    if (mServedView != NULL) {
        if (DEBUG) Logger::V(TAG, "FINISH INPUT: %p", mServedView.Get());

        if (mCurrentTextBoxAttribute != NULL) {
            // try {
            mService->FinishInput(mClient);
            // } catch (RemoteException e) {
            // }
        }

        NotifyInputConnectionFinished();

        mServedView = NULL;
        mCompletions = NULL;
        mServedConnecting = FALSE;
        ClearConnectionLocked();
    }
}

/**
 * Notifies the served view that the current InputConnection will no longer be used.
 */
void CInputMethodManager::NotifyInputConnectionFinished()
{
    if (mServedView != NULL && mServedInputConnection != NULL) {
        // We need to tell the previously served view that it is no
        // longer the input target, so it can reset its state.  Schedule
        // this call on its window's Handler so it will be on the correct
        // thread and outside of our lock.
        View* view = VIEW_PROBE(mServedView);
        AutoPtr<ViewRootImpl> viewRootImpl = view->GetViewRootImpl();
        if (viewRootImpl != NULL) {
            // This will result in a call to reportFinishInputConnection() below.
            viewRootImpl->DispatchFinishInputConnection(mServedInputConnection);
        }
    }
}

ECode CInputMethodManager::ReportFinishInputConnection(
    /* [in] */ IInputConnection* ic)
{
    if (mServedInputConnection.Get() != ic) {
        Boolean ret = FALSE;
        ic->FinishComposingText(&ret);
        // To avoid modifying the public InputConnection interface
        if (IBaseInputConnection::Probe(ic) != NULL) {
            BaseInputConnection* baseConnection = reinterpret_cast<BaseInputConnection*>(ic->Probe(EIID_BaseInputConnection));
            baseConnection->ReportFinish();
        }
    }
    return NOERROR;
}

ECode CInputMethodManager::DisplayCompletions(
    /* [in] */ IView* view,
    /* [in] */ ArrayOf<ICompletionInfo*>* completions)
{
    FAIL_RETURN(CheckFocus());
    AutoLock lock(mHLock);

    Boolean check = FALSE;
    if (mServedView.Get() != view && (mServedView == NULL ||
            (mServedView->CheckInputConnectionProxy(view, &check), !check))) {
        return NOERROR;
    }

    mCompletions = completions;
    if (mCurMethod != NULL) {
        // try {
        return mCurMethod->DisplayCompletions(mCompletions);
        // } catch (RemoteException e) {
        // }
    }
    return NOERROR;
}

ECode CInputMethodManager::UpdateExtractedText(
    /* [in] */ IView* view,
    /* [in] */ Int32 token,
    /* [in] */ IExtractedText* text)
{
    FAIL_RETURN(CheckFocus());
    AutoLock lock(mHLock);

    Boolean check = FALSE;
    if (mServedView.Get() != view && (mServedView == NULL ||
            (mServedView->CheckInputConnectionProxy(view, &check), !check))) {
        return NOERROR;
    }

    if (mCurMethod != NULL) {
        // try {
        return mCurMethod->UpdateExtractedText(token, text);
        // } catch (RemoteException e) {
        // }
    }
    return NOERROR;
}

ECode CInputMethodManager::ShowSoftInput(
    /* [in] */ IView* view,
    /* [in] */ Int32 flags,
    /* [out] */ Boolean* show)
{
    return ShowSoftInput(view, flags, NULL, show);
}

ECode CInputMethodManager::ShowSoftInput(
    /* [in] */ IView* view,
    /* [in] */ Int32 flags,
    /* [in] */ IResultReceiver* resultReceiver,
    /* [out] */ Boolean* show)
{
    VALIDATE_NOT_NULL(show);
    FAIL_RETURN(CheckFocus());
    AutoLock lock(mHLock);

    Boolean check = FALSE;
    //Logger::V(TAG, "CInputMethodManager::ShowSoftInputEx, view:%p, ServedView:%p", view, mServedView.Get());
    if (mServedView.Get() != view && (mServedView == NULL ||
            (mServedView->CheckInputConnectionProxy(view, &check), !check))) {
        *show = FALSE;
        return NOERROR;
    }
    // try {
    return mService->ShowSoftInput(mClient, flags, resultReceiver, show);
    // } catch (RemoteException e) {
    // }

    // return false;
}

ECode CInputMethodManager::ShowSoftInputUnchecked(
    /* [in] */ Int32 flags,
    /* [in] */ IResultReceiver* resultReceiver)
{
    // try {
    Boolean ret = FALSE;
    return mService->ShowSoftInput(mClient, flags, resultReceiver, &ret);
    // } catch (RemoteException e) {
    // }
}

ECode CInputMethodManager::HideSoftInputFromWindow(
    /* [in] */ IBinder* windowToken,
    /* [in] */ Int32 flags,
    /* [out] */ Boolean* hide)
{
    return HideSoftInputFromWindow(windowToken, flags, NULL, hide);
}

ECode CInputMethodManager::HideSoftInputFromWindow(
    /* [in] */ IBinder* windowToken,
    /* [in] */ Int32 flags,
    /* [in] */ IResultReceiver* resultReceiver,
    /* [out] */ Boolean* hide)
{
    VALIDATE_NOT_NULL(hide);
    FAIL_RETURN(CheckFocus());
    AutoLock lock(mHLock);

    AutoPtr<IBinder> token;
    if (mServedView == NULL ||
            (mServedView->GetWindowToken((IBinder**)&token), token.Get() != windowToken)) {
        *hide = FALSE;
        return NOERROR;
    }

    // try {
    return mService->HideSoftInput(mClient, flags, resultReceiver, hide);
    // } catch (RemoteException e) {
    // }
    // return false;
}

ECode CInputMethodManager::ToggleSoftInputFromWindow(
    /* [in] */ IBinder* windowToken,
    /* [in] */ Int32 showFlags,
    /* [in] */ Int32 hideFlags)
{
    AutoLock lock(mHLock);

    AutoPtr<IBinder> token;
    if (mServedView == NULL ||
            (mServedView->GetWindowToken((IBinder**)&token), token.Get() != windowToken)) {
        return NOERROR;
    }
    if (mCurMethod != NULL) {
        // try {
        return mCurMethod->ToggleSoftInput(showFlags, hideFlags);
        // } catch (RemoteException e) {
        // }
    }
    return NOERROR;
}

ECode CInputMethodManager::ToggleSoftInput(
    /* [in] */ Int32 showFlags,
    /* [in] */ Int32 hideFlags)
{
    if (mCurMethod != NULL) {
        // try {
        return mCurMethod->ToggleSoftInput(showFlags, hideFlags);
        // } catch (RemoteException e) {
        // }
    }
    return NOERROR;
}

ECode CInputMethodManager::RestartInput(
    /* [in] */ IView* view)
{
    FAIL_RETURN(CheckFocus());
    {
        AutoLock lock(mHLock);

        Boolean check = FALSE;
        if (mServedView.Get() != view && (mServedView == NULL ||
                (mServedView->CheckInputConnectionProxy(view, &check), !check))) {
            return NOERROR;
        }

        mServedConnecting = TRUE;
    }

    StartInputInner(NULL, 0, 0, 0);
    return NOERROR;
}

Boolean CInputMethodManager::StartInputInner(
    /* [in] */ IBinder* windowGainingFocus,
    /* [in] */ Int32 controlFlags,
    /* [in] */ Int32 softInputMode,
    /* [in] */ Int32 windowFlags)
{
    AutoPtr<IView> view;
    {
        AutoLock lock(mHLock);

        view = mServedView;

        // Make sure we have a window token for the served view.
        // if (DEBUG) Log.v(TAG, "Starting input: view=" + view);
        if (view == NULL) {
            if (DEBUG) Logger::V(TAG, "ABORT input: no served view!");
            return FALSE;
        }
    }

    // Now we need to get an input connection from the served view.
    // This is complicated in a couple ways: we can't be holding our lock
    // when calling out to the view, and we need to make sure we call into
    // the view on the same thread that is driving its view hierarchy.
    AutoPtr<IHandler> vh;
    view->GetHandler((IHandler**)&vh);
    if (vh == NULL) {
        // If the view doesn't have a handler, something has changed out
        // from under us, so just close the current input.
        // If we don't close the current input, the current input method can remain on the
        // screen without a connection.
        if (DEBUG) Logger::V(TAG, "ABORT input: no handler for view! Close current input.");
        CloseCurrentInput();
        return FALSE;
    }

    AutoPtr<ILooper> looper;
    vh->GetLooper((ILooper**)&looper);
    if (looper != Looper::GetMyLooper()) {
        // The view is running on a different thread than our own, so
        // we need to reschedule our work for over there.
        if (DEBUG) Logger::V(TAG, "Starting input: reschedule to view thread");
        AutoPtr<IRunnable> runnable = new StartInputRunnable(this);
        Boolean result;
        vh->Post(runnable, &result);
        return FALSE;
    }

    // Okay we are now ready to call into the served view and have it
    // do its stuff.
    // Life is good: let's hook everything up!
    AutoPtr<IEditorInfo> tba;
    ASSERT_SUCCEEDED(CEditorInfo::New((IEditorInfo**)&tba));
    AutoPtr<IContext> ctx;
    view->GetContext((IContext**)&ctx);
    String pkgName;
    ctx->GetPackageName(&pkgName);
    tba->SetPackageName(pkgName);
    Int32 id;
    view->GetId(&id);
    tba->SetFieldId(id);
    AutoPtr<IInputConnection> ic;
    view->OnCreateInputConnection(tba, (IInputConnection**)&ic);
    if (DEBUG) {
        Logger::V(TAG, "Starting input: IEditorInfo=%p, IInputConnection=%p, old mServedInputConnection=%p",
            tba.Get(), ic.Get(), mServedInputConnection.Get());
    }

    {
        AutoLock lock(mHLock);

        // Now that we are locked again, validate that our state hasn't
        // changed.
        if (mServedView != view || !mServedConnecting) {
            // Something else happened, so abort.
            if (DEBUG) {
                Logger::V(TAG, "Starting input: finished by someone else (view=%p, conn=%d)",
                    mServedView.Get(), mServedConnecting);
            }
            return FALSE;
        }

        // If we already have a text box, then this view is already
        // connected so we want to restart it.
        if (mCurrentTextBoxAttribute == NULL) {
            controlFlags |= CONTROL_START_INITIAL;
        }

        // Hook 'em up and let 'er rip.
        mCurrentTextBoxAttribute = tba;
        mServedConnecting = FALSE;
        // Notify the served view that its previous input connection is finished
        NotifyInputConnectionFinished();
        mServedInputConnection = ic;
        AutoPtr<CControlledInputConnectionWrapper> servedContext;
        if (ic != NULL) {
            tba->GetInitialSelStart(&mCursorSelStart);
            tba->GetInitialSelEnd(&mCursorSelEnd);
            mCursorCandStart = -1;
            mCursorCandEnd = -1;
            mCursorRect->SetEmpty();
            CControlledInputConnectionWrapper::NewByFriend(looper, ic, this,
                (CControlledInputConnectionWrapper**)&servedContext);
        }
        else {
            servedContext = NULL;
        }

        if (mServedInputConnectionWrapper != NULL) {
            mServedInputConnectionWrapper->Deactivate();
        }
        mServedInputConnectionWrapper = servedContext;

        // try {
        if (DEBUG) {
            Logger::V(TAG, "START INPUT: %p, ic=%p, IEditorInfo=%p, controlFlags=%08x",
                view.Get(), ic.Get(), tba.Get(), controlFlags);
        }
        AutoPtr<IInputBindResult> res;
        if (windowGainingFocus != NULL) {
            mService->WindowGainedFocus(mClient, windowGainingFocus,
                controlFlags, softInputMode, windowFlags,
                tba, servedContext, (IInputBindResult**)&res);
        }
        else {
            mService->StartInput(mClient,
                servedContext, tba, controlFlags, (IInputBindResult**)&res);
        }
        if (DEBUG) Logger::V(TAG, "Starting input: Bind result=%p", res.Get());
        if (res != NULL) {
            String id;
            res->GetId(&id);
            if (!id.IsNull()) {
                res->GetSequence(&mBindSequence);
                mCurMethod = NULL;
                res->GetIIMSession((IIInputMethodSession**)&mCurMethod);
                mCurId = id;
            }
            else if (mCurMethod == NULL) {
                // This means there is no input method available.
                if (DEBUG) Logger::V(TAG, "ABORT input: no input method!");
                return TRUE;
            }
        }
        if (mCurMethod != NULL && mCompletions != NULL) {
            // try {
            mCurMethod->DisplayCompletions(mCompletions);
            // } catch (RemoteException e) {
            // }
        }
        // } catch (RemoteException e) {
        //     Log.w(TAG, "IME died: " + mCurId, e);
        // }
    }

    return TRUE;
}

ECode CInputMethodManager::WindowDismissed(
    /* [in] */ IBinder* appWindowToken)
{
    FAIL_RETURN(CheckFocus());
    AutoLock lock(mHLock);

    AutoPtr<IBinder> token;
    if (mServedView != NULL &&
            (mServedView->GetWindowToken((IBinder**)&token), token.Get() == appWindowToken)) {
        FinishInputLocked();
    }
    return NOERROR;
}

ECode CInputMethodManager::FocusIn(
    /* [in] */ IView* view)
{
    AutoLock lock(mHLock);
    FocusInLocked(view);
    return NOERROR;
}

void CInputMethodManager::FocusInLocked(
    /* [in] */ IView* view)
{
    // if (DEBUG) Log.v(TAG, "focusIn: " + view);
    if(DEBUG)
        Logger::V(TAG, "CInputMethodManager::FocusInLocked, line:%d, view:%p", __LINE__, view);

    AutoPtr<IView> rootView;
    view->GetRootView((IView**)&rootView);
    if (mCurRootView != rootView) {
        // This is a request from a window that isn't in the window with
        // IME focus, so ignore it.
        // if (DEBUG) Log.v(TAG, "Not IME target window, ignoring");
        return;
    }

    mNextServedView = view;
    ScheduleCheckFocusLocked(view);
}

ECode CInputMethodManager::FocusOut(
    /* [in] */ IView* view)
{
    AutoLock lock(mHLock);

    // if (DEBUG) Log.v(TAG, "focusOut: " + view
    //         + " mServedView=" + mServedView
    //         + " winFocus=" + view.hasWindowFocus());
    if (mNextServedView.Get() != view) {
        // The following code would auto-hide the IME if we end up
        // with no more views with focus.  This can happen, however,
        // whenever we go into touch mode, so it ends up hiding
        // at times when we don't really want it to.  For now it
        // seems better to just turn it all off.
        Boolean focus = FALSE;
        if (FALSE && (view->HasWindowFocus(&focus), focus)) {
            mNextServedView = NULL;
            ScheduleCheckFocusLocked(view);
        }
    }
    return NOERROR;
}

void CInputMethodManager::ScheduleCheckFocusLocked(
    /* [in] */ IView* view)
{
    View* _view = VIEW_PROBE(view);
    AutoPtr<ViewRootImpl> viewRootImpl = _view->GetViewRootImpl();
    if (viewRootImpl != NULL) {
        viewRootImpl->DispatchCheckFocus();
    }
}

ECode CInputMethodManager::CheckFocus()
{
    if (CheckFocusNoStartInput(FALSE, TRUE)) {
        StartInputInner(NULL, 0, 0, 0);
    }
    return NOERROR;
}

Boolean CInputMethodManager::CheckFocusNoStartInput(
    /* [in] */ Boolean forceNewFocus,
    /* [in] */ Boolean finishComposingText)
{
    // This is called a lot, so short-circuit before locking.
    if (mServedView == mNextServedView && !forceNewFocus) {
        return FALSE;
    }

    AutoPtr<IInputConnection> ic;
    {
        AutoLock lock(mHLock);

        if (mServedView == mNextServedView && !forceNewFocus) {
            return FALSE;
        }
        // if (DEBUG) Log.v(TAG, "checkFocus: view=" + mServedView
        //         + " next=" + mNextServedView
        //         + " restart=" + mNextServedNeedsStart);

        if (mNextServedView == NULL) {
            FinishInputLocked();
            // In this case, we used to have a focused view on the window,
            // but no longer do.  We should make sure the input method is
            // no longer shown, since it serves no purpose.
            CloseCurrentInput();
            return FALSE;
        }

        ic = mServedInputConnection;

        mServedView = mNextServedView;
        mCurrentTextBoxAttribute = NULL;
        mCompletions = NULL;
        mServedConnecting = TRUE;
    }

    if (finishComposingText && ic != NULL) {
        Boolean ret = FALSE;
        ic->FinishComposingText(&ret);
    }

    return TRUE;
}

void CInputMethodManager::CloseCurrentInput()
{
    // try {
    Boolean ret = FALSE;
    mService->HideSoftInput(mClient, IInputMethodManager::HIDE_NOT_ALWAYS, NULL, &ret);
    // } catch (RemoteException e) {
    // }
}

ECode CInputMethodManager::OnWindowFocus(
    /* [in] */ IView* rootView,
    /* [in] */ IView* focusedView,
    /* [in] */ Int32 softInputMode,
    /* [in] */ Boolean first,
    /* [in] */ Int32 windowFlags)
{
    Boolean forceNewFocus = FALSE;

    {
        AutoLock lock(mHLock);

        // if (DEBUG) Log.v(TAG, "onWindowFocus: " + focusedView
        //         + " softInputMode=" + softInputMode
        //         + " first=" + first + " flags=#"
        //         + Integer.toHexString(windowFlags));
        if (mHasBeenInactive) {
            // if (DEBUG) Log.v(TAG, "Has been inactive!  Starting fresh");
            mHasBeenInactive = FALSE;
            forceNewFocus = TRUE;
        }

        FocusInLocked(focusedView != NULL ? focusedView : rootView);
    }

    Int32 controlFlags = 0;
    if (focusedView != NULL) {
        controlFlags |= CONTROL_WINDOW_VIEW_HAS_FOCUS;
        Boolean result = FALSE;
        if (focusedView->OnCheckIsTextEditor(&result), result) {
            controlFlags |= CONTROL_WINDOW_IS_TEXT_EDITOR;
        }
    }
    if (first) {
        controlFlags |= CONTROL_WINDOW_FIRST;
    }

    AutoPtr<IBinder> binder;
    rootView->GetWindowToken((IBinder**)&binder);
    if (CheckFocusNoStartInput(forceNewFocus, TRUE)) {
        // We need to restart input on the current focus view.  This
        // should be done in conjunction with telling the system service
        // about the window gaining focus, to help make the transition
        // smooth.
        if (StartInputInner(binder, controlFlags, softInputMode, windowFlags)) {
            return NOERROR;
        }
    }

    // For some reason we didn't do a startInput + windowFocusGain, so
    // we'll just do a window focus gain and call it a day.
    {
        AutoLock lock(mHLock);

        // try {
        //     if (DEBUG) Log.v(TAG, "Reporting focus gain, without startInput");
        AutoPtr<IInputBindResult> result;
        mService->WindowGainedFocus(mClient, binder,
                controlFlags, softInputMode, windowFlags, NULL, NULL, (IInputBindResult**)&result);
        // } catch (RemoteException e) {
        // }
    }
    return NOERROR;
}

ECode CInputMethodManager::StartGettingWindowFocus(
    /* [in] */ IView* rootView)
{
    AutoLock lock(mHLock);
    mCurRootView = rootView;
    return NOERROR;
}

ECode CInputMethodManager::UpdateSelection(
    /* [in] */ IView* view,
    /* [in] */ Int32 selStart,
    /* [in] */ Int32 selEnd,
    /* [in] */ Int32 candidatesStart,
    /* [in] */ Int32 candidatesEnd)
{
    FAIL_RETURN(CheckFocus());
    {
        AutoLock lock(mHLock);

        Boolean check = FALSE;
        if ((mServedView.Get() != view && (mServedView == NULL ||
            (mServedView->CheckInputConnectionProxy(view, &check), !check))) ||
            mCurrentTextBoxAttribute == NULL || mCurMethod == NULL) {
            return NOERROR;
        }

        if (mCursorSelStart != selStart || mCursorSelEnd != selEnd
                || mCursorCandStart != candidatesStart
                || mCursorCandEnd != candidatesEnd) {
            // if (DEBUG) Log.d(TAG, "updateSelection");

            // try {
                // if (DEBUG) Log.v(TAG, "SELECTION CHANGE: " + mCurMethod);
            mCurMethod->UpdateSelection(mCursorSelStart, mCursorSelEnd,
                    selStart, selEnd, candidatesStart, candidatesEnd);
            mCursorSelStart = selStart;
            mCursorSelEnd = selEnd;
            mCursorCandStart = candidatesStart;
            mCursorCandEnd = candidatesEnd;
            // } catch (RemoteException e) {
            //     Log.w(TAG, "IME died: " + mCurId, e);
            // }
        }
    }
    return NOERROR;
}

ECode CInputMethodManager::ViewClicked(
    /* [in] */ IView* view)
{
    const Boolean focusChanged = mServedView != mNextServedView;
    FAIL_RETURN(CheckFocus());
    {
        AutoLock lock(mHLock);

        Boolean check = FALSE;
        if ((mServedView.Get() != view && (mServedView.Get() == NULL
                || !(mServedView->CheckInputConnectionProxy(view, &check), check)))
                || mCurrentTextBoxAttribute == NULL || mCurMethod == NULL) {
            return NOERROR;
        }
        // try {
        //     if (DEBUG) Log.v(TAG, "onViewClicked: " + focusChanged);
        return mCurMethod->ViewClicked(focusChanged);
        // } catch (RemoteException e) {
        //     Log.w(TAG, "IME died: " + mCurId, e);
        // }
    }
    return NOERROR;
}

ECode CInputMethodManager::IsWatchingCursor(
    /* [in] */ IView* view,
    /* [out] */ Boolean* isWatching)
{
    VALIDATE_NOT_NULL(isWatching);
    *isWatching = FALSE;
    return NOERROR;
}

ECode CInputMethodManager::UpdateCursor(
    /* [in] */ IView* view,
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom)
{
    FAIL_RETURN(CheckFocus());
    {
        AutoLock lock(mHLock);

        Boolean check = FALSE;
        if ((mServedView.Get() != view && (mServedView == NULL ||
            (mServedView->CheckInputConnectionProxy(view, &check), !check))) ||
            mCurrentTextBoxAttribute == NULL || mCurMethod == NULL) {
            return NOERROR;
        }

        mTmpCursorRect->Set(left, top, right, bottom);
        Boolean equal = FALSE;
        if (mCursorRect->Equals(mTmpCursorRect, &equal), !equal) {
            // if (DEBUG) Log.d(TAG, "updateCursor");

            // try {
                // if (DEBUG) Log.v(TAG, "CURSOR CHANGE: " + mCurMethod);
            mCurMethod->UpdateCursor(mTmpCursorRect);
            mCursorRect->Set(mTmpCursorRect);
            // } catch (RemoteException e) {
            //     Log.w(TAG, "IME died: " + mCurId, e);
            // }
        }
    }
    return NOERROR;
}

ECode CInputMethodManager::SendAppPrivateCommand(
    /* [in] */ IView* view,
    /* [in] */ const String& action,
    /* [in] */ IBundle* data)
{
    FAIL_RETURN(CheckFocus());
    {
        AutoLock lock(mHLock);

        Boolean check;
        if ((mServedView.Get() != view && (mServedView == NULL ||
                (mServedView->CheckInputConnectionProxy(view, &check), !check))) ||
                mCurrentTextBoxAttribute == NULL || mCurMethod == NULL) {
            return NOERROR;
        }
        // try {
            // if (DEBUG) Log.v(TAG, "APP PRIVATE COMMAND " + action + ": " + data);
        mCurMethod->AppPrivateCommand(action, data);
        // } catch (RemoteException e) {
        //     Log.w(TAG, "IME died: " + mCurId, e);
        // }
    }
    return NOERROR;
}

ECode CInputMethodManager::SetInputMethod(
    /* [in] */ IBinder* token,
    /* [in] */ const String& id)
{
    // try {
    return mService->SetInputMethod(token, id);
    // } catch (RemoteException e) {
    //     throw new RuntimeException(e);
    // }
}

ECode CInputMethodManager::SetInputMethodAndSubtype(
    /* [in] */ IBinder* token,
    /* [in] */ const String& id,
    /* [in] */ IInputMethodSubtype* subtype)
{
    // try {
    return mService->SetInputMethodAndSubtype(token, id, subtype);
    // } catch (RemoteException e) {
    //     throw new RuntimeException(e);
    // }
}

ECode CInputMethodManager::HideSoftInputFromInputMethod(
    /* [in] */ IBinder* token,
    /* [in] */ Int32 flags)
{
    // try {
    return mService->HideMySoftInput(token, flags);
    // } catch (RemoteException e) {
    //     throw new RuntimeException(e);
    // }
}

ECode CInputMethodManager::ShowSoftInputFromInputMethod(
    /* [in] */ IBinder* token,
    /* [in] */ Int32 flags)
{
    // try {
    return mService->ShowMySoftInput(token, flags);
    // } catch (RemoteException e) {
    //     throw new RuntimeException(e);
    // }
}

ECode CInputMethodManager::DispatchKeyEvent(
    /* [in] */ IContext* context,
    /* [in] */ Int32 seq,
    /* [in] */ IKeyEvent* key,
    /* [in] */ IInputMethodManagerFinishedEventCallback* callback)
{
    Boolean handled = FALSE;
    {
        AutoLock lock(mHLock);

        if (DEBUG) Logger::D(TAG, "dispatchKeyEvent");

        if (mCurMethod != NULL) {
            Int32 action = 0, keyCode = 0;
            key->GetAction(&action);
            key->GetKeyCode(&keyCode);
            if (action == IKeyEvent::ACTION_DOWN && keyCode == IKeyEvent::KEYCODE_SYM) {
                ShowInputMethodPickerLocked();
                handled = TRUE;
            }
            else {
                // try {
                if (DEBUG) Logger::V(TAG, "DISPATCH KEY: %p, %s", mCurMethod.Get(), mCurId.string());
                Int64 startTime = SystemClock::GetUptimeMillis();
                EnqueuePendingEventLocked(startTime, seq, mCurId, callback);
                ECode ec = mCurMethod->DispatchKeyEvent(seq, key, mInputMethodCallback);
                if (FAILED(ec)) {
                    Int32 action = 0;
                    key->GetAction(&action);
                    Logger::W(TAG, "IME died: %s dropping: key action %d", mCurId.string(), action);
                }
                return ec;
                // } catch (RemoteException e) {
                //     Log.w(TAG, "IME died: " + mCurId + " dropping: " + key, e);
                // }
            }
        }
    }

    return callback->FinishedEvent(seq, handled);
}

ECode CInputMethodManager::DispatchTrackballEvent(
    /* [in] */ IContext* context,
    /* [in] */ Int32 seq,
    /* [in] */ IMotionEvent* motion,
    /* [in] */ IInputMethodManagerFinishedEventCallback* callback)
{
    {
        AutoLock lock(mHLock);
        if (DEBUG) Logger::D(TAG, "dispatchTrackballEvent");

        if (mCurMethod != NULL && mCurrentTextBoxAttribute != NULL) {
            // try {
            if (DEBUG) Logger::V(TAG, "DISPATCH TRACKBALL: %p, %s", mCurMethod.Get(), mCurId.string());
            Int64 startTime = SystemClock::GetUptimeMillis();
            EnqueuePendingEventLocked(startTime, seq, mCurId, callback);
            ECode ec = mCurMethod->DispatchTrackballEvent(seq, motion, mInputMethodCallback);
            if (FAILED(ec)) {
                Logger::W(TAG, "IME died: %s dropping trackball action %p", mCurId.string(), motion);
            }
            return ec;
            // } catch (RemoteException e) {
            //     Log.w(TAG, "IME died: " + mCurId + " dropping trackball: " + motion, e);
            // }
        }
    }

    return callback->FinishedEvent(seq, FALSE);
}

ECode CInputMethodManager::DispatchGenericMotionEvent(
    /* [in] */ IContext* context,
    /* [in] */ Int32 seq,
    /* [in] */ IMotionEvent* motion,
    /* [in] */ IInputMethodManagerFinishedEventCallback* callback)
{
    {
        AutoLock lock(mHLock);
        if (DEBUG) Logger::D(TAG, "dispatchGenericMotionEvent");

        if (mCurMethod != NULL && mCurrentTextBoxAttribute != NULL) {
            // try {
            if (DEBUG) Logger::V(TAG, "DISPATCH GENERIC MOTION: %p, %s", mCurMethod.Get(), mCurId.string());
            Int64 startTime = SystemClock::GetUptimeMillis();
            EnqueuePendingEventLocked(startTime, seq, mCurId, callback);
            ECode ec = mCurMethod->DispatchGenericMotionEvent(seq, motion, mInputMethodCallback);
            if (FAILED(ec)) {
                Logger::W(TAG, "IME died: %s dropping generic motion %p", mCurId.string(), motion);
            }
            return ec;
            // } catch (RemoteException e) {
            //     Log.w(TAG, "IME died: " + mCurId + " dropping generic motion: " + motion, e);
            // }
        }
    }

    return callback->FinishedEvent(seq, FALSE);
}

void CInputMethodManager::FinishedEvent(
    /* [in] */ Int32 seq,
    /* [in] */ Boolean handled)
{
    AutoPtr<IInputMethodManagerFinishedEventCallback> callback;
    {
        AutoLock lock(mHLock);

        AutoPtr<PendingEvent> p = DequeuePendingEventLocked(seq);
        if (p == NULL) {
            return; // spurious, event already finished or timed out
        }
        mH->RemoveMessages(MSG_EVENT_TIMEOUT, p);
        callback = p->mCallback;
        RecyclePendingEventLocked(p);
    }
    callback->FinishedEvent(seq, handled);
}

void CInputMethodManager::TimeoutEvent(
    /* [in] */ Int32 seq)
{
    AutoPtr<IInputMethodManagerFinishedEventCallback> callback;
    {
        AutoLock lock(mHLock);

        AutoPtr<PendingEvent> p = DequeuePendingEventLocked(seq);
        if (p == NULL) {
            return; // spurious, event already finished or timed out
        }
        Int64 delay = SystemClock::GetUptimeMillis() - p->mStartTime;
        Logger::W(TAG, "Timeout waiting for IME to handle input event after %lld ms: %s", delay, p->mInputMethodId.string());
        callback = p->mCallback;
        RecyclePendingEventLocked(p);
    }
    callback->FinishedEvent(seq, FALSE);
}

void CInputMethodManager::EnqueuePendingEventLocked(
    /* [in] */ Int64 startTime,
    /* [in] */ Int32 seq,
    /* [in] */ const String& inputMethodId,
    /* [in] */ IInputMethodManagerFinishedEventCallback* callback)
{
    AutoPtr<PendingEvent> p = ObtainPendingEventLocked(startTime, seq, inputMethodId, callback);
    p->mNext = mFirstPendingEvent;
    mFirstPendingEvent = p;

    AutoPtr<IMessage> msg;
    mH->ObtainMessage(MSG_EVENT_TIMEOUT, seq, 0, p, (IMessage**)&msg);
    msg->SetAsynchronous(TRUE);
    Boolean result;
    mH->SendMessageDelayed(msg, INPUT_METHOD_NOT_RESPONDING_TIMEOUT, &result);
}

AutoPtr<CInputMethodManager::PendingEvent> CInputMethodManager::DequeuePendingEventLocked(
    /* [in] */ Int32 seq)
{
    AutoPtr<PendingEvent> p = mFirstPendingEvent;
    if (p == NULL) {
        return NULL;
    }
    if (p->mSeq == seq) {
        mFirstPendingEvent = p->mNext;
    }
    else {
        AutoPtr<PendingEvent> prev;
        do {
            prev = p;
            p = p->mNext;
            if (p == NULL) {
                return NULL;
            }
        } while (p->mSeq != seq);
        prev->mNext = p->mNext;
    }
    p->mNext = NULL;
    return p;
}

AutoPtr<CInputMethodManager::PendingEvent> CInputMethodManager::ObtainPendingEventLocked(
    /* [in] */ Int64 startTime,
    /* [in] */ Int32 seq,
    /* [in] */ const String& inputMethodId,
    /* [in] */ IInputMethodManagerFinishedEventCallback* callback)
{
    AutoPtr<PendingEvent> p = mPendingEventPool;
    if (p != NULL) {
        mPendingEventPoolSize -= 1;
        mPendingEventPool = p->mNext;
        p->mNext = NULL;
    }
    else {
        p = new PendingEvent();
    }

    p->mStartTime = startTime;
    p->mSeq = seq;
    p->mInputMethodId = inputMethodId;
    p->mCallback = callback;
    return p;
}

void CInputMethodManager::RecyclePendingEventLocked(
    /* [in] */ PendingEvent* p)
{
    p->mInputMethodId = NULL;
    p->mCallback = NULL;

    if (mPendingEventPoolSize < MAX_PENDING_EVENT_POOL_SIZE) {
        mPendingEventPoolSize += 1;
        p->mNext = mPendingEventPool;
        mPendingEventPool = p;
    }
}

ECode CInputMethodManager::ShowInputMethodPicker()
{
    AutoLock lock(mHLock);
    ShowInputMethodPickerLocked();
    return NOERROR;
}

void CInputMethodManager::ShowInputMethodPickerLocked()
{
    // try {
    mService->ShowInputMethodPickerFromClient(mClient);
    // } catch (RemoteException e) {
    //     Log.w(TAG, "IME died: " + mCurId, e);
    // }
}

ECode CInputMethodManager::ShowInputMethodAndSubtypeEnabler(
    /* [in] */ const String& imiId)
{
    AutoLock lock(mHLock);
    // try {
    return mService->ShowInputMethodAndSubtypeEnablerFromClient(mClient, imiId);
    // } catch (RemoteException e) {
    //     Log.w(TAG, "IME died: " + mCurId, e);
    // }
}

ECode CInputMethodManager::GetCurrentInputMethodSubtype(
    /* [out] */ IInputMethodSubtype** subtype)
{
    VALIDATE_NOT_NULL(subtype);
    {
        AutoLock lock(mHLock);
        // try {
        return mService->GetCurrentInputMethodSubtype(subtype);
        // } catch (RemoteException e) {
        //     Log.w(TAG, "IME died: " + mCurId, e);
        //     return null;
        // }
    }
}

ECode CInputMethodManager::SetCurrentInputMethodSubtype(
    /* [in] */ IInputMethodSubtype* subtype,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    {
        AutoLock lock(mHLock);
        // try {
        return mService->SetCurrentInputMethodSubtype(subtype, result);
        // } catch (RemoteException e) {
        //     Log.w(TAG, "IME died: " + mCurId, e);
        //     return false;
        // }
    }
}

ECode CInputMethodManager::GetShortcutInputMethodsAndSubtypes(
    /* [out] */ IObjectMap** ret)
{
    assert(0);
    VALIDATE_NOT_NULL(ret);
    {
        AutoLock lock(mHLock);

        CObjectMap::New(ret);
        // try {
        // TODO: We should change the return type from List<Object> to List<Parcelable>
        AutoPtr<IObjectContainer> info;
        mService->GetShortcutInputMethodsAndSubtypes((IObjectContainer**)&info);
        // "info" has imi1, subtype1, subtype2, imi2, subtype2, imi3, subtype3..in the list
        AutoPtr<IObjectContainer> subtypes;

        AutoPtr<IObjectEnumerator> infoEnum;
        info->GetObjectEnumerator((IObjectEnumerator**)&infoEnum);
        Boolean hasNext = FALSE;
        while(infoEnum->MoveNext(&hasNext), hasNext) {
            AutoPtr<IInterface> o;
            infoEnum->Current((IInterface**)&o);

            if (IInputMethodInfo::Probe(o) != NULL) {
                Boolean contains = FALSE;
                if ((*ret)->ContainsKey(o, &contains), contains) {
                    // Log.e(TAG, "IMI list already contains the same InputMethod.");
                    break;
                }

                // subtypes = new ArrayList<InputMethodSubtype>();
                subtypes = NULL;
                CObjectContainer::New((IObjectContainer**)&subtypes);
                (*ret)->Put(IInputMethodInfo::Probe(o), subtypes);
            }
            else if (subtypes != NULL && IInputMethodSubtype::Probe(o) != NULL) {
                subtypes->Add(IInputMethodSubtype::Probe(o));
            }
        }

        // } catch (RemoteException e) {
        //     Log.w(TAG, "IME died: " + mCurId, e);
        // }
        return NOERROR;
    }
}

ECode CInputMethodManager::SwitchToLastInputMethod(
    /* [in] */ IBinder* imeToken,
    /* [out] */ Boolean* switched)
{
    {
        AutoLock lock(mHLock);
        // try {
        return mService->SwitchToLastInputMethod(imeToken, switched);
        // } catch (RemoteException e) {
        //     Log.w(TAG, "IME died: " + mCurId, e);
        //     return false;
        // }
    }
}

ECode CInputMethodManager::SwitchToNextInputMethod(
    /* [in] */ IBinder* imeToken,
    /* [in] */ Boolean onlyCurrentIme,
    /* [out] */ Boolean* switched)
{
    {
        AutoLock lock(mHLock);
        // try {
        return mService->SwitchToNextInputMethod(imeToken, onlyCurrentIme, switched);
        // } catch (RemoteException e) {
        //     Log.w(TAG, "IME died: " + mCurId, e);
        //     return false;
        // }
    }
}

ECode CInputMethodManager::SetAdditionalInputMethodSubtypes(
    /* [in] */ const String& imiId,
    /* [in] */ ArrayOf<IInputMethodSubtype*>* subtypes)
{
    {
        AutoLock lock(mHLock);
        // try {
        return mService->SetAdditionalInputMethodSubtypes(imiId, subtypes);
        // } catch (RemoteException e) {
        //     Log.w(TAG, "IME died: " + mCurId, e);
        // }
    }
}

ECode CInputMethodManager::GetLastInputMethodSubtype(
    /* [out] */ IInputMethodSubtype** subtype)
{
    VALIDATE_NOT_NULL(subtype);
    {
        AutoLock lock(mHLock);
        // try {
        return mService->GetLastInputMethodSubtype(subtype);
        // } catch (RemoteException e) {
        //     Log.w(TAG, "IME died: " + mCurId, e);
        //     return null;
        // }
    }
}

void CInputMethodManager::HandleBind(
    /* [in] */ IInputBindResult* res)
{
    assert(res != NULL);
    {
        AutoLock lock(mHLock);

        Int32 sequence;
        res->GetSequence(&sequence);
        if (mBindSequence < 0 || mBindSequence != sequence) {
            // Log.w(TAG, "Ignoring onBind: cur seq=" + mBindSequence
            //         + ", given seq=" + res.sequence);
            return;
        }

        mCurMethod = NULL;
        res->GetIIMSession((IIInputMethodSession**)&mCurMethod);
        res->GetId(&mCurId);
        mBindSequence = sequence;
    }
    StartInputInner(NULL, 0, 0, 0);
}

void CInputMethodManager::HandleUnBind(
    /* [in] */ Int32 sequence)
{
    Boolean startInput = FALSE;
    AutoLock lock(mHLock);

    if (mBindSequence == sequence) {
        if (FALSE) {
            assert(0);
            // XXX the server has already unbound!
            // if (mCurMethod != NULL && mCurrentTextBoxAttribute != NULL) {
            //     try {
            //         mCurMethod.finishInput();
            //     } catch (RemoteException e) {
            //         Log.w(TAG, "IME died: " + mCurId, e);
            //     }
            // }
        }
        ClearBindingLocked();

        // If we were actively using the last input method, then
        // we would like to re-connect to the next input method.
        Boolean focused = FALSE;
        if (mServedView != NULL && (mServedView->IsFocused(&focused), focused)) {
            mServedConnecting = TRUE;
        }
        if (mActive) {
            startInput = TRUE;
        }
    }
    if (startInput) {
        StartInputInner(NULL, 0, 0, 0);
    }
}

void CInputMethodManager::HandleSetActive(
    /* [in] */ Boolean active)
{
    AutoLock lock(mHLock);

    mActive = active;
    mFullscreenMode = FALSE;
    if (!active) {
        // Some other client has starting using the IME, so note
        // that this happened and make sure our own editor's
        // state is reset.
        mHasBeenInactive = TRUE;
        // try {
        // Note that finishComposingText() is allowed to run
        // even when we are not active.
        mIInputContext->FinishComposingText();
        // } catch (RemoteException e) {
        // }
        // Check focus again in case that "onWindowFocus" is called before
        // handling this message.
        Boolean hasFocus = FALSE;
        if (mServedView != NULL && (mServedView->HasWindowFocus(&hasFocus), hasFocus)) {
            // "finishComposingText" has been already called above. So we
            // should not call mServedInputConnection.finishComposingText here.
            // Also, please note that this handler thread could be different
            // from a thread that created mServedView. That could happen
            // the current activity is running in the system process.
            // In that case, we really should not call
            // mServedInputConnection.finishComposingText.
            if (CheckFocusNoStartInput(mHasBeenInactive, FALSE)) {
                StartInputInner(NULL, 0, 0, 0);
            }
        }
    }
}

void CInputMethodManager::HandleEventTimeout(
    /* [in] */ Int32 seq)
{
    TimeoutEvent(seq);
}

} // namespace InputMethod
} // namespace View
} // namespace Droid
} // namespace Elastos
