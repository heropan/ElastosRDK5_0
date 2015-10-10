
#include "elastos/droid/text/method/CMultiTapKeyListenerHelper.h"
#include "elastos/droid/text/method/CMultiTapKeyListener.h"
#include "elastos/droid/ext/frameworkext.h"

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
    REFCOUNT_ADD(*ret);
    return NOERROR;
}

} // namespace Method
} // namespace Text
} // namepsace Droid
} // namespace Elastos

