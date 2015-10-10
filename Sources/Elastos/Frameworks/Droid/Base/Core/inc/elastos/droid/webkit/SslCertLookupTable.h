
#ifndef __ELASTOS_DROID_WEBKIT_SSLCERTLOOKUPTABLE_H__
#define __ELASTOS_DROID_WEBKIT_SSLCERTLOOKUPTABLE_H__

#include "elastos/droid/ext/frameworkext.h"

using Elastos::Droid::Os::IBundle;
using Elastos::Droid::Net::Http::ISslError;

namespace Elastos {
namespace Droid {
namespace Webkit {

/**
 * Stores the user's decision of whether to allow or deny an invalid certificate.
 *
 * This class is not threadsafe. It is used only on the WebCore thread. Also, it
 * is used only by the Chromium HTTP stack.
 */
class SslCertLookupTable : public ElRefBase
{
public:
    static CARAPI_(AutoPtr<SslCertLookupTable>) GetInstance();

    CARAPI_(void) SetIsAllowed(
        /* [in] */ ISslError* sslError);

    // We allow the decision to be re-used if it's for the same host and is for
    // an error of equal or greater severity than this error.
    CARAPI_(Boolean) IsAllowed(
        /* [in] */ ISslError* sslError);

    CARAPI_(void) Clear();

private:
    SslCertLookupTable();

private:
    static AutoPtr<SslCertLookupTable> sTable;
    // We store the most severe error we're willing to allow for each host.
    AutoPtr<IBundle> mTable;
};

} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_WEBKIT_SSLCERTLOOKUPTABLE_H__
