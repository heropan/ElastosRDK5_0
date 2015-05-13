
#ifndef __CPERIODICSYNC_H__
#define __CPERIODICSYNC_H__

#include "_CPeriodicSync.h"

using Elastos::Droid::Accounts::IAccount;
using Elastos::Droid::Os::IBundle;

namespace Elastos {
namespace Droid {
namespace Content {

CarClass(CPeriodicSync)
{
public:
    CPeriodicSync();

    ~CPeriodicSync();

    /** The account to be synced */
    CARAPI GetAccount(
        /* [out] */ IAccount** account);

    /** The authority of the sync */
    CARAPI GetAuthority(
        /* [out] */ String* authority);

    /** Any extras that parameters that are to be passed to the sync adapter. */
    CARAPI GetExtras(
        /* [out] */ IBundle** extras);

    /** How frequently the sync should be scheduled, in seconds. */
    CARAPI GetPeriod(
        /* [out] */ Int64* period);

    CARAPI Equals(
        /* [in] */ IInterface* obj,
        /* [out] */ Boolean* isEqual);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI constructor();

    /** Creates a new PeriodicSync, copying the Bundle */
    CARAPI constructor(
        /* [in] */ IAccount* account,
        /* [in] */ const String& authority,
        /* [in] */ IBundle* extras,
        /* [in] */ Int64 period);

private:
    /** The account to be synced */
    AutoPtr<IAccount> mAccount;

    /** The authority of the sync */
    String mAuthority;

    /** Any extras that parameters that are to be passed to the sync adapter. */
    AutoPtr<IBundle> mExtras;

    /** How frequently the sync should be scheduled, in seconds. */
    Int64 mPeriod;

};

}
}
}

#endif // __CPERIODICSYNC_H__
