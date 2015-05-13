
#ifndef __CPROTOCOLVERSION_H__
#define __CPROTOCOLVERSION_H__

#include "_CProtocolVersion.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Xnet {
namespace Provider {
namespace Jsse {

CarClass(CProtocolVersion)
{
public:
    CARAPI GetName(
        /* [out] */ String * pName);

    CARAPI GetVersion(
        /* [out, callee] */ ArrayOf<Byte> ** ppVer);

private:
    // TODO: Add your private member variables here.
};

}
}
}
}
}
}

#endif // __CPROTOCOLVERSION_H__
