#ifndef __CCONTENTOBSERVABLE_H__
#define __CCONTENTOBSERVABLE_H__

#include "_CContentObservable.h"
#include "database/ContentObservable.h"

namespace Elastos {
namespace Droid {
namespace Database {

CarClass(CContentObservable), public ContentObservable
{
public:
    CARAPI RegisterObserver(
        /* [in] */ IInterface* observer);

    CARAPI UnregisterObserver(
        /* [in] */ IInterface* observer);

    CARAPI UnregisterAll();

    CARAPI DispatchChange(
        /* [in] */ Boolean selfChange);

    CARAPI DispatchChange(
        /* [in] */ Boolean selfChange,
        /* [in] */ IUri* uri);

    CARAPI NotifyChange(
        /* [in] */ Boolean selfChange);

private:
    // TODO: Add your private member variables here.
};

} //Database
} //Droid
} //Elastos

#endif // __CCONTENTOBSERVABLE_H__
