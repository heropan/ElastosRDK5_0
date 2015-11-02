#include "elastos/droid/speech/RecognitionService.h"
#include <elastos/utility/logging/Logger.h>
#include "elastos/droid/ext/frameworkext.h"

using Elastos::Utility::Logging::Logger;

namespace Elastos {
namespace Droid {
namespace Speech {

const CString RecognitionService::SERVICE_INTERFACE = "android.speech.RecognitionService";
const CString RecognitionService::SERVICE_META_DATA = "android.speech";
const CString RecognitionService::TAG = "RecognitionService";
const Boolean RecognitionService::DBG = FALSE;
const Int32 RecognitionService::MSG_START_LISTENING = 1;
const Int32 RecognitionService::MSG_STOP_LISTENING = 2;
const Int32 RecognitionService::MSG_CANCEL = 3;
const Int32 RecognitionService::MSG_RESET = 4;

/******************************RecognitionService::RecognitionServiceCallback*************************/
PInterface RecognitionService::RecognitionServiceCallback::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)(IRecognitionServiceCallback*)this;
    }
    else if (riid == EIID_IRecognitionServiceCallback) {
        return (IRecognitionServiceCallback*)this;
    }
    return NULL;
}

UInt32 RecognitionService::RecognitionServiceCallback::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 RecognitionService::RecognitionServiceCallback::Release()
{
    return ElRefBase::Release();
}

ECode RecognitionService::RecognitionServiceCallback::GetInterfaceID(
    /* [in] */ IInterface* Object,
    /* [out] */ InterfaceID* iID)
{
    VALIDATE_NOT_NULL(iID);
    if (iID == NULL) {
        return E_INVALID_ARGUMENT;
    }

    if (Object == (IInterface*)(IRecognitionServiceCallback*)this) {
        *iID = EIID_IRecognitionServiceCallback;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

RecognitionService::RecognitionServiceCallback::RecognitionServiceCallback(
    /* [in]  */ IIRecognitionListener* listener,
    /* [in]  */ RecognitionService* rs)
{
    mListener = listener;
    mHost = rs;
}

ECode RecognitionService::RecognitionServiceCallback::BeginningOfSpeech()
{
    if (DBG) {
        Logger::D(TAG, String("beginningOfSpeech\n"));
    }
    mListener->OnBeginningOfSpeech();
    return NOERROR;
}

ECode RecognitionService::RecognitionServiceCallback::BufferReceived(
    /* [in]  */ ArrayOf<Byte>* buffer)
{
    mListener->OnBufferReceived(buffer);
    return NOERROR;
}

ECode RecognitionService::RecognitionServiceCallback::EndOfSpeech()
{
    mListener->OnEndOfSpeech();
    return NOERROR;
}

ECode RecognitionService::RecognitionServiceCallback::Error(
    /* [in]  */ Int32 error)
{
    Boolean result;
    mHost->mHandler->SendEmptyMessage(RecognitionService::MSG_RESET, &result);
    mListener->OnError(error);
    return NOERROR;
}

ECode RecognitionService::RecognitionServiceCallback::PartialResults(
    /* [in]  */ IBundle* partialResults)
{
    mListener->OnPartialResults(partialResults);
    return NOERROR;
}

ECode RecognitionService::RecognitionServiceCallback::ReadyForSpeech(
    /* [in]  */ IBundle* params)
{
    mListener->OnReadyForSpeech(params);
    return NOERROR;
}

ECode RecognitionService::RecognitionServiceCallback::Results(
    /* [in]  */ IBundle* results)
{
    Boolean result;
    mHost->mHandler->SendEmptyMessage(RecognitionService::MSG_RESET, &result);
    mListener->OnResults(results);
    return NOERROR;
}

ECode RecognitionService::RecognitionServiceCallback::RmsChanged(
    /* [in]  */ Float rmsdB)
{
    mListener->OnRmsChanged(rmsdB);
    return NOERROR;
}

/******************************RecognitionService::RecognitionServiceStartListeningArgs*************************/
CAR_INTERFACE_IMPL(RecognitionService::RecognitionServiceStartListeningArgs, IInterface)

RecognitionService::RecognitionServiceStartListeningArgs::RecognitionServiceStartListeningArgs(
    /* [in]  */ IIntent* intent,
    /* [in]  */ IIRecognitionListener* listener)
{
    mIntent = intent;
    mListener = listener;
}

/******************************RecognitionService::RecognitionServiceHandler*************************/

ECode RecognitionService::RecognitionServiceHandler::HandleMessage(
    /* [in] */ IMessage* msg)
{
    Int32 what;
    msg->GetWhat(&what);
    AutoPtr<IInterface> obj;
    msg->GetObj((IInterface**)&obj);

    switch (what) {
        case RecognitionService::MSG_START_LISTENING: {
            RecognitionServiceStartListeningArgs* args = (RecognitionServiceStartListeningArgs*)obj.Get();
            mHost->DispatchStartListening(args->mIntent.Get(), args->mListener.Get());
            break;
        }
        case RecognitionService::MSG_STOP_LISTENING:
            mHost->DispatchStopListening(IIRecognitionListener::Probe(obj));
            break;
        case RecognitionService::MSG_CANCEL:
            mHost->DispatchCancel(IIRecognitionListener::Probe(obj));
            break;
        case RecognitionService::MSG_RESET:
            mHost->DispatchClearCallback();
            break;
    }
    return NOERROR;
}

/******************************RecognitionService::RecognitionServiceBinder*************************/
CAR_INTERFACE_IMPL(RecognitionService::RecognitionServiceBinder, IIRecognitionService)

RecognitionService::RecognitionServiceBinder::RecognitionServiceBinder(
    /* [in]  */ RecognitionService* service)
{
    mInternalService = service;
}

ECode RecognitionService::RecognitionServiceBinder::StartListening(
    /* [in]  */ IIntent* recognizerIntent,
    /* [in]  */ IIRecognitionListener* listener)
{
    if (DBG){
        //Java:    Log.d(TAG, "startListening called by:" + listener.asBinder());
        String strOut = String("startListening called by:") + String("\n");
        Logger::D(TAG, strOut);
    }
    if (mInternalService != NULL && mInternalService->CheckPermissions(listener)) {
        AutoPtr<RecognitionServiceStartListeningArgs> rslArgs =
            new RecognitionServiceStartListeningArgs(recognizerIntent, listener);
        AutoPtr<IMessage> msg;
        mInternalService->mHandler->ObtainMessage(
            RecognitionService::MSG_START_LISTENING, rslArgs.Get(), (IMessage**)&msg);
        Boolean result;
        return mInternalService->mHandler->SendMessage(msg, &result);
    }
    return NOERROR;
}

ECode RecognitionService::RecognitionServiceBinder::StopListening(
    /* [in]  */ IIRecognitionListener* listener)
{
    if (DBG){
        //Java:    Log.d(TAG, "stopListening called by:" + listener.asBinder());
        String strOut = String("stopListening called by:") + String("\n");
        Logger::D(TAG, strOut);

    }
    if (mInternalService != NULL && mInternalService->CheckPermissions(listener)) {
        AutoPtr<IMessage> msg;
        mInternalService->mHandler->ObtainMessage(
            RecognitionService::MSG_STOP_LISTENING, listener, (IMessage**)&msg);
        Boolean result;
        return mInternalService->mHandler->SendMessage(msg, &result);
    }
    return NOERROR;
}

ECode RecognitionService::RecognitionServiceBinder::Cancel(
    /* [in]  */ IIRecognitionListener* listener)
{
    if (DBG){
        //Java:    Log.d(TAG, "cancel called by:" + listener.asBinder());
        String strOut = String("cancel called by:") + String("\n");
        Logger::D(TAG, strOut);
    }
    if (mInternalService != NULL && mInternalService->CheckPermissions(listener)) {
        AutoPtr<IMessage> msg;
        mInternalService->mHandler->ObtainMessage(
            RecognitionService::MSG_CANCEL, listener, (IMessage**)&msg);
        Boolean result;
        return mInternalService->mHandler->SendMessage(msg, &result);
    }
    return NOERROR;
}

ECode RecognitionService::RecognitionServiceBinder::ClearReference()
{
    mInternalService = NULL;
    return NOERROR;
}

/******************************RecognitionService*************************/
void RecognitionService::DispatchStartListening(
    /* [in]  */ IIntent* intent,
    /* [in]  */ IIRecognitionListener* listener)
{
    if (mCurrentCallback == NULL) {
        if (DBG){
            //Java:    Log.d(TAG, "created new mCurrentCallback, listener = " + listener.asBinder());
            String strOut = String("created new mCurrentCallback, listener = ") + String("\n");
            Logger::D(TAG, strOut);
        }
        mCurrentCallback = new RecognitionServiceCallback(listener, this);
        OnStartListening(intent, mCurrentCallback);
    } else {
        //try {
            listener->OnError(ISpeechRecognizer::ERROR_RECOGNIZER_BUSY);
        //} catch (RemoteException e) {
            //Java:    Log.d(TAG, "onError call from startListening failed");
            Logger::D(TAG, String("onError call from startListening failed\n"));
        //}
        //Java:    Log.i(TAG, "concurrent startListening received - ignoring this call");
        Logger::I(TAG, String("concurrent startListening received - ignoring this call\n"));
    }
}

void RecognitionService::DispatchStopListening(
    /* [in]  */ IIRecognitionListener* listener)
{
    //try {
        if (mCurrentCallback == NULL) {
            listener->OnError(ISpeechRecognizer::ERROR_CLIENT);
            //Java:    Log.w(TAG, "stopListening called with no preceding startListening - ignoring");
            Logger::W(TAG, String("stopListening called with no preceding startListening - ignoring\n"));
        } else if ( (mCurrentCallback->mListener).Get() != listener) {
            listener->OnError(ISpeechRecognizer::ERROR_RECOGNIZER_BUSY);
            //Java:    Log.w(TAG, "stopListening called by other caller than startListening - ignoring");
            Logger::W(TAG, String("stopListening called by other caller than startListening - ignoring\n"));
        } else { // the correct state
            OnStopListening(mCurrentCallback);
        }
    //} catch (RemoteException e) { // occurs if onError fails
        //Java:    Log.d(TAG, "onError call from stopListening failed");
        Logger::D(TAG, String("onError call from stopListening failed\n"));
    //}
}

void RecognitionService::DispatchCancel(
    /* [in]  */ IIRecognitionListener* listener)
{
    if (mCurrentCallback == NULL) {
        if (DBG){
            //Java:    Log.d(TAG, "cancel called with no preceding startListening - ignoring");
            Logger::D(TAG, String("cancel called with no preceding startListening - ignoring\n"));
        }
    } else if ( (mCurrentCallback->mListener).Get() != listener) {
        //Java:    Log.w(TAG, "cancel called by client who did not call startListening - ignoring");
        Logger::W(TAG, String("cancel called by client who did not call startListening - ignoring\n"));
    } else { // the correct state
        OnCancel(mCurrentCallback);
        mCurrentCallback = NULL;
        if (DBG){
            //Java:    Log.d(TAG, "canceling - setting mCurrentCallback to null");
            Logger::D(TAG, String("canceling - setting mCurrentCallback to null\n"));
        }
    }
}

void RecognitionService::DispatchClearCallback()
{
    mCurrentCallback = NULL;
}

Boolean RecognitionService::CheckPermissions(
    /* [in]  */ IIRecognitionListener* listener)
{
    if (DBG){
        //Java:    Log.d(TAG, "checkPermissions");
        Logger::D(TAG, String("checkPermissions\n"));
    }
    if (/*RecognitionService.this.checkCallingOrSelfPermission(Elastos::Droid::Manifest::permission::RECORD_AUDIO) == PackageManager.PERMISSION_GRANTED*/FALSE) {
        return TRUE;
    }
    //try {
        //Java:    Log.e(TAG, "call for recognition service without RECORD_AUDIO permissions");
        Logger::E(TAG, String("call for recognition service without RECORD_AUDIO permissions\n"));
        listener->OnError(ISpeechRecognizer::ERROR_INSUFFICIENT_PERMISSIONS);
    //} catch (RemoteException re) {
        //Java:    Log.e(TAG, "sending ERROR_INSUFFICIENT_PERMISSIONS message failed", re);
        Logger::E(TAG, String("sending ERROR_INSUFFICIENT_PERMISSIONS message failed\n"));
    //}
    return FALSE;
}

AutoPtr</*IIBinder*/IBinder> RecognitionService::OnBind(
    /* [in]  */ IIntent* intent)
{
    if (DBG){
        //Java:    Log.d(TAG, "onBind, intent=" + intent);
        String strIntent;
        intent->ToString(&strIntent);
        Logger::D(TAG, String("onBind, intent=") + strIntent + String("\n"));
    }
    return /*mBinder*/NULL;
}

ECode RecognitionService::OnDestroy()
{
    if (DBG){
        //Java:    Log.d(TAG, "onDestroy");
        Logger::D(TAG, String("onDestroy\n"));
    }
    mCurrentCallback = NULL;
    mBinder->ClearReference();
    Service::OnDestroy();
    return NOERROR;
}

RecognitionService::RecognitionService()
{
    mBinder = new RecognitionServiceBinder(this);
    mHandler = new RecognitionServiceHandler(this);
}

} // namespace Speech
} // namepsace Droid
} // namespace Elastos
