
#ifndef __CDATEFORMATHELPER_H__
#define __CDATEFORMATHELPER_H__

#include "_CDateFormatHelper.h"

using Libcore::ICU::ILocale;

namespace Elastos {
namespace Text {

CarClass(CDateFormatHelper)
{
public:
    CARAPI GetAvailableLocales(
        /* [out, callee] */ ArrayOf<ILocale*> ** locales);

    CARAPI GetDateInstance(
        /* [out] */ IDateFormat** instance);

    CARAPI GetDateInstanceEx(
        /* [in] */ Int32 style,
        /* [out] */ IDateFormat** instance);

    CARAPI GetDateInstanceEx2(
        /* [in] */ Int32 style,
        /* [in] */ ILocale* locale,
        /* [out] */ IDateFormat** instance);

    CARAPI GetDateTimeInstance(
        /* [out] */ IDateFormat** instance);

    CARAPI GetDateTimeInstanceEx(
        /* [in] */ Int32 dateStyle,
        /* [in] */ Int32 timeStyle,
        /* [out] */ IDateFormat** instance);

    CARAPI GetDateTimeInstanceEx2(
        /* [in] */ Int32 dateStyle,
        /* [in] */ Int32 timeStyle,
        /* [in] */ ILocale* locale,
        /* [out] */ IDateFormat** instance);

    CARAPI GetInstance(
        /* [out] */ IDateFormat** instance);

    CARAPI GetTimeInstance(
        /* [out] */ IDateFormat** instance);

    CARAPI GetTimeInstanceEx(
        /* [in] */ Int32 style,
        /* [out] */ IDateFormat** instance);

    CARAPI GetTimeInstanceEx2(
        /* [in] */ Int32 style,
        /* [in] */ ILocale* locale,
        /* [out] */ IDateFormat** instance);
};

} // namespace Text
} // namespace Elastos

#endif // __CDATEFORMATHELPER_H__
