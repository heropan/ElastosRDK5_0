
#ifndef __CMEDIARECORDERHELPER_H__
#define __CMEDIARECORDERHELPER_H__

#include "_CMediaRecorderHelper.h"
#include "ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Media {

CarClass(CMediaRecorderHelper)
{
public:
    /**
     * Gets the maximum value for audio sources.
     * @see android.media.MediaRecorder.AudioSource
     */
    CARAPI GetAudioSourceMax(
        /* [out] */ Int32* result);

};

} // namespace Media
} // namespace Droid
} // namespace Elastos

#endif // __CMEDIARECORDERHELPER_H__
