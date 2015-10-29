
#include "elastos/droid/text/method/CTimeKeyListenerHelper.h"
#include "elastos/droid/text/method/TimeKeyListener.h"
#include "elastos/droid/ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Text {
namespace Method {

CAR_INTERFACE_IMPL(CTimeKeyListenerHelper, Singleton, ITimeKeyListenerHelper)

CAR_SINGLETON_IMPL(CTimeKeyListenerHelper)

ECode CTimeKeyListenerHelper::GetInstance(
    /* [out] */ ITimeKeyListener** ret)
{
    return TimeKeyListener::GetInstance(ret);
}

ECode CTimeKeyListenerHelper::GetCHARACTERS(
    /* [out, callee] */ ArrayOf<Char32>** ret)
{
    return TimeKeyListener::GetCHARACTERS(ret);
}

} // namespace Method
} // namespace Text
} // namepsace Droid
} // namespace Elastos

