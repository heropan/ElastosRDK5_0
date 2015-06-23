
#include "content/CSyncOperation.h"
#include "content/CSyncStorageEngineHelper.h"
#include "os/CBundle.h"

namespace Elastos {
namespace Droid {
namespace Content {

CSyncOperation::CSyncOperation()
    : mAccount(NULL)
    , mUserId(0)
    , mSyncSource(0)
    , mAllowParallelSyncs(FALSE)
    , mExtras(NULL)
    , mEarliestRunTime(0)
    , mExpedited(FALSE)
    , mPendingOperation(NULL)
    , mBackoff(0)
    , mDelayUntil(0)
    , mEffectiveRunTime(0)
{}

CSyncOperation::~CSyncOperation()
{}

ECode CSyncOperation::GetAccount(
    /* [out] */ IAccount** account)
{
    VALIDATE_NOT_NULL(account)
    *account = mAccount;
    REFCOUNT_ADD(*account);
    return NOERROR;
}

ECode CSyncOperation::GetUserId(
    /* [out] */ Int32* userId)
{
    VALIDATE_NOT_NULL(userId)
    *userId = mUserId;
    return NOERROR;
}

ECode CSyncOperation::GetSyncSource(
    /* [out] */ Int32* syncSource)
{
    VALIDATE_NOT_NULL(syncSource)
    *syncSource = mSyncSource;
    return NOERROR;
}

ECode CSyncOperation::SetSyncSource(
    /* [in] */ Int32 syncSource)
{
    mSyncSource = syncSource;
    return NOERROR;
}

ECode CSyncOperation::GetAuthority(
    /* [out] */ String* authority)
{
    VALIDATE_NOT_NULL(authority)
    *authority = mAuthority;
    return NOERROR;
}

ECode CSyncOperation::SetAuthority(
    /* [in] */ const String& authority)
{
    mAuthority = authority;
    return NOERROR;
}

ECode CSyncOperation::AllowParallelSyncs(
    /* [out] */ Boolean* allowParallelSyncs)
{
    VALIDATE_NOT_NULL(allowParallelSyncs)
    *allowParallelSyncs = mAllowParallelSyncs;
    return NOERROR;
}

ECode CSyncOperation::GetExtras(
    /* [out] */ IBundle** extras)
{
    VALIDATE_NOT_NULL(extras)
    *extras = mExtras;
    REFCOUNT_ADD(*extras);
    return NOERROR;
}

ECode CSyncOperation::SetExtras(
    /* [in] */ IBundle* extras)
{
    mExtras = extras;
    return NOERROR;
}

ECode CSyncOperation::GetKey(
    /* [out] */ String* key)
{
    VALIDATE_NOT_NULL(key)
    *key = mKey;
    return NOERROR;
}

ECode CSyncOperation::GetEarliestRunTime(
    /* [out] */ Int64* earliestRunTime)
{
    VALIDATE_NOT_NULL(earliestRunTime)
    *earliestRunTime = mEarliestRunTime;
    return NOERROR;
}

ECode CSyncOperation::SetEarliestRunTime(
    /* [in] */ Int64 earliestRunTime)
{
    mEarliestRunTime = earliestRunTime;
    return NOERROR;
}

ECode CSyncOperation::GetExpedited(
    /* [out] */ Boolean* expedited)
{
    VALIDATE_NOT_NULL(expedited)
    *expedited = mExpedited;
    return NOERROR;
}

ECode CSyncOperation::SetExpedited(
    /* [in] */ Boolean expedited)
{
    mExpedited = expedited;
    return NOERROR;
}

ECode CSyncOperation::GetPendingOperation(
    /* [out] */ ISyncStorageEnginePendingOperation** pendingOperation)
{
    VALIDATE_NOT_NULL(pendingOperation)
    *pendingOperation = mPendingOperation;
    REFCOUNT_ADD(*pendingOperation);
    return NOERROR;
}

ECode CSyncOperation::SetPendingOperation(
    /* [in] */ ISyncStorageEnginePendingOperation* pendingOperation)
{
    mPendingOperation = pendingOperation;
    return NOERROR;
}

ECode CSyncOperation::GetBackoff(
    /* [out] */ Int64* backoff)
{
    VALIDATE_NOT_NULL(backoff)
    *backoff = mBackoff;
    return NOERROR;
}

ECode CSyncOperation::SetBackoff(
    /* [in] */ Int64 backoff)
{
    mBackoff = backoff;
    return NOERROR;
}

ECode CSyncOperation::GetDelayUntil(
    /* [out] */ Int64* delayUntil)
{
    VALIDATE_NOT_NULL(delayUntil)
    *delayUntil = mDelayUntil;
    return NOERROR;
}

ECode CSyncOperation::SetDelayUntil(
    /* [in] */ Int64 delayUntil)
{
    mDelayUntil = delayUntil;
    return NOERROR;
}

ECode CSyncOperation::GetEffectiveRunTime(
    /* [out] */ Int64* effectiveRunTime)
{
    VALIDATE_NOT_NULL(effectiveRunTime)
    *effectiveRunTime = mEffectiveRunTime;
    return NOERROR;
}

ECode CSyncOperation::SetEffectiveRunTime(
    /* [in] */ Int64 effectiveRunTime)
{
    mEffectiveRunTime = effectiveRunTime;
    return NOERROR;
}

ECode CSyncOperation::ToString(
    /* [out] */ String* str)
{
    return Dump(TRUE, str);
}

ECode CSyncOperation::Dump(
    /* [in] */ Boolean useOneLine,
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str)
    AutoPtr<IStringBuilder> sb = new StringBuilder();
    String name;
    FAIL_RETURN(mAccount->GetName(&name))
    sb->AppendString(name);
    sb->AppendString(String(" u"));
    sb->AppendInt32(mUserId);
    sb->AppendString(String(" ("));
    String type;
    FAIL_RETURN(mAccount->GetType(&type))
    sb->AppendString(type);
    sb->AppendString(String(")"));
    sb->AppendString(String(", "));
    sb->AppendString(mAuthority);
    sb->AppendString(String(", "));
    AutoPtr<ISyncStorageEngineHelper> engineHelper;
    FAIL_RETURN(CSyncStorageEngineHelper::AcquireSingleton((ISyncStorageEngineHelper**)&engineHelper))
    String source;
    AutoPtr<ArrayOf<String> > sources;
    FAIL_RETURN(engineHelper->GetSOURCES((ArrayOf<String>**)&sources))
    if (mSyncSource < sources->GetLength()) {
        source = (*sources)[mSyncSource];
    }
    sb->AppendString(source);
    sb->AppendString(String(", earliestRunTime "));
    sb->AppendInt64(mEarliestRunTime);
    if (mExpedited) {
        sb->AppendString(String(", EXPEDITED"));
    }

    if (!useOneLine) {
        if (NULL != mExtras) {
            AutoPtr<IObjectContainer> objContainer;
            FAIL_RETURN(mExtras->KeySet((IObjectContainer**)&objContainer))
            if (NULL != objContainer) {
                Int32 count = 0;
                FAIL_RETURN(objContainer->GetObjectCount(&count))
                if (count > 0) {
                    sb->AppendString(String("\n    "));
                    FAIL_RETURN(ExtrasToStringBuilder(mExtras, sb))
                }
            }
        }
    }

    return sb->ToString(str);
}

ECode CSyncOperation::IsInitialization(
    /* [out] */ Boolean* isInitialization)
{
    VALIDATE_NOT_NULL(isInitialization)
    return mExtras->GetBoolean(IContentResolver::SYNC_EXTRAS_INITIALIZE, FALSE, isInitialization);
}

ECode CSyncOperation::IsExpedited(
    /* [out] */ Boolean* isExpedited)
{
    VALIDATE_NOT_NULL(isExpedited)
    return mExtras->GetBoolean(IContentResolver::SYNC_EXTRAS_EXPEDITED, FALSE, isExpedited);
}

ECode CSyncOperation::IgnoreBackoff(
    /* [out] */ Boolean* ignoreBackoff)
{
    VALIDATE_NOT_NULL(ignoreBackoff)
    return mExtras->GetBoolean(IContentResolver::SYNC_EXTRAS_IGNORE_BACKOFF, FALSE, ignoreBackoff);
}

ECode CSyncOperation::UpdateEffectiveRunTime()
{
    Boolean ignore = FALSE;
    FAIL_RETURN(IgnoreBackoff(&ignore))

    if (ignore) {
        mEffectiveRunTime = mEarliestRunTime;
    }
    else {
        mEffectiveRunTime = Elastos::Core::Math::Max(Elastos::Core::Math::Max(mEarliestRunTime, mDelayUntil), mBackoff);
    }

    return NOERROR;
}

ECode CSyncOperation::CompareTo(
    /* [in] */ IInterface* obj,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(obj)
    VALIDATE_NOT_NULL(result)
    AutoPtr<ISyncOperation> other = (ISyncOperation*) obj;
    Boolean expedited = FALSE;
    FAIL_RETURN(other->GetExpedited(&expedited))

    if (mExpedited != expedited) {
        *result = mExpedited ? -1 : 1;
        return NOERROR;
    }

    Int64 effectiveRunTime = 0;
    FAIL_RETURN(other->GetEffectiveRunTime(&effectiveRunTime))
    if (mEffectiveRunTime == effectiveRunTime) {
        *result = 0;
        return NOERROR;
    }

    *result = mEffectiveRunTime < effectiveRunTime ? -1 : 1;
    return NOERROR;
}

ECode CSyncOperation::constructor(
    /* [in] */ ISyncOperation* other)
{
    VALIDATE_NOT_NULL(other)
    FAIL_RETURN(other->GetAccount((IAccount**)&mAccount))
    FAIL_RETURN(other->GetUserId(&mUserId))
    FAIL_RETURN(other->GetSyncSource(&mSyncSource))
    FAIL_RETURN(other->GetAuthority(&mAuthority))
    AutoPtr<IBundle> extras;
    FAIL_RETURN(other->GetExtras((IBundle**)&extras))
    FAIL_RETURN(CBundle::New(extras, (IBundle**)&mExtras))
    FAIL_RETURN(other->GetExpedited(&mExpedited))
    mEarliestRunTime = SystemClock::GetElapsedRealtime();
    FAIL_RETURN(other->GetBackoff(&mBackoff))
    FAIL_RETURN(other->GetDelayUntil(&mDelayUntil))
    FAIL_RETURN(other->AllowParallelSyncs(&mAllowParallelSyncs))
    FAIL_RETURN(UpdateEffectiveRunTime())
    FAIL_RETURN(ToKey(&mKey))
    return NOERROR;
}

ECode CSyncOperation::constructor(
    /* [in] */ IAccount* account,
    /* [in] */ Int32 userId,
    /* [in] */ Int32 source,
    /* [in] */ const String& authority,
    /* [in] */ IBundle* extras,
    /* [in] */ Int64 delayInMs,
    /* [in] */ Int64 backoff,
    /* [in] */ Int64 delayUntil,
    /* [in] */ Boolean allowParallelSyncs)
{
    mAccount = account;
    mUserId = userId;
    mSyncSource = source;
    mAuthority = authority;
    mAllowParallelSyncs = allowParallelSyncs;
    FAIL_RETURN(CBundle::New(extras, (IBundle**)&mExtras))
    FAIL_RETURN(RemoveFalseExtra(IContentResolver::SYNC_EXTRAS_UPLOAD))
    FAIL_RETURN(RemoveFalseExtra(IContentResolver::SYNC_EXTRAS_MANUAL))
    FAIL_RETURN(RemoveFalseExtra(IContentResolver::SYNC_EXTRAS_IGNORE_SETTINGS))
    FAIL_RETURN(RemoveFalseExtra(IContentResolver::SYNC_EXTRAS_IGNORE_BACKOFF))
    FAIL_RETURN(RemoveFalseExtra(IContentResolver::SYNC_EXTRAS_DO_NOT_RETRY))
    FAIL_RETURN(RemoveFalseExtra(IContentResolver::SYNC_EXTRAS_DISCARD_LOCAL_DELETIONS))
    FAIL_RETURN(RemoveFalseExtra(IContentResolver::SYNC_EXTRAS_EXPEDITED))
    FAIL_RETURN(RemoveFalseExtra(IContentResolver::SYNC_EXTRAS_OVERRIDE_TOO_MANY_DELETIONS))
    mDelayUntil = delayUntil;
    mBackoff = backoff;
    Int64 now = SystemClock::GetElapsedRealtime();

    if (delayInMs < 0) {
        mExpedited = TRUE;
        mEarliestRunTime = now;
    }
    else {
        mExpedited = FALSE;
        mEarliestRunTime = now + delayInMs;
    }

    FAIL_RETURN(UpdateEffectiveRunTime())
    FAIL_RETURN(ToKey(&mKey))
    return NOERROR;
}

ECode CSyncOperation::ExtrasToStringBuilder(
    /* [in] */ IBundle* bundle,
    /* [in] */ IStringBuilder* sb)
{
    sb->AppendString(String("["));
    AutoPtr<IObjectContainer> objContainer;
    FAIL_RETURN(bundle->KeySet((IObjectContainer**)&objContainer))

    if (NULL != objContainer) {
        AutoPtr<IObjectEnumerator> objEnumerator;
        FAIL_RETURN(objContainer->GetObjectEnumerator((IObjectEnumerator**)&objEnumerator))
        Boolean hasNext = FALSE;
        AutoPtr<ICharSequence> keyObj;
        String key;
        while ((objEnumerator->MoveNext(&hasNext), hasNext)) {
            keyObj = NULL;
            FAIL_RETURN(objEnumerator->Current((IInterface**)&keyObj))
            FAIL_RETURN(keyObj->ToString(&key))
            sb->AppendString(key);
            sb->AppendString(String("="));
            AutoPtr<IInterface> obj;
            FAIL_RETURN(bundle->Get(key, (IInterface**)&obj))
            sb->AppendObject(obj);
            sb->AppendString(String(" "));
        }
    }

    sb->AppendString(String("]"));
    return NOERROR;
}

ECode CSyncOperation::ToKey(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str)
    AutoPtr<IStringBuilder> sb = new StringBuilder();
    sb->AppendString(String("authority: "));
    sb->AppendString(mAuthority);
    sb->AppendString(String(" account {name="));
    String name;
    FAIL_RETURN(mAccount->GetName(&name))
    sb->AppendString(name);
    sb->AppendString(String(", user="));
    sb->AppendInt32(mUserId);
    sb->AppendString(String(", type="));
    String type;
    FAIL_RETURN(mAccount->GetType(&type));
    sb->AppendString(String("}"));
    sb->AppendString(String(" extras: "));
    FAIL_RETURN(ExtrasToStringBuilder(mExtras, sb))
    return sb->ToString(str);
}

ECode CSyncOperation::RemoveFalseExtra(
    /* [in] */ const String& extraName)
{
    Boolean ret = FALSE;
    FAIL_RETURN(mExtras->GetBoolean(extraName, FALSE, &ret))

    if (!ret) {
        FAIL_RETURN(mExtras->Remove(extraName))
    }

    return NOERROR;
}

}
}
}

