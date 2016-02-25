
#ifndef ___AUDIOFORMAT_H__
#define ___AUDIOFORMAT_H__

#include "_Elastos.Droid.Media.h"
#include <system/audio.h>

namespace Elastos {
namespace Droid {
namespace Media {

static inline audio_format_t AudioFormatToNative(int audioFormat)
{
    switch (audioFormat) {
    case IAudioFormat::ENCODING_PCM_16BIT:
        return AUDIO_FORMAT_PCM_16_BIT;
    case IAudioFormat::ENCODING_PCM_8BIT:
        return AUDIO_FORMAT_PCM_8_BIT;
    case IAudioFormat::ENCODING_PCM_FLOAT:
        return AUDIO_FORMAT_PCM_FLOAT;
    case IAudioFormat::ENCODING_AC3:
        return AUDIO_FORMAT_AC3;
    case IAudioFormat::ENCODING_E_AC3:
        return AUDIO_FORMAT_E_AC3;
    case IAudioFormat::ENCODING_DEFAULT:
        return AUDIO_FORMAT_DEFAULT;
    default:
        return AUDIO_FORMAT_INVALID;
    }
}

static inline Int32 AudioFormatFromNative(audio_format_t nativeFormat)
{
    switch (nativeFormat) {
    case AUDIO_FORMAT_PCM_16_BIT:
        return IAudioFormat::ENCODING_PCM_16BIT;
    case AUDIO_FORMAT_PCM_8_BIT:
        return IAudioFormat::ENCODING_PCM_8BIT;
    case AUDIO_FORMAT_PCM_FLOAT:
        return IAudioFormat::ENCODING_PCM_FLOAT;
    case AUDIO_FORMAT_AC3:
        return IAudioFormat::ENCODING_AC3;
    case AUDIO_FORMAT_E_AC3:
        return IAudioFormat::ENCODING_E_AC3;
    case AUDIO_FORMAT_DEFAULT:
        return IAudioFormat::ENCODING_DEFAULT;
    default:
        return IAudioFormat::ENCODING_INVALID;
    }
}

static inline audio_channel_mask_t OutChannelMaskToNative(Int32 channelMask)
{
    switch (channelMask) {
    case IAudioFormat::CHANNEL_OUT_DEFAULT:
    case IAudioFormat::CHANNEL_INVALID:
        return AUDIO_CHANNEL_NONE;
    default:
        return (audio_channel_mask_t)(channelMask>>2);
    }
}

static inline Int32 OutChannelMaskFromNative(audio_channel_mask_t nativeMask)
{
    switch (nativeMask) {
    case AUDIO_CHANNEL_NONE:
        return IAudioFormat::CHANNEL_OUT_DEFAULT;
    default:
        return (Int32)nativeMask << 2;
    }
}

static inline audio_channel_mask_t InChannelMaskToNative(Int32 channelMask)
{
    return (audio_channel_mask_t)channelMask;
}

static inline Int32 InChannelMaskFromNative(audio_channel_mask_t nativeMask)
{
    return (Int32)nativeMask;
}

} // namespace Media
} // namespace Droid
} // namespace Elastos

#endif // ___AUDIOFORMAT_H__
