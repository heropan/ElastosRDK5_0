
#include "elastos/droid/speech/CRecognizerIntent.h"
#include "elastos/droid/speech/RecognizerIntent.h"
#include "elastos/droid/ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Speech {

ECode CRecognizerIntent::GetVoiceDetailsIntent(
    /* [in] */ IContext * pContext,
    /* [out] */ IIntent ** ppRet)
{
    VALIDATE_NOT_NULL(ppRet);
    AutoPtr<IIntent> temp = RecognizerIntent::GetVoiceDetailsIntent(pContext);
    *ppRet = temp;;
    REFCOUNT_ADD(*ppRet);
    return NOERROR;
}

}//namespace Speech
}//namespace Droid
}//namespace Elastos

