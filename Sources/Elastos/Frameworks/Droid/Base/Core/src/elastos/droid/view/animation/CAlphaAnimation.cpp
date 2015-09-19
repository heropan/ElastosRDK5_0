
#include "view/animation/CAlphaAnimation.h"

namespace Elastos {
namespace Droid {
namespace View {
namespace Animation {

CAR_OBJECT_IMPL(CAlphaAnimation);

ECode CAlphaAnimation::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return AlphaAnimation::constructor(context, attrs);
}

ECode CAlphaAnimation::constructor(
    /* [in] */ Float fromAlpha,
    /* [in] */ Float toAlpha)
{
    return AlphaAnimation::constructor(fromAlpha, toAlpha);
}

}// namespace Animation
}// namespace View
}// namespace Droid
}// namespace Elastos
