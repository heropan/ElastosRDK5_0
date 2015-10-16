
#ifndef __ELASTOS_DROID_MEDIA_CMEDIACODEC_H__
#define __ELASTOS_DROID_MEDIA_CMEDIACODEC_H__

#include "_Elastos_Droid_Media_CMediaCodec.h"
#include "elastos/droid/ext/frameworkext.h"
#include <media/stagefright/MediaCodec.h>
#include <media/stagefright/foundation/ALooper.h>
#include <gui/SurfaceTextureClient.h>
#include "CMediaCrypto.h"
#include "CMediaFormat.h"

using Elastos::Droid::View::ISurface;
using Elastos::IO::IByteBuffer;

namespace Elastos {
namespace Droid {
namespace Media {

CarClass(CMediaCodec)
{
public:
    /**
     * Instantiate a decoder supporting input data of the given mime type.
     *
     * The following is a partial list of defined mime types and their semantics:
     * <ul>
     * <li>"video/x-vnd.on2.vp8" - VPX video (i.e. video in .webm)
     * <li>"video/avc" - H.264/AVC video
     * <li>"video/mp4v-es" - MPEG4 video
     * <li>"video/3gpp" - H.263 video
     * <li>"audio/3gpp" - AMR narrowband audio
     * <li>"audio/amr-wb" - AMR wideband audio
     * <li>"audio/mpeg" - MPEG1/2 audio layer III
     * <li>"audio/mp4a-latm" - AAC audio
     * <li>"audio/vorbis" - vorbis audio
     * <li>"audio/g711-alaw" - G.711 alaw audio
     * <li>"audio/g711-mlaw" - G.711 ulaw audio
     * </ul>
     *
     * @param type The mime type of the input data.
     */
    static CARAPI CreateDecoderByType(
        /* [in] */ const String& type,
        /* [out] */ IMediaCodec** result);

    /**
     * Instantiate an encoder supporting output data of the given mime type.
     * @param type The desired mime type of the output data.
     */
    static CARAPI CreateEncoderByType(
        /* [in] */ const String& type,
        /* [out] */ IMediaCodec** result);

    /**
     * If you know the exact name of the component you want to instantiate
     * use this method to instantiate it. Use with caution.
     * Likely to be used with information obtained from {@link android.media.MediaCodecList}
     * @param name The name of the codec to be instantiated.
     */
    static CARAPI CreateByCodecName(
        /* [in] */ const String& name,
        /* [out] */ IMediaCodec** result);

    /**
     * Configures a component.
     *
     * @param format The format of the input data (decoder) or the desired
     *               format of the output data (encoder).
     * @param surface Specify a surface on which to render the output of this
     *                decoder.
     * @param crypto  Specify a crypto object to facilitate secure decryption
     *                of the media data.
     * @param flags   Specify {@link #CONFIGURE_FLAG_ENCODE} to configure the
     *                component as an encoder.
     */
    CARAPI Configure(
        /* [in] */ IMediaFormat* format,
        /* [in] */ ISurface* surface,
        /* [in] */ IMediaCrypto* crypto,
        /* [in] */ Int32 flags);

    /**
     * After filling a range of the input buffer at the specified index
     * submit it to the component.
     *
     * Many decoders require the actual compressed data stream to be
     * preceded by "codec specific data", i.e. setup data used to initialize
     * the codec such as PPS/SPS in the case of AVC video or code tables
     * in the case of vorbis audio.
     * The class {@link android.media.MediaExtractor} provides codec
     * specific data as part of
     * the returned track format in entries named "csd-0", "csd-1" ...
     *
     * These buffers should be submitted using the flag {@link #BUFFER_FLAG_CODEC_CONFIG}.
     *
     * To indicate that this is the final piece of input data (or rather that
     * no more input data follows unless the decoder is subsequently flushed)
     * specify the flag {@link #BUFFER_FLAG_END_OF_STREAM}.
     *
     * @param index The index of a client-owned input buffer previously returned
     *              in a call to {@link #dequeueInputBuffer}.
     * @param offset The byte offset into the input buffer at which the data starts.
     * @param size The number of bytes of valid input data.
     * @param presentationTimeUs The time at which this buffer should be rendered.
     * @param flags A bitmask of flags {@link #BUFFER_FLAG_SYNC_FRAME},
     *              {@link #BUFFER_FLAG_CODEC_CONFIG} or {@link #BUFFER_FLAG_END_OF_STREAM}.
     * @throws CryptoException if a crypto object has been specified in
     *         {@link #configure}
     */
    CARAPI QueueInputBuffer(
        /* [in] */ Int32 index,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 size,
        /* [in] */ Int64 presentationTimeUs,
        /* [in] */ Int32 flags);
    /**
     * Similar to {@link #queueInputBuffer} but submits a buffer that is
     * potentially encrypted.
     * @param index The index of a client-owned input buffer previously returned
     *              in a call to {@link #dequeueInputBuffer}.
     * @param offset The byte offset into the input buffer at which the data starts.
     * @param info Metadata required to facilitate decryption, the object can be
     *             reused immediately after this call returns.
     * @param presentationTimeUs The time at which this buffer should be rendered.
     * @param flags A bitmask of flags {@link #BUFFER_FLAG_SYNC_FRAME},
     *              {@link #BUFFER_FLAG_CODEC_CONFIG} or {@link #BUFFER_FLAG_END_OF_STREAM}.
     */
    CARAPI QueueSecureInputBuffer(
        /* [in] */ Int32 index,
        /* [in] */ Int32 offset,
        /* [in] */ IMediaCodecCryptoInfo* info,
        /* [in] */ Int64 presentationTimeUs,
        /* [in] */ Int32 flags);

    /**
     * Returns the index of an input buffer to be filled with valid data
     * or -1 if no such buffer is currently available.
     * This method will return immediately if timeoutUs == 0, wait indefinitely
     * for the availability of an input buffer if timeoutUs &lt; 0 or wait up
     * to "timeoutUs" microseconds if timeoutUs &gt; 0.
     * @param timeoutUs The timeout in microseconds, a negative timeout indicates "infinite".
     */
    CARAPI DequeueInputBuffer(
        /* [in] */ Int64 timeoutUs,
        /* [out] */ Int32* result);

    /**
     * Dequeue an output buffer, block at most "timeoutUs" microseconds.
     * Returns the index of an output buffer that has been successfully
     * decoded or one of the INFO_* constants below.
     * @param info Will be filled with buffer meta data.
     * @param timeoutUs The timeout in microseconds, a negative timeout indicates "infinite".
     */
    CARAPI DequeueOutputBuffer(
        /* [in] */ IMediaCodecBufferInfo* info,
        /* [in] */ Int64 timeoutUs,
        /* [out] */ Int32* index);

    /**
     * If you are done with a buffer, use this call to return the buffer to
     * the codec. If you previously specified a surface when configuring this
     * video decoder you can optionally render the buffer.
     * @param index The index of a client-owned output buffer previously returned
     *              in a call to {@link #dequeueOutputBuffer}.
     * @param render If a valid surface was specified when configuring the codec,
     *               passing true renders this output buffer to the surface.
     */
    CARAPI ReleaseOutputBuffer(
        /* [in] */ Int32 index,
        /* [in] */ Boolean render);

    /**
     * Call this after dequeueOutputBuffer signals a format change by returning
     * {@link #INFO_OUTPUT_FORMAT_CHANGED}
     */
    CARAPI GetOutputFormat(
        /* [out] */ IMediaFormat** result);

    /**
     * Call this after start() returns.
     */
    CARAPI GetInputBuffers(
        /* [out] */ ArrayOf<IByteBuffer*>** result);

    /**
     * Call this after start() returns and whenever dequeueOutputBuffer
     * signals an output buffer change by returning
     * {@link #INFO_OUTPUT_BUFFERS_CHANGED}
     */
    CARAPI GetOutputBuffers(
        /* [out] */ ArrayOf<IByteBuffer*>** result);

    /**
     * If a surface has been specified in a previous call to {@link #configure}
     * specifies the scaling mode to use. The default is "scale to fit".
     */
    CARAPI SetVideoScalingMode(
        /* [in] */ Int32 mode);

    CARAPI ReleaseResources();

    /**
     * After successfully configuring the component, call start. On return
     * you can query the component for its input/output buffers.
     */
    CARAPI Start();

    /**
     * Finish the decode/encode session, note that the codec instance
     * remains active and ready to be {@link #start}ed again.
     * To ensure that it is available to other client call {@link #release}
     * and don't just rely on garbage collection to eventually do this for you.
     */
    CARAPI Stop();

    /**
     * Flush both input and output ports of the component, all indices
     * previously returned in calls to {@link #dequeueInputBuffer} and
     * {@link #dequeueOutputBuffer} become invalid.
     */
    CARAPI Flush();

    CARAPI constructor(
        /* [in] */ const String& name,
        /* [in] */ Boolean nameIsType,
        /* [in] */ Boolean encoder);

    CMediaCodec();

    ~CMediaCodec();

private:
    CARAPI NativeConfigure(
        /* [in] */ ArrayOf<String>* keys,
        /* [in] */ ArrayOf<IInterface*>* values,
        /* [in] */ ISurface* surface,
        /* [in] */ IMediaCrypto* crypto,
        /* [in] */ Int32 flags);

 //   CARAPI_( AutoPtr<Map<String, Object> > ) GetOutputFormatNative();

    CARAPI GetBuffers(
        /* [in] */ Boolean input,
        /* [out, callee] */ ArrayOf<IByteBuffer*>** buffer);

    CARAPI_(void) NativeInit();

    CARAPI_(void) NativeSetup(
        /* [in] */ const String& name,
        /* [in] */ Boolean nameIsType,
        /* [in] */ Boolean encoder);

    CARAPI_(void) NativeFinalize();

protected:
    CARAPI_(void) Finalize();

private:
    Int32 mNativeContext;
    static const String TAG;
    android::sp<android::ALooper> mLooper;
    android::sp<android::MediaCodec> mCodec;
    android::sp<android::SurfaceTextureClient> mSurfaceTextureClient;
};


} // namespace Media
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_MEDIA_CMEDIACODEC_H__
