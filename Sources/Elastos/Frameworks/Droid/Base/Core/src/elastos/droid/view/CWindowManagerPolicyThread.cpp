#include "elastos/droid/view/CWindowManagerPolicyThread.h"
#include "elastos/droid/ext/frameworkext.h"

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
    REFCOUNT_ADD(mThread);
    return NOERROR;
}
ECode CWindowManagerPolicyThread::GetLooper(
    /* [out] */ ILooper** looper)
{
    *looper = mLooper;
    REFCOUNT_ADD(mLooper);
    return NOERROR;
}

} // namespace View
} // namespace Droid
} // namespace Elastos
