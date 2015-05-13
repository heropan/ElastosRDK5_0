
#ifndef __CSUPPLICANTSTATE_H__
#define __CSUPPLICANTSTATE_H__

#include "_CSupplicantState.h"

namespace Elastos {
namespace Droid {
namespace Net {
namespace Wifi {

CarClass(CSupplicantState)
{
public:
    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ SupplicantState state);

    CARAPI Set(
        /* [in] */ SupplicantState state);

    CARAPI Get(
        /* [out] */ SupplicantState* state);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);
private:
    SupplicantState mState;
};

} // namespace Wifi
} // namespace Net
} // namespace Droid
} // namespace Elastos

#endif // __CSUPPLICANTSTATE_H__
