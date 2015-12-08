
#include "elastos/droid/speech/tts/CTtsEnginesHelper.h"
#include "elastos/droid/speech/tts/CTtsEngines.h"
#include "elastos/droid/ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Speech {
namespace Tts {

ECode CTtsEnginesHelper::ParseLocalePref(
    /* [in] */ const String& pref,
    /* [out, callee] */ ArrayOf<String> ** ppRet)
{
    VALIDATE_NOT_NULL(ppRet);
    AutoPtr<ArrayOf<String> > temp = CTtsEngines::ParseLocalePref(pref);
    *ppRet = temp;
    REFCOUNT_ADD(*ppRet);
    return NOERROR;
}

} // namespace Tts
} // namespace Speech
} // namespace Droid
} // namespace Elastos

