
#include "webkit/CWebViewClassicHelper.h"

namespace Elastos {
namespace Droid {
namespace Webkit {

ECode CWebViewClassicHelper::FromWebView(
    /* [in] */ IWebView* webView,
    /* [out] */ IWebViewClassic** wvc)
{
	return E_NOT_IMPLEMENTED;
}

ECode CWebViewClassicHelper::EnablePlatformNotifications()
{
    return E_NOT_IMPLEMENTED;
}

ECode CWebViewClassicHelper::DisablePlatformNotifications()
{
    return E_NOT_IMPLEMENTED;
}

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
ECode CWebViewClassicHelper::FindAddress(
    /* [in] */ const String& addr,
    /* [out] */ String* outAddr)
{
	return E_NOT_IMPLEMENTED;
}

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
ECode CWebViewClassicHelper::FindAddressEx(
    /* [in] */ const String& addr,
    /* [in] */ Boolean caseInsensitive,
    /* [out] */ String* outAddr)
{
	return E_NOT_IMPLEMENTED;
}

/**
 * See {@link WebView#getPluginList()}
 */
//@Deprecated
//synchronized
ECode CWebViewClassicHelper::GetPluginList(
    /* [out] */ IPluginList** pluginList)
{
	return E_NOT_IMPLEMENTED;
}

// Note: must be called before first WebViewClassic is created.
ECode CWebViewClassicHelper::SetShouldMonitorWebCoreThread()
{
	return E_NOT_IMPLEMENTED;
}

}
}
}