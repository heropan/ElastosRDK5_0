
#include "CCat.h"

namespace Elastos {
namespace HelloCar {

CAR_OBJECT_IMPL(CCat)

ECode CCat::CanFly(
    /* [out] */ Boolean* canFly)
{
    printf("Though cat has nine lives, but it can not fly!\n");
    return Animal::CanFly(canFly);
}

} // HelloCar
} // Elastos