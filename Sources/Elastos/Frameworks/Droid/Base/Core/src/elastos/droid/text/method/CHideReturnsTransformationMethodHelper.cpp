
#include "elastos/droid/text/method/CHideReturnsTransformationMethodHelper.h"
#include "elastos/droid/text/method/CHideReturnsTransformationMethod.h"
#include "ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Text {
namespace Method {

ECode CHideReturnsTransformationMethodHelper::GetInstance(
    /* [out] */ IHideReturnsTransformationMethod** ret)
{
    VALIDATE_NOT_NULL(ret);
    AutoPtr<IHideReturnsTransformationMethod> hrtm = CHideReturnsTransformationMethod::GetInstance();
    *ret = hrtm;
    REFCOUNT_ADD(*ret);
    return NOERROR;
}

} //namespace Method
} //namespace Text
} //namespace Droid
} //namespace Elastos
