#include "speech/tts/TextToSpeechService.h"
#include "os/Process.h"
#include "os/Looper.h"
#include "content/CIntent.h"
#include <elastos/Logger.h>
#include "speech/tts/CSynthesisRequest.h"
#include "speech/tts/EventLogger.h"
#include "speech/tts/PlaybackSynthesisCallback.h"
#include "text/TextUtils.h"
#include "speech/tts/FileSynthesisCallback.h"
#include "speech/tts/SilencePlaybackQueueItem.h"
#include "os/CBinder.h"
#include "speech/tts/CTtsEngines.h"
#include "content/pm/CApplicationInfo.h"
//#include "provider/Settings.h"
#include "speech/tts/TextToSpeech.h"
#include <elastos/StringUtils.h>

using Elastos::Core::StringUtils;
using Elastos::IO::CFile;
using Elastos::Droid::Os::EIID_IIdleHandler;
using Elastos::Core::EIID_IRunnable;
using Elastos::Core::CStringWrapper;
using Elastos::Droid::Os::CBinder;
using Elastos::Droid::Content::CIntent;
using Elastos::Droid::Os::Looper;
using Elastos::Droid::Os::IMessageQueue;
using Elastos::Droid::Text::TextUtils;
using Elastos::Droid::Content::Pm::CApplicationInfo;
using Elastos::Droid::Os::EIID_IBinder;
using Elastos::Droid::Os::EIID_IHandler;
using Elastos::Core::Threading::Mutex;
using Elastos::Utility::Logging::Logger;

namespace Elastos {
namespace Droid {
namespace Speech {
namespace Tts {


const Boolean TextToSpeechService::DBG = FALSE;
const CString TextToSpeechService::TAG = "TextToSpeechService";

const Int32 TextToSpeechService::MAX_SPEECH_ITEM_CHAR_LENGTH = 4000;
const CString TextToSpeechService::SYNTH_THREAD_NAME = "SynthThread";

/******************************TextToSpeechService::SynthThread*************************/
CAR_INTERFACE_IMPL(TextToSpeechService::SynthThread, IIdleHandler)

TextToSpeechService::SynthThread::SynthThread(
    /* [in] */ TextToSpeechService* ttss)//:HandlerThread(String(SYNTH_THREAD_NAME), Process::THREAD_PRIORITY_DEFAULT)
{
    mFirstIdle = TRUE;
    mTtss = ttss;
}

void TextToSpeechService::SynthThread::OnLooperPrepared()
{
    AutoPtr<ILooper> looper;
    // TODO
    // GetLooper((ILooper**)&looper);
    AutoPtr<IMessageQueue> messageQueue;
    looper->GetQueue((IMessageQueue**)&messageQueue);
    messageQueue->AddIdleHandler(THIS_PROBE(IIdleHandler));
}

ECode TextToSpeechService::SynthThread::QueueIdle(
    /* [out] */ Boolean* result)
{
    if (mFirstIdle) {
        mFirstIdle = FALSE;
    } else {
        BroadcastTtsQueueProcessingCompleted();
    }
    return TRUE;
}

void TextToSpeechService::SynthThread::BroadcastTtsQueueProcessingCompleted()
{
    AutoPtr<IIntent> i;
    CIntent::New(ITextToSpeech::ACTION_TTS_QUEUE_PROCESSING_COMPLETED, (IIntent**)&i);
    if (DBG){
        //Java:    Log.d(TAG, "Broadcasting: " + i);
        String strIntent;
        i->ToString(&strIntent);
        Logger::D(TAG, String("Broadcasting: ")+strIntent+String("\n"));
    }
    mTtss->SendBroadcast(i);
}

/******************************TextToSpeechService::SynthHandler::RunnableSynthHandlerEnqueueSpeechItem*************************/

TextToSpeechService::SynthHandler::RunnableSynthHandlerEnqueueSpeechItem::RunnableSynthHandlerEnqueueSpeechItem(
    /* [in] */ SynthHandler* sh,
    /* [in] */ SpeechItem* si)
{
    mSh = sh;
    mSi = si;
}

ECode TextToSpeechService::SynthHandler::RunnableSynthHandlerEnqueueSpeechItem::Run()
{
    mSh->SetCurrentSpeechItem(mSi);
    mSi->Play();
    mSh->SetCurrentSpeechItem(NULL);
    return NOERROR;
}

/******************************TextToSpeechService::SynthHandler*************************/

ECode TextToSpeechService::SynthHandler::HandleMessage(
    /* [in] */ IMessage* message)
{
    return E_ILLEGAL_ARGUMENT_EXCEPTION;
}

TextToSpeechService::SynthHandler::SynthHandler(
    /* [in] */ ILooper* looper,
    /* [in] */ TextToSpeechService* ttss)
    : HandlerBase(looper)
{
    mTtss = ttss;
}

AutoPtr<TextToSpeechService::SpeechItem> TextToSpeechService::SynthHandler::GetCurrentSpeechItem()
{
    Mutex::Autolock lock(mLock);
    return mCurrentSpeechItem;
}

AutoPtr<TextToSpeechService::SpeechItem> TextToSpeechService::SynthHandler::SetCurrentSpeechItem(
    /* [in] */ TextToSpeechService::SpeechItem* speechItem)
{
    Mutex::Autolock lock(mLock);
    AutoPtr<TextToSpeechService::SpeechItem> old = mCurrentSpeechItem;
    mCurrentSpeechItem = speechItem;
    return old;
}

AutoPtr<TextToSpeechService::SpeechItem> TextToSpeechService::SynthHandler::MaybeRemoveCurrentSpeechItem(
    /* [in] */ IInterface* callerIdentity)
{
    Mutex::Autolock lock(mLock);
    if ((mCurrentSpeechItem != NULL) && ((SpeechItem*)((mCurrentSpeechItem->GetCallerIdentity()).Get())) == callerIdentity) {
        AutoPtr<TextToSpeechService::SpeechItem> current = mCurrentSpeechItem;
        mCurrentSpeechItem = NULL;
        return current;
    }
    return NULL;
}

Boolean TextToSpeechService::SynthHandler::IsSpeaking()
{
    return GetCurrentSpeechItem() != NULL;
}

void TextToSpeechService::SynthHandler::Quit()
{
    // Don't process any more speech items
    (Handler::mLooper)->Quit();    //Java:    Stop the current speech item
    AutoPtr<TextToSpeechService::SpeechItem> current = SetCurrentSpeechItem((TextToSpeechService::SpeechItem*)NULL);
    if (current != NULL) {
        current->Stop();
    }

    // The AudioPlaybackHandler will be destroyed by the caller.
}

Int32 TextToSpeechService::SynthHandler::EnqueueSpeechItem(
    /* [in] */ Int32 queueMode,
    /* [in] */ TextToSpeechService::SpeechItem* speechItem)
{
    if (!speechItem->IsValid()) {
        speechItem->DispatchOnError();
        return ITextToSpeech::TTS_ERROR;
    }

    if (queueMode == ITextToSpeech::QUEUE_FLUSH) {
        StopForApp( ( speechItem->GetCallerIdentity() ).Get() );
    } else if (queueMode == TextToSpeech::QUEUE_DESTROY) {
        StopAll();
    }

    AutoPtr<IInterface> callerIdentity = speechItem->GetCallerIdentity();
    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteInterfacePtr((IInterface*)(callerIdentity.Get()));

    AutoPtr<IRunnable> runnable = new RunnableSynthHandlerEnqueueSpeechItem(this, speechItem);
    Boolean result;
    ECode ec = this->Post(runnable, &result);

    if (!FAILED(ec)) {
        return ITextToSpeech::TTS_SUCCESS;
    }
    else {
        //Java:    Log.w(TAG, "SynthThread has quit");
        Logger::W(TAG, String("SynthThread has quit\n"));
        speechItem->DispatchOnError();
        return ITextToSpeech::TTS_ERROR;
    }
}

Int32 TextToSpeechService::SynthHandler::StopForApp(
    /* [in] */ IInterface* callerIdentity)
{
    if (callerIdentity == NULL) {
        return ITextToSpeech::TTS_ERROR;
    }

    //Java:    removeCallbacksAndMessages(callerIdentity);

    // This stops writing data to the file / or publishing
    // items to the audio playback handler.
    //
    // Note that the current speech item must be removed only if it
    // belongs to the callingApp, else the item will be "orphaned" and
    // not stopped correctly if a stop request comes along for the item
    // from the app it belongs to.
    AutoPtr<TextToSpeechService::SpeechItem> current = MaybeRemoveCurrentSpeechItem(callerIdentity);
    if (current != NULL) {
        current->Stop();
    }

    // Remove any enqueued audio too.
    (mTtss->mAudioPlaybackHandler)->StopForApp(callerIdentity);

    return ITextToSpeech::TTS_SUCCESS;
}

Int32 TextToSpeechService::SynthHandler::StopAll()
{
    // Stop the current speech item unconditionally.
    AutoPtr<TextToSpeechService::SpeechItem> current = SetCurrentSpeechItem(NULL);
    if (current != NULL) {
        current->Stop();
    }
    // Remove all other items from the queue.
    //Java:    removeCallbacksAndMessages(null);

    // Remove all pending playback as well.
    (mTtss->mAudioPlaybackHandler)->Stop();

    return ITextToSpeech::TTS_SUCCESS;
}

/******************************TextToSpeechService::SpeechItem*************************/
PInterface TextToSpeechService::SpeechItem::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)(ITextToSpeechServiceUtteranceProgressDispatcher*)this;
    }
    else if (riid == EIID_ITextToSpeechServiceUtteranceProgressDispatcher) {
        return (ITextToSpeechServiceUtteranceProgressDispatcher*)this;
    }
    return NULL;
}

UInt32 TextToSpeechService::SpeechItem::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 TextToSpeechService::SpeechItem::Release()
{
    return ElRefBase::Release();
}

ECode TextToSpeechService::SpeechItem::GetInterfaceID(
    /* [in] */ IInterface* Object,
    /* [out] */ InterfaceID* iID)
{
    VALIDATE_NOT_NULL(iID);
    if (iID == NULL) {
        return E_INVALID_ARGUMENT;
    }

    if (Object == (IInterface*)(ITextToSpeechServiceUtteranceProgressDispatcher*)this) {
        *iID = EIID_ITextToSpeechServiceUtteranceProgressDispatcher;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

TextToSpeechService::SpeechItem::SpeechItem(
    /* [in] */ IInterface* caller,
    /* [in] */ Int32 callerUid,
    /* [in] */ Int32 callerPid,
    /* [in] */ IBundle* params,
    /* [in] */ TextToSpeechService* ttss)
{
    mStarted = FALSE;
    mStopped = FALSE;

    mCallerIdentity = caller;
    mParams = params;
    mCallerUid = callerUid;
    mCallerPid = callerPid;
    mTtss = ttss;
}

AutoPtr<IInterface> TextToSpeechService::SpeechItem::GetCallerIdentity()
{
    return mCallerIdentity;
}

Int32 TextToSpeechService::SpeechItem::Play()
{
    if(TRUE)
    {
        Mutex::Autolock lock(mLock);
        if (mStarted) {
            //throw new IllegalStateException("play() called twice");
            Logger::E(TAG, String("play() called twice\n"));
            return 0;//E_ILLEGAL_STATE_EXCEPTION
        }
        mStarted = TRUE;
    }
    return PlayImpl();
}

void TextToSpeechService::SpeechItem::Stop()
{
    if(TRUE)
    {
        Mutex::Autolock lock(mLock);
        if (mStopped) {
            //throw new IllegalStateException("stop() called twice");
            Logger::E(TAG, String("stop() called twice\n"));
            //E_ILLEGAL_STATE_EXCEPTION
        }
        mStopped = TRUE;
    }
    StopImpl();
}

ECode TextToSpeechService::SpeechItem::DispatchOnDone()
{
    String utteranceId = GetUtteranceId();
    if (!utteranceId.IsNull()) {
        (mTtss->mCallbacks)->DispatchOnDone(GetCallerIdentity(), utteranceId);
    }
    return NOERROR;
}

ECode TextToSpeechService::SpeechItem::DispatchOnStart()
{
    String utteranceId = GetUtteranceId();
    if (!utteranceId.IsNull()) {
        (mTtss->mCallbacks)->DispatchOnStart(GetCallerIdentity(), utteranceId);
    }
    return NOERROR;
}

ECode TextToSpeechService::SpeechItem::DispatchOnError()
{
    String utteranceId = GetUtteranceId();
    if (!utteranceId.IsNull()) {
        (mTtss->mCallbacks)->DispatchOnError(GetCallerIdentity(), utteranceId);
    }
    return NOERROR;
}

Int32 TextToSpeechService::SpeechItem::GetCallerUid()
{
    return mCallerUid;
}

Int32 TextToSpeechService::SpeechItem::GetCallerPid()
{
    return mCallerPid;
}

Boolean TextToSpeechService::SpeechItem::IsStopped()
{
    Mutex::Autolock lock(mLock);
    return mStopped;
}

Int32 TextToSpeechService::SpeechItem::GetStreamType()
{
    return GetIntParam(ITextToSpeechEngine::KEY_PARAM_STREAM, ITextToSpeechEngine::DEFAULT_STREAM);
}

Float TextToSpeechService::SpeechItem::GetVolume()
{
    return GetFloatParam(ITextToSpeechEngine::KEY_PARAM_VOLUME, ITextToSpeechEngine::DEFAULT_VOLUME);
}

Float TextToSpeechService::SpeechItem::GetPan()
{
    return GetFloatParam(ITextToSpeechEngine::KEY_PARAM_PAN, ITextToSpeechEngine::DEFAULT_PAN);
}

String TextToSpeechService::SpeechItem::GetUtteranceId()
{
    return GetStringParam(ITextToSpeechEngine::KEY_PARAM_UTTERANCE_ID, String(NULL));
}

String TextToSpeechService::SpeechItem::GetStringParam(
    /* [in] */ const String& key,
    /* [in] */ const String& defaultValue)
{
    //Java:    return mParams == null ? defaultValue : mParams.getString(key, defaultValue);
    if(mParams == NULL)
    {
        return defaultValue;
    } else {
        String strParam;
        mParams->GetStringEx(key, defaultValue, &strParam);
        return strParam;
    }
}

Int32 TextToSpeechService::SpeechItem::GetIntParam(
    /* [in] */ const String& key,
    /* [in] */ Int32 defaultValue)
{
    //Java:    return mParams == null ? defaultValue : mParams.getInt(key, defaultValue);
    if(mParams == NULL)
    {
        return defaultValue;
    } else {
        Int32 nParam;
        mParams->GetInt32Ex(key, defaultValue, &nParam);
        return nParam;

    }
}

Float TextToSpeechService::SpeechItem::GetFloatParam(
    /* [in] */ const String& key,
    /* [in] */ Float defaultValue)
{
    //Java:    return mParams == null ? defaultValue : mParams.getFloat(key, defaultValue);
    if(mParams == NULL)
    {
        return defaultValue;
    } else {
        Float fParam;
        mParams->GetFloatEx(key, defaultValue, &fParam);
        return fParam;
    }
}

/******************************TextToSpeechService::SynthesisSpeechItem*************************/
TextToSpeechService::SynthesisSpeechItem::SynthesisSpeechItem(
    /* [in] */ IInterface* callerIdentity,
    /* [in] */ Int32 callerUid,
    /* [in] */ Int32 callerPid,
    /* [in] */ IBundle* params,
    /* [in] */ const String& text,
    /* [in] */ TextToSpeechService* ttss):SpeechItem(callerIdentity, callerUid, callerPid, params, ttss)
{
    mText = text;
    CSynthesisRequest::New(mText, mParams, (ISynthesisRequest**)&mSynthesisRequest);
    mDefaultLocale = mTtss->GetSettingsLocale();
    SetRequestParams(mSynthesisRequest.Get());
    mEventLogger = new EventLogger(mSynthesisRequest, callerUid, callerPid, ttss->mPackageName);
}

String TextToSpeechService::SynthesisSpeechItem::GetText()
{
    return mText;
}

Boolean TextToSpeechService::SynthesisSpeechItem::IsValid()
{
    if (mText.IsNull()) {
        //Java:    Log.e(TAG, "null synthesis text");
        Logger::E(TAG, String("null synthesis text\n"));
        return FALSE;
    }
    if (mText.GetLength() >= TextToSpeechService::MAX_SPEECH_ITEM_CHAR_LENGTH) {
        //Java:    Log.w(TAG, "Text too long: " + mText.length() + " chars");
        Logger::W(TAG, String("Text too long: ")+StringUtils::Int32ToString(mText.GetLength())+String("\n"));
        return FALSE;
    }
    return TRUE;
}

Int32 TextToSpeechService::SynthesisSpeechItem::PlayImpl()
{
    AutoPtr<AbstractSynthesisCallback> synthesisCallback;
    mEventLogger->OnRequestProcessingStart();
    if(TRUE){
        Mutex::Autolock lock(mLock);
        // stop() might have been called before we enter this
        // synchronized block.
        if (IsStopped()) {
            return ITextToSpeech::TTS_ERROR;
        }
        mSynthesisCallback = CreateSynthesisCallback();
        synthesisCallback = mSynthesisCallback;
    }
    mTtss->OnSynthesizeText(mSynthesisRequest.Get(), (ISynthesisCallback*)(synthesisCallback.Get()));
    return synthesisCallback->IsDone() ? ITextToSpeech::TTS_SUCCESS : ITextToSpeech::TTS_ERROR;
}

AutoPtr<AbstractSynthesisCallback> TextToSpeechService::SynthesisSpeechItem::CreateSynthesisCallback()
{
    AutoPtr<AbstractSynthesisCallback> asCallback = new PlaybackSynthesisCallback(GetStreamType(), GetVolume(), GetPan(),
                    mTtss->mAudioPlaybackHandler, this, GetCallerIdentity(), mEventLogger);
    return asCallback;
}

void TextToSpeechService::SynthesisSpeechItem::SetRequestParams(
    /* [in] */ ISynthesisRequest* request)
{
    ((CSynthesisRequest*)request)->SetLanguage(GetLanguage(), GetCountry(), GetVariant());
    ((CSynthesisRequest*)request)->SetSpeechRate(GetSpeechRate());

    ((CSynthesisRequest*)request)->SetPitch(GetPitch());
}

void TextToSpeechService::SynthesisSpeechItem::StopImpl()
{
    AutoPtr<AbstractSynthesisCallback> synthesisCallback;
    if(TRUE){
        Mutex::Autolock lock(mLock);
        synthesisCallback = mSynthesisCallback;
    }
    if (synthesisCallback != NULL) {
        // If the synthesis callback is null, it implies that we haven't
        // entered the synchronized(this) block in playImpl which in
        // turn implies that synthesis would not have started.
        synthesisCallback->Stop();
        mTtss->OnStop();
    }
}

String TextToSpeechService::SynthesisSpeechItem::GetLanguage()
{
    return GetStringParam(ITextToSpeechEngine::KEY_PARAM_LANGUAGE, (*mDefaultLocale)[0]);
}

Boolean TextToSpeechService::SynthesisSpeechItem::HasLanguage()
{
    String stringParam = GetStringParam(ITextToSpeechEngine::KEY_PARAM_LANGUAGE, String(NULL));
    AutoPtr<ICharSequence> cs;
    CStringWrapper::New(stringParam, (ICharSequence**)&cs);
    return !TextUtils::IsEmpty(cs.Get());
}

String TextToSpeechService::SynthesisSpeechItem::GetCountry()
{
    if (!HasLanguage()){
        return (*mDefaultLocale)[1];
    }
    return GetStringParam(ITextToSpeechEngine::KEY_PARAM_COUNTRY, String(""));
}

String TextToSpeechService::SynthesisSpeechItem::GetVariant()
{
    if (!HasLanguage()){
        return (*mDefaultLocale)[2];
    }
    return GetStringParam(ITextToSpeechEngine::KEY_PARAM_VARIANT, String(""));
}

Int32 TextToSpeechService::SynthesisSpeechItem::GetSpeechRate()
{
    return GetIntParam(ITextToSpeechEngine::KEY_PARAM_RATE, mTtss->GetDefaultSpeechRate());
}

Int32 TextToSpeechService::SynthesisSpeechItem::GetPitch()
{
    return GetIntParam(ITextToSpeechEngine::KEY_PARAM_PITCH, ITextToSpeechEngine::DEFAULT_PITCH);
}

/******************************TextToSpeechService::SynthesisToFileSpeechItem*************************/
TextToSpeechService::SynthesisToFileSpeechItem::SynthesisToFileSpeechItem(
    /* [in] */ IInterface* callerIdentity,
    /* [in] */ Int32 callerUid,
    /* [in] */ Int32 callerPid,
    /* [in] */ IBundle* params,
    /* [in] */ const String& text,
    /* [in] */ IFile* file,
    /* [in] */ TextToSpeechService* ttss):SynthesisSpeechItem(callerIdentity, callerUid, callerPid, params, text, ttss)
{
    mFile = file;
}

AutoPtr<AbstractSynthesisCallback> TextToSpeechService::SynthesisToFileSpeechItem::CreateSynthesisCallback()
{
    return new FileSynthesisCallback(mFile);
}

Int32 TextToSpeechService::SynthesisToFileSpeechItem::PlayImpl()
{
    DispatchOnStart();
    Int32 status = SynthesisSpeechItem::PlayImpl();
    if (status == ITextToSpeech::TTS_SUCCESS) {
        DispatchOnDone();
    } else {
        DispatchOnError();
    }
    return status;
}

/******************************TextToSpeechService::AudioSpeechItem*************************/
TextToSpeechService::AudioSpeechItem::AudioSpeechItem(
    /* [in] */ IInterface* callerIdentity,
    /* [in] */ Int32 callerUid,
    /* [in] */ Int32 callerPid,
    /* [in] */ IBundle* params,
    /* [in] */ IUri* uri,
    /* [in] */ TextToSpeechService* ttss):SpeechItem(callerIdentity, callerUid, callerPid, params, ttss)
{
    mItem = new AudioPlaybackQueueItem(this, (GetCallerIdentity()).Get(),
                    mTtss.Get(), uri, GetStreamType());
}

Boolean TextToSpeechService::AudioSpeechItem::IsValid()
{
    return TRUE;
}

Int32 TextToSpeechService::AudioSpeechItem::PlayImpl()
{
    (mTtss->mAudioPlaybackHandler)->Enqueue(mItem);
    return ITextToSpeech::TTS_SUCCESS;
}

void TextToSpeechService::AudioSpeechItem::StopImpl()
{
    // Do nothing.
}

/******************************TextToSpeechService::SilenceSpeechItem*************************/
TextToSpeechService::SilenceSpeechItem::SilenceSpeechItem(
    /* [in] */ IInterface* callerIdentity,
    /* [in] */ Int32 callerUid,
    /* [in] */ Int32 callerPid,
    /* [in] */ IBundle* params,
    /* [in] */ Int64 duration,
    /* [in] */ TextToSpeechService* ttss):SpeechItem(callerIdentity, callerUid, callerPid, params, ttss)
{
    mDuration = duration;
}

Boolean TextToSpeechService::SilenceSpeechItem::IsValid()
{
    return TRUE;
}

Int32 TextToSpeechService::SilenceSpeechItem::PlayImpl()
{
    AutoPtr<SilencePlaybackQueueItem> spqi = new SilencePlaybackQueueItem( this, GetCallerIdentity(), mDuration);
    (mTtss->mAudioPlaybackHandler)->Enqueue(spqi.Get());
    return ITextToSpeech::TTS_SUCCESS;
}

void TextToSpeechService::SilenceSpeechItem::StopImpl()
{
    // Do nothing, handled by AudioPlaybackHandler#stopForApp
}

/******************************TextToSpeechService::TextToSpeechServiceStub*************************/
PInterface TextToSpeechService::TextToSpeechServiceStub::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)(IITextToSpeechService*)this;
    }
    else if (riid == EIID_IITextToSpeechService) {
        return (IITextToSpeechService*)this;
    }
    return NULL;
}

UInt32 TextToSpeechService::TextToSpeechServiceStub::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 TextToSpeechService::TextToSpeechServiceStub::Release()
{
    return ElRefBase::Release();
}

ECode TextToSpeechService::TextToSpeechServiceStub::GetInterfaceID(
    /* [in] */ IInterface* Object,
    /* [out] */ InterfaceID* iID)
{
    VALIDATE_NOT_NULL(iID);
    if (iID == NULL) {
        return E_INVALID_ARGUMENT;
    }

    if (Object == (IInterface*)(IITextToSpeechService*)this) {
        *iID = EIID_IITextToSpeechService;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

TextToSpeechService::TextToSpeechServiceStub::TextToSpeechServiceStub(
    /* [in] */ TextToSpeechService* ttss)
{
    mTtss = ttss;
}

ECode TextToSpeechService::TextToSpeechServiceStub::Speak(
    /* [in] */ /*IIBinder*/IBinder* caller,
    /* [in] */ const String & text,
    /* [in] */ Int32 queueMode,
    /* [in] */ IBundle* params,
    /* [out] */ Int32* result)
{
    AutoPtr< ArrayOf<IInterface*> > aryInterface = ArrayOf<IInterface*>::Alloc(3);
    (*aryInterface)[0] = (IInterface*)caller;
    AutoPtr<ICharSequence> cs;
    CStringWrapper::New(text, (ICharSequence**)&cs);
    (*aryInterface)[1] = (IInterface*)(cs.Get());
    (*aryInterface)[2] = (IInterface*)params;

    if (!CheckNonNull( aryInterface.Get() )) {
        *result = ITextToSpeech::TTS_ERROR;
        return NOERROR;
    }

    AutoPtr<SpeechItem> item = new SynthesisSpeechItem(caller,
                    CBinder::GetCallingUid(), CBinder::GetCallingPid(), params, text, mTtss.Get());
    *result = (mTtss->mSynthHandler)->EnqueueSpeechItem(queueMode, item.Get());
    return NOERROR;
}

ECode TextToSpeechService::TextToSpeechServiceStub::SynthesizeToFile(
    /* [in] */ /*IIBinder*/IBinder* caller,
    /* [in] */ const String & text,
    /* [in] */ const String & filename,
    /* [in] */ IBundle* params,
    /* [out] */ Int32* result)
{
    AutoPtr< ArrayOf<IInterface*> > aryInterface = ArrayOf<IInterface*>::Alloc(4);
    (*aryInterface)[0] = (IInterface*)caller;
    AutoPtr<ICharSequence> cs;
    CStringWrapper::New(text, (ICharSequence**)&cs);
    (*aryInterface)[1] = (IInterface*)(cs.Get());
    AutoPtr<ICharSequence> cs2;
    CStringWrapper::New(filename, (ICharSequence**)&cs2);
    (*aryInterface)[2] = (IInterface*)(cs2.Get());
    (*aryInterface)[3] = (IInterface*)params;
    if (!CheckNonNull( aryInterface.Get() )) {
        *result = ITextToSpeech::TTS_ERROR;
        return NOERROR;
    }

    AutoPtr<IFile> file;
    CFile::New(filename, (IFile**)&file);
    AutoPtr<SpeechItem> item = new SynthesisToFileSpeechItem(caller, CBinder::GetCallingUid(), CBinder::GetCallingPid(), params, text, file.Get(), mTtss.Get());
    *result = (mTtss->mSynthHandler)->EnqueueSpeechItem(ITextToSpeech::QUEUE_ADD, item.Get());
    return NOERROR;
}

ECode TextToSpeechService::TextToSpeechServiceStub::PlayAudio(
    /* [in] */ /*IIBinder*/IBinder* caller,
    /* [in] */ IUri* audioUri,
    /* [in] */ Int32 queueMode,
    /* [in] */ IBundle* params,
    /* [out] */ Int32* result)
{
    AutoPtr< ArrayOf<IInterface*> > aryInterface = ArrayOf<IInterface*>::Alloc(3);
    (*aryInterface)[0] = (IInterface*)caller;
    (*aryInterface)[1] = (IInterface*)audioUri;
    (*aryInterface)[2] = (IInterface*)params;
    if (!CheckNonNull(aryInterface.Get())) {
        *result = ITextToSpeech::TTS_ERROR;
        return NOERROR;
    }

    AutoPtr<SpeechItem> item = new AudioSpeechItem(caller,
                    CBinder::GetCallingUid(), CBinder::GetCallingPid(), params, audioUri, mTtss.Get());
    *result = (mTtss->mSynthHandler)->EnqueueSpeechItem(queueMode, item.Get());
    return NOERROR;
}

ECode TextToSpeechService::TextToSpeechServiceStub::PlaySilence(
    /* [in] */ /*IIBinder*/IBinder* caller,
    /* [in] */ Int64 duration,
    /* [in] */ Int32 queueMode,
    /* [in] */ IBundle* params,
    /* [out] */ Int32* result)
{
    AutoPtr< ArrayOf<IInterface*> > aryInterface = ArrayOf<IInterface*>::Alloc(2);
    (*aryInterface)[0] = (IInterface*)caller;
    (*aryInterface)[1] = (IInterface*)params;
    if (!CheckNonNull(aryInterface.Get())) {
        *result = ITextToSpeech::TTS_ERROR;
        return NOERROR;
    }

    AutoPtr<SpeechItem> item = new SilenceSpeechItem(caller,
                    CBinder::GetCallingUid(), CBinder::GetCallingPid(), params, duration, mTtss.Get());
    *result = (mTtss->mSynthHandler)->EnqueueSpeechItem(queueMode, item.Get());
    return NOERROR;
}

ECode TextToSpeechService::TextToSpeechServiceStub::IsSpeaking(
    /* [out] */ Boolean* result)
{
    return (mTtss->mSynthHandler)->IsSpeaking() || (mTtss->mAudioPlaybackHandler)->IsSpeaking();
}

ECode TextToSpeechService::TextToSpeechServiceStub::Stop(
    /* [in] */ /*IIBinder*/IBinder* caller,
    /* [out] */ Int32* result)
{
    AutoPtr< ArrayOf<IInterface*> > aryInterface = ArrayOf<IInterface*>::Alloc(1);
    (*aryInterface)[0] = (IInterface*)caller;
    if (!CheckNonNull(aryInterface.Get())) {
        *result = ITextToSpeech::TTS_ERROR;
        return NOERROR;
    }
    *result = (mTtss->mSynthHandler)->StopForApp(caller);
    return NOERROR;
}

ECode TextToSpeechService::TextToSpeechServiceStub::GetLanguage(
    /* [out] */ ArrayOf<String>** result)
{
    *result = mTtss->OnGetLanguage();
    return NOERROR;
}

ECode TextToSpeechService::TextToSpeechServiceStub::IsLanguageAvailable(
    /* [in] */ const String & lang,
    /* [in] */ const String & country,
    /* [in] */ const String & variant,
    /* [out] */ Int32* result)
{
    AutoPtr< ArrayOf<IInterface*> > aryInterface = ArrayOf<IInterface*>::Alloc(1);
    AutoPtr<ICharSequence> cs;
    CStringWrapper::New(lang, (ICharSequence**)&cs);
    (*aryInterface)[0] = (IInterface*)(cs.Get());

    if (!CheckNonNull(aryInterface.Get())) {
        *result = ITextToSpeech::TTS_ERROR;
        return NOERROR;
    }
    *result = mTtss->OnIsLanguageAvailable(lang, country, variant);
    return NOERROR;
}

ECode TextToSpeechService::TextToSpeechServiceStub::GetFeaturesForLanguage(
    /* [in] */ const String & lang,
    /* [in] */ const String & country,
    /* [in] */ const String & variant,
    /* [out] */ ArrayOf<String>** result)
{
    AutoPtr< Set<String> > features = mTtss->OnGetFeaturesForLanguage(lang, country, variant);
    AutoPtr< ArrayOf<String> > featuresArray;
    if (!features->IsEmpty()) {
        featuresArray = ArrayOf<String>::Alloc(features->GetSize());

        Set<String>::Iterator item = features->Begin();
        Int32 i = 0;
        for(; item != features->End(); item++) {
            (*featuresArray)[i++] = *item;
        }
    }
    else {
        featuresArray = ArrayOf<String>::Alloc(0);
    }

    *result = featuresArray;
    INTERFACE_ADDREF(*result);
    return NOERROR;
}

ECode TextToSpeechService::TextToSpeechServiceStub::LoadLanguage(
    /* [in] */ const String & lang,
    /* [in] */ const String & country,
    /* [in] */ const String & variant,
    /* [out] */ Int32* result)
{
    AutoPtr< ArrayOf<IInterface*> > aryInterface = ArrayOf<IInterface*>::Alloc(1);
    AutoPtr<ICharSequence> cs;
    CStringWrapper::New(lang, (ICharSequence**)&cs);
    (*aryInterface)[0] = (IInterface*)(cs.Get());

    if (!CheckNonNull(aryInterface.Get())) {
        *result = ITextToSpeech::TTS_ERROR;
        return NOERROR;
    }

    *result = mTtss->OnLoadLanguage(lang, country, variant);
    return NOERROR;
}

ECode TextToSpeechService::TextToSpeechServiceStub::SetCallback(
    /* [in] */ /*IIBinder*/IBinder* caller,
    /* [in] */ IITextToSpeechCallback* cb)
{
    AutoPtr< ArrayOf<IInterface*> > aryInterface = ArrayOf<IInterface*>::Alloc(1);
    (*aryInterface)[0] = (IInterface*)caller;
    // Note that passing in a null callback is a valid use case.
    if (!CheckNonNull(aryInterface.Get())) {
        return NOERROR;
    }

    (mTtss->mCallbacks)->SetCallback(caller, cb);
    return NOERROR;
}

String TextToSpeechService::TextToSpeechServiceStub::Intern(
    /* [in] */ const String& in)
{
    // The input parameter will be non null.
    //return in.intern();
    return in;
}

Boolean TextToSpeechService::TextToSpeechServiceStub::CheckNonNull(
    /* [in] */ ArrayOf<IInterface*>* args)
{
    Int32 aryLen = args->GetLength();
    for (Int32 i = 0; i < aryLen; i++) {
        if( (*args)[i] == NULL ) {
            return FALSE;
        }
    }
    return TRUE;
}

/******************************TextToSpeechService::CallbackMap*************************/
TextToSpeechService::CallbackMap::CallbackMap(
            /* [in] */ TextToSpeechService* ttss)
{
    mTtss = ttss;
}

void TextToSpeechService::CallbackMap::SetCallback(
    /* [in] */ /*IIBinder*/IBinder* caller,
    /* [in] */ IITextToSpeechCallback* cb)
{
    Mutex::Autolock lock(mCallerToCallbackLock);
    AutoPtr<IITextToSpeechCallback> old = NULL;
    HashMap< AutoPtr</*IIBinder*/IBinder>, AutoPtr<IITextToSpeechCallback> >::Iterator oldI;
    if (cb != NULL) {
//        Register(cb, caller);
        oldI = ( mCallerToCallback.Insert( HashMap< AutoPtr</*IIBinder*/IBinder>, AutoPtr<IITextToSpeechCallback> >::ValueType(caller, cb) ) ).mFirst;
        old = oldI->mSecond;
    } else {
        //Java:    old = mCallerToCallback.remove(caller);
        mCallerToCallback.Erase(caller);
    }
    if (old != NULL && old.Get() != cb) {
//        Unregister(old);
    }
}

void TextToSpeechService::CallbackMap::DispatchOnDone(
    /* [in] */ IInterface* callerIdentity,
    /* [in] */ const String& utteranceId)
{
    AutoPtr<IITextToSpeechCallback> cb = GetCallbackFor(callerIdentity);
    if (cb == NULL) return;
    //try {
        cb->OnDone(utteranceId);
    //} catch (RemoteException e) {
        //Java:    Log.e(TAG, "Callback onDone failed: " + e);
        //Logger::E(TAG, String("Callback onDone failed: \n"));
    //}
}

void TextToSpeechService::CallbackMap::DispatchOnStart(
    /* [in] */ IInterface* callerIdentity,
    /* [in] */ const String& utteranceId)
{
    AutoPtr<IITextToSpeechCallback> cb = GetCallbackFor(callerIdentity);
    if (cb == NULL) return;
    //try {
        cb->OnStart(utteranceId);
    //} catch (RemoteException e) {
        //Java:    Log.e(TAG, "Callback onStart failed: " + e);
        //Logger::E(TAG, String("Callback onStart failed: \n"));
    //}
}

void TextToSpeechService::CallbackMap::DispatchOnError(
    /* [in] */ IInterface* callerIdentity,
    /* [in] */ const String& utteranceId)
{
    AutoPtr<IITextToSpeechCallback> cb = GetCallbackFor(callerIdentity);
    if (cb == NULL) return;
    //try {
        cb->OnError(utteranceId);
    //} catch (RemoteException e) {
        //Log.e(TAG, "Callback onError failed: " + e);
        //Logger::E(TAG, String("Callback onError failed: \n"));
    //}
}

void TextToSpeechService::CallbackMap::OnCallbackDied(
    /* [in] */ IITextToSpeechCallback* callback,
    /* [in] */ IInterface* cookie)
{
    AutoPtr</*IIBinder*/IBinder> caller = (/*IIBinder*/IBinder*) cookie;
    if(TRUE){
        Mutex::Autolock lock(mCallerToCallbackLock);
        mCallerToCallback.Erase(caller);
    }
    (mTtss->mSynthHandler)->StopForApp(caller);
}

void TextToSpeechService::CallbackMap::Kill()
{
    Mutex::Autolock lock(mCallerToCallbackLock);
    mCallerToCallback.Clear();
    //Java:    super.kill();
}

AutoPtr<IITextToSpeechCallback> TextToSpeechService::CallbackMap::GetCallbackFor(
    /* [in] */ IInterface* caller)
{
    AutoPtr<IITextToSpeechCallback> cb;
    AutoPtr</*IIBinder*/IBinder> asBinder = (/*IIBinder*/IBinder*) caller;
    if(TRUE){
        Mutex::Autolock lock(mCallerToCallbackLock);
        cb = (mCallerToCallback.Find(asBinder.Get()))->mSecond;
    }
    return cb;
}

/******************************TextToSpeechService*************************/
ECode TextToSpeechService::OnCreate()
{
    if (DBG) {
        //Java:    Log.d(TAG, "onCreate()");
        Logger::D(TAG, String("onCreate() \n"));
    }
    Service::OnCreate();

    AutoPtr<SynthThread> synthThread = new SynthThread(this);
//    synthThread->Start();
//    mSynthHandler = new SynthHandler(synthThread->GetLooper());

    mAudioPlaybackHandler = new AudioPlaybackHandler();
    mAudioPlaybackHandler->Start();

    CTtsEngines::New(this, (ITtsEngines**)&mEngineHelper);//Java:    mEngineHelper = new TtsEngines(this);

    mCallbacks = new CallbackMap(this);

    //Java:    mPackageName = getApplicationInfo().packageName;
    AutoPtr<IApplicationInfo> appInfo;
    GetApplicationInfo((IApplicationInfo**)&appInfo);
    ((CApplicationInfo*)(appInfo.Get()))->GetPackageName(&mPackageName);

    AutoPtr< ArrayOf<String> > defaultLocale = GetSettingsLocale();
    // Load default language
    OnLoadLanguage((*defaultLocale)[0], (*defaultLocale)[1], (*defaultLocale)[2]);
    return NOERROR;
}

ECode TextToSpeechService::OnDestroy()
{
    if (DBG){
        //Java:    Log.d(TAG, "onDestroy()");
        Logger::D(TAG, String("onDestroy() \n"));
    }

    // Tell the synthesizer to stop
    mSynthHandler->Quit();
    // Tell the audio playback thread to stop.
    mAudioPlaybackHandler->Quit();
    // Unregister all callbacks.
    mCallbacks->Kill();

    Service::OnDestroy();
    return NOERROR;
}

AutoPtr<Set<String> > TextToSpeechService::OnGetFeaturesForLanguage(
    /* [in] */ const String& lang,
    /* [in] */ const String& country,
    /* [in] */ const String& variant)
{
    AutoPtr<Set<String> > ret = new Set<String>();
    return ret;
}

Int32 TextToSpeechService::GetDefaultSpeechRate()
{
    return GetSecureSettingInt(/*ISettingsSecure::TTS_DEFAULT_RATE*/String("tts_default_rate"), ITextToSpeechEngine::DEFAULT_RATE);
}

AutoPtr< ArrayOf<String> > TextToSpeechService::GetSettingsLocale()
{
    String locale;
    mEngineHelper->GetLocalePrefForEngine(mPackageName, &locale);
    return CTtsEngines::ParseLocalePref(locale);
}

Int32 TextToSpeechService::GetSecureSettingInt(
    /* [in] */ const String& name,
    /* [in] */ Int32 defaultValue)
{
    AutoPtr<IContentResolver> cr;
    GetContentResolver((IContentResolver**)&cr);
    Int32 secureSettingInt;
//    Settings::Secure::GetInt32(cr.Get(), name, defaultValue, &secureSettingInt);
    return secureSettingInt;
}


AutoPtr</*IIBinder*/IBinder> TextToSpeechService::OnBind(
    /* [in] */ IIntent* intent)
{
    String strAction;
    intent->GetAction(&strAction);
    if (ITextToSpeechEngine::INTENT_ACTION_TTS_SERVICE.Equals(strAction)) {
        return (IBinder*)(mBinder->Probe(EIID_IBinder));
    }
    return NULL;
}

TextToSpeechService::TextToSpeechService()
{
    mBinder = (IITextToSpeechService*)(new TextToSpeechServiceStub(this));
}

} // namespace Tts
} // namespace Speech
} // namepsace Droid
} // namespace Elastos
