// wuweizuo automatic build .cpp file from .java file.

#include "LocationProviderFactory.h"

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Content {
namespace Browser {

//=====================================================================
//                LocationProviderFactory::InnerRunnable
//=====================================================================
LocationProviderFactory::InnerRunnable::InnerRunnable(
    /* [in] */ LocationProviderFactory* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

ECode LocationProviderFactory::InnerRunnable::Run()
{
    // ==================before translated======================
    // updateNewLocation(location);
    assert(0);
    return NOERROR;
}

//=====================================================================
//            LocationProviderFactory::LocationProviderImpl
//=====================================================================
const String LocationProviderFactory::LocationProviderImpl::TAG("LocationProvider");

LocationProviderFactory::LocationProviderImpl::LocationProviderImpl(
    /* [in] */ IContext* context)
{
    // ==================before translated======================
    // mContext = context;
}

ECode LocationProviderFactory::LocationProviderImpl::Start(
    /* [in] */ Boolean gpsEnabled)
{
    // ==================before translated======================
    // unregisterFromLocationUpdates();
    // registerForLocationUpdates(gpsEnabled);
    assert(0);
    return NOERROR;
}

ECode LocationProviderFactory::LocationProviderImpl::Stop()
{
    // ==================before translated======================
    // unregisterFromLocationUpdates();
    assert(0);
    return NOERROR;
}

Boolean LocationProviderFactory::LocationProviderImpl::IsRunning()
{
    // ==================before translated======================
    // return mIsRunning;
    assert(0);
    return FALSE;
}

ECode LocationProviderFactory::LocationProviderImpl::OnLocationChanged(
    /* [in] */ ILocation* location)
{
    VALIDATE_NOT_NULL(location);
    // ==================before translated======================
    // // Callbacks from the system location sevice are queued to this thread, so it's
    // // possible that we receive callbacks after unregistering. At this point, the
    // // native object will no longer exist.
    // if (mIsRunning) {
    //     updateNewLocation(location);
    // }
    assert(0);
    return NOERROR;
}

ECode LocationProviderFactory::LocationProviderImpl::OnStatusChanged(
    /* [in] */ const String& provider,
    /* [in] */ Int32 status,
    /* [in] */ IBundle* extras)
{
    VALIDATE_NOT_NULL(extras);
    assert(0);
    return NOERROR;
}

ECode LocationProviderFactory::LocationProviderImpl::OnProviderEnabled(
    /* [in] */ const String& provider)
{
    assert(0);
    return NOERROR;
}

ECode LocationProviderFactory::LocationProviderImpl::OnProviderDisabled(
    /* [in] */ const String& provider)
{
    assert(0);
    return NOERROR;
}

ECode LocationProviderFactory::LocationProviderImpl::UpdateNewLocation(
    /* [in] */ ILocation* location)
{
    VALIDATE_NOT_NULL(location);
    // ==================before translated======================
    // LocationProviderAdapter.newLocationAvailable(
    //         location.getLatitude(), location.getLongitude(),
    //         location.getTime() / 1000.0,
    //         location.hasAltitude(), location.getAltitude(),
    //         location.hasAccuracy(), location.getAccuracy(),
    //         location.hasBearing(), location.getBearing(),
    //         location.hasSpeed(), location.getSpeed());
    assert(0);
    return NOERROR;
}

ECode LocationProviderFactory::LocationProviderImpl::EnsureLocationManagerCreated()
{
    // ==================before translated======================
    // if (mLocationManager != null) return;
    // mLocationManager = (LocationManager) mContext.getSystemService(
    //         Context.LOCATION_SERVICE);
    // if (mLocationManager == null) {
    //     Log.e(TAG, "Could not get location manager.");
    // }
    assert(0);
    return NOERROR;
}

ECode LocationProviderFactory::LocationProviderImpl::RegisterForLocationUpdates(
    /* [in] */ Boolean isGpsEnabled)
{
    // ==================before translated======================
    // ensureLocationManagerCreated();
    // if (usePassiveOneShotLocation()) return;
    //
    // assert !mIsRunning;
    // mIsRunning = true;
    //
    // // We're running on the main thread. The C++ side is responsible to
    // // bounce notifications to the Geolocation thread as they arrive in the mainLooper.
    // try {
    //     Criteria criteria = new Criteria();
    //     mLocationManager.requestLocationUpdates(0, 0, criteria, this,
    //             ThreadUtils.getUiThreadLooper());
    //     if (isGpsEnabled) {
    //         criteria.setAccuracy(Criteria.ACCURACY_FINE);
    //         mLocationManager.requestLocationUpdates(0, 0, criteria, this,
    //                 ThreadUtils.getUiThreadLooper());
    //     }
    // } catch (SecurityException e) {
    //     Log.e(TAG, "Caught security exception registering for location updates from " +
    //         "system. This should only happen in DumpRenderTree.");
    // } catch (IllegalArgumentException e) {
    //     Log.e(TAG, "Caught IllegalArgumentException registering for location updates.");
    // }
    assert(0);
    return NOERROR;
}

ECode LocationProviderFactory::LocationProviderImpl::UnregisterFromLocationUpdates()
{
    // ==================before translated======================
    // if (mIsRunning) {
    //     mIsRunning = false;
    //     mLocationManager.removeUpdates(this);
    // }
    assert(0);
    return NOERROR;
}

Boolean LocationProviderFactory::LocationProviderImpl::UsePassiveOneShotLocation()
{
    // ==================before translated======================
    // if (!isOnlyPassiveLocationProviderEnabled()) return false;
    //
    // // Do not request a location update if the only available location provider is
    // // the passive one. Make use of the last known location and call
    // // onLocationChanged directly.
    // final Location location = mLocationManager.getLastKnownLocation(
    //         LocationManager.PASSIVE_PROVIDER);
    // if (location != null) {
    //     ThreadUtils.runOnUiThread(new Runnable() {
    //         @Override
    //         public void run() {
    //             updateNewLocation(location);
    //         }
    //     });
    // }
    // return true;
    assert(0);
    return FALSE;
}

Boolean LocationProviderFactory::LocationProviderImpl::IsOnlyPassiveLocationProviderEnabled()
{
    // ==================before translated======================
    // List<String> providers = mLocationManager.getProviders(true);
    // return providers != null && providers.size() == 1
    //         && providers.get(0).equals(LocationManager.PASSIVE_PROVIDER);
    assert(0);
    return FALSE;
}

//=====================================================================
//                       LocationProviderFactory
//=====================================================================
AutoPtr<LocationProviderFactory::LocationProvider> LocationProviderFactory::sProviderImpl;

ECode LocationProviderFactory::SetLocationProviderImpl(
    /* [in] */  LocationProviderFactory)
{
    // ==================before translated======================
    // assert sProviderImpl == null;
    // sProviderImpl = provider;
    assert(0);
    return NOERROR;
}

AutoPtr<LocationProvider> LocationProviderFactory::Get(
    /* [in] */ IContext* context)
{
    // ==================before translated======================
    // if (sProviderImpl == null) {
    //     sProviderImpl = new LocationProviderImpl(context);
    // }
    // return sProviderImpl;
    assert(0);
    AutoPtr<LocationProvider> empty;
    return empty;
}

LocationProviderFactory::LocationProviderFactory()
{
}

} // namespace Browser
} // namespace Content
} // namespace Webkit
} // namespace Droid
} // namespace Elastos


