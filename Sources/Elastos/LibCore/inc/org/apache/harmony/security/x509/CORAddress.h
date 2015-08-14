
#ifndef __ORG_APACHE_HARMONY_SECURITY_X509_CORADDRESS_H__
#define __ORG_APACHE_HARMONY_SECURITY_X509_CORADDRESS_H__

#include "_CORAddress.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Security {
namespace X509 {

CarClass(CORAddress)
{
public:
    CARAPI GetEncoded(
        /* [out, callee] */ ArrayOf<Byte> ** ppEncoded);

private:
    // TODO: Add your private member variables here.
};

}
}
}
}
}

#endif // __ORG_APACHE_HARMONY_SECURITY_X509_CORADDRESS_H__
