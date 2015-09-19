
namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Media {

//===============================================================
//              AudioRecordInput::AudioRecordThread
//===============================================================

AudioRecordInput::AudioRecordThread::AudioRecordThread(
    /* [in] */ AudioRecordThread* owner)
    : mOwner(owner)
    , mKeepAlive(TRUE)
{
}

//@Override
void AudioRecordInput::AudioRecordThread::Run()
{
    Process::SetThreadPriority(IProcess::THREAD_PRIORITY_URGENT_AUDIO);
    // try {
        mAudioRecord->StartRecording();
    // } catch (IllegalStateException e) {
    //     Log.e(TAG, "startRecording failed", e);
    //     return;
    // }

    while (mKeepAlive) {
        Int32 capacity;
        mBuffer->Capacity(&capacity);
        Int32 bytesRead = mAudioRecord->Read(mBuffer, capacity);
        if (bytesRead > 0) {
            NativeOnData(mNativeAudioRecordInputStream, bytesRead,
                         mHardwareDelayBytes);
        }
        else {
//            Log.e(TAG, "read failed: " + bytesRead);
            if (bytesRead == AudioRecord.ERROR_INVALID_OPERATION) {
                // This can happen if there is already an active
                // AudioRecord (e.g. in another tab).
                mKeepAlive = FALSE;
            }
        }
    }

    // try {
        mAudioRecord->Stop();
    // } catch (IllegalStateException e) {
    //     Log.e(TAG, "stop failed", e);
    // }
}

void AudioRecordInput::AudioRecordThread::JoinRecordThread()
{
    mKeepAlive = FALSE;
    while (IsAlive()) {
        // try {
            Join();
        // } catch (InterruptedException e) {
        //     // Ignore.
        // }
    }
}

//===============================================================
//                      AudioRecordInput
//===============================================================

const String AudioRecordInput::TAG("AudioRecordInput");
// Set to true to enable debug logs. Always check in as false.
const Boolean AudioRecordInput::DEBUG = FALSE;
// We are unable to obtain a precise measurement of the hardware delay on
// Android. This is a conservative lower-bound based on measurments. It
// could surely be tightened with further testing.
static const Int32 AudioRecordInput::HARDWARE_DELAY_MS;

AudioRecordInput::AudioRecordInput(
    /* [in] */ Int64 nativeAudioRecordInputStream,
    /* [in] */ Int32 sampleRate,
    /* [in] */ Int32 channels,
    /* [in] */ Int32 bitsPerSample,
    /* [in] */ Int32 bytesPerBuffer,
    /* [in] */ Boolean usePlatformAEC)
    : mNativeAudioRecordInputStream(nativeAudioRecordInputStream)
    , mSampleRate(sampleRate)
    , mChannels(channels)
    , mBitsPerSample(bitsPerSample)
    , mHardwareDelayBytes(HARDWARE_DELAY_MS * sampleRate / 1000 * bitsPerSample / 8)
    , mUsePlatformAEC(usePlatformAEC)
{
    // We use a direct buffer so that the native class can have access to
    // the underlying memory address. This avoids the need to copy from a
    // jbyteArray to native memory. More discussion of this here:
    // http://developer.android.com/training/articles/perf-jni.html
    ByteBuffer::AllocateDirect(bytesPerBuffer, (IByteBuffer**)&mBuffer);

    // Rather than passing the ByteBuffer with every OnData call (requiring
    // the potentially expensive GetDirectBufferAddress) we simply have the
    // the native class cache the address to the memory once.
    //
    // Unfortunately, profiling with traceview was unable to either confirm
    // or deny the advantage of this approach, as the values for
    // nativeOnData() were not stable across runs.
    NativeCacheDirectBufferAddress(mNativeAudioRecordInputStream, mBuffer);
}

//@CalledByNative
AutoPtr<AudioRecordInput> AudioRecordInput::CreateAudioRecordInput(
    /* [in] */ Int64 nativeAudioRecordInputStream,
    /* [in] */ Int32 sampleRate,
    /* [in] */ Int32 channels,
    /* [in] */ Int32 bitsPerSample,
    /* [in] */ Int32 bytesPerBuffer,
    /* [in] */ Boolean usePlatformAEC)
{
    AutoPtr<AudioRecordInput> ret = new AudioRecordInput(nativeAudioRecordInputStream, sampleRate, channels,
                                bitsPerSample, bytesPerBuffer, usePlatformAEC);
    return ret;
}

//@SuppressLint("NewApi")
//@CalledByNative
Boolean AudioRecordInput::Open()
{
    if (mAudioRecord != NULL) {
//        Log.e(TAG, "open() called twice without a close()");
        return FALSE;
    }

    Int32 channelConfig;
    if (mChannels == 1) {
        channelConfig = IAudioFormat::CHANNEL_IN_MONO;
    }
    else if (mChannels == 2) {
        channelConfig = IAudioFormat::CHANNEL_IN_STEREO;
    }
    else {
//        Log.e(TAG, "Unsupported number of channels: " + mChannels);
        return FALSE;
    }

    Int32 audioFormat;
    if (mBitsPerSample == 8) {
        audioFormat = IAudioFormat::ENCODING_PCM_8BIT;
    }
    else if (mBitsPerSample == 16) {
        audioFormat = IAudioFormat::ENCODING_PCM_16BIT;
    }
    else {
//        Log.e(TAG, "Unsupported bits per sample: " + mBitsPerSample);
        return FALSE;
    }

    // TODO(ajm): Do we need to make this larger to avoid underruns? The
    // Android documentation notes "this size doesn't guarantee a smooth
    // recording under load".
    AutoPtr<IAudioRecordHelper> arHelper;
    CAudioRecordHelper::AcquireSingleton((IAudioRecordHelper**)&arHelper);
    Int32 minBufferSize;
    arHelper->GetMinBufferSize(mSampleRate, channelConfig, audioFormat, &minBufferSize);
    if (minBufferSize < 0) {
//        Log.e(TAG, "getMinBufferSize error: " + minBufferSize);
        return FALSE;
    }

    // We will request mBuffer.capacity() with every read call. The
    // underlying AudioRecord buffer should be at least this large.
    Int32 capacity;
    mBuffer->Capacity(&capacity);
    Int32 audioRecordBufferSizeInBytes = Math::Max(capacity, minBufferSize);
    // try {
        // TODO(ajm): Allow other AudioSource types to be requested?
        CAudioRecord::(IAudioSource::VOICE_COMMUNICATION,
                                       mSampleRate,
                                       channelConfig,
                                       audioFormat,
                                       audioRecordBufferSizeInBytes,
                                       (IAudioRecord**)&mAudioRecord);
    // } catch (IllegalArgumentException e) {
    //     Log.e(TAG, "AudioRecord failed", e);
    //     return false;
    // }

    AutoPtr<IAcousticEchoCancelerHelper> aecHelper;
    CAcousticEchoCancelerHelper::AcquireSingleton((IAcousticEchoCancelerHelper**)&aecHelper);
    Boolean isAvailable = FALSE;
    aecHelper->IsAvailable(&isAvailable);
    if (isAvailable) {
        Int32 id;
        mAudioRecord->GetAudioSessionId(&id);
        aecHelper->Create(id, (IAcousticEchoCanceler**)&mAEC);
        if (mAEC == NULL) {
//            Log.e(TAG, "AcousticEchoCanceler.create failed");
            return FALSE;
        }

        Int32 ret;
        mAEC->SetEnabled(mUsePlatformAEC, &ret);
        if (ret != IAudioEffect::SUCCESS) {
//            Log.e(TAG, "setEnabled error: " + ret);
            return FALSE;
        }

        if (DEBUG) {
            Descriptor descriptor = mAEC.getDescriptor();
            Log.d(TAG, "AcousticEchoCanceler " +
                    "name: " + descriptor.name + ", " +
                    "implementor: " + descriptor.implementor + ", " +
                    "uuid: " + descriptor.uuid);
        }
    }

    return TRUE;
}

//@CalledByNative
void AudioRecordInput::Start()
{
    if (mAudioRecord == NULL) {
//        Log.e(TAG, "start() called before open().");
        return;
    }

    if (mAudioRecordThread != NULL) {
        // start() was already called.
        return;
    }

    mAudioRecordThread = new AudioRecordThread();
    mAudioRecordThread->Start();
}

//@CalledByNative
void AudioRecordInput::Stop()
{
    if (mAudioRecordThread == NULL) {
        // start() was never called, or stop() was already called.
        return;
    }

    mAudioRecordThread->JoinRecordThread();
    mAudioRecordThread = NULL;
}

//@SuppressLint("NewApi")
//@CalledByNative
void AudioRecordInput::Close()
{
    if (mAudioRecordThread != NULL) {
//        Log.e(TAG, "close() called before stop().");
        return;
    }

    if (mAudioRecord == NULL) {
        // open() was not called.
        return;
    }

    if (mAEC != NULL) {
        mAEC->Release();
        mAEC = NULL;
    }

    mAudioRecord->Release();
    mAudioRecord = NULL;
}

void AudioRecordInput::NativeCacheDirectBufferAddress(
    /* [in] */ Int64 nativeAudioRecordInputStream,
    /* [in] */ IByteBuffer* buffer)
{
}

void AudioRecordInput::NativeOnData(
    /* [in] */ Int64 nativeAudioRecordInputStream,
    /* [in] */ Int32 size,
    /* [in] */ Int32 hardwareDelayBytes)
{
}

} // namespace Media
} // namespace Webkit
} // namespace Droid
} // namespace Elastos