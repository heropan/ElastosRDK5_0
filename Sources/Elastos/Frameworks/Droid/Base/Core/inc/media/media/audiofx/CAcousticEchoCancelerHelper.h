
#ifndef __ELASTOS_DROID_MEDIA_AUDIOFX_CACOUSTICECHOCANCELERHELPER_H__
#define __ELASTOS_DROID_MEDIA_AUDIOFX_CACOUSTICECHOCANCELERHELPER_H__

#include "_CAcousticEchoCancelerHelper.h"
#include <ext/frameworkext.h>

namespace Elastos {
namespace Droid {
namespace Media {
namespace Audiofx {

CarClass(CAcousticEchoCancelerHelper)
{
public:
    /**
     * Checks if the device implements acoustic echo cancellation.
     * @return true if the device implements acoustic echo cancellation, false otherwise.
     */
    CARAPI IsAvailable(
        /* [out] */ Boolean* result);

        /**
     * Creates an AcousticEchoCanceler and attaches it to the AudioRecord on the audio
     * session specified.
     * @param audioSession system wide unique audio session identifier. The AcousticEchoCanceler
     * will be applied to the AudioRecord with the same audio session.
     * @return AcousticEchoCanceler created or null if the device does not implement AEC.
     */
    CARAPI Create(
        /* [in] */ Int32 audioSession,
        /* [out] */ IAcousticEchoCanceler** result);
};

} // namespace Audiofx
} // namespace Media
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_MEDIA_AUDIOFX_CACOUSTICECHOCANCELERHELPER_H__

