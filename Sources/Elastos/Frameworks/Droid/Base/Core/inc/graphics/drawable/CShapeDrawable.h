
#ifndef __ELASTOS_DROID_GRAPHICS_DRAWABLE_CSHAPEDRAWABLE_H__
#define __ELASTOS_DROID_GRAPHICS_DRAWABLE_CSHAPEDRAWABLE_H__

#include "_Elastos_Droid_Graphics_Drawable_CShapeDrawable.h"
#include "graphics/drawable/CShapeDrawable.h"
#include "graphics/drawable/ShapeDrawable.h"


namespace Elastos {
namespace Droid {
namespace Graphics {
namespace Drawable {

CarClass(CShapeDrawable), public ShapeDrawable
{
public:
    CARAPI_(PInterface) Probe(
    /* [in] */ REIID riid);

    IDRAWABLE_METHODS_DECL()

    /**
     * Returns the Shape of this ShapeDrawable.
     */
    CARAPI GetShape(
        /* [out] */ IShape** shape);

    /**
     * Sets the Shape of this ShapeDrawable.
     */
    CARAPI SetShape(
        /* [in] */ IShape* s);

    /**
     * Sets a ShaderFactory to which requests for a
     * {@link android.graphics.Shader} object will be made.
     *
     * @param fact an instance of your ShaderFactory implementation
     */
    CARAPI SetShaderFactory(
        /* [in] */ IShaderFactory* fact);

    /**
     * Returns the ShaderFactory used by this ShapeDrawable for requesting a
     * {@link android.graphics.Shader}.
     */
    CARAPI GetShaderFactory(
        /* [out] */ IShaderFactory** fact);

    /**
     * Returns the Paint used to draw the shape.
     */
    CARAPI GetPaint(
        /* [out] */ IPaint** paint);

    /**
     * Sets padding for the shape.
     *
     * @param left    padding for the left side (in pixels)
     * @param top     padding for the top (in pixels)
     * @param right   padding for the right side (in pixels)
     * @param bottom  padding for the bottom (in pixels)
     */
    CARAPI SetPadding(
        /* [in] */ Int32 left,
        /* [in] */ Int32 top,
        /* [in] */ Int32 right,
        /* [in] */ Int32 bottom);

    /**
     * Sets padding for this shape, defined by a Rect object.
     * Define the padding in the Rect object as: left, top, right, bottom.
     */
    CARAPI SetPadding(
        /* [in] */ IRect* padding);

    /**
     * Sets the intrinsic (default) width for this shape.
     *
     * @param width the intrinsic width (in pixels)
     */
    CARAPI SetIntrinsicWidth(
        /* [in] */ Int32 width);

    /**
     * Sets the intrinsic (default) height for this shape.
     *
     * @param height the intrinsic height (in pixels)
     */
    CARAPI SetIntrinsicHeight(
        /* [in] */ Int32 height);

    /**
     * ShapeDrawable constructor.
     */
    CARAPI constructor();

    /**
     * Creates a ShapeDrawable with a specified Shape.
     *
     * @param s the Shape that this ShapeDrawable should be
     */
    CARAPI constructor(
        /* [in] */ IShape* s);

    CARAPI constructor(
        /* [in] */ Handle32 state);
};

} // namespace Drawable
} // namespace Graphics
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_GRAPHICS_DRAWABLE_CSHAPEDRAWABLE_H__
