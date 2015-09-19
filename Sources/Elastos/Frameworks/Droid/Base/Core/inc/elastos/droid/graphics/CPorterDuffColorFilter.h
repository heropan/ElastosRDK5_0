
#ifndef __ELASTOS_DROID_GRAPHICS_CPorterDuffColorFilter_H__
#define __ELASTOS_DROID_GRAPHICS_CPorterDuffColorFilter_H__

#include "_Elastos_Droid_Graphics_CPorterDuffColorFilter.h"
#include "graphics/ColorFilter.h"

namespace Elastos {
namespace Droid {
namespace Graphics {

CarClass(CPorterDuffColorFilter), public ColorFilter
{
public:
    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    /**
     * Create a colorfilter that uses the specified color and porter-duff mode.
     *
     * @param srcColor       The source color used with the specified
     *                       porter-duff mode
     * @param mode           The porter-duff mode that is applied
     */
    CARAPI constructor(
        /* [in] */ Int32 srcColor,
        /* [in] */ PorterDuffMode mode);

private:
    static CARAPI_(Int32) NativeCreatePorterDuffFilter(
        /* [in] */ Int32 srcColor,
        /* [in] */ PorterDuffMode porterDuffMode);

    static CARAPI_(Int32) NCreatePorterDuffFilter(
        /* [in] */ Int32 nativeFilter,
        /* [in] */ Int32 srcColor,
        /* [in] */ PorterDuffMode porterDuffMode);
};

} // namespace Graphics
} // namepsace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_GRAPHICS_CPorterDuffColorFilter_H__
