
#include "view/animation/CLayoutAnimationController.h"

namespace Elastos {
namespace Droid {
namespace View {
namespace Animation {

CAR_OBJECT_IMPL(CLayoutAnimationController);

ECode CLayoutAnimationController::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return LayoutAnimationController::constructor(context, attrs);
}

ECode CLayoutAnimationController::constructor(
    /* [in] */ IAnimation* animation)
{
    return LayoutAnimationController::constructor(animation);
}

ECode CLayoutAnimationController::constructor(
    /* [in] */ IAnimation* animation,
    /* [in] */ Float delay)
{
    return LayoutAnimationController::constructor(animation, delay);
}

}   //namespace Animation
}   //namespace View
}   //namespace Droid
}   //namespace Elastos
