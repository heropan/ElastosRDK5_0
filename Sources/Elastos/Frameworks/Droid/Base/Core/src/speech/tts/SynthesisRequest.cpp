#include "speech/tts/SynthesisRequest.h"
#include "os/CBundle.h"

using Elastos::Droid::Os::CBundle;

namespace Elastos {
namespace Droid {
namespace Speech {
namespace Tts {

SynthesisRequest::SynthesisRequest()
{}

SynthesisRequest::SynthesisRequest(
    /* [in] */ const String& text,
    /* [in] */ IBundle* params)
{
    Init(text, params);
}

void SynthesisRequest::Init(
    /* [in] */ const String& text,
    /* [in] */ IBundle* params)
{
    mText = text;
    // Makes a copy of params.
    CBundle::New(params, (IBundle**)&mParams);
}

String SynthesisRequest::GetText()
{
    return mText;
}

String SynthesisRequest::GetLanguage()
{
    return mLanguage;
}

String SynthesisRequest::GetCountry()
{
    return mCountry;
}

String SynthesisRequest::GetVariant()
{
    return mVariant;
}

Int32 SynthesisRequest::GetSpeechRate()
{
    return mSpeechRate;
}

Int32 SynthesisRequest::GetPitch()
{
    return mPitch;
}

AutoPtr<IBundle> SynthesisRequest::GetParams()
{
    return mParams;
}

void SynthesisRequest::SetLanguage(
    /* [in] */ const String& language,
    /* [in] */ const String& country,
    /* [in] */ const String& variant)
{
    mLanguage = language;
    mCountry = country;
    mVariant = variant;
}

void SynthesisRequest::SetSpeechRate(
    /* [in] */ Int32 speechRate)
{
    mSpeechRate = speechRate;
}

void SynthesisRequest::SetPitch(
    /* [in] */ Int32 pitch)
{
    mPitch = pitch;
}

} // namespace Tts
} // namespace Speech
} // namepsace Droid
} // namespace Elastos
