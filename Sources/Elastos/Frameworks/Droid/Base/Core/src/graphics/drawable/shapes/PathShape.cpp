#include "graphics/drawable/shapes/PathShape.h"
#include "graphics/CPath.h"

namespace Elastos {
namespace Droid {
namespace Graphics {
namespace Drawable {
namespace Shapes {

PathShape::PathShape()
    : mStdWidth(0)
    , mStdHeight(0)
    , mScaleX(0)
    , mScaleY(0)
{}

/**
 * PathShape constructor.
 *
 * @param path       a Path that defines the geometric paths for this shape
 * @param stdWidth   the standard width for the shape. Any changes to the
 *                   width with resize() will result in a width scaled based
 *                   on the new width divided by this width.
 * @param stdHeight  the standard height for the shape. Any changes to the
 *                   height with resize() will result in a height scaled based
 *                   on the new height divided by this height.
 */
PathShape::PathShape(
    /* [in] */ IPath* path,
    /* [in] */ Float stdWidth,
    /* [in] */ Float stdHeight)
    : mPath(path)
    , mStdWidth(stdWidth)
    , mStdHeight(stdHeight)
{}

ECode PathShape::Init(
    /* [in] */ IPath* path,
    /* [in] */ Float stdWidth,
    /* [in] */ Float stdHeight)
{
    mPath = path;
    mStdWidth = stdWidth;
    mStdHeight = stdHeight;
    return NOERROR;
}

ECode PathShape::Draw(
    /* [in] */ ICanvas* canvas,
    /* [in] */ IPaint* paint)
{
    Int32 count;
    canvas->Save(&count);
    canvas->Scale(mScaleX, mScaleY);
    canvas->DrawPath(mPath, paint);
    canvas->Restore();
    return NOERROR;
}

//@Override
void PathShape::OnResize(
    /* [in] */ Float width,
    /* [in] */ Float height)
{
    mScaleX = width / mStdWidth;
    mScaleY = height / mStdHeight;
}

void PathShape::Clone(
    /* [in] */ PathShape* other)
{
    Shape::Clone((Shape*)other);
    CPath::New(mPath, (IPath**)&other->mPath);
    other->mStdWidth = mStdWidth;
    other->mStdHeight = mStdHeight;
    other->mScaleX = mScaleX;
    other->mScaleY = mScaleY;
}

} // namespace Shapes
} // namespace Drawable
} // namespace Graphics
} // namespace Droid
} // namespace Elastos
