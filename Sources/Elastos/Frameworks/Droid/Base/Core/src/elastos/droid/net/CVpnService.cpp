
#include "elastos/droid/net/CVpnService.h"

namespace Elastos {
namespace Droid {
namespace Net {

ECode CVpnService::Protect(
    /* [in] */ Int32 socket,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    AutoPtr<IParcelFileDescriptorHelper> helper;
    CParcelFileDescriptorHelper::AcquireSingleton((IParcelFileDescriptorHelper**)&helper);

    AutoPtr<IParcelFileDescriptor> dup;
    ECode ec = helper->FromFd(socket, (IParcelFileDescriptor**)&dup);
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

    AutoPtr<IVpnConfigHelper> helper;
    CVpnConfigHelper::AcquireSingleton((IVpnConfigHelper**)&helper);

    AutoPtr<IIntent> intent;
    helper->GetIntentForConfirmation((IIntent**)&intent);
    return intent;
}

} // namespace Net
} // namepsace Droid
} // namespace Elastos
