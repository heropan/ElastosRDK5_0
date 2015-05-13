
#include "database/ContentObservable.h"

namespace Elastos {
namespace Droid {
namespace Database {

ECode ContentObservable::RegisterObserver(
    /* [in] */ IInterface* observer)
{
    return Observable::RegisterObserver(observer);
}

ECode ContentObservable::DispatchChange(
    /* [in] */ Boolean selfChange)
{
    return DispatchChangeEx(selfChange, NULL);
}

ECode ContentObservable::DispatchChangeEx(
    /* [in] */ Boolean selfChange,
    /* [in] */ IUri* uri)
{
    Mutex::Autolock lock(mObserversLock);

    List< AutoPtr<IInterface> >::Iterator iter;
    for (iter = mObservers.Begin(); iter != mObservers.End(); ++iter) {
        AutoPtr<IContentObserver> observer = IContentObserver::Probe(*iter);
        Boolean result;
        if (!selfChange || (observer->DeliverSelfNotifications(&result), result)) {
            observer->DispatchChange(selfChange);
        }
    }
    return NOERROR;
}

ECode ContentObservable::NotifyChange(
    /* [in] */ Boolean selfChange)
{
    Mutex::Autolock lock(mObserversLock);

    List< AutoPtr<IInterface> >::Iterator iter;
    for (iter = mObservers.Begin(); iter != mObservers.End(); ++iter) {
        AutoPtr<IContentObserver> observer = IContentObserver::Probe(*iter);
        observer->OnChange(selfChange);
    }
    return NOERROR;
}

} //Database
} //Droid
} //Elastos