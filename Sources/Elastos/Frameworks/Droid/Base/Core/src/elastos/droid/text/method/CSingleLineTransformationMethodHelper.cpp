#include "elastos/droid/text/method/CSingleLineTransformationMethodHelper.h"
#include "elastos/droid/text/method/CSingleLineTransformationMethod.h"
#include "elastos/droid/ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Text {
namespace Method {

CAR_SINGLETON_IMPL(CSingleLineTransformationMethodHelper)

CAR_INTERFACE_IMPL(CSingleLineTransformationMethodHelper, Singleton, ISingleLineTransformationMethodHelper)

ECode CSingleLineTransformationMethodHelper::GetInstance(
    /* [out] */ ISingleLineTransformationMethod** ret)
{
    return CSingleLineTransformationMethod::GetInstance(ret);
}

} // namespace Method
} // namespace Text
} // namepsace Droid
} // namespace Elastos

