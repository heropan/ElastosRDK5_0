
#include "webkit/WebTextView.h"

using Elastos::Net::CURL;
using Elastos::Net::IURL;

namespace Elastos {
namespace Droid {
namespace Webkit {

// Types used with setType.  Keep in sync with CachedInput.h
const Int32 WebTextView::NORMAL_TEXT_FIELD;
const Int32 WebTextView::TEXT_AREA;
const Int32 WebTextView::PASSWORD;
const Int32 WebTextView::SEARCH;
const Int32 WebTextView::EMAIL;
const Int32 WebTextView::NUMBER;
const Int32 WebTextView::TELEPHONE;
const Int32 WebTextView::URL;

const Int32 WebTextView::FORM_NOT_AUTOFILLABLE;

const String WebTextView::LOGTAG("WebTextView");

String WebTextView::UrlForAutoCompleteData(
    /* [in] */ const String& urlString)
{
    // Remove any fragment or query string.
    AutoPtr<IURL> url;
    //try {
    CURL::New(urlString, (IURL**)&url);
    //} catch (MalformedURLException e) {
    //    Log.e(LOGTAG, "Unable to parse URL "+url);
    //}

    if (url != NULL) {
        String protocol;
        String host;
        String path;
        url->GetProtocol(&protocol);
        url->GetHost(&host);
        url->GetPath(&path);
        return protocol + "://" + host + path;
    }
    else {
        return String(NULL);
    }
}

} // namespace Webkit
} // namepsace Droid
} // namespace Elastos