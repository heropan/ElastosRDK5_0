
#include "database/Observable.h"
#include <elastos/utility/logging/Slogger.h>
#include <elastos/utility/etl/Algorithm.h>

using Elastos::Utility::Logging::Slogger;

namespace Elastos {
namespace Droid {
namespace Database {

ECode Observable::RegisterObserver(
    /* [in] */ IInterface* observer)
{
    if (observer == NULL) {
        Slogger::E("Observable", "The observer is NULL.");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    Mutex::Autolock lock(mObserversLock);

    List< AutoPtr<IInterface> >::Iterator it =
            Find(mObservers.Begin(), mObservers.End(), AutoPtr<IInterface>(observer));
    if (it != mObservers.End()) {
        //throw new IllegalStateException("Observer " + observer + " is already registered.");
        Slogger::E(String("Observable"), "Observer %p is already registered.", observer);
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    mObservers.PushBack(observer);

    return NOERROR;
}

ECode Observable::UnregisterObserver(
    /* [in] */ IInterface* observer)
{
    if (observer == NULL) {
        Slogger::E("Observable", "The observer is NULL.");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    Mutex::Autolock lock(mObserversLock);

    List< AutoPtr<IInterface> >::Iterator it =
            Find(mObservers.Begin(), mObservers.End(), AutoPtr<IInterface>(observer));
    if (it == mObservers.End()) {
        //throw new IllegalStateException("Observer " + observer + " was not registered.");
        Slogger::E(String("Observable"), "Observer %p was not registered.", observer);
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    mObservers.Erase(it);

    return NOERROR;
}

ECode Observable::UnregisterAll()
{
    Mutex::Autolock lock(mObserversLock);
    mObservers.Clear();
    return NOERROR;
}

} //Database
} //Droid
} //Elastos