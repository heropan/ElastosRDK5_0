
#ifndef __ELASTOS_DROID_NET_CNETWORKINFO_H__
#define __ELASTOS_DROID_NET_CNETWORKINFO_H__

#include "_Elastos_Droid_Net_CNetworkInfo.h"
#include "elastos/droid/net/NetworkInfo.h"

namespace Elastos {
namespace Droid {
namespace Net {

/**
 * Describes the status of a network interface.
 * <p>Use {@link ConnectivityManager#getActiveNetworkInfo()} to get an instance that represents
 * the current network connection.
 */
CarClass(CNetworkInfo)
    , public NetworkInfo
{
public:
    CAR_OBJECT_DECL()

};

} // namespace Net
} // namespace Droid
} // namespace Elastos
#endif // __ELASTOS_DROID_NET_CNETWORKINFO_H__

#if 0 // old CNetworkInfo.h
#ifndef __ELASTOS_DROID_NET_CNETWORKINFO_H__
#define __ELASTOS_DROID_NET_CNETWORKINFO_H__

#include "ext/frameworkdef.h"
#include "_Elastos_Droid_Net_CNetworkInfo.h"
#include <elastos/utility/etl/HashMap.h>

using Elastos::Utility::Etl::HashMap;
namespace Elastos {
namespace Droid {
namespace Net {

CarClass(CNetworkInfo)
{
public:
    CNetworkInfo();

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ Int32 type);

    CARAPI constructor(
        /* [in] */ Int32 type,
        /* [in] */ Int32 subtype,
        /* [in] */ const String& typeName,
        /* [in] */ const String& subtypeName);

    CARAPI constructor(
        /* [in] */ INetworkInfo* source);

    // BEGIN privacy-added
    // SM: I've no idea where this is used
    /**{@hide}* Used for set state to get better working Privacy*/
    CARAPI SetState(
        /* [in] */ NetworkInfoState state);
    // END privacy-added

    CARAPI GetType(
        /* [out] */ Int32* networktype);

    CARAPI GetSubtype(
        /* [out] */ Int32* subtype);

    CARAPI GetSubtypeName(
        /* [out] */ String* subtypename);

    CARAPI SetSubtype(
        /* [in] */ Int32 subtype,
        /* [in] */ const String& subtypeName);

    CARAPI GetTypeName(
        /* [out] */ String* name);

    CARAPI IsConnectedOrConnecting(
        /* [out] */ Boolean* result);

    CARAPI IsConnected(
        /* [out] */ Boolean* result);

    CARAPI IsAvailable(
        /* [out] */ Boolean* result);

    CARAPI SetIsAvailable(
        /* [in] */ Boolean isAvailable);

    CARAPI IsFailover(
        /* [out] */ Boolean* result);

    CARAPI SetFailover(
        /* [in] */ Boolean isFailover);

    CARAPI IsRoaming(
        /* [out] */ Boolean* result);

    CARAPI SetRoaming(
        /* [in] */ Boolean isRoaming);

    CARAPI GetState(
        /* [out] */ NetworkInfoState* state);

    CARAPI GetDetailedState(
        /* [out] */ NetworkInfoDetailedState* detailedState);

    CARAPI GetReason(
        /* [out] */ String* result);

    CARAPI GetExtraInfo(
        /* [out] */ String* result);

    CARAPI SetExtraInfo(
        /* [in] */ const String& extraInfo);

    CARAPI SetDetailedState(
        /* [in] */ NetworkInfoDetailedState detailedState,
        /* [in] */ const String& reason,
        /* [in] */ const String& extraInfo);

    CARAPI ToString(
        /* [out] */ String* result);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

private:
    static Int32 InternalInit();

private:
    static const Boolean DBG;

    Int32 mNetworkType;
    Int32 mSubtype;
    String mTypeName;
    String mSubtypeName;
    NetworkInfoState mState;
    NetworkInfoDetailedState mDetailedState;
    String mReason;
    String mExtraInfo;
    Boolean mIsFailover;
    Boolean mIsRoaming;
    /**
     * Indicates whether network connectivity is possible:
     */
    Boolean mIsAvailable;

    static Int32 mInitFlag;
    static HashMap<NetworkInfoDetailedState, NetworkInfoState> mStateMap;
};

} // namespace Net
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_NET_CNETWORKINFO_H__
#endif