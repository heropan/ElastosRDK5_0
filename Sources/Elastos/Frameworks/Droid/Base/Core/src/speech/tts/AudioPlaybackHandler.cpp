#include "speech/tts/AudioPlaybackHandler.h"
#include <elastos/utility/logging/Logger.h>
#include <Elastos.CoreLibrary.h>
#include "ext/frameworkext.h"

using Elastos::Core::EIID_IRunnable;
using Elastos::Core::CThread;
using Elastos::Utility::Logging::Logger;

namespace Elastos {
namespace Droid {
namespace Speech {
namespace Tts {

const CString AudioPlaybackHandler::TAG = "TTS.AudioPlaybackHandler";
const Boolean AudioPlaybackHandler::DBG = FALSE;

/******************************AudioPlaybackHandler::AudioPlaybackHandlerMessageLoop*************************/
PInterface AudioPlaybackHandler::AudioPlaybackHandlerMessageLoop::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)(IRunnable*)this;
    }
    else if (riid == EIID_IRunnable) {
        return (IRunnable*)this;
    }
    return NULL;
}

UInt32 AudioPlaybackHandler::AudioPlaybackHandlerMessageLoop::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 AudioPlaybackHandler::AudioPlaybackHandlerMessageLoop::Release()
{
    return ElRefBase::Release();
}

ECode AudioPlaybackHandler::AudioPlaybackHandlerMessageLoop::GetInterfaceID(
    /* [in] */ IInterface* Object,
    /* [out] */ InterfaceID* iID)
{
    VALIDATE_NOT_NULL(iID);
    if (iID == NULL) {
        return E_INVALID_ARGUMENT;
    }

    if (Object == (IInterface*)(IRunnable*)this) {
        *iID = EIID_IRunnable;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

ECode AudioPlaybackHandler::AudioPlaybackHandlerMessageLoop::Run()
{
    while (TRUE) {
        AutoPtr<PlaybackQueueItem> item = NULL;
            //try {
                item = (mAph->mQueue).GetFront();
                //(mAph->mQueue).Pop();
                (mAph->mQueue).PopFront();
            //} catch (InterruptedException ie) {
                /*
                if (DBG){
                    //Java:    Log.d(TAG, "MessageLoop : Shutting down (interrupted)");
                    Logger::D(TAG, String("MessageLoop : Shutting down (interrupted)\n"));
                }
                return E_INTERRUPTED_EXCEPTION;
                */
            //}

            // If stop() or stopForApp() are called between mQueue.take()
            // returning and mCurrentWorkItem being set, the current work item
            // will be run anyway.

            mAph->mCurrentWorkItem = item;
            item->Run();
            mAph->mCurrentWorkItem = NULL;
    }
    return NOERROR;
}

AudioPlaybackHandler::AudioPlaybackHandlerMessageLoop::AudioPlaybackHandlerMessageLoop(
    /* [in] */ AudioPlaybackHandler* aph)
{
    mAph = aph;
}

/******************************AudioPlaybackHandler*************************/

AudioPlaybackHandler::AudioPlaybackHandler()
{
    //Java:    mHandlerThread = new Thread(new MessageLoop(), "TTS.AudioPlaybackThread");
    AutoPtr<AudioPlaybackHandlerMessageLoop> aphMl = new AudioPlaybackHandlerMessageLoop(this);
    CThread::New( aphMl, String("TTS.AudioPlaybackThread"), (IThread**)&mHandlerThread);
}

void AudioPlaybackHandler::Start()
{
    mHandlerThread->Start();
}

void AudioPlaybackHandler::Stop(
    /* [in] */ PlaybackQueueItem* item)
{
    if (item == NULL) {
        return;
    }

    item->Stop(FALSE);
}

void AudioPlaybackHandler::Enqueue(
    /* [in] */ PlaybackQueueItem* item)
{
    //try {
        //mQueue.Push(item);
        mQueue.PushBack(item);
    //} catch (InterruptedException ie) {
        // This exception will never be thrown, since we allow our queue
        // to be have an unbounded size. put() will therefore never block.
    //}
}

void AudioPlaybackHandler::StopForApp(
    /* [in] */ IInterface* callerIdentity)
{
    if (DBG){
        //Java:    Log.d(TAG, "Removing all callback items for : " + callerIdentity);
        Logger::D(TAG, String("Removing all callback items for : ") + String("\n"));
    }
    RemoveWorkItemsFor(callerIdentity);

    const AutoPtr<PlaybackQueueItem> current = mCurrentWorkItem;
    if (current != NULL && ((current->GetCallerIdentity()).Get() == callerIdentity)) {
        Stop(current);
    }
}

void AudioPlaybackHandler::Stop()
{
    if (DBG){
        //Java:    Log.d(TAG, "Stopping all items");
        Logger::D(TAG, String("Stopping all items\n"));
    }
    RemoveAllMessages();

    Stop(mCurrentWorkItem);
}

Boolean AudioPlaybackHandler::IsSpeaking()
{
    return (mQueue.GetFront() != NULL) || (mCurrentWorkItem != NULL);
}

void AudioPlaybackHandler::Quit()
{
    RemoveAllMessages();
    Stop(mCurrentWorkItem);
    mHandlerThread->Interrupt();
}

void AudioPlaybackHandler::RemoveAllMessages()
{
    /*
    while( mQueue.IsEmpty() == FALSE )
    {
        mQueue.Pop();
    }
    */
    mQueue.Clear();
}

void AudioPlaybackHandler::RemoveWorkItemsFor(
    /* [in] */ IInterface* callerIdentity)
{
    Int32 queueLen = mQueue.GetSize();
    for(Int32 i = 0; i<queueLen; i++)
    {
        AutoPtr<PlaybackQueueItem> item = mQueue.GetFront();
        AutoPtr<IInterface> ci;
        ci = item->GetCallerIdentity();   //item->GetCallerIdentity((IInterface**)&ciT);
        if (ci.Get() != callerIdentity) {
            //mQueue.Push(item);
            mQueue.PushBack(item);
        }
        //mQueue.Pop();
        mQueue.PopFront();
    }
}

} // namespace Tts
} // namespace Speech
} // namepsace Droid
} // namespace Elastos
