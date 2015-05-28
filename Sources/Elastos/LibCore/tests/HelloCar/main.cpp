/*
 * Usages:
 * compile:
 *      cd HelloCar
 *      cd eco
 *      emake
 *      cd ..
 *      emake
 *      pd @
 *      elcopy Elastos.HelloCar.eco /system/lib
 *      elcopy helloCar
 *
 * run:
 *      adb shell
 *      cd /data/data/com.elastos.runtime/elastos
 *      ./helloCar
 *
 * output:
 *      Though cat has nine lives, but it can not fly!
 *      Kitty can not fly!
 *
 *      HuaHua : wang! wang!
 *      HuaHua can not fly!
 *
 *      Though cat has nine lives, but it can not fly!
 *      CAnimalHelper::CanFly : Kitty can not fly!
 */

#include <elautoptr.h>
#include <Elastos.HelloCar.h>
#include <elastos/coredef.h>
 #include <elastos/core/Math.h>
#include <elastos/utility/etl/List.h>

using namespace Elastos;

using Elastos::Core::IThread;
using Elastos::Core::Math;
using Elastos::Utility::Etl::List;

using Elastos::HelloCar::IAnimal;
using Elastos::HelloCar::IDog;
using Elastos::HelloCar::CDog;
using Elastos::HelloCar::CCat;
using Elastos::HelloCar::IAnimalHelper;
using Elastos::HelloCar::CAnimalHelper;
using Elastos::HelloCar::CMyThread;

void testEtl()
{
    List<String> animals;
    animals.PushBack(String("cat"));
    animals.PushBack(String("dog"));

    Double value = Elastos::Core::Math::Cos(90);
    printf("Math::Cos(90) %.2f\n", value);
}

int main(int argc, char *argv[])
{
    Boolean canFly;
    String name;

    AutoPtr<IAnimal> cat;
    CCat::New((IAnimal**)&cat);
    cat->SetName(String("Kitty"));
    cat->GetName(&name);
    cat->CanFly(&canFly);
    printf("%s %s!\n\n", name.string(), canFly ? "can fly" : "can not fly");

    AutoPtr<IDog> dog;
    CDog::New(2, String("HuaHua"), (IDog**)&dog);
    dog->Bark();

    IAnimal* da = IAnimal::Probe(dog);
    da->GetName(&name);
    da->CanFly(&canFly);
    printf("%s %s!\n\n", name.string(), canFly ? "can fly" : "can not fly");

    AutoPtr<IAnimalHelper> helper;
    CAnimalHelper::AcquireSingleton((IAnimalHelper**)&helper);
    helper->CanFly(cat, &canFly);
    cat->GetName(&name);
    printf("CAnimalHelper::CanFly : %s %s!\n\n", name.string(), canFly ? "can fly" : "can not fly");

    AutoPtr<IThread> thread;
    CMyThread::New((IThread**)&thread);
    assert(thread.Get() != NULL && "Thread is null!");
    thread->Start();

    return 0;
}
