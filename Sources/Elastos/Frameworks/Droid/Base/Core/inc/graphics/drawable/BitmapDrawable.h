
#ifndef __ELASTOS_DROID_GRAPHICS_DRAWABLE_H_H__
#define __ELASTOS_DROID_GRAPHICS_DRAWABLE_H_H__

#include <ext/frameworkext.h>
#include "graphics/drawable/Drawable.h"


using Elastos::Droid::Utility::IDisplayMetrics;

namespace Elastos {
namespace Droid {
namespace Graphics {
namespace Drawable {

class BitmapDrawable : public Drawable
{
public:
    class BitmapState
        : public Drawable::ConstantState
    {
    public:
        BitmapState(
            /* [in] */ IBitmap* bitmap);

        BitmapState(
            /* [in] */ BitmapState* bitmap);

        CARAPI NewDrawable(
            /* [out] */ IDrawable** drawable);

        CARAPI NewDrawable(
            /* [in] */ IResources* res,
            /* [out] */ IDrawable** drawable);

        CARAPI GetChangingConfigurations(
            /* [out] */ Int32* config);

    public:
        AutoPtr<IBitmap> mBitmap;
        Int32 mChangingConfigurations;
        Int32 mGravity;
        AutoPtr<IPaint> mPaint;
        ShaderTileMode mTileModeX;
        ShaderTileMode mTileModeY;
        Int32 mTargetDensity;
        Boolean mRebuildShader;
    };

public:
    BitmapDrawable();

    BitmapDrawable(
        /* [in] */ IResources* res);

    BitmapDrawable(
        /* [in] */ IBitmap* bitmap);

    BitmapDrawable(
        /* [in] */ IResources* res,
        /* [in] */ IBitmap* bitmap);

    BitmapDrawable(
        /* [in] */ const String& filepath);

    BitmapDrawable(
        /* [in] */ IResources* res,
        /* [in] */ const String& filepath);

    BitmapDrawable(
        /* [in] */ IInputStream* is);

    BitmapDrawable(
        /* [in] */ IResources* res,
        /* [in] */ IInputStream* is);

    CARAPI_(AutoPtr<IPaint>) GetPaint();

    /**
     * Returns the bitmap used by this drawable to render. May be null.
     */
    CARAPI_(AutoPtr<IBitmap>) GetBitmap();

    /**
     * Set the density scale at which this drawable will be rendered. This
     * method assumes the drawable will be rendered at the same density as the
     * specified canvas.
     *
     * @param canvas The Canvas from which the density scale must be obtained.
     *
     * @see android.graphics.Bitmap#setDensity(Int32)
     * @see android.graphics.Bitmap#getDensity()
     */
    virtual CARAPI SetTargetDensity(
        /* [in] */ ICanvas* canvas);

    /**
     * Set the density scale at which this drawable will be rendered.
     *
     * @param metrics The DisplayMetrics indicating the density scale for this drawable.
     *
     * @see android.graphics.Bitmap#setDensity(Int32)
     * @see android.graphics.Bitmap#getDensity()
     */
    virtual CARAPI SetTargetDensity(
        /* [in] */ IDisplayMetrics* metrics);

    /**
     * Set the density at which this drawable will be rendered.
     *
     * @param density The density scale for this drawable.
     *
     * @see android.graphics.Bitmap#setDensity(Int32)
     * @see android.graphics.Bitmap#getDensity()
     */
    virtual CARAPI SetTargetDensity(
        /* [in] */ Int32 density);

    /** Get the gravity used to position/stretch the bitmap within its bounds.
     * See android.view.Gravity
     * @return the gravity applied to the bitmap
     */
    virtual CARAPI_(Int32) GetGravity();

    /** Set the gravity used to position/stretch the bitmap within its bounds.
     *  See android.view.Gravity
     * @param gravity the gravity
     */
    virtual CARAPI SetGravity(
        /* [in] */ Int32 gravity);

    /**
     * Enables or disables anti-aliasing for this drawable. Anti-aliasing affects
     * the edges of the bitmap only so it applies only when the drawable is rotated.
     *
     * @param aa True if the bitmap should be anti-aliased, false otherwise.
     */
    virtual CARAPI SetAntiAlias(
        /* [in] */ Boolean aa);

    //@Override
    CARAPI SetFilterBitmap(
        /* [in] */ Boolean filter);

    //@Override
    CARAPI SetDither(
        /* [in] */ Boolean dither);

    /**
     * Indicates the repeat behavior of this drawable on the X axis.
     *
     * @return {@link Shader.TileMode#CLAMP} if the bitmap does not repeat,
     *         {@link Shader.TileMode#REPEAT} or {@link Shader.TileMode#MIRROR} otherwise.
     */
    virtual CARAPI_(ShaderTileMode) GetTileModeX();

    /**
     * Indicates the repeat behavior of this drawable on the Y axis.
     *
     * @return {@link Shader.TileMode#CLAMP} if the bitmap does not repeat,
     *         {@link Shader.TileMode#REPEAT} or {@link Shader.TileMode#MIRROR} otherwise.
     */
    virtual CARAPI_(ShaderTileMode) GetTileModeY();

    /**
     * Sets the repeat behavior of this drawable on the X axis. By default, the drawable
     * does not repeat its bitmap. Using {@link Shader.TileMode#REPEAT} or
     * {@link Shader.TileMode#MIRROR} the bitmap can be repeated (or tiled) if the bitmap
     * is smaller than this drawable.
     *
     * @param mode The repeat mode for this drawable.
     *
     * @see #setTileModeY(android.graphics.Shader.TileMode)
     * @see #setTileModeXY(android.graphics.Shader.TileMode, android.graphics.Shader.TileMode)
     */
    virtual CARAPI SetTileModeX(
        /* [in] */ ShaderTileMode mode);

    /**
     * Sets the repeat behavior of this drawable on the Y axis. By default, the drawable
     * does not repeat its bitmap. Using {@link Shader.TileMode#REPEAT} or
     * {@link Shader.TileMode#MIRROR} the bitmap can be repeated (or tiled) if the bitmap
     * is smaller than this drawable.
     *
     * @param mode The repeat mode for this drawable.
     *
     * @see #setTileModeX(android.graphics.Shader.TileMode)
     * @see #setTileModeXY(android.graphics.Shader.TileMode, android.graphics.Shader.TileMode)
     */
    CARAPI SetTileModeY(
        /* [in] */ ShaderTileMode mode);

    /**
     * Sets the repeat behavior of this drawable on both axis. By default, the drawable
     * does not repeat its bitmap. Using {@link Shader.TileMode#REPEAT} or
     * {@link Shader.TileMode#MIRROR} the bitmap can be repeated (or tiled) if the bitmap
     * is smaller than this drawable.
     *
     * @param xmode The X repeat mode for this drawable.
     * @param ymode The Y repeat mode for this drawable.
     *
     * @see #setTileModeX(android.graphics.Shader.TileMode)
     * @see #setTileModeY(android.graphics.Shader.TileMode)
     */
    virtual CARAPI SetTileModeXY(
        /* [in] */ ShaderTileMode xmode,
        /* [in] */ ShaderTileMode ymode);

    //@Override
    CARAPI_(Int32) GetChangingConfigurations();

    //@Override
    CARAPI Draw(
        /* [in] */ ICanvas* canvas);

    //@Override
    CARAPI SetAlpha(
        /* [in] */ Int32 alpha);

    //@Override
    CARAPI SetColorFilter(
        /* [in] */ IColorFilter* cf);

    //@Override
    CARAPI_(AutoPtr<IDrawable>) Mutate();

    //@Override
    CARAPI Inflate(
        /* [in] */ IResources* r,
        /* [in] */ IXmlPullParser* parser,
        /* [in] */ IAttributeSet* attrs);

    //@Override
    CARAPI_(Int32) GetIntrinsicWidth();

    //@Override
    CARAPI_(Int32) GetIntrinsicHeight();

    //@Override
    CARAPI_(Int32) GetOpacity();

    //@Override
    CARAPI_(AutoPtr<IDrawableConstantState>) GetConstantState();

protected:
    //@Override
    CARAPI_(void) OnBoundsChange(
        /* [in] */ IRect* bounds);

    CARAPI Init();

    CARAPI Init(
        /* [in] */ IResources* res);

    CARAPI Init(
        /* [in] */ IBitmap* bitmap);

    CARAPI Init(
        /* [in] */ IResources* res,
        /* [in] */ IBitmap* bitmap);

    CARAPI Init(
        /* [in] */ const String& filepath);

    CARAPI Init(
        /* [in] */ IResources* res,
        /* [in] */ const String& filepath);

    CARAPI Init(
        /* [in] */ IInputStream* is);

    CARAPI Init(
        /* [in] */ IResources* res,
        /* [in] */ IInputStream* is);

    CARAPI Init(
        /* [in] */ BitmapState* state,
        /* [in] */ IResources* res);

private:
    CARAPI_(void) ComputeBitmapSize();

    CARAPI_(void) SetBitmap(
        /* [in] */ IBitmap* bitmap);

private:
    static const Int32 DEFAULT_PAINT_FLAGS;

    AutoPtr<BitmapState> mBitmapState;
    AutoPtr<IBitmap>  mBitmap;
    Int32 mTargetDensity;

    AutoPtr<IRect> mDstRect;

    Boolean mApplyGravity;
    Boolean mMutated;

    // These are scaled to match the target density.
    Int32 mBitmapWidth;
    Int32 mBitmapHeight;
};

} // namespace Drawable
} // namespace Graphics
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_GRAPHICS_DRAWABLE_H_H__

