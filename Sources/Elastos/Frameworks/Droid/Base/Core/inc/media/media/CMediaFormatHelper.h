
#ifndef __CMEDIAFORMATHELPER_H__
#define __CMEDIAFORMATHELPER_H__

#include "_CMediaFormatHelper.h"

namespace Elastos {
namespace Droid {
namespace Media {

CarClass(CMediaFormatHelper)
{
public:
    CARAPI CreateAudioFormat(
        /* [in] */ const String& mime,
        /* [in] */ Int32 sampleRate,
        /* [in] */ Int32 channelCount,
        /* [out] */ IMediaFormat** result);

    /**
     * Creates a minimal video format.
     * @param mime The mime type of the content.
     * @param width The width of the content (in pixels)
     * @param height The height of the content (in pixels)
     */
    CARAPI CreateVideoFormat(
        /* [in] */ const String& mime,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [out] */ IMediaFormat** result);
};

} // namespace Media
} // namepsace Droid
} // namespace Elastos

#endif // __CMEDIAFORMATHELPER_H__
