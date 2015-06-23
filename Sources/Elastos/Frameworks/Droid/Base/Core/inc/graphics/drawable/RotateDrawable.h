
#ifndef __ROTATEDRAWABLE_H__
#define __ROTATEDRAWABLE_H__

#include "graphics/drawable/Drawable.h"

namespace Elastos {
namespace Droid {
namespace Graphics {
namespace Drawable {

class RotateDrawable : public Drawable
{
public:
    /**
     * <p>Represents the state of a rotation for a given drawable. The same
     * rotate drawable can be invoked with different states to drive several
     * rotations at the same time.</p>
     */
    class RotateState
        : public Drawable::ConstantState
    {
    public:
        RotateState(
            /* [in] */ RotateState* source,
            /* [in] */ RotateDrawable* owner,
            /* [in] */ IResources* res);

        CARAPI NewDrawable(
            /* [out] */ IDrawable** drawable);

        CARAPI NewDrawable(
            /* [in] */ IResources* res,
            /* [out] */ IDrawable** drawable);

        CARAPI GetChangingConfigurations(
            /* [out] */ Int32* config);

        CARAPI_(Boolean) CanConstantState();

    public:
        AutoPtr<IDrawable> mDrawable;

        Int32 mChangingConfigurations;

        Boolean mPivotXRel;
        Float mPivotX;
        Boolean mPivotYRel;
        Float mPivotY;

        Float mFromDegrees;
        Float mToDegrees;

        Float mCurrentDegrees;

    private:
        Boolean mCanConstantState;
        Boolean mCheckedConstantState;
    };

public:
    /**
     * <p>Create a new rotating drawable with an empty state.</p>
     */
    RotateDrawable();

    CARAPI Draw(
        /* [in] */ ICanvas* canvas);

    /**
     * Returns the drawable rotated by this RotateDrawable.
     */
    virtual CARAPI_(AutoPtr<IDrawable>) GetDrawable();

    //@Override
    CARAPI_(Int32) GetChangingConfigurations();

    CARAPI SetAlpha(
        /* [in] */ Int32 alpha);

    CARAPI SetColorFilter(
        /* [in] */ IColorFilter* cf);

    CARAPI_(Int32) GetOpacity();

    CARAPI InvalidateDrawable(
        /* [in] */ IDrawable* who);

    CARAPI ScheduleDrawable(
        /* [in] */ IDrawable* who,
        /* [in] */ IRunnable* what,
        /* [in] */ Int64 when);

    CARAPI UnscheduleDrawable(
        /* [in] */ IDrawable* who,
        /* [in] */ IRunnable* what);

    //@Override
    CARAPI_(Boolean) GetPadding(
        /* [in] */ IRect* padding);

    //@Override
    CARAPI_(Boolean) SetVisible(
        /* [in] */ Boolean visible,
        /* [in] */ Boolean restart);

    //@Override
    CARAPI_(Boolean) IsStateful();

    //@Override
    CARAPI_(Int32) GetIntrinsicWidth();

    //@Override
    CARAPI_(Int32) GetIntrinsicHeight();

    //@Override
    CARAPI_(AutoPtr<IDrawableConstantState>) GetConstantState();

    //@Override
    CARAPI Inflate(
        /* [in] */ IResources* r,
        /* [in] */ IXmlPullParser* parser,
        /* [in] */ IAttributeSet* attrs);

    //@Override
    CARAPI_(AutoPtr<IDrawable>) Mutate();

protected:
    CARAPI Init();

    CARAPI Init(
        /* [in] */ RotateState* rotateState,
        /* [in] */ IResources* res);

    //@Override
    CARAPI_(Boolean) OnStateChange(
        /* [in] */ const ArrayOf<Int32>* state);

    //@Override
    CARAPI_(Boolean) OnLevelChange(
        /* [in] */ Int32 level);

    //@Override
    CARAPI_(void) OnBoundsChange(
        /* [in] */ IRect* bounds);

private:
    /**
     * <p>Create a new rotating drawable with the specified state. A copy of
     * this state is used as the internal state for the newly created
     * drawable.</p>
     *
     * @param rotateState the state for this drawable
     */
    RotateDrawable(
        /* [in] */ RotateState* rotateState,
        /* [in] */ IResources* res);

private:
    static const Float MAX_LEVEL;

    AutoPtr<RotateState> mState;
    Boolean mMutated;
};

} // namespace Drawable
} // namespace Graphics
} // namespace Droid
} // namespace Elastos

#endif //__ROTATEDRAWABLE_H__
