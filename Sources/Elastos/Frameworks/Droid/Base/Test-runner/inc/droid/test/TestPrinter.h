
#ifndef __TESTPRINTER_H__
#define __TESTPRINTER_H__

#include <ext/frameworkdef.h>
#include "Elastos.Droid.Test_server.h"
#include <elastos/HashSet.h>

using Elastos::Utility::HashSet;
using Elastos::Utility::IList;
using Eunit::Framework::ITest;
using Eunit::Framework::ITestListener;
using Eunit::Framework::ITestFailure;

namespace Elastos {
namespace Droid {
namespace Test {

class TestPrinter
    : public ElRefBase
    , public ITestListener
    , public ITestRunnerListener
{
public:
    TestPrinter(
        /* [in] */ const String& tag,
        /* [in] */ Boolean onlyFailures);

    CAR_INTERFACE_DECL()

    CARAPI Started(
        /* [in] */ const String& className);

    CARAPI Finished(
        /* [in] */ const String& className);

    CARAPI Performance(
        /* [in] */ const String& className,
        /* [in] */ Int64 itemTimeNS,
        /* [in] */ Int32 iterations,
        /* [in] */ IList* itermediates);

    CARAPI Passed(
        /* [in] */ const String& className);

    CARAPI Failed(
        /* [in] */ const String& className,
        /* [in] */ ITestFailure* failure);

    CARAPI AddError(
        /* [in] */ ITest* test,
        /* [in] */ ECode ec);

    CARAPI AddFailure(
        /* [in] */ ITest* test,
        /* [in] */ ITestFailure* failure);

    CARAPI EndTest(
        /* [in] */ ITest* test);

    CARAPI StartTest(
        /* [in] */ ITest* test);

private:
    CARAPI_(void) Failed(
        /* [in] */ ITest* test,
        /* [in] */ ITestFailure* failure);

private:
    String mTag;
    Boolean mOnlyFailures;
    HashSet<String> mFailedTests;
};

} // namespace Test
} // namespace Droid
} // namespace Elastos

#endif //__TESTPRINTER_H__
