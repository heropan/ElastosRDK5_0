#include "elastos/droid/location/CGeocoderHelper.h"
#include "elastos/droid/location/CGeocoder.h"

namespace Elastos {
namespace Droid {
namespace Location {

ECode CGeocoderHelper::constructor()
{
    return NOERROR;
}

ECode CGeocoderHelper::IsPresent(
    /* [out] */ Boolean* isPresent)
{
    return CGeocoder::IsPresent(isPresent);
}

}// namespace Elastos
}// namespace Droid
}// namespace Location