
#include "net/CMailToHelper.h"
#include "net/Uri.h"

namespace Elastos {
namespace Droid {
namespace Net {

Ecode CTrafficStatsHelper::SetThreadStatsTag(
    /* [in] */ Int32 tag)
{
    return CTrafficStats::SetThreadStatsTag(tag);
}

Ecode CTrafficStatsHelper::GetThreadStatsTag(
    /* [out] */ Int32* result)
{
    return CTrafficStats::GetThreadStatsTag(result);
}

Ecode CTrafficStatsHelper::ClearThreadStatsTag()
{
    return CTrafficStats::ClearThreadStatsTag();
}

Ecode CTrafficStatsHelper::SetThreadStatsUid(
        /* [in] */ Int32 uid)
{
    return CTrafficStats::SetThreadStatsUid(uid);
}

Ecode CTrafficStatsHelper::ClearThreadStatsUid()
{
    return CTrafficStats::ClearThreadStatsUid();
}

Ecode CTrafficStatsHelper::TagSocket(
        /* [in] */ Socket socket)
{
    return CTrafficStats::TagSocket(socket);
}

Ecode CTrafficStatsHelper::UntagSocket(
        /* [in] */ Socket socket)
{
    return CTrafficStats::UntagSocket(socket);
}

Ecode CTrafficStatsHelper::StartDataProfiling(
        /* [in] */ IContext context)
{
    return CTrafficStats::StartDataProfiling(context);
}

Ecode CTrafficStatsHelper::stopDataProfiling(
    /* [in] */  IContext context,
    /* [out] */ INetworkStats** Retvalue)
{
    return CTrafficStats::stopDataProfiling(context, Retvalue);
}

Ecode CTrafficStatsHelper::IncrementOperationCount(
        /* [in] */ Int32 operationCount)
{
    return CTrafficStats::IncrementOperationCount(operationCount);
}

Ecode CTrafficStatsHelper::IncrementOperationCount(
    /* [in] */ Int32 tag,
    /* [in] */ Int32 operationCount)
{
    return CTrafficStats::IncrementOperationCount(tag, operationCount);
}

Ecode CTrafficStatsHelper::CloseQuietly(
    /* [in] */ INetworkStatsSession session)
{
    return CTrafficStats::CloseQuietly(session);
}

Ecode CTrafficStatsHelper::GetMobileTxPackets(
    /* [out] */ Int64* total)
{
    return CTrafficStats::GetMobileTxPackets(total);
}

Ecode CTrafficStatsHelper::GetMobileRxPackets(
    /* [out] */ Int64* total)
{
    return CTrafficStats::GetMobileRxPackets(total);
}

Ecode CTrafficStatsHelper::GetMobileTxBytes(
     /* [out] */ Int64* total)
{
    return CTrafficStats::GetMobileTxBytes(total);
}

Ecode CTrafficStatsHelper::GetMobileRxBytes(
     /* [out] */ Int64* total)
{
    return CTrafficStats::GetMobileRxBytes(total);
}

Ecode CTrafficStatsHelper::GetTxPackets(
    /* [in] */ const String& iface),
    /* [out] */ Int64* Retvalue)
{
    return CTrafficStats::GetTxPackets(iface, Retvalue);
}

Ecode CTrafficStatsHelper::GetRxPackets(
    /* [in] */ const String& iface,
    /* [out] */ Int64* Retvalue)
{
    return CTrafficStats::GetTxPackets(iface, Retvalue);
}

Ecode CTrafficStatsHelper::GetTxBytes(
    /* [in] */ const String& iface,
    /* [out] */ Int64* Retvalue)
{
    return CTrafficStats::GetTxBytes(iface, Retvalue);
}

Ecode CTrafficStatsHelper::GetRxBytes(
    /* [in] */ const String& iface,
    /* [out] */ Int64* Retvalue)
{
    return CTrafficStats::GetTxBytes(iface, Retvalue);
}

Ecode CTrafficStatsHelper::GetTotalTxPackets(
    /* [out] */ Int64* Retvalue)
{
    return CTrafficStats::GetTotalTxPackets(Retvalue);
}

Ecode CTrafficStatsHelper::GetTotalRxPackets(
    /* [out] */ Int64* Retvalue)
{
    return CTrafficStats::GetTotalTxPackets(Retvalue);
}

static CTrafficStatsHelper::GetTotalTxBytes(
    /* [out] */ Int64* Retvalue)
{
    return CTrafficStats::GetTotalTxBytes(Retvalue);
}

static CTrafficStatsHelper::GetTotalRxBytes(
    /* [out] */ Int64* Retvalue)
{
    return CTrafficStats::GetTotalRxBytes(Retvalue);
}

Ecode CTrafficStatsHelper::GetUidTxPackets(
    /* [in] */ Int32 uid,
    /* [out] */ Int64* Retvalue)
{
    return CTrafficStats::GetUidTxPackets(uid, Retvalue);
}

Ecode CTrafficStatsHelper::GetUidRxPackets(
    /* [in] */ Int32 uid,
    /* [out] */ Int64* Retvalue)
{
    return CTrafficStats::GetUidRxPackets(uid, Retvalue);
}

Ecode CTrafficStatsHelper::GetUidTcpTxBytes(
    /* [in] */ Int32 uid,
    /* [out] */ Int64* Retvalue)
{
    return CTrafficStats::GetUidTcpTxBytes(uid, Retvalue);
}

Ecode CTrafficStatsHelper::GetUidTcpRxBytes(
    /* [in] */ Int32 uid,
    /* [out] */ Int64* Retvalue)
{
    return CTrafficStats::GetUidTcpRxBytes(uid, Retvalue);
}

Ecode CTrafficStatsHelper::GetUidUdpTxBytes(
    /* [in] */ Int32 uid,
    /* [out] */ Int64* Retvalue)
{
    return CTrafficStats::GetUidUdpTxBytes(uid, Retvalue);
}

Ecode CTrafficStatsHelper::GetUidUdpRxBytes(
    /* [in] */ Int32 uid,
    /* [out] */ Int64* Retvalue)
{
    return CTrafficStats::GetUidUdpRxBytes(uid, Retvalue);
}

Ecode CTrafficStatsHelper::GetUidTcpTxSegments(
    /* [in] */ Int32 uid,
    /* [out] */ Int64* Retvalue)
{
    return CTrafficStats::GetUidTcpTxSegments(uid, Retvalue);
}

Ecode CTrafficStatsHelper::GetUidTcpRxSegments(
    /* [in] */ Int32 uid,
    /* [out] */ Int64* Retvalue)
{
    return CTrafficStats::GetUidTcpTxSegments(uid, Retvalue);
}

Ecode CTrafficStatsHelper::GetUidUdpTxPackets(
    /* [in] */ Int32 uid,
    /* [out] */ Int64* Retvalue)
{
    return CTrafficStats::GetUidUdpTxPackets(uid, Retvalue);
}

Ecode CTrafficStatsHelper::GetUidUdpRxPackets(
    /* [in] */ Int32 uid,
    /* [out] */ Int64* Retvalue)
{
    return CTrafficStats::GetUidUdpRxPackets(uid, Retvalue);
}


} // namespace Net
} // namepsace Droid
} // namespace Elastos
