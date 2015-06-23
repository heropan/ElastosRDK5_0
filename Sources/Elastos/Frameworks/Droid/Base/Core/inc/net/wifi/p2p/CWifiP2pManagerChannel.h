#ifndef __CWIFIP2PMANAGERCHANNEL_H__
#define __CWIFIP2PMANAGERCHANNEL_H__

#include "_CWifiP2pManagerChannel.h"
#include "ext/frameworkdef.h"
#include "util/AsyncChannel.h"
#include "os/HandlerBase.h"
#include <elastos/Pair.h>
#include <elastos/utility/etl/HashMap.h>

using Elastos::Utility::Etl::Pair;
using Elastos::Utility::Etl::HashMap;
using Elastos::Droid::Os::HandlerBase;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Internal::Utility::AsyncChannel;
using Elastos::Droid::Net::Wifi::P2p::Nsd::IWifiP2pServiceResponse;
using Elastos::Droid::Net::Wifi::P2p::Nsd::IWifiP2pUpnpServiceResponse;
using Elastos::Droid::Net::Wifi::P2p::Nsd::IWifiP2pDnsSdServiceResponse;

namespace Elastos {
namespace Droid {
namespace Net {
namespace Wifi {
namespace P2p {

CarClass(CWifiP2pManagerChannel)
{
private:
    class P2pHandler
        : public HandlerBase
    {
    public:
        P2pHandler(
            /* [in] */ ILooper* looper,
            /* [in] */ CWifiP2pManagerChannel* context);

        virtual CARAPI HandleMessage(
            /* [in] */ IMessage* msg);
    private:
        CWifiP2pManagerChannel* mHost;

    };

public:
    CWifiP2pManagerChannel();

    ~CWifiP2pManagerChannel();

    CARAPI constructor(
        /* [in] */ IContext * context,
        /* [in] */ ILooper * looper,
        /* [in] */ IWifiP2pManagerChannelListener* l);

    CARAPI GetHandler(
        /* [out] */ IHandler** handler);

    CARAPI SetHandler(
        /* [in] */ IHandler* handler);

    CARAPI GetContext(
        /* [out] */ IContext** context);

    CARAPI SetContext(
        /* [in] */ IContext* context);

    CARAPI GetChannelListener(
        /* [out] */ IWifiP2pManagerChannelListener** listener);

    CARAPI SetChannelListener(
        /* [in] */ IWifiP2pManagerChannelListener* listener);

    CARAPI GetServRspListener(
        /* [out] */ IWifiP2pManagerServiceResponseListener** listener);

    CARAPI SetServRspListener(
        /* [in] */ IWifiP2pManagerServiceResponseListener* listener);

    CARAPI GetDnsSdServRspListener(
        /* [out] */ IWifiP2pManagerDnsSdServiceResponseListener** listener);

    CARAPI SetDnsSdServRspListener(
        /* [in] */ IWifiP2pManagerDnsSdServiceResponseListener* listener);

    CARAPI GetDnsSdTxtListener(
        /* [out] */ IWifiP2pManagerDnsSdTxtRecordListener** listener);

    CARAPI SetDnsSdTxtListener(
        /* [in] */ IWifiP2pManagerDnsSdTxtRecordListener* listener);

    CARAPI GetUpnpServRspListener(
        /* [out] */ IWifiP2pManagerUpnpServiceResponseListener** listener);

    CARAPI SetUpnpServRspListener(
        /* [in] */ IWifiP2pManagerUpnpServiceResponseListener* listener);

    CARAPI HandleServiceResponse(
        /* [in] */ IWifiP2pServiceResponse* resp);

    CARAPI HandleUpnpServiceResponse(
        /* [in] */ IWifiP2pUpnpServiceResponse* resp);

    CARAPI HandleDnsSdServiceResponse(
        /* [in] */ IWifiP2pDnsSdServiceResponse* resp);

    CARAPI PutListener(
        /* [in] */ IInterface* listener,
        /* [out] */ Int32* val);

    CARAPI GetListener(
        /* [in] */ Int32 key,
        /* [out] */ IInterface** obj);

    CARAPI SetDialogListener(
        /* [in] */ IWifiP2pManagerDialogListener* listener);

public:
    static const Int32 INVALID_LISTENER_KEY;
    AutoPtr<IWifiP2pManagerChannelListener> mChannelListener;
    AutoPtr<IWifiP2pManagerServiceResponseListener> mServRspListener;
    AutoPtr<IWifiP2pManagerDnsSdServiceResponseListener> mDnsSdServRspListener;
    AutoPtr<IWifiP2pManagerDnsSdTxtRecordListener> mDnsSdTxtListener;
    AutoPtr<IWifiP2pManagerUpnpServiceResponseListener> mUpnpServRspListener;
    HashMap<Int32, AutoPtr<IInterface> > mListenerMap;
    Mutex mListenerMapLock;
    Int32 mListenerKey;
    AutoPtr<IWifiP2pManagerDialogListener> mDialogListener;

    AutoPtr<AsyncChannel> mAsyncChannel;
    AutoPtr<IHandler> mHandler;
    AutoPtr<IContext> mContext;
};

}
}
}
}
}
#endif // __CWIFIP2PMANAGERCHANNEL_H__
