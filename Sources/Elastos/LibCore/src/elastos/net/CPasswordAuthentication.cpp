
#include "CPasswordAuthentication.h"

namespace Elastos {
namespace Net {

CAR_INTERFACE_IMPL(CPasswordAuthentication, Object, IPasswordAuthentication)

CAR_OBJECT_IMPL(CPasswordAuthentication)

CPasswordAuthentication::~CPasswordAuthentication()
{
}

ECode CPasswordAuthentication::constructor(
    /* [in] */ const String& userName,
    /* [in] */ const ArrayOf<Char32>& password)
{
    mUserName = userName;
    mPassword = password.Clone();

    return NOERROR;
}

ECode CPasswordAuthentication::GetPassword(
    /* [out, callee] */ ArrayOf<Char32>** password)
{
    VALIDATE_NOT_NULL(password);
    *password = mPassword->Clone();
    REFCOUNT_ADD(*password);
    return NOERROR;
}

ECode CPasswordAuthentication::GetUserName(
    /* [out] */ String* userName)
{
    VALIDATE_NOT_NULL(userName);
    *userName = mUserName;
    return NOERROR;
}

} // namespace Net
} // namespace Elastos
