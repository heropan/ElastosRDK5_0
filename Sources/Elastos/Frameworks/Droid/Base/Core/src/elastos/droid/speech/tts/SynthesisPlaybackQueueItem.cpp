#include "elastos/droid/speech/tts/SynthesisPlaybackQueueItem.h"
#include <elastos/utility/logging/Logger.h>

using Elastos::Utility::Logging::Logger;

namespace Elastos {
namespace Droid {
namespace Speech {
namespace Tts {

/******************************SynthesisPlaybackQueueItem::ListEntry*************************/
SynthesisPlaybackQueueItem::ListEntry::ListEntry(
    /* [in] */ ArrayOf<Byte>* bytes)
{
    mBytes = bytes;
}

/******************************SynthesisPlaybackQueueItem*************************/
const CString SynthesisPlaybackQueueItem::TAG = "TTS.SynthQueueItem";
const Boolean SynthesisPlaybackQueueItem::DBG = FALSE;

const Int64 SynthesisPlaybackQueueItem::MAX_UNCONSUMED_AUDIO_MS = 500;


SynthesisPlaybackQueueItem::SynthesisPlaybackQueueItem(
    /* [in] */ Int32 streamType,
    /* [in] */ Int32 sampleRate,
    /* [in] */ Int32 audioFormat,
    /* [in] */ Int32 channelCount,
    /* [in] */ Float volume,
    /* [in] */ Float pan,
    /* [in] */ ITextToSpeechServiceUtteranceProgressDispatcher* dispatcher,
    /* [in] */ IInterface* callerIdentity,
    /* [in] */ EventLogger* logger):PlaybackQueueItem(dispatcher, callerIdentity)
{
    mUnconsumedBytes = 0;

    mStopped = FALSE;
    mDone = FALSE;
    mIsError = FALSE;

    mAudioTrack = new BlockingAudioTrack(streamType, sampleRate, audioFormat, channelCount, volume, pan);
    mLogger = logger;
}

ECode SynthesisPlaybackQueueItem::Run()
{
    const AutoPtr<ITextToSpeechServiceUtteranceProgressDispatcher> dispatcher = GetDispatcher();
    dispatcher->DispatchOnStart();

    if (!mAudioTrack->Init()) {
        dispatcher->DispatchOnError();
        return NOERROR;
    }

    //try {
        AutoPtr< ArrayOf<Byte> > buffer = NULL;

        // take() will block until:
        //
        // (a) there is a buffer available to tread. In which case
        // a non null value is returned.
        // OR (b) stop() is called in which case it will return null.
        // OR (c) done() is called in which case it will return null.
        while ( (buffer = Take(), buffer) != NULL) {
            mAudioTrack->Write(buffer);
            mLogger->OnAudioDataWritten();
        }

    //} catch (InterruptedException ie) {
        /*
        if (DBG){
            //Java:    Log.d(TAG, "Interrupted waiting for buffers, cleaning up.");
            Logger::D(TAG, String("Interrupted waiting for buffers, cleaning up.\n"));
        }
        */
    //}

    mAudioTrack->WaitAndRelease();

    if (mIsError) {
        dispatcher->DispatchOnError();
    } else {
        dispatcher->DispatchOnDone();
    }

    mLogger->OnWriteData();
    return NOERROR;
}

ECode SynthesisPlaybackQueueItem::Stop(
    /* [in] */ Boolean isError)
{
    //try {
        //Java:    mListLock.lock();
        {
            AutoLock lock(mListLock);
            // Update our internal state.
            mStopped = TRUE;
            mIsError = isError;

            // Wake up the audio playback thread if it was waiting on take().
            // take() will return null since mStopped was true, and will then
            // break out of the data write loop.
            //Java:    mReadReady.signal();

            // Wake up the synthesis thread if it was waiting on put(). Its
            // buffers will no longer be copied since mStopped is true. The
            // PlaybackSynthesisCallback that this synthesis corresponds to
            // would also have been stopped, and so all calls to
            // Callback.onDataAvailable( ) will return errors too.
            //Java:    mNotFull.signal();
        }
    //} finally {
        //Java:    mListLock.unlock();
    //}

    // Stop the underlying audio track. This will stop sending
    // data to the mixer and discard any pending buffers that the
    // track holds.
    mAudioTrack->Stop();
    return NOERROR;
}

void SynthesisPlaybackQueueItem::Done()
{
    //try {
        //mListLock.lock();
        AutoLock lock(mListLock);

        // Update state.
        mDone = TRUE;

        // Unblocks the audio playback thread if it was waiting on take()
        // after having consumed all available buffers. It will then return
        // null and leave the write loop.
        //Java:    mReadReady.signal();

        // Just so that engines that try to queue buffers after
        // calling done() don't block the synthesis thread forever. Ideally
        // this should be called from the same thread as put() is, and hence
        // this call should be pointless.
        //Java:    mNotFull.signal();

    //} finally {
        //mListLock.unlock();
    //}
}

void SynthesisPlaybackQueueItem::Put(
    /* [in] */ ArrayOf<Byte>* buffer)// throws InterruptedException
{
    //try {
        //Java:    mListLock.lock();
        AutoLock lock(mListLock);
        Int64 unconsumedAudioMs = 0;

        while ((unconsumedAudioMs = mAudioTrack->GetAudioLengthMs(mUnconsumedBytes)) >
                MAX_UNCONSUMED_AUDIO_MS && !mStopped) {
            //Java:    mNotFull.await();
        }

        // Don't bother queueing the buffer if we've stopped. The playback thread
        // would have woken up when stop() is called (if it was blocked) and will
        // proceed to leave the write loop since take() will return null when
        // stopped.
        if (mStopped) {
            return;
        }

        AutoPtr<ListEntry> listEntryT = new ListEntry(buffer);

        mDataBufferList.PushBack(listEntryT);
        mUnconsumedBytes += buffer->GetLength();
        //Java:    mReadReady.signal();
    //} finally {
        //Java:    mListLock.unlock();
    //}
}

AutoPtr< ArrayOf<Byte> > SynthesisPlaybackQueueItem::Take()
{
    //try {
        //Java:    mListLock.lock();
        AutoLock lock(mListLock);

        // Block if there are no available buffers, and stop() has not
        // been called and done() has not been called.
        while (mDataBufferList.IsEmpty() && !mStopped && !mDone) {
            //Java:    mReadReady.await();
        }

        // If stopped, return null so that we can exit the playback loop
        // as soon as possible.
        if (mStopped) {
            return NULL;
        }

            // Remove the first entry from the queue.
            AutoPtr<ListEntry> entry = mDataBufferList.GetFront();
            mDataBufferList.Remove(0);

            // This is the normal playback loop exit case, when done() was
            // called. (mDone will be true at this point).
            if (entry == NULL) {
                return NULL;
            }

            mUnconsumedBytes -= (entry->mBytes)->GetLength();
            // Unblock the waiting writer. We use signal() and not signalAll()
            // because there will only be one thread waiting on this (the
            // Synthesis thread).
            //Java:    mNotFull.signal();

            return entry->mBytes;
    //} finally {
        //Java:    mListLock.unlock();
    //}
}

} // namespace Tts
} // namespace Speech
} // namepsace Droid
} // namespace Elastos
