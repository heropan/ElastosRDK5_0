
#ifndef __ELASTOS_DROID_WEBKIT_GEOLOCATIONPERMISSIONS_H__
#define __ELASTOS_DROID_WEBKIT_GEOLOCATIONPERMISSIONS_H__

#include "elastos/droid/ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Webkit {

/**
 * This class is used to manage permissions for the WebView's Geolocation
 * JavaScript API.
 *
 * Geolocation permissions are applied to an origin, which consists of the
 * host, scheme and port of a URI. In order for web content to use the
 * Geolocation API, permission must be granted for that content's origin.
 *
 * This class stores Geolocation permissions. An origin's permission state can
 * be either allowed or denied. This class uses Strings to represent
 * an origin.
 *
 * When an origin attempts to use the Geolocation API, but no permission state
 * is currently set for that origin,
 * {@link WebChromeClient#onGeolocationPermissionsShowPrompt(String,GeolocationPermissions.Callback) WebChromeClient.onGeolocationPermissionsShowPrompt()}
 * is called. This allows the permission state to be set for that origin.
 *
 * The methods of this class can be used to modify and interrogate the stored
 * Geolocation permissions at any time.
 */
// Within WebKit, Geolocation permissions may be applied either temporarily
// (for the duration of the page) or permanently. This class deals only with
// permanent permissions.
class GeolocationPermissions
    : public Object
    , public IGeolocationPermissions
{
public:
    CAR_INTERFACE_DECL();

    CARAPI constructor();

    /**
     * Gets the singleton instance of this class.
     *
     * @return the singleton {@link GeolocationPermissions} instance
     */
    static CARAPI_(AutoPtr<IGeolocationPermissions>) GetInstance();

    /**
     * Gets the set of origins for which Geolocation permissions are stored.
     *
     * @param callback a {@link ValueCallback} to receive the result of this
     *                 request. This object's
     *                 {@link ValueCallback#onReceiveValue(T) onReceiveValue()}
     *                 method will be invoked asynchronously with a set of
     *                 Strings containing the origins for which Geolocation
     *                 permissions are stored.
     */
    // Note that we represent the origins as strings. These are created using
    // WebCore::SecurityOrigin::toString(). As long as all 'HTML 5 modules'
    // (Database, Geolocation etc) do so, it's safe to match up origins based
    // on this string.
    virtual CARAPI GetOrigins(
        /* [in] */ IValueCallback* callback);

    /**
     * Gets the Geolocation permission state for the specified origin.
     *
     * @param origin the origin for which Geolocation permission is requested
     * @param callback a {@link ValueCallback} to receive the result of this
     *                 request. This object's
     *                 {@link ValueCallback#onReceiveValue(T) onReceiveValue()}
     *                 method will be invoked asynchronously with a boolean
     *                 indicating whether or not the origin can use the
     *                 Geolocation API.
     */
    virtual CARAPI GetAllowed(
        /* [in] */ const String& origin,
        /* [in] */ IValueCallback* callback);

    /**
     * Clears the Geolocation permission state for the specified origin.
     *
     * @param origin the origin for which Geolocation permissions are cleared
     */
    virtual CARAPI Clear(
        /* [in] */ const String& origin);

    /**
     * Allows the specified origin to use the Geolocation API.
     *
     * @param origin the origin for which Geolocation API use is allowed
     */
    virtual CARAPI Allow(
        /* [in] */ const String& origin);

    /**
     * Clears the Geolocation permission state for all origins.
     */
    virtual CARAPI ClearAll();

    CARAPI ToString(
        /* [out] */ String* info);
};

} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_WEBKIT_GEOLOCATIONPERMISSIONS_H__
