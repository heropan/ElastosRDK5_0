// wuweizuo automatic build .h file from .java file.
// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
// Populates data fields from Android contacts profile API (i.e. "me" contact).

#ifndef _ELASTOS_DROID_WEBKIT_COMPONENTS_PERSONALAUTOFILLPOPULATOR_H_
#define _ELASTOS_DROID_WEBKIT_COMPONENTS_PERSONALAUTOFILLPOPULATOR_H_

#include "elatypes.h"
#include "elautoptr.h"
#include "ext/frameworkext.h"
#include "content/ContentResolver.h"
#include "content/Context.h"
#include "content/pm/PackageManager.h"
// #include "database/Cursor.h"
#include "net/Uri.h"
#include "provider/ContactsContract.h"
// #include "webkit/native/base/CalledByNative.h"
// #include "webkit/native/base/JNINamespace.h"

// package org.chromium.components.browser.autofill;
// import android.content.ContentResolver;
// import android.content.Context;
// import android.content.pm.PackageManager;
// import android.database.Cursor;
// import android.net.Uri;
// import android.provider.ContactsContract;
// import org.chromium.base.CalledByNative;
// import org.chromium.base.JNINamespace;

using Elastos::Droid::Content::IContentResolver;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::Pm::IPackageManager;
using Elastos::Droid::Database::ICursor;
using Elastos::Droid::Net::IUri;
using Elastos::Droid::Provider::IContactsContract;
// using Elastos::Droid::Webkit::Base::CalledByNative;
// using Elastos::Droid::Webkit::Base::JNINamespace;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Components {

/**
  * Loads user profile information stored under the "Me" contact.
  * Requires permissions: READ_CONTACTS and READ_PROFILE.
  */
// @JNINamespace("autofill")
class PersonalAutofillPopulator : public Object
{
private:
    /**
      * SQL query definitions for obtaining specific profile information.
      */
    class ProfileQuery : public Object
    {
    public:
        virtual CARAPI_(AutoPtr< ArrayOf<String> >) Projection() = 0;

        virtual CARAPI_(String) MimeType() = 0;

    public:
        AutoPtr<IUri> profileDataUri;
    };

    class EmailProfileQuery
        : public Object
        , public ProfileQuery
    {
    public:
        // @Override
        CARAPI_(AutoPtr< ArrayOf<String> >) Projection();

        // @Override
        CARAPI_(String) MimeType();

    private:
        static const Int32 EMAIL_ADDRESS = 0;
    };

    class PhoneProfileQuery
        : public Object
        , public ProfileQuery
    {
    public:
        // @Override
        CARAPI_(AutoPtr< ArrayOf<String> >) Projection();

        // @Override
        CARAPI_(String) MimeType();

    private:
        static const Int32 NUMBER = 0;
    };

    class AddressProfileQuery
        : public Object
        , public ProfileQuery
    {
    public:
        // @Override
        CARAPI_(AutoPtr< ArrayOf<String> >) Projection();

        // @Override
        CARAPI_(String) MimeType();

    private:
        static const Int32 STREET = 0;
        static const Int32 POBOX = 1;
        static const Int32 NEIGHBORHOOD = 2;
        static const Int32 CITY = 3;
        static const Int32 REGION = 4;
        static const Int32 POSTALCODE = 5;
        static const Int32 COUNTRY = 6;
    };

    class NameProfileQuery
        : public Object
        , public ProfileQuery
    {
    public:
        // @Override
        CARAPI_(AutoPtr< ArrayOf<String> >) Projection();

        // @Override
        CARAPI_(String) MimeType();

    private:
        static const Int32 GIVEN_NAME = 0;
        static const Int32 MIDDLE_NAME = 1;
        static const Int32 FAMILY_NAME = 2;
        static const Int32 SUFFIX = 3;
    };

public:
    /**
      * Constructor
      * @param context a valid android context reference
      */
    PersonalAutofillPopulator(
        /* [in] */ IContext* context);

    /**
      * Static factory method for instance creation.
      * @param context valid Android context.
      * @return PersonalAutofillPopulator new instance of PersonalAutofillPopulator.
      */
    // @CalledByNative
    static CARAPI_(AutoPtr<PersonalAutofillPopulator>) Create(
        /* [in] */ IContext* context);

private:
    /**
      * Takes a query object, transforms into actual query and returns cursor.
      * Primary contact values will be first.
      */
    CARAPI_(AutoPtr<ICursor>) CursorFromProfileQuery(
        /* [in] */ ProfileQuery* query,
        /* [in] */ IContentResolver* contentResolver);

    // Check if the user has granted permissions.
    CARAPI_(Boolean) HasPermissions(
        /* [in] */ IContext* context);

    // Populating data fields.
    CARAPI PopulateName(
        /* [in] */ IContentResolver* contentResolver);

    CARAPI PopulateEmail(
        /* [in] */ IContentResolver* contentResolver);

    CARAPI PopulateAddress(
        /* [in] */ IContentResolver* contentResolver);

    CARAPI PopulatePhone(
        /* [in] */ IContentResolver* contentResolver);

    // @CalledByNative
    CARAPI_(String) GetFirstName();

    // @CalledByNative
    CARAPI_(String) GetLastName();

    // @CalledByNative
    CARAPI_(String) GetMiddleName();

    // @CalledByNative
    CARAPI_(String) GetSuffix();

    // @CalledByNative
    CARAPI_(AutoPtr< ArrayOf<String> >) GetEmailAddresses();

    // @CalledByNative
    CARAPI_(String) GetStreet();

    // @CalledByNative
    CARAPI_(String) GetPobox();

    // @CalledByNative
    CARAPI_(String) GetNeighborhood();

    // @CalledByNative
    CARAPI_(String) GetCity();

    // @CalledByNative
    CARAPI_(String) GetRegion();

    // @CalledByNative
    CARAPI_(String) GetPostalCode();

    // @CalledByNative
    CARAPI_(String) GetCountry();

    // @CalledByNative
    CARAPI_(AutoPtr< ArrayOf<String> >) GetPhoneNumbers();

    // @CalledByNative
    CARAPI_(Boolean) GetHasPermissions();

private:
    // Extracted data variables.
    AutoPtr< ArrayOf<String> > mEmailAddresses;
    String mGivenName;
    String mMiddleName;
    String mFamilyName;
    String mSuffix;
    String mPobox;
    String mStreet;
    String mNeighborhood;
    String mCity;
    String mRegion;
    String mCountry;
    String mPostalCode;
    AutoPtr< ArrayOf<String> > mPhoneNumbers;
    Boolean mHasPermissions;
};

} // namespace Components
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif // _ELASTOS_DROID_WEBKIT_COMPONENTS_PERSONALAUTOFILLPOPULATOR_H_

