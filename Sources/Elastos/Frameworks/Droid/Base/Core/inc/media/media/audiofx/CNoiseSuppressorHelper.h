
#ifndef __CNOISESUPPRESSORHELPER_H__
#define __CNOISESUPPRESSORHELPER_H__

#include "_CNoiseSuppressorHelper.h"
#include <ext/frameworkext.h>

namespace Elastos {
namespace Droid {
namespace Media {
namespace Audiofx {

CarClass(CNoiseSuppressorHelper)
{
public:
    /**
     * Checks if the device implements noise suppression.
     * @return true if the device implements noise suppression, false otherwise.
     */
    CARAPI IsAvailable(
        /* [out] */ Boolean* result);

    /**
     * Creates a NoiseSuppressor and attaches it to the AudioRecord on the audio
     * session specified.
     * @param audioSession system wide unique audio session identifier. The NoiseSuppressor
     * will be applied to the AudioRecord with the same audio session.
     * @return NoiseSuppressor created or null if the device does not implement noise
     * suppression.
     */
    CARAPI Create(
        /* [in] */ Int32 audioSession,
        /* [out] */ INoiseSuppressor** result);
};

} // namespace Audiofx
} // namespace Media
} // namepsace Droid
} // namespace Elastos

#endif // __CNOISESUPPRESSORHELPER_H__

