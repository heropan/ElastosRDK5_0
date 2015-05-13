
#include "media/CDecoderCapabilitiesHelper.h"
#include "media/CDecoderCapabilities.h"

namespace Elastos {
namespace Droid {
namespace Media {

ECode CDecoderCapabilitiesHelper::GetVideoDecoders(
   /* [out, callee] */ ArrayOf<VideoDecoder>** result)
{
    return CDecoderCapabilities::GetVideoDecoders(result);
}

ECode CDecoderCapabilitiesHelper::GetAudioDecoders(
    /* [out, callee] */ ArrayOf<AudioDecoder>** result)
{
    return CDecoderCapabilities::GetAudioDecoders(result);
}

} // namespace Media
} // namepsace Droid
} // namespace Elastos
