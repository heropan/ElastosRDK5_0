
#ifndef __CSERVERHELLODONE_H__
#define __CSERVERHELLODONE_H__

#include "_CServerHelloDone.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Xnet {
namespace Provider {
namespace Jsse {

CarClass(CServerHelloDone)
{
public:
    CARAPI GetLength(
        /* [out] */ Int32 * pLength);

    CARAPI constructor();

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

#endif // __CSERVERHELLODONE_H__
