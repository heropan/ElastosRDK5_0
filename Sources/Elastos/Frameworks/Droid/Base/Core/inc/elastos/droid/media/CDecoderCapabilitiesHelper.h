
#ifndef __ELASTOS_DROID_MEDIA_CDECODERCAPABILITIESHELPER_H__
#define __ELASTOS_DROID_MEDIA_CDECODERCAPABILITIESHELPER_H__

#include "_Elastos_Droid_Media_CDecoderCapabilitiesHelper.h"
#include "elastos/droid/ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Media {

CarClass(CDecoderCapabilitiesHelper)
{
public:
    /**
     * Returns the list of video decoder types
     * @see android.media.DecoderCapabilities.VideoDecoder
     */
    CARAPI GetVideoDecoders(
        /* [out, callee] */ ArrayOf<VideoDecoder>** result);

    /**
     * Returns the list of audio decoder types
     * @see android.media.DecoderCapabilities.AudioDecoder
     */
    CARAPI GetAudioDecoders(
        /* [out, callee] */ ArrayOf<AudioDecoder>** result);
};

} // namespace Media
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_MEDIA_CDECODERCAPABILITIESHELPER_H__
