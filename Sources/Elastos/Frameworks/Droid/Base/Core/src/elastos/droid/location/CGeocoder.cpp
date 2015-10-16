
#include "elastos/droid/location/CGeocoder.h"
#include "elastos/droid/location/CGeocoderParams.h"
#include "elastos/droid/os/CServiceManager.h"
#include "elastos/droid/os/CServiceManager.h"

using Elastos::Droid::Os::IServiceManager;
using Elastos::Droid::Os::CServiceManager;
using Libcore::ICU::ILocaleHelper;
using Libcore::ICU::CLocaleHelper;

namespace Elastos {
namespace Droid {
namespace Location {

const String CGeocoder::TAG("Geocoder");

ECode CGeocoder::IsPresent(
    /* [out] */ Boolean* isPresent)
{
    AutoPtr<IServiceManager> serviceManager;
    AutoPtr<IILocationManager> lm;

    CServiceManager::AcquireSingleton((IServiceManager**)&serviceManager);
    serviceManager->GetService(IContext::LOCATION_SERVICE, (IInterface**)&lm);

//    try {
    return lm->GeocoderIsPresent(isPresent);
//    } catch (RemoteException e) {
//        Log.e(TAG, "isPresent: got RemoteException", e);
//        return false;
//    }
}

ECode CGeocoder::GetFromLocation(
    /* [in] */ Double latitude,
    /* [in] */ Double longitude,
    /* [in] */ Int32 maxResults,
    /* [out] */ IObjectContainer** addressContainer)
{
    VALIDATE_NOT_NULL(addressContainer);

    if (latitude < -90.0 || latitude > 90.0) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("latitude == " + latitude);
    }
    if (longitude < -180.0 || longitude > 180.0) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("longitude == " + longitude);
    }
//    try {
    String ex;
    return mService->GetFromLocation(latitude, longitude, maxResults,
        mParams, addressContainer, &ex);
//    if (ex.IsNull()) {
//        throw new IOException(ex);
//    }
//    else {
//        return results;
//    }
//    } catch (RemoteException e) {
//        Log.e(TAG, "getFromLocation: got RemoteException", e);
//        return null;
//    }
}

ECode CGeocoder::GetFromLocationName(
    /* [in] */ const String& locationName,
    /* [in] */ Int32 maxResults,
    /* [out] */ IObjectContainer** addressContainer)
{
    if (locationName == NULL) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("locationName == null");
    }
//    try {
    String ex;
    return mService->GetFromLocationName(locationName,
        0, 0, 0, 0, maxResults, mParams, addressContainer, &ex);
//    if (ex != null) {
//        throw new IOException(ex);
//    } else {
//        return results;
//    }
//    } catch (RemoteException e) {
//        Log.e(TAG, "getFromLocationName: got RemoteException", e);
//        return null;
//    }

    return NOERROR;
}

ECode CGeocoder::GetFromLocationName(
    /* [in] */ const String& locationName,
    /* [in] */ Int32 maxResults,
    /* [in] */ Double lowerLeftLatitude,
    /* [in] */ Double lowerLeftLongitude,
    /* [in] */ Double upperRightLatitude,
    /* [in] */ Double upperRightLongitude,
    /* [out] */ IObjectContainer** addressContainer)
{
    if (locationName == NULL) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("locationName == null");
    }
    if (lowerLeftLatitude < -90.0 || lowerLeftLatitude > 90.0) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("lowerLeftLatitude == "
//            + lowerLeftLatitude);
    }
    if (lowerLeftLongitude < -180.0 || lowerLeftLongitude > 180.0) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("lowerLeftLongitude == "
//            + lowerLeftLongitude);
    }
    if (upperRightLatitude < -90.0 || upperRightLatitude > 90.0) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("upperRightLatitude == "
//            + upperRightLatitude);
    }
    if (upperRightLongitude < -180.0 || upperRightLongitude > 180.0) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("upperRightLongitude == "
//            + upperRightLongitude);
    }
//    try {
    String ex;
    return mService->GetFromLocationName(locationName,
        lowerLeftLatitude, lowerLeftLongitude, upperRightLatitude, upperRightLongitude,
        maxResults, mParams, addressContainer, &ex);
//    if (ex != null) {
//        throw new IOException(ex);
//    } else {
//        return result;
//    }
//    } catch (RemoteException e) {
//        Log.e(TAG, "getFromLocationName: got RemoteException", e);
//        return null;
//    }

    return NOERROR;
}

ECode CGeocoder::constructor(
    /* [in] */ IContext* context,
    /* [in] */ ILocale* locale)
{
    if (locale == NULL) {
        return E_NULL_POINTER_EXCEPTION;
//        throw new NullPointerException("locale == null");
    }
    ASSERT_SUCCEEDED(CGeocoderParams::New(context, locale, (IGeocoderParams**)&mParams));
    AutoPtr<IServiceManager> serviceManager;
    ASSERT_SUCCEEDED(CServiceManager::AcquireSingleton((IServiceManager**)&serviceManager));
    ASSERT_SUCCEEDED(serviceManager->GetService(IContext::LOCATION_SERVICE, (IInterface**)&mService));

    return NOERROR;
}

ECode CGeocoder::constructor(
    /* [in] */ IContext* context)
{
    AutoPtr<ILocaleHelper> localeHelper;
    CLocaleHelper::AcquireSingleton((ILocaleHelper**)&localeHelper);
    AutoPtr<ILocale> locale;
    localeHelper->GetDefault((ILocale**)&locale);
    return constructor(context, locale);
}

}//namespace Location
}//namespace Droid
}//namespace Elastos
