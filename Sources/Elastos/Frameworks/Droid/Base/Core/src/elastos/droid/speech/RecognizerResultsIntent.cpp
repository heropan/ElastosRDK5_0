#include "elastos/droid/speech/RecognizerResultsIntent.h"
//#include "elastos/droid/ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Speech {

const String RecognizerResultsIntent::ACTION_VOICE_SEARCH_RESULTS("android.speech.action.VOICE_SEARCH_RESULTS");
const String RecognizerResultsIntent::EXTRA_VOICE_SEARCH_RESULT_STRINGS("android.speech.extras.VOICE_SEARCH_RESULT_STRINGS");
const String RecognizerResultsIntent::EXTRA_VOICE_SEARCH_RESULT_URLS("android.speech.extras.VOICE_SEARCH_RESULT_URLS");
const String RecognizerResultsIntent::EXTRA_VOICE_SEARCH_RESULT_HTML("android.speech.extras.VOICE_SEARCH_RESULT_HTML");
const String RecognizerResultsIntent::EXTRA_VOICE_SEARCH_RESULT_HTML_BASE_URLS("android.speech.extras.VOICE_SEARCH_RESULT_HTML_BASE_URLS");
const String RecognizerResultsIntent::EXTRA_VOICE_SEARCH_RESULT_HTTP_HEADERS("android.speech.extras.EXTRA_VOICE_SEARCH_RESULT_HTTP_HEADERS");
const String RecognizerResultsIntent::URI_SCHEME_INLINE("inline");

RecognizerResultsIntent::RecognizerResultsIntent()
{
    //Not for instantiating
}

} // namespace Speech
} // namepsace Droid
} // namespace Elastos
