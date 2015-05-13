
#ifndef __ELASTOS_DROID_NET_CNETWORKCONFIG_H__
#define __ELASTOS_DROID_NET_CNETWORKCONFIG_H__

#include "_CNetworkConfig.h"
#include <elastos/Mutex.h>

namespace Elastos {
namespace Droid {
namespace Net {

CarClass(CNetworkConfig)
{
public:
    CARAPI constructor(
        /* [in] */ const String& init);

    CARAPI IsDefault(
        /* [out] */ Boolean* result);

    CARAPI GetName(
        /* [out] */ String* name);

    CARAPI SetName(
        /* [in] */ const String& name);

    CARAPI GetType(
        /* [out] */ Int32* type);

    CARAPI SetType(
        /* [in] */ Int32 type);

    CARAPI GetRadio(
        /* [out] */ Int32* radio);

    CARAPI SetRadio(
        /* [in] */ Int32 radio);

    CARAPI GetPriority(
        /* [out] */ Int32* priority);

    CARAPI SetPriority(
        /* [in] */ Int32 priority);

    CARAPI GetDependencyMet(
        /* [out] */ Boolean* dependencyMet);

    CARAPI SetDependencyMet(
        /* [in] */ Boolean dependencyMet);

    CARAPI GetRestoreTime(
        /* [out] */ Int32* restoretime);

    CARAPI SetRestoreTime(
        /* [in] */ Int32 restoretime);

public:
    /**
     * Human readable string
     */
    String mName;

    /**
     * Type from ConnectivityManager
     */
    Int32 mType;

    /**
     * the radio number from radio attributes config
     */
    Int32 mRadio;

    /**
     * higher number == higher priority when turning off connections
     */
    Int32 mPriority;

    /**
     * indicates the boot time dependencyMet setting
     */
    Boolean mDependencyMet;

    /**
     * indicates the default restoral timer in seconds
     * if the network is used as a special network feature
     * -1 indicates no restoration of default
     */
    Int32 mRestoreTime;
};

} // namespace Net
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_NET_CNETWORKCONFIG_H__

