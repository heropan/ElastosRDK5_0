
#ifndef __ELASTOS_DROID_OS_CHANDLERTHREAD_H__
#define __ELASTOS_DROID_OS_CHANDLERTHREAD_H__

#include "_Elastos_Droid_Os_CHandlerThread.h"
#include <elastos/core/Thread.h>

using Elastos::Core::IClassLoader;
using Elastos::Core::IRunnable;
using Elastos::Core::Thread;

namespace Elastos {
namespace Droid {
namespace Os {

/**
 * Handy class for starting a new thread that has a looper. The looper can then be
 * used to create handler classes. Note that start() must still be called.
 */
CarClass(CHandlerThread)
    , public Thread
{
public:
    CAR_OBJECT_DECL()

    CHandlerThread();

    CARAPI constructor(
        /* [in] */ const String& name);

    /**
     * Constructs a HandlerThread.
     * @param name
     * @param priority The priority to run the thread at. The value supplied must be from
     * {@link android.os.Process} and not from java.lang.Thread.
     */
    CARAPI constructor(
        /* [in] */ const String& name,
        /* [in] */ Int32 priority);

    CARAPI Run();

    CARAPI GetLooper(
        /* [out] */ ILooper** looper);

    CARAPI Quit(
        /* [out] */ Boolean* result);

    CARAPI QuitSafely(
        /* [out] */ Boolean* result);

    CARAPI GetThreadId(
        /* [out] */ Int32* tid);

protected:
    /**
     * Call back method that can be explicitly overridden if needed to execute some
     * setup before Looper loops.
     */
    CARAPI_(void) OnLooperPrepared();

public:
    Int32 mPriority;
    Int32 mTid;
    AutoPtr<ILooper> mLooper;
};

} // namespace Os
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_OS_CHANDLERTHREAD_H__
