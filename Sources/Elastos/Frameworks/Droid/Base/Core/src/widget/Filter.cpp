
#include "widget/Filter.h"
#include "os/SystemClock.h"
#ifdef DROID_CORE
#include "os/CHandlerThread.h"
#endif
using Elastos::Core::CStringWrapper;
using Elastos::Droid::Os::IHandlerThread;
using Elastos::Droid::Os::CHandlerThread;
using Elastos::Droid::Os::IProcess;

namespace Elastos {
namespace Droid {
namespace Widget {


const String Filter::TAG("Filter");
const String Filter::THREAD_NAME = String("Filter");
const Int32 Filter::FILTER_TOKEN;
const Int32 Filter::FINISH_TOKEN;

//==============================================================================
//              Filter::FilterResults
//==============================================================================
CAR_INTERFACE_IMPL(Filter::FilterResults, IFilterResults);

ECode Filter::FilterResults::SetCount(
    /* [in] */ Int32 count)
{
    mCount = count;
    return NOERROR;
}

ECode Filter::FilterResults::GetCount(
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count);
    *count = mCount;
    return NOERROR;
}

ECode Filter::FilterResults::SetValues(
    /* [in] */ IInterface* values)
{
    mValues = values;
    return NOERROR;
}

ECode Filter::FilterResults::GetValues(
    /* [out] */ IInterface** values)
{
    VALIDATE_NOT_NULL(values);
    *values = mValues;
    REFCOUNT_ADD(*values);
    return NOERROR;
}

//==============================================================================
//              Filter::RequestArguments
//==============================================================================
CAR_INTERFACE_IMPL(Filter::RequestArguments, IInterface)

//==============================================================================
//              Filter::RequestHandler
//==============================================================================

Filter::RequestHandler::RequestHandler(
    /* [in] */ ILooper* looper,
    /* [in] */ Filter* host)
    : HandlerBase(looper)
    , mHost(host)
{
}

/**
 * <p>Messages received from the request handler are processed in the
 * UI thread. The processing involves calling
 * {@link Filter#publishResults(CharSequence,
 * android.widget.Filter.FilterResults)}
 * to post the results back in the UI and then notifying the listener,
 * if any.</p>
 *
 * @param msg the filtering results
 */
ECode Filter::RequestHandler::HandleMessage(
    /* [in] */ IMessage* msg)
{
    Int32 what;
    msg->GetWhat(&what);
    AutoPtr<IInterface> obj;
    msg->GetObj((IInterface**)&obj);

    switch (what) {
        case Filter::FILTER_TOKEN: {
            RequestArguments* args = (RequestArguments*)obj.Get();
            mHost->HandleFilterMessage(args);
            break;
        }
        case Filter::FINISH_TOKEN:
            mHost->HandleFinishMessage();
            break;
    }

    return NOERROR;
}

//==============================================================================
//              Filter::ResultsHandler
//==============================================================================
Filter::ResultsHandler::ResultsHandler(
    /* [in] */ Filter* host)
    : mHost(host)
{
}

ECode Filter::ResultsHandler::HandleMessage(
    /* [in] */ IMessage* msg)
{
    AutoPtr<IInterface> obj;
    msg->GetObj((IInterface**)&obj);
    RequestArguments* args = (RequestArguments*)obj.Get();
    mHost->HandleResultsMessage(args);
    return NOERROR;
}

//==============================================================================
//              Filter
//==============================================================================

Filter::Filter()
{
    mResultHandler = new ResultsHandler(this);
}

ECode Filter::SetDelayer(
    /* [in] */ IFilterDelayer* delayer)
{
    AutoLock lock(mLock);
    mDelayer = delayer;
    return NOERROR;
}

ECode Filter::DoFilter(
    /* [in] */ ICharSequence* constraint)
{
    return DoFilter(constraint, NULL);
}

ECode Filter::DoFilter(
    /* [in] */ ICharSequence* constraint,
    /* [in] */ IFilterListener* listener)
{
    AutoLock lock(mLock);

    if (mThreadHandler == NULL) {
        AutoPtr<IHandlerThread> thread;
        CHandlerThread::New(THREAD_NAME, IProcess::THREAD_PRIORITY_BACKGROUND,
                (IHandlerThread**)&thread);
        thread->Start();

        AutoPtr<ILooper> looper;
        thread->GetLooper((ILooper**)&looper);
        mThreadHandler = new RequestHandler(looper, this);
    }

    Int64 delay = 0;
    if (mDelayer != NULL) {
        mDelayer->GetPostingDelay(constraint, &delay);
    }

    AutoPtr<RequestArguments> args = new RequestArguments();
    // make sure we use an immutable copy of the constraint, so that
    // it doesn't change while the filter operation is in progress
    //
    String str("");
    if (constraint) {
        constraint->ToString(&str);
    }
    CStringWrapper::New(str, (ICharSequence**)&args->mConstraint);
    args->mListener = listener;

    AutoPtr<IMessage> message;
    mThreadHandler->ObtainMessage(FILTER_TOKEN, args, (IMessage**)&message);

    Boolean result;
    mThreadHandler->RemoveMessages(FILTER_TOKEN);
    mThreadHandler->RemoveMessages(FINISH_TOKEN);
    mThreadHandler->SendMessageDelayed(message, delay, &result);
    return NOERROR;
}

ECode Filter::ConvertResultToString(
    /* [in] */ IInterface* resultValue,
    /* [out] */ ICharSequence** cs)
{
    VALIDATE_NOT_NULL(cs);
    String str;
    if (resultValue && ICharSequence::Probe(resultValue)) {
        ICharSequence::Probe(resultValue)->ToString(&str);
    }

    return CStringWrapper::New(str, cs);
}

ECode Filter::HandleFilterMessage(
    /* [in] */ RequestArguments* args)
{
    assert(args != NULL);
    AutoPtr<IFilterResults> results;
    ECode ec = PerformFiltering(args->mConstraint, (IFilterResults**)&results);
    if (FAILED(ec)) {
        args->mResults = new FilterResults();
    }
    else {
        args->mResults = results;
    }

    AutoPtr<IMessage> message;
    mResultHandler->ObtainMessage(FILTER_TOKEN, args, (IMessage**)&message);
    message->SendToTarget();

    AutoLock lock(mLock);
    if (mThreadHandler != NULL) {
        AutoPtr<IMessage> finishMessage;
        mThreadHandler->ObtainMessage(FINISH_TOKEN, (IMessage**)&finishMessage);
        Boolean result;
        mThreadHandler->SendMessageDelayed(finishMessage, 3000, &result);
    }

    return NOERROR;
}

ECode Filter::HandleFinishMessage()
{
    AutoLock lock(mLock);

    if (mThreadHandler != NULL) {
        AutoPtr<ILooper> looper;
        mThreadHandler->GetLooper((ILooper**)&looper);
        looper->Quit();
        mThreadHandler = NULL;
    }
    return NOERROR;
}

ECode Filter::HandleResultsMessage(
    /* [in] */ RequestArguments* args)
{
    assert(args != NULL);
    PublishResults(args->mConstraint, args->mResults);
    if (args->mListener != NULL) {
        Int32 count = -1;
        if (args->mResults != NULL) {
            args->mResults->GetCount(&count);
        }
        args->mListener->OnFilterComplete(count);
    }

    return NOERROR;
}

} // namespace Widget
} // namespace Droid
} // namespace Elastos
