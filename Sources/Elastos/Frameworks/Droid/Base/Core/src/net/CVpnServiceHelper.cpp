
#include "net/CVpnServiceHelper.h"

namespace Elastos {
namespace Droid {
namespace Net {

ECode CVpnServiceHelper::Prepare(
    /* [in] */ IContext* context,
    /* [out] */ IIntent** intent)
{
    VALIDATE_NOT_NULL(intent);
    AutoPtr<IIntent> temp = CVpnService::PrepareImpl(context);
    *intent = temp;
    return NOERROR;
}

} // namespace Net
} // namepsace Droid
} // namespace Elastos
