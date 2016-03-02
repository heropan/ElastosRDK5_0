
#ifndef __ELASTOS_DROID_TELEPHONY_CPRECISEDATACONNECTIONSTATE_H__
#define __ELASTOS_DROID_TELEPHONY_CPRECISEDATACONNECTIONSTATE_H__

#include "Elastos.Droid.Net.h"
#include "_Elastos_Droid_Telephony_CPreciseDataConnectionState.h"
#include "elastos/droid/ext/frameworkdef.h"
#include <elastos/core/Object.h>

using Elastos::Droid::Net::ILinkProperties;

namespace Elastos {
namespace Droid {
namespace Telephony {

CarClass(CPreciseDataConnectionState)
    , public Object
    , public IPreciseDataConnectionState
    , public IParcelable
{
public:
    CPreciseDataConnectionState();

    virtual ~CPreciseDataConnectionState();

    CAR_INTERFACE_DECL()

    CAR_OBJECT_DECL()

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ Int32 state,
        /* [in] */ Int32 networkType,
        /* [in] */ const String& apnType,
        /* [in] */ const String& apn,
        /* [in] */ const String& reason,
        /* [in] */ ILinkProperties* linkProperties,
        /* [in] */ const String& failCause);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI GetDataConnectionState(
        /* [out] */ Int32* state);

    CARAPI GetDataConnectionNetworkType(
        /* [out] */ Int32* networkType);

    CARAPI GetDataConnectionAPNType(
        /* [out] */ String* apnType);

    CARAPI GetDataConnectionAPN(
        /* [out] */ String* apn);

    CARAPI GetDataConnectionChangeReason(
        /* [out] */ String* reason);

    CARAPI GetDataConnectionLinkProperties(
        /* [out] */ ILinkProperties** linkProperties);

    CARAPI GetDataConnectionFailCause(
        /* [out] */ String* failCause);

    CARAPI GetHashCode(
        /* [out] */ Int32* hashCode);

    CARAPI Equals(
        /* [in] */ IInterface* other,
        /* [out] */ Boolean* res);

    CARAPI ToString(
        /* [out] */ String* str);

private:
    Int32 mState;
    Int32 mNetworkType;
    String mAPNType;
    String mAPN;
    String mReason;
    AutoPtr<ILinkProperties> mLinkProperties;
    String mFailCause;
};

} // namespace Telephony
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_TELEPHONY_CPRECISEDATACONNECTIONSTATE_H__
