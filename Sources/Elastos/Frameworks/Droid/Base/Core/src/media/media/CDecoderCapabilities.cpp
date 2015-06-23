
#include "media/CDecoderCapabilities.h"

#include <media/MediaProfiles.h>
#include <elastos/utility/etl/List.h>

using Elastos::Utility::Etl::List;
using Elastos::Core::CObjectContainer;

namespace Elastos {
namespace Droid {
namespace Media {

//=================================================================================
//              CDecoderCapabilities
//=================================================================================
Boolean CDecoderCapabilities::InitStatic()
{
//    System.loadLibrary("media_jni");
    Native_init();
    return TRUE;
}

/*static*/ Boolean CDecoderCapabilities::mInit = InitStatic();

CDecoderCapabilities::CDecoderCapabilities()
{}

CDecoderCapabilities::~CDecoderCapabilities()
{}

ECode CDecoderCapabilities::constructor()
{
    return NOERROR;
}

ECode CDecoderCapabilities::GetVideoDecoders(
    /* [out, callee] */ ArrayOf<VideoDecoder>** result)
{
    VALIDATE_NOT_NULL(result);


    Int32 nDecoders = Native_get_num_video_decoders();
    AutoPtr<ArrayOf<VideoDecoder> > vd = ArrayOf<VideoDecoder>::Alloc(nDecoders);
    for (Int32 i = 0; i < nDecoders; i++) {
        vd->Set(i, VIDEO_DECODER_WMV);
//        decoderList->PushBack(VideoDecoder.values()[Native_get_video_decoder_type(i)]);
    }

    *result = vd;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

ECode CDecoderCapabilities::GetAudioDecoders(
    /* [out, callee] */ ArrayOf<AudioDecoder>** result)
{
    VALIDATE_NOT_NULL(result);

    Int32 nDecoders = Native_get_num_audio_decoders();
    AutoPtr<ArrayOf<AudioDecoder> > ad = ArrayOf<AudioDecoder>::Alloc(nDecoders);
    for (Int32 i = 0; i < nDecoders; i++) {
        ad->Set(i, AUDIO_DECODER_WMA);
//        decoderList->PushBack(AudioDecoder.values()[Native_get_audio_decoder_type(i)]);
    }

    *result = ad;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

//---------------------------------------------------
//    Native method : android_media_MediaProfiles.cpp
//---------------------------------------------------

static Mutex sLock;
android::MediaProfiles *ssProfiles = NULL;

void CDecoderCapabilities::Native_init()
{
//    ALOGV("native_init");
    Mutex::Autolock lock(sLock);

    if (ssProfiles == NULL) {
        ssProfiles = android::MediaProfiles::getInstance();
    }
}

Int32 CDecoderCapabilities::Native_get_num_video_decoders()
{
//    ALOGV("native_get_num_video_decoders");
    return ssProfiles->getVideoDecoders().size();
}

Int32 CDecoderCapabilities::Native_get_video_decoder_type(
    /* [in] */ Int32 index)
{
//    ALOGV("native_get_video_decoder_type: %d", index);
    android::Vector<android::video_decoder> decoders = ssProfiles->getVideoDecoders();
    Int32 nSize = decoders.size();
    if (index < 0 || index >= nSize) {
//        jniThrowException(env, "java/lang/IllegalArgumentException", "out of array boundary");
        return -1;
    }

    return decoders[index];
}

Int32 CDecoderCapabilities::Native_get_num_audio_decoders()
{
//    ALOGV("native_get_num_audio_decoders");
    return ssProfiles->getAudioDecoders().size();
}

Int32 CDecoderCapabilities::Native_get_audio_decoder_type(
    /* [in] */ Int32 index)
{
//    ALOGV("native_get_audio_decoder_type: %d", index);
    android::Vector<android::audio_decoder> decoders = ssProfiles->getAudioDecoders();
    Int32 nSize = decoders.size();
    if (index < 0 || index >= nSize) {
//        jniThrowException(env, "java/lang/IllegalArgumentException", "out of array boundary");
        return -1;
    }

    return decoders[index];
}

} // namespace Media
} // namepsace Droid
} // namespace Elastos
