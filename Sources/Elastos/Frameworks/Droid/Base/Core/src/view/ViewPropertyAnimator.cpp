#include "view/ViewPropertyAnimator.h"
#include "animation/CValueAnimator.h"
#include "view/View.h"

using Elastos::Droid::Animation::ValueAnimator;
using Elastos::Droid::Animation::CValueAnimator;
using Elastos::Droid::Animation::IValueAnimator;
using Elastos::Droid::Animation::EIID_IAnimatorListener;
using Elastos::Droid::Animation::EIID_IAnimatorUpdateListener;

namespace Elastos {
namespace Droid {
namespace View {

const Int32 ViewPropertyAnimator::NONE;
const Int32 ViewPropertyAnimator::TRANSLATION_X;
const Int32 ViewPropertyAnimator::TRANSLATION_Y;
const Int32 ViewPropertyAnimator::SCALE_X;
const Int32 ViewPropertyAnimator::SCALE_Y;
const Int32 ViewPropertyAnimator::ROTATION;
const Int32 ViewPropertyAnimator::ROTATION_X;
const Int32 ViewPropertyAnimator::ROTATION_Y;
const Int32 ViewPropertyAnimator::_X;
const Int32 ViewPropertyAnimator::_Y;
const Int32 ViewPropertyAnimator::ALPHA;
const Int32 ViewPropertyAnimator::TRANSFORM_MASK;

/*---------------------------------AnimatorEventListener---------------------------------*/

PInterface ViewPropertyAnimator::AnimatorEventListener::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)(IAnimatorListener*)this;
    } else if (riid == EIID_IAnimatorListener) {
        return (IAnimatorListener*)this;
    } else if (riid == EIID_IAnimatorUpdateListener) {
        return (IAnimatorUpdateListener*)this;
    }
    return NULL;
}

UInt32 ViewPropertyAnimator::AnimatorEventListener::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 ViewPropertyAnimator::AnimatorEventListener::Release()
{
    return ElRefBase::Release();
}

ECode ViewPropertyAnimator::AnimatorEventListener::GetInterfaceID(
    /* [in] */ IInterface *object,
    /* [out] */ InterfaceID *pIID)
{
    if (object == (IAnimatorListener*)this) {
        *pIID = EIID_IAnimatorListener;
    } else if (object == (IAnimatorUpdateListener*)this) {
        *pIID = EIID_IAnimatorUpdateListener;
    } else {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return NOERROR;
}

ViewPropertyAnimator::AnimatorEventListener::AnimatorEventListener(
    /* [in] */ ViewPropertyAnimator* host) : mHost(host)
{}

ECode ViewPropertyAnimator::AnimatorEventListener::OnAnimationStart(
    /* [in] */ IAnimator* animation)
{
    if (mHost->mAnimatorSetupMap != NULL) {
        RNIterator it = mHost->mAnimatorSetupMap->Find(animation);
        if (it != mHost->mAnimatorSetupMap->End()) {
            it->mSecond->Run();
            mHost->mAnimatorSetupMap->Erase(animation);
        }
    }
    if (mHost->mAnimatorOnStartMap != NULL) {
        RNIterator it = mHost->mAnimatorOnStartMap->Find(animation);
        if (it != mHost->mAnimatorOnStartMap->End()) {
            it->mSecond->Run();
            mHost->mAnimatorOnStartMap->Erase(animation);
        }
    }
    if (mHost->mListener != NULL) {
        mHost->mListener->OnAnimationStart(animation);
    }
    return NOERROR;
}

ECode ViewPropertyAnimator::AnimatorEventListener::OnAnimationEnd(
    /* [in] */ IAnimator* animation)
{
    mHost->mView->SetHasTransientState(FALSE);
    if (mHost->mListener != NULL) {
        mHost->mListener->OnAnimationEnd(animation);
    }
    if (mHost->mAnimatorOnEndMap != NULL) {
        RNIterator it = mHost->mAnimatorOnEndMap->Find(animation);
        if (it != mHost->mAnimatorOnEndMap->End()) {
            it->mSecond->Run();
            mHost->mAnimatorOnEndMap->Erase(animation);
        }
    }
    if (mHost->mAnimatorCleanupMap != NULL) {
        RNIterator it = mHost->mAnimatorCleanupMap->Find(animation);
        if (it != mHost->mAnimatorCleanupMap->End()) {
            it->mSecond->Run();
            mHost->mAnimatorCleanupMap->Erase(animation);
        }
    }
    mHost->mAnimatorMap->Erase(animation);
    return NOERROR;
}

ECode ViewPropertyAnimator::AnimatorEventListener::OnAnimationCancel(
    /* [in] */ IAnimator* animation)
{
    if (mHost->mListener != NULL) {
        mHost->mListener->OnAnimationCancel(animation);
    }

    if (mHost->mAnimatorOnEndMap != NULL) {
        if (mHost->mAnimatorOnEndMap->IsEmpty() == FALSE) {
            mHost->mAnimatorOnEndMap->Erase(animation);
        }
    }

    return NOERROR;
}

ECode ViewPropertyAnimator::AnimatorEventListener::OnAnimationRepeat(
    /* [in] */ IAnimator* animation)
{
    if (mHost->mListener != NULL) {
        mHost->mListener->OnAnimationRepeat(animation);
    }
    return NOERROR;
}

ECode ViewPropertyAnimator::AnimatorEventListener::OnAnimationUpdate(
    /* [in] */ IValueAnimator* animation)
{
    AutoPtr<PropertyBundle> propertyBundle;
    PIterator it = mHost->mAnimatorMap->Find(animation);
    if(it != mHost->mAnimatorMap->End())
    {
        propertyBundle = it->mSecond;
    }
    if (propertyBundle == NULL) {
        // Shouldn't happen, but just to play it safe
        return NOERROR;
    }
    AutoPtr<IDisplayList> dl;
    mHost->mView->GetDisplayList((IDisplayList**)&dl);
    Boolean useDisplayListProperties = dl != NULL;

    // alpha requires slightly different treatment than the other (transform) properties.
    // The logic in setAlpha() is not simply setting mAlpha, plus the invalidation
    // logic is dependent on how the view handles an internal call to onSetAlpha().
    // We track what kinds of properties are set, and how alpha is handled when it is
    // set, and perform the invalidation steps appropriately.
    Boolean alphaHandled = FALSE;
    if (!useDisplayListProperties) {
        VIEW_PROBE(mHost->mView)->InvalidateParentCaches();
    }
    Float fraction;
    animation->GetAnimatedFraction(&fraction);
    Int32 propertyMask = propertyBundle->mPropertyMask;
    if ((propertyMask & TRANSFORM_MASK) != 0) {
        VIEW_PROBE(mHost->mView)->InvalidateViewProperty(FALSE, FALSE);
    }
    AutoPtr< List<AutoPtr<NameValuesHolder> > > valueList = propertyBundle->mNameValuesHolder;
    if (valueList != NULL) {
        List<AutoPtr<NameValuesHolder> >::Iterator lsIt = valueList->Begin();
        for (; lsIt != valueList->End(); lsIt++) {
            AutoPtr<NameValuesHolder> values = *lsIt;
            Float value = values->mFromValue + fraction * values->mDeltaValue;
            if (values->mNameConstant == ViewPropertyAnimator::ALPHA) {
                alphaHandled = VIEW_PROBE(mHost->mView)->SetAlphaNoInvalidation(value);
            } else {
                mHost->SetValue(values->mNameConstant, value);
            }
        }
    }
    if ((propertyMask & TRANSFORM_MASK) != 0) {
        VIEW_PROBE(mHost->mView)->mTransformationInfo->mMatrixDirty = TRUE;
        if (!useDisplayListProperties) {
            VIEW_PROBE(mHost->mView)->mPrivateFlags |= View::PFLAG_DRAWN; // force another invalidation
        }
    }
    // invalidate(false) in all cases except if alphaHandled gets set to true
    // via the call to setAlphaNoInvalidation(), above
    if (alphaHandled) {
        VIEW_PROBE(mHost->mView)->Invalidate(TRUE);
    } else {
        VIEW_PROBE(mHost->mView)->InvalidateViewProperty(FALSE, FALSE);
    }
    return NOERROR;
}

/*---------------------------------PropertyBundle---------------------------------*/
ViewPropertyAnimator::PropertyBundle::PropertyBundle(
    /* [in] */ Int32 propertyMask,
    /* [in] */ List<AutoPtr<NameValuesHolder> >* nameValuesHolder)
    : mNameValuesHolder(nameValuesHolder)
{
    mPropertyMask = propertyMask;
}

ViewPropertyAnimator::PropertyBundle::~PropertyBundle()
{
}

Boolean ViewPropertyAnimator::PropertyBundle::Cancel(
    /* [in] */ Int32 propertyConstant)
{
    if ((mPropertyMask & propertyConstant) != 0 && mNameValuesHolder != NULL) {
        List<AutoPtr<NameValuesHolder> >::Iterator lsIt = mNameValuesHolder->Begin();
        for (; lsIt != mNameValuesHolder->End(); lsIt++) {
            AutoPtr<NameValuesHolder> nameValuesHolder = *lsIt;
            if (nameValuesHolder->mNameConstant == propertyConstant) {
                mNameValuesHolder->Erase(lsIt);
                mPropertyMask &= ~propertyConstant;
                return TRUE;
            }
        }
    }
    return FALSE;
}

/*---------------------------------NameValuesHolder---------------------------------*/

ViewPropertyAnimator::NameValuesHolder::NameValuesHolder(
    /* [in] */ Int32 nameConstant,
    /* [in] */ Float fromeValue,
    /* [in] */ Float deltaValue)
    : mNameConstant(nameConstant)
    , mFromValue(fromeValue)
    , mDeltaValue(deltaValue)
{}

/*---------------------------------InnerRunnable---------------------------------*/

ViewPropertyAnimator::InnerRunnable::InnerRunnable(
    /* [in] */ ViewPropertyAnimator* host)
    : mHost(host)
{}

ECode ViewPropertyAnimator::InnerRunnable::Run()
{
    mHost->StartAnimation();
    return NOERROR;
}

/*---------------------------------InnerRunnableEx---------------------------------*/
ViewPropertyAnimator::InnerRunnableEx::InnerRunnableEx(
    /* [in] */ ViewPropertyAnimator* host,
    /* [in] */ Int32 type)
    : mHost(host)
    , mType(type)
{}

ECode ViewPropertyAnimator::InnerRunnableEx::Run()
{
    mHost->mView->SetLayerType(mType, NULL);
    return NOERROR;
}

/*---------------------------------ViewPropertyAnimator---------------------------------*/

ViewPropertyAnimator::ViewPropertyAnimator(
    /* [in] */ IView* view)
    : mView(view)
    , mDuration(0)
    , mDurationSet(FALSE)
    , mStartDelay(0)
    , mStartDelaySet(FALSE)
    , mInterpolatorSet(FALSE)
{
    mPendingAnimations = new List<AutoPtr<NameValuesHolder> >();
    mAnimatorMap = new HashMap<AutoPtr<IAnimator>, AutoPtr<PropertyBundle> >();
    mAnimatorEventListener = new AnimatorEventListener(this);
    mAnimationStarter = new InnerRunnable(this);
    VIEW_PROBE(view)->EnsureTransformationInfo();
}

ViewPropertyAnimator::ViewPropertyAnimator()
    : mDuration(0)
    , mDurationSet(FALSE)
    , mStartDelay(0)
    , mStartDelaySet(FALSE)
    , mInterpolatorSet(FALSE)
{
    mPendingAnimations = new List<AutoPtr<NameValuesHolder> >();
    mAnimatorMap = new HashMap<AutoPtr<IAnimator>, AutoPtr<PropertyBundle> >();
    mAnimatorEventListener = new AnimatorEventListener(this);
    mAnimationStarter = new InnerRunnable(this);
}

ECode ViewPropertyAnimator::Init(
    /* [in] */ IView* v)
{
    mView = v;
    VIEW_PROBE(v)->EnsureTransformationInfo();
    return NOERROR;
}

ViewPropertyAnimator::~ViewPropertyAnimator()
{
    mPendingAnimations = NULL;
    mAnimatorMap = NULL;
    mAnimatorSetupMap = NULL;
    mAnimatorCleanupMap = NULL;
    mAnimatorOnStartMap = NULL;
    mAnimatorOnEndMap = NULL;
}

ECode ViewPropertyAnimator::SetDuration(
    /* [in] */ Int64 duration)
{
    if (duration < 0) {
        // throw new IllegalArgumentException("Animators cannot have negative duration: " +
        //         duration);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    mDurationSet = TRUE;
    mDuration = duration;
    return NOERROR;
}

Int64 ViewPropertyAnimator::GetDuration()
{
    if (mDurationSet) {
        return mDuration;
    } else {
        // Just return the default from ValueAnimator, since that's what we'd get if
        // the value has not been set otherwise
        AutoPtr<IValueAnimator> va;
        CValueAnimator::New((IValueAnimator**)&va);
        Int64 duration;
        va->GetDuration(&duration);
        return duration;
    }
}

Int64 ViewPropertyAnimator::GetStartDelay()
{
    if (mStartDelaySet) {
        return mStartDelay;
    } else {
        // Just return the default from ValueAnimator (0), since that's what we'd get if
        // the value has not been set otherwise
        return 0;
    }
}

ECode ViewPropertyAnimator::SetStartDelay(
    /* [in] */ Int64 startDelay)
{
    if (startDelay < 0) {
        // throw new IllegalArgumentException("Animators cannot have negative duration: " +
        //         startDelay);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    mStartDelaySet = TRUE;
    mStartDelay = startDelay;
    return NOERROR;
}

ECode ViewPropertyAnimator::SetInterpolator(
    /* [in] */ ITimeInterpolator* interpolator)
{
    mInterpolatorSet = TRUE;
    mInterpolator = interpolator;
    return NOERROR;
}

ECode ViewPropertyAnimator::SetListener(
    /* [in] */ IAnimatorListener* listener)
{
    mListener = listener;
    return NOERROR;
}

ECode ViewPropertyAnimator::Start()
{
    VIEW_PROBE(mView)->RemoveCallbacks(mAnimationStarter);
    StartAnimation();
    return NOERROR;
}

ECode ViewPropertyAnimator::Cancel()
{
    if (mAnimatorMap->IsEmpty() == FALSE) {
        HashMap<AutoPtr<IAnimator>, AutoPtr<PropertyBundle> > mAnimatorMapCopy(mAnimatorMap->Begin(), mAnimatorMap->End());
        PIterator it = mAnimatorMapCopy.Begin();
        for (; it != mAnimatorMapCopy.End(); it++) {
            it->mFirst->Cancel();
        }
    }
    mPendingAnimations->Clear();
    VIEW_PROBE(mView)->RemoveCallbacks(mAnimationStarter);
    return NOERROR;
}

ECode ViewPropertyAnimator::X(
    /* [in] */ Float value)
{
    AnimateProperty(_X, value);
    return NOERROR;
}

ECode ViewPropertyAnimator::XBy(
    /* [in] */ Float value)
{
    AnimatePropertyBy(_X, value);
    return NOERROR;
}

ECode ViewPropertyAnimator::Y(
    /* [in] */ Float value)
{
    AnimateProperty(_Y, value);
    return NOERROR;
}

ECode ViewPropertyAnimator::YBy(
    /* [in] */ Float value)
{
    AnimatePropertyBy(_Y, value);
    return NOERROR;
}

ECode ViewPropertyAnimator::Rotation(
    /* [in] */ Float value)
{
    AnimateProperty(ROTATION, value);
    return NOERROR;
}

ECode ViewPropertyAnimator::RotationBy(
    /* [in] */ Float value)
{
    AnimatePropertyBy(ROTATION, value);
    return NOERROR;
}

ECode ViewPropertyAnimator::RotationX(
    /* [in] */ Float value)
{
    AnimateProperty(ROTATION_X, value);
    return NOERROR;
}

ECode ViewPropertyAnimator::RotationXBy(
    /* [in] */ Float value)
{
    AnimatePropertyBy(ROTATION_X, value);
    return NOERROR;
}

ECode ViewPropertyAnimator::RotationY(
    /* [in] */ Float value)
{
    AnimateProperty(ROTATION_Y, value);
    return NOERROR;
}

ECode ViewPropertyAnimator::RotationYBy(
    /* [in] */ Float value)
{
    AnimatePropertyBy(ROTATION_Y, value);
    return NOERROR;
}

ECode ViewPropertyAnimator::TranslationX(
    /* [in] */ Float value)
{
    AnimateProperty(TRANSLATION_X, value);
    return NOERROR;
}

ECode ViewPropertyAnimator::TranslationXBy(
    /* [in] */ Float value)
{
    AnimatePropertyBy(TRANSLATION_X, value);
    return NOERROR;
}


ECode ViewPropertyAnimator::TranslationY(
    /* [in] */ Float value)
{
    AnimateProperty(TRANSLATION_Y, value);
    return NOERROR;
}

ECode ViewPropertyAnimator::TranslationYBy(
    /* [in] */ Float value)
{
    AnimatePropertyBy(TRANSLATION_Y, value);
    return NOERROR;
}

ECode ViewPropertyAnimator::ScaleX(
    /* [in] */ Float value)
{
    AnimateProperty(SCALE_X, value);
    return NOERROR;
}

ECode ViewPropertyAnimator::ScaleXBy(
    /* [in] */ Float value)
{
    AnimatePropertyBy(SCALE_X, value);
    return NOERROR;
}

ECode ViewPropertyAnimator::ScaleY(
    /* [in] */ Float value)
{
    AnimateProperty(SCALE_Y, value);
    return NOERROR;
}

ECode ViewPropertyAnimator::ScaleYBy(
    /* [in] */ Float value)
{
    AnimatePropertyBy(SCALE_Y, value);
    return NOERROR;
}

ECode ViewPropertyAnimator::Alpha(
    /* [in] */ Float value)
{
    AnimateProperty(ALPHA, value);
    return NOERROR;
}

ECode ViewPropertyAnimator::AlphaBy(
    /* [in] */ Float value)
{
    AnimatePropertyBy(ALPHA, value);
    return NOERROR;
}

ECode ViewPropertyAnimator::WithLayer()
{
    mPendingSetupAction = new InnerRunnableEx(this, IView::LAYER_TYPE_HARDWARE);
    Int32 currentLayerType;
    mView->GetLayerType(&currentLayerType);
    mPendingCleanupAction = new InnerRunnableEx(this, currentLayerType);
    if (mAnimatorSetupMap == NULL) {
        mAnimatorSetupMap = new HashMap<AutoPtr<IAnimator>, AutoPtr<IRunnable> >();
    }
    if (mAnimatorCleanupMap == NULL) {
        mAnimatorCleanupMap = new HashMap<AutoPtr<IAnimator>, AutoPtr<IRunnable> >();
    }
    return NOERROR;
}

ECode ViewPropertyAnimator::WithStartAction(
    /* [in] */ IRunnable* runnable)
{
    mPendingOnStartAction = runnable;
    if (runnable != NULL && mAnimatorOnStartMap == NULL) {
        mAnimatorOnStartMap = new HashMap<AutoPtr<IAnimator>, AutoPtr<IRunnable> >();
    }
    return NOERROR;
}

ECode ViewPropertyAnimator::WithEndAction(
    /* [in] */ IRunnable* runnable)
{
    mPendingOnEndAction = runnable;
    if (runnable != NULL && mAnimatorOnEndMap == NULL) {
        mAnimatorOnEndMap = new HashMap<AutoPtr<IAnimator>, AutoPtr<IRunnable> >();
    }
    return NOERROR;
}

ECode ViewPropertyAnimator::StartAnimation()
{
    mView->SetHasTransientState(TRUE);
    AutoPtr<ArrayOf<Float> > array = ArrayOf<Float>::Alloc(1);
    (*array)[0] = 1.0f;
    AutoPtr<IValueAnimator> animator = ValueAnimator::OfFloat(array);
    AutoPtr<List<AutoPtr<NameValuesHolder> > > nameValueList = new List<AutoPtr<NameValuesHolder> >(*mPendingAnimations);
    mPendingAnimations->Clear();
    Int32 propertyMask = 0;
    List<AutoPtr<NameValuesHolder> >::Iterator it = nameValueList->Begin();
    for (; it != nameValueList->End(); it++) {
        AutoPtr<NameValuesHolder> nameValuesHolder = *it;
        propertyMask |= nameValuesHolder->mNameConstant;
    }
    AutoPtr<PropertyBundle> pb = new PropertyBundle(propertyMask, nameValueList);
    (*mAnimatorMap)[animator] = pb;
    if (mPendingSetupAction != NULL) {
        (*mAnimatorSetupMap)[animator] = mPendingSetupAction;
        mPendingSetupAction = NULL;
    }
    if (mPendingCleanupAction != NULL) {
        (*mAnimatorCleanupMap)[animator] = mPendingCleanupAction;
        mPendingCleanupAction = NULL;
    }
    if (mPendingOnStartAction != NULL) {
        (*mAnimatorOnStartMap)[animator] = mPendingOnStartAction;
        mPendingOnStartAction = NULL;
    }
    if (mPendingOnEndAction != NULL) {
        (*mAnimatorOnEndMap)[animator] = mPendingOnEndAction;
        mPendingOnEndAction = NULL;
    }
    animator->AddUpdateListener(mAnimatorEventListener);
    animator->AddListener(mAnimatorEventListener);
    if (mStartDelaySet) {
        animator->SetStartDelay(mStartDelay);
    }
    if (mDurationSet) {
        animator->SetDuration(mDuration);
    }
    if (mInterpolatorSet) {
        animator->SetInterpolator(mInterpolator);
    }
    animator->Start();
    return NOERROR;
}

ECode ViewPropertyAnimator::AnimateProperty(
    /* [in] */ Int32 constantName,
    /* [in] */ Float toValue)
{
    Float fromValue = GetValue(constantName);
    Float deltaValue = toValue - fromValue;
    AnimatePropertyBy(constantName, fromValue, deltaValue);
    return NOERROR;
}

ECode ViewPropertyAnimator::AnimatePropertyBy(
    /* [in] */ Int32 constantName,
    /* [in] */ Float byValue)
{
    Float fromValue = GetValue(constantName);
    AnimatePropertyBy(constantName, fromValue, byValue);
    return NOERROR;
}

ECode ViewPropertyAnimator::AnimatePropertyBy(
    /* [in] */ Int32 constantName,
    /* [in] */ Float startValue,
    /* [in] */ Float byValue)
{
    // First, cancel any existing animations on this property
    if (mAnimatorMap->IsEmpty() == FALSE) {
        AutoPtr<IAnimator> animatorToCancel = NULL;
        PIterator it = mAnimatorMap->Begin();
        for (; it != mAnimatorMap->End(); it++) {
            AutoPtr<PropertyBundle> bundle = it->mSecond;
            if (bundle->Cancel(constantName)) {
                // property was canceled - cancel the animation if it's now empty
                // Note that it's safe to break out here because every new animation
                // on a property will cancel a previous animation on that property, so
                // there can only ever be one such animation running.
                if (bundle->mPropertyMask == NONE) {
                    // the animation is no longer changing anything - cancel it
                    animatorToCancel = it->mFirst;
                    break;
                }
            }
        }
        if (animatorToCancel != NULL) {
            animatorToCancel->Cancel();
        }
    }

    AutoPtr<NameValuesHolder> nameValuePair = new NameValuesHolder(constantName, startValue, byValue);
    mPendingAnimations->PushBack(nameValuePair);
    VIEW_PROBE(mView)->RemoveCallbacks(mAnimationStarter);
    VIEW_PROBE(mView)->Post(mAnimationStarter);
    return NOERROR;
}

ECode ViewPropertyAnimator::SetValue(
    /* [in] */ Int32 propertyConstant,
    /* [in] */ Float value)
{
    AutoPtr<View::TransformationInfo> info = VIEW_PROBE(mView)->mTransformationInfo;
    AutoPtr<IDisplayList> displayList = VIEW_PROBE(mView)->mDisplayList;
    switch (propertyConstant) {
        case TRANSLATION_X:
            info->mTranslationX = value;
            if (displayList != NULL) displayList->SetTranslationX(value);
            break;
        case TRANSLATION_Y:
            info->mTranslationY = value;
            if (displayList != NULL) displayList->SetTranslationY(value);
            break;
        case ROTATION:
            info->mRotation = value;
            if (displayList != NULL) displayList->SetRotation(value);
            break;
        case ROTATION_X:
            info->mRotationX = value;
            if (displayList != NULL) displayList->SetRotationX(value);
            break;
        case ROTATION_Y:
            info->mRotationY = value;
            if (displayList != NULL) displayList->SetRotationY(value);
            break;
        case SCALE_X:
            info->mScaleX = value;
            if (displayList != NULL) displayList->SetScaleX(value);
            break;
        case SCALE_Y:
            info->mScaleY = value;
            if (displayList != NULL) displayList->SetScaleY(value);
            break;
        case _X:
            info->mTranslationX = value - VIEW_PROBE(mView)->mLeft;
            if (displayList != NULL) displayList->SetTranslationX(value - VIEW_PROBE(mView)->mLeft);
            break;
        case _Y:
            info->mTranslationY = value - VIEW_PROBE(mView)->mTop;
            if (displayList != NULL) displayList->SetTranslationY(value - VIEW_PROBE(mView)->mTop);
            break;
        case ALPHA:
            info->mAlpha = value;
            if (displayList != NULL) displayList->SetAlpha(value);
            break;
    }
    return NOERROR;
}

Float ViewPropertyAnimator::GetValue(
    /* [in] */ Int32 propertyConstant)
{
    AutoPtr<View::TransformationInfo> info = VIEW_PROBE(mView)->mTransformationInfo;
    switch (propertyConstant) {
        case TRANSLATION_X:
            return info->mTranslationX;
        case TRANSLATION_Y:
            return info->mTranslationY;
        case ROTATION:
            return info->mRotation;
        case ROTATION_X:
            return info->mRotationX;
        case ROTATION_Y:
            return info->mRotationY;
        case SCALE_X:
            return info->mScaleX;
        case SCALE_Y:
            return info->mScaleY;
        case _X:
            return VIEW_PROBE(mView)->mLeft + info->mTranslationX;
        case _Y:
            return VIEW_PROBE(mView)->mTop + info->mTranslationY;
        case ALPHA:
            return info->mAlpha;
    }
    return 0;
}

}// namespace View
}// namespace Droid
}// namespace Elastos
