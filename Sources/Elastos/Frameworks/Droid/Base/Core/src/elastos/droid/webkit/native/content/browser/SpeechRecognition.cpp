// wuweizuo automatic build .cpp file from .java file.

#include "SpeechRecognition.h"

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Content {
namespace Browser {

//=====================================================================
//                     SpeechRecognition::Listener
//=====================================================================
ECode SpeechRecognition::Listener::OnBeginningOfSpeech()
{
    // ==================before translated======================
    // mState = STATE_CAPTURING_SPEECH;
    // nativeOnSoundStart(mNativeSpeechRecognizerImplAndroid);
    assert(0);
    return NOERROR;
}

ECode SpeechRecognition::Listener::OnBufferReceived(
    /* [in] */ ArrayOf<Byte>* buffer)
{
    VALIDATE_NOT_NULL(buffer);
    assert(0);
    return NOERROR;
}

ECode SpeechRecognition::Listener::OnEndOfSpeech()
{
    // ==================before translated======================
    // // Ignore onEndOfSpeech in continuous mode to let terminate() take care of ending
    // // events. The Android API documentation is vague as to when onEndOfSpeech is called in
    // // continuous mode, whereas the Web Speech API defines a stronger semantic on the
    // // equivalent (onsoundend) event. Thus, the only way to provide a valid onsoundend
    // // event is to trigger it when the last result is received or the session is aborted.
    // if (!mContinuous) {
    //     nativeOnSoundEnd(mNativeSpeechRecognizerImplAndroid);
    //     // Since Android doesn't have a dedicated event for when audio capture is finished,
    //     // we fire it after speech has ended.
    //     nativeOnAudioEnd(mNativeSpeechRecognizerImplAndroid);
    //     mState = STATE_IDLE;
    // }
    assert(0);
    return NOERROR;
}

ECode SpeechRecognition::Listener::OnError(
    /* [in] */ Int32 error)
{
    // ==================before translated======================
    // int code = SpeechRecognitionError.NONE;
    //
    // // Translate Android SpeechRecognizer errors to Web Speech API errors.
    // switch(error) {
    //     case SpeechRecognizer.ERROR_AUDIO:
    //         code = SpeechRecognitionError.AUDIO;
    //         break;
    //     case SpeechRecognizer.ERROR_CLIENT:
    //         code = SpeechRecognitionError.ABORTED;
    //         break;
    //     case SpeechRecognizer.ERROR_RECOGNIZER_BUSY:
    //     case SpeechRecognizer.ERROR_INSUFFICIENT_PERMISSIONS:
    //         code = SpeechRecognitionError.NOT_ALLOWED;
    //         break;
    //     case SpeechRecognizer.ERROR_NETWORK_TIMEOUT:
    //     case SpeechRecognizer.ERROR_NETWORK:
    //     case SpeechRecognizer.ERROR_SERVER:
    //         code = SpeechRecognitionError.NETWORK;
    //         break;
    //     case SpeechRecognizer.ERROR_NO_MATCH:
    //         code = SpeechRecognitionError.NO_MATCH;
    //         break;
    //     case SpeechRecognizer.ERROR_SPEECH_TIMEOUT:
    //         code = SpeechRecognitionError.NO_SPEECH;
    //         break;
    //     default:
    //         assert false;
    //         return;
    // }
    //
    // terminate(code);
    assert(0);
    return NOERROR;
}

ECode SpeechRecognition::Listener::OnEvent(
    /* [in] */ Int32 event,
    /* [in] */ IBundle* bundle)
{
    VALIDATE_NOT_NULL(bundle);
    assert(0);
    return NOERROR;
}

ECode SpeechRecognition::Listener::OnPartialResults(
    /* [in] */ IBundle* bundle)
{
    VALIDATE_NOT_NULL(bundle);
    // ==================before translated======================
    // handleResults(bundle, true);
    assert(0);
    return NOERROR;
}

ECode SpeechRecognition::Listener::OnReadyForSpeech(
    /* [in] */ IBundle* bundle)
{
    VALIDATE_NOT_NULL(bundle);
    // ==================before translated======================
    // mState = STATE_AWAITING_SPEECH;
    // nativeOnAudioStart(mNativeSpeechRecognizerImplAndroid);
    assert(0);
    return NOERROR;
}

ECode SpeechRecognition::Listener::OnResults(
    /* [in] */ IBundle* bundle)
{
    VALIDATE_NOT_NULL(bundle);
    // ==================before translated======================
    // handleResults(bundle, false);
    // // We assume that onResults is called only once, at the end of a session, thus we
    // // terminate. If one day the recognition provider changes dictation mode behavior to
    // // call onResults several times, we should terminate only if (!mContinuous).
    // terminate(SpeechRecognitionError.NONE);
    assert(0);
    return NOERROR;
}

ECode SpeechRecognition::Listener::OnRmsChanged(
    /* [in] */ Float rms)
{
    assert(0);
    return NOERROR;
}

ECode SpeechRecognition::Listener::HandleResults(
    /* [in] */ IBundle* bundle,
    /* [in] */ Boolean provisional)
{
    VALIDATE_NOT_NULL(bundle);
    // ==================before translated======================
    // if (mContinuous && provisional) {
    //     // In continuous mode, Android's recognizer sends final results as provisional.
    //     provisional = false;
    // }
    //
    // ArrayList<String> list = bundle.getStringArrayList(
    //         SpeechRecognizer.RESULTS_RECOGNITION);
    // String[] results = list.toArray(new String[list.size()]);
    //
    // float[] scores = bundle.getFloatArray(SpeechRecognizer.CONFIDENCE_SCORES);
    //
    // nativeOnRecognitionResults(mNativeSpeechRecognizerImplAndroid,
    //                            results,
    //                            scores,
    //                            provisional);
    assert(0);
    return NOERROR;
}

//=====================================================================
//                          SpeechRecognition
//=====================================================================
const String SpeechRecognition::PROVIDER_PACKAGE_NAME("com.google.android.googlequicksearchbox");
const Int32 SpeechRecognition::PROVIDER_MIN_VERSION;
const Int32 SpeechRecognition::STATE_IDLE;
const Int32 SpeechRecognition::STATE_AWAITING_SPEECH;
const Int32 SpeechRecognition::STATE_CAPTURING_SPEECH;
AutoPtr<IComponentName> SpeechRecognition::sRecognitionProvider;

Boolean SpeechRecognition::Initialize(
    /* [in] */ IContext* context)
{
    // ==================before translated======================
    // if (!SpeechRecognizer.isRecognitionAvailable(context))
    //     return false;
    //
    // PackageManager pm = context.getPackageManager();
    // Intent intent = new Intent(RecognitionService.SERVICE_INTERFACE);
    // final List<ResolveInfo> list = pm.queryIntentServices(intent, PackageManager.GET_SERVICES);
    //
    // for (ResolveInfo resolve : list) {
    //     ServiceInfo service = resolve.serviceInfo;
    //
    //     if (!service.packageName.equals(PROVIDER_PACKAGE_NAME))
    //         continue;
    //
    //     int versionCode;
    //     try {
    //         versionCode = pm.getPackageInfo(service.packageName, 0).versionCode;
    //     } catch (NameNotFoundException e) {
    //         continue;
    //     }
    //
    //     if (versionCode < PROVIDER_MIN_VERSION)
    //         continue;
    //
    //     sRecognitionProvider = new ComponentName(service.packageName, service.name);
    //
    //     return true;
    // }
    //
    // // If we reach this point, we failed to find a suitable recognition provider.
    // return false;
    assert(0);
    return FALSE;
}

SpeechRecognition::SpeechRecognition(
    /* [in] */ const IContext* context,
    /* [in] */ Int64 nativeSpeechRecognizerImplAndroid)
    : Continuous mode may not work as expected with a different provider.
             mRecognizer = SpeechRecognizer.createSpeechRecognizer(mContext);
         }

         mRecognizer.setRecognitionListener(mListener);
     }
{
    // ==================before translated======================
    // mContext = context;
    // mContinuous = false;
    // mNativeSpeechRecognizerImplAndroid = nativeSpeechRecognizerImplAndroid;
    // mListener = new Listener();
    // mIntent = new Intent(RecognizerIntent.ACTION_RECOGNIZE_SPEECH);
    //
    // if (sRecognitionProvider != null) {
    //     mRecognizer = SpeechRecognizer.createSpeechRecognizer(mContext, sRecognitionProvider);
    // } else {
    //     // It is possible to force-enable the speech recognition web platform feature (using a
    //     // command-line flag) even if initialize() failed to find the PROVIDER_PACKAGE_NAME
    //     // provider, in which case the first available speech recognition provider is used.
    //     // Caveat: Continuous mode may not work as expected with a different provider.
    //     mRecognizer = SpeechRecognizer.createSpeechRecognizer(mContext);
    // }
    //
    // mRecognizer.setRecognitionListener(mListener);
}

ECode SpeechRecognition::Terminate(
    /* [in] */ Int32 error)
{
    // ==================before translated======================
    //
    // if (mState != STATE_IDLE) {
    //     if (mState == STATE_CAPTURING_SPEECH) {
    //         nativeOnSoundEnd(mNativeSpeechRecognizerImplAndroid);
    //     }
    //     nativeOnAudioEnd(mNativeSpeechRecognizerImplAndroid);
    //     mState = STATE_IDLE;
    // }
    //
    // if (error != SpeechRecognitionError.NONE)
    //     nativeOnRecognitionError(mNativeSpeechRecognizerImplAndroid, error);
    //
    // mRecognizer.destroy();
    // mRecognizer = null;
    // nativeOnRecognitionEnd(mNativeSpeechRecognizerImplAndroid);
    // mNativeSpeechRecognizerImplAndroid = 0;
    assert(0);
    return NOERROR;
}

AutoPtr<SpeechRecognition> SpeechRecognition::CreateSpeechRecognition(
    /* [in] */ IContext* context,
    /* [in] */ Int64 nativeSpeechRecognizerImplAndroid)
{
    // ==================before translated======================
    // return new SpeechRecognition(context, nativeSpeechRecognizerImplAndroid);
    assert(0);
    AutoPtr<SpeechRecognition> empty;
    return empty;
}

ECode SpeechRecognition::StartRecognition(
    /* [in] */ const String& language,
    /* [in] */ Boolean continuous,
    /* [in] */ Boolean interimResults)
{
    // ==================before translated======================
    // if (mRecognizer == null)
    //     return;
    //
    // mContinuous = continuous;
    // mIntent.putExtra("android.speech.extra.DICTATION_MODE", continuous);
    // mIntent.putExtra(RecognizerIntent.EXTRA_LANGUAGE, language);
    // mIntent.putExtra(RecognizerIntent.EXTRA_PARTIAL_RESULTS, interimResults);
    // mRecognizer.startListening(mIntent);
    assert(0);
    return NOERROR;
}

ECode SpeechRecognition::AbortRecognition()
{
    // ==================before translated======================
    // if (mRecognizer == null)
    //     return;
    //
    // mRecognizer.cancel();
    // terminate(SpeechRecognitionError.ABORTED);
    assert(0);
    return NOERROR;
}

ECode SpeechRecognition::StopRecognition()
{
    // ==================before translated======================
    // if (mRecognizer == null)
    //     return;
    //
    // mContinuous = false;
    // mRecognizer.stopListening();
    assert(0);
    return NOERROR;
}

ECode SpeechRecognition::NativeOnAudioStart(
    /* [in] */ Int64 nativeSpeechRecognizerImplAndroid)
{
    assert(0);
    return NOERROR;
}

ECode SpeechRecognition::NativeOnSoundStart(
    /* [in] */ Int64 nativeSpeechRecognizerImplAndroid)
{
    assert(0);
    return NOERROR;
}

ECode SpeechRecognition::NativeOnSoundEnd(
    /* [in] */ Int64 nativeSpeechRecognizerImplAndroid)
{
    assert(0);
    return NOERROR;
}

ECode SpeechRecognition::NativeOnAudioEnd(
    /* [in] */ Int64 nativeSpeechRecognizerImplAndroid)
{
    assert(0);
    return NOERROR;
}

ECode SpeechRecognition::NativeOnRecognitionResults(
    /* [in] */ Int64 nativeSpeechRecognizerImplAndroid,
    /* [in] */ ArrayOf<String>* results,
    /* [in] */ ArrayOf<Float>* scores,
    /* [in] */ Boolean provisional)
{
    VALIDATE_NOT_NULL(results);
    VALIDATE_NOT_NULL(scores);
    assert(0);
    return NOERROR;
}

ECode SpeechRecognition::NativeOnRecognitionError(
    /* [in] */ Int64 nativeSpeechRecognizerImplAndroid,
    /* [in] */ Int32 error)
{
    assert(0);
    return NOERROR;
}

ECode SpeechRecognition::NativeOnRecognitionEnd(
    /* [in] */ Int64 nativeSpeechRecognizerImplAndroid)
{
    assert(0);
    return NOERROR;
}

} // namespace Browser
} // namespace Content
} // namespace Webkit
} // namespace Droid
} // namespace Elastos


