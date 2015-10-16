
#ifndef __ELASTOS_DROID_APP_FASTBITMAPDRAWABLE_H__
#define __ELASTOS_DROID_APP_FASTBITMAPDRAWABLE_H__

#include "elastos/droid/ext/frameworkdef.h"
#include "elastos/droid/graphics/drawable/Drawable.h"

using Elastos::Droid::Graphics::IBitmap;
using Elastos::Droid::Graphics::IPaint;
using Elastos::Droid::Graphics::ICanvas;
using Elastos::Droid::Graphics::IColorFilter;

namespace Elastos{
namespace Droid{
namespace App{

/**
 * Special drawable that draws a wallpaper as fast as possible.  Assumes
 * no scaling or placement off (0,0) of the wallpaper (this should be done
 * at the time the bitmap is loaded).
 */
class FastBitmapDrawable : public Elastos::Droid::Graphics::Drawable::Drawable
{
public:
    FastBitmapDrawable();

    virtual CARAPI Draw(
        /* [in] */ ICanvas* canvas);

    virtual CARAPI_(Int32) GetOpacity();

    using Elastos::Droid::Graphics::Drawable::Drawable::SetBounds;
    virtual CARAPI SetBounds(
        /* [in] */ Int32 left,
        /* [in] */ Int32 top,
        /* [in] */ Int32 right,
        /* [in] */ Int32 bottom);

    virtual CARAPI SetAlpha(
        /* [in] */ Int32 alpha)
    {
        return E_UNSUPPORTED_OPERATION_EXCEPTION;
    }

    virtual CARAPI SetColorFilter(
        /* [in] */ IColorFilter* cf)
    {
        return E_UNSUPPORTED_OPERATION_EXCEPTION;
    }

    virtual CARAPI SetDither(
        /* [in] */ Boolean dither)
    {
        return E_UNSUPPORTED_OPERATION_EXCEPTION;
    }

    virtual CARAPI SetFilterBitmap(
        /* [in] */ Boolean filter)
    {
        return E_UNSUPPORTED_OPERATION_EXCEPTION;
    }

    virtual CARAPI_(Int32) GetIntrinsicWidth()
    {
        return mWidth;
    }

    virtual CARAPI_(Int32) GetIntrinsicHeight()
    {
        return mHeight;
    }

    virtual CARAPI_(Int32) GetMinimumWidth()
    {
        return mWidth;
    }

    virtual CARAPI_(Int32) GetMinimumHeight()
    {
        return mHeight;
    }

protected:
    CARAPI Init(
        /* [in] */ IBitmap* bitmap);

private:
    AutoPtr<IBitmap> mBitmap;
    Int32 mWidth;
    Int32 mHeight;
    Int32 mDrawLeft;
    Int32 mDrawTop;
    AutoPtr<IPaint> mPaint;
};

} // namespace App
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_APP_FASTBITMAPDRAWABLE_H__
