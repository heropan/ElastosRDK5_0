
#ifndef __CPAINTDRAWABLE_H__
#define __CPAINTDRAWABLE_H__

#include "_CPaintDrawable.h"
#include "graphics/drawable/PaintDrawable.h"
#include "graphics/drawable/DrawableMacro.h"

namespace Elastos {
namespace Droid {
namespace Graphics {
namespace Drawable {

CarClass(CPaintDrawable), PaintDrawable
{
public:
    IDRAWABLE_METHODS_DECL()

    CARAPI GetShape(
        /* [out] */ IShape** shape);

    CARAPI SetShape(
        /* [in] */ IShape* s);

    CARAPI SetShaderFactory(
        /* [in] */ IShaderFactory* fact);

    CARAPI GetShaderFactory(
        /* [out] */ IShaderFactory** fact);

    CARAPI GetPaint(
        /* [out] */ IPaint** paint);

    CARAPI SetPadding(
        /* [in] */ Int32 left,
        /* [in] */ Int32 top,
        /* [in] */ Int32 right,
        /* [in] */ Int32 bottom);

    CARAPI SetPaddingEx(
        /* [in] */ IRect* padding);

    CARAPI SetIntrinsicWidth(
        /* [in] */ Int32 width);

    CARAPI SetIntrinsicHeight(
        /* [in] */ Int32 height);

    /**
     * Specify radius for the corners of the rectangle. If this is > 0, then the
     * drawable is drawn in a round-rectangle, rather than a rectangle.
     * @param radius the radius for the corners of the rectangle
     */
    CARAPI SetCornerRadius(
        /* [in] */ Float radius);

    /**
     * Specify radii for each of the 4 corners. For each corner, the array
     * contains 2 values, [X_radius, Y_radius]. The corners are ordered
     * top-left, top-right, bottom-right, bottom-left
     * @param radii the x and y radii of the corners
     */
    CARAPI SetCornerRadii(
        /* [in] */ ArrayOf<Float>* radii);

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ Int32 color);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);
};

} // namespace Drawable
} // namespace Graphics
} // namespace Droid
} // namespace Elastos

#endif //__CPAINTDRAWABLE_H__
