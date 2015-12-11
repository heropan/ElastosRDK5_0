
#include "elastos/droid/server/am/AppErrorResult.h"

namespace Elastos {
namespace Droid {
namespace Server {
namespace Am {

// 6ddb165e-4a71-44dd-abd1-cac05cc32b0d
extern "C" const InterfaceID EIID_AppErrorResult =
    { 0x6ddb165e, 0x4a71, 0x44dd, { 0xab, 0xd1, 0xca, 0xc0, 0x5c, 0xc3, 0x2b, 0x0d } };

CAR_INTERFACE_IMPL_WITH_CPP_CAST(AppErrorResult, IInterface, AppErrorResult)

AppErrorResult::AppErrorResult()
    : mHasResult(FALSE)
    , mResult(0)
{}

void AppErrorResult::SetResult(
    /* [in] */ Int32 res)
{
    AutoLock lock(this);
    mHasResult = TRUE;
    mResult = res;
    NotifyAll();
}

Int32 AppErrorResult::GetResult()
{
    AutoLock lock(this);
    while (!mHasResult) {
        // try {
        Wait();
        // } catch (InterruptedException e) {
        // }
    }

    return mResult;
}

} // namespace Am
} // namespace Server
} // namespace Droid
} // namespace Elastos
