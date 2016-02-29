
#ifndef  __ELASTOS_DROID_SYSTEMUI_RECENTS_MISC_REFERENCECOUNTEDTRIGGER_H__
#define  __ELASTOS_DROID_SYSTEMUI_RECENTS_MISC_REFERENCECOUNTEDTRIGGER_H__

#include "_SystemUI.h"
#include "Elastos.Droid.Animation.h"
#include "Elastos.Droid.Content.h"
#include "Elastos.Droid.Os.h"
#include "Elastos.CoreLibrary.Core.h"
#include "Elastos.CoreLibrary.Utility.h"
#include <elastos/droid/animation/AnimatorListenerAdapter.h>
#include <elastos/droid/os/Runnable.h>

using Elastos::Droid::Animation::AnimatorListenerAdapter;
using Elastos::Droid::Animation::IAnimator;
using Elastos::Droid::Animation::IAnimatorListener;
using Elastos::Droid::Content::IContext;
using Elastos::Core::IRunnable;
using Elastos::Droid::Os::Runnable;
using Elastos::Utility::IArrayList;

namespace Elastos {
namespace Droid {
namespace SystemUI {
namespace Recents {
namespace Misc {

/**
 * Used to generate successive incremented names.
 */
class ReferenceCountedTrigger
    : public Object
{
private:
    class IncrementRunnable
        : public Runnable
    {
    public:
        IncrementRunnable(
            /* [in] */ ReferenceCountedTrigger* host);

        // @Override
        CARAPI Run();

    private:
        ReferenceCountedTrigger* mHost;
    };

    class DecrementRunnable
        : public Runnable
    {
    public:
        DecrementRunnable(
            /* [in] */ ReferenceCountedTrigger* host);

        // @Override
        CARAPI Run();

    private:
        ReferenceCountedTrigger* mHost;
    };

    class MyAnimatorListenerAdapter
        : public AnimatorListenerAdapter
    {
    public:
        MyAnimatorListenerAdapter(
            /* [in] */ ReferenceCountedTrigger* host);

        CARAPI OnAnimationEnd(
            /* [in] */ IAnimator* animation);

    private:
        ReferenceCountedTrigger* mHost;
    };

public:
    ReferenceCountedTrigger(
        /* [in] */ IContext* context,
        /* [in] */ IRunnable* firstIncRunnable,
        /* [in] */ IRunnable* lastDecRunnable,
        /* [in] */ IRunnable* errorRunanable);

    /** Increments the ref count */
    CARAPI_(void) Increment();

    /** Convenience method to increment this trigger as a runnable */
    CARAPI_(AutoPtr<IRunnable>) IncrementAsRunnable();

    /** Adds a runnable to the last-decrement runnables list. */
    CARAPI_(void) AddLastDecrementRunnable(
        /* [in] */ IRunnable* r);

    /** Decrements the ref count */
    CARAPI_(void) Decrement();

    /** Convenience method to decrement this trigger as a runnable. */
    CARAPI_(AutoPtr<IRunnable>) DecrementAsRunnable();

    /** Convenience method to decrement this trigger as a animator listener. */
    CARAPI_(AutoPtr<IAnimatorListener>) DecrementOnAnimationEnd();

    /** Returns the current ref count */
    CARAPI_(Int32) GetCount();

public:
    // Convenience runnables
    AutoPtr<IncrementRunnable> mIncrementRunnable;
    AutoPtr<DecrementRunnable> mDecrementRunnable;

    AutoPtr<IContext> mContext;
    Int32 mCount;
    AutoPtr<IArrayList> mFirstIncRunnables;
    AutoPtr<IArrayList> mLastDecRunnables;
    AutoPtr<IRunnable> mErrorRunnable;
};

} // namespace Misc
} // namespace Recents
} // namespace SystemUI
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_SYSTEMUI_RECENTS_MISC_REFERENCECOUNTEDTRIGGER_H__
