
#ifndef __ELASTOS_DROID_VIEW_ANIMATION_SCALEANIMATION_H__
#define __ELASTOS_DROID_VIEW_ANIMATION_SCALEANIMATION_H__

#include "view/animation/Animation.h"

using Elastos::Droid::Content::Res::IResources;
using Elastos::Droid::Utility::IDisplayMetrics;

namespace Elastos {
namespace Droid {
namespace View {
namespace Animation {

class ScaleAnimation : public Animation
{
public:
    ScaleAnimation(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    ScaleAnimation(
        /* [in] */ Float fromX,
        /* [in] */ Float toX,
        /* [in] */ Float fromY,
        /* [in] */ Float toY);

    ScaleAnimation(
        /* [in] */ Float fromX,
        /* [in] */ Float toX,
        /* [in] */ Float fromY,
        /* [in] */ Float toY,
        /* [in] */ Float pivotX,
        /* [in] */ Float pivotY);

    ScaleAnimation(
        /* [in] */ Float fromX,
        /* [in] */ Float toX,
        /* [in] */ Float fromY,
        /* [in] */ Float toY,
        /* [in] */ Float pivotXType,
        /* [in] */ Float pivotXValue,
        /* [in] */ Float pivotYType,
        /* [in] */ Float pivotYValue);

    //@Override
    CARAPI Initialize(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ Int32 parentWidth,
        /* [in] */ Int32 parentHeight);

protected:
    ScaleAnimation();

    //@Override
    CARAPI_(void) ApplyTransformation(
        /* [in] */ Float interpolatedTime,
        /* [in] */ ITransformation* t);

    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI Init(
        /* [in] */ Float fromX,
        /* [in] */ Float toX,
        /* [in] */ Float fromY,
        /* [in] */ Float toY);

    CARAPI Init(
        /* [in] */ Float fromX,
        /* [in] */ Float toX,
        /* [in] */ Float fromY,
        /* [in] */ Float toY,
        /* [in] */ Float pivotX,
        /* [in] */ Float pivotY);

    CARAPI Init(
        /* [in] */ Float fromX,
        /* [in] */ Float toX,
        /* [in] */ Float fromY,
        /* [in] */ Float toY,
        /* [in] */ Float pivotXType,
        /* [in] */ Float pivotXValue,
        /* [in] */ Float pivotYType,
        /* [in] */ Float pivotYValue);

    CARAPI_(Float) ResolveScale(
        /* [in] */ Float scale,
        /* [in] */ Int32 type,
        /* [in] */ Int32 data,
        /* [in] */ Int32 size,
        /* [in] */ Int32 psize);

    virtual CARAPI_(AutoPtr<IAnimation>) GetCloneInstance();

    CARAPI_(AutoPtr<IAnimation>) Clone();

private:
    /**
     * Called at the end of constructor methods to initialize, if possible, values for
     * the pivot point. This is only possible for ABSOLUTE pivot values.
     */
    CARAPI_(void) InitializePivotPoint();

private:
    AutoPtr<IResources> mResources;
    Float mFromX;
    Float mToX;
    Float mFromY;
    Float mToY;

    Int32 mFromXType;
    Int32 mToXType;
    Int32 mFromYType;
    Int32 mToYType;

    Int32 mFromXData;
    Int32 mToXData;
    Int32 mFromYData;
    Int32 mToYData;

    Int32 mPivotXType;
    Int32 mPivotYType;
    Float mPivotXValue;
    Float mPivotYValue;

    Float mPivotX;
    Float mPivotY;
};

}   //namespace Animation
}   //namespace View
}   //namespace Droid
}   //namespace Elastos
#endif //__ELASTOS_DROID_VIEW_ANIMATION_SCALEANIMATION_H__
