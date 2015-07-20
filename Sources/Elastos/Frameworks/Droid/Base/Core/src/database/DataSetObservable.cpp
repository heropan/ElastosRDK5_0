
#include "database/DataSetObservable.h"

namespace Elastos {
namespace Droid {
namespace Database {

ECode DataSetObservable::NotifyChanged()
{
    AutoLock lock(mObserversLock);

    List< AutoPtr<IInterface> >::ReverseIterator rit = mObservers.RBegin();
    for (; rit != mObservers.REnd(); ++rit) {
        AutoPtr<IDataSetObserver> observer = IDataSetObserver::Probe(*rit);
        observer->OnChanged();
    }

    return NOERROR;
}

ECode DataSetObservable::NotifyInvalidated()
{
    AutoLock lock(mObserversLock);

    List< AutoPtr<IInterface> >::ReverseIterator rit = mObservers.RBegin();
    for (; rit != mObservers.REnd(); ++rit) {
        AutoPtr<IDataSetObserver> observer = IDataSetObserver::Probe(*rit);
        observer->OnInvalidated();
    }

    return NOERROR;
}

} //Database
} //Droid
} //Elastos
