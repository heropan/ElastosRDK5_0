
#include "text/method/CScrollingMovementMethodHelper.h"
#include "text/method/CScrollingMovementMethod.h"
#include "ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Text {
namespace Method {

ECode CScrollingMovementMethodHelper::GetInstance(
    /* [out] */ IMovementMethod** ret)
{
    VALIDATE_NOT_NULL(ret);
    AutoPtr<IMovementMethod> mm = CScrollingMovementMethod::GetInstance();
    *ret = mm;
    INTERFACE_ADDREF(*ret);
    return NOERROR;
}

} // namespace Method
} // namespace Text
} // namepsace Droid
} // namespace Elastos

