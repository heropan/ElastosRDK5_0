
#ifndef __GEOLOCATIONSERVICE_H__
#define __GEOLOCATIONSERVICE_H__

#include "ext/frameworkext.h"

using Elastos::Droid::Content::IContext;
using Elastos::Droid::Os::IBundle;
using Elastos::Droid::Location::ILocation;
using Elastos::Droid::Location::ILocationListener;
using Elastos::Droid::Location::ILocationManager;
using Elastos::Droid::Location::EIID_ILocationListener;

#ifdef __cplusplus
extern "C"
{
#endif
    extern void Elastos_GeolocationServiceBridge_NewLocationAvailable(Int64 nativeObject, IInterface* location);
    extern void Elastos_GeolocationServiceBridge_NewErrorAvailable(Int64 nativeObject, const Elastos::String& message);
#ifdef __cplusplus
}
#endif

struct ElaGeolocationServiceCallback
{
    AutoPtr<IInterface> (*createGeolocationService)(IInterface* context, Int64 nativeObject);
    Boolean (*start)(IInterface* obj);
    void (*stop)(IInterface* obj);
    void (*setEnableGps)(IInterface* obj, Boolean enable);
};

namespace Elastos {
namespace Droid {
namespace Webkit {

/**
 * Implements the Java side of GeolocationServiceAndroid.
 */
class GeolocationService
    : public ElRefBase
    , public ILocationListener
{
public:
    /**
     * Constructor
     * @param context The context from which we obtain the system service.
     * @param nativeObject The native object to which this object will report position updates and
     *     errors.
     */
    GeolocationService(
        /* [in] */ IContext* context,
        /* [in] */ Int64 nativeObject);

    CAR_INTERFACE_DECL();

    /**
     * Start listening for location updates.
     */
    virtual CARAPI_(Boolean) Start();

    /**
     * Stop listening for location updates.
     */
    virtual CARAPI_(void) Stop();

    /**
     * Sets whether to use the GPS.
     * @param enable Whether to use the GPS.
     */
    virtual CARAPI_(void) SetEnableGps(
        /* [in] */ Boolean enable);

    /**
     * LocationListener implementation.
     * Called when the location has changed.
     * @param location The new location, as a Location object.
     */
    CARAPI OnLocationChanged(
        /* [in] */ ILocation* location);

    /**
     * LocationListener implementation.
     * Called when the provider status changes.
     * @param provider The name of the provider.
     * @param status The new status of the provider.
     * @param extras an optional Bundle with provider specific data.
     */
    CARAPI OnStatusChanged(
        /* [in] */ const String& providerName,
        /* [in] */ Int32 status,
        /* [in] */ IBundle* extras);

    /**
     * LocationListener implementation.
     * Called when the provider is enabled.
     * @param provider The name of the location provider that is now enabled.
     */
    CARAPI OnProviderEnabled(
        /* [in] */ const String& providerName);

    /**
     * LocationListener implementation.
     * Called when the provider is disabled.
     * @param provider The name of the location provider that is now disabled.
     */
    CARAPI OnProviderDisabled(
        /* [in] */ const String& providerName);

    static AutoPtr<IInterface> CreateGeolocationService(
        /* [in] */ IInterface* context,
        /* [in] */ Int64 nativeObject);

    static Boolean Start(
        /* [in] */ IInterface* obj);

    static void Stop(
        /* [in] */ IInterface* obj);

    static void SetEnableGps(
        /* [in] */ IInterface* obj,
        /* [in] */ Boolean enable);
private:
    /**
     * Registers this object with the location service.
     */
    CARAPI_(void) RegisterForLocationUpdates();

    /**
     * Unregisters this object from the location service.
     */
    CARAPI_(void) UnregisterFromLocationUpdates();

    /**
     * Reports an error if neither the network nor the GPS provider is available.
     */
    CARAPI_(void) MaybeReportError(
        /* [in] */ const String& message);

    // Native functions
    static CARAPI_(void) NativeNewLocationAvailable(
        /* [in] */ Int64 nativeObject,
        /* [in] */ ILocation* location);

    static CARAPI_(void) NativeNewErrorAvailable(
        /* [in] */ Int64 nativeObject,
        /* [in] */ const String& message);

public:
    static struct ElaGeolocationServiceCallback sElaGeolocationServiceCallback;

private:
    // Log tag
    static const String TAG;

    Int64 mNativeObject;
    AutoPtr<ILocationManager> mLocationManager;
    Boolean mIsGpsEnabled;
    Boolean mIsRunning;
    Boolean mIsNetworkProviderAvailable;
    Boolean mIsGpsProviderAvailable;
};

} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif //__GEOLOCATIONSERVICE_H__
