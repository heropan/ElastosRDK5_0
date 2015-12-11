#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/widget/DoubleDigitManager.h"
#include "elastos/droid/os/CHandler.h"

using Elastos::Core::CInteger32;
using Elastos::Droid::Os::CHandler;

namespace Elastos {
namespace Droid {
namespace Widget {

//===============================================================
//        DoubleDigitManager::
//===============================================================
CAR_INTERFACE_IMPL(DoubleDigitManager, Object, IDoubleDigitManager)

DoubleDigitManager::DoubleDigitManager()
{
}

ECode DoubleDigitManager::constructor(
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
    if (mIntermediateDigit == NULL) {
        CInteger32::New(digit, (IInteger32**)&mIntermediateDigit);

        AutoPtr<IHandler> handler;
        CHandler::New((IHandler**)&handler);
        AutoPtr<IRunnable> r = new DoubleDigitManagerRunnable(this);
        Boolean post = FALSE;
        handler->PostDelayed(r, mTimeoutInMillins, &post);

        Boolean res = FALSE;
        mCallBack->SingleDigitIntermediate(digit, &res);
        if (!res) {

            // this wasn't a good candidate for the intermediate digit,
            // make it the final digit (since there is no opportunity to
            // reject the final digit).
            mIntermediateDigit = NULL;
            mCallBack->SingleDigitFinal(digit);
        }
    }
    else {
        Int32 intValue = 0;
        mIntermediateDigit->GetValue(&intValue);
        Boolean resault = FALSE;
        mCallBack->TwoDigitsFinal(intValue, digit, &resault);
        if (resault)
            mIntermediateDigit = NULL;
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
    if (mHost->mIntermediateDigit != NULL) {
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

