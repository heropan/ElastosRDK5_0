
#ifndef __ELASTOS_DROID_NET_CNETWORKSTATS_H__
#define __ELASTOS_DROID_NET_CNETWORKSTATS_H__

#include "_CNetworkStats.h"

using Elastos::IO::IPrintWriter;

namespace Elastos {
namespace Droid {
namespace Net {

CarClass(CNetworkStats)
{
public:
    CNetworkStats();

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ Int64 elapsedRealtime,
        /* [in] */ Int32 initialSize);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI Clone(
        /* [out] */ INetworkStats** result);

    CARAPI AddIfaceValues(
        /* [in] */ const String& iface,
        /* [in] */ Int64 rxBytes,
        /* [in] */ Int64 rxPackets,
        /* [in] */ Int64 txBytes,
        /* [in] */ Int64 txPackets);

    CARAPI AddValues(
        /* [in] */ const String& iface,
        /* [in] */ Int32 uid,
        /* [in] */ Int32 set,
        /* [in] */ Int32 tag,
        /* [in] */ Int64 rxBytes,
        /* [in] */ Int64 rxPackets,
        /* [in] */ Int64 txBytes,
        /* [in] */ Int64 txPackets,
        /* [in] */ Int64 operations);

    CARAPI AddValuesEx(
        /* [in] */ INetworkStatsEntry* entry);

    CARAPI GetValues(
        /* [in] */ Int32 i,
        /* [in] */ INetworkStatsEntry* recycle,
        /* [out] */ INetworkStatsEntry** result);

    CARAPI GetElapsedRealtime(
        /* [out] */ Int64* result);

    CARAPI GetElapsedRealtimeAge(
        /* [out] */ Int64* result);

    CARAPI GetSize(
        /* [out] */ Int32* result);

    CARAPI GetInternalSize(
        /* [out] */ Int32* result);

    CARAPI CombineValues(
        /* [in] */ const String& iface,
        /* [in] */ Int32 uid,
        /* [in] */ Int32 tag,
        /* [in] */ Int64 rxBytes,
        /* [in] */ Int64 rxPackets,
        /* [in] */ Int64 txBytes,
        /* [in] */ Int64 txPackets,
        /* [in] */ Int64 operations);

    CARAPI CombineValuesEx(
        /* [in] */ const String& iface,
        /* [in] */ Int32 uid,
        /* [in] */ Int32 set,
        /* [in] */ Int32 tag,
        /* [in] */ Int64 rxBytes,
        /* [in] */ Int64 rxPackets,
        /* [in] */ Int64 txBytes,
        /* [in] */ Int64 txPackets,
        /* [in] */ Int64 operations);

    CARAPI CombineValuesEx2(
        /* [in] */ INetworkStatsEntry* entry);

    CARAPI CombineAllValues(
        /* [in] */ INetworkStats* another);

    CARAPI FindIndex(
        /* [in] */ const String& iface,
        /* [in] */ Int32 uid,
        /* [in] */ Int32 set,
        /* [in] */ Int32 tag,
        /* [out] */ Int32* result);

    CARAPI FindIndexHinted(
        /* [in] */ const String& iface,
        /* [in] */ Int32 uid,
        /* [in] */ Int32 set,
        /* [in] */ Int32 tag,
        /* [in] */ Int32 hintIndex,
        /* [out] */ Int32* result);

    CARAPI SpliceOperationsFrom(
        /* [in] */ INetworkStats* stats);

    CARAPI GetUniqueIfaces(
        /* [out, callee] */ ArrayOf<String>** result);

    CARAPI GetUniqueUids(
        /* [out, callee] */ ArrayOf<Int32>** result);

    CARAPI GetTotalBytes(
        /* [out] */ Int64* result);

    CARAPI GetTotal(
        /* [in] */ INetworkStatsEntry* recycle,
        /* [out] */ INetworkStatsEntry** result);

    CARAPI GetTotalEx(
        /* [in] */ INetworkStatsEntry* recycle,
        /* [in] */ Int32 limitUid,
        /* [out] */ INetworkStatsEntry** result);

    CARAPI GetTotalEx2(
        /* [in] */ INetworkStatsEntry* recycle,
        /* [in] */ IObjectContainer* limitIface,
        /* [out] */ INetworkStatsEntry** result);

    CARAPI GetTotalIncludingTags(
        /* [in] */ INetworkStatsEntry* recycle,
        /* [out] */ INetworkStatsEntry** result);

    CARAPI Subtract(
        /* [in] */ INetworkStats* right,
        /* [out] */ INetworkStats** result);

    CARAPI GroupedByIface(
        /* [out] */ INetworkStats** result);

    CARAPI GroupedByUid(
        /* [out] */ INetworkStats** result);

    CARAPI WithoutUids(
        /* [in] */ const ArrayOf<Int32>& uids,
        /* [out] */ INetworkStats** result);

    CARAPI Dump(
        /* [in] */ const String& prefix,
        /* [in] */ IPrintWriter* pw);

    CARAPI ToString(
        /* [out] */ String* result);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI GetIFace(
        /* [out, callee] */ ArrayOf<String>** face);

    CARAPI GetUid(
        /* [out, callee] */ ArrayOf<Int32>** uid);

    CARAPI GetSet(
        /* [out, callee] */ ArrayOf<Int32>** set);

    CARAPI GetTag(
        /* [out, callee] */ ArrayOf<Int32>** tag);

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

    static CARAPI Subtract(
        /* [in] */ INetworkStats* left,
        /* [in] */ INetworkStats* right,
        /* [in] */ INonMonotonicObserver* observer,
        /* [in] */ IInterface* cookie,
        /* [out] */ INetworkStats** result);

    static CARAPI SetToString(
        /* [in] */ Int32 set,
        /* [out] */ String* result);

    static CARAPI TagToString(
        /* [in] */ Int32 tag,
        /* [out] */ String* result);

private:
    CARAPI_(AutoPtr<INetworkStatsEntry>) GetTotal(
        /* [in] */ INetworkStatsEntry* recycle,
        /* [in] */ IObjectContainer* limitIface,
        /* [in] */ Int32 limitUid,
        /* [in] */ Boolean includeTags);

    // TODO: move fields to "mVariable" notation

private:
    /**
     * {@link SystemClock#elapsedRealtime()} timestamp when this data was
     * generated.
     */
    Int64 mElapsedRealtime;
    Int32 mSize;
    AutoPtr< ArrayOf<String> > mIface;
    AutoPtr< ArrayOf<Int32> > mUid;
    AutoPtr< ArrayOf<Int32> > mSet;
    AutoPtr< ArrayOf<Int32> > mTag;
    AutoPtr< ArrayOf<Int64> > mRxBytes;
    AutoPtr< ArrayOf<Int64> > mRxPackets;
    AutoPtr< ArrayOf<Int64> > mTxBytes;
    AutoPtr< ArrayOf<Int64> > mTxPackets;
    AutoPtr< ArrayOf<Int64> > mOperations;
};

} // namespace Net
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_NET_CNETWORKSTATS_H__

