
#ifndef __CPROCESSSTARTRESULT_H__
#define __CPROCESSSTARTRESULT_H__

#include "_CProcessStartResult.h"

namespace Elastos {
namespace Droid {
namespace Os {

CarClass(CProcessStartResult)
{
public:
    CProcessStartResult();

    CARAPI GetPid(
        /* [out] */ Int32* pid);

    CARAPI SetPid(
        /* [in] */ Int32 pid);

    CARAPI GetUsingWrapper(
        /* [out] */ Boolean* usingWrapper);

    CARAPI SetUsingWrapper(
        /* [in] */ Boolean usingWrapper);

private:
    /**
     * The PID of the newly started process.
     * Always >= 0.  (If the start failed, an exception will have been thrown instead.)
     */
    Int32 mPid;

    /**
     * True if the process was started with a wrapper attached.
     */
    Boolean mUsingWrapper;
};

} // namespace Os
} // namespace Droid
} // namespace Elastos

#endif //__CPROCESSSTARTRESULT_H__
