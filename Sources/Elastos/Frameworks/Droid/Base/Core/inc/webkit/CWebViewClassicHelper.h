
#ifndef __ELASTOS_DROID_WEBKIT_WEBVIEWCLASSICHELPER_H__
#define __ELASTOS_DROID_WEBKIT_WEBVIEWCLASSICHELPER_H__

#include "_CWebViewClassicHelper.h"

namespace Elastos {
namespace Droid {
namespace Webkit {

CarClass(CWebViewClassicHelper)
{
public:
    CARAPI FromWebView(
        /* [in] */ IWebView* webView,
        /* [out] */ IWebViewClassic** wvc);

    CARAPI EnablePlatformNotifications();

    CARAPI DisablePlatformNotifications();

    /**
     * Return the first substring consisting of the address of a physical
     * location. Currently, only addresses in the United States are detected,
     * and consist of:
     * - a house number
     * - a street name
     * - a street type (Road, Circle, etc), either spelled out or abbreviated
     * - a city name
     * - a state or territory, either spelled out or two-letter abbr.
     * - an optional 5 digit or 9 digit zip code.
     *
     * All names must be correctly capitalized, and the zip code, if present,
     * must be valid for the state. The street type must be a standard USPS
     * spelling or abbreviation. The state or territory must also be spelled
     * or abbreviated using USPS standards. The house number may not exceed
     * five digits.
     * @param addr The string to search for addresses.
     *
     * @return the address, or if no address is found, return null.
     */
    CARAPI FindAddress(
        /* [in] */ const String& addr,
        /* [out] */ String* outAddr);

    /**
     * Return the first substring consisting of the address of a physical
     * location. Currently, only addresses in the United States are detected,
     * and consist of:
     * - a house number
     * - a street name
     * - a street type (Road, Circle, etc), either spelled out or abbreviated
     * - a city name
     * - a state or territory, either spelled out or two-letter abbr.
     * - an optional 5 digit or 9 digit zip code.
     *
     * Names are optionally capitalized, and the zip code, if present,
     * must be valid for the state. The street type must be a standard USPS
     * spelling or abbreviation. The state or territory must also be spelled
     * or abbreviated using USPS standards. The house number may not exceed
     * five digits.
     * @param addr The string to search for addresses.
     * @param caseInsensitive addr Set to true to make search ignore case.
     *
     * @return the address, or if no address is found, return null.
     */
    CARAPI FindAddress(
        /* [in] */ const String& addr,
        /* [in] */ Boolean caseInsensitive,
        /* [out] */ String* outAddr);

    /**
     * See {@link WebView#getPluginList()}
     */
    //@Deprecated
    //synchronized
    CARAPI GetPluginList(
        /* [out] */ IPluginList** pluginList);

    // Note: must be called before first WebViewClassic is created.
    CARAPI SetShouldMonitorWebCoreThread();
};

} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_WEBKIT_WEBVIEWCLASSICHELPER_H__
