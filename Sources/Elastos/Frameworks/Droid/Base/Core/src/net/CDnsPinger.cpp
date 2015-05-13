
#include "net/CDnsPinger.h"
#include "net/NetworkUtils.h"
#include "net/CConnectivityManager.h"
#include "R.h"
#include "provider/Settings.h"
#include "os/SystemClock.h"
#include <elastos/Logger.h>

using Elastos::Net::IDatagramPacket;
using Elastos::Net::CDatagramPacket;
using Elastos::Net::CDatagramSocket;
using Elastos::Net::INetworkInterface;
using Elastos::Net::INetworkInterfaceHelper;
using Elastos::Net::CNetworkInterfaceHelper;
using Elastos::Core::CObjectContainer;
using Elastos::Utility::Logging::Logger;
using Elastos::Droid::Os::SystemClock;
using Elastos::Droid::Net::NetworkUtils;
using Elastos::Droid::Net::IConnectivityManager;
using Elastos::Droid::Net::CConnectivityManager;
using Elastos::Droid::Provider::Settings;
using Elastos::Droid::Provider::ISettingsGlobal;
using Elastos::Droid::Content::IContentResolver;
using Elastos::Droid::Content::Res::IResources;

namespace Elastos {
namespace Droid {
namespace Net {

const Boolean CDnsPinger::DBG = FALSE;
const Int32 CDnsPinger::RECEIVE_POLL_INTERVAL_MS = 200;
const Int32 CDnsPinger::DNS_PORT = 53;
const Int32 CDnsPinger::SOCKET_TIMEOUT_MS = 1;
const Int32 CDnsPinger::BASE = 0x00050000; //Protocol.BASE_DNS_PINGER;
const Int32 CDnsPinger::ACTION_PING_DNS = 0x00050001; // = BASE + 1;
const Int32 CDnsPinger::ACTION_LISTEN_FOR_RESPONSE = 0x00050002; // = BASE + 2;
const Int32 CDnsPinger::ACTION_CANCEL_ALL_PINGS = 0x00050003; // = BASE + 3;
const AutoPtr<IRandom> CDnsPinger::sRandom = NULL;
const AutoPtr<IAtomicInteger32> CDnsPinger::sCounter = NULL;
const Byte CDnsPinger::mDnsQuery[] = {
    0, 0, // [0-1] is for ID (will set each time)
    1, 0, // [2-3] are flags.  Set byte[2] = 1 for recursion desired (RD) on.  Currently on.
    0, 1, // [4-5] bytes are for number of queries (QCOUNT)
    0, 0, // [6-7] unused count field for dns response packets
    0, 0, // [8-9] unused count field for dns response packets
    0, 0, // [10-11] unused count field for dns response packets
    3, 'w', 'w', 'w',
    6, 'g', 'o', 'o', 'g', 'l', 'e',
    3, 'c', 'o', 'm',
    0,    // null terminator of address (also called empty TLD)
    0, 1, // QTYPE, set to 1 = A (host address)
    0, 1  // QCLASS, set to 1 = IN (internet)
};


CAR_INTERFACE_IMPL(CDnsPinger::DnsArg, IInterface)

CDnsPinger::DnsArg::DnsArg(
    /* [in] */ IInetAddress* dns,
    /* [in] */ Int32 seq)
{
    mDns = dns;
    mSeq = seq;
}

IHANDLER_METHODS_IMPL(CDnsPinger, Handler)

PInterface CDnsPinger::Probe(
    /* [in] */ REIID riid)
{
    return _CDnsPinger::Probe(riid);
}

CDnsPinger::CDnsPinger()
    : mConnectionType(0)
{
}

ECode CDnsPinger::constructor(
    /* [in] */ IContext* context,
    /* [in] */ const String& tagstr,
    /* [in] */ ILooper* looper,
    /* [in] */ IHandler* target,
    /* [in] */ Int32 connectionType)
{
    Handler::Init(looper);

    TAG = tagstr;
    mContext = context;
    mTarget = target;
    mConnectionType = connectionType;
    Boolean isValid;
    CConnectivityManager::IsNetworkTypeValid(connectionType, &isValid);
    if (!isValid) {
        Logger::E(TAG, "Invalid connectionType in constructor: %d", connectionType);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    AutoPtr<IInetAddress> defaultDns;
    GetDefaultDns((IInetAddress**)&defaultDns);
    mDefaultDns.PushBack(defaultDns);
    mEventCounter = 0;
    return NOERROR;
}

ECode CDnsPinger::HandleMessage(
    /* [in] */ IMessage* msg)
{
    Int32 what, arg1, arg2;
    msg->GetWhat(&what);
    msg->GetArg1(&arg1);
    msg->GetArg2(&arg2);
    AutoPtr<IInterface> obj;
    msg->GetObj((IInterface**)&obj);

    switch(what) {
        case ACTION_PING_DNS: {
            DnsArg* arg = (DnsArg*)obj.Get();
            HandleActionPingDNS(arg1, arg2, arg);
            break;
        }
        case ACTION_LISTEN_FOR_RESPONSE: {
            HandleActionListenForResponse(arg1);
            break;
        }
        case ACTION_CANCEL_ALL_PINGS:
            HandleActionCancelAllPings();
            break;

    }

    return NOERROR;
}

void CDnsPinger::HandleActionPingDNS(
    /* [in] */ Int32 arg1,
    /* [in] */ Int32 arg2,
    /* [in] */ DnsArg* dnsArg)
{
    assert(dnsArg != NULL);
    Int32 val;
    mCurrentToken->Get(&val);
    if (dnsArg->mSeq != val) {
        return;
    }

//  try {
    AutoPtr<ActivePing> newActivePing = new ActivePing();
    AutoPtr<IInetAddress> dnsAddress = dnsArg->mDns;
    newActivePing->mInternalId = arg1;
    newActivePing->mTimeout = arg2;
    CDatagramSocket::New((IDatagramSocket**)&(newActivePing->mSocket));
    //Set some socket properties
    newActivePing->mSocket->SetSoTimeout(SOCKET_TIMEOUT_MS);

    //Try to bind but continue ping if bind fails
    AutoPtr<ILinkProperties> linkProp;
    GetCurrentLinkProperties((ILinkProperties**)&linkProp);
    String name;
    linkProp->GetInterfaceName(&name);
    AutoPtr<INetworkInterfaceHelper> netInterfaceHelper;
    CNetworkInterfaceHelper::AcquireSingleton((INetworkInterfaceHelper**)&netInterfaceHelper);
    AutoPtr<INetworkInterface> netInterface;
    netInterfaceHelper->GetByName(name, (INetworkInterface**)&netInterface);
    ECode ec = newActivePing->mSocket->SetNetworkInterface(netInterface);
    if (FAILED(ec)) {
        //loge("sendDnsPing::Error binding to socket " + e);
    }

    Int32 num;
    sRandom->NextInt32(&num);
    newActivePing->mPacketId = (Int16)num;
    AutoPtr< ArrayOf<Byte> > buf = ArrayOf<Byte>::Alloc(sizeof(mDnsQuery));
    for (Int32 i = 0; i < 32; i++) {
        (*buf)[i] = mDnsQuery[i];
    }
    (*buf)[0] = (Byte) (newActivePing->mPacketId >> 8);
    (*buf)[1] = (Byte) newActivePing->mPacketId;

//  Send the DNS query
    AutoPtr<IDatagramPacket> packet;
    CDatagramPacket::New(buf, buf->GetLength(), dnsAddress, DNS_PORT, (IDatagramPacket**)&packet);
    if (DBG) {
        //Logger::I(TAG, "Sending a ping " + newActivePing->mInternalId +
        //" to " + dnsAddress->GetHostAddress()
        //+ " with packetId " + newActivePing::packetId + ".");
    }

    newActivePing->mSocket->Send(packet);
    mActivePings.PushBack(newActivePing);
    mEventCounter++;

    AutoPtr<IMessage> msg;
    ObtainMessageEx2(ACTION_LISTEN_FOR_RESPONSE, mEventCounter, 0, (IMessage**)&msg);
    Boolean result;
    SendMessageDelayed(msg, RECEIVE_POLL_INTERVAL_MS, &result);
//   } catch (IOException e) {
//   SendResponse(msg::arg1, -9999, SOCKET_EXCEPTION);
//  }
}

void CDnsPinger::HandleActionListenForResponse(
    /* [in] */ Int32 arg1)
{
    if (arg1 != mEventCounter) {
        return;
    }

    List< AutoPtr<ActivePing> >::Iterator iter;
    for (iter = mActivePings.Begin(); iter != mActivePings.End(); ++iter) {
        AutoPtr<ActivePing> curPing = *iter;
    //   try {
        /** Each socket will block for {@link #SOCKET_TIMEOUT_MS} in receive() */
        AutoPtr< ArrayOf<Byte> > responseBuf = ArrayOf<Byte>::Alloc(2);
        AutoPtr<IDatagramPacket> replyPacket;
        CDatagramPacket::New(responseBuf, 2, (IDatagramPacket**)&replyPacket);

        curPing->mSocket->Receive(replyPacket);
        //Check that ID field matches (we're throwing out the rest of the packet)
        if ((*responseBuf)[0] == (Byte) (curPing->mPacketId >> 8) &&
           (*responseBuf)[1] == (Byte) curPing->mPacketId) {
           curPing->mResult = (Int32) (SystemClock::GetElapsedRealtime() - curPing->mStart);
        } else {
            if (DBG) {
                Logger::I(TAG, "response ID didn't match, ignoring packet");
            }
        }
    //     } catch (SocketTimeoutException e) {
    //         A timeout here doesn't mean anything - squelsh this exception
    //     } catch (Exception e) {
    //         if (DBG) {
    //             log("DnsPinger.pingDns got socket exception: " + e);
    //         }
    //         curPing::result = SOCKET_EXCEPTION;
    //     }
    }

    iter = mActivePings.Begin();
    while (iter != mActivePings.End()) {
        AutoPtr<ActivePing> curPing = *iter;
        if (curPing->mResult != NULL) {
            SendResponse(curPing->mInternalId, curPing->mPacketId, curPing->mResult);
            curPing->mSocket->Close();
            iter = mActivePings.Erase(iter);
        } else if (SystemClock::GetElapsedRealtime() > curPing->mStart + curPing->mTimeout) {
            SendResponse(curPing->mInternalId, curPing->mPacketId, IDnsPinger::_TIMEOUT);
            curPing->mSocket->Close();
            iter = mActivePings.Erase(iter);
        }

        else {
            ++iter;
        }
    }

    if (!mActivePings.IsEmpty()) {
        AutoPtr<IMessage> msg;
        ObtainMessageEx2(ACTION_LISTEN_FOR_RESPONSE, mEventCounter, 0, (IMessage**)&msg);
        Boolean result;
        SendMessageDelayed(msg, RECEIVE_POLL_INTERVAL_MS, &result);
    }
    return;
}

void CDnsPinger::HandleActionCancelAllPings()
{
    List< AutoPtr<ActivePing> >::Iterator iter;
    for (iter = mActivePings.Begin(); iter != mActivePings.End(); ++iter) {
        AutoPtr<ActivePing> activePing = *iter;
        activePing->mSocket->Close();
    }
    mActivePings.Clear();
    return;
}

ECode CDnsPinger::GetDnsList(
    /* [out] */ IObjectContainer** dnslist)
{
    VALIDATE_NOT_NULL(dnslist);
    AutoPtr<ILinkProperties> curLinkProps;
    GetCurrentLinkProperties((ILinkProperties**)&curLinkProps);
    AutoPtr<IObjectContainer> objectcontainer;
    if (curLinkProps == NULL) {
        //Logger::E(TAG, "getCurLinkProperties:: LP for type" + mConnectionType + " is NULL!");
        CObjectContainer::New((IObjectContainer**)&objectcontainer);
        List< AutoPtr<IInetAddress> >::Iterator iter;
        for (mDefaultDns.Begin(); iter != mDefaultDns.End(); ++iter) {
            objectcontainer->Add((*iter)->Probe(EIID_IInterface));
        }
        *dnslist = objectcontainer;
        INTERFACE_ADDREF(*dnslist);
        return NOERROR;
    }

    AutoPtr<IObjectContainer> dnses;
    curLinkProps->GetDnses((IObjectContainer**)&dnses);
    Int32 num;
    dnses->GetObjectCount(&num);
    if (dnses == NULL || num == 0) {
        Logger::E(TAG, "getDns::LinkProps has NULL dns - returning default");
        CObjectContainer::New((IObjectContainer**)&objectcontainer);
        List< AutoPtr<IInetAddress> >::Iterator iter;
        for (mDefaultDns.Begin(); iter != mDefaultDns.End(); ++iter) {
            objectcontainer->Add((IInterface*)*iter);
        }
        *dnslist = objectcontainer;
        INTERFACE_ADDREF(*dnslist);
        return NOERROR;
    }

    *dnslist = dnses;
    INTERFACE_ADDREF(*dnslist);
    return NOERROR;
}

/**
 * Send a ping.  The response will come via a {@link #DNS_PING_RESULT} to the handler
 * specified at creation.
 * @param dns address of dns server to ping
 * @param timeout timeout for ping
 * @return an ID field, which will also be included in the {@link #DNS_PING_RESULT} message.
 */
ECode CDnsPinger::PingDnsAsync(
    /* [in] */ IInetAddress* dns,
    /* [in] */ Int32 timeout,
    /* [in] */ Int32 delay,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    Int32 id;
    sCounter->IncrementAndGet(&id);
    *result = id;

    Int32 val;
    mCurrentToken->Get(&val);
    AutoPtr<DnsArg> newDnsArg = new DnsArg(dns, val);
    AutoPtr<IMessage> msg;
    ObtainMessageEx3(ACTION_PING_DNS, id, timeout, newDnsArg, (IMessage**)&msg);
    Boolean bval;
    return SendMessageDelayed(msg, delay, &bval);
}

ECode CDnsPinger::CancelPings()
{
    Int32 num;
    mCurrentToken->IncrementAndGet(&num);
    Boolean result;
    return SendEmptyMessage(ACTION_CANCEL_ALL_PINGS, &result);
}

ECode CDnsPinger::SendResponse(
    /* [in] */ Int32 internalId,
    /* [in] */ Int32 externalId,
    /* [in] */ Int32 responseVal)
{
//    if(DBG) {
//        Logger::I("Responding to packet " + internalId +
//                " externalId " + externalId +
//                " and val " + responseVal);
//    }

    AutoPtr<IMessage> msg;
    ObtainMessageEx2(DNS_PING_RESULT, internalId, responseVal, (IMessage**)&msg);
    Boolean result;
    return mTarget->SendMessage(msg, &result);
}

ECode CDnsPinger::GetCurrentLinkProperties(
    /* [out] */ ILinkProperties** result)
{
    VALIDATE_NOT_NULL(result);
    if (mConnectivityManager == NULL) {
        AutoPtr<IInterface> obj;
        mContext->GetSystemService(IContext::CONNECTIVITY_SERVICE, (IInterface**)&obj);
        mConnectivityManager = IConnectivityManager::Probe(obj);
    }

    return mConnectivityManager->GetLinkProperties(mConnectionType, result);
}

ECode CDnsPinger::GetDefaultDns(
    /* [out] */ IInetAddress** result)
{
    VALIDATE_NOT_NULL(result);
    AutoPtr<IContentResolver> resolver;
    FAIL_RETURN(mContext->GetContentResolver((IContentResolver**)&resolver))
    String dns;
    FAIL_RETURN(Settings::Global::GetString(resolver, ISettingsGlobal::DEFAULT_DNS_SERVER, &dns))
    if (dns.IsNullOrEmpty()) {
        AutoPtr<IResources> resources;
        mContext->GetResources((IResources**)&resources);
        resources->GetString(R::string::config_default_dns_server, &dns);
    }

    AutoPtr< IInetAddress> addr;
    ECode ec = NetworkUtils::NumericToInetAddress(dns, (IInetAddress**)&addr);
    if (FAILED(ec)) {
        //loge("getDefaultDns::malformed default dns address");
        *result = NULL;
        return ec;
    }
    *result = addr;
    INTERFACE_ADDREF(*result);
    return NOERROR;
}

} // namespace Net
} // namespace Droid
} // namespace Elastos
