
#ifndef __CACCESSDESCRIPTION_H__
#define __CACCESSDESCRIPTION_H__

#include "_CAccessDescription.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Security {
namespace X509 {

CarClass(CAccessDescription)
{
public:
    CARAPI GetEncoded(
        /* [out, callee] */ ArrayOf<Byte> ** ppEncoded);

    CARAPI ToString(
        /* [out] */ String * pStr);

private:
    // TODO: Add your private member variables here.
};

}
}
}
}
}

#endif // __CACCESSDESCRIPTION_H__
