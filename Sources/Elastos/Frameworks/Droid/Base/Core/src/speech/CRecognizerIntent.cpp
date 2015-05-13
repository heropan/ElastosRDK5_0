
#include "speech/CRecognizerIntent.h"
#include "speech/RecognizerIntent.h"
#include "ext/frameworkext.h"

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
    INTERFACE_ADDREF(*ppRet);
    return NOERROR;
}

}//namespace Speech
}//namespace Droid
}//namespace Elastos

