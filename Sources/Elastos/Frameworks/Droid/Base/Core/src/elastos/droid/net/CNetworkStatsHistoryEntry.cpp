
#include "net/CNetworkStatsHistoryEntry.h"

namespace Elastos {
namespace Droid {
namespace Net {

ECode CNetworkStatsHistoryEntry::GetBucketDuration(
    /* [out] */ Int64* bucketDuration)
{
    VALIDATE_NOT_NULL(bucketDuration);
    *bucketDuration = mBucketDuration;
    return NOERROR;
}

ECode CNetworkStatsHistoryEntry::SetBucketDuration(
    /* [in] */ Int64 bucketDuration)
{
    mBucketDuration = bucketDuration;
    return NOERROR;
}

ECode CNetworkStatsHistoryEntry::GetBucketStart(
    /* [out] */ Int64* bucketStart)
{
    VALIDATE_NOT_NULL(bucketStart);
    *bucketStart = mBucketStart;
    return NOERROR;
}

ECode CNetworkStatsHistoryEntry::SetBucketStart(
    /* [in] */ Int64 bucketStart)
{
    mBucketStart = bucketStart;
    return NOERROR;
}

ECode CNetworkStatsHistoryEntry::GetActiveTime(
    /* [out] */ Int64* activeTime)
{
    VALIDATE_NOT_NULL(activeTime);
    *activeTime = mActiveTime;
    return NOERROR;
}

ECode CNetworkStatsHistoryEntry::SetActiveTime(
    /* [in] */ Int64 activeTime)
{
    mActiveTime = activeTime;
    return NOERROR;
}

ECode CNetworkStatsHistoryEntry::GetRxBytes(
    /* [out] */ Int64* rxBytes)
{
    VALIDATE_NOT_NULL(rxBytes);
    *rxBytes = mRxBytes;
    return NOERROR;
}

ECode CNetworkStatsHistoryEntry::SetRxBytes(
    /* [in] */ Int64 rxBytes)
{
    mRxBytes = rxBytes;
    return NOERROR;
}

ECode CNetworkStatsHistoryEntry::GetRxPackets(
    /* [out] */ Int64* rxPackets)
{
    VALIDATE_NOT_NULL(rxPackets);
    *rxPackets = mRxPackets;
    return NOERROR;
}

ECode CNetworkStatsHistoryEntry::SetRxPackets(
    /* [in] */ Int64 rxPackets)
{
    mRxPackets = rxPackets;
    return NOERROR;
}

ECode CNetworkStatsHistoryEntry::GetTxBytes(
    /* [out] */ Int64* txBytes)
{
    VALIDATE_NOT_NULL(txBytes);
    *txBytes = mTxBytes;
    return NOERROR;
}

ECode CNetworkStatsHistoryEntry::SetTxBytes(
    /* [in] */ Int64 txBytes)
{
    mTxBytes = txBytes;
    return NOERROR;
}

ECode CNetworkStatsHistoryEntry::GetTxPackets(
    /* [out] */ Int64* txPackets)
{
    VALIDATE_NOT_NULL(txPackets);
    *txPackets = mTxPackets;
    return NOERROR;
}

ECode CNetworkStatsHistoryEntry::SetTxPackets(
    /* [in] */ Int64 txPackets)
{
    mTxPackets = txPackets;
    return NOERROR;
}

ECode CNetworkStatsHistoryEntry::GetOperations(
    /* [out] */ Int64* operations)
{
    VALIDATE_NOT_NULL(operations);
    *operations = mOperations;
    return NOERROR;
}

ECode CNetworkStatsHistoryEntry::SetOperations(
    /* [in] */ Int64 operations)
{
    mOperations = operations;
    return NOERROR;
}

} // namespace Net
} // namepsace Droid
} // namespace Elastos
