
#ifndef __ELASTOS_DROID_NET_CNETWORKSTATSHISTORY_H__
#define __ELASTOS_DROID_NET_CNETWORKSTATSHISTORY_H__

#include "_Elastos_Droid_Net_CNetworkStatsHistory.h"
#include "elastos/droid/ext/frameworkext.h"

using Elastos::IO::IDataOutputStream;
using Elastos::IO::IDataInputStream;
using Elastos::Core::IRandom;
using Elastos::Utility::CRandom;
using Elastos::Droid::Internal::Utility::IIndentingPrintWriter;

namespace Elastos {
namespace Droid {
namespace Net {

CarClass(CNetworkStatsHistory)
{
public:
    class DataStreamUtils
    {
    public:
        static CARAPI ReadFullLongArray(
           /* [in] */ IDataInputStream* in,
           /* [out] */ ArrayOf<Int64>** values);

        static CARAPI ReadVarLong(
            /* [in] */ IDataInputStream* in,
            /* [out] */ Int64* values);

        static CARAPI WriteVarLong(
            /* [in] */ IDataOutputStream* out,
            /* [in] */ Int64 values);

        static CARAPI ReadVarLongArray(
            /* [in] */ IDataInputStream* in,
            /* [out] */ ArrayOf<Int64>** values);

        static CARAPI WriteVarLongArray(
            /* [in] */ IDataOutputStream* out,
            /* [in] */ ArrayOf<Int64>* values,
            /* [in] */ Int32 size);
    };

    class ParcelUtils
    {
    public:
        static CARAPI ReadLongArray(
            /* [in] */ IParcel* source,
            /* [out] */ArrayOf<Int64>** result);

        static CARAPI WriteLongArray(
            /* [in] */ IParcel* out,
            /* [in] */ ArrayOf<Int64>* values,
            /* [in] */ Int32 size);
    };

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ Int64 bucketDuration);

    CARAPI constructor(
        /* [in] */ Int64 bucketDuration,
        /* [in] */ Int32 initialSize);

    CARAPI constructor(
        /* [in] */ Int64 bucketDuration,
        /* [in] */ Int32 initialSize,
        /* [in] */ Int32 fields);

    CARAPI constructor(
        /* [in] */ INetworkStatsHistory* existing,
        /* [in] */ Int64 bucketDuration);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI constructor(
        /* [in] */ IDataInputStream* in);

    CARAPI WriteToStream(
        /* [in] */ IDataOutputStream* out);

    CARAPI GetSize(
        /* [out] */ Int32* result);

    CARAPI GetBucketDuration(
        /* [out] */ Int64* result);

    CARAPI GetStart(
        /* [out] */ Int64* result);

    CARAPI GetEnd(
        /* [out] */ Int64* result);

    CARAPI GetTotalBytes(
        /* [out] */ Int64* result);

    CARAPI GetIndexBefore(
        /* [in] */ Int64 time,
        /* [out] */ Int32* result);

    CARAPI GetIndexAfter(
        /* [in] */ Int64 time,
        /* [out] */ Int32* result);

    CARAPI GetValues(
        /* [in] */ Int32 i,
        /* [in] */ INetworkStatsHistoryEntry* recycle,
        /* [out] */ INetworkStatsHistoryEntry** result);

    CARAPI RecordData(
        /* [in] */ Int64 start,
        /* [in] */ Int64 end,
        /* [in] */ Int64 rxBytes,
        /* [in] */ Int64 txBytes);

    CARAPI RecordData(
        /* [in] */ Int64 start,
        /* [in] */ Int64 end,
        /* [in] */ INetworkStatsEntry* entry);

    CARAPI RecordEntireHistory(
        /* [in] */ INetworkStatsHistory* input);

    CARAPI RecordHistory(
        /* [in] */ INetworkStatsHistory* input,
        /* [in] */ Int64 start,
        /* [in] */ Int64 end);

    CARAPI RemoveBucketsBefore(
        /* [in] */ Int64 cutoff);

    CARAPI GetValues(
        /* [in] */ Int64 start,
        /* [in] */ Int64 end,
        /* [in] */ INetworkStatsHistoryEntry* recycle,
        /* [out] */ INetworkStatsHistoryEntry** result);

    CARAPI GetValues(
        /* [in] */ Int64 start,
        /* [in] */ Int64 end,
        /* [in] */ Int64 now,
        /* [in] */ INetworkStatsHistoryEntry* recycle,
        /* [out] */ INetworkStatsHistoryEntry** result);

    CARAPI GenerateRandom(
        /* [in] */ Int64 start,
        /* [in] */ Int64 end,
        /* [in] */ Int64 bytes);

    CARAPI GenerateRandom(
        /* [in] */ Int64 start,
        /* [in] */ Int64 end,
        /* [in] */ Int64 rxBytes,
        /* [in] */ Int64 rxPackets,
        /* [in] */ Int64 txBytes,
        /* [in] */ Int64 txPackets,
        /* [in] */ Int64 operations,
        /* [in] */ IRandom* r);

    CARAPI EstimateResizeBuckets(
        /* [in] */ Int64 newBucketDuration,
        /* [out] */ Int32* result);

    CARAPI Dump(
        /* [in] */ IIndentingPrintWriter* pw,
        /* [in] */ Boolean fullHistory);

    CARAPI ToString(
        /* [out] */ String* result);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    static CARAPI RandomLong(
        /* [in] */ IRandom* r,
        /* [in] */ Int64 start,
        /* [in] */ Int64 end,
        /* [out] */ Int64* result);

    CARAPI GetBucketCount(
        /* [out] */ Int32* bucketCount);

    CARAPI GetBucketStart(
        /* [out, callee] */ ArrayOf<Int64>** bucketStart);

    CARAPI GetRxBytes(
        /* [out, callee] */ ArrayOf<Int64>** rxBytes);

    CARAPI GetRxPackets(
        /* [out, callee] */ ArrayOf<Int64>** rxPackets);

    CARAPI GetTxBytes(
        /* [out, callee] */ ArrayOf<Int64>** txBytes);

    CARAPI GetTxPackets(
        /* [out, callee] */ ArrayOf<Int64>** txPackets);

    CARAPI GetOperations(
        /* [out, callee] */ ArrayOf<Int64>** operations);


private:
    void EnsureBuckets(
        /* [in] */ Int64 start,
        /* [in] */ Int64 end);

    void InsertBucket(
        /* [in] */ Int32 index,
        /* [in] */ Int64 start);

    static Int64 GetLong(
        /* [in] */ ArrayOf<Int64>* array,
        /* [in] */ Int32 i,
        /* [in] */ Int64 value);

    static void SetLong(
        /* [in] */ ArrayOf<Int64>* array,
        /* [in] */ Int32 i,
        /* [in] */ Int64 value);

    static void AddLong(
        /* [in] */ ArrayOf<Int64>* array,
        /* [in] */ Int32 i,
        /* [in] */ Int64 value);

private:
    static const Int32 VERSION_INIT;
    static const Int32 VERSION_ADD_PACKETS;
    static const Int32 VERSION_ADD_ACTIVE;

    Int64 mBucketDuration;
    Int32 mBucketCount;
    AutoPtr< ArrayOf<Int64> > mBucketStart;
    AutoPtr< ArrayOf<Int64> > mActiveTime;
    AutoPtr< ArrayOf<Int64> > mRxBytes;
    AutoPtr< ArrayOf<Int64> > mRxPackets;
    AutoPtr< ArrayOf<Int64> > mTxBytes;
    AutoPtr< ArrayOf<Int64> > mTxPackets;
    AutoPtr< ArrayOf<Int64> > mOperations;
    Int64 mTotalBytes;
};

} // namespace Net
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_NET_CNETWORKSTATSHISTORY_H__

