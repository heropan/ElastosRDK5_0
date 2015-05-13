
#include "text/format/CFormatter.h"
#include "text/format/Formatter.h"
#include "ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Text {
namespace Format {

ECode CFormatter::FormatFileSize(
    /* [in] */ IContext* context,
    /* [in] */ Int64 number,
    /* [out] */ String* ret)
{
    VALIDATE_NOT_NULL(ret);
    *ret = Formatter::FormatFileSize(context, number);
    return NOERROR;
}

ECode CFormatter::FormatIpAddress(
    /* [in] */ Int32 ipv4Address,
    /* [out] */ String* ret)
{
    VALIDATE_NOT_NULL(ret);
    *ret = Formatter::FormatIpAddress(ipv4Address);
    return NOERROR;
}

ECode CFormatter::FormatShortFileSize(
    /* [in] */ IContext* context,
    /* [in] */ Int64 number,
    /* [out] */ String* ret)
{
    VALIDATE_NOT_NULL(ret);
    *ret = Formatter::FormatShortFileSize(context, number);
    return NOERROR;
}

} // namespace Format
} // namespace Text
} // namepsace Droid
} // namespace Elastos
