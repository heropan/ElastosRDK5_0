
#ifndef __ELASTOS_DROID_MEDIA_CDECODERCAPABILITIES_H__
#define __ELASTOS_DROID_MEDIA_CDECODERCAPABILITIES_H__

#include "_Elastos_Droid_Media_CDecoderCapabilities.h"
#include "elastos/droid/ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Media {

extern "C" const InterfaceID EIID_AudioDecoder;
extern "C" const InterfaceID EIID_VideoDecoder;

CarClass(CDecoderCapabilities)
{
public:
    CDecoderCapabilities();

    ~CDecoderCapabilities();

    CARAPI constructor();

    /**
     * Returns the list of video decoder types
     * @see android.media.DecoderCapabilities.VideoDecoder
     */
    static CARAPI GetVideoDecoders(
        /* [out, callee] */ ArrayOf<VideoDecoder>** result);

    /**
     * Returns the list of audio decoder types
     * @see android.media.DecoderCapabilities.AudioDecoder
     */
    static CARAPI GetAudioDecoders(
        /* [out, callee] */ ArrayOf<AudioDecoder>** result);

private:
    // Implemented by JNI
    static CARAPI_(void) Native_init();

    static CARAPI_(Int32) Native_get_num_video_decoders();

    static CARAPI_(Int32) Native_get_video_decoder_type(
        /* [in] */ Int32 index);

    static CARAPI_(Int32) Native_get_num_audio_decoders();

    static CARAPI_(Int32) Native_get_audio_decoder_type(
        /* [in] */ Int32 index);

    static CARAPI_(Boolean) InitStatic();

private:
    static Boolean mInit;
};

} // namespace Media
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_MEDIA_CDECODERCAPABILITIES_H__
