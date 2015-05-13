#ifndef __CRECTSHAPE_H__
#define __CRECTSHAPE_H__

#include "graphics/drawable/shapes/RectShape.h"
#include "_CRectShape.h"

namespace Elastos {
namespace Droid {
namespace Graphics {
namespace Drawable {
namespace Shapes {

CarClass(CRectShape), public RectShape
{
public:
    CARAPI constructor();

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

#endif //__CRECTSHAPE_H__