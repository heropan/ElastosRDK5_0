
#include "ext/frameworkdef.h"
#include "CPinyinEnvironmentHelper.h"
#include "CPinyinEnvironment.h"

namespace Elastos {
namespace Droid {
namespace Inputmethods {
namespace PinyinIME {

AutoPtr<IPinyinEnvironment> CPinyinEnvironmentHelper::mInstance;

ECode CPinyinEnvironmentHelper::GetInstance(
    /* [out] */ IPinyinEnvironment** env)
{
    VALIDATE_NOT_NULL(env);
    if (NULL == mInstance) {
        CPinyinEnvironment::New((IPinyinEnvironment**)&mInstance);
    }
    *env = mInstance;
    REFCOUNT_ADD(*env);
    return NOERROR;
}

} // namespace PinyinIME
} // namespace Inputmethods
} // namespace Droid
} // namespace Elastos
