
#ifndef __CAUDIOTRACKHELPER_H__
#define __CAUDIOTRACKHELPER_H__

#include "_CAudioTrackHelper.h"

namespace Elastos {
namespace Droid {
namespace Media {

CarClass(CAudioTrackHelper)
{
public:
    CARAPI GetMinVolume(
        /* [out] */ Float* minVolume);

    CARAPI GetMaxVolume(
        /* [out] */ Float* maxVolume);

    CARAPI GetNativeOutputSampleRate(
        /* [in] */ Int32 streamType,
        /* [out] */ Int32* rate);

    CARAPI GetMinBufferSize(
        /* [in] */ Int32 sampleRateInHz,
        /* [in] */ Int32 channelConfig,
        /* [in] */ Int32 audioFormat,
        /* [out] */ Int32* size);

};
} // namespace Media
} // namepsace Droid
} // namespace Elastos
#endif //__CAUDIOTRACKHELPER_H__
