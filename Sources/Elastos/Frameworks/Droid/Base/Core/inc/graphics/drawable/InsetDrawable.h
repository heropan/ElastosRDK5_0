
#ifndef __INSETDRAWABLE_H__
#define __INSETDRAWABLE_H__

#include "ext/frameworkext.h"
#include "graphics/drawable/Drawable.h"
#include "graphics/CRect.h"

namespace Elastos {
namespace Droid {
namespace Graphics {
namespace Drawable {

class InsetDrawable : public Drawable
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

        CARAPI NewDrawableEx(
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
    /*package*/ InsetDrawable();

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
    CARAPI_(Int32) GetChangingConfigurations();

    //@Override
    CARAPI_(Boolean) GetPadding(
        /* [in] */ IRect* padding);

    //@Override
    CARAPI_(Boolean) SetVisible(
        /* [in] */ Boolean visible,
        /* [in] */ Boolean restart);

    CARAPI SetAlpha(
        /* [in] */ Int32 alpha);

    CARAPI SetColorFilter(
        /* [in] */ IColorFilter* cf);

    CARAPI_(Int32) GetOpacity();

    //@Override
    CARAPI_(Boolean) IsStateful();

    //@Override
    CARAPI_(Int32) GetIntrinsicWidth();

    //@Override
    CARAPI_(Int32) GetIntrinsicHeight();

    //@Override
    CARAPI_(AutoPtr<IDrawableConstantState>) GetConstantState();

    //@Override
    CARAPI_(AutoPtr<IDrawable>) Mutate();

protected:
    CARAPI Init();

    CARAPI Init(
        /* [in] */ IDrawable* drawable,
        /* [in] */ Int32 inset);

    CARAPI Init(
        /* [in] */ IDrawable* drawable,
        /* [in] */ Int32 insetLeft,
        /* [in] */ Int32 insetTop,
        /* [in] */ Int32 insetRight,
        /* [in] */ Int32 insetBottom);

    CARAPI Init(
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

#endif //__INSETDRAWABLE_H__
