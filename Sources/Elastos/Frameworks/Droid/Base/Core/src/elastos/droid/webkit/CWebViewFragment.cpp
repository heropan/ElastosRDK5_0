
#include "elastos/droid/webkit/CWebViewFragment.h"

namespace Elastos {
namespace Droid {
namespace Webkit {

ECode CWebViewFragment::constructor()
{
    return NOERROR;
}

IFRAGMENT_METHODS_IMPL(CWebViewFragment, WebViewFragment);

PInterface CWebViewFragment::Probe(
        /* [in]  */ REIID riid)
{
    return _CWebViewFragment::Probe(riid);
}

UInt32 CWebViewFragment::AddRef()
{
    return _CWebViewFragment::AddRef();
}

UInt32 CWebViewFragment::Release()
{
    return _CWebViewFragment::Release();
}

ECode CWebViewFragment::GetInterfaceID(
    /* [in] */ IInterface *Object,
    /* [out] */ InterfaceID *IID)
{
    return _CWebViewFragment::GetInterfaceID(Object, IID);
}

ECode CWebViewFragment::GetWebView(
    /* [out] */ IWebView** webView)
{
    VALIDATE_NOT_NULL(webView);
    *webView = WebViewFragment::GetWebView();
    REFCOUNT_ADD(*webView);
    return NOERROR;
}

} // namespace Webkit
} // namespace Droid
} // namespace Elastos
