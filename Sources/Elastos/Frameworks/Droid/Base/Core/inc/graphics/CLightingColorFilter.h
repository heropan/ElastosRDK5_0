
#ifndef __CLIGHTINGCOLORFILTER_H__
#define __CLIGHTINGCOLORFILTER_H__

#include "_CLightingColorFilter.h"
#include "graphics/ColorFilter.h"

namespace Elastos {
namespace Droid {
namespace Graphics {

CarClass(CLightingColorFilter), public ColorFilter
{
public:
    /**
     * Create a colorfilter that multiplies the RGB channels by one color, and then adds a second color,
     * pinning the result for each component to [0..255]. The alpha components of the mul and add arguments
     * are ignored.
     */
    CARAPI constructor(
        /* [in] */ Int32 mul,
        /* [in] */ Int32 add);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

private:
    static CARAPI_(Int32) NativeCreateLightingFilter(
        /* [in] */ Int32 mul,
        /* [in] */ Int32 add);

    static CARAPI_(Int32) NCreateLightingFilter(
        /* [in] */ Int32 skFilter,
        /* [in] */ Int32 mul,
        /* [in] */ Int32 add);
};

} // namespace Graphics
} // namepsace Droid
} // namespace Elastos

#endif // __CLIGHTINGCOLORFILTER_H__
