
#ifndef __COLORDRAWABLE_H__
#define __COLORDRAWABLE_H__

#include "graphics/drawable/Drawable.h"

namespace Elastos {
namespace Droid {
namespace Graphics {
namespace Drawable {

class ColorDrawable : public Drawable
{
protected:
    class ColorState
        : public Drawable::ConstantState
    {
    public:
        ColorState(
            /* [in] */ ColorState* state);

        CARAPI NewDrawable(
            /* [out] */ IDrawable** drawable);

        CARAPI NewDrawableEx(
            /* [in] */ IResources* res,
            /* [out] */ IDrawable** drawable);

        CARAPI GetChangingConfigurations(
            /* [out] */ Int32* config);

    public:
        Int32 mBaseColor; // base color, independent of setAlpha()
        Int32 mUseColor;  // basecolor modulated by setAlpha()
        Int32 mChangingConfigurations;
    };

public:
    /**
     * Creates a new black ColorDrawable.
     */
    ColorDrawable();

    /**
     * Creates a new ColorDrawable with the specified color.
     *
     * @param color The color to draw.
     */
    ColorDrawable(
        /* [in] */ Int32 color);

    ColorDrawable(
        /* [in] */ ColorState* state);

    //@Override
    CARAPI_(Int32) GetChangingConfigurations();

    /**
     * A mutable BitmapDrawable still shares its Bitmap with any other Drawable
     * that comes from the same resource.
     *
     * @return This drawable.
     */
    //@Override
    CARAPI_(AutoPtr<IDrawable>) Mutate();

    //@Override
    CARAPI Draw(
        /* [in] */ ICanvas* canvas);

    /**
     * Gets the drawable's color value.
     *
     * @return int The color to draw.
     */
    virtual CARAPI GetColor(
        /* [out] */ Int32* color);

    /**
     * Sets the drawable's color value. This action will clobber the results of prior calls to
     * {@link #setAlpha(int)} on this object, which side-affected the underlying color.
     *
     * @param color The color to draw.
     */
    virtual CARAPI SetColor(
        /* [in] */ Int32 color);

    /**
     * Returns the alpha value of this drawable's color.
     *
     * @return A value between 0 and 255.
     */
    virtual CARAPI_(Int32) GetAlpha();

    /**
     * Sets the color's alpha value.
     *
     * @param alpha The alpha value to set, between 0 and 255.
     */
    //@Override
    CARAPI SetAlpha(
        /* [in] */ Int32 alpha);

    /**
     * Setting a color filter on a ColorDrawable has no effect.
     *
     * @param colorFilter Ignore.
     */
    //@Override
    CARAPI SetColorFilter(
        /* [in] */ IColorFilter* colorFilter);

    //@Override
    CARAPI_(Int32) GetOpacity();

    //@Override
    CARAPI Inflate(
        /* [in] */ IResources* r,
        /* [in] */ IXmlPullParser* parser,
        /* [in] */ IAttributeSet* attrs);

    //@Override
    CARAPI_(AutoPtr<IDrawableConstantState>) GetConstantState();

protected:
    CARAPI Init(
        /* [in] */ Int32 color);

    CARAPI Init(
        /* [in] */ ColorState* state);

private:
    AutoPtr<ColorState> mState;
    AutoPtr<IPaint> mPaint;
    Boolean mMutated;
};

} // namespace Drawable
} // namespace Graphics
} // namespace Droid
} // namespace Elastos

#endif //__COLORDRAWABLE_H__

