#include "elastos/droid/speech/tts/SynthesisRequest.h"
#include "elastos/droid/os/CBundle.h"

using Elastos::Droid::Os::CBundle;

namespace Elastos {
namespace Droid {
namespace Speech {
namespace Tts {


/******************
 * SynthesisRequest
 *******************************************************************************************************/

CAR_INTERFACE_IMPL(SynthesisRequest, Object, ISynthesisRequest)

SynthesisRequest::SynthesisRequest()
{}

SynthesisRequest::~SynthesisRequest()
{}

ECode SynthesisRequest::constructor()
{
    return NOERROR;
}

ECode SynthesisRequest::constructor(
    /* [in] */ const String& text,
    /* [in] */ IBundle* params)
{
    CString::New(text, (ICharSequence**)&mText);

    // Makes a copy of params.
    return CBundle::New(params, (IBundle**)&mParams);
}

ECode SynthesisRequest::constructor(
    /* [in] */ ICharSequence* text,
    /* [in] */ IBundle* params)
{
    mText = text;

    // Makes a copy of params.
    return CBundle::New(params, (IBundle**)&mParams);
}

ECode CSynthesisRequest::GetText(
    /* [out] */ String* ret)
{
    VALIDATE_NOT_NULL(ret);
    *ret = mText;
    return NOERROR;
}

ECode CSynthesisRequest::GetCharSequenceText(
    /* [out] */ ICharSequence** ret)
{
    VALIDATE_NOT_NULL(ret);
    *ret = mText;
    return NOERROR;
}

ECode CSynthesisRequest::GetVoiceName(
    /* [out] */ String* ret)
{
    VALIDATE_NOT_NULL(ret);
    *ret = mVoiceName;
    return NOERROR;
}

ECode CSynthesisRequest::GetLanguage(
    /* [out] */ String* ret)
{
    VALIDATE_NOT_NULL(ret);
    *ret = mLanguage;
    return NOERROR;
}

ECode CSynthesisRequest::GetCountry(
    /* [out] */ String* ret)
{
    VALIDATE_NOT_NULL(ret);
    *ret = mCountry;
    return NOERROR;
}

ECode CSynthesisRequest::GetVariant(
    /* [out] */ String* ret)
{
    VALIDATE_NOT_NULL(ret);
    *ret = mVariant;
    return NOERROR;
}

ECode CSynthesisRequest::GetSpeechRate(
    /* [out] */ Int32* ret)
{
    VALIDATE_NOT_NULL(ret);
    *ret = mSpeechRate;
    return NOERROR;
}

ECode CSynthesisRequest::GetPitch(
    /* [out] */ Int32* ret)
{
    VALIDATE_NOT_NULL(ret);
    *ret = mPitch;
    return NOERROR;
}

ECode CSynthesisRequest::GetParams(
    /* [out] */ IBundle** ret)
{
    VALIDATE_NOT_NULL(ret);
    *ret = mParams;
    REFCOUNT_ADD(*ret);
    return NOERROR;
}

ECode CSynthesisRequest::GetCallerUid(
    /* [out] */ Int32* ret)
{
    VALIDATE_NOT_NULL(ret);
    *ret = mCallerUid;
    return NOERROR;
}

ECode SynthesisRequest::SetLanguage(
    /* [in] */ const String& language,
    /* [in] */ const String& country,
    /* [in] */ const String& variant)
{
    mLanguage = language;
    mCountry = country;
    mVariant = variant;
}

ECode SynthesisRequest::SetVoiceName(
    /* [in] */ const String& voiceName)
{
    mVoiceName = voiceName;
}
ECode SynthesisRequest::SetSpeechRate(
    /* [in] */ Int32 speechRate)
{
    mSpeechRate = speechRate;
}

ECode SynthesisRequest::SetPitch(
    /* [in] */ Int32 pitch)
{
    mPitch = pitch;
}

ECode SynthesisRequest::SetCallerUid(
    /* [in] */ Int32 uid)
{
    mCallerUid = uid;
}

} // namespace Tts
} // namespace Speech
} // namepsace Droid
} // namespace Elastos
