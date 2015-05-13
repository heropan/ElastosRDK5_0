
#ifndef __CDATEFORMAT_H__
#define __CDATEFORMAT_H__

#include "_CDateFormat.h"
#include "text/format/DateFormat.h"

using Elastos::Core::ICharSequence;
using Elastos::Utility::ICalendar;
using Elastos::Utility::IDate;
using Elastos::Droid::Content::IContext;

namespace Elastos {
namespace Droid {
namespace Text {
namespace Format {

CarClass(CDateFormat), public DateFormat
{
public:
    CARAPI Is24HourFormat(
        /* [in] */ IContext* context,
        /* [out] */ Boolean* result);

    CARAPI GetTimeFormat(
        /* [in] */ IContext* context,
        /* [out] */ Elastos::Text::IDateFormat** format);

    CARAPI GetDateFormat(
        /* [in] */ IContext* context,
        /* [out] */ Elastos::Text::IDateFormat** format);

    CARAPI GetDateFormatForSetting(
        /* [in] */ IContext* context,
        /* [in] */ const String& value,
        /* [out] */ Elastos::Text::IDateFormat** format);

    CARAPI GetLongDateFormat(
        /* [in] */ IContext* context,
        /* [out] */ Elastos::Text::IDateFormat** format);

    CARAPI GetMediumDateFormat(
        /* [in] */ IContext* context,
        /* [out] */ Elastos::Text::IDateFormat** format);

    CARAPI GetDateFormatOrder(
        /* [in] */ IContext* context,
        /* [out, callee] */ ArrayOf<Char32>** order);

    CARAPI Format(
        /* [in] */ ICharSequence* inFormat,
        /* [in] */ Int64 inTimeInMillis,
        /* [out] */ ICharSequence** result);

    CARAPI FormatEx(
        /* [in] */ ICharSequence* inFormat,
        /* [in] */ IDate* inDate,
        /* [out] */ ICharSequence** result);

    CARAPI HasSeconds(
        /* [in] */ ICharSequence* inFormat,
        /* [out] */ Boolean* result);

    CARAPI FormatEx2(
        /* [in] */ ICharSequence* inFormat,
        /* [in] */ ICalendar* inDate,
        /* [out] */ ICharSequence** result);
};

} // namespace Format
} // namespace Text
} // namepsace Droid
} // namespace Elastos

#endif // __CDATEFORMAT_H__
