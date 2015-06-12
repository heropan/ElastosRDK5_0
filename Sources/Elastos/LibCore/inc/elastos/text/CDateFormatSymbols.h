
#ifndef __ELASTOS_TEXT_CDATEFORMATSYMBOLS_H__
#define __ELASTOS_TEXT_CDATEFORMATSYMBOLS_H__

#include "_Elastos_Text_CDateFormatSymbols.h"
#include "DateFormatSymbols.h"

using Elastos::Utility::ILocale;

namespace Elastos {
namespace Text {

CarClass(CDateFormatSymbols) , public DateFormatSymbols
{
public:
    CARAPI_(PInterface) Probe(
            /* [in] */ REIID riid);

    // @Override
    CARAPI Clone(
        /* [out] */ IDateFormatSymbols** instance);

    CARAPI Equals(
        /* [in] */ IInterface* object,
        /* [out] */ Boolean* res);

    CARAPI GetLongStandAloneMonths(
        /* [out, callee] */ ArrayOf<String>** longStandAloneMonths);

    CARAPI GetShortStandAloneMonths(
        /* [out, callee] */ ArrayOf<String>** shortStandAloneMonths);

    CARAPI GetLongStandAloneWeekdays(
        /* [out, callee] */ ArrayOf<String>** longStandAloneWeekdays);

    CARAPI GetShortStandAloneWeekdays(
        /* [out, callee] */ ArrayOf<String>** shortStandAloneWeekdays);

    CARAPI GetCustomZoneStrings(
        /* [out] */ Boolean* customZoneStrings);

    CARAPI GetLocale(
        /* [out] */ ILocale** locale);

    CARAPI GetAmPmStrings(
        /* [out, callee] */ ArrayOf<String>** amPmStrings);

    CARAPI GetEras(
        /* [out, callee] */ ArrayOf<String>** eras);

    CARAPI GetLocalPatternChars(
        /* [out] */ String* localPatternChars);

    CARAPI GetMonths(
        /* [out, callee] */ ArrayOf<String>** months);

    CARAPI GetShortMonths(
        /* [out, callee] */ ArrayOf<String>** shortMonths);

    CARAPI GetShortWeekdays(
        /* [out, callee] */ ArrayOf<String>** shortWeekdays);

    CARAPI GetWeekdays(
        /* [out, callee] */ ArrayOf<String>** weekdays);

    CARAPI GetZoneStrings(
        /* [out, callee] */ ArrayOf<IObjectContainer*> ** outarray);

    CARAPI SetAmPmStrings(
        /* [in] */ const ArrayOf<String>& data);

    CARAPI SetEras(
        /* [in] */ const ArrayOf<String>& data);

    CARAPI SetLocalPatternChars(
        /* [in] */ const String& data);

    CARAPI SetMonths(
        /* [in] */ const ArrayOf<String>& data);

    CARAPI SetShortMonths(
        /* [in] */ const ArrayOf<String>& data);

    CARAPI SetShortWeekdays(
        /* [in] */ const ArrayOf<String>& data);

    CARAPI SetWeekdays(
        /* [in] */ const ArrayOf<String>& data);

    CARAPI SetZoneStrings(
        /* [in] */ ArrayOf<IObjectContainer*> * zoneStrings);

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ ILocale* locale);

};

} // namespace Text
} // namespace Elastos

#endif // __ELASTOS_TEXT_CDATEFORMATSYMBOLS_H__
