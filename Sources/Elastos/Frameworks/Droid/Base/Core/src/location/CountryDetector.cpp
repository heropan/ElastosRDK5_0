
#include "location/CountryDetector.h"

namespace Elastos {
namespace Droid {
namespace Location {

const String CountryDetector::TAG("CountryDetector");

/**
 * @hide - hide this constructor because it has a parameter of type
 *       ICountryDetector, which is a system private class. The right way to
 *       create an instance of this class is using the factory
 *       Context.getSystemService.
 */
CountryDetector::CountryDetector(
    /* [in] */ ICountryDetector* service)
{
    Init(service);
}

/**
 * Start detecting the country that the user is in.
 *
 * @return the country if it is available immediately, otherwise null will
 *         be returned.
 */
ECode CountryDetector::DetectCountry(
    /* [out] */ ICountry** country)
{
    VALIDATE_NOT_NULL(country);

    //try {
    //    return mService.detectCountry();
    //} catch (RemoteException e) {
    //    Log.e(TAG, "detectCountry: RemoteException", e);
    //    return null;
    //}

    return mService->DetectCountry(country);
}

/**
 * Add a listener to receive the notification when the country is detected
 * or changed.
 *
 * @param listener will be called when the country is detected or changed.
 * @param looper a Looper object whose message queue will be used to
 *        implement the callback mechanism. If looper is null then the
 *        callbacks will be called on the main thread.
 */
#if 0
ECode CountryDetector::AddCountryListener(
    /* [in] */ ICountryListener* listener,
    /* [in] */ IApartment* looper)
{
/*
    {
        Mutex::AutoLock lock(mListenersLock);
        if (!mListeners.containsKey(listener)) {
            ListenerTransport transport = new ListenerTransport(listener, looper);
            try {
                mService.addCountryListener(transport);
                mListeners.put(listener, transport);
            } catch (RemoteException e) {
                Log.e(TAG, "addCountryListener: RemoteException", e);
            }
        }
    }
*/
}
#endif

/**
 * Remove the listener
 */
ECode CountryDetector::RemoveCountryListener(
    /* [in] */ ICountryListener* listener)
{
/*
    synchronized (mListeners) {
        ListenerTransport transport = mListeners.get(listener);
        if (transport != null) {
            try {
                mListeners.remove(listener);
                mService.removeCountryListener(transport);
            } catch (RemoteException e) {
                Log.e(TAG, "removeCountryListener: RemoteException", e);
            }
        }
    }
*/
    return E_NOT_IMPLEMENTED;
}

/**
 * @hide - hide this constructor because it has a parameter of type
 *       ICountryDetector, which is a system private class. The right way to
 *       create an instance of this class is using the factory
 *       Context.getSystemService.
 */
ECode CountryDetector::Init(
    /* [in] */ ICountryDetector* service)
{
    mService = service;
//    mListeners = new HashMap<CountryListener, ListenerTransport>();
    return E_NOT_IMPLEMENTED;
}

} // namespace Location
} // namepsace Droid
} // namespace Elastos