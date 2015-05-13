#include "view/CWindowManagerPolicyThread.h"
#include "ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace View {

ECode CWindowManagerPolicyThread::Set(
    /* [in] */ IThread* thread,
    /* [in] */ ILooper* looper)
{
    mThread = thread;
    mLooper = looper;
    return NOERROR;
}

ECode CWindowManagerPolicyThread::GetThread(
    /* [out] */ IThread** thread)
{
    *thread = mThread;
    INTERFACE_ADDREF(mThread);
    return NOERROR;
}
ECode CWindowManagerPolicyThread::GetLooper(
    /* [out] */ ILooper** looper)
{
    *looper = mLooper;
    INTERFACE_ADDREF(mLooper);
    return NOERROR;
}

} // namespace View
} // namespace Droid
} // namespace Elastos
