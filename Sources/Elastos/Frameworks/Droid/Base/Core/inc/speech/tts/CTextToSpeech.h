
#ifndef __ELASTOS_DROID_SPEECH_TTS_CTEXTTOSPEECH_H__
#define __ELASTOS_DROID_SPEECH_TTS_CTEXTTOSPEECH_H__

#include "_Elastos_Droid_Speech_Tts_CTextToSpeech.h"
#include "speech/tts/TextToSpeech.h"

namespace Elastos {
namespace Droid {
namespace Speech {
namespace Tts {

CarClass(CTextToSpeech) , public TextToSpeech
{
public:
    CARAPI Shutdown();

    CARAPI AddSpeech(
        /* [in] */ const String& text,
        /* [in] */ const String& packagename,
        /* [in] */ Int32 resourceId,
        /* [out] */ Int32* ret);

    CARAPI AddSpeech(
        /* [in] */ const String& text,
        /* [in] */ const String& filename,
        /* [out] */ Int32* ret);

    CARAPI AddEarcon(
        /* [in] */ const String& earcon,
        /* [in] */ const String& packagename,
        /* [in] */ Int32 resourceId,
        /* [out] */ Int32* ret);

    CARAPI AddEarcon(
        /* [in] */ const String& earcon,
        /* [in] */ const String& filename,
        /* [out] */ Int32* ret);

    CARAPI Speak(
        /* [in] */ const String& text,
        /* [in] */ Int32 queueMode,
        /* [in] */ IObjectStringMap* params,
        /* [out] */ Int32* ret);

    CARAPI PlayEarcon(
        /* [in] */ const String& earcon,
        /* [in] */ Int32 queueMode,
        /* [in] */ IObjectStringMap* params,
        /* [out] */ Int32* ret);

    CARAPI PlaySilence(
        /* [in] */ Int64 durationInMs,
        /* [in] */ Int32 queueMode,
        /* [in] */ IObjectStringMap* params,
        /* [out] */ Int32* ret);

    CARAPI GetFeatures(
        /* [in] */ ILocale* locale,
        /* [out] */ IObjectContainer** ret);

    CARAPI IsSpeaking(
        /* [out] */ Boolean* ret);

    CARAPI Stop(
        /* [out] */ Int32* ret);

    CARAPI SetSpeechRate(
        /* [in] */ Float speechRate,
        /* [out] */ Int32* ret);

    CARAPI SetPitch(
        /* [in] */ Float pitch,
        /* [out] */ Int32* ret);

    CARAPI GetCurrentEngine(
        /* [out] */ String* ret);

    CARAPI SetLanguage(
        /* [in] */ ILocale* loc,
        /* [out] */ Int32* ret);

    CARAPI GetLanguage(
        /* [out] */ ILocale** ret);

    CARAPI IsLanguageAvailable(
        /* [in] */ ILocale* loc,
        /* [out] */ Int32* ret);

    CARAPI SynthesizeToFile(
        /* [in] */ const String& text,
        /* [in] */ IObjectStringMap* params,
        /* [in] */ const String& filename,
        /* [out] */ Int32* ret);

    CARAPI SetOnUtteranceCompletedListener(
        /* [in] */ ITextToSpeechOnUtteranceCompletedListener* listener,
        /* [out] */ Int32* ret);

    CARAPI SetOnUtteranceProgressListener(
        /* [in] */ IUtteranceProgressListener* listener,
        /* [out] */ Int32* ret);

    CARAPI SetEngineByPackageName(
        /* [in] */ const String& enginePackageName,
        /* [out] */ Int32* ret);

    CARAPI GetDefaultEngine(
        /* [out] */ String* ret);

    CARAPI AreDefaultsEnforced(
        /* [out] */ Boolean* ret);

    CARAPI GetEngines(
        /* [out] */ IObjectContainer** ret);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ ITextToSpeechOnInitListener* listener);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ ITextToSpeechOnInitListener* listener,
        /* [in] */ const String& engine);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ ITextToSpeechOnInitListener* listener,
        /* [in] */ const String& engine,
        /* [in] */ const String& packageName,
        /* [in] */ Boolean useFallback);

};

}//namespace Tts
}//namespace Speech
}//namespace Droid
}//namespace Elastos

#endif // __ELASTOS_DROID_SPEECH_TTS_CTEXTTOSPEECH_H__
