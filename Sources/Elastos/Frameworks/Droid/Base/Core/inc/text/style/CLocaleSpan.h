
#ifndef __CLOCALESPAN_H__
#define __CLOCALESPAN_H__

#include "_CLocaleSpan.h"
#include "text/style/LocaleSpan.h"
#include "text/style/CharacterStyleMacro.h"
#include "text/style/MetricAffectingSpanMacro.h"
#include "text/ParcelableSpanMacro.h"

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

#endif // __CLOCALESPAN_H__
