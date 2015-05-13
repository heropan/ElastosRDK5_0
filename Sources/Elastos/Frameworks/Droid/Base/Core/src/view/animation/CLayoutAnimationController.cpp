
#include "view/animation/CLayoutAnimationController.h"

namespace Elastos {
namespace Droid {
namespace View {
namespace Animation {

ECode CLayoutAnimationController::GetOrder(
    /* [out] */ Int32* order)
{
    VALIDATE_NOT_NULL(order);
    *order = LayoutAnimationController::GetOrder();

    return NOERROR;
}

ECode CLayoutAnimationController::SetOrder(
    /* [in] */ Int32 order)
{
    return LayoutAnimationController::SetOrder(order);
}

ECode CLayoutAnimationController::SetAnimation(
    /* [in] */ IContext* context,
    /* [in] */ Int32 resourceID)
{
    return LayoutAnimationController::SetAnimation(context, resourceID);
}

ECode CLayoutAnimationController::SetAnimationEx(
    /* [in] */ IAnimation* animation)
{
    return LayoutAnimationController::SetAnimation(animation);
}

ECode CLayoutAnimationController::GetAnimation(
    /* [out] */ IAnimation** animation)
{
    VALIDATE_NOT_NULL(animation);
    AutoPtr<IAnimation> temp = LayoutAnimationController::GetAnimation();
    *animation = temp;
    INTERFACE_ADDREF(*animation);

    return NOERROR;
}

ECode CLayoutAnimationController::SetInterpolator(
    /* [in] */ IContext* context,
    /* [in] */ Int32 resourceID)
{
    return LayoutAnimationController::SetInterpolator(context, resourceID);
}

ECode CLayoutAnimationController::SetInterpolatorEx(
    /* [in] */ IInterpolator* interpolator)
{
    return LayoutAnimationController::SetInterpolator(interpolator);
}

ECode CLayoutAnimationController::GetInterpolator(
    /* [out] */ IInterpolator** interpolator)
{
    VALIDATE_NOT_NULL(interpolator);
    AutoPtr<IInterpolator> temp = LayoutAnimationController::GetInterpolator();
    *interpolator = temp;
    INTERFACE_ADDREF(*interpolator);

    return NOERROR;
}

ECode CLayoutAnimationController::GetDelay(
    /* [out] */ Float* delay)
{
    VALIDATE_NOT_NULL(delay);
    *delay = LayoutAnimationController::GetDelay();

    return NOERROR;
}

ECode CLayoutAnimationController::SetDelay(
    /* [in] */ Float delay)
{
    return LayoutAnimationController::SetDelay(delay);
}

ECode CLayoutAnimationController::WillOverlap(
    /* [out] */ Boolean* willOverlap)
{
    VALIDATE_NOT_NULL(willOverlap);
    *willOverlap = LayoutAnimationController::WillOverlap();

    return NOERROR;
}

ECode CLayoutAnimationController::Start()
{
    return LayoutAnimationController::Start();
}

ECode CLayoutAnimationController::GetAnimationForView(
    /* [in] */ IView* view,
    /* [out] */ IAnimation** animation)
{
    VALIDATE_NOT_NULL(animation);
    AutoPtr<IAnimation> temp = LayoutAnimationController::GetAnimationForView(view);
    *animation = temp;
    INTERFACE_ADDREF(*animation);

    return NOERROR;
}

ECode CLayoutAnimationController::IsDone(
    /* [out] */ Boolean* isDone)
{
    VALIDATE_NOT_NULL(isDone);
    *isDone = LayoutAnimationController::IsDone();

    return NOERROR;
}

ECode CLayoutAnimationController::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return LayoutAnimationController::Init(context, attrs);
}

ECode CLayoutAnimationController::constructor(
    /* [in] */ IAnimation* animation)
{
    return LayoutAnimationController::Init(animation);
}

ECode CLayoutAnimationController::constructor(
    /* [in] */ IAnimation* animation,
    /* [in] */ Float delay)
{
    return LayoutAnimationController::Init(animation, delay);
}

}   //namespace Animation
}   //namespace View
}   //namespace Droid
}   //namespace Elastos
