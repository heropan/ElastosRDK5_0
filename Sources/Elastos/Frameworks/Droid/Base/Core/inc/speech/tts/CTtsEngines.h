
#ifndef __ELASTOS_DROID_SPEECH_TTS_CTTSENGINES_H__
#define __ELASTOS_DROID_SPEECH_TTS_CTTSENGINES_H__

#include "_CTtsEngines.h"

#include "speech/tts/TtsEngines.h"

namespace Elastos {
namespace Droid {
namespace Speech {
namespace Tts {

CarClass(CTtsEngines)
    , public TtsEngines
{
public:
    CARAPI GetDefaultEngine(
        /* [out] */ String * pRet);

    CARAPI GetHighestRankedEngineName(
        /* [out] */ String * pRet);

    CARAPI GetEngineInfo(
        /* [in] */ const String& packageName,
        /* [out] */ ITextToSpeechEngineInfo ** ppRet);

    CARAPI GetEngines(
        /* [out] */ IObjectContainer ** ppRet);

    CARAPI IsEngineInstalled(
        /* [in] */ const String& engine,
        /* [out] */ Boolean * pRet);

    CARAPI GetSettingsIntent(
        /* [in] */ const String& engine,
        /* [out] */ IIntent ** ppRet);

    CARAPI GetLocalePrefForEngine(
        /* [in] */ const String& engineName,
        /* [out] */ String * pRet);

    CARAPI UpdateLocalePrefForEngine(
        /* [in] */ const String& name,
        /* [in] */ const String& newLocale);

    CARAPI constructor(
        /* [in] */ IContext * pCtx);

private:
    // TODO: Add your private member variables here.
};

}//namespace Tts
}//namespace Speech
}//namespace Droid
}//namespace Elastos

#endif // __ELASTOS_DROID_SPEECH_TTS_CTTSENGINES_H__
