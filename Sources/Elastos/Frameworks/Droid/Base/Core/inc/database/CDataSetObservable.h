
#ifndef __ELASTOS_DROID_DATABASE_CDATASETOBSERVABLE_H__
#define __ELASTOS_DROID_DATABASE_CDATASETOBSERVABLE_H__

#include "_Elastos_Droid_Database_CDataSetObservable.h"
#include "database/DataSetObservable.h"

namespace Elastos {
namespace Droid {
namespace Database {

CarClass(CDataSetObservable), public DataSetObservable
{
public:
    CARAPI RegisterObserver(
        /* [in] */ IInterface* observer);

    CARAPI UnregisterObserver(
        /* [in] */ IInterface* observer);

    CARAPI UnregisterAll();

    CARAPI NotifyChanged();

    CARAPI NotifyInvalidated();
};

} //Database
} //Droid
} //Elastos

#endif // __ELASTOS_DROID_DATABASE_CDATASETOBSERVABLE_H__
