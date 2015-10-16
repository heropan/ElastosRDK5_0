
#ifndef __ELASTOS_DROID_TRANSITION_TRANSITIONSET_H__
#define __ELASTOS_DROID_TRANSITION_TRANSITIONSET_H__

#include "elastos/droid/transition/Transition.h"

namespace Elastos {
namespace Droid {
namespace Transition {

class TransitionSet
    : public Transition
    , public ITransitionSet
{
public:
    class TransitionListenerAdapter_1
        : public TransitionListenerAdapter
    {
    public:
        TransitionListenerAdapter_1(
            /* [in] */ ITransition* nxtT);

        CARAPI OnTransitionEnd(
            /* [in] */ ITransition* transition);

    public:
        AutoPtr<ITransition> mNextTransition;
    };

    /**
     * This listener is used to detect when all child transitions are done, at
     * which point this transition set is also done.
     */
    class TransitionSetListener
        : public TransitionListenerAdapter
    {
    public:
        TransitionSetListener(
            /* [in] */ ITransitionSet* transitionSet);

        CARAPI OnTransitionStart(
            /* [in] */ ITransition* transition);

        CARAPI OnTransitionEnd(
            /* [in] */ ITransition* transition);

    public:
        AutoPtr<ITransitionSet> mTransitionSet;
    };

public:
    CAR_INTERFACE_DECL()

    /**
     * Constructs an empty transition set. Add child transitions to the
     * set by calling {@link #addTransition(Transition)} )}. By default,
     * child transitions will play {@link #ORDERING_TOGETHER together}.
     */
    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI SetOrdering(
        /* [in] */ Int32 ordering,
        /* [out] */ ITransitionSet** result);

    CARAPI GetOrdering(
        /* [out] */ Int32* result);

    CARAPI AddTransition(
        /* [in] */ ITransition* transition,
        /* [out] */ ITransitionSet** result);

    CARAPI GetTransitionCount(
        /* [out] */ Int32* result);

    CARAPI GetTransitionAt(
        /* [in] */ Int32 index,
        /* [out] */ ITransition** result);

    CARAPI SetDuration(
        /* [in] */ Int64 duration,
        /* [out] */ ITransitionSet** result);

    CARAPI SetStartDelay(
        /* [in] */ Int64 startDelay,
        /* [out] */ ITransitionSet** result);

    CARAPI SetInterpolator(
        /* [in] */ ITimeInterpolator* interpolator,
        /* [out] */ ITransitionSet** result);

    CARAPI AddTarget(
        /* [in] */ IView* target,
        /* [out] */ ITransition** result);

    CARAPI AddTarget(
        /* [in] */ Int32 targetId,
        /* [out] */ ITransition** result);

    CARAPI AddTarget(
        /* [in] */ String targetName,
        /* [out] */ ITransition** result);

    // CARAPI AddTarget(
    //     /* [in] */ Class targetType,
    //     /* [out] */ ITransitionSet** result);

    CARAPI AddListener(
        /* [in] */ ITransitionListener* listener,
        /* [out] */ ITransitionSet** result);

    CARAPI RemoveTarget(
        /* [in] */ Int32 targetId,
        /* [out] */ ITransition** result);

    CARAPI RemoveTarget(
        /* [in] */ IView* target,
        /* [out] */ ITransition** result);

    // CARAPI RemoveTarget(
    //     /* [in] */ Class target,
    //     /* [out] */ ITransition** result);

    CARAPI RemoveTarget(
        /* [in] */ String target,
        /* [out] */ ITransition** result);

    CARAPI ExcludeTarget(
        /* [in] */ IView* target,
        /* [in] */ Boolean exclude,
        /* [out] */ ITransition** result);

    CARAPI ExcludeTarget(
        /* [in] */ String targetName,
        /* [in] */ Boolean exclude,
        /* [out] */ ITransition** result);

    CARAPI ExcludeTarget(
        /* [in] */ Int32 targetId,
        /* [in] */ Boolean exclude,
        /* [out] */ ITransition** result);

    // CARAPI ExcludeTarget(
    //     /* [in] */ Class type,
    //      [in]  Boolean exclude,
    //     /* [out] */ ITransition** result);

    CARAPI RemoveListener(
        /* [in] */ ITransitionListener* listener,
        /* [out] */ ITransitionSet** result);

    CARAPI SetPathMotion(
        /* [in] */ IPathMotion* pathMotion);

    /** @hide */
    CARAPI ForceVisibility(
        /* [in] */ Int32 visibility,
        /* [in] */ Boolean isStartValue);

    CARAPI RemoveTransition(
        /* [in] */ ITransition* transition,
        /* [out] */ ITransitionSet** result);

    CARAPI CaptureStartValues(
        /* [in] */ ITransitionValues* transitionValues);

    CARAPI CaptureEndValues(
        /* [in] */ ITransitionValues* transitionValues);

    CARAPI CapturePropagationValues(
        /* [in] */ ITransitionValues* transitionValues);

    /** @hide */
    CARAPI Pause(
        /* [in] */ IView* sceneRoot);

    /** @hide */
    CARAPI Resume(
        /* [in] */ IView* sceneRoot);

    CARAPI SetSceneRoot(
        /* [in] */ IViewGroup* sceneRoot,
        /* [out] */ ITransitionSet** result);

    CARAPI SetCanRemoveViews(
        /* [in] */ Boolean canRemoveViews);

    CARAPI SetPropagation(
        /* [in] */ ITransitionPropagation* propagation);

    CARAPI SetEpicenterCallback(
        /* [in] */ IEpicenterCallback* epicenterCallback);

    CARAPI ToString(
        /* [in] */ String indent,
        /* [out] */ String* res);

    CARAPI Clone(
        /* [out] */ ITransitionSet** result);

    CARAPI CreateAnimators(
        /* [in] */ IViewGroup* sceneRoot,
        /* [in] */ ITransitionValuesMaps* startValues,
        /* [in] */ ITransitionValuesMaps* endValues,
        /* [in] */ IArrayList* startValuesList,
        /* [in] */ IArrayList* endValuesList);

    CARAPI RunAnimators();

    CARAPI Cancel();

private:
    /**
     * Sets up listeners for each of the child transitions. This is used to
     * determine when this transition set is finished (all child transitions
     * must finish first).
     */
    CARAPI_(void) SetupStartEndListeners();

public:
    AutoPtr<IArrayList> mTransitions;
    Int32 mCurrentListeners;
    Boolean mStarted;

private:
    Boolean mPlayTogether;
};

} // namespace Transition
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_TRANSITION_TRANSITIONSET_H__
