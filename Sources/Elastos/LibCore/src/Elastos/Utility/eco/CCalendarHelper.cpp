
#include "CCalendarHelper.h"
#include "Calendar.h"

#include "CTimeZoneGetterHelper.h"

using Elastos::Utility::CTimeZoneGetterHelper;

namespace Elastos{
namespace Utility{

ECode CCalendarHelper::GetAvailableLocales(
    /* [out, callee] */ ArrayOf<ILocale*>** locales)
{
    return Calendar::GetAvailableLocales(locales);
}

ECode CCalendarHelper::GetInstance(
    /* [out] */ ICalendar ** ppCalenar)
{
    return Calendar::GetInstance(ppCalenar);
}

ECode CCalendarHelper::GetInstanceEx(
    /* [in] */ ILocale * pLocale,
    /* [out] */ ICalendar ** ppCalenar)
{
    return Calendar::GetInstance(pLocale, ppCalenar);
}

ECode CCalendarHelper::GetInstanceEx2(
    /* [in] */ ITimeZone * pTimezone,
    /* [out] */ ICalendar ** ppCalenar)
{
    return Calendar::GetInstance(pTimezone, ppCalenar);
}

ECode CCalendarHelper::GetInstanceEx3(
    /* [in] */ ITimeZone * pTimezone,
    /* [in] */ ILocale * pLocale,
    /* [out] */ ICalendar ** ppCalenar)
{
    return Calendar::GetInstance(pTimezone, pLocale, ppCalenar);
}

} // Utility
} // Elastos
