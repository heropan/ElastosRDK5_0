#include "SimpleDateFormat.h"
#include "CDateFormatSymbols.h"
#include "NumberFormat.h"
#include "CGregorianCalendar.h"
#include "CDate.h"
#include "CSimpleTimeZone.h"
#include "CTimeZoneHelper.h"
#include "CInteger64.h"
#include "CInteger32.h"
#include "CAttributedString.h"
#include "CLocaleData.h"
#include "CLocale.h"
#include "TimeZones.h"
#include "CArrayOf.h"

using Elastos::Utility::CGregorianCalendar;
using Elastos::Utility::IGregorianCalendar;
using Elastos::Utility::EIID_IDate;
using Elastos::Utility::IDate;
using Elastos::Utility::CDate;
using Elastos::Utility::ISimpleTimeZone;
using Elastos::Utility::CSimpleTimeZone;
using Elastos::Utility::ITimeZoneHelper;
using Elastos::Utility::CTimeZoneHelper;
using Elastos::Core::EIID_INumber;
using Elastos::Core::IInteger64;
using Elastos::Core::CInteger64;
using Elastos::Core::IInteger32;
using Elastos::Core::CInteger32;
using Elastos::Core::ICharSequence;
using Elastos::Core::EIID_ICharSequence;
using Elastos::Core::CArrayOf;
using Elastos::Core::ICloneable;
using Elastos::Text::IAttributedString;
using Elastos::Text::CAttributedString;
using Elastos::Text::IDecimalFormatSymbols;
using Libcore::ICU::ILocaleData;
using Libcore::ICU::CLocaleData;
using Libcore::ICU::ILocaleDataHelper;
using Libcore::ICU::CLocaleDataHelper;
using Libcore::ICU::ILocale;
using Libcore::ICU::CLocale;
using Libcore::ICU::ILocaleHelper;
using Libcore::ICU::CLocaleHelper;
using Libcore::ICU::ITimeZones;
using Libcore::ICU::TimeZones;

namespace Elastos {
namespace Text {

const String SimpleDateFormat::PATTERN_CHARS("GyMdkHmsSEDFwWahKzZLc");
const Int32 SimpleDateFormat::RFC_822_TIMEZONE_FIELD = 18;
const Int32 SimpleDateFormat::STAND_ALONE_MONTH_FIELD = 19;
const Int32 SimpleDateFormat::STAND_ALONE_DAY_OF_WEEK_FIELD = 20;

ECode SimpleDateFormat::Init()
{
    AutoPtr<ILocaleHelper> pILocaleHelper;
    CLocaleHelper::AcquireSingleton((ILocaleHelper**)&pILocaleHelper);
    AutoPtr<ILocale> pILocale;
    pILocaleHelper->GetDefault((ILocale**)&pILocale);
    Init((ILocale*)pILocale);
    DefaultPattern(&mPattern);
    CDateFormatSymbols::New((ILocale*)pILocale, (IDateFormatSymbols**)&mFormatData);
    return NOERROR;
}

ECode SimpleDateFormat::Init(
    /* [in] */ const String& pattern)
{
    AutoPtr<ILocaleHelper> pILocaleHelper;
    CLocaleHelper::AcquireSingleton((ILocaleHelper**)&pILocaleHelper);
    AutoPtr<ILocale> pILocale;
    pILocaleHelper->GetDefault((ILocale**)&pILocale);
    Init(pattern, (ILocale*)pILocale);
    return NOERROR;
}

ECode SimpleDateFormat::ValidateFormat(
    /* [in] */ Char32 format)
{
    Int32 index = PATTERN_CHARS.IndexOf(format);
    if (index == -1) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return NOERROR;
}

ECode SimpleDateFormat::ValidatePattern(
    /* [in] */ const String& tem)
{
    Boolean quote = FALSE;
    Int32 next, last = -1, count = 0;

    AutoPtr<ArrayOf<Char32> > charArray = tem.GetChars();
    const Int32 patternLength = charArray->GetLength();
    for (Int32 i = 0; i < patternLength; i++) {
        next = (*charArray)[i];
        if (next == '\'') {
            if (count > 0) {
                ValidateFormat((Char32) last);
                count = 0;
            }
            if (last == next) {
                last = -1;
            } else {
                last = next;
            }
            quote = !quote;
            continue;
        }
        if (!quote
                && (last == next || (next >= 'a' && next <= 'z') || (next >= 'A' && next <= 'Z'))) {
            if (last == next) {
                count++;
            } else {
                if (count > 0) {
                    ValidateFormat((Char32) last);
                }
                last = next;
                count = 1;
            }
        } else {
            if (count > 0) {
                ValidateFormat((Char32) last);
                count = 0;
            }
            last = -1;
        }
    }
    if (count > 0) {
        ValidateFormat((Char32) last);
    }

    if (quote) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return NOERROR;
}

ECode SimpleDateFormat::Init(
    /* [in] */ const String& tem,
    /* [in] */ IDateFormatSymbols* value)
{
    AutoPtr<ILocaleHelper> pILocaleHelper;
    CLocaleHelper::AcquireSingleton((ILocaleHelper**)&pILocaleHelper);
    AutoPtr<ILocale> pILocale;
    pILocaleHelper->GetDefault((ILocale**)&pILocale);
    Init((ILocale*)pILocale);
    ValidatePattern(tem);
    mPattern = tem;
    value->Clone((IDateFormatSymbols **)&mFormatData);
    return NOERROR;
}

ECode SimpleDateFormat::Init(
    /* [in] */ const String& tem,
    /* [in] */ ILocale* locale)
{
    FAIL_RETURN(Init(locale));
    ValidatePattern(tem);
    mPattern = tem;
    CDateFormatSymbols::New(locale, (IDateFormatSymbols**)&mFormatData);
    return NOERROR;
}

ECode SimpleDateFormat::Init(
    /* [in] */ ILocale* locale)
{
    FAIL_RETURN(NumberFormat::GetInstance(locale, (INumberFormat**)&mNumberFormat));
    mNumberFormat->SetParseIntegerOnly(TRUE);
    mNumberFormat->SetGroupingUsed(FALSE);
    CGregorianCalendar::New(locale, (IGregorianCalendar**)&mCalendar);
    mCalendar->Add(ICalendar::YEAR, -80);
    mCalendar->Get(ICalendar::YEAR, &mCreationYear);
    mCalendar->GetTime((IDate**)&mDefaultCenturyStart);
    return NOERROR;
}

SimpleDateFormat::~SimpleDateFormat()
{
}

ECode SimpleDateFormat::ApplyLocalizedPattern(
    /* [in] */ const String& tem)
{
    String fromChars;
    mFormatData->GetLocalPatternChars(&fromChars);
    ConvertPattern(tem, fromChars, PATTERN_CHARS, TRUE, &mPattern);
    return NOERROR;
}

ECode SimpleDateFormat::ApplyPattern(
    /* [in] */ const String& tem)
{
    ValidatePattern(tem);
    mPattern = tem;
    return NOERROR;
}

ECode SimpleDateFormat::DefaultPattern(
    /* [out] */ String* pattern)
{
    VALIDATE_NOT_NULL(pattern)
    *pattern = NULL;

    AutoPtr<ILocale> locdef;
    AutoPtr<ILocaleHelper>  lochelper;
    FAIL_RETURN(CLocaleHelper::AcquireSingleton((ILocaleHelper **)&lochelper));
    lochelper->GetDefault((ILocale **)&locdef);
    AutoPtr<ILocaleData> localeData;
    AutoPtr<ILocaleDataHelper> locdatahelper;
    FAIL_RETURN(CLocaleDataHelper::AcquireSingleton((ILocaleDataHelper **)&locdatahelper));
    locdatahelper->Get(locdef,(ILocaleData **)&localeData);
    String datestr , timestr;
    localeData->GetDateFormat(Libcore::ICU::DateFormat_SHORT,&datestr);
    localeData->GetTimeFormat(Libcore::ICU::DateFormat_SHORT,&timestr);
    *pattern = datestr + String(" ") + timestr;
    return NOERROR;
}

ECode SimpleDateFormat::FormatToCharacterIterator(
    /* [in] */ IInterface* object,
    /* [out] */ IAttributedCharacterIterator** charactorIterator)
{
    VALIDATE_NOT_NULL(charactorIterator)
    *charactorIterator = NULL;

    if (object == NULL) {
        return E_NULL_POINTER_EXCEPTION;
    }
    if (object->Probe(EIID_IDate) != NULL) {
        FormatToCharacterIteratorImpl((IDate*) object, charactorIterator);
        return NOERROR;
    }
    if (object->Probe(EIID_INumber) != NULL ) {
        AutoPtr<IDate> pIDate;
        Int64 pValue;
        AutoPtr<INumber> pINumber = reinterpret_cast<INumber*>(object->Probe(EIID_INumber));
        pINumber->Int64Value(&pValue);
        CDate::New(pValue, (IDate**)&pIDate);
        return FormatToCharacterIteratorImpl(pIDate, charactorIterator);
    }
    return E_ILLEGAL_ARGUMENT_EXCEPTION;
}

ECode SimpleDateFormat::FormatToCharacterIteratorImpl(
    /* [in] */ IDate* date,
    /* [out] */ IAttributedCharacterIterator** charactorIterator)
{
    VALIDATE_NOT_NULL(charactorIterator)
    *charactorIterator = NULL;

    StringBuffer buffer;
    List<AutoPtr<IFieldPosition> > fields;

    StringBuffer formattedDate;
    // format the date, and find fields
    FormatImpl(date, &buffer, NULL, &fields, &formattedDate);

    // create and AttributedString with the formatted buffer
    AutoPtr<IAttributedString> as;
    String outstr;
    buffer.SubstringEx(0, buffer.GetLength(),&outstr);
    CAttributedString::New(outstr, (IAttributedString**)&as);

    // add DateFormat field attributes to the AttributedString
    IInterface * object;
    Int32 bi, ei;
    AutoPtr<IFieldPosition> pos;
    List<AutoPtr<IFieldPosition> >::Iterator iter;
    for (iter = fields.Begin(); iter != fields.End(); ++iter) {
        pos = *iter;
        AutoPtr<IFormatField> attribute;
        pos->GetFieldAttribute((IAttributedCharacterIteratorAttribute**)&attribute);
        object = attribute->Probe(EIID_IInterface);
        pos->GetBeginIndex(&bi);
        pos->GetEndIndex(&ei);
        as->AddAttributeEx(attribute, object, bi, ei);
    }

    // return the CharacterIterator from AttributedString
    return as->GetIterator(charactorIterator);
}

ECode SimpleDateFormat::FormatImpl(
    /* [in] */ IDate* date,
    /* [in] */ StringBuffer* buffer,
    /* [in] */ IFieldPosition* field,
    /* [in] */ List< AutoPtr<IFieldPosition> >* fields,
    /* [out] */ StringBuffer* formattedDate)
{
    VALIDATE_NOT_NULL(formattedDate);
    VALIDATE_NOT_NULL(buffer);

    Boolean quote = FALSE;
    Int32 next = 0, last = -1, count = 0;
    mCalendar->SetTime(date);
    if (field != NULL) {
        field->Clear();
    }

    AutoPtr<ArrayOf<Char32> > charArray = mPattern.GetChars();
    const Int32 patternLength = charArray->GetLength();
    for (Int32 i = 0; i < patternLength; i++) {
        next = (*charArray)[i];
        if (next == '\'') {
            if (count > 0) {
                Append(buffer, field, fields, (Char32) last, count);
                count = 0;
            }
            if (last == next) {
                (*buffer) += '\'';
                last = -1;
            } else {
                last = next;
            }
            quote = !quote;
            continue;
        }
        if (!quote && (last == next || (next >= 'a' && next <= 'z') || (next >= 'A' && next <= 'Z'))) {
            if (last == next) {
                count++;
            } else {
                if (count > 0) {
                    Append(buffer, field, fields, (Char32) last, count);
                }
                last = next;
                count = 1;
            }
        } else {
            if (count > 0) {
                Append(buffer, field, fields, (Char32) last, count);
                count = 0;
            }
            last = -1;
            (*buffer) += (Char32)next;
        }
    }
    if (count > 0) {
        Append(buffer, field, fields, (Char32) last, count);
    }
    formattedDate->Reset();
    formattedDate->AppendString( buffer->ToString());
    return NOERROR;
}

ECode SimpleDateFormat::Append(
    /* [in] */ StringBuffer* buffer,
    /* [in] */ IFieldPosition* position,
    /* [in] */ List< AutoPtr<IFieldPosition> >* fields,
    /* [in] */ Char32 format,
    /* [in] */ Int32 count)
{
    Int32 field = -1;
    Int32 index = PATTERN_CHARS.IndexOf(format);
    if (index == -1) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    Int32 beginPosition = buffer->GetLength();
    AutoPtr<IDateFormatField> dateFormatField;
    AutoPtr<ArrayOf<String> > arrayOfStrings1, arrayOfStrings2;
    switch (index) {
        case IDateFormat::ERA_FIELD:
            dateFormatField = DateFormat::Field::ERA;
            Int32 pValue;
            mCalendar->Get(ICalendar::ERA, &pValue);
            mFormatData->GetEras((ArrayOf<String>**)&arrayOfStrings1);
            (*buffer) += (*arrayOfStrings1)[pValue];
        break;
        case IDateFormat::YEAR_FIELD:
            dateFormatField = DateFormat::Field::YEAR;
            Int32 year;
            mCalendar->Get(ICalendar::YEAR, &year);
            /*
             * For 'y' and 'yyy', we're consistent with Unicode and previous releases
             * of Android. But this means we're inconsistent with the RI.
             *     http://unicode.org/reports/tr35/
             */

            if (count == 2) {
                AppendNumber(buffer, 2, year % 100);
            } else {
                AppendNumber(buffer, count, year);
            }
            break;
        case STAND_ALONE_MONTH_FIELD: // L
            dateFormatField = DateFormat::Field::MONTH;
            mFormatData->GetLongStandAloneMonths((ArrayOf<String>**)&arrayOfStrings1);
            mFormatData->GetShortStandAloneMonths((ArrayOf<String>**)&arrayOfStrings2);
            AppendMonth(buffer, count, arrayOfStrings1, arrayOfStrings2);
            break;
        case IDateFormat::MONTH_FIELD: // M
            dateFormatField = DateFormat::Field::MONTH;
            mFormatData->GetMonths((ArrayOf<String>**)&arrayOfStrings1);
            mFormatData->GetShortMonths((ArrayOf<String>**)&arrayOfStrings2);
            AppendMonth(buffer, count, arrayOfStrings1, arrayOfStrings2);
            break;
        case IDateFormat::DATE_FIELD:
            dateFormatField = DateFormat::Field::DAY_OF_MONTH;
            field = ICalendar::DATE;
            break;
        case IDateFormat::HOUR_OF_DAY1_FIELD: // k
            dateFormatField = DateFormat::Field::HOUR_OF_DAY1;
            Int32 hour;
            mCalendar->Get(ICalendar::HOUR_OF_DAY, &hour);
            AppendNumber(buffer, count, hour == 0 ? 24 : hour);
            break;
        case IDateFormat::HOUR_OF_DAY0_FIELD: // H
            dateFormatField = DateFormat::Field::HOUR_OF_DAY0;
            field = ICalendar::HOUR_OF_DAY;
            break;
        case IDateFormat::MINUTE_FIELD:
            dateFormatField = DateFormat::Field::MINUTE;
            field = ICalendar::MINUTE;
            break;
        case IDateFormat::SECOND_FIELD:
            dateFormatField = DateFormat::Field::SECOND;
            field = ICalendar::SECOND;
            break;
        case IDateFormat::MILLISECOND_FIELD:
            dateFormatField = DateFormat::Field::MILLISECOND;
            Int32 value;
            mCalendar->Get(ICalendar::MILLISECOND, &value);
            AppendNumber(buffer, count, value);
            break;
        case STAND_ALONE_DAY_OF_WEEK_FIELD:
            dateFormatField = DateFormat::Field::DAY_OF_WEEK;
            mFormatData->GetLongStandAloneWeekdays((ArrayOf<String>**)&arrayOfStrings1);
            mFormatData->GetShortStandAloneWeekdays((ArrayOf<String>**)&arrayOfStrings2);
            AppendDayOfWeek(buffer, count, arrayOfStrings1, arrayOfStrings2);
            break;
        case IDateFormat::DAY_OF_WEEK_FIELD:
            dateFormatField = DateFormat::Field::DAY_OF_WEEK;
            mFormatData->GetWeekdays((ArrayOf<String>**)&arrayOfStrings1);
            mFormatData->GetShortWeekdays((ArrayOf<String>**)&arrayOfStrings2);
            AppendDayOfWeek(buffer, count, arrayOfStrings1, arrayOfStrings2);
            break;
        case IDateFormat::DAY_OF_YEAR_FIELD:
            dateFormatField = DateFormat::Field::DAY_OF_YEAR;
            field = ICalendar::DAY_OF_YEAR;
            break;
        case IDateFormat::DAY_OF_WEEK_IN_MONTH_FIELD:
            dateFormatField = DateFormat::Field::DAY_OF_WEEK_IN_MONTH;
            field = ICalendar::DAY_OF_WEEK_IN_MONTH;
            break;
        case IDateFormat::WEEK_OF_YEAR_FIELD:
            dateFormatField = DateFormat::Field::WEEK_OF_YEAR;
            field = ICalendar::WEEK_OF_YEAR;
            break;
        case IDateFormat::WEEK_OF_MONTH_FIELD:
            dateFormatField = DateFormat::Field::WEEK_OF_MONTH;
            field = ICalendar::WEEK_OF_MONTH;
            break;
        case IDateFormat::AM_PM_FIELD:
            {
                dateFormatField = DateFormat::Field::AM_PM;
                Int32 pValue2 = 0;
                mCalendar->Get(ICalendar::AM_PM, &pValue2);
                mFormatData->GetAmPmStrings((ArrayOf<String>**)&arrayOfStrings1);

                (*buffer) += (*arrayOfStrings1)[pValue2];
            }
            break;
        case IDateFormat::HOUR1_FIELD: // h
            dateFormatField = DateFormat::Field::HOUR1;
            mCalendar->Get(ICalendar::HOUR, &hour);
            AppendNumber(buffer, count, hour == 0 ? 12 : hour);
            break;
        case IDateFormat::HOUR0_FIELD: // K
            dateFormatField = DateFormat::Field::HOUR0;
            field = ICalendar::HOUR;
            break;
        case IDateFormat::TIMEZONE_FIELD: // z
            dateFormatField = DateFormat::Field::TIME_ZONE;
            AppendTimeZone(buffer, count, TRUE);
            break;
        case RFC_822_TIMEZONE_FIELD: // Z
            dateFormatField = DateFormat::Field::TIME_ZONE;
            AppendNumericTimeZone(buffer, FALSE);
            break;
    }

    if (field != -1) {
        Int32 value;
        mCalendar->Get(field, &value);
        AppendNumber(buffer, count, value);
    }
    if (fields != NULL) {
        AutoPtr<IFieldPosition> newPosition;
        CFieldPosition::New(dateFormatField, (IFieldPosition**)&newPosition);
        newPosition->SetBeginIndex(beginPosition);
        newPosition->SetEndIndex(buffer->GetLength());
        fields->PushBack(newPosition);
    } else {
        // Set to the first occurrence
        AutoPtr<IFormatField> ff;
        position->GetFieldAttribute((IAttributedCharacterIteratorAttribute**)&ff);
        Int32 f, ei;
        position->GetField(&f);
        position->GetEndIndex(&ei);
        if ( (ff == dateFormatField || ff == NULL) && (f == index) && (ei == 0) ) {
            position->SetBeginIndex(beginPosition);
            position->SetEndIndex(buffer->GetLength());
        }
    }

    return NOERROR;
}

ECode SimpleDateFormat::AppendDayOfWeek(
    /* [in] */ StringBuffer* buffer,
    /* [in] */ Int32 count,
    /* [in] */ ArrayOf<String>* longs,
    /* [in] */ ArrayOf<String>* shorts)
{
    Int32 pValue;
    mCalendar->Get(ICalendar::DAY_OF_WEEK, &pValue);
    Boolean isLong = (count > 3);
    (*buffer) += isLong ? (*longs)[pValue] : (*shorts)[pValue];
    return NOERROR;
}

ECode SimpleDateFormat::AppendMonth(
    /* [in] */ StringBuffer* buffer,
    /* [in] */ Int32 count,
    /* [in] */ ArrayOf<String>* longs,
    /* [in] */ ArrayOf<String>* shorts)
{
    Int32 month;
    mCalendar->Get(ICalendar::MONTH, &month);
    if (count <= 2) {
        return AppendNumber(buffer, count, month + 1);
    }

    Boolean isLong = (count > 3);
    (*buffer) += isLong ? (*longs)[month] : (*shorts)[month];
    return NOERROR;
}

ECode SimpleDateFormat::AppendTimeZone(
    /* [in] */ StringBuffer* buffer,
    /* [in] */ Int32 count,
    /* [in] */ Boolean generalTimeZone)
{
    VALIDATE_NOT_NULL(buffer);

    if (generalTimeZone) {
        AutoPtr<ITimeZone> tz;
        mCalendar->GetTimeZone((ITimeZone**)&tz);
        Int32 pValue;
        mCalendar->Get(ICalendar::DST_OFFSET, &pValue);
        Boolean daylight = (pValue != 0) ? TRUE : FALSE;
        Int32 style = (count < 4) ? 0 : 1;
        Boolean customZoneStrings;
        mFormatData->GetCustomZoneStrings(&customZoneStrings);
        if (!customZoneStrings) {
            String name;
            AutoPtr<ILocale> locale;
            mFormatData->GetLocale((ILocale**)&locale);
            tz->GetDisplayNameEx3(daylight, style, (ILocale*)locale, &name);
            (*buffer) += name;
            return NOERROR;
        }
        // We can't call TimeZone.getDisplayName() because it would not use
        // the custom DateFormatSymbols of this SimpleDateFormat.
        String ID;
        tz->GetID(&ID);
        String custom;

        AutoPtr< ArrayOf<IObjectContainer*> > outarray;
        mFormatData->GetZoneStrings((ArrayOf<IObjectContainer*>**)&outarray);
        Int32 length = outarray->GetLength();
        AutoPtr< ArrayOf<IArrayOf*> > outresult = ArrayOf<IArrayOf*>::Alloc(length);
        for (Int32 i = 0; i < length; ++i) {
            AutoPtr<IObjectContainer> newBc = (*outarray)[i];
            Int32 objlen = 0;
            newBc->GetObjectCount(&objlen);
            AutoPtr<IArrayOf> arrstr;
            CArrayOf::New(EIID_ICharSequence, objlen, (IArrayOf**)&arrstr);
            AutoPtr<IObjectEnumerator> oenum;
            newBc->GetObjectEnumerator((IObjectEnumerator**)&oenum);
            Boolean isflag = FALSE;
            Int32 j = 0;
            while (oenum->MoveNext(&isflag), isflag) {
                AutoPtr<IInterface> outface;
                oenum->Current((IInterface**)&outface);
                arrstr->Put(j, outface);
                ++j;
            }
            outresult->Set(i, arrstr);
        }
        TimeZones::GetDisplayName(outresult, ID, daylight, style, &custom);
        if (!custom.IsNull()) {
           buffer->AppendString(custom);
           return NOERROR;
        }
    }
    // We didn't find what we were looking for, so default to a numeric time zone.
    AppendNumericTimeZone(buffer, generalTimeZone);
    return NOERROR;
}

ECode SimpleDateFormat::AppendNumericTimeZone(
    /* [in] */ StringBuffer* buffer,
    /* [in] */ Boolean generalTimeZone)
{
    assert(buffer != NULL);

    Int32 pValue1, pValue2;
    mCalendar->Get(ICalendar::ZONE_OFFSET, &pValue1);
    mCalendar->Get(ICalendar::DST_OFFSET, &pValue2);
    Int32 offset = pValue1 + pValue2;
    Char32 sign = '+';
    if (offset < 0) {
        sign = '-';
        offset = -offset;
    }
    if (generalTimeZone) {
        (*buffer) += "GMT";
    }
    (*buffer) += sign;
    AppendNumber(buffer, 2, offset / 3600000);
    if (generalTimeZone) {
        (*buffer) += ':';
    }
    AppendNumber(buffer, 2, (offset % 3600000) / 60000);
    return NOERROR;
}

ECode SimpleDateFormat::AppendNumber(
    /* [in] */ StringBuffer* buffer,
    /* [in] */ Int32 count,
    /* [in] */ Int32 value)
{
    // TODO: we could avoid using the NumberFormat in most cases for a significant speedup.
    // The only problem is that we expose the NumberFormat to third-party code, so we'd have
    // some work to do to work out when the optimization is valid.
    Int32 minimumIntegerDigits;
    mNumberFormat->GetMinimumIntegerDigits(&minimumIntegerDigits);
    mNumberFormat->SetMinimumIntegerDigits(count);
    AutoPtr<IFieldPosition> fp;
    CFieldPosition::New(0, (IFieldPosition**)&fp);
    String result("");
    AutoPtr<IStringBuffer> outsb;
    FAIL_RETURN(mNumberFormat->FormatInt64Ex(value, buffer, fp, (IStringBuffer **)&outsb));
    outsb->ToString(&result);
    mNumberFormat->SetMinimumIntegerDigits(minimumIntegerDigits);
    buffer->Reset();
    buffer->AppendString(result);
    return NOERROR;
}

ECode SimpleDateFormat::Error(
    /* [in] */ IParsePosition* position,
    /* [in] */ Int32 offset,
    /* [in] */ ITimeZone* zone,
    /* [out] */ IDate** date)
{
    VALIDATE_NOT_NULL(date)

    position->SetErrorIndex(offset);
    mCalendar->SetTimeZone(zone);
    *date = NULL;
    return NOERROR;
}

ECode SimpleDateFormat::FormatDateEx(
    /* [in] */ IDate* date,
    /* [in] */ IStringBuffer * buffer,
    /* [in] */ IFieldPosition* fieldPos,
    /* [out] */ IStringBuffer ** formatString)
{
    VALIDATE_NOT_NULL(formatString)
    *formatString = NULL;
    VALIDATE_NOT_NULL(buffer)

    // Harmony delegates to ICU's SimpleDateFormat, we implement it directly
    StringBuffer result;
    FormatImpl(date, (StringBuffer *)buffer, fieldPos, NULL, &result);
    String str;
    result.ToString(&str);
    AutoPtr<IStringBuffer> outsb = new StringBuffer(str);
    *formatString = outsb;
    INTERFACE_ADDREF(*formatString);
    return NOERROR;
}

ECode SimpleDateFormat::Get2DigitYearStart(
    /* [out] */ IDate** start)
{
    VALIDATE_NOT_NULL(start)

    return ICloneable::Probe(mDefaultCenturyStart)->Clone((IInterface**)start);
}

ECode SimpleDateFormat::GetDateFormatSymbols(
    /* [out] */ IDateFormatSymbols** symbols)
{
    VALIDATE_NOT_NULL(symbols)

    return  mFormatData->Clone(symbols);
}

ECode SimpleDateFormat::Parse(
    /* [in] */ const String& string,
    /* [in] */ Int32 offset,
    /* [in] */ Char32 format,
    /* [in] */ Int32 count,
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    Int32 index = PATTERN_CHARS.IndexOf(format);
    if (index == -1) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    Int32 field = -1;
    // TODO: what's 'absolute' for? when is 'count' negative, and why?
    Int32 absolute = 0;
    if (count < 0) {
        count = -count;
        absolute = count;
    }
    AutoPtr<IParsePosition> position;
    AutoPtr<INumber> result;
    Int32 errorIndex;
    AutoPtr<ArrayOf<String> > array1, array2;
    switch (index) {
        case IDateFormat::ERA_FIELD:
            mFormatData->GetEras((ArrayOf<String>**)&array1);
            return ParseText(string, offset, array1, ICalendar::ERA, value);
        case IDateFormat::YEAR_FIELD:
            if (count >= 3) {
                field = ICalendar::YEAR;
            } else {
                CParsePosition::New(offset, (IParsePosition**)&position);
                ParseNumber(absolute, string, position, (INumber**)&result);
                if (result == NULL) {
                    position->GetErrorIndex(&errorIndex);
                    *value = -errorIndex - 1;
                    return NOERROR;
                }
                Int32 year;
                result->Int32Value(&year);
                // A two digit year must be exactly two digits, i.e. 01
                position->GetIndex(&index);
                if ((index - offset) == 2 && year >= 0) {
                    year += mCreationYear / 100 * 100;
                    if (year < mCreationYear) {
                        year += 100;
                    }
                }
                mCalendar->Set(ICalendar::YEAR, year);
                position->GetIndex(&index);
                *value = index;
                return NOERROR;
            }
            break;
        case STAND_ALONE_MONTH_FIELD:// L
            mFormatData->GetLongStandAloneMonths((ArrayOf<String>**)&array1);
            mFormatData->GetShortStandAloneMonths((ArrayOf<String>**)&array2);
            return ParseMonth(string, offset, count, absolute, array1, array2, value);
        case IDateFormat::MONTH_FIELD:// M
            mFormatData->GetMonths((ArrayOf<String>**)&array1);
            mFormatData->GetShortMonths((ArrayOf<String>**)&array2);
            return ParseMonth(string, offset, count, absolute, array1, array2, value);
        case IDateFormat::DATE_FIELD:
            field = ICalendar::DATE;
            break;
        case IDateFormat::HOUR_OF_DAY1_FIELD:// k
            CParsePosition::New(offset, (IParsePosition**)&position);
            ParseNumber(absolute, string, position, (INumber**)&result);
            if (result == NULL) {
                position->GetErrorIndex(&errorIndex);
                *value = -errorIndex - 1;
                return NOERROR;
            }
            Int32 hour;
            result->Int32Value(&hour);
            if (hour == 24) {
                hour = 0;
            }
            mCalendar->Set(ICalendar::HOUR_OF_DAY, hour);
            position->GetIndex(&index);
            *value = index;
            return NOERROR;
        case IDateFormat::HOUR_OF_DAY0_FIELD:// H
            field = ICalendar::HOUR_OF_DAY;
            break;
        case IDateFormat::MINUTE_FIELD:
            field = ICalendar::MINUTE;
            break;
        case IDateFormat::SECOND_FIELD:
            field = ICalendar::SECOND;
            break;
        case IDateFormat::MILLISECOND_FIELD:
            field = ICalendar::MILLISECOND;
            break;
        case STAND_ALONE_DAY_OF_WEEK_FIELD:
            mFormatData->GetLongStandAloneWeekdays((ArrayOf<String>**)&array1);
            mFormatData->GetShortStandAloneWeekdays((ArrayOf<String>**)&array2);
            return ParseDayOfWeek(string, offset, array1, array2, value);
        case IDateFormat::DAY_OF_WEEK_FIELD:
            mFormatData->GetWeekdays((ArrayOf<String>**)&array1);
            mFormatData->GetShortWeekdays((ArrayOf<String>**)&array2);
            return ParseDayOfWeek(string, offset, array1, array2, value);
        case IDateFormat::DAY_OF_YEAR_FIELD:
            field = ICalendar::DAY_OF_YEAR;
            break;
        case IDateFormat::DAY_OF_WEEK_IN_MONTH_FIELD:
            field = ICalendar::DAY_OF_WEEK_IN_MONTH;
            break;
        case IDateFormat::WEEK_OF_YEAR_FIELD:
            field = ICalendar::WEEK_OF_YEAR;
            break;
        case IDateFormat::WEEK_OF_MONTH_FIELD:
            field = ICalendar::WEEK_OF_MONTH;
            break;
        case IDateFormat::AM_PM_FIELD:
            mFormatData->GetAmPmStrings((ArrayOf<String>**)&array1);
            return ParseText(string, offset, array1, ICalendar::AM_PM, value);
        case IDateFormat::HOUR1_FIELD:// h
            CParsePosition::New(offset, (IParsePosition**)&position);
            ParseNumber(absolute, string, position, (INumber**)&result);
            if (result == NULL) {
                position->GetErrorIndex(&errorIndex);
                *value = -errorIndex - 1;
                return NOERROR;
            }
            result->Int32Value(&hour);
            if (hour == 12) {
                hour = 0;
            }
            mCalendar->Set(ICalendar::HOUR, hour);
            position->GetIndex(&index);
            *value = index;
            return NOERROR;
        case IDateFormat::HOUR0_FIELD:// K
            field = ICalendar::HOUR;
            break;
        case IDateFormat::TIMEZONE_FIELD:// z
            return ParseTimeZone(string, offset, value);
        case RFC_822_TIMEZONE_FIELD:// Z
            return ParseTimeZone(string, offset, value);
    }
    if (field != -1) {
        return ParseNumber(absolute, string, offset, field, 0, value);
    }
    *value = offset;
    return NOERROR;
}

ECode SimpleDateFormat::ParseDayOfWeek(
    /* [in] */ const String& string,
    /* [in] */ Int32 offset,
    /* [in] */ ArrayOf<String>* longs,
    /* [in] */ ArrayOf<String>* shorts,
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    Int32 index;
    ParseText(string, offset, longs, ICalendar::DAY_OF_WEEK, &index);
    if (index < 0) {
        ParseText(string, offset, shorts, ICalendar::DAY_OF_WEEK, &index);
    }
    *value = index;
    return NOERROR;
}

ECode SimpleDateFormat::ParseMonth(
    /* [in] */ const String& string,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count,
    /* [in] */ Int32 absolute,
    /* [in] */ ArrayOf<String>* longs,
    /* [in] */ ArrayOf<String>* shorts,
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    if (count <= 2) {
        return ParseNumber(absolute, string, offset, ICalendar::MONTH, -1, value);
    }
    Int32 index = 0;
    ParseText(string, offset, longs, ICalendar::MONTH, &index);
    if (index < 0) {
        ParseText(string, offset, shorts, ICalendar::MONTH, &index);
    }
    *value = index;
    return NOERROR;
}

ECode SimpleDateFormat::ParseEx(
    /* [in] */ const String& string,
    /* [in] */ IParsePosition* position,
    /* [out] */ IDate** date)
{
    VALIDATE_NOT_NULL(date)
    // Harmony delegates to ICU's SimpleDateFormat, we implement it directly
    Boolean quote = FALSE;
    Int32 next, last = -1, count = 0, offset;
    position->GetIndex(&offset);
    AutoPtr<ArrayOf<Char32> > strCharArray = string.GetChars();
    Int32 length = strCharArray->GetLength();
    mCalendar->Clear();
    AutoPtr<ITimeZone> zone;
    mCalendar->GetTimeZone((ITimeZone**)&zone);

    AutoPtr<ArrayOf<Char32> > charArray = mPattern.GetChars();
    const Int32 patternLength = charArray->GetLength();
    for (Int32 i = 0; i < patternLength; i++) {
        next = (*charArray)[i];
        if (next == '\'') {
            if (count > 0) {
                Int32 v;
                Parse(string, offset, (Char32) last, count, &v);
                if ((offset = v) < 0) {
                    return Error(position, -offset - 1, zone, date);
                }
                count = 0;
            }
            if (last == next) {
                if (offset >= length || (*strCharArray)[offset] != '\'') {
                    return Error(position, offset, zone, date);
                }
                offset++;
                last = -1;
            } else {
                last = next;
            }
            quote = !quote;
            continue;
        }
        if (!quote
                && (last == next || (next >= 'a' && next <= 'z') || (next >= 'A' && next <= 'Z'))) {
            if (last == next) {
                count++;
            } else {
                if (count > 0) {
                    Int32 v;
                    Parse(string, offset, (Char32) last, -count, &v);
                    if ((offset = v) < 0) {
                        return Error(position, -offset - 1, zone, date);
                    }
                }
                last = next;
                count = 1;
            }
        } else {
            if (count > 0) {
                Int32 v;
                Parse(string, offset, (Char32) last, count, &v);
                if ((offset = v) < 0) {
                    return Error(position, -offset - 1, zone, date);
                }
                count = 0;
            }
            last = -1;
            if (offset >= length || (*strCharArray)[offset] != next) {
                return Error(position, offset, (ITimeZone*)zone, date);
            }
            offset++;
        }
    }
    if (count > 0) {
        Int32 v;
        Parse(string, offset, (Char32) last, count, &v);
        if ((offset = v) < 0) {
            return Error(position, -offset - 1, (ITimeZone*)zone, date);
        }
    }
    AutoPtr<IDate> d;
    ECode ec = mCalendar->GetTime((IDate**)&d);
    if (ec != NOERROR)
    {
        return Error(position, offset, (ITimeZone *)zone ,date);
    }
    position->SetIndex(offset);
    mCalendar->SetTimeZone(zone);
    *date = d;
    INTERFACE_ADDREF(*date);
    return NOERROR;
}

ECode SimpleDateFormat::ParseNumber(
    /* [in] */ Int32 max,
    /* [in] */ const String& string,
    /* [in] */ IParsePosition* position,
    /* [out] */ INumber** number)
{
    VALIDATE_NOT_NULL(number)

    AutoPtr<ArrayOf<Char32> > charArray = string.GetChars();
    Int32 digit, length = charArray->GetLength(), result = 0;
    Int32 index;
    position->GetIndex(&index);
    if (max > 0 && max < length - index) {
        length = index + max;
    }
    while (index < length
            && ((*charArray)[index] == ' ' || (*charArray)[index] == '\t')) {
        ++index;
    }
    if (max == 0) {
        position->SetIndex(index);
        AutoPtr<INumber> n;
        mNumberFormat->ParseEx(string, position, (INumber **)&n);

        // In RTL locales, NumberFormat might have parsed "2012-" in an ISO date as the
        // negative number -2012.
        // Ideally, we wouldn't have this broken API that exposes a NumberFormat and expects
        // us to use it. The next best thing would be a way to ask the NumberFormat to parse
        // positive numbers only, but icu4c supports negative (BCE) years. The best we can do
        // is try to recognize when icu4c has done this, and undo it.
        Int64 val = 0;
        if (n != NULL && (n->Int64Value(&val) ,val < 0)) {
            if (mNumberFormat->Probe(EIID_IDecimalFormat) != NULL) {
                AutoPtr<IDecimalFormat> df = (IDecimalFormat *) mNumberFormat->Probe(EIID_IDecimalFormat);
                Int32 indnum = 0;
                position->GetIndex(&indnum);
                Char32 lastChar = string.GetChar(indnum - 1);
                AutoPtr<IDecimalFormatSymbols> structdf;
                df->GetDecimalFormatSymbols((IDecimalFormatSymbols **)&structdf);
                Char32 minusSign = 0;
                structdf->GetMinusSign(&minusSign);
                if (lastChar == minusSign) {
                    Int64 lvalue = 0;
                    n->Int64Value(&lvalue);
                    lvalue = -lvalue;
                    n = NULL;
                    CInteger64::New(lvalue,(IInteger64 **)&n);
                    // n = Long.valueOf(-n.longValue()); // Make the value positive.
                    position->SetIndex(indnum - 1); // Spit out the negative sign.
                }
            }
        }
        *number = n;
        INTERFACE_ADDREF(*number);
        return NOERROR;
    }

    result = 0;
    digit = 0;
    AutoPtr<ArrayOf<Char32> > char32Array = string.GetChars();
    while (index < length
            && (digit = Character::ToDigit((*char32Array)[index], 10)) != -1) {
        result = result * 10 + digit;
        ++index;
    }
    Int32 i;
    position->GetIndex(&i);
    if (index == i) {
        position->SetErrorIndex(index);
        *number = NULL;
        return NOERROR;
    }
    position->SetIndex(index);
    AutoPtr<INumber> outn;
    CInteger32::New(result,(IInteger32 **)&outn);
    *number = outn;
    INTERFACE_ADDREF(*number);
    //return Integer.valueOf(result);
    return NOERROR;
}

ECode SimpleDateFormat::ParseNumber(
    /* [in] */ Int32 max,
    /* [in] */ const String& string,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 field,
    /* [in] */ Int32 skew,
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    AutoPtr<IParsePosition> position;
    CParsePosition::New(offset, (IParsePosition**)&position);
    AutoPtr<INumber> result;
    ParseNumber(max, string, position, (INumber**)&result);
    if (result == NULL) {
        position->GetErrorIndex(value);
        *value = -(*value) - 1;
        return NOERROR;
    }
    Int32 v;
    result->Int32Value(&v);
    mCalendar->Set(field, v + skew);
    return position->GetIndex(value);
}

ECode SimpleDateFormat::ParseText(
    /* [in] */ const String& string,
    /* [in] */ Int32 offset,
    /* [in] */ ArrayOf<String>* text,
    /* [in] */ Int32 field,
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);

    Int32 found = -1;
    for (Int32 i = 0; i < text->GetLength(); i++) {
        if ((*text)[i].IsEmpty()) {
            continue;
        }
        if ( string.Substring( offset, (*text)[i].GetLength() + offset )
                .EqualsIgnoreCase( (*text)[i].Substring(0, (*text)[i].GetLength() ) ) ) {
            // Search for the longest match, in case some fields are subsets
            if (found == -1 || (*text)[i].GetLength() > (*text)[found].GetLength()) {
                found = i;
            }
        }
    }
    if (found != -1) {
        mCalendar->Set(field, found);
        *value = offset + (*text)[found].GetLength();
        return NOERROR;
    }
    *value = -offset - 1;
    return NOERROR;
}

AutoPtr<ArrayOf<String> > SimpleDateFormat::ObjectContainerToStringArray(
    /* [in] */ IObjectContainer* bc)
{
    if (bc == NULL)
        return NULL;

    Int32 count;
    bc->GetObjectCount(&count);

    AutoPtr<ArrayOf<String> > array = ArrayOf<String>::Alloc(count);

    AutoPtr<IObjectEnumerator> enumerator;
    bc->GetObjectEnumerator((IObjectEnumerator**)&enumerator);
    Boolean hasNext = FALSE;
    Int32 i = 0;
    while (enumerator->MoveNext(&hasNext), hasNext) {
        AutoPtr<ICharSequence> seq;
        enumerator->Current((IInterface**)&seq);
        String str;
        seq->ToString(&str);

        (*array)[i++] = str;
    }

    return array;
}

ECode SimpleDateFormat::ParseTimeZone(
    /* [in] */ const String& string,
    /* [in] */ Int32 offset,
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    Boolean foundGMT;
    String str("GMT");
    if (string.Substring(offset, 3).EqualsIgnoreCase(str.Substring(0, 3))) {
        foundGMT = TRUE;
    } else {
        foundGMT = FALSE;
    }
    if (foundGMT) {
        offset += 3;
    }
    AutoPtr<ArrayOf<Char32> > charArray = string.GetChars();
    Char32 sign;
    if (offset < charArray->GetLength()
            && ((sign = (*charArray)[offset]) == '+' || sign == '-')) {
        AutoPtr<IParsePosition> position;
        CParsePosition::New(offset + 1, (IParsePosition**)&position);
        AutoPtr<INumber> result;
        mNumberFormat->ParseEx(string, position, (INumber**)&result);
        if (result == NULL) {
            Int32 ei;
            position->GetErrorIndex(&ei);
            *value = -ei - 1;
            return NOERROR;
        }
        Int32 hour;
        result->Int32Value(&hour);
        Int32 raw = hour * 3600000;
        Int32 index;
        position->GetIndex(&index);
        if (index < charArray->GetLength() && (*charArray)[index] == ':') {
            position->SetIndex(index + 1);
            result = NULL;
            mNumberFormat->ParseEx(string, position, (INumber**)&result);
            if (result == NULL) {
                Int32 ei;
                position->GetErrorIndex(&ei);
                *value = -ei - 1;
                return NOERROR;
            }
            Int32 minute;
            result->Int32Value(&minute);
            raw += minute * 60000;
        } else if (hour >= 24) {
            raw = (hour / 100 * 3600000) + (hour % 100 * 60000);
        }
        if (sign == '-') {
            raw = -raw;
        }
        AutoPtr<ISimpleTimeZone> stz;
        CSimpleTimeZone::New(raw, String(""), (ISimpleTimeZone**)&stz);
        mCalendar->SetTimeZone(stz);
        position->GetIndex(value);
        return NOERROR;
    }
    if (foundGMT) {
        AutoPtr<ITimeZoneHelper> tzh;
        CTimeZoneHelper::AcquireSingleton((ITimeZoneHelper**)&tzh);
        AutoPtr<ITimeZone> tz;
        tzh->GetTimeZone(String("GMT"), (ITimeZone**)&tz);
        mCalendar->SetTimeZone(tz);
        *value = offset;
        return NOERROR;
    }

    AutoPtr<ArrayOf<IObjectContainer*> > zones = ((CDateFormatSymbols *)mFormatData.Get())->InternalZoneStrings();
    for (Int32 i = 0; i < zones->GetLength(); ++i) {
        AutoPtr<IObjectContainer> bc = (*zones)[i];
        if (!bc) continue;

        AutoPtr<ArrayOf<String> > element = ObjectContainerToStringArray(bc);
        for (Int32 j = ITimeZones::LONG_NAME; j < ITimeZones::NAME_COUNT; ++j) {
            String zoneStr = (*element)[j];
            if (string.RegionMatchesIgnoreCase(offset, zoneStr, 0, zoneStr.GetLength())) {
                AutoPtr<ITimeZoneHelper> tzh;
                CTimeZoneHelper::AcquireSingleton((ITimeZoneHelper**)&tzh);
                AutoPtr<ITimeZone> zone;
                tzh->GetTimeZone((*element)[ITimeZones::OLSON_NAME], (ITimeZone**)&zone);
                if (zone == NULL) {
                    *value = -offset - 1;
                    return NOERROR;
                }
                Int32 raw;
                zone->GetRawOffset(&raw);
                Boolean isUsed;
                zone->UseDaylightTime(&isUsed);
                if (j == ITimeZones::LONG_NAME_DST || j == ITimeZones::SHORT_NAME_DST) {
                    // Not all time zones use a one-hour difference, so we need to query
                    // the TimeZone. (Australia/Lord_Howe is the usual example of this.)
                    Int32 dstSavings;
                    zone->GetDSTSavings(&dstSavings);
                    // One problem with TimeZone.getDSTSavings is that it will return 0 if the
                    // time zone has stopped using DST, even if we're parsing a date from
                    // the past. In that case, assume the default.
                    if (dstSavings == 0) {
                        // TODO: we should change this to use TimeZone.getOffset(long),
                        // but that requires the complete date to be parsed first.
                        dstSavings = 3600000;
                    }
                    raw += dstSavings;
                }
                AutoPtr<ISimpleTimeZone> stz;
                CSimpleTimeZone::New(raw, String(""), (ISimpleTimeZone**)&stz);
                mCalendar->SetTimeZone(stz);
                *value = offset + zoneStr.GetLength();
                return NOERROR;
            }
        }
    }
    *value = -offset - 1;
    return NOERROR;
}

ECode SimpleDateFormat::Set2DigitYearStart(
    /* [in] */ IDate* date)
{
    mDefaultCenturyStart = NULL;
    FAIL_RETURN (ICloneable::Probe(date)->Clone((IInterface**)&mDefaultCenturyStart));
    AutoPtr<ICalendar> cal;
    CGregorianCalendar::New((IGregorianCalendar**)&cal);
    cal->SetTime((IDate*)mDefaultCenturyStart);
    mCreationYear = NULL;
    cal->Get(ICalendar::YEAR, &mCreationYear);
    return NOERROR;
}

ECode SimpleDateFormat::SetDateFormatSymbols(
    /* [in] */ IDateFormatSymbols* value)
{
    mFormatData = NULL;
    return value->Clone((IDateFormatSymbols **)&mFormatData);
}

ECode SimpleDateFormat::ToLocalizedPattern(
    /* [out] */ String* patterns)
{
    String localPatternChars;
    mFormatData->GetLocalPatternChars(&localPatternChars);
    return ConvertPattern(mPattern, PATTERN_CHARS, localPatternChars, FALSE, patterns);
}

ECode SimpleDateFormat::ConvertPattern(
    /* [in] */ const String& tem,
    /* [in] */ const String& fromChars,
    /* [in] */ const String& toChars,
    /* [in] */ Boolean check,
    /* [out] */ String* patterns)
{
    VALIDATE_NOT_NULL(patterns);
    if (!check && fromChars.Equals(toChars)) {
        *patterns = tem;
        return NOERROR;
    }

    Boolean quote = FALSE;
    StringBuffer output("");
    AutoPtr<ArrayOf<Char32> > charArray = tem.GetChars();
    AutoPtr<ArrayOf<Char32> > toCharArray = toChars.GetChars();
    Int32 length = charArray->GetLength();
    Int32 index;
    Char32 next;
    for (Int32 i = 0; i < length; i++) {
        next = (*charArray)[i];
        if (next == '\'') {
            quote = !quote;
        }
        if (!quote && (index = fromChars.IndexOf(next)) != -1) {
            output += (*toCharArray)[index];
        } else if (check && !quote && ((next >= 'a' && next <= 'z') || (next >= 'A' && next <= 'Z'))) {
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
        } else {
            output += next;
        }
    }
    if (quote) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    output.SubstringEx(0, output.GetLength(), patterns);
    return NOERROR;
}

ECode SimpleDateFormat::ToPattern(
    /* [out] */ String* pattern)
{
    VALIDATE_NOT_NULL(pattern);
    *pattern = mPattern;
    return NOERROR;
}

ECode SimpleDateFormat::Equals(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)

    if (this->Probe(EIID_ISimpleDateFormat) == ISimpleDateFormat::Probe(object)) {
        *result = TRUE;
        return NOERROR;
    }
    if (object->Probe(EIID_ISimpleDateFormat) == NULL) {
        *result = FALSE;
        return NOERROR;
    }

    AutoPtr<ISimpleDateFormat> simple = ISimpleDateFormat::Probe(object);
    Boolean res1 = FALSE, res2 = FALSE, res3 = FALSE;
    DateFormat::Equals(object, &res1);

    String p;
    simple->ToPattern(&p);
    res2 = mPattern.Equals(p);

    AutoPtr<IDateFormatSymbols> dfs;
    simple->GetDateFormatSymbols((IDateFormatSymbols**)&dfs);

    mFormatData->Equals(dfs, &res3);
    *result = res1 && res2 && res3;

    return NOERROR;
}

} // namespace Text
} // namespace Elastos
