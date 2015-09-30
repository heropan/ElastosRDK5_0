
#include "elastos/droid/text/method/CSingleLineTransformationMethodHelper.h"
#include "elastos/droid/text/method/CSingleLineTransformationMethod.h"
#include "ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Text {
namespace Method {

ECode CSingleLineTransformationMethodHelper::GetInstance(
    /* [out] */ ISingleLineTransformationMethod** ret)
{
    VALIDATE_NOT_NULL(ret);
    AutoPtr<ISingleLineTransformationMethod> stm = CSingleLineTransformationMethod::GetInstance();
    *ret = stm;
    REFCOUNT_ADD(*ret);
    return NOERROR;
}

} // namespace Method
} // namespace Text
} // namepsace Droid
} // namespace Elastos

