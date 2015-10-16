
#include "elastos/droid/view/CInputQueue.h"
#include "elastos/droid/ext/frameworkext.h"

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
    REFCOUNT_ADD(*inputChannel);
    return NOERROR;
}

} // namespace View
} // namepsace Droid
} // namespace Elastos
