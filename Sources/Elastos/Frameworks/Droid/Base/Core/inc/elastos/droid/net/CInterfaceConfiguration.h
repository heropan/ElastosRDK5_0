
#ifndef __ELASTOS_DROID_NET_CINTERFACECONFIGURATION_H__
#define __ELASTOS_DROID_NET_CINTERFACECONFIGURATION_H__

#include "elastos/droid/ext/frameworkdef.h"
#include "_Elastos_Droid_Net_CInterfaceConfiguration.h"
#include <elastos/core/Object.h>
#include <elastos/utility/etl/HashSet.h>

using Elastos::Droid::Collect::ISets;

using Elastos::Net::IInetAddress;
using Elastos::Utility::IHashSet;
using Elastos::Utility::IIterable;

namespace Elastos {
namespace Droid {
namespace Net {

CarClass(CInterfaceConfiguration)
    , public Object
    , public IParcelable
    , public IInterfaceConfiguration
{
public:
    CAR_OBJECT_DECL()

    CAR_INTERFACE_DECL()

    CInterfaceConfiguration();

    CARAPI constructor();

    CARAPI ToString(
        /* [out] */ String* result);

    CARAPI GetFlags(
        /* [out] */ IIterable** result);

    CARAPI HasFlag(
        /* [in] */ const String& flag,
        /* [out] */ Boolean* result);

    CARAPI ClearFlag(
        /* [in] */ const String& flag);

    CARAPI SetFlag(
        /* [in] */ const String& flag);

    /**
     * Set flags to mark interface as up.
     */
    CARAPI SetInterfaceUp();

    /**
     * Set flags to mark interface as down.
     */
    CARAPI SetInterfaceDown();

    CARAPI GetLinkAddress(
        /* [out] */ ILinkAddress** result);

    CARAPI SetLinkAddress(
        /* [in] */ ILinkAddress* addr);

    CARAPI GetHardwareAddress(
        /* [out] */ String* result);

    CARAPI SetHardwareAddress(
        /* [in] */ const String& hwAddr);

    /**
     * This function determines if the interface is up and has a valid IP
     * configuration (IP address has a non zero octet).
     *
     * Note: It is supposed to be quick and hence should not initiate
     * any network activity
     */
    CARAPI IsActive(
        /* [out] */ Boolean* result);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* src);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

private:
    static CARAPI ValidateFlag(
        /* [in] */ const String& flag);

private:
    String mHwAddr;
    AutoPtr<ILinkAddress> mAddr;
    AutoPtr<IHashSet> mFlags;

    static const String FLAG_UP;
    static const String FLAG_DOWN;
};

} // namespace Net
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_NET_CINTERFACECONFIGURATION_H__
