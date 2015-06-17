
#include "MyRunnable.h"
#include <elastos/core/Thread.h>
#include <stdio.h>

using Elastos::Core::EIID_IRunnable;

CAR_INTERFACE_IMPL(MyRunnable, Object, IRunnable)

MyRunnable::MyRunnable()
    : mStop(FALSE)
{
}

ECode MyRunnable::Run()
{

    return NOERROR;
}

