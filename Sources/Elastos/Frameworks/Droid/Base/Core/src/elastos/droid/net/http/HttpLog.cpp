
#include "HttpLog.h"
// #include <SystemClock.h>
#include <elastos/utility/logging/Logger.h>
#include <elastos/core/Thread.h>

using namespace Elastos::Core;
using namespace Elastos::Utility::Logging;


namespace Elastos {
namespace Droid {
namespace Net {
namespace Http {

const String HttpLog::LOGTAG = String("http");
const Boolean HttpLog::DEBUG = FALSE;
const Boolean HttpLog::LOGV = FALSE;

void HttpLog::V(
    /* [in] */ const String& logMe) {
    AutoPtr<IThread> thread = Thread::GetCurrentThread();
    String name;
    thread->GetName(&name);
    Logger::V(LOGTAG, /*SystemClock::uptimeMillis() +*/ String(" ") + name + String(" ") + logMe);
}

void HttpLog::E(
    /* [in] */ const String& logMe) {
    Logger::E(LOGTAG, logMe);
}

}
}
}
}
