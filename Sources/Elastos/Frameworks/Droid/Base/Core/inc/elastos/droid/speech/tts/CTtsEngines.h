
#ifndef __ELASTOS_DROID_SPEECH_TTS_CTTSENGINES_H__
#define __ELASTOS_DROID_SPEECH_TTS_CTTSENGINES_H__

#include "_Elastos_Droid_Speech_Tts_CTtsEngines.h"

#include "elastos/droid/speech/tts/TtsEngines.h"

namespace Elastos {
namespace Droid {
namespace Speech {
namespace Tts {

CarClass(CTtsEngines)
    , public TtsEngines
{
public:
    CAR_OBJECT_DECL();

    CTtsEngines();

    virtual ~CTtsEngines();

private:
};

} // namespace Tts
} // namespace Speech
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_SPEECH_TTS_CTTSENGINES_H__
