#include "elastos/droid/animation/AnimatorSet.h"
#include "elastos/droid/animation/ValueAnimator.h"
#include "elastos/droid/animation/CAnimatorSet.h"
#include "elastos/droid/animation/CAnimatorSetBuilder.h"

using Elastos::Utility::IIterator;
using Elastos::Utility::CArrayList;

namespace Elastos {
namespace Droid {
namespace Animation {
// {df3afd4a-f894-4214-b149-fef6f804e5e0}
extern "C" const InterfaceID EIID_AnimatorSet =
        { 0xdf3afd4a, 0xf894, 0x4214, { 0xb1, 0x49, 0xfe, 0xf6, 0xf8, 0x04, 0xe5, 0xe0 } };

// {fa05e37f-0d4d-4a83-99d5-f03d1c07087d}
extern "C" const InterfaceID EIID_DependencyListener =
        { 0xfa05e37f, 0x0d4d, 0x4a83, { 0x99, 0xd5, 0xf0, 0x3d, 0x1c, 0x07, 0x08, 0x7d } };

// {841440c0-19b9-4a09-87df-a7f122d2911e}
extern "C" const InterfaceID EIID_AnimatorSetListener =
        { 0x841440c0, 0x19b9, 0x4a09, { 0x87, 0xdf, 0xa7, 0xf1, 0x22, 0xd2, 0x91, 0x1e } };

//==============================================================================
//                  AnimatorSet::DependencyListener
//==============================================================================
PInterface AnimatorSet::DependencyListener::Probe(
    /* [in] */ REIID riid)
{
    if ( riid == EIID_IInterface) {
        return (IInterface*)(IAnimatorListener *)this;
    }
    else if ( riid == EIID_IAnimatorListener ) {
        return (IAnimatorListener *)this;
    }
    else if( riid == EIID_DependencyListener) {
        return reinterpret_cast<PInterface>(this);
    }
    return Object::Probe(riid);
}

UInt32 AnimatorSet::DependencyListener::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 AnimatorSet::DependencyListener::Release()
{
    return ElRefBase::Release();
}

ECode AnimatorSet::DependencyListener::GetInterfaceID(
    /* [in] */ IInterface* object,
    /* [out] */ InterfaceID* iid)
{
    VALIDATE_NOT_NULL(iid);
    if (object == (IInterface*)(IAnimatorListener *)this) {
        *iid = EIID_IAnimatorListener ;
    }
    else {
        return Object::GetInterfaceID(object, iid);
    }
    return NOERROR;
}

AnimatorSet::DependencyListener::DependencyListener(
    /* [in] */ AnimatorSet* animatorset,
    /* [in] */ Node* node,
    /* [in] */ Int32 rule)
    : mRule(rule)
{
    mAnimatorSet = animatorset;
    mNode = node;
}

ECode AnimatorSet::DependencyListener::OnAnimationStart(
    /* [in] */ IAnimator* animation)
{
    if (mRule == Dependency::WITH) {
        StartIfReady(animation);
    }
    return NOERROR;
}

ECode AnimatorSet::DependencyListener::OnAnimationEnd(
    /* [in] */ IAnimator* animation)
{
    if (mRule == Dependency::AFTER)
        StartIfReady(animation);
    return NOERROR;
}

ECode AnimatorSet::DependencyListener::OnAnimationCancel(
    /* [in] */ IAnimator* animation)
{
    return NOERROR;
}

ECode AnimatorSet::DependencyListener::OnAnimationRepeat(
    /* [in] */ IAnimator* animation)
{
    return NOERROR;
}

void AnimatorSet::DependencyListener::StartIfReady(
    /* [in] */ IAnimator* dependencyAnimation)
{
    if (mAnimatorSet->mTerminated) {
        // if the parent AnimatorSet was canceled, then don't start any dependent anims
        return;
    }
    AutoPtr<Dependency> dependencyToRemove = NULL;
    List<AutoPtr<Dependency> >::Iterator it = mNode->mTmpDependencies.Begin();
    for (; it != mNode->mTmpDependencies.End(); ++it) {
        AutoPtr<Dependency> dependency = *it;
        if (dependency->mRule == mRule &&
                 dependencyAnimation == dependency->mNode->mAnimation) {
            // rule fired - remove the dependency and listener and check to
            // see whether it's time to start the animation
            dependencyToRemove = dependency;
            dependencyAnimation->RemoveListener(this);
            break;
        }
    }
    mNode->mTmpDependencies.Remove(dependencyToRemove);
    if (mNode->mTmpDependencies.IsEmpty()) {
        // all dependencies satisfied: start the animation
        mNode->mAnimation->Start();
        mAnimatorSet->mPlayingSet.PushBack(mNode->mAnimation);
    }
}

//==============================================================================
//                  AnimatorSet::AnimatorSetListener
//==============================================================================
PInterface AnimatorSet::AnimatorSetListener::Probe(
    /* [in] */ REIID riid)
{
    if ( riid == EIID_IInterface) {
        return (IInterface*)(IAnimatorListener *)this;
    }
    else if ( riid == EIID_IAnimatorListener ) {
        return (IAnimatorListener *)this;
    }
    else if( riid == EIID_AnimatorSetListener) {
        return reinterpret_cast<PInterface>(this);
    }
    return Object::Probe(riid);
}

UInt32 AnimatorSet::AnimatorSetListener::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 AnimatorSet::AnimatorSetListener::Release()
{
    return ElRefBase::Release();
}

ECode AnimatorSet::AnimatorSetListener::GetInterfaceID(
    /* [in] */ IInterface* object,
    /* [out] */ InterfaceID* iid)
{
    VALIDATE_NOT_NULL(iid);
    if (object == (IInterface*)(IAnimatorListener *)this) {
        *iid = EIID_IAnimatorListener ;
    }
    else {
        return Object::GetInterfaceID(object, iid);
    }
    return NOERROR;
}

AnimatorSet::AnimatorSetListener::AnimatorSetListener(
    /* [in] */ AnimatorSet* animatorSet) : mAnimatorSet(animatorSet)
{}

ECode AnimatorSet::AnimatorSetListener::OnAnimationCancel(
    /* [in] */ IAnimator* animation)
{
    if (!mAnimatorSet->mTerminated) {
        // Listeners are already notified of the AnimatorSet canceling in cancel().
        // The logic below only kicks in when animations end normally
        if (mAnimatorSet->mPlayingSet.IsEmpty()) {
            if (mAnimatorSet->mListeners.IsEmpty() == FALSE) {
                List<AutoPtr<IAnimatorListener> >::Iterator it = mAnimatorSet->mListeners.Begin();
                for (; it != mAnimatorSet->mListeners.Begin(); ++it) {
                    (*it)->OnAnimationCancel((IAnimator*)(mAnimatorSet->Probe(EIID_IAnimator)));
                }
            }
        }
    }
    return NOERROR;
}

ECode AnimatorSet::AnimatorSetListener::OnAnimationEnd(
    /* [in] */ IAnimator* animation)
{
    // mAnimatorSet's reference may be released to 0 when (*itListeners)->OnAnimationEnd()
    mAnimatorSet->Probe(EIID_IAnimatorSet)->AddRef();
    animation->RemoveListener(this);
    mAnimatorSet->mPlayingSet.Remove(animation);
    AutoPtr<IAnimator> key = animation;
    AutoPtr<Node> animNode = (mAnimatorSet->mNodeMap)[key];
    animNode->mDone = TRUE;
    if (!(mAnimatorSet->mTerminated)) {
        // Listeners are already notified of the AnimatorSet ending in cancel() or
        // end(); the logic below only kicks in when animations end normally
        List<AutoPtr<Node> > sortedNodes(mAnimatorSet->mSortedNodes);
        Boolean allDone = TRUE;
        List<AutoPtr<Node> >::Iterator sortedIt = sortedNodes.Begin();
        for (; sortedIt != sortedNodes.End(); ++sortedIt) {
            if (!((*sortedIt)->mDone)) {
                allDone = FALSE;
                break;
            }
        }
        if (allDone) {
            // If this was the last child animation to end, then notify listeners that this
            // AnimatorSet has ended
            if (mAnimatorSet->mListeners.IsEmpty() == FALSE) {
                List<AutoPtr<IAnimatorListener> >::Iterator itListeners = mAnimatorSet->mListeners.Begin();
                for (; itListeners != mAnimatorSet->mListeners.End(); ++itListeners) {
                    (*itListeners)->OnAnimationEnd((IAnimator*)(mAnimatorSet->Probe(EIID_IAnimator)));
                }
            }
            mAnimatorSet->mStarted = FALSE;
            mAnimatorSet->mPaused = FALSE;
        }
    }
    mAnimatorSet->Probe(EIID_IAnimatorSet)->Release();
    return NOERROR;
}

ECode AnimatorSet::AnimatorSetListener::OnAnimationRepeat(
    /* [in] */ IAnimator* animation)
{
    return NOERROR;
}

ECode AnimatorSet::AnimatorSetListener::OnAnimationStart(
    /* [in] */ IAnimator* animation)
{
    return NOERROR;
}

//==============================================================================
//                  AnimatorSet::Dependency
//==============================================================================
AnimatorSet::Dependency::Dependency(
    /* [in] */ Node* node,
    /* [in] */ Int32 rule)
    : mNode(node)
    , mRule(rule)
{
}

//==============================================================================
//                  AnimatorSet::Node
//==============================================================================
CAR_INTERFACE_IMPL(AnimatorSet::Node, Object, ICloneable);
AnimatorSet::Node::Node(
    /* [in] */ IAnimator* animation)
    : mAnimation(animation)
    , mDone(FALSE)
{}

AnimatorSet::Node::Node()
    : mDone(FALSE)
{}

AnimatorSet::Node::~Node()
{
}

void AnimatorSet::Node::AddDependency(
    /* [in] */ Dependency* dependency)
{
    AutoPtr<Dependency> dependencyTmp = dependency;
    mDependencies.PushBack(dependencyTmp);
    List<AutoPtr<Node> >::Iterator itFind = Find(mNodeDependencies.Begin(), mNodeDependencies.End(), dependency->mNode);
    if (itFind == mNodeDependencies.End()) {
        mNodeDependencies.PushBack(dependency->mNode);
    }
    AutoPtr<Node> dependencyNode = dependency->mNode;
    dependencyNode->mNodeDependents.PushBack(this);
}

ECode AnimatorSet::Node::Clone(
    /* [out] */ IInterface** obj)
{
    VALIDATE_NOT_NULL(obj);
    AutoPtr<Node> newObject = new Node();
    mAnimation->Clone((IAnimator**)(&(newObject->mAnimation)));
    newObject->mDependencies.Assign(mDependencies.Begin(), mDependencies.End());
    newObject->mTmpDependencies.Assign(mTmpDependencies.Begin(), mTmpDependencies.End());
    newObject->mNodeDependencies.Assign(mNodeDependencies.Begin(), mNodeDependencies.End());
    newObject->mNodeDependents.Assign(mNodeDependents.Begin(), mNodeDependents.End());
    newObject->mDone = mDone;
    *obj = (ICloneable*)newObject->Probe(EIID_ICloneable);
    REFCOUNT_ADD(*obj);
    return NOERROR;
}

AnimatorSet::AnimatorListenerAdapterIMPL::AnimatorListenerAdapterIMPL(
    /* [in] */ AnimatorSet* host,
    /* [in] */ List<AutoPtr<Node> >* nodes)
    : mHost(host)
    , mNodes(nodes)
    , mCanceled(FALSE)
{}

AnimatorSet::AnimatorListenerAdapterIMPL::~AnimatorListenerAdapterIMPL()
{
}

ECode AnimatorSet::AnimatorListenerAdapterIMPL::OnAnimationStart(
    /* [in] */ IAnimator* animation)
{
    return NOERROR;
}

ECode AnimatorSet::AnimatorListenerAdapterIMPL::OnAnimationEnd(
    /* [in] */ IAnimator* animation)
{
    if (!mCanceled) {
        List<AutoPtr<Node> >::Iterator it = mNodes->Begin();
        for (; it != mNodes->End(); ++it) {
            AutoPtr<Node> node = *it;
            node->mAnimation->Start();
            mHost->mPlayingSet.PushBack(node->mAnimation);
        }
    }
    mHost->mDelayAnim = NULL;
    return NOERROR;
}

ECode AnimatorSet::AnimatorListenerAdapterIMPL::OnAnimationCancel(
    /* [in] */ IAnimator* animation)
{
    mCanceled = TRUE;
    return NOERROR;
}

ECode AnimatorSet::AnimatorListenerAdapterIMPL::OnAnimationRepeat(
    /* [in] */ IAnimator* animation)
{
    return NOERROR;
}

//==============================================================================
//                  AnimatorSet
//==============================================================================
UInt32 AnimatorSet::AddRef()
{
    return Animator::AddRef();
}

UInt32 AnimatorSet::Release()
{
    return Animator::Release();
}

ECode AnimatorSet::GetInterfaceID(
    /* [in] */ IInterface* object,
    /* [out] */ InterfaceID* iid)
{
    VALIDATE_NOT_NULL(iid);
    if (object == reinterpret_cast<PInterface>((AnimatorSet *)this)) {
        *iid = EIID_AnimatorSet;
    }
    else if (object == (IInterface*)(IAnimatorSet *)this) {
        *iid = EIID_IAnimatorSet;
    }

    return Animator::GetInterfaceID(object, iid);
}

PInterface AnimatorSet::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_AnimatorSet) {
        return reinterpret_cast<PInterface>(this);
    }
    else if (riid == EIID_IAnimatorSet) {
        return (IInterface*)(IAnimatorSet*)this;
    }

    return Animator::Probe(riid);
}

AnimatorSet::AnimatorSet()
    : mTerminated(FALSE)
    , mNeedsSort(TRUE)
    , mStarted(FALSE)
    , mStartDelay(0)
    , mDuration(-1)
    , mReversible(TRUE)
{
}

AnimatorSet::~AnimatorSet()
{
}

ECode AnimatorSet::PlayTogether(
    /* [in] */ ArrayOf<IAnimator*>* items)
{
    if (items != NULL && items->GetLength() > 0) {
        mNeedsSort = TRUE;
        AutoPtr<IAnimatorSetBuilder> builder;
        Play((*items)[0], (IAnimatorSetBuilder**)&builder);
        for (Int32 i = 1; i < items->GetLength(); ++i) {
            builder->With((*items)[i]);
        }
    }
    return NOERROR;
}

ECode AnimatorSet::PlayTogether(
    /* [in] */ ICollection* items)
{
    if (items != NULL) {
        Int32 count;
        items->GetSize(&count);

        if (count > 0) {
            mNeedsSort = TRUE;
            AutoPtr<IAnimatorSetBuilder> builder;
            AutoPtr<IIterator> it;
            items->GetIterator((IIterator**)&it);
            Boolean hasNext = FALSE;
            while (it->HasNext(&hasNext), hasNext) {
                AutoPtr<IAnimator> anim;
                it->GetNext((IInterface**)&anim);
                if (builder == NULL) {
                    Play(anim, (IAnimatorSetBuilder**)&builder);
                }
                else {
                    builder->With(anim);
                }
            }
        }
    }
    return NOERROR;
}

ECode AnimatorSet::PlaySequentially(
    /* [in] */ ArrayOf<IAnimator*>* items)
{
    if (items != NULL) {
        mNeedsSort = TRUE;
        if (items->GetLength() == 1) {
            AutoPtr<IAnimatorSetBuilder> builder;
            Play((*items)[0], (IAnimatorSetBuilder**)&builder);
        } else {
            mReversible = FALSE;
            for (Int32 i = 0; i < items->GetLength() - 1; ++i) {
                AutoPtr<IAnimatorSetBuilder> builder;
                Play((*items)[i], (IAnimatorSetBuilder**)&builder);
                builder->Before((*items)[i+1]);
            }
        }
    }
    return NOERROR;
}

ECode AnimatorSet::PlaySequentially(
    /* [in] */ IList* items)
{
    Int32 size = 0;
    if (items != NULL && (items->GetSize(&size), size) > 0) {
        mNeedsSort = TRUE;
        if (size == 1) {
            AutoPtr<IAnimator> animator;
            items->Get(0, (IInterface**)&animator);
            AutoPtr<IAnimatorSetBuilder> as;
            Play(animator, (IAnimatorSetBuilder**)&as);
        } else {
            mReversible = FALSE;
            for (Int32 i = 0; i < size - 1; ++i) {
                AutoPtr<IAnimator> animator;
                items->Get(i, (IInterface**)&animator);
                AutoPtr<IAnimator> animator2;
                items->Get(i + 1, (IInterface**)&animator2);
                AutoPtr<IAnimatorSetBuilder> as;
                Play(animator, (IAnimatorSetBuilder**)&as);
                as->Before(animator2);
            }
        }
    }

    return NOERROR;
}

ECode AnimatorSet::GetChildAnimations(
    /* [out, callee] */ ArrayOf<IAnimator*>** childAnimations)
{
    VALIDATE_NOT_NULL(childAnimations);
    Int32 size = mNodes.GetSize();

    AutoPtr<ArrayOf<IAnimator*> > array = ArrayOf<IAnimator*>::Alloc(size);
    List<AutoPtr<Node> >::Iterator it = mNodes.Begin();
    Int32 i = 0;
    for (; it != mNodes.End(); ++it, ++i) {
        array->Set(i, (*it)->mAnimation);
    }
    *childAnimations = array;
    REFCOUNT_ADD(*childAnimations);
    return NOERROR;
}

ECode AnimatorSet::GetChildAnimations(
    /* [out] */ IArrayList** childAnimations)
{
    VALIDATE_NOT_NULL(childAnimations);
    AutoPtr<IArrayList> array;
    CArrayList::New((IArrayList**)&array);

    Int32 pos = 0;
    List<AutoPtr<Node> >::Iterator it = mNodes.Begin();
    for (; it != mNodes.End(); ++it) {
        array->Add(pos++, ((*it)->mAnimation).Get());
    }
    *childAnimations = array;
    REFCOUNT_ADD(*childAnimations);
    return NOERROR;
}

ECode AnimatorSet::SetTarget(
    /* [in] */ IInterface* target)
{
    List<AutoPtr<Node> >::Iterator it = mNodes.Begin();
    for (; it != mNodes.End(); ++it) {
        AutoPtr<IAnimator> animation = (*it)->mAnimation;
        if (animation->Probe(EIID_IAnimatorSet) || animation->Probe(EIID_IObjectAnimator)) {
            animation->SetTarget(target);
        }
    }
    return NOERROR;
}

ECode AnimatorSet::SetInterpolator(
    /* [in] */ ITimeInterpolator* interpolator)
{
    mInterpolator = interpolator;
    return NOERROR;
}

ECode AnimatorSet::GetInterpolator(
    /* [out] */ ITimeInterpolator** interpolator)
{
    VALIDATE_NOT_NULL(interpolator);
    *interpolator = mInterpolator;
    REFCOUNT_ADD(*interpolator);
    return NOERROR;
}

ECode AnimatorSet::Play(
    /* [in] */ IAnimator* anim,
    /* [out] */ IAnimatorSetBuilder** builder)
{
    VALIDATE_NOT_NULL(builder);
    if (anim != NULL) {
        mNeedsSort = TRUE;
        return CAnimatorSetBuilder::New(THIS_PROBE(IAnimatorSet), anim, builder);
    }

    *builder = NULL;
    return NOERROR;
}

ECode AnimatorSet::Cancel()
{
    mTerminated = TRUE;
    Boolean started = FALSE;
    if (IsStarted(&started), started) {
        List<AutoPtr<IAnimatorListener> > tmpListeners(mListeners);
        if (tmpListeners.IsEmpty() == FALSE) {
            List<AutoPtr<IAnimatorListener> >::Iterator it = tmpListeners.Begin();
            for (; it != tmpListeners.End(); it++) {
                (*it)->OnAnimationCancel(THIS_PROBE(IAnimator));
            }
        }
        Boolean running;
        if (mDelayAnim != NULL && (IAnimator::Probe(mDelayAnim)->IsRunning(&running), running)) {
            // If we're currently in the startDelay period, just cancel that animator and
            // send out the end event to all listeners
            IAnimator::Probe(mDelayAnim)->Cancel();
        } else  if (mSortedNodes.IsEmpty() == FALSE) {
            List<AutoPtr<Node> >::Iterator it = mSortedNodes.Begin();
            for (; it != mSortedNodes.End(); it++) {
                (*it)->mAnimation->Cancel();
            }
        }
        if (tmpListeners.IsEmpty() == FALSE) {
            List<AutoPtr<IAnimatorListener> >::Iterator it = tmpListeners.Begin();
            for (; it != tmpListeners.End(); it++) {
                (*it)->OnAnimationEnd(THIS_PROBE(IAnimator));
            }
        }
        mStarted = FALSE;
    }
    return NOERROR;
}

ECode AnimatorSet::End()
{
    mTerminated = TRUE;
    Boolean started = FALSE;
    if (IsStarted(&started), started) {
        if (mSortedNodes.GetSize() != mNodes.GetSize()) {
            // hasn't been started yet - sort the nodes now, then end them
            SortNodes();
            List<AutoPtr<Node> >::Iterator it = mSortedNodes.Begin();
            for (; it != mSortedNodes.End(); it++) {
                if (mSetListener == NULL) {
                    mSetListener = new AnimatorSetListener(this);
                }
                (*it)->mAnimation->AddListener(mSetListener);
            }
        }
        if (mDelayAnim != NULL) {
            IAnimator::Probe(mDelayAnim)->Cancel();
        }
        if (mSortedNodes.IsEmpty() == FALSE) {
            List<AutoPtr<Node> >::Iterator it = mSortedNodes.Begin();
            for (; it != mSortedNodes.End(); it++) {
                (*it)->mAnimation->End();
            }
        }
        if (mListeners.IsEmpty() == FALSE) {
            List<AutoPtr<IAnimatorListener> > tmpListeners(mListeners);
            List<AutoPtr<IAnimatorListener> >::Iterator it = tmpListeners.Begin();
            for (; it != tmpListeners.End(); it++) {
                (*it)->OnAnimationEnd(THIS_PROBE(IAnimator));
            }
        }
        mStarted = FALSE;
    }
    return NOERROR;
}

ECode AnimatorSet::IsRunning(
    /* [out] */ Boolean* _running)
{
    VALIDATE_NOT_NULL(_running);
    List<AutoPtr<Node> >::Iterator it = mNodes.Begin();
    for (; it != mNodes.End(); it++) {
        Boolean running;
        (*it)->mAnimation->IsRunning(&running);
        if (running) {
            *_running = TRUE;
            return NOERROR;
        }
    }
    *_running = FALSE;
    return NOERROR;
}

ECode AnimatorSet::IsStarted(
    /* [out] */ Boolean* started)
{
    VALIDATE_NOT_NULL(started);
    *started = mStarted;
    return NOERROR;
}

ECode AnimatorSet::GetStartDelay(
    /* [out] */ Int64* delay)
{
    VALIDATE_NOT_NULL(delay);
    *delay = mStartDelay;
    return NOERROR;
}

ECode AnimatorSet::SetStartDelay(
    /* [in] */ Int64 startDelay)
{
    if (mStartDelay > 0) {
        mReversible = FALSE;
    }
    mStartDelay = startDelay;
    return NOERROR;
}

ECode AnimatorSet::GetDuration(
    /* [out] */ Int64* duration)
{
    VALIDATE_NOT_NULL(duration);
    *duration = mDuration;
    return NOERROR;
}

ECode AnimatorSet::SetDuration(
    /* [in] */ Int64 duration)
{
    if (duration < 0) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    // Just record the value for now - it will be used later when the AnimatorSet starts
    mDuration = duration;
    return NOERROR;
}

ECode AnimatorSet::SetupStartValues()
{
    List<AutoPtr<Node> >::Iterator it = mNodes.Begin();
    for (; it != mNodes.End(); it++) {
        (*it)->mAnimation->SetupStartValues();
    }
    return NOERROR;
}

ECode AnimatorSet::SetupEndValues()
{
    List<AutoPtr<Node> >::Iterator it = mNodes.Begin();
    for (; it != mNodes.End(); it++) {
        (*it)->mAnimation->SetupEndValues();
    }
    return NOERROR;
}

ECode AnimatorSet::Pause()
{
    Boolean previouslyPaused = mPaused;
    Animator::Pause();
    if (!previouslyPaused && mPaused) {
        if (mDelayAnim != NULL) {
            IAnimator::Probe(mDelayAnim)->Pause();
        } else {
            List<AutoPtr<Node> >::Iterator it = mNodes.Begin();
            for (; it != mNodes.End(); it++) {
                (*it)->mAnimation->Pause();
            }
        }
    }
    return NOERROR;
}

ECode AnimatorSet::Resume()
{
    Boolean previouslyPaused = mPaused;
    Animator::Resume();
    if (previouslyPaused && !mPaused) {
        if (mDelayAnim != NULL) {
            IAnimator::Probe(mDelayAnim)->Resume();
        } else {
            List<AutoPtr<Node> >::Iterator it = mNodes.Begin();
            for (; it != mNodes.End(); it++) {
                (*it)->mAnimation->Resume();
            }
        }
    }
    return NOERROR;
}

ECode AnimatorSet::Start()
{
    mTerminated = FALSE;
    mStarted = TRUE;
    mPaused = FALSE;

    List<AutoPtr<Node> >::Iterator it = mNodes.Begin();
    for (; it != mNodes.End(); it++) {
        (*it)->mAnimation->SetAllowRunningAsynchronously(FALSE);
    }

    if (mDuration >= 0) {
        // If the duration was set on this AnimatorSet, pass it along to all child animations
        List<AutoPtr<Node> >::Iterator it = mNodes.Begin();
        for (; it != mNodes.End(); it++) {
            // TODO: don't set the duration of the timing-only nodes created by AnimatorSet to
            // insert "play-after" delays
            (*it)->mAnimation->SetDuration(mDuration);
        }
    }

    if (mInterpolator != NULL) {
        List<AutoPtr<Node> >::Iterator it = mNodes.Begin();
        for (; it != mNodes.End(); it++) {
            (*it)->mAnimation->SetInterpolator(mInterpolator);
        }
    }

    // First, sort the nodes (if necessary). This will ensure that sortedNodes
    // contains the animation nodes in the correct order.
    SortNodes();

    it = mSortedNodes.Begin();
    for (; it!= mSortedNodes.End(); ++it) {
        AutoPtr<Node> node = *it;
        // First, clear out the old listeners
        AutoPtr<ArrayOf<IAnimatorListener*> > oldListeners;
        node->mAnimation->GetListeners((ArrayOf<IAnimatorListener*>**)&oldListeners);
        if (oldListeners != NULL && oldListeners->GetLength() > 0) {
            AutoPtr<ArrayOf<IAnimatorListener*> > clonedListeners = oldListeners->Clone();

            for (Int32 i = 0; i < clonedListeners->GetLength(); i++) {
                AutoPtr<IAnimatorListener> listener = (*clonedListeners)[i];
                if (listener->Probe(EIID_DependencyListener) || listener->Probe(EIID_AnimatorSetListener)) {
                    node->mAnimation->RemoveListener(listener);
                }
            }
        }
    }

    // nodesToStart holds the list of nodes to be started immediately. We don't want to
    // start the animations in the loop directly because we first need to set up
    // dependencies on all of the nodes. For example, we don't want to start an animation
    // when some other animation also wants to start when the first animation begins.
    AutoPtr< List<AutoPtr<Node> > > nodesToStart = new List<AutoPtr<Node> >;
    it = mSortedNodes.Begin();
    for (; it!= mSortedNodes.End(); ++it) {
        AutoPtr<Node> node = *it;
        if (mSetListener == NULL) {
            mSetListener = new AnimatorSetListener(this);
        }
        if (node->mDependencies.IsEmpty()) {
            nodesToStart->PushBack(node);
        } else {
            List<AutoPtr<Dependency> >::Iterator dependencyIt= node->mDependencies.Begin();
            for (; dependencyIt != node->mDependencies.End(); ++dependencyIt) {
                AutoPtr<Dependency> dependency = *dependencyIt;
                AutoPtr<DependencyListener> lTemp = new DependencyListener(this, node, dependency->mRule);
                dependency->mNode->mAnimation->AddListener(lTemp);
            }
            node->mTmpDependencies.Clear();
            node->mTmpDependencies.Assign(node->mDependencies.Begin(), node->mDependencies.End());
        }
        node->mAnimation->AddListener(mSetListener);
    }

    // Now that all dependencies are set up, start the animations that should be started.
    if (mStartDelay <= 0) {
        List<AutoPtr<Node> >::Iterator nodeIt = nodesToStart->Begin();
        for (; nodeIt != nodesToStart->End(); nodeIt++) {
            (*nodeIt)->mAnimation->Start();
            mPlayingSet.PushBack((*nodeIt)->mAnimation);
        }
    }
    else {
        AutoPtr<ArrayOf<Float> > fArray = ArrayOf<Float>::Alloc(2);
        (*fArray)[0] = 0.0f; (*fArray)[1] = 1.0f;
        mDelayAnim = ValueAnimator::OfFloat(fArray);
        IAnimator::Probe(mDelayAnim)->SetDuration(mStartDelay);
        AutoPtr<IAnimatorListener> aladapter = new AnimatorListenerAdapterIMPL(this, nodesToStart);
        IAnimator::Probe(mDelayAnim)->AddListener(aladapter);
        IAnimator::Probe(mDelayAnim)->Start();
    }

    if (!mListeners.IsEmpty()) {
        List<AutoPtr<IAnimatorListener> > tmpListeners(mListeners);
        List<AutoPtr<IAnimatorListener> >::Iterator tmpIt = tmpListeners.Begin();
        for (; tmpIt != tmpListeners.End(); ++tmpIt) {
            (*tmpIt)->OnAnimationStart(THIS_PROBE(IAnimator));
        }
    }
    if (mNodes.IsEmpty() && mStartDelay == 0) {
        // Handle unusual case where empty AnimatorSet is started - should send out
        // end event immediately since the event will not be sent out at all otherwise
        mStarted = FALSE;
        if (mListeners.IsEmpty() == FALSE) {
            List<AutoPtr<IAnimatorListener> > tmpListeners(mListeners);
            List<AutoPtr<IAnimatorListener> >::Iterator tmpIt = tmpListeners.Begin();
            for (; tmpIt != tmpListeners.End(); ++tmpIt) {
                (*tmpIt)->OnAnimationEnd(THIS_PROBE(IAnimator));
            }
        }
    }
    return NOERROR;
}

ECode AnimatorSet::Clone(
    /* [out] */ IInterface** object)
{
    AutoPtr<CAnimatorSet> newObject;
    CAnimatorSet::NewByFriend((CAnimatorSet**)&newObject);
    AnimatorSet* anim = newObject.Get();
    CloneSuperData(anim);
    /*
     * The basic clone() operation copies all items. This doesn't work very well for
     * AnimatorSet, because it will copy references that need to be recreated and state
     * that may not apply. What we need to do now is put the clone in an uninitialized
     * state, with fresh, empty data structures. Then we will build up the nodes list
     * manually, as we clone each Node (and its animation). The clone will then be sorted,
     * and will populate any appropriate lists, when it is started.
     */
    anim->mNeedsSort = TRUE;
    anim->mTerminated = FALSE;
    anim->mStarted = FALSE;
    anim->mReversible = mReversible;
    anim->mSetListener = NULL;

    // Walk through the old nodes list, cloning each node and adding it to the new nodemap.
    // One problem is that the old node dependencies point to nodes in the old AnimatorSet.
    // We need to track the old/new nodes in order to reconstruct the dependencies in the clone.
    HashMap<AutoPtr<Node>, AutoPtr<Node> > nodeCloneMap; // <old, new>
    List<AutoPtr<Node> >::Iterator it = mNodes.Begin();
    for (; it != mNodes.End(); it++) {
        AutoPtr<Node> node = *it;
        AutoPtr<IInterface> tmp;
        node->Clone((IInterface**)&tmp);
        AutoPtr<Node> nodeClone = (Node*)ICloneable::Probe(tmp);
        nodeCloneMap[node] = nodeClone;
        anim->mNodes.PushBack(nodeClone);
        (anim->mNodeMap)[nodeClone->mAnimation] = nodeClone;
        // Clear out the dependencies in the clone; we'll set these up manually later
        nodeClone->mDependencies.Clear();
        nodeClone->mTmpDependencies.Clear();
        nodeClone->mNodeDependents.Clear();
        nodeClone->mNodeDependencies.Clear();

        // clear out any listeners that were set up by the AnimatorSet; these will
        // be set up when the clone's nodes are sorted
        Animator* animator = reinterpret_cast<Animator*>(nodeClone->mAnimation->Probe(EIID_Animator));
        List<AutoPtr<IAnimatorListener> > cloneListeners(animator->mListeners);
        if (cloneListeners.IsEmpty() == FALSE) {
            List<AutoPtr<IAnimatorListener> > listenersToRemove;
            List<AutoPtr<IAnimatorListener> >::Iterator cloneIt = cloneListeners.Begin();
            for (; cloneIt != cloneListeners.End(); cloneIt++) {
                AutoPtr<IAnimatorListener> listener = *cloneIt;
                if (listener->Probe(EIID_AnimatorSetListener)) {
                    listenersToRemove.PushBack(listener);
                }
            }
            if (listenersToRemove.IsEmpty() == FALSE) {
                List<AutoPtr<IAnimatorListener> >::Iterator removeIt = listenersToRemove.Begin();
                for (; removeIt != listenersToRemove.End(); removeIt++) {
                    cloneListeners.Remove(*removeIt);
                }
            }
        }
    }
    // Now that we've cloned all of the nodes, we're ready to walk through their
    // dependencies, mapping the old dependencies to the new nodes
    List<AutoPtr<Node> >::Iterator nodeIt = mNodes.Begin();
    for (; nodeIt != mNodes.End(); nodeIt++) {
        AutoPtr<Node> node = *nodeIt;
        AutoPtr<Node> nodeClone = nodeCloneMap[node];
        if (node->mDependencies.IsEmpty() == FALSE) {
            List<AutoPtr<Dependency> >::Iterator dependencyIt = node->mDependencies.Begin();
            for (; dependencyIt != node->mDependencies.End(); dependencyIt++) {
                AutoPtr<Dependency> dependency = *dependencyIt;
                AutoPtr<Node> clonedDependencyNode = nodeCloneMap[dependency->mNode];
                AutoPtr<Dependency> cloneDependency = new Dependency(clonedDependencyNode,
                        dependency->mRule);
                nodeClone->AddDependency(cloneDependency);
            }
        }
    }

    *object = (IInterface*)newObject->Probe(EIID_IInterface);
    REFCOUNT_ADD(*object);
    return NOERROR;
}

ECode AnimatorSet::SortNodes()
{
    if (mNeedsSort) {
        mSortedNodes.Clear();
        List<AutoPtr<Node> > roots;
        List<AutoPtr<Node> >::Iterator nodeIt = mNodes.Begin();
        for (; nodeIt != mNodes.End(); ++nodeIt) {
            AutoPtr<Node> node = (*nodeIt);
            if (node->mDependencies.IsEmpty()) {
                roots.PushBack(node);
            }
        }
        List<AutoPtr<Node> > tmpRoots;
        while (roots.IsEmpty() == FALSE) {
            List<AutoPtr<Node> >::Iterator rootsIt = roots.Begin();
            for (; rootsIt != roots.End(); ++rootsIt) {
                AutoPtr<Node> root = (*rootsIt);
                mSortedNodes.PushBack(root);
                if (root->mNodeDependents.IsEmpty() == FALSE) {
                    List<Node* >::Iterator itDependents = root->mNodeDependents.Begin();
                    for (; itDependents != root->mNodeDependents.End(); ++itDependents) {
                        AutoPtr<Node> node = *itDependents;
                        node->mNodeDependencies.Remove(root);
                        if (node->mNodeDependencies.IsEmpty()) {
                            tmpRoots.PushBack(node);
                        }
                    }
                }
            }
            roots.Clear();
            roots.Insert(roots.End(), tmpRoots.Begin(), tmpRoots.End());
            tmpRoots.Clear();
        }
        mNeedsSort = FALSE;
        if (mSortedNodes.GetSize() != mNodes.GetSize()) {
//            throw new IllegalStateException("Circular dependencies cannot exist"
//                    + " in AnimatorSet");
            return E_ILLEGAL_STATE_EXCEPTION;
        }
    } else {
        // Doesn't need sorting, but still need to add in the nodeDependencies list
        // because these get removed as the event listeners fire and the dependencies
        // are satisfied
        List<AutoPtr<Node> >::Iterator nodeIt = mNodes.Begin();
        for (; nodeIt != mNodes.End(); ++nodeIt) {
            AutoPtr<Node> node = *nodeIt;
            if (node->mDependencies.IsEmpty() == FALSE) {
                List<AutoPtr<Dependency> >::Iterator dependencyIt = node->mDependencies.Begin();
                for (; dependencyIt != node->mDependencies.End(); ++dependencyIt) {
                    AutoPtr<Dependency> dependency = *dependencyIt;
                    List<AutoPtr<Node> >::Iterator itFind =
                            Find(node->mNodeDependencies.Begin(), node->mNodeDependencies.End(), dependency->mNode);
                    if (itFind == node->mNodeDependencies.End()) {
                        node->mNodeDependencies.PushBack(dependency->mNode);
                    }
                }
            }
            // nodes are 'done' by default; they become un-done when started, and done
            // again when ended
            node->mDone = FALSE;
        }
    }
    return NOERROR;
}

ECode AnimatorSet::CanReverse(
    /* [out] */ Boolean* can)
{
    VALIDATE_NOT_NULL(can);
    *can = FALSE;
    if (!mReversible)  {
        return NOERROR;
    }
    // Loop to make sure all the Nodes can reverse.
    List<AutoPtr<Node> >::Iterator it = mNodes.Begin();
    for (; it != mNodes.End(); it++) {
        Boolean tmp = FALSE;
        Int64 delay = 0;
        if (!((*it)->mAnimation->CanReverse(&tmp), tmp) || ((*it)->mAnimation->GetStartDelay(&delay), delay) > 0) {
            return NOERROR;
        }
    }
    *can = TRUE;
    return NOERROR;
}

ECode AnimatorSet::Reverse()
{
    Boolean can = FALSE;
    if (CanReverse(&can), can) {
        List<AutoPtr<Node> >::Iterator it = mNodes.Begin();
        for (; it != mNodes.End(); it++) {
            (*it)->mAnimation->Reverse();
        }
    }
    return NOERROR;
}

}   //namespace Animation
}   //namespace Droid
}   //namespace Elastos
