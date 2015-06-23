
#include "CEthernetManager.h"
#include "ext/frameworkdef.h"
#include "os/ServiceManager.h"
#include <elastos/utility/logging/Slogger.h>

using Elastos::Droid::Os::ServiceManager;
using Elastos::Droid::Content::IContext;
using Elastos::Utility::Logging::Slogger;

namespace Elastos {
namespace Droid {
namespace Net {
namespace Ethernet {

ECode CEthernetManager::GetInstance(
    /* [out] */ IEthernetManager** manager)
{
    VALIDATE_NOT_NULL(manager);
    return E_NOT_IMPLEMENTED;
}

ECode CEthernetManager::IsConfigured(
    /* [out] */ Boolean* result)
{
    // try {
    VALIDATE_NOT_NULL(result)
    return mService->IsConfigured(result);
    // } catch (RemoteException e) {
    //     Slog.i(TAG, "Can not check eth config state");
    // }
}

ECode CEthernetManager::GetSavedConfig(
    /* [out] */ IEthernetDevInfo** info)
{
    // try {
    VALIDATE_NOT_NULL(info)
    return mService->GetSavedConfig(info);
    // } catch (RemoteException e) {
    //     Slog.i(TAG, "Can not get eth config");
    // }
}

ECode CEthernetManager::UpdateDevInfo(
    /* [in] */ IEthernetDevInfo* info)
{
    // try {
    return mService->UpdateDevInfo(info);
    // } catch (RemoteException e) {
    //     Slog.i(TAG, "Can not update ethernet device info");
    // }
}

ECode CEthernetManager::GetDeviceNameList(
    /* [out] */ IObjectContainer** list)
{
    // try {
    VALIDATE_NOT_NULL(list)
    return mService->GetDeviceNameList(list);
    // } catch (RemoteException e) {
    //     return null;
    // }
}

ECode CEthernetManager::SetEnabled(
    /* [in] */ Boolean enable)
{
    // try {
    return mService->SetState(enable ? ETHERNET_STATE_ENABLED : ETHERNET_STATE_DISABLED);
    // } catch (RemoteException e) {
    //     Slog.i(TAG,"Can not set new state");
    // }
}

ECode CEthernetManager::GetState(
    /* [out] */ Int32* val)
{
    // try {
    VALIDATE_NOT_NULL(val)
    return mService->GetState(val);
    // } catch (RemoteException e) {
    //     return 0;
    // }
}

ECode CEthernetManager::GetTotalInterface(
    /* [out] */ Int32* val)
{
    // try {
    VALIDATE_NOT_NULL(val)
    return mService->GetTotalInterface(val);
    // } catch (RemoteException e) {
    //     return 0;
    // }
}

ECode CEthernetManager::IsOn(
    /* [out] */ Boolean* result)
{
    // try {
    VALIDATE_NOT_NULL(result)
    return mService->IsOn(result);
    // } catch (RemoteException e) {
    //     return false;
    // }
}

ECode CEthernetManager::IsDhcp(
    /* [out] */ Boolean* result)
{
    // try {
    VALIDATE_NOT_NULL(result)
    return mService->IsDhcp(result);
    // } catch (RemoteException e) {
    //     return false;
    // }
}

ECode CEthernetManager::RemoveInterfaceFormService(
    /* [in] */ const String& name)
{
    // try{
    return mService->RemoveInterfaceFormService(name);
    // } catch (RemoteException e) {
    // }
}

ECode CEthernetManager::AddInterfaceToService(
    /* [in] */ const String& name,
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);
    // try{
    return mService->AddInterfaceToService(name, res);
    // } catch (RemoteException e) {
    //     return false;
    // }
}

ECode CEthernetManager::SetDefaultConf()
{
    // try {
    return mService->SetMode(IEthernetDevInfo::ETHERNET_CONN_MODE_DHCP);
    // } catch (RemoteException e) {
    // }
}

ECode CEthernetManager::CheckLink(
    /* [in] */ const String& ifname,
    /* [out] */ Int32* res)
{
    VALIDATE_NOT_NULL(res);
    // try {
    return mService->CheckLink(ifname, res);
    // } catch (RemoteException e) {
    //     return 0;
    // }
}

ECode CEthernetManager::constructor()
{
    if (mService == NULL)
        mService = (IIEthernetManager*)ServiceManager::GetService(
            IContext::ETHERNET_SERVICE).Get();

    return NOERROR;
}

} // Ethernet
} // Net
} // Droid
} // Elastos
