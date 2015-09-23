
#ifndef __ELASTOS_DROID_NET_NETWORKFACTORY_H__
#define __ELASTOS_DROID_NET_NETWORKFACTORY_H__

#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/os/Handler.h"

using Elastos::Droid::Utility::ISparseArray;
using Elastos::Droid::Os::Handler;
using Elastos::Droid::Os::IMessenger;
using Elastos::Droid::Content::IContext;

namespace Elastos {
namespace Droid {
namespace Net {

/**
 * A NetworkFactory is an entity that creates NetworkAgent objects.
 * The bearers register with ConnectivityService using {@link #register} and
 * their factory will start receiving scored NetworkRequests.  NetworkRequests
 * can be filtered 3 ways: by NetworkCapabilities, by score and more complexly by
 * overridden function.  All of these can be dynamic - changing NetworkCapabilities
 * or score forces re-evaluation of all current requests.
 *
 * If any requests pass the filter some overrideable functions will be called.
 * If the bearer only cares about very simple start/stopNetwork callbacks, those
 * functions can be overridden.  If the bearer needs more interaction, it can
 * override addNetworkRequest and removeNetworkRequest which will give it each
 * request that passes their current filters.
 * @hide
 **/
class NetworkFactory
    : public Handler
    , public INetworkFactory
{
public:
    CAR_INTERFACE_DECL()

    NetworkFactory();

    CARAPI constructor(
        /* [in] */ ILooper* looper,
        /* [in] */ IContext* context,
        /* [in] */ const String& logTag,
        /* [in] */ INetworkCapabilities* filter);

    CARAPI Register();

    CARAPI Unregister();

    // @Override
    CARAPI HandleMessage(
        /* [in] */ IMessage* msg);

    /**
     * Overridable function to provide complex filtering.
     * Called for every request every time a new NetworkRequest is seen
     * and whenever the filterScore or filterNetworkCapabilities change.
     *
     * acceptRequest can be overriden to provide complex filter behavior
     * for the incoming requests
     *
     * For output, this class will call {@link #needNetworkFor} and
     * {@link #releaseNetworkFor} for every request that passes the filters.
     * If you don't need to see every request, you can leave the base
     * implementations of those two functions and instead override
     * {@link #startNetwork} and {@link #stopNetwork}.
     *
     * If you want to see every score fluctuation on every request, set
     * your score filter to a very high number and watch {@link #needNetworkFor}.
     *
     * @return {@code true} to accept the request.
     */
    CARAPI AcceptRequest(
        /* [in] */ INetworkRequest* request,
        /* [in] */ Int32 score,
        /* [out] */ Boolean* result);

    // override to do simple mode (request independent)
    CARAPI StartNetwork();

    CARAPI StopNetwork();

    // override to do fancier stuff
    CARAPI NeedNetworkFor(
        /* [in] */ INetworkRequest* networkRequest,
        /* [in] */ Int32 score);

    CARAPI ReleaseNetworkFor(
        /* [in] */ INetworkRequest* networkRequest);

    CARAPI AddNetworkRequest(
        /* [in] */ INetworkRequest* networkRequest,
        /* [in] */ Int32 score);

    CARAPI RemoveNetworkRequest(
        /* [in] */ INetworkRequest* networkRequest);

    CARAPI SetScoreFilter(
        /* [in] */ Int32 score);

    CARAPI SetCapabilityFilter(
        /* [in] */ INetworkCapabilities* netCap);

    CARAPI Log(
        /* [in] */ const String& s);

private:
    class NetworkRequestInfo {
    public:
        const AutoPtr<INetworkRequest> mRequest;
        Int32 mScore;
        Boolean mRequested; // do we have a request outstanding, limited by score
        NetworkRequestInfo(INetworkRequest* request, Int32 score)
            : mRequest(request)
        {
            mScore = score;
            mRequested = FALSE;
        }
    };

    CARAPI HandleAddRequest(
        /* [in] */ INetworkRequest* request,
        /* [in] */ Int32 score);

    CARAPI HandleRemoveRequest(
        /* [in] */ INetworkRequest* request);

    CARAPI HandleSetScore(
        /* [in] */ Int32 score);

    CARAPI HandleSetFilter(
        /* [in] */ INetworkCapabilities* netCap);

    CARAPI EvalRequest(
        /* [in] */ NetworkRequestInfo* n);

    CARAPI EvalRequests();

    static AutoPtr<ISparseArray> CreateSparseArray();

    static const Boolean DBG;

    static const Boolean VDBG;

    static const Int32 BASE;

    /**
     * Internally used to set our best-guess score.
     * msg.arg1 = new score
     */
    static const Int32 CMD_SET_SCORE;

    /**
     * Internally used to set our current filter for coarse bandwidth changes with
     * technology changes.
     * msg.obj = new filter
     */
    static const Int32 CMD_SET_FILTER;

    const IContext* mContext;

    // LOG_TAG has duplicated macro
    const String mLOG_TAG;

    const AutoPtr<ISparseArray> mNetworkRequests;

    Int32 mScore;

    AutoPtr<INetworkCapabilities> mCapabilityFilter;

    Int32 mRefCount;

    AutoPtr<IMessenger> mMessenger;

};

} // namespace Net
} // namespace Droid
} // namespace Elastos
#endif // __ELASTOS_DROID_NET_NETWORKFACTORY_H__
