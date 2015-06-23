
#ifndef __CTRAFFICSTATS_H__
#define __CTRAFFICSTATS_H__

#include "_CTrafficStats.h"
#include "ext/frameworkext.h"
#include "elastos/Mutex.h"
#include "content/Context.h"


using Elastos::Droid::Content::IContext;
using Elastos::Droid::Net::INetworkStats;
using Elastos::Net::ISocket;

namespace Elastos {
namespace Droid {
namespace Net {


struct IfaceStat {
    UInt64 rxBytes;
    UInt64 rxPackets;
    UInt64 txBytes;
    UInt64 txPackets;
};

enum IfaceStatType {
    RX_BYTES = 0,
    RX_PACKETS = 1,
    TX_BYTES = 2,
    TX_PACKETS = 3
};

enum Tx_Rx {
    TX,
    RX
};

enum Tcp_Udp {
    TCP,
    UDP,
    TCP_AND_UDP
};



CarClass(CTrafficStats)
{
public:

    CARAPI constructor();

    /**
     * Get the number of bytes sent through the network for this UID.
     * The statistics are across all interfaces.
     *
     * {@see android.os.Process#myUid()}.
     *
     * @param uid The UID of the process to examine.
     * @return number of bytes.  If the statistics are not supported by this device,
     * {@link #UNSUPPORTED} will be returned.
     */
    CARAPI GetUidTxBytes(
        /* [in] */ Int32 uid,
        /* [out] */ Int64* result);


    /**
     * Get the number of bytes received through the network for this UID.
     * The statistics are across all interfaces.
     *
     * {@see android.os.Process#myUid()}.
     *
     * @param uid The UID of the process to examine.
     * @return number of bytes
     */
     CARAPI GetUidRxBytes(
        /* [in] */ Int32 uid,
        /* [out] */ Int64* result);

    /**
     * Set active tag to use when accounting {@link Socket} traffic originating
     * from the current thread. Only one active tag per thread is supported.
     * <p>
     * Changes only take effect during subsequent calls to
     * {@link #tagSocket(Socket)}.
     * <p>
     * Tags between {@code 0xFFFFFF00} and {@code 0xFFFFFFFF} are reserved and
     * used internally by system services like {@link DownloadManager} when
     * performing traffic on behalf of an application.
     */
    CARAPI SetThreadStatsTag(
        /* [in] */ Int32 tag);

    /**
     * Get the active tag used when accounting {@link Socket} traffic originating
     * from the current thread. Only one active tag per thread is supported.
     * {@link #tagSocket(Socket)}.
     */
    CARAPI GetThreadStatsTag(
        /* [out] */ Int32* result);

    CARAPI ClearThreadStatsTag();

     /**
     * Set specific UID to use when accounting {@link Socket} traffic
     * originating from the current thread. Designed for use when performing an
     * operation on behalf of another application.
     * <p>
     * Changes only take effect during subsequent calls to
     * {@link #tagSocket(Socket)}.
     * <p>
     * To take effect, caller must hold
     * {@link android.Manifest.permission#UPDATE_DEVICE_STATS} permission.
     *
     * {@hide}
     */
    CARAPI SetThreadStatsUid(
        /* [in] */ Int32 uid);


    CARAPI ClearThreadStatsUid();

    /**
     * Tag the given {@link Socket} with any statistics parameters active for
     * the current thread. Subsequent calls always replace any existing
     * parameters. When finished, call {@link #untagSocket(Socket)} to remove
     * statistics parameters.
     *
     * @see #setThreadStatsTag(int)
     * @see #setThreadStatsUid(int)
     */
    CARAPI TagSocket(
        /* [in] */ ISocket* socket);

    /**
     * Remove any statistics parameters from the given {@link Socket}.
     */
    CARAPI UntagSocket(
        /* [in] */ ISocket* socket);

     /**
     * Start profiling data usage for current UID. Only one profiling session
     * can be active at a time.
     *
     * @hide
     */
    CARAPI StartDataProfiling(
        /* [in] */ IContext* context);

    /**
     * Stop profiling data usage for current UID.
     *
     * @return Detailed {@link NetworkStats} of data that occurred since last
     *         {@link #startDataProfiling(Context)} call.
     * @hide
     */
    CARAPI StopDataProfiling(
        /* [in] */  IContext* context,
        /* [out] */ INetworkStats** Retvalue);

    /**
     * Increment count of network operations performed under the accounting tag
     * currently active on the calling thread. This can be used to derive
     * bytes-per-operation.
     *
     * @param operationCount Number of operations to increment count by.
     */
    CARAPI IncrementOperationCount(
        /* [in] */ Int32 operationCount);

    /**
     * Increment count of network operations performed under the given
     * accounting tag. This can be used to derive bytes-per-operation.
     *
     * @param tag Accounting tag used in {@link #setThreadStatsTag(int)}.
     * @param operationCount Number of operations to increment count by.
     */
    CARAPI IncrementOperationCount(
        /* [in] */ Int32 tag,
        /* [in] */ Int32 operationCount);

    CARAPI CloseQuietly(
         /* [in] */ INetworkStatsSession* session);

    /**
     * Get the total number of packets transmitted through the mobile interface.
     *
     * @return number of packets.  If the statistics are not supported by this device,
     * {@link #UNSUPPORTED} will be returned.
     */
    CARAPI GetMobileTxPackets(
         /* [out] */ Int64* result);

    /**
     * Get the total number of packets received through the mobile interface.
     *
     * @return number of packets.  If the statistics are not supported by this device,
     * {@link #UNSUPPORTED} will be returned.
     */
    CARAPI GetMobileRxPackets(
         /* [out] */ Int64* result);

    /**
     * Get the total number of bytes transmitted through the mobile interface.
     *
     * @return number of bytes.  If the statistics are not supported by this device,
     * {@link #UNSUPPORTED} will be returned.
     */
    CARAPI GetMobileTxBytes(
         /* [out] */ Int64* result);


    /**
     * Get the total number of bytes received through the mobile interface.
     *
     * @return number of bytes.  If the statistics are not supported by this device,
     * {@link #UNSUPPORTED} will be returned.
     */
    CARAPI GetMobileRxBytes(
         /* [out] */ Int64* result);



    /**
     * Get the total number of packets transmitted through the specified interface.
     *
     * @return number of packets.  If the statistics are not supported by this interface,
     * {@link #UNSUPPORTED} will be returned.
     * @hide
     */
    CARAPI GetTxPackets(
        /* [in] */ const String& iface,
        /* [out] */ Int64* Retvalue);

    /**
     * Get the total number of packets received through the specified interface.
     *
     * @return number of packets.  If the statistics are not supported by this interface,
     * {@link #UNSUPPORTED} will be returned.
     * @hide
     */
    CARAPI GetRxPackets(
        /* [in] */ const String& iface,
        /* [out] */ Int64* Retvalue);

    /**
     * Get the total number of bytes transmitted through the specified interface.
     *
     * @return number of bytes.  If the statistics are not supported by this interface,
     * {@link #UNSUPPORTED} will be returned.
     * @hide
     */
    CARAPI GetTxBytes(
        /* [in] */ const String& iface,
        /* [out] */ Int64* Retvalue);

    /**
     * Get the total number of bytes received through the specified interface.
     *
     * @return number of bytes.  If the statistics are not supported by this interface,
     * {@link #UNSUPPORTED} will be returned.
     * @hide
     */
    CARAPI GetRxBytes(
        /* [in] */ const String& iface,
        /* [out] */ Int64* Retvalue);

    /**
     * Get the total number of packets sent through all network interfaces.
     *
     * @return the number of packets.  If the statistics are not supported by this device,
     * {@link #UNSUPPORTED} will be returned.
     */
    CARAPI GetTotalTxPackets(
        /* [out] */ Int64* result);

    /**
     * Get the total number of packets received through all network interfaces.
     *
     * @return number of packets.  If the statistics are not supported by this device,
     * {@link #UNSUPPORTED} will be returned.
     */
    CARAPI GetTotalRxPackets(
        /* [out] */ Int64* result);

    /**
     * Get the total number of bytes sent through all network interfaces.
     *
     * @return number of bytes.  If the statistics are not supported by this device,
     * {@link #UNSUPPORTED} will be returned.
     */
    CARAPI GetTotalTxBytes(
        /* [out] */ Int64* Retvalue);

    /**
     * Get the total number of bytes received through all network interfaces.
     *
     * @return number of bytes.  If the statistics are not supported by this device,
     * {@link #UNSUPPORTED} will be returned.
     */
    CARAPI GetTotalRxBytes(
        /* [out] */ Int64* Retvalue);


    /**
     * Get the number of packets (TCP segments + UDP) sent through
     * the network for this UID.
     * The statistics are across all interfaces.
     *
     * {@see android.os.Process#myUid()}.
     *
     * @param uid The UID of the process to examine.
     * @return number of packets.
     * If the statistics are not supported by this device,
     * {@link #UNSUPPORTED} will be returned.
     */
    CARAPI GetUidTxPackets(
        /* [in] */ Int32 uid,
        /* [out] */ Int64* Retvalue);

    /**
     * Get the number of packets (TCP segments + UDP) received through
     * the network for this UID.
     * The statistics are across all interfaces.
     *
     * {@see android.os.Process#myUid()}.
     *
     * @param uid The UID of the process to examine.
     * @return number of packets
     */
    CARAPI GetUidRxPackets(
        /* [in] */ Int32 uid,
        /* [out] */ Int64* Retvalue);

    /**
     * Get the number of TCP payload bytes sent for this UID.
     * This total does not include protocol and control overheads at
     * the transport and the lower layers of the networking stack.
     * The statistics are across all interfaces.
     *
     * {@see android.os.Process#myUid()}.
     *
     * @param uid The UID of the process to examine.
     * @return number of bytes.  If the statistics are not supported by this device,
     * {@link #UNSUPPORTED} will be returned.
     */
    CARAPI GetUidTcpTxBytes(
        /* [in] */ Int32 uid,
        /* [out] */ Int64* Retvalue);

    /**
     * Get the number of TCP payload bytes received for this UID.
     * This total does not include protocol and control overheads at
     * the transport and the lower layers of the networking stack.
     * The statistics are across all interfaces.
     *
     * {@see android.os.Process#myUid()}.
     *
     * @param uid The UID of the process to examine.
     * @return number of bytes.  If the statistics are not supported by this device,
     * {@link #UNSUPPORTED} will be returned.
     */
    CARAPI GetUidTcpRxBytes(
        /* [in] */ Int32 uid,
        /* [out] */ Int64* Retvalue);

    /**
     * Get the number of UDP payload bytes sent for this UID.
     * This total does not include protocol and control overheads at
     * the transport and the lower layers of the networking stack.
     * The statistics are across all interfaces.
     *
     * {@see android.os.Process#myUid()}.
     *
     * @param uid The UID of the process to examine.
     * @return number of bytes.  If the statistics are not supported by this device,
     * {@link #UNSUPPORTED} will be returned.
     */
    CARAPI GetUidUdpTxBytes(
        /* [in] */ Int32 uid,
        /* [out] */ Int64* Retvalue);

    /**
     * Get the number of UDP payload bytes received for this UID.
     * This total does not include protocol and control overheads at
     * the transport and the lower layers of the networking stack.
     * The statistics are across all interfaces.
     *
     * {@see android.os.Process#myUid()}.
     *
     * @param uid The UID of the process to examine.
     * @return number of bytes.  If the statistics are not supported by this device,
     * {@link #UNSUPPORTED} will be returned.
     */
    CARAPI GetUidUdpRxBytes(
        /* [in] */ Int32 uid,
        /* [out] */ Int64* Retvalue);

    /**
     * Get the number of TCP segments sent for this UID.
     * Does not include TCP control packets (SYN/ACKs/FIN/..).
     * The statistics are across all interfaces.
     *
     * {@see android.os.Process#myUid()}.
     *
     * @param uid The UID of the process to examine.
     * @return number of TCP segments.  If the statistics are not supported by this device,
     * {@link #UNSUPPORTED} will be returned.
     */
    CARAPI GetUidTcpTxSegments(
        /* [in] */ Int32 uid,
        /* [out] */ Int64* Retvalue);

    /**
     * Get the number of TCP segments received for this UID.
     * Does not include TCP control packets (SYN/ACKs/FIN/..).
     * The statistics are across all interfaces.
     *
     * {@see android.os.Process#myUid()}.
     *
     * @param uid The UID of the process to examine.
     * @return number of TCP segments.  If the statistics are not supported by this device,
     * {@link #UNSUPPORTED} will be returned.
     */
    CARAPI GetUidTcpRxSegments(
        /* [in] */ Int32 uid,
        /* [out] */ Int64* Retvalue);

    /**
     * Get the number of UDP packets sent for this UID.
     * Includes DNS requests.
     * The statistics are across all interfaces.
     *
     * {@see android.os.Process#myUid()}.
     *
     * @param uid The UID of the process to examine.
     * @return number of packets.  If the statistics are not supported by this device,
     * {@link #UNSUPPORTED} will be returned.
     */
    CARAPI GetUidUdpTxPackets(
        /* [in] */ Int32 uid,
        /* [out] */ Int64* Retvalue);

    /**
     * Get the number of UDP packets received for this UID.
     * Includes DNS responses.
     * The statistics are across all interfaces.
     *
     * {@see android.os.Process#myUid()}.
     *
     * @param uid The UID of the process to examine.
     * @return number of packets.  If the statistics are not supported by this device,
     * {@link #UNSUPPORTED} will be returned.
     */
    CARAPI GetUidUdpRxPackets(
        /* [in] */ Int32 uid,
        /* [out] */ Int64* Retvalue);



private:
    // TODO: Add your private member variables here.

    static Mutex sProfilingLock;
    static AutoPtr<INetworkStatsService> sStatsService;
    static INetworkStatsService* GetStatsService();

    /**
     * Snapshot of {@link NetworkStats} when the currently active profiling
     * session started, or {@code null} if no session active.
     *
     * @see #startDataProfiling(Context)
     * @see #stopDataProfiling(Context)
     */
    static INetworkStats* sActiveProfilingStart;

    //Native function
    static CARAPI_(Int64) GetIfaceStatType(
        /* [in] */ const char* iface,
        /* [in] */ IfaceStatType type);

    static CARAPI_(Int64) NativeGetIfaceStat(
        /* [in] */ const String& iface,
        /* [in] */ Int32 type);

    static CARAPI_(Int64) NativeGetTotalStat(
        /* [in] */ Int32 type);

    static CARAPI_(Int32) ParseIfaceStat(
        /* [in] */ const char* iface,
        /* [in] */ IfaceStat* stat);

    static CARAPI_(Int64) GetUidBytes(
        /* [in] */ Int32 uid,
        /* [in] */ Int32 tx_or_rx,
        /* [in] */ Int32 tcp_or_udp);

    static CARAPI_(Int64) ReadNumber(
        /* [in] */ const char* filename);


    static CARAPI_(Int64) GetUidPkts(
        /* [in] */ Int32 uid,
        /* [in] */ Int32 tx_or_rx,
        /* [in] */ Int32 tcp_or_udp);

    /**
     * Return detailed {@link NetworkStats} for the current UID. Requires no
     * special permission.
     */
    static CARAPI_(AutoPtr<INetworkStats>) GetDataLayerSnapshotForUid(
        /* [in] */ IContext* context);

    //Native function

    const static  Int32 TYPE_RX_BYTES = 0;
    const static  Int32 TYPE_RX_PACKETS = 1;
    const static  Int32 TYPE_TX_BYTES = 2;
    const static  Int32 TYPE_TX_PACKETS = 3;

    static const String TAG;

};

} // namespace net
} // namepsace Droid
} // namespace Elasto

#endif // __CTRAFFICSTATS_H__
