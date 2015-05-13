
#include "eunit/runner/BaseTestRunner.h"

using Elastos::Utility::CProperties;
using Eunit::Framework::EIID_ITestListener;

namespace Eunit {
namespace Runner {

const String BaseTestRunner::SUITE_METHODNAME("suite");
AutoPtr<IProperties> BaseTestRunner::sPreferences;
Int32 BaseTestRunner::sMaxMessageLength = 500;
Boolean BaseTestRunner::sFilterStack = TRUE;

CAR_INTERFACE_IMPL_2(BaseTestRunner, ITestListener, IBaseTestRunner)

ECode BaseTestRunner::StartTest(
    /* [in] */ ITest* test)
{
    Mutex::Autolock lock(mLock);

    String name;
    IObject::Probe(test)->ToString(&name);
    return TestStarted(name);
}

void BaseTestRunner::SetPreferences(
    /* [in] */ IProperties* preferences)
{
    sPreferences = preferences;
}

AutoPtr<IProperties> BaseTestRunner::GetPreferences()
{
    if (sPreferences == NULL) {
        CProperties::New((IProperties**)&sPreferences);
        assert(0);
        // fPreferences.put("loading", "true");
        // fPreferences.put("filterstack", "true");
        ReadPreferences();
    }
    return sPreferences;
}

} // namespace Runner
} // namespace Eunit
