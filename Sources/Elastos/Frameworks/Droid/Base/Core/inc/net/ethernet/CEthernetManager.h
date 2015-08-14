#ifndef __ELASTOS_DROID_NET_ETHERNET_CETHERNETMANAGER_H__
#define __ELASTOS_DROID_NET_ETHERNET_CETHERNETMANAGER_H__

#include "_CEthernetManager.h"

using Elastos::Droid::Os::IHandler;

namespace Elastos {
namespace Droid {
namespace Net {
namespace Ethernet {

CarClass(CEthernetManager)
{
public:
    static CARAPI GetInstance(
        /* [out] */ IEthernetManager** manager);

    CARAPI IsConfigured(
        /* [out] */ Boolean* result);

    CARAPI GetSavedConfig(
        /* [out] */ IEthernetDevInfo** info);

    CARAPI UpdateDevInfo(
        /* [in] */ IEthernetDevInfo* info);

    CARAPI GetDeviceNameList(
        /* [out] */ IObjectContainer** list);

    CARAPI SetEnabled(
        /* [in] */ Boolean enable);

    CARAPI GetState(
        /* [out] */ Int32* val);

    CARAPI GetTotalInterface(
        /* [out] */ Int32* val);

    CARAPI IsOn(
        /* [out] */ Boolean* result);

    CARAPI IsDhcp(
        /* [out] */ Boolean* result);

    CARAPI RemoveInterfaceFormService(
        /* [in] */ const String& name);

    CARAPI AddInterfaceToService(
        /* [in] */ const String& name,
        /* [out] */ Boolean* res);

    CARAPI SetDefaultConf();

    CARAPI CheckLink(
        /* [in] */ const String& ifname,
        /* [out] */ Int32* res);

    CARAPI constructor();

public:
    AutoPtr<IIEthernetManager> mService;
};

} // Ethernet
} // Net
} // Droid
} // Elastos

#endif // __ELASTOS_DROID_NET_ETHERNET_CETHERNETMANAGER_H__
