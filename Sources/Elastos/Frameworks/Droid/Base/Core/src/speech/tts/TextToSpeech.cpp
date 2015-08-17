#include "speech/tts/TextToSpeech.h"
#include "speech/tts/CTtsEngines.h"
#include "speech/tts/UtteranceProgressListener.h"
#include "os/CBundle.h"
#include "content/CIntent.h"
#include "text/TextUtils.h"
//#include "net/Uri.h"
//#include "net/CUriBuilder.h"
#include <elastos/utility/logging/Logger.h>
#include <elastos/core/StringUtils.h>

using Elastos::Core::StringUtils;
using Libcore::ICU::CLocale;
using Elastos::Core::CString;
using Elastos::Utility::Logging::Logger;
using Elastos::Core::CObjectContainer;
using Elastos::Droid::Os::CBundle;
using Elastos::Droid::Os::/*IIBinder*/IBinder;
using Elastos::Droid::Content::EIID_IServiceConnection;
using Elastos::Droid::Content::CIntent;
using Elastos::Droid::Content::IContentResolver;
using Elastos::Droid::Text::TextUtils;
using Elastos::Droid::Net::IUriBuilder;

namespace Elastos {
namespace Droid {
namespace Speech {
namespace Tts {

/******************************TextToSpeech::TextToSpeechEngineInfo*************************/
PInterface TextToSpeech::TextToSpeechEngineInfo::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)(ITextToSpeechEngineInfo*)this;
    }
    else if (riid == EIID_ITextToSpeechEngineInfo) {
        return (ITextToSpeechEngineInfo*)this;
    }
    return NULL;
}

UInt32 TextToSpeech::TextToSpeechEngineInfo::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 TextToSpeech::TextToSpeechEngineInfo::Release()
{
    return ElRefBase::Release();
}

ECode TextToSpeech::TextToSpeechEngineInfo::GetInterfaceID(
    /* [in] */ IInterface* Object,
    /* [out] */ InterfaceID* iID)
{
    VALIDATE_NOT_NULL(iID);
    if (iID == NULL) {
        return E_INVALID_ARGUMENT;
    }

    if (Object == (IInterface*)(ITextToSpeechEngineInfo*)this) {
        *iID = EIID_ITextToSpeechEngineInfo;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

ECode TextToSpeech::TextToSpeechEngineInfo::ToString(
    /* [out] */ String* ret)
{
    *ret = String("EngineInfo{name=") + name + String("}");
    return NOERROR;
}

String TextToSpeech::TextToSpeechEngineInfo::ToString()
{
    return String("EngineInfo{name=") + name + String("}");
}

/******************************TextToSpeech::TextToSpeechActionR*************************/
//None

/******************************TextToSpeech::TextToSpeechActionRShutdown*************************/
TextToSpeech::TextToSpeechActionRShutdown::TextToSpeechActionRShutdown(
    /* [in] */ TextToSpeech* tts)
{
    mTts = tts;
}

Handle32 TextToSpeech::TextToSpeechActionRShutdown::Run(
    /* [in] */ IITextToSpeechService* service)
{
    service->SetCallback((/*IIBinder*/IBinder*)((mTts->GetCallerIdentity()).Get()), NULL);
    Int32 stopRet;
    service->Stop((/*IIBinder*/IBinder*)((mTts->GetCallerIdentity()).Get()), &stopRet);
    (mTts->mServiceConnection)->Disconnect();
    // Context#unbindService does not result in a call to
    // ServiceConnection#onServiceDisconnected. As a result, the
    // service ends up being destroyed (if there are no other open
    // connections to it) but the process lives on and the
    // ServiceConnection continues to refer to the destroyed service.
    //
    // This leads to tons of log spam about SynthThread being dead.
    (mTts->mServiceConnection) = NULL;
    (mTts->mCurrentEngine) = String(NULL);
    return (Handle32)NULL;
}

/******************************TextToSpeech::TextToSpeechActionRSpeak*************************/
TextToSpeech::TextToSpeechActionRSpeak::TextToSpeechActionRSpeak(
    /* [in] */ TextToSpeech* tts,
    /* [in] */ const String& text,
    /* [in] */ Int32 queueMode,
    /* [in] */ IObjectStringMap* params)
{
    mTts = tts;
    mText = text;
    mQueueMode = queueMode;
    mParams = params;
}

Handle32 TextToSpeech::TextToSpeechActionRSpeak::Run(
    /* [in] */ IITextToSpeechService* service)
{
    AutoPtr<IUri> utteranceUri = (*((mTts->mUtterances).Find(mText))).mSecond;
    if (utteranceUri != NULL) {
        Int32 nRet;
        service->PlayAudio((/*IIBinder*/IBinder*)((mTts->GetCallerIdentity()).Get()), utteranceUri.Get(), mQueueMode, (mTts->GetParams(mParams)).Get(), &nRet);
        return (Handle32)nRet;
    } else {
        Int32 nRet;
        service->Speak((/*IIBinder*/IBinder*)((mTts->GetCallerIdentity()).Get()), mText, mQueueMode, (mTts->GetParams(mParams)).Get(), &nRet);
        return (Handle32)nRet;
    }
}

/******************************TextToSpeech::TextToSpeechActionRPlayEarcon*************************/
TextToSpeech::TextToSpeechActionRPlayEarcon::TextToSpeechActionRPlayEarcon(
    /* [in] */ TextToSpeech* tts,
    /* [in] */ const String& earcon,
    /* [in] */ Int32 queueMode,
    /* [in] */ IObjectStringMap* params)
{
    mTts = tts;
    mEarcon = earcon;
    mQueueMode = queueMode;
    mParams = params;
}

Handle32 TextToSpeech::TextToSpeechActionRPlayEarcon::Run(
    /* [in] */ IITextToSpeechService* service)
{
    AutoPtr<IUri> earconUri = (*((mTts->mEarcons).Find(mEarcon))).mSecond;
    if (earconUri == NULL) {
        return (Handle32)ITextToSpeech::TTS_ERROR;
    }
    Int32 nRet;
    service->PlayAudio((/*IIBinder*/IBinder*)((mTts->GetCallerIdentity()).Get()), earconUri, mQueueMode, (mTts->GetParams(mParams)).Get(), &nRet);
    return (Handle32)nRet;
}

/******************************TextToSpeech::TextToSpeechActionRPlaySilence*************************/
TextToSpeech::TextToSpeechActionRPlaySilence::TextToSpeechActionRPlaySilence(
    /* [in] */ TextToSpeech* tts,
    /* [in] */ Int64 durationInMs,
    /* [in] */ Int32 queueMode,
    /* [in] */ IObjectStringMap* params)
{
    mTts = tts;
    mDurationInMs = durationInMs;
    mQueueMode = queueMode;
    mParams = params;
}

Handle32 TextToSpeech::TextToSpeechActionRPlaySilence::Run(
    /* [in] */ IITextToSpeechService* service)
{
    Int32 nRet;
    service->PlaySilence((/*IIBinder*/IBinder*)((mTts->GetCallerIdentity()).Get()), mDurationInMs, mQueueMode, mTts->GetParams(mParams), &nRet);
    return (Handle32)nRet;
}

/******************************TextToSpeech::TextToSpeechActionRGetFeatures*************************/
TextToSpeech::TextToSpeechActionRGetFeatures::TextToSpeechActionRGetFeatures(
    /* [in] */ TextToSpeech* tts,
    /* [in] */ ILocale* locale)
{
    mTts = tts;
    mLocale = locale;
}

Handle32 TextToSpeech::TextToSpeechActionRGetFeatures::Run(
    /* [in] */ IITextToSpeechService* service)
{
    String strISO3Language, strISO3Country, strVariant;
    mLocale->GetISO3Language(&strISO3Language);
    mLocale->GetISO3Country(&strISO3Country);
    mLocale->GetVariant(&strVariant);
    AutoPtr< ArrayOf<String> > features;
    service->GetFeaturesForLanguage(strISO3Language, strISO3Country, strVariant, (ArrayOf<String>**)&features);
    if (features != NULL) {
        AutoPtr<IObjectContainer> oc;
        CObjectContainer::New((IObjectContainer**)&oc);
        Int32 aryLen = features->GetLength();
        String strTemp;
        for(Int32 i = 0; i<aryLen; i++)
        {
            strTemp = (*features)[i];
            AutoPtr<ICharSequence> cs;
            CString::New(strTemp, (ICharSequence**)&cs);
            oc->Add((IInterface*)(cs.Get()));
        }
        return (Handle32)(oc.Get());
    }
    return (Handle32)NULL;
}

/******************************TextToSpeech::TextToSpeechActionRIsSpeaking*************************/
TextToSpeech::TextToSpeechActionRIsSpeaking::TextToSpeechActionRIsSpeaking(
    /* [in] */ TextToSpeech* tts)
{
    mTts = tts;
}

Handle32 TextToSpeech::TextToSpeechActionRIsSpeaking::Run(
    /* [in] */ IITextToSpeechService* service)
{
    Boolean bRet;
    service->IsSpeaking(&bRet);
    return (Handle32)bRet;
}

/******************************TextToSpeech::TextToSpeechActionRStop*************************/
TextToSpeech::TextToSpeechActionRStop::TextToSpeechActionRStop(
    /* [in] */ TextToSpeech* tts)
{
    mTts = tts;
}

Handle32 TextToSpeech::TextToSpeechActionRStop::Run(
    /* [in] */ IITextToSpeechService* service)
{
    Int32 nRet;
    service->Stop((/*IIBinder*/IBinder*)((mTts->GetCallerIdentity()).Get()), &nRet);
    return (Handle32)nRet;
}

/******************************TextToSpeech::TextToSpeechActionRSetLanguage*************************/
TextToSpeech::TextToSpeechActionRSetLanguage::TextToSpeechActionRSetLanguage(
    /* [in] */ TextToSpeech* tts,
    /* [in] */ ILocale* locale)
{
    mTts = tts;
    mLocale = locale;
}

Handle32 TextToSpeech::TextToSpeechActionRSetLanguage::Run(
    /* [in] */ IITextToSpeechService* service)
{
    if (mLocale == NULL) {
        return ITextToSpeech::LANG_NOT_SUPPORTED;
    }
    String language, country, variant;
    mLocale->GetISO3Language(&language);
    mLocale->GetISO3Country(&country);
    mLocale->GetVariant(&variant);
    // Check if the language, country, variant are available, and cache
    // the available parts.
    // Note that the language is not actually set here, instead it is cached so it
    // will be associated with all upcoming utterances.
    Int32 result;
    service->LoadLanguage(language, country, variant, &result);
    if (result >= ITextToSpeech::LANG_AVAILABLE){
        if (result < ITextToSpeech::LANG_COUNTRY_VAR_AVAILABLE) {
            variant = "";
            if (result < ITextToSpeech::LANG_COUNTRY_AVAILABLE) {
                country = "";
            }
        }
        (mTts->mParams)->PutString(ITextToSpeechEngine::KEY_PARAM_LANGUAGE, language);
        (mTts->mParams)->PutString(ITextToSpeechEngine::KEY_PARAM_COUNTRY, country);
        (mTts->mParams)->PutString(ITextToSpeechEngine::KEY_PARAM_VARIANT, variant);
    }
    return (Handle32)result;
}

/******************************TextToSpeech::TextToSpeechActionRGetLanguage*************************/
TextToSpeech::TextToSpeechActionRGetLanguage::TextToSpeechActionRGetLanguage(
    /* [in] */ TextToSpeech* tts)
{
    mTts = tts;
}

Handle32 TextToSpeech::TextToSpeechActionRGetLanguage::Run(
    /* [in] */ IITextToSpeechService* service)
{
    AutoPtr< ArrayOf<String> > locStrings;
    service->GetLanguage((ArrayOf<String>**)&locStrings);
    if (locStrings != NULL && locStrings->GetLength() == 3) {
        AutoPtr<ILocale> localeRet;
        CLocale::New((*locStrings)[0], (*locStrings)[1], (*locStrings)[2], (ILocale**)&localeRet);
        return (Handle32)(localeRet.Get());
    }
    return (Handle32)NULL;
}

/******************************TextToSpeech::TextToSpeechActionRIsLanguageAvailable*************************/
TextToSpeech::TextToSpeechActionRIsLanguageAvailable::TextToSpeechActionRIsLanguageAvailable(
    /* [in] */ TextToSpeech* tts,
    /* [in] */ ILocale* locale)
{
    mTts = tts;
    mLocale = locale;
}

Handle32 TextToSpeech::TextToSpeechActionRIsLanguageAvailable::Run(
    /* [in] */ IITextToSpeechService* service)
{
    String language, country, variant;
    mLocale->GetISO3Language(&language);
    mLocale->GetISO3Country(&country);
    mLocale->GetVariant(&variant);
    Int32 nRet;
    service->IsLanguageAvailable(language, country, variant, &nRet);
    return (Handle32)nRet;
}

/******************************TextToSpeech::TextToSpeechActionRSynthesizeToFile*************************/
TextToSpeech::TextToSpeechActionRSynthesizeToFile::TextToSpeechActionRSynthesizeToFile(
    /* [in] */ TextToSpeech* tts,
    /* [in] */ const String& text,
    /* [in] */ IObjectStringMap* params,
    /* [in] */ const String& filename)
{
    mTts = tts;
    mText = text;
    mParams = params;
    mFilename = filename;
}

Handle32 TextToSpeech::TextToSpeechActionRSynthesizeToFile::Run(
    /* [in] */ IITextToSpeechService* service)
{
    Int32 nRet;
    service->SynthesizeToFile((/*IIBinder*/IBinder*)((mTts->GetCallerIdentity()).Get()), mText, mFilename, (mTts->GetParams(mParams)).Get(), &nRet);
    return (Handle32)nRet;
}


/******************************TextToSpeech::TextToSpeechConnection::TextToSpeechConnectionCallback*************************/
PInterface TextToSpeech::TextToSpeechConnection::TextToSpeechConnectionCallback::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)(IITextToSpeechCallback*)this;
    }
    else if (riid == EIID_IITextToSpeechCallback) {
        return (IITextToSpeechCallback*)this;
    }
    return NULL;
}

UInt32 TextToSpeech::TextToSpeechConnection::TextToSpeechConnectionCallback::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 TextToSpeech::TextToSpeechConnection::TextToSpeechConnectionCallback::Release()
{
    return ElRefBase::Release();
}

ECode TextToSpeech::TextToSpeechConnection::TextToSpeechConnectionCallback::GetInterfaceID(
    /* [in] */ IInterface* Object,
    /* [out] */ InterfaceID* iID)
{
    VALIDATE_NOT_NULL(iID);
    if (iID == NULL) {
        return E_INVALID_ARGUMENT;
    }

    if (Object == (IInterface*)(IITextToSpeechCallback*)this) {
        *iID = EIID_IITextToSpeechCallback;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

TextToSpeech::TextToSpeechConnection::TextToSpeechConnectionCallback::TextToSpeechConnectionCallback(
    /* [in] */ TextToSpeech* tts)
{
    mTts = tts;
}

ECode TextToSpeech::TextToSpeechConnection::TextToSpeechConnectionCallback::OnDone(
    /* [in] */ const String& utteranceId)
{
    AutoPtr<IUtteranceProgressListener> listener = mTts -> mUtteranceProgressListener;
    if (listener != NULL) {
        return listener -> OnDone(utteranceId);
    }
    return NOERROR;
}

ECode TextToSpeech::TextToSpeechConnection::TextToSpeechConnectionCallback::OnError(
    /* [in] */ const String& utteranceId)
{
    AutoPtr<IUtteranceProgressListener> listener = mTts -> mUtteranceProgressListener;
    if (listener != NULL) {
        return listener -> OnError(utteranceId);
    }
    return NOERROR;
}

ECode TextToSpeech::TextToSpeechConnection::TextToSpeechConnectionCallback::OnStart(
    /* [in] */ const String& utteranceId)
{
    AutoPtr<IUtteranceProgressListener> listener = mTts -> mUtteranceProgressListener;
    if (listener != NULL) {
        return listener -> OnStart(utteranceId);
    }
    return NOERROR;
}

/******************************TextToSpeech::TextToSpeechConnection*************************/
PInterface TextToSpeech::TextToSpeechConnection::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)(IServiceConnection*)this;
    }
    else if (riid == EIID_IServiceConnection) {
        return (IServiceConnection*)this;
    }
    return NULL;
}

UInt32 TextToSpeech::TextToSpeechConnection::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 TextToSpeech::TextToSpeechConnection::Release()
{
    return ElRefBase::Release();
}

ECode TextToSpeech::TextToSpeechConnection::GetInterfaceID(
    /* [in] */ IInterface* Object,
    /* [out] */ InterfaceID* iID)
{
    VALIDATE_NOT_NULL(iID);
    if (iID == NULL) {
        return E_INVALID_ARGUMENT;
    }

    if (Object == (IInterface*)(IServiceConnection*)this) {
        *iID = EIID_IServiceConnection;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

TextToSpeech::TextToSpeechConnection::TextToSpeechConnection(
    /* [in] */ TextToSpeech* pTts)
{
    mTts = pTts;
    mCallback = new TextToSpeechConnectionCallback(mTts);
}

ECode TextToSpeech::TextToSpeechConnection::OnServiceConnected(
    /* [in] */ IComponentName* name//,
//    /* [in] */ IIBinder* service
    )
{
    //Java:    Log.i(TAG, "Connected to " + name);
    String shortStringComponentName;
    name -> ToString(&shortStringComponentName);
    Logger::I(mTts->TAG, String("Connected to ")+shortStringComponentName+String("\n"));

    AutoLock lock(mTts->mStartLock);
    if (mTts->mServiceConnection != NULL) {
        // Disconnect any previous service connection
        (mTts->mServiceConnection)->Disconnect();
    }
    mTts->mServiceConnection = this;
//    mService = (IITextToSpeechService*)(service);
    //try {
        mService->SetCallback((/*IIBinder*/IBinder*)(GetCallerIdentity().Get()), mCallback);
        mTts->DispatchOnInit(ITextToSpeech::TTS_SUCCESS);
    //} catch (RemoteException re) {
        //Java:    Log.e(TAG, "Error connecting to service, setCallback() failed");
        /*
        Logger::E(mTts->TAG, String("Error connecting to service, setCallback() failed\n"));
        mTts->DispatchOnInit(ERROR);
        */
    //}
    return NOERROR;
}

/*AutoPtr<IIBinder>*/AutoPtr<IInterface> TextToSpeech::TextToSpeechConnection::GetCallerIdentity()
{
    return mCallback;
}

ECode TextToSpeech::TextToSpeechConnection::OnServiceDisconnected(
    /* [in] */ IComponentName* name)
{
    AutoLock lock(mTts->mStartLock);
    mService = NULL;
    // If this is the active connection, clear it
    if ((mTts->mServiceConnection).Get() == this) {
        mTts->mServiceConnection = NULL;
    }
    return NOERROR;
}

void TextToSpeech::TextToSpeechConnection::Disconnect()
{
    (mTts->mContext)->UnbindService(this);

    AutoLock lock(mTts->mStartLock);
    mService = NULL;
    // If this is the active connection, clear it
    if ((mTts->mServiceConnection).Get() == this) {
        (mTts->mServiceConnection) = NULL;
    }
}

Handle32 TextToSpeech::TextToSpeechConnection::RunAction(
    /* [in] */ TextToSpeechActionR* action,
    /* [in] */ Handle32 errorResult,
    /* [in] */ const String& method,
    /* [in] */ Boolean reconnect)
{
    AutoLock lock(mTts->mStartLock);
    //try {
        if (mService == NULL) {
            //Java:    Log.w(TAG, method + " failed: not connected to TTS engine");
            Logger::W(mTts->TAG, method + String(" failed: not connected to TTS engine\n"));
            return errorResult;
        }
        return action->Run(mService);
    //} catch (RemoteException ex) {
        //Java:    Log.e(TAG, method + " failed", ex);
        Logger::E(mTts->TAG, method + String(" failed\n"));
        if (reconnect) {
            Disconnect();
            mTts->InitTts();
        }
        return errorResult;
    //}
}

/******************************TextToSpeech*************************/
const Int32 TextToSpeech::QUEUE_DESTROY = 2;
const CString TextToSpeech::TAG = "TextToSpeech";

TextToSpeech::TextToSpeech()
{}

TextToSpeech::TextToSpeech(
    /* [in] */ IContext* context,
    /* [in] */ ITextToSpeechOnInitListener* listener)
{
    Init(context, listener, String(NULL));
}

TextToSpeech::TextToSpeech(
    /* [in] */ IContext* context,
    /* [in] */ ITextToSpeechOnInitListener* listener,
    /* [in] */ const String& engine)
{
    Init(context, listener, engine, String(NULL), TRUE);
}

TextToSpeech::TextToSpeech(
    /* [in] */ IContext* context,
    /* [in] */ ITextToSpeechOnInitListener* listener,
    /* [in] */ const String& engine,
    /* [in] */ const String& packageName,
    /* [in] */ Boolean useFallback)
{
    Init(context, listener, engine, packageName, useFallback);
}

void TextToSpeech::Init(
    /* [in] */ IContext* context,
    /* [in] */ ITextToSpeechOnInitListener* listener)
{
    Init(context, listener, String(NULL));
}

void TextToSpeech::Init(
    /* [in] */ IContext* context,
    /* [in] */ ITextToSpeechOnInitListener* listener,
    /* [in] */ const String& engine)
{
    Init(context, listener, engine, String(NULL), TRUE);
}

void TextToSpeech::Init(
    /* [in] */ IContext* context,
    /* [in] */ ITextToSpeechOnInitListener* listener,
    /* [in] */ const String& engine,
    /* [in] */ const String& packageName,
    /* [in] */ Boolean useFallback)
{
    mContext = context;
    mInitListener = listener;
    mRequestedEngine = engine;
    mUseFallback = useFallback;

    //Java:    mEarcons = new HashMap<String, Uri>();
    //Java:    mUtterances = new HashMap<String, Uri>();
    mUtteranceProgressListener = NULL;

    AutoPtr<CTtsEngines> cttsEngines;
    CTtsEngines::NewByFriend(mContext, (CTtsEngines**)&cttsEngines);  //Java:    mEnginesHelper = new TtsEngines(mContext);
    mEnginesHelper = cttsEngines;
    if (!packageName.IsNull()) {
        mPackageName = packageName;
    } else {
        mContext->GetPackageName(&mPackageName);
    }
    InitTts();
}

Handle32 TextToSpeech::RunActionNoReconnect(
    /* [in] */ TextToSpeechActionR* action,
    /* [in] */ Handle32 errorResult,
    /* [in] */ const String& method)
{
    return RunAction(action, errorResult, method, FALSE);
}

Handle32 TextToSpeech::RunAction(
    /* [in] */ TextToSpeechActionR* action,
    /* [in] */ Handle32 errorResult,
    /* [in] */ const String& method)
{
    return RunAction(action, errorResult, method, TRUE);
}

Handle32 TextToSpeech::RunAction(
    /* [in] */ TextToSpeechActionR* action,
    /* [in] */ Handle32 errorResult,
    /* [in] */ const String& method,
    /* [in] */ Boolean reconnect)
{
    AutoLock lock(mStartLock);
    if (mServiceConnection == NULL) {
        //Java:    Log.w(TAG, method + " failed: not bound to TTS engine");
        Logger::W(TAG, method + String(" failed: not bound to TTS engine\n"));
        return errorResult;
    }
    return mServiceConnection->RunAction(action, errorResult, method, reconnect);
}

Int32 TextToSpeech::InitTts()
{
    // Step 1: Try connecting to the engine that was requested.
    if (!mRequestedEngine.IsNull()) {
        Boolean bIsEngineInstalled;
        if ((mEnginesHelper->IsEngineInstalled(mRequestedEngine, &bIsEngineInstalled), bIsEngineInstalled)) {
            if (ConnectToEngine(mRequestedEngine)) {
                mCurrentEngine = mRequestedEngine;
                return ITextToSpeech::TTS_SUCCESS;
            }
            else if (!mUseFallback) {
                mCurrentEngine = String(NULL);
                DispatchOnInit(ITextToSpeech::TTS_ERROR);
                return ITextToSpeech::TTS_ERROR;
            }
        }
        else if (!mUseFallback) {
            //Java:    Log.i(TAG, "Requested engine not installed: " + mRequestedEngine);
            Logger::I(TAG, String("Requested engine not installed: ")+mRequestedEngine+String("\n"));
            mCurrentEngine = String(NULL);
            DispatchOnInit(ITextToSpeech::TTS_ERROR);
            return ITextToSpeech::TTS_ERROR;
        }
    }

    // Step 2: Try connecting to the user's default engine.
    /*const*/ String defaultEngine = GetDefaultEngine();
    if (!defaultEngine.IsNull() && !defaultEngine.Equals(mRequestedEngine)) {
        if (ConnectToEngine(defaultEngine)) {
            mCurrentEngine = defaultEngine;
            return ITextToSpeech::TTS_SUCCESS;
        }
    }

    // Step 3: Try connecting to the highest ranked engine in the
    // system.
    /*const*/ String highestRanked;
    mEnginesHelper->GetHighestRankedEngineName(&highestRanked);
    if (!highestRanked.IsNull() && !highestRanked.Equals(mRequestedEngine) &&
            !highestRanked.Equals(defaultEngine)) {
        if (ConnectToEngine(highestRanked)) {
            mCurrentEngine = highestRanked;
            return ITextToSpeech::TTS_SUCCESS;
        }
    }

    // NOTE: The API currently does not allow the caller to query whether
    // they are actually connected to any engine. This might fail for various
    // reasons like if the user disables all her TTS engines.

    mCurrentEngine = String(NULL);
    DispatchOnInit(ITextToSpeech::TTS_ERROR);
    return ITextToSpeech::TTS_ERROR;
}

Boolean TextToSpeech::ConnectToEngine(
    /* [in] */ const String& engine)
{
    AutoPtr<TextToSpeechConnection> connection = /*new TextToSpeechConnection(this)*/NULL;
    AutoPtr<IIntent> intent;
    CIntent::New(ITextToSpeechEngine::INTENT_ACTION_TTS_SERVICE, (IIntent**)&intent);
    intent->SetPackage(engine);
    Boolean bound;
    mContext->BindService(intent.Get(), connection.Get(), IContext::BIND_AUTO_CREATE, &bound);
    if (!bound) {
        //Java:    Log.e(TAG, "Failed to bind to " + engine);
        Logger::E(TAG, String("Failed to bind to ")+engine+String("\n"));
        return FALSE;
    } else {
        //Java:    Log.i(TAG, "Sucessfully bound to " + engine);
        Logger::I(TAG, String("Sucessfully bound to ")+engine+String("\n"));
        return TRUE;
    }
}

void TextToSpeech::DispatchOnInit(
    /* [in] */ Int32 result)
{
    AutoLock lock(mStartLock);
    if (mInitListener != NULL) {
        mInitListener->OnInit(result);
        mInitListener = NULL;
    }
}

/*AutoPtr<IIBinder>*/AutoPtr<IInterface> TextToSpeech::GetCallerIdentity()
{
    return mServiceConnection->GetCallerIdentity();
}

void TextToSpeech::Shutdown()
{
    AutoPtr<TextToSpeechActionR> ttsActionR  = new TextToSpeechActionRShutdown(this);
    RunActionNoReconnect(ttsActionR.Get(), (Handle32)NULL, String("shutdown"));
}

Int32 TextToSpeech::AddSpeech(
    /* [in] */ const String& text,
    /* [in] */ const String& packagename,
    /* [in] */ Int32 resourceId)
{
    AutoLock lock(mStartLock);
    mUtterances.Insert(Map<String, AutoPtr<IUri> >::ValueType(text, MakeResourceUri(packagename, resourceId)) );
    return ITextToSpeech::TTS_SUCCESS;
}

Int32 TextToSpeech::AddSpeech(
    /* [in] */ const String& text,
    /* [in] */ const String& filename)
{
    AutoLock lock(mStartLock);
    AutoPtr<IUri> uri = /*Uri::Parse(filename)*/NULL;
    mUtterances.Insert(Map<String, AutoPtr<IUri> >::ValueType(text, uri.Get() ) );
    return ITextToSpeech::TTS_SUCCESS;
}

Int32 TextToSpeech::AddEarcon(
    /* [in] */ const String& earcon,
    /* [in] */ const String& packagename,
    /* [in] */ Int32 resourceId)
{
    AutoLock lock(mStartLock);
    mEarcons.Insert(Map<String, AutoPtr<IUri> >::ValueType(earcon, MakeResourceUri(packagename, resourceId) ) );
    return ITextToSpeech::TTS_SUCCESS;
}

Int32 TextToSpeech::AddEarcon(
    /* [in] */ const String& earcon,
    /* [in] */ const String& filename)
{
    AutoLock lock(mStartLock);
    AutoPtr<IUri> uri = /*Uri::Parse(filename)*/NULL;
    mEarcons.Insert(Map<String, AutoPtr<IUri> >::ValueType(earcon, uri.Get() ) );
    return ITextToSpeech::TTS_SUCCESS;
}

AutoPtr<IUri> TextToSpeech::MakeResourceUri(
    /* [in] */ const String& packageName,
    /* [in] */ Int32 resourceId)
{
    //Java:    return new Uri.Builder().scheme(ContentResolver.SCHEME_ANDROID_RESOURCE).encodedAuthority(packageName).appendEncodedPath(String.valueOf(resourceId)).build();
    AutoPtr<IUriBuilder> ub;
//    CUriBuilder::New((IUriBuilder**)&ub);
//    ub -> Scheme(IContentResolver::SCHEME_ANDROID_RESOURCE);
//    ub -> EncodedAuthority(packageName);
//    ub -> AppendEncodedPath(StringUtils::Int32ToString(resourceId));

    AutoPtr<IUri> uRet;
//    ub -> Build((IUri**)&uRet);
    return uRet;
}

Int32 TextToSpeech::Speak(
    /* [in] */ /*const*/ String text,
    /* [in] */ /*const*/ Int32 queueMode,
    /* [in] */ /*const*/ IObjectStringMap* params)
{
    AutoPtr<TextToSpeechActionR> ttsActionR = new TextToSpeechActionRSpeak(this, text, queueMode, params);
    return (Int32)RunAction(ttsActionR.Get(), ITextToSpeech::TTS_ERROR, String("speak") );
}

Int32 TextToSpeech::PlayEarcon(
    /* [in] */ /*const*/ String earcon,
    /* [in] */ /*const*/ Int32 queueMode,
    /* [in] */ /*const*/ IObjectStringMap* params)
{
    AutoPtr<TextToSpeechActionR> ttsActionR = new TextToSpeechActionRPlayEarcon(this, earcon, queueMode, params);
    return (Int32)RunAction(ttsActionR.Get(), ITextToSpeech::TTS_ERROR, String("playEarcon") );
}

Int32 TextToSpeech::PlaySilence(
    /* [in] */ /*const*/ Int64 durationInMs,
    /* [in] */ /*const*/ Int32 queueMode,
    /* [in] */ /*const*/ IObjectStringMap* params)
{
    AutoPtr<TextToSpeechActionR> ttsActionR = new TextToSpeechActionRPlaySilence(this, durationInMs, queueMode, params);
    return (Int32)RunAction(ttsActionR.Get(), ITextToSpeech::TTS_ERROR, String("playSilence") );
}

AutoPtr< Set<String> > TextToSpeech::GetFeatures(
    /* [in] */ ILocale* locale)
{
    AutoPtr<TextToSpeechActionR> ttsActionR = new TextToSpeechActionRGetFeatures(this, locale);
    AutoPtr<IObjectContainer> oc;
    oc = (IObjectContainer*)RunAction(ttsActionR.Get(), (Handle32)NULL, String("getFeatures") );

    AutoPtr<Set<String> > sRet = new Set<String>();
    if(oc != NULL) {
        AutoPtr<IObjectEnumerator> it;
        oc->GetObjectEnumerator((IObjectEnumerator**)&it);
        Boolean succeeded = FALSE;
        String strTemp;
        while(it->MoveNext(&succeeded), succeeded) {
            AutoPtr<ICharSequence> cs;
            it->Current((IInterface**)&cs);
            cs->ToString(&strTemp);
            sRet->Insert(strTemp);
        }
    }

    return sRet;
}

Boolean TextToSpeech::IsSpeaking()
{
    AutoPtr<TextToSpeechActionR> ttsActionR = new TextToSpeechActionRIsSpeaking(this);
    return (Boolean)RunAction(ttsActionR.Get(), FALSE, String("isSpeaking") );
}

Int32 TextToSpeech::Stop()
{
    AutoPtr<TextToSpeechActionR> ttsActionR = new TextToSpeechActionRStop(this);
    return (Int32)RunAction(ttsActionR.Get(), ITextToSpeech::TTS_ERROR, String("stop") );
}

Int32 TextToSpeech::SetSpeechRate(
    /* [in] */ Float speechRate)
{
    if (speechRate > 0.0f) {
        Int32 intRate = (Int32)(speechRate * 100);
        if (intRate > 0) {
            if(TRUE){
                AutoLock lock(mStartLock);
                mParams->PutInt32(ITextToSpeechEngine::KEY_PARAM_RATE, intRate);
            }
            return ITextToSpeech::TTS_SUCCESS;
        }
    }
    return ITextToSpeech::TTS_ERROR;
}

Int32 TextToSpeech::SetPitch(
    /* [in] */ Float pitch)
{
    if (pitch > 0.0f) {
        Int32 intPitch = (Int32)(pitch * 100);
        if (intPitch > 0) {
            if(TRUE){
                AutoLock lock(mStartLock);
                mParams->PutInt32(ITextToSpeechEngine::KEY_PARAM_PITCH, intPitch);
            }
            return ITextToSpeech::TTS_SUCCESS;
        }
    }
    return ITextToSpeech::TTS_ERROR;
}

String TextToSpeech::GetCurrentEngine()
{
    return mCurrentEngine;
}

Int32 TextToSpeech::SetLanguage(
    /* [in] */ ILocale* loc)
{
    AutoPtr<TextToSpeechActionR> ttsActionR = new TextToSpeechActionRSetLanguage(this, loc);
    return (Int32)RunAction(ttsActionR.Get(), ITextToSpeech::LANG_NOT_SUPPORTED, String("setLanguage") );
}

AutoPtr<ILocale> TextToSpeech::GetLanguage()
{
    AutoPtr<TextToSpeechActionR> ttsActionR = new TextToSpeechActionRGetLanguage(this);
    AutoPtr<ILocale> lRet = (ILocale*)RunAction(ttsActionR.Get(), (Handle32)NULL, String("getLanguage") );
    lRet->Release();//???
    return lRet;
}

Int32 TextToSpeech::IsLanguageAvailable(
    /* [in] */ ILocale* loc)
{
    AutoPtr<TextToSpeechActionR> ttsActionR = new TextToSpeechActionRIsLanguageAvailable(this, loc);
    return (Int32)RunAction(ttsActionR.Get(), ITextToSpeech::LANG_NOT_SUPPORTED, String("isLanguageAvailable") );
}

Int32 TextToSpeech::SynthesizeToFile(
    /* [in] */ /*const*/ String text,
    /* [in] */ /*const*/ IObjectStringMap* params,
    /* [in] */ /*const*/ String filename)
{
    AutoPtr<TextToSpeechActionR> ttsActionR = new TextToSpeechActionRSynthesizeToFile(this, text, params, filename);
    return (Int32)RunAction(ttsActionR.Get(), ITextToSpeech::TTS_ERROR, String("synthesizeToFile") );
}

AutoPtr<IBundle> TextToSpeech::GetParams(
    /* [in] */ IObjectStringMap* params)
{
    Int32 paramsLen;
    if (params != NULL && ((params->GetSize(&paramsLen), paramsLen)!=0)) {
        AutoPtr<IBundle> bundle;
        CBundle::New(mParams, (IBundle**)&bundle);
        CopyIntParam(bundle.Get(), params, ITextToSpeechEngine::KEY_PARAM_STREAM );
        CopyStringParam(bundle.Get(), params, ITextToSpeechEngine::KEY_PARAM_UTTERANCE_ID );
        CopyFloatParam(bundle.Get(), params, ITextToSpeechEngine::KEY_PARAM_VOLUME );
        CopyFloatParam(bundle.Get(), params, ITextToSpeechEngine::KEY_PARAM_PAN );

        // Copy feature strings defined by the framework.
        CopyStringParam(bundle.Get(), params, ITextToSpeechEngine::KEY_FEATURE_NETWORK_SYNTHESIS );
        CopyStringParam(bundle.Get(), params, ITextToSpeechEngine::KEY_FEATURE_EMBEDDED_SYNTHESIS );

        // Copy over all parameters that start with the name of the
        // engine that we are currently connected to. The engine is
        // free to interpret them as it chooses.
        AutoPtr<ICharSequence> cs;
        CString::New(mCurrentEngine, (ICharSequence**)&cs);
        if (!TextUtils::IsEmpty(cs)) {
            AutoPtr< ArrayOf<String> > keys;
            AutoPtr<IObjectContainer> values;
            params -> GetAllItems((ArrayOf<String>**)&keys, (IObjectContainer**)&values);
            Int32 keysLen = keys->GetLength();
            for(Int32 i=0; i<keysLen; i++)
            {
                if( ((*keys)[i]).StartWith(mCurrentEngine) )
                {
                    String val;
                    AutoPtr<ICharSequence> csEx;
                    params->Get((*keys)[i], (IInterface**)&csEx);
                    bundle->PutString((*keys)[i], (csEx->ToString(&val), val));
                }
            }
        }

        return bundle;
    } else {
        return mParams;
    }
}

void TextToSpeech::CopyStringParam(
    /* [in] */ IBundle* bundle,
    /* [in] */ IObjectStringMap* params,
    /* [in] */ const String& key)
{
    AutoPtr<ICharSequence> cs;
    params->Get(key, (IInterface**)&cs);
    String value;
    cs->ToString(&value);
    if (!value.IsNull()) {
        bundle->PutString(key, value);
    }
}

void TextToSpeech::CopyIntParam(
    /* [in] */ IBundle* bundle,
    /* [in] */ IObjectStringMap* params,
    /* [in] */ const String& key)
{
    AutoPtr<ICharSequence> cs;
    params->Get(key, (IInterface**)&cs);
    String valueString;
    cs->ToString(&valueString);
    if (!TextUtils::IsEmpty(valueString)) {
        //try {
            Int32 value = StringUtils::ParseInt32(valueString);
            bundle->PutInt32(key, value);
        //} catch (NumberFormatException ex) {
            // don't set the value in the bundle
        //}
    }
}

void TextToSpeech::CopyFloatParam(
    /* [in] */ IBundle* bundle,
    /* [in] */ IObjectStringMap* params,
    /* [in] */ const String& key)
{
    AutoPtr<ICharSequence> cs;
    params->Get(key, (IInterface**)&cs);
    String valueString;
    cs->ToString(&valueString);
    if (!TextUtils::IsEmpty(valueString)) {
        //try {
            Float value = StringUtils::ParseFloat(valueString);
            bundle->PutFloat(key, value);
        //} catch (NumberFormatException ex) {
            // don't set the value in the bundle
        //}
    }
}

Int32 TextToSpeech::SetOnUtteranceCompletedListener(
    /* [in] */ /*const*/ ITextToSpeechOnUtteranceCompletedListener* listener)
{
    mUtteranceProgressListener = UtteranceProgressListener::From(listener);
    return ITextToSpeech::TTS_SUCCESS;
}

Int32 TextToSpeech::SetOnUtteranceProgressListener(
    /* [in] */ IUtteranceProgressListener* listener)
{
    mUtteranceProgressListener = listener;
    return ITextToSpeech::TTS_SUCCESS;
}

Int32 TextToSpeech::SetEngineByPackageName(
    /* [in] */ const String& enginePackageName)
{
    mRequestedEngine = enginePackageName;
    return InitTts();
}

String TextToSpeech::GetDefaultEngine()
{
    String strRet;
    mEnginesHelper->GetDefaultEngine(&strRet);
    return strRet;
}

Boolean TextToSpeech::AreDefaultsEnforced()
{
    return FALSE;
}

AutoPtr< List< AutoPtr<ITextToSpeechEngineInfo> > > TextToSpeech::GetEngines()
{
    AutoPtr< List< AutoPtr<ITextToSpeechEngineInfo> > > lRet = new List< AutoPtr<ITextToSpeechEngineInfo> >();

    AutoPtr<IObjectContainer> oc;
    mEnginesHelper->GetEngines( (IObjectContainer**)&oc );
    if(oc != NULL) {
        AutoPtr<IObjectEnumerator> it;
        oc->GetObjectEnumerator((IObjectEnumerator**)&it);
        Boolean succeeded = FALSE;
        while(it->MoveNext(&succeeded), succeeded) {
            AutoPtr<ITextToSpeechEngineInfo> ttsEi;
            it->Current((IInterface**)&ttsEi);

            lRet->PushBack(ttsEi);

        }
    }

    return lRet;
}

} // namespace Tts
} // namespace Speech
} // namepsace Droid
} // namespace Elastos
