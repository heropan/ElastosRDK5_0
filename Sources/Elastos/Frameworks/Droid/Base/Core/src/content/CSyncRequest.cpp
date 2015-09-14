#include "content/CSyncRequest.h"

namespace Elastos {
namespace Droid {
namespace Content {

CAR_INTERFACE_IMPL_2(CSyncRequest, Object, ISyncRequest, IParcelable)

CAR_OBJECT_IMPL(CSyncRequest)

CSyncRequest::CSyncRequest()
    : mDisallowMetered(0)
    , mSyncFlexTimeSecs(0)
    , mSyncRunTimeSecs(0)
    , mIsPeriodic(FALSE)
    , mIsAuthority(FALSE)
    , mIsExpedited(FALSE)
{
}

ECode CSyncRequest:: constructor()
{
    return NOERROR;
}

ECode CSyncRequest:: constructor(
    /* [in] */ ISyncRequestBuilder* b)
{
    mSyncFlexTimeSecs = b.mSyncFlexTimeSecs;
    mSyncRunTimeSecs = b.mSyncRunTimeSecs;
    mAccountToSync = b.mAccount;
    mAuthority = b.mAuthority;
    mIsPeriodic = (b.mSyncType == Builder.SYNC_TYPE_PERIODIC);
    mIsAuthority = (b.mSyncTarget == Builder.SYNC_TARGET_ADAPTER);
    mIsExpedited = b.mExpedited;
    mExtras = new Bundle(b.mCustomExtras);
    // For now we merge the sync config extras & the custom extras into one bundle.
    // TODO: pass the configuration extras through separately.
    mExtras.putAll(b.mSyncConfigExtras);
    mDisallowMetered = b.mDisallowMetered;
    return NOERROR;
}

ECode CSyncRequest:: IsPeriodic(
    /* [out] */ Boolean* periodic)
{
    VALIDATE_NOT_NULL(periodic)
    *periodic = mIsPeriodic;
    return NOERROR;
}

ECode CSyncRequest:: IsExpedited(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    *result = mIsExpedited;
    return NOERROR;
}

ECode CSyncRequest:: GetAccount(
    /* [out] */ IAccount** account)
{
    VALIDATE_NOT_NULL(account)
    *account = mAccount;
    REFCOUNT_ADD(*account)
    return NOERROR;
}

ECode CSyncRequest:: GetProvider(
    /* [out] */ String* provider)
{
    VALIDATE_NOT_NULL(provider)
    *provider = mAuthority;
    return NOERROR;
}

ECode CSyncRequest:: GetBundle(
    /* [out] */ IBundle** extras)
{
    VALIDATE_NOT_NULL(extras)
    *extras = mExtras;
    REFCOUNT_ADD(*extras)
    return NOERROR;
}

ECode CSyncRequest:: GetSyncFlexTime(
    /* [out] */ Int64* time)
{
    VALIDATE_NOT_NULL(time)
    *time = mSyncFlexTimeSecs;
    return NOERROR;
}

ECode CSyncRequest:: GetSyncRunTime(
    /* [out] */ Int64* time)
{
    VALIDATE_NOT_NULL(time)
    *time = mSyncRunTimeSecs;
    return NOERROR;
}

ECode CSyncRequest:: WriteToParcel(
    /* [in] */ IParcel* parcel)
{
    parcel->WriteBundle(mExtras);
    parcel->WriteInt64(mSyncFlexTimeSecs);
    parcel->WriteInt64(mSyncRunTimeSecs);
    parcel->WriteInt32((mIsPeriodic ? 1 : 0));
    parcel->WriteInt32((mDisallowMetered ? 1 : 0));
    parcel->WriteInt32((mIsAuthority ? 1 : 0));
    parcel->WriteInt32((mIsExpedited? 1 : 0));
    IParcelable* p = IParcelable::Probe(mAccountToSync);
    p->WriteToParcel(parcel);
    parcel->WriteString(mAuthority);
    return NOERROR;
}

ECode CSyncRequest:: ReadFromParcel(
    /* [in] */ IParcel* parcel)
{
    AutoPtr<IInterface> obj;
    mExtras = in.readBundle();
    mSyncFlexTimeSecs = in.readLong();
    mSyncRunTimeSecs = in.readLong();
    mIsPeriodic = (in.readInt() != 0);
    mDisallowMetered = (in.readInt() != 0);
    mIsAuthority = (in.readInt() != 0);
    mIsExpedited = (in.readInt() != 0);
    CAccount::New((IAccount**)&mAccountToSync);
    IParcelable::Probe(mAccountToSync)->ReadFromParcel(parcel);
    mAuthority = in.readString();
    return NOERROR;
}

}
}
}