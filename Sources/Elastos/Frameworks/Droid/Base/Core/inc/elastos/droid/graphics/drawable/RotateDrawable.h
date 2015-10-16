
#ifndef __ELASTOS_DROID_GRAPHICS_DRAWABLE_ROTATEDRAWABLE_H__
#define __ELASTOS_DROID_GRAPHICS_DRAWABLE_ROTATEDRAWABLE_H__

#include "elastos/droid/graphics/drawable/Drawable.h"

namespace Elastos {
namespace Droid {
namespace Graphics {
namespace Drawable {

class RotateDrawable
    : public Drawable
    , public IRotateDrawable
    , public IDrawableCallback
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
    CAR_INTERFACE_DECL();

    /**
     * <p>Create a new rotating drawable with an empty state.</p>
     */
    RotateDrawable();

    CARAPI Draw(
        /* [in] */ ICanvas* canvas);

    /**
     * Returns the drawable rotated by this RotateDrawable.
     */
    virtual CARAPI GetDrawable(
        /* [out] */ IDrawable** drawable);

    //@Override
    CARAPI GetChangingConfigurations(
        /* [out] */ Int32* configuration);

    CARAPI SetAlpha(
        /* [in] */ Int32 alpha);

    CARAPI SetColorFilter(
        /* [in] */ IColorFilter* cf);

    CARAPI GetOpacity(
        /* [out] */ Int32* opacity);

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
    CARAPI GetPadding(
        /* [in] */ IRect* padding,
        /* [out] */ Boolean* isPadding);

    //@Override
    CARAPI SetVisible(
        /* [in] */ Boolean visible,
        /* [in] */ Boolean restart,
        /* [out] */ Boolean* isDifferent);

    //@Override
    CARAPI IsStateful(
        /* [out] */ Boolean* isStateful);

    //@Override
    CARAPI GetIntrinsicWidth(
        /* [out] */ Int32* width);

    //@Override
    CARAPI GetIntrinsicHeight(
        /* [out] */ Int32* height);

    //@Override
    CARAPI GetConstantState(
        /* [out] */ IDrawableConstantState** state);

    //@Override
    CARAPI Inflate(
        /* [in] */ IResources* r,
        /* [in] */ IXmlPullParser* parser,
        /* [in] */ IAttributeSet* attrs);

    //@Override
    CARAPI Mutate(
        /* [out] */ IDrawable** drawable);

    virtual CARAPI SetDrawable(
        /* [in] */ IDrawable* drawable);

    virtual CARAPI SetFromDegrees(
        /* [in] */ Float degrees);

    virtual CARAPI GetFromDegrees(
        /* [in] */ Float* degrees);

    virtual CARAPI SetToDegrees(
        /* [in] */ Float degrees);

    virtual CARAPI GetToDegrees(
        /* [in] */ Float* degrees);

    virtual CARAPI SetPivotX(
        /* [in] */ Float pivotX);

    virtual CARAPI GetPivotX(
        /* [in] */ Float* pivotX);

    virtual CARAPI SetPivotXRelative(
        /* [in] */ Boolean pivotX);

    virtual CARAPI IsPivotXRelative(
        /* [in] */ Boolean* pivotX);

    virtual CARAPI SetPivotY(
        /* [in] */ Float pivotY);

    virtual CARAPI GetPivotY(
        /* [in] */ Float* pivotY);

    virtual CARAPI SetPivotYRelative(
        /* [in] */ Boolean pivotY);

    virtual CARAPI IsPivotYRelative(
        /* [in] */ Boolean* pivotY);

protected:
    CARAPI constructor();

    CARAPI constructor(
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

#endif //__ELASTOS_DROID_GRAPHICS_DRAWABLE_ROTATEDRAWABLE_H__
