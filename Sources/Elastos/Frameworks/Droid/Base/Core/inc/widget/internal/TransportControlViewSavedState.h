
#ifndef __TRANSPORTCONTROLVIEWSAVEDSTATE_H__
#define __TRANSPORTCONTROLVIEWSAVEDSTATE_H__

#include "view/ViewBaseSavedState.h"

using Elastos::Droid::View::ViewBaseSavedState;

namespace Elastos {
namespace Droid {
namespace Widget {
namespace Internal {

class TransportControlViewSavedState : public ViewBaseSavedState
{
public:

    TransportControlViewSavedState();

    TransportControlViewSavedState(
        /* [in] */ IParcelable* superState);

    virtual CARAPI WriteToParcel(
        /* [in] */ IParcel* out);

    virtual CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

protected:
    virtual CARAPI Init(
        /* [in] */ IParcelable* superState);

    virtual CARAPI Init();

    Boolean wasShowing;
};

}// namespace Internal
}// namespace Widget
}// namespace Droid
}// namespace Elastos
#endif
