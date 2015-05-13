
#include "text/method/CMultiTapKeyListenerHelper.h"
#include "text/method/CMultiTapKeyListener.h"
#include "ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Text {
namespace Method {

IBaseKeyListenerHelper_METHODS_IMPL(CMultiTapKeyListenerHelper, CMultiTapKeyListener, CMultiTapKeyListener)

ECode CMultiTapKeyListenerHelper::GetInstance(
    /* [in] */ Boolean autotext,
    /* [in] */ Capitalize cap,
    /* [out] */ IMultiTapKeyListener** ret)
{
    VALIDATE_NOT_NULL(ret);
    AutoPtr<IMultiTapKeyListener> mtkl = CMultiTapKeyListener::GetInstance(autotext, cap);
    *ret = mtkl;
    INTERFACE_ADDREF(*ret);
    return NOERROR;
}

} // namespace Method
} // namespace Text
} // namepsace Droid
} // namespace Elastos

