
#include "content/CPeriodicSync.h"
#include "content/CSyncStorageEngineHelper.h"
#include "os/CBundle.h"
#include <ext/frameworkext.h>

using Elastos::Droid::Os::CBundle;

namespace Elastos {
namespace Droid {
namespace Content {

CPeriodicSync::CPeriodicSync()
    : mAccount(NULL)
    , mExtras(NULL)
    , mPeriod(0)
{}

CPeriodicSync::~CPeriodicSync()
{}

ECode CPeriodicSync::GetAccount(
    /* [out] */ IAccount** account)
{
    VALIDATE_NOT_NULL(account)
    *account = mAccount;
    INTERFACE_ADDREF(*account);
    return NOERROR;
}

ECode CPeriodicSync::GetAuthority(
    /* [out] */ String* authority)
{
    VALIDATE_NOT_NULL(authority)
    *authority = mAuthority;
    return NOERROR;
}

ECode CPeriodicSync::GetExtras(
    /* [out] */ IBundle** extras)
{
    VALIDATE_NOT_NULL(extras)
    *extras = mExtras;
    INTERFACE_ADDREF(*extras);
    return NOERROR;
}

ECode CPeriodicSync::GetPeriod(
    /* [out] */ Int64* period)
{
    VALIDATE_NOT_NULL(period)
    *period = mPeriod;
    return NOERROR;
}

ECode CPeriodicSync::Equals(
    /* [in] */ IInterface* obj,
    /* [out] */ Boolean* isEqual)
{
    VALIDATE_NOT_NULL(isEqual)

    if (_CObject_Compare(obj, (IPeriodicSync*) this)) {
        *isEqual = TRUE;
        return NOERROR;
    }

    if (!(IPeriodicSync::Probe(obj) != NULL)) {
        *isEqual = FALSE;
        return NOERROR;
    }

    AutoPtr<IPeriodicSync> other = (IPeriodicSync*) obj;
    AutoPtr<IAccount> account;
    AutoPtr<IBundle> extras;
    String authority;
    Int64 period = 0;
    FAIL_RETURN(other->GetAccount((IAccount**)&account))
    FAIL_RETURN(other->GetExtras((IBundle**)&extras))
    FAIL_RETURN(other->GetAuthority(&authority))
    FAIL_RETURN(other->GetPeriod(&period))
    Boolean ret = FALSE;
    AutoPtr<ISyncStorageEngineHelper> engineHelper;
    FAIL_RETURN(CSyncStorageEngineHelper::AcquireSingleton((ISyncStorageEngineHelper**)&engineHelper))
    FAIL_RETURN(engineHelper->Equals(mExtras, extras, &ret))

    *isEqual = _CObject_Compare(mAccount, account)
            && mAuthority.Equals(authority)
            && mPeriod == period
            && ret;

    return NOERROR;
}

ECode CPeriodicSync::ReadFromParcel(
    /* [in] */ IParcel* source)
{
//***    FAIL_RETURN(CAccount::New(String("null"), String("null"), (IAccount*)&mAccount))
    AutoPtr<IParcelable> parcelable = (IParcelable*) mAccount->Probe(EIID_IParcelable);
    FAIL_RETURN(parcelable->ReadFromParcel(source))
    source->ReadString(&mAuthority);
    source->ReadInterfacePtr((Handle32*)&mExtras);
    source->ReadInt64(&mPeriod);
    return NOERROR;
}

ECode CPeriodicSync::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    AutoPtr<IParcelable> parcelable = (IParcelable*) mAccount->Probe(EIID_IParcelable);
    FAIL_RETURN(parcelable->WriteToParcel(dest))
    dest->WriteString(mAuthority);
    dest->WriteInterfacePtr(mExtras);
    dest->WriteInt64(mPeriod);
    return NOERROR;
}

ECode CPeriodicSync::constructor()
{
    return NOERROR;
}

ECode CPeriodicSync::constructor(
    /* [in] */ IAccount* account,
    /* [in] */ const String& authority,
    /* [in] */ IBundle* extras,
    /* [in] */ Int64 period)
{
    mAccount = account;
    mAuthority = authority;
    FAIL_RETURN(CBundle::New(extras, (IBundle**)&mExtras))
    mPeriod = period;
    return NOERROR;
}

} // namespace Content
} // namespace Elastos
} // namespace Droid

