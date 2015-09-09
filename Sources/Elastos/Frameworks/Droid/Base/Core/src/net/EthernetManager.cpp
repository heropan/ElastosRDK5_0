
#include "EthernetManager.h"

namespace Elastos {
namespace Droid {
namespace Net {

CAR_INTERFACE_IMPL(EthernetManager, Object, IEthernetManager)

const String EthernetManager::TAG = String("EthernetManager");

ECode EthernetManager::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IIEthernetManager* service)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        mContext = context;
        mService = service;

#endif
}

ECode EthernetManager::GetConfiguration(
    /* [out] */ IIpConfiguration** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        if (mService == null) {
            return new IpConfiguration();
        }
        try {
            return mService.getConfiguration();
        } catch (RemoteException e) {
            return new IpConfiguration();
        }

#endif
}

ECode EthernetManager::SetConfiguration(
    /* [in] */ IIpConfiguration* config)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        if (mService == null) {
            return;
        }
        try {
            mService.setConfiguration(config);
        } catch (RemoteException e) {
        }

#endif
}

} // namespace Net
} // namespace Droid
} // namespace Elastos
