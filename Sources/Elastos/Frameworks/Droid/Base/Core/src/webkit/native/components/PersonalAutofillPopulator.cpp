// wuweizuo automatic build .cpp file from .java file.

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Components {

//=====================================================================
//               PersonalAutofillPopulator::ProfileQuery
//=====================================================================
)
{
}

)
{
}

//=====================================================================
//             PersonalAutofillPopulator::EmailProfileQuery
//=====================================================================
const Int32 PersonalAutofillPopulator::EmailProfileQuery::EMAIL_ADDRESS;

AutoPtr< ArrayOf<String> > PersonalAutofillPopulator::EmailProfileQuery::Projection()
{
    /*
    return new String[] {
        ContactsContract.CommonDataKinds.Email.ADDRESS,
    };
    */
    return AutoPtr< ArrayOf<String> >(this);
}

String PersonalAutofillPopulator::EmailProfileQuery::MimeType()
{
    /*
    return ContactsContract.CommonDataKinds.Email.CONTENT_ITEM_TYPE;
    */
    return String("");
}

//=====================================================================
//             PersonalAutofillPopulator::PhoneProfileQuery
//=====================================================================
const Int32 PersonalAutofillPopulator::PhoneProfileQuery::NUMBER;

AutoPtr< ArrayOf<String> > PersonalAutofillPopulator::PhoneProfileQuery::Projection()
{
    /*
    return new String[] {
        ContactsContract.CommonDataKinds.Phone.NUMBER,
    };
    */
    return AutoPtr< ArrayOf<String> >(this);
}

String PersonalAutofillPopulator::PhoneProfileQuery::MimeType()
{
    /*
    return ContactsContract.CommonDataKinds.Phone.CONTENT_ITEM_TYPE;
    */
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
    /*
    return new String[] {
        ContactsContract.CommonDataKinds.StructuredPostal.STREET,
            ContactsContract.CommonDataKinds.StructuredPostal.POBOX,
            ContactsContract.CommonDataKinds.StructuredPostal.NEIGHBORHOOD,
            ContactsContract.CommonDataKinds.StructuredPostal.CITY,
            ContactsContract.CommonDataKinds.StructuredPostal.REGION,
            ContactsContract.CommonDataKinds.StructuredPostal.POSTCODE,
            ContactsContract.CommonDataKinds.StructuredPostal.COUNTRY,
    };
    */
    return AutoPtr< ArrayOf<String> >(this);
}

String PersonalAutofillPopulator::AddressProfileQuery::MimeType()
{
    /*
    return ContactsContract.CommonDataKinds.StructuredPostal.CONTENT_ITEM_TYPE;
    */
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
    /*
    return new String[] {
        ContactsContract.CommonDataKinds.StructuredName.GIVEN_NAME,
            ContactsContract.CommonDataKinds.StructuredName.MIDDLE_NAME,
            ContactsContract.CommonDataKinds.StructuredName.FAMILY_NAME,
            ContactsContract.CommonDataKinds.StructuredName.SUFFIX
    };
    */
    return AutoPtr< ArrayOf<String> >(this);
}

String PersonalAutofillPopulator::NameProfileQuery::MimeType()
{
    /*
    return ContactsContract.CommonDataKinds.StructuredName.CONTENT_ITEM_TYPE;
    */
    return String("");
}

//=====================================================================
//                      PersonalAutofillPopulator
//=====================================================================
PersonalAutofillPopulator::PersonalAutofillPopulator(
    /* [in] */ IContext* context)
{
    /*
    mHasPermissions = hasPermissions(context);
    if (mHasPermissions) {
        ContentResolver contentResolver = context.getContentResolver();
        populateName(contentResolver);
        populateEmail(contentResolver);
        populateAddress(contentResolver);
        populatePhone(contentResolver);
    }
    */
}

AutoPtr<PersonalAutofillPopulator> PersonalAutofillPopulator::Create(
    /* [in] */ IContext* context)
{
    /*
    return new PersonalAutofillPopulator(context);
    */
    return AutoPtr<PersonalAutofillPopulator>(this);
}

AutoPtr<ICursor> PersonalAutofillPopulator::CursorFromProfileQuery(
    /* [in] */ ProfileQuery* query,
    /* [in] */ IContentResolver* contentResolver)
{
    /*
    String sortDescriptor = ContactsContract.Contacts.Data.IS_PRIMARY + " DESC";
    return contentResolver.query(
            query.profileDataUri,
            query.projection(),
            ContactsContract.Contacts.Data.MIMETYPE + " = ?",
            new String[]{query.mimeType()},
            sortDescriptor
            );
    */
    return AutoPtr<ICursor>(this);
}

Boolean PersonalAutofillPopulator::HasPermissions(
    /* [in] */ IContext* context)
{
    /*
    String [] permissions = {
        "android.permission.READ_CONTACTS",
        "android.permission.READ_PROFILE"
    };
    for (String permission : permissions) {
        int res = context.checkCallingOrSelfPermission(permission);
        if (res != PackageManager.PERMISSION_GRANTED) return false;
    }
    return true;
    */
    return FALSE;
}

ECode PersonalAutofillPopulator::PopulateName(
    /* [in] */ IContentResolver* contentResolver)
{
    VALIDATE_NOT_NULL(contentResolver);
    /*
    NameProfileQuery nameProfileQuery = new NameProfileQuery();
    Cursor nameCursor = cursorFromProfileQuery(nameProfileQuery, contentResolver);
    if (nameCursor.moveToNext()) {
        mGivenName = nameCursor.getString(nameProfileQuery.GIVEN_NAME);
        mMiddleName = nameCursor.getString(nameProfileQuery.MIDDLE_NAME);
        mFamilyName = nameCursor.getString(nameProfileQuery.FAMILY_NAME);
        mSuffix = nameCursor.getString(nameProfileQuery.SUFFIX);
    }
    nameCursor.close();
    */
    return NOERROR;
}

ECode PersonalAutofillPopulator::PopulateEmail(
    /* [in] */ IContentResolver* contentResolver)
{
    VALIDATE_NOT_NULL(contentResolver);
    /*
    EmailProfileQuery emailProfileQuery = new EmailProfileQuery();
    Cursor emailCursor = cursorFromProfileQuery(emailProfileQuery, contentResolver);
    mEmailAddresses = new String[emailCursor.getCount()];
    for (int i = 0; emailCursor.moveToNext(); i++) {
        mEmailAddresses[i] = emailCursor.getString(emailProfileQuery.EMAIL_ADDRESS);
    }
    emailCursor.close();
    */
    return NOERROR;
}

ECode PersonalAutofillPopulator::PopulateAddress(
    /* [in] */ IContentResolver* contentResolver)
{
    VALIDATE_NOT_NULL(contentResolver);
    /*
    AddressProfileQuery addressProfileQuery = new AddressProfileQuery();
    Cursor addressCursor = cursorFromProfileQuery(addressProfileQuery, contentResolver);
    if(addressCursor.moveToNext()) {
        mPobox = addressCursor.getString(addressProfileQuery.POBOX);
        mStreet = addressCursor.getString(addressProfileQuery.STREET);
        mNeighborhood = addressCursor.getString(addressProfileQuery.NEIGHBORHOOD);
        mCity = addressCursor.getString(addressProfileQuery.CITY);
        mRegion = addressCursor.getString(addressProfileQuery.REGION);
        mPostalCode = addressCursor.getString(addressProfileQuery.POSTALCODE);
        mCountry = addressCursor.getString(addressProfileQuery.COUNTRY);
    }
    addressCursor.close();
    */
    return NOERROR;
}

ECode PersonalAutofillPopulator::PopulatePhone(
    /* [in] */ IContentResolver* contentResolver)
{
    VALIDATE_NOT_NULL(contentResolver);
    /*
    PhoneProfileQuery phoneProfileQuery = new PhoneProfileQuery();
    Cursor phoneCursor = cursorFromProfileQuery(phoneProfileQuery, contentResolver);
    mPhoneNumbers = new String[phoneCursor.getCount()];
    for (int i = 0; phoneCursor.moveToNext(); i++) {
        mPhoneNumbers[i] = phoneCursor.getString(phoneProfileQuery.NUMBER);
    }
    phoneCursor.close();
    */
    return NOERROR;
}

String PersonalAutofillPopulator::GetFirstName()
{
    /*
    return mGivenName;
    */
    return String("");
}

String PersonalAutofillPopulator::GetLastName()
{
    /*
    return mFamilyName;
    */
    return String("");
}

String PersonalAutofillPopulator::GetMiddleName()
{
    /*
    return mMiddleName;
    */
    return String("");
}

String PersonalAutofillPopulator::GetSuffix()
{
    /*
    return mSuffix;
    */
    return String("");
}

AutoPtr< ArrayOf<String> > PersonalAutofillPopulator::GetEmailAddresses()
{
    /*
    return mEmailAddresses;
    */
    return AutoPtr< ArrayOf<String> >(this);
}

String PersonalAutofillPopulator::GetStreet()
{
    /*
    return mStreet;
    */
    return String("");
}

String PersonalAutofillPopulator::GetPobox()
{
    /*
    return mPobox;
    */
    return String("");
}

String PersonalAutofillPopulator::GetNeighborhood()
{
    /*
    return mNeighborhood;
    */
    return String("");
}

String PersonalAutofillPopulator::GetCity()
{
    /*
    return mCity;
    */
    return String("");
}

String PersonalAutofillPopulator::GetRegion()
{
    /*
    return mRegion;
    */
    return String("");
}

String PersonalAutofillPopulator::GetPostalCode()
{
    /*
    return mPostalCode;
    */
    return String("");
}

String PersonalAutofillPopulator::GetCountry()
{
    /*
    return mCountry;
    */
    return String("");
}

AutoPtr< ArrayOf<String> > PersonalAutofillPopulator::GetPhoneNumbers()
{
    /*
    return mPhoneNumbers;
    */
    return AutoPtr< ArrayOf<String> >(this);
}

Boolean PersonalAutofillPopulator::GetHasPermissions()
{
    /*
    return mHasPermissions;
    */
    return FALSE;
}

} // namespace Components
} // namespace Webkit
} // namespace Droid
} // namespace Elastos


