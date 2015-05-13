#ifndef __CVIEWPROPERTYANIMATOR_H__
#define __CVIEWPROPERTYANIMATOR_H__

#include "_CViewPropertyAnimator.h"
#include "ViewPropertyAnimator.h"

using Elastos::Droid::Animation::ITimeInterpolator;
using Elastos::Droid::Animation::IAnimatorListener;

namespace Elastos {
namespace Droid {
namespace View {

CarClass(CViewPropertyAnimator) , public ViewPropertyAnimator
{
public:
    CViewPropertyAnimator();

    CARAPI constructor(
        /* [in] */ IView* v);

    CARAPI SetDuration(
        /* [in] */ Int64 duration);

    CARAPI GetDuration(
        /* [out] */ Int64* duration);

    CARAPI SetStartDelay(
        /* [in] */ Int64 delay);

    CARAPI GetStartDelay(
        /* [out] */ Int64* delay);

    CARAPI SetInterpolator(
        /* [in] */ ITimeInterpolator* interpolator);

    CARAPI SetListener(
        /* [in] */ IAnimatorListener* listener);

    CARAPI Start();

    CARAPI Cancel();

    CARAPI X(
        /* [in] */ Float value);

    CARAPI XBy(
        /* [in] */ Float value);

    CARAPI Y(
        /* [in] */ Float value);

    CARAPI YBy(
        /* [in] */ Float value);

    CARAPI Rotation(
        /* [in] */ Float value);

    CARAPI RotationBy(
        /* [in] */ Float value);

    CARAPI RotationX(
        /* [in] */ Float value);

    CARAPI RotationXBy(
        /* [in] */ Float value);

    CARAPI RotationY(
        /* [in] */ Float value);

    CARAPI RotationYBy(
        /* [in] */ Float value);

    CARAPI TranslationX(
        /* [in] */ Float value);

    CARAPI TranslationXBy(
        /* [in] */ Float value);

    CARAPI TranslationY(
        /* [in] */ Float value);

    CARAPI TranslationYBy(
        /* [in] */ Float value);

    CARAPI ScaleX(
        /* [in] */ Float value);

    CARAPI ScaleXBy(
        /* [in] */ Float value);

    CARAPI ScaleY(
        /* [in] */ Float value);

    CARAPI ScaleYBy(
        /* [in] */ Float value);

    CARAPI Alpha(
        /* [in] */ Float value);

    CARAPI AlphaBy(
        /* [in] */ Float value);

    CARAPI WithLayer();

    CARAPI WithStartAction(
        /* [in] */ IRunnable* runnable);

    CARAPI WithEndAction(
        /* [in] */ IRunnable* runnable);
};

}// namespace View
}// namespace Droid
}// namespace Elastos
#endif