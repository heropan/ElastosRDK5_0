
#ifndef __ELASTOS_DROID_GRAPHICS_CTYPEFACE_H__
#define __ELASTOS_DROID_GRAPHICS_CTYPEFACE_H__

#include "_CTypeface.h"
#include "graphics/Typeface.h"

using namespace Elastos;

namespace Elastos {
namespace Droid {
namespace Graphics {

/**
 * The Typeface class specifies the typeface and intrinsic style of a font.
 * This is used in the paint, along with optionally Paint settings like
 * textSize, textSkewX, textScaleX to specify
 * how text appears when drawn (and measured).
 */
CarClass(CTypeface), public Typeface
{
public:
    CARAPI constructor(
        /* [in] */ Int32 ni);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    /** Returns the typeface's intrinsic style attributes */
    CARAPI GetStyle(
        /* [out] */ Int32* style);

    /** Returns true if getStyle() has the BOLD bit set. */
    CARAPI IsBold(
        /* [out] */ Boolean* isBold);

    /** Returns true if getStyle() has the ITALIC bit set. */
    CARAPI IsItalic(
        /* [out] */ Boolean* isItalic);
};

} // namespace Graphics
} // namepsace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_GRAPHICS_CTYPEFACE_H__
