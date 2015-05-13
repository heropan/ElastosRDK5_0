
#ifndef __CSYNCOPERATION_H__
#define __CSYNCOPERATION_H__

#include "_CSyncOperation.h"
#include <ext/frameworkext.h>
#include <elastos/StringBuilder.h>
#include <elastos/Math.h>
#include "../os/SystemClock.h"

namespace Elastos {
namespace Droid {
namespace Content {

using namespace Elastos::Core;
using namespace Elastos::Droid::Os;
using Elastos::Droid::Accounts::IAccount;

CarClass(CSyncOperation)
{
public:
    CSyncOperation();

    ~CSyncOperation();

    CARAPI GetAccount(
        /* [out] */ IAccount** account);

    CARAPI GetUserId(
        /* [out] */ Int32* userId);

    CARAPI GetSyncSource(
        /* [out] */ Int32* syncSource);

    CARAPI SetSyncSource(
        /* [in] */ Int32 syncSource);

    CARAPI GetAuthority(
        /* [out] */ String* authority);

    CARAPI SetAuthority(
        /* [in] */ const String& authority);

    CARAPI AllowParallelSyncs(
        /* [out] */ Boolean* allowParallelSyncs);

    CARAPI GetExtras(
        /* [out] */ IBundle** extras);

    CARAPI SetExtras(
        /* [in] */ IBundle* extras);

    CARAPI GetKey(
        /* [out] */ String* key);

    CARAPI GetEarliestRunTime(
        /* [out] */ Int64* earliestRunTime);

    CARAPI SetEarliestRunTime(
        /* [in] */ Int64 earliestRunTime);

    CARAPI GetExpedited(
        /* [out] */ Boolean* expedited);

    CARAPI SetExpedited(
        /* [in] */ Boolean expedited);

    CARAPI GetPendingOperation(
        /* [out] */ ISyncStorageEnginePendingOperation** pendingOperation);

    CARAPI SetPendingOperation(
        /* [in] */ ISyncStorageEnginePendingOperation* pendingOperation);

    CARAPI GetBackoff(
        /* [out] */ Int64* backoff);

    CARAPI SetBackoff(
        /* [in] */ Int64 backoff);

    CARAPI GetDelayUntil(
        /* [out] */ Int64* delayUntil);

    CARAPI SetDelayUntil(
        /* [in] */ Int64 delayUntil);

    CARAPI GetEffectiveRunTime(
        /* [out] */ Int64* effectiveRunTime);

    CARAPI SetEffectiveRunTime(
        /* [in] */ Int64 effectiveRunTime);

    CARAPI ToString(
        /* [out] */ String* str);

    CARAPI Dump(
        /* [in] */ Boolean useOneLine,
        /* [out] */ String* str);

    CARAPI IsInitialization(
        /* [out] */ Boolean* isInitialization);

    CARAPI IsExpedited(
        /* [out] */ Boolean* isExpedited);

    CARAPI IgnoreBackoff(
        /* [out] */ Boolean* ignoreBackoff);

    CARAPI UpdateEffectiveRunTime();

    CARAPI CompareTo(
        /* [in] */ IInterface* obj,
        /* [out] */ Int32* result);

    CARAPI constructor(
        /* [in] */ ISyncOperation* other);

    CARAPI constructor(
        /* [in] */ IAccount* account,
        /* [in] */ Int32 userId,
        /* [in] */ Int32 source,
        /* [in] */ const String& authority,
        /* [in] */ IBundle* extras,
        /* [in] */ Int64 delayInMs,
        /* [in] */ Int64 backoff,
        /* [in] */ Int64 delayUntil,
        /* [in] */ Boolean allowParallelSyncs);

public:
    static CARAPI ExtrasToStringBuilder(
        /* [in] */ IBundle* bundle,
        /* [in] */ IStringBuilder* sb);

private:
    CARAPI ToKey(
        /* [out] */ String* str);

    CARAPI RemoveFalseExtra(
        /* [in] */ const String& extraName);

private:
    AutoPtr<IAccount> mAccount;
    Int32 mUserId;
    Int32 mSyncSource;
    String mAuthority;
    Boolean mAllowParallelSyncs;
    AutoPtr<IBundle> mExtras;
    String mKey;
    Int64 mEarliestRunTime;
    Boolean mExpedited;
    AutoPtr<ISyncStorageEnginePendingOperation> mPendingOperation;
    Int64 mBackoff;
    Int64 mDelayUntil;
    Int64 mEffectiveRunTime;

};

}
}
}

#endif // __CSYNCOPERATION_H__
