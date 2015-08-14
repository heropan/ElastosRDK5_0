
#ifndef __ELASTOS_DROID_GRAPHICS_CCOLORMATRIXCOLORFILTER_H__
#define __ELASTOS_DROID_GRAPHICS_CCOLORMATRIXCOLORFILTER_H__

#include "_CColorMatrixColorFilter.h"
#include "graphics/ColorFilter.h"

namespace Elastos {
namespace Droid {
namespace Graphics {

CarClass(CColorMatrixColorFilter), public ColorFilter
{
public:
    /**
     * Create a colorfilter that transforms colors through a 4x5 color matrix.
     *
     * @param matrix 4x5 matrix used to transform colors. It is copied into
     *               the filter, so changes made to the matrix after the filter
     *               is constructed will not be reflected in the filter.
     */
    CARAPI constructor(
        /* [in] */ IColorMatrix* matrix);

    /**
     * Create a colorfilter that transforms colors through a 4x5 color matrix.
     *
     * @param array array of floats used to transform colors, treated as a 4x5
     *              matrix. The first 20 entries of the array are copied into
     *              the filter. See ColorMatrix.
     */
    CARAPI constructor(
        /* [in] */ const ArrayOf<Float>& array);

    CARAPI_(PInterface) Probe(
            /* [in]  */ REIID riid);

private:
    static CARAPI_(Int32) NativeColorMatrixFilter(
        /* [in] */ const ArrayOf<Float>& array);

    static CARAPI_(Int32) NColorMatrixFilter(
        /* [in] */ Int32 nativeFilter,
        /* [in] */ const ArrayOf<Float>& array);
};

} // namespace Graphics
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_GRAPHICS_CCOLORMATRIXCOLORFILTER_H__
