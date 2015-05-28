
#include "TimeZone.h"
#include "CSimpleTimeZone.h"
#include "ZoneInfoDB.h"
#include <elastos/StringUtils.h>
#include "CStringWrapper.h"
#include "CPatternHelper.h"
#include "CLocaleHelper.h"
#include "CTimeZones.h"

using Elastos::Core::StringUtils;
using Elastos::Core::ICharSequence;
using Elastos::Core::CStringWrapper;
using Elastos::Utility::Regex::IPatternHelper;
using Elastos::Utility::Regex::CPatternHelper;
using Elastos::Utility::Regex::IMatcher;
using Libcore::ICU::ILocaleHelper;
using Libcore::ICU::CLocaleHelper;
using Libcore::ICU::ITimeZones;
using Libcore::ICU::CTimeZones;

namespace Elastos {
namespace Utility {

static AutoPtr<ITimeZone> CreateSimpleTimeZone(
    /* [in] */ Int32 offset,
    /* [in] */ const char* name)
{
    AutoPtr<CSimpleTimeZone> ctz;
    CSimpleTimeZone::NewByFriend(offset, String(name), (CSimpleTimeZone**)&ctz);
    assert(ctz != NULL && " Failed to CreateSimpleTimeZone");
    AutoPtr<ITimeZone> tz = (ITimeZone*)ctz;
    return tz;
}

static AutoPtr<IPattern> CreatePattern(
   /* [in] */ const char* name)
{
   AutoPtr<CPatternHelper> patternHelper;
   CPatternHelper::AcquireSingletonByFriend((CPatternHelper**)&patternHelper);

   AutoPtr<IPattern> pattern;
   patternHelper->Compile(String(name), (IPattern**)&pattern);
   return pattern;
}

AutoPtr<ITimeZone> TimeZone::sGMT = CreateSimpleTimeZone(0, "GMT");
AutoPtr<ITimeZone> TimeZone::sUTC = CreateSimpleTimeZone(0, "UTC");
AutoPtr<ITimeZone> TimeZone::sCHINA = CreateSimpleTimeZone(8, "UTC");
AutoPtr<ITimeZone> TimeZone::sDefaultTimeZone = NULL;
AutoPtr<IPattern> TimeZone::sCustomZoneIDPattern = CreatePattern("^GMT[-+](\\d{1,2})(:?(\\d\\d))?$");

AutoPtr<ArrayOf<String> > TimeZone::GetAvailableIDs()
{
    AutoPtr<ArrayOf<String> > array = ZoneInfoDB::GetAvailableIDs();
    return array;
}

AutoPtr<ArrayOf<String> > TimeZone::GetAvailableIDs(
    /* [in] */ Int32 offsetMillis)
{
    AutoPtr<ArrayOf<String> > array = ZoneInfoDB::GetAvailableIDs(offsetMillis);
    return array;
}

AutoPtr<ITimeZone> TimeZone::GetDefault()
{
    if (sDefaultTimeZone == NULL) {
        sDefaultTimeZone = ZoneInfoDB::GetSystemDefault();
    }

    assert(sDefaultTimeZone != NULL && "DefaultTimeZone can not be null!");

    AutoPtr<ITimeZone> tz;
    sDefaultTimeZone->Clone((ITimeZone**)&tz);
    return tz;
}

ECode TimeZone::GetDisplayName(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);

    AutoPtr<ILocaleHelper> localeHelper;
    CLocaleHelper::AcquireSingleton((ILocaleHelper**)&localeHelper);
    AutoPtr<ILocale> locale;
    localeHelper->GetDefault((ILocale**)&locale);
    return GetDisplayName(FALSE, ITimeZone::LONG, locale, name);
}

ECode TimeZone::GetDisplayName(
    /* [in] */ ILocale* locale,
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);
    return GetDisplayName(FALSE, ITimeZone::LONG, locale, name);
}

ECode TimeZone::GetDisplayName(
    /* [in] */ Boolean daylightTime,
    /* [in] */ Int32 style,
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);

    AutoPtr<ILocaleHelper> localeHelper;
    CLocaleHelper::AcquireSingleton((ILocaleHelper**)&localeHelper);
    AutoPtr<ILocale> locale;
    localeHelper->GetDefault((ILocale**)&locale);
    return GetDisplayName(daylightTime, style, locale, name);
}

ECode TimeZone::GetDisplayName(
    /* [in] */ Boolean daylightTime,
    /* [in] */ Int32 style,
    /* [in] */ ILocale* locale,
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);

    if (style != ITimeZone::SHORT && style != ITimeZone::LONG) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    Boolean isUsed;
    UseDaylightTime(&isUsed);

    Boolean useDaylight = daylightTime && isUsed;

    AutoPtr< ArrayOf<IArrayOf*> > zoneStrings;
    AutoPtr<ITimeZones> tzs;
    CTimeZones::AcquireSingleton((ITimeZones** )&tzs);
    tzs->GetZoneStrings(locale, (ArrayOf<IArrayOf*>**)&zoneStrings);
    String result;
    tzs->GetDisplayName(zoneStrings, mID, daylightTime, style, &result);
    if (!result.IsNull()) {
        *name = result;
        return NOERROR;
    }

    Int32 offset;
    GetRawOffset(&offset);
    if (useDaylight && this->Probe(EIID_ISimpleTimeZone) != NULL) {
        Int32 savings;
        GetDSTSavings(&savings);
        offset += savings;
    }

    offset /= 60000;

    Char32 sign = '+';
    if (offset < 0) {
        sign = '-';
        offset = -offset;
    }

    StringBuffer builder(9);
    builder.AppendCStr("GMT");
    builder.AppendChar(sign);
    AppendNumber(&builder, 2, offset / 60);
    builder.AppendChar(':');
    AppendNumber(&builder, 2, offset % 60);
    return builder.ToString(name);
}

void TimeZone::AppendNumber(
    /* [out] */ StringBuffer* buffer,
    /* [in] */ Int32 count,
    /* [in] */ Int32 value)
{
    String string = StringUtils::Int32ToString(value);
    Char32 c = '0';
    for (UInt32 i = 0; i < count - string.GetLength(); i++) {
        buffer->AppendChar(c);
    }

    buffer->AppendString(string);
}

ECode TimeZone::GetID(
    /* [out] */ String* id)
{
    VALIDATE_NOT_NULL(id);
    *id = mID;
    return NOERROR;
}

ECode TimeZone::GetDSTSavings(
    /* [out] */ Int32* savings)
{
    VALIDATE_NOT_NULL(savings);
    Boolean isUsed;
    UseDaylightTime(&isUsed);
    *savings = isUsed ? 3600000 : 0;
    return NOERROR;
}

ECode TimeZone::GetOffset(
    /* [in] */ Int64 time,
    /* [out] */ Int32* offset)
{
    VALIDATE_NOT_NULL(offset);

    AutoPtr<IDate> date;
    CDate::New(time, (IDate**)&date);
    Boolean isIn;
    InDaylightTime(date, &isIn);
    Int32 oldoffset;
    GetRawOffset(&oldoffset);

    if (isIn) {
        Int32 dst;
        GetDSTSavings(&dst);
        *offset = oldoffset + dst;
    }
    else {
        *offset = oldoffset;
    }
    return NOERROR;
}

ECode TimeZone::GetTimeZone(
    /* [in] */ const String& id,
    /* [out] */ ITimeZone** timeZone)
{
    VALIDATE_NOT_NULL(timeZone);
    *timeZone = NULL;
    if (id.IsNull()) {
        return E_NULL_POINTER_EXCEPTION;
    }

    // Special cases? These can clone an existing instance.
    // TODO: should we just add a cache to ZoneInfoDB instead?
    if (id.GetLength() == 3) {
        if (id.Equals("GMT")) {
            return sGMT->Clone(timeZone);
        }
        if (id.Equals("UTC")) {
            return sUTC->Clone(timeZone);
        }
    }

    // In the database?
    AutoPtr<ITimeZone> zone = ZoneInfoDB::MakeTimeZone(id);
    // Custom time zone?
    if (zone == NULL && id.GetLength() > 3 && id.StartWith("GMT")) {
        zone = GetCustomTimeZone(id);
    }

    // We never return null; on failure we return the equivalent of "GMT".
    if (zone != NULL) {
        *timeZone = zone;
        INTERFACE_ADDREF(*timeZone);
        return NOERROR;
    }

    return sGMT->Clone(timeZone);
}

AutoPtr<ITimeZone> TimeZone::GetCustomTimeZone(
    /* [in] */ const String& id)
{
    AutoPtr<IMatcher> m;
    sCustomZoneIDPattern->Matcher(id, (IMatcher**)&m);
    Boolean isMatches = FALSE;
    m->Matches(&isMatches);
    if (!isMatches) {
        return NULL;
    }

    AutoPtr<ITimeZone> tz;

    Int32 hour = 0;
    Int32 minute = 0;
    String grstr;
    m->Group(1 , &grstr);
    hour = StringUtils::ParseInt32(grstr);
    m->Group(3, &grstr);
    if (!grstr.IsNull()) {
        minute = StringUtils::ParseInt32(grstr);
    }

    if (hour < 0 || hour > 23 || minute < 0 || minute > 59) {
       return NULL;
    }
    Char32 sign = id.GetChar(3);
    Int32 raw = (hour * 3600000) + (minute * 60000);
    if (sign == '-') {
       raw = -raw;
    }

    String cleanId("");
    cleanId.AppendFormat("GMT%c%02d:%02d", sign, hour, minute);
    CSimpleTimeZone::New(raw, cleanId , (ISimpleTimeZone **)&tz);
    return tz;
}

ECode TimeZone::HasSameRules(
    /* [in] */ ITimeZone* timezone,
    /* [out] */ Boolean* hasRules)
{
    VALIDATE_NOT_NULL(timezone);
    VALIDATE_NOT_NULL(hasRules);

    Int32 offset, toffset;
    GetRawOffset(&offset);
    timezone->GetRawOffset(&toffset);
    *hasRules = (offset == toffset);
    return NOERROR;
}

ECode TimeZone::SetDefault(
    /* [in] */ ITimeZone* timeZone)
{
    sDefaultTimeZone = NULL;
    if (timeZone != NULL) {
        timeZone->Clone((ITimeZone**)&sDefaultTimeZone);
    }

    return NOERROR;
}

ECode TimeZone::SetID(
    /* [in] */ const String& id)
{
    if (id.IsNull()) {
        return E_NULL_POINTER_EXCEPTION;
    }

    mID = id;
    return NOERROR;
}

AutoPtr<ITimeZone> TimeZone::GMT()
{
    AutoPtr<ITimeZone> tz;
    sGMT->Clone((ITimeZone**)&tz);
    return tz;
}

AutoPtr<ITimeZone> TimeZone::UTC()
{
    AutoPtr<ITimeZone> tz;
    sUTC->Clone((ITimeZone**)&tz);
    return tz;
}

} // Utility
} // Elastos
