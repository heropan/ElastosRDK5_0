#include "CVideoEditorProfile.h"
#include "media/MediaProfiles.h"

using namespace android;

namespace Elastos {
namespace Droid {
namespace Media {
namespace Videoeditor {

CVideoEditorProfile::CVideoEditorProfile()
    : maxInputVideoFrameWidth(0)
    , maxInputVideoFrameHeight(0)
    , maxOutputVideoFrameWidth(0)
    , maxOutputVideoFrameHeight(0)
{
}

ECode CVideoEditorProfile::constructor(
    /* [in] */ Int32 inputWidth,
    /* [in] */ Int32 inputHeight,
    /* [in] */ Int32 outputWidth,
    /* [in] */ Int32 outputHeight)
{
    maxInputVideoFrameWidth = inputWidth;
    maxInputVideoFrameHeight = inputHeight;
    maxOutputVideoFrameWidth = outputWidth;
    maxOutputVideoFrameHeight = outputHeight;
    return NOERROR;
}

ECode CVideoEditorProfile::Get(
    /* [out] */ IVideoEditorProfile** result)
{
    *result = NativeGetVideoeditorProfile();
    INTERFACE_ADDREF(*result);
    return NOERROR;
}

ECode CVideoEditorProfile::GetExportProfile(
    /* [in] */ Int32 vidCodec,
    /* [out] */ Int32* result)
{
    Int32 profile = -1;

    switch (vidCodec) {
        case IMediaProperties::VCODEC_H263:
        case IMediaProperties::VCODEC_H264:
        case IMediaProperties::VCODEC_MPEG4:
             profile = NativeGetVideoeditorExportProfile(vidCodec);
             break;
        default :
           //throw new IllegalArgumentException("Unsupported video codec" + vidCodec);
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    *result = profile;
    return NOERROR;
}

ECode CVideoEditorProfile::GetExportLevel(
    /* [in] */ Int32 vidCodec,
    /* [out] */ Int32* result)
{
    Int32 level = -1;

    switch (vidCodec) {
        case IMediaProperties::VCODEC_H263:
        case IMediaProperties::VCODEC_H264:
        case IMediaProperties::VCODEC_MPEG4:
             level = NativeGetVideoeditorExportLevel(vidCodec);
             break;
        default :
           //throw new IllegalArgumentException("Unsupported video codec" + vidCodec);
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    *result = level;
    return NOERROR;
}

static android::Mutex sLock;
MediaProfiles *sProfiles = NULL;

void CVideoEditorProfile::NativeInit()
{
    ALOGV("native_init");
    android::Mutex::Autolock lock(sLock);

    if (sProfiles == NULL) {
        sProfiles = MediaProfiles::getInstance();
    }
}

AutoPtr<IVideoEditorProfile> CVideoEditorProfile::NativeGetVideoeditorProfile()
{
    ALOGV("native_get_videoeditor_profile");

    Int32 maxInputFrameWidth =
            sProfiles->getVideoEditorCapParamByName("videoeditor.input.width.max");
    Int32 maxInputFrameHeight =
            sProfiles->getVideoEditorCapParamByName("videoeditor.input.height.max");
    Int32 maxOutputFrameWidth =
            sProfiles->getVideoEditorCapParamByName("videoeditor.output.width.max");
    Int32 maxOutputFrameHeight =
            sProfiles->getVideoEditorCapParamByName("videoeditor.output.height.max");

    // Check on the values retrieved
    if (maxInputFrameWidth == -1 || maxInputFrameHeight == -1 ||
        maxOutputFrameWidth == -1 || maxOutputFrameHeight == -1) {

    //    jniThrowException(env, "java/lang/RuntimeException",\
    //        "Error retrieving videoeditor profile params");
        return NULL;
    }
    ALOGV("native_get_videoeditor_profile \
       inWidth:%d inHeight:%d,outWidth:%d, outHeight:%d",\
       maxInputFrameWidth,maxInputFrameHeight,\
       maxOutputFrameWidth,maxOutputFrameHeight);

    AutoPtr<IVideoEditorProfile> profile;
    CVideoEditorProfile::New(maxInputFrameWidth, maxInputFrameHeight,
        maxOutputFrameWidth, maxOutputFrameHeight, (IVideoEditorProfile**)&profile);
    return profile;
}

Int32 CVideoEditorProfile::NativeGetVideoeditorExportProfile(
    /* [in] */ Int32 codec)
{
    ALOGV("android_media_MediaProfiles_native_get_export_profile index ");
    Int32 profile = 0;
    profile = sProfiles->getVideoEditorExportParamByName("videoeditor.export.profile", codec);
    // Check the values retrieved
    if (profile == -1) {
        //jniThrowException(env, "java/lang/RuntimeException",\ "Error retrieving videoeditor export profile params");
        return -1;
    }
    return profile;
}

Int32 CVideoEditorProfile::NativeGetVideoeditorExportLevel(
    /* [in] */ Int32 codec)
{
    ALOGV("android_media_MediaProfiles_native_get_export_level");
    Int32 level = 0;
    level = sProfiles->getVideoEditorExportParamByName("videoeditor.export.level", codec);
    // Check the values retrieved
    if (level == -1) {
        //jniThrowException(env, "java/lang/RuntimeException",\ "Error retrieving videoeditor export level params");
        return -1;
    }
    return level;
}

} // namespace Videoeditor
} // namespace Media
} // namepsace Droid
} // namespace Elastos
