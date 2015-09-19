#ifndef __ELASTOS_DROID_SPEECH_TTS_SynthesisPlaybackQueueItem_H__
#define __ELASTOS_DROID_SPEECH_TTS_SynthesisPlaybackQueueItem_H__

#include "speech/tts/PlaybackQueueItem.h"
#include "speech/tts/BlockingAudioTrack.h"
#include "speech/tts/EventLogger.h"
#include <ext/frameworkext.h>
#include <elastos/utility/etl/List.h>

using Elastos::Utility::Etl::List;
namespace Elastos {
namespace Droid {
namespace Speech {
namespace Tts {

/**
 * Manages the playback of a list of byte arrays representing audio data
 * that are queued by the engine to an audio track.
 */
//final
class SynthesisPlaybackQueueItem
    : public PlaybackQueueItem
{
public:
    //static final
    class ListEntry
        : public ElRefBase
    {
    public:
        //final
        CARAPI_(AutoPtr< ArrayOf<Byte> >) mBytes;
    public:
        ListEntry(
            /* [in] */ ArrayOf<Byte>* bytes);
    };

public:
    SynthesisPlaybackQueueItem(
        /* [in] */ Int32 streamType,
        /* [in] */ Int32 sampleRate,
        /* [in] */ Int32 audioFormat,
        /* [in] */ Int32 channelCount,
        /* [in] */ Float volume,
        /* [in] */ Float pan,
        /* [in] */ ITextToSpeechServiceUtteranceProgressDispatcher* dispatcher,
        /* [in] */ IInterface* callerIdentity,
        /* [in] */ EventLogger* logger);

    //@Override
    CARAPI Run();

    //@Override
    CARAPI Stop(
        /* [in] */ Boolean isError);

    CARAPI_(void) Done();

    CARAPI_(void) Put(
        /* [in] */ ArrayOf<Byte>* buffer);// throws InterruptedException

private:
    AutoPtr< ArrayOf<Byte> > Take();// throws InterruptedException

private:
    static const CString TAG;// = "TTS.SynthQueueItem";
    static const Boolean DBG;// = FALSE;

    /**
     * Maximum length of audio we leave unconsumed by the audio track.
     * Calls to {@link #put(byte[])} will block until we have less than
     * this amount of audio left to play back.
     */
    static const Int64 MAX_UNCONSUMED_AUDIO_MS;// = 500;

    /**
     * Guards accesses to mDataBufferList and mUnconsumedBytes.
     */
    //private final Lock mListLock = new ReentrantLock();
    //private final Condition mReadReady = mListLock.newCondition();
    //private final Condition mNotFull = mListLock.newCondition();
    Object mListLock;

    // Guarded by mListLock.
    List< AutoPtr<ListEntry> > mDataBufferList;
    // Guarded by mListLock.
    Int32 mUnconsumedBytes;// = 0;

    /*
     * While mStopped and mIsError can be written from any thread, mDone is written
     * only from the synthesis thread. All three variables are read from the
     * audio playback thread.
     */
    volatile Boolean mStopped;// = FALSE;
    volatile Boolean mDone;// = FALSE;
    volatile Boolean mIsError;// = FALSE;

    AutoPtr<BlockingAudioTrack> mAudioTrack;
    AutoPtr<EventLogger> mLogger;
};

} // namespace Tts
} // namespace Speech
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_SPEECH_TTS_SynthesisPlaybackQueueItem_H__
