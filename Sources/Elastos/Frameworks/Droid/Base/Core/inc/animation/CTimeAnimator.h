
#ifndef  __ELASTOS_DROID_ANIMATION_CTIMEANIMATOR_H__
#define  __ELASTOS_DROID_ANIMATION_CTIMEANIMATOR_H__

#include "_CTimeAnimator.h"
#include "animation/TimeAnimator.h"

namespace Elastos {
namespace Droid {
namespace Animation {

CarClass(CTimeAnimator) , public TimeAnimator
{
public:
    CAR_OBJECT_DECL();

    CAR_INTERFACE_DECL();
};

} // namespace Animation
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_ANIMATION_CTIMEANIMATOR_H__
