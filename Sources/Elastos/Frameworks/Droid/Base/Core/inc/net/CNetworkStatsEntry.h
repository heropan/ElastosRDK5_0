
#ifndef __CNETWORKSTATSENTRY_H__
#define __CNETWORKSTATSENTRY_H__

#include "_CNetworkStatsEntry.h"
#include "ext/frameworkdef.h"

namespace Elastos {
namespace Droid {
namespace Net {

CarClass(CNetworkStatsEntry)
{
public:
    CNetworkStatsEntry();

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ Int64 rxBytes,
        /* [in] */ Int64 rxPackets,
        /* [in] */ Int64 txBytes,
        /* [in] */ Int64 txPackets,
        /* [in] */ Int64 operations);

    CARAPI constructor(
        /* [in] */ const String& iface,
        /* [in] */ Int32 uid,
        /* [in] */ Int32 set,
        /* [in] */ Int32 tag,
        /* [in] */ Int64 rxBytes,
        /* [in] */ Int64 rxPackets,
        /* [in] */ Int64 txBytes,
        /* [in] */ Int64 txPackets,
        /* [in] */ Int64 operations);

    CARAPI IsNegative(
        /* [out] */ Boolean* result);

    CARAPI IsEmpty(
        /* [out] */ Boolean* result);

    CARAPI Add(
        /* [in] */ INetworkStatsEntry* another);

    CARAPI ToString(
        /* [out] */ String* result);

    CARAPI SetIface(
        /* [in] */ const String& iface);

    CARAPI GetIface(
        /* [out] */ String* iface);

    CARAPI SetUid(
        /* [in] */ Int32 uid);

    CARAPI GetUid(
        /* [out] */ Int32* uid);

    CARAPI SetSet(
        /* [in] */ Int32 set);

    CARAPI GetSet(
        /* [out] */ Int32* set);

    CARAPI SetTag(
        /* [in] */ Int32 tag);

    CARAPI GetTag(
        /* [out] */ Int32* tag);

    CARAPI SetRxBytes(
        /* [in] */ Int64 rxBytes);

    CARAPI GetRxBytes(
        /* [out] */ Int64* rxBytes);

    CARAPI SetRxPackets(
        /* [in] */ Int64 rxPackets);

    CARAPI GetRxPackets(
        /* [out] */ Int64* rxPackets);

    CARAPI SetTxBytes(
        /* [in] */ Int64 txBytes);

    CARAPI GetTxBytes(
        /* [out] */ Int64* txBytes);

    CARAPI SetTxPackets(
        /* [in] */ Int64 txPackets);

    CARAPI GetTxPackets(
        /* [out] */ Int64* txPackets);

    CARAPI SetOperations(
        /* [in] */ Int64 operations);

    CARAPI GetOperations(
        /* [out] */ Int64* operations);

public:
    String mIface;
    Int32 mUid;
    Int32 mSet;
    Int32 mTag;
    Int64 mRxBytes;
    Int64 mRxPackets;
    Int64 mTxBytes;
    Int64 mTxPackets;
    Int64 mOperations;
};

} // namespace Net
} // namepsace Droid
} // namespace Elastos

#endif // __CNETWORKSTATSENTRY_H__

