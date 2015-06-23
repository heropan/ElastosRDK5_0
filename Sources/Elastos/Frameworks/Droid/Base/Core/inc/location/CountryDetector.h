
#ifndef __COUNTRYDETECTOR_H__
#define __COUNTRYDETECTOR_H__

#include "ext/frameworkext.h"
#include <elastos/utility/etl/HashMap.h>

using Elastos::Core::Mutex;
using Elastos::Utility::Etl::HashMap;
using Elastos::Droid::Location::ICountryListener;


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
class CountryDetector {
public:
    CountryDetector() {}

    /**
     * @hide - hide this constructor because it has a parameter of type
     *       ICountryDetector, which is a system private class. The right way to
     *       create an instance of this class is using the factory
     *       Context.getSystemService.
     */
    CountryDetector(
        /* [in] */ ICountryDetector* service);

    /**
     * Start detecting the country that the user is in.
     *
     * @return the country if it is available immediately, otherwise null will
     *         be returned.
     */
    virtual CARAPI DetectCountry(
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
//    virtual CARAPI AddCountryListener(
//        /* [in] */ ICountryListener* listener,
//        /* [in] */ IApartment* looper);

    /**
     * Remove the listener
     */
    virtual CARAPI RemoveCountryListener(
        /* [in] */ ICountryListener* listener);

protected:
    /**
     * @hide - hide this constructor because it has a parameter of type
     *       ICountryDetector, which is a system private class. The right way to
     *       create an instance of this class is using the factory
     *       Context.getSystemService.
     */
    virtual CARAPI Init(
        /* [in] */ ICountryDetector* service);

private:
    /**
     * The class to wrap the ICountryListener.Stub and CountryListener objects
     * together. The CountryListener will be notified through the specific
     * looper once the country changed and detected.
     */
/*    private final static class ListenerTransport extends ICountryListener.Stub {

        private final CountryListener mListener;

        private final Handler mHandler;

        public ListenerTransport(CountryListener listener, Looper looper) {
            mListener = listener;
            if (looper != null) {
                mHandler = new Handler(looper);
            } else {
                mHandler = new Handler();
            }
        }

        public void onCountryDetected(final Country country) {
            mHandler.post(new Runnable() {
                public void run() {
                    mListener.onCountryDetected(country);
                }
            });
        }
    };
*/
    static const String TAG;// = "CountryDetector";
    AutoPtr<ICountryDetector> mService;
//    HashMap<AutoPtr<ICountryListener>, AutoPtr<ListenerTransport> > mListeners;

    Mutex mListenersLock;
};

}//namespace Location
}//namespace Droid
}//namespace Elastos

#endif //__COUNTRYDETECTOR_H__
