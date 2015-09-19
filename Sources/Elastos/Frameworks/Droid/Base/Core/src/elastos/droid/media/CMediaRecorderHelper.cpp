
#include "media/CMediaRecorderHelper.h"
#include "media/CMediaRecorder.h"

namespace Elastos {
namespace Droid {
namespace Media {

ECode CMediaRecorderHelper::GetAudioSourceMax(
    /* [out] */ Int32* result)
{
    return CMediaRecorder::GetAudioSourceMax(result);
}

} // namespace Media
} // namespace Droid
} // namespace Elastos
