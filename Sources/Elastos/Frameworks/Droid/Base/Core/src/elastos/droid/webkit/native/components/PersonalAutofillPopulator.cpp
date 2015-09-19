// wuweizuo automatic build .cpp file from .java file.

#include "PersonalAutofillPopulator.h"

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Components {

//=====================================================================
//               PersonalAutofillPopulator::ProfileQuery
//=====================================================================
AutoPtr<IUri> PersonalAutofillPopulator::ProfileQuery::profileDataUri = PersonalAutofillPopulator::ProfileQuery::MiddleInitProfiledatauri();

AutoPtr<IUri> PersonalAutofillPopulator::ProfileQuery::MiddleInitProfiledatauri()
{
    // ==================before translated======================
    // ->WWZ_SIGN: FUNC_CALL_START {
    // ContactsContract.Profile.CONTENT_URI,
    //                  ContactsContract.Contacts.Data.CONTENT_DIRECTORY
    // ->WWZ_SIGN: FUNC_CALL_END }
    assert(0);
    AutoPtr<IUri> empty;
    return empty;
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
    assert(0);
    AutoPtr< ArrayOf<String> > empty;
    return empty;
}

String PersonalAutofillPopulator::EmailProfileQuery::MimeType()
{
    // ==================before translated======================
    // return ContactsContract.CommonDataKinds.Email.CONTENT_ITEM_TYPE;
    assert(0);
    return String("");
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
    assert(0);
    AutoPtr< ArrayOf<String> > empty;
    return empty;
}

String PersonalAutofillPopulator::PhoneProfileQuery::MimeType()
{
    // ==================before translated======================
    // return ContactsContract.CommonDataKinds.Phone.CONTENT_ITEM_TYPE;
    assert(0);
    return String("");
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
    assert(0);
    AutoPtr< ArrayOf<String> > empty;
    return empty;
}

String PersonalAutofillPopulator::AddressProfileQuery::MimeType()
{
    // ==================before translated======================
    // return ContactsContract.CommonDataKinds.StructuredPostal.CONTENT_ITEM_TYPE;
    assert(0);
    return String("");
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
    assert(0);
    AutoPtr< ArrayOf<String> > empty;
    return empty;
}

String PersonalAutofillPopulator::NameProfileQuery::MimeType()
{
    // ==================before translated======================
    // return ContactsContract.CommonDataKinds.StructuredName.CONTENT_ITEM_TYPE;
    assert(0);
    return String("");
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
}

AutoPtr<PersonalAutofillPopulator> PersonalAutofillPopulator::Create(
    /* [in] */ IContext* context)
{
    // ==================before translated======================
    // return new PersonalAutofillPopulator(context);
    assert(0);
    AutoPtr<PersonalAutofillPopulator> empty;
    return empty;
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
    assert(0);
    AutoPtr<ICursor> empty;
    return empty;
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
    assert(0);
    return FALSE;
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
    assert(0);
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
    assert(0);
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
    assert(0);
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
    assert(0);
    return NOERROR;
}

String PersonalAutofillPopulator::GetFirstName()
{
    // ==================before translated======================
    // return mGivenName;
    assert(0);
    return String("");
}

String PersonalAutofillPopulator::GetLastName()
{
    // ==================before translated======================
    // return mFamilyName;
    assert(0);
    return String("");
}

String PersonalAutofillPopulator::GetMiddleName()
{
    // ==================before translated======================
    // return mMiddleName;
    assert(0);
    return String("");
}

String PersonalAutofillPopulator::GetSuffix()
{
    // ==================before translated======================
    // return mSuffix;
    assert(0);
    return String("");
}

AutoPtr< ArrayOf<String> > PersonalAutofillPopulator::GetEmailAddresses()
{
    // ==================before translated======================
    // return mEmailAddresses;
    assert(0);
    AutoPtr< ArrayOf<String> > empty;
    return empty;
}

String PersonalAutofillPopulator::GetStreet()
{
    // ==================before translated======================
    // return mStreet;
    assert(0);
    return String("");
}

String PersonalAutofillPopulator::GetPobox()
{
    // ==================before translated======================
    // return mPobox;
    assert(0);
    return String("");
}

String PersonalAutofillPopulator::GetNeighborhood()
{
    // ==================before translated======================
    // return mNeighborhood;
    assert(0);
    return String("");
}

String PersonalAutofillPopulator::GetCity()
{
    // ==================before translated======================
    // return mCity;
    assert(0);
    return String("");
}

String PersonalAutofillPopulator::GetRegion()
{
    // ==================before translated======================
    // return mRegion;
    assert(0);
    return String("");
}

String PersonalAutofillPopulator::GetPostalCode()
{
    // ==================before translated======================
    // return mPostalCode;
    assert(0);
    return String("");
}

String PersonalAutofillPopulator::GetCountry()
{
    // ==================before translated======================
    // return mCountry;
    assert(0);
    return String("");
}

AutoPtr< ArrayOf<String> > PersonalAutofillPopulator::GetPhoneNumbers()
{
    // ==================before translated======================
    // return mPhoneNumbers;
    assert(0);
    AutoPtr< ArrayOf<String> > empty;
    return empty;
}

Boolean PersonalAutofillPopulator::GetHasPermissions()
{
    // ==================before translated======================
    // return mHasPermissions;
    assert(0);
    return FALSE;
}

} // namespace Components
} // namespace Webkit
} // namespace Droid
} // namespace Elastos


