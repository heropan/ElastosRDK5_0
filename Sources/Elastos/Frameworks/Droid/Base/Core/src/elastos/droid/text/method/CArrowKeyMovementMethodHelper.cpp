
#include "elastos/droid/text/method/CArrowKeyMovementMethodHelper.h"
#include "elastos/droid/text/method/CArrowKeyMovementMethod.h"
#include "elastos/droid/ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Text {
namespace Method {

ECode CArrowKeyMovementMethodHelper::GetInstance(
    /* [out] */ IMovementMethod** ret)
{
    VALIDATE_NOT_NULL(ret);
    AutoPtr<IMovementMethod> mm = CArrowKeyMovementMethod::GetInstance();
    *ret = mm;
    REFCOUNT_ADD(*ret);
    return NOERROR;
}

} // namespace Method
} // namespace Text
} // namepsace Droid
} // namespace Elastos

