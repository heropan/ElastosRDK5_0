
#include "elastos/droid/net/http/HttpLog.h"

using Elastos::Droid::Os::ISystemClock;
using Elastos::Droid::Utility::ILog;

namespace Elastos {
namespace Droid {
namespace Net {
namespace Http {

const String HttpLog::LOGTAG("http");
const Boolean HttpLog::DEBUG = false;
const Boolean HttpLog::LOGV = false;

ECode HttpLog::V(
    /* [in] */ const String& logMe)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    AutoPtr<IThread> thread = Thread::GetCurrentThread();
    String name;
    thread->GetName(&name);
    Logger::V(LOGTAG, /*SystemClock::uptimeMillis() +*/ String(" ") + name + String(" ") + logMe);
#endif
}

ECode HttpLog::E(
    /* [in] */ const String& logMe)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    Logger::E(LOGTAG, logMe);
#endif
}

} // namespace Http
} // namespace Net
} // namespace Droid
} // namespace Elastos
