
namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Media {

//===============================================================
//              MediaCodecBridge::DequeueInputResult
//===============================================================

MediaCodecBridge::DequeueInputResult::DequeueInputResult(
    /* [in] */ Int32 status,
    /* [in] */ Int32 index)
    : mStatus(status)
    , mIndex(index)
{
}

//@CalledByNative("DequeueInputResult")
Int32 MediaCodecBridge::DequeueInputResult::Status()
{
    return mStatus;
}

//@CalledByNative("DequeueInputResult")
Int32 MediaCodecBridge::DequeueInputResult::Index()
{
    return mIndex;
}

//===============================================================
//                MediaCodecBridge::CodecInfo
//===============================================================

MediaCodecBridge::CodecInfo::CodecInfo(
    /* [in] */ const String& codecType,
    /* [in] */ const String& codecName,
    /* [in] */ Int32 direction)
    : mCodecType(codecType)
    , mCodecName(codecName)
    , mDirection(direction)
{
}

//@CalledByNative("CodecInfo")
String MediaCodecBridge::CodecInfo::CodecType()
{
    return mCodecType;
}

//@CalledByNative("CodecInfo")
String MediaCodecBridge::CodecInfo::CodecName()
{
    return mCodecName;
}

//@CalledByNative("CodecInfo")
Int32 MediaCodecBridge::CodecInfo::Direction()
{
    return mDirection;
}

//===============================================================
//            MediaCodecBridge::DequeueOutputResult
//===============================================================

MediaCodecBridge::DequeueOutputResult::DequeueOutputResult(
    /* [in] */ Int32 status,
    /* [in] */ Int32 index,
    /* [in] */ Int32 flags,
    /* [in] */ Int32 offset,
    /* [in] */ Int64 presentationTimeMicroseconds,
    /* [in] */ Int32 numBytes)
    : mStatus(status)
    , mIndex(index)
    , mFlags(flags)
    , mOffset(offset)
    , mPresentationTimeMicroseconds(presentationTimeMicroseconds)
    , mNumBytes(numBytes)
{
}

//@CalledByNative("DequeueOutputResult")
Int32 MediaCodecBridge::DequeueOutputResult::Status()
{
    return mStatus;
}

//@CalledByNative("DequeueOutputResult")
Int32 MediaCodecBridge::DequeueOutputResult::Index()
{
    return mIndex;
}

//@CalledByNative("DequeueOutputResult")
Int32 MediaCodecBridge::DequeueOutputResult::Flags()
{
    return mFlags;
}

//@CalledByNative("DequeueOutputResult")
Int32 MediaCodecBridge::DequeueOutputResult::Offset()
{
    return mOffset;
}

//@CalledByNative("DequeueOutputResult")
Int64 MediaCodecBridge::DequeueOutputResult::PresentationTimeMicroseconds()
{
    return mPresentationTimeMicroseconds;
}

//@CalledByNative("DequeueOutputResult")
Int32 MediaCodecBridge::DequeueOutputResult::NumBytes()
{
    return mNumBytes;
}

//===============================================================
//                      MediaCodecBridge
//===============================================================

const String MediaCodecBridge::TAG("MediaCodecBridge");

// Error code for MediaCodecBridge. Keep this value in sync with
// MediaCodecStatus in media_codec_bridge.h.
const Int32 MediaCodecBridge::MEDIA_CODEC_OK;
const Int32 MediaCodecBridge::MEDIA_CODEC_DEQUEUE_INPUT_AGAIN_LATER;
const Int32 MediaCodecBridge::MEDIA_CODEC_DEQUEUE_OUTPUT_AGAIN_LATER;
const Int32 MediaCodecBridge::MEDIA_CODEC_OUTPUT_BUFFERS_CHANGED;
const Int32 MediaCodecBridge::MEDIA_CODEC_OUTPUT_FORMAT_CHANGED;
const Int32 MediaCodecBridge::MEDIA_CODEC_INPUT_END_OF_STREAM;
const Int32 MediaCodecBridge::MEDIA_CODEC_OUTPUT_END_OF_STREAM;
const Int32 MediaCodecBridge::MEDIA_CODEC_NO_KEY;
const Int32 MediaCodecBridge::MEDIA_CODEC_STOPPED;
const Int32 MediaCodecBridge::MEDIA_CODEC_ERROR;

// Codec direction.  Keep this in sync with media_codec_bridge.h.
const Int32 MediaCodecBridge::MEDIA_CODEC_DECODER;
const Int32 MediaCodecBridge::MEDIA_CODEC_ENCODER;

// Max adaptive playback size to be supplied to the decoder.
const Int32 MediaCodecBridge::MAX_ADAPTIVE_PLAYBACK_WIDTH;
const Int32 MediaCodecBridge::MAX_ADAPTIVE_PLAYBACK_HEIGHT;

// After a flush(), dequeueOutputBuffer() can often produce empty presentation timestamps
// for several frames. As a result, the player may find that the time does not increase
// after decoding a frame. To detect this, we check whether the presentation timestamp from
// dequeueOutputBuffer() is larger than input_timestamp - MAX_PRESENTATION_TIMESTAMP_SHIFT_US
// after a flush. And we set the presentation timestamp from dequeueOutputBuffer() to be
// non-decreasing for the remaining frames.
const Int64 MediaCodecBridge::MAX_PRESENTATION_TIMESTAMP_SHIFT_US;

MediaCodecBridge::MediaCodecBridge(
    /* [in] */ MediaCodec* mediaCodec,
    /* [in] */ const String& mime,
    /* [in] */ Boolean adaptivePlaybackSupported)
    : mFlushed(FALSE)
    , mLastPresentationTimeUs(0)
    , mAdaptivePlaybackSupported(FALSE)
    , mMediaCodec(mediaCodec)
    , mMime(mime)
    , mLastPresentationTimeUs(0)
    , mFlushed(TRUE)
    , mAdaptivePlaybackSupported(adaptivePlaybackSupported)
{
    assert(mediaCodec != NULL);
}

/**
 * Get a list of supported android codec mimes.
 */
//@CalledByNative
AutoPtr< ArrayOf<CodecInfo> > MediaCodecBridge::GetCodecsInfo()
{
    // Return the first (highest-priority) codec for each MIME type.
    Map<String, CodecInfo> encoderInfoMap = new HashMap<String, CodecInfo>();
    Map<String, CodecInfo> decoderInfoMap = new HashMap<String, CodecInfo>();
    int count = MediaCodecList.getCodecCount();
    for (int i = 0; i < count; ++i) {
        MediaCodecInfo info = MediaCodecList.getCodecInfoAt(i);
        int direction =
            info.isEncoder() ? MEDIA_CODEC_ENCODER : MEDIA_CODEC_DECODER;
        String codecString = info.getName();
        String[] supportedTypes = info.getSupportedTypes();
        for (int j = 0; j < supportedTypes.length; ++j) {
            Map<String, CodecInfo> map = info.isEncoder() ? encoderInfoMap : decoderInfoMap;
            if (!map.containsKey(supportedTypes[j])) {
                map.put(supportedTypes[j], new CodecInfo(
                    supportedTypes[j], codecString, direction));
            }
        }
    }
    ArrayList<CodecInfo> codecInfos = new ArrayList<CodecInfo>(
        decoderInfoMap.size() + encoderInfoMap.size());
    codecInfos.addAll(encoderInfoMap.values());
    codecInfos.addAll(decoderInfoMap.values());
    return codecInfos.toArray(new CodecInfo[codecInfos.size()]);
}

String MediaCodecBridge::GetDecoderNameForMime(
    /* [in] */ const String& mime)
{
    AutoPtr<IMediaCodecList> mediaCodecList;
    CMediaCodecList::AcquireSingleton((IMediaCodecList**)&mediaCodecList);
    Int32 count;
    mediaCodecList->GetCodecCount(&count);
    for (Int32 i = 0; i < count; ++i) {
        AutoPtr<IMediaCodecInfo> info;
        mediaCodecList->GetCodecInfoAt(i, (IMediaCodecInfo**)&info);
        Boolean isEncoder = FALSE;
        info->IsEncoder(&isEncoder);
        if (isEncoder()) {
            continue;
        }

        AutoPtr< ArrayOf<String> > supportedTypes;
        info->GetSupportedTypes((ArrayOf<String>**)&supportedTypes);
        Int32 length = supportedTypes->GetLength();
        for (Int32 j = 0; j < length; ++j) {
            if ((*supportedTypes)[j].EqualsIgnoreCase(mime)) {
                String name;
                info->GetName(&name);
                return name;
            }
        }
    }

    return String(NULL);
}

//@CalledByNative
AutoPtr<MediaCodecBridge> MediaCodecBridge::Create(
    /* [in] */ const String& mime,
    /* [in] */ Boolean isSecure,
    /* [in] */ Int32 direction)
{
    // Creation of ".secure" codecs sometimes crash instead of throwing exceptions
    // on pre-JBMR2 devices.
    if (isSecure && Build.VERSION.SDK_INT < Build.VERSION_CODES.JELLY_BEAN_MR2) {
        return NULL;
    }

    AutoPtr<IMediaCodec> mediaCodec;
    Boolean adaptivePlaybackSupported = FALSE;
    AutoPtr<IMediaCodecHelper> mediaCodecHelper;
    CMediaCodecHelper::AcquireSingleton((IMediaCodecHelper**)&mediaCodecHelper)
    // try {
        // |isSecure| only applies to video decoders.
        if (mime.StartsWith("video") && isSecure && direction == MEDIA_CODEC_DECODER) {
            String decoderName = GetDecoderNameForMime(mime);
            if (decoderName == NULL) {
                return NULL;
            }

            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.KITKAT) {
                // To work around an issue that we cannot get the codec info from the secure
                // decoder, create an insecure decoder first so that we can query its codec
                // info. http://b/15587335.
                AutoPtr<IMediaCodec> insecureCodec;
                mediaCodecHelper->CreateByCodecName(decoderName, (IMediaCodec**)&insecureCodec);
                adaptivePlaybackSupported = CodecSupportsAdaptivePlayback(insecureCodec, mime);
                insecureCodec->Release();
            }
            String _decoderName(decoderName);
            _decoderName += ".secure";
            mediaCodecHelper->CreateByCodecName(_decoderName, (IMediaCodec**)&mediaCodec);
        }
        else {
            if (direction == MEDIA_CODEC_ENCODER) {
                mediaCodecHelper->CreateEncoderByType(mime, (IMediaCodec**)&mediaCodec);
            }
            else {
                mediaCodecHelper->CreateDecoderByType(mime, (IMediaCodec**)&mediaCodec);
                adaptivePlaybackSupported = CodecSupportsAdaptivePlayback(mediaCodec, mime);
            }
        }
    // } catch (Exception e) {
    //     Log.e(TAG, "Failed to create MediaCodec: " +  mime + ", isSecure: "
    //             + isSecure + ", direction: " + direction, e);
    // }

    if (mediaCodec == NULL) {
        return NULL;
    }

    AutoPtr<MediaCodecBridge> ret = new MediaCodecBridge(mediaCodec, mime, adaptivePlaybackSupported);
    return ret;
}

//@CalledByNative
void MediaCodecBridge::Release()
{
    // try {
        mMediaCodec->Release();
    // } catch (IllegalStateException e) {
    //     // The MediaCodec is stuck in a wrong state, possibly due to losing
    //     // the surface.
    //     Log.e(TAG, "Cannot release media codec", e);
    // }

    mMediaCodec = NULL;
    if (mAudioTrack != NULL) {
        mAudioTrack->Release();
    }
}

//@CalledByNative
Boolean MediaCodecBridge::Start()
{
    // try {
        mMediaCodec->Start();
        mMediaCodec->GetInputBuffers((ArrayOf<IByteBuffer>**)&mInputBuffers);
    // } catch (IllegalStateException e) {
    //     Log.e(TAG, "Cannot start the media codec", e);
    //     return false;
    // }

    return TRUE;
}

//@CalledByNative
AutoPtr<DequeueInputResult> MediaCodecBridge::DequeueInputBuffer(
    /* [in] */ Int64 timeoutUs)
{
    Int32 status = MEDIA_CODEC_ERROR;
    Int32 index = -1;
    // try {
        Int32 indexOrStatus;
        mMediaCodec->DequeueInputBuffer(timeoutUs, &indexOrStatus);
        if (indexOrStatus >= 0) { // index!
            status = MEDIA_CODEC_OK;
            index = indexOrStatus;
        }
        else if (indexOrStatus == IMediaCodec::INFO_TRY_AGAIN_LATER) {
//            Log.e(TAG, "dequeueInputBuffer: MediaCodec.INFO_TRY_AGAIN_LATER");
            status = MEDIA_CODEC_DEQUEUE_INPUT_AGAIN_LATER;
        }
        else {
//            Log.e(TAG, "Unexpected index_or_status: " + indexOrStatus);
            assert(FALSE);
        }
    // } catch (Exception e) {
    //     Log.e(TAG, "Failed to dequeue input buffer", e);
    // }

    AutoPtr<DequeueInputResult> ret = new DequeueInputResult(status, index);
    return ret;
}

//@CalledByNative
Int32 MediaCodecBridge::Flush()
{
    // try {
        mFlushed = TRUE;
        if (mAudioTrack != NULL) {
            // Need to call pause() here, or otherwise flush() is a no-op.
            mAudioTrack->Pause();
            mAudioTrack->Flush();
        }
        mMediaCodec->Flush();
    // } catch (IllegalStateException e) {
    //     Log.e(TAG, "Failed to flush MediaCodec", e);
    //     return MEDIA_CODEC_ERROR;
    // }

    return MEDIA_CODEC_OK;
}

//@CalledByNative
void MediaCodecBridge::Stop()
{
    mMediaCodec->Stop();
    if (mAudioTrack != NULL) {
        mAudioTrack->Pause();
    }
}

//@CalledByNative
Int32 MediaCodecBridge::GetOutputHeight()
{
    AutoPtr<IMediaFormat> format;
    mMediaCodec->GetOutputFormat((IMediaFormat**)&format);
    Int32 height;
    format->GetInt32(IMediaFormat::KEY_HEIGHT, &height);
    return height;
}

//@CalledByNative
Int32 MediaCodecBridge::GetOutputWidth()
{
    AutoPtr<IMediaFormat> format;
    mMediaCodec->GetOutputFormat((IMediaFormat**)&format);
    Int32 width;
    format->GetInt32(IMediaFormat::KEY_WIDTH, &width)
    return width;
}

//@CalledByNative
AutoPtr<IByteBuffer> MediaCodecBridge::GetInputBuffer(
    /* [in] */ Int32 index)
{
    return (*mInputBuffers)[index];
}

//@CalledByNative
AutoPtr<IByteBuffer> MediaCodecBridge::GetOutputBuffer(
    /* [in] */ Int32 index)
{
    return (*mOutputBuffers)[index];
}

//@CalledByNative
Int32 MediaCodecBridge::GetInputBuffersCount()
{
    return mInputBuffers->GetLength();
}

//@CalledByNative
Int32 MediaCodecBridge::GetOutputBuffersCount()
{
    return mOutputBuffers != NULL ? mOutputBuffers->GetLength() : -1;
}

//@CalledByNative
Int32 MediaCodecBridge::GetOutputBuffersCapacity()
{
    Int32 capacity;
    return mOutputBuffers != NULL ? ((*mOutputBuffers)[0]->Capacity(&capacity), capacity) : -1;
}

//@CalledByNative
Boolean MediaCodecBridge::GetOutputBuffers()
{
    // try {
        mMediaCodec->GetOutputBuffers((ArrayOf<IByteBuffer>**)&mOutputBuffers);
    // } catch (IllegalStateException e) {
    //     Log.e(TAG, "Cannot get output buffers", e);
    //     return false;
    // }

    return TRUE;
}

//@CalledByNative
Int32 MediaCodecBridge::QueueInputBuffer(
    /* [in] */ Int32 index,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 size,
    /* [in] */ Int64 presentationTimeUs,
    /* [in] */ Int32 flags)
{
    ResetLastPresentationTimeIfNeeded(presentationTimeUs);
    // try {
        mMediaCodec->QueueInputBuffer(index, offset, size, presentationTimeUs, flags);
    // } catch (Exception e) {
    //     Log.e(TAG, "Failed to queue input buffer", e);
    //     return MEDIA_CODEC_ERROR;
    // }

    return MEDIA_CODEC_OK;
}

//@CalledByNative
void MediaCodecBridge::SetVideoBitrate(
    /* [in] */ Int32 bps)
{
    AutoPtr<IBundle> b;
    CBundle::New((IBundle**)&b);
    b->PutInt32(IMediaCodec::PARAMETER_KEY_VIDEO_BITRATE, bps);
    mMediaCodec->SetParameters(b);
}

//@CalledByNative
void MediaCodecBridge::RequestKeyFrameSoon()
{
    AutoPtr<IBundle> b;
    CBundle::New((IBundle**)&b);
    b->PutInt32(IMediaCodec::PARAMETER_KEY_REQUEST_SYNC_FRAME, 0);
    mMediaCodec->SetParameters(b);
}

//@CalledByNative
Int32 MediaCodecBridge::QueueSecureInputBuffer(
    /* [in] */ Int32 index,
    /* [in] */ Int32 offset,
    /* [in] */ ArrayOf<Byte>* iv,
    /* [in] */ ArrayOf<Byte>* keyId,
    /* [in] */ ArrayOf<Int32>* numBytesOfClearData,
    /* [in] */ ArrayOf<Int32>* numBytesOfEncryptedData,
    /* [in] */ Int32 numSubSamples,
    /* [in] */ Int64 presentationTimeUs)
{
    ResetLastPresentationTimeIfNeeded(presentationTimeUs);
    // try {
        AutoPtr<IMediaCodecCryptoInfo> cryptoInfo;
        CMediaCodecCryptoInfo((IMediaCodecCryptoInfo**)&cryptoInfo);
        cryptoInfo->Set(numSubSamples, numBytesOfClearData, numBytesOfEncryptedData,
                keyId, iv, IMediaCodec::CRYPTO_MODE_AES_CTR);
        mMediaCodec->QueueSecureInputBuffer(index, offset, cryptoInfo, presentationTimeUs, 0);
    // } catch (MediaCodec.CryptoException e) {
    //     Log.e(TAG, "Failed to queue secure input buffer", e);
    //     if (e.getErrorCode() == MediaCodec.CryptoException.ERROR_NO_KEY) {
    //         Log.e(TAG, "MediaCodec.CryptoException.ERROR_NO_KEY");
    //         return MEDIA_CODEC_NO_KEY;
    //     }
    //     Log.e(TAG, "MediaCodec.CryptoException with error code " + e.getErrorCode());
    //     return MEDIA_CODEC_ERROR;
    // } catch (IllegalStateException e) {
    //     Log.e(TAG, "Failed to queue secure input buffer", e);
    //     return MEDIA_CODEC_ERROR;
    // }
    return MEDIA_CODEC_OK;
}

//@CalledByNative
void MediaCodecBridge::ReleaseOutputBuffer(
    /* [in] */ Int32 index,
    /* [in] */ Boolean render)
{
    // try {
        mMediaCodec->ReleaseOutputBuffer(index, render);
    // } catch (IllegalStateException e) {
    //     // TODO(qinmin): May need to report the error to the caller. crbug.com/356498.
    //     Log.e(TAG, "Failed to release output buffer", e);
    // }
}

//@CalledByNative
AutoPtr<DequeueOutputResult> MediaCodecBridge::DequeueOutputBuffer(
    /* [in] */ Int64 timeoutUs)
{
    AutoPtr<IMediaCodecBufferInfo> info;
    CMediaCodecBufferInfo::New((IMediaCodecBufferInfo**)&info);
    Int32 status = MEDIA_CODEC_ERROR;
    Int32 index = -1;
    // try {
        Int32 indexOrStatus;
        mMediaCodec->DequeueOutputBuffer(info, timeoutUs, &indexOrStatus);
        Int64 timeUs;
        info->GetpresentationTimeUs(&timeUs);
        if (timeUs < mLastPresentationTimeUs) {
            // TODO(qinmin): return a special code through DequeueOutputResult
            // to notify the native code the the frame has a wrong presentation
            // timestamp and should be skipped.
            info->SetPresentationTimeUs(mLastPresentationTimeUs);
        }

        info->GetPresentationTimeUs(&mLastPresentationTimeUs);

        if (indexOrStatus >= 0) { // index!
            status = MEDIA_CODEC_OK;
            index = indexOrStatus;
        }
        else if (indexOrStatus == IMediaCodec::INFO_OUTPUT_BUFFERS_CHANGED) {
            status = MEDIA_CODEC_OUTPUT_BUFFERS_CHANGED;
        }
        else if (indexOrStatus == IMediaCodec::INFO_OUTPUT_FORMAT_CHANGED) {
            status = MEDIA_CODEC_OUTPUT_FORMAT_CHANGED;
        }
        else if (indexOrStatus == IMediaCodec::INFO_TRY_AGAIN_LATER) {
            status = MEDIA_CODEC_DEQUEUE_OUTPUT_AGAIN_LATER;
        }
        else {
            //Log.e(TAG, "Unexpected index_or_status: " + indexOrStatus);
            assert(FALSE);
        }
    // } catch (IllegalStateException e) {
    //     Log.e(TAG, "Failed to dequeue output buffer", e);
    // }

    Int32 flags, offset, size;
    info->GetFlags(&flags);
    info->GetOffset(&offset);
    info->GetSize(&size);
    Int64 presentationTimeUs;
    info->GetPresentationTimeUs(&presentationTimeUs);
    AutoPtr<DequeueOutputResult> ret = new DequeueOutputResult(
            status, index, flags, offset, presentationTimeUs, size);

    return ret;
}

//@CalledByNative
Boolean MediaCodecBridge::ConfigureVideo(
    /* [in] */ IMediaFormat* format,
    /* [in] */ ISurface* surface,
    /* [in] */ IMediaCrypto* crypto,
    /* [in] */ Int32 flags)
{
    // try {
        if (mAdaptivePlaybackSupported) {
            format->SetInteger(IMediaFormat::KEY_MAX_WIDTH, MAX_ADAPTIVE_PLAYBACK_WIDTH);
            format->SetInteger(IMediaFormat::KEY_MAX_HEIGHT, MAX_ADAPTIVE_PLAYBACK_HEIGHT);
        }

        mMediaCodec->Configure(format, surface, crypto, flags);

        return TRUE;
    // } catch (IllegalStateException e) {
    //     Log.e(TAG, "Cannot configure the video codec", e);
    // }

    return FALSE;
}

//@CalledByNative
AutoPtr<IMediaFormat> MediaCodecBridge::CreateAudioFormat(
    /* [in] */ const String& mime,
    /* [in] */ Int32 sampleRate,
    /* [in] */ Int32 channelCount)
{
    AutoPtr<IMediaFormatHelper> helper;
    CMediaFormatHelper::AcquireSingleton((IMediaFormatHelper**)&helper);
    AutoPtr<IMediaFormat> format;
    helper->CreateAudioFormat(mime, sampleRate, channelCount, (IMediaFormat**)&format);
    return format;
}

//@CalledByNative
AutoPtr<IMediaFormat> MediaCodecBridge::CreateVideoDecoderFormat(
    /* [in] */ const String& mime,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    AutoPtr<IMediaFormatHelper> helper;
    CMediaFormatHelper::AcquireSingleton((IMediaFormatHelper**)&helper);
    AutoPtr<IMediaFormat> format;
    helper->CreateVideoFormat(mime, width, height, (IMediaFormat**)&format);
    return format;
}

//@CalledByNative
AutoPtr<IMediaFormat> MediaCodecBridge::CreateVideoEncoderFormat(
    /* [in] */ const String& mime,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Int32 bitRate,
    /* [in] */ Int32 frameRate,
    /* [in] */ Int32 iFrameInterval,
    /* [in] */ Int32 colorFormat)
{
    AutoPtr<IMediaFormatHelper> helper;
    CMediaFormatHelper::AcquireSingleton((IMediaFormatHelper**)&helper);
    AutoPtr<IMediaFormat> format;
    helper->CreateVideoFormat(mime, width, height, (IMediaFormat**)&format);
    format->SetInt32(IMediaFormat::KEY_BIT_RATE, bitRate);
    format->SetInt32(IMediaFormat::KEY_FRAME_RATE, frameRate);
    format->SetInt32(IMediaFormat::KEY_I_FRAME_INTERVAL, iFrameInterval);
    format->SetInt32(IMediaFormat::KEY_COLOR_FORMAT, colorFormat);
    return format;
}

//@CalledByNative
Boolean MediaCodecBridge::IsAdaptivePlaybackSupported(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    if (!mAdaptivePlaybackSupported)
        return FALSE;
    return width <= MAX_ADAPTIVE_PLAYBACK_WIDTH && height <= MAX_ADAPTIVE_PLAYBACK_HEIGHT;
}

Boolean MediaCodecBridge::CodecSupportsAdaptivePlayback(
    /* [in] */ IMediaCodec* mediaCodec,
    /* [in] */ const String& mime)
{
    if (Build.VERSION.SDK_INT < Build.VERSION_CODES.KITKAT || mediaCodec == null) {
        return FALSE;
    }

    // try {
        AutoPtr<IMediaCodecInfo> info;
        mediaCodec->GetCodecInfo((IMediaCodecInfo**)&info);
        Boolean isEncoder;
        info->IsEncoder(&isEncoder);
        if (isEncoder) {
            return FALSE;
        }

        AutoPtr<ICodecCapabilities> capabilities;
        info->GetCapabilitiesForType(mime, (ICodecCapabilities**)&capabilities);
        Boolean isFeatureSupported;
        return (capabilities != NULL) && (capabilities->IsFeatureSupported(
                ICodecCapabilities::FEATURE_AdaptivePlayback, &isFeatureSupported), isFeatureSupported);
    // } catch (IllegalArgumentException e) {
    //       Log.e(TAG, "Cannot retrieve codec information", e);
    // }

    return FALSE;
}

//@CalledByNative
void MediaCodecBridge::SetCodecSpecificData(
    /* [in] */ IMediaFormat* format,
    /* [in] */ Int32 index,
    /* [in] */ ArrayOf<Byte>* bytes)
{
    String name;
    if (index == 0) {
        name = "csd-0";
    }
    else if (index == 1) {
        name = "csd-1";
    }

    if (name != NULL) {
        AutoPtr<IByteBuffer> buf;
        ByteBuffer::Wrap(bytes, (IByteBuffer**)&buf);
        format->SetByteBuffer(name, buf);
    }
}

//@CalledByNative
void MediaCodecBridge::SetFrameHasADTSHeader(
    /* [in] */ IMediaFormat* format)
{
    format->SetInteger(IMediaFormat::KEY_IS_ADTS, 1);
}

//@CalledByNative
Boolean MediaCodecBridge::ConfigureAudio(
    /* [in] */ IMediaFormat* format,
    /* [in] */ IMediaCrypto* crypto,
    /* [in] */ Int32 flags,
    /* [in] */ Boolean playAudio)
{
    // try {
        mMediaCodec->Configure(format, NULL, crypto, flags);
        if (playAudio) {
            Int32 sampleRate;
            format->GetInt32(IMediaFormat::KEY_SAMPLE_RATE, &sampleRate);
            Int32 channelCount;
            format->GetInt32(IMediaFormat::KEY_CHANNEL_COUNT, &channelCount);
            Int32 channelConfig = GetAudioFormat(channelCount);
            // Using 16bit PCM for output. Keep this value in sync with
            // kBytesPerAudioOutputSample in media_codec_bridge.cc.
            Int32 minBufferSize = /*AudioTrack.*/GetMinBufferSize(sampleRate, channelConfig,
                    IAudioFormat::ENCODING_PCM_16BIT);
            CAudioTrack::New(IAudioManager::STREAM_MUSIC, sampleRate, channelConfig,
                    IAudioFormat::ENCODING_PCM_16BIT, minBufferSize, IAudioTrack::MODE_STREAM,
                    (IAudioTrack**)&mAudioTrack);
            Int32 state;
            mAudioTrack->GetState(&state);
            if (state == IAudioTrack::STATE_UNINITIALIZED) {
                mAudioTrack = NULL;
                return FALSE;
            }
        }

        return TRUE;
    // } catch (IllegalStateException e) {
    //     Log.e(TAG, "Cannot configure the audio codec", e);
    // }

    return FALSE;
}

/**
 *  Play the audio buffer that is passed in.
 *
 *  @param buf Audio buffer to be rendered.
 *  @return The number of frames that have already been consumed by the
 *  hardware. This number resets to 0 after each flush call.
 */
//@CalledByNative
Int64 MediaCodecBridge::PlayOutputBuffer(
    /* [in] */ ArrayOf<Byte>* buf)
{
    if (mAudioTrack == NULL) {
        return 0;
    }

    Int32 state;
    mAudioTrack->GetPlayState(&state);
    if (IAudioTrack::PLAYSTATE_PLAYING != state) {
        mAudioTrack->Play();
    }

    Int32 size;
    mAudioTrack->Write(buf, 0, buf->GetLength(), &size);
    if (buf->GetLength() != size) {
//        Log.i(TAG, "Failed to send all data to audio output, expected size: " +
//                buf.length + ", actual size: " + size);
    }
    // TODO(qinmin): Returning the head position allows us to estimate
    // the current presentation time in native code. However, it is
    // better to use AudioTrack.getCurrentTimestamp() to get the last
    // known time when a frame is played. However, we will need to
    // convert the java nano time to C++ timestamp.
    // If the stream runs too long, getPlaybackHeadPosition() could
    // overflow. AudioTimestampHelper in MediaSourcePlayer has the same
    // issue. See http://crbug.com/358801.
    Int64 position;
    mAudioTrack->GetPlaybackHeadPosition(&position);
    return position;
}

//@CalledByNative
void MediaCodecBridge::SetVolume(
    /* [in] */ Double volume)
{
    if (mAudioTrack != NULL) {
        mAudioTrack->SetStereoVolume((Float) volume, (Float) volume);
    }
}

void MediaCodecBridge::ResetLastPresentationTimeIfNeeded(
    /* [in] */ Int64 presentationTimeUs)
{
    if (mFlushed) {
        mLastPresentationTimeUs =
                Math::Max(presentationTimeUs - MAX_PRESENTATION_TIMESTAMP_SHIFT_US, 0);
        mFlushed = FALSE;
    }
}

Int32 MediaCodecBridge::GetAudioFormat(
    /* [in] */ Int32 channelCount)
{
    switch (channelCount) {
        case 1:
            return IAudioFormat::CHANNEL_OUT_MONO;
        case 2:
            return IAudioFormat::CHANNEL_OUT_STEREO;
        case 4:
            return IAudioFormat::CHANNEL_OUT_QUAD;
        case 6:
            return IAudioFormat::CHANNEL_OUT_5POINT1;
        case 8:
            return IAudioFormat::CHANNEL_OUT_7POINT1;
        default:
            return IAudioFormat::CHANNEL_OUT_DEFAULT;
    }
}

} // namespace Media
} // namespace Webkit
} // namespace Droid
} // namespace Elastos
