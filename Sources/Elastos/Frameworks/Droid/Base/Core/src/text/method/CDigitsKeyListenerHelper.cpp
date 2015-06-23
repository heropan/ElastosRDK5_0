
#include "text/method/CDigitsKeyListenerHelper.h"
#include "text/method/CDigitsKeyListener.h"
#include "ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Text {
namespace Method {

IBaseKeyListenerHelper_METHODS_IMPL(CDigitsKeyListenerHelper, CDigitsKeyListener, CDigitsKeyListener)

ECode CDigitsKeyListenerHelper::GetInstance(
    /* [out] */ IDigitsKeyListener** ret)
{
    VALIDATE_NOT_NULL(ret);
    AutoPtr<IDigitsKeyListener> dkl = CDigitsKeyListener::GetInstance();
    *ret = dkl;
    REFCOUNT_ADD(*ret);
    return NOERROR;
}

ECode CDigitsKeyListenerHelper::GetInstance(
    /* [in] */ Boolean sign,
    /* [in] */ Boolean decimal,
    /* [out] */ IDigitsKeyListener** ret)
{
    VALIDATE_NOT_NULL(ret);
    AutoPtr<IDigitsKeyListener> dkl = CDigitsKeyListener::GetInstance(sign, decimal);
    *ret = dkl;
    REFCOUNT_ADD(*ret);
    return NOERROR;
}

ECode CDigitsKeyListenerHelper::GetInstance(
    /* [in] */ const String& accepted,
    /* [out] */ IDigitsKeyListener** ret)
{
    VALIDATE_NOT_NULL(ret);
    AutoPtr<IDigitsKeyListener> dkl = CDigitsKeyListener::GetInstance(accepted);
    *ret = dkl;
    REFCOUNT_ADD(*ret);
    return NOERROR;
}

} // namespace Method
} // namespace Text
} // namepsace Droid
} // namespace Elastos

