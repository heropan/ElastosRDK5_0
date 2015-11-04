
#ifndef __ELASTOS_DROID_WEBKIT_CWEBVIEW_H__
#define __ELASTOS_DROID_WEBKIT_CWEBVIEW_H__

#include "_Elastos_Droid_Webkit_CWebView.h"
#include "elastos/droid/webkit/WebView.h"

namespace Elastos {
namespace Droid {
namespace Webkit {

CarClass(CWebView), public WebView
{
public:
    CAR_OBJECT_DECL()

    // CARAPI_(PInterface) Probe(
    //     /* [in]  */ REIID riid);

// TODO    IVIEW_METHODS_DECL()
// TODO    IVIEWGROUP_METHODS_DECL()
// TODO    IVIEWPARENT_METHODS_DECL()
// TODO    IVIEWMANAGER_METHODS_DECL()
// TODO    IDRAWABLECALLBACK_METHODS_DECL()
// TODO    IKEYEVENTCALLBACK_METHODS_DECL()
// TODO    IACCESSIBILITYEVENTSOURCE_METHODS_DECL()

    static CARAPI_(void) PostInvalidate(
        /* [in] */ IInterface* obj);
};

} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_WEBKIT_CWEBVIEW_H__
