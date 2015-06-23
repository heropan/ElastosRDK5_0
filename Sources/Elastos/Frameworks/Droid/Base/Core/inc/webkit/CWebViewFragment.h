
#ifndef __CWEBVIEWFRAGMENT_H__
#define __CWEBVIEWFRAGMENT_H__

#include "_CWebViewFragment.h"
#include "ext/frameworkext.h"

#include "app/Fragment.h"
#include "webkit/WebViewFragment.h"

namespace Elastos {
namespace Droid {
namespace Webkit {

CarClass(CWebViewFragment) , public WebViewFragment
{
public:
    CARAPI constructor();

    CAR_INTERFACE_DECL();

    IFRAGMENT_METHODS_DECL();

    CARAPI GetWebView(
        /* [out] */ IWebView** webView);
};

} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif // __CWEBVIEWFRAGMENT_H__
