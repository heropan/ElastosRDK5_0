// wuweizuo automatic build .cpp file from .java file.

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Content {
namespace Browser {

//=====================================================================
//            LocationProviderFactory::LocationProviderImpl
//=====================================================================
const String LocationProviderFactory::LocationProviderImpl::TAG("LocationProvider");

LocationProviderFactory::LocationProviderImpl::LocationProviderImpl(
    /* [in] */ IContext* context)
{
    /*
    mContext = context;
    */
}

ECode LocationProviderFactory::LocationProviderImpl::Start(
    /* [in] */ Boolean gpsEnabled)
{
    /*
    unregisterFromLocationUpdates();
    registerForLocationUpdates(gpsEnabled);
    */
    return NOERROR;
}

ECode LocationProviderFactory::LocationProviderImpl::Stop()
{
    /*
    unregisterFromLocationUpdates();
    */
    return NOERROR;
}

Boolean LocationProviderFactory::LocationProviderImpl::IsRunning()
{
    /*
    return mIsRunning;
    */
}

ECode LocationProviderFactory::LocationProviderImpl::OnLocationChanged(
    /* [in] */ ILocation* location)
{
    VALIDATE_NOT_NULL(location);
    /*
    // Callbacks from the system location sevice are queued to this thread, so it's
    // possible that we receive callbacks after unregistering. At this point, the
    // native object will no longer exist.
    if (mIsRunning) {
        updateNewLocation(location);
    }
    */
    return NOERROR;
}

ECode LocationProviderFactory::LocationProviderImpl::OnStatusChanged(
    /* [in] */ String provider,
    /* [in] */ Int32 status,
    /* [in] */ IBundle* extras)
{
    VALIDATE_NOT_NULL(extras);
    return NOERROR;
}

ECode LocationProviderFactory::LocationProviderImpl::OnProviderEnabled(
    /* [in] */ String provider)
{
    return NOERROR;
}

ECode LocationProviderFactory::LocationProviderImpl::OnProviderDisabled(
    /* [in] */ String provider)
{
    return NOERROR;
}

ECode LocationProviderFactory::LocationProviderImpl::UpdateNewLocation(
    /* [in] */ ILocation* location)
{
    VALIDATE_NOT_NULL(location);
    /*
    LocationProviderAdapter.newLocationAvailable(
            location.getLatitude(), location.getLongitude(),
            location.getTime() / 1000.0,
            location.hasAltitude(), location.getAltitude(),
            location.hasAccuracy(), location.getAccuracy(),
            location.hasBearing(), location.getBearing(),
            location.hasSpeed(), location.getSpeed());
    */
    return NOERROR;
}

ECode LocationProviderFactory::LocationProviderImpl::EnsureLocationManagerCreated()
{
    /*
    if (mLocationManager != null) return;
    mLocationManager = (LocationManager) mContext.getSystemService(
            Context.LOCATION_SERVICE);
    if (mLocationManager == null) {
        Log.e(TAG, "Could not get location manager.");
    }
    */
    return NOERROR;
}

ECode LocationProviderFactory::LocationProviderImpl::RegisterForLocationUpdates(
    /* [in] */ Boolean isGpsEnabled)
{
    /*
    ensureLocationManagerCreated();
    if (usePassiveOneShotLocation()) return;


    assert !mIsRunning;
    mIsRunning = true;


    // We're running on the main thread. The C++ side is responsible to
    // bounce notifications to the Geolocation thread as they arrive in the mainLooper.
    try {
        Criteria criteria = new Criteria();
        mLocationManager.requestLocationUpdates(0, 0, criteria, this,
                ThreadUtils.getUiThreadLooper());
        if (isGpsEnabled) {
            criteria.setAccuracy(Criteria.ACCURACY_FINE);
            mLocationManager.requestLocationUpdates(0, 0, criteria, this,
                    ThreadUtils.getUiThreadLooper());
        }
    } catch (SecurityException e) {
        Log.e(TAG, "Caught security exception registering for location updates from " +
            "system. This should only happen in DumpRenderTree.");
    } catch (IllegalArgumentException e) {
        Log.e(TAG, "Caught IllegalArgumentException registering for location updates.");
    }
    */
    return NOERROR;
}

ECode LocationProviderFactory::LocationProviderImpl::UnregisterFromLocationUpdates()
{
    /*
    if (mIsRunning) {
        mIsRunning = false;
        mLocationManager.removeUpdates(this);
    }
    */
    return NOERROR;
}

Boolean LocationProviderFactory::LocationProviderImpl::UsePassiveOneShotLocation()
{
    /*
    if (!isOnlyPassiveLocationProviderEnabled()) return false;


    // Do not request a location update if the only available location provider is
    // the passive one. Make use of the last known location and call
    // onLocationChanged directly.
    final Location location = mLocationManager.getLastKnownLocation(
            LocationManager.PASSIVE_PROVIDER);
    if (location != null) {
        ThreadUtils.runOnUiThread(// [wuweizuo auto add note here: new InnerRunnable(this)] new Runnable() {
            @Override
            public void run() {
                updateNewLocation(location);
            }
        });
    }
    return true;
    */
}

Boolean LocationProviderFactory::LocationProviderImpl::IsOnlyPassiveLocationProviderEnabled()
{
    /*
    List<String> providers = mLocationManager.getProviders(true);
    return providers != null && providers.size() == 1
            && providers.get(0).equals(LocationManager.PASSIVE_PROVIDER);
    */
}

//=====================================================================
//                LocationProviderFactory::InnerRunnable
//=====================================================================
LocationProviderFactory::InnerRunnable::InnerRunnable(
    /* [in] */ LocationProviderFactory* owner)
    : mOwner(owner)
{
    /*
    mOwner = owner;
    */
}

ECode LocationProviderFactory::InnerRunnable::Run()
{
    /*
    updateNewLocation(location);
    */
    return NOERROR;
}

//=====================================================================
//                       LocationProviderFactory
//=====================================================================
AutoPtr<LocationProviderFactory::LocationProvider> LocationProviderFactory::sProviderImpl;

ECode LocationProviderFactory::SetLocationProviderImpl(
    /* [in] */  LocationProviderFactory)
{
    /*
    assert sProviderImpl == null;
    sProviderImpl = provider;
    */
    return NOERROR;
}

AutoPtr<LocationProvider> LocationProviderFactory::Get(
    /* [in] */ IContext* context)
{
    /*
    if (sProviderImpl == null) {
        sProviderImpl = new LocationProviderImpl(context);
    }
    return sProviderImpl;
    */
}

LocationProviderFactory::LocationProviderFactory()
{
}

} // namespace Browser
} // namespace Content
} // namespace Webkit
} // namespace Droid
} // namespace Elastos


