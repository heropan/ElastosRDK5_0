
#include "content/CSyncInfo.h"
#include <ext/frameworkext.h>

namespace Elastos {
namespace Droid {
namespace Content {

CSyncInfo::CSyncInfo()
    : mAuthorityId(0)
    , mAccount(NULL)
    , mStartTime(0)
{}

CSyncInfo::~CSyncInfo()
{}

ECode CSyncInfo::GetAuthorityId(
    /* [out] */ Int32* authorityId)
{
    VALIDATE_NOT_NULL(authorityId)
    *authorityId = mAuthorityId;
    return NOERROR;
}

ECode CSyncInfo::GetAccount(
    /* [out] */ IAccount** account)
{
    VALIDATE_NOT_NULL(account)
    *account = mAccount;
    INTERFACE_ADDREF(*account);
    return NOERROR;
}

ECode CSyncInfo::GetAuthority(
    /* [out] */ String* authority)
{
    VALIDATE_NOT_NULL(authority)
    *authority = mAuthority;
    return NOERROR;
}

ECode CSyncInfo::GetStartTime(
    /* [out] */ Int64* startTime)
{
    VALIDATE_NOT_NULL(startTime)
    *startTime = mStartTime;
    return NOERROR;
}

ECode CSyncInfo::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    source->ReadInt32(&mAuthorityId);
//***    FAIL_RETURN(CAccount::New(String("null"), String("null"), (IAccount*)&mAccount))
    AutoPtr<IParcelable> parcelable = (IParcelable*) mAccount->Probe(EIID_IParcelable);
    FAIL_RETURN(parcelable->ReadFromParcel(source))
    source->ReadString(&mAuthority);
    source->ReadInt64(&mStartTime);
    return NOERROR;
}

ECode CSyncInfo::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    dest->WriteInt32(mAuthorityId);
    AutoPtr<IParcelable> parcelable = (IParcelable*) mAccount->Probe(EIID_IParcelable);
    FAIL_RETURN(parcelable->WriteToParcel(dest))
    dest->WriteString(mAuthority);
    dest->WriteInt64(mStartTime);
    return NOERROR;
}

ECode CSyncInfo::constructor()
{
    return NOERROR;
}

ECode CSyncInfo::constructor(
    /* [in] */ Int32 authorityId,
    /* [in] */ IAccount* account,
    /* [in] */ const String& authority,
    /* [in] */ Int64 startTime)
{
    mAuthorityId = authorityId;
    mAccount = account;
    mAuthority = authority;
    mStartTime = startTime;
    return NOERROR;
}

}
}
}

