#ifndef __ELASTOS_DROID_SPEECH_TTS_SynthesisRequest_H__
#define __ELASTOS_DROID_SPEECH_TTS_SynthesisRequest_H__

#include "Elastos.Droid.Core_server.h"
//
using Elastos::Droid::Os::IBundle;

namespace Elastos {
namespace Droid {
namespace Speech {
namespace Tts {

/**
 * Contains data required by engines to synthesize speech. This data is :
 * <ul>
 *   <li>The text to synthesize</li>
 *   <li>The synthesis locale, represented as a language, country and a variant.
 *   The language is an ISO 639-3 letter language code, and the country is an
 *   ISO 3166 alpha 3 code. The variant is not specified.</li>
 *   <li>The synthesis speech rate, with 100 being the normal, and
 *   higher values representing higher speech rates.</li>
 *   <li>The voice pitch, with 100 being the default pitch.</li>
 * </ul>
 *
 * Any additional parameters sent to the text to speech service are passed in
 * uninterpreted, see the @code{params} argument in {@link TextToSpeech#speak}
 * and {@link TextToSpeech#synthesizeToFile}.
 */
//final
class SynthesisRequest
{
public:
    SynthesisRequest(
        /* [in] */ const String& text,
        /* [in] */ IBundle* params);

    CARAPI_(void) Init(
        /* [in] */ const String& text,
        /* [in] */ IBundle* params);

    /**
     * Gets the text which should be synthesized.
     */
    CARAPI_(String) GetText();

    /**
     * Gets the ISO 3-letter language code for the language to use.
     */
    CARAPI_(String) GetLanguage();

    /**
     * Gets the ISO 3-letter country code for the language to use.
     */
    CARAPI_(String) GetCountry();

    /**
     * Gets the language variant to use.
     */
    CARAPI_(String) GetVariant();

    /**
     * Gets the speech rate to use. The normal rate is 100.
     */
    CARAPI_(Int32) GetSpeechRate();

    /**
     * Gets the pitch to use. The normal pitch is 100.
     */
    CARAPI_(Int32) GetPitch();

    /**
     * Gets the additional params, if any.
     */
    CARAPI_(AutoPtr<IBundle>) GetParams();

    /**
     * Sets the locale for the request.
     */
    CARAPI_(void) SetLanguage(
        /* [in] */ const String& language,
        /* [in] */ const String& country,
        /* [in] */ const String& variant);

    /**
     * Sets the speech rate.
     */
    CARAPI_(void) SetSpeechRate(
        /* [in] */ Int32 speechRate);

    /**
     * Sets the pitch.
     */
    CARAPI_(void) SetPitch(
        /* [in] */ Int32 pitch);

protected:
    SynthesisRequest();

private:
    String mText;
    AutoPtr<IBundle> mParams;
    String mLanguage;
    String mCountry;
    String mVariant;
    Int32 mSpeechRate;
    Int32 mPitch;
};

} // namespace Tts
} // namespace Speech
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_SPEECH_TTS_SynthesisRequest_H__
