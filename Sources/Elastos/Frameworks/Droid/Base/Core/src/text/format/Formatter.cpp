#include "text/format/Formatter.h"
#include "R.h"
#include "net/NetworkUtils.h"

using Elastos::Core::ICharSequence;
using Elastos::Core::CString;
using Elastos::Droid::Net::NetworkUtils;

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
    Int32 suffix = R::string::byteShort;
    if (result > 900) {
        suffix = R::string::kilobyteShort;
        result = result / 1024;
    }
    if (result > 900) {
        suffix = R::string::megabyteShort;
        result = result / 1024;
    }
    if (result > 900) {
        suffix = R::string::gigabyteShort;
        result = result / 1024;
    }
    if (result > 900) {
        suffix = R::string::terabyteShort;
        result = result / 1024;
    }
    if (result > 900) {
        suffix = R::string::petabyteShort;
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
    res->GetString(R::string::fileSizeSuffix, args, &ret);
    return ret;
}

String Formatter::FormatIpAddress(
    /* [in] */ Int32 ipv4Address)
{
    String ret;
    AutoPtr<IInetAddress> ina;
    NetworkUtils::Int32ToInetAddress(ipv4Address, (IInetAddress**)&ina);
    ina->GetHostAddress(&ret);
    return ret;
}


} // namespace Format
} // namespace Text
} // namepsace Droid
} // namespace Elastos
