
#include "speech/CSpeechRecognizerHelper.h"

#include "speech/CSpeechRecognizer.h"

namespace Elastos {
namespace Droid {
namespace Speech {

ECode CSpeechRecognizerHelper::IsRecognitionAvailable(
    /* [in] */ IContext * pContext,
    /* [out] */ Boolean * pRet)
{
    VALIDATE_NOT_NULL(pRet);
    *pRet = CSpeechRecognizer::IsRecognitionAvailable(pContext);
    return NOERROR;
}

ECode CSpeechRecognizerHelper::CreateSpeechRecognizer(
    /* [in] */ IContext * pContext,
    /* [out] */ ISpeechRecognizer ** ppRet)
{
    VALIDATE_NOT_NULL(ppRet);
    AutoPtr<ISpeechRecognizer> temp = CSpeechRecognizer::CreateSpeechRecognizer(pContext);
    *ppRet = temp;
    INTERFACE_ADDREF(*ppRet);
    return NOERROR;
}

ECode CSpeechRecognizerHelper::CreateSpeechRecognizerEx(
    /* [in] */ IContext * pContext,
    /* [in] */ IComponentName * pServiceComponent,
    /* [out] */ ISpeechRecognizer ** ppRet)
{
    VALIDATE_NOT_NULL(ppRet);
    AutoPtr<ISpeechRecognizer> temp = CSpeechRecognizer::CreateSpeechRecognizer(pContext, pServiceComponent);
    *ppRet = temp;
    INTERFACE_ADDREF(*ppRet);
    return NOERROR;
}

}//namespace Speech
}//namespace Droid
}//namespace Elastos

