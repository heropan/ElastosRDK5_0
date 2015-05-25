
#ifndef __CSTRUCTTIMEVAL_H__
#define __CSTRUCTTIMEVAL_H__

#include "_CStructTimeval.h"

namespace Libcore {
namespace IO {

/**
 * Corresponds to C's {@code struct timeval} from
 * <a href="http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/sys_time.h.html">&lt;sys/time.h&gt;</a>
 */
CarClass(CStructTimeval)
{
public:
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
    Int64 tv_sec;

    /** Microseconds. */
    Int64 tv_usec;
};

} // namespace IO
} // namespace Libcore

#endif // __CSTRUCTTIMEVAL_H__
