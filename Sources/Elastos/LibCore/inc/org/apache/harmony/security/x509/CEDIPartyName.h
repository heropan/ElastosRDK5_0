
#ifndef __CEDIPARTYNAME_H__
#define __CEDIPARTYNAME_H__

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

#endif // __CEDIPARTYNAME_H__
