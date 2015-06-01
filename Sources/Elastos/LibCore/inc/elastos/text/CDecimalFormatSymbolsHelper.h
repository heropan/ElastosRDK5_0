
#ifndef __CDECIMALFORMATSYMBOLSHELPER_H__
#define __CDECIMALFORMATSYMBOLSHELPER_H__

#include "_CDecimalFormatSymbolsHelper.h"
#include <cmdef.h>
#include <Elastos.CoreLibrary.h>

using Libcore::ICU::ILocale;

namespace Elastos {
namespace Text {

CarClass(CDecimalFormatSymbolsHelper)
{
public:
    CARAPI GetInstance(
        /* [out] */ IDecimalFormatSymbols** instance);

    CARAPI GetInstance(
        /* [in] */ ILocale* locale,
        /* [out] */ IDecimalFormatSymbols** instance);

    CARAPI GetAvailableLocales(
        /* [out, callee] */ ArrayOf<ILocale*>** locales);

};

} // namespace Text
} // namespace Elastos

#endif // __CDECIMALFORMATSYMBOLSHELPER_H__
