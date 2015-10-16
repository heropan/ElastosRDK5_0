#include "CWifiP2pManager.h"
#include "CWifiP2pManagerChannel.h"
#include "CWifiP2pDevice.h"
#include "elastos/droid/os/CBundle.h"
#include "elastos/droid/os/CMessageHelper.h"
#include <elastos/utility/logging/Slogger.h>

using Elastos::Droid::Os::IMessage;
using Elastos::Droid::Os::IBundle;
using Elastos::Droid::Os::CBundle;
using Elastos::Droid::Os::IMessageHelper;
using Elastos::Droid::Os::CMessageHelper;
using Elastos::Droid::Utility::IProtocol;
using Elastos::Droid::Internal::Utility::AsyncChannel;
using Elastos::Droid::Net::Wifi::P2p::CWifiP2pManagerChannel;
using Elastos::Droid::Net::Wifi::P2p::CWifiP2pDevice;
using Elastos::Utility::Logging::Slogger;

namespace Elastos {
namespace Droid {
namespace Net {
namespace Wifi {
namespace P2p {

const String CWifiP2pManager::TAG("WifiP2pManager");
const Boolean CWifiP2pManager::DBG = TRUE;
const Int32 CWifiP2pManager::BASE = IProtocol::BASE_WIFI_P2P_MANAGER;

ECode CWifiP2pManager::GetService(
    /* [out] */ IIWifiP2pManager** service)
{
    VALIDATE_NOT_NULL(service);
    *service = mService;
    REFCOUNT_ADD(*service);
    return NOERROR;
}

ECode CWifiP2pManager::SetService(
    /* [in] */ IIWifiP2pManager* service)
{
    mService = service;
    return NOERROR;
}

ECode CWifiP2pManager::Initialize(
    /* [in] */ IContext* srcContext,
    /* [in] */ ILooper* srcLooper,
    /* [in] */ IWifiP2pManagerChannelListener* listener,
    /* [out] */ IWifiP2pManagerChannel** channel)
{
    VALIDATE_NOT_NULL(channel);
    *channel = NULL;

    AutoPtr<IMessenger> messenger;
    GetMessenger((IMessenger**)&messenger);
    if (messenger == NULL) {
        return NOERROR;
    }

    AutoPtr<IWifiP2pManagerChannel> c;
    CWifiP2pManagerChannel::New(srcContext, srcLooper, listener, (IWifiP2pManagerChannel**)&c);
    CWifiP2pManagerChannel* wpmc = (CWifiP2pManagerChannel*)c.Get();
    AutoPtr<AsyncChannel> asyncChannel = wpmc->mAsyncChannel;
    AutoPtr<IHandler> handler = wpmc->mHandler;

    Int32 status = asyncChannel->ConnectSync(srcContext, handler, messenger);
    if (status == AsyncChannel::STATUS_SUCCESSFUL) {
        *channel = c;
        REFCOUNT_ADD(*channel);
    }
    else {
        Slogger::E(TAG, "Initialize failed, ConnectSync status %d", status);
    }

    return NOERROR;
}

ECode CWifiP2pManager::DiscoverPeers(
    /* [in] */ IWifiP2pManagerChannel* c,
    /* [in] */ IWifiP2pManagerActionListener* listener)
{
    if (DBG) Slogger::D(TAG, "DiscoverPeers");
    FAIL_RETURN(CheckChannel(c));

    CWifiP2pManagerChannel* wpmc = (CWifiP2pManagerChannel*)c;
    AutoPtr<AsyncChannel> asyncChannel = wpmc->mAsyncChannel;

    Int32 val;
    c->PutListener(listener ? listener->Probe(EIID_IInterface) : NULL, &val);
    asyncChannel->SendMessage(DISCOVER_PEERS, 0, val);
    return NOERROR;
}

ECode CWifiP2pManager::StopPeerDiscovery(
    /* [in] */ IWifiP2pManagerChannel* c,
    /* [in] */ IWifiP2pManagerActionListener* listener)
{
    if (DBG) Slogger::D(TAG, "StopPeerDiscovery");
    FAIL_RETURN(CheckChannel(c));

    CWifiP2pManagerChannel* wpmc = (CWifiP2pManagerChannel*)c;
    AutoPtr<AsyncChannel> asyncChannel = wpmc->mAsyncChannel;

    Int32 val;
    c->PutListener(listener ? listener->Probe(EIID_IInterface) : NULL, &val);
    asyncChannel->SendMessage(STOP_DISCOVERY, 0, val);
    return NOERROR;
}

ECode CWifiP2pManager::Connect(
    /* [in] */ IWifiP2pManagerChannel* c,
    /* [in] */ IWifiP2pConfig* config,
    /* [in] */ IWifiP2pManagerActionListener* listener)
{
    if (DBG)  {
        String info;
        if (config) config->ToString(&info);
        Slogger::E(TAG, "Connect %s", info.string());
    }

    FAIL_RETURN(CheckChannel(c));

    CWifiP2pManagerChannel* wpmc = (CWifiP2pManagerChannel*)c;
    AutoPtr<AsyncChannel> asyncChannel = wpmc->mAsyncChannel;

    Int32 val;
    c->PutListener(listener ? listener->Probe(EIID_IInterface) : NULL, &val);
    asyncChannel->SendMessage(CONNECT, 0, val,
        config ? config->Probe(EIID_IInterface) : NULL);

    return NOERROR;
}

ECode CWifiP2pManager::CancelConnect(
    /* [in] */ IWifiP2pManagerChannel* c,
    /* [in] */ IWifiP2pManagerActionListener* listener)
{
    if (DBG) Slogger::D(TAG, "CancelConnect");
    FAIL_RETURN(CheckChannel(c));

    CWifiP2pManagerChannel* wpmc = (CWifiP2pManagerChannel*)c;
    AutoPtr<AsyncChannel> asyncChannel = wpmc->mAsyncChannel;

    Int32 val;
    c->PutListener(listener ? listener->Probe(EIID_IInterface) : NULL, &val);
    asyncChannel->SendMessage(CANCEL_CONNECT, 0, val);
    return NOERROR;
}

ECode CWifiP2pManager::CreateGroup(
    /* [in] */ IWifiP2pManagerChannel* c,
    /* [in] */ IWifiP2pManagerActionListener* listener)
{
    if (DBG) Slogger::D(TAG, "CreateGroup");
    FAIL_RETURN(CheckChannel(c));

    CWifiP2pManagerChannel* wpmc = (CWifiP2pManagerChannel*)c;
    AutoPtr<AsyncChannel> asyncChannel = wpmc->mAsyncChannel;

    Int32 val;
    c->PutListener(listener ? listener->Probe(EIID_IInterface) : NULL, &val);
    asyncChannel->SendMessage(CREATE_GROUP, IWifiP2pGroup::PERSISTENT_NET_ID, val);
    return NOERROR;
}

ECode CWifiP2pManager::RemoveGroup(
    /* [in] */ IWifiP2pManagerChannel* c,
    /* [in] */ IWifiP2pManagerActionListener* listener)
{
    if (DBG) Slogger::D(TAG, "RemoveGroup");
    FAIL_RETURN(CheckChannel(c));

    CWifiP2pManagerChannel* wpmc = (CWifiP2pManagerChannel*)c;
    AutoPtr<AsyncChannel> asyncChannel = wpmc->mAsyncChannel;

    Int32 val;
    c->PutListener(listener ? listener->Probe(EIID_IInterface) : NULL, &val);
    asyncChannel->SendMessage(REMOVE_GROUP, 0, val);
    return NOERROR;
}

ECode CWifiP2pManager::AddLocalService(
    /* [in] */ IWifiP2pManagerChannel* c,
    /* [in] */ IWifiP2pServiceInfo* servInfo,
    /* [in] */ IWifiP2pManagerActionListener* listener)
{
    if (DBG) Slogger::D(TAG, "AddLocalService");
    FAIL_RETURN(CheckChannel(c));
    FAIL_RETURN(CheckServiceInfo(servInfo));

    CWifiP2pManagerChannel* wpmc = (CWifiP2pManagerChannel*)c;
    AutoPtr<AsyncChannel> asyncChannel = wpmc->mAsyncChannel;

    Int32 val;
    c->PutListener(listener ? listener->Probe(EIID_IInterface) : NULL, &val);
    asyncChannel->SendMessage(ADD_LOCAL_SERVICE, 0, val,
        servInfo ? servInfo->Probe(EIID_IInterface) : NULL);
    return NOERROR;
}

ECode CWifiP2pManager::RemoveLocalService(
    /* [in] */ IWifiP2pManagerChannel* c,
    /* [in] */ IWifiP2pServiceInfo* servInfo,
    /* [in] */ IWifiP2pManagerActionListener* listener)
{
    if (DBG) Slogger::D(TAG, "RemoveLocalService");
    FAIL_RETURN(CheckChannel(c));
    FAIL_RETURN(CheckServiceInfo(servInfo));

    CWifiP2pManagerChannel* wpmc = (CWifiP2pManagerChannel*)c;
    AutoPtr<AsyncChannel> asyncChannel = wpmc->mAsyncChannel;

    Int32 val;
    c->PutListener(listener ? listener->Probe(EIID_IInterface) : NULL, &val);
    asyncChannel->SendMessage(REMOVE_LOCAL_SERVICE, 0, val,
        servInfo ? servInfo->Probe(EIID_IInterface) : NULL);
    return NOERROR;
}

ECode CWifiP2pManager::ClearLocalServices(
    /* [in] */ IWifiP2pManagerChannel* c,
    /* [in] */ IWifiP2pManagerActionListener* listener)
{
    if (DBG) Slogger::D(TAG, "ClearLocalServices");
    FAIL_RETURN(CheckChannel(c));

    CWifiP2pManagerChannel* wpmc = (CWifiP2pManagerChannel*)c;
    AutoPtr<AsyncChannel> asyncChannel = wpmc->mAsyncChannel;

    Int32 val;
    c->PutListener(listener ? listener->Probe(EIID_IInterface) : NULL, &val);
    asyncChannel->SendMessage(CLEAR_LOCAL_SERVICES, 0, val);
    return NOERROR;
}

ECode CWifiP2pManager::SetServiceResponseListener(
    /* [in] */ IWifiP2pManagerChannel* c,
    /* [in] */ IWifiP2pManagerServiceResponseListener* listener)
{
    FAIL_RETURN(CheckChannel(c));
    return c->SetServRspListener(listener);
}

ECode CWifiP2pManager::SetDnsSdResponseListeners(
    /* [in] */ IWifiP2pManagerChannel* c,
    /* [in] */ IWifiP2pManagerDnsSdServiceResponseListener* servListener,
    /* [in] */ IWifiP2pManagerDnsSdTxtRecordListener* txtListener)
{
    FAIL_RETURN(CheckChannel(c));
    c->SetDnsSdServRspListener(servListener);
    c->SetDnsSdTxtListener(txtListener);
    return NOERROR;
}

ECode CWifiP2pManager::SetUpnpServiceResponseListener(
    /* [in] */ IWifiP2pManagerChannel* c,
    /* [in] */ IWifiP2pManagerUpnpServiceResponseListener* listener)
{
    FAIL_RETURN(CheckChannel(c));
    return c->SetUpnpServRspListener(listener);
}

ECode CWifiP2pManager::DiscoverServices(
    /* [in] */ IWifiP2pManagerChannel* c,
    /* [in] */ IWifiP2pManagerActionListener* listener)
{
    if (DBG) Slogger::D(TAG, "DiscoverServices");
    FAIL_RETURN(CheckChannel(c));

    CWifiP2pManagerChannel* wpmc = (CWifiP2pManagerChannel*)c;
    AutoPtr<AsyncChannel> asyncChannel = wpmc->mAsyncChannel;

    Int32 val;
    c->PutListener(listener ? listener->Probe(EIID_IInterface) : NULL, &val);
    asyncChannel->SendMessage(DISCOVER_SERVICES, 0, val);
    return NOERROR;
}

ECode CWifiP2pManager::AddServiceRequest(
    /* [in] */ IWifiP2pManagerChannel* c,
    /* [in] */ IWifiP2pServiceRequest* req,
    /* [in] */ IWifiP2pManagerActionListener* listener)
{
    if (DBG) Slogger::D(TAG, "AddServiceRequest");
    FAIL_RETURN(CheckChannel(c));
    FAIL_RETURN(CheckServiceRequest(req));

    CWifiP2pManagerChannel* wpmc = (CWifiP2pManagerChannel*)c;
    AutoPtr<AsyncChannel> asyncChannel = wpmc->mAsyncChannel;

    Int32 val;
    c->PutListener(listener ? listener->Probe(EIID_IInterface) : NULL, &val);
    asyncChannel->SendMessage(ADD_SERVICE_REQUEST, 0, val,
        req ? req->Probe(EIID_IInterface) : req);
    return NOERROR;
}

ECode CWifiP2pManager::RemoveServiceRequest(
    /* [in] */ IWifiP2pManagerChannel* c,
    /* [in] */ IWifiP2pServiceRequest* req,
    /* [in] */ IWifiP2pManagerActionListener* listener)
{
    if (DBG) Slogger::D(TAG, "RemoveServiceRequest");
    FAIL_RETURN(CheckChannel(c));
    FAIL_RETURN(CheckServiceRequest(req));

    CWifiP2pManagerChannel* wpmc = (CWifiP2pManagerChannel*)c;
    AutoPtr<AsyncChannel> asyncChannel = wpmc->mAsyncChannel;

    Int32 val;
    c->PutListener(listener ? listener->Probe(EIID_IInterface) : NULL, &val);
    asyncChannel->SendMessage(REMOVE_SERVICE_REQUEST, 0, val,
        req ? req->Probe(EIID_IInterface) : req);
    return NOERROR;
}

ECode CWifiP2pManager::ClearServiceRequests(
    /* [in] */ IWifiP2pManagerChannel* c,
    /* [in] */ IWifiP2pManagerActionListener* listener)
{
    if (DBG) Slogger::D(TAG, "ClearServiceRequests");
    FAIL_RETURN(CheckChannel(c));

    CWifiP2pManagerChannel* wpmc = (CWifiP2pManagerChannel*)c;
    AutoPtr<AsyncChannel> asyncChannel = wpmc->mAsyncChannel;

    Int32 val;
    c->PutListener(listener ? listener->Probe(EIID_IInterface) : NULL, &val);
    asyncChannel->SendMessage(CLEAR_SERVICE_REQUESTS, 0, val);
    return NOERROR;
}

ECode CWifiP2pManager::RequestPeers(
    /* [in] */ IWifiP2pManagerChannel* c,
    /* [in] */ IWifiP2pManagerPeerListListener* listener)
{
    if (DBG) Slogger::D(TAG, "RequestPeers");
    FAIL_RETURN(CheckChannel(c));

    CWifiP2pManagerChannel* wpmc = (CWifiP2pManagerChannel*)c;
    AutoPtr<AsyncChannel> asyncChannel = wpmc->mAsyncChannel;

    Int32 val;
    c->PutListener(listener ? listener->Probe(EIID_IInterface) : NULL, &val);
    asyncChannel->SendMessage(REQUEST_PEERS, 0, val);
    return NOERROR;
}

ECode CWifiP2pManager::RequestConnectionInfo(
    /* [in] */ IWifiP2pManagerChannel* c,
    /* [in] */ IWifiP2pManagerConnectionInfoListener* listener)
{
    if (DBG) Slogger::D(TAG, "RequestConnectionInfo");
    FAIL_RETURN(CheckChannel(c));

    CWifiP2pManagerChannel* wpmc = (CWifiP2pManagerChannel*)c;
    AutoPtr<AsyncChannel> asyncChannel = wpmc->mAsyncChannel;

    Int32 val;
    c->PutListener(listener ? listener->Probe(EIID_IInterface) : NULL, &val);
    asyncChannel->SendMessage(REQUEST_CONNECTION_INFO, 0, val);
    return NOERROR;
}

ECode CWifiP2pManager::RequestGroupInfo(
    /* [in] */ IWifiP2pManagerChannel* c,
    /* [in] */ IWifiP2pManagerGroupInfoListener* listener)
{
    if (DBG) Slogger::D(TAG, "RequestGroupInfo");
    FAIL_RETURN(CheckChannel(c));

    CWifiP2pManagerChannel* wpmc = (CWifiP2pManagerChannel*)c;
    AutoPtr<AsyncChannel> asyncChannel = wpmc->mAsyncChannel;

    Int32 val;
    c->PutListener(listener ? listener->Probe(EIID_IInterface) : NULL, &val);
    asyncChannel->SendMessage(REQUEST_GROUP_INFO, 0, val);
    return NOERROR;
}

ECode CWifiP2pManager::SetDeviceName(
    /* [in] */ IWifiP2pManagerChannel* c,
    /* [in] */ const String& devName,
    /* [in] */ IWifiP2pManagerActionListener* listener)
{
    if (DBG) Slogger::D(TAG, "SetDeviceName: [%s]", devName.string());
    FAIL_RETURN(CheckChannel(c));

    AutoPtr<IWifiP2pDevice> d;
    CWifiP2pDevice::New((IWifiP2pDevice**)&d);
    d->SetDeviceName(devName);

    CWifiP2pManagerChannel* wpmc = (CWifiP2pManagerChannel*)c;
    AutoPtr<AsyncChannel> asyncChannel = wpmc->mAsyncChannel;

    Int32 val;
    c->PutListener(listener ? listener->Probe(EIID_IInterface) : NULL, &val);
    asyncChannel->SendMessage(SET_DEVICE_NAME, 0, val, d->Probe(EIID_IInterface));
    return NOERROR;
}

/** @hide */
ECode CWifiP2pManager::SetWFDInfo(
    /* [in] */ IWifiP2pManagerChannel* c,
    /* [in] */ IWifiP2pWfdInfo* wfdInfo,
    /* [in] */ IWifiP2pManagerActionListener* listener)
{
    if (DBG) {
        String info;
        if (wfdInfo) wfdInfo->ToString(&info);
        Slogger::D(TAG, "SetWFDInfo: \n", info.string());
    }
    FAIL_RETURN(CheckChannel(c));

    CWifiP2pManagerChannel* wpmc = (CWifiP2pManagerChannel*)c;
    AutoPtr<AsyncChannel> asyncChannel = wpmc->mAsyncChannel;

    Int32 val;
    c->PutListener(listener ? listener->Probe(EIID_IInterface) : NULL, &val);
    asyncChannel->SendMessage(SET_WFD_INFO, 0, val,
        wfdInfo ? wfdInfo->Probe(EIID_IInterface) : NULL);
    return NOERROR;
}

ECode CWifiP2pManager::SetGroupOwnerPsk(
    /* [in] */ IWifiP2pManagerChannel* c,
    /* [in] */ const String& psk,
    /* [in] */ IWifiP2pManagerActionListener* listener)
{
    if (DBG) Slogger::D(TAG, "SetGroupOwnerPsk");
    FAIL_RETURN(CheckChannel(c));

    AutoPtr<IWifiP2pDevice> d;
    CWifiP2pDevice::New((IWifiP2pDevice**)&d);
    d->SetDeviceName(psk);

    CWifiP2pManagerChannel* wpmc = (CWifiP2pManagerChannel*)c;
    AutoPtr<AsyncChannel> asyncChannel = wpmc->mAsyncChannel;

    Int32 val;
    c->PutListener(listener ? listener->Probe(EIID_IInterface) : NULL, &val);
    asyncChannel->SendMessage(SET_GO_PSK, 0, val,
        d ? d->Probe(EIID_IInterface) : NULL);
    return NOERROR;
}

ECode CWifiP2pManager::SetDialogListener(
    /* [in] */ IWifiP2pManagerChannel* c,
    /* [in] */ IWifiP2pManagerDialogListener* listener)
{
    FAIL_RETURN(CheckChannel(c));
    c->SetDialogListener(listener);

    /**
     * mAsyncChannel should always stay private and inaccessible from the app
     * to prevent an app from sending a message with a fake app name to gain
     * control over the dialogs
     */
    AutoPtr<IMessageHelper> helper;
    CMessageHelper::AcquireSingleton((IMessageHelper**)&helper);
    AutoPtr<IMessage> msg;
    helper->Obtain((IMessage**)&msg);

    AutoPtr<IBundle> bundle;
    CBundle::New((IBundle**)&bundle);
    AutoPtr<IContext> context;
    c->GetContext((IContext**)&context);
    String pkgName;
    context->GetPackageName(&pkgName);
    bundle->PutString(APP_PKG_BUNDLE_KEY, pkgName);
    bundle->PutBoolean(RESET_DIALOG_LISTENER_BUNDLE_KEY, listener == NULL);
    msg->SetWhat(SET_DIALOG_LISTENER);
    msg->SetData(bundle);

    CWifiP2pManagerChannel* wpmc = (CWifiP2pManagerChannel*)c;
    AutoPtr<AsyncChannel> asyncChannel = wpmc->mAsyncChannel;
    asyncChannel->SendMessage(msg);
    return NOERROR;
}

ECode CWifiP2pManager::DeletePersistentGroup(
    /* [in] */ IWifiP2pManagerChannel* c,
    /* [in] */ Int32 netId,
    /* [in] */ IWifiP2pManagerActionListener* listener)
{
    FAIL_RETURN(CheckChannel(c));

    CWifiP2pManagerChannel* wpmc = (CWifiP2pManagerChannel*)c;
    AutoPtr<AsyncChannel> asyncChannel = wpmc->mAsyncChannel;

    Int32 val;
    c->PutListener(listener ? listener->Probe(EIID_IInterface) : NULL, &val);
    asyncChannel->SendMessage(DELETE_PERSISTENT_GROUP, netId, val);
    return NOERROR;
}

ECode CWifiP2pManager::RequestPersistentGroupInfo(
    /* [in] */ IWifiP2pManagerChannel* c,
    /* [in] */ IWifiP2pManagerPersistentGroupInfoListener* listener)
{
    FAIL_RETURN(CheckChannel(c));

    CWifiP2pManagerChannel* wpmc = (CWifiP2pManagerChannel*)c;
    AutoPtr<AsyncChannel> asyncChannel = wpmc->mAsyncChannel;

    Int32 val;
    c->PutListener(listener ? listener->Probe(EIID_IInterface) : NULL, &val);
    asyncChannel->SendMessage(REQUEST_PERSISTENT_GROUP_INFO, 0, val);
    return NOERROR;
}

ECode CWifiP2pManager::GetMessenger(
    /* [out] */ IMessenger** msg)
{
    VALIDATE_NOT_NULL(msg);
    *msg = NULL;

    // try {
    return mService->GetMessenger((IMessenger**)msg);
    // } catch (RemoteException e) {
    //     return null;
    // }
}

ECode CWifiP2pManager::CheckChannel(
    /* [in] */ IWifiP2pManagerChannel* c)
{
    if (c == NULL) {
        Slogger::E(TAG, "E_ILLEGAL_ARGUMENT_EXCEPTION: Channel needs to be initialized");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    return NOERROR;
}

ECode CWifiP2pManager::CheckServiceInfo(
    /* [in] */ IWifiP2pServiceInfo* info)
{
    if (info == NULL) {
        Slogger::E(TAG, "E_ILLEGAL_ARGUMENT_EXCEPTION: service info is NULL");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    return NOERROR;
}

ECode CWifiP2pManager::CheckServiceRequest(
    /* [in] */ IWifiP2pServiceRequest* req)
{
    if (req == NULL) {
        Slogger::E(TAG, "E_ILLEGAL_ARGUMENT_EXCEPTION: service request is NULL");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    return NOERROR;
}

}
}
}
}
}
