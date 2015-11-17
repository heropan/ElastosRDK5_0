#ifndef __ELASTOS_DROID_INTERNAL_VIEW_CWINDOWMANAGERPOLICYTHREAD_H__
#define __ELASTOS_DROID_INTERNAL_VIEW_CWINDOWMANAGERPOLICYTHREAD_H__

#include "_Elastos_Droid_Internal_View_CWindowManagerPolicyThread.h"

using Elastos::Core::IThread;
using Elastos::Droid::Os::ILooper;

namespace Elastos {
namespace Droid {
namespace Internal {
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
} // namespace Internal
} // namespace Droid
} // namespace Elastos

#endif
