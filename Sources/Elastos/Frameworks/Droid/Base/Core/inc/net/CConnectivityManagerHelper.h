
#ifndef __ELASTOS_DROID_NET_CCONNECTIVITYMANAGERHELPER_H__
#define __ELASTOS_DROID_NET_CCONNECTIVITYMANAGERHELPER_H__

#include "_Elastos_Droid_Net_CConnectivityManagerHelper.h"
#include <elastos/core/Singleton.h>

using Elastos::Droid::Content::IContext;
using Elastos::Core::Singleton;

namespace Elastos {
namespace Droid {
namespace Net {

CarClass(CConnectivityManagerHelper)
    , public Singleton
    , public IConnectivityManagerHelper
{
public:
    CAR_INTERFACE_DECL()

    CAR_SINGLETON_DECL()

    /**
     * Tests if a given integer represents a valid network type.
     * @param networkType the type to be tested
     * @return a boolean.  {@code true} if the type is valid, else {@code false}
     */
    CARAPI IsNetworkTypeValid(
        /* [in] */ Int32 networkType,
        /* [out] */ Boolean* result);

    /**
     * Returns a non-localized string representing a given network type.
     * ONLY used for debugging output.
     * @param type the type needing naming
     * @return a String for the given type, or a string version of the type ("87")
     * if no name is known.
     * {@hide}
     */
    CARAPI GetNetworkTypeName(
        /* [in] */ Int32 networkType,
        /* [out] */ String* result);

    /**
     * Checks if a given type uses the cellular data connection.
     * This should be replaced in the future by a network property.
     * @param networkType the type to check
     * @return a boolean - {@code true} if uses cellular network, else {@code false}
     * {@hide}
     */
    CARAPI  IsNetworkTypeMobile(
        /* [in] */ Int32 networkType,
        /* [out] */ Boolean* result);

    /**
     * Checks if the given network type is backed by a Wi-Fi radio.
     *
     * @hide
     */
    CARAPI IsNetworkTypeWifi(
        /* [in] */ int networkType,
        /* [out] */ Boolean* result);

    /**
     * Removes the NET_CAPABILITY_NOT_RESTRICTED capability from the given
     * NetworkCapabilities object if all the capabilities it provides are
     * typically provided by restricted networks.
     *
     * TODO: consider:
     * - Moving to NetworkCapabilities
     * - Renaming it to guessRestrictedCapability and make it set the
     *   restricted capability bit in addition to clearing it.
     * @hide
     */
    CARAPI MaybeMarkCapabilitiesRestricted(
        /* [in] */ INetworkCapabilities* nc);

    CARAPI  From(
        /* [in] */ IContext* ctx,
        /* [out] */ IConnectivityManager** result);

    /** {@hide */
    CARAPI EnforceTetherChangePermission(
        /* [in] */ IContext* context);

};

} // namespace Net
} // namepsace Droid
} // namespace Elastos

#endif // __CCONNECTIVITYMANAGERHELPER_H__
