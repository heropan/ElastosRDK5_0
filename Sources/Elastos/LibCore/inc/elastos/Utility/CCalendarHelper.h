
#ifndef __CCALENDARHELPER_H__
#define __CCALENDARHELPER_H__

#include "_CCalendarHelper.h"

using Libcore::ICU::ILocale;

namespace Elastos{
namespace Utility{

CarClass(CCalendarHelper)
{
public:
    CARAPI GetAvailableLocales(
        /* [out, callee] */ ArrayOf<ILocale*>** locales);

    CARAPI GetInstance(
        /* [out] */ ICalendar ** ppCalenar);

    CARAPI GetInstanceEx(
        /* [in] */ ILocale * pLocale,
        /* [out] */ ICalendar ** ppCalenar);

    CARAPI GetInstanceEx2(
        /* [in] */ ITimeZone * pTimezone,
        /* [out] */ ICalendar ** ppCalenar);

    CARAPI GetInstanceEx3(
        /* [in] */ ITimeZone * pTimezone,
        /* [in] */ ILocale * pLocale,
        /* [out] */ ICalendar ** ppCalenar);
};

} // namespace Utility
} // namespace Elastos

#endif // __CCALENDARHELPER_H__
