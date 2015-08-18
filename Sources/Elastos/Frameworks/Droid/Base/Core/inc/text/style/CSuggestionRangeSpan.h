
#ifndef __ELASTOS_DROID_TEXT_STYLE_CSUGGESTIONRANGESPAN_H__
#define __ELASTOS_DROID_TEXT_STYLE_CSUGGESTIONRANGESPAN_H__

#include "_Elastos_Droid_Text_Style_CSuggestionRangeSpan.h"
#include "text/style/SuggestionRangeSpan.h"



namespace Elastos {
namespace Droid {
namespace Text {
namespace Style {

CarClass(CSuggestionRangeSpan), public SuggestionRangeSpan
{
public:
    ICHARACTERSTYLE_METHODS_DECL()
    IPARCELABLESPAN_METHODS_DECL()

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI SetBackgroundColor(
        /* [in] */ Int32 backgroundColor);

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IParcel* src);
};

} // namespace Style
} // namespace Text
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_TEXT_STYLE_CSUGGESTIONRANGESPAN_H__
