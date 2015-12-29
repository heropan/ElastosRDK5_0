#include "elastos/droid/speech/CRecognizerIntentHelper.h"
#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/content/CIntent.h"
#include "elastos/droid/content/pm/CActivityInfo.h"
#include "elastos/droid/content/CComponentName.h"
#include "Elastos.Droid.Internal.h"
#include "Elastos.Droid.App.h"
#include "Elastos.Droid.Os.h"

using Elastos::Droid::Content::CIntent;
using Elastos::Droid::App::IActivity;
using Elastos::Droid::Content::Pm::IResolveInfo;
using Elastos::Droid::Content::Pm::IActivityInfo;
using Elastos::Droid::Content::Pm::IIPackageManager;
using Elastos::Droid::Content::Pm::CActivityInfo;
using Elastos::Droid::Content::IComponentName;
using Elastos::Droid::Content::CComponentName;

namespace Elastos {
namespace Droid {
namespace Speech {

const String CRecognizerIntentHelper::EXTRA_CALLING_PACKAGE("calling_package");

const String CRecognizerIntentHelper::ACTION_RECOGNIZE_SPEECH("android.speech.action.RECOGNIZE_SPEECH");

const String CRecognizerIntentHelper::ACTION_WEB_SEARCH("android.speech.action.WEB_SEARCH");

const String CRecognizerIntentHelper::ACTION_VOICE_SEARCH_HANDS_FREE("android.speech.action.VOICE_SEARCH_HANDS_FREE");

const String CRecognizerIntentHelper::EXTRA_SECURE("android.speech.extras.EXTRA_SECURE");

const String CRecognizerIntentHelper::EXTRA_SPEECH_INPUT_MINIMUM_LENGTH_MILLIS("android.speech.extras.SPEECH_INPUT_MINIMUM_LENGTH_MILLIS");

const String CRecognizerIntentHelper::EXTRA_SPEECH_INPUT_COMPLETE_SILENCE_LENGTH_MILLIS("android.speech.extras.SPEECH_INPUT_COMPLETE_SILENCE_LENGTH_MILLIS");

const String CRecognizerIntentHelper::EXTRA_SPEECH_INPUT_POSSIBLY_COMPLETE_SILENCE_LENGTH_MILLIS("android.speech.extras.SPEECH_INPUT_POSSIBLY_COMPLETE_SILENCE_LENGTH_MILLIS");

const String CRecognizerIntentHelper::EXTRA_LANGUAGE_MODEL("android.speech.extra.LANGUAGE_MODEL");

const String CRecognizerIntentHelper::LANGUAGE_MODEL_FREE_FORM("free_form");

const String CRecognizerIntentHelper::LANGUAGE_MODEL_WEB_SEARCH("web_search");

const String CRecognizerIntentHelper::EXTRA_PROMPT("android.speech.extra.PROMPT");

const String CRecognizerIntentHelper::EXTRA_LANGUAGE("android.speech.extra.LANGUAGE");

const String CRecognizerIntentHelper::EXTRA_ORIGIN("android.speech.extra.ORIGIN");

const String CRecognizerIntentHelper::EXTRA_MAX_RESULTS("android.speech.extra.MAX_RESULTS");

const String CRecognizerIntentHelper::EXTRA_WEB_SEARCH_ONLY("android.speech.extra.WEB_SEARCH_ONLY");

const String CRecognizerIntentHelper::EXTRA_PARTIAL_RESULTS("android.speech.extra.PARTIAL_RESULTS");

const String CRecognizerIntentHelper::EXTRA_RESULTS_PENDINGINTENT("android.speech.extra.RESULTS_PENDINGINTENT");

const String CRecognizerIntentHelper::EXTRA_RESULTS_PENDINGINTENT_BUNDLE("android.speech.extra.RESULTS_PENDINGINTENT_BUNDLE");

const Int32 CRecognizerIntentHelper::RESULT_NO_MATCH = IActivity::RESULT_FIRST_USER;

const Int32 CRecognizerIntentHelper::RESULT_CLIENT_ERROR = IActivity::RESULT_FIRST_USER + 1;

const Int32 CRecognizerIntentHelper::RESULT_SERVER_ERROR = IActivity::RESULT_FIRST_USER + 2;

const Int32 CRecognizerIntentHelper::RESULT_NETWORK_ERROR = IActivity::RESULT_FIRST_USER + 3;

const Int32 CRecognizerIntentHelper::RESULT_AUDIO_ERROR = IActivity::RESULT_FIRST_USER + 4;

const String CRecognizerIntentHelper::EXTRA_RESULTS("android.speech.extra.RESULTS");

const String CRecognizerIntentHelper::EXTRA_CONFIDENCE_SCORES("android.speech.extra.CONFIDENCE_SCORES");

const String CRecognizerIntentHelper::DETAILS_META_DATA("android.speech.DETAILS");

const String CRecognizerIntentHelper::ACTION_GET_LANGUAGE_DETAILS("android.speech.action.GET_LANGUAGE_DETAILS");

const String CRecognizerIntentHelper::EXTRA_ONLY_RETURN_LANGUAGE_PREFERENCE("android.speech.extra.ONLY_RETURN_LANGUAGE_PREFERENCE");

const String CRecognizerIntentHelper::EXTRA_LANGUAGE_PREFERENCE("android.speech.extra.LANGUAGE_PREFERENCE");

const String CRecognizerIntentHelper::EXTRA_SUPPORTED_LANGUAGES("android.speech.extra.SUPPORTED_LANGUAGES");

CAR_INTERFACE_IMPL(CRecognizerIntentHelper, Singleton, IRecognizerIntent);
CAR_SINGLETON_IMPL(CRecognizerIntentHelper)

ECode CRecognizerIntentHelper::GetVoiceDetailsIntent(
    /* [in] */ IContext* pContext,
    /* [out] */ IIntent** ppRet)
{
    VALIDATE_NOT_NULL(ppRet);

    AutoPtr<IIntent> voiceSearchIntent;
    CIntent::New(ACTION_WEB_SEARCH, (IIntent**)&voiceSearchIntent);

    AutoPtr<IPackageManager> pm;
    pContext->GetPackageManager((IPackageManager**)&pm);
    AutoPtr<IResolveInfo> ri;
    pm->ResolveActivity(voiceSearchIntent, IPackageManager::GET_META_DATA, (IResolveInfo**)&ri);

    AutoPtr<IActivityInfo> activityInfo;
    ri->GetActivityInfo((IActivityInfo**)&activityInfo);
    AutoPtr<IBundle> metaData;
/*
    if (activityInfo == NULL) {
        activityInfo->GetMetaData((IBundle**)&metaData);
    }
    if (ri == NULL || (activityInfo == NULL || metaData == NULL)) {
*/
    if (ri == NULL || activityInfo == NULL) {
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

