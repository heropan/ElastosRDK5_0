
#ifndef __ELASTOS_DROID_TEXT_STYLE_CMASKFILTERSPAN_H__
#define __ELASTOS_DROID_TEXT_STYLE_CMASKFILTERSPAN_H__

#include "_CMaskFilterSpan.h"
#include "text/style/MaskFilterSpan.h"


namespace Elastos {
namespace Droid {
namespace Text {
namespace Style {

CarClass(CMaskFilterSpan), public MaskFilterSpan
{
public:
    ICHARACTERSTYLE_METHODS_DECL()

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI GetMaskFilter(
        /* [out] */ IMaskFilter** ret);

    CARAPI constructor(
        /* [in] */ IMaskFilter* filter);
};

} // namespace Style
} // namespace Text
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_TEXT_STYLE_CMASKFILTERSPAN_H__
