
#include "ext/frameworkdef.h"
#include "elastos/droid/text/format/CDateFormat.h"

namespace Elastos {
namespace Droid {
namespace Text {
namespace Format {

CAR_INTERFACE_IMPL(CDateFormat, Singleton, IDateFormat)

CAR_SINGLETON_IMPL(CDateFormat)

ECode CDateFormat::Is24HourFormat(
    /* [in] */ IContext* context,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = DateFormat::Is24HourFormat(context);
    return NOERROR;
}

ECode CDateFormat::GetTimeFormat(
    /* [in] */ IContext* context,
    /* [out] */ Elastos::Text::IDateFormat** format)
{
    VALIDATE_NOT_NULL(format);
    AutoPtr<Elastos::Text::IDateFormat> f = DateFormat::GetTimeFormat(context);
    *format = f;
    REFCOUNT_ADD(*format);
    return NOERROR;
}

ECode CDateFormat::GetDateFormat(
    /* [in] */ IContext* context,
    /* [out] */ Elastos::Text::IDateFormat** format)
{
    VALIDATE_NOT_NULL(format);
    AutoPtr<Elastos::Text::IDateFormat> f = DateFormat::GetDateFormat(context);
    *format = f;
    REFCOUNT_ADD(*format);
    return NOERROR;
}

ECode CDateFormat::GetDateFormatForSetting(
    /* [in] */ IContext* context,
    /* [in] */ const String& value,
    /* [out] */ Elastos::Text::IDateFormat** format)
{
    VALIDATE_NOT_NULL(format);
    AutoPtr<Elastos::Text::IDateFormat> f = DateFormat::GetDateFormatForSetting(context, value);
    *format = f;
    REFCOUNT_ADD(*format);
    return NOERROR;
}

ECode CDateFormat::GetLongDateFormat(
    /* [in] */ IContext* context,
    /* [out] */ Elastos::Text::IDateFormat** format)
{
    VALIDATE_NOT_NULL(format);
    AutoPtr<Elastos::Text::IDateFormat> f = DateFormat::GetLongDateFormat(context);
    *format = f;
    REFCOUNT_ADD(*format);
    return NOERROR;
}

ECode CDateFormat::GetMediumDateFormat(
    /* [in] */ IContext* context,
    /* [out] */ Elastos::Text::IDateFormat** format)
{
    VALIDATE_NOT_NULL(format);
    AutoPtr<Elastos::Text::IDateFormat> f = DateFormat::GetMediumDateFormat(context);
    *format = f;
    REFCOUNT_ADD(*format);
    return NOERROR;
}

ECode CDateFormat::GetDateFormatOrder(
    /* [in] */ IContext* context,
    /* [out, callee] */ ArrayOf<Char32>** order)
{
    VALIDATE_NOT_NULL(order);
    AutoPtr< ArrayOf<Char32> > o =DateFormat::GetDateFormatOrder(context);
    *order = o;
    REFCOUNT_ADD(*order);
    return NOERROR;
}

ECode CDateFormat::Format(
    /* [in] */ ICharSequence* inFormat,
    /* [in] */ Int64 inTimeInMillis,
    /* [out] */ ICharSequence** result)
{
    VALIDATE_NOT_NULL(result);
    AutoPtr<ICharSequence> r = DateFormat::Format(inFormat, inTimeInMillis);
    *result = r;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

ECode CDateFormat::Format(
    /* [in] */ ICharSequence* inFormat,
    /* [in] */ IDate* inDate,
    /* [out] */ ICharSequence** result)
{
    VALIDATE_NOT_NULL(result);
    AutoPtr<ICharSequence> r = DateFormat::Format(inFormat, inDate);
    *result = r;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

ECode CDateFormat::HasSeconds(
    /* [in] */ ICharSequence* inFormat,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = DateFormat::HasSeconds(inFormat);
    return NOERROR;
}

ECode CDateFormat::Format(
    /* [in] */ ICharSequence* inFormat,
    /* [in] */ ICalendar* inDate,
    /* [out] */ ICharSequence** result)
{
    VALIDATE_NOT_NULL(result);
    AutoPtr<ICharSequence> r = DateFormat::Format(inFormat, inDate);
    *result = r.Get();
    REFCOUNT_ADD(*result);
    return NOERROR;
}

} // namespace Format
} // namespace Text
} // namepsace Droid
} // namespace Elastos
