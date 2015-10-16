
#include "elastos/droid/location/CCountryDetector.h"

namespace Elastos {
namespace Droid {
namespace Location {

/**
 * @hide - hide this constructor because it has a parameter of type
 *       ICountryDetector, which is a system private class. The right way to
 *       create an instance of this class is using the factory
 *       Context.getSystemService.
 */
ECode CCountryDetector::constructor(
    /* [in] */ ICountryDetector* service)
{
    return CountryDetector::Init(service);
}

/**
 * Start detecting the country that the user is in.
 *
 * @return the country if it is available immediately, otherwise null will
 *         be returned.
 */
ECode CCountryDetector::DetectCountry(
    /* [out] */ ICountry** country)
{
    return CountryDetector::DetectCountry(country);
}

#if 0
/**
 * Add a listener to receive the notification when the country is detected
 * or changed.
 *
 * @param listener will be called when the country is detected or changed.
 * @param looper a Looper object whose message queue will be used to
 *        implement the callback mechanism. If looper is null then the
 *        callbacks will be called on the main thread.
 */
ECode CCountryDetector::AddCountryListener(
    /* [in] */ ICountryListener* listener,
    /* [in] */ IApartment* looper)
{
    return CountryDetector::AddCountryListener(listener, looper);
}
#endif

/**
 * Remove the listener
 */
ECode CCountryDetector::RemoveCountryListener(
    /* [in] */ ICountryListener* listener)
{
    return CountryDetector::RemoveCountryListener(listener);
}

}//namespace Location
}//namespace Droid
}//namespace Elastos