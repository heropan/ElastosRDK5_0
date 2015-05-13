
#include "webkit/CAutoFillProfile.h"

namespace Elastos {
namespace Droid {
namespace Webkit {

ECode CAutoFillProfile::constructor(
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
    /* [in] */ const String& phoneNumber)
{
    mUniqueId = uniqueId;
    mFullName = fullName;
    mEmailAddress = email;
    mCompanyName = companyName;
    mAddressLine1 = addressLine1;
    mAddressLine2 = addressLine2;
    mCity = city;
    mState = state;
    mZipCode = zipCode;
    mCountry = country;
    mPhoneNumber = phoneNumber;
    return NOERROR;
}

ECode CAutoFillProfile::GetUniqueId(
    /* [out] */ Int32* id)
{
    VALIDATE_NOT_NULL(id);
    *id = mUniqueId;
    return NOERROR;
}

ECode CAutoFillProfile::GetFullName(
    /* [out] */ String* fullName)
{
    VALIDATE_NOT_NULL(fullName);
    *fullName = mFullName;
    return NOERROR;
}

ECode CAutoFillProfile::GetEmailAddress(
    /* [out] */ String* address)
{
    VALIDATE_NOT_NULL(address);
    *address = mEmailAddress;
    return NOERROR;
}

ECode CAutoFillProfile::GetCompanyName(
    /* [out] */ String* companyName)
{
    VALIDATE_NOT_NULL(companyName);
    *companyName = mCompanyName;
    return NOERROR;
}

ECode CAutoFillProfile::GetAddressLine1(
    /* [out] */ String* addressLine1)
{
    VALIDATE_NOT_NULL(addressLine1);
    *addressLine1 = mAddressLine1;
    return NOERROR;
}

ECode CAutoFillProfile::GetAddressLine2(
    /* [out] */ String* addressLine2)
{
    VALIDATE_NOT_NULL(addressLine2);
    *addressLine2 = mAddressLine2;
    return NOERROR;
}

ECode CAutoFillProfile::GetCity(
    /* [out] */ String* city)
{
    VALIDATE_NOT_NULL(city);
    *city = mCity;
    return NOERROR;
}

ECode CAutoFillProfile::GetState(
    /* [out] */ String* state)
{
    VALIDATE_NOT_NULL(state);
    *state = mState;
    return NOERROR;
}

ECode CAutoFillProfile::GetZipCode(
    /* [out] */ String* zipCode)
{
    VALIDATE_NOT_NULL(zipCode);
    *zipCode = mZipCode;
    return NOERROR;
}

ECode CAutoFillProfile::GetCountry(
    /* [out] */ String* country)
{
    VALIDATE_NOT_NULL(country);
    *country = mCountry;
    return NOERROR;
}

ECode CAutoFillProfile::GetPhoneNumber(
    /* [out] */ String* phoneNumber)
{
    VALIDATE_NOT_NULL(phoneNumber);
    *phoneNumber = mPhoneNumber;
    return NOERROR;
}

} // namespace Webkit
} // namespace Droid
} // namespace Elastos