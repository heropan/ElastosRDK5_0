
#ifndef __ELASTOS_DROID_MEDIA_AUDIOFX_CVISUALIZERHELPER_H__
#define __ELASTOS_DROID_MEDIA_AUDIOFX_CVISUALIZERHELPER_H__

#include "_CVisualizerHelper.h"
#include "ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Media {
namespace Audiofx {

CarClass(CVisualizerHelper)
{
public:
    /**
     * Returns the capture size range.
     * @return the mininum capture size is returned in first array element and the maximum in second
     * array element.
     */
    GetCaptureSizeRange(
        /* [out, callee] */ ArrayOf<Int32>** captureSizeRange);

    /**
     * Returns the maximum capture rate for the callback capture method. This is the maximum value
     * for the rate parameter of the
     * {@link #setDataCaptureListener(OnDataCaptureListener, int, boolean, boolean)} method.
     * @return the maximum capture rate expressed in milliHertz
     */
    GetMaxCaptureRate(
        /* [out] */ Int32* captureRate);
};

} // namespace Audiofx
} // namespace Media
} // namepsace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_MEDIA_AUDIOFX_CVISUALIZERHELPER_H__
