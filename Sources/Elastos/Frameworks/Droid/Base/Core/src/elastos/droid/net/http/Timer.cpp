
#include "Timer.h"
#include "os/SystemClock.h"
#include "HttpLog.h"

using Elastos::Droid::Os::SystemClock;

namespace Elastos {
namespace Droid {
namespace Net {
namespace Http {

Timer::Timer()
    : mStart(0)
    , mLast(0)
{
    mStart = mLast = SystemClock::GetUptimeMillis();
}

Timer::~Timer()
{}

ECode Timer::Mark(
    /* [in] */ const String& message) {
    Int64 now = SystemClock::GetUptimeMillis();
    if (HttpLog::LOGV) {
        HttpLog::V(message + String(" ") + StringUtils::Int64ToString(now - mLast)
            + String(" total ") + StringUtils::Int64ToString(now - mStart));
    }
    mLast = now;

    return NOERROR;
}

}
}
}
}
