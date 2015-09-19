
#ifndef __ELASTOS_DROID_WEBKIT_ANDROIDWEBVIEW_AWGEOLOCATIONPERMISSIONS_H__
#define __ELASTOS_DROID_WEBKIT_ANDROIDWEBVIEW_AWGEOLOCATIONPERMISSIONS_H__

// import android.content.SharedPreferences;
// import android.webkit.ValueCallback;

// import org.chromium.base.ThreadUtils;
// import org.chromium.net.GURLUtils;

// import java.util.HashSet;
// import java.util.Set;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace AndroidWebview {

/**
 * This class is used to manage permissions for the WebView's Geolocation JavaScript API.
 *
 * Callbacks are posted on the UI thread.
 */
class AwGeolocationPermissions
{
private:
    class GetAllowedRunnable
        : public Object
        , public IRunnable
    {
    public:
        GetAllowedRunnable(
            /* [in] */ AwGeolocationPermissions* owner,
            /* [in] */ IValueCallback* callback,
            /* [in] */ Boolean finalAllowed);

        CARAPI Run();

    private:
        AwGeolocationPermissions* mOwner;
        IValueCallback* mCallback;
        Boolean mFinalAllowed;
    };

    class GetOriginsRunnable
        : public Object
        , public IRunnable
    {
    public:
        GetOriginsRunnable(
            /* [in] */ AwGeolocationPermissions* owner,
            /* [in] */ IValueCallback* callback,
            /* [in] */ Set<String>* origins);

        CARAPI Run();

    private:
        AwGeolocationPermissions* mOwner;
        IValueCallback* mCallback;
        Set<String>* mOrigins
    };

public:
    AwGeolocationPermissions(
        /* [in] */ ISharedPreferences* sharedPreferences);

    /**
     * Set one origin to be allowed.
     */
    CARAPI_(void) Allow(
        /* [in] */ String origin);

    /**
     * Set one origin to be denied.
     */
    CARAPI_(void) Deny(
        /* [in] */ String origin);

    /**
     * Clear the stored permission for a particular origin.
     */
    CARAPI_(void) Clear(
        /* [in] */ String origin);

    /**
     * Clear stored permissions for all origins.
     */
    CARAPI_(void) ClearAll();

    /**
     * Synchronous method to get if an origin is set to be allowed.
     */
    CARAPI_(Boolean) IsOriginAllowed(
        /* [in] */ String origin);

    /**
     * Returns true if the origin is either set to allowed or denied.
     */
    CARAPI_(Boolean) HasOrigin(
        /* [in] */ String origin);

    /**
     * Asynchronous method to get if an origin set to be allowed.
     */
    CARAPI_(void) GetAllowed(
        /* [in] */ String origin,
        /* [in] */ const IValueCallback* callback);

    /**
     * Async method to get the domains currently allowed or denied.
     */
    CARAPI_(void) GetOrigins(
        /* [in] */ const IValueCallback* callback);

private:
    /**
     * Get the domain of an URL using the GURL library.
     */
    CARAPI_(String) GetOriginKey(
        /* [in] */ String url);

private:
    static const String PREF_PREFIX;
    const AutoPtr<ISharedPreferences> mSharedPreferences;
};

} // namespace AndroidWebview
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif//__ELASTOS_DROID_WEBKIT_ANDROIDWEBVIEW_AWGEOLOCATIONPERMISSIONS_H__
