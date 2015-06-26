
#ifndef __ELASTOS_NET_MyRunnable_H__
#define __ELASTOS_NET_MyRunnable_H__

#include "Elastos.CoreLibrary_server.h"
#include <elastos.h>
#include <eltypes.h>
#include <stdio.h>
#include <elastos/core/Object.h>

using Elastos::Core::IRunnable;

class MyRunnable
    : public Object
    , public IRunnable
{
public:
    MyRunnable();

    CAR_INTERFACE_DECL()

    CARAPI Run();

public:
    Boolean mStop;
};

#endif //__ELASTOS_NET_MyRunnable_H__
