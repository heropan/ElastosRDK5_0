
#ifndef __ELASTOS_DROID_NET_CTHROTTLEMANAGER_H__
#define __ELASTOS_DROID_NET_CTHROTTLEMANAGER_H__

#include "_CThrottleManager.h"

using Elastos::Droid::Net::IIThrottleManager;

namespace Elastos {
namespace Droid {
namespace Net {

CarClass(CThrottleManager)
{
public:
    CARAPI constructor(
        /* [in] */ IIThrottleManager* service);

    /**
     * returns a long of the ms from the epoch to the time the current cycle ends for the
     * named interface
     * {@hide}
     */
    CARAPI GetResetTime(
        /* [in] */ const String& iface,
        /* [out] */ Int64* result);

    /**
     * returns a long of the ms from the epoch to the time the current cycle started for the
     * named interface
     * {@hide}
     */
    CARAPI GetPeriodStartTime(
        /* [in] */ const String& iface,
        /* [out] */ Int64* result);

    /**
     * returns a long of the byte count either read or written on the named interface
     * for the period described.  Direction is either DIRECTION_RX or DIRECTION_TX and
     * period may only be PERIOD_CYCLE for the current cycle (other periods may be supported
     * in the future).  Ago indicates the number of periods in the past to lookup - 0 means
     * the current period, 1 is the last one, 2 was two periods ago..
     * {@hide}
     */
    CARAPI GetByteCount(
        /* [in] */ const String& iface,
        /* [in] */ Int32 direction,
        /* [in] */ Int32 period,
        /* [in] */ Int32 ago,
        /* [out] */ Int64* result);

    /**
     * returns the number of bytes read+written after which a particular cliff
     * takes effect on the named iface.  Currently only cliff #1 is supported (1 step)
     * {@hide}
     */
    CARAPI GetCliffThreshold(
        /* [in] */ const String& iface,
        /* [in] */ Int32 cliff,
        /* [out] */ Int64* result);

    /**
     * returns the thottling bandwidth (bps) for a given cliff # on the named iface.
     * only cliff #1 is currently supported.
     * {@hide}
     */
    CARAPI GetCliffLevel(
        /* [in] */ const String& iface,
        /* [in] */ Int32 cliff,
        /* [out] */ Int32* result);

    /**
     * returns the help URI for throttling
     * {@hide}
     */
    CARAPI GetHelpUri(
        /* [out] */ String* result);

private:
    // TODO: Add your private member variables here.
    AutoPtr<IIThrottleManager> mService;
};

} // namespace Net
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_NET_CTHROTTLEMANAGER_H__
