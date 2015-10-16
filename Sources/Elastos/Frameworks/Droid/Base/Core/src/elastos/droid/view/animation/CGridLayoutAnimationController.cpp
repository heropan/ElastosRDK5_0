
#include "elastos/droid/view/animation/CGridLayoutAnimationController.h"

namespace Elastos {
namespace Droid {
namespace View {
namespace Animation {

CAR_OBJECT_IMPL(CGridLayoutAnimationController);

ECode CGridLayoutAnimationController::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return GridLayoutAnimationController::constructor(context, attrs);
}

ECode CGridLayoutAnimationController::constructor(
    /* [in] */ IAnimation* animation)
{
    return GridLayoutAnimationController::constructor(animation);
}

ECode CGridLayoutAnimationController::constructor(
    /* [in] */ IAnimation* animation,
    /* [in] */ Float columnDelay,
    /* [in] */ Float rowDelay)
{
    return GridLayoutAnimationController::constructor(animation, columnDelay, rowDelay);
}

}   //namespace Animation
}   //namespace View
}   //namespace Droid
}   //namespace Elastos
