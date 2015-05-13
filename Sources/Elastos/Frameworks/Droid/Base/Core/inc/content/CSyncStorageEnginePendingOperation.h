
#ifndef __CSYNCSTORAGEENGINEPENDINGOPERATION_H__
#define __CSYNCSTORAGEENGINEPENDINGOPERATION_H__

#include "_CSyncStorageEnginePendingOperation.h"

using Elastos::Droid::Accounts::IAccount;
using Elastos::Droid::Os::IBundle;

namespace Elastos {
namespace Droid {
namespace Content {

CarClass(CSyncStorageEnginePendingOperation)
{
public:
    CSyncStorageEnginePendingOperation();

    ~CSyncStorageEnginePendingOperation();

    CARAPI GetAccount(
        /* [out] */ IAccount** account);

    CARAPI GetUserId(
        /* [out] */ Int32* userId);

    CARAPI GetSyncSource(
        /* [out] */ Int32* syncSource);

    CARAPI GetAuthority(
        /* [out] */ String* authority);

    // note: read-only.
    CARAPI GetExtras(
        /* [out] */ IBundle** extras);

    CARAPI GetExpedited(
        /* [out] */ Boolean* expedited);

    CARAPI GetAuthorityId(
        /* [out] */ Int32* authorityId);

    CARAPI SetAuthorityId(
        /* [in] */ Int32 authorityId);

    CARAPI GetFlatExtras(
        /* [out, callee] */ ArrayOf<Byte>** flatExtras);

    CARAPI SetFlatExtras(
        /* [in] */ ArrayOf<Byte>* flatExtras);

    CARAPI constructor(
        /* [in] */ IAccount* account,
        /* [in] */ Int32 userId,
        /* [in] */ Int32 source,
        /* [in] */ const String& authority,
        /* [in] */ IBundle* extras,
        /* [in] */ Boolean expedited);

    CARAPI constructor(
        /* [in] */ ISyncStorageEnginePendingOperation* other);

private:
    AutoPtr<IAccount> mAccount;
    Int32 mUserId;
    Int32 mSyncSource;
    String mAuthority;
    AutoPtr<IBundle> mExtras; // note: read-only.
    Boolean mExpedited;
    Int32 mAuthorityId;
    AutoPtr<ArrayOf<Byte> > mFlatExtras;

};

}
}
}

#endif // __CSYNCSTORAGEENGINEPENDINGOPERATION_H__
