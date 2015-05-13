
#ifndef __CTrafficStatsHelper_H__
#define __CTrafficStatsHelper_H__

#include "_CTrafficStats.h"
#include "elautptr.h"
#include "net/CTrafficStats.h"

namespace Elastos {
namespace Droid {
namespace Net {

CarClass(CTrafficStatsHelper)
{
public:
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
        /* [in] */ Socket socket);

    /**
     * Remove any statistics parameters from the given {@link Socket}.
     */
    CARAPI UntagSocket(
        /* [in] */ Socket socket);

     /**
     * Start profiling data usage for current UID. Only one profiling session
     * can be active at a time.
     *
     * @hide
     */
    CARAPI StartDataProfiling(
        /* [in] */ IContext context);

    /**
     * Stop profiling data usage for current UID.
     *
     * @return Detailed {@link NetworkStats} of data that occurred since last
     *         {@link #startDataProfiling(Context)} call.
     * @hide
     */
    CARAPI stopDataProfiling(
        /* [in] */  IContext context,
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
     //anthony
     // check overloading
    CARAPI IncrementOperationCount(
        /* [in] */ Int32 tag,
        /* [in] */ Int32 operationCount);

    CARAPI CloseQuietly(
         /* [in] */ INetworkStatsSession session);

    /**
     * Get the total number of packets transmitted through the mobile interface.
     *
     * @return number of packets.  If the statistics are not supported by this device,
     * {@link #UNSUPPORTED} will be returned.
     */
    CARAPI GetMobileTxPackets(
         /* [out] */ Int64* total);

    /**
     * Get the total number of packets received through the mobile interface.
     *
     * @return number of packets.  If the statistics are not supported by this device,
     * {@link #UNSUPPORTED} will be returned.
     */
    CARAPI GetMobileRxPackets(
         /* [out] */ Int64* total);

    /**
     * Get the total number of bytes transmitted through the mobile interface.
     *
     * @return number of bytes.  If the statistics are not supported by this device,
     * {@link #UNSUPPORTED} will be returned.
     */
    CARAPI GetMobileTxBytes(
         /* [out] */ Int64* total);

    /**
     * Get the total number of bytes received through the mobile interface.
     *
     * @return number of bytes.  If the statistics are not supported by this device,
     * {@link #UNSUPPORTED} will be returned.
     */
    CARAPI GetMobileRxBytes(
         /* [out] */ Int64* total);

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
        /* [out] */ Int64* Retvalue);

    /**
     * Get the total number of packets received through all network interfaces.
     *
     * @return number of packets.  If the statistics are not supported by this device,
     * {@link #UNSUPPORTED} will be returned.
     */
    CARAPI GetTotalRxPackets(
        /* [out] */ Int64* Retvalue);

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

};

} // namespace Net
} // namepsace Droid
} // namespace Elastos


#endif // __CTrafficStatsHelper_H__
