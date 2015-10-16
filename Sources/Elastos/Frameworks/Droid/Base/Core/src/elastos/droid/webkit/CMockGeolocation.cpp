
#include "elastos/droid/webkit/CWebViewCore.h"
#include "elastos/droid/webkit/CMockGeolocation.h"

#ifdef __cplusplus
extern "C"
{
#endif
    extern void Elastos_MockGeolocation_setUseMock(IInterface* obj);
    extern void Elastos_MockGeolocation_setPosition(IInterface* obj, Double, Double, Double);
    extern void Elastos_MockGeolocation_setError(IInterface* obj, Int32, const Elastos::String&);
    extern void Elastos_MockGeolocation_setPermission(IInterface* obj, Boolean);
#ifdef __cplusplus
}
#endif

namespace Elastos {
namespace Droid {
namespace Webkit {

ECode CMockGeolocation::constructor(
    /* [in] */ IWebViewCore* webViewCore)
{
    mWebViewCore = webViewCore;
    return  NOERROR;
}

/**
 * Sets use of the mock Geolocation client. Also resets that client.
 */
ECode CMockGeolocation::SetUseMock()
{
    NativeSetUseMock(mWebViewCore);
    return NOERROR;
}

/**
 * Set the position for the mock Geolocation service.
 */
ECode CMockGeolocation::SetPosition(
    /* [in] */ Double latitude,
    /* [in] */ Double longitude,
    /* [in] */ Double accuracy)
{
    // This should only ever be called on the WebKit thread.
    NativeSetPosition(mWebViewCore, latitude, longitude, accuracy);
    return NOERROR;
}

/**
 * Set the error for the mock Geolocation service.
 */
ECode CMockGeolocation::SetError(
    /* [in] */ Int32 code,
    /* [in] */ const String& message)
{
    // This should only ever be called on the WebKit thread.
    NativeSetError(mWebViewCore, code, message);
    return NOERROR;
}

ECode CMockGeolocation::SetPermission(
    /* [in] */ Boolean allow)
{
    // This should only ever be called on the WebKit thread.
    NativeSetPermission(mWebViewCore, allow);
    return NOERROR;
}

// Native functions
void CMockGeolocation::NativeSetUseMock(
    /* [in] */ IWebViewCore* webViewCore)
{
    Elastos_MockGeolocation_setUseMock(webViewCore);
}

void CMockGeolocation::NativeSetPosition(
    /* [in] */ IWebViewCore* webViewCore,
    /* [in] */ Double latitude,
    /* [in] */ Double longitude,
    /* [in] */ Double accuracy)
{
    Elastos_MockGeolocation_setPosition(webViewCore, latitude, longitude, accuracy);
}

void CMockGeolocation::NativeSetError(
    /* [in] */ IWebViewCore* webViewCore,
    /* [in] */ Int32 code,
    /* [in] */ const String& message)
{
    Elastos_MockGeolocation_setError(webViewCore, code, message);
}

void CMockGeolocation::NativeSetPermission(
    /* [in] */ IWebViewCore* webViewCore,
    /* [in] */ Boolean allow)
{
    Elastos_MockGeolocation_setPermission(webViewCore, allow);
}

} // namespace Webkit
} // namespace Droid
} // namespace Elastos
