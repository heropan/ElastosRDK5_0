
#ifndef __CARCSHAPE_H__
#define __CARCSHAPE_H__

#include "graphics/drawable/shapes/ArcShape.h"
#include "_CArcShape.h"

namespace Elastos {
namespace Droid {
namespace Graphics {
namespace Drawable {
namespace Shapes {

CarClass(CArcShape), public ArcShape
{
public:
    /**
     * ArcShape constructor.
     *
     * @param startAngle the angle (in degrees) where the arc begins
     * @param sweepAngle the sweep angle (in degrees). Anything equal to or
     *                   greater than 360 results in a complete circle/oval.
     */
    CARAPI constructor(
        /* [in] */ Float startAngle,
        /* [in] */ Float sweepAngle);

    CARAPI GetWidth(
        /* [out] */ Float* width);

    CARAPI GetHeight(
        /* [out] */ Float* height);

    CARAPI Draw(
        /* [in] */ ICanvas* canvas,
        /* [in] */ IPaint* paint);

    CARAPI Resize(
        /* [in] */ Float width,
        /* [in] */ Float height);

    CARAPI HasAlpha(
        /* [out] */ Boolean* hasAlpha);

    CARAPI Clone(
        /* [out] */ IShape** shape);
};

} // namespace Shapes
} // namespace Drawable
} // namespace Graphics
} // namespace Droid
} // namespace Elastos

#endif //__CARCSHAPE_H__
