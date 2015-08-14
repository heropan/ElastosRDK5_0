
#ifndef __ELASTOS_DROID_GRAPHICS_DRAWABLE_PICTUREDRAWABLE_H__
#define __ELASTOS_DROID_GRAPHICS_DRAWABLE_PICTUREDRAWABLE_H__

#include "graphics/drawable/Drawable.h"
#include "graphics/CPicture.h"

namespace Elastos {
namespace Droid {
namespace Graphics {
namespace Drawable {

/**
 * Drawable subclass that wraps a Picture, allowing the picture to be used
 * whereever a Drawable is supported.
 */
class PictureDrawable : public Drawable
{
public:
    /**
     * Construct a new drawable referencing the specified picture. The picture
     * may be null.
     *
     * @param picture The picture to associate with the drawable. May be null.
     */
    PictureDrawable(
        /* [in] */ IPicture* picture);

    /**
     * Return the picture associated with the drawable. May be null.
     *
     * @return the picture associated with the drawable, or null.
     */
    virtual CARAPI_(AutoPtr<IPicture>) GetPicture();

    /**
     * Associate a picture with this drawable. The picture may be null.
     *
     * @param picture The picture to associate with the drawable. May be null.
     */
    virtual CARAPI SetPicture(
        /* [in] */ IPicture* picture);

    //@Override
    CARAPI Draw(
        /* [in] */ ICanvas* canvas);

    //@Override
    CARAPI_(Int32) GetIntrinsicWidth();

    //@Override
    CARAPI_(Int32) GetIntrinsicHeight();

    //@Override
    CARAPI_(Int32) GetOpacity();

    //@Override
    CARAPI SetFilterBitmap(
        /* [in] */ Boolean filter);

    //@Override
    CARAPI SetDither(
        /* [in] */ Boolean dither);

    //@Override
    CARAPI SetColorFilter(
        /* [in] */ IColorFilter* colorFilter);

    //@Override
    CARAPI SetAlpha(
        /* [in] */ Int32 alpha);

protected:
    PictureDrawable();

    CARAPI Init(
        /* [in] */ IPicture* picture);

private:
    AutoPtr<IPicture> mPicture;
};

} // namespace Drawable
} // namespace Graphics
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_GRAPHICS_DRAWABLE_PICTUREDRAWABLE_H__
