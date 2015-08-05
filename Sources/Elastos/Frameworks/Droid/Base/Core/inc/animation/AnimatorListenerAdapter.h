
#ifndef  __ELASTOS_DROID_ANIMATION_ANIMATORLISTENERADAPTER_H__
#define  __ELASTOS_DROID_ANIMATION_ANIMATORLISTENERADAPTER_H__

#include "Elastos.Droid.Core_server.h"

namespace Elastos {
namespace Droid {
namespace Animation {

class AnimatorListenerAdapter
    : public Object
    , public IAnimatorListener
{
public:
    CAR_INTERFACE_DECL();

    virtual CARAPI OnAnimationStart(
        /* [in] */ IAnimator* animation);

    virtual CARAPI OnAnimationEnd(
        /* [in] */ IAnimator* animation);

    virtual CARAPI OnAnimationCancel(
        /* [in] */ IAnimator* animation);

    virtual CARAPI OnAnimationRepeat(
        /* [in] */ IAnimator* animation);
};

} // namespace Animation
} // namepsace Droid
} // namespace Elastos

#endif //__ANIMATORLISTENERADAPTER_H__
