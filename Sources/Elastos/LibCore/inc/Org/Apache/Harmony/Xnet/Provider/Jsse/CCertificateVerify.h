
#ifndef __CCERTIFICATEVERIFY_H__
#define __CCERTIFICATEVERIFY_H__

#include "_CCertificateVerify.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Xnet {
namespace Provider {
namespace Jsse {

CarClass(CCertificateVerify)
{
public:
    CARAPI GetLength(
        /* [out] */ Int32 * pLength);

    CARAPI constructor(
        /* [in] */ ArrayOf<Byte> * pHash);

    CARAPI constructor(
        /* [in] */ Org::Apache::Harmony::Xnet::Provider::Jsse::IHandshakeIODataStream * pIds,
        /* [in] */ Int32 length);

private:
    // TODO: Add your private member variables here.
};

}
}
}
}
}
}

#endif // __CCERTIFICATEVERIFY_H__
