
#ifndef __ELASTOS_DROID_WEBKIT_WEBCORETHREADWATCHDOG_H__
#define __ELASTOS_DROID_WEBKIT_WEBCORETHREADWATCHDOG_H__

#include "ext/frameworkext.h"
#include "os/Runnable.h"
#include <elastos/utility/etl/HashSet.h>

#include "webkit/CWebViewClassic.h"

using Elastos::Core::IRunnable;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::IDialogInterface;
using Elastos::Droid::Content::IDialogInterfaceOnCancelListener;
using Elastos::Droid::Content::IDialogInterfaceOnClickListener;
using Elastos::Droid::Os::Runnable;
using Elastos::Droid::Os::HandlerBase;
using Elastos::Core::IThread;
using Elastos::Droid::View::IView;
using Elastos::Droid::Webkit::CWebViewClassic;

_ETL_NAMESPACE_BEGIN

template<> struct Hash< AutoPtr<CWebViewClassic> >
{
    size_t operator() (AutoPtr<CWebViewClassic> x) const { return (size_t)x.Get(); }
};

template<> struct EqualTo< AutoPtr<CWebViewClassic> >
{
    Boolean operator()(const AutoPtr<CWebViewClassic>& x,
                       const AutoPtr<CWebViewClassic>& y) const
    {
        return x.Get() == y.Get();
    }
};

_ETL_NAMESPACE_END

namespace Elastos {
namespace Droid {
namespace Webkit {

// A Runnable that will monitor if the WebCore thread is still
// processing messages by pinging it every so often. It is safe
// to call the public methods of this class from any thread.
class WebCoreThreadWatchdog : public Runnable
{
private:
    class PageNotRespondingRunnable : public Runnable
    {
    private:
        class MyDialogInterfaceOnClickListenerClose
            : public ElRefBase
            , public IDialogInterfaceOnClickListener
        {
        public:
            MyDialogInterfaceOnClickListenerClose(
                /* [in] */ PageNotRespondingRunnable* owner);

            CAR_INTERFACE_DECL();

            CARAPI OnClick(
                /* [in] */ IDialogInterface* dialog,
                /* [in] */ Int32 which);

        private:
            PageNotRespondingRunnable* mOwner;
        };

        class MyDialogInterfaceOnClickListenerWait
            : public ElRefBase
            , public IDialogInterfaceOnClickListener
        {
        public:
            MyDialogInterfaceOnClickListenerWait(
                /* [in] */ PageNotRespondingRunnable* owner);

            CAR_INTERFACE_DECL();

            CARAPI OnClick(
                /* [in] */ IDialogInterface* dialog,
                /* [in] */ Int32 which);

        private:
            PageNotRespondingRunnable* mOwner;
        };

        class MyDialogInterfaceOnCancelListener
            : public ElRefBase
            , public IDialogInterfaceOnCancelListener
        {
        public:
            MyDialogInterfaceOnCancelListener(
                /* [in] */ PageNotRespondingRunnable* owner);

            CAR_INTERFACE_DECL();

            CARAPI OnCancel(
                /* [in] */ IDialogInterface* dialog);

        private:
            PageNotRespondingRunnable* mOwner;
        };

    public:
        PageNotRespondingRunnable(
            /* [in] */ WebCoreThreadWatchdog* host,
            /* [in] */ IContext* context,
            /* [in] */ IHandler* watchdogHandler);

        CARAPI Run();

    public:
        AutoPtr<IContext> mContext;

    private:
        AutoPtr<WebCoreThreadWatchdog> mHost;
        AutoPtr<IHandler> mWatchdogHandler;
    };

    class InnerHandler : public HandlerBase
    {
    public:
        InnerHandler(
            /* [in] */ WebCoreThreadWatchdog* owner);

        CARAPI HandleMessage(
            /* [in] */ IMessage* msg);

    private:
        WebCoreThreadWatchdog* mOwner;
    };

public:
    //synchronized
    static CARAPI_(AutoPtr<WebCoreThreadWatchdog>) Start(
        /* [in] */ IHandler* webCoreThreadHandler);

    //synchronized
    static CARAPI_(void) RegisterWebView(
        /* [in] */ CWebViewClassic* w);

    //synchronized
    static CARAPI_(void) UnregisterWebView(
        /* [in] */ CWebViewClassic* w);

    //synchronized
    static CARAPI_(void) Pause();

    //synchronized
    static CARAPI_(void) Resume();

    CARAPI Run();

private:
    CARAPI_(void) AddWebView(
        /* [in] */ CWebViewClassic* w);

    CARAPI_(void) RemoveWebView(
        /* [in] */ CWebViewClassic* w);

    WebCoreThreadWatchdog(
        /* [in] */ IHandler* webCoreThreadHandler);

    CARAPI_(void) PauseWatchdog();

    CARAPI_(void) ResumeWatchdog();

    CARAPI_(void) CreateHandler();

private:
    static AutoPtr<WebCoreThreadWatchdog> sInstance;
    static AutoPtr<IThread> sInstanceThread;

    // A message with this id is sent by the WebCore thread to notify the
    // Watchdog that the WebCore thread is still processing messages
    // (i.e. everything is OK).
    static const Int32 IS_ALIVE = 100;

    // This message is placed in the Watchdog's queue and removed when we
    // receive an IS_ALIVE. If it is ever processed, we consider the
    // WebCore thread unresponsive.
    static const Int32 TIMED_OUT = 101;

    // Wait 10s after hearing back from the WebCore thread before checking it's still alive.
    static const Int32 HEARTBEAT_PERIOD = 10 * 1000;

    // If there's no callback from the WebCore thread for 30s, prompt the user the page has
    // become unresponsive.
    static const Int32 TIMEOUT_PERIOD = 30 * 1000;

    // After the first timeout, use a shorter period before re-prompting the user.
    static const Int32 SUBSEQUENT_TIMEOUT_PERIOD = 15 * 1000;

    AutoPtr<IHandler> mWebCoreThreadHandler;
    AutoPtr<IHandler> mHandler;
    Boolean mPaused;

    HashSet< AutoPtr<CWebViewClassic> > mWebViews;

    static Object sLock;
};

} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_WEBKIT_WEBCORETHREADWATCHDOG_H__
