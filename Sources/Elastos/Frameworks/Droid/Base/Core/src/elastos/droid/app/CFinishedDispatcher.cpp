#include "app/CFinishedDispatcher.h"
#include "unistd.h"

namespace Elastos {
namespace Droid {
namespace App {

using Elastos::Core::EIID_IRunnable;
using Elastos::Droid::Content::EIID_IIntentReceiver;

CFinishedDispatcher::CFinishedDispatcher()
{}

CFinishedDispatcher::~CFinishedDispatcher()
{
    mWho = NULL;
    mHandler = NULL;
    mIntent = NULL;
    mResultExtras = NULL;
}

ECode CFinishedDispatcher::constructor(
    /* [in] */ IPendingIntent* pi,
    /* [in] */ IPendingIntentOnFinished* who,
    /* [in] */ IHandler* handler)
{
    mPendingIntent=pi;
    mWho=who;
    mHandler=handler;
    return NOERROR;
}

ECode CFinishedDispatcher::PerformReceive(
    /* [in] */ IIntent* intent,
    /* [in] */ Int32 resultCode,
    /* [in] */ const String&  data,
    /* [in] */ IBundle* extras,
    /* [in] */ Boolean ordered,
    /* [in] */ Boolean sticky,
    /* [in] */ Int32 sendingUser)
{
    mIntent = intent;
    mResultCode = resultCode;
    mResultData = data;
    mResultExtras = extras;

    Boolean result;
    if (NULL == mHandler) {
        FAIL_RETURN(Run());
    } else {
        mHandler->Post(IRunnable::Probe(this), &result);
    }
    return NOERROR;
}

ECode CFinishedDispatcher::Run()
{
    return mWho->OnSendFinished(mPendingIntent, mIntent, mResultCode, mResultData, mResultExtras);
}

ECode CFinishedDispatcher::ToString(
    /* [out] */ String* str)
{
    return NOERROR;
}

} // namespace App
} // namespace Droid
} // namespace Elastos
