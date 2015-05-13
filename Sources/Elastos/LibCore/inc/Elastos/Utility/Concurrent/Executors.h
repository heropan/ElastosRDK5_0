
#ifndef __EXECUTORS_H__
#define __EXECUTORS_H__

#include "cmdef.h"
#ifdef ELASTOS_CORE
#include "Elastos.Core_server.h"
#else
#include "Elastos.Core.h"
#endif

using Elastos::Core::IRunnable;
using Elastos::Core::Threading::IThread;
using Elastos::Core::Threading::IThreadGroup;
using Elastos::Utility::Concurrent::Atomic::IAtomicInteger32;

namespace Elastos {
namespace Utility {
namespace Concurrent {

class Executors
{
public:
    /**
     * A callable that runs given task and returns given result
     */
    class RunnableAdapter
        : public ElLightRefBase
        , public ICallable
    {
    public:
        RunnableAdapter(
            /* [in] */ IRunnable* task,
            /* [in] */ IInterface* result)
            : mTask(task)
            , mResult(result)
        {}

        CAR_INTERFACE_DECL();

        CARAPI Call(
            /* [out] */ IInterface** result);

    public:
        AutoPtr<IRunnable> mTask;
        AutoPtr<IInterface> mResult;
    };

    /**
     * The default thread factory
     */
    class DefaultThreadFactory
        : public ElLightRefBase
        , public IThreadFactory
    {
    public:
        DefaultThreadFactory();

        CAR_INTERFACE_DECL();

        CARAPI NewThread(
            /* [in] */ IRunnable* r,
            /* [out] */ IThread** thread);

    private:
        static const AutoPtr<IAtomicInteger32> sPoolNumber;
        AutoPtr<IThreadGroup> mGroup;
        const AutoPtr<IAtomicInteger32> mThreadNumber;
        String mNamePrefix;
    };

public:
    /**
     * Returns a default thread factory used to create new threads.
     * This factory creates all new threads used by an Executor in the
     * same {@link ThreadGroup}. Each new
     * thread is created as a non-daemon thread with priority set to
     * the smaller of <tt>Thread.NORM_PRIORITY</tt> and the maximum
     * priority permitted in the thread group.  New threads have names
     * accessible via {@link Thread#getName} of
     * <em>pool-N-thread-M</em>, where <em>N</em> is the sequence
     * number of this factory, and <em>M</em> is the sequence number
     * of the thread created by this factory.
     * @return a thread factory
     */
    static CARAPI_(AutoPtr<IThreadFactory>) GetDefaultThreadFactory();

    /**
     * Returns a {@link Callable} object that, when
     * called, runs the given task and returns the given result.  This
     * can be useful when applying methods requiring a
     * <tt>Callable</tt> to an otherwise resultless action.
     * @param task the task to run
     * @param result the result to return
     * @return a callable object
     * @throws NullPointerException if task null
     */
    static CARAPI Callable(
        /* [in] */ IRunnable* task,
        /* [in] */ IInterface* result,
        /* [out] */ ICallable** object);
};

} // namespace Concurrent
} // namespace Utility
} // namespace Elastos

#endif //__EXECUTORS_H__
