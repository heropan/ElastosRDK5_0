
#include "elastos/droid/transition/TransitionSet.h"
#include "elastos/droid/transition/CTransitionValues.h"

using Elastos::Droid::Content::Res::ITypedArray;

using Elastos::Utility::CArrayList;
using Elastos::Utility::IListIterator;
using Elastos::Utility::IIterator;

namespace Elastos {
namespace Droid {
namespace Transition {

//===============================================================
// TransitionSet::
//===============================================================
CAR_INTERFACE_IMPL(TransitionSet, Transition, ITransitionSet)

ECode TransitionSet::constructor()
{
    return NOERROR;
}

ECode TransitionSet::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    Transition::constructor(context, attrs);
    assert(0 && "TODO");
    AutoPtr<ITypedArray> a;// = context->ObtainStyledAttributes(attrs, R.styleable.TransitionSet);
    Int32 ordering = 0;
    // a->GetInt32(R.styleable.TransitionSet_transitionOrdering,
    //         TransitionSet::ORDERING_TOGETHER, &ordering);
    AutoPtr<ITransitionSet> res;
    SetOrdering(ordering, (ITransitionSet**)&res);
    a->Recycle();
    return NOERROR;
}

ECode TransitionSet::SetOrdering(
    /* [in] */ Int32 ordering,
    /* [out] */ ITransitionSet** result)
{
    VALIDATE_NOT_NULL(result)

    switch (ordering) {
        case ORDERING_SEQUENTIAL:
            mPlayTogether = FALSE;
            break;
        case ORDERING_TOGETHER:
            mPlayTogether = TRUE;
            break;
        default:
            return E_RUNTIME_EXCEPTION;
            // throw new AndroidRuntimeException("Invalid parameter for TransitionSet " +
            //         "ordering: " + ordering);
    }
    *result = ITransitionSet::Probe(this);
    REFCOUNT_ADD(*result)
    return NOERROR;
}

ECode TransitionSet::GetOrdering(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result)

    *result = mPlayTogether ? ORDERING_TOGETHER : ORDERING_SEQUENTIAL;
    return NOERROR;
}

ECode TransitionSet::AddTransition(
    /* [in] */ ITransition* transition,
    /* [out] */ ITransitionSet** result)
{
    VALIDATE_NOT_NULL(result)

    if (transition != NULL) {
        mTransitions->Add(transition);
        AutoPtr<Transition> ct = (Transition*)transition;
        ct->mParent = this;
        if (mDuration >= 0) {
            AutoPtr<ITransition> res;
            transition->SetDuration(mDuration, (ITransition**)&res);
        }
    }
    *result = ITransitionSet::Probe(this);
    REFCOUNT_ADD(*result)
    return NOERROR;
}

ECode TransitionSet::GetTransitionCount(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result)

    return mTransitions->GetSize(result);
}

ECode TransitionSet::GetTransitionAt(
    /* [in] */ Int32 index,
    /* [out] */ ITransition** result)
{
    VALIDATE_NOT_NULL(result)

    Int32 numTransitions = 0;
    mTransitions->GetSize(&numTransitions);
    if (index < 0 || index >= numTransitions) {
        *result = NULL;
        return NOERROR;
    }
    AutoPtr<IInterface> t;
    mTransitions->Get(index, (IInterface**)&t);
    *result = ITransition::Probe(t);
    REFCOUNT_ADD(*result)
    return NOERROR;
}

ECode TransitionSet::SetDuration(
    /* [in] */ Int64 duration,
    /* [out] */ ITransitionSet** result)
{
    VALIDATE_NOT_NULL(result)

    AutoPtr<ITransition> res;
    Transition::SetDuration(duration, (ITransition**)&res);
    if (mDuration >= 0 && mTransitions != NULL) {
        Int32 numTransitions = 0;
        mTransitions->GetSize(&numTransitions);
        for (Int32 i = 0; i < numTransitions; ++i) {
            AutoPtr<IInterface> t;
            mTransitions->Get(i, (IInterface**)&t);
            AutoPtr<ITransition> p = ITransition::Probe(t);
            p->SetDuration(duration, (ITransition**)&res);
        }
    }
    *result = ITransitionSet::Probe(this);
    REFCOUNT_ADD(*result)
    return NOERROR;
}

ECode TransitionSet::SetStartDelay(
    /* [in] */ Int64 startDelay,
    /* [out] */ ITransitionSet** result)
{
    VALIDATE_NOT_NULL(result)

    AutoPtr<ITransition> p;
    Transition::SetStartDelay(startDelay, (ITransition**)&p);
    *result = ITransitionSet::Probe(p);
    REFCOUNT_ADD(*result)
    return NOERROR;
}

ECode TransitionSet::SetInterpolator(
    /* [in] */ ITimeInterpolator* interpolator,
    /* [out] */ ITransitionSet** result)
{
    VALIDATE_NOT_NULL(result)

    AutoPtr<ITransition> p;
    Transition::SetInterpolator(interpolator, (ITransition**)&p);
    *result = ITransitionSet::Probe(p);
    REFCOUNT_ADD(*result)
    return NOERROR;
}

ECode TransitionSet::AddTarget(
    /* [in] */ IView* target,
    /* [out] */ ITransition** result)
{
    VALIDATE_NOT_NULL(result)

    Int32 numTransitions = 0;
    mTransitions->GetSize(&numTransitions);
    for (Int32 i = 0; i < numTransitions; i++) {
        AutoPtr<IInterface> t;
        mTransitions->Get(i, (IInterface**)&t);
        AutoPtr<ITransition> p = ITransition::Probe(t);
        AutoPtr<ITransition> res;
        p->AddTarget(target, (ITransition**)&res);
    }
    AutoPtr<ITransition> p;
    Transition::AddTarget(target, (ITransition**)&p);
    *result = p;
    REFCOUNT_ADD(*result)
    return NOERROR;
}

ECode TransitionSet::AddTarget(
    /* [in] */ Int32 targetId,
    /* [out] */ ITransition** result)
{
    VALIDATE_NOT_NULL(result)

    Int32 numTransitions = 0;
    mTransitions->GetSize(&numTransitions);
    for (Int32 i = 0; i < numTransitions; i++) {
        AutoPtr<IInterface> t;
        mTransitions->Get(i, (IInterface**)&t);
        AutoPtr<ITransition> p = ITransition::Probe(t);
        AutoPtr<ITransition> res;
        p->AddTarget(targetId, (ITransition**)&res);
    }
    AutoPtr<ITransition> p;
    Transition::AddTarget(targetId, (ITransition**)&p);
    *result = p;
    REFCOUNT_ADD(*result)
    return NOERROR;
}

ECode TransitionSet::AddTarget(
    /* [in] */ String targetName,
    /* [out] */ ITransition** result)
{
    VALIDATE_NOT_NULL(result)

    Int32 numTransitions = 0;
    mTransitions->GetSize(&numTransitions);
    for (Int32 i = 0; i < numTransitions; i++) {
        AutoPtr<IInterface> t;
        mTransitions->Get(i, (IInterface**)&t);
        AutoPtr<ITransition> p = ITransition::Probe(t);
        AutoPtr<ITransition> res;
        p->AddTarget(targetName, (ITransition**)&res);
    }
    AutoPtr<ITransition> p;
    Transition::AddTarget(targetName, (ITransition**)&p);
    *result = p;
    REFCOUNT_ADD(*result)
    return NOERROR;
}

// ECode TransitionSet::AddTarget(
//     /* [in] */ Class targetType,
//     /* [out] */ ITransitionSet** result)
// {
//     VALIDATE_NOT_NULL(result)

//     Int32 numTransitions = 0;
//     mTransitions->GetSize(&numTransitions);
//     for (Int32 i = 0; i < numTransitions; i++) {
//         AutoPtr<IInterface> t;
//         mTransitions->Get(i, (IInterface**)&t);
//         AutoPtr<ITransition> p = ITransition::Probe(t);
//         p->AddTarget(targetType);
//     }
//     AutoPtr<ITransition> p;
//     Transition::AddTarget(targetType, (ITransition**)&p);
//     *result = ITransitionSet::Probe(p);
//     REFCOUNT_ADD(*result)
//     return NOERROR;
// }

ECode TransitionSet::AddListener(
    /* [in] */ ITransitionListener* listener,
    /* [out] */ ITransitionSet** result)
{
    VALIDATE_NOT_NULL(result)

    AutoPtr<ITransition> p;
    Transition::AddListener(listener, (ITransition**)&p);
    *result = ITransitionSet::Probe(p);
    REFCOUNT_ADD(*result)
    return NOERROR;
}

ECode TransitionSet::RemoveTarget(
    /* [in] */ Int32 targetId,
    /* [out] */ ITransition** result)
{
    VALIDATE_NOT_NULL(result)

    Int32 numTransitions = 0;
    mTransitions->GetSize(&numTransitions);
    for (Int32 i = 0; i < numTransitions; i++) {
        AutoPtr<IInterface> t;
        mTransitions->Get(i, (IInterface**)&t);
        AutoPtr<ITransition> p = ITransition::Probe(t);
        AutoPtr<ITransition> res;
        p->RemoveTarget(targetId, (ITransition**)&res);
    }
    AutoPtr<ITransition> p;
    Transition::RemoveTarget(targetId, (ITransition**)&p);
    *result = p;
    REFCOUNT_ADD(*result)
    return NOERROR;
}

ECode TransitionSet::RemoveTarget(
    /* [in] */ IView* target,
    /* [out] */ ITransition** result)
{
    VALIDATE_NOT_NULL(result)

    Int32 numTransitions = 0;
    mTransitions->GetSize(&numTransitions);
    for (Int32 i = 0; i < numTransitions; i++) {
        AutoPtr<IInterface> t;
        mTransitions->Get(i, (IInterface**)&t);
        AutoPtr<ITransition> p = ITransition::Probe(t);
        AutoPtr<ITransition> res;
        p->RemoveTarget(target, (ITransition**)&res);
    }
    AutoPtr<ITransition> p;
    Transition::RemoveTarget(target, (ITransition**)&p);
    *result = p;
    REFCOUNT_ADD(*result)
    return NOERROR;
}

// ECode TransitionSet::RemoveTarget(
//     /* [in] */ Class target,
//     /* [out] */ ITransitionSet** result)
// {
//     VALIDATE_NOT_NULL(result)

//     Int32 numTransitions = 0;
//     mTransitions->GetSize(&numTransitions);
//     for (Int32 i = 0; i < numTransitions; i++) {
//         AutoPtr<IInterface> t;
//         mTransitions->Get(i, (IInterface**)&t);
//         AutoPtr<ITransition> p = ITransition::Probe(t);
//         p->RemoveTarget(target);
//     }
//     AutoPtr<ITransition> p;
//     Transition::RemoveTarget(target, (ITransition**)&p);
//     *result = ITransitionSet::Probe(p);
//     REFCOUNT_ADD(*result)
//     return NOERROR;
// }

ECode TransitionSet::RemoveTarget(
    /* [in] */ String target,
    /* [out] */ ITransition** result)
{
    VALIDATE_NOT_NULL(result)

    Int32 numTransitions = 0;
    mTransitions->GetSize(&numTransitions);
    for (Int32 i = 0; i < numTransitions; i++) {
        AutoPtr<IInterface> t;
        mTransitions->Get(i, (IInterface**)&t);
        AutoPtr<ITransition> p = ITransition::Probe(t);
        AutoPtr<ITransition> res;
        p->RemoveTarget(target, (ITransition**)&res);
    }
    AutoPtr<ITransition> p;
    Transition::RemoveTarget(target, (ITransition**)&p);
    *result = p;
    REFCOUNT_ADD(*result)
    return NOERROR;
}

ECode TransitionSet::ExcludeTarget(
    /* [in] */ IView* target,
    /* [in] */ Boolean exclude,
    /* [out] */ ITransition** result)
{
    VALIDATE_NOT_NULL(result)

    Int32 numTransitions = 0;
    mTransitions->GetSize(&numTransitions);
    for (Int32 i = 0; i < numTransitions; i++) {
        AutoPtr<IInterface> t;
        mTransitions->Get(i, (IInterface**)&t);
        AutoPtr<ITransition> p = ITransition::Probe(t);
        AutoPtr<ITransition> res;
        p->ExcludeTarget(target, exclude, (ITransition**)&res);
    }
    return Transition::ExcludeTarget(target, exclude, result);
}

ECode TransitionSet::ExcludeTarget(
    /* [in] */ String targetName,
    /* [in] */ Boolean exclude,
    /* [out] */ ITransition** result)
{
    VALIDATE_NOT_NULL(result)

    Int32 numTransitions = 0;
    mTransitions->GetSize(&numTransitions);
    for (Int32 i = 0; i < numTransitions; i++) {
        AutoPtr<IInterface> t;
        mTransitions->Get(i, (IInterface**)&t);
        AutoPtr<ITransition> p = ITransition::Probe(t);
        AutoPtr<ITransition> res;
        p->ExcludeTarget(targetName, exclude, (ITransition**)&res);
    }
    return Transition::ExcludeTarget(targetName, exclude, result);
}

ECode TransitionSet::ExcludeTarget(
    /* [in] */ Int32 targetId,
    /* [in] */ Boolean exclude,
    /* [out] */ ITransition** result)
{
    VALIDATE_NOT_NULL(result)

    Int32 numTransitions = 0;
    mTransitions->GetSize(&numTransitions);
    for (Int32 i = 0; i < numTransitions; i++) {
        AutoPtr<IInterface> t;
        mTransitions->Get(i, (IInterface**)&t);
        AutoPtr<ITransition> p = ITransition::Probe(t);
        AutoPtr<ITransition> res;
        p->ExcludeTarget(targetId, exclude, (ITransition**)&res);
    }
    return Transition::ExcludeTarget(targetId, exclude, result);
}

// ECode TransitionSet::ExcludeTarget(
//     /* [in] */ Class type,
//     /* [in] */ Boolean exclude,
//     /* [out] */ ITransition** result)
// {
//     VALIDATE_NOT_NULL(result)

//     Int32 numTransitions = 0;
//     mTransitions->GetSize(&numTransitions);
//     for (Int32 i = 0; i < numTransitions; i++) {
//         AutoPtr<IInterface> t;
//         mTransitions->Get(i, (IInterface**)&t);
//         AutoPtr<ITransition> p = ITransition::Probe(t);
//         p->ExcludeTarget(type, exclude);
//     }
//     return Transition::ExcludeTarget(type, exclude, result);
// }

ECode TransitionSet::RemoveListener(
    /* [in] */ ITransitionListener* listener,
    /* [out] */ ITransitionSet** result)
{
    VALIDATE_NOT_NULL(result)

    AutoPtr<ITransition> p;
    Transition::RemoveListener(listener, (ITransition**)&p);
    *result = ITransitionSet::Probe(p);
    REFCOUNT_ADD(*result)
    return NOERROR;
}

ECode TransitionSet::SetPathMotion(
    /* [in] */ IPathMotion* pathMotion)
{
    Transition::SetPathMotion(pathMotion);
    Int32 numTransitions = 0;
    mTransitions->GetSize(&numTransitions);
    for (Int32 i = 0; i < numTransitions; i++) {
        AutoPtr<IInterface> t;
        mTransitions->Get(i, (IInterface**)&t);
        AutoPtr<ITransition> p = ITransition::Probe(t);
        p->SetPathMotion(pathMotion);
    }
    return NOERROR;
}

ECode TransitionSet::ForceVisibility(
    /* [in] */ Int32 visibility,
    /* [in] */ Boolean isStartValue)
{
    Int32 numTransitions = 0;
    mTransitions->GetSize(&numTransitions);
    for (Int32 i = 0; i < numTransitions; i++) {
        AutoPtr<IInterface> t;
        mTransitions->Get(i, (IInterface**)&t);
        AutoPtr<ITransition> p = ITransition::Probe(t);
        p->ForceVisibility(visibility, isStartValue);
    }
    return NOERROR;
}

ECode TransitionSet::RemoveTransition(
    /* [in] */ ITransition* transition,
    /* [out] */ ITransitionSet** result)
{
    VALIDATE_NOT_NULL(result)

    mTransitions->Remove(transition);
    AutoPtr<Transition> ct = (Transition*)transition;
    ct->mParent = NULL;
    *result = this;
    REFCOUNT_ADD(*result)
    return NOERROR;
}

void TransitionSet::SetupStartEndListeners()
{
    AutoPtr<TransitionSetListener> listener = new TransitionSetListener(this);
    Int32 numTransitions = 0;
    mTransitions->GetSize(&numTransitions);
    for (Int32 i = 0; i < numTransitions; i++) {
        AutoPtr<IInterface> t;
        mTransitions->Get(i, (IInterface**)&t);
        AutoPtr<ITransition> childTransition = ITransition::Probe(t);
        AutoPtr<ITransition> res;
        childTransition->AddListener(listener, (ITransition**)&res);
    }
    mTransitions->GetSize(&mCurrentListeners);
}

ECode TransitionSet::CreateAnimators(
    /* [in] */ IViewGroup* sceneRoot,
    /* [in] */ ITransitionValuesMaps* startValues,
    /* [in] */ ITransitionValuesMaps* endValues,
    /* [in] */ IArrayList* startValuesList,
    /* [in] */ IArrayList* endValuesList)
{
    Int64 startDelay = 0;
    GetStartDelay(&startDelay);
    Int32 numTransitions = 0;
    mTransitions->GetSize(&numTransitions);
    for (Int32 i = 0; i < numTransitions; i++) {
        AutoPtr<IInterface> t;
        mTransitions->Get(i, (IInterface**)&t);
        AutoPtr<ITransition> childTransition = ITransition::Probe(t);
        // We only set the start delay on the first transition if we are playing
        // the transitions sequentially.
        if (startDelay > 0 && (mPlayTogether || i == 0)) {
            Int64 childStartDelay = 0;
            childTransition->GetStartDelay(&childStartDelay);
            AutoPtr<ITransition> res;
            if (childStartDelay > 0) {
                childTransition->SetStartDelay(startDelay + childStartDelay, (ITransition**)&res);
            }
            else {
                childTransition->SetStartDelay(startDelay, (ITransition**)&res);
            }
        }
        assert(0 && "TODO");
        // childTransition->CreateAnimators(sceneRoot, startValues, endValues, startValuesList,
        //         endValuesList);
    }
    return NOERROR;
}

ECode TransitionSet::RunAnimators()
{
    Boolean bEmp = FALSE;
    if ((mTransitions->IsEmpty(&bEmp), bEmp)) {
        Start();
        End();
        return NOERROR;
    }
    SetupStartEndListeners();
    Int32 numTransitions = 0;
    mTransitions->GetSize(&numTransitions);
    if (!mPlayTogether) {
        // Setup sequence with listeners
        // TODO: Need to add listeners in such a way that we can remove them later if canceled
        for (Int32 i = 1; i < numTransitions; ++i) {
            AutoPtr<IInterface> pt;
            mTransitions->Get(i - 1, (IInterface**)&pt);
            AutoPtr<ITransition> previousTransition = ITransition::Probe(pt);
            AutoPtr<IInterface> nt;
            mTransitions->Get(i, (IInterface**)&nt);
            AutoPtr<ITransition> nextTransition = ITransition::Probe(nt);
            AutoPtr<TransitionListenerAdapter_1> p = new TransitionListenerAdapter_1(nextTransition);
            AutoPtr<ITransition> res;
            previousTransition->AddListener(ITransitionListener::Probe(p), (ITransition**)&res);
        }
        AutoPtr<IInterface> ft;
        mTransitions->Get(0, (IInterface**)&ft);
        AutoPtr<ITransition> firstTransition = ITransition::Probe(ft);
        if (firstTransition != NULL) {
            firstTransition->RunAnimators();
        }
    }
    else {
        for (Int32 i = 0; i < numTransitions; ++i) {
            AutoPtr<IInterface> t;
            mTransitions->Get(i, (IInterface**)&t);
            AutoPtr<ITransition> p = ITransition::Probe(t);
            p->RunAnimators();
        }
    }
    return NOERROR;
}

ECode TransitionSet::CaptureStartValues(
    /* [in] */ ITransitionValues* transitionValues)
{
    AutoPtr<CTransitionValues> ctv = (CTransitionValues*)transitionValues;
    Boolean b = FALSE;
    if ((IsValidTarget(ctv->mView, &b), b)) {
        AutoPtr<IIterator> lit;
        mTransitions->GetIterator((IIterator**)&lit);
        Boolean bHasMore = FALSE;
        while ((lit->HasNext(&bHasMore), bHasMore)) {
            AutoPtr<IInterface> p;
            lit->GetNext((IInterface**)&p);
            AutoPtr<ITransition> childTransition = ITransition::Probe(p);
            Boolean bValidT = FALSE;
            if ((childTransition->IsValidTarget(ctv->mView, &bValidT), bValidT)) {
                childTransition->CaptureStartValues(transitionValues);
                ctv->mTargetedTransitions->Add(childTransition);
            }
        }
    }
    return NOERROR;
}

ECode TransitionSet::CaptureEndValues(
    /* [in] */ ITransitionValues* transitionValues)
{
    AutoPtr<CTransitionValues> ctv = (CTransitionValues*)transitionValues;
    Boolean b = FALSE;
    if ((IsValidTarget(ctv->mView, &b), b)) {
        AutoPtr<IIterator> lit;
        mTransitions->GetIterator((IIterator**)&lit);
        Boolean bHasMore = FALSE;
        while ((lit->HasNext(&bHasMore), bHasMore)) {
            AutoPtr<IInterface> p;
            lit->GetNext((IInterface**)&p);
            AutoPtr<ITransition> childTransition = ITransition::Probe(p);
            Boolean bValidT = FALSE;
            if ((childTransition->IsValidTarget(ctv->mView, &bValidT), bValidT)) {
                childTransition->CaptureEndValues(transitionValues);
                ctv->mTargetedTransitions->Add(childTransition);
            }
        }
    }
    return NOERROR;
}

ECode TransitionSet::CapturePropagationValues(
    /* [in] */ ITransitionValues* transitionValues)
{
    Transition::CapturePropagationValues(transitionValues);
    Int32 numTransitions = 0;
    mTransitions->GetSize(&numTransitions);
    for (Int32 i = 0; i < numTransitions; ++i) {
        AutoPtr<IInterface> t;
        mTransitions->Get(i, (IInterface**)&t);
        AutoPtr<ITransition> p = ITransition::Probe(t);
        p->CapturePropagationValues(transitionValues);
    }
    return NOERROR;
}

ECode TransitionSet::Pause(
    /* [in] */ IView* sceneRoot)
{
    Transition::Pause(sceneRoot);
    Int32 numTransitions = 0;
    mTransitions->GetSize(&numTransitions);
    for (Int32 i = 0; i < numTransitions; ++i) {
        AutoPtr<IInterface> t;
        mTransitions->Get(i, (IInterface**)&t);
        AutoPtr<ITransition> p = ITransition::Probe(t);
        p->Pause(sceneRoot);
    }
    return NOERROR;
}

ECode TransitionSet::Resume(
    /* [in] */ IView* sceneRoot)
{
    Transition::Resume(sceneRoot);
    Int32 numTransitions = 0;
    mTransitions->GetSize(&numTransitions);
    for (Int32 i = 0; i < numTransitions; ++i) {
        AutoPtr<IInterface> t;
        mTransitions->Get(i, (IInterface**)&t);
        AutoPtr<ITransition> p = ITransition::Probe(t);
        p->Resume(sceneRoot);
    }
    return NOERROR;
}

ECode TransitionSet::Cancel()
{
    Transition::Cancel();
    Int32 numTransitions = 0;
    mTransitions->GetSize(&numTransitions);
    for (Int32 i = 0; i < numTransitions; ++i) {
        AutoPtr<IInterface> t;
        mTransitions->Get(i, (IInterface**)&t);
        AutoPtr<ITransition> p = ITransition::Probe(t);
        p->Cancel();
    }
    return NOERROR;
}

ECode TransitionSet::SetSceneRoot(
    /* [in] */ IViewGroup* sceneRoot,
    /* [out] */ ITransitionSet** result)
{
    VALIDATE_NOT_NULL(result)

    AutoPtr<ITransition> res;
    Transition::SetSceneRoot(sceneRoot, (ITransition**)&res);
    Int32 numTransitions = 0;
    mTransitions->GetSize(&numTransitions);
    for (Int32 i = 0; i < numTransitions; ++i) {
        AutoPtr<IInterface> t;
        mTransitions->Get(i, (IInterface**)&t);
        AutoPtr<ITransition> p = ITransition::Probe(t);
        p->SetSceneRoot(sceneRoot, (ITransition**)&res);
    }
    *result = this;
    REFCOUNT_ADD(*result)
    return NOERROR;
}

ECode TransitionSet::SetCanRemoveViews(
    /* [in] */ Boolean canRemoveViews)
{
    Transition::SetCanRemoveViews(canRemoveViews);
    Int32 numTransitions = 0;
    mTransitions->GetSize(&numTransitions);
    for (Int32 i = 0; i < numTransitions; ++i) {
        AutoPtr<IInterface> t;
        mTransitions->Get(i, (IInterface**)&t);
        AutoPtr<ITransition> p = ITransition::Probe(t);
        p->SetCanRemoveViews(canRemoveViews);
    }
    return NOERROR;
}

ECode TransitionSet::SetPropagation(
    /* [in] */ ITransitionPropagation* propagation)
{
    Transition::SetPropagation(propagation);
    Int32 numTransitions = 0;
    mTransitions->GetSize(&numTransitions);
    for (Int32 i = 0; i < numTransitions; ++i) {
        AutoPtr<IInterface> t;
        mTransitions->Get(i, (IInterface**)&t);
        AutoPtr<ITransition> p = ITransition::Probe(t);
        p->SetPropagation(propagation);
    }
    return NOERROR;
}

ECode TransitionSet::SetEpicenterCallback(
    /* [in] */ IEpicenterCallback* epicenterCallback)
{
    Transition::SetEpicenterCallback(epicenterCallback);
    Int32 numTransitions = 0;
    mTransitions->GetSize(&numTransitions);
    for (Int32 i = 0; i < numTransitions; ++i) {
        AutoPtr<IInterface> t;
        mTransitions->Get(i, (IInterface**)&t);
        AutoPtr<ITransition> p = ITransition::Probe(t);
        p->SetEpicenterCallback(epicenterCallback);
    }
    return NOERROR;
}

ECode TransitionSet::ToString(
    /* [in] */ String indent,
    /* [out] */ String* res)
{
    VALIDATE_NOT_NULL(res)

    String result;
    Transition::ToString(indent, &result);
    Int32 numTransitions = 0;
    mTransitions->GetSize(&numTransitions);
    for (Int32 i = 0; i < numTransitions; ++i) {
        AutoPtr<IInterface> t;
        mTransitions->Get(i, (IInterface**)&t);
        AutoPtr<ITransition> p = ITransition::Probe(t);
        result += "\n";
        String strP;
        p->ToString(indent + "  ", &strP);
        result += strP;
    }
    *res = result;
    return NOERROR;
}

ECode TransitionSet::Clone(
    /* [out] */ ITransitionSet** result)
{
    VALIDATE_NOT_NULL(result)

    AutoPtr<IInterface> p;
    Transition::Clone((IInterface**)&p);
    AutoPtr<ITransitionSet> clone = ITransitionSet::Probe(p);
    AutoPtr<TransitionSet> cClone = (TransitionSet*)clone.Get();
    CArrayList::New((IArrayList**)&(cClone->mTransitions));
    Int32 numTransitions = 0;
    mTransitions->GetSize(&numTransitions);
    for (Int32 i = 0; i < numTransitions; ++i) {
        AutoPtr<IInterface> t;
        mTransitions->Get(i, (IInterface**)&t);
        AutoPtr<ITransition> pT = ITransition::Probe(t);
        AutoPtr<Transition> cPt = (Transition*)pT.Get();
        AutoPtr<IInterface> pInf;
        cPt->Clone((IInterface**)&pInf);
        AutoPtr<ITransition> cl = ITransition::Probe(pInf);
        AutoPtr<ITransitionSet> res;
        clone->AddTransition(cl, (ITransitionSet**)&res);
    }
    *result = clone;
    REFCOUNT_ADD(*result)
    return NOERROR;
}

//===============================================================
// TransitionSet::TransitionSetListener::
//===============================================================

TransitionSet::TransitionSetListener::TransitionSetListener(
    /* [in] */ ITransitionSet* transitionSet)
{
    mTransitionSet = transitionSet;
}

ECode TransitionSet::TransitionSetListener::OnTransitionStart(
    /* [in] */ ITransition* transition)
{
    AutoPtr<TransitionSet> cts = (TransitionSet*)mTransitionSet.Get();
    if (!cts->mStarted) {
        mTransitionSet->Start();
        cts->mStarted = TRUE;
    }
    return NOERROR;
}

ECode TransitionSet::TransitionSetListener::OnTransitionEnd(
    /* [in] */ ITransition* transition)
{
    AutoPtr<TransitionSet> cts = (TransitionSet*)mTransitionSet.Get();
    --cts->mCurrentListeners;
    if (cts->mCurrentListeners == 0) {
        // All child trans
        cts->mStarted = FALSE;
        mTransitionSet->End();
    }
    AutoPtr<ITransition> res;
    transition->RemoveListener(this, (ITransition**)&res);
    return NOERROR;
}

//===============================================================
// TransitionSet::TransitionListenerAdapter_1::
//===============================================================
TransitionSet::TransitionListenerAdapter_1::TransitionListenerAdapter_1(
    /* [in] */ ITransition* nxtT)
{
    mNextTransition = nxtT;
}

ECode TransitionSet::TransitionListenerAdapter_1::OnTransitionEnd(
    /* [in] */ ITransition* transition)
{
    mNextTransition->RunAnimators();
    AutoPtr<ITransition> res;
    transition->RemoveListener(this, (ITransition**)&res);
    return NOERROR;
}

} // namespace Transition
} // namepsace Droid
} // namespace Elastos
