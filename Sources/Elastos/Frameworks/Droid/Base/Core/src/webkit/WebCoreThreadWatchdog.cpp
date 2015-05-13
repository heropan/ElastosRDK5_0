
#include "app/CAlertDialogBuilder.h"
#include "os/Looper.h"
#include "os/CMessageHelper.h"
#include "os/Process.h"
#include "webkit/WebCoreThreadWatchdog.h"
#include "webkit/CWebViewClassic.h"
#include "webkit/CWebViewCore.h"
#include "view/View.h"
#include "view/ViewRootImpl.h"
#include "R.h"

using Elastos::Core::EIID_IRunnable;
using Elastos::Core::Threading::Mutex;
using Elastos::Core::Threading::CThread;
using Elastos::Droid::App::IAlertDialog;
using Elastos::Droid::App::CAlertDialogBuilder;
using Elastos::Droid::App::IAlertDialogBuilder;
using Elastos::Droid::Content::EIID_IDialogInterfaceOnClickListener;
using Elastos::Droid::Content::EIID_IDialogInterfaceOnCancelListener;
using Elastos::Droid::Os::EIID_IHandler;
using Elastos::Droid::Os::IBinder;
using Elastos::Droid::Os::IMessage;
using Elastos::Droid::Os::IMessageHelper;
using Elastos::Droid::Os::CMessageHelper;
using Elastos::Droid::Os::Looper;
using Elastos::Droid::Os::Process;
using Elastos::Droid::View::EIID_IView;
using Elastos::Droid::View::EIID_View;
using Elastos::Droid::View::ViewRootImpl;
using Elastos::Utility::HashSet;
using Elastos::Utility::Iterator;

namespace Elastos {
namespace Droid {
namespace Webkit {

//=====================================================================================================
//       WebCoreThreadWatchdog::PageNotRespondingRunnable::MyDialogInterfaceOnClickListenerClose
//=====================================================================================================

WebCoreThreadWatchdog::PageNotRespondingRunnable::MyDialogInterfaceOnClickListenerClose::MyDialogInterfaceOnClickListenerClose(
    /* [in] */ PageNotRespondingRunnable* owner)
    : mOwner(owner)
{
}

CAR_INTERFACE_IMPL(WebCoreThreadWatchdog::PageNotRespondingRunnable::MyDialogInterfaceOnClickListenerClose, IDialogInterfaceOnClickListener);

ECode WebCoreThreadWatchdog::PageNotRespondingRunnable::MyDialogInterfaceOnClickListenerClose::OnClick(
    /* [in] */ IDialogInterface* dialog,
    /* [in] */ Int32 which)
{
    // User chose to force close.
    Process::KillProcess(Process::MyPid());
    return NOERROR;
}

//=====================================================================================================
//        WebCoreThreadWatchdog::PageNotRespondingRunnable::MyDialogInterfaceOnClickListenerWait
//=====================================================================================================

WebCoreThreadWatchdog::PageNotRespondingRunnable::MyDialogInterfaceOnClickListenerWait::MyDialogInterfaceOnClickListenerWait(
    /* [in] */ PageNotRespondingRunnable* owner)
    : mOwner(owner)
{
}

CAR_INTERFACE_IMPL(WebCoreThreadWatchdog::PageNotRespondingRunnable::MyDialogInterfaceOnClickListenerWait, IDialogInterfaceOnClickListener);

ECode WebCoreThreadWatchdog::PageNotRespondingRunnable::MyDialogInterfaceOnClickListenerWait::OnClick(
    /* [in] */ IDialogInterface* dialog,
    /* [in] */ Int32 which)
{
    // The user chose to wait. The last HEARTBEAT message
    // will still be in the WebCore thread's queue, so all
    // we need to do is post another TIMED_OUT so that the
    // user will get prompted again if the WebCore thread
    // doesn't sort itself out.

    AutoPtr<IMessageHelper> msgHelper;
    AutoPtr<IMessage> msg;

    CMessageHelper::AcquireSingleton((IMessageHelper**)&msgHelper);
    msgHelper->ObtainEx3(mOwner->mWatchdogHandler, TIMED_OUT, (IMessage**)&msg);

    Boolean bResult = FALSE;
    return mOwner->mWatchdogHandler->SendMessageDelayed(msg, SUBSEQUENT_TIMEOUT_PERIOD, &bResult);
}

//=====================================================================================================
//         WebCoreThreadWatchdog::PageNotRespondingRunnable::MyDialogInterfaceOnCancelListener
//=====================================================================================================

WebCoreThreadWatchdog::PageNotRespondingRunnable::MyDialogInterfaceOnCancelListener::MyDialogInterfaceOnCancelListener(
    /* [in] */ PageNotRespondingRunnable* owner)
    : mOwner(owner)
{
}

CAR_INTERFACE_IMPL(WebCoreThreadWatchdog::PageNotRespondingRunnable::MyDialogInterfaceOnCancelListener, IDialogInterfaceOnCancelListener);

ECode WebCoreThreadWatchdog::PageNotRespondingRunnable::MyDialogInterfaceOnCancelListener::OnCancel(
    /* [in] */ IDialogInterface* dialog)
{
    AutoPtr<IMessageHelper> msgHelper;
    AutoPtr<IMessage> msg;

    CMessageHelper::AcquireSingleton((IMessageHelper**)&msgHelper);
    msgHelper->ObtainEx3(mOwner->mWatchdogHandler, TIMED_OUT, (IMessage**)&msg);

    Boolean bResult = FALSE;
    return mOwner->mWatchdogHandler->SendMessageDelayed(msg, SUBSEQUENT_TIMEOUT_PERIOD, &bResult);
}

//===============================================================
//       WebCoreThreadWatchdog::PageNotRespondingRunnable
//===============================================================

WebCoreThreadWatchdog::PageNotRespondingRunnable::PageNotRespondingRunnable(
    /* [in] */ WebCoreThreadWatchdog* host,
    /* [in] */ IContext* context,
    /* [in] */ IHandler* watchdogHandler)
{}

ECode WebCoreThreadWatchdog::PageNotRespondingRunnable::Run()
{
    AutoPtr<IThread> thread;
    Looper::GetMainLooper()->GetThread((IThread**)&thread);
    assert(thread == Thread::GetCurrentThread());
    AutoPtr<IAlertDialogBuilder> alertDialogBuilder;
    CAlertDialogBuilder::New(mContext, (IAlertDialogBuilder**)&alertDialogBuilder);

    alertDialogBuilder->SetMessage(R::string::webpage_unresponsive);
    AutoPtr<IDialogInterfaceOnClickListener> listenerClose = new MyDialogInterfaceOnClickListenerClose(this);
    alertDialogBuilder->SetPositiveButton(R::string::force_close, listenerClose);
    AutoPtr<IDialogInterfaceOnClickListener> listenerWait = new MyDialogInterfaceOnClickListenerWait(this);
    alertDialogBuilder->SetNegativeButton(R::string::wait, listenerWait);
    AutoPtr<IDialogInterfaceOnCancelListener> cancelListener = new MyDialogInterfaceOnCancelListener(this);
    alertDialogBuilder->SetOnCancelListener(cancelListener);
    alertDialogBuilder->SetIcon(R::drawable::ic_dialog_alert);
    AutoPtr<IAlertDialog> alertDialog;
    alertDialogBuilder->Show((IAlertDialog**)&alertDialog);

    return NOERROR;
}

//===============================================================
//          WebCoreThreadWatchdog::InnerHandler
//===============================================================

WebCoreThreadWatchdog::InnerHandler::InnerHandler(
    /* [in] */ WebCoreThreadWatchdog* owner)
    : mOwner(owner)
{
}

ECode WebCoreThreadWatchdog::InnerHandler::HandleMessage(
    /* [in] */ IMessage* msg)
{
    AutoPtr<IMessageHelper> msgHelper;
    CMessageHelper::AcquireSingleton((IMessageHelper**)&msgHelper);

    Int32 what;
    msg->GetWhat(&what);
    switch (what) {
    case IS_ALIVE: {
        Mutex::Autolock lock(sLock);
            if (mOwner->mPaused) {
                return NOERROR;
            }

            // The WebCore thread still seems alive. Reset the countdown timer.
            RemoveMessages(TIMED_OUT);

            AutoPtr<IMessage> msg1, msg2, msg3;

            Boolean bResult = FALSE;
            msgHelper->ObtainEx3(this, TIMED_OUT, (IMessage**)&msg1);
            SendMessageDelayed(msg1, TIMEOUT_PERIOD, &bResult);

            msgHelper->ObtainEx3(mOwner->mHandler, IS_ALIVE, (IMessage**)&msg2);
            msgHelper->ObtainEx4(mOwner->mWebCoreThreadHandler, CWebViewCore::EventHub::HEARTBEAT, msg2, (IMessage**)&msg3);
            mOwner->mWebCoreThreadHandler->SendMessageDelayed(msg3, HEARTBEAT_PERIOD, &bResult);
        }
        break;

    case TIMED_OUT: {
        Boolean postedDialog = FALSE;
            //synchronized (WebCoreThreadWatchdog.class)
            {
                Mutex::Autolock lock(sLock);

                HashSet< AutoPtr<CWebViewClassic> >::Iterator iter = mOwner->mWebViews.Begin();
                // Check each WebView we are aware of and find one that is capable of
                // showing the user a prompt dialog.
                while (iter != mOwner->mWebViews.End()) {
                    ++iter;
                    AutoPtr<IWebView> activeView;
                    (*iter)->GetWebView((IWebView**)&activeView);

                    AutoPtr<IBinder> binder;
                    Elastos::Droid::View::View* view;
                    AutoPtr<IView> iview;
                    activeView->GetWindowToken((IBinder**)&binder);
                    if (binder != NULL &&
                            (iview = (IView*)activeView->Probe(EIID_IView),
                             view = (Elastos::Droid::View::View*)iview->Probe(EIID_View),
                             view->GetViewRootImpl() != NULL)) {

                        AutoPtr<IContext> context;
                        activeView->GetContext((IContext**)&context);
                        AutoPtr<PageNotRespondingRunnable> pageNotRespondingRunnable = new PageNotRespondingRunnable(this->mOwner, context, this);

                        AutoPtr<IView> _iview = (IView*)activeView->Probe(EIID_IView);
                        Elastos::Droid::View::View* _view = (Elastos::Droid::View::View*)_iview->Probe(EIID_View);

                        postedDialog = _view->Post(pageNotRespondingRunnable);

                        if (postedDialog) {
                            // We placed the message into the UI thread for an attached
                            // WebView so we've made our best attempt to display the
                            // "page not responding" dialog to the user. Although the
                            // message is in the queue, there is no guarantee when/if
                            // the runnable will execute. In the case that the runnable
                            // never executes, the user will need to terminate the
                            // process manually.
                            break;
                        }
                    }
                }

                if (!postedDialog) {
                    // There's no active webview we can use to show the dialog, so
                    // wait again. If we never get a usable view, the user will
                    // never get the chance to terminate the process, and will
                    // need to do it manually.
                    AutoPtr<IMessage> msg;
                    msgHelper->ObtainEx3(this, TIMED_OUT, (IMessage**)&msg);
                    Boolean bResult = FALSE;
                    SendMessageDelayed(msg,
                            SUBSEQUENT_TIMEOUT_PERIOD, &bResult);
                }
            }
        }
        break;
    }

    return NOERROR;
}

//===============================================================
//                     WebCoreThreadWatchdog
//===============================================================

AutoPtr<WebCoreThreadWatchdog> WebCoreThreadWatchdog::sInstance;
AutoPtr<IThread> WebCoreThreadWatchdog::sInstanceThread;

// A message with this id is sent by the WebCore thread to notify the
// Watchdog that the WebCore thread is still processing messages
// (i.e. everything is OK).
const Int32 WebCoreThreadWatchdog::IS_ALIVE;

// This message is placed in the Watchdog's queue and removed when we
// receive an IS_ALIVE. If it is ever processed, we consider the
// WebCore thread unresponsive.
const Int32 WebCoreThreadWatchdog::TIMED_OUT;

// Wait 10s after hearing back from the WebCore thread before checking it's still alive.
const Int32 WebCoreThreadWatchdog::HEARTBEAT_PERIOD;

// If there's no callback from the WebCore thread for 30s, prompt the user the page has
// become unresponsive.
const Int32 WebCoreThreadWatchdog::TIMEOUT_PERIOD;

// After the first timeout, use a shorter period before re-prompting the user.
const Int32 WebCoreThreadWatchdog::SUBSEQUENT_TIMEOUT_PERIOD;

Mutex WebCoreThreadWatchdog::sLock;

WebCoreThreadWatchdog::WebCoreThreadWatchdog(
    /* [in] */ IHandler* webCoreThreadHandler)
    : mPaused(FALSE)
    , mWebCoreThreadHandler(webCoreThreadHandler)
{}

//synchronized
AutoPtr<WebCoreThreadWatchdog> WebCoreThreadWatchdog::Start(
    /* [in] */ IHandler* webCoreThreadHandler)
{
    Mutex::Autolock lock(sLock);
    if (sInstance == NULL) {
        sInstance = new WebCoreThreadWatchdog(webCoreThreadHandler);
        if (sInstanceThread == NULL) {
            CThread::New((IRunnable*)sInstance->Probe(EIID_IRunnable), String("WebCoreThreadWatchdog"), (IThread**)&sInstanceThread);
        }

        sInstanceThread->Start();
    }
    return sInstance;
}

//synchronized
void WebCoreThreadWatchdog::RegisterWebView(
    /* [in] */ CWebViewClassic* w)
{
    Mutex::Autolock lock(sLock);
    if (sInstance != NULL) {
        sInstance->AddWebView(w);
    }
}

//synchronized
void WebCoreThreadWatchdog::UnregisterWebView(
    /* [in] */ CWebViewClassic* w)
{
    Mutex::Autolock lock(sLock);
    if (sInstance != NULL) {
        sInstance->RemoveWebView(w);
    }
}

//synchronized
void WebCoreThreadWatchdog::Pause()
{
    Mutex::Autolock lock(sLock);
    if (sInstance != NULL) {
        sInstance->PauseWatchdog();
    }
}

//synchronized
void WebCoreThreadWatchdog::Resume()
{
    Mutex::Autolock lock(sLock);
    if (sInstance != NULL) {
        sInstance->ResumeWatchdog();
    }
}

ECode WebCoreThreadWatchdog::Run()
{
    Looper::Prepare();

    CreateHandler();

    // Send the initial control to WebViewCore and start the timeout timer as long as we aren't
    // paused.
    // synchronized (WebCoreThreadWatchdog.class)
    {
        Mutex::Autolock lock(sLock);
        if (!mPaused) {
            AutoPtr<IMessageHelper> msgHelper;
            AutoPtr<IMessage> msg1, msg2, msg3;

            CMessageHelper::AcquireSingleton((IMessageHelper**)&msgHelper);
            msgHelper->ObtainEx3(mHandler, IS_ALIVE, (IMessage**)&msg1);
            msgHelper->ObtainEx4(mWebCoreThreadHandler, CWebViewCore::EventHub::HEARTBEAT, msg1, (IMessage**)&msg2);
            msg2->SendToTarget();

            Boolean bResult = FALSE;
            msgHelper->ObtainEx3(mHandler, TIMED_OUT, (IMessage**)&msg3);
            mHandler->SendMessageDelayed(msg3, TIMEOUT_PERIOD, &bResult);
        }
    }

    Looper::Loop();

    return NOERROR;
}

void WebCoreThreadWatchdog::AddWebView(
    /* [in] */ CWebViewClassic* w)
{
    mWebViews.Insert(w);
}

void WebCoreThreadWatchdog::RemoveWebView(
    /* [in] */ CWebViewClassic* w)
{
    mWebViews.Erase(w);
}

void WebCoreThreadWatchdog::PauseWatchdog()
{
    mPaused = TRUE;

    if (mHandler == NULL) {
        return;
    }

    mHandler->RemoveMessages(TIMED_OUT);
    mHandler->RemoveMessages(IS_ALIVE);
    mWebCoreThreadHandler->RemoveMessages(CWebViewCore::EventHub::HEARTBEAT);
}

void WebCoreThreadWatchdog::ResumeWatchdog()
{
    if (!mPaused) {
        // Do nothing if we get a call to resume without being paused.
        // This can happen during the initialisation of the WebView.
        return;
    }

    mPaused = FALSE;

    if (mHandler == NULL) {
        return;
    }

    AutoPtr<IMessageHelper> msgHelper;
    AutoPtr<IMessage> msg1, msg2, msg3;

    CMessageHelper::AcquireSingleton((IMessageHelper**)&msgHelper);
    msgHelper->ObtainEx3(mHandler, IS_ALIVE, (IMessage**)&msg1);
    msgHelper->ObtainEx4(mWebCoreThreadHandler, CWebViewCore::EventHub::HEARTBEAT, msg1, (IMessage**)&msg2);
    msg2->SendToTarget();

    Boolean bResult = FALSE;
    msgHelper->ObtainEx3(mHandler, TIMED_OUT, (IMessage**)&msg3);
    mHandler->SendMessageDelayed(msg3, TIMEOUT_PERIOD, &bResult);
}

void WebCoreThreadWatchdog::CreateHandler()
{
    Mutex::Autolock lock(sLock);
    mHandler = new InnerHandler(this);
}

} // namespace Webkit
} // namepsace Droid
} // namespace Elastos