#include "elastos/droid/speech/RecognizerIntent.h"
#include "elastos/droid/content/CIntent.h"
#include "elastos/droid/content/CComponentName.h"
#include "elastos/droid/content/pm/CResolveInfo.h"
#include "elastos/droid/content/pm/CActivityInfo.h"
#include "elastos/droid/content/CComponentName.h"
#include "elastos/droid/ext/frameworkext.h"

using Elastos::Droid::Content::CIntent;
using Elastos::Droid::Content::Pm::IResolveInfo;
using Elastos::Droid::Content::Pm::CActivityInfo;
using Elastos::Droid::Content::Pm::CResolveInfo;
using Elastos::Droid::Content::IComponentName;
using Elastos::Droid::Content::CComponentName;
using Elastos::Droid::Os::IBundle;

namespace Elastos {
namespace Droid {
namespace Speech {

const String RecognizerIntent::EXTRA_CALLING_PACKAGE("calling_package");

const String RecognizerIntent::ACTION_RECOGNIZE_SPEECH("android.speech.action.RECOGNIZE_SPEECH");

const String RecognizerIntent::ACTION_WEB_SEARCH("android.speech.action.WEB_SEARCH");

const String RecognizerIntent::ACTION_VOICE_SEARCH_HANDS_FREE("android.speech.action.VOICE_SEARCH_HANDS_FREE");

const String RecognizerIntent::EXTRA_SECURE("android.speech.extras.EXTRA_SECURE");

const String RecognizerIntent::EXTRA_SPEECH_INPUT_MINIMUM_LENGTH_MILLIS("android.speech.extras.SPEECH_INPUT_MINIMUM_LENGTH_MILLIS");

const String RecognizerIntent::EXTRA_SPEECH_INPUT_COMPLETE_SILENCE_LENGTH_MILLIS("android.speech.extras.SPEECH_INPUT_COMPLETE_SILENCE_LENGTH_MILLIS");

const String RecognizerIntent::EXTRA_SPEECH_INPUT_POSSIBLY_COMPLETE_SILENCE_LENGTH_MILLIS("android.speech.extras.SPEECH_INPUT_POSSIBLY_COMPLETE_SILENCE_LENGTH_MILLIS");

const String RecognizerIntent::EXTRA_LANGUAGE_MODEL("android.speech.extra.LANGUAGE_MODEL");

const String RecognizerIntent::LANGUAGE_MODEL_FREE_FORM("free_form");

const String RecognizerIntent::LANGUAGE_MODEL_WEB_SEARCH("web_search");

const String RecognizerIntent::EXTRA_PROMPT("android.speech.extra.PROMPT");

const String RecognizerIntent::EXTRA_LANGUAGE("android.speech.extra.LANGUAGE");

const String RecognizerIntent::EXTRA_ORIGIN("android.speech.extra.ORIGIN");

const String RecognizerIntent::EXTRA_MAX_RESULTS("android.speech.extra.MAX_RESULTS");

const String RecognizerIntent::EXTRA_WEB_SEARCH_ONLY("android.speech.extra.WEB_SEARCH_ONLY");

const String RecognizerIntent::EXTRA_PARTIAL_RESULTS("android.speech.extra.PARTIAL_RESULTS");

const String RecognizerIntent::EXTRA_RESULTS_PENDINGINTENT("android.speech.extra.RESULTS_PENDINGINTENT");

const String RecognizerIntent::EXTRA_RESULTS_PENDINGINTENT_BUNDLE("android.speech.extra.RESULTS_PENDINGINTENT_BUNDLE");

const Int32 RecognizerIntent::RESULT_NO_MATCH = IActivity::RESULT_FIRST_USER;

const Int32 RecognizerIntent::RESULT_CLIENT_ERROR = IActivity::RESULT_FIRST_USER + 1;

const Int32 RecognizerIntent::RESULT_SERVER_ERROR = IActivity::RESULT_FIRST_USER + 2;

const Int32 RecognizerIntent::RESULT_NETWORK_ERROR = IActivity::RESULT_FIRST_USER + 3;

const Int32 RecognizerIntent::RESULT_AUDIO_ERROR = IActivity::RESULT_FIRST_USER + 4;

const String RecognizerIntent::EXTRA_RESULTS("android.speech.extra.RESULTS");

const String RecognizerIntent::EXTRA_CONFIDENCE_SCORES("android.speech.extra.CONFIDENCE_SCORES");

const String RecognizerIntent::DETAILS_META_DATA("android.speech.DETAILS");

const String RecognizerIntent::ACTION_GET_LANGUAGE_DETAILS("android.speech.action.GET_LANGUAGE_DETAILS");

const String RecognizerIntent::EXTRA_ONLY_RETURN_LANGUAGE_PREFERENCE("android.speech.extra.ONLY_RETURN_LANGUAGE_PREFERENCE");

const String RecognizerIntent::EXTRA_LANGUAGE_PREFERENCE("android.speech.extra.LANGUAGE_PREFERENCE");

const String RecognizerIntent::EXTRA_SUPPORTED_LANGUAGES("android.speech.extra.SUPPORTED_LANGUAGES");

CAR_INTERFACE_IMPL(RecognizerIntent::RecognizerIntent, Object, IRecognizerIntent);

RecognizerIntent::RecognizerIntent()
{}

RecognizerIntent::~RecognizerIntent()
{}

ECode RecognizerIntent::constructor()
{
    return NOERROR;
}

AutoPtr<IIntent> RecognizerIntent::GetVoiceDetailsIntent(
    /* [in] */ IContext* context)
{
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
        return NULL;
    }

    String className;
    metaData->GetString(DETAILS_META_DATA, &className);
    if (className.IsNull()) {
        return NULL;
    }

    AutoPtr<IIntent> detailsIntent;
    CIntent::New(ACTION_GET_LANGUAGE_DETAILS, (IIntent**)&detailsIntent);
    String packageName;
    ((CActivityInfo*)( activityInfo.Get() ) )->GetPackageName(&packageName);
    AutoPtr<IComponentName> cn;
    CComponentName::New(packageName, className, (IComponentName**)&cn);
    detailsIntent->SetComponent(cn.Get());
    return detailsIntent;
}

} // namespace Speech
} // namepsace Droid
} // namespace Elastos
