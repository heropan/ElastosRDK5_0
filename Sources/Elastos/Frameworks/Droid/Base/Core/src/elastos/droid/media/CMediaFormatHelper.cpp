
#include "elastos/droid/media/CMediaFormatHelper.h"
#include "elastos/droid/media/CMediaFormat.h"

namespace Elastos {
namespace Droid {
namespace Media {

ECode CMediaFormatHelper::CreateAudioFormat(
    /* [in] */ const String& mime,
    /* [in] */ Int32 sampleRate,
    /* [in] */ Int32 channelCount,
    /* [out] */ IMediaFormat** result)
{
    return CMediaFormat::CreateAudioFormat(mime, sampleRate, channelCount, result);
}

ECode CMediaFormatHelper::CreateVideoFormat(
    /* [in] */ const String& mime,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [out] */ IMediaFormat** result)
{
    return CMediaFormat::CreateAudioFormat(mime, width, height, result);
}


} // namespace Media
} // namepsace Droid
} // namespace Elastos
