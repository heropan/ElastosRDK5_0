
#include "view/CInputChannelHelper.h"
#include "view/CInputChannel.h"
#include "ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace View {

ECode CInputChannelHelper::OpenInputChannelPair(
    /* [in] */ const String& name,
    /* [out] */ IInputChannel** inputChannel0,
    /* [out] */ IInputChannel** inputChannel1)
{
    VALIDATE_NOT_NULL(inputChannel0);
    VALIDATE_NOT_NULL(inputChannel1);

    AutoPtr<IInputChannel> channel0, channel1;
    FAIL_RETURN(CInputChannel::OpenInputChannelPair(name, (CInputChannel**)&channel0, (CInputChannel**)&channel1));
    *inputChannel0 = channel0;
    REFCOUNT_ADD(*inputChannel0);
    *inputChannel1 = channel1;
    REFCOUNT_ADD(*inputChannel1);

    return NOERROR;
}

} // namespace View
} // namepsace Droid
} // namespace Elastos
