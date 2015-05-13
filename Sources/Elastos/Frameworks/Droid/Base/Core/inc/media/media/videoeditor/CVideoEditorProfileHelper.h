
#ifndef __CVIDEOEDITORPROFILEHELPER_H__
#define __CVIDEOEDITORPROFILEHELPER_H__

#include "_CVideoEditorProfileHelper.h"
#include <ext/frameworkext.h>

namespace Elastos {
namespace Droid {
namespace Media {
namespace Videoeditor {

/**
 * The VideoEditorProfile class is used to retrieve the
 * predefined videoeditor profile settings for videoeditor applications.
 * These settings are read-only.
 *
 * <p>The videoeditor profile specifies the following set of parameters:
 * <ul>
 * <li> max input video frame width
 * <li> max input video frame height
 * <li> max output video frame width
 * <li> max output video frame height
 * </ul>
 * {@hide}
 */
CarClass(CVideoEditorProfileHelper)
{
public:
    /**
     * Returns the videoeditor profile
     */
    CARAPI Get(
        /* [out] */ IVideoEditorProfile** result);

    /**
     * Returns the supported profile by given video codec
     */
    CARAPI GetExportProfile(
        /* [in] */ Int32 vidCodec,
        /* [out] */ Int32* result);

    /**
     * Returns the supported level by given video codec
     */
    CARAPI GetExportLevel(
        /* [in] */ Int32 vidCodec,
        /* [out] */ Int32* result);

};

} // namespace Videoeditor
} // namespace Media
} // namepsace Droid
} // namespace Elastos

#endif // __CVIDEOEDITORPROFILEHELPER_H__

