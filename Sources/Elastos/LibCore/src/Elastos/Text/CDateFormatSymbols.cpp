#include "CDateFormatSymbols.h"
#include "cmdef.h"

namespace Elastos {
namespace Text {

PInterface CDateFormatSymbols::Probe(
        /* [in] */ REIID riid)
{
    return _CDateFormatSymbols::Probe(riid);
}

ECode CDateFormatSymbols::Clone(
    /* [out] */ IDateFormatSymbols** instance)
{
    return DateFormatSymbols::Clone(instance);
}

ECode CDateFormatSymbols::Equals(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* res)
{
    return DateFormatSymbols::Equals(object, res);
}

ECode CDateFormatSymbols::GetLongStandAloneMonths(
    /* [out, callee] */ ArrayOf<String>** longStandAloneMonths)
{
    return DateFormatSymbols::GetLongStandAloneMonths(longStandAloneMonths);
}

ECode CDateFormatSymbols::GetShortStandAloneMonths(
    /* [out, callee] */ ArrayOf<String>** shortStandAloneMonths)
{
    return DateFormatSymbols::GetShortStandAloneMonths(shortStandAloneMonths);
}

ECode CDateFormatSymbols::GetLongStandAloneWeekdays(
    /* [out, callee] */ ArrayOf<String>** longStandAloneWeekdays)
{
    return DateFormatSymbols::GetLongStandAloneWeekdays(longStandAloneWeekdays);
}

ECode CDateFormatSymbols::GetShortStandAloneWeekdays(
    /* [out, callee] */ ArrayOf<String>** shortStandAloneWeekdays)
{
    return DateFormatSymbols::GetShortStandAloneWeekdays(shortStandAloneWeekdays);
}

ECode CDateFormatSymbols::GetCustomZoneStrings(
    /* [out] */ Boolean* customZoneStrings)
{
    return DateFormatSymbols::GetCustomZoneStrings(customZoneStrings);
}

ECode CDateFormatSymbols::GetLocale(
    /* [out] */ ILocale** locale)
{
    return DateFormatSymbols::GetLocale(locale);
}

ECode CDateFormatSymbols::GetAmPmStrings(
    /* [out, callee] */ ArrayOf<String>** amPmStrings)
{
    return DateFormatSymbols::GetAmPmStrings(amPmStrings);
}

ECode CDateFormatSymbols::GetEras(
    /* [out, callee] */ ArrayOf<String>** eras)
{
    return DateFormatSymbols::GetEras(eras);
}

ECode CDateFormatSymbols::GetLocalPatternChars(
    /* [out] */ String* localPatternChars)
{
    return DateFormatSymbols::GetLocalPatternChars(localPatternChars);
}

ECode CDateFormatSymbols::GetMonths(
    /* [out, callee] */ ArrayOf<String>** months)
{
    return DateFormatSymbols::GetMonths(months);
}

ECode CDateFormatSymbols::GetShortMonths(
    /* [out, callee] */ ArrayOf<String>** shortMonths)
{
    return DateFormatSymbols::GetShortMonths(shortMonths);
}

ECode CDateFormatSymbols::GetShortWeekdays(
    /* [out, callee] */ ArrayOf<String>** shortWeekdays)
{
    return DateFormatSymbols::GetShortWeekdays(shortWeekdays);
}

ECode CDateFormatSymbols::GetWeekdays(
    /* [out, callee] */ ArrayOf<String>** weekdays)
{
    return DateFormatSymbols::GetWeekdays(weekdays);
}

ECode CDateFormatSymbols::GetZoneStrings(
    /* [out, callee] */ ArrayOf<IObjectContainer*> ** outarray)
{
    return DateFormatSymbols::GetZoneStrings(outarray);
}

ECode CDateFormatSymbols::SetAmPmStrings(
    /* [in] */ const ArrayOf<String>& data)
{
    return DateFormatSymbols::SetAmPmStrings(data);
}

ECode CDateFormatSymbols::SetEras(
    /* [in] */ const ArrayOf<String>& data)
{
    return DateFormatSymbols::SetEras(data);
}

ECode CDateFormatSymbols::SetLocalPatternChars(
    /* [in] */ const String& data)
{
    return DateFormatSymbols::SetLocalPatternChars(data);
}

ECode CDateFormatSymbols::SetMonths(
    /* [in] */ const ArrayOf<String>& data)
{
    return DateFormatSymbols::SetMonths(data);
}

ECode CDateFormatSymbols::SetShortMonths(
    /* [in] */ const ArrayOf<String>& data)
{
    return DateFormatSymbols::SetShortMonths(data);
}

ECode CDateFormatSymbols::SetShortWeekdays(
    /* [in] */ const ArrayOf<String>& data)
{
    return DateFormatSymbols::SetShortWeekdays(data);
}

ECode CDateFormatSymbols::SetWeekdays(
    /* [in] */ const ArrayOf<String>& data)
{
    return DateFormatSymbols::SetWeekdays(data);
}

ECode CDateFormatSymbols::SetZoneStrings(
    /* [in] */ ArrayOf<IObjectContainer*> * zoneStrings)
{
    return DateFormatSymbols::SetZoneStrings(zoneStrings);
}

ECode CDateFormatSymbols::constructor()
{
    return DateFormatSymbols::Init();
}

ECode CDateFormatSymbols::constructor(
    /* [in] */ ILocale* locale)
{
    return DateFormatSymbols::Init(locale);
}

} // namespace Text
} // namespace Elastos
