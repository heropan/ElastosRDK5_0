
#ifndef __CTIMER_H__
#define __CTIMER_H__

#include "_CTimer.h"
#include "Timer.h"

namespace Elastos {
namespace Utility {

CarClass(CTimer) , public Timer
{
public:
    CARAPI constructor(
        /* [in] */ const String& name,
        /* [in] */ Boolean isDaemon);

    CARAPI constructor(
        /* [in] */ const String& name);

    CARAPI constructor(
        /* [in] */ Boolean isDaemon);

    CARAPI constructor();

    CARAPI Cancel();

    CARAPI Purge(
        /* [out] */ Int32* value);

    CARAPI Schedule(
        /* [in] */ ITimerTask* task,
        /* [in] */ IDate* when);

    CARAPI Schedule(
        /* [in] */ ITimerTask* task,
        /* [in] */ Int64 delay);

    CARAPI Schedule(
        /* [in] */ ITimerTask* task,
        /* [in] */ Int64 delay,
        /* [in] */ Int64 period);

    CARAPI Schedule(
        /* [in] */ ITimerTask* task,
        /* [in] */ IDate* when,
        /* [in] */ Int64 period);

    CARAPI ScheduleAtFixedRate(
        /* [in] */ ITimerTask* task,
        /* [in] */ Int64 delay,
        /* [in] */ Int64 period);

    CARAPI scheduleAtFixedRate(
        /* [in] */ ITimerTask* task,
        /* [in] */ IDate* when,
        /* [in] */ Int64 period);
};

} // namespace Utility
} // namespace Elastos

#endif // __CTIMER_H__