#include "elastos/droid/media/CMediaPlayerTrackInfo.h"
#include "elastos/droid/ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Media {

CMediaPlayerTrackInfo::CMediaPlayerTrackInfo()
    : mTrackType(IMediaPlayerTrackInfo::MEDIA_TRACK_TYPE_UNKNOWN)
{}

ECode CMediaPlayerTrackInfo::constructor()
{
    return NOERROR;
}

ECode CMediaPlayerTrackInfo::GetTrackType(
    /* [out] */ Int32* type)
{
    VALIDATE_NOT_NULL(type);
    *type = mTrackType;
    return NOERROR;
}

ECode CMediaPlayerTrackInfo::SetTrackType(
    /* [in] */ Int32 type)
{
    mTrackType = type;
    return NOERROR;
}

ECode CMediaPlayerTrackInfo::GetLanguage(
    /* [out] */ String* language)
{
    VALIDATE_NOT_NULL(language);
    *language = mLanguage;
    return NOERROR;
}

ECode CMediaPlayerTrackInfo::SetLanguage(
    /* [in] */ const String& language)
{
    mLanguage = language;
    return NOERROR;
}

ECode CMediaPlayerTrackInfo::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    VALIDATE_NOT_NULL(dest);
    FAIL_RETURN(dest->WriteInt32(mTrackType));
    return dest->WriteString(mLanguage);
}

ECode CMediaPlayerTrackInfo::ReadFromParcel(
    /* [in] */ IParcel* src)
{
    VALIDATE_NOT_NULL(src);
    FAIL_RETURN(src->ReadInt32(&mTrackType));
    return src->ReadString(&mLanguage);
}

} // namespace media
} // namepsace Droid
} // namespace Elastos
