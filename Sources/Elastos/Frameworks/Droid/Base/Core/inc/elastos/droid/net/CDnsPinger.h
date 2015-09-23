
#ifndef __ELASTOS_DROID_NET_CDNSPINGER_H__
#define __ELASTOS_DROID_NET_CDNSPINGER_H__

#include "_Elastos_Droid_Net_CDnsPinger.h"
#include "elastos/droid/ext/frameworkdef.h"
#include "elastos/droid/os/SystemClock.h"
#include "elastos/droid/os/Handler.h"

#include <elastos/utility/etl/List.h>

using Elastos::Net::IDatagramSocket;
using Elastos::Net::IInetAddress;
using Elastos::Utility::Etl::List;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Os::ILooper;
using Elastos::Droid::Os::Handler;
using Elastos::Droid::Os::IHandler;
using Elastos::Droid::Os::SystemClock;

using Elastos::Utility::IRandom;
using Elastos::Utility::Concurrent::Atomic::IAtomicInteger32;
using Elastos::Utility::IList;

namespace Elastos {
namespace Droid {
namespace Net {

CarClass(CDnsPinger)
    , public Handler
    , public IDnsPinger
{
private:
        class DnsArg;

public:
    CAR_OBJECT_DECL()

    CAR_INTERFACE_DECL()

    CDnsPinger();

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ const String& tagstr,
        /* [in] */ ILooper* looper,
        /* [in] */ IHandler* target,
        /* [in] */ Int32 connectionType);

    CARAPI HandleMessage(
        /* [in] */ IMessage* msg);

    void HandleActionPingDNS(
        /* [in] */ Int32 arg1,
        /* [in] */ Int32 arg2,
        /* [in] */ DnsArg* dnsArg);

    void HandleActionListenForResponse(
        /* [in] */ Int32 arg1);

    void HandleActionCancelAllPings();

    /**
     * Returns a list of DNS addresses, coming from either the link properties of the
     * specified connection or the default system DNS if the link properties has no dnses.
     * @return a non-empty non-null list
     */
    CARAPI GetDnsList(
        /* [out] */ IList** dnslist);

    /**
     * Send a ping.  The response will come via a {@link #DNS_PING_RESULT} to the handler
     * specified at creation.
     * @param dns address of dns server to ping
     * @param timeout timeout for ping
     * @return an ID field, which will also be included in the {@link #DNS_PING_RESULT} message.
     */
    CARAPI PingDnsAsync(
        /* [in] */ IInetAddress* dns,
        /* [in] */ Int32 timeout,
        /* [in] */ Int32 delay,
        /* [out] */ Int32* result);

    CARAPI CancelPings();

private:
    class ActivePing
        : public Object
    {
    public:
        ActivePing() {
            mStart = SystemClock::GetElapsedRealtime();
        }

    public:
        AutoPtr<IDatagramSocket> mSocket;
        Int32 mInternalId;
        Int16 mPacketId;
        Int32 mTimeout;
        Int32 mResult;
        Int64 mStart;// = SystemClock::GetElapsedRealtime();
    };

    /* Message argument for ACTION_PING_DNS */
    class DnsArg
        : public Object
    {
    public:
        DnsArg(
          /* [in] */ IInetAddress* dns,
          /* [in] */ Int32 seq);

    public:
        AutoPtr<IInetAddress> mDns;
        Int32 mSeq;
    };

    CARAPI SendResponse(
        /* [in] */ Int32 internalId,
        /* [in] */ Int32 externalId,
        /* [in] */ Int32 responseVal);

    CARAPI GetCurrentLinkProperties(
        /* [out] */ ILinkProperties** result);

    CARAPI GetDefaultDns(
        /* [out] */ IInetAddress** result);

    CARAPI Log(
        /* [in] */ const String& s);

    CARAPI Loge(
        /* [in] */ const String& s);

    static const Boolean DBG;

    static const Int32 RECEIVE_POLL_INTERVAL_MS;
    static const Int32 DNS_PORT;

    /** Short socket timeout so we don't block one any 'receive' call */
    static const Int32 SOCKET_TIMEOUT_MS;

    /** Used to generate IDs */
    static const AutoPtr<IRandom> sRandom;
    static const AutoPtr<IAtomicInteger32> sCounter;

    AutoPtr<IConnectivityManager> mConnectivityManager;
    AutoPtr<IContext> mContext;
    Int32 mConnectionType;
    AutoPtr<IHandler> mTarget;
    AutoPtr<IList> mDefaultDns;
    String TAG;

    //Invalidates old dns requests upon a cancel
    AutoPtr<IAtomicInteger32> mCurrentToken;

    static const Int32 BASE;

    /**
     * Send a new ping via a socket.  arg1 is ID, arg2 is timeout, obj is InetAddress to ping
     */
    static const Int32 ACTION_PING_DNS;
    static const Int32 ACTION_LISTEN_FOR_RESPONSE;
    static const Int32 ACTION_CANCEL_ALL_PINGS;

    AutoPtr<IList> mActivePings;
    Int32 mEventCounter;

    static const Byte mDnsQuery[];
};

} // namespace Net
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_NET_CDNSPINGER_H__
