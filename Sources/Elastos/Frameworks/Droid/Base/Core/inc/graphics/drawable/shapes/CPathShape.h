
#ifndef __CPATHSHAPE_H__
#define __CPATHSHAPE_H__

#include "graphics/drawable/shapes/PathShape.h"
#include "_CPathShape.h"

namespace Elastos {
namespace Droid {
namespace Graphics {
namespace Drawable {
namespace Shapes {

CarClass(CPathShape), public PathShape
{
public:
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
    CARAPI constructor(
        /* [in] */ IPath* path,
        /* [in] */ Float stdWidth,
        /* [in] */ Float stdHeight);

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

#endif //__CPATHSHAPE_H__
