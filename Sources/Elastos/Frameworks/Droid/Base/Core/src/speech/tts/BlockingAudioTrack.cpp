#include "speech/tts/BlockingAudioTrack.h"
#include "media/media/CAudioTrack.h"
#include <elastos/core/Math.h>
#include <elastos/utility/logging/Logger.h>
#include <unistd.h> //for Method: unsigned sleep(unsigned)
#include <elastos/core/StringUtils.h>

using Elastos::Core::StringUtils;
using Elastos::Utility::Logging::Logger;
using Elastos::Droid::Media::CAudioTrack;
using Elastos::Droid::Media::IAudioFormat;

namespace Elastos {
namespace Droid {
namespace Speech {
namespace Tts {

const CString BlockingAudioTrack::TAG = "TTS.BlockingAudioTrack";
const Boolean DBG = FALSE;

const Int64 BlockingAudioTrack::MIN_SLEEP_TIME_MS = 20;

const Int64 BlockingAudioTrack::MAX_SLEEP_TIME_MS = 2500;

const Int64 BlockingAudioTrack::MAX_PROGRESS_WAIT_MS = MAX_SLEEP_TIME_MS;

const Int32 BlockingAudioTrack::MIN_AUDIO_BUFFER_SIZE = 8192;


BlockingAudioTrack::BlockingAudioTrack(
    /* [in] */ Int32 streamType,
    /* [in] */ Int32 sampleRate,
    /* [in] */ Int32 audioFormat,
    /* [in] */ Int32 channelCount,
    /* [in] */ Float volume,
    /* [in] */ Float pan)
{
    mStreamType = streamType;
    mSampleRateInHz = sampleRate;
    mAudioFormat = audioFormat;
    mChannelCount = channelCount;
    mVolume = volume;
    mPan = pan;

    mBytesPerFrame = GetBytesPerFrame(mAudioFormat) * mChannelCount;
    mIsShortUtterance = FALSE;
    mAudioBufferSize = 0;
    mBytesWritten = 0;

    mAudioTrack = NULL;
    mStopped = FALSE;
}

Boolean BlockingAudioTrack::Init()
{
    AutoPtr<IAudioTrack> track = CreateStreamingAudioTrack();
    {
        AutoLock lock(mAudioTrackLock);
        mAudioTrack = track;
    }

    if (track == NULL) {
        return FALSE;
    } else {
        return TRUE;
    }
}

void BlockingAudioTrack::Stop()
{
    AutoLock lock(mAudioTrackLock);
    if (mAudioTrack != NULL) {
        mAudioTrack->Stop();
    }
    mStopped = TRUE;
}

Int32 BlockingAudioTrack::Write(
    /* [in] */ ArrayOf<Byte>* data)
{
    AutoPtr<IAudioTrack> track = NULL;
    {
        AutoLock lock(mAudioTrackLock);
        track = mAudioTrack;
    }

    if (track == NULL || mStopped) {
        return -1;
    }
    const Int32 bytesWritten = WriteToAudioTrack(track, data);

    mBytesWritten += bytesWritten;
    return bytesWritten;
}

void BlockingAudioTrack::WaitAndRelease()
{
    AutoPtr<IAudioTrack> track = NULL;
    {
        AutoLock lock(mAudioTrackLock);
        track = mAudioTrack;
    }
    if (track == NULL) {
        if (DBG){
            //Java:    Log.d(TAG, "Audio track null [duplicate call to waitAndRelease ?]");
            Logger::D(TAG, String("Audio track null [duplicate call to waitAndRelease ?]\n"));
            return;
        }

    }

    // For "small" audio tracks, we have to stop() them to make them mixable,
    // else the audio subsystem will wait indefinitely for us to fill the buffer
    // before rendering the track mixable.
    //
    // If mStopped is true, the track would already have been stopped, so not
    // much point not doing that again.
    if (mBytesWritten < mAudioBufferSize && !mStopped) {
        if (DBG) {
            //Java:    Log.d(TAG, "Stopping audio track to flush audio, state was : " + track.getPlayState() + ",stopped= " + mStopped);
            Int32 playState;
            String strOut = String("Stopping audio track to flush audio, state was : ") + StringUtils::Int32ToString((track->GetPlayState(&playState), playState)) + String(",stopped= ") + StringUtils::BooleanToString(mStopped) + String("\n");
            Logger::D(TAG, strOut);
        }

        mIsShortUtterance = TRUE;
        track->Stop();
    }

    // Block until the audio track is done only if we haven't stopped yet.
    if (!mStopped) {
        if (DBG){
            //Java:    Log.d(TAG, "Waiting for audio track to complete : " + mAudioTrack.hashCode());
            String strOut = String("Waiting for audio track to complete : ") + String("\n");
            Logger::D(TAG, strOut);
        }
        BlockUntilDone(mAudioTrack);
    }

    // The last call to AudioTrack.write( ) will return only after
    // all data from the audioTrack has been sent to the mixer, so
    // it's safe to release at this point.
    if (DBG){
        //Java:    Log.d(TAG, "Releasing audio track [" + track.hashCode() + "]");
        String strOut = String("Releasing audio track [") + String("]\n");
        Logger::D(TAG, strOut);
    }
    {
        AutoLock lock(mAudioTrackLock);
        mAudioTrack = NULL;
    }
    track->Release();
}


Int32 BlockingAudioTrack::GetChannelConfig(
    /* [in] */ Int32 channelCount)
{
    if (channelCount == 1) {
        return IAudioFormat::CHANNEL_OUT_MONO;
        return 0;
    } else if (channelCount == 2){
        return IAudioFormat::CHANNEL_OUT_STEREO;
        return 0;
    }

    return 0;
}

Int64 BlockingAudioTrack::GetAudioLengthMs(
    /* [in] */ Int32 numBytes)
{
    Int32 unconsumedFrames = numBytes / mBytesPerFrame;
    Int64 estimatedTimeMs = unconsumedFrames * 1000 / mSampleRateInHz;

    return estimatedTimeMs;
}

Int32 BlockingAudioTrack::WriteToAudioTrack(
    /* [in] */ IAudioTrack* audioTrack,
    /* [in] */ ArrayOf<Byte>* bytes)
{
    Int32 playState;
    if ((audioTrack->GetPlayState(&playState), playState) != IAudioTrack::PLAYSTATE_PLAYING) {
        if (DBG){
            //Java:    Log.d(TAG, "AudioTrack not playing, restarting : " + audioTrack.hashCode());
            String strOut = String("AudioTrack not playing, restarting : ") + String("\n");
            Logger::D(TAG, strOut);
        }
        audioTrack->Play();
    }

    Int32 count = 0;
    while (count < bytes->GetLength()) {
        // Note that we don't take bufferCopy.mOffset into account because
        // it is guaranteed to be 0.
        Int32 written;
        audioTrack->Write(bytes, count, bytes->GetLength(), &written);
        if (written <= 0) {
            break;
        }
        count += written;
    }
    return count;
}

AutoPtr<IAudioTrack> BlockingAudioTrack::CreateStreamingAudioTrack()
{
    const Int32 channelConfig = GetChannelConfig(mChannelCount);

    Int32 minBufferSizeInBytes;
    CAudioTrack::GetMinBufferSize(mSampleRateInHz, channelConfig, mAudioFormat, &minBufferSizeInBytes);
    Int32 bufferSizeInBytes = Elastos::Core::Math::Max(MIN_AUDIO_BUFFER_SIZE, minBufferSizeInBytes);

    AutoPtr<IAudioTrack> audioTrack;
    CAudioTrack::New(mStreamType, mSampleRateInHz, channelConfig, mAudioFormat, bufferSizeInBytes, IAudioTrack::MODE_STREAM, (IAudioTrack**)&audioTrack);
    Int32 atState;
    if ((audioTrack->GetState(&atState), atState) != IAudioTrack::STATE_INITIALIZED) {
        //Java:    Log.w(TAG, "Unable to create audio track.");
        Logger::W(TAG, String("Unable to create audio track.\n"));
        audioTrack->Release();
        return NULL;
    }

    mAudioBufferSize = bufferSizeInBytes;

    SetupVolume(audioTrack, mVolume, mPan);
    return audioTrack;
}

Int32 BlockingAudioTrack::GetBytesPerFrame(
    /* [in] */ Int32 audioFormat)
{
    if (audioFormat == IAudioFormat::ENCODING_PCM_8BIT) {
        return 1;
    } else if (audioFormat == IAudioFormat::ENCODING_PCM_16BIT) {
        return 2;
    }

    return -1;
}


void BlockingAudioTrack::BlockUntilDone(
    /* [in] */ IAudioTrack* audioTrack)
{
    if (mBytesWritten <= 0) {
        return;
    }

    if (mIsShortUtterance) {
        // In this case we would have called AudioTrack#stop() to flush
        // buffers to the mixer. This makes the playback head position
        // unobservable and notification markers do not work reliably. We
        // have no option but to wait until we think the track would finish
        // playing and release it after.
        //
        // This isn't as bad as it looks because (a) We won't end up waiting
        // for much longer than we should because even at 4khz mono, a short
        // utterance weighs in at about 2 seconds, and (b) such short utterances
        // are expected to be relatively infrequent and in a stream of utterances
        // this shows up as a slightly longer pause.
        BlockUntilEstimatedCompletion();
    } else {
        BlockUntilCompletion(audioTrack);
    }
}

void BlockingAudioTrack::BlockUntilEstimatedCompletion()
{
    const Int32 lengthInFrames = mBytesWritten / mBytesPerFrame;
    const Int64 estimatedTimeMs = (lengthInFrames * 1000 / mSampleRateInHz);

    if (DBG) {
        //Java:    Log.d(TAG, "About to sleep for: " + estimatedTimeMs + "ms for a short utterance");
        String strOut = String("About to sleep for: ") + StringUtils::Int64ToString(estimatedTimeMs) + String("ms for a short utterance\n");
        Logger::D(TAG, strOut);
    }

    //try {
        //Thread.sleep(estimatedTimeMs);
        sleep(estimatedTimeMs);
    //} catch (InterruptedException ie) {
        // Do nothing.
    //}
}

void BlockingAudioTrack::BlockUntilCompletion(
    /* [in] */ IAudioTrack* audioTrack
    )
{
    const Int32 lengthInFrames = mBytesWritten / mBytesPerFrame;

    Int32 previousPosition = -1;
    Int32 currentPosition = 0;
    Int64 blockedTimeMs = 0;

    Int32 playBackHeadPosition;
    Int32 playState;
    while ((currentPosition = (audioTrack->GetPlaybackHeadPosition(&playBackHeadPosition), playBackHeadPosition)) < lengthInFrames &&
                (audioTrack->GetPlayState(&playState), playState) == /*IAudioTrack::PLAYSTATE_PLAYING*/3 && !mStopped) {
        Int32 sampleRate;
        audioTrack->GetSampleRate(&sampleRate);
        const Int64 estimatedTimeMs = ((lengthInFrames - currentPosition) * 1000) / sampleRate;
        const Int64 sleepTimeMs = Clip(estimatedTimeMs, MIN_SLEEP_TIME_MS, MAX_SLEEP_TIME_MS);

        // Check if the audio track has made progress since the last loop
        // iteration. We should then add in the amount of time that was
        // spent sleeping in the last iteration.
        if (currentPosition == previousPosition) {
            // This works only because the sleep time that would have been calculated
            // would be the same in the previous iteration too.
            blockedTimeMs += sleepTimeMs;
            // If we've taken too long to make progress, bail.
            if (blockedTimeMs > MAX_PROGRESS_WAIT_MS) {
                //Java:    Log.w(TAG, "Waited unsuccessfully for " + MAX_PROGRESS_WAIT_MS + "ms " + "for AudioTrack to make progress, Aborting");
                String strOut = String("Waited unsuccessfully for ") + StringUtils::Int64ToString(MAX_PROGRESS_WAIT_MS) + String("ms for AudioTrack to make progress, Aborting\n");
                Logger::D(TAG, strOut);
                break;
            }
        }
        else {
            blockedTimeMs = 0;
        }
        previousPosition = currentPosition;

        if (DBG) {
            //Java:    Log.d(TAG, "About to sleep for : " + sleepTimeMs + " ms," + " Playback position : " + currentPosition + ", Length in frames : " + lengthInFrames);
            String strOut = String("About to sleep for : ") + StringUtils::Int64ToString(sleepTimeMs) + String("ms, Playback position : ") + StringUtils::Int32ToString(currentPosition) + String(", Length in frames : ") + StringUtils::Int32ToString(lengthInFrames) + String("\n");
            Logger::D(TAG, strOut);
        }
        //try {
            sleep(sleepTimeMs);
        //} catch (InterruptedException ie) {
            //break;
        //}
    }
}

void BlockingAudioTrack::SetupVolume(
    /* [in] */ IAudioTrack* audioTrack,
    /* [in] */ Float volume,
    /* [in] */ Float pan)
{
    const Float vol = Clip(volume, 0.0f, 1.0f);
    const Float panning = Clip(pan, -1.0f, 1.0f);

    Float volLeft = vol;
    Float volRight = vol;
    if (panning > 0.0f) {
        volLeft *= (1.0f - panning);
    }
    else if (panning < 0.0f) {
        volRight *= (1.0f + panning);
    }
    if (DBG){
        //Java:    Log.d(TAG, "volLeft=" + volLeft + ",volRight=" + volRight);
        String strOut = String("volLeft=") + StringUtils::DoubleToString(volLeft) + String(",volRight=") + StringUtils::DoubleToString(volRight) + String("\n");
        Logger::D(TAG, strOut);
    }
    Int32 ssvState;
    if ((audioTrack->SetStereoVolume(volLeft, volRight, &ssvState), ssvState) != IAudioTrack::/*SUCCESS*/IAudioTrack::SUCCESS) {
        //Java:    Log.e(TAG, "Failed to set volume");
        Logger::E(TAG, String("Failed to set volume\n"));
    }
}

Int64 BlockingAudioTrack::Clip(
    /* [in] */ Int64 value,
    /* [in] */ Int64 min,
    /* [in] */ Int64 max)
{
    if (value < min) {
        return min;
    }

    if (value > max) {
        return max;
    }

    return value;
}

Float BlockingAudioTrack::Clip(
    /* [in] */ Float value,
    /* [in] */ Float min,
    /* [in] */ Float max)
{
    return value > max ? max : (value < min ? min : value);
}

} // namespace Tts
} // namespace Speech
} // namepsace Droid
} // namespace Elastos
