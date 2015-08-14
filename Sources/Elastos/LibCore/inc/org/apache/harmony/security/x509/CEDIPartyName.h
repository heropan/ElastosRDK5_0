
#ifndef __ORG_APACHE_HARMONY_SECURITY_X509_CEDIPARTYNAME_H__
#define __ORG_APACHE_HARMONY_SECURITY_X509_CEDIPARTYNAME_H__

#include "_CEDIPartyName.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Security {
namespace X509 {

CarClass(CEDIPartyName)
{
public:
    CARAPI GetEncoded(
        /* [out, callee] */ ArrayOf<Byte> ** ppEncode);

private:
    // TODO: Add your private member variables here.
};

}
}
}
}
}

#endif // __ORG_APACHE_HARMONY_SECURITY_X509_CEDIPARTYNAME_H__
