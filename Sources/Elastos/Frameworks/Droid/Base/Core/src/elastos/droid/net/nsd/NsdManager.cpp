
#include "elastos/droid/net/nsd/NsdManager.h"

using Elastos::Droid::Content::IContext;
using Elastos::Droid::Internal::Utility::IAsyncChannel;
using Elastos::Droid::Internal::Utility::IProtocol;
using Elastos::Droid::Os::IHandler;
using Elastos::Droid::Os::IHandlerThread;
using Elastos::Droid::Os::ILooper;
using Elastos::Droid::Os::IMessage;
using Elastos::Droid::Os::IMessenger;
using Elastos::Droid::Text::ITextUtils;
using Elastos::Droid::Utility::ILog;

using Elastos::Utility::Concurrent::ICountDownLatch;

namespace Elastos {
namespace Droid {
namespace Net {
namespace Nsd {

CAR_INTERFACE_IMPL(NsdManager, Object, INsdManager)

const String NsdManager::TAG("NsdManager");
const Int32 NsdManager::BASE = IProtocol::BASE_NSD_MANAGER;
const Int32 NsdManager::INVALID_LISTENER_KEY = 0;
const Int32 NsdManager::BUSY_LISTENER_KEY = -1;

NsdManager::NsdManager()
    : mListenerKey(1)
{
#if 0 // TODO: Translate codes below
    private final SparseArray mListenerMap = new SparseArray();
    private final SparseArray<NsdServiceInfo> mServiceMap = new SparseArray<NsdServiceInfo>();
    private final Object mMapLock = new Object();
    private final AsyncChannel mAsyncChannel = new AsyncChannel();
    private final CountDownLatch mConnected = new CountDownLatch(1);
#endif
}

ECode NsdManager::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IINsdManager* service)
{
    mService = service;
    mContext = context;
    return Init();
}

ECode NsdManager::PutListener(
    /* [in] */ IInterface* listener,
    /* [in] */ INsdServiceInfo* s,
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    if (listener == NULL) {
        Slogger::E(TAG, "listener cannot be null");
        return INVALID_LISTENER_KEY;
    }

    Int32 key;
    AutoLock lock(mMapLock);
    Int32 valueIndex;
    mListenerMap->IndexOfValue(listener, &valueIndex);
    if (valueIndex != -1) {
        return BUSY_LISTENER_KEY;
    }
    do {
        key = mListenerKey++;
    }
    while (key == INVALID_LISTENER_KEY);

    typedef typename HashMap<Int32, AutoPtr<IInterface> >::ValueType PairValueType1;
    mListenerMap.Insert(PairValueType1(key, listener));

    typedef typename HashMap<Int32, AutoPtr<INsdServiceInfo> >::ValueType PairValueType2;
    mListenerMap.Insert(PairValueType2(key, s));
    return key;
#endif
}

ECode NsdManager::GetListener(
    /* [in] */ Int32 key,
    /* [out] */ IInterface** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
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
#endif
}

ECode NsdManager::GetNsdService(
    /* [in] */ Int32 key,
    /* [out] */ INsdServiceInfo** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    AutoLock lock(mMapLock);
    HashMap<Int32, AutoPtr<INsdServiceInfo> >::Iterator it = mServiceMap.Find(key);
    if(it != mServiceMap.End())
        return it->mSecond;

    return NULL;
#endif
}

ECode NsdManager::RemoveListener(
    /* [in] */ Int32 key)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    if (key == INVALID_LISTENER_KEY) return;

    AutoLock lock(mMapLock);

    HashMap<Int32, AutoPtr<IInterface> >::Iterator it = mListenerMap.Find(key);
    if(it != mListenerMap.End())
        mListenerMap.Erase(it);

    HashMap<Int32, AutoPtr<INsdServiceInfo> >::Iterator ite = mServiceMap.Find(key);
    if(ite != mServiceMap.End())
        mServiceMap.Erase(ite);
#endif
}

ECode NsdManager::GetListenerKey(
    /* [in] */ IInterface* listener,
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    AutoLock lock(mMapLock);
    HashMap<Int32, AutoPtr<IInterface> >::Iterator it;
    for (it = mListenerMap.Begin(); it != mListenerMap.End(); ++it) {
        if (it->mSecond.Get() == listener) {
            return it->mFirst;
        }
    }

    return INVALID_LISTENER_KEY;
#endif
}

ECode NsdManager::GetNsdServiceInfoType(
    /* [in] */ INsdServiceInfo* s,
    /* [out] */ String* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        if (s == null) return "?";
        return s.getServiceType();
#endif
}

ECode NsdManager::Init()
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
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
#endif
}

ECode NsdManager::RegisterService(
    /* [in] */ INsdServiceInfo* serviceInfo,
    /* [in] */ Int32 protocolType,
    /* [in] */ INsdManagerRegistrationListener* listener)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
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

    Int32 key = PutListener(listener, serviceInfo);
    if (key == BUSY_LISTENER_KEY) {
        // throw new IllegalArgumentException("listener already in use");
        Logger::E(TAG, "listener already in use");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    mAsyncChannel->SendMessage(INsdManager::REGISTER_SERVICE, 0, key, serviceInfo);
    return NOERROR;
#endif
}

ECode NsdManager::UnregisterService(
    /* [in] */ INsdManagerRegistrationListener* listener)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
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
#endif
}

ECode NsdManager::DiscoverServices(
    /* [in] */ const String& serviceType,
    /* [in] */ Int32 protocolType,
    /* [in] */ INsdManagerDiscoveryListener* listener)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
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
    Int32 key = PutListener(listener, s);
    if (key == BUSY_LISTENER_KEY) {
        // throw new IllegalArgumentException("listener already in use");
        Logger::E(TAG, "listener already in use");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    mAsyncChannel->SendMessage(INsdManager::DISCOVER_SERVICES, 0, key, s);
    return NOERROR;
#endif
}

ECode NsdManager::StopServiceDiscovery(
    /* [in] */ INsdManagerDiscoveryListener* listener)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
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
#endif
}

ECode NsdManager::ResolveService(
    /* [in] */ INsdServiceInfo* serviceInfo,
    /* [in] */ INsdManagerResolveListener* listener)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
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

    Int32 key = PutListener(listener, serviceInfo);
    if (key == BUSY_LISTENER_KEY) {
        // throw new IllegalArgumentException("listener already in use");
        Logger::E(TAG, "listener already in use");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    mAsyncChannel->SendMessage(INsdManager::RESOLVE_SERVICE, 0, key, serviceInfo);
    return NOERROR;
#endif
}

ECode NsdManager::SetEnabled(
    /* [in] */ Boolean enabled)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    // try {
    return mService->SetEnabled(enabled);
    // } catch (RemoteException e) { }
#endif
}

ECode NsdManager::GetMessenger(
    /* [out] */ IMessenger** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    // try {
    AutoPtr<IMessenger> messenger;
    mService->GetMessenger((IMessenger**)&messenger);
    return messenger;
    // } catch (RemoteException e) {
    //     return NULL;
    // }
#endif
}

//===========================================================================
// NsdManager::ServiceHandler
//===========================================================================
NsdManager::ServiceHandler::ServiceHandler(
    /* [in] */ ILooper* looper)
    : Handler(looper)
{}

ECode NsdManager::ServiceHandler::HandleMessage(
    /* [in] */ IMessage* message)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    VALIDATE_NOT_NULL(message);

    Int32 what, arg1, arg2;
    message->GetWhat(&what);
    message->GetArg1(&arg1);
    message->GetArg2(&arg2);

    // Slogger::D("CNsdManager::ServiceHandler", " >> HandleMessage %s", CodeToString(what).string());

    switch (what) {
        case AsyncChannel::CMD_CHANNEL_HALF_CONNECTED:
            mHost->mAsyncChannel->SendMessage(AsyncChannel::CMD_CHANNEL_FULL_CONNECTION);
            return;
        case AsyncChannel::CMD_CHANNEL_FULLY_CONNECTED:
            mHost->mConnected->CountDown();
            return;
        case AsyncChannel::CMD_CHANNEL_DISCONNECTED:
            Slogger::E("CNsdManager::ServiceHandler", "Channel lost");
            return;
        default:
            break;
        }
        AutoPtr<IInterface> listener;
        mHost->GetListener(arg2, (IInterface**)&listener);
        if (NULL == listener) {
            Slogger::D(TAG, "Stale key " + message.arg2);
            return;
        }
        AutoPtr<INsdServiceInfo> ns;
        GetNsdService(message->arg2, (INsdServiceInfo**)&ns);
        switch (message->what) {
        case INsdManager::DISCOVER_SERVICES_STARTED: {
            AutoPtr<IInterface> obj;
            message->GetObj((IInterface**)&obj);
            AutoPtr<INsdServiceInfo> info = INsdServiceInfo::Probe(obj);
            String s;
            GetNsdServiceInfoType(info, &s);
            AutoPtr<INsdManagerDiscoveryListener> dl = INsdManagerDiscoveryListener::Probe(listener);
            dl->OnDiscoveryStarted(s);
            break;
        }
        case INsdManager::DISCOVER_SERVICES_FAILED: {
            Int32 arg2;
            message->GetArg2(&arg2);
            RemoveListener(arg2);
            AutoPtr<INsdManagerDiscoveryListener> dl = INsdManagerDiscoveryListener::Probe(listener);
            String type;
            GetNsdServiceInfoType(ns, &type)
            Int32 arg1;
            message->GetArg1(&arg1)
            dl->OnStartDiscoveryFailed(type, arg1);
            break;
        }
        case INsdManager::SERVICE_FOUND: {
            AutoPtr<IInterface> obj;
            message->GetObj((IInterface**)&obj);
            INsdServiceInfo* info = INsdServiceInfo::Probe(obj);
            INsdManagerDiscoveryListener* dl = INsdManagerDiscoveryListener::Probe(listener);
            dl->OnServiceFound(info);
            break;
        }
        case INsdManager::SERVICE_LOST: {
            AutoPtr<IInterface> obj;
            message->GetObj((IInterface**)&obj);
            INsdServiceInfo* info = INsdServiceInfo::Probe(obj);
            INsdManagerDiscoveryListener* dl = INsdManagerDiscoveryListener::Probe(listener);
            dl->OnServiceLost(info);
            break;
        }
        case INsdManager::STOP_DISCOVERY_FAILED: {
                    // removeListener(message.arg2);
                    // ((DiscoveryListener) listener).onStopDiscoveryFailed(getNsdServiceInfoType(ns),
                            // message.arg1);
            break;
        }
        case INsdManager::STOP_DISCOVERY_SUCCEEDED: {
                    // removeListener(message.arg2);
                    // ((DiscoveryListener) listener).onDiscoveryStopped(getNsdServiceInfoType(ns));
            break;
        }
        case INsdManager::REGISTER_SERVICE_FAILED: {
                    // removeListener(message.arg2);
                    // ((RegistrationListener) listener).onRegistrationFailed(ns, message.arg1);
            break;
        }
        case INsdManager::REGISTER_SERVICE_SUCCEEDED: {
            AutoPtr<IInterface> obj;
            message->GetObj((IInterface**)&obj);
            INsdServiceInfo* info = INsdServiceInfo::Probe(obj);
            INsdManagerRegistrationListener* dl = INsdManagerRegistrationListener::Probe(listener);
            assert(dl != NULL);
            dl->OnServiceRegistered(info);
            break;
        }
        case INsdManager::UNREGISTER_SERVICE_FAILED: {
                    // removeListener(message.arg2);
                    // ((RegistrationListener) listener).onUnregistrationFailed(ns, message.arg1);
            break;
        }
        case INsdManager::UNREGISTER_SERVICE_SUCCEEDED: {
                    // removeListener(message.arg2);
                    // ((RegistrationListener) listener).onServiceUnregistered(ns);
            break;
        }
        case INsdManager::RESOLVE_SERVICE_FAILED: {
                    // removeListener(message.arg2);
                    // ((ResolveListener) listener).onResolveFailed(ns, message.arg1);
            break;
        }
        case INsdManager::RESOLVE_SERVICE_SUCCEEDED: {
            Int32 arg2;
            message->GetArg2(&arg2);
            removeListener(arg2);
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

    return NOERROR;
#endif
}

} // namespace Nsd
} // namespace Net
} // namespace Droid
} // namespace Elastos
