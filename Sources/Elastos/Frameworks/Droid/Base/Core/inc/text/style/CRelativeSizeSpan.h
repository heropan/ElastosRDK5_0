
#ifndef __CRELATIVESIZESPAN_H__
#define __CRELATIVESIZESPAN_H__

#include "_CRelativeSizeSpan.h"
#include "text/style/RelativeSizeSpan.h"
#include "text/style/CharacterStyleMacro.h"
#include "text/style/MetricAffectingSpanMacro.h"
#include "text/ParcelableSpanMacro.h"

namespace Elastos {
namespace Droid {
namespace Text {
namespace Style {

CarClass(CRelativeSizeSpan)
    , public RelativeSizeSpan
{
public:
    ICHARACTERSTYLE_METHODS_DECL()
    IMETRICAFFECTINGSPAN_METHODS_DECL()
    IPARCELABLESPAN_METHODS_DECL()

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI GetSizeChange(
        /* [out] */ Float* ret);

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ Float proportion);

    CARAPI constructor(
        /* [in] */ IParcel* src);
};

} // namespace Style
} // namespace Text
} // namepsace Droid
} // namespace Elastos

#endif // __CRELATIVESIZESPAN_H__
