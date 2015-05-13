
#ifndef __CPASSWORDAUTHENTICATION_H__
#define __CPASSWORDAUTHENTICATION_H__

#include "_CPasswordAuthentication.h"

namespace Elastos {
namespace Net {

CarClass(CPasswordAuthentication)
{
public:
    ~CPasswordAuthentication();

    CARAPI constructor(
        /* [in] */ const String& userName,
        /* [in] */ const ArrayOf<Char32>& password);

    CARAPI GetPassword(
        /* [out, callee] */ ArrayOf<Char32>** password);

    CARAPI GetUserName(
        /* [out] */ String* userName);

private:
    String mUserName;

    AutoPtr<ArrayOf<Char32> > mPassword;
};

} // namespace Net
} // namespace Elastos

#endif //__CPASSWORDAUTHENTICATION_H__
