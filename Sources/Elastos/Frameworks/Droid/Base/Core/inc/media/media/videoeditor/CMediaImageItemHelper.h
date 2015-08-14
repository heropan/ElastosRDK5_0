
#ifndef __ELASTOS_DROID_MEDIA_VIDEOEDITOR_CMEDIAIMAGEITEMHELPER_H__
#define __ELASTOS_DROID_MEDIA_VIDEOEDITOR_CMEDIAIMAGEITEMHELPER_H__

#include "_CMediaImageItemHelper.h"
#include <ext/frameworkext.h>
#include "media/media/videoeditor/CMediaImageItem.h"

namespace Elastos {
namespace Droid {
namespace Media {
namespace Videoeditor {
/**
 * This is the super class for all effects. An effect can only be applied to a
 * single media item.
 * {@hide}
 */
CarClass(CMediaImageItemHelper)
{
public:

    CARAPI NextPowerOf2(
        /* [in] */ Int32 n,
        /* [out] */ Int32* result);

};

} // namespace Videoeditor
} // namespace Media
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_MEDIA_VIDEOEDITOR_CMEDIAIMAGEITEMHELPER_H__

