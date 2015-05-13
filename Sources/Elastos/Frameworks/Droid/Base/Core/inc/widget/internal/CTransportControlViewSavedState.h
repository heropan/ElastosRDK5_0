#ifndef __CTRASNSPORTCONTROLVIEWSAVEDSTATE_H__
#define __CTRASNSPORTCONTROLVIEWSAVEDSTATE_H__

#include "_CTransportControlViewSavedState.h"
#include "TransportControlViewSavedState.h"

namespace Elastos {
namespace Droid {
namespace Widget {
namespace Internal {

class TransportControlView;

CarClass(CTransportControlViewSavedState) , public TransportControlViewSavedState
{
public:
    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI GetSuperState(
        /* [out] */ IParcelable** superState);

    CARAPI constructor(
        /* [in] */ IParcelable* superState);

    CARAPI constructor();

    friend class TransportControlView;
};

}// namespace Internal
}// namespace Widget
}// namespace Droid
}// namespace Elastos
#endif
