
#ifndef __ELASTOS_DROID_PROVIDER_CCONTACTSCONTRACT_H__
#define __ELASTOS_DROID_PROVIDER_CCONTACTSCONTRACT_H__

#include "_Elastos_Droid_Provider_CContactsContract.h"
#include "ContactsContract.h"

using Elastos::Droid::Net::IUri;

namespace Elastos {
namespace Droid {
namespace Provider {

CarClass(CContactsContract)
{
public:
    /** A content:// style uri to the authority for the contacts provider */
    CARAPI GetAUTHORITY_URI(
        /* [out] */ IUri** uri);

    /**
     * This method can be used to identify whether the given ID is associated with profile
     * data.  It does not necessarily indicate that the ID is tied to valid data, merely
     * that accessing data using this ID will result in profile access checks and will only
     * return data from the profile.
     *
     * @param id The ID to check.
     * @return Whether the ID is associated with profile data.
     */
    CARAPI IsProfileId(
        /* [in] */ Int64 id,
        /* [out] */ Boolean* res);

    /**
     * Creates a snippet out of the given content that matches the given query.
     * @param content - The content to use to compute the snippet.
     * @param displayName - Display name for the contact - if this already contains the search
     *        content, no snippet should be shown.
     * @param query - String to search for in the content.
     * @param snippetStartMatch - Marks the start of the matching string in the snippet.
     * @param snippetEndMatch - Marks the end of the matching string in the snippet.
     * @param snippetEllipsis - Ellipsis string appended to the end of the snippet (if too long).
     * @param snippetMaxTokens - Maximum number of words from the snippet that will be displayed.
     * @return The computed snippet, or null if the snippet could not be computed or should not be
     *         shown.
     *
     *  @hide
     */
    CARAPI Snippetize(
        /* [in] */ const String& content,
        /* [in] */ const String& displayName,
        /* [in] */ const String& query,
        /* [in] */ Char32 snippetStartMatch,
        /* [in] */ Char32 snippetEndMatch,
        /* [in] */ const String& snippetEllipsis,
        /* [in] */ Int32 snippetMaxTokens,
        /* [out] */ String* snippet);
};

} //Provider
} //Droid
} //Elastos

#endif //__ELASTOS_DROID_PROVIDER_CCONTACTSCONTRACT_H__
