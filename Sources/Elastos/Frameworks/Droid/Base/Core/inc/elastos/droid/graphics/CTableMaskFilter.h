
#ifndef __ELASTOS_DROID_GRAPHICS_CTABLEMASKFILTER_H__
#define __ELASTOS_DROID_GRAPHICS_CTABLEMASKFILTER_H__

#include "_Elastos_Droid_Graphics_CTableMaskFilter.h"
#include "graphics/MaskFilter.h"

namespace Elastos {
namespace Droid {
namespace Graphics {

CarClass(CTableMaskFilter), public MaskFilter
{
public:
    CARAPI constructor(
        /* [in] */ const ArrayOf<Byte>& table);

    CARAPI constructor(
        /* [in] */ Int32 ni);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    static CARAPI CreateClipTable(
        /* [in] */ Int32 min,
        /* [in] */ Int32 max,
        /* [out] */ ITableMaskFilter** tf);

    static CARAPI CreateGammaTable(
        /* [in] */ Float gamma,
        /* [out] */ ITableMaskFilter** tf);

private:
    static CARAPI_(Int32) NativeNewTable(
        /* [in] */ const ArrayOf<Byte>& table);

    static CARAPI_(Int32) NativeNewClip(
        /* [in] */ Int32 min,
        /* [in] */ Int32 max);

    static CARAPI_(Int32) NativeNewGamma(
        /* [in] */ Float gamma);
};

} // namespace Graphics
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_GRAPHICS_CTABLEMASKFILTER_H__
