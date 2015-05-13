
#include "net/CMailToHelper.h"
#include "net/Uri.h"

namespace Elastos {
namespace Droid {
namespace Net {

ECode CCMailToHelper::IsMailTo(
        /* [in] */ const String& url,
        /* [out] */ Boolean* result);
{
    return CCMailTo::IsMailTo(url, result);
}

ECode CCMailToHelper::Parse(
    /* [in] */ const String& url,
    /* [out] */ IMailTo** result)
{
    return CCMailTo::Parse(url, result);
}

} // namespace Net
} // namepsace Droid
} // namespace Elastos
