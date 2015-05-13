#ifndef __BlockingAudioTrack_H__
#define __BlockingAudioTrack_H__

#include "Elastos.Droid.Core_server.h"
#include <elastos/Mutex.h>

using Elastos::Core::Threading::Mutex;
using Elastos::Droid::Media::IAudioTrack;

namespace Elastos {
namespace Droid {
namespace Speech {
namespace Tts {

/**
 * Exposes parts of the {@link AudioTrack} API by delegating calls to an
 * underlying {@link AudioTrack}. Additionally, provides methods like
 * {@link #waitAndRelease()} that will block until all audiotrack
 * data has been flushed to the mixer, and is estimated to have completed
 * playback.
 */
class BlockingAudioTrack
    : public ElRefBase
{
public:
    BlockingAudioTrack(
        /* [in] */ Int32 streamType,
        /* [in] */ Int32 sampleRate,
        /* [in] */ Int32 audioFormat,
        /* [in] */ Int32 channelCount,
        /* [in] */ Float volume,
        /* [in] */ Float pan);

    CARAPI_(Boolean) Init();

    CARAPI_(void) Stop();

    CARAPI_(Int32) Write(
        /* [in] */ ArrayOf<Byte>* data);

    CARAPI_(void) WaitAndRelease();

    static CARAPI_(Int32) GetChannelConfig(
        /* [in] */ Int32 channelCount);

    CARAPI_(Int64) GetAudioLengthMs(
        /* [in] */ Int32 numBytes);

private:
    static CARAPI_(Int32) WriteToAudioTrack(
        /* [in] */ IAudioTrack* audioTrack,
        /* [in] */ ArrayOf<Byte>* bytes);

    CARAPI_(AutoPtr<IAudioTrack>) CreateStreamingAudioTrack();

    static CARAPI_(Int32) GetBytesPerFrame(
        /* [in] */ Int32 audioFormat);

    CARAPI_(void) BlockUntilDone(
        /* [in] */ IAudioTrack* audioTrack
        );

    CARAPI_(void) BlockUntilEstimatedCompletion();

    CARAPI_(void) BlockUntilCompletion(
        /* [in] */ IAudioTrack* audioTrack
        );

    static CARAPI_(void) SetupVolume(
        /* [in] */ IAudioTrack* audioTrack,
        /* [in] */ Float volume,
        /* [in] */ Float pan);

    static CARAPI_(Int64) Clip(
        /* [in] */ Int64 value,
        /* [in] */ Int64 min,
        /* [in] */ Int64 max);

    static CARAPI_(Float) Clip(
        /* [in] */ Float value,
        /* [in] */ Float min,
        /* [in] */ Float max);

private:
    static const CString TAG;// = "TTS.BlockingAudioTrack";
    static const Boolean DBG;// = FALSE;


    /**
     * The minimum increment of time to wait for an AudioTrack to finish
     * playing.
     */
    static const Int64 MIN_SLEEP_TIME_MS;// = 20;

    /**
     * The maximum increment of time to sleep while waiting for an AudioTrack
     * to finish playing.
     */
    static const Int64 MAX_SLEEP_TIME_MS;// = 2500;

    /**
     * The maximum amount of time to wait for an audio track to make progress while
     * it remains in PLAYSTATE_PLAYING. This should never happen in normal usage, but
     * could happen in exceptional circumstances like a media_server crash.
     */
    static const Int64 MAX_PROGRESS_WAIT_MS;// = MAX_SLEEP_TIME_MS;

    /**
     * Minimum size of the buffer of the underlying {@link android.media.AudioTrack}
     * we create.
     */
    static const Int32 MIN_AUDIO_BUFFER_SIZE;// = 8192;


    Int32 mStreamType;// = 0;
    Int32 mSampleRateInHz;// = 0;
    Int32 mAudioFormat;// = 0;
    Int32 mChannelCount;// = 0;
    Float mVolume;// = 0.0;
    Float mPan;// = 0.0;

    Int32 mBytesPerFrame;// = 0;
    /**
     * A "short utterance" is one that uses less bytes than the audio
     * track buffer size (mAudioBufferSize). In this case, we need to call
     * {@link AudioTrack#stop()} to send pending buffers to the mixer, and slightly
     * different logic is required to wait for the track to finish.
     *
     * Not volatile, accessed only from the audio playback thread.
     */
    Boolean mIsShortUtterance;// = FALSE;
    /**
     * Will be valid after a call to {@link #init()}.
     */
    Int32 mAudioBufferSize;// = 0;
    Int32 mBytesWritten;// = 0;

    // Need to be seen by stop() which can be called from another thread. mAudioTrack will be
    // set to null only after waitAndRelease().
    Mutex mAudioTrackLock;
    AutoPtr<IAudioTrack> mAudioTrack;
    /*volatile*/ Boolean mStopped;// = FALSE;

};

} // namespace Tts
} // namespace Speech
} // namepsace Droid
} // namespace Elastos

#endif // __BlockingAudioTrack_H__