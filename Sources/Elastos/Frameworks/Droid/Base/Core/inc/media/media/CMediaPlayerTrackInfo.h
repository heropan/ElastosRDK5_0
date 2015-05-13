#ifndef __CMEDIAPLAYERTRACKINFO_H__
#define __CMEDIAPLAYERTRACKINFO_H__

#include "_CMediaPlayerTrackInfo.h"

namespace Elastos {
namespace Droid {
namespace Media {

/**
 * Class for MediaPlayer to return each audio/video/subtitle track's metadata.
 *
 * @see android.media.MediaPlayer#getTrackInfo
 */
CarClass(CMediaPlayerTrackInfo)
{
public:
    CMediaPlayerTrackInfo();

    CARAPI constructor();

    /**
     * Gets the track type.
     * @return TrackType which indicates if the track is video, audio, timed text.
     */
    CARAPI GetTrackType(
        /* [out] */ Int32* type);

    CARAPI SetTrackType(
        /* [in] */ Int32 type);

    /**
     * Gets the language code of the track.
     * @return a language code in either way of ISO-639-1 or ISO-639-2.
     * When the language is unknown or could not be determined,
     * ISO-639-2 language code, "und", is returned.
     */
    CARAPI GetLanguage(
        /* [out] */ String* language);

    CARAPI SetLanguage(
        /* [in] */ const String& language);

    /**
     * {@inheritDoc}
     */
    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    /**
     * {@inheritDoc}
     */
    CARAPI ReadFromParcel(
        /* [in] */ IParcel* src);
public:
    Int32 mTrackType;
    String mLanguage;
};

} // namespace media
} // namepsace Droid
} // namespace Elastos

#endif // __CMEDIAPLAYERTRACKINFO_H__
