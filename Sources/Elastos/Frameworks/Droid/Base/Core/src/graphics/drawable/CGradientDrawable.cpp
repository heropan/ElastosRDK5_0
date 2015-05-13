
#include "ext/frameworkext.h"
#include "graphics/drawable/CGradientDrawable.h"

namespace Elastos {
namespace Droid {
namespace Graphics {
namespace Drawable {

ECode CGradientDrawable::constructor()
{
    return GradientDrawable::Init();
}

/**
 * Create a new gradient drawable given an orientation and an array
 * of colors for the gradient.
 */
ECode CGradientDrawable::constructor(
    /* [in] */ GradientDrawableOrientation orientation,
    /* [in] */ ArrayOf<Int32>* colors)
{
    return GradientDrawable::Init(orientation, colors);
}

ECode CGradientDrawable::constructor(
    /* [in] */ IDrawableConstantState* state)
{
    return GradientDrawable::Init((GradientState*)state);
}

PInterface CGradientDrawable::Probe(
    /* [in] */ REIID riid)
{
    return _CGradientDrawable::Probe(riid);
}

IDRAWABLE_METHODS_IMPL(CGradientDrawable, GradientDrawable);

/**
 * Specify radii for each of the 4 corners. For each corner, the array
 * contains 2 values, [X_radius, Y_radius]. The corners are ordered
 * top-left, top-right, bottom-right, bottom-left
 */
ECode CGradientDrawable::SetCornerRadii(
    /* [in] */ ArrayOf<Float>* radii)
{
    return GradientDrawable::SetCornerRadii(radii);
}

/**
 * Specify radius for the corners of the gradient. If this is > 0, then the
 * drawable is drawn in a round-rectangle, rather than a rectangle.
 */
ECode CGradientDrawable::SetCornerRadius(
    /* [in] */ Float radius)
{
    return GradientDrawable::SetCornerRadius(radius);
}

/**
 * Set the stroke width and color for the drawable. If width is zero,
 * then no stroke is drawn.
 */
ECode CGradientDrawable::SetStroke(
    /* [in] */ Int32 width,
    /* [in] */ Int32 color)
{
    return GradientDrawable::SetStroke(width, color);
}

ECode CGradientDrawable::SetStrokeEx(
    /* [in] */ Int32 width,
    /* [in] */ Int32 color,
    /* [in] */ Float dashWidth,
    /* [in] */ Float dashGap)
{
    return GradientDrawable::SetStroke(width, color, dashWidth, dashGap);
}

ECode CGradientDrawable::SetSize(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    return GradientDrawable::SetSize(width, height);
}

ECode CGradientDrawable::SetShape(
    /* [in] */ Int32 shape)
{
    return GradientDrawable::SetShape(shape);
}

ECode CGradientDrawable::SetGradientType(
    /* [in] */ Int32 gradient)
{
    return GradientDrawable::SetGradientType(gradient);
}

ECode CGradientDrawable::SetGradientCenter(
    /* [in] */ Float x,
    /* [in] */ Float y)
{
    return GradientDrawable::SetGradientCenter(x, y);
}

ECode CGradientDrawable::SetGradientRadius(
    /* [in] */ Float gradientRadius)
{
    return GradientDrawable::SetGradientRadius(gradientRadius);
}

ECode CGradientDrawable::SetUseLevel(
    /* [in] */ Boolean useLevel)
{
    return GradientDrawable::SetUseLevel(useLevel);
}

ECode CGradientDrawable::GetOrientation(
    /*[out] */ Int32* orientation)
{
    return GradientDrawable::GetOrientation(orientation);
}

ECode CGradientDrawable::SetOrientation(
    /* [in] */ Int32 orientation)
{
    return GradientDrawable::SetOrientation(orientation);
}

ECode CGradientDrawable::SetColors(
    /* [in] */ ArrayOf<Int32>* colors)
{
    return GradientDrawable::SetColors(colors);
}

ECode CGradientDrawable::SetColor(
    /* [in] */ Int32 argb)
{
    return GradientDrawable::SetColor(argb);
}

} // namespace Drawable
} // namespace Graphics
} // namespace Droid
} // namespace Elastos
