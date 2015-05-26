
#include "coredef.h"
#include "CDatabaseHelper.h"
#include "Database.h"

namespace Elastos {
namespace Sql {
namespace SQLite {

CAR_SINGLETON_IMPL(CDatabaseHelper, Object, IDatabaseHelper);

ECode CDatabaseHelper::Complete(
    /* [in] */ const String& sql,
    /* [out] */ Boolean * value)
{
    *value = Database::Complete(sql);
    return NOERROR;
}

ECode CDatabaseHelper::Version(
    /* [out] */ String * str)
{
    *str = Database::Version();
    return NOERROR;
}

ECode CDatabaseHelper::ErrorString(
    /* [in] */ Int32 error_code,
    /* [out] */ String * str)
{
    *str = Database::ErrorString(error_code);
    return NOERROR;
}

ECode CDatabaseHelper::Status(
    /* [in] */ Int32 op,
    /* [in] */ const ArrayOf<Int32>& info,
    /* [in] */ Boolean flag,
    /* [out] */ Int32 * value)
{
    *value = Database::Status(op,(ArrayOf<Int32>* )&info,flag);
    return NOERROR;
}

ECode CDatabaseHelper::LongFromJulian(
    /* [in] */ Double d,
    /* [out] */ Int64 * value)
{
    *value = Database::LongFromJulian(d);
    return NOERROR;
}

ECode CDatabaseHelper::LongFromJulian(
    /* [in] */ const String& s ,
    /* [out] */ Int64 * value)
{
    *value = Database::LongFromJulian(s);
    return NOERROR;
}

ECode CDatabaseHelper::JulianFromLong(
    /* [in] */ Int64 ms,
    /* [out] */ Double * value)
{
    *value = Database::JulianFromLong(ms);
    return NOERROR;
}

} // namespace SQLite
} // namespace Sql
} // namespace Elastos
