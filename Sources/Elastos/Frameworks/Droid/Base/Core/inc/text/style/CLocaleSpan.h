
#ifndef __ELASTOS_DROID_TEXT_STYLE_CLOCALESPAN_H__
#define __ELASTOS_DROID_TEXT_STYLE_CLOCALESPAN_H__

#include "_CLocaleSpan.h"
#include "text/style/LocaleSpan.h"




using Libcore::ICU::ILocale;

namespace Elastos {
namespace Droid {
namespace Text {
namespace Style {

CarClass(CLocaleSpan), public LocaleSpan
{
public:
    ICHARACTERSTYLE_METHODS_DECL()
    IMETRICAFFECTINGSPAN_METHODS_DECL()
    IPARCELABLESPAN_METHODS_DECL()

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI GetLocale(
        /* [out] */ ILocale** ret);

    CARAPI constructor(
        /* [in] */ ILocale* locale);

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IParcel* src);
};

} // namespace Style
} // namespace Text
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_TEXT_STYLE_CLOCALESPAN_H__
