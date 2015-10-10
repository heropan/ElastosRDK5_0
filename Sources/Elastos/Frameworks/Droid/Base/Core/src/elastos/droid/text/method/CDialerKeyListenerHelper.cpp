
#include "elastos/droid/text/method/CDialerKeyListenerHelper.h"
#include "elastos/droid/text/method/CDialerKeyListener.h"
#include "elastos/droid/ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Text {
namespace Method {

IBaseKeyListenerHelper_METHODS_IMPL(CDialerKeyListenerHelper, CDialerKeyListener, CDialerKeyListener)

ECode CDialerKeyListenerHelper::GetCHARACTERS(
    /* [out, callee] */ ArrayOf<Char32>** ret)
{
    VALIDATE_NOT_NULL(ret);
    AutoPtr< ArrayOf<Char32> > o = CDialerKeyListener::GetCHARACTERS();
    *ret = o;
    REFCOUNT_ADD(*ret);
    return NOERROR;
}

ECode CDialerKeyListenerHelper::GetInstance(
    /* [out] */ IDialerKeyListener** ret)
{
    VALIDATE_NOT_NULL(ret);
    AutoPtr<IDialerKeyListener> dkl = CDialerKeyListener::GetInstance();
    *ret = dkl;
    REFCOUNT_ADD(*ret);
    return NOERROR;
}

} // namespace Method
} // namespace Text
} // namepsace Droid
} // namespace Elastos

