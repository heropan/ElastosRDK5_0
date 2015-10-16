
#include "elastos/droid/speech/tts/CSynthesisRequest.h"
#include "elastos/droid/ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Speech {
namespace Tts {

ECode CSynthesisRequest::GetText(
    /* [out] */ String* ret)
{
    VALIDATE_NOT_NULL(ret);
    *ret = SynthesisRequest::GetText();
    return NOERROR;
}

ECode CSynthesisRequest::GetLanguage(
    /* [out] */ String* ret)
{
    VALIDATE_NOT_NULL(ret);
    *ret = SynthesisRequest::GetLanguage();
    return NOERROR;
}

ECode CSynthesisRequest::GetCountry(
    /* [out] */ String* ret)
{
    VALIDATE_NOT_NULL(ret);
    *ret = SynthesisRequest::GetCountry();
    return NOERROR;
}

ECode CSynthesisRequest::GetVariant(
    /* [out] */ String* ret)
{
    VALIDATE_NOT_NULL(ret);
    *ret = SynthesisRequest::GetVariant();
    return NOERROR;
}

ECode CSynthesisRequest::GetSpeechRate(
    /* [out] */ Int32* ret)
{
    VALIDATE_NOT_NULL(ret);
    *ret = SynthesisRequest::GetSpeechRate();
    return NOERROR;
}

ECode CSynthesisRequest::GetPitch(
    /* [out] */ Int32* ret)
{
    VALIDATE_NOT_NULL(ret);
    *ret = SynthesisRequest::GetPitch();
    return NOERROR;
}

ECode CSynthesisRequest::GetParams(
    /* [out] */ IBundle** ret)
{
    VALIDATE_NOT_NULL(ret);
    AutoPtr<IBundle> b = SynthesisRequest::GetParams();
    *ret = b;
    REFCOUNT_ADD(*ret);
    return NOERROR;
}

ECode CSynthesisRequest::constructor(
    /* [in] */ const String& text,
    /* [in] */ IBundle* params)
{
    Init(text, params);
    return NOERROR;
}

}//namespace Tts
}//namespace Speech
}//namespace Droid
}//namespace Elastos

