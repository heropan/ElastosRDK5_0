
#ifndef __ELASTOS_DROID_PROVIDER_CONTACTSCONTRACTCONTACTS_H__
#define __ELASTOS_DROID_PROVIDER_CONTACTSCONTRACTCONTACTS_H__

#include "ext/frameworkext.h"

using Elastos::IO::IInputStream;
using Elastos::Droid::Net::IUri;
using Elastos::Droid::Content::IContentResolver;

namespace Elastos {
namespace Droid {
namespace Provider {

class ContactsContractContacts
{
public:
    /**
     * The content:// style URI for this table
     */
    static CARAPI GetCONTENT_URI(
        /* [out] */ IUri** uri);

    /**
     * A content:// style URI for this table that should be used to create
     * shortcuts or otherwise create long-term links to contacts. This URI
     * should always be followed by a "/" and the contact's {@link #LOOKUP_KEY}.
     * It can optionally also have a "/" and last known contact ID appended after
     * that. This "complete" format is an important optimization and is highly recommended.
     * <p>
     * As long as the contact's row ID remains the same, this URI is
     * equivalent to {@link #CONTENT_URI}. If the contact's row ID changes
     * as a result of a sync or aggregation, this URI will look up the
     * contact using indirect information (sync IDs or constituent raw
     * contacts).
     * <p>
     * Lookup key should be appended unencoded - it is stored in the encoded
     * form, ready for use in a URI.
     */
    static CARAPI GetCONTENT_LOOKUP_URI(
        /* [out] */ IUri** uri);

    /**
     * Base {@link Uri} for referencing a single {@link Contacts} entry,
     * created by appending {@link #LOOKUP_KEY} using
     * {@link Uri#withAppendedPath(Uri, String)}. Provides
     * {@link OpenableColumns} columns when queried, or returns the
     * referenced contact formatted as a vCard when opened through
     * {@link ContentResolver#openAssetFileDescriptor(Uri, String)}.
     */
    static CARAPI GetCONTENT_VCARD_URI(
        /* [out] */ IUri** uri);

    /**
     * Base {@link Uri} for referencing multiple {@link Contacts} entry,
     * created by appending {@link #LOOKUP_KEY} using
     * {@link Uri#withAppendedPath(Uri, String)}. The lookup keys have to be
     * encoded and joined with the colon (":") separator. The resulting string
     * has to be encoded again. Provides
     * {@link OpenableColumns} columns when queried, or returns the
     * referenced contact formatted as a vCard when opened through
     * {@link ContentResolver#openAssetFileDescriptor(Uri, String)}.
     *
     * This is private API because we do not have a well-defined way to
     * specify several entities yet. The format of this Uri might change in the future
     * or the Uri might be completely removed.
     *
     * @hide
     */
    static CARAPI GetCONTENT_MULTIVCARD_URI(
        /* [out] */ IUri** uri);

    /**
     * The content:// style URI used for "type-to-filter" functionality on the
     * {@link #CONTENT_URI} URI. The filter string will be used to match
     * various parts of the contact name. The filter argument should be passed
     * as an additional path segment after this URI.
     */
    static CARAPI GetCONTENT_FILTER_URI(
        /* [out] */ IUri** uri);

    /**
     * The content:// style URI for this table joined with useful data from
     * {@link ContactsContract.Data}, filtered to include only starred contacts
     * and the most frequently contacted contacts.
     */
    static CARAPI GetCONTENT_STREQUENT_URI(
        /* [out] */ IUri** uri);

    /**
     * The content:// style URI for showing frequently contacted person listing.
     * @hide
     */
    static CARAPI GetCONTENT_FREQUENT_URI(
        /* [out] */ IUri** uri);

    /**
     * The content:// style URI used for "type-to-filter" functionality on the
     * {@link #CONTENT_STREQUENT_URI} URI. The filter string will be used to match
     * various parts of the contact name. The filter argument should be passed
     * as an additional path segment after this URI.
     */
    static CARAPI GetCONTENT_STREQUENT_FILTER_URI(
        /* [out] */ IUri** uri);

    static CARAPI GetCONTENT_GROUP_URI(
        /* [out] */ IUri** uri);

    /**
     * Builds a {@link #CONTENT_LOOKUP_URI} style {@link Uri} describing the
     * requested {@link Contacts} entry.
     *
     * @param contactUri A {@link #CONTENT_URI} row, or an existing
     *            {@link #CONTENT_LOOKUP_URI} to attempt refreshing.
     */
    static CARAPI GetLookupUri(
        /* [in] */ IContentResolver* resolver,
        /* [in] */ IUri* contactUri,
        /* [out] */ IUri** uri);

    /**
     * Build a {@link #CONTENT_LOOKUP_URI} lookup {@link Uri} using the
     * given {@link ContactsContract.Contacts#_ID} and {@link #LOOKUP_KEY}.
     */
    static CARAPI GetLookupUri(
        /* [in] */ Int64 contactId,
        /* [in] */ const String& lookupKey,
        /* [out] */ IUri** uri);

    /**
     * Computes a content URI (see {@link #CONTENT_URI}) given a lookup URI.
     * <p>
     * Returns null if the contact cannot be found.
     */
    static CARAPI LookupContact(
        /* [in] */ IContentResolver* resolver,
        /* [in] */ IUri* lookupUri,
        /* [out] */ IUri** uri);

    /**
     * Mark a contact as having been contacted. Updates two fields:
     * {@link #TIMES_CONTACTED} and {@link #LAST_TIME_CONTACTED}. The
     * TIMES_CONTACTED field is incremented by 1 and the LAST_TIME_CONTACTED
     * field is populated with the current system time.
     *
     * @param resolver the ContentResolver to use
     * @param contactId the person who was contacted
     *
     * @deprecated The class DataUsageStatUpdater of the Android support library should
     *     be used instead.
     */
    // @Deprecated
    static CARAPI MarkAsContacted(
        /* [in] */ IContentResolver* resolver,
        /* [in] */ Int64 contactId);

    /**
     * Opens an InputStream for the contacts's photo and returns the
     * photo as a byte stream.
     * @param cr The content resolver to use for querying
     * @param contactUri the contact whose photo should be used. This can be used with
     * either a {@link #CONTENT_URI} or a {@link #CONTENT_LOOKUP_URI} URI.
     * @param preferHighres If this is true and the contact has a higher resolution photo
     * available, it is returned. If false, this function always tries to get the thumbnail
     * @return an InputStream of the photo, or null if no photo is present
     */
    static CARAPI OpenContactPhotoInputStream(
        /* [in] */ IContentResolver* cr,
        /* [in] */ IUri* contactUri,
        /* [in] */ Boolean preferHighres,
        /* [out] */ IInputStream** stream);

    /**
     * Opens an InputStream for the contacts's thumbnail photo and returns the
     * photo as a byte stream.
     * @param cr The content resolver to use for querying
     * @param contactUri the contact whose photo should be used. This can be used with
     * either a {@link #CONTENT_URI} or a {@link #CONTENT_LOOKUP_URI} URI.
     * @return an InputStream of the photo, or null if no photo is present
     * @see #openContactPhotoInputStream(ContentResolver, Uri, boolean), if instead
     * of the thumbnail the high-res picture is preferred
     */
    static CARAPI OpenContactPhotoInputStream(
        /* [in] */ IContentResolver* cr,
        /* [in] */ IUri* contactUri,
        /* [out] */ IInputStream** stream);
};

} //Provider
} //Droid
} //Elastos

#endif //__ELASTOS_DROID_PROVIDER_CONTACTSCONTRACTCONTACTS_H__
