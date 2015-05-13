
#include "content/CSyncStorageEngine.h"
#include "content/CSyncStorageEngineAuthorityInfo.h"
#include "util/CPair.h"
#include "os/CBundle.h"

using Elastos::Droid::Os::CBundle;
using Elastos::Droid::Os::IBundle;
using Elastos::Droid::Utility::IPair;
using Elastos::Droid::Utility::CPair;
using Elastos::Core::CObjectContainer;

namespace Elastos {
namespace Droid {
namespace Content {

CSyncStorageEngineAuthorityInfo::CSyncStorageEngineAuthorityInfo()
    : mAccount(NULL)
    , mUserId(0)
    , mIdent(0)
    , mEnabled(FALSE)
    , mSyncable(0)
    , mBackoffTime(0)
    , mBackoffDelay(0)
    , mDelayUntil(0)
    , mPeriodicSyncs(new List<AutoPtr<IPair> >())
{}

CSyncStorageEngineAuthorityInfo::~CSyncStorageEngineAuthorityInfo()
{
    mPeriodicSyncs->Clear();
}

ECode CSyncStorageEngineAuthorityInfo::GetAccount(
    /* [out] */ IAccount** account)
{
    VALIDATE_NOT_NULL(account)
    *account = mAccount;
    INTERFACE_ADDREF(*account);
    return NOERROR;
}

ECode CSyncStorageEngineAuthorityInfo::GetUserId(
    /* [out] */ Int32* userId)
{
    VALIDATE_NOT_NULL(userId)
    *userId = mUserId;
    return NOERROR;
}

ECode CSyncStorageEngineAuthorityInfo::GetAuthority(
    /* [out] */ String* authority)
{
    VALIDATE_NOT_NULL(authority)
    *authority = mAuthority;
    return NOERROR;
}

ECode CSyncStorageEngineAuthorityInfo::GetIdent(
    /* [out] */ Int32* ident)
{
    VALIDATE_NOT_NULL(ident)
    *ident = mIdent;
    return NOERROR;
}

ECode CSyncStorageEngineAuthorityInfo::GetEnabled(
    /* [out] */ Boolean* enabled)
{
    VALIDATE_NOT_NULL(enabled)
    *enabled = mEnabled;
    return NOERROR;
}

ECode CSyncStorageEngineAuthorityInfo::SetEnabled(
    /* [in] */ Boolean enabled)
{
    mEnabled = enabled;
    return NOERROR;
}

ECode CSyncStorageEngineAuthorityInfo::GetSyncable(
    /* [out] */ Int32* syncable)
{
    VALIDATE_NOT_NULL(syncable)
    *syncable = mSyncable;
    return NOERROR;
}

ECode CSyncStorageEngineAuthorityInfo::SetSyncable(
    /* [in] */ Int32 syncable)
{
    mSyncable = syncable;
    return NOERROR;
}

ECode CSyncStorageEngineAuthorityInfo::GetBackoffTime(
    /* [out] */ Int64* backoffTime)
{
    VALIDATE_NOT_NULL(backoffTime)
    *backoffTime = mBackoffTime;
    return NOERROR;
}

ECode CSyncStorageEngineAuthorityInfo::SetBackoffTime(
    /* [in] */ Int64 backoffTime)
{
    mBackoffTime = backoffTime;
    return NOERROR;
}

ECode CSyncStorageEngineAuthorityInfo::GetBackoffDelay(
    /* [out] */ Int64* backoffDelay)
{
    VALIDATE_NOT_NULL(backoffDelay)
    *backoffDelay = mBackoffDelay;
    return NOERROR;
}

ECode CSyncStorageEngineAuthorityInfo::SetBackoffDelay(
    /* [in] */ Int64 backoffDelay)
{
    mBackoffDelay = backoffDelay;
    return NOERROR;
}

ECode CSyncStorageEngineAuthorityInfo::GetDelayUntil(
    /* [out] */ Int64* delayUntil)
{
    VALIDATE_NOT_NULL(delayUntil)
    *delayUntil = mDelayUntil;
    return NOERROR;
}

ECode CSyncStorageEngineAuthorityInfo::SetDelayUntil(
    /* [in] */ Int64 delayUntil)
{
    mDelayUntil = delayUntil;
    return NOERROR;
}

ECode CSyncStorageEngineAuthorityInfo::GetPeriodicSyncs(
    /* [out] */ IObjectContainer** periodicSyncs)
{
    VALIDATE_NOT_NULL(periodicSyncs)
    FAIL_RETURN(CObjectContainer::New(periodicSyncs))
    List<AutoPtr<IPair> >::Iterator it = mPeriodicSyncs->Begin();
    AutoPtr<IPair> pair;
    for (; it != mPeriodicSyncs->End(); it++) {
        pair = *it;
        FAIL_RETURN((*periodicSyncs)->Add(pair))
    }
    return NOERROR;
}

ECode CSyncStorageEngineAuthorityInfo::GetPeriodicSyncs(
    /* [out] */ List<AutoPtr<IPair> > **periodicSyncs)
{
    VALIDATE_NOT_NULL(periodicSyncs)
    *periodicSyncs = mPeriodicSyncs;
    INTERFACE_ADDREF(*periodicSyncs)
    return NOERROR;
}

ECode CSyncStorageEngineAuthorityInfo::constructor(
    /* [in] */ ISyncStorageEngineAuthorityInfo* toCopy)
{
    VALIDATE_NOT_NULL(toCopy)
    FAIL_RETURN(toCopy->GetAccount((IAccount**)&mAccount))
    FAIL_RETURN(toCopy->GetUserId(&mUserId))
    FAIL_RETURN(toCopy->GetAuthority(&mAuthority))
    FAIL_RETURN(toCopy->GetIdent(&mIdent))
    FAIL_RETURN(toCopy->GetEnabled(&mEnabled))
    FAIL_RETURN(toCopy->GetSyncable(&mSyncable))
    FAIL_RETURN(toCopy->GetBackoffTime(&mBackoffTime))
    FAIL_RETURN(toCopy->GetBackoffDelay(&mBackoffDelay))
    FAIL_RETURN(toCopy->GetDelayUntil(&mDelayUntil))

    AutoPtr<IObjectContainer> container;
    AutoPtr<IObjectEnumerator> enumerator;
    FAIL_RETURN(toCopy->GetPeriodicSyncs((IObjectContainer**)&container))

    if (NULL != container) {
        FAIL_RETURN(container->GetObjectEnumerator((IObjectEnumerator**)&enumerator))
        Boolean hasNext = FALSE;
        AutoPtr<IPair> pair;
        AutoPtr<IBundle> extras;
        AutoPtr<IInteger64> secondObj;
        Int64 second = 0;

        while ((enumerator->MoveNext(&hasNext), hasNext)) {
            pair = NULL;
            extras = NULL;
            secondObj = NULL;

            FAIL_RETURN(enumerator->Current((IInterface**)&pair))
            FAIL_RETURN(pair->GetFirst((IInterface**)&extras))
            FAIL_RETURN(pair->GetSecond((IInterface**)&secondObj))
            FAIL_RETURN(secondObj->GetValue(&second))
            AutoPtr<IBundle> newExtras;
            FAIL_RETURN(CBundle::New(extras, (IBundle**)&newExtras))
            AutoPtr<IInteger64> newSecond;
            FAIL_RETURN(CInteger64::New(second, (IInteger64**)&newSecond))
            AutoPtr<IPair> newPair;
            FAIL_RETURN(CPair::New(newExtras, newSecond, (IPair**)&newPair))
            mPeriodicSyncs->PushBack(newPair);
        }
    }

    return NOERROR;
}

ECode CSyncStorageEngineAuthorityInfo::constructor(
    /* [in] */ IAccount* account,
    /* [in] */ Int32 userId,
    /* [in] */ const String& authority,
    /* [in] */ Int32 ident)
{
    mAccount = account;
    mUserId = userId;
    mAuthority = authority;
    mIdent = ident;
    mEnabled = CSyncStorageEngine::SYNC_ENABLED_DEFAULT;
    mSyncable = -1; // default to "unknown"
    mBackoffTime = -1; // if < 0 then we aren't in backoff mode
    mBackoffDelay = -1; // if < 0 then we aren't in backoff mode
    AutoPtr<IBundle> extras;
    FAIL_RETURN(CBundle::New((IBundle**)&extras))
    AutoPtr<IInteger64> secondObj;
    FAIL_RETURN(CInteger64::New(CSyncStorageEngine::DEFAULT_POLL_FREQUENCY_SECONDS, (IInteger64**)&secondObj))
    AutoPtr<IPair> pair;
    FAIL_RETURN(CPair::New(extras, secondObj, (IPair**)&pair))
    mPeriodicSyncs->PushBack(pair);
    return NOERROR;
}

}
}
}

