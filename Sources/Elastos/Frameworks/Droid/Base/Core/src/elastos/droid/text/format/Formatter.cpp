#include "elastos/droid/text/format/Formatter.h"
#include "elastos/droid/R.h"
// #include "elastos/droid/net/NetworkUtils.h"

using Elastos::Core::ICharSequence;
using Elastos::Core::CString;
// using Elastos::Droid::Net::NetworkUtils;

namespace Elastos {
namespace Droid {
namespace Text {
namespace Format {

String Formatter::FormatFileSize(
    /* [in] */ IContext* context,
    /* [in] */ Int64 number)
{
    return FormatFileSize(context, number, FALSE);
}

String Formatter::FormatShortFileSize(
    /* [in] */ IContext* context,
    /* [in] */ Int64 number)
{
    return FormatFileSize(context, number, TRUE);
}

String Formatter::FormatFileSize(
    /* [in] */ IContext* context,
    /* [in] */ Int64 number,
    /* [in] */ Boolean shorter)
{
    if (context == NULL) {
        return String("");
    }

    Float result = number;
    Int32 suffix;
    assert(0 && "TODO");
    // suffix = R::string::byteShort;
    if (result > 900) {
        // suffix = R::string::kilobyteShort;
        result = result / 1024;
    }
    if (result > 900) {
        // suffix = R::string::megabyteShort;
        result = result / 1024;
    }
    if (result > 900) {
        // suffix = R::string::gigabyteShort;
        result = result / 1024;
    }
    if (result > 900) {
        // suffix = R::string::terabyteShort;
        result = result / 1024;
    }
    if (result > 900) {
        // suffix = R::string::petabyteShort;
        result = result / 1024;
    }
    String value;
    if (result < 1) {
        value.AppendFormat("%.2f", result);
    }
    else if (result < 10) {
        if (shorter) {
            value.AppendFormat("%.1f", result);
        }
        else {
            value.AppendFormat("%.2f", result);
        }
    }
    else if (result < 100) {
        if (shorter) {
            value.AppendFormat("%.0f", result);
        }
        else {
            value.AppendFormat("%.2f", result);
        }
    }
    else {
        value.AppendFormat("%.0f", result);
    }
    String resStr;
    context->GetString(suffix, &resStr);
    AutoPtr< ArrayOf<IInterface*> > args = ArrayOf<IInterface*>::Alloc(2);
    AutoPtr<ICharSequence> cs;
    CString::New(value, (ICharSequence**)&cs);
    args->Set(0, cs);
    cs = NULL;
    CString::New(resStr, (ICharSequence**)&cs);
    args->Set(1, cs);
    AutoPtr<IResources> res;
    context->GetResources((IResources**)&res);
    String ret;
    assert(0 && "TODO");
    // res->GetString(R::string::fileSizeSuffix, args, &ret);
    return ret;
}

String Formatter::FormatIpAddress(
    /* [in] */ Int32 ipv4Address)
{
    String ret;
    AutoPtr<IInetAddress> ina;
    // NetworkUtils::Int32ToInetAddress(ipv4Address, (IInetAddress**)&ina);
    ina->GetHostAddress(&ret);
    return ret;
}

String Formatter::FormatShortElapsedTime(
    /* [in] */ IContext* context,
    /* [in] */ Int64 millis)
{
    Int64 secondsLong = millis / 1000;

    Int32 days = 0, hours = 0, minutes = 0;
    if (secondsLong >= SECONDS_PER_DAY) {
        days = (Int32)(secondsLong / SECONDS_PER_DAY);
        secondsLong -= days * SECONDS_PER_DAY;
    }
    if (secondsLong >= SECONDS_PER_HOUR) {
        hours = (Int32)(secondsLong / SECONDS_PER_HOUR);
        secondsLong -= hours * SECONDS_PER_HOUR;
    }
    if (secondsLong >= SECONDS_PER_MINUTE) {
        minutes = (Int32)(secondsLong / SECONDS_PER_MINUTE);
        secondsLong -= minutes * SECONDS_PER_MINUTE;
    }
    Int32 seconds = (Int32)secondsLong;

    if (days >= 2) {
        days += (hours + 12) / 24;
        assert(0 && "TODO");
        // return context.getString(com.android.internal.R.string.durationDays, days);
        return String("");
    } else if (days > 0) {
        if (hours == 1) {
            // return context.getString(com.android.internal.R.string.durationDayHour, days, hours);
            return String("");
        }
        // return context.getString(com.android.internal.R.string.durationDayHours, days, hours);
        return String("");
    } else if (hours >= 2) {
        hours += (minutes + 30) / 60;
        // return context.getString(com.android.internal.R.string.durationHours, hours);
        return String("");
    } else if (hours > 0) {
        if (minutes == 1) {
            // return context.getString(com.android.internal.R.string.durationHourMinute, hours,
            //             minutes);
            return String("");
        }
        // return context.getString(com.android.internal.R.string.durationHourMinutes, hours,
        //             minutes);
        return String("");
    } else if (minutes >= 2) {
        minutes += (seconds + 30) / 60;
        // return context.getString(com.android.internal.R.string.durationMinutes, minutes);
        return String("");
    } else if (minutes > 0) {
        if (seconds == 1) {
            // return context.getString(com.android.internal.R.string.durationMinuteSecond, minutes,
            //             seconds);
            return String("");
        }
        // return context.getString(com.android.internal.R.string.durationMinuteSeconds, minutes,
        //             seconds);
        return String("");
    } else if (seconds == 1) {
        // return context.getString(com.android.internal.R.string.durationSecond, seconds);
        return String("");
    } else {
        // return context.getString(com.android.internal.R.string.durationSeconds, seconds);
        return String("");
    }
}

} // namespace Format
} // namespace Text
} // namepsace Droid
} // namespace Elastos
