
#ifndef __ELASTOS_DROID_GRAPHICS_DRAWABLE_CGRADIENTDRAWABLE_H__
#define __ELASTOS_DROID_GRAPHICS_DRAWABLE_CGRADIENTDRAWABLE_H__

#include "_Elastos_Droid_Graphics_Drawable_CGradientDrawable.h"
#include "graphics/drawable/GradientDrawable.h"


namespace Elastos {
namespace Droid {
namespace Graphics {
namespace Drawable {

CarClass(CGradientDrawable), public  GradientDrawable
{
public:
    CARAPI constructor();

    /**
     * Create a new gradient drawable given an orientation and an array
     * of colors for the gradient.
     */
    CARAPI constructor(
        /* [in] */ GradientDrawableOrientation orientation,
        /* [in] */ ArrayOf<Int32>* colors);

    CARAPI constructor(
        /* [in] */ IDrawableConstantState* state);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    IDRAWABLE_METHODS_DECL()

    /**
     * Specify radii for each of the 4 corners. For each corner, the array
     * contains 2 values, [X_radius, Y_radius]. The corners are ordered
     * top-left, top-right, bottom-right, bottom-left
     */
    CARAPI SetCornerRadii(
        /* [in] */ ArrayOf<Float>* radii);

    /**
     * Specify radius for the corners of the gradient. If this is > 0, then the
     * drawable is drawn in a round-rectangle, rather than a rectangle.
     */
    CARAPI SetCornerRadius(
        /* [in] */ Float radius);

    /**
     * Set the stroke width and color for the drawable. If width is zero,
     * then no stroke is drawn.
     */
    CARAPI SetStroke(
        /* [in] */ Int32 width,
        /* [in] */ Int32 color);

    CARAPI SetStroke(
        /* [in] */ Int32 width,
        /* [in] */ Int32 color,
        /* [in] */ Float dashWidth,
        /* [in] */ Float dashGap);

    CARAPI SetSize(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height);

    CARAPI SetShape(
        /* [in] */ Int32 shape);

    CARAPI SetGradientType(
        /* [in] */ Int32 gradient);

    CARAPI SetGradientCenter(
        /* [in] */ Float x,
        /* [in] */ Float y);

    CARAPI SetGradientRadius(
        /* [in] */ Float gradientRadius);

    CARAPI SetUseLevel(
        /* [in] */ Boolean useLevel);

    CARAPI GetOrientation(
        /*[out] */ Int32* orientation);

    CARAPI SetOrientation(
        /* [in] */ Int32 orientation);

    CARAPI SetColors(
        /* [in] */ ArrayOf<Int32>* colors);

    CARAPI SetColor(
        /* [in] */ Int32 argb);
};

} // namespace Drawable
} // namespace Graphics
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_GRAPHICS_DRAWABLE_CGRADIENTDRAWABLE_H__
