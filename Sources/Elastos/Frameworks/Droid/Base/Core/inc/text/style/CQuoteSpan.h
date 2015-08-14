
#ifndef __ELASTOS_DROID_TEXT_STYLE_CQUOTESPAN_H__
#define __ELASTOS_DROID_TEXT_STYLE_CQUOTESPAN_H__

#include "_CQuoteSpan.h"
#include "text/style/QuoteSpan.h"



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

#endif // __ELASTOS_DROID_TEXT_STYLE_CQUOTESPAN_H__
