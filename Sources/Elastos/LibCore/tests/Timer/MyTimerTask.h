
#ifndef __MyTimerTask_H__
#define __MyTimerTask_H__

#include <Elastos.Core.h>
#include <Elastos.Core.h>
#include <elrefbase.h>
#include <elastos/Mutex.h>
#include <elastos/TimerTask.h>

using namespace Elastos::Core::Threading;

class MyTimerTask : public TimerTask
{
public:
    CARAPI Run();

private:
    static Int32 sCount;
};

#endif //__MyTimerTask_H__
