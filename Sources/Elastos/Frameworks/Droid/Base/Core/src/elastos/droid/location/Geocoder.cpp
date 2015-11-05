
#include "elastos/droid/location/Geocoder.h"
#include "elastos/droid/location/CGeocoderParams.h"
#include "elastos/droid/os/CServiceManager.h"

using Elastos::Droid::Os::IServiceManager;
using Elastos::Droid::Os::CServiceManager;
using Elastos::Utility::ILocaleHelper;
using Elastos::Utility::CLocaleHelper;

namespace Elastos {
namespace Droid {
namespace Location {

const String Geocoder::TAG("Geocoder");

CAR_INTERFACE_IMPL(Geocoder, Object, IGeocoder);

Geocoder::Geocoder()
{}

ECode Geocoder::constructor(
    /* [in] */ IContext* context,
    /* [in] */ ILocale* locale)
{
    if (locale != NULL) {
        CGeocoderParams::New(context, locale, (IGeocoderParams**)&mParams);
        AutoPtr<IServiceManager> serviceManager;
        ASSERT_SUCCEEDED(CServiceManager::AcquireSingleton((IServiceManager**)&serviceManager));
        ASSERT_SUCCEEDED(serviceManager->GetService(IContext::LOCATION_SERVICE, (IInterface**)&mService));
        return NOERROR;
    }
    else {
        return E_NULL_POINTER_EXCEPTION;
    }
}

ECode Geocoder::constructor(
    /* [in] */ IContext* context)
{
    AutoPtr<ILocaleHelper> localeHelper;
    CLocaleHelper::AcquireSingleton((ILocaleHelper**)&localeHelper);
    AutoPtr<ILocale> locale;
    localeHelper->GetDefault((ILocale**)&locale);
    return constructor(context, locale);
}

ECode Geocoder::GetFromLocation(
    /* [in] */ Double latitude,
    /* [in] */ Double longitude,
    /* [in] */ Int32 maxResults,
    /* [out] */ IList** addressContainer)
{
    VALIDATE_NOT_NULL(addressContainer);

    if (latitude < -90.0 || latitude > 90.0) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (longitude < -180.0 || longitude > 180.0) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    String ex;
    mService->GetFromLocation(latitude, longitude, maxResults,
        mParams, addressContainer, &ex);
    if (!ex.IsNull()) {
        return E_IO_EXCEPTION;
    }
    return NOERROR;
}

ECode Geocoder::GetFromLocationName(
    /* [in] */ const String& locationName,
    /* [in] */ Int32 maxResults,
    /* [out] */ IList** addressContainer)
{
    if (locationName == NULL) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    String ex;
    mService->GetFromLocationName(locationName,
        0, 0, 0, 0, maxResults, mParams, addressContainer, &ex);
    if (!ex.IsNull()) {
        return E_IO_EXCEPTION;
    }
    return NOERROR;
}

ECode Geocoder::GetFromLocationName(
    /* [in] */ const String& locationName,
    /* [in] */ Int32 maxResults,
    /* [in] */ Double lowerLeftLatitude,
    /* [in] */ Double lowerLeftLongitude,
    /* [in] */ Double upperRightLatitude,
    /* [in] */ Double upperRightLongitude,
    /* [out] */ IList** addressContainer)
{
    if (locationName == NULL) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (lowerLeftLatitude < -90.0 || lowerLeftLatitude > 90.0) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (lowerLeftLongitude < -180.0 || lowerLeftLongitude > 180.0) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (upperRightLatitude < -90.0 || upperRightLatitude > 90.0) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (upperRightLongitude < -180.0 || upperRightLongitude > 180.0) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    String ex;
    return mService->GetFromLocationName(locationName,
        lowerLeftLatitude, lowerLeftLongitude, upperRightLatitude, upperRightLongitude,
        maxResults, mParams, addressContainer, &ex);
    if (!ex.IsNull()) {
        return E_IO_EXCEPTION;
    }
    return NOERROR;
}

}//namespace Location
}//namespace Droid
}//namespace Elastos
