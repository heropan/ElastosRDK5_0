
#include "view/CInputQueue.h"
#include "ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace View {

ECode CInputQueue::constructor(
    /* [in] */ IInputChannel* channel)
{
    mChannel = channel;

    return NOERROR;
}

ECode CInputQueue::GetInputChannel(
    /* [out] */ IInputChannel** inputChannel)
{
    VALIDATE_NOT_NULL(inputChannel);
    *inputChannel = mChannel;
    INTERFACE_ADDREF(*inputChannel);
    return NOERROR;
}

} // namespace View
} // namepsace Droid
} // namespace Elastos
