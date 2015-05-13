
#ifndef __ROTATEANIMATION_H__
#define __ROTATEANIMATION_H__

#include "view/animation/Animation.h"

namespace Elastos {
namespace Droid {
namespace View {
namespace Animation {

class RotateAnimation : public Animation
{
public:
    RotateAnimation(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    RotateAnimation(
        /* [in] */ Float fromDegrees,
        /* [in] */ Float toDegrees);

    RotateAnimation(
        /* [in] */ Float fromDegrees,
        /* [in] */ Float toDegrees,
        /* [in] */ Float pivotX,
        /* [in] */ Float pivotY);

    RotateAnimation(
        /* [in] */ Float fromDegrees,
        /* [in] */ Float toDegrees,
        /* [in] */ Int32 pivotXType,
        /* [in] */ Float pivotXValue,
        /* [in] */ Int32 pivotYType,
        /* [in] */ Float pivotYValue);

    //@Override
    CARAPI Initialize(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ Int32 parentWidth,
        /* [in] */ Int32 parentHeight);

protected:
    RotateAnimation();

    //@Override
    CARAPI_(void) ApplyTransformation(
        /* [in] */ Float interpolatedTime,
        /* [in] */ ITransformation* t);

    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI Init(
        /* [in] */ Float fromDegrees,
        /* [in] */ Float toDegrees);

    CARAPI Init(
        /* [in] */ Float fromDegrees,
        /* [in] */ Float toDegrees,
        /* [in] */ Float pivotX,
        /* [in] */ Float pivotY);

    CARAPI Init(
        /* [in] */ Float fromDegrees,
        /* [in] */ Float toDegrees,
        /* [in] */ Int32 pivotXType,
        /* [in] */ Float pivotXValue,
        /* [in] */ Int32 pivotYType,
        /* [in] */ Float pivotYValue);

    virtual CARAPI_(AutoPtr<IAnimation>) GetCloneInstance();

    CARAPI_(AutoPtr<IAnimation>) Clone();

private:
    /**
     * Called at the end of constructor methods to initialize, if possible, values for
     * the pivot point. This is only possible for ABSOLUTE pivot values.
     */
    CARAPI_(void) InitializePivotPoint();

private:
    Float mFromDegrees;
    Float mToDegrees;

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
#endif //__ROTATEANIMATION_H__
