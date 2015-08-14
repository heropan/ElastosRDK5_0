
#ifndef __ELASTOS_DROID_MEDIA_VIDEOEDITOR_CMEDIAPROPERTIESHELPER_H__
#define __ELASTOS_DROID_MEDIA_VIDEOEDITOR_CMEDIAPROPERTIESHELPER_H__

#include "_CMediaPropertiesHelper.h"
#include <ext/frameworkext.h>

using Elastos::Droid::Utility::IPair;

namespace Elastos {
namespace Droid {
namespace Media {
namespace Videoeditor {

/**
 * This is the super class for all effects. An effect can only be applied to a
 * single media item.
 * {@hide}
 */
CarClass(CMediaPropertiesHelper)
{
public:
    CMediaPropertiesHelper();

    constructor();

    CARAPI GetAllSupportedAspectRatios(
        /* [out] */ ArrayOf<Int32>** result);

    /**
     * Get the supported resolutions for the specified aspect ratio.
     *
     * @param aspectRatio The aspect ratio for which the resolutions are
     *        requested
     * @return The array of width and height pairs
     */
    //static
    CARAPI GetSupportedResolutions(
        /* [in] */ Int32 aspectRatio,
        /* [out] */ ArrayOf<IPair*>** result);

    /**
     * @return The array of supported video codecs
     */
    CARAPI GetSupportedVideoCodecs(
        /* [out] */ ArrayOf<Int32>** result);

    /**
     * @return The array of supported audio codecs
     */
    CARAPI GetSupportedAudioCodecs(
        /* [out] */ ArrayOf<Int32>** result);

    /**
     * @return The array of supported file formats
     */
    CARAPI GetSupportedVideoFileFormat(
        /* [out] */ ArrayOf<Int32>** result);

    /**
     * @return The array of supported video bitrates
     */
    CARAPI GetSupportedVideoBitrates(
        /* [out] */ ArrayOf<Int32>** result);

    /**
     * @return The maximum value for the audio volume
     */
    CARAPI GetSupportedMaxVolume(
        /* [out] */ Int32* result);

    /**
     * @return The maximum number of audio tracks supported
     */
    CARAPI GetSupportedAudioTrackCount(
        /* [out] */ Int32* result);

private:
    static ArrayOf<Int32>* ASPECT_RATIOS;

    // @SuppressWarnings({"unchecked"})
    static ArrayOf<IPair*>* ASPECT_RATIO_3_2_RESOLUTIONS;

    // @SuppressWarnings({"unchecked"})
    static ArrayOf<IPair*>* ASPECT_RATIO_4_3_RESOLUTIONS;

    // @SuppressWarnings({"unchecked"})
    static ArrayOf<IPair*>* ASPECT_RATIO_5_3_RESOLUTIONS;

    // @SuppressWarnings({"unchecked"})
    static ArrayOf<IPair*>* ASPECT_RATIO_11_9_RESOLUTIONS;

    // @SuppressWarnings({"unchecked"})
    static ArrayOf<IPair*>* ASPECT_RATIO_16_9_RESOLUTIONS;

    /**
     *  The array of supported bitrates
     */
    static ArrayOf<Int32>* SUPPORTED_BITRATES;

    /**
     *  The array of supported video codecs
     */
    static ArrayOf<Int32>* SUPPORTED_VCODECS;

    /**
     *  The array of supported audio codecs
     */
    static ArrayOf<Int32>* SUPPORTED_ACODECS ;

    /**
     * The array of the supported file formats
     */
    static ArrayOf<Int32>* SUPPORTED_VIDEO_FILE_FORMATS;

};

} // namespace videoeditor
} // namespace Media
} // namepsace Droid
} // namespace Elastos
#endif // __ELASTOS_DROID_MEDIA_VIDEOEDITOR_CMEDIAPROPERTIESHELPER_H__

