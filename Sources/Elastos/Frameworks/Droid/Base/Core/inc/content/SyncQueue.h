
#ifndef __SYNCQUEUE_H__
#define __SYNCQUEUE_H__

#include <ext/frameworkext.h>
#include <elastos/utility/etl/HashMap.h>
#include "content/SyncAdaptersCache.h"

using namespace Elastos::Core;
using Elastos::Droid::Accounts::IAccount;
using Elastos::Utility::Etl::HashMap;

namespace Elastos {
namespace Droid {
namespace Content {

/**
 * Queue of pending sync operations. Not inherently thread safe, external
 * callers are responsible for locking.
 *
 * @hide
 */
class SyncQueue
    : public ElRefBase
    , public ISyncQueue
{
public:
    SyncQueue(
        /* [in] */ ISyncStorageEngine* engine,
        /* [in] */ SyncAdaptersCache* syncAdapters);

    ~SyncQueue();

    CAR_INTERFACE_DECL()

    CARAPI AddPendingOperations(
        /* [in] */ Int32 userId);

    CARAPI Add(
        /* [in] */ ISyncOperation* operation,
        /* [out] */ Boolean* result);

    CARAPI RemoveUser(
        /* [in] */ Int32 userId);

    /**
     * Remove the specified operation if it is in the queue.
     * @param operation the operation to remove
     */
    CARAPI Remove(
        /* [in] */ ISyncOperation* operation);

    CARAPI OnBackoffChanged(
        /* [in] */ IAccount* account,
        /* [in] */ Int32 userId,
        /* [in] */ const String& providerName,
        /* [in] */ Int64 backoff);

    CARAPI OnDelayUntilTimeChanged(
        /* [in] */ IAccount* account,
        /* [in] */ const String& providerName,
        /* [in] */ Int64 delayUntil);

    CARAPI Remove(
        /* [in] */ IAccount* account,
        /* [in] */ Int32 userId,
        /* [in] */ const String& authority);

    CARAPI GetOperations(
        /* [out, callee] */ ArrayOf<ISyncOperation *>** operations);

    CARAPI Dump(
        /* [in] */ IStringBuilder* sb);

private:
    CARAPI Add(
        /* [in] */ ISyncOperation* operation,
        /* [in] */ ISyncStorageEnginePendingOperation* pop,
        /* [out] */ Boolean* result);

private:
    static const String TAG;

private:
    AutoPtr<ISyncStorageEngine> mSyncStorageEngine;
    AutoPtr<SyncAdaptersCache> mSyncAdapters;

    // A Map of SyncOperations operationKey -> SyncOperation that is designed for
    // quick lookup of an enqueued SyncOperation.
    AutoPtr<HashMap<String, AutoPtr<ISyncOperation> > > mOperationsMap;
};

}
}
}

#endif // __SYNCQUEUE_H__
