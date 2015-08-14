
#ifndef __ELASTOS_DROID_MEDIA_VIDEOEDITOR_CVIDEOEDITORPROFILE_H__
#define __ELASTOS_DROID_MEDIA_VIDEOEDITOR_CVIDEOEDITORPROFILE_H__

#include "_CVideoEditorProfile.h"
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
CarClass(CVideoEditorProfile)
{
public:
    // Private constructor called by JNI
    CVideoEditorProfile();

    constructor(
        /* [in] */ Int32 inputWidth,
        /* [in] */ Int32 inputHeight,
        /* [in] */ Int32 outputWidth,
        /* [in] */ Int32 outputHeight);

    /**
     * Returns the videoeditor profile
     */
    static CARAPI Get(
        /* [out] */ IVideoEditorProfile** result);

    /**
     * Returns the supported profile by given video codec
     */
    static CARAPI GetExportProfile(
        /* [in] */ Int32 vidCodec,
        /* [out] */ Int32* result);

    /**
     * Returns the supported level by given video codec
     */
    static CARAPI GetExportLevel(
        /* [in] */ Int32 vidCodec,
        /* [out] */ Int32* result);

private:
    // Methods implemented by JNI
    static CARAPI_(void) NativeInit();

    static CARAPI_(AutoPtr<IVideoEditorProfile>) NativeGetVideoeditorProfile();

    static CARAPI_(Int32) NativeGetVideoeditorExportProfile(
        /* [in] */ Int32 codec);

    static CARAPI_(Int32) NativeGetVideoeditorExportLevel(
        /* [in] */ Int32 level);

public:
    /**
     * The max input video frame width
     */
    Int32 maxInputVideoFrameWidth;

    /**
     * The max input video frame height
     */
    Int32 maxInputVideoFrameHeight;

    /**
     * The max ouput video frame width
     */
    Int32 maxOutputVideoFrameWidth;

    /**
     * The max ouput video frame height
     */
    Int32 maxOutputVideoFrameHeight;

};

} // namespace Videoeditor
} // namespace Media
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_MEDIA_VIDEOEDITOR_CVIDEOEDITORPROFILE_H__

