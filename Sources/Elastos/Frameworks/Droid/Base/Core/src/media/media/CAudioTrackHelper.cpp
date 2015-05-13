
#include "media/CAudioTrackHelper.h"
#include "media/CAudioTrack.h"
//#include "media/ElAudioSystem.h"
#include <media/AudioTrack.h>
#include <elastos/Logger.h>
#include <elastos/Math.h>

using Elastos::Utility::Logging::Logger;

namespace Elastos {
namespace Droid {
namespace Media {

ECode CAudioTrackHelper::GetMinVolume(
    /* [out] */ Float* minVolume)
{
    return CAudioTrack::GetMinVolume(minVolume);
}
ECode CAudioTrackHelper::GetMaxVolume(
    /* [out] */ Float* maxVolume)
{
    return CAudioTrack::GetMaxVolume(maxVolume);
}
ECode CAudioTrackHelper::GetNativeOutputSampleRate(
    /* [in] */ Int32 streamType,
    /* [out] */ Int32* rate)
{
    VALIDATE_NOT_NULL(rate);
    *rate = CAudioTrack::NativeGetOutputSampleRate(streamType);
    return NOERROR;
}
ECode CAudioTrackHelper::GetMinBufferSize(
    /* [in] */ Int32 sampleRateInHz,
    /* [in] */ Int32 channelConfig,
    /* [in] */ Int32 audioFormat,
    /* [out] */ Int32* size)
{
    VALIDATE_NOT_NULL(size);

    Int32 channelCount = 0;
    switch(channelConfig) {
    case IAudioFormat::CHANNEL_OUT_MONO:
    case IAudioFormat::CHANNEL_CONFIGURATION_MONO:
        channelCount = 1;
        break;
    case IAudioFormat::CHANNEL_OUT_STEREO:
    case IAudioFormat::CHANNEL_CONFIGURATION_STEREO:
        channelCount = 2;
        break;
    default:

        if ((channelConfig & CAudioTrack::SUPPORTED_OUT_CHANNELS) != channelConfig) {
            // input channel configuration features unsupported channels
            Logger::E("AudioTrack", "getMinBufferSize(): Invalid channel configuration.");
            *size = IAudioTrack::ERROR_BAD_VALUE;
            return NOERROR;
        }
        else {
            channelCount = Elastos::Core::Math::BitCount(channelConfig);
        }
    }

    if ((audioFormat != IAudioFormat::ENCODING_PCM_16BIT)
            && (audioFormat != IAudioFormat::ENCODING_PCM_8BIT)) {
        Logger::E("AudioTrack", "getMinBufferSize(): Invalid audio format.");
        *size = IAudioTrack::ERROR_BAD_VALUE;
        return NOERROR;
    }

    if ( (sampleRateInHz < 4000) || (sampleRateInHz > 48000) ) {
        Logger::E("AudioTrack", "getMinBufferSize(): %d Hz is not a supported sample rate.", sampleRateInHz);
        *size = IAudioTrack::ERROR_BAD_VALUE;
        return NOERROR;
    }

    Int32 BufferSize = CAudioTrack::NativeGetMinBuffSize(sampleRateInHz, channelCount, audioFormat);
    if ((BufferSize == -1) || (BufferSize == 0)) {
        Logger::E("AudioTrack", "getMinBufferSize(): error querying hardware.");
        *size = IAudioTrack::ERROR;
        return NOERROR;
    }
    else {
        *size = BufferSize;
    }
    return NOERROR;
}

} // namespace Media
} // namepsace Droid
} // namespace Elastos
