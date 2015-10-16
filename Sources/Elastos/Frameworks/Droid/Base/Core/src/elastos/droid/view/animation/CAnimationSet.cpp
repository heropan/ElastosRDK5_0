
#include "elastos/droid/view/animation/CAnimationSet.h"

namespace Elastos {
namespace Droid {
namespace View {
namespace Animation {

CAR_OBJECT_IMPL(CAnimationSet);

ECode CAnimationSet::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return AnimationSet::constructor(context, attrs);
}

ECode CAnimationSet::constructor(
    /* [in] */ Boolean shareInterpolator)
{
    return AnimationSet::constructor(shareInterpolator);
}

}// namespace Animation
}// namespace View
}// namespace Droid
}// namespace Elastos
