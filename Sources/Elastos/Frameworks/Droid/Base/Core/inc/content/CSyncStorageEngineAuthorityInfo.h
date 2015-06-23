
#ifndef __CSYNCSTORAGEENGINEAUTHORITYINFO_H__
#define __CSYNCSTORAGEENGINEAUTHORITYINFO_H__

#include "_CSyncStorageEngineAuthorityInfo.h"
#include <ext/frameworkext.h>
#include <elastos/utility/etl/List.h>

using Elastos::Utility::Etl::List;
using namespace Elastos::Core;
using Elastos::Droid::Accounts::IAccount;
using Elastos::Droid::Utility::IPair;

namespace Elastos {
namespace Droid {
namespace Content {

CarClass(CSyncStorageEngineAuthorityInfo)
{
public:
    CSyncStorageEngineAuthorityInfo();

    ~CSyncStorageEngineAuthorityInfo();

    CARAPI GetAccount(
        /* [out] */ IAccount** account);

    CARAPI GetUserId(
        /* [out] */ Int32* userId);

    CARAPI GetAuthority(
        /* [out] */ String* authority);

    CARAPI GetIdent(
        /* [out] */ Int32* ident);

    CARAPI GetEnabled(
        /* [out] */ Boolean* enabled);

    CARAPI SetEnabled(
        /* [in] */ Boolean enabled);

    CARAPI GetSyncable(
        /* [out] */ Int32* syncable);

    CARAPI SetSyncable(
        /* [in] */ Int32 syncable);

    CARAPI GetBackoffTime(
        /* [out] */ Int64* backoffTime);

    CARAPI SetBackoffTime(
        /* [in] */ Int64 backoffTime);

    CARAPI GetBackoffDelay(
        /* [out] */ Int64* backoffDelay);

    CARAPI SetBackoffDelay(
        /* [in] */ Int64 backoffDelay);

    CARAPI GetDelayUntil(
        /* [out] */ Int64* delayUntil);

    CARAPI SetDelayUntil(
        /* [in] */ Int64 delayUntil);

    CARAPI GetPeriodicSyncs(
        /* [out] */ IObjectContainer** periodicSyncs);

    CARAPI GetPeriodicSyncs(
        /* [out] */ List<AutoPtr<IPair> > **periodicSyncs);

    /**
     * Copy constructor for making deep-ish copies. Only the bundles stored
     * in periodic syncs can make unexpected changes.
     *
     * @param toCopy AuthorityInfo to be copied.
     */
    CARAPI constructor(
        /* [in] */ ISyncStorageEngineAuthorityInfo* toCopy);

    CARAPI constructor(
        /* [in] */ IAccount* account,
        /* [in] */ Int32 userId,
        /* [in] */ const String& authority,
        /* [in] */ Int32 ident);

private:
    AutoPtr<IAccount> mAccount;
    Int32 mUserId;
    String mAuthority;
    Int32 mIdent;
    Boolean mEnabled;
    Int32 mSyncable;
    Int64 mBackoffTime;
    Int64 mBackoffDelay;
    Int64 mDelayUntil;
    AutoPtr<List<AutoPtr<IPair> > > mPeriodicSyncs;

};

}
}
}

#endif // __CSYNCSTORAGEENGINEAUTHORITYINFO_H__
