
#include "text/method/CPasswordTransformationMethodHelper.h"
#include "text/method/CPasswordTransformationMethod.h"
#include "ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Text {
namespace Method {

ECode CPasswordTransformationMethodHelper::GetInstance(
    /* [out] */ IPasswordTransformationMethod** ret)
{
    VALIDATE_NOT_NULL(ret);
    AutoPtr<IPasswordTransformationMethod> ptm = CPasswordTransformationMethod::GetInstance();
    *ret = ptm;
    REFCOUNT_ADD(*ret);
    return NOERROR;
}

} // namespace Method
} // namespace Text
} // namepsace Droid
} // namespace Elastos

