#include "elastos/droid/speech/RecognizerIntentHelper.h"
#include "elastos/droid/speech/RecognizerIntentHelper.h"
#include "elastos/droid/ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Speech {

const String RecognizerIntentHelper::EXTRA_CALLING_PACKAGE("calling_package");

const String RecognizerIntentHelper::ACTION_RECOGNIZE_SPEECH("android.speech.action.RECOGNIZE_SPEECH");

const String RecognizerIntentHelper::ACTION_WEB_SEARCH("android.speech.action.WEB_SEARCH");

const String RecognizerIntentHelper::ACTION_VOICE_SEARCH_HANDS_FREE("android.speech.action.VOICE_SEARCH_HANDS_FREE");

const String RecognizerIntentHelper::EXTRA_SECURE("android.speech.extras.EXTRA_SECURE");

const String RecognizerIntentHelper::EXTRA_SPEECH_INPUT_MINIMUM_LENGTH_MILLIS("android.speech.extras.SPEECH_INPUT_MINIMUM_LENGTH_MILLIS");

const String RecognizerIntentHelper::EXTRA_SPEECH_INPUT_COMPLETE_SILENCE_LENGTH_MILLIS("android.speech.extras.SPEECH_INPUT_COMPLETE_SILENCE_LENGTH_MILLIS");

const String RecognizerIntentHelper::EXTRA_SPEECH_INPUT_POSSIBLY_COMPLETE_SILENCE_LENGTH_MILLIS("android.speech.extras.SPEECH_INPUT_POSSIBLY_COMPLETE_SILENCE_LENGTH_MILLIS");

const String RecognizerIntentHelper::EXTRA_LANGUAGE_MODEL("android.speech.extra.LANGUAGE_MODEL");

const String RecognizerIntentHelper::LANGUAGE_MODEL_FREE_FORM("free_form");

const String RecognizerIntentHelper::LANGUAGE_MODEL_WEB_SEARCH("web_search");

const String RecognizerIntentHelper::EXTRA_PROMPT("android.speech.extra.PROMPT");

const String RecognizerIntentHelper::EXTRA_LANGUAGE("android.speech.extra.LANGUAGE");

const String RecognizerIntentHelper::EXTRA_ORIGIN("android.speech.extra.ORIGIN");

const String RecognizerIntentHelper::EXTRA_MAX_RESULTS("android.speech.extra.MAX_RESULTS");

const String RecognizerIntentHelper::EXTRA_WEB_SEARCH_ONLY("android.speech.extra.WEB_SEARCH_ONLY");

const String RecognizerIntentHelper::EXTRA_PARTIAL_RESULTS("android.speech.extra.PARTIAL_RESULTS");

const String RecognizerIntentHelper::EXTRA_RESULTS_PENDINGINTENT("android.speech.extra.RESULTS_PENDINGINTENT");

const String RecognizerIntentHelper::EXTRA_RESULTS_PENDINGINTENT_BUNDLE("android.speech.extra.RESULTS_PENDINGINTENT_BUNDLE");

const Int32 RecognizerIntentHelper::RESULT_NO_MATCH = IActivity::RESULT_FIRST_USER;

const Int32 RecognizerIntentHelper::RESULT_CLIENT_ERROR = IActivity::RESULT_FIRST_USER + 1;

const Int32 RecognizerIntentHelper::RESULT_SERVER_ERROR = IActivity::RESULT_FIRST_USER + 2;

const Int32 RecognizerIntentHelper::RESULT_NETWORK_ERROR = IActivity::RESULT_FIRST_USER + 3;

const Int32 RecognizerIntentHelper::RESULT_AUDIO_ERROR = IActivity::RESULT_FIRST_USER + 4;

const String RecognizerIntentHelper::EXTRA_RESULTS("android.speech.extra.RESULTS");

const String RecognizerIntentHelper::EXTRA_CONFIDENCE_SCORES("android.speech.extra.CONFIDENCE_SCORES");

const String RecognizerIntentHelper::DETAILS_META_DATA("android.speech.DETAILS");

const String RecognizerIntentHelper::ACTION_GET_LANGUAGE_DETAILS("android.speech.action.GET_LANGUAGE_DETAILS");

const String RecognizerIntentHelper::EXTRA_ONLY_RETURN_LANGUAGE_PREFERENCE("android.speech.extra.ONLY_RETURN_LANGUAGE_PREFERENCE");

const String RecognizerIntentHelper::EXTRA_LANGUAGE_PREFERENCE("android.speech.extra.LANGUAGE_PREFERENCE");

const String RecognizerIntentHelper::EXTRA_SUPPORTED_LANGUAGES("android.speech.extra.SUPPORTED_LANGUAGES");

CAR_INTERFACE_IMPL(RecognizerIntentHelper, Singleton, IRecognizerIntent);
CAR_SINGLETON_IMPL(RecognizerIntentHelper)

ECode RecognizerIntentHelper::GetVoiceDetailsIntent(
    /* [in] */ IContext* pContext,
    /* [out] */ IIntent** ppRet)
{
    VALIDATE_NOT_NULL(ppRet);

    AutoPtr<IIntent> voiceSearchIntent;
    CIntent::New(ACTION_WEB_SEARCH, (IIntent**)&voiceSearchIntent);

    AutoPtr<IPackageManager> pm;
    context->GetPackageManager((IPackageManager**)&pm);
    AutoPtr<IResolveInfo> ri;
    pm->ResolveActivity(voiceSearchIntent, IPackageManager::GET_META_DATA, (IResolveInfo**)&ri);

    AutoPtr<IActivityInfo> activityInfo;
    ri->GetActivityInfo((IActivityInfo**)&activityInfo);
    AutoPtr<IBundle> metaData;
    if (activityInfo == NULL) {
        activityInfo->GetMetaData((IBundle**)&metaData);
    }
    if (ri == NULL || (activityInfo == NULL || metaData == NULL)) {
        *ppRet = NULL;
        return NOERROR;
    }

    String className;
    metaData->GetString(DETAILS_META_DATA, &className);
    if (className.IsNull()) {
        *ppRet = NULL;
        return NOERROR;
    }

    AutoPtr<IIntent> detailsIntent;
    CIntent::New(ACTION_GET_LANGUAGE_DETAILS, (IIntent**)&detailsIntent);
    String packageName;
    ((CActivityInfo*)( activityInfo.Get() ) )->GetPackageName(&packageName);
    AutoPtr<IComponentName> cn;
    CComponentName::New(packageName, className, (IComponentName**)&cn);
    detailsIntent->SetComponent(cn.Get());

    *ppRet = detailsIntent;
    return NOERROR;
}

} // namespace Speech
} // namespace Droid
} // namespace Elastos

