#include "elastos/droid/media/session/CMediaControllerPlaybackInfo.h"

namespace Elastos {
namespace Droid {
namespace Media {
namespace Session {

CAR_INTERFACE_IMPL(CMediaControllerPlaybackInfo, Object, IMediaControllerPlaybackInfo)

CAR_OBJECT_IMPL(CMediaControllerPlaybackInfo)

CMediaControllerPlaybackInfo::CMediaControllerPlaybackInfo()
    : mVolumeType(0)
    , mVolumeControl(0)
    , mMaxVolume(0)
    , mCurrentVolume(0)
    , mAudioAttrs(0)
{
}

CMediaControllerPlaybackInfo::~CMediaControllerPlaybackInfo()
{
}

ECode CMediaControllerPlaybackInfo::constructor(
    /* [in] */ Int32 type,
    /* [in] */ IAudioAttributes * attrs,
    /* [in] */ Int32 control,
    /* [in] */ Int32 max,
    /* [in] */ Int32 current)
{
    mVolumeType = type;
    mAudioAttrs = attrs;
    mVolumeControl = control;
    mMaxVolume = max;
    mCurrentVolume = current;
    return NOERROR;
}

ECode CMediaControllerPlaybackInfo::GetPlaybackType(
    /* [out] */ Int32 * result)
{
    VALIDATE_NOT_NULL(result)
    *result = mVolumeType;
    return NOERROR;
}

ECode CMediaControllerPlaybackInfo::GetAudioAttributes(
    /* [out] */ IAudioAttributes ** result)
{
    VALIDATE_NOT_NULL(result)
    *result = mAudioAttrs;
    REFCOUNT_ADD(*result)
    return NOERROR;
}

ECode CMediaControllerPlaybackInfo::GetVolumeControl(
    /* [out] */ Int32 * result)
{
    VALIDATE_NOT_NULL(result)
    *result = mVolumeControl;
    return NOERROR;
}

ECode CMediaControllerPlaybackInfo::GetMaxVolume(
    /* [out] */ Int32 * result)
{
    VALIDATE_NOT_NULL(result)
    *result = mMaxVolume;
    return NOERROR;
}

ECode CMediaControllerPlaybackInfo::GetCurrentVolume(
    /* [out] */ Int32 * result)
{
    VALIDATE_NOT_NULL(result)
    *result = mCurrentVolume;
    return NOERROR;
}

} // namespace Session
} // namespace Media
} // namepsace Droid
} // namespace Elastos
