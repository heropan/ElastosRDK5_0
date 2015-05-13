
#include "location/LocationManager.h"
#include "location/CLocationRequest.h"
#include "location/CProviderProperties.h"
#include "location/CLocationProvider.h"
#include "location/CGeofence.h"
#include "location/CGpsStatus.h"
#include "os/Build.h"
#include <elastos/Math.h>
#include <elastos/StringUtils.h>

using Elastos::Core::StringUtils;
using Elastos::Droid::Os::Build;
using Elastos::Droid::Content::Pm::IApplicationInfo;

namespace Elastos {
namespace Droid {
namespace Location {

const String LocationManager::TAG("LocationManager");
/**
 * @hide - hide this constructor because it has a parameter
 * of type ILocationManager, which is a system private class. The
 * right way to create an instance of this class is using the
 * factory Context.getSystemService.
 */
LocationManager::LocationManager(
    /* [in] */ IContext* context,
    /* [in] */ IILocationManager* service)
{
    Init(context, service);
}

/**
 * Returns a list of the names of all known location providers.
 * <p>All providers are returned, including ones that are not permitted to
 * be accessed by the calling activity or are currently disabled.
 *
 * @return list of Strings containing names of the provider
 */
ECode LocationManager::GetAllProviders(
    /* [out] */ IObjectContainer** allProvders)
{
    assert(allProvders != NULL);
    *allProvders = NULL;
    return mService->GetAllProviders(allProvders);
    return NOERROR;
}

/**
 * Returns a list of the names of location providers.
 *
 * @param enabledOnly if true then only the providers which are currently
 * enabled are returned.
 * @return list of Strings containing names of the providers
 */
ECode LocationManager::GetProviders(
    /* [in] */ Boolean enabledOnly,
    /* [out] */ IObjectContainer** providers)
{
    assert(providers != NULL);
    *providers = NULL;
    return mService->GetProviders(NULL, enabledOnly, providers);
}

/**
 * Returns the information associated with the location provider of the
 * given name, or null if no provider exists by that name.
 *
 * @param name the provider name
 * @return a LocationProvider, or null
 *
 * @throws IllegalArgumentException if name is null or does not exist
 * @throws SecurityException if the caller is not permitted to access the
 * given provider.
 */
ECode LocationManager::GetProvider(
    /* [in] */ const String& name,
    /* [out] */ ILocationProvider** provider)
{
    assert(provider != NULL);
    FAIL_RETURN(CheckProvider(name));

    AutoPtr<IProviderProperties> properties;
    ECode ec = mService->GetProviderProperties(name, (IProviderProperties**)&properties);
    if (FAILED(ec) || properties == NULL) {
        *provider = NULL;
        return NOERROR;
    }

    AutoPtr<ILocationProvider> temp = CreateProvider(name, properties);
    *provider = temp;
    INTERFACE_ADDREF(*provider);

    return NOERROR;
}

/**
 * Returns a list of the names of LocationProviders that satisfy the given
 * criteria, or null if none do.  Only providers that are permitted to be
 * accessed by the calling activity will be returned.
 *
 * @param criteria the criteria that the returned providers must match
 * @param enabledOnly if true then only the providers which are currently
 * enabled are returned.
 * @return list of Strings containing names of the providers
 */
ECode LocationManager::GetProviders(
    /* [in] */ ICriteria* criteria,
    /* [in] */ Boolean enabledOnly,
    /* [out] */ IObjectContainer** providers)
{
    assert(providers != NULL);
    *providers = NULL;
    FAIL_RETURN(CheckCriteria(criteria));

    return mService->GetProviders(criteria, enabledOnly, providers);
}

/**
 * Returns the name of the provider that best meets the given criteria. Only providers
 * that are permitted to be accessed by the calling activity will be
 * returned.  If several providers meet the criteria, the one with the best
 * accuracy is returned.  If no provider meets the criteria,
 * the criteria are loosened in the following sequence:
 *
 * <ul>
 * <li> power requirement
 * <li> accuracy
 * <li> bearing
 * <li> speed
 * <li> altitude
 * </ul>
 *
 * <p> Note that the requirement on monetary cost is not removed
 * in this process.
 *
 * @param criteria the criteria that need to be matched
 * @param enabledOnly if true then only a provider that is currently enabled is returned
 * @return name of the provider that best matches the requirements
 */
ECode LocationManager::GetBestProvider(
    /* [in] */ ICriteria* criteria,
    /* [in] */ Boolean enabledOnly,
    /* [out] */ String* provider)
{
    assert(provider != NULL);
    *provider = "";
    FAIL_RETURN(CheckCriteria(criteria));

    return mService->GetBestProvider(criteria, enabledOnly, provider);
}

/**
 * Register for location updates using the named provider, and a
 * pending intent.
 *
 * <p>See {@link #requestLocationUpdates(long, float, Criteria, PendingIntent)}
 * for more detail on how to use this method.
 *
 * @param provider the name of the provider with which to register
 * @param minTime minimum time interval between location updates, in milliseconds
 * @param minDistance minimum distance between location updates, in meters
 * @param listener a {@link LocationListener} whose
 * {@link LocationListener#onLocationChanged} method will be called for
 * each location update
 *
 * @throws IllegalArgumentException if provider is null or doesn't exist
 * on this device
 * @throws IllegalArgumentException if listener is null
 * @throws RuntimeException if the calling thread has no Looper
 * @throws SecurityException if no suitable permission is present
 */
ECode LocationManager::RequestLocationUpdates(
    /* [in] */ const String& provider,
    /* [in] */ Int64 minTime,
    /* [in] */ Float minDistance,
    /* [in] */ ILocationListener* listener)
{
    FAIL_RETURN(CheckProvider(provider));
    FAIL_RETURN(CheckListener(listener));

    AutoPtr<ILocationRequest> request = CLocationRequest::CreateFromDeprecatedProvider(
            provider, minTime, minDistance, FALSE);
    RequestLocationUpdates(request, listener, NULL, NULL);

    return NOERROR;
}

/**
 * Register for location updates using the named provider, and a callback on
 * the specified looper thread.
 *
 * <p>See {@link #requestLocationUpdates(long, float, Criteria, PendingIntent)}
 * for more detail on how to use this method.
 *
 * @param provider the name of the provider with which to register
 * @param minTime minimum time interval between location updates, in milliseconds
 * @param minDistance minimum distance between location updates, in meters
 * @param listener a {@link LocationListener} whose
 * {@link LocationListener#onLocationChanged} method will be called for
 * each location update
 * @param looper a Looper object whose message queue will be used to
 * implement the callback mechanism, or null to make callbacks on the calling
 * thread
 *
 * @throws IllegalArgumentException if provider is null or doesn't exist
 * @throws IllegalArgumentException if listener is null
 * @throws SecurityException if no suitable permission is present
 */
ECode LocationManager::RequestLocationUpdates(
    /* [in] */ const String& provider,
    /* [in] */ Int64 minTime,
    /* [in] */ Float minDistance,
    /* [in] */ ILocationListener* listener,
    /* [in] */ ILooper* looper)
{
    FAIL_RETURN(CheckProvider(provider));
    FAIL_RETURN(CheckListener(listener));

    AutoPtr<ILocationRequest> request = CLocationRequest::CreateFromDeprecatedProvider(
            provider, minTime, minDistance, FALSE);
    RequestLocationUpdates(request, listener, looper, NULL);

    return NOERROR;
}

/**
 * Register for location updates using a Criteria, and a callback
 * on the specified looper thread.
 *
 * <p>See {@link #requestLocationUpdates(long, float, Criteria, PendingIntent)}
 * for more detail on how to use this method.
 *
 * @param minTime minimum time interval between location updates, in milliseconds
 * @param minDistance minimum distance between location updates, in meters
 * @param criteria contains parameters for the location manager to choose the
 * appropriate provider and parameters to compute the location
 * @param listener a {@link LocationListener} whose
 * {@link LocationListener#onLocationChanged} method will be called for
 * each location update
 * @param looper a Looper object whose message queue will be used to
 * implement the callback mechanism, or null to make callbacks on the calling
 * thread
 *
 * @throws IllegalArgumentException if criteria is null
 * @throws IllegalArgumentException if listener is null
 * @throws SecurityException if no suitable permission is present
 */
ECode LocationManager::RequestLocationUpdates(
    /* [in] */ Int64 minTime,
    /* [in] */ Float minDistance,
    /* [in] */ ICriteria* criteria,
    /* [in] */ ILocationListener* listener,
    /* [in] */ ILooper* looper)
{
    FAIL_RETURN(CheckCriteria(criteria));
    FAIL_RETURN(CheckListener(listener));

    AutoPtr<ILocationRequest> request = CLocationRequest::CreateFromDeprecatedCriteria(
            criteria, minTime, minDistance, FALSE);
    RequestLocationUpdates(request, listener, looper, NULL);

    return NOERROR;
}

/**
 * Register for location updates using the named provider, and a
 * pending intent.
 *
 * <p>See {@link #requestLocationUpdates(long, float, Criteria, PendingIntent)}
 * for more detail on how to use this method.
 *
 * @param provider the name of the provider with which to register
 * @param minTime minimum time interval between location updates, in milliseconds
 * @param minDistance minimum distance between location updates, in meters
 * @param intent a {@link PendingIntent} to be sent for each location update
 *
 * @throws IllegalArgumentException if provider is null or doesn't exist
 * on this device
 * @throws IllegalArgumentException if intent is null
 * @throws SecurityException if no suitable permission is present
 */
ECode LocationManager::RequestLocationUpdates(
    /* [in] */ const String& provider,
    /* [in] */ Int64 minTime,
    /* [in] */ Float minDistance,
    /* [in] */ IPendingIntent* intent)
{
    FAIL_RETURN(CheckProvider(provider));
    FAIL_RETURN(CheckPendingIntent(intent));

    AutoPtr<ILocationRequest> request = CLocationRequest::CreateFromDeprecatedProvider(
            provider, minTime, minDistance, FALSE);
    RequestLocationUpdates(request, NULL, NULL, intent);

    return NOERROR;
}

/**
 * Register for location updates using a Criteria and pending intent.
 *
 * <p>The <code>requestLocationUpdates()</code> and
 * <code>requestSingleUpdate()</code> register the current activity to be
 * updated periodically by the named provider, or by the provider matching
 * the specified {@link Criteria}, with location and status updates.
 *
 * <p> It may take a while to receive the first location update. If
 * an immediate location is required, applications may use the
 * {@link #getLastKnownLocation(String)} method.
 *
 * <p> Location updates are received either by {@link LocationListener}
 * callbacks, or by broadcast intents to a supplied {@link PendingIntent}.
 *
 * <p> If the caller supplied a pending intent, then location updates
 * are sent with a key of {@link #KEY_LOCATION_CHANGED} and a
 * {@link android.location.Location} value.
 *
 * <p> The location update interval can be controlled using the minTime parameter.
 * The elapsed time between location updates will never be less than
 * minTime, although it can be more depending on the Location Provider
 * implementation and the update interval requested by other applications.
 *
 * <p> Choosing a sensible value for minTime is important to conserve
 * battery life. Each location update requires power from
 * GPS, WIFI, Cell and other radios. Select a minTime value as high as
 * possible while still providing a reasonable user experience.
 * If your application is not in the foreground and showing
 * location to the user then your application should avoid using an active
 * provider (such as {@link #NETWORK_PROVIDER} or {@link #GPS_PROVIDER}),
 * but if you insist then select a minTime of 5 * 60 * 1000 (5 minutes)
 * or greater. If your application is in the foreground and showing
 * location to the user then it is appropriate to select a faster
 * update interval.
 *
 * <p> The minDistance parameter can also be used to control the
 * frequency of location updates. If it is greater than 0 then the
 * location provider will only send your application an update when
 * the location has changed by at least minDistance meters, AND
 * at least minTime milliseconds have passed. However it is more
 * difficult for location providers to save power using the minDistance
 * parameter, so minTime should be the primary tool to conserving battery
 * life.
 *
 * <p> If your application wants to passively observe location
 * updates triggered by other applications, but not consume
 * any additional power otherwise, then use the {@link #PASSIVE_PROVIDER}
 * This provider does not actively turn on or modify active location
 * providers, so you do not need to be as careful about minTime and
 * minDistance. However if your application performs heavy work
 * on a location update (such as network activity) then you should
 * select non-zero values for minTime and/or minDistance to rate-limit
 * your update frequency in the case another application enables a
 * location provider with extremely fast updates.
 *
 * <p>In case the provider is disabled by the user, updates will stop,
 * and a provider availability update will be sent.
 * As soon as the provider is enabled again,
 * location updates will immediately resume and a provider availability
 * update sent. Providers can also send status updates, at any time,
 * with extra's specific to the provider. If a callback was supplied
 * then status and availability updates are via
 * {@link LocationListener#onProviderDisabled},
 * {@link LocationListener#onProviderEnabled} or
 * {@link LocationListener#onStatusChanged}. Alternately, if a
 * pending intent was supplied then status and availability updates
 * are broadcast intents with extra keys of
 * {@link #KEY_PROVIDER_ENABLED} or {@link #KEY_STATUS_CHANGED}.
 *
 * <p> If a {@link LocationListener} is used but with no Looper specified
 * then the calling thread must already
 * be a {@link android.os.Looper} thread such as the main thread of the
 * calling Activity. If a Looper is specified with a {@link LocationListener}
 * then callbacks are made on the supplied Looper thread.
 *
 * <p class="note"> Prior to Jellybean, the minTime parameter was
 * only a hint, and some location provider implementations ignored it.
 * From Jellybean and onwards it is mandatory for Android compatible
 * devices to observe both the minTime and minDistance parameters.
 *
 * @param minTime minimum time interval between location updates, in milliseconds
 * @param minDistance minimum distance between location updates, in meters
 * @param criteria contains parameters for the location manager to choose the
 * appropriate provider and parameters to compute the location
 * @param intent a {@link PendingIntent} to be sent for each location update
 *
 * @throws IllegalArgumentException if criteria is null
 * @throws IllegalArgumentException if intent is null
 * @throws SecurityException if no suitable permission is present
 */
ECode LocationManager::RequestLocationUpdates(
    /* [in] */ Int64 minTime,
    /* [in] */ Float minDistance,
    /* [in] */ ICriteria* criteria,
    /* [in] */ IPendingIntent* intent)
{
    FAIL_RETURN(CheckCriteria(criteria));
    FAIL_RETURN(CheckPendingIntent(intent));

    AutoPtr<ILocationRequest> request = CLocationRequest::CreateFromDeprecatedCriteria(
            criteria, minTime, minDistance, FALSE);
    RequestLocationUpdates(request, NULL, NULL, intent);

    return NOERROR;
}

/**
 * Register for a single location update using the named provider and
 * a callback.
 *
 * <p>See {@link #requestLocationUpdates(long, float, Criteria, PendingIntent)}
 * for more detail on how to use this method.
 *
 * @param provider the name of the provider with which to register
 * @param listener a {@link LocationListener} whose
 * {@link LocationListener#onLocationChanged} method will be called when
 * the location update is available
 * @param looper a Looper object whose message queue will be used to
 * implement the callback mechanism, or null to make callbacks on the calling
 * thread
 *
 * @throws IllegalArgumentException if provider is null or doesn't exist
 * @throws IllegalArgumentException if listener is null
 * @throws SecurityException if no suitable permission is present
 */
ECode LocationManager::RequestSingleUpdate(
    /* [in] */ const String& provider,
    /* [in] */ ILocationListener* listener,
    /* [in] */ ILooper* looper)
{
    FAIL_RETURN(CheckProvider(provider));
    FAIL_RETURN(CheckListener(listener));

    AutoPtr<ILocationRequest> request = CLocationRequest::CreateFromDeprecatedProvider(
            provider, 0, 0, TRUE);
    RequestLocationUpdates(request, listener, looper, NULL);

    return NOERROR;
}

/**
 * Register for a single location update using a Criteria and
 * a callback.
 *
 * <p>See {@link #requestLocationUpdates(long, float, Criteria, PendingIntent)}
 * for more detail on how to use this method.
 *
 * @param criteria contains parameters for the location manager to choose the
 * appropriate provider and parameters to compute the location
 * @param listener a {@link LocationListener} whose
 * {@link LocationListener#onLocationChanged} method will be called when
 * the location update is available
 * @param looper a Looper object whose message queue will be used to
 * implement the callback mechanism, or null to make callbacks on the calling
 * thread
 *
 * @throws IllegalArgumentException if criteria is null
 * @throws IllegalArgumentException if listener is null
 * @throws SecurityException if no suitable permission is present
 */
ECode LocationManager::RequestSingleUpdate(
    /* [in] */ ICriteria* criteria,
    /* [in] */ ILocationListener* listener,
    /* [in] */ ILooper* looper)
{
    FAIL_RETURN(CheckCriteria(criteria));
    FAIL_RETURN(CheckListener(listener));

    AutoPtr<ILocationRequest> request = CLocationRequest::CreateFromDeprecatedCriteria(
            criteria, 0, 0, TRUE);
    RequestLocationUpdates(request, listener, looper, NULL);

    return NOERROR;
}

/**
 * Register for a single location update using a named provider and pending intent.
 *
 * <p>See {@link #requestLocationUpdates(long, float, Criteria, PendingIntent)}
 * for more detail on how to use this method.
 *
 * @param provider the name of the provider with which to register
 * @param intent a {@link PendingIntent} to be sent for the location update
 *
 * @throws IllegalArgumentException if provider is null or doesn't exist
 * @throws IllegalArgumentException if intent is null
 * @throws SecurityException if no suitable permission is present
 */
ECode LocationManager::RequestSingleUpdate(
    /* [in] */ const String& provider,
    /* [in] */ IPendingIntent* intent)
{
    FAIL_RETURN(CheckProvider(provider));
    FAIL_RETURN(CheckPendingIntent(intent));

    AutoPtr<ILocationRequest> request = CLocationRequest::CreateFromDeprecatedProvider(
            provider, 0, 0, TRUE);
    RequestLocationUpdates(request, NULL, NULL, intent);

    return NOERROR;
}

/**
 * Register for a single location update using a Criteria and pending intent.
 *
 * <p>See {@link #requestLocationUpdates(long, float, Criteria, PendingIntent)}
 * for more detail on how to use this method.
 *
 * @param criteria contains parameters for the location manager to choose the
 * appropriate provider and parameters to compute the location
 * @param intent a {@link PendingIntent} to be sent for the location update
 *
 * @throws IllegalArgumentException if provider is null or doesn't exist
 * @throws IllegalArgumentException if intent is null
 * @throws SecurityException if no suitable permission is present
 */
ECode LocationManager::RequestSingleUpdate(
    /* [in] */ ICriteria* criteria,
    /* [in] */ IPendingIntent* intent)
{
    FAIL_RETURN(CheckCriteria(criteria));
    FAIL_RETURN(CheckPendingIntent(intent));

    AutoPtr<ILocationRequest> request = CLocationRequest::CreateFromDeprecatedCriteria(
            criteria, 0, 0, TRUE);
    RequestLocationUpdates(request, NULL, NULL, intent);

    return NOERROR;
}

/**
 * Register for fused location updates using a LocationRequest and callback.
 *
 * <p>Upon a location update, the system delivers the new {@link Location} to the
 * provided {@link LocationListener}, by calling its {@link
 * LocationListener#onLocationChanged} method.</p>
 *
 * <p>The system will automatically select and enable the best providers
 * to compute a location for your application. It may use only passive
 * locations, or just a single location source, or it may fuse together
 * multiple location sources in order to produce the best possible
 * result, depending on the quality of service requested in the
 * {@link LocationRequest}.
 *
 * <p>LocationRequest can be null, in which case the system will choose
 * default, low power parameters for location updates. You will occasionally
 * receive location updates as available, without a major power impact on the
 * system. If your application just needs an occasional location update
 * without any strict demands, then pass a null LocationRequest.
 *
 * <p>Only one LocationRequest can be registered for each unique callback
 * or pending intent. So a subsequent request with the same callback or
 * pending intent will over-write the previous LocationRequest.
 *
 * <p> If a pending intent is supplied then location updates
 * are sent with a key of {@link #KEY_LOCATION_CHANGED} and a
 * {@link android.location.Location} value. If a callback is supplied
 * then location updates are made using the
 * {@link LocationListener#onLocationChanged} callback, on the specified
 * Looper thread. If a {@link LocationListener} is used
 * but with a null Looper then the calling thread must already
 * be a {@link android.os.Looper} thread (such as the main thread) and
 * callbacks will occur on this thread.
 *
 * <p> Provider status updates and availability updates are deprecated
 * because the system is performing provider fusion on the applications
 * behalf. So {@link LocationListener#onProviderDisabled},
 * {@link LocationListener#onProviderEnabled}, {@link LocationListener#onStatusChanged}
 * will not be called, and intents with extra keys of
 * {@link #KEY_PROVIDER_ENABLED} or {@link #KEY_STATUS_CHANGED} will not
 * be received.
 *
 * <p> To unregister for Location updates, use: {@link #removeUpdates(LocationListener)}.
 *
 * @param request quality of service required, null for default low power
 * @param listener a {@link LocationListener} whose
 * {@link LocationListener#onLocationChanged} method will be called when
 * the location update is available
 * @param looper a Looper object whose message queue will be used to
 * implement the callback mechanism, or null to make callbacks on the calling
 * thread
 *
 * @throws IllegalArgumentException if listener is null
 * @throws SecurityException if no suitable permission is present
 *
 * @hide
 */
ECode LocationManager::RequestLocationUpdates(
    /* [in] */ ILocationRequest* request,
    /* [in] */ ILocationListener* listener,
    /* [in] */ ILooper* looper)
{
    FAIL_RETURN(CheckListener(listener));
    RequestLocationUpdates(request, listener, looper, NULL);

    return NOERROR;
}

/**
 * Register for fused location updates using a LocationRequest and a pending intent.
 *
 * <p>Upon a location update, the system delivers the new {@link Location} with your provided
 * {@link PendingIntent}, as the value for {@link LocationManager#KEY_LOCATION_CHANGED}
 * in the intent's extras.</p>
 *
 * <p> To unregister for Location updates, use: {@link #removeUpdates(PendingIntent)}.
 *
 * <p> See {@link #requestLocationUpdates(LocationRequest, LocationListener, Looper)}
 * for more detail.
 *
 * @param request quality of service required, null for default low power
 * @param intent a {@link PendingIntent} to be sent for the location update
 *
 * @throws IllegalArgumentException if intent is null
 * @throws SecurityException if no suitable permission is present
 *
 * @hide
 */
ECode LocationManager::RequestLocationUpdates(
    /* [in] */ ILocationRequest* request,
    /* [in] */ IPendingIntent* intent)
{
    FAIL_RETURN(CheckPendingIntent(intent));
    RequestLocationUpdates(request, NULL, NULL, intent);

    return NOERROR;
}

/**
 * Removes all location updates for the specified LocationListener.
 *
 * <p>Following this call, updates will no longer
 * occur for this listener.
 *
 * @param listener listener object that no longer needs location updates
 * @throws IllegalArgumentException if listener is null
 */
ECode LocationManager::RemoveUpdates(
    /* [in] */ ILocationListener* listener)
{
    FAIL_RETURN(CheckListener(listener));
    String packageName;
    mContext->GetPackageName(&packageName);
    AutoPtr<IILocationListener> transport;
    {
        Mutex::Autolock lock(mutexListeners);
        HashMap<AutoPtr<ILocationListener>, AutoPtr<IILocationListener> >::Iterator it
                = mListeners.Find(listener);
        if (it != mListeners.End()) {
            transport = it->mSecond;
            mListeners.Erase(it);
        }
    }

    if (transport == NULL) return NOERROR;

    return mService->RemoveUpdates((IILocationListener*)transport->Probe(EIID_IILocationListener), NULL, packageName);
}

/**
 * Removes all location updates for the specified pending intent.
 *
 * <p>Following this call, updates will no longer for this pending intent.
 *
 * @param intent pending intent object that no longer needs location updates
 * @throws IllegalArgumentException if intent is null
 */
ECode LocationManager::RemoveUpdates(
    /* [in] */ IPendingIntent* intent)
{
    FAIL_RETURN(CheckPendingIntent(intent));
    String packageName;
    mContext->GetPackageName(&packageName);

    return mService->RemoveUpdates(NULL, intent, packageName);
}

/**
 * Set a proximity alert for the location given by the position
 * (latitude, longitude) and the given radius.
 *
 * <p> When the device
 * detects that it has entered or exited the area surrounding the
 * location, the given PendingIntent will be used to create an Intent
 * to be fired.
 *
 * <p> The fired Intent will have a boolean extra added with key
 * {@link #KEY_PROXIMITY_ENTERING}. If the value is true, the device is
 * entering the proximity region; if false, it is exiting.
 *
 * <p> Due to the approximate nature of position estimation, if the
 * device passes through the given area briefly, it is possible
 * that no Intent will be fired.  Similarly, an Intent could be
 * fired if the device passes very close to the given area but
 * does not actually enter it.
 *
 * <p> After the number of milliseconds given by the expiration
 * parameter, the location manager will delete this proximity
 * alert and no longer monitor it.  A value of -1 indicates that
 * there should be no expiration time.
 *
 * <p> Internally, this method uses both {@link #NETWORK_PROVIDER}
 * and {@link #GPS_PROVIDER}.
 *
 * <p>Before API version 17, this method could be used with
 * {@link android.Manifest.permission#ACCESS_FINE_LOCATION} or
 * {@link android.Manifest.permission#ACCESS_COARSE_LOCATION}.
 * From API version 17 and onwards, this method requires
 * {@link android.Manifest.permission#ACCESS_FINE_LOCATION} permission.
 *
 * @param latitude the latitude of the central point of the
 * alert region
 * @param longitude the longitude of the central point of the
 * alert region
 * @param radius the radius of the central point of the
 * alert region, in meters
 * @param expiration time for this proximity alert, in milliseconds,
 * or -1 to indicate no expiration
 * @param intent a PendingIntent that will be used to generate an Intent to
 * fire when entry to or exit from the alert region is detected
 *
 * @throws SecurityException if {@link android.Manifest.permission#ACCESS_FINE_LOCATION}
 * permission is not present
 */
ECode LocationManager::AddProximityAlert(
    /* [in] */ Double latitude,
    /* [in] */ Double longitude,
    /* [in] */ Float radius,
    /* [in] */ Int64 expiration,
    /* [in] */ IPendingIntent* intent)
{
    FAIL_RETURN(CheckPendingIntent(intent));
    if (expiration < 0) expiration = Elastos::Core::Math::INT64_MAX_VALUE;

    AutoPtr<IGeofence> fence = CGeofence::CreateCircle(latitude, longitude, radius);
    AutoPtr<ILocationRequest> request;
    CLocationRequest::New((ILocationRequest**)&request);
    request->SetExpireIn(expiration);
    //try {
    //    mService.requestGeofence(request, fence, intent, mContext.getPackageName());
    //} catch (RemoteException e) {
    //    Log.e(TAG, "RemoteException", e);
    //}


    String packageName;
    mContext->GetPackageName(&packageName);

    return mService->RequestGeofence(request, fence, intent, packageName);
}

/**
 * Add a geofence with the specified LocationRequest quality of service.
 *
 * <p> When the device
 * detects that it has entered or exited the area surrounding the
 * location, the given PendingIntent will be used to create an Intent
 * to be fired.
 *
 * <p> The fired Intent will have a boolean extra added with key
 * {@link #KEY_PROXIMITY_ENTERING}. If the value is true, the device is
 * entering the proximity region; if false, it is exiting.
 *
 * <p> The geofence engine fuses results from all location providers to
 * provide the best balance between accuracy and power. Applications
 * can choose the quality of service required using the
 * {@link LocationRequest} object. If it is null then a default,
 * low power geo-fencing implementation is used. It is possible to cross
 * a geo-fence without notification, but the system will do its best
 * to detect, using {@link LocationRequest} as a hint to trade-off
 * accuracy and power.
 *
 * <p> The power required by the geofence engine can depend on many factors,
 * such as quality and interval requested in {@link LocationRequest},
 * distance to nearest geofence and current device velocity.
 *
 * @param request quality of service required, null for default low power
 * @param fence a geographical description of the geofence area
 * @param intent pending intent to receive geofence updates
 *
 * @throws IllegalArgumentException if fence is null
 * @throws IllegalArgumentException if intent is null
 * @throws SecurityException if {@link android.Manifest.permission#ACCESS_FINE_LOCATION}
 * permission is not present
 *
 * @hide
 */
ECode LocationManager::AddGeofence(
    /* [in] */ ILocationRequest* request,
    /* [in] */ IGeofence* fence,
    /* [in] */ IPendingIntent* intent)
{
    FAIL_RETURN(CheckPendingIntent(intent));
    FAIL_RETURN(CheckGeofence(fence));

    String packageName;
    mContext->GetPackageName(&packageName);

    return mService->RequestGeofence(request, fence, intent, packageName);
}

/**
 * Removes the proximity alert with the given PendingIntent.
 *
 * <p>Before API version 17, this method could be used with
 * {@link android.Manifest.permission#ACCESS_FINE_LOCATION} or
 * {@link android.Manifest.permission#ACCESS_COARSE_LOCATION}.
 * From API version 17 and onwards, this method requires
 * {@link android.Manifest.permission#ACCESS_FINE_LOCATION} permission.
 *
 * @param intent the PendingIntent that no longer needs to be notified of
 * proximity alerts
 *
 * @throws IllegalArgumentException if intent is null
 * @throws SecurityException if {@link android.Manifest.permission#ACCESS_FINE_LOCATION}
 * permission is not present
 */
ECode LocationManager::RemoveProximityAlert(
    /* [in] */ IPendingIntent* intent)
{
    FAIL_RETURN(CheckPendingIntent(intent));
    String packageName;
    mContext->GetPackageName(&packageName);

    return mService->RemoveGeofence(NULL, intent, packageName);
}

/**
 * Remove a single geofence.
 *
 * <p>This removes only the specified geofence associated with the
 * specified pending intent. All other geofences remain unchanged.
 *
 * @param fence a geofence previously passed to {@link #addGeofence}
 * @param intent a pending intent previously passed to {@link #addGeofence}
 *
 * @throws IllegalArgumentException if fence is null
 * @throws IllegalArgumentException if intent is null
 * @throws SecurityException if {@link android.Manifest.permission#ACCESS_FINE_LOCATION}
 * permission is not present
 *
 * @hide
 */
ECode LocationManager::RemoveGeofence(
    /* [in] */ IGeofence* fence,
    /* [in] */ IPendingIntent* intent)
{
    FAIL_RETURN(CheckPendingIntent(intent));
    FAIL_RETURN(CheckGeofence(fence));
    String packageName;
    mContext->GetPackageName(&packageName);

    return mService->RemoveGeofence(fence, intent, packageName);
}

/**
 * Remove all geofences registered to the specified pending intent.
 *
 * @param intent a pending intent previously passed to {@link #addGeofence}
 *
 * @throws IllegalArgumentException if intent is null
 * @throws SecurityException if {@link android.Manifest.permission#ACCESS_FINE_LOCATION}
 * permission is not present
 *
 * @hide
 */
ECode LocationManager::RemoveAllGeofences(
    /* [in] */ IPendingIntent* intent)
{
    FAIL_RETURN(CheckPendingIntent(intent));
    String packageName;
    mContext->GetPackageName(&packageName);

    return mService->RemoveGeofence(NULL, intent, packageName);
}

/**
 * Returns the current enabled/disabled status of the given provider.
 *
 * <p>If the user has enabled this provider in the Settings menu, true
 * is returned otherwise false is returned
 *
 * @param provider the name of the provider
 * @return true if the provider is enabled
 *
 * @throws IllegalArgumentException if provider is null
 * @throws SecurityException if no suitable permission is present
 */
ECode LocationManager::IsProviderEnabled(
    /* [in] */ const String& provider,
    /* [out] */ Boolean* result)
{
    assert(result != NULL);
    *result = FALSE;
    FAIL_RETURN(CheckProvider(provider));
    return mService->IsProviderEnabled(provider, result);
}

/**
 * Get the last known location.
 *
 * <p>This location could be very old so use
 * {@link Location#getElapsedRealtimeNanos} to calculate its age. It can
 * also return null if no previous location is available.
 *
 * <p>Always returns immediately.
 *
 * @return The last known location, or null if not available
 * @throws SecurityException if no suitable permission is present
 *
 * @hide
 */
ECode LocationManager::GetLastLocation(
    /* [out] */ ILocation** location)
{
    VALIDATE_NOT_NULL(location);
    *location = NULL;
    String packageName;
    mContext->GetPackageName(&packageName);

    return mService->GetLastLocation(NULL, packageName, location);
}

/**
 * Returns a Location indicating the data from the last known
 * location fix obtained from the given provider.
 *
 * <p> This can be done
 * without starting the provider.  Note that this location could
 * be out-of-date, for example if the device was turned off and
 * moved to another location.
 *
 * <p> If the provider is currently disabled, null is returned.
 *
 * @param provider the name of the provider
 * @return the last known location for the provider, or null
 *
 * @throws SecurityException if no suitable permission is present
 * @throws IllegalArgumentException if provider is null or doesn't exist
 */
ECode LocationManager::GetLastKnownLocation(
    /* [in] */ const String& provider,
    /* [out] */ ILocation** location)
{
    VALIDATE_NOT_NULL(location);
    *location = NULL;
    FAIL_RETURN(CheckProvider(provider));
    String packageName;
    mContext->GetPackageName(&packageName);
    AutoPtr<ILocationRequest> request = CLocationRequest::CreateFromDeprecatedProvider(
            provider, 0, 0, TRUE);

    return mService->GetLastLocation(request, packageName, location);
}

// --- Mock provider support ---
// TODO: It would be fantastic to deprecate mock providers entirely, and replace
// with something closer to LocationProviderBase.java

/**
 * Creates a mock location provider and adds it to the set of active providers.
 *
 * @param name the provider name
 *
 * @throws SecurityException if the ACCESS_MOCK_LOCATION permission is not present
 * or the {@link android.provider.Settings.Secure#ALLOW_MOCK_LOCATION
 * Settings.Secure.ALLOW_MOCK_LOCATION} system setting is not enabled
 * @throws IllegalArgumentException if a provider with the given name already exists
 */
ECode LocationManager::AddTestProvider(
    /* [in] */ const String& name,
    /* [in] */ Boolean requiresNetwork,
    /* [in] */ Boolean requiresSatellite,
    /* [in] */ Boolean requiresCell,
    /* [in] */ Boolean hasMonetaryCost,
    /* [in] */ Boolean supportsAltitude,
    /* [in] */ Boolean supportsSpeed,
    /* [in] */ Boolean supportsBearing,
    /* [in] */ Int32 powerRequirement,
    /* [in] */ Int32 accuracy)
{
    AutoPtr<IProviderProperties> properties;
    CProviderProperties::New(requiresNetwork,
            requiresSatellite, requiresCell, hasMonetaryCost, supportsAltitude, supportsSpeed,
            supportsBearing, powerRequirement, accuracy, (IProviderProperties**)&properties);
    Boolean match;
    StringUtils::Matches(name, ILocationProvider::BAD_CHARS_REGEX, &match);
    if(match)
    {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    return mService->AddTestProvider(name, properties);
}

/**
 * Removes the mock location provider with the given name.
 *
 * @param provider the provider name
 *
 * @throws SecurityException if the ACCESS_MOCK_LOCATION permission is not present
 * or the {@link android.provider.Settings.Secure#ALLOW_MOCK_LOCATION
 * Settings.Secure.ALLOW_MOCK_LOCATION}} system setting is not enabled
 * @throws IllegalArgumentException if no provider with the given name exists
 */
ECode LocationManager::RemoveTestProvider(
    /* [in] */ const String& provider)
{
    return mService->RemoveTestProvider(provider);
}

/**
 * Sets a mock location for the given provider.
 * <p>This location will be used in place of any actual location from the provider.
 * The location object must have a minimum number of fields set to be
 * considered a valid LocationProvider Location, as per documentation
 * on {@link Location} class.
 *
 * @param provider the provider name
 * @param loc the mock location
 *
 * @throws SecurityException if the ACCESS_MOCK_LOCATION permission is not present
 * or the {@link android.provider.Settings.Secure#ALLOW_MOCK_LOCATION
 * Settings.Secure.ALLOW_MOCK_LOCATION}} system setting is not enabled
 * @throws IllegalArgumentException if no provider with the given name exists
 * @throws IllegalArgumentException if the location is incomplete
 */
ECode LocationManager::SetTestProviderLocation(
    /* [in] */ const String provider,
    /* [in] */ ILocation* loc)
{
    VALIDATE_NOT_NULL(loc);

    Boolean result = FALSE;
    loc->IsComplete(&result);
    if (!result) {

        AutoPtr<IApplicationInfo> appInfo;
        mContext->GetApplicationInfo((IApplicationInfo**)&appInfo);
        Int32 targetSdkVersion;
        appInfo->GetTargetSdkVersion(&targetSdkVersion);
        if (targetSdkVersion <= Build::VERSION_CODES::JELLY_BEAN) {
            // just log on old platform (for backwards compatibility)
            loc->MakeComplete();
        } else {
            // really throw it!
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
        }
    }

    return mService->SetTestProviderLocation(provider, loc);
}

/**
 * Removes any mock location associated with the given provider.
 *
 * @param provider the provider name
 *
 * @throws SecurityException if the ACCESS_MOCK_LOCATION permission is not present
 * or the {@link android.provider.Settings.Secure#ALLOW_MOCK_LOCATION
 * Settings.Secure.ALLOW_MOCK_LOCATION}} system setting is not enabled
 * @throws IllegalArgumentException if no provider with the given name exists
 */
ECode LocationManager::ClearTestProviderLocation(
    /* [in] */ const String& provider)
{
    return mService->ClearTestProviderLocation(provider);
}

/**
 * Sets a mock enabled value for the given provider.  This value will be used in place
 * of any actual value from the provider.
 *
 * @param provider the provider name
 * @param enabled the mock enabled value
 *
 * @throws SecurityException if the ACCESS_MOCK_LOCATION permission is not present
 * or the {@link android.provider.Settings.Secure#ALLOW_MOCK_LOCATION
 * Settings.Secure.ALLOW_MOCK_LOCATION}} system setting is not enabled
 * @throws IllegalArgumentException if no provider with the given name exists
 */
ECode LocationManager::SetTestProviderEnabled(
    /* [in] */ const String& provider,
    /* [in] */ Boolean enabled)
{
    return mService->SetTestProviderEnabled(provider, enabled);
}

/**
 * Removes any mock enabled value associated with the given provider.
 *
 * @param provider the provider name
 *
 * @throws SecurityException if the ACCESS_MOCK_LOCATION permission is not present
 * or the {@link android.provider.Settings.Secure#ALLOW_MOCK_LOCATION
 * Settings.Secure.ALLOW_MOCK_LOCATION}} system setting is not enabled
 * @throws IllegalArgumentException if no provider with the given name exists
 */
ECode LocationManager::ClearTestProviderEnabled(
    /* [in] */ const String& provider)
{
    return mService->ClearTestProviderEnabled(provider);
}

/**
 * Sets mock status values for the given provider.  These values will be used in place
 * of any actual values from the provider.
 *
 * @param provider the provider name
 * @param status the mock status
 * @param extras a Bundle containing mock extras
 * @param updateTime the mock update time
 *
 * @throws SecurityException if the ACCESS_MOCK_LOCATION permission is not present
 * or the {@link android.provider.Settings.Secure#ALLOW_MOCK_LOCATION
 * Settings.Secure.ALLOW_MOCK_LOCATION}} system setting is not enabled
 * @throws IllegalArgumentException if no provider with the given name exists
 */
ECode LocationManager::SetTestProviderStatus(
    /* [in] */ const String& provider,
    /* [in] */ Int32 status,
    /* [in] */ IBundle* extras,
    /* [in] */ Int64 updateTime)
{
    return mService->SetTestProviderStatus(provider, status, extras, updateTime);
}

/**
 * Removes any mock status values associated with the given provider.
 *
 * @param provider the provider name
 *
 * @throws SecurityException if the ACCESS_MOCK_LOCATION permission is not present
 * or the {@link android.provider.Settings.Secure#ALLOW_MOCK_LOCATION
 * Settings.Secure.ALLOW_MOCK_LOCATION}} system setting is not enabled
 * @throws IllegalArgumentException if no provider with the given name exists
 */
ECode LocationManager::ClearTestProviderStatus(
    /* [in] */ const String& provider)
{
    return mService->ClearTestProviderStatus(provider);
}

// --- GPS-specific support ---

/**
 * Adds a GPS status listener.
 *
 * @param listener GPS status listener object to register
 *
 * @return true if the listener was successfully added
 *
 * @throws SecurityException if the ACCESS_FINE_LOCATION permission is not present
 */
ECode LocationManager::AddGpsStatusListener(
    /* [in] */ IGpsStatusListener* listener,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;
    HashMap<AutoPtr<IGpsStatusListener>, AutoPtr<IGpsStatusListenerTransport> >::Iterator it =
                  mGpsStatusListeners.Find(listener);
    if (it != mGpsStatusListeners.End() && (it->mSecond != NULL)) {
        // listener is already registered
        *result = TRUE;
        return NOERROR;
    }

    AutoPtr<IGpsStatusListenerTransport> transport;
    CGpsStatusListenerTransport::New((Handle32)this, listener, (IGpsStatusListenerTransport**)&transport);
    ECode ec = mService->AddGpsStatusListener((IIGpsStatusListener*)(transport->Probe(EIID_IIGpsStatusListener)), result);
    if(FAILED(ec))
    {
        return ec;
    }
    if (*result) {
        mGpsStatusListeners[listener] = transport;
    }
    return NOERROR;
}

/**
 * Removes a GPS status listener.
 *
 * @param listener GPS status listener object to remove
 */
ECode LocationManager::RemoveGpsStatusListener(
    /* [in] */ IGpsStatusListener* listener)
{
    HashMap<AutoPtr<IGpsStatusListener>, AutoPtr<IGpsStatusListenerTransport> >::Iterator it =
                  mGpsStatusListeners.Find(listener);
    AutoPtr<IGpsStatusListenerTransport> transport;
    if (it != mGpsStatusListeners.End()) {
        transport = it->mSecond;
        mGpsStatusListeners.Erase(it);
    }

    if (transport != NULL) {
        mService->RemoveGpsStatusListener(transport);
    }
    return NOERROR;
}

/**
 * Adds an NMEA listener.
 *
 * @param listener a {@link GpsStatus.NmeaListener} object to register
 *
 * @return true if the listener was successfully added
 *
 * @throws SecurityException if the ACCESS_FINE_LOCATION permission is not present
 */
ECode LocationManager::AddNmeaListener(
    /* [in] */ IGpsStatusNmeaListener* listener,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;
    HashMap<AutoPtr<IGpsStatusNmeaListener>, AutoPtr<IGpsStatusListenerTransport> >::Iterator it
              = mNmeaListeners.Find(listener);
    if (it != mNmeaListeners.End() && (it->mSecond != NULL)) {
        // listener is already registered
        *result = TRUE;
        return NOERROR;
    }

    AutoPtr<IGpsStatusListenerTransport> transport;
    CGpsStatusListenerTransport::New((Handle32)this, listener, (IGpsStatusListenerTransport**)&transport);
    ECode ec = mService->AddGpsStatusListener((IIGpsStatusListener*)(transport->Probe(EIID_IIGpsStatusListener)), result);
    if(FAILED(ec))
    {
        return NOERROR;
    }

    if (*result) {
        mNmeaListeners[listener] = transport;
    }
    return NOERROR;
}

/**
 * Removes an NMEA listener.
 *
 * @param listener a {@link GpsStatus.NmeaListener} object to remove
 */
ECode LocationManager::RemoveNmeaListener(
    /* [in] */ IGpsStatusNmeaListener* listener)
{
    HashMap<AutoPtr<IGpsStatusNmeaListener>, AutoPtr<IGpsStatusListenerTransport> >::Iterator it
              = mNmeaListeners.Find(listener);

    if (it != mNmeaListeners.End() && it->mSecond) {
        AutoPtr<IGpsStatusListenerTransport> transport = it->mSecond;
        mNmeaListeners.Erase(it);
        mService->RemoveGpsStatusListener(transport);
    }
    return NOERROR;
}

 /**
 * Retrieves information about the current status of the GPS engine.
 * This should only be called from the {@link GpsStatus.Listener#onGpsStatusChanged}
 * callback to ensure that the data is copied atomically.
 *
 * The caller may either pass in a {@link GpsStatus} object to set with the latest
 * status information, or pass null to create a new {@link GpsStatus} object.
 *
 * @param status object containing GPS status details, or null.
 * @return status object containing updated GPS status.
 */
ECode LocationManager::GetGpsStatus(
    /* [in] */ IGpsStatus* inStatus,
    /* [out] */ IGpsStatus** outStatus)
{
    VALIDATE_NOT_NULL(outStatus);

    AutoPtr<IGpsStatus> status = inStatus;
    if (status == NULL) {
        FAIL_RETURN(CGpsStatus::New((IGpsStatus**)&status))
    }
    status->SetStatusEx(mGpsStatus);

    *outStatus = status;
    INTERFACE_ADDREF(*outStatus);

    return NOERROR;
}

/**
 * Sends additional commands to a location provider.
 * Can be used to support provider specific extensions to the Location Manager API
 *
 * @param provider name of the location provider.
 * @param command name of the command to send to the provider.
 * @param extras optional arguments for the command (or null).
 * The provider may optionally fill the extras Bundle with results from the command.
 *
 * @return true if the command succeeds.
 */
ECode LocationManager::SendExtraCommand(
    /* [in] */ const String& provider,
    /* [in] */ const String& command,
    /* [in] */ IBundle* extras,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;
    return mService->SendExtraCommand(provider, command, &extras, result);
}

/**
 * Used by NetInitiatedActivity to report user response
 * for network initiated GPS fix requests.
 *
 * @hide
 */
ECode LocationManager::SendNiResponse(
    /* [in] */ Int32 notifId,
    /* [in] */ Int32 userResponse,
    /* [out] */ Boolean* result)
{
    *result = FALSE;
    return mService->SendNiResponse(notifId, userResponse, result);
}

/**
 * @hide - hide this constructor because it has a parameter
 * of type ILocationManager, which is a system private class. The
 * right way to create an instance of this class is using the
 * factory Context.getSystemService.
 */
ECode LocationManager::Init(
    /* [in] */ IContext* context,
    /* [in] */ IILocationManager* service)
{
    CGpsStatus::New((IGpsStatus**)&mGpsStatus);
    mContext = context;
    mService = service;
    return NOERROR;
}

AutoPtr<ILocationProvider> LocationManager::CreateProvider(
    /* [in] */ const String& name,
    /* [in] */ IProviderProperties* properties)
{
    AutoPtr<ILocationProvider> tmp;
    CLocationProvider::New(name, properties, (ILocationProvider**)&tmp);
    return tmp;
}

AutoPtr<IILocationListener> LocationManager::WrapListener(
    /* [in] */ ILocationListener* listener,
    /* [in] */ ILooper* looper)
{
    if (listener == NULL) return NULL;
    //synchronized (mListeners)
    {
        Mutex::Autolock lock(mutexListeners);
        AutoPtr<ILocationListener> tempKey = listener;
        HashMap<AutoPtr<ILocationListener>, AutoPtr<IILocationListener> >::Iterator it =
                mListeners.Find(tempKey);
        AutoPtr<IILocationListener> transport;
        if ((it == mListeners.End()) || (it->mSecond == NULL)) {
            CListenerTransport::New((Handle32)this, listener, looper, (IILocationListener**)&transport);
        }

        mListeners[tempKey] = transport;

        return transport;
    }
}

void LocationManager::RequestLocationUpdates(
    /* [in] */ ILocationRequest* request,
    /* [in] */ ILocationListener* listener,
    /* [in] */ ILooper* looper,
    /* [in] */ IPendingIntent* intent)
{
    String packageName;
    mContext->GetPackageName(&packageName);

    // wrap the listener class
    AutoPtr<IILocationListener> transport = WrapListener(listener, looper);
    mService->RequestLocationUpdates(request, transport, intent, packageName);
}

ECode LocationManager::CheckProvider(
    /* [in] */ const String& provider)
{
    if(provider.IsNull())
    {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return NOERROR;
}

ECode LocationManager::CheckCriteria(
    /* [in] */ ICriteria* criteria)
{
    if(criteria == NULL)
    {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return NOERROR;
}

ECode LocationManager::CheckListener(
    /* [in] */ ILocationListener* listener)
{
    if(listener == NULL)
    {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return NOERROR;
}

ECode LocationManager::CheckPendingIntent(
    /* [in] */ IPendingIntent* intent)
{
    if(intent == NULL)
    {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    Boolean result = FALSE;
    intent->IsTargetedToPackage(&result);
    if (!result) {
        AutoPtr<IApplicationInfo> aInfo;
        mContext->GetApplicationInfo((IApplicationInfo**)&aInfo);
        Int32 version;
        aInfo->GetTargetSdkVersion(&version);
        if(version > Build::VERSION_CODES::JELLY_BEAN)
        {
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
        }
    }
    return NOERROR;
}

ECode LocationManager::CheckGeofence(
    /* [in] */ IGeofence* fence)
{
    if(fence == NULL)
    {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return NOERROR;
}

} // namespace Location
} // namepsace Droid
} // namespace Elastos
