
#include "elastos/droid/text/method/CMultiTapKeyListenerHelper.h"
#include "elastos/droid/text/method/MultiTapKeyListener.h"
#include "elastos/droid/ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Text {
namespace Method {

CAR_INTERFACE_IMPL(CMultiTapKeyListenerHelper, Singleton, IMultiTapKeyListenerHelper)

CAR_SINGLETON_IMPL(CMultiTapKeyListenerHelper)

ECode CMultiTapKeyListenerHelper::GetInstance(
    /* [in] */ Boolean autotext,
    /* [in] */ Capitalize cap,
    /* [out] */ IMultiTapKeyListener** ret)
{
    VALIDATE_NOT_NULL(ret);
    AutoPtr<IMultiTapKeyListener> mtk;
    MultiTapKeyListener::GetInstance(autotext, cap, (IMultiTapKeyListener**)&mtk);
    *ret = mtk;
    REFCOUNT_ADD(*ret);
    return NOERROR;
}

} // namespace Method
} // namespace Text
} // namepsace Droid
} // namespace Elastos

