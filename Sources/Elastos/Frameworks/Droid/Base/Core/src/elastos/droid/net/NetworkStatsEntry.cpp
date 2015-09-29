
#include "elastos/droid/net/NetworkStatsEntry.h"

namespace Elastos {
namespace Droid {
namespace Net {

CAR_INTERFACE_IMPL(NetworkStatsEntry, Object, INetworkStatsEntry)

ECode NetworkStatsEntry::constructor()
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    return constructor(INetworkStats::IFACE_ALL, INetworkStats::UID_ALL,
        INetworkStats::SET_DEFAULT, INetworkStats::TAG_NONE, 0L, 0L, 0L, 0L, 0L);
#endif
}

ECode NetworkStatsEntry::constructor(
    /* [in] */ Int64 rxBytes,
    /* [in] */ Int64 rxPackets,
    /* [in] */ Int64 txBytes,
    /* [in] */ Int64 txPackets,
    /* [in] */ Int64 operations)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    return constructor(INetworkStats::IFACE_ALL, INetworkStats::UID_ALL,
        INetworkStats::SET_DEFAULT, INetworkStats::TAG_NONE,
        rxBytes, rxPackets, txBytes, txPackets, operations);
#endif
}

ECode NetworkStatsEntry::constructor(
    /* [in] */ const String& iface,
    /* [in] */ Int32 uid,
    /* [in] */ Int32 set,
    /* [in] */ Int32 tag,
    /* [in] */ Int64 rxBytes,
    /* [in] */ Int64 rxPackets,
    /* [in] */ Int64 txBytes,
    /* [in] */ Int64 txPackets,
    /* [in] */ Int64 operations)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    mIface = iface;
    mUid = uid;
    mSet = set;
    mTag = tag;
    mRxBytes = rxBytes;
    mRxPackets = rxPackets;
    mTxBytes = txBytes;
    mTxPackets = txPackets;
    mOperations = operations;
    return NOERROR;
#endif
}

ECode NetworkStatsEntry::IsNegative(
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    *result = mRxBytes < 0 || mRxPackets < 0 || mTxBytes < 0 || mTxPackets < 0 || mOperations < 0;
    return NOERROR;
#endif
}

ECode NetworkStatsEntry::IsEmpty(
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    *result = mRxBytes == 0 && mRxPackets == 0 && mTxBytes == 0 && mTxPackets == 0
            && mOperations == 0;
    return NOERROR;
#endif
}

ECode NetworkStatsEntry::Add(
    /* [in] */ INetworkStatsEntry* another)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    Int64 num;
    another->GetRxBytes(&num);
    mRxBytes += num;
    another->GetRxPackets(&num);
    mRxPackets += num;
    another->GetTxBytes(&num);
    mTxBytes += num;
    another->GetTxPackets(&num);
    mTxPackets += num;
    another->GetOperations(&num);
    mOperations += num;
    return NOERROR;
#endif
}

ECode NetworkStatsEntry::ToString(
    /* [out] */ String* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    StringBuilder builder;
    builder += String("iface=");
    builder += mIface;
    builder += String(" uid=");
    builder += StringUtils::ToString(mUid);
    builder += String(" set=");
    builder += StringUtils::ToString(mSet);
    builder += String(" tag=");
    builder += StringUtils::ToString(mTag);
    builder += String(" rxBytes=");
    builder += StringUtils::ToString(mRxBytes);
    builder += String(" rxPackets=");
    builder += StringUtils::ToString(mRxPackets);
    builder += String(" txBytes=");
    builder += StringUtils::ToString(mTxBytes);
    builder += String(" txPackets=");
    builder += StringUtils::ToString(mTxPackets);
    builder += String(" operations=");
    builder += StringUtils::ToString(mOperations);
    *result = builder.ToString();
    return NOERROR;
#endif
}

ECode NetworkStatsEntry::Equals(
    /* [in] */ IObject* o,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
                if (o instanceof Entry) {
                    final Entry e = (Entry) o;
                    return uid == e.uid && set == e.set && tag == e.tag && rxBytes == e.rxBytes
                            && rxPackets == e.rxPackets && txBytes == e.txBytes
                            && txPackets == e.txPackets && operations == e.operations
                            && iface.equals(e.iface);
                }
                return false;

#endif
}

ECode NetworkStatsEntry::GetIface(
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);

    *result = mIface;
    return NOERROR;
}

ECode NetworkStatsEntry::SetIface(
    /* [in] */ const String& iface)
{
    mIface = iface;
    return NOERROR;
}


ECode NetworkStatsEntry::GetUid(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    *result = mUid;
    return NOERROR;
}


ECode NetworkStatsEntry::SetUid(
    /* [in] */ Int32 uid)
{
    mUid = uid;
    return NOERROR;
}


ECode NetworkStatsEntry::GetSet(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    *result = mSet;
    return NOERROR;
}


ECode NetworkStatsEntry::SetSet(
    /* [in] */ Int32 set)
{
    mSet = set;
    return NOERROR;
}


ECode NetworkStatsEntry::GetTag(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    *result = mTag;
    return NOERROR;
}


ECode NetworkStatsEntry::SetTag(
    /* [in] */ Int32 tag)
{
    mTag = tag;
    return NOERROR;
}


ECode NetworkStatsEntry::GetRxBytes(
    /* [out] */ Int64* result)
{
    VALIDATE_NOT_NULL(result);
    *result = mRxBytes;
    return NOERROR;
}


ECode NetworkStatsEntry::SetRxBytes(
    /* [in] */ Int64 rxBytes)
{
    mRxBytes = rxBytes;
    return NOERROR;
}


ECode NetworkStatsEntry::GetRxPackets(
    /* [out] */ Int64* result)
{
    VALIDATE_NOT_NULL(result);
    *result = mRxPackets;
    return NOERROR;
}


ECode NetworkStatsEntry::SetRxPackets(
    /* [in] */ Int64 rxPackets)
{
    mRxPackets = rxPackets;
    return NOERROR;
}


ECode NetworkStatsEntry::GetTxBytes(
    /* [out] */ Int64* result)
{
    VALIDATE_NOT_NULL(result);
    *result = mTxBytes;
    return NOERROR;
}


ECode NetworkStatsEntry::SetTxBytes(
    /* [in] */ Int64 txBytes)
{
    mTxBytes = txBytes;
    return NOERROR;
}


ECode NetworkStatsEntry::GetTxPackets(
    /* [out] */ Int64* result)
{
    VALIDATE_NOT_NULL(result);
    *result = mTxPackets;
    return NOERROR;
}


ECode NetworkStatsEntry::SetTxPackets(
    /* [in] */ Int64 txPackets)
{
    mTxPackets = txPackets;
    return NOERROR;
}


ECode NetworkStatsEntry::GetOperations(
    /* [out] */ Int64* result)
{
    VALIDATE_NOT_NULL(result);
    *result = mOperations;
    return NOERROR;
}


ECode NetworkStatsEntry::SetOperations(
    /* [in] */ Int64 operations)
{
    mOperations = operations;
    return NOERROR;
}

} // namespace Net
} // namespace Droid
} // namespace Elastos
