
#ifndef __ELASTOS_DROID_VIEW_ANIMATION_ALPHAANIMATION_H__
#define __ELASTOS_DROID_VIEW_ANIMATION_ALPHAANIMATION_H__

#include "view/animation/Animation.h"

namespace Elastos {
namespace Droid {
namespace View {
namespace Animation {

class AlphaAnimation : public Animation
{
public:
    AlphaAnimation(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    AlphaAnimation(
        /* [in] */ Float fromAlpha,
        /* [in] */ Float toAlpha);

    CARAPI_(Boolean) WillChangeTransformationMatrix();

    CARAPI_(Boolean) WillChangeBounds();

    CARAPI_(Boolean) HasAlpha();

protected:
    AlphaAnimation();

    CARAPI_(void) ApplyTransformation(
        /* [in] */ Float interpolatedTime,
        /* [in] */ ITransformation* t);

    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI Init(
        /* [in] */ Float fromAlpha,
        /* [in] */ Float toAlpha);

    virtual CARAPI_(AutoPtr<IAnimation>) GetCloneInstance();

    CARAPI_(AutoPtr<IAnimation>) Clone();
private:
    Float mFromAlpha;
    Float mToAlpha;
};

}// namespace Animation
}// namespace View
}// namespace Droid
}// namespace Elastos

#endif //__ELASTOS_DROID_VIEW_ANIMATION_ALPHAANIMATION_H__
