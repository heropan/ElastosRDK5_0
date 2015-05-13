
#ifndef __CBLURMASKFILTER_H__
#define __CBLURMASKFILTER_H__

#include "_CBlurMaskFilter.h"
#include "graphics/MaskFilter.h"

namespace Elastos {
namespace Droid {
namespace Graphics {

/**
 * This takes a mask, and blurs its edge by the specified radius. Whether or
 * or not to include the original mask, and whether the blur goes outside,
 * inside, or straddles, the original mask's border, is controlled by the
 * Blur enum.
 */
CarClass(CBlurMaskFilter), public MaskFilter
{
public:
    /**
     * Create a blur maskfilter.
     *
     * @param radius The radius to extend the blur from the original mask. Must be > 0.
     * @param style  The Blur to use
     * @return       The new blur maskfilter
     */
    CARAPI constructor(
        /* [in] */ Float radius,
        /* [in] */ Int32 style);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

private:
    static CARAPI_(Int32) NativeConstructor(
        /* [in] */ Float radius,
        /* [in] */ Int32 style);
};

} // namespace Graphics
} // namepsace Droid
} // namespace Elastos

#endif // __CBLURMASKFILTER_H__
