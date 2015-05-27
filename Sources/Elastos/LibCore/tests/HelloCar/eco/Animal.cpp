#include "Animal.h"
#include <stdio.h>

namespace Elastos {
namespace HelloCar {

// 实现某个 CAR 接口的类都需要使用 CAR_INTERFACE_DECL/CAR_INTERFACE_IMPL
CAR_INTERFACE_IMPL(Animal, Object, IAnimal)

// 使用成员初始化列表初始化成员变量
Animal::Animal()
    : mAge(1)
    , mName("Animal")
{
}

ECode Animal::constructor()
{
    return NOERROR;
}

ECode Animal::constructor(
    /* [in] */ Int32 age,
    /* [in] */ const String& name)
{
    mAge = age;
    mName = name;
    return NOERROR;
}

ECode Animal::SetName(
    /* [in] */ const String& name)
{
    mName = name;
    return NOERROR;
}

ECode Animal::GetName(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name)
    *name = mName;
    return NOERROR;
}

ECode Animal::SetAge(
    /* [in] */ Int32 age)
{
    mAge = age;
    return NOERROR;
}

ECode Animal::GetAge(
    /* [out] */ Int32* age)
{
    VALIDATE_NOT_NULL(age)
    *age = mAge;
    return NOERROR;
}

ECode Animal::CanFly(
    /* [out] */ Boolean* canFly)
{
    VALIDATE_NOT_NULL(canFly);
    *canFly = FALSE;
    return NOERROR;
}

} // HelloCar
} // Elastos