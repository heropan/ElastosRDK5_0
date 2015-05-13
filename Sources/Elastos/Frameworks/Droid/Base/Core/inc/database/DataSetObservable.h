
#ifndef __DATASETOBSERVABLE_H__
#define __DATASETOBSERVABLE_H__

#include "database/Observable.h"

namespace Elastos {
namespace Droid {
namespace Database {
/**
 * A specialization of Observable for DataSetObserver that provides methods for
 * invoking the various callback methods of DataSetObserver.
 */
class DataSetObservable : public Observable
{
public:
    /**
     * Invokes onChanged on each observer. Called when the data set being observed has
     * changed, and which when read contains the new state of the data.
     */
    virtual CARAPI NotifyChanged();

    /**
     * Invokes onInvalidated on each observer. Called when the data set being monitored
     * has changed such that it is no longer valid.
     */
    virtual CARAPI NotifyInvalidated();
};

} //Database
} //Droid
} //Elastos

#endif //__DATASETOBSERVABLE_H__
