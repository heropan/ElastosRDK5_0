
#ifndef __ELASTOS_DROID_LOCATION_CCOUNTRYDETECTOR_H__
#define __ELASTOS_DROID_LOCATION_CCOUNTRYDETECTOR_H__

#include "elastos/droid/ext/frameworkext.h"
#include "_Elastos_Droid_Location_CCountryDetector.h"
#include "location/CountryDetector.h"

namespace Elastos {
namespace Droid {
namespace Location {

/**
 * This class provides access to the system country detector service. This
 * service allows applications to obtain the country that the user is in.
 * <p>
 * The country will be detected in order of reliability, like
 * <ul>
 * <li>Mobile network</li>
 * <li>Location</li>
 * <li>SIM's country</li>
 * <li>Phone's locale</li>
 * </ul>
 * <p>
 * Call the {@link #detectCountry()} to get the available country immediately.
 * <p>
 * To be notified of the future country change, use the
 * {@link #addCountryListener}
 * <p>
 * <p>
 * You do not instantiate this class directly; instead, retrieve it through
 * {@link android.content.Context#getSystemService
 * Context.getSystemService(Context.COUNTRY_DETECTOR)}.
 * <p>
 * Both ACCESS_FINE_LOCATION and ACCESS_COARSE_LOCATION permissions are needed.
 *
 * @hide
 */

CarClass(CCountryDetector), public CountryDetector
{
public:
    /**
     * @hide - hide this constructor because it has a parameter of type
     *       ICountryDetector, which is a system private class. The right way to
     *       create an instance of this class is using the factory
     *       Context.getSystemService.
     */
    CARAPI constructor(
        /* [in] */ ICountryDetector* service);

    /**
     * Start detecting the country that the user is in.
     *
     * @return the country if it is available immediately, otherwise null will
     *         be returned.
     */
    CARAPI DetectCountry(
        /* [out] */ ICountry** country);

    /**
     * Add a listener to receive the notification when the country is detected
     * or changed.
     *
     * @param listener will be called when the country is detected or changed.
     * @param looper a Looper object whose message queue will be used to
     *        implement the callback mechanism. If looper is null then the
     *        callbacks will be called on the main thread.
     */
//    CARAPI AddCountryListener(
//        /* [in] */ ICountryListener* listener,
//        /* [in] */ IApartment* looper);

    /**
     * Remove the listener
     */
    CARAPI RemoveCountryListener(
        /* [in] */ ICountryListener* listener);
};

}//namespace Location
}//namespace Droid
}//namespace Elastos

#endif //__ELASTOS_DROID_LOCATION_CCOUNTRYDETECTOR_H__
