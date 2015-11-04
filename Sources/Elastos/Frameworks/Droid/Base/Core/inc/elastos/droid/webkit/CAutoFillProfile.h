
#ifndef __ELASTOS_DROID_WEBKIT_CAUTOFILLPROFILE_H__
#define __ELASTOS_DROID_WEBKIT_CAUTOFILLPROFILE_H__

#include "elastos/droid/ext/frameworkext.h"
#include "_Elastos_Droid_Webkit_CAutoFillProfile.h"

namespace Elastos {
namespace Droid {
namespace Webkit {

CarClass(CAutoFillProfile)
    , public Object
    , public IAutoFillProfile
{
public:
    CAR_INTERFACE_DECL()

    CAR_OBJECT_DECL()

    CARAPI constructor(
        /* [in] */ Int32 uniqueId,
        /* [in] */ const String& fullName,
        /* [in] */ const String& email,
        /* [in] */ const String& companyName,
        /* [in] */ const String& addressLine1,
        /* [in] */ const String& addressLine2,
        /* [in] */ const String& city,
        /* [in] */ const String& state,
        /* [in] */ const String& zipCode,
        /* [in] */ const String& country,
        /* [in] */ const String& phoneNumber);

    CARAPI GetUniqueId(
        /* [out] */ Int32* id);

    CARAPI GetFullName(
        /* [out] */ String* fullName);

    CARAPI GetEmailAddress(
        /* [out] */ String* address);

    CARAPI GetCompanyName(
        /* [out] */ String* companyName);

    CARAPI GetAddressLine1(
        /* [out] */ String* addressLine1);

    CARAPI GetAddressLine2(
        /* [out] */ String* addressLine2);

    CARAPI GetCity(
        /* [out] */ String* city);

    CARAPI GetState(
        /* [out] */ String* state);

    CARAPI GetZipCode(
        /* [out] */ String* zipCode);

    CARAPI GetCountry(
        /* [out] */ String* country);

    CARAPI GetPhoneNumber(
        /* [out] */ String* phoneNumber);

    virtual CARAPI_(Int32) GetUniqueId() { return mUniqueId; }
    virtual CARAPI_(String) GetFullName() { return mFullName; }
    virtual CARAPI_(String) GetEmailAddress() { return mEmailAddress; }
    virtual CARAPI_(String) GetCompanyName() { return mCompanyName; }
    virtual CARAPI_(String) GetAddressLine1() { return mAddressLine1; }
    virtual CARAPI_(String) GetAddressLine2() { return mAddressLine2; }
    virtual CARAPI_(String) GetCity() { return mCity; }
    virtual CARAPI_(String) GetState() { return mState; }
    virtual CARAPI_(String) GetZipCode() { return mZipCode; }
    virtual CARAPI_(String) GetCountry() { return mCountry; }
    virtual CARAPI_(String) GetPhoneNumber() { return mPhoneNumber; }

private:
    Int32 mUniqueId;
    String mFullName;
    String mEmailAddress;
    String mCompanyName;
    String mAddressLine1;
    String mAddressLine2;
    String mCity;
    String mState;
    String mZipCode;
    String mCountry;
    String mPhoneNumber;
};

} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_WEBKIT_CAUTOFILLPROFILE_H__
