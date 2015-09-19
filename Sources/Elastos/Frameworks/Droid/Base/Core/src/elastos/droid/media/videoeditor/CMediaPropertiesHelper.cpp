
#include "ext/frameworkext.h"
#include "CMediaPropertiesHelper.h"
#include "CVideoEditorProfileHelper.h"
#include "CVideoEditorProfile.h"
#include "utility/CPair.h"

using Elastos::Core::IInteger32;
using Elastos::Core::CInteger32;
using Elastos::Droid::Utility::CPair;

namespace Elastos {
namespace Droid {
namespace Media {
namespace Videoeditor {

ArrayOf<Int32>* CMediaPropertiesHelper::ASPECT_RATIOS;
ArrayOf<IPair*>* CMediaPropertiesHelper::ASPECT_RATIO_3_2_RESOLUTIONS;
ArrayOf<IPair*>* CMediaPropertiesHelper::ASPECT_RATIO_4_3_RESOLUTIONS;
ArrayOf<IPair*>* CMediaPropertiesHelper::ASPECT_RATIO_5_3_RESOLUTIONS;
ArrayOf<IPair*>* CMediaPropertiesHelper::ASPECT_RATIO_11_9_RESOLUTIONS;
ArrayOf<IPair*>* CMediaPropertiesHelper::ASPECT_RATIO_16_9_RESOLUTIONS;
ArrayOf<Int32>* CMediaPropertiesHelper::SUPPORTED_BITRATES;
ArrayOf<Int32>* CMediaPropertiesHelper::SUPPORTED_VCODECS;
ArrayOf<Int32>* CMediaPropertiesHelper::SUPPORTED_ACODECS ;
ArrayOf<Int32>* CMediaPropertiesHelper::SUPPORTED_VIDEO_FILE_FORMATS;

CMediaPropertiesHelper::CMediaPropertiesHelper()
{
    ASPECT_RATIOS = ArrayOf<Int32>::Alloc(5);
    ASPECT_RATIOS->Set(0, IMediaProperties::ASPECT_RATIO_3_2);
    ASPECT_RATIOS->Set(1, IMediaProperties::ASPECT_RATIO_16_9);
    ASPECT_RATIOS->Set(2, IMediaProperties::ASPECT_RATIO_4_3);
    ASPECT_RATIOS->Set(3, IMediaProperties::ASPECT_RATIO_5_3);
    ASPECT_RATIOS->Set(4, IMediaProperties::ASPECT_RATIO_11_9);

    AutoPtr<IInteger32> a, b;
    AutoPtr<IPair> tmp;

    ASPECT_RATIO_3_2_RESOLUTIONS = ArrayOf<IPair*>::Alloc(2);
    CInteger32::New(720, (IInteger32**)&a);
    CInteger32::New(IMediaProperties::HEIGHT_480, (IInteger32**)&b);
    CPair::New(a, b, (IPair**)&tmp);
    ASPECT_RATIO_3_2_RESOLUTIONS->Set(0, tmp);
    a = NULL;
    b = NULL;
    tmp = NULL;
    CInteger32::New(1080, (IInteger32**)&a);
    CInteger32::New(IMediaProperties::HEIGHT_720, (IInteger32**)&b);
    CPair::New(a, b, (IPair**)&tmp);
    ASPECT_RATIO_3_2_RESOLUTIONS->Set(1, tmp);

    a = NULL;
    b = NULL;
    tmp = NULL;
    ASPECT_RATIO_4_3_RESOLUTIONS = ArrayOf<IPair*>::Alloc(2);
    CInteger32::New(640, (IInteger32**)&a);
    CInteger32::New(IMediaProperties::HEIGHT_480, (IInteger32**)&b);
    CPair::New(a, b, (IPair**)&tmp);
    ASPECT_RATIO_4_3_RESOLUTIONS->Set(0, tmp);
    a = NULL;
    b = NULL;
    tmp = NULL;
    CInteger32::New(960, (IInteger32**)&a);
    CInteger32::New(IMediaProperties::HEIGHT_720, (IInteger32**)&b);
    CPair::New(a, b, (IPair**)&tmp);
    ASPECT_RATIO_4_3_RESOLUTIONS->Set(1, tmp);

    a = NULL;
    b = NULL;
    tmp = NULL;
    ASPECT_RATIO_5_3_RESOLUTIONS = ArrayOf<IPair*>::Alloc(1);
    CInteger32::New(800, (IInteger32**)&a);
    CInteger32::New(IMediaProperties::HEIGHT_480, (IInteger32**)&b);
    CPair::New(a, b, (IPair**)&tmp);
    ASPECT_RATIO_5_3_RESOLUTIONS->Set(0, tmp);

    a = NULL;
    b = NULL;
    tmp = NULL;
    ASPECT_RATIO_11_9_RESOLUTIONS = ArrayOf<IPair*>::Alloc(2);
    CInteger32::New(176, (IInteger32**)&a);
    CInteger32::New(IMediaProperties::HEIGHT_144, (IInteger32**)&b);
    CPair::New(a, b, (IPair**)&tmp);
    ASPECT_RATIO_11_9_RESOLUTIONS->Set(0, tmp);
    a = NULL;
    b = NULL;
    tmp = NULL;
    CInteger32::New(352, (IInteger32**)&a);
    CInteger32::New(IMediaProperties::HEIGHT_288, (IInteger32**)&b);
    CPair::New(a, b, (IPair**)&tmp);
    ASPECT_RATIO_11_9_RESOLUTIONS->Set(1, tmp);

    a = NULL;
    b = NULL;
    tmp = NULL;
    ASPECT_RATIO_16_9_RESOLUTIONS = ArrayOf<IPair*>::Alloc(3);
    CInteger32::New(848, (IInteger32**)&a);
    CInteger32::New(IMediaProperties::HEIGHT_480, (IInteger32**)&b);
    CPair::New(a, b, (IPair**)&tmp);
    ASPECT_RATIO_16_9_RESOLUTIONS->Set(0, tmp);
    a = NULL;
    b = NULL;
    tmp = NULL;
    CInteger32::New(1280, (IInteger32**)&a);
    CInteger32::New(IMediaProperties::HEIGHT_720, (IInteger32**)&b);
    CPair::New(a, b, (IPair**)&tmp);
    ASPECT_RATIO_16_9_RESOLUTIONS->Set(1, tmp);
    a = NULL;
    b = NULL;
    tmp = NULL;
    CInteger32::New(1920, (IInteger32**)&a);
    CInteger32::New(IMediaProperties::HEIGHT_1080, (IInteger32**)&b);
    CPair::New(a, b, (IPair**)&tmp);
    ASPECT_RATIO_16_9_RESOLUTIONS->Set(2, tmp);

    SUPPORTED_BITRATES = ArrayOf<Int32>::Alloc(13);
    SUPPORTED_BITRATES->Set(0, IMediaProperties::BITRATE_28K);
    SUPPORTED_BITRATES->Set(1, IMediaProperties::BITRATE_40K);
    SUPPORTED_BITRATES->Set(2, IMediaProperties::BITRATE_64K);
    SUPPORTED_BITRATES->Set(3, IMediaProperties::BITRATE_96K);
    SUPPORTED_BITRATES->Set(4, IMediaProperties::BITRATE_128K);
    SUPPORTED_BITRATES->Set(5, IMediaProperties::BITRATE_192K);
    SUPPORTED_BITRATES->Set(6, IMediaProperties::BITRATE_256K);
    SUPPORTED_BITRATES->Set(7, IMediaProperties::BITRATE_384K);
    SUPPORTED_BITRATES->Set(8, IMediaProperties::BITRATE_512K);
    SUPPORTED_BITRATES->Set(9, IMediaProperties::BITRATE_800K);
    SUPPORTED_BITRATES->Set(10, IMediaProperties::BITRATE_2M);
    SUPPORTED_BITRATES->Set(11, IMediaProperties::BITRATE_5M);
    SUPPORTED_BITRATES->Set(12, IMediaProperties::BITRATE_8M);

    SUPPORTED_VCODECS = ArrayOf<Int32>::Alloc(3);
    SUPPORTED_VCODECS->Set(0, IMediaProperties::VCODEC_H264);
    SUPPORTED_VCODECS->Set(1, IMediaProperties::VCODEC_H263);
    SUPPORTED_VCODECS->Set(2, IMediaProperties::VCODEC_MPEG4);

    SUPPORTED_ACODECS = ArrayOf<Int32>::Alloc(3);
    SUPPORTED_ACODECS->Set(0, IMediaProperties::ACODEC_AAC_LC);
    SUPPORTED_ACODECS->Set(1, IMediaProperties::ACODEC_AMRNB);
    SUPPORTED_ACODECS->Set(2, IMediaProperties::ACODEC_AMRWB);

    SUPPORTED_VIDEO_FILE_FORMATS = ArrayOf<Int32>::Alloc(3);
    SUPPORTED_VIDEO_FILE_FORMATS->Set(0, IMediaProperties::FILE_3GP);
    SUPPORTED_VIDEO_FILE_FORMATS->Set(1, IMediaProperties::FILE_MP4);
    SUPPORTED_VIDEO_FILE_FORMATS->Set(2, IMediaProperties::FILE_M4V);
}

ECode CMediaPropertiesHelper::GetAllSupportedAspectRatios(
    /* [out] */ ArrayOf<Int32>** result)
{
    VALIDATE_NOT_NULL(result);

    *result = ASPECT_RATIOS;
    return NOERROR;
}

ECode CMediaPropertiesHelper::GetSupportedResolutions(
    /* [in] */ Int32 aspectRatio,
    /* [out] */ ArrayOf<IPair*>** result)
{
    VALIDATE_NOT_NULL(result);

    AutoPtr<ArrayOf<IPair*> > resolutions;
    switch (aspectRatio) {
        case IMediaProperties::ASPECT_RATIO_3_2: {
            resolutions = ASPECT_RATIO_3_2_RESOLUTIONS;
            break;
        }

        case IMediaProperties::ASPECT_RATIO_4_3: {
            resolutions = ASPECT_RATIO_4_3_RESOLUTIONS;
            break;
        }

        case IMediaProperties::ASPECT_RATIO_5_3: {
            resolutions = ASPECT_RATIO_5_3_RESOLUTIONS;
            break;
        }

        case IMediaProperties::ASPECT_RATIO_11_9: {
            resolutions = ASPECT_RATIO_11_9_RESOLUTIONS;
            break;
        }

        case IMediaProperties::ASPECT_RATIO_16_9: {
            resolutions = ASPECT_RATIO_16_9_RESOLUTIONS;
            break;
        }

        default: {
            // throw new IllegalArgumentException("Unknown aspect ratio: " + aspectRatio);
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
        }
    }

    /** Check the platform specific maximum export resolution */
    AutoPtr<IVideoEditorProfileHelper> helper;
    CVideoEditorProfileHelper::AcquireSingleton((IVideoEditorProfileHelper**)&helper);
    AutoPtr<IVideoEditorProfile> veProfile;
    helper->Get((IVideoEditorProfile**)&veProfile);
    if (veProfile == NULL) {
        // throw new RuntimeException("Can't get the video editor profile");
        return E_RUNTIME_EXCEPTION;
    }
    Int32 maxWidth = ((CVideoEditorProfile*)veProfile.Get())->maxOutputVideoFrameWidth;
    Int32 maxHeight = ((CVideoEditorProfile*)veProfile.Get())->maxOutputVideoFrameHeight;
    AutoPtr<ArrayOf<IPair*> > tmpResolutions = ArrayOf<IPair*>::Alloc(resolutions->GetLength());
    Int32 numSupportedResolution = 0;
    Int32 i = 0;

    /** Get supported resolution list */
    for (i = 0; i < resolutions->GetLength(); i++) {
        AutoPtr<IInteger32> a, b;
        (*resolutions)[i]->GetFirst((IInterface**)&a);
        (*resolutions)[i]->GetSecond((IInterface**)&b);
        Int32 ia, ib;
        a->GetValue(&ia);
        b->GetValue(&ib);
        if ((ia <= maxWidth) && (ib <= maxHeight)) {
            tmpResolutions->Set(numSupportedResolution, (*resolutions)[i]);
            numSupportedResolution++;
        }
    }

    AutoPtr<ArrayOf<IPair*> > supportedResolutions = ArrayOf<IPair*>::Alloc(numSupportedResolution);
    // System.arraycopy(tmpResolutions, 0, supportedResolutions, 0, numSupportedResolution);
    for(Int32 i = 0; i < numSupportedResolution; i++) {
        supportedResolutions->Set(i, (*tmpResolutions)[i]);
    }

    *result = supportedResolutions;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

ECode CMediaPropertiesHelper::GetSupportedVideoCodecs(
    /* [out] */ ArrayOf<Int32>** result)
{
    VALIDATE_NOT_NULL(result);

    *result = SUPPORTED_VCODECS;
    return NOERROR;
}

ECode CMediaPropertiesHelper::GetSupportedAudioCodecs(
    /* [out] */ ArrayOf<Int32>** result)
{
    VALIDATE_NOT_NULL(result);

    *result = SUPPORTED_ACODECS;
    return NOERROR ;
}

ECode CMediaPropertiesHelper::GetSupportedVideoFileFormat(
    /* [out] */ ArrayOf<Int32>** result)
{
    VALIDATE_NOT_NULL(result);

    *result = SUPPORTED_VIDEO_FILE_FORMATS;
    return NOERROR ;
}

ECode CMediaPropertiesHelper::GetSupportedVideoBitrates(
    /* [out] */ ArrayOf<Int32>** result)
{
    VALIDATE_NOT_NULL(result);

    *result = SUPPORTED_BITRATES;
    return NOERROR ;
}

ECode CMediaPropertiesHelper::GetSupportedMaxVolume(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    *result = IMediaProperties::AUDIO_MAX_VOLUME_PERCENT;
    return NOERROR ;
}

ECode CMediaPropertiesHelper::GetSupportedAudioTrackCount(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    *result = IMediaProperties::AUDIO_MAX_TRACK_COUNT;
    return NOERROR ;
}

} // namespace videoeditor
} // namespace Media
} // namepsace Droid
} // namespace Elastos
