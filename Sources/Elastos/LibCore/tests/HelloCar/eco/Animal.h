
#ifndef __HELLOCAR_ANIMAL_H__
#define __HELLOCAR_ANIMAL_H__

#include <Elastos.HelloCar_server.h>
#include <elastos/core/Object.h>

using Elastos::Core::Object;

namespace Elastos {
namespace HelloCar {

class Animal
    : public Object
    , public IAnimal
{
public:
    CAR_INTERFACE_DECL()

    Animal();

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ Int32 age,
        /* [in] */ const String& name);

    CARAPI SetName(
        /* [in] */ const String& name);

    CARAPI GetName(
        /* [out] */ String* name);

    CARAPI SetAge(
        /* [in] */ Int32 age);

    CARAPI GetAge(
        /* [out] */ Int32* age);

    CARAPI CanFly(
        /* [out] */ Boolean* canFly);

protected:
    Int32 mAge;
    String mName;
};

} // HelloCar
} // Elastos

#endif //__HELLOCAR_ANIMAL_H__