
#include "CDecimalFormatSymbolsHelper.h"
#include "DecimalFormatSymbols.h"

namespace Elastos {
namespace Text {

ECode CDecimalFormatSymbolsHelper::GetInstance(
    /* [out] */ IDecimalFormatSymbols** instance)
{
    return DecimalFormatSymbols::GetInstance(instance);
}

ECode CDecimalFormatSymbolsHelper::GetInstanceEx(
    /* [in] */ ILocale* locale,
    /* [out] */ IDecimalFormatSymbols** instance)
{
    return DecimalFormatSymbols::GetInstance(locale,instance);
}

ECode CDecimalFormatSymbolsHelper::GetAvailableLocales(
    /* [out, callee] */ ArrayOf<ILocale*>** locales)
{
    return DecimalFormatSymbols::GetAvailableLocales(locales);
}

} // namespace Text
} // namespace Elastos