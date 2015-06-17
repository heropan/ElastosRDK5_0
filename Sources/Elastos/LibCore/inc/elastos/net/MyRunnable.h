
#ifndef __ELASTOS_NET_MyRunnable_H__
#define __ELASTOS_NET_MyRunnable_H__

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

private:
    //CARAPI_(Mutex*) GetSelfLock();

private:
    //Mutex mLock;
};

#endif //__ELASTOS_NET_MyRunnable_H__
