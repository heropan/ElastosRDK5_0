
#include "elastos/droid/webkit/CWebViewFactory.h"
#include "elastos/droid/webkit/WebViewFactory.h"

namespace Elastos {
namespace Droid {
namespace Webkit {

CAR_INTERFACE_IMPL(CWebViewFactory, Object, IWebViewFactory);

CAR_SINGLETON_IMPL(CWebViewFactory);

ECode CWebViewFactory::GetWebViewPackageName(
        /* [out] */ String* packageName)
{
    VALIDATE_NOT_NULL(packageName);
    *packageName = WebViewFactory::GetWebViewPackageName();
    return NOERROR;
}

ECode CWebViewFactory::GetLoadedPackageInfo(
    /* [out] */ IPackageInfo** packageInfo)
{
    VALIDATE_NOT_NULL(packageInfo);
    *packageInfo = WebViewFactory::GetLoadedPackageInfo();
    REFCOUNT_ADD(*packageInfo);
    return NOERROR;
}

ECode CWebViewFactory::PrepareWebViewInZygote()
{
    WebViewFactory::PrepareWebViewInZygote();
    return NOERROR;
}

ECode CWebViewFactory::PrepareWebViewInSystemServer()
{
    WebViewFactory::PrepareWebViewInSystemServer();
    return NOERROR;
}

ECode CWebViewFactory::OnWebViewUpdateInstalled()
{
    WebViewFactory::OnWebViewUpdateInstalled();
    return NOERROR;
}

} // namespace Webkit
} // namepsace Droid
} // namespace Elastos
