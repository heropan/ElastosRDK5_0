
#ifndef __ELASTOS_DROID_CONTENT_CSYNCRESULTHELPER_H__
#define __ELASTOS_DROID_CONTENT_CSYNCRESULTHELPER_H__

#include "_CSyncResultHelper.h"

namespace Elastos {
namespace Droid {
namespace Content {

CarClass(CSyncResultHelper)
{
public:
    /**
     * This instance of a SyncResult is returned by the SyncAdapter in response to a
     * sync request when a sync is already underway. The SyncManager will reschedule the
     * sync request to try again later.
     */
    CARAPI GetALREADY_IN_PROGRESS(
        /* [out] */ ISyncResult** syncResult);

};

}
}
}

#endif // __ELASTOS_DROID_CONTENT_CSYNCRESULTHELPER_H__
