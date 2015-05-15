
#ifndef __CFORKJOINTASKHELPER_H__
#define __CFORKJOINTASKHELPER_H__

#include "_CForkJoinTaskHelper.h"

using Elastos::Core::IRunnable;

namespace Elastos {
namespace Utility {
namespace Concurrent {

CarClass(CForkJoinTaskHelper)
{
public:
    CARAPI InvokeAll(
        /* [in] */ IForkJoinTask* t1,
        /* [in] */ IForkJoinTask* t2);

    CARAPI InvokeAllEx(
        /* [in] */ ArrayOf<IForkJoinTask*>* tasks);

    CARAPI InvokeAllEx2(
        /* [in] */ ICollection* tasks,
        /* [out] */ ICollection** outcol);

    CARAPI HelpQuiesce();

    CARAPI GetPool(
        /* [out] */ IForkJoinPool** outpool);

    CARAPI InForkJoinPool(
        /* [out] */ Boolean* value);

    CARAPI GetQueuedTaskCount(
        /* [out] */ Int32* value);

    CARAPI GetSurplusQueuedTaskCount(
        /* [out] */ Int32* value);

    CARAPI Adapt(
        /* [in] */ IRunnable* runnable,
        /* [out] */ IForkJoinTask** outjoin);

    CARAPI AdaptEx(
        /* [in] */ IRunnable* runnable,
        /* [in] */ IInterface* result,
        /* [out] */ IForkJoinTask** outjoin);

    CARAPI AdaptEx2(
        /* [in] */ ICallable* callable,
        /* [out] */ IForkJoinTask** outjoin);
};

} // namespace Concurrent
} // namespace Utility
} // namespace Elastos

#endif //__CFORKJOINTASKHELPER_H__