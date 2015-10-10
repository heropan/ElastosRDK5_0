
#ifndef __ELASTOS_DROID_WEBKIT_WEBTEXTVIEW_H__
#define __ELASTOS_DROID_WEBKIT_WEBTEXTVIEW_H__

#include "elastos/droid/ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Webkit {

// TODO: Move these to a better place.
class WebTextView
{
public:
    static CARAPI_(String) UrlForAutoCompleteData(
        /* [in] */ const String& urlString);

public:
    // Types used with setType.  Keep in sync with CachedInput.h
    static const Int32 NORMAL_TEXT_FIELD = 0;
    static const Int32 TEXT_AREA = 1;
    static const Int32 PASSWORD = 2;
    static const Int32 SEARCH = 3;
    static const Int32 EMAIL = 4;
    static const Int32 NUMBER = 5;
    static const Int32 TELEPHONE = 6;
    static const Int32 URL = 7;

    static const Int32 FORM_NOT_AUTOFILLABLE = -1;

private:
    static const String LOGTAG;
};

} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_WEBKIT_WEBTEXTVIEW_H__
