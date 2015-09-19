
#include "content/pm/CVerifierInfo.h"

namespace Elastos {
namespace Droid {
namespace Content {
namespace Pm {

CVerifierInfo::CVerifierInfo()
{}

CVerifierInfo::~CVerifierInfo()
{}

ECode CVerifierInfo::constructor(
    /* [in] */ const String& packageName,
    /* [in] */ Elastos::Security::IPublicKey* publicKey)
{
    if (packageName.GetLength() == 0) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    VALIDATE_NOT_NULL(publicKey);

    mPackageName = packageName;
    mPublicKey = publicKey;

    return NOERROR;
}

ECode CVerifierInfo::constructor(
    /* [in] */ IParcel* source)
{
    VALIDATE_NOT_NULL(source);

    source->ReadString(&mPackageName);
    source->ReadInterfacePtr((Handle32*)&mPublicKey);

    return NOERROR;
}

ECode CVerifierInfo::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CVerifierInfo::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    VALIDATE_NOT_NULL(dest);

    dest->WriteString(mPackageName);
    dest->WriteInterfacePtr(mPublicKey);

    return NOERROR;
}

ECode CVerifierInfo::GetPackageName(
    /* [out] */ String* packageName)
{
    VALIDATE_NOT_NULL(packageName);
    *packageName = mPackageName;
    return NOERROR;
}

ECode CVerifierInfo::SetPackageName(
    /* [in] */ const String& packageName)
{
    mPackageName = packageName;
    return NOERROR;
}

ECode CVerifierInfo::GetPublicKey(
    /* [out] */ Elastos::Security::IPublicKey** publicKey)
{
    VALIDATE_NOT_NULL(publicKey);
    *publicKey = mPublicKey;
    return NOERROR;
}

ECode CVerifierInfo::SetPublicKey(
    /* [in] */ Elastos::Security::IPublicKey* publicKey)
{
    mPublicKey = publicKey;
    return NOERROR;
}

}
}
}
}
