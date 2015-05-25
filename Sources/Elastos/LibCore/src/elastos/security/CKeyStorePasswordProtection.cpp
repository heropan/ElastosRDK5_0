
#include "CKeyStorePasswordProtection.h"
#include "cmdef.h"

namespace Elastos {
namespace Security {

CKeyStorePasswordProtection::CKeyStorePasswordProtection()
    : mIsDestroyed(FALSE)
{}

ECode CKeyStorePasswordProtection::GetPassword(
    /* [out, callee] */ ArrayOf<Char32> **pwd)
{
    Object::Autolock lock(mLock);
    VALIDATE_NOT_NULL(pwd)
    if (mIsDestroyed) {
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    *pwd = mPassword;
    INTERFACE_ADDREF(*pwd)
    return NOERROR;
}

ECode CKeyStorePasswordProtection::Destroy()
{
    Object::Autolock lock(mLock);
    if (mPassword) {
        mPassword = NULL;
    }
    return NOERROR;
}

ECode CKeyStorePasswordProtection::IsDestroyed(
    /* [out] */ Boolean *isDestroyed)
{
    Object::Autolock lock(mLock);
    VALIDATE_NOT_NULL(isDestroyed)
    *isDestroyed = mIsDestroyed;
    return NOERROR;
}

ECode CKeyStorePasswordProtection::constructor(
    /* [in] */ ArrayOf<Char32> *password)
{
    if (password){
        mPassword = password->Clone();
    }
    return NOERROR;
}

}
}

