
#include "elastos/droid/net/NetworkAgent.h"
// #include <CArrayList.h>

using Elastos::Droid::Internal::Utility::IProtocol;
// using Elastos::Utility::CArrayList;

namespace Elastos {
namespace Droid {
namespace Net {

CAR_INTERFACE_IMPL(NetworkAgent, Handler, INetworkAgent)

const Boolean NetworkAgent::DBG = TRUE;
const Boolean NetworkAgent::VDBG = FALSE;
#if 0 // TODO: Waiting for IProtocol
const Int32 NetworkAgent::BASE = IProtocol::BASE_NETWORK_AGENT;
#else
const Int32 NetworkAgent::BASE = 0x00081000;
#endif

AutoPtr<IArrayList> NetworkAgent::CreatePreConnectQueue()
{
    AutoPtr<IArrayList> rev;
#if 0 // TODO: Waiting for CArrayList
    CArrayList::New((IArrayList**)&rev);
#else
    assert(0);
#endif
    return rev;
}

NetworkAgent::NetworkAgent()
    : mPreConnectedQueue(CreatePreConnectQueue())
{}

ECode NetworkAgent::constructor(
    /* [in] */ ILooper* looper,
    /* [in] */ IContext* context,
    /* [in] */ const String& logTag,
    /* [in] */ INetworkInfo* ni,
    /* [in] */ INetworkCapabilities* nc,
    /* [in] */ ILinkProperties* lp,
    /* [in] */ Int32 score)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        this(looper, context, logTag, ni, nc, lp, score, null);

#endif
}

ECode NetworkAgent::constructor(
    /* [in] */ ILooper* looper,
    /* [in] */ IContext* context,
    /* [in] */ const String& logTag,
    /* [in] */ INetworkInfo* ni,
    /* [in] */ INetworkCapabilities* nc,
    /* [in] */ ILinkProperties* lp,
    /* [in] */ Int32 score,
    /* [in] */ INetworkMisc* misc)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        super(looper);
        LOG_TAG = logTag;
        mContext = context;
        if (ni == null || nc == null || lp == null) {
            throw new IllegalArgumentException();
        }
        if (VDBG) log("Registering NetworkAgent");
        ConnectivityManager cm = (ConnectivityManager)mContext.getSystemService(
                Context.CONNECTIVITY_SERVICE);
        cm.registerNetworkAgent(new Messenger(this), new NetworkInfo(ni),
                new LinkProperties(lp), new NetworkCapabilities(nc), score, misc);

#endif
}

ECode NetworkAgent::HandleMessage(
    /* [in] */ IMessage* msg)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        switch (msg.what) {
            case AsyncChannel.CMD_CHANNEL_FULL_CONNECTION: {
                if (mAsyncChannel != null) {
                    log("Received new connection while already connected!");
                } else {
                    if (VDBG) log("NetworkAgent fully connected");
                    AsyncChannel ac = new AsyncChannel();
                    ac.connected(null, this, msg.replyTo);
                    ac.replyToMessage(msg, AsyncChannel.CMD_CHANNEL_FULLY_CONNECTED,
                            AsyncChannel.STATUS_SUCCESSFUL);
                    synchronized (mPreConnectedQueue) {
                        mAsyncChannel = ac;
                        for (Message m : mPreConnectedQueue) {
                            ac.sendMessage(m);
                        }
                        mPreConnectedQueue.clear();
                    }
                }
                break;
            }
            case AsyncChannel.CMD_CHANNEL_DISCONNECT: {
                if (VDBG) log("CMD_CHANNEL_DISCONNECT");
                if (mAsyncChannel != null) mAsyncChannel.disconnect();
                break;
            }
            case AsyncChannel.CMD_CHANNEL_DISCONNECTED: {
                if (DBG) log("NetworkAgent channel lost");
                // let the client know CS is done with us.
                unwanted();
                synchronized (mPreConnectedQueue) {
                    mAsyncChannel = null;
                }
                break;
            }
            case CMD_SUSPECT_BAD: {
                log("Unhandled Message " + msg);
                break;
            }
            case CMD_REPORT_NETWORK_STATUS: {
                if (VDBG) {
                    log("CMD_REPORT_NETWORK_STATUS(" +
                            (msg.arg1 == VALID_NETWORK ? "VALID)" : "INVALID)"));
                }
                networkStatus(msg.arg1);
                break;
            }
        }

#endif
}

ECode NetworkAgent::QueueOrSendMessage(
    /* [in] */ Int32 what,
    /* [in] */ IObject* obj)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        synchronized (mPreConnectedQueue) {
            if (mAsyncChannel != null) {
                mAsyncChannel.sendMessage(what, obj);
            } else {
                Message msg = Message.obtain();
                msg.what = what;
                msg.obj = obj;
                mPreConnectedQueue.add(msg);
            }
        }

#endif
}

ECode NetworkAgent::SendLinkProperties(
    /* [in] */ ILinkProperties* linkProperties)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        queueOrSendMessage(EVENT_NETWORK_PROPERTIES_CHANGED, new LinkProperties(linkProperties));

#endif
}

ECode NetworkAgent::SendNetworkInfo(
    /* [in] */ INetworkInfo* networkInfo)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        queueOrSendMessage(EVENT_NETWORK_INFO_CHANGED, new NetworkInfo(networkInfo));

#endif
}

ECode NetworkAgent::SendNetworkCapabilities(
    /* [in] */ INetworkCapabilities* networkCapabilities)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        queueOrSendMessage(EVENT_NETWORK_CAPABILITIES_CHANGED,
                new NetworkCapabilities(networkCapabilities));

#endif
}

ECode NetworkAgent::SendNetworkScore(
    /* [in] */ Int32 score)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        if (score < 0) {
            throw new IllegalArgumentException("Score must be >= 0");
        }
        queueOrSendMessage(EVENT_NETWORK_SCORE_CHANGED, new Integer(score));

#endif
}

ECode NetworkAgent::AddUidRanges(
    /* [in] */ ArrayOf<IUidRange*>* ranges)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        queueOrSendMessage(EVENT_UID_RANGES_ADDED, ranges);

#endif
}

ECode NetworkAgent::RemoveUidRanges(
    /* [in] */ ArrayOf<IUidRange*>* ranges)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        queueOrSendMessage(EVENT_UID_RANGES_REMOVED, ranges);

#endif
}

ECode NetworkAgent::ExplicitlySelected()
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        queueOrSendMessage(EVENT_SET_EXPLICITLY_SELECTED, 0);

#endif
}

ECode NetworkAgent::NetworkStatus(
    /* [in] */ Int32 status)
{
    return NOERROR;
}

ECode NetworkAgent::Log(
    /* [in] */ const String& s)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        Log.d(LOG_TAG, "NetworkAgent: " + s);

#endif
}


} // namespace Net
} // namespace Droid
} // namespace Elastos
