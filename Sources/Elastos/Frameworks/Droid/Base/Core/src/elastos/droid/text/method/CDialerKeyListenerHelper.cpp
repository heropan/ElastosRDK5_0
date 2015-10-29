
#include "elastos/droid/text/method/CDialerKeyListenerHelper.h"
#include "elastos/droid/text/method/DialerKeyListener.h"
#include "elastos/droid/ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Text {
namespace Method {

CAR_INTERFACE_IMPL(CDialerKeyListenerHelper, Singleton, IDialerKeyListenerHelper)

CAR_SINGLETON_IMPL(CDialerKeyListenerHelper)

ECode CDialerKeyListenerHelper::GetCHARACTERS(
    /* [out, callee] */ ArrayOf<Char32>** ret)
{
    return DialerKeyListener::GetCHARACTERS(ret);
}

ECode CDialerKeyListenerHelper::GetInstance(
    /* [out] */ IDialerKeyListener** ret)
{
    return DialerKeyListener::GetInstance(ret);
}

} // namespace Method
} // namespace Text
} // namepsace Droid
} // namespace Elastos

