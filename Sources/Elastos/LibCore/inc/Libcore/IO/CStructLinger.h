
#ifndef __CSTRUCTLINGER_H__
#define __CSTRUCTLINGER_H__

#include "_CStructLinger.h"

namespace Libcore {
namespace IO {

/**
 * Corresponds to C's {@code struct linger} from
 * <a href="http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/sys_socket.h.html">&lt;sys/socket.h&gt;</a>
 */
CarClass(CStructLinger)
{
public:
    CARAPI GetOnoff(
        /* [out] */ Int32* onoff);

    CARAPI GetLinger(
        /* [out] */ Int32* linger);

    CARAPI IsOn(
        /* [out] */ Boolean* isOn);

    CARAPI constructor(
        /* [in] */ Int32 onoff,
        /* [in] */ Int32 linger);

private:
    /** Whether or not linger is enabled. Non-zero is on. */
    Int32 mOnoff;

    /** Linger time in seconds. */
    Int32 mLinger;
};

} // namespace IO
} // namespace Libcore

#endif // __CSTRUCTLINGER_H__
