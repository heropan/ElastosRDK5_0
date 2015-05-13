
#include "CDateFormatHelper.h"
#include "DateFormat.h"

namespace Elastos {
namespace Text {

ECode CDateFormatHelper::GetAvailableLocales(
    /* [out, callee] */ ArrayOf<ILocale*>** locales)
{
    return DateFormat::GetAvailableLocales(locales);
}

ECode CDateFormatHelper::GetDateInstance(
    /* [out] */ IDateFormat** instance)
{
    return DateFormat::GetDateInstance(instance);
}

ECode CDateFormatHelper::GetDateInstanceEx(
    /* [in] */ Int32 style,
    /* [out] */ IDateFormat** instance)
{
    return DateFormat::GetDateInstanceEx(style,instance);
}

ECode CDateFormatHelper::GetDateInstanceEx2(
    /* [in] */ Int32 style,
    /* [in] */ ILocale* locale,
    /* [out] */ IDateFormat** instance)
{
    return DateFormat::GetDateInstanceEx2(style,locale,instance);
}

ECode CDateFormatHelper::GetDateTimeInstance(
    /* [out] */ IDateFormat** instance)
{
    return DateFormat::GetDateTimeInstance(instance);
}

ECode CDateFormatHelper::GetDateTimeInstanceEx(
    /* [in] */ Int32 dateStyle,
    /* [in] */ Int32 timeStyle,
    /* [out] */ IDateFormat** instance)
{
    return DateFormat::GetDateTimeInstanceEx(dateStyle,timeStyle,instance);
}

ECode CDateFormatHelper::GetDateTimeInstanceEx2(
    /* [in] */ Int32 dateStyle,
    /* [in] */ Int32 timeStyle,
    /* [in] */ ILocale* locale,
    /* [out] */ IDateFormat** instance)
{
    return DateFormat::GetDateTimeInstanceEx2(dateStyle,timeStyle,locale,instance);
}

ECode CDateFormatHelper::GetInstance(
    /* [out] */ IDateFormat** instance)
{
    return DateFormat::GetInstance(instance);
}

ECode CDateFormatHelper::GetTimeInstance(
    /* [out] */ IDateFormat** instance)
{
    return DateFormat::GetTimeInstance(instance);
}

ECode CDateFormatHelper::GetTimeInstanceEx(
    /* [in] */ Int32 style,
    /* [out] */ IDateFormat** instance)
{
    return DateFormat::GetTimeInstanceEx(style,instance);
}

ECode CDateFormatHelper::GetTimeInstanceEx2(
    /* [in] */ Int32 style,
    /* [in] */ ILocale* locale,
    /* [out] */ IDateFormat** instance)
{
    return DateFormat::GetTimeInstanceEx2(style,locale,instance);
}

} // namespace Text
} // namespace Elastos
