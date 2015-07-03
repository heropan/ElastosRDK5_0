
#ifndef __AUDIORECORDINPUT_H__
#define __AUDIORECORDINPUT_H__

// import android.annotation.SuppressLint;
// import android.media.AudioFormat;
// import android.media.AudioRecord;
// import android.media.MediaRecorder.AudioSource;
// import android.media.audiofx.AcousticEchoCanceler;
// import android.media.audiofx.AudioEffect;
// import android.media.audiofx.AudioEffect.Descriptor;
// import android.os.Process;
// import android.util.Log;

// import org.chromium.base.CalledByNative;
// import org.chromium.base.JNINamespace;

// import java.nio.ByteBuffer;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Media {

// Owned by its native counterpart declared in audio_record_input.h. Refer to
// that class for general comments.
//@JNINamespace("media")
class AudioRecordInput
{
private:
    class AudioRecordThread
        : public Object
        , public IThread
    {
    public:
        AudioRecordThread(
            /* [in] */ AudioRecordThread* owner);

        //@Override
        CARAPI Run();

        CARAPI_(void) JoinRecordThread();

    private:
        AudioRecordThread* mOwner;

        // The "volatile" synchronization technique is discussed here:
        // http://stackoverflow.com/a/106787/299268
        // and more generally in this article:
        // https://www.ibm.com/developerworks/java/library/j-jtp06197/
        volatile Boolean mKeepAlive;
    };

private:
    AudioRecordInput(
        /* [in] */ Int64 nativeAudioRecordInputStream,
        /* [in] */ Int32 sampleRate,
        /* [in] */ Int32 channels,
        /* [in] */ Int32 bitsPerSample,
        /* [in] */ Int32 bytesPerBuffer,
        /* [in] */ Boolean usePlatformAEC);

    //@CalledByNative
    static CARAPI_(AutoPtr<AudioRecordInput>) CreateAudioRecordInput(
        /* [in] */ Int64 nativeAudioRecordInputStream,
        /* [in] */ Int32 sampleRate,
        /* [in] */ Int32 channels,
        /* [in] */ Int32 bitsPerSample,
        /* [in] */ Int32 bytesPerBuffer,
        /* [in] */ Boolean usePlatformAEC);

    //@SuppressLint("NewApi")
    //@CalledByNative
    CARAPI_(Boolean) Open();

    //@CalledByNative
    CARAPI_(void) Start();

    //@CalledByNative
    CARAPI_(void) Stop();

    //@SuppressLint("NewApi")
    //@CalledByNative
    CARAPI_(void) Close();

    CARAPI_(void) NativeCacheDirectBufferAddress(
        /* [in] */ Int64 nativeAudioRecordInputStream,
        /* [in] */ IByteBuffer* buffer);

    CARAPI_(void) NativeOnData(
        /* [in] */ Int64 nativeAudioRecordInputStream,
        /* [in] */ Int32 size,
        /* [in] */ Int32 hardwareDelayBytes);

private:
    static const String TAG;
    // Set to true to enable debug logs. Always check in as false.
    static const Boolean DEBUG;
    // We are unable to obtain a precise measurement of the hardware delay on
    // Android. This is a conservative lower-bound based on measurments. It
    // could surely be tightened with further testing.
    static const Int32 HARDWARE_DELAY_MS = 100;

    const Int64 mNativeAudioRecordInputStream;
    const Int32 mSampleRate;
    const Int32 mChannels;
    const Int32 mBitsPerSample;
    const Int32 mHardwareDelayBytes;
    const Boolean mUsePlatformAEC;
    AutoPtr<IByteBuffer> mBuffer;
    AutoPtr<AudioRecord> mAudioRecord;
    AutoPtr<AudioRecordThread> mAudioRecordThread;
    AutoPtr<IAcousticEchoCanceler> mAEC;
};

} // namespace Media
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif//__AUDIORECORDINPUT_H__
