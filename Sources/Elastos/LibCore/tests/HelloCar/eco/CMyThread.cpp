
#include "CMyThread.h"

namespace Elastos {
namespace HelloCar {

CAR_OBJECT_IMPL(CMyThread)

ECode CMyThread::constructor()
{
    return Thread::constructor();
}

ECode CMyThread::Run()
{
    printf("CMyThread::Run\n");
    return Thread::Run();
}

} // HelloCar
} // Elastos