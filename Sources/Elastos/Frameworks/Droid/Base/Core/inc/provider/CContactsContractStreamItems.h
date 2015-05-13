
#ifndef __CCONTACTSCONTRACTSTREAMITEMS_H__
#define __CCONTACTSCONTRACTSTREAMITEMS_H__

#include "_CContactsContractStreamItems.h"

using Elastos::Droid::Net::IUri;

namespace Elastos {
namespace Droid {
namespace Provider {

CarClass(CContactsContractStreamItems)
{
public:
    CARAPI constructor();

    /**
     * The content:// style URI for this table, which handles social network stream
     * updates for the user's contacts.
     */
    CARAPI GetCONTENTURI(
        /* [out] */ IUri** uri);

    /**
     * <p>
     * A content:// style URI for the photos stored in a sub-table underneath
     * stream items.  This is only used for inserts, and updates - queries and deletes
     * for photos should be performed by appending
     * {@link StreamItems.StreamItemPhotos#CONTENT_DIRECTORY} path to URIs for a
     * specific stream item.
     * </p>
     * <p>
     * When using this URI, the stream item ID for the photo(s) must be identified
     * in the {@link ContentValues} passed in.
     * </p>
     */
    CARAPI GetCONTENTPHOTOURI(
        /* [out] */ IUri** uri);

    /**
     * This URI allows the caller to query for the maximum number of stream items
     * that will be stored under any single raw contact.
     */
    CARAPI GetCONTENTLIMITURI(
        /* [out] */ IUri** uri);
};

} //Provider
} //Droid
} //Elastos

#endif //__CCONTACTSCONTRACTRAWCONTACTS_H__