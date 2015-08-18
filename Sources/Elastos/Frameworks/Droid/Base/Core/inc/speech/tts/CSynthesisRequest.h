
#ifndef __ELASTOS_DROID_SPEECH_TTS_CSYNTHESISREQUEST_H__
#define __ELASTOS_DROID_SPEECH_TTS_CSYNTHESISREQUEST_H__

#include "_Elastos_Droid_Speech_Tts_CSynthesisRequest.h"
#include "SynthesisRequest.h"

using Elastos::Droid::Os::IBundle;

namespace Elastos {
namespace Droid {
namespace Speech {
namespace Tts {

CarClass(CSynthesisRequest)
    , public SynthesisRequest
{
public:
    CARAPI GetText(
        /* [out] */ String* ret);

    CARAPI GetLanguage(
        /* [out] */ String* ret);

    CARAPI GetCountry(
        /* [out] */ String* ret);

    CARAPI GetVariant(
        /* [out] */ String* ret);

    CARAPI GetSpeechRate(
        /* [out] */ Int32* ret);

    CARAPI GetPitch(
        /* [out] */ Int32* ret);

    CARAPI GetParams(
        /* [out] */ IBundle** ret);

    CARAPI constructor(
        /* [in] */ const String& text,
        /* [in] */ IBundle* params);

};

}//namespace Tts
}//namespace Speech
}//namespace Droid
}//namespace Elastos

#endif // __ELASTOS_DROID_SPEECH_TTS_CSYNTHESISREQUEST_H__
