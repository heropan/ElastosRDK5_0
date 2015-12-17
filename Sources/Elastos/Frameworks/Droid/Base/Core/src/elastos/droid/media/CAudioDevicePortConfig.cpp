#include "elastos/droid/media/CAudioDevicePortConfig.h"

namespace Elastos {
namespace Droid {
namespace Media {

CAR_INTERFACE_IMPL(CAudioDevicePortConfig, AudioPortConfig, IAudioDevicePortConfig)

CAR_OBJECT_IMPL(CAudioDevicePortConfig)

CAudioDevicePortConfig::CAudioDevicePortConfig()
{
}

CAudioDevicePortConfig::~CAudioDevicePortConfig()
{
}

ECode CAudioDevicePortConfig::constructor(
    /* [in] */ IAudioDevicePort* devicePort,
    /* [in] */ Int32 samplingRate,
    /* [in] */ Int32 channelMask,
    /* [in] */ Int32 format,
    /* [in] */ IAudioGainConfig* gain)
{
    return AudioPortConfig::constructor(IAudioPort::Probe(devicePort),
        samplingRate, channelMask, format, gain);
}

ECode CAudioDevicePortConfig::constructor(
    /* [in] */ IAudioDevicePortConfig* config)
{
    AutoPtr<IAudioDevicePort> port;
    config->Port((IAudioDevicePort**)&port);
    Int32 samplingRate, channelMask, format;
    IAudioPortConfig::Probe(config)->SamplingRate(&samplingRate);
    IAudioPortConfig::Probe(config)->ChannelMask(&channelMask);
    IAudioPortConfig::Probe(config)->Format(&format);
    AutoPtr<IAudioGainConfig> gain;
    IAudioPortConfig::Probe(config)->Gain((IAudioGainConfig**)&gain);
    return constructor(port, samplingRate, channelMask, format, gain);
}

ECode CAudioDevicePortConfig::Port(
    /* [out] */ IAudioDevicePort** result)
{
    VALIDATE_NOT_NULL(result)
    *result = IAudioDevicePort::Probe(mPort);
    REFCOUNT_ADD(*result)
    return NOERROR;
}

} // namespace Media
} // namepsace Droid
} // namespace Elastos
