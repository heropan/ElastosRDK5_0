
namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Media {

const String WebAudioMediaCodecBridge::LOG_TAG("WebAudioMediaCodec");

// TODO(rtoy): What is the correct timeout value for reading
// from a file in memory?
const Int64 WebAudioMediaCodecBridge::TIMEOUT_MICROSECONDS;

//@CalledByNative
String WebAudioMediaCodecBridge::CreateTempFile(
    /* [in] */ IContext* ctx)
{
    AutoPtr<IFile> outputDirectory;
    ctx->GetCacheDir((IFile**)&outputDirectory);
    AutoPtr<IFile> outputFile;
    File::CreateTempFile(String("webaudio"), String(".dat"), outputDirectory,
        (IFile**)&outputFile);

    String str;
    outputFile->GetAbsolutePath(&str);

    return str;
}

//@CalledByNative
Boolean WebAudioMediaCodecBridge::DecodeAudioFile(
    /* [in] */ IContext* ctx,
    /* [in] */ Int64 nativeMediaCodecBridge,
    /* [in] */ Int32 inputFD,
    /* [in] */ Int64 dataSize)
{

    if (dataSize < 0 || dataSize > 0x7fffffff)
        return FALSE;

    AutoPtr<IMediaExtractor> extractor;
    CMediaExtractor::New((IMediaExtractor**)&extractor);

    AutoPtr<IParcelFileDescriptorHelper> parcelFileDescriptorHelper;
    CParcelFileDescriptorHelper::AcquireSingleton((IParcelFileDescriptorHelper**)&parcelFileDescriptorHelper);
    AutoPtr<IParcelFileDescriptor> encodedFD;
    parcelFileDescriptorHelper->AdoptFd(inputFD, (IParcelFileDescriptor**)&encodedFD);
    // try {
        AutoPtr<IFileDescriptor> fileDescriptor;
        encodedFD->GetFileDescriptor((IFileDescriptor**)&fileDescriptor);
        extractor->SetDataSource(fileDescriptor, 0, dataSize);
    // } catch (Exception e) {
    //     e.printStackTrace();
    //     encodedFD.detachFd();
    //     return false;
    // }

    Int32 count;
    extractor->GetTrackCount(&count);
    if (count <= 0) {
        encodedFD->DetachFd();
        return FALSE;
    }

    AutoPtr<IMediaFormat> format;
    extractor->GetTrackFormat(0, (IMediaFormat**)&format);

    // Number of channels specified in the file
    Int32 inputChannelCount;
    format->GetInt32(IMediaFormat::KEY_CHANNEL_COUNT, &inputChannelCount);

    // Number of channels the decoder will provide. (Not
    // necessarily the same as inputChannelCount.  See
    // crbug.com/266006.)
    Int32 outputChannelCount = inputChannelCount;

    Int32 sampleRate;
    format->GetInteger(IMediaFormat::KEY_SAMPLE_RATE, &sampleRate);
    String mime;
    format->GetString(IMediaFormat::KEY_MIME, &mime);

    Int64 durationMicroseconds = 0;
    Boolean containsKey;
    format->ContainsKey(IMediaFormat::KEY_DURATION, &containsKey);
    if (containsKey) {
        // try {
            format->GetInt64(IMediaFormat::KEY_DURATION, &durationMicroseconds);
        // } catch (Exception e) {
        //     Log.d(LOG_TAG, "Cannot get duration");
        // }
    }

    // If the duration is too long, set to 0 to force the caller
    // not to preallocate space.  See crbug.com/326856.
    // FIXME: What should be the limit? We're arbitrarily using
    // about 2148 sec (35.8 min).
    if (durationMicroseconds > 0x7fffffff) {
        durationMicroseconds = 0;
    }

    // Log.d(LOG_TAG, "Initial: Tracks: " + extractor.getTrackCount() +
    //       " Format: " + format);

    // Create decoder
    AutoPtr<IMediaCodec> codec;
    // try {
        AutoPtr<IMediaCodecHelper> mediaCodecHelper;
        CMediaCodecHelper::AcquireSingleton((IMediaCodecHelper**)&mediaCodecHelper);
        mediaCodecHelper->CreateDecoderByType(mime, (IMediaCodec**)&codec);
    // } catch (Exception e) {
    //     Log.w(LOG_TAG, "Failed to create MediaCodec for mime type: " + mime);
    //     encodedFD.detachFd();
    //     return false;
    // }

    codec->Configure(format, NULL /* surface */, NULL /* crypto */, 0 /* flags */);
    codec->Start();

    AutoPtr< ArrayOf<IByteBuffer> > codecInputBuffers;
    codec->GetInputBuffers((ArrayOf<IByteBuffer>**)&codecInputBuffers);
    AutoPtr< ArrayOf<IByteBuffer> > codecOutputBuffers;
    codec->GetOutputBuffers((ArrayOf<IByteBuffer>**)&codecOutputBuffers);

    // A track must be selected and will be used to read samples.
    extractor->SelectTrack(0);

    Boolean sawInputEOS = FALSE;
    Boolean sawOutputEOS = FALSE;
    Boolean destinationInitialized = FALSE;

    // Keep processing until the output is done.
    while (!sawOutputEOS) {
        if (!sawInputEOS) {
            // Input side
            Int32 inputBufIndex;
            codec->DequeueInputBuffer(TIMEOUT_MICROSECONDS, &inputBufIndex);

            if (inputBufIndex >= 0) {
                AutoPtr<IByteBuffer> dstBuf = (*codecInputBuffers)[inputBufIndex];
                Int32 sampleSize;
                extractor->ReadSampleData(dstBuf, 0, &sampleSize);
                Int64 presentationTimeMicroSec = 0;

                if (sampleSize < 0) {
                    sawInputEOS = TRUE;
                    sampleSize = 0;
                }
                else {
                    extractor->GetSampleTime(&presentationTimeMicroSec);
                }

                codec->QueueInputBuffer(inputBufIndex,
                                       0, /* offset */
                                       sampleSize,
                                       presentationTimeMicroSec,
                                       sawInputEOS ? MediaCodec.BUFFER_FLAG_END_OF_STREAM : 0);

                if (!sawInputEOS) {
                    extractor->Advance();
                }
            }
        }

        // Output side
        AutoPtr<IMediaCodecBufferInfo> info;
        CMediaCodecBufferInfo::New((IMediaCodecBufferInfo**)&info);
        Int32 outputBufIndex;
        codec->DequeueOutputBuffer(info, TIMEOUT_MICROSECONDS, &outputBufIndex);

        if (outputBufIndex >= 0) {
            AutoPtr<IByteBuffer> buf = (*codecOutputBuffers)[outputBufIndex];

            if (!destinationInitialized) {
                // Initialize the destination as late as possible to
                // catch any changes in format. But be sure to
                // initialize it BEFORE we send any decoded audio,
                // and only initialize once.
//                Log.d(LOG_TAG, "Final:  Rate: " + sampleRate +
//                      " Channels: " + inputChannelCount +
//                      " Mime: " + mime +
//                      " Duration: " + durationMicroseconds + " microsec");

                NativeInitializeDestination(nativeMediaCodecBridge,
                                            inputChannelCount,
                                            sampleRate,
                                            durationMicroseconds);
                destinationInitialized = TRUE;
            }

            if (destinationInitialized && info.size > 0) {
                NativeOnChunkDecoded(nativeMediaCodecBridge, buf, info.size,
                                     inputChannelCount, outputChannelCount);
            }

            buf->Clear();
            codec->ReleaseOutputBuffer(outputBufIndex, FALSE /* render */);

            if ((info.flags & MediaCodec.BUFFER_FLAG_END_OF_STREAM) != 0) {
                sawOutputEOS = TRUE;
            }
        }
        else if (outputBufIndex == IMediaCodec::INFO_OUTPUT_BUFFERS_CHANGED) {
            codec->GetOutputBuffers((ArrayOf<IByteBuffer>**)&codecOutputBuffers);
        }
        else if (outputBufIndex == IMediaCodec::INFO_OUTPUT_FORMAT_CHANGED) {
            AutoPtr<IMediaFormat> newFormat;
            codec->GetOutputFormat((IMediaFormat**)&newFormat);
            newFormat->GetInt32(IMediaFormat::KEY_CHANNEL_COUNT, &outputChannelCount);
            newFormat->GetInt32(IMediaFormat::KEY_SAMPLE_RATE, &sampleRate);
//            Log.d(LOG_TAG, "output format changed to " + newFormat);
        }
    }

    encodedFD->DetachFd();

    codec->Stop();
    codec->Release();
    codec = NULL;

    return TRUE;
}

void WebAudioMediaCodecBridge::NativeOnChunkDecoded(
    /* [in] */ Int64 nativeWebAudioMediaCodecBridge,
    /* [in] */ IByteBuffer* buf,
    /* [in] */ Int32 size,
    /* [in] */ Int32 inputChannelCount,
    /* [in] */ Int32 outputChannelCount)
{
}

void WebAudioMediaCodecBridge::NativeInitializeDestination(
    /* [in] */ Int64 nativeWebAudioMediaCodecBridge,
    /* [in] */ Int32 inputChannelCount,
    /* [in] */ Int32 sampleRate,
    /* [in] */ Int64 durationMicroseconds)
{
}

} // namespace Media
} // namespace Webkit
} // namespace Droid
} // namespace Elastos
