
#include "ext/frameworkext.h"
#include "graphics/drawable/CPaintDrawable.h"


namespace Elastos {
namespace Droid {
namespace Graphics {
namespace Drawable {

ECode CPaintDrawable::constructor()
{
    return PaintDrawable::Init();
}

ECode CPaintDrawable::constructor(
    /* [in] */ Int32 color)
{
    return PaintDrawable::Init(color);
}

PInterface CPaintDrawable::Probe(
    /* [in] */ REIID riid)
{
    return _CPaintDrawable::Probe(riid);
}

IDRAWABLE_METHODS_IMPL(
    CPaintDrawable, PaintDrawable);

ECode CPaintDrawable::GetShape(
    /* [out] */ IShape** shape)
{
    VALIDATE_NOT_NULL(shape);
    AutoPtr<IShape> s = PaintDrawable::GetShape();
    *shape = s;
    INTERFACE_ADDREF(*shape);
    return NOERROR;
}

ECode CPaintDrawable::SetShape(
    /* [in] */ IShape* s)
{
    return PaintDrawable::SetShape(s);
}

ECode CPaintDrawable::SetShaderFactory(
    /* [in] */ IShaderFactory* fact)
{
    return PaintDrawable::SetShaderFactory(fact);
}

ECode CPaintDrawable::GetShaderFactory(
    /* [out] */ IShaderFactory** fact)
{
    VALIDATE_NOT_NULL(fact);
    AutoPtr<IShaderFactory> f = PaintDrawable::GetShaderFactory();
    *fact = f;
    INTERFACE_ADDREF(*fact);
    return NOERROR;
}

ECode CPaintDrawable::GetPaint(
    /* [out] */ IPaint** paint)
{
    VALIDATE_NOT_NULL(paint);
    AutoPtr<IPaint> p = PaintDrawable::GetPaint();
    *paint = p;
    INTERFACE_ADDREF(*paint);
    return NOERROR;
}

ECode CPaintDrawable::SetPadding(
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom)
{
    return PaintDrawable::SetPadding(left, top, right, bottom);
}

ECode CPaintDrawable::SetPaddingEx(
    /* [in] */ IRect* padding)
{
    return PaintDrawable::SetPadding(padding);
}

ECode CPaintDrawable::SetIntrinsicWidth(
    /* [in] */ Int32 width)
{
    return PaintDrawable::SetIntrinsicWidth(width);
}

ECode CPaintDrawable::SetIntrinsicHeight(
    /* [in] */ Int32 height)
{
    return PaintDrawable::SetIntrinsicHeight(height);
}

/**
 * Specify radius for the corners of the rectangle. If this is > 0, then the
 * drawable is drawn in a round-rectangle, rather than a rectangle.
 * @param radius the radius for the corners of the rectangle
 */
ECode CPaintDrawable::SetCornerRadius(
    /* [in] */ Float radius)
{
    return PaintDrawable::SetCornerRadius(radius);
}

/**
 * Specify radii for each of the 4 corners. For each corner, the array
 * contains 2 values, [X_radius, Y_radius]. The corners are ordered
 * top-left, top-right, bottom-right, bottom-left
 * @param radii the x and y radii of the corners
 */
ECode CPaintDrawable::SetCornerRadii(
    /* [in] */ ArrayOf<Float>* radii)
{
    return PaintDrawable::SetCornerRadii(radii);
}

} // namespace Drawable
} // namespace Graphics
} // namespace Droid
} // namespace Elastos
