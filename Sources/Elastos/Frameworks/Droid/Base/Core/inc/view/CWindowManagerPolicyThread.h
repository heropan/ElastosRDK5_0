#ifndef __ELASTOS_DROID_VIEW_CWINDOWMANAGERPOLICYTHREAD_H__
#define __ELASTOS_DROID_VIEW_CWINDOWMANAGERPOLICYTHREAD_H__

#include "_CWindowManagerPolicyThread.h"

using Elastos::Core::IThread;
using Elastos::Droid::Os::ILooper;

namespace Elastos {
namespace Droid {
namespace View {

CarClass(CWindowManagerPolicyThread)
{
public:
    CARAPI Set(
        /* [in] */ IThread* thread,
        /* [in] */ ILooper* looper);

    CARAPI GetThread(
        /* [out] */ IThread** thd);

    CARAPI GetLooper(
        /* [out] */ ILooper** lpr);

private:
    AutoPtr<IThread> mThread;
    AutoPtr<ILooper> mLooper;
};

} // namespace View
} // namespace Droid
} // namespace Elastos

#endif
