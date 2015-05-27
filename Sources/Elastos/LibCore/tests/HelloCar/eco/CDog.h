
#ifndef __HELLOCAR_CDOG_H__
#define __HELLOCAR_CDOG_H__

#include "_Elastos_HelloCar_CDog.h"
#include "Animal.h"

namespace Elastos {
namespace HelloCar {

CarClass(CDog)
    , public Animal
    , public IDog
{
public:
    CAR_INTERFACE_DECL()

    CAR_OBJECT_DECL()

    CARAPI Bark();
};

} // HelloCar
} // Elastos

#endif //__HELLOCAR_CDOG_H__
