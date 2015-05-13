
#include "view/animation/CAnimationUtils.h"
#include "view/animation/AnimationUtils.h"

namespace Elastos {
namespace Droid {
namespace View {
namespace Animation {

ECode CAnimationUtils::CurrentAnimationTimeMillis(
    /* [out] */ Int64* time)
{
    VALIDATE_NOT_NULL(time);
    *time = AnimationUtils::CurrentAnimationTimeMillis();
    return NOERROR;
}

ECode CAnimationUtils::LoadAnimation(
    /* [in] */ IContext* context,
    /* [in] */ Int32 id,
    /* [out] */ IAnimation** animation)
{
    return AnimationUtils::LoadAnimation(context, id, animation);
}

ECode CAnimationUtils::LoadLayoutAnimation(
    /* [in] */ IContext* context,
    /* [in] */ Int32 id,
    /* [out] */ ILayoutAnimationController** controller)
{
    return AnimationUtils::LoadLayoutAnimation(context, id, controller);
}

ECode CAnimationUtils::MakeInAnimation(
    /* [in] */ IContext* context,
    /* [in] */ Boolean fromLeft,
    /* [out] */ IAnimation** animation)
{
    return AnimationUtils::MakeInAnimation(context, fromLeft, animation);
}

ECode CAnimationUtils::MakeOutAnimation(
    /* [in] */ IContext* context,
    /* [in] */ Boolean toRight,
    /* [out] */ IAnimation** animation)
{
    return AnimationUtils::MakeOutAnimation(context, toRight, animation);
}

ECode CAnimationUtils::MakeInChildBottomAnimation(
    /* [in] */ IContext* context,
    /* [out] */ IAnimation** animation)
{
    return AnimationUtils::MakeInChildBottomAnimation(context, animation);
}

ECode CAnimationUtils::LoadInterpolator(
    /* [in] */ IContext* context,
    /* [in] */ Int32 id,
    /* [out] */ IInterpolator** interpolator)
{
    return AnimationUtils::LoadInterpolator(context, id, interpolator);
}

}   //namespace Animation
}   //namespace View
}   //namespace Droid
}   //namespace Elastos
