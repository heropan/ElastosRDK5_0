
#include "ext/frameworkext.h"
#include "net/CLinkCapabilities.h"
#include "net/CLinkCapabilitiesHelper.h"

namespace Elastos {
namespace Droid {
namespace Net {

ECode CLinkCapabilitiesHelper::CreateNeedsMap(
    /* [in] */ const String& applicationRole,
    /* [out] */ ILinkCapabilities** result)
{
    VALIDATE_NOT_NULL(result);
    return CLinkCapabilities::CreateNeedsMap(applicationRole,result);
}

} // namespace Net
} // namepsace Droid
} // namespace Elastos
