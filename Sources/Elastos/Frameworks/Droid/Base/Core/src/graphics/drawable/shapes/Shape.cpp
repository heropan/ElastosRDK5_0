
#include "graphics/drawable/shapes/Shape.h"

namespace Elastos {
namespace Droid {
namespace Graphics {
namespace Drawable {
namespace Shapes {


Shape::Shape()
    : mWidth(0)
    , mHeight(0)
{
}

 /**
 * Returns the width of the Shape.
 */
Float Shape::GetWidth()
{
    return mWidth;
}

/**
 * Returns the height of the Shape.
 */
Float Shape::GetHeight()
{
    return mHeight;
}

/**
 * Resizes the dimensions of this shape.
 * Must be called before {@link #draw(Canvas,Paint)}.
 *
 * @param width the width of the shape (in pixels)
 * @param height the height of the shape (in pixels)
 */
ECode Shape::Resize(
    /* [in] */ Float width,
    /* [in] */ Float height)
{
    if (width < 0) {
        width = 0;
    }
    if (height < 0) {
        height =0;
    }
    if (mWidth != width || mHeight != height) {
        mWidth = width;
        mHeight = height;
        OnResize(width, height);
    }

    return NOERROR;
}

/**
 * Checks whether the Shape is opaque.
 * Default impl returns true. Override if your subclass can be opaque.
 *
 * @return true if any part of the drawable is <em>not</em> opaque.
 */
Boolean Shape::HasAlpha()
{
    return TRUE;
}

/**
 * Callback method called when {@link #resize(float,float)} is executed.
 *
 * @param width the new width of the Shape
 * @param height the new height of the Shape
 */
void Shape::OnResize(
    /* [in] */ Float width,
    /* [in] */ Float height)
{}

void Shape::Clone(
    /* [in] */ Shape* other)
{
    other->mWidth = mWidth;
    other->mHeight = mHeight;
}

} // namespace Shapes
} // namespace Drawable
} // namespace Graphics
} // namespace Droid
} // namespace Elastos
