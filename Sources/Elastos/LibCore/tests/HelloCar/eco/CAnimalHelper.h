
#ifndef __HELLOCAR_CANIMALHELPER_H__
#define __HELLOCAR_CANIMALHELPER_H__

#include "_Elastos_HelloCar_CAnimalHelper.h"
#include <elastos/core/Singleton.h>

using Elastos::Core::Singleton;

namespace Elastos {
namespace HelloCar {

CarClass(CAnimalHelper)
    , public Singleton
    , public IAnimalHelper
{
public:
    CAR_INTERFACE_DECL()

    CAR_SINGLETON_DECL()

    CARAPI CanFly(
        /* [in] */ IAnimal* animal,
        /* [out] */ Boolean* canFly);

};

} // HelloCar
} // Elastos

#endif //__HELLOCAR_CANIMALHELPER_H__
