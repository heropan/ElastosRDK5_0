
#ifndef __ORG_APACHE_HARMONY_SECURITY_X509_CBASICCONSTRAINTS_H__
#define __ORG_APACHE_HARMONY_SECURITY_X509_CBASICCONSTRAINTS_H__

#include "_CBasicConstraints.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Security {
namespace X509 {

CarClass(CBasicConstraints)
{
public:
    CARAPI GetEncoded(
        /* [out, callee] */ ArrayOf<Byte> ** ppEncode);

    CARAPI DumpValue(
        /* [in] */ Elastos::Core::IStringBuilder * pSb,
        /* [in] */ const String& prefix);

    CARAPI DumpValueEx(
        /* [in] */ Elastos::Core::IStringBuilder * pSb);

    CARAPI GetPathLenConstraint(
        /* [out] */ Int32 * pPathLenConstraint);

    CARAPI constructor(
        /* [in] */ ArrayOf<Byte> * pEncoding);

private:
    // TODO: Add your private member variables here.
};

}
}
}
}
}

#endif // __ORG_APACHE_HARMONY_SECURITY_X509_CBASICCONSTRAINTS_H__
