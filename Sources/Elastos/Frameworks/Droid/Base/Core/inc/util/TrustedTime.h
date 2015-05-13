#ifndef __ELASTOS_DROID_UTILITY_TRUSTEDTIME_H__
#define __ELASTOS_DROID_UTILITY_TRUSTEDTIME_H__

#include "ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Utility {

/**
 * Interface that provides trusted time information, possibly coming from an NTP
 * server. Implementations may cache answers until {@link #forceRefresh()}.
 *
 * @hide
 */
class TrustedTime
{
public:
    /**
     * Force update with an external trusted time source, returning {@code true}
     * when successful.
     */
    virtual Boolean ForceRefresh() = 0;

    /**
     * Check if this instance has cached a response from a trusted time source.
     */
    virtual Boolean HasCache() = 0;

    /**
     * Return time since last trusted time source contact, or
     * {@link Long#MAX_VALUE} if never contacted.
     */
    virtual Int64 GetCacheAge() = 0;

    /**
     * Return certainty of cached trusted time in milliseconds, or
     * {@link Long#MAX_VALUE} if never contacted. Smaller values are more
     * precise.
     */
    virtual Int64 GetCacheCertainty() = 0;

    /**
     * Return current time similar to {@link System#currentTimeMillis()},
     * possibly using a cached authoritative time source.
     */
    virtual Int64 CurrentTimeMillis() = 0;
};

} // namespace Utility
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_UTILITY_TRUSTEDTIME_H__
