
#ifndef __COVALSHAPE_H__
#define __COVALSHAPE_H__

#include "graphics/drawable/shapes/OvalShape.h"
#include "_COvalShape.h"

namespace Elastos {
namespace Droid {
namespace Graphics {
namespace Drawable {
namespace Shapes {

CarClass(COvalShape), public OvalShape
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

#endif //__COVALSHAPE_H__
