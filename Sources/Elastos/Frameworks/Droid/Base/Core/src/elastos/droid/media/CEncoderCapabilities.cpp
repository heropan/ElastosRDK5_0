
#include "elastos/droid/media/CEncoderCapabilities.h"

#include <media/MediaProfiles.h>
#include <elastos/utility/etl/List.h>

using Elastos::Utility::Etl::List;


namespace Elastos {
namespace Droid {
namespace Media {

//7e15e374-9cb8-49de-bbc4-84a30ac7e036
extern "C" const InterfaceID EIID_AudioEncoderCap =
        { 0x7e15e374, 0x9cb8, 0x49de, { 0xbb, 0xc4, 0x84, 0xa3, 0x0a, 0xc7, 0xe0, 0x36 } };

//71e4c0c0-d344-43a7-a063-ad746682bed5
extern "C" const InterfaceID EIID_VideoEncoderCap =
        { 0x71e4c0c0, 0xd344, 0x43a7, { 0xa0, 0x63, 0xad, 0x74, 0x66, 0x82, 0xbe, 0xd5 } };

CAR_INTERFACE_IMPL_WITH_CPP_CAST(CEncoderCapabilities::VideoEncoderCap, IInterface, VideoEncoderCap)
CAR_INTERFACE_IMPL_WITH_CPP_CAST(CEncoderCapabilities::AudioEncoderCap, IInterface, AudioEncoderCap)

//=============================================================

static Object sLock;
android::MediaProfiles *sssProfiles = NULL;

/*static*/ String CEncoderCapabilities::TAG("EncoderCapabilities");

Boolean CEncoderCapabilities::InitStatic()
{
//    System.loadLibrary("media_jni");
//    ALOGV("native_init");
    AutoLock lock(sLock);

    if (sssProfiles == NULL) {
        sssProfiles = android::MediaProfiles::getInstance();
    }
    return TRUE;
}

/*static*/ Boolean CEncoderCapabilities::mInit = InitStatic();

CEncoderCapabilities::VideoEncoderCap::VideoEncoderCap(
    /* [in] */ Int32 codec,
    /* [in] */ Int32 minBitRate,
    /* [in] */ Int32 maxBitRate,
    /* [in] */ Int32 minFrameRate,
    /* [in] */ Int32 maxFrameRate,
    /* [in] */ Int32 minFrameWidth,
    /* [in] */ Int32 maxFrameWidth,
    /* [in] */ Int32 minFrameHeight,
    /* [in] */ Int32 maxFrameHeight)
    : mCodec(codec)
    , mMinBitRate(minBitRate)
    , mMaxBitRate(maxBitRate)
    , mMinFrameRate(minFrameRate)
    , mMaxFrameRate(maxFrameRate)
    , mMinFrameWidth(minFrameWidth)
    , mMaxFrameWidth(maxFrameWidth)
    , mMinFrameHeight(minFrameHeight)
    , mMaxFrameHeight(maxFrameHeight)
{}

CEncoderCapabilities::AudioEncoderCap::AudioEncoderCap(
    /* [in] */ Int32 codec,
    /* [in] */ Int32 minBitRate,
    /* [in] */ Int32 maxBitRate,
    /* [in] */ Int32 minSampleRate,
    /* [in] */ Int32 maxSampleRate,
    /* [in] */ Int32 minChannels,
    /* [in] */ Int32 maxChannels)
    : mCodec(codec)
    , mMinChannels(minChannels)
    , mMaxChannels(maxChannels)
    , mMinSampleRate(minSampleRate)
    , mMaxSampleRate(maxSampleRate)
    , mMinBitRate(minBitRate)
    , mMaxBitRate(maxBitRate)
{}

CEncoderCapabilities::CEncoderCapabilities()
{}

CEncoderCapabilities::~CEncoderCapabilities()
{}

ECode CEncoderCapabilities::constructor()
{
    return NOERROR;
}

ECode CEncoderCapabilities::GetOutputFileFormats(
    /* [out,callee] */ ArrayOf<Int32>** result)
{
    VALIDATE_NOT_NULL(result);
    *result = NULL;

    Int32 nFormats = Native_get_num_file_formats();
    if (nFormats == 0) {
        return NOERROR;
    }

    AutoPtr< ArrayOf<Int32> > formats = ArrayOf<Int32>::Alloc(nFormats);
    for (Int32 i = 0; i < nFormats; ++i) {
        (*formats)[i] = Native_get_file_format(i);
    }

    *result = formats;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

ECode CEncoderCapabilities::GetVideoEncoders(
    /* [out] */ IObjectContainer** result)
{
    VALIDATE_NOT_NULL(result);
    *result = NULL;

    Int32 nEncoders = Native_get_num_video_encoders();
    if (nEncoders == 0) {
        return NOERROR;
    }

    List< AutoPtr<VideoEncoderCap> > encoderList;
    for (Int32 i = 0; i < nEncoders; ++i) {
        encoderList.PushBack(Native_get_video_encoder_cap(i));
    }

    AutoPtr<IObjectContainer> oc;
    CObjectContainer::New((IObjectContainer**)&oc);
    List< AutoPtr<VideoEncoderCap> >::Iterator it = encoderList.Begin();
    for(; it != encoderList.End(); ++it) {
        oc->Add((*it));
    }
    *result = oc;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

ECode CEncoderCapabilities::GetAudioEncoders(
    /* [out] */ IObjectContainer** result)
{
    VALIDATE_NOT_NULL(result);
    *result = NULL;

    Int32 nEncoders = Native_get_num_audio_encoders();
    if (nEncoders == 0) {
        return NOERROR;
    }

    List< AutoPtr<AudioEncoderCap> > encoderList;
    for (Int32 i = 0; i < nEncoders; ++i) {
        encoderList.PushBack(Native_get_audio_encoder_cap(i));
    }

    AutoPtr<IObjectContainer> oc;
    CObjectContainer::New((IObjectContainer**)&oc);
    List< AutoPtr<AudioEncoderCap> >::Iterator it = encoderList.Begin();
    for(; it != encoderList.End(); ++it) {
        oc->Add((*it));
    }
    *result = oc;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

//---------------------------------------------------
//    Native method : android_media_MediaProfiles.cpp
//---------------------------------------------------

Int32 CEncoderCapabilities::Native_get_num_file_formats()
{
//    ALOGV("native_get_num_file_formats");
    return sssProfiles->getOutputFileFormats().size();
}

Int32 CEncoderCapabilities::Native_get_file_format(
    /* [in] */ Int32 index)
{
//    ALOGV("native_get_file_format: %d", index);
    android::Vector<android::output_format> formats = sssProfiles->getOutputFileFormats();
    Int32 nSize = formats.size();
    if (index < 0 || index >= nSize) {
//        jniThrowException(env, "java/lang/IllegalArgumentException", "out of array boundary");
        return -1;
    }
    return formats[index];
}

Int32 CEncoderCapabilities::Native_get_num_video_encoders()
{
//    ALOGV("native_get_num_video_encoders");
    return sssProfiles->getVideoEncoders().size();
}

AutoPtr<CEncoderCapabilities::VideoEncoderCap> CEncoderCapabilities::Native_get_video_encoder_cap(
    /* [in] */ Int32 index)
{
//    ALOGV("native_get_video_encoder_cap: %d", index);
    android::Vector<android::video_encoder> encoders = sssProfiles->getVideoEncoders();
    Int32 nSize = encoders.size();
    if (index < 0 || index >= nSize) {
//        jniThrowException(env, "java/lang/IllegalArgumentException", "out of array boundary");
        return NULL;
    }

    android::video_encoder encoder = encoders[index];
    Int32 minBitRate = sssProfiles->getVideoEncoderParamByName("enc.vid.bps.min", encoder);
    Int32 maxBitRate = sssProfiles->getVideoEncoderParamByName("enc.vid.bps.max", encoder);
    Int32 minFrameRate = sssProfiles->getVideoEncoderParamByName("enc.vid.fps.min", encoder);
    Int32 maxFrameRate = sssProfiles->getVideoEncoderParamByName("enc.vid.fps.max", encoder);
    Int32 minFrameWidth = sssProfiles->getVideoEncoderParamByName("enc.vid.width.min", encoder);
    Int32 maxFrameWidth = sssProfiles->getVideoEncoderParamByName("enc.vid.width.max", encoder);
    Int32 minFrameHeight = sssProfiles->getVideoEncoderParamByName("enc.vid.height.min", encoder);
    Int32 maxFrameHeight = sssProfiles->getVideoEncoderParamByName("enc.vid.height.max", encoder);

    // Check on the values retrieved
    if ((minBitRate == -1 || maxBitRate == -1) ||
        (minFrameRate == -1 || maxFrameRate == -1) ||
        (minFrameWidth == -1 || maxFrameWidth == -1) ||
        (minFrameHeight == -1 || maxFrameHeight == -1)) {

//        jniThrowException(env, "java/lang/RuntimeException", "Error retrieving video encoder capability params");
        return NULL;
    }

    // Construct an instance of the VideoEncoderCap and set its member variables
//     jclass videoEncoderCapClazz = env->FindClass("android/media/EncoderCapabilities$VideoEncoderCap");
//     jmethodID videoEncoderCapConstructorMethodID = env->GetMethodID(videoEncoderCapClazz, "<init>", "(IIIIIIIII)V");
//     jobject cap = env->NewObject(videoEncoderCapClazz,
//                                  videoEncoderCapConstructorMethodID,
//                                  static_cast<int>(encoder),
//                                  minBitRate, maxBitRate,
//                                  minFrameRate, maxFrameRate,
//                                  minFrameWidth, maxFrameWidth,
//                                  minFrameHeight, maxFrameHeight);
//     return cap;
    AutoPtr<VideoEncoderCap> cap = new VideoEncoderCap(encoder,
                                                       minBitRate,
                                                       maxBitRate,
                                                       minFrameRate,
                                                       maxFrameRate,
                                                       minFrameWidth,
                                                       maxFrameWidth,
                                                       minFrameHeight,
                                                       maxFrameHeight);
    return cap;
}

Int32 CEncoderCapabilities::Native_get_num_audio_encoders()
{
//    ALOGV("native_get_num_audio_encoders");
    return sssProfiles->getAudioEncoders().size();
}

AutoPtr<CEncoderCapabilities::AudioEncoderCap> CEncoderCapabilities::Native_get_audio_encoder_cap(
    /* [in] */ Int32 index)
{
//    ALOGV("native_get_audio_encoder_cap: %d", index);
    android::Vector<android::audio_encoder> encoders = sssProfiles->getAudioEncoders();
    Int32 nSize = encoders.size();
    if (index < 0 || index >= nSize) {
//        jniThrowException(env, "java/lang/IllegalArgumentException", "out of array boundary");
        return NULL;
    }

    android::audio_encoder encoder = encoders[index];
    Int32 minBitRate = sssProfiles->getAudioEncoderParamByName("enc.aud.bps.min", encoder);
    Int32 maxBitRate = sssProfiles->getAudioEncoderParamByName("enc.aud.bps.max", encoder);
    Int32 minSampleRate = sssProfiles->getAudioEncoderParamByName("enc.aud.hz.min", encoder);
    Int32 maxSampleRate = sssProfiles->getAudioEncoderParamByName("enc.aud.hz.max", encoder);
    Int32 minChannels = sssProfiles->getAudioEncoderParamByName("enc.aud.ch.min", encoder);
    Int32 maxChannels = sssProfiles->getAudioEncoderParamByName("enc.aud.ch.max", encoder);

    // Check on the values retrieved
    if ((minBitRate == -1 || maxBitRate == -1) ||
        (minSampleRate == -1 || maxSampleRate == -1) ||
        (minChannels == -1 || maxChannels == -1)) {

//        jniThrowException(env, "java/lang/RuntimeException", "Error retrieving video encoder capability params");
        return NULL;
    }

//     jclass audioEncoderCapClazz = env->FindClass("android/media/EncoderCapabilities$AudioEncoderCap");
//     jmethodID audioEncoderCapConstructorMethodID = env->GetMethodID(audioEncoderCapClazz, "<init>", "(IIIIIII)V");
//     jobject cap = env->NewObject(audioEncoderCapClazz,
//                                  audioEncoderCapConstructorMethodID,
//                                  static_cast<int>(encoder),
//                                  minBitRate, maxBitRate,
//                                  minSampleRate, maxSampleRate,
//                                  minChannels, maxChannels);
//     return cap;
    AutoPtr<AudioEncoderCap> cap = new AudioEncoderCap(encoder,
                                                       minBitRate,
                                                       maxBitRate,
                                                       minSampleRate,
                                                       maxSampleRate,
                                                       minChannels,
                                                       maxChannels);
    return cap;
}

} // namespace Media
} // namepsace Droid
} // namespace Elastos
