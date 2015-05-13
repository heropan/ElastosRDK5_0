#ifndef  __CTIMEANIMATOR_H__
#define  __CTIMEANIMATOR_H__

#include "_CTimeAnimator.h"
#include "animation/TimeAnimator.h"
#include "animation/AnimatorMacro.h"
#include "animation/ValueAnimatorMacro.h"

namespace Elastos {
namespace Droid {
namespace Animation {

CarClass(CTimeAnimator) , public TimeAnimator
{
public:
    IANIMATOR_METHOD_DECL()
    IVALUEANIMATOR_METHOD_DECL()

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI SetTimeListener(
        /* [in] */ ITimeListener* listener);

};

} // namespace Animation
} // namepsace Droid
} // namespace Elastos

#endif //__CTIMEANIMATOR_H__
