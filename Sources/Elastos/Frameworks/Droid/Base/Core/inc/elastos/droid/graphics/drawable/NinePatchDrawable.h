
#ifndef __ELASTOS_DROID_GRAPHICS_DRAWABLE_NINEPATCHDRAWABLE_H__
#define __ELASTOS_DROID_GRAPHICS_DRAWABLE_NINEPATCHDRAWABLE_H__

#include "graphics/drawable/Drawable.h"
#include "graphics/Insets.h"

using Elastos::Droid::Utility::IDisplayMetrics;

namespace Elastos {
namespace Droid {
namespace Graphics {
namespace Drawable {

class NinePatchDrawable
    : public Drawable
    , public INinePatchDrawable
{
public:
    class NinePatchState
        : public Drawable::ConstantState
    {
    public:
        NinePatchState(
            /* [in] */ INinePatch* ninePatch,
            /* [in] */ IRect* padding);

        NinePatchState(
            /* [in] */ INinePatch* ninePatch,
            /* [in] */ IRect* padding,
            /* [in] */ IRect* layoutInsets);

        NinePatchState(
            /* [in] */ INinePatch* ninePatch,
            /* [in] */ IRect* rect,
            /* [in] */ IRect* layoutInsets,
            /* [in] */ Boolean dither);

        // Copy constructor
        NinePatchState(
            /* [in] */ NinePatchState* state);

        CARAPI NewDrawable(
            /* [out] */ IDrawable** drawable);

        CARAPI NewDrawable(
            /* [in] */ IResources* res,
            /* [out] */ IDrawable** drawable);

        CARAPI GetChangingConfigurations(
            /* [out] */ Int32* config);

    private:
        CARAPI_(void) constructor(
            /* [in] */ INinePatch* ninePatch,
            /* [in] */ IRect* rect,
            /* [in] */ IRect* layoutInsets,
            /* [in] */ Boolean dither);

    public:
        AutoPtr<INinePatch> mNinePatch;
        AutoPtr<IRect> mPadding;
        AutoPtr<Insets> mLayoutInsets;
        Boolean mDither;
        Int32 mChangingConfigurations;
        Int32 mTargetDensity;
    };

public:
    CAR_INTERFACE_DECL();

    NinePatchDrawable();

    NinePatchDrawable(
        /* [in] */ IBitmap* bitmap,
        /* [in] */ ArrayOf<Byte>* chunk,
        /* [in] */ IRect* padding,
        /* [in] */ const String& srcName);

    NinePatchDrawable(
        /* [in] */ IResources* res,
        /* [in] */ IBitmap* bitmap,
        /* [in] */ ArrayOf<Byte>* chunk,
        /* [in] */ IRect* padding,
        /* [in] */ const String& srcName);

    /**
     * Create drawable from raw nine-patch data, setting initial target density
     * based on the display metrics of the resources.
     *
     * @hide
     */
    NinePatchDrawable(
        /* [in] */ IResources* res,
        /* [in] */ IBitmap* bitmap,
        /* [in] */ ArrayOf<Byte>* chunk,
        /* [in] */ IRect* padding,
        /* [in] */ IRect* layoutInsets,
        /* [in] */ const String& srcName);

    NinePatchDrawable(
        /* [in] */ INinePatch* patch);

    NinePatchDrawable(
        /* [in] */ IResources* res,
        /* [in] */ INinePatch* patch);

    NinePatchDrawable(
        /* [in] */ NinePatchState* state,
        /* [in] */ IResources* res);

    virtual CARAPI SetTargetDensity(
        /* [in] */ ICanvas* canvas);

    virtual CARAPI SetTargetDensity(
        /* [in] */ IDisplayMetrics* metrics);

    virtual CARAPI SetTargetDensity(
        /* [in] */ Int32 density);

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

    /**
     * @hide
     */
    //@Override
    CARAPI_(AutoPtr<IInsets>) GetLayoutInsets();

    //@Override
    CARAPI SetAlpha(
        /* [in] */ Int32 alpha);

    //@Override
    CARAPI SetColorFilter(
        /* [in] */ IColorFilter* cf);

    //@Override
    CARAPI SetDither(
        /* [in] */ Boolean dither);

    //@Override
    CARAPI SetFilterBitmap(
        /* [in] */ Boolean filter);

    //@Override
    CARAPI Inflate(
        /* [in] */ IResources* r,
        /* [in] */ IXmlPullParser* parser,
        /* [in] */ IAttributeSet* attrs);

    virtual CARAPI GetPaint(
        /* [out] */ IPaint** paint);

    //@Override
    CARAPI GetIntrinsicWidth(
        /* [out] */ Int32* width);

    //@Override
    CARAPI GetIntrinsicHeight(
        /* [out] */ Int32* height);

    //@Override
    CARAPI GetMinimumWidth(
        /* [out] */ Int32* width);

    //@Override
    CARAPI GetMinimumHeight(
        /* [out] */ Int32* height);

    //@Override
    CARAPI GetOpacity(
        /* [out] */ Int32* opacity);

    //@Override
    CARAPI GetTransparentRegion(
        /* [out] */ IRegion** bounds);

    //@Override
    CARAPI GetConstantState(
        /* [out] */ IDrawableConstantState** state);

    //@Override
    CARAPI Mutate(
        /* [out] */ IDrawable** drawable);

protected:
    CARAPI constructor(
        /* [in] */ IBitmap* bitmap,
        /* [in] */ ArrayOf<Byte>* chunk,
        /* [in] */ IRect* padding,
        /* [in] */ const String& srcName);

    CARAPI constructor(
        /* [in] */ IResources* res,
        /* [in] */ IBitmap* bitmap,
        /* [in] */ ArrayOf<Byte>* chunk,
        /* [in] */ IRect* padding,
        /* [in] */ const String& srcName);

    CARAPI constructor(
        /* [in] */ IResources* res,
        /* [in] */ IBitmap* bitmap,
        /* [in] */ ArrayOf<Byte>* chunk,
        /* [in] */ IRect* padding,
        /* [in] */ IRect* layoutInsets,
        /* [in] */ const String& srcName);

    CARAPI constructor(
        /* [in] */ INinePatch* patch);

    CARAPI constructor(
        /* [in] */ IResources* res,
        /* [in] */ INinePatch* patch);

    CARAPI constructor(
        /* [in] */ NinePatchState* state,
        /* [in] */ IResources* res);

private:
    CARAPI_(void) SetNinePatchState(
        /* [in] */ NinePatchState* state,
        /* [in] */ IResources* res);

    static CARAPI_(AutoPtr<Insets>) ScaleFromDensity(
        /* [in] */ Insets* insets,
        /* [in] */ Int32 sdensity,
        /* [in] */ Int32 tdensity);

    CARAPI_(void) ComputeBitmapSize();

private:
    // dithering helps a lot, and is pretty cheap, so default is true
    static const Boolean DEFAULT_DITHER = TRUE;
    AutoPtr<NinePatchState> mNinePatchState;
    AutoPtr<INinePatch> mNinePatch;
    AutoPtr<IRect> mPadding;
    AutoPtr<Insets> mLayoutInsets;
    AutoPtr<IPaint> mPaint;
    Boolean mMutated;

    Int32 mTargetDensity;

    // These are scaled to match the target density.
    Int32 mBitmapWidth;
    Int32 mBitmapHeight;
};

} // namespace Drawable
} // namespace Graphics
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_GRAPHICS_DRAWABLE_NINEPATCHDRAWABLE_H__
