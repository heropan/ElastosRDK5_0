
#include "elastos/droid/net/VpnService.h"

namespace Elastos {
namespace Droid {
namespace Net {

//=============================================================
// VpnService::Callback
//=============================================================
ECode VpnService::Callback::OnTransact(
    /* [in] */ Int32 code,
    /* [in] */ IParcel* data,
    /* [in] */ IParcel* reply,
    /* [in] */ Int32 flags,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
            if (code == IBinder.LAST_CALL_TRANSACTION) {
                onRevoke();
                return TRUE;
            }
            return FALSE;
#endif
}

//=============================================================
// VpnService
//=============================================================
#if 0 // TODO: Waiting for Service
CAR_INTERFACE_IMPL(VpnService, Service, IVpnService)
#else
CAR_INTERFACE_IMPL(VpnService, Object, IVpnService)
#endif

ECode VpnService::constructor()
{
    return NOERROR;
}

ECode VpnService::GetService(
    /* [out] */ IIConnectivityManager** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return IConnectivityManager::Probe(
                ServiceManager::GetService(Context.CONNECTIVITY_SERVICE));
#endif
}

ECode VpnService::Prepare(
    /* [in] */ IContext* context,
    /* [out] */ IIntent** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        try {
            if (getService().prepareVpn(context.getPackageName(), NULL)) {
                return NULL;
            }
        } catch (RemoteException e) {
            // ignore
        }
        return VpnConfig.getIntentForConfirmation();
#endif
}

ECode VpnService::Protect(
    /* [in] */ Int32 socket,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return NetworkUtils.protectFromVpn(socket);
#endif
}

ECode VpnService::Protect(
    /* [in] */ ISocket* socket,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return protect(socket.getFileDescriptor$().getInt$());
#endif
}

ECode VpnService::Protect(
    /* [in] */ IDatagramSocket* socket,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return protect(socket.getFileDescriptor$().getInt$());
#endif
}

ECode VpnService::AddAddress(
    /* [in] */ IInetAddress* address,
    /* [in] */ Int32 prefixLength,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        check(address, prefixLength);
        try {
            return getService().addVpnAddress(address.getHostAddress(), prefixLength);
        } catch (RemoteException e) {
            throw new IllegalStateException(e);
        }
#endif
}

ECode VpnService::RemoveAddress(
    /* [in] */ IInetAddress* address,
    /* [in] */ Int32 prefixLength,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        check(address, prefixLength);
        try {
            return getService().removeVpnAddress(address.getHostAddress(), prefixLength);
        } catch (RemoteException e) {
            throw new IllegalStateException(e);
        }
#endif
}

ECode VpnService::OnBind(
    /* [in] */ IIntent* intent,
    /* [out] */ IBinder** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        if (intent != NULL && SERVICE_INTERFACE.equals(intent.getAction())) {
            return new Callback();
        }
        return NULL;
#endif
}

ECode VpnService::OnRevoke()
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        stopSelf();
#endif
}

ECode VpnService::Check(
    /* [in] */ IInetAddress* address,
    /* [in] */ Int32 prefixLength)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        if (address.isLoopbackAddress()) {
            throw new IllegalArgumentException("Bad address");
        }
        if (IInet4Address::Probe(address) != NULL) {
            if (prefixLength < 0 || prefixLength > 32) {
                throw new IllegalArgumentException("Bad prefixLength");
            }
        } else if (IInet6Address::Probe(address) != NULL) {
            if (prefixLength < 0 || prefixLength > 128) {
                throw new IllegalArgumentException("Bad prefixLength");
            }
        } else {
            throw new IllegalArgumentException("Unsupported family");
        }
#endif
}

//=====================================================
// VpnServiceBuilder
//=====================================================

CAR_INTERFACE_IMPL(VpnServiceBuilder, Object, IVpnServiceBuilder)

VpnServiceBuilder::VpnServiceBuilder()
    : mConfig(CreateConfig())
    , mAddresses(CreateList())
    , mRoutes(CreateList())
{}

AutoPtr<IVpnConfig> VpnServiceBuilder::CreateConfig()
{
    AutoPtr<IVpnConfig> rev;
#if 0 // TODO: Waiting for CVpnConfig
    CVpnConfig::New((IVpnConfig**)&rev);
#else
    assert(0);
#endif
    return rev;

}

AutoPtr<IList> VpnServiceBuilder::CreateList()
{
    AutoPtr<IList> rev;
#if 0 // TODO: Waiting for CList
    CList::New((IList**)&rev);
#else
    assert(0);
#endif
    return rev;

}

ECode VpnServiceBuilder::constructor()
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
                mConfig.user = VpnService.this.getClass().getName();
#endif
}

ECode VpnServiceBuilder::SetSession(
    /* [in] */ const String& session,
    /* [out] */ IVpnServiceBuilder** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
                mConfig.session = session;
                return this;
#endif
}

ECode VpnServiceBuilder::SetConfigureIntent(
    /* [in] */ IPendingIntent* intent,
    /* [out] */ IVpnServiceBuilder** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
                mConfig.configureIntent = intent;
                return this;
#endif
}

ECode VpnServiceBuilder::SetMtu(
    /* [in] */ Int32 mtu,
    /* [out] */ IVpnServiceBuilder** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
                if (mtu <= 0) {
                    throw new IllegalArgumentException("Bad mtu");
                }
                mConfig.mtu = mtu;
                return this;
#endif
}

ECode VpnServiceBuilder::AddAddress(
    /* [in] */ IInetAddress* address,
    /* [in] */ Int32 prefixLength,
    /* [out] */ IVpnServiceBuilder** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
                check(address, prefixLength);
                if (address.isAnyLocalAddress()) {
                    throw new IllegalArgumentException("Bad address");
                }
                mAddresses.add(new LinkAddress(address, prefixLength));
                mConfig.updateAllowedFamilies(address);
                return this;
#endif
}

ECode VpnServiceBuilder::AddAddress(
    /* [in] */ const String& address,
    /* [in] */ Int32 prefixLength,
    /* [out] */ IVpnServiceBuilder** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
                return addAddress(InetAddress.parseNumericAddress(address), prefixLength);
#endif
}

ECode VpnServiceBuilder::AddRoute(
    /* [in] */ IInetAddress* address,
    /* [in] */ Int32 prefixLength,
    /* [out] */ IVpnServiceBuilder** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
                check(address, prefixLength);
                Int32 offset = prefixLength / 8;
                byte[] bytes = address.getAddress();
                if (offset < bytes.length) {
                    for (bytes[offset] <<= prefixLength % 8; offset < bytes.length; ++offset) {
                        if (bytes[offset] != 0) {
                            throw new IllegalArgumentException("Bad address");
                        }
                    }
                }
                mRoutes.add(new RouteInfo(new LinkAddress(address, prefixLength), NULL));
                mConfig.updateAllowedFamilies(address);
                return this;
#endif
}

ECode VpnServiceBuilder::AddRoute(
    /* [in] */ const String& address,
    /* [in] */ Int32 prefixLength,
    /* [out] */ IVpnServiceBuilder** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
                return addRoute(InetAddress.parseNumericAddress(address), prefixLength);
#endif
}

ECode VpnServiceBuilder::AddDnsServer(
    /* [in] */ IInetAddress* address,
    /* [out] */ IVpnServiceBuilder** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
                if (address.isLoopbackAddress() || address.isAnyLocalAddress()) {
                    throw new IllegalArgumentException("Bad address");
                }
                if (mConfig.dnsServers == NULL) {
                    mConfig.dnsServers = new ArrayList<String>();
                }
                mConfig.dnsServers.add(address.getHostAddress());
                return this;
#endif
}

ECode VpnServiceBuilder::AddDnsServer(
    /* [in] */ const String& address,
    /* [out] */ IVpnServiceBuilder** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
                return addDnsServer(InetAddress.parseNumericAddress(address));
#endif
}

ECode VpnServiceBuilder::AddSearchDomain(
    /* [in] */ const String& domain,
    /* [out] */ IVpnServiceBuilder** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
                if (mConfig.searchDomains == NULL) {
                    mConfig.searchDomains = new ArrayList<String>();
                }
                mConfig.searchDomains.add(domain);
                return this;
#endif
}

ECode VpnServiceBuilder::AllowFamily(
    /* [in] */ Int32 family,
    /* [out] */ IVpnServiceBuilder** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
                if (family == AF_INET) {
                    mConfig.allowIPv4 = TRUE;
                } else if (family == AF_INET6) {
                    mConfig.allowIPv6 = TRUE;
                } else {
                    throw new IllegalArgumentException(family + " is neither " + AF_INET + " nor " +
                            AF_INET6);
                }
                return this;
#endif
}

ECode VpnServiceBuilder::VerifyApp(
    /* [in] */ const String& packageName)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
                IPackageManager pm = IPackageManager::Probe(
                        ServiceManager::GetService("package"));
                try {
                    pm.getApplicationInfo(packageName, 0, UserHandle.getCallingUserId());
                } catch (RemoteException e) {
                    throw new IllegalStateException(e);
                }
#endif
}

ECode VpnServiceBuilder::AddAllowedApplication(
    /* [in] */ const String& packageName,
    /* [out] */ IVpnServiceBuilder** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
                if (mConfig.disallowedApplications != NULL) {
                    throw new UnsupportedOperationException("addDisallowedApplication already called");
                }
                verifyApp(packageName);
                if (mConfig.allowedApplications == NULL) {
                    mConfig.allowedApplications = new ArrayList<String>();
                }
                mConfig.allowedApplications.add(packageName);
                return this;
#endif
}

ECode VpnServiceBuilder::AddDisallowedApplication(
    /* [in] */ const String& packageName,
    /* [out] */ IVpnServiceBuilder** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
                if (mConfig.allowedApplications != NULL) {
                    throw new UnsupportedOperationException("addAllowedApplication already called");
                }
                verifyApp(packageName);
                if (mConfig.disallowedApplications == NULL) {
                    mConfig.disallowedApplications = new ArrayList<String>();
                }
                mConfig.disallowedApplications.add(packageName);
                return this;
#endif
}

ECode VpnServiceBuilder::AllowBypass(
    /* [out] */ IVpnServiceBuilder** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
                mConfig.allowBypass = TRUE;
                return this;
#endif
}

ECode VpnServiceBuilder::SetBlocking(
    /* [in] */ Boolean blocking,
    /* [out] */ IVpnServiceBuilder** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
                mConfig.blocking = blocking;
                return this;
#endif
}

ECode VpnServiceBuilder::Establish(
    /* [out] */ IParcelFileDescriptor** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
                mConfig.addresses = mAddresses;
                mConfig.routes = mRoutes;
                try {
                    return getService().establishVpn(mConfig);
                } catch (RemoteException e) {
                    throw new IllegalStateException(e);
                }
#endif
}

} // namespace Net
} // namespace Droid
} // namespace Elastos

#if 0 // old CVpnService.cpp
#include "net/CVpnService.h"

namespace Elastos {
namespace Droid {
namespace Net {

ECode CVpnService::Protect(
    /* [in] */ Int32 socket,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    AutoPtr<IParcelFileDescriptor> dup;
    ECode ec = CParcelFileDescriptor::FromFd(socket, (IParcelFileDescriptor**)&dup);
    if (FAILED(ec)) {
        *result = FALSE;
        return NOERROR;
    }

    *result = GetService()->ProtectVpn(dup);
    dup->Close();
    return NOERROR;
}

ECode CVpnService::Protect(
    /* [in] */ ISocket* socket,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    AutoPtr<IFileDescriptor> descriptor;
    socket->GetFileDescriptor((IFileDescriptor**)&descriptor);

    Int32 fd;
    descriptor->GetDescriptor(&fd);

    return Protect(fd, result);
}

ECode CVpnService::Protect(
    /* [in] */ IDatagramSocket* socket,
    /* [out] */ Boolean* reault)
{
    VALIDATE_NOT_NULL(reault);

    AutoPtr<IFileDescriptor> descriptor;
    socket->GetFileDescriptor((IFileDescriptor**)&descriptor);

    Int32 fd;
    descriptor->GetDescriptor(&fd);

    return Protect(fd, result);
}

ECode CVpnService::OnBind(
    /* [in] */ IIntent* intent,
    /* [out] */ IBinder** binder)
{
    VALIDATE_NOT_NULL(binder);

    String action;
    intent->GetAction(&action);

    if (intent != NULL && IVpnService::SERVICE_INTERFACE.Equals(action)) {

    }

    return NULL;
}

ECode CVpnService::OnRevoke()
{
    return Service::StopSelf();
}

AutoPtr<IIConnectivityManager> CVpnService::GetService()
{
    AutoPtr<IServiceManager> mgr;
    CServiceManager::AcquireSingleton((IServiceManager**)&mgr);

    AutoPtr<IInterface> service;
    mgr->GetService(IContext::CONNECTIVITY_SERVICE, (IInterface**)&service);

    AutoPtr<IIConnectivityManager> connService;
    connService = IIConnectivityManager::Probe(service);
    return connService;
}

AutoPtr<IIntent> CVpnService::PrepareImpl(
    /* [in] */ IContext* context)
{
    String packageName;
    context->GetPackageName(&packageName);

    Boolean result;
    GetService()->PrepareVpn(packageName, NULL, &result);

    if (result == FALSE) {
        return NULL;
    }

    AutoPtr<IIntent> intent;
    CVpnConfigHelper::GetIntentForConfirmation((IIntent**)&intent);
    return intent;
}

} // namespace Net
} // namepsace Droid
} // namespace Elastos
#endif


#if 0 // old CVpnServiceBuilder.cpp
#include "net/CVpnServiceBuilder.h"
#include "net/CVpnService.h"

namespace Elastos {
namespace Droid {
namespace Net {

CVpnServiceBuilder::CVpnServiceBuilder()
{}

ECode CVpnServiceBuilder::SetSession(
    /* [in] */ const String& session)
{
    return mConfig->SetSession(session);
}

ECode CVpnServiceBuilder::SetConfigureIntent(
    /* [in] */ IPendingIntent* intent)
{
    return mConfig->SetConfigureIntent(session);
}

ECode CVpnServiceBuilder::SetMtu(
    /* [in] */ Int32 mtu)
{
    if (mtu <= 0) {
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    return mConfig->SetMtu(mtu);
}

ECode CVpnServiceBuilder::AddAddress(
    /* [in] */ IInetAddress* address,
    /* [in] */ Int32 prefixLength)
{
    Check(address, prefixLength);

    Boolean isAnyLocalAddress;
    address->IsAnyLocalAddress(&isAnyLocalAddress);

    if (isAnyLocalAddress) {
        Logger::D("VpnService", "Bad address");
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    String hostAddress;
    address->GetHostAddress(&hostAddress);

    mAddresses += ' ';
    mAddresses += hostAddress;
    mAddresses += '/';
    mAddresses += prefixLength;

    return NOERROR;
}

ECode CVpnServiceBuilder::AddAddress(
    /* [in] */ const String& address,
    /* [in] */ Int32 prefixLength)
{
    AutoPtr<IInetAddressHelper> addressHelper;
    CInetAddressHelper::AcquireSingleton((IInetAddressHelper**)&addressHelper);

    AutoPtr<IInetAddress> addressObj;
    addressHelper->ParseNumericAddress(address, (IInetAddress**)&addressObj);

    return AddAddress(addressObj, prefixLength);
}

ECode CVpnServiceBuilder::AddRoute(
    /* [in] */ IInetAddress* address,
    /* [in] */ Int32 prefixLength)
{
    Check(address, prefixLength);

    Int32 offset = prefixLength / 8;

    AutoPtr< ArrayOf<Byte> > bytes;
    address->GetAddress((ArrayOf<Byte>**)&bytes);

    Int32 length = bytes->GetLength();

    if (offset < length) {
        for ((*bytes)[offset] <<= prefixLength % 8; offset < length; ++offset) {
            if ((*bytes)[offset] != 0) {
                Logger::D("VpnService", "Bad address");
                return E_ILLEGAL_STATE_EXCEPTION;
            }
        }
    }

    String hostAddress;
    address->GetHostAddress(&hostAddress);

    mRoutes += ' ';
    mRoutes += hostAddress;
    mRoutes += '/';
    mRoutes += prefixLength;

    return NOERROR;
}

ECode CVpnServiceBuilder::AddRoute(
    /* [in] */ const String& address,
    /* [in] */ Int32 prefixLength)
{
    AutoPtr<IInetAddressHelper> addressHelper;
    CInetAddressHelper::AcquireSingleton((IInetAddressHelper**)&addressHelper);

    AutoPtr<IInetAddress> addressObj;
    addressHelper->ParseNumericAddress(address, (IInetAddress**)&addressObj);

    return AddRoute(addressObj, prefixLength);
}

ECode CVpnServiceBuilder::AddDnsServer(
    /* [in] */ IInetAddress* address)
{
    Boolean isLoopbackAddress;
    address->IsLoopbackAddress(isLoopbackAddress);

    Boolean isAnyLocalAddress;
    address->IsAnyLocalAddress(&isAnyLocalAddress);

    if (isLoopbackAddress || isAnyLocalAddress) {
        Logger::D("VpnService", "Bad address");
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    AutoPtr<IObjectContainer> dnsServers;
    mConfig->GetDnsServers((IObjectContainer**)&dnsServers);

    if (dnsServers == NULL) {
        CObjectContainer::New((IObjectContainer**)&dnsServers);
        mConfig->SetDnsServers(dnsServers->Get());
    }

    String hostAddress;
    address->GetHostAddress(&hostAddress);

    AutoPtr<ICharSequence> elmt;
    CString::New(hostAddress, (IcharSequence**)&elmt);
    return dnsServers->Add(elmt->Get());
}

ECode CVpnServiceBuilder::AddDnsServer(
    /* [in] */ const String& address)
{
    AutoPtr<IInetAddressHelper> addressHelper;
    CInetAddressHelper::AcquireSingleton((IInetAddressHelper**)&addressHelper);

    AutoPtr<IInetAddress> addressObj;
    addressHelper->ParseNumericAddress(address, (IInetAddress**)&addressObj);

    return AddDnsServer(addressObj, prefixLength);
}

ECode CVpnServiceBuilder::AddSearchDomain(
    /* [in] */ const String& dnsDomain)
{
    AutoPtr<IObjectContainer> searchDomains;
    mConfig->GetSearchDomains((IObjectContainer**)&searchDomains);

    if (searchDomains == NULL) {
        CObjectContainer::New((IObjectContainer**)&searchDomains);
        mConfig->SetSearchDomains(searchDomains->Get());
    }

    AutoPtr<ICharSequence> elmt;
    CString::New(dnsDomain, (IcharSequence**)&elmt);
    return searchDomains->Add(elmt->Get());
}

ECode CVpnServiceBuilder::Establish(
    /* [out] */ IParcelFileDescriptor** pfd)
{
    VALIDATE_NOT_NULL(pfd);

    mConfig->SetAddresses((String)mAddresses);
    mConfig->SetRoutes((String)mRoutes);

    ECode ec = CVpnService::GetService()->EstablishVpn(mConfig);
    if (FAILED(ec)) {
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    return NOERROR;
}

ECode CVpnServiceBuilder::Check(
    /* [in] */ IInetAddress* address,
    /* [in] */ Int32 prefixLength)
{
    Boolean isLoopbackAddress;
    address->IsLoopbackAddress(isLoopbackAddress);

    if (isLoopbackAddress) {
        Logger::D("VpnService", "Bad address");
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    if (IInet4Address::Probe(address) != NULL) {
        if (prefixLength < 0 || prefixLength > 32) {
            Logger::D("VpnService", "Bad prefixLength");
            return E_ILLEGAL_STATE_EXCEPTION;
        }
    } else if (IInet6Address::Probe(address) != NULL) {
        if (prefixLength < 0 || prefixLength > 128) {
            Logger::D("VpnService", "Bad prefixLength");
            return E_ILLEGAL_STATE_EXCEPTION;
        }
    } else {
        Logger::D("VpnService", "Unsupported family");
        return E_ILLEGAL_STATE_EXCEPTION;
    }
}

} // namespace Net
} // namepsace Droid
} // namespace Elastos
#endif