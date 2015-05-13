
#include "ext/frameworkext.h"
#include "graphics/drawable/CShapeDrawable.h"

namespace Elastos {
namespace Droid {
namespace Graphics {
namespace Drawable {

/**
 * ShapeDrawable constructor.
 */
ECode CShapeDrawable::constructor()
{
    return ShapeDrawable::Init();
}

/**
 * Creates a ShapeDrawable with a specified Shape.
 *
 * @param s the Shape that this ShapeDrawable should be
 */
ECode CShapeDrawable::constructor(
    /* [in] */ IShape* s)
{
    return ShapeDrawable::Init(s);
}

ECode CShapeDrawable::constructor(
    /* [in] */ Handle32 state)
{
    return ShapeDrawable::Init((ShapeState*)state);
}

PInterface CShapeDrawable::Probe(
    /* [in] */ REIID riid)
{
    return _CShapeDrawable::Probe(riid);
}

IDRAWABLE_METHODS_IMPL(
    CShapeDrawable, ShapeDrawable);

/**
 * Returns the Shape of this ShapeDrawable.
 */
ECode CShapeDrawable::GetShape(
    /* [out] */ IShape** shape)
{
    VALIDATE_NOT_NULL(shape);
    AutoPtr<IShape> s = ShapeDrawable::GetShape();
    *shape = s;
    INTERFACE_ADDREF(*shape);
    return NOERROR;
}

/**
 * Sets the Shape of this ShapeDrawable.
 */
ECode CShapeDrawable::SetShape(
    /* [in] */ IShape* s)
{
    return ShapeDrawable::SetShape(s);
}

/**
 * Sets a ShaderFactory to which requests for a
 * {@link android.graphics.Shader} object will be made.
 *
 * @param fact an instance of your ShaderFactory implementation
 */
ECode CShapeDrawable::SetShaderFactory(
    /* [in] */ IShaderFactory* fact)
{
    return ShapeDrawable::SetShaderFactory(fact);
}

/**
 * Returns the ShaderFactory used by this ShapeDrawable for requesting a
 * {@link android.graphics.Shader}.
 */
ECode CShapeDrawable::GetShaderFactory(
    /* [out] */ IShaderFactory** fact)
{
    VALIDATE_NOT_NULL(fact);
    AutoPtr<IShaderFactory> f = ShapeDrawable::GetShaderFactory();
    *fact = f;
    INTERFACE_ADDREF(*fact);
    return NOERROR;
}

/**
 * Returns the Paint used to draw the shape.
 */
ECode CShapeDrawable::GetPaint(
    /* [out] */ IPaint** paint)
{
    VALIDATE_NOT_NULL(paint);
    AutoPtr<IPaint> p = ShapeDrawable::GetPaint();
    *paint = p;
    INTERFACE_ADDREF(*paint);
    return NOERROR;
}

/**
 * Sets padding for the shape.
 *
 * @param left    padding for the left side (in pixels)
 * @param top     padding for the top (in pixels)
 * @param right   padding for the right side (in pixels)
 * @param bottom  padding for the bottom (in pixels)
 */
ECode CShapeDrawable::SetPadding(
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom)
{
    return ShapeDrawable::SetPadding(left, top, right, bottom);
}

/**
 * Sets padding for this shape, defined by a Rect object.
 * Define the padding in the Rect object as: left, top, right, bottom.
 */
ECode CShapeDrawable::SetPaddingEx(
    /* [in] */ IRect* padding)
{
    return ShapeDrawable::SetPadding(padding);
}

/**
 * Sets the intrinsic (default) width for this shape.
 *
 * @param width the intrinsic width (in pixels)
 */
ECode CShapeDrawable::SetIntrinsicWidth(
    /* [in] */ Int32 width)
{
    return ShapeDrawable::SetIntrinsicWidth(width);
}

/**
 * Sets the intrinsic (default) height for this shape.
 *
 * @param height the intrinsic height (in pixels)
 */
ECode CShapeDrawable::SetIntrinsicHeight(
    /* [in] */ Int32 height)
{
    return ShapeDrawable::SetIntrinsicHeight(height);
}

} // namespace Drawable
} // namespace Graphics
} // namespace Droid
} // namespace Elastos
