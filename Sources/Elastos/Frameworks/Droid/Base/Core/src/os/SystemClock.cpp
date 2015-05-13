
#include "os/SystemClock.h"
#include <elastos/Thread.h>
#include <utils/SystemClock.h>

using Elastos::Core::Threading::Thread;

namespace Elastos {
namespace Droid {
namespace Os {

void SystemClock::Sleep(
    /* [in] */ Int64 ms)
{
    Int64 start = GetUptimeMillis();
    Int64 duration = ms;
    Boolean interrupted = FALSE;
    do {
        // try {
        ECode ec = Thread::Sleep(duration);
        if (ec == E_INTERRUPTED_EXCEPTION) {
            interrupted = TRUE;
        }
        // }
        // catch (InterruptedException e) {
        //     interrupted = true;
        // }
        duration = start + ms - GetUptimeMillis();
    } while (duration > 0);

    if (interrupted) {
        // Important: we don't want to quietly eat an interrupt() event,
        // so we make sure to re-interrupt the thread so that the next
        // call to Thread.sleep() or Object.wait() will be interrupted.
        Thread::GetCurrentThread()->Interrupt();
    }
}

/**
 * Sets the current wall time, in milliseconds.  Requires the calling
 * process to have appropriate permissions.
 *
 * @return if the clock was successfully set to the specified time.
 */
Boolean SystemClock::SetCurrentTimeMillis(Int64 millis)
{
    return (android::setCurrentTimeMillis(millis) == 0);
}

/**
 * Returns milliseconds since boot, not counting time spent in deep sleep.
 * <b>Note:</b> This value may get reset occasionally (before it would
 * otherwise wrap around).
 *
 * @return milliseconds of non-sleep uptime since boot.
 */
Int64 SystemClock::GetUptimeMillis()
{
    return (Int64)android::uptimeMillis();
}

/**
 * Returns milliseconds since boot, including time spent in sleep.
 *
 * @return elapsed milliseconds since boot.
 */
Int64 SystemClock::GetElapsedRealtime()
{
    return (Int64)android::elapsedRealtime();
}

/**
 * Returns nanoseconds since boot, including time spent in sleep.
 *
 * @return elapsed nanoseconds since boot.
 */
Int64 SystemClock::GetElapsedRealtimeNanos()
{
    return (Int64)android::elapsedRealtimeNano();
}

} // namespace Os
} // namespace Droid
} // namespace Elastos
