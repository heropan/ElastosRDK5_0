
#ifndef __ELASTOS_DROID_WEBKIT_WEBICONDATABASE_H__
#define __ELASTOS_DROID_WEBKIT_WEBICONDATABASE_H__

#include "ext/frameworkext.h"
#include "webkit/WebViewFactory.h"

using Elastos::Droid::Graphics::IBitmap;
using Elastos::Droid::Content::IContentResolver;

namespace Elastos {
namespace Droid {
namespace Webkit {

/**
 * Functions for manipulating the icon database used by WebView.
 * These functions require that a WebView be constructed before being invoked
 * and WebView.getIconDatabase() will return a WebIconDatabase object. This
 * WebIconDatabase object is a single instance and all methods operate on that
 * single object.
 */
class WebIconDatabase
{
public:
    /**
     * Get the global instance of WebIconDatabase.
     * @return A single instance of WebIconDatabase. It will be the same
     *         instance for the current process each time this method is
     *         called.
     */
    static CARAPI_(AutoPtr<IWebIconDatabase>) GetInstance()
    {
        // XXX: Must be created in the UI thread.
        AutoPtr<IWebIconDatabase> temp;
        WebViewFactory::GetProvider()->GetWebIconDatabase((IWebIconDatabase**)&temp);
        return temp;
    }
};

} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_WEBKIT_WEBICONDATABASE_H__
