
#include "text/method/CArrowKeyMovementMethodHelper.h"
#include "text/method/CArrowKeyMovementMethod.h"
#include "ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Text {
namespace Method {

AutoPtr<IArrowKeyMovementMethod> CArrowKeyMovementMethodHelper::sInstance;

CAR_SINGLETON_IMPL(CArrowKeyMovementMethodHelper)

ECode CArrowKeyMovementMethodHelper::GetInstance(
    /* [out] */ IMovementMethod** ret)
{
    VALIDATE_NOT_NULL(ret);
    if (sInstance == NULL) {
        sInstance = new ArrowKeyMovementMethod();
    }
    *ret = IMovementMethod::Probe(sInstance);
    REFCOUNT_ADD(*ret);
    return NOERROR;
}

} // namespace Method
} // namespace Text
} // namepsace Droid
} // namespace Elastos

