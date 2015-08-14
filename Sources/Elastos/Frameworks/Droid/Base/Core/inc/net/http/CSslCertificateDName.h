
#ifndef __ELASTOS_DROID_NET_HTTP_CSSLCERTIFICATEDNAME_H__
#define __ELASTOS_DROID_NET_HTTP_CSSLCERTIFICATEDNAME_H__

#include "_CSslCertificateDName.h"

namespace Elastos {
namespace Droid {
namespace Net {
namespace Http {

/**
 * A distinguished name helper class: a 3-tuple of:
 * - common name (CN),
 * - organization (O),
 * - organizational unit (OU)
 */
CarClass(CSslCertificateDName)
{
public:
    CARAPI constructor(
        /* [in] */ const String& dName);

    /**
     * @return The distinguished name (normally includes CN, O, and OU names)
     */
    CARAPI GetDName(
        /* [out] */ String* dName);

    /**
     * @return The Common-name (CN) component of this name
     */
    CARAPI GetCName(
        /* [out] */ String* cName);

    /**
     * @return The Organization (O) component of this name
     */
    CARAPI GetOName(
        /* [out] */ String* oName);

    /**
     * @return The Organizational Unit (OU) component of this name
     */
    CARAPI GetUName(
        /* [out] */ String* uName);

private:
    /**
     * Distinguished name (normally includes CN, O, and OU names)
     */
    String mDName;

    /**
     * Common-name (CN) component of the name
     */
    String mCName;

    /**
     * Organization (O) component of the name
     */
    String mOName;

    /**
     * Organizational Unit (OU) component of the name
     */
    String mUName;
};

}
}
}
}

#endif // __ELASTOS_DROID_NET_HTTP_CSSLCERTIFICATEDNAME_H__
