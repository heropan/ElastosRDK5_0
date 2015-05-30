
#ifndef __CSTRUCTTIMEVAL_H__
#define __CSTRUCTTIMEVAL_H__

#include "coredef.h"
#include "core/Object.h"
#include "_Elastos_Droid_System_CStructTimeval.h"

using Elastos::Core::Object;

namespace Elastos {
namespace Droid {
namespace System {

/**
 * Corresponds to C's {@code struct timeval} from
 * <a href="http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/sys_time.h.html">&lt;sys/time.h&gt;</a>
 */
CarClass(CStructTimeval) , public Object
{
public:
    CAR_OBJECT_DECL()

    CAR_INTERFACE_DECL()

    CARAPI GetSec(
        /* [out] */ Int64* sec);

    CARAPI GetUsec(
        /* [out] */ Int64* usec);

    CARAPI ToMillis(
        /* [out] */ Int64* millis);

    CARAPI constructor(
        /* [in] */ Int64 sec,
        /* [in] */ Int64 usec);

    static CARAPI FromMillis(
        /* [in] */ Int64 millis,
        /* [out] */ IStructTimeval** tv);

private:
    /** Seconds. */
    Int64 mTv_sec;

    /** Microseconds. */
    Int64 mTv_usec;
};

} // namespace System
} // namespace Droid
} // namespace Elastos

#endif // __CSTRUCTTIMEVAL_H__
