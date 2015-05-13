#ifndef __RECTSHAPE_H__
#define __RECTSHAPE_H__

#include "graphics/drawable/shapes/Shape.h"
#include "graphics/CRectF.h"

namespace Elastos {
namespace Droid {
namespace Graphics {
namespace Drawable {
namespace Shapes {

/**
 * Defines a rectangle shape.
 * The rectangle can be drawn to a Canvas with its own draw() method,
 * but more graphical control is available if you instead pass
 * the RectShape to a {@link android.graphics.drawable.ShapeDrawable}.
 */
class RectShape : public Shape
{
public:
    /**
     * RectShape constructor.
     */
    RectShape();

    //@Override
    CARAPI Draw(
        /* [in] */ ICanvas* canvas,
        /* [in] */ IPaint* paint);

    CARAPI_(void) Clone(
        /* [in] */ RectShape* other);

protected:
    CARAPI Init();

    //@Override
    CARAPI_(void) OnResize(
        /* [in] */ Float width,
        /* [in] */ Float height);

    /**
     * Returns the RectF that defines this rectangle's bounds.
     */
    CARAPI_(AutoPtr<CRectF>) Rect();

private:
    AutoPtr<CRectF> mRect;
};

} // namespace Shapes
} // namespace Drawable
} // namespace Graphics
} // namespace Droid
} // namespace Elastos

#endif //__RECTSHAPE_H__
