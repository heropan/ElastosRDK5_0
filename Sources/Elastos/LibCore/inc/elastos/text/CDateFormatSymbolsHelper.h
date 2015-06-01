
#ifndef __CDATEFORMATSYMBOLSHELPER_H__
#define __CDATEFORMATSYMBOLSHELPER_H__

#include "_CDateFormatSymbolsHelper.h"

using Libcore::ICU::ILocale;

namespace Elastos {
namespace Text {

CarClass(CDateFormatSymbolsHelper)
{
public:

    CARAPI GetInstance(
        /* [out] */ IDateFormatSymbols** instance);

    CARAPI GetInstance(
        /* [in] */ ILocale* locale,
        /* [out] */ IDateFormatSymbols** instance);

    CARAPI GetAvailableLocales(
        /* [out, callee] */ ArrayOf<ILocale*>** arrayOfLocales);

};

} // namespace Text
} // namespace Elastos

#endif // __CDATEFORMATSYMBOLSHELPER_H__
