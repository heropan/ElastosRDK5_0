
#ifndef __CSQL_CDATE_H__
#define __CSQL_CDATE_H__

#include "_Elastos_Sql_CSqlDate.h"
#include "Date.h"

using Elastos::Utility::Date;

namespace Elastos {
namespace Sql {

CarClass(CSqlDate)
    , public Date
{
public:
    CAR_INTERFACE_DECL();

    CAR_OBJECT_DECL();

    CARAPI GetHours(
        /* [out] */ Int32 * value);

    CARAPI GetMinutes(
        /* [out] */ Int32 * value);

    CARAPI GetSeconds(
        /* [out] */ Int32 * value);

    CARAPI SetHours(
        /* [in] */ Int32 hour);

    CARAPI SetMinutes(
        /* [in] */ Int32 minute);

    CARAPI SetSeconds(
        /* [in] */ Int32 second);

    CARAPI SetTime(
        /* [in] */ Int64 milliseconds);

    CARAPI ToString(
        /* [out] */ String * str);

    CARAPI constructor(
        /* [in] */ Int32 theYear,
        /* [in] */ Int32 theMonth,
        /* [in] */ Int32 theDay);

    CARAPI constructor(
        /* [in] */ Int64 theDate);

public:
    static CARAPI_(AutoPtr<ISQLDate>) ValueOf(
        /* [in] */ const String& dateString);

private:
    CARAPI_(void) Format(
        /* [in] */ Int32 date,
        /* [in] */ Int32 digits,
        /* [in] */ StringBuilder * sb);

private:
    static CARAPI_(Int64) NormalizeTime(
        /* [in] */ Int64 theTime);

private:
    const static String PADDING;
};

} // namespace Sql
} // namespace Elastos

#endif // __CSQL_CDATE_H__
