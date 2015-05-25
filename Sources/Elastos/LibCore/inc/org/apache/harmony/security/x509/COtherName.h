
#ifndef __COTHERNAME_H__
#define __COTHERNAME_H__

#include "_COtherName.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Security {
namespace X509 {

CarClass(COtherName)
{
public:
    CARAPI GetValue(
        /* [out, callee] */ ArrayOf<Byte> ** ppValue);

    CARAPI GetEncoded(
        /* [out, callee] */ ArrayOf<Byte> ** ppEncoded);

    CARAPI constructor(
        /* [in] */ const String& typeID,
        /* [in] */ ArrayOf<Byte> * pValue);

private:
    // TODO: Add your private member variables here.
};

}
}
}
}
}

#endif // __COTHERNAME_H__
