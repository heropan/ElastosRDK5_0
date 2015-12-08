
#ifndef __ELASTOS_DROID_SPEECH_TTS_CTTSENGINESHELPER_H__
#define __ELASTOS_DROID_SPEECH_TTS_CTTSENGINESHELPER_H__

#include "_Elastos_Droid_Speech_Tts_CTtsEnginesHelper.h"

namespace Elastos {
namespace Droid {
namespace Speech {
namespace Tts {

CarClass(CTtsEnginesHelper)
{
public:
    CARAPI ParseLocalePref(
        /* [in] */ const String& pref,
        /* [out, callee] */ ArrayOf<String> ** ppRet);

private:
};

} // namespace Tts
} // namespace Speech
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_SPEECH_TTS_CTTSENGINESHELPER_H__
