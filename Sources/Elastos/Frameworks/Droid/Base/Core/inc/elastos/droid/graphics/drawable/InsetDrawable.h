
#ifndef __ELASTOS_DROID_GRAPHICS_DRAWABLE_INSETDRAWABLE_H__
#define __ELASTOS_DROID_GRAPHICS_DRAWABLE_INSETDRAWABLE_H__

#include "elastos/droid/ext/frameworkext.h"
#include "graphics/drawable/Drawable.h"
#include "graphics/CRect.h"

namespace Elastos {
namespace Droid {
namespace Graphics {
namespace Drawable {

class InsetDrawable
    : public Drawable
    , public IInsetDrawable
    , public IDrawableCallback
{
public:
    class InsetState
        : public Drawable::ConstantState
    {
    public:
        InsetState(
            /* [in] */ InsetState* orig,
            /* [in] */ InsetDrawable* owner,
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

        Int32 mInsetLeft;
        Int32 mInsetTop;
        Int32 mInsetRight;
        Int32 mInsetBottom;

        Boolean mCheckedConstantState;
        Boolean mCanConstantState;
    };

public:
    CAR_INTERFACE_DECL();

    InsetDrawable();

    InsetDrawable(
        /* [in] */ IDrawable* drawable,
        /* [in] */ Int32 inset);

    InsetDrawable(
        /* [in] */ IDrawable* drawable,
        /* [in] */ Int32 insetLeft,
        /* [in] */ Int32 insetTop,
        /* [in] */ Int32 insetRight,
        /* [in] */ Int32 insetBottom);

    //@Override
    CARAPI Inflate(
        /* [in] */ IResources* r,
        /* [in] */ IXmlPullParser* parser,
        /* [in] */ IAttributeSet* attrs);

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
    CARAPI Draw(
        /* [in] */ ICanvas* canvas);

    //@Override
    CARAPI GetChangingConfigurations(
        /* [out] */ Int32* configuration);

    //@Override
    CARAPI GetPadding(
        /* [in] */ IRect* padding,
        /* [out] */ Boolean* isPadding);

    //@Override
    CARAPI SetVisible(
        /* [in] */ Boolean visible,
        /* [in] */ Boolean restart,
        /* [out] */ Boolean* isDifferent);

    CARAPI SetAlpha(
        /* [in] */ Int32 alpha);

    CARAPI SetColorFilter(
        /* [in] */ IColorFilter* cf);

    CARAPI GetOpacity(
        /* [out] */ Int32* opacity);

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
    CARAPI Mutate(
        /* [out] */ IDrawable** drawable);

    virtual CARAPI GetDrawable(
        /* [out] */ IDrawable** drawable);

protected:
    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IDrawable* drawable,
        /* [in] */ Int32 inset);

    CARAPI constructor(
        /* [in] */ IDrawable* drawable,
        /* [in] */ Int32 insetLeft,
        /* [in] */ Int32 insetTop,
        /* [in] */ Int32 insetRight,
        /* [in] */ Int32 insetBottom);

    CARAPI constructor(
        /* [in] */ InsetState* state,
        /* [in] */ IResources* res);

    //@Override
    CARAPI_(Boolean) OnStateChange(
        /* [in] */ const ArrayOf<Int32>* state);

    //@Override
    CARAPI_(void) OnBoundsChange(
        /* [in] */ IRect* bounds);

private:
    InsetDrawable(
        /* [in] */ InsetState* state,
        /* [in] */ IResources* res);

private:
    // Most of this is copied from ScaleDrawable.
    AutoPtr<InsetState> mInsetState;
    AutoPtr<CRect> mTmpRect;
    Boolean mMutated;
};

} // namespace Drawable
} // namespace Graphics
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_GRAPHICS_DRAWABLE_INSETDRAWABLE_H__
