
#ifndef __HELLOCAR_CCAT_H__
#define __HELLOCAR_CCAT_H__

#include "_Elastos_HelloCar_CCat.h"
#include "Animal.h"

namespace Elastos {
namespace HelloCar {

CarClass(CCat)
    , public Animal
{
public:
    CAR_OBJECT_DECL()

    CARAPI CanFly(
        /* [out] */ Boolean* canFly);

};

} // HelloCar
} // Elastos

#endif //__HELLOCAR_CCAT_H__
