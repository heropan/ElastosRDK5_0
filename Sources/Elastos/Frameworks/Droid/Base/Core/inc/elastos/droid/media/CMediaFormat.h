
#ifndef __ELASTOS_DROID_MEDIA_CMEDIAFORMAT_H__
#define __ELASTOS_DROID_MEDIA_CMEDIAFORMAT_H__

#include "_Elastos_Droid_Media_CMediaFormat.h"
#include "elastos/droid/ext/frameworkext.h"

using Elastos::IO::IByteBuffer;
using Elastos::Utility::IObjectStringMap;

namespace Elastos {
namespace Droid {
namespace Media {

/**
 * Encapsulates the information describing the format of media data,
 * be it audio or video.
 *
 * The format of the media data is specified as string/value pairs.
 *
 * Keys common to all formats, <b>all keys not marked optional are mandatory</b>:
 *
 * <table>
 * <tr><th>Name</th><th>Value Type</th><th>Description</th></tr>
 * <tr><td>{@link #KEY_MIME}</td><td>String</td><td>The type of the format.</td></tr>
 * <tr><td>{@link #KEY_MAX_INPUT_SIZE}</td><td>Integer</td><td>optional, maximum size of a buffer of input data</td></tr>
 * <tr><td>{@link #KEY_BIT_RATE}</td><td>Integer</td><td><b>encoder-only</b>, desired bitrate in bits/second</td></tr>
 * </table>
 *
 * Video formats have the following keys:
 * <table>
 * <tr><th>Name</th><th>Value Type</th><th>Description</th></tr>
 * <tr><td>{@link #KEY_WIDTH}</td><td>Integer</td><td></td></tr>
 * <tr><td>{@link #KEY_HEIGHT}</td><td>Integer</td><td></td></tr>
 * <tr><td>{@link #KEY_COLOR_FORMAT}</td><td>Integer</td><td><b>encoder-only</b></td></tr>
 * <tr><td>{@link #KEY_FRAME_RATE}</td><td>Integer or Float</td><td><b>encoder-only</b></td></tr>
 * <tr><td>{@link #KEY_I_FRAME_INTERVAL}</td><td>Integer</td><td><b>encoder-only</b></td></tr>
 * </table>
 *
 * Audio formats have the following keys:
 * <table>
 * <tr><th>Name</th><th>Value Type</th><th>Description</th></tr>
 * <tr><td>{@link #KEY_CHANNEL_COUNT}</td><td>Integer</td><td></td></tr>
 * <tr><td>{@link #KEY_SAMPLE_RATE}</td><td>Integer</td><td></td></tr>
 * <tr><td>{@link #KEY_IS_ADTS}</td><td>Integer</td><td>optional, if <em>decoding</em> AAC audio content, setting this key to 1 indicates that each audio frame is prefixed by the ADTS header.</td></tr>
 * <tr><td>{@link #KEY_AAC_PROFILE}</td><td>Integer</td><td><b>encoder-only</b>, optional, if content is AAC audio, specifies the desired profile.</td></tr>
 * <tr><td>{@link #KEY_CHANNEL_MASK}</td><td>Integer</td><td>optional, a mask of audio channel assignments</td></tr>
 * <tr><td>{@link #KEY_FLAC_COMPRESSION_LEVEL}</td><td>Integer</td><td><b>encoder-only</b>, optional, if content is FLAC audio, specifies the desired compression level.</td></tr>
 * </table>
 *
 */
CarClass(CMediaFormat)
{
public:
    CMediaFormat();

    /**
     * Creates an empty MediaFormat
     */
    constructor();

    constructor(
        /* [in] */ IObjectStringMap* map);

    /**
     * Returns true iff a key of the given name exists in the format.
     */
    CARAPI ContainsKey(
        /* [in] */ const String& name,
        /* [out] */ Boolean* result);

    /**
     * Returns the value of an integer key.
     */
    CARAPI GetInt32(
        /* [in] */ const String& name,
        /* [out] */ Int32* result);

    /**
     * Returns the value of a long key.
     */
    CARAPI GetInt64(
        /* [in] */ const String& name,
        /* [out] */ Int64* result);

    /**
     * Returns the value of a float key.
     */
    CARAPI GetFloat(
        /* [in] */ const String& name,
        /* [out] */ Float* result);

    /**
     * Returns the value of a string key.
     */
    CARAPI GetString(
        /* [in] */ const String& name,
        /* [out] */ String* result);

    /**
     * Returns the value of a ByteBuffer key.
     */
    CARAPI GetByteBuffer(
        /* [in] */ const String& name,
        /* [out] */ IByteBuffer** result);

    /**
     * Sets the value of an integer key.
     */
    CARAPI SetInt32(
        /* [in] */ const String& name,
        /* [in] */ Int32 value);

    /**
     * Sets the value of a long key.
     */
    CARAPI SetInt64(
        /* [in] */ const String& name,
        /* [in] */ Int64 value);

    /**
     * Sets the value of a float key.
     */
    CARAPI SetFloat(
        /* [in] */ const String& name,
        /* [in] */ Float value);

    /**
     * Sets the value of a string key.
     */
    CARAPI SetString(
        /* [in] */ const String& name,
        /* [in] */ const String& value);

    /**
     * Sets the value of a ByteBuffer key.
     */
    CARAPI SetByteBuffer(
        /* [in] */ const String& name,
        /* [in] */ IByteBuffer* bytes);

    CARAPI ToString(
        /* [out] */ String* result);

    /**
     * Creates a minimal audio format.
     * @param mime The mime type of the content.
     * @param sampleRate The sampling rate of the content.
     * @param channelCount The number of audio channels in the content.
     */
    static CARAPI CreateAudioFormat(
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
   static CARAPI CreateVideoFormat(
        /* [in] */ const String& mime,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [out] */ IMediaFormat** result);

    CARAPI GetMap(
        /* [out] */ IObjectStringMap** result);

private:
    AutoPtr<IObjectStringMap> mMap;
};

} // namespace Media
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_MEDIA_CMEDIAFORMAT_H__
