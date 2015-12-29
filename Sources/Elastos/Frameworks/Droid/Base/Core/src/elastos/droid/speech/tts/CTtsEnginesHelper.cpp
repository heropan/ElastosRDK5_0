
#include "elastos/droid/speech/tts/CTtsEnginesHelper.h"
#include "elastos/droid/speech/tts/CTtsEngines.h"
#include "elastos/droid/ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Speech {
namespace Tts {

ECode CTtsEnginesHelper::NormalizeTTSLocale(
    /* [in] */ ILocale* ttsLocale,
    /* [out] */ ILocale** outLocale)
{
    return TtsEngines::NormalizeTTSLocale(ttsLocale, outLocale);
}

} // namespace Tts
} // namespace Speech
} // namespace Droid
} // namespace Elastos

