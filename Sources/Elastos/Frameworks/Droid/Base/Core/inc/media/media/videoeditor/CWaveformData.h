
#ifndef __CWAVEFORMDATA_H__
#define __CWAVEFORMDATA_H__

#include "_CWaveformData.h"
#include <ext/frameworkext.h>

namespace Elastos {
namespace Droid {
namespace Media {
namespace Videoeditor {

/**
 * Class which describes the waveform data of an audio track. The gain values
 * represent the average gain for an audio frame. For audio codecs which do not
 * operate on a per frame bases (eg. ALAW, ULAW) a reasonable audio frame
 * duration will be assumed (eg. 50ms).
 * {@hide}
 */
CarClass(CWaveformData)
{
public:
    CWaveformData();

    /*
     * Constructor
     *
     * @param audioWaveformFilename The name of the audio waveform file
     *
     * The file format is as following:
     * <ul>
     *  <li>first 4 bytes provide the number of samples for each value, as
     *  big-endian signed</li>
     *  <li>4 following bytes is the total number of values in the file, as
     *  big-endian signed</li>
     *  <li>then, all values follow as bytes</li>
     * </ul>
     *
     * @throws IOException on failure of file input stream operations
     * @throws IllegalArgumentException if audioWaveformFilename is null
     */
    CARAPI constructor(
        /* [in] */ const String& audioWaveformFilename);

    /**
     * @return The duration of a frame in milliseconds
     */
    CARAPI GetFrameDuration(
        /* [out] */ Int32* result);

    /**
     * @return The number of frames within the waveform data
     */
    CARAPI GetFramesCount(
        /* [out] */ Int32* result);

    /**
     * @return The array of frame gains. The size of the array is the frames
     *         count. The values of the frame gains range from 0 to 255.
     */
    CARAPI GetFrameGains(
        /* [out] */ ArrayOf<Int16>** result);

private:
    /*
     *  Instance variables
     */
    Int32 mFrameDurationMs;
    Int32 mFramesCount;
    AutoPtr<ArrayOf<Int16> > mGains;
};


} // namespace Videoeditor
} // namespace Media
} // namepsace Droid
} // namespace Elastos
#endif // __CWAVEFORMDATA_H__

