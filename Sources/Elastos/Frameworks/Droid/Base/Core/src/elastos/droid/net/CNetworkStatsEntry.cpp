
#include "net/CNetworkStatsEntry.h"
#include <elastos/core/StringBuilder.h>
#include <elastos/core/StringUtils.h>

using Elastos::Core::StringUtils;
using Elastos::Core::StringBuilder;

namespace Elastos {
namespace Droid {
namespace Net {

CNetworkStatsEntry::CNetworkStatsEntry()
{}

ECode CNetworkStatsEntry::constructor()
{
    return constructor(INetworkStats::IFACE_ALL, INetworkStats::UID_ALL,
        INetworkStats::SET_DEFAULT, INetworkStats::TAG_NONE, 0L, 0L, 0L, 0L, 0L);
}

ECode CNetworkStatsEntry::constructor(
    /* [in] */ Int64 rxBytes,
    /* [in] */ Int64 rxPackets,
    /* [in] */ Int64 txBytes,
    /* [in] */ Int64 txPackets,
    /* [in] */ Int64 operations)
{
    return constructor(INetworkStats::IFACE_ALL, INetworkStats::UID_ALL,
        INetworkStats::SET_DEFAULT, INetworkStats::TAG_NONE,
        rxBytes, rxPackets, txBytes, txPackets, operations);
}

ECode CNetworkStatsEntry::constructor(
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
}

ECode CNetworkStatsEntry::IsNegative(
    /* [out] */ Boolean* result)
{
    *result = mRxBytes < 0 || mRxPackets < 0 || mTxBytes < 0 || mTxPackets < 0 || mOperations < 0;
    return NOERROR;
}

ECode CNetworkStatsEntry::IsEmpty(
    /* [out] */ Boolean* result)
{
    *result = mRxBytes == 0 && mRxPackets == 0 && mTxBytes == 0 && mTxPackets == 0
            && mOperations == 0;
    return NOERROR;
}

ECode CNetworkStatsEntry::Add(
    /* [in] */ INetworkStatsEntry* another)
{
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
}

//@Override
ECode CNetworkStatsEntry::ToString(
    /* [out] */ String* result)
{
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
}

ECode CNetworkStatsEntry::SetIface(
    /* [in] */ const String& iface)
{
    mIface = iface;
    return NOERROR;
}

ECode CNetworkStatsEntry::GetIface(
    /* [out] */ String* iface)
{
    VALIDATE_NOT_NULL(iface);
    *iface = mIface;
    return NOERROR;
}

ECode CNetworkStatsEntry::SetUid(
    /* [in] */ Int32 uid)
{
    mUid = uid;
    return NOERROR;
}

ECode CNetworkStatsEntry::GetUid(
    /* [out] */ Int32* uid)
{
    VALIDATE_NOT_NULL(uid);
    *uid = mUid;
    return NOERROR;
}

ECode CNetworkStatsEntry::SetSet(
    /* [in] */ Int32 set)
{
    mSet = set;
    return NOERROR;
}

ECode CNetworkStatsEntry::GetSet(
    /* [out] */ Int32* set)
{
    VALIDATE_NOT_NULL(set);
    *set = mSet;
    return NOERROR;
}

ECode CNetworkStatsEntry::SetTag(
    /* [in] */ Int32 tag)
{
    mTag = tag;
    return NOERROR;
}

ECode CNetworkStatsEntry::GetTag(
    /* [out] */ Int32* tag)
{
    VALIDATE_NOT_NULL(tag);
    *tag = mTag;
    return NOERROR;
}

ECode CNetworkStatsEntry::SetRxBytes(
    /* [in] */ Int64 rxBytes)
{
    mRxBytes = rxBytes;
    return NOERROR;
}

ECode CNetworkStatsEntry::GetRxBytes(
    /* [out] */ Int64* rxBytes)
{
    VALIDATE_NOT_NULL(rxBytes);
    *rxBytes = mRxBytes;
    return NOERROR;
}

ECode CNetworkStatsEntry::SetRxPackets(
    /* [in] */ Int64 rxPackets)
{
    mRxPackets = rxPackets;
    return NOERROR;
}

ECode CNetworkStatsEntry::GetRxPackets(
    /* [out] */ Int64* rxPackets)
{
    VALIDATE_NOT_NULL(rxPackets);
    *rxPackets = mRxPackets;
    return NOERROR;
}

ECode CNetworkStatsEntry::SetTxBytes(
    /* [in] */ Int64 txBytes)
{
    mTxBytes = txBytes;
    return NOERROR;
}

ECode CNetworkStatsEntry::GetTxBytes(
    /* [out] */ Int64* txBytes)
{
    VALIDATE_NOT_NULL(txBytes);
    *txBytes = mTxBytes;
    return NOERROR;
}

ECode CNetworkStatsEntry::SetTxPackets(
    /* [in] */ Int64 txPackets)
{
    mTxPackets = txPackets;
    return NOERROR;
}

ECode CNetworkStatsEntry::GetTxPackets(
    /* [out] */ Int64* txPackets)
{
    VALIDATE_NOT_NULL(txPackets);
    *txPackets = mTxPackets;
    return NOERROR;
}

ECode CNetworkStatsEntry::SetOperations(
    /* [in] */ Int64 operations)
{
    mOperations = operations;
    return NOERROR;
}

ECode CNetworkStatsEntry::GetOperations(
    /* [out] */ Int64* operations)
{
    VALIDATE_NOT_NULL(operations);
    *operations = mOperations;
    return NOERROR;
}

} // namespace Net
} // namepsace Droid
} // namespace Elastos
