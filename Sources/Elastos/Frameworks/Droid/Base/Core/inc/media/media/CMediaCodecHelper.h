
#ifndef __CMEDIACODECHELPER_H__
#define __CMEDIACODECHELPER_H__

#include "_CMediaCodecHelper.h"
#include <ext/frameworkext.h>

using Elastos::Droid::Media::IMediaCodec;

namespace Elastos {
namespace Droid {
namespace Media {

CarClass(CMediaCodecHelper)
{
public:
    CARAPI CreateDecoderByType(
        /* [in] */ const String& type,
        /* [out] */ IMediaCodec** result);

    CARAPI CreateEncoderByType(
        /* [in] */ const String& type,
        /* [out] */ IMediaCodec** result);

    CARAPI CreateByCodecName(
        /* [in] */ const String& name,
        /* [out] */ IMediaCodec** result);
};

} // namespace Media
} // namepsace Droid
} // namespace Elastos

#endif // __CMEDIACODECHELPER_H__
