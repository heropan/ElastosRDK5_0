
#include "elastos/droid/view/animation/LayoutAnimationController.h"
#include "elastos/droid/view/animation/AnimationUtils.h"
#include "elastos/droid/view/animation/Animation.h"
#include "elastos/droid/view/animation/CLinearInterpolator.h"
#include "elastos/droid/view/CViewGroupLayoutParams.h"
#include <elastos/core/Math.h>
#include <stdlib.h>

using namespace Elastos::Core;

namespace Elastos {
namespace Droid {
namespace View {
namespace Animation {

CAR_INTERFACE_IMPL(LayoutAnimationController::AnimationParameters, Object, IAnimationParameters);
LayoutAnimationController::AnimationParameters::AnimationParameters()
        : mCount(0), mIndex(0)
{}

ECode LayoutAnimationController::AnimationParameters::GetCount(
    /* [out] */ Int32* count)
{
    *count = mCount;
    return NOERROR;
}

ECode LayoutAnimationController::AnimationParameters::SetCount(
    /* [in] */ Int32 count)
{
    mCount = count;
    return NOERROR;
}

ECode LayoutAnimationController::AnimationParameters::GetIndex(
    /* [out] */ Int32* index)
{
    *index = mIndex;
    return NOERROR;
}

ECode LayoutAnimationController::AnimationParameters::SetIndex(
    /* [in] */ Int32 index)
{
    mIndex = index;
    return NOERROR;
}

CAR_INTERFACE_IMPL(LayoutAnimationController, Object, ILayoutAnimationController);
LayoutAnimationController::LayoutAnimationController()
    : mDelay(0.f)
    , mOrder(0.f)
    , mDuration(0L)
    , mMaxDelay(0L)
{}

LayoutAnimationController::LayoutAnimationController(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    constructor(context, attrs);
}

LayoutAnimationController::LayoutAnimationController(
    /* [in] */ IAnimation* animation,
    /* [in] */ Float delay)
{
    constructor(animation, delay);
}

ECode LayoutAnimationController::GetOrder(
    /* [out] */ Int32* order)
{
    VALIDATE_NOT_NULL(order);
    *order = mOrder;
    return NOERROR;
}

ECode LayoutAnimationController::SetOrder(
    /* [in] */ Int32 order)
{
    mOrder = order;
    return NOERROR;
}

ECode LayoutAnimationController::SetAnimation(
    /* [in] */ IContext* context,
    /* [in] */ Int32 resourceID)
{
    AutoPtr<IAnimation> animation;
    FAIL_RETURN(AnimationUtils::LoadAnimation(
            context, resourceID, (IAnimation**)&animation));
    return SetAnimation(animation);
}

ECode LayoutAnimationController::SetAnimation(
    /* [in] */ IAnimation* animation)
{
    mAnimation = animation;
    mAnimation->SetFillBefore(TRUE);

    return NOERROR;
}

ECode LayoutAnimationController::GetAnimation(
    /* [out] */ IAnimation** animation)
{
    VALIDATE_NOT_NULL(animation);
    *animation = mAnimation;
    REFCOUNT_ADD(*animation);
    return NOERROR;
}

ECode LayoutAnimationController::SetInterpolator(
    /* [in] */ IContext* context,
    /* [in] */ Int32 resourceID)
{
    AutoPtr<IInterpolator> interpolator;
    FAIL_RETURN(AnimationUtils::LoadInterpolator(
            context, resourceID, (IInterpolator**)&interpolator));
    return SetInterpolator(interpolator);
}

ECode LayoutAnimationController::SetInterpolator(
    /* [in] */ IInterpolator* interpolator)
{
    mInterpolator = interpolator;
    return NOERROR;
}

ECode LayoutAnimationController::GetInterpolator(
    /* [out] */ IInterpolator** interpolator)
{
    VALIDATE_NOT_NULL(interpolator);
    *interpolator = mInterpolator;
    REFCOUNT_ADD(*interpolator);
    return NOERROR;
}

ECode LayoutAnimationController::GetDelay(
    /* [out] */ Float* delay)
{
    VALIDATE_NOT_NULL(delay);
    *delay = mDelay;
    return NOERROR;
}

ECode LayoutAnimationController::SetDelay(
    /* [in] */ Float delay)
{
    mDelay = delay;
    return NOERROR;
}

ECode LayoutAnimationController::WillOverlap(
    /* [out] */ Boolean* willOverlap)
{
    VALIDATE_NOT_NULL(willOverlap);
    *willOverlap = mDelay < 1.0f;
    return NOERROR;
}

ECode LayoutAnimationController::Start()
{
    mAnimation->GetDuration(&mDuration);
    mMaxDelay = Elastos::Core::Math::INT64_MIN_VALUE;
    mAnimation->SetStartTime(-1);

    return NOERROR;
}

ECode LayoutAnimationController::GetAnimationForView(
    /* [in] */ IView* view,
    /* [out] */ IAnimation** animation)
{
    VALIDATE_NOT_NULL(animation);
    Int64 startOffset;
    mAnimation->GetStartOffset(&startOffset);
    Int64 delay = GetDelayForView(view) + startOffset;
    mMaxDelay = Elastos::Core::Math::Max(mMaxDelay, delay);

    mAnimation->Clone((IInterface**)animation);
    animation->SetStartOffset(delay);
    return NOERROR;
}

ECode LayoutAnimationController::IsDone(
    /* [out] */ Boolean* done)
{
    VALIDATE_NOT_NULL(done);
    Int64 startTime;
    mAnimation->GetStartTime(&startTime);
    *done AnimationUtils::CurrentAnimationTimeMillis() >
        startTime + mMaxDelay + mDuration;
    return NOERROR;
}

Int64 LayoutAnimationController::GetDelayForView(
    /* [in] */ IView* view)
{
    AutoPtr<IViewGroupLayoutParams> lp;
    view->GetLayoutParams((IViewGroupLayoutParams**)&lp);
    AutoPtr<IAnimationParameters> basic;
    lp->GetLayoutAnimationParameters((IAnimationParameters**)&basic);
    AutoPtr<AnimationParameters> params =
        (AnimationParameters*)IAnimationParameters::Probe(basic);
    if (params == NULL) {
        return 0;
    }

    Int64 duration;
    mAnimation->GetDuration(&duration);
    Float delay = mDelay * duration;
    Int64 viewDelay = (Int64)(GetTransformedIndex(params) * delay);
    Float totalDelay = delay * params->mCount;

    if (mInterpolator == NULL) {
        ASSERT_SUCCEEDED(CLinearInterpolator::New((ILinearInterpolator**)&mInterpolator));
    }

    Float normalizedDelay = viewDelay / totalDelay;
    mInterpolator->GetInterpolation(normalizedDelay, &normalizedDelay);

    return (Int64)(normalizedDelay * totalDelay);
}

Float LayoutAnimationController::GetRandomFloat()
{
    srand((Int32)time(0));
    Int32 a = rand();
    srand((Int32)time(0)/Elastos::Core::Math::Abs(a));
    Int32 b = rand();
    Float result = Elastos::Core::Math::Abs(a) / Elastos::Core::Math::Abs(b);
    if(result > 1)
        result = (Float)(1/result);
    if(result != result)
        return GetRandomFloat();
    return result;
}

Int32 LayoutAnimationController::GetTransformedIndex(
    /* [in] */ AnimationParameters* params)
{
    switch (GetOrder()) {
        case ILayoutAnimationController::ORDER_REVERSE:
            return params->mCount - 1 - params->mIndex;
        case ILayoutAnimationController::ORDER_RANDOM:
            return (Int32) (params->mCount * GetRandomFloat());
        case ILayoutAnimationController::ORDER_NORMAL:
        default:
            return params->mIndex;
    }
}

ECode LayoutAnimationController::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    AutoPtr<ArrayOf<Int32> > attrIds = ArrayOf<Int32>::Alloc(
            const_cast<Int32*>(R::styleable::LayoutAnimation),
            ARRAY_SIZE(R::styleable::LayoutAnimation));
    AutoPtr<ITypedArray> a;
    context->ObtainStyledAttributes(attrs, attrIds, (ITypedArray**)&a);

    AutoPtr<ITypedValue> value;
    a->PeekValue(R::styleable::LayoutAnimation_delay, (ITypedValue**)&value);

    AutoPtr<Animation::Description> d = Animation::Description::ParseValue(value);
    mDelay = d->mValue;

    a->GetInt32(R::styleable::LayoutAnimation_animationOrder,
        ILayoutAnimationController::ORDER_NORMAL, &mOrder);

    Int32 resource;
    a->GetResourceId(R::styleable::LayoutAnimation_animation, 0, &resource);
    if (resource > 0) {
        SetAnimation(context, resource);
    }

    a->GetResourceId(R::styleable::LayoutAnimation_interpolator, 0, &resource);
    if (resource > 0) {
        SetInterpolator(context, resource);
    }

    a->Recycle();

    return NOERROR;
}

ECode LayoutAnimationController::constructor(
    /* [in] */ IAnimation* animation,
    /* [in] */ Float delay)
{
    mDelay = delay;
    SetAnimation(animation);

    return NOERROR;
}

}   //namespace Animation
}   //namespace View
}   //namespace Droid
}   //namespace Elastos
