
#include "CDog.h"

namespace Elastos {
namespace HelloCar {

CAR_INTERFACE_IMPL(CDog, Animal, IDog)

CAR_OBJECT_IMPL(CDog)

ECode CDog::Bark()
{
    printf("%s : wang! wang!\n", mName.string());
    return NOERROR;
}

} // HelloCar
} // Elastos