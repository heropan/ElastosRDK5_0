
#ifndef __HANDLERTHREAD_H__
#define __HANDLERTHREAD_H__

#include "ext/frameworkext.h"


namespace Elastos {
namespace Droid {
namespace Os {

/**
 * Handy class for starting a new thread that has a looper. The looper can then be
 * used to create handler classes. Note that start() must still be called.
 */
class HandlerThread : public ThreadBase
{
public:
    HandlerThread(
        /* [in] */ const String& name);

    HandlerThread(
        /* [in] */ const String& name,
        /* [in] */ Int32 priority);

    CARAPI Run();

    CARAPI GetLooper(
        /* [out] */ ILooper** looper);

    CARAPI Quit(
        /* [out] */ Boolean* result);

    CARAPI GetThreadId(
        /* [out] */ Int32* tid);

protected:
    /**
     * Call back method that can be explicitly overridden if needed to execute some
     * setup before Looper loops.
     */
    virtual CARAPI_(void) OnLooperPrepared();

private:
    Int32 mPriority;
    Int32 mTid;
    AutoPtr<ILooper> mLooper;
};

} // namespace Os
} // namespace Droid
} // namespace Elastos

#endif //__HANDLERTHREAD_H__
