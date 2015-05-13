
#ifndef __SQL_DATE_H__
#define __SQL_DATE_H__

#include <cmdef.h>
#include <elastos.h>
#include <elautoptr.h>
#include <elastos/StringBuilder.h>

using Elastos::Core::StringBuilder;

namespace Elastos {
namespace Sql {

class Date
{
public:
    Date();

    Date(
        /* [in] */ Int32 year,
        /* [in] */ Int32 month,
        /* [in] */ Int32 day);

    Date(
        /* [in] */ Int32 year,
        /* [in] */ Int32 month,
        /* [in] */ Int32 day,
        /* [in] */ Int32 hour,
        /* [in] */ Int32 minute);

    Date(
        /* [in] */ Int32 year,
        /* [in] */ Int32 month,
        /* [in] */ Int32 day,
        /* [in] */ Int32 hour,
        /* [in] */ Int32 minute,
        /* [in] */ Int32 second);

    Date(
        /* [in] */ Int64 milliseconds);

    Date(
        /* [in] */ const String& string);

    CARAPI_(Boolean) IsAfter(
        /* [in] */ Elastos::Utility::IDate* date);

    CARAPI_(Boolean) IsBefore(
        /* [in] */ Elastos::Utility::IDate* date);

    CARAPI_(AutoPtr<Elastos::Utility::IDate>) Clone();

    CARAPI_(Int32) CompareTo(
        /* [in] */ Elastos::Utility::IDate* date);

    CARAPI_(Boolean) EqualsEx(
        /* [in] */ Elastos::Utility::IDate* date);

    CARAPI_(Boolean) Equals(
        /* [in] */ IInterface* date);

    CARAPI_(Int32) GetDate();

    CARAPI_(Int32) GetDay();

    CARAPI_(Int32) GetHours();

    CARAPI_(Int32) GetMinutes();

    CARAPI_(Int32) GetMonth();

    CARAPI_(Int32) GetSeconds();

    CARAPI_(Int64) GetTime();

    CARAPI_(Int32) GetTimezoneOffset();

    CARAPI_(Int32) GetYear();

    CARAPI SetDate(
        /* [in] */ Int32 day);

    CARAPI SetHours(
        /* [in] */ Int32 hour);

    CARAPI SetMinutes(
        /* [in] */ Int32 minute);

    CARAPI SetMonth(
        /* [in] */ Int32 month);

    CARAPI SetSeconds(
        /* [in] */ Int32 second);

    CARAPI SetTime(
        /* [in] */ Int64 milliseconds);

    CARAPI SetYear(
        /* [in] */ Int32 year);

    CARAPI_(String) ToGMTString();

    CARAPI_(String) ToLocaleString();

    CARAPI_(String) ToString();

    CARAPI_(void) Init(
        /* [in] */ Int32 year,
        /* [in] */ Int32 month,
        /* [in] */ Int32 day);

    CARAPI_(void) InitEx(
        /* [in] */ Int64 milliseconds);

    CARAPI_(void) InitEx2(
        /* [in] */ Int32 year,
        /* [in] */ Int32 month,
        /* [in] */ Int32 day,
        /* [in] */ Int32 hour,
        /* [in] */ Int32 minute,
        /* [in] */ Int32 second);

public:

    static CARAPI Parse(
        /* [in] */ const String& string,
        /* [out] */ Int64* value);

    static CARAPI_(Int64) UTC(
        /* [in] */ Int32 year,
        /* [in] */ Int32 month,
        /* [in] */ Int32 day,
        /* [in] */ Int32 hour,
        /* [in] */ Int32 minute,
        /* [in] */ Int32 second);

private:

    static CARAPI_(void) AppendTwoDigits(
        /* [in] */ StringBuilder& sb,
        /* [in] */ Int32 n);

    static CARAPI_(Int32) Zone(
        /* [in] */ const String& text);

    static CARAPI_(Int32) Parse(
        /* [in] */ const String& string,
        /* [in] */ ArrayOf<String> const& array);

    static CARAPI_(Int32) GetCreationYear();
private:
    // Used by parse()
    static Int32 sCreationYear;

    Int64 mMilliseconds;
};

} // namespace Sql
} // namespace Elastos

#endif // __SQL_DATE_H__
