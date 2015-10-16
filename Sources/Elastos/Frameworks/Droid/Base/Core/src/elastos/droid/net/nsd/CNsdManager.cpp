#include "CNsdManager.h"
#include "CNsdServiceInfo.h"
#include <elastos/utility/logging/Slogger.h>
#include "elastos/droid/os/CHandlerThread.h"

using Elastos::Utility::Logging::Slogger;
using Elastos::Core::CString;
using Elastos::Droid::Utility::IProtocol;
using Elastos::Droid::Net::Nsd::CNsdServiceInfo;
using Elastos::Droid::Os::CHandlerThread;

namespace Elastos {
namespace Droid {
namespace Net {
namespace Nsd {

const String CNsdManager::TAG("NsdManager");
const Int32 CNsdManager::BASE = IProtocol::BASE_NSD_MANAGER;
const Int32 CNsdManager::INVALID_LISTENER_KEY = 0;

CNsdManager::CNsdManager()
    : mListenerKey(1)
{
}

CNsdManager::~CNsdManager()
{
}

ECode CNsdManager::constructor(
    /* [in] */ IContext *context,
    /* [in] */ IINsdManager* service)
{
    mService = service;
    mContext = context;
    return Init();
}

ECode CNsdManager::RegisterService(
    /* [in] */  INsdServiceInfo* serviceInfo,
    /* [in] */  Int32 protocolType,
    /* [in] */  INsdManagerRegistrationListener* listener)
{
    String name, type;
    serviceInfo->GetServiceName(&name);
    serviceInfo->GetServiceType(&type);
    if (name.IsNullOrEmpty() || type.IsNullOrEmpty()) {
        Slogger::E(TAG, "Service name or type cannot be empty");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    Int32 port;
    serviceInfo->GetPort(&port);
    if (port <= 0) {
        Slogger::E(TAG, "Invalid port number");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (listener == NULL) {
        Slogger::E(TAG, "listener cannot be NULL");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (protocolType != PROTOCOL_DNS_SD) {
        Slogger::E(TAG, "Unsupported protocol");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    Int32 ret = PutListener(listener, serviceInfo);
    mAsyncChannel->SendMessage(INsdManager::REGISTER_SERVICE, 0, ret, serviceInfo);
    return NOERROR;
}

ECode CNsdManager::UnregisterService(
    /* [in] */  INsdManagerRegistrationListener* listener)
{
    if (listener == NULL) {
        Slogger::E(TAG, "listener cannot be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    Int32 id = GetListenerKey(listener->Probe(EIID_IInterface));
    if (id == INVALID_LISTENER_KEY) {
        Slogger::E(TAG, "listener not registered");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    mAsyncChannel->SendMessage(INsdManager::UNREGISTER_SERVICE, 0, id);
    return NOERROR;
}


ECode CNsdManager::DiscoverServices(
    /* [in] */  const String& serviceType,
    /* [in] */  Int32 protocolType,
    /* [in] */  INsdManagerDiscoveryListener* listener)
{
    if (listener == NULL) {
        Slogger::E(TAG, "listener cannot be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    if (serviceType.IsNullOrEmpty()) {
        Slogger::E(TAG, "Service type cannot be empty");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    if (protocolType != PROTOCOL_DNS_SD) {
        Slogger::E(TAG, "Unsupported protocol");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    AutoPtr<INsdServiceInfo> s;
    CNsdServiceInfo::New((INsdServiceInfo**)&s);
    s->SetServiceType(serviceType);
    Int32 ret = PutListener(listener, s);
    mAsyncChannel->SendMessage(INsdManager::DISCOVER_SERVICES, 0, ret, s);
    return NOERROR;
}

ECode CNsdManager::StopServiceDiscovery(
    /* [in] */  INsdManagerDiscoveryListener* listener)
{
    if (listener == NULL) {
        Slogger::E(TAG, "listener cannot be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    Int32 id = GetListenerKey(listener);
    if (id == INVALID_LISTENER_KEY) {
        Slogger::E(TAG, "service discovery not active on listener");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    mAsyncChannel->SendMessage(INsdManager::STOP_DISCOVERY, 0, id);
    return NOERROR;
}

/**
 * Resolve a discovered service. An application can resolve a service right before
 * establishing a connection to fetch the IP and port details on which to setup
 * the connection.
 *
 * @param serviceInfo service to be resolved
 * @param listener to receive callback upon success or failure. Cannot be NULL.
 */
ECode CNsdManager::ResolveService(
    /* [in] */  INsdServiceInfo* serviceInfo,
    /* [in] */  INsdManagerResolveListener* listener)
{
    if (listener == NULL) {
        Slogger::E(TAG, "listener cannot be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    String name, type;
    serviceInfo->GetServiceName(&name);
    serviceInfo->GetServiceType(&type);

    if (name.IsNullOrEmpty() || type.IsNullOrEmpty()) {
        Slogger::E(TAG, "Service name or type cannot be empty");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    Int32 ret = PutListener(listener, serviceInfo);
    mAsyncChannel->SendMessage(INsdManager::RESOLVE_SERVICE, 0, ret, serviceInfo);
    return NOERROR;
}

ECode CNsdManager::SetEnabled(
    /* [in] */  Boolean enabled)
{
    // try {
    return mService->SetEnabled(enabled);
    // } catch (RemoteException e) { }
}

Int32 CNsdManager::PutListener(
    /* [in] */ IInterface* listener,
    /* [in] */ INsdServiceInfo* s)
{
    if (listener == NULL) {
        Slogger::E(TAG, "listener cannot be null");
        return INVALID_LISTENER_KEY;
    }

    Int32 key;
    AutoLock lock(mMapLock);
    do {
        key = mListenerKey++;
    }
    while (key == INVALID_LISTENER_KEY);

    typedef typename HashMap<Int32, AutoPtr<IInterface> >::ValueType PairValueType1;
    mListenerMap.Insert(PairValueType1(key, listener));

    typedef typename HashMap<Int32, AutoPtr<INsdServiceInfo> >::ValueType PairValueType2;
    mListenerMap.Insert(PairValueType2(key, s));
    return key;
}

ECode CNsdManager::GetListener(
    /* [in] */ Int32 key,
    /* [out] */ IInterface** listener)
{
    VALIDATE_NOT_NULL(listener);
    *listener = NULL;

    if (key == INVALID_LISTENER_KEY) {
        return NOERROR;
    }

    AutoLock lock(mMapLock);
    HashMap<Int32, AutoPtr<IInterface> >::Iterator it = mListenerMap.Find(key);
    if(it != mListenerMap.End()) {
        *listener = it->mSecond;
        REFCOUNT_ADD(*listener);
    }

    return NOERROR;
}

AutoPtr<INsdServiceInfo> CNsdManager::GetNsdService(
    /* [in] */ Int32 key)
{
    AutoLock lock(mMapLock);
    HashMap<Int32, AutoPtr<INsdServiceInfo> >::Iterator it = mServiceMap.Find(key);
    if(it != mServiceMap.End())
        return it->mSecond;

    return NULL;
}

void CNsdManager::RemoveListener(
    /* [in] */ Int32 key)
{
    if (key == INVALID_LISTENER_KEY) return;

    AutoLock lock(mMapLock);

    HashMap<Int32, AutoPtr<IInterface> >::Iterator it = mListenerMap.Find(key);
    if(it != mListenerMap.End())
        mListenerMap.Erase(it);

    HashMap<Int32, AutoPtr<INsdServiceInfo> >::Iterator ite = mServiceMap.Find(key);
    if(ite != mServiceMap.End())
        mServiceMap.Erase(ite);
}

Int32 CNsdManager::GetListenerKey(
    /* [in] */ IInterface* listener)
{
    AutoLock lock(mMapLock);
    HashMap<Int32, AutoPtr<IInterface> >::Iterator it;
    for (it = mListenerMap.Begin(); it != mListenerMap.End(); ++it) {
        if (it->mSecond.Get() == listener) {
            return it->mFirst;
        }
    }

    return INVALID_LISTENER_KEY;
}

ECode CNsdManager::Init()
{
    AutoPtr<IMessenger> messenger = GetMessenger();
    if (messenger == NULL)
    {
        Slogger::E(TAG, "Failed to initialize");
        return E_RUNTIME_EXCEPTION;
    }

    AutoPtr<IHandlerThread> t;
    CHandlerThread::New(String("NsdManager"), (IHandlerThread**)&t);
    t->Start();

    AutoPtr<ILooper> looper;
    t->GetLooper((ILooper**)&looper);
    mHandler = new ServiceHandler(looper, this);
    mAsyncChannel->Connect(mContext, mHandler, messenger);
    // try {
    ECode ec = mConnected->Await();
    if (FAILED(ec))
        Slogger::E(TAG, "interrupted wait at init");
    // } catch (InterruptedException e) {
    //    Log.e(TAG, "interrupted wait at init");
    // }
    return NOERROR;
}

AutoPtr<IMessenger> CNsdManager::GetMessenger()
{
    // try {
    AutoPtr<IMessenger> messenger;
    mService->GetMessenger((IMessenger**)&messenger);
    return messenger;
    // } catch (RemoteException e) {
    //     return NULL;
    // }
}

//===========================================================================
// CNsdManager::ServiceHandler
//===========================================================================
CNsdManager::ServiceHandler::ServiceHandler(
    /* [in] */ ILooper* looper,
    /* [in] */ CNsdManager* host)
    : HandlerBase(looper)
    , mHost(host)
{
}

String CNsdManager::ServiceHandler::CodeToString(
    /* [in] */ Int32 what)
{
    switch (what) {
        case AsyncChannel::CMD_CHANNEL_HALF_CONNECTED: return String("CMD_CHANNEL_HALF_CONNECTED");
        case AsyncChannel::CMD_CHANNEL_FULLY_CONNECTED: return String("CMD_CHANNEL_FULLY_CONNECTED");
        case AsyncChannel::CMD_CHANNEL_DISCONNECTED: return String("CMD_CHANNEL_DISCONNECTED");

        case INsdManager::DISCOVER_SERVICES_STARTED: return String("DISCOVER_SERVICES_STARTED");
        case INsdManager::DISCOVER_SERVICES_FAILED: return String("DISCOVER_SERVICES_FAILED");
        case INsdManager::SERVICE_FOUND: return String("SERVICE_FOUND");
        case INsdManager::SERVICE_LOST: return String("SERVICE_LOST");
        case INsdManager::STOP_DISCOVERY_FAILED: return String("STOP_DISCOVERY_FAILED");
        case INsdManager::STOP_DISCOVERY_SUCCEEDED: return String("STOP_DISCOVERY_SUCCEEDED");
        case INsdManager::REGISTER_SERVICE_FAILED: return String("REGISTER_SERVICE_FAILED");
        case INsdManager::REGISTER_SERVICE_SUCCEEDED: return String("REGISTER_SERVICE_SUCCEEDED");
        case INsdManager::UNREGISTER_SERVICE_FAILED: return String("UNREGISTER_SERVICE_FAILED");
        case INsdManager::UNREGISTER_SERVICE_SUCCEEDED: return String("UNREGISTER_SERVICE_SUCCEEDED");
        case INsdManager::RESOLVE_SERVICE_FAILED: return String("RESOLVE_SERVICE_FAILED");
        case INsdManager::RESOLVE_SERVICE_SUCCEEDED: return String("RESOLVE_SERVICE_SUCCEEDED");
    }
    return String("unknown message.");
}

ECode CNsdManager::ServiceHandler::HandleMessage(
    /* [in] */ IMessage* message)
{
    VALIDATE_NOT_NULL(message);

    Int32 what, arg1, arg2;
    message->GetWhat(&what);
    message->GetArg1(&arg1);
    message->GetArg2(&arg2);

    AutoPtr<IInterface> listener;
    mHost->GetListener(arg2, (IInterface**)&listener);

    Slogger::D("CNsdManager::ServiceHandler", " >> HandleMessage %s", CodeToString(what).string());

    Boolean listenerRemove = TRUE;
    switch (what) {
        case AsyncChannel::CMD_CHANNEL_HALF_CONNECTED:
            mHost->mAsyncChannel->SendMessage(AsyncChannel::CMD_CHANNEL_FULL_CONNECTION);
            mHost->mConnected->CountDown();
            break;
        case AsyncChannel::CMD_CHANNEL_FULLY_CONNECTED:
            // Ignore
            break;
        case AsyncChannel::CMD_CHANNEL_DISCONNECTED:
            Slogger::E("CNsdManager::ServiceHandler", "Channel lost");
            break;
        case INsdManager::DISCOVER_SERVICES_STARTED: {
            AutoPtr<IInterface> obj;
            message->GetObj((IInterface**)&obj);
            INsdServiceInfo* info = INsdServiceInfo::Probe(obj);
            assert(info != NULL);
            String s;
            info->GetServiceType(&s);
            INsdManagerDiscoveryListener* dl = INsdManagerDiscoveryListener::Probe(listener);
            assert(dl != NULL);
            dl->OnDiscoveryStarted(s);
            // Keep listener until stop discovery
            listenerRemove = FALSE;
            break;
        }
        case INsdManager::DISCOVER_SERVICES_FAILED: {
            AutoPtr<INsdServiceInfo> info = mHost->GetNsdService(arg2);
            String s;
            info->GetServiceType(&s);
            INsdManagerDiscoveryListener* dl = INsdManagerDiscoveryListener::Probe(listener);
            assert(dl != NULL);
            dl->OnStartDiscoveryFailed(s, arg1);
            break;
        }
        case INsdManager::SERVICE_FOUND: {
            AutoPtr<IInterface> obj;
            message->GetObj((IInterface**)&obj);
            INsdServiceInfo* info = INsdServiceInfo::Probe(obj);
            INsdManagerDiscoveryListener* dl = INsdManagerDiscoveryListener::Probe(listener);
            dl->OnServiceFound(info);
            // Keep listener until stop discovery
            listenerRemove = FALSE;
            break;
        }
        case INsdManager::SERVICE_LOST: {
            AutoPtr<IInterface> obj;
            message->GetObj((IInterface**)&obj);
            INsdServiceInfo* info = INsdServiceInfo::Probe(obj);
            INsdManagerDiscoveryListener* dl = INsdManagerDiscoveryListener::Probe(listener);
            dl->OnServiceLost(info);
            // Keep listener until stop discovery
            listenerRemove = FALSE;
            break;
        }
        case INsdManager::STOP_DISCOVERY_FAILED: {
            AutoPtr<INsdServiceInfo> info = mHost->GetNsdService(arg2);
            String s;
            info->GetServiceType(&s);
            INsdManagerDiscoveryListener* dl = INsdManagerDiscoveryListener::Probe(listener);
            assert(dl != NULL);
            dl->OnStopDiscoveryFailed(s, arg1);
            break;
        }
        case INsdManager::STOP_DISCOVERY_SUCCEEDED: {
            AutoPtr<INsdServiceInfo> info = mHost->GetNsdService(arg2);
            String s;
            info->GetServiceType(&s);
            INsdManagerDiscoveryListener* dl = INsdManagerDiscoveryListener::Probe(listener);
            assert(dl != NULL);
            dl->OnDiscoveryStopped(s);
            break;
        }
        case INsdManager::REGISTER_SERVICE_FAILED: {
            AutoPtr<INsdServiceInfo> info = mHost->GetNsdService(arg2);
            INsdManagerRegistrationListener* dl = INsdManagerRegistrationListener::Probe(listener);
            assert(dl != NULL);
            dl->OnRegistrationFailed(info, arg1);
            break;
        }
        case INsdManager::REGISTER_SERVICE_SUCCEEDED: {
            AutoPtr<IInterface> obj;
            message->GetObj((IInterface**)&obj);
            INsdServiceInfo* info = INsdServiceInfo::Probe(obj);
            INsdManagerRegistrationListener* dl = INsdManagerRegistrationListener::Probe(listener);
            assert(dl != NULL);
            dl->OnServiceRegistered(info);
            // Keep listener until unregister
            listenerRemove = FALSE;
            break;
        }
        case INsdManager::UNREGISTER_SERVICE_FAILED: {
            AutoPtr<INsdServiceInfo> info = mHost->GetNsdService(arg2);
            INsdManagerRegistrationListener* dl = INsdManagerRegistrationListener::Probe(listener);
            assert(dl != NULL);
            dl->OnUnregistrationFailed(info, arg1);
            break;
        }
        case INsdManager::UNREGISTER_SERVICE_SUCCEEDED: {
            AutoPtr<IInterface> obj;
            message->GetObj((IInterface**)&obj);
            INsdServiceInfo* info = INsdServiceInfo::Probe(obj);
            INsdManagerRegistrationListener* dl = INsdManagerRegistrationListener::Probe(listener);
            assert(dl != NULL);
            dl->OnServiceUnregistered(info);
             // Keep listener until unregister
            listenerRemove = FALSE;
            break;
        }
        case INsdManager::RESOLVE_SERVICE_FAILED: {
            AutoPtr<INsdServiceInfo> info = mHost->GetNsdService(arg2);
            INsdManagerResolveListener* dl = INsdManagerResolveListener::Probe(listener);
            assert(dl != NULL);
            dl->OnResolveFailed(info, arg1);
            break;
        }
        case INsdManager::RESOLVE_SERVICE_SUCCEEDED: {
            AutoPtr<IInterface> obj;
            message->GetObj((IInterface**)&obj);
            INsdServiceInfo* info = INsdServiceInfo::Probe(obj);
            INsdManagerResolveListener* dl = INsdManagerResolveListener::Probe(listener);
            dl->OnServiceResolved(info);
        }

        default:
            Slogger::D("CNsdManager::ServiceHandler", "Ignored %d", what);
            break;
    }

    if (listenerRemove) {
        mHost->RemoveListener(arg2);
    }

    return NOERROR;
}

}
}
}
}
