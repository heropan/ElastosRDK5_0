
#ifndef __CCORNERPATHEFFECT_H__
#define __CCORNERPATHEFFECT_H__

#include "_CCornerPathEffect.h"
#include "graphics/PathEffect.h"

namespace Elastos {
namespace Droid {
namespace Graphics {

CarClass(CCornerPathEffect), public PathEffect
{
public:
    /**
     * Transforms geometries that are drawn (either STROKE or FILL styles) by
     * replacing any sharp angles between line segments into rounded angles of
     * the specified radius.
     * @param radius Amount to round sharp angles between line segments.
     */
    CARAPI constructor(
        /* [in] */ Float radius);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

private:
    static CARAPI_(Int32) NativeCreate(
        /* [in] */ Float radius);
};

} // namespace Graphics
} // namepsace Droid
} // namespace Elastos

#endif // __CCORNERPATHEFFECT_H__
