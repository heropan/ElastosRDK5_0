
#ifndef __CTESTFAILURE_H__
#define __CTESTFAILURE_H__

#include "_CTestFailure.h"

namespace Eunit {
namespace Framework {

CarClass(CTestFailure)
{
public:
    CARAPI constructor(
        /* [in] */ ITest* failedTest,
        /* [in] */ ECode ec,
        /* [in] */ const String& message,
        /* [in] */ const String& backtrace);

    /**
     * Gets the failed test.
     */
    CARAPI FailedTest(
        /* [out] */ ITest** test);

    /**
     * Gets the thrown exception.
     */
    CARAPI ThrownException(
        /* [out] */ ECode* ec);

    CARAPI Trace(
        /* [out] */ String* traceStr);

    CARAPI ExceptionMessage(
        /* [out] */ String* message);

    CARAPI IsFailure(
        /* [out] */ Boolean* result);

protected:
    AutoPtr<IWeakReference> mFailedTest;
    ECode mErrorCode;
    String mMessage;
    String mBacktrace;
};

} // namespace Framework
} // namespace Eunit

#endif //__CTESTFAILURE_H__
