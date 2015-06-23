
#include "database/CContentObservable.h"

namespace Elastos {
namespace Droid {
namespace Database {

ECode CContentObservable::RegisterObserver(
    /* [in] */ IInterface* observer)
{
    return ContentObservable::RegisterObserver(observer);
}

ECode CContentObservable::UnregisterObserver(
    /* [in] */ IInterface* observer)
{
    return ContentObservable::UnregisterObserver(observer);
}

ECode CContentObservable::UnregisterAll()
{
    return ContentObservable::UnregisterAll();
}

ECode CContentObservable::DispatchChange(
    /* [in] */ Boolean selfChange)
{
    return ContentObservable::DispatchChange(selfChange);
}

ECode CContentObservable::DispatchChange(
    /* [in] */ Boolean selfChange,
    /* [in] */ IUri* uri)
{
    return ContentObservable::DispatchChange(selfChange, uri);
}

ECode CContentObservable::NotifyChange(
    /* [in] */ Boolean selfChange)
{
    return ContentObservable::NotifyChange(selfChange);
}

} //Database
} //Droid
} //Elastos