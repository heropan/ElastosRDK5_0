
#ifndef __ELASTOS_DROID_GRAPHICS_CNINEPATCH_H__
#define __ELASTOS_DROID_GRAPHICS_CNINEPATCH_H__

#include "_CNinePatch.h"
#include "graphics/NinePatch.h"

namespace Elastos {
namespace Droid {
namespace Graphics {

/**
 * The NinePatch class permits drawing a bitmap in nine sections.
 * The four corners are unscaled; the four edges are scaled in one axis,
 * and the middle is scaled in both axes. Normally, the middle is
 * transparent so that the patch can provide a selection about a rectangle.
 * Essentially, it allows the creation of custom graphics that will scale the
 * way that you define, when content added within the image exceeds the normal
 * bounds of the graphic. For a thorough explanation of a NinePatch image,
 * read the discussion in the
 * <a href="{@docRoot}guide/topics/graphics/2d-graphics.html#nine-patch">2D
 * Graphics</a> document.
 * <p>
 * The <a href="{@docRoot}guide/developing/tools/draw9patch.html">Draw 9-Patch</a>
 * tool offers an extremely handy way to create your NinePatch images,
 * using a WYSIWYG graphics editor.
 * </p>
 */
CarClass(CNinePatch), NinePatch
{
public:
    /**
     * Create a drawable projection from a bitmap to nine patches.
     *
     * @param bitmap    The bitmap describing the patches.
     * @param chunk     The 9-patch data chunk describing how the underlying
     *                  bitmap is split apart and drawn.
     * @param srcName   The name of the source for the bitmap. Might be null.
     */
    CARAPI constructor(
        /* [in] */ IBitmap* bitmap,
        /* [in] */ ArrayOf<Byte>* chunk,
        /* [in] */ const String& srcName);

    /**
     * @hide
     */
    CARAPI constructor(
        /* [in] */ INinePatch* patch);

    CARAPI SetPaint(
        /* [in] */ IPaint* p);

    /**
     * Draw a bitmap of nine patches.
     *
     * @param canvas    A container for the current matrix and clip used to draw the bitmap.
     * @param location  Where to draw the bitmap.
     */
    CARAPI Draw(
        /* [in] */ ICanvas* canvas,
        /* [in] */ IRectF* location);

    /**
     * Draw a bitmap of nine patches.
     *
     * @param canvas    A container for the current matrix and clip used to draw the bitmap.
     * @param location  Where to draw the bitmap.
     */
    CARAPI Draw(
        /* [in] */ ICanvas* canvas,
        /* [in] */ IRect* location);

    /**
     * Draw a bitmap of nine patches.
     *
     * @param canvas    A container for the current matrix and clip used to draw the bitmap.
     * @param location  Where to draw the bitmap.
     * @param paint     The Paint to draw through.
     */
    CARAPI Draw(
        /* [in] */ ICanvas* canvas,
        /* [in] */ IRect* location,
        /* [in] */ IPaint* paint);

    /**
     * Return the underlying bitmap's density, as per
     * {@link Bitmap#getDensity() Bitmap.getDensity()}.
     */
    CARAPI GetDensity(
        /* [out] */ Int32* density);

    CARAPI GetWidth(
        /* [out] */ Int32* width);

    CARAPI GetHeight(
        /* [out] */ Int32* height);

    CARAPI HasAlpha(
        /* [out] */ Boolean* hasAlpha);

    CARAPI GetTransparentRegion(
        /* [in] */ IRect* location,
        /* [out] */ IRegion** region);
};

} // namespace Graphics
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_GRAPHICS_CNINEPATCH_H__

