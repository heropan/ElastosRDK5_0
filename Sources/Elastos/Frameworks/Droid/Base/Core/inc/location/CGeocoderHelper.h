#ifndef __ELASTOS_DROID_LOCATION_CGEOCODERHELPER_H__
#define __ELASTOS_DROID_LOCATION_CGEOCODERHELPER_H__

#include "_CGeocoderHelper.h"

namespace Elastos {
namespace Droid {
namespace Location {

CarClass(CGeocoderHelper)
{
public:
    CARAPI constructor();

    CARAPI IsPresent(
        /* [out] */ Boolean* isPresent);
};

} // namespace Elastos
} // namespace Droid
} // namespace Location

#endif
