#include "ext/frameworkext.h"
#include "widget/DoubleDigitManager.h"
#include "os/CHandler.h"

using Elastos::Core::CInteger32;
using Elastos::Droid::Os::CHandler;

namespace Elastos {
namespace Droid {
namespace Widget {

DoubleDigitManager::DoubleDigitManager()
{

}

DoubleDigitManager::DoubleDigitManager(
    /* [in] */ Int64 timeoutInMillis,
    /* [in] */ IDoubleDigitManagerCallBack* callBack)
{
    mTimeoutInMillins = timeoutInMillis;
    mCallBack = callBack;
}

ECode DoubleDigitManager::Init(
    /* [in] */ Int64 timeoutInMillis,
    /* [in] */ IDoubleDigitManagerCallBack* callBack)
{
    mTimeoutInMillins = timeoutInMillis;
    mCallBack = callBack;
    return NOERROR;
}

ECode DoubleDigitManager::ReportDigit(
    /* [in] */ Int32 digit)
{
    if (!mIntermediateDigit) {
        CInteger32::New(digit, (IInteger32**)&mIntermediateDigit);

        AutoPtr<IHandler> handler;
        CHandler::New((IHandler**)&handler);
        AutoPtr<IRunnable> r = new DoubleDigitManagerRunnable(this);
        Boolean post = FALSE;
        handler->PostDelayed(r, mTimeoutInMillins, &post);

        Boolean res = FALSE, resault = FALSE;

        mCallBack->SingleDigitIntermediate(digit, &res);
        Int32 intValue = 0;
        mIntermediateDigit->GetValue(&intValue);
        mCallBack->TwoDigitsFinal(intValue, digit, &resault);

        if (!res) {
            mIntermediateDigit = NULL;
            mCallBack->SingleDigitFinal(digit);
        } else if (resault) {
            mIntermediateDigit = NULL;
        }
    }
    return NOERROR;
}

//===============================================================
//        DoubleDigitManager::DoubleDigitManagerRunnable
//===============================================================
DoubleDigitManager::DoubleDigitManagerRunnable::DoubleDigitManagerRunnable(
    /* [in] */ DoubleDigitManager* host) :
    mHost(host)
{
}

ECode DoubleDigitManager::DoubleDigitManagerRunnable::Run()
{
    if (mHost->mIntermediateDigit) {
        Int32 value = 0;
        mHost->mIntermediateDigit->GetValue(&value);
        mHost->mCallBack->SingleDigitFinal(value);
        mHost->mIntermediateDigit = NULL;
    }
    return NOERROR;
}

} // namespace Widget
} // namespace Droid
} // namespace Elastos

