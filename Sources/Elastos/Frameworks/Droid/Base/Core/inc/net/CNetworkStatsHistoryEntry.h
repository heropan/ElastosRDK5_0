
#ifndef __ELASTOS_DROID_NET_CNETWORKSTATSHISTORYENTRY_H__
#define __ELASTOS_DROID_NET_CNETWORKSTATSHISTORYENTRY_H__

#include "_CNetworkStatsHistoryEntry.h"
#include "ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Net {

CarClass(CNetworkStatsHistoryEntry)
{
public:

    CARAPI GetBucketDuration(
        /* [out] */ Int64* bucketDuration);

    CARAPI SetBucketDuration(
        /* [in] */ Int64 bucketDuration);

    CARAPI GetBucketStart(
        /* [out] */ Int64* bucketStart);

    CARAPI SetBucketStart(
        /* [in] */ Int64 bucketStart);

    CARAPI GetActiveTime(
        /* [out] */ Int64* activeTime);

    CARAPI SetActiveTime(
        /* [in] */ Int64 activeTime);

    CARAPI GetRxBytes(
        /* [out] */ Int64* rxBytes);

    CARAPI SetRxBytes(
        /* [in] */ Int64 rxBytes);

    CARAPI GetRxPackets(
        /* [out] */ Int64* rxPackets);

    CARAPI SetRxPackets(
        /* [in] */ Int64 rxPackets);

    CARAPI GetTxBytes(
        /* [out] */ Int64* txBytes);

    CARAPI SetTxBytes(
        /* [in] */ Int64 txBytes);

    CARAPI GetTxPackets(
        /* [out] */ Int64* txPackets);

    CARAPI SetTxPackets(
        /* [in] */ Int64 txPackets);

    CARAPI GetOperations(
        /* [out] */ Int64* operations);

    CARAPI SetOperations(
        /* [in] */ Int64 operations);
public:
    Int64 mBucketDuration;
    Int64 mBucketStart;
    Int64 mActiveTime;
    Int64 mRxBytes;
    Int64 mRxPackets;
    Int64 mTxBytes;
    Int64 mTxPackets;
    Int64 mOperations;
};

} // namespace Net
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_NET_CNETWORKSTATSHISTORYENTRY_H__

