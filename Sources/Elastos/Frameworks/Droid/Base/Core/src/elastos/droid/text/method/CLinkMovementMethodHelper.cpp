
#include "elastos/droid/text/method/CLinkMovementMethodHelper.h"
#include "elastos/droid/text/method/CLinkMovementMethod.h"
#include "elastos/droid/ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Text {
namespace Method {

ECode CLinkMovementMethodHelper::GetInstance(
    /* [out] */ IMovementMethod** ret)
{
    VALIDATE_NOT_NULL(ret);
    AutoPtr<IMovementMethod> mm = CLinkMovementMethod::GetInstance();
    *ret = mm;
    REFCOUNT_ADD(*ret);
    return NOERROR;
}

} // namespace Method
} // namespace Text
} // namepsace Droid
} // namespace Elastos

