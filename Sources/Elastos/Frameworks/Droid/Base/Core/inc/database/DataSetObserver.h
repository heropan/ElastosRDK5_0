#ifndef __DATASETONSERVER_H__
#define __DATASETOBSERVER_H__

#include "ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Database {
/**
 * Receives call backs when a data set has been changed, or made invalid. The typically data sets
 * that are observed are {@link Cursor}s or {@link android.widget.Adapter}s.
 * DataSetObserver must be implemented by objects which are added to a DataSetObservable.
 */
class DataSetObserver
{
public:
    /**
     * This method is called when the entire data set has changed,
     * most likely through a call to {@link Cursor#requery()} on a {@link Cursor}.
     */
    virtual CARAPI OnChanged();

    /**
     * This method is called when the entire data becomes invalid,
     * most likely through a call to {@link Cursor#deactivate()} or {@link Cursor#close()} on a
     * {@link Cursor}.
     */
    virtual CARAPI OnInvalidated();
};

} //Database
} //Droid
} //Elastos

#endif //__DATASETONSERVER_H__
