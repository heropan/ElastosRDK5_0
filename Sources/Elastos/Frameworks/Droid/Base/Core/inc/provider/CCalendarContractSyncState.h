
#ifndef __CCALENDARCONTRACTSYNCSTATE_H__
#define __CCALENDARCONTRACTSYNCSTATE_H__

#include "_CCalendarContractSyncState.h"

using Elastos::Droid::Net::IUri;

namespace Elastos {
namespace Droid {
namespace Provider {

/**
 * A table provided for sync adapters to use for storing private sync state data.
 *
 * @see SyncStateContract
 */
CarClass(CCalendarContractSyncState)
{
public:
    /**
     * This utility class cannot be instantiated
     */
    CARAPI constructor();

    /**
     * The content:// style URL for the top-level calendar authority
     */
    CARAPI GetCONTENTURI(
        /* [out] */ IUri** uri);

private:
    static const String CONTENT_DIRECTORY;
};

} //Provider
} //Droid
} //Elastos

#endif //__CCALENDARCONTRACTSYNCSTATE_H__