
#include "webkit/CWebViewHelper.h"
#include "webkit/WebView.h"

namespace Elastos {
namespace Droid {
namespace Webkit {

ECode CWebViewHelper::EnablePlatformNotifications()
{
    WebView::EnablePlatformNotifications();
    return NOERROR;
}

ECode CWebViewHelper::DisablePlatformNotifications()
{
    WebView::DisablePlatformNotifications();
    return NOERROR;
}

ECode CWebViewHelper::FindAddress(
    /* [in] */ const String& addr,
    /* [out] */ String * outAddr)
{
    VALIDATE_NOT_NULL(outAddr);
    *outAddr = WebView::FindAddress(addr);

    return NOERROR;
}

ECode CWebViewHelper::GetPluginList(
    /* [out] */ IPluginList ** pluginList)
{
    VALIDATE_NOT_NULL(pluginList);
    AutoPtr<IPluginList> _pluginList = WebView::GetPluginList();
    *pluginList = _pluginList.Get();
    REFCOUNT_ADD(*pluginList);

    return NOERROR;
}

} // namespace Webkit
} // namepsace Droid
} // namespace Elastos