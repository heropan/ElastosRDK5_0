
#include "database/CDataSetObservable.h"

namespace Elastos {
namespace Droid {
namespace Database {

ECode CDataSetObservable::RegisterObserver(
    /* [in] */ IInterface* observer)
{
    return DataSetObservable::RegisterObserver(observer);
}

ECode CDataSetObservable::UnregisterObserver(
    /* [in] */ IInterface* observer)
{
    return DataSetObservable::UnregisterObserver(observer);
}

ECode CDataSetObservable::UnregisterAll()
{
    return DataSetObservable::UnregisterAll();
}

ECode CDataSetObservable::NotifyChanged()
{
    return DataSetObservable::NotifyChanged();
}

ECode CDataSetObservable::NotifyInvalidated()
{
    return DataSetObservable::NotifyInvalidated();
}

} //Database
} //Droid
} //Elastos