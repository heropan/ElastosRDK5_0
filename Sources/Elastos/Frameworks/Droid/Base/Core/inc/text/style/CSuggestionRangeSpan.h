
#ifndef __CSUGGESTIONRANGESPAN_H__
#define __CSUGGESTIONRANGESPAN_H__

#include "_CSuggestionRangeSpan.h"
#include "text/style/SuggestionRangeSpan.h"
#include "text/style/CharacterStyleMacro.h"
#include "text/ParcelableSpanMacro.h"

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

#endif // __CSUGGESTIONRANGESPAN_H__
