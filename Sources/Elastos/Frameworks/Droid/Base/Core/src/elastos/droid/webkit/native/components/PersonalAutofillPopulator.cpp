
#include "elastos/droid/webkit/native/components/PersonalAutofillPopulator.h"
#include "elastos/droid/provider/ContactsContractProfile.h"

using Elastos::Droid::Provider::ContactsContractProfile;
using Elastos::Droid::Provider::IContactsContractContactsData;
using Elastos::Droid::Provider::IContactsContractCommonDataKindsEmail;
using Elastos::Droid::Provider::IContactsContractCommonDataKindsPhone;
using Elastos::Droid::Provider::IContactsContractCommonDataKindsStructuredPostal;
using Elastos::Droid::Provider::IContactsContractCommonDataKindsStructuredName;
using Elastos::Droid::Content::Pm::IPackageManager;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Components {

//=====================================================================
//               PersonalAutofillPopulator::ProfileQuery
//=====================================================================
AutoPtr<IUri> PersonalAutofillPopulator::ProfileQuery::mProfileDataUri = PersonalAutofillPopulator::ProfileQuery::MiddleInitProfiledataUri();

AutoPtr<IUri> PersonalAutofillPopulator::ProfileQuery::MiddleInitProfiledataUri()
{
    // ==================before translated======================
    // ->WWZ_SIGN: FUNC_CALL_START {
    // ContactsContract.Profile.CONTENT_URI,
    //                  ContactsContract.Contacts.Data.CONTENT_DIRECTORY
    // ->WWZ_SIGN: FUNC_CALL_END }

    assert(0);
    AutoPtr<IUri> uri;
    //Uri::WithAppendedPath(ContactsContract.Profile.CONTENT_URI, ContactsContract.Contacts.Data.CONTENT_DIRECTORY, (IUri**)&uri);
    return uri;
}

//=====================================================================
//             PersonalAutofillPopulator::EmailProfileQuery
//=====================================================================
const Int32 PersonalAutofillPopulator::EmailProfileQuery::EMAIL_ADDRESS;

AutoPtr< ArrayOf<String> > PersonalAutofillPopulator::EmailProfileQuery::Projection()
{
    // ==================before translated======================
    // return new String[] {
    //     ContactsContract.CommonDataKinds.Email.ADDRESS,
    // };

    AutoPtr< ArrayOf<String> > result = ArrayOf<String>::Alloc(1);
    result->Set(0, IContactsContractCommonDataKindsEmail::ADDRESS);
    return result;
}

String PersonalAutofillPopulator::EmailProfileQuery::MimeType()
{
    // ==================before translated======================
    // return ContactsContract.CommonDataKinds.Email.CONTENT_ITEM_TYPE;

    String result = IContactsContractCommonDataKindsEmail::CONTENT_ITEM_TYPE;
    return result;
}

//=====================================================================
//             PersonalAutofillPopulator::PhoneProfileQuery
//=====================================================================
const Int32 PersonalAutofillPopulator::PhoneProfileQuery::NUMBER;

AutoPtr< ArrayOf<String> > PersonalAutofillPopulator::PhoneProfileQuery::Projection()
{
    // ==================before translated======================
    // return new String[] {
    //     ContactsContract.CommonDataKinds.Phone.NUMBER,
    // };

    AutoPtr< ArrayOf<String> > result = ArrayOf<String>::Alloc(1);
    result->Set(0, IContactsContractCommonDataKindsPhone::NUMBER);
    return result;
}

String PersonalAutofillPopulator::PhoneProfileQuery::MimeType()
{
    // ==================before translated======================
    // return ContactsContract.CommonDataKinds.Phone.CONTENT_ITEM_TYPE;

    String result = IContactsContractCommonDataKindsPhone::CONTENT_ITEM_TYPE;
    return result;
}

//=====================================================================
//            PersonalAutofillPopulator::AddressProfileQuery
//=====================================================================
const Int32 PersonalAutofillPopulator::AddressProfileQuery::STREET;
const Int32 PersonalAutofillPopulator::AddressProfileQuery::POBOX;
const Int32 PersonalAutofillPopulator::AddressProfileQuery::NEIGHBORHOOD;
const Int32 PersonalAutofillPopulator::AddressProfileQuery::CITY;
const Int32 PersonalAutofillPopulator::AddressProfileQuery::REGION;
const Int32 PersonalAutofillPopulator::AddressProfileQuery::POSTALCODE;
const Int32 PersonalAutofillPopulator::AddressProfileQuery::COUNTRY;

AutoPtr< ArrayOf<String> > PersonalAutofillPopulator::AddressProfileQuery::Projection()
{
    // ==================before translated======================
    // return new String[] {
    //     ContactsContract.CommonDataKinds.StructuredPostal.STREET,
    //         ContactsContract.CommonDataKinds.StructuredPostal.POBOX,
    //         ContactsContract.CommonDataKinds.StructuredPostal.NEIGHBORHOOD,
    //         ContactsContract.CommonDataKinds.StructuredPostal.CITY,
    //         ContactsContract.CommonDataKinds.StructuredPostal.REGION,
    //         ContactsContract.CommonDataKinds.StructuredPostal.POSTCODE,
    //         ContactsContract.CommonDataKinds.StructuredPostal.COUNTRY,
    // };

    AutoPtr< ArrayOf<String> > result = ArrayOf<String>::Alloc(7);
    result->Set(0, IContactsContractCommonDataKindsStructuredPostal::STREET);
    result->Set(1, IContactsContractCommonDataKindsStructuredPostal::POBOX);
    result->Set(2, IContactsContractCommonDataKindsStructuredPostal::NEIGHBORHOOD);
    result->Set(3, IContactsContractCommonDataKindsStructuredPostal::CITY);
    result->Set(4, IContactsContractCommonDataKindsStructuredPostal::REGION);
    result->Set(5, IContactsContractCommonDataKindsStructuredPostal::POSTCODE);
    result->Set(6, IContactsContractCommonDataKindsStructuredPostal::COUNTRY);
    return result;
}

String PersonalAutofillPopulator::AddressProfileQuery::MimeType()
{
    // ==================before translated======================
    // return ContactsContract.CommonDataKinds.StructuredPostal.CONTENT_ITEM_TYPE;

    String result = IContactsContractCommonDataKindsStructuredPostal::CONTENT_ITEM_TYPE;
    return result;
}

//=====================================================================
//             PersonalAutofillPopulator::NameProfileQuery
//=====================================================================
const Int32 PersonalAutofillPopulator::NameProfileQuery::GIVEN_NAME;
const Int32 PersonalAutofillPopulator::NameProfileQuery::MIDDLE_NAME;
const Int32 PersonalAutofillPopulator::NameProfileQuery::FAMILY_NAME;
const Int32 PersonalAutofillPopulator::NameProfileQuery::SUFFIX;

AutoPtr< ArrayOf<String> > PersonalAutofillPopulator::NameProfileQuery::Projection()
{
    // ==================before translated======================
    // return new String[] {
    //     ContactsContract.CommonDataKinds.StructuredName.GIVEN_NAME,
    //         ContactsContract.CommonDataKinds.StructuredName.MIDDLE_NAME,
    //         ContactsContract.CommonDataKinds.StructuredName.FAMILY_NAME,
    //         ContactsContract.CommonDataKinds.StructuredName.SUFFIX
    // };

    AutoPtr< ArrayOf<String> > result = ArrayOf<String>::Alloc(4);
    result->Set(0, IContactsContractCommonDataKindsStructuredName::GIVEN_NAME);
    result->Set(1, IContactsContractCommonDataKindsStructuredName::MIDDLE_NAME);
    result->Set(2, IContactsContractCommonDataKindsStructuredName::FAMILY_NAME);
    result->Set(3, IContactsContractCommonDataKindsStructuredName::SUFFIX);
    return result;
}

String PersonalAutofillPopulator::NameProfileQuery::MimeType()
{
    // ==================before translated======================
    // return ContactsContract.CommonDataKinds.StructuredName.CONTENT_ITEM_TYPE;

    String result = IContactsContractCommonDataKindsStructuredName::CONTENT_ITEM_TYPE;
    return result;
}

//=====================================================================
//                      PersonalAutofillPopulator
//=====================================================================
PersonalAutofillPopulator::PersonalAutofillPopulator(
    /* [in] */ IContext* context)
{
    // ==================before translated======================
    // mHasPermissions = hasPermissions(context);
    // if (mHasPermissions) {
    //     ContentResolver contentResolver = context.getContentResolver();
    //     populateName(contentResolver);
    //     populateEmail(contentResolver);
    //     populateAddress(contentResolver);
    //     populatePhone(contentResolver);
    // }

    mHasPermissions = HasPermissions(context);
    if (mHasPermissions) {
        AutoPtr<IContentResolver> contentResolver;
        context->GetContentResolver((IContentResolver**)&contentResolver);
        PopulateName(contentResolver);
        PopulateEmail(contentResolver);
        PopulateAddress(contentResolver);
        PopulatePhone(contentResolver);
    }
}

AutoPtr<PersonalAutofillPopulator> PersonalAutofillPopulator::Create(
    /* [in] */ IContext* context)
{
    // ==================before translated======================
    // return new PersonalAutofillPopulator(context);

    AutoPtr<PersonalAutofillPopulator> result = new PersonalAutofillPopulator(context);
    return result;
}

AutoPtr<ICursor> PersonalAutofillPopulator::CursorFromProfileQuery(
    /* [in] */ ProfileQuery* query,
    /* [in] */ IContentResolver* contentResolver)
{
    // ==================before translated======================
    // String sortDescriptor = ContactsContract.Contacts.Data.IS_PRIMARY + " DESC";
    // return contentResolver.query(
    //         query.profileDataUri,
    //         query.projection(),
    //         ContactsContract.Contacts.Data.MIMETYPE + " = ?",
    //         new String[]{query.mimeType()},
    //         sortDescriptor
    //         );

    String sortDescriptor = String(""); //wwz: IS_PRIMARY isnot exist now; IContactsContractContactsData::IS_PRIMARY + " DESC";
    AutoPtr<ICursor> result;
    AutoPtr< ArrayOf<String> > mimeType = ArrayOf<String>::Alloc(1);
    mimeType->Set(0, query->MimeType());
    contentResolver->Query(
        query->mProfileDataUri,
        query->Projection(),
        /* wwz: mIMETYPE isnot exist; IContactsContractContactsData::MIMETYPE + */ String(" = ?"),
        mimeType,
        sortDescriptor,
        (ICursor**)&result
        );

    return result;
}

Boolean PersonalAutofillPopulator::HasPermissions(
    /* [in] */ IContext* context)
{
    // ==================before translated======================
    // String [] permissions = {
    //     "android.permission.READ_CONTACTS",
    //     "android.permission.READ_PROFILE"
    // };
    // for (String permission : permissions) {
    //     int res = context.checkCallingOrSelfPermission(permission);
    //     if (res != PackageManager.PERMISSION_GRANTED) return false;
    // }
    // return true;

    AutoPtr< ArrayOf<String> > permissions = ArrayOf<String>::Alloc(2);
    String str0("android.permission.READ_CONTACTS");
    String str1("android.permission.READ_PROFILE");
    permissions->Set(0, str0);
    permissions->Set(1, str1);
    String permission("");
    Int32 res = 0;
    for (Int32 idx = 0; idx < permissions->GetLength(); ++idx) {
        permission = (*permissions)[idx];
        context->CheckCallingOrSelfPermission(permission, &res);
        if (res != IPackageManager::PERMISSION_GRANTED) return FALSE;
    }

    return TRUE;
}

ECode PersonalAutofillPopulator::PopulateName(
    /* [in] */ IContentResolver* contentResolver)
{
    VALIDATE_NOT_NULL(contentResolver);
    // ==================before translated======================
    // NameProfileQuery nameProfileQuery = new NameProfileQuery();
    // Cursor nameCursor = cursorFromProfileQuery(nameProfileQuery, contentResolver);
    // if (nameCursor.moveToNext()) {
    //     mGivenName = nameCursor.getString(nameProfileQuery.GIVEN_NAME);
    //     mMiddleName = nameCursor.getString(nameProfileQuery.MIDDLE_NAME);
    //     mFamilyName = nameCursor.getString(nameProfileQuery.FAMILY_NAME);
    //     mSuffix = nameCursor.getString(nameProfileQuery.SUFFIX);
    // }
    // nameCursor.close();

    AutoPtr<NameProfileQuery> nameProfileQuery = new NameProfileQuery();
    AutoPtr<ICursor> nameCursor = CursorFromProfileQuery(nameProfileQuery, contentResolver);
    Boolean moveToNext = FALSE;
    nameCursor->MoveToNext(&moveToNext);
    if (moveToNext) {
        nameCursor->GetString(nameProfileQuery->GIVEN_NAME, &mGivenName);
        nameCursor->GetString(nameProfileQuery->MIDDLE_NAME, &mMiddleName);
        nameCursor->GetString(nameProfileQuery->FAMILY_NAME, &mFamilyName);
        nameCursor->GetString(nameProfileQuery->SUFFIX, &mSuffix);
    }
    //wwz: func Close isnot exist; nameCursor->Close();

    return NOERROR;
}

ECode PersonalAutofillPopulator::PopulateEmail(
    /* [in] */ IContentResolver* contentResolver)
{
    VALIDATE_NOT_NULL(contentResolver);
    // ==================before translated======================
    // EmailProfileQuery emailProfileQuery = new EmailProfileQuery();
    // Cursor emailCursor = cursorFromProfileQuery(emailProfileQuery, contentResolver);
    // mEmailAddresses = new String[emailCursor.getCount()];
    // for (int i = 0; emailCursor.moveToNext(); i++) {
    //     mEmailAddresses[i] = emailCursor.getString(emailProfileQuery.EMAIL_ADDRESS);
    // }
    // emailCursor.close();

    AutoPtr<EmailProfileQuery> emailProfileQuery = new EmailProfileQuery();
    AutoPtr<ICursor> emailCursor = CursorFromProfileQuery(emailProfileQuery, contentResolver);
    Int32 count = 0;
    emailCursor->GetCount(&count);
    mEmailAddresses = ArrayOf<String>::Alloc(count);
    Boolean moveToNext = FALSE;
    String item("");
    for (Int32 i = 0; emailCursor->MoveToNext(&moveToNext), moveToNext; ++i) {
        emailCursor->GetString(emailProfileQuery->EMAIL_ADDRESS, &item);
        (*mEmailAddresses)[i] = item;
    }
    //wwz: func Close isnot exist; emailCursor->Close();

    return NOERROR;
}

ECode PersonalAutofillPopulator::PopulateAddress(
    /* [in] */ IContentResolver* contentResolver)
{
    VALIDATE_NOT_NULL(contentResolver);
    // ==================before translated======================
    // AddressProfileQuery addressProfileQuery = new AddressProfileQuery();
    // Cursor addressCursor = cursorFromProfileQuery(addressProfileQuery, contentResolver);
    // if(addressCursor.moveToNext()) {
    //     mPobox = addressCursor.getString(addressProfileQuery.POBOX);
    //     mStreet = addressCursor.getString(addressProfileQuery.STREET);
    //     mNeighborhood = addressCursor.getString(addressProfileQuery.NEIGHBORHOOD);
    //     mCity = addressCursor.getString(addressProfileQuery.CITY);
    //     mRegion = addressCursor.getString(addressProfileQuery.REGION);
    //     mPostalCode = addressCursor.getString(addressProfileQuery.POSTALCODE);
    //     mCountry = addressCursor.getString(addressProfileQuery.COUNTRY);
    // }
    // addressCursor.close();

    AutoPtr<AddressProfileQuery> addressProfileQuery = new AddressProfileQuery();
    AutoPtr<ICursor> addressCursor = CursorFromProfileQuery(addressProfileQuery, contentResolver);
    Boolean moveToNext = FALSE;
    addressCursor->MoveToNext(&moveToNext);
    if (moveToNext) {
        addressCursor->GetString(addressProfileQuery->POBOX, &mPobox);
        addressCursor->GetString(addressProfileQuery->STREET, &mStreet);
        addressCursor->GetString(addressProfileQuery->NEIGHBORHOOD, &mNeighborhood);
        addressCursor->GetString(addressProfileQuery->CITY, &mCity);
        addressCursor->GetString(addressProfileQuery->REGION, &mRegion);
        addressCursor->GetString(addressProfileQuery->POSTALCODE, &mPostalCode);
        addressCursor->GetString(addressProfileQuery->COUNTRY, &mCountry);
    }
    //wwz: func Close isnot exist; addressCursor->Close();

    return NOERROR;
}

ECode PersonalAutofillPopulator::PopulatePhone(
    /* [in] */ IContentResolver* contentResolver)
{
    VALIDATE_NOT_NULL(contentResolver);
    // ==================before translated======================
    // PhoneProfileQuery phoneProfileQuery = new PhoneProfileQuery();
    // Cursor phoneCursor = cursorFromProfileQuery(phoneProfileQuery, contentResolver);
    // mPhoneNumbers = new String[phoneCursor.getCount()];
    // for (int i = 0; phoneCursor.moveToNext(); i++) {
    //     mPhoneNumbers[i] = phoneCursor.getString(phoneProfileQuery.NUMBER);
    // }
    // phoneCursor.close();

    AutoPtr<PhoneProfileQuery> phoneProfileQuery = new PhoneProfileQuery();
    AutoPtr<ICursor> phoneCursor = CursorFromProfileQuery(phoneProfileQuery, contentResolver);
    Int32 count = 0;
    phoneCursor->GetCount(&count);
    mPhoneNumbers = ArrayOf<String>::Alloc(count);
    Boolean moveToNext = FALSE;
    String item("");
    for (Int32 i = 0; phoneCursor->MoveToNext(&moveToNext), moveToNext; ++i) {
        phoneCursor->GetString(phoneProfileQuery->NUMBER, &item);
        (*mPhoneNumbers)[i] = item;
    }
    //wwz: func Close isnot exist; phoneCursor->Close();

    return NOERROR;
}

String PersonalAutofillPopulator::GetFirstName()
{
    // ==================before translated======================
    // return mGivenName;

    return mGivenName;
}

String PersonalAutofillPopulator::GetLastName()
{
    // ==================before translated======================
    // return mFamilyName;

    return mFamilyName;
}

String PersonalAutofillPopulator::GetMiddleName()
{
    // ==================before translated======================
    // return mMiddleName;

    return mMiddleName;
}

String PersonalAutofillPopulator::GetSuffix()
{
    // ==================before translated======================
    // return mSuffix;

    return mSuffix;
}

AutoPtr< ArrayOf<String> > PersonalAutofillPopulator::GetEmailAddresses()
{
    // ==================before translated======================
    // return mEmailAddresses;

    return mEmailAddresses;
}

String PersonalAutofillPopulator::GetStreet()
{
    // ==================before translated======================
    // return mStreet;

    return mStreet;
}

String PersonalAutofillPopulator::GetPobox()
{
    // ==================before translated======================
    // return mPobox;

    return mPobox;
}

String PersonalAutofillPopulator::GetNeighborhood()
{
    // ==================before translated======================
    // return mNeighborhood;

    return mNeighborhood;
}

String PersonalAutofillPopulator::GetCity()
{
    // ==================before translated======================
    // return mCity;

    return mCity;
}

String PersonalAutofillPopulator::GetRegion()
{
    // ==================before translated======================
    // return mRegion;

    return mRegion;
}

String PersonalAutofillPopulator::GetPostalCode()
{
    // ==================before translated======================
    // return mPostalCode;

    return mPostalCode;
}

String PersonalAutofillPopulator::GetCountry()
{
    // ==================before translated======================
    // return mCountry;

    return mCountry;
}

AutoPtr< ArrayOf<String> > PersonalAutofillPopulator::GetPhoneNumbers()
{
    // ==================before translated======================
    // return mPhoneNumbers;

    return mPhoneNumbers;
}

Boolean PersonalAutofillPopulator::GetHasPermissions()
{
    // ==================before translated======================
    // return mHasPermissions;

    return mHasPermissions;
}

} // namespace Components
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

