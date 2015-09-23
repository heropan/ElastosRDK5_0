
#include "elastos/droid/net/NetworkFactory.h"

using Elastos::Droid::Internal::Utility::IProtocol;

namespace Elastos {
namespace Droid {
namespace Net {

CAR_INTERFACE_IMPL(NetworkFactory, Handler, INetworkFactory)

const Boolean NetworkFactory::DBG = true;
const Boolean NetworkFactory::VDBG = false;
const Int32 NetworkFactory::BASE = IProtocol::BASE_NETWORK_FACTORY;
const Int32 NetworkFactory::CMD_SET_SCORE = BASE + 2;
const Int32 NetworkFactory::CMD_SET_FILTER = BASE + 3;

NetworkFactory::NetworkFactory()
    : mRefCount(0)
    , mMessenger(NULL)
{}

AutoPtr<ISparseArray> NetworkFactory::CreateSparseArray()
{
    AutoPtr<ISparseArray> rev;
#if 0 // TODO: Translated code below
    rev = new SparseArray<NetworkRequestInfo>();
#endif
    return rev;
}

ECode NetworkFactory::constructor(
    /* [in] */ ILooper* looper,
    /* [in] */ IContext* context,
    /* [in] */ const String& logTag,
    /* [in] */ INetworkCapabilities* filter)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        super(looper);
        LOG_TAG = logTag;
        mContext = context;
        mCapabilityFilter = filter;

#endif
}

ECode NetworkFactory::Register()
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        if (DBG) log("Registering NetworkFactory");
        if (mMessenger == null) {
            mMessenger = new Messenger(this);
            ConnectivityManager.from(mContext).registerNetworkFactory(mMessenger, LOG_TAG);
        }

#endif
}

ECode NetworkFactory::Unregister()
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        if (DBG) log("Unregistering NetworkFactory");
        if (mMessenger != null) {
            ConnectivityManager.from(mContext).unregisterNetworkFactory(mMessenger);
            mMessenger = null;
        }

#endif
}

ECode NetworkFactory::HandleMessage(
    /* [in] */ IMessage* msg)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        switch (msg.what) {
            case CMD_REQUEST_NETWORK: {
                handleAddRequest((NetworkRequest)msg.obj, msg.arg1);
                break;
            }
            case CMD_CANCEL_REQUEST: {
                handleRemoveRequest((NetworkRequest) msg.obj);
                break;
            }
            case CMD_SET_SCORE: {
                handleSetScore(msg.arg1);
                break;
            }
            case CMD_SET_FILTER: {
                handleSetFilter((NetworkCapabilities) msg.obj);
                break;
            }
        }

#endif
}

ECode NetworkFactory::HandleAddRequest(
    /* [in] */ INetworkRequest* request,
    /* [in] */ Int32 score)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        NetworkRequestInfo n = mNetworkRequests.get(request.requestId);
        if (n == null) {
            if (DBG) log("got request " + request + " with score " + score);
            n = new NetworkRequestInfo(request, score);
            mNetworkRequests.put(n.request.requestId, n);
        } else {
            if (VDBG) log("new score " + score + " for exisiting request " + request);
            n.score = score;
        }
        if (VDBG) log("  my score=" + mScore + ", my filter=" + mCapabilityFilter);
        evalRequest(n);

#endif
}

ECode NetworkFactory::HandleRemoveRequest(
    /* [in] */ INetworkRequest* request)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        NetworkRequestInfo n = mNetworkRequests.get(request.requestId);
        if (n != null && n.requested) {
            mNetworkRequests.remove(request.requestId);
            releaseNetworkFor(n.request);
        }

#endif
}

ECode NetworkFactory::HandleSetScore(
    /* [in] */ Int32 score)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        mScore = score;
        evalRequests();

#endif
}

ECode NetworkFactory::HandleSetFilter(
    /* [in] */ INetworkCapabilities* netCap)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        mCapabilityFilter = netCap;
        evalRequests();

#endif
}

ECode NetworkFactory::AcceptRequest(
    /* [in] */ INetworkRequest* request,
    /* [in] */ Int32 score,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return true;

#endif
}

ECode NetworkFactory::EvalRequest(
    /* [in] */ NetworkRequestInfo* n)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        if (n.requested == false && n.score < mScore &&
                n.request.networkCapabilities.satisfiedByNetworkCapabilities(
                mCapabilityFilter) && acceptRequest(n.request, n.score)) {
            needNetworkFor(n.request, n.score);
            n.requested = true;
        } else if (n.requested == true &&
                (n.score > mScore || n.request.networkCapabilities.satisfiedByNetworkCapabilities(
                mCapabilityFilter) == false || acceptRequest(n.request, n.score) == false)) {
            releaseNetworkFor(n.request);
            n.requested = false;
        }

#endif
}

ECode NetworkFactory::EvalRequests()
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        for (int i = 0; i < mNetworkRequests.size(); i++) {
            NetworkRequestInfo n = mNetworkRequests.valueAt(i);
            evalRequest(n);
        }

#endif
}

ECode NetworkFactory::StartNetwork()
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below

#endif
}

ECode NetworkFactory::StopNetwork()
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below

#endif
}

ECode NetworkFactory::NeedNetworkFor(
    /* [in] */ INetworkRequest* networkRequest,
    /* [in] */ Int32 score)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        if (++mRefCount == 1) startNetwork();

#endif
}

ECode NetworkFactory::ReleaseNetworkFor(
    /* [in] */ INetworkRequest* networkRequest)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        if (--mRefCount == 0) stopNetwork();

#endif
}

ECode NetworkFactory::AddNetworkRequest(
    /* [in] */ INetworkRequest* networkRequest,
    /* [in] */ Int32 score)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        sendMessage(obtainMessage(CMD_REQUEST_NETWORK,
                new NetworkRequestInfo(networkRequest, score)));

#endif
}

ECode NetworkFactory::RemoveNetworkRequest(
    /* [in] */ INetworkRequest* networkRequest)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        sendMessage(obtainMessage(CMD_CANCEL_REQUEST, networkRequest));

#endif
}

ECode NetworkFactory::SetScoreFilter(
    /* [in] */ Int32 score)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        sendMessage(obtainMessage(CMD_SET_SCORE, score, 0));

#endif
}

ECode NetworkFactory::SetCapabilityFilter(
    /* [in] */ INetworkCapabilities* netCap)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        sendMessage(obtainMessage(CMD_SET_FILTER, new NetworkCapabilities(netCap)));

#endif
}

ECode NetworkFactory::Log(
    /* [in] */ const String& s)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        Log.d(LOG_TAG, s);

#endif
}


} // namespace Net
} // namespace Droid
} // namespace Elastos
