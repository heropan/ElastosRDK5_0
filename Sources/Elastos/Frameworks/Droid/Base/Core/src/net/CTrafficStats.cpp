
#include "ext/frameworkext.h"
#include "net/CTrafficStats.h"
#include "net/CNetworkStats.h"
#include "os/Process.h"
#include "os/ServiceManager.h"
#include "elastos/StringBuffer.h"
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <elastos/Logger.h>
#include <elastos/Slogger.h>
// Returns an ASCII decimal number read from the specified file, -1 on error.



using Elastos::Utility::Logging::Slogger;



namespace Elastos {
namespace Droid {
namespace Net {

static const uint64_t VALUE_UNKNOWN = -1;
static const char* IFACE_STAT_ALL = "/proc/net/xt_qtaguid/iface_stat_all";
const String CTrafficStats::TAG=String("CTrafficStats");

INetworkStats* CTrafficStats::sActiveProfilingStart;
Mutex CTrafficStats::sProfilingLock;
AutoPtr<INetworkStatsService> CTrafficStats::sStatsService;


ECode CTrafficStats::constructor()
{
    return NOERROR;
}


Int64 CTrafficStats::ReadNumber(
    /* [in] */const char* filename)
{
    Char8 buf[80];
    Int32 fd = open(filename, O_RDONLY);
    if (fd < 0) {
        if (errno != ENOENT){
            Slogger::E(TAG, String("Can't open") + filename + strerror(errno));
        }
        return -1;
    }

    Int32 len = read(fd, buf, sizeof(buf) - 1);
    if (len < 0) {
        //Slogger::E(TAG, "Can't open %s: %s" + filename + strerror(errno));
        Slogger::E(TAG, String("Can't open") + filename + strerror(errno) );
        close(fd);
        return -1;
    }

    close(fd);
    buf[len] = '\0';
    return ((Int64)(buf));

}

ECode CTrafficStats::GetUidTxBytes(
    /* [in] */ Int32 uid,
    /* [out] */ Int64* result)
{
    VALIDATE_NOT_NULL(result);
    *result= GetUidBytes(uid, TX, TCP_AND_UDP);
    return NOERROR;
}

ECode CTrafficStats::GetUidRxBytes(
    /* [in] */ Int32 uid,
    /* [out] */ Int64* result)
{
    VALIDATE_NOT_NULL(result);
    *result= GetUidBytes(uid, RX, TCP_AND_UDP);
    return NOERROR;
}


INetworkStatsService* CTrafficStats::GetStatsService()
{
    Mutex::Autolock lock(sProfilingLock);

    if (sStatsService!=NULL)
        return sStatsService;

    sStatsService =(INetworkStatsService*)ServiceManager::GetService(IContext::NETWORK_STATS_SERVICE).Get();
    return sStatsService;
}

ECode CTrafficStats::SetThreadStatsTag(
    /* [in] */ Int32 tag)
{
    //anthony check
    //Not implement
    //NetworkManagementSocketTagger::SetThreadSocketStatsTag(tag);
    return NOERROR;
}

ECode CTrafficStats::GetThreadStatsTag(
    /* [out] */ Int32* result)
{
    //anthony check
    //Not implement
    //*result=NetworkManagementSocketTagger::GetThreadSocketStatsTag();
    return NOERROR;
}

ECode CTrafficStats::ClearThreadStatsTag()
{
    //anthony check
    //Not implement
    //NetworkManagementSocketTagger::SetThreadSocketStatsTag(-1);
    return NOERROR;
}

ECode CTrafficStats::SetThreadStatsUid(
    /* [in] */ Int32 uid)
{
    //anthony check
    //Not implement
    //NetworkManagementSocketTagger::SetThreadSocketStatsUid(uid);
    return NOERROR;
}

ECode CTrafficStats::ClearThreadStatsUid()
{
    //anthony check
    //Not implement
    //NetworkManagementSocketTagger::SetThreadSocketStatsUid(-1);
    return NOERROR;
}

ECode CTrafficStats::TagSocket(
    /* [in] */ ISocket* socket)
{
    //anthony check
    //feature is rejected
//    SocketTagger::Get()->Tag(socket);
    return NOERROR;
}

ECode CTrafficStats::UntagSocket(
    /* [in] */ ISocket* socket)
{
    //anthony check
    //feature is rejected
    //SocketTagger::Get()->Untag(socket);
    return NOERROR;
}

ECode CTrafficStats::StartDataProfiling(
    /* [in] */ IContext* context)
{
    Mutex::Autolock lock(sProfilingLock);

    if (sActiveProfilingStart != NULL) {
        Slogger::E(TAG, "already profiling data");
        return E_RUNTIME_EXCEPTION;
    }

    // take snapshot in time; we calculate delta later
    sActiveProfilingStart = GetDataLayerSnapshotForUid(context);
}

ECode CTrafficStats::StopDataProfiling(
    /* [in] */  IContext* context,
    /* [out] */ INetworkStats** retvalue)
{
    VALIDATE_NOT_NULL(retvalue);
    Mutex::Autolock lock(sProfilingLock);

    if (sActiveProfilingStart == NULL) {
        Slogger::E(TAG, "not profiling data");
        return E_RUNTIME_EXCEPTION;
    }

    // subtract starting values and return delta
    AutoPtr<INetworkStats> profilingStop = GetDataLayerSnapshotForUid(context);
    AutoPtr<INetworkStats> profilingDelta;
    CNetworkStats::Subtract(profilingStop, sActiveProfilingStart, NULL, NULL, (INetworkStats**)&profilingDelta);

    sActiveProfilingStart = NULL;
    *retvalue = profilingDelta;
    INTERFACE_ADDREF(*retvalue);
    return NOERROR;
}

ECode CTrafficStats::IncrementOperationCount(
    /* [in] */ Int32 operationCount)
{
    Int32 tag;
    GetThreadStatsTag(&tag);
    IncrementOperationCountEx(tag, operationCount);
    return NOERROR;
}

ECode CTrafficStats::IncrementOperationCountEx(
    /* [in] */ Int32 tag,
    /* [in] */ Int32 operationCount)
{
    Int32 uid = Process::MyUid();
    GetStatsService()->IncrementOperationCount(uid, tag, operationCount);
    return NOERROR;
}

ECode CTrafficStats::CloseQuietly(
    /* [in] */ INetworkStatsSession* session)
{
    // TODO: move to NetworkStatsService once it exists
    if (session != NULL) {
            session->Close();
            return NOERROR;
    }
    else
    {
        return E_NULL_POINTER_EXCEPTION;
    }
}

ECode CTrafficStats::GetMobileTxPackets(
    /* [out] */ Int64* result)
{
    VALIDATE_NOT_NULL(result);
    *result = 0;

    AutoPtr<ArrayOf<String> > inface;
    GetStatsService()->GetMobileIfaces((ArrayOf<String>**)&inface);

    Int64 temptotal = 0;
    for (Int32 i = 0; i < inface->GetLength(); i++) {
        GetTxPackets((*inface)[i], &temptotal);
        *result += temptotal;
    }

    return NOERROR;
}


ECode CTrafficStats::GetMobileRxPackets(
    /* [out] */ Int64* result)
{
    VALIDATE_NOT_NULL(result);
    *result = 0;

    AutoPtr<ArrayOf<String> > inface;
    GetStatsService()->GetMobileIfaces((ArrayOf<String>**)&inface);

    Int64 temptotal = 0;
    for (Int32 i = 0; i < inface->GetLength(); i++) {
       GetRxPackets((*inface)[i], &temptotal);
       *result += temptotal;
    }
    return NOERROR;
}

ECode CTrafficStats::GetMobileTxBytes(
    /* [out] */ Int64* result)
{
    VALIDATE_NOT_NULL(result);
    *result = 0;

    AutoPtr<ArrayOf<String> > inface;
    GetStatsService()->GetMobileIfaces((ArrayOf<String>**)&inface);

    Int64 temptotal = 0;
    for (Int32 i = 0; i < inface->GetLength(); i++)
    {
        GetTxBytes((*inface)[i], &temptotal);
        *result += temptotal;
    }

    return NOERROR;
}

ECode CTrafficStats::GetMobileRxBytes(
     /* [out] */ Int64* result)
{
    VALIDATE_NOT_NULL(result);
    *result = 0;

    AutoPtr<ArrayOf<String> > inface;
    GetStatsService()->GetMobileIfaces((ArrayOf<String>**)&inface);

    Int64 temptotal = 0;
    for (Int32 i = 0; i < inface->GetLength(); i++)
    {
        GetRxBytes((*inface)[i], &temptotal);
        *result += temptotal;
    }

    return NOERROR;
}


ECode CTrafficStats::GetTxPackets(
    /* [in] */ const String& iface,
    /* [out] */ Int64* retvalue)
{
    VALIDATE_NOT_NULL(retvalue);
    *retvalue = NativeGetIfaceStat(iface, TYPE_TX_PACKETS);
    return NOERROR;
}

ECode CTrafficStats::GetRxPackets(
    /* [in] */ const String& iface,
    /* [out] */ Int64* retvalue)
{
    VALIDATE_NOT_NULL(retvalue);
    *retvalue= NativeGetIfaceStat(iface, TYPE_RX_PACKETS);
    return NOERROR;
}

ECode CTrafficStats::GetTxBytes(
    /* [in] */ const String& iface,
    /* [out] */ Int64* retvalue)
{
    VALIDATE_NOT_NULL(retvalue);
    *retvalue = NativeGetIfaceStat(iface, TYPE_TX_BYTES);
    return NOERROR;
}

ECode CTrafficStats::GetRxBytes(
    /* [in] */ const String& iface,
    /* [out] */ Int64* retvalue)
{
    VALIDATE_NOT_NULL(retvalue);
    *retvalue = NativeGetIfaceStat(iface, TYPE_RX_BYTES);
    return NOERROR;
}

ECode CTrafficStats::GetTotalTxPackets(
    /* [out] */ Int64* result)
{
    VALIDATE_NOT_NULL(result);
    *result = NativeGetTotalStat(TYPE_TX_PACKETS);
    return NOERROR;
}

ECode CTrafficStats::GetTotalRxPackets(
    /* [out] */ Int64* result)
{
    VALIDATE_NOT_NULL(result);
    *result = NativeGetTotalStat(TYPE_RX_PACKETS);
    return NOERROR;
}

ECode CTrafficStats::GetTotalTxBytes(
    /* [out] */ Int64* retvalue)
{
    VALIDATE_NOT_NULL(retvalue);
    *retvalue = NativeGetTotalStat(TYPE_TX_BYTES);
    return NOERROR;
}

ECode CTrafficStats::GetTotalRxBytes(
    /* [out] */ Int64* retvalue)
{
    VALIDATE_NOT_NULL(retvalue);
    *retvalue = NativeGetTotalStat(TYPE_RX_BYTES);
    return NOERROR;
}

Int64 CTrafficStats::NativeGetTotalStat(
    /* [in] */ Int32 type)
{
    return GetIfaceStatType(NULL, (IfaceStatType) type);
}

Int64 CTrafficStats::NativeGetIfaceStat(
    /* [in] */ const String& iface,
    /* [in] */ Int32 type)
{
     const char* tempstring=iface.string();
     UInt64 tempstat = GetIfaceStatType(tempstring, (IfaceStatType) type);
     Int64 stat=tempstat;

     return stat;
}

Int64 CTrafficStats::GetIfaceStatType(
    /* [in] */ const char* iface,
    /* [in] */ IfaceStatType type)
{
       struct IfaceStat* stat;
       memset(stat, 0, sizeof(IfaceStat));

      //anthony check
       if (ParseIfaceStat(iface, stat)) {
           return VALUE_UNKNOWN;
       }

       switch (type) {
           case RX_BYTES:
               return stat->rxBytes;
           case RX_PACKETS:
               return stat->rxPackets;
           case TX_BYTES:
               return stat->txBytes;
           case TX_PACKETS:
               return stat->txPackets;
           default:
               return VALUE_UNKNOWN;
       }
}

Int32 CTrafficStats::ParseIfaceStat(
    /* [in] */ const char* iface,
    /* [in] */ IfaceStat* stat)
{
       FILE *fp = fopen(IFACE_STAT_ALL, "r");
       if (!fp) {
           return errno;
       }

       char buffer[256];
       char cur_iface[32];
      Int32 active;
       UInt64 rxBytes, rxPackets, txBytes, txPackets, devRxBytes, devRxPackets, devTxBytes,
               devTxPackets;

       while (fgets(buffer, 256, fp) != NULL) {
           if (sscanf(buffer, "%31s %d %llu %llu %llu %llu %llu %llu %llu %llu", cur_iface, &active,
                      &rxBytes, &rxPackets, &txBytes, &txPackets, &devRxBytes, &devRxPackets,
                      &devTxBytes, &devTxPackets) != 10) {
               continue;
           }

           if (!iface || !strcmp(iface, cur_iface)) {
               stat->rxBytes += rxBytes;
               stat->rxPackets += rxPackets;
               stat->txBytes += txBytes;
               stat->txPackets += txPackets;

               if (active) {
                   stat->rxBytes += devRxBytes;
                   stat->rxPackets += devRxPackets;
                   stat->txBytes += devTxBytes;
                   stat->txPackets += devTxPackets;
               }
           }
       }

       fclose(fp);
       return 0;
}

Int64 CTrafficStats::GetUidBytes(
    /* [in] */ Int32 uid,
    /* [in] */ Int32 tx_or_rx,
    /* [in] */ Int32 tcp_or_udp)
{
       char tcp_filename[80], udp_filename[80];
       Int64 tcp_bytes = -1, udp_bytes = -1, total_bytes = -1;

       switch (tx_or_rx) {
           case TX:
               sprintf(tcp_filename, "/proc/uid_stat/%d/tcp_snd", uid);
               sprintf(udp_filename, "/proc/uid_stat/%d/udp_snd", uid);
               break;
           case RX:
               sprintf(tcp_filename, "/proc/uid_stat/%d/tcp_rcv", uid);
               sprintf(udp_filename, "/proc/uid_stat/%d/udp_rcv", uid);
               break;
           default:
               return -1;
       }


       switch (tcp_or_udp) {
           case TCP:
               tcp_bytes = ReadNumber(tcp_filename);
               total_bytes = (tcp_bytes >= 0) ? tcp_bytes : -1;
               break;
           case UDP:
               udp_bytes = ReadNumber(udp_filename);
               total_bytes = (udp_bytes >= 0) ? udp_bytes : -1;
               break;
           case TCP_AND_UDP:
               tcp_bytes = ReadNumber(tcp_filename);
               total_bytes += (tcp_bytes >= 0 ? tcp_bytes : 0);

               udp_bytes = ReadNumber(udp_filename);
               total_bytes += (udp_bytes >= 0 ? udp_bytes : 0);
               break;
           default:
               return -1;
       }

       return total_bytes;
}

Int64 CTrafficStats::GetUidPkts(
    /* [in] */ Int32 uid,
    /* [in] */ Int32 tx_or_rx,
    /* [in] */ Int32 tcp_or_udp)
{
       char tcp_filename[80], udp_filename[80];
       Int64 tcp_pkts = -1, udp_pkts = -1, total_pkts = -1;

       switch (tx_or_rx) {
           case TX:
               sprintf(tcp_filename, "/proc/uid_stat/%d/tcp_snd_pkt", uid);
               sprintf(udp_filename, "/proc/uid_stat/%d/udp_snd_pkt", uid);
               break;
           case RX:
               sprintf(tcp_filename, "/proc/uid_stat/%d/tcp_rcv_pkt", uid);
               sprintf(udp_filename, "/proc/uid_stat/%d/udp_rcv_pkt", uid);
               break;
           default:
               return -1;
       }


       switch (tcp_or_udp) {
           case TCP:
               tcp_pkts = ReadNumber(tcp_filename);
               total_pkts = (tcp_pkts >= 0) ? tcp_pkts : -1;
               break;
           case UDP:
               udp_pkts = ReadNumber(udp_filename);
               total_pkts = (udp_pkts >= 0) ? udp_pkts : -1;
               break;
           case TCP_AND_UDP:
               tcp_pkts = ReadNumber(tcp_filename);
               total_pkts += (tcp_pkts >= 0 ? tcp_pkts : 0);

               udp_pkts = ReadNumber(udp_filename);
               total_pkts += (udp_pkts >= 0 ? udp_pkts : 0);
               break;
           default:
               return -1;
       }

       return total_pkts;
}

ECode CTrafficStats::GetUidTxPackets(
    /* [in] */ Int32 uid,
    /* [out] */ Int64* retvalue)
{
    VALIDATE_NOT_NULL(retvalue);
    *retvalue = GetUidPkts(uid, TX, TCP_AND_UDP);
    return NOERROR;
}

ECode CTrafficStats::GetUidRxPackets(
    /* [in] */ Int32 uid,
    /* [out] */ Int64* retvalue)
{
    VALIDATE_NOT_NULL(retvalue);
    *retvalue = GetUidPkts(uid, RX, TCP_AND_UDP);
    return NOERROR;
}

ECode CTrafficStats::GetUidTcpTxBytes(
    /* [in] */ Int32 uid,
    /* [out] */ Int64* retvalue)
{
    VALIDATE_NOT_NULL(retvalue);
    *retvalue = GetUidPkts(uid, RX, TCP_AND_UDP);
    return NOERROR;
}

ECode CTrafficStats::GetUidTcpRxBytes(
    /* [in] */ Int32 uid,
    /* [out] */ Int64* retvalue)
{
    VALIDATE_NOT_NULL(retvalue);
    *retvalue = GetUidBytes(uid, RX, TCP);
    return NOERROR;
}

ECode CTrafficStats::GetUidUdpTxBytes(
    /* [in] */ Int32 uid,
    /* [out] */ Int64* retvalue)
{
    VALIDATE_NOT_NULL(retvalue);
    *retvalue = GetUidBytes(uid, TX, UDP);
    return NOERROR;
}

ECode CTrafficStats::GetUidUdpRxBytes(
    /* [in] */ Int32 uid,
    /* [out] */ Int64* retvalue)
{
    VALIDATE_NOT_NULL(retvalue);
    *retvalue = GetUidBytes(uid, RX, UDP);
    return NOERROR;
}

ECode CTrafficStats::GetUidTcpTxSegments(
    /* [in] */ Int32 uid,
    /* [out] */ Int64* retvalue)
{
    VALIDATE_NOT_NULL(retvalue);
    *retvalue = GetUidPkts(uid, TX, TCP);
    return NOERROR;
}

ECode CTrafficStats::GetUidTcpRxSegments(
    /* [in] */ Int32 uid,
    /* [out] */ Int64* retvalue)
{
    VALIDATE_NOT_NULL(retvalue);
    *retvalue = GetUidPkts(uid, RX, TCP);
    return NOERROR;
}

ECode CTrafficStats::GetUidUdpTxPackets(
    /* [in] */ Int32 uid,
    /* [out] */ Int64* retvalue)
{
    VALIDATE_NOT_NULL(retvalue);
    *retvalue = GetUidPkts(uid, TX, UDP);
    return NOERROR;
}

ECode CTrafficStats::GetUidUdpRxPackets(
    /* [in] */ Int32 uid,
    /* [out] */ Int64* retvalue)
{
    VALIDATE_NOT_NULL(retvalue);
    *retvalue = GetUidPkts(uid, RX, UDP);
    return NOERROR;
}

AutoPtr<INetworkStats> CTrafficStats::GetDataLayerSnapshotForUid(
    /* [in] */ IContext* context)
{
    Int32 uid = Process::MyUid();
    AutoPtr<INetworkStats> result;
    GetStatsService()->GetDataLayerSnapshotForUid(uid, (INetworkStats**)&result);
    return result;
}

} // namespace net
} // namepsace Droid
} // namespace Elastos
