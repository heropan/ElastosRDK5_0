
#include "elastos/droid/view/CInputChannelHelper.h"
#include "elastos/droid/view/CInputChannel.h"
#include "elastos/droid/ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace View {

CAR_INTERFACE_IMPL(CInputChannelHelper, Singleton, IInputChannelHelper)

CAR_SINGLETON_IMPL(CInputChannelHelper)

ECode CInputChannelHelper::OpenInputChannelPair(
    /* [in] */ const String& name,
    /* [out, callee] */ ArrayOf<IInputChannel*>** icp)
{
    VALIDATE_NOT_NULL(icp);

    FAIL_RETURN(CInputChannel::OpenInputChannelPair(name, icp))

    return NOERROR;
}

} // namespace View
} // namepsace Droid
} // namespace Elastos
