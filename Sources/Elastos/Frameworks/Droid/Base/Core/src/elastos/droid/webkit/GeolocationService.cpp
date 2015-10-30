#include "elastos/droid/webkit/GeolocationService.h"

using Elastos::Droid::Location::ILocationProvider;

namespace Elastos {
namespace Droid {
namespace Webkit {

struct ElaGeolocationServiceCallback GeolocationService::sElaGeolocationServiceCallback =
{
    &GeolocationService::CreateGeolocationService,
    &GeolocationService::Start,
    &GeolocationService::Stop,
    &GeolocationService::SetEnableGps
};
const String GeolocationService::TAG("geolocationService");

/**
 * Constructor
 * @param context The context from which we obtain the system service.
 * @param nativeObject The native object to which this object will report position updates and
 *     errors.
 */
GeolocationService::GeolocationService(
    /* [in] */ IContext* context,
    /* [in] */ Int64 nativeObject)
    : mNativeObject(nativeObject)
    , mIsGpsEnabled(FALSE)
    , mIsRunning(FALSE)
    , mIsNetworkProviderAvailable(FALSE)
    , mIsGpsProviderAvailable(FALSE)
{
    // Register newLocationAvailable with platform service.
    context->GetSystemService(IContext::LOCATION_SERVICE, (IInterface**)&mLocationManager);
    if (mLocationManager == NULL) {
//        Log.e(TAG, "Could not get location manager.");
    }
}

CAR_INTERFACE_IMPL(GeolocationService, ILocationListener);

/**
 * Start listening for location updates.
 */
Boolean GeolocationService::Start()
{
    RegisterForLocationUpdates();
    mIsRunning = TRUE;
    return mIsNetworkProviderAvailable || mIsGpsProviderAvailable;
}

/**
 * Stop listening for location updates.
 */
void GeolocationService::Stop()
{
    UnregisterFromLocationUpdates();
    mIsRunning = FALSE;
}

/**
 * Sets whether to use the GPS.
 * @param enable Whether to use the GPS.
 */
void GeolocationService::SetEnableGps(
    /* [in] */ Boolean enable)
{
    if (mIsGpsEnabled != enable) {
        mIsGpsEnabled = enable;
        if (mIsRunning) {
            // There's no way to unregister from a single provider, so we can
            // only unregister from all, then reregister with all but the GPS.
            UnregisterFromLocationUpdates();
            RegisterForLocationUpdates();
            // Check that the providers are still available after we re-register.
            MaybeReportError(String("The last location provider is no longer available"));
        }
    }
}

AutoPtr<IInterface> GeolocationService::CreateGeolocationService(
    /* [in] */ IInterface* context,
    /* [in] */ Int64 nativeObject)
{
    AutoPtr<GeolocationService> result = new GeolocationService((IContext*)context, nativeObject);
    return result;
}

Boolean GeolocationService::Start(
    /* [in] */ IInterface* obj)
{
    AutoPtr<GeolocationService> gService = (GeolocationService*)obj;
    return gService->Start();
}

void GeolocationService::Stop(
    /* [in] */ IInterface* obj)
{
    AutoPtr<GeolocationService> gService = (GeolocationService*)obj;
    gService->Stop();
}

void GeolocationService::SetEnableGps(
    /* [in] */ IInterface* obj,
    /* [in] */ Boolean enable)
{
    AutoPtr<GeolocationService> gService = (GeolocationService*)obj;
    gService->SetEnableGps(enable);
}

/**
 * LocationListener implementation.
 * Called when the location has changed.
 * @param location The new location, as a Location object.
 */
ECode GeolocationService::OnLocationChanged(
    /* [in] */ ILocation* location)
{
    // Callbacks from the system location sevice are queued to this thread, so it's possible
    // that we receive callbacks after unregistering. At this point, the native object will no
    // longer exist.
    if (mIsRunning) {
        NativeNewLocationAvailable(mNativeObject, location);
    }

    return NOERROR;
}

/**
 * LocationListener implementation.
 * Called when the provider status changes.
 * @param provider The name of the provider.
 * @param status The new status of the provider.
 * @param extras an optional Bundle with provider specific data.
 */
ECode GeolocationService::OnStatusChanged(
    /* [in] */ const String& providerName,
    /* [in] */ Int32 status,
    /* [in] */ IBundle* extras)
{
    Boolean isAvailable = (status == ILocationProvider::AVAILABLE);
    if (ILocationManager::NETWORK_PROVIDER.Equals(providerName)) {
        mIsNetworkProviderAvailable = isAvailable;
    }
    else if (ILocationManager::GPS_PROVIDER.Equals(providerName)) {
        mIsGpsProviderAvailable = isAvailable;
    }

    MaybeReportError(String("The last location provider is no longer available"));

    return NOERROR;
}

/**
 * LocationListener implementation.
 * Called when the provider is enabled.
 * @param provider The name of the location provider that is now enabled.
 */
ECode GeolocationService::OnProviderEnabled(
    /* [in] */ const String& providerName)
{
    // No need to notify the native side. It's enough to start sending
    // valid position fixes again.
    if (ILocationManager::NETWORK_PROVIDER.Equals(providerName)) {
        mIsNetworkProviderAvailable = TRUE;
    }
    else if (ILocationManager::GPS_PROVIDER.Equals(providerName)) {
        mIsGpsProviderAvailable = TRUE;
    }

    return NOERROR;
}

/**
 * LocationListener implementation.
 * Called when the provider is disabled.
 * @param provider The name of the location provider that is now disabled.
 */
ECode GeolocationService::OnProviderDisabled(
    /* [in] */ const String& providerName)
{
    if (ILocationManager::NETWORK_PROVIDER.Equals(providerName)) {
        mIsNetworkProviderAvailable = FALSE;
    }
    else if (ILocationManager::GPS_PROVIDER.Equals(providerName)) {
        mIsGpsProviderAvailable = FALSE;
    }

    MaybeReportError(String("The last location provider was disabled"));

    return NOERROR;
}

/**
 * Registers this object with the location service.
 */
void GeolocationService::RegisterForLocationUpdates()
{
    //try {
        // Registration may fail if providers are not present on the device.
        //try {
            mLocationManager->RequestLocationUpdates(ILocationManager::NETWORK_PROVIDER, 0, 0, this);
            mIsNetworkProviderAvailable = TRUE;
        //} catch(IllegalArgumentException e) { }
        if (mIsGpsEnabled) {
            //try {
                mLocationManager->RequestLocationUpdates(ILocationManager::GPS_PROVIDER, 0, 0, this);
                mIsGpsProviderAvailable = TRUE;
            //} catch(IllegalArgumentException e) { }
        }
    //} catch(SecurityException e) {
    //    Log.e(TAG, "Caught security exception registering for location updates from system. " +
    //        "This should only happen in DumpRenderTree.");
    //}
}

/**
 * Unregisters this object from the location service.
 */
void GeolocationService::UnregisterFromLocationUpdates()
{
    mLocationManager->RemoveUpdates(this);
    mIsNetworkProviderAvailable = FALSE;
    mIsGpsProviderAvailable = FALSE;
}

/**
 * Reports an error if neither the network nor the GPS provider is available.
 */
void GeolocationService::MaybeReportError(
    /* [in] */ const String& message)
{
    // Callbacks from the system location sevice are queued to this thread, so it's possible
    // that we receive callbacks after unregistering. At this point, the native object will no
    // longer exist.
    if (mIsRunning && !mIsNetworkProviderAvailable && !mIsGpsProviderAvailable) {
        NativeNewErrorAvailable(mNativeObject, message);
    }
}

// Native functions
void GeolocationService::NativeNewLocationAvailable(
    /* [in] */ Int64 nativeObject,
    /* [in] */ ILocation* location)
{
    Elastos_GeolocationServiceBridge_NewLocationAvailable(nativeObject, location);
}

void GeolocationService::NativeNewErrorAvailable(
    /* [in] */ Int64 nativeObject,
    /* [in] */ const String& message)
{
    Elastos_GeolocationServiceBridge_NewErrorAvailable(nativeObject, message);
}

ECode GeolocationService::ToString(
    /* [out] */ String* info)
{
    VALIDATE_NOT_NULL(info);
    *info = "GeolocationService";
    return NOERROR;
}

} // namespace Webkit
} // namepsace Droid
} // namespace Elastos
