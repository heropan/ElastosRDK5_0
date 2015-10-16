
#include "elastos/droid/provider/CContactsContract.h"

namespace Elastos {
namespace Droid {
namespace Provider {

ECode CContactsContract::GetAUTHORITY_URI(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    return ContactsContract::GetAUTHORITY_URI(uri);
}

ECode CContactsContract::IsProfileId(
    /* [in] */ Int64 id,
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);

    return ContactsContract::IsProfileId(id, res);
}

ECode CContactsContract::Snippetize(
    /* [in] */ const String& content,
    /* [in] */ const String& displayName,
    /* [in] */ const String& query,
    /* [in] */ Char32 snippetStartMatch,
    /* [in] */ Char32 snippetEndMatch,
    /* [in] */ const String& snippetEllipsis,
    /* [in] */ Int32 snippetMaxTokens,
    /* [out] */ String* snippet)
{
    VALIDATE_NOT_NULL(snippet);

    return ContactsContract::Snippetize(content, displayName, query, snippetStartMatch,
        snippetEndMatch, snippetEllipsis, snippetMaxTokens, snippet);
}

} //Provider
} //Droid
} //Elastos