
#ifndef __CQUOTESPAN_H__
#define __CQUOTESPAN_H__

#include "_CQuoteSpan.h"
#include "text/style/QuoteSpan.h"
#include "text/style/LeadingMarginSpanMacro.h"
#include "text/ParcelableSpanMacro.h"

namespace Elastos {
namespace Droid {
namespace Text {
namespace Style {

CarClass(CQuoteSpan), public QuoteSpan
{
public:
    ILEADINGMARGINSPAN_METHODS_DECL()
    IPARCELABLESPAN_METHODS_DECL()

    CARAPI GetColor(
        /* [out] */ Int32* ret);

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ Int32 color);

    CARAPI constructor(
        /* [in] */ IParcel* src);
};

} // namespace Style
} // namespace Text
} // namepsace Droid
} // namespace Elastos

#endif // __CQUOTESPAN_H__
