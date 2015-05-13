
#ifndef __CRANDOMBITSSUPPLIERHELPER_H__
#define __CRANDOMBITSSUPPLIERHELPER_H__

#include "_CRandomBitsSupplierHelper.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Security {
namespace Provider {
namespace Crypto {

CarClass(CRandomBitsSupplierHelper)
{
public:
    CARAPI IsServiceAvailable(
        /* [out] */ Boolean * pAvailable);

    CARAPI GetRandomBits(
        /* [in] */ Int32 numBytes,
        /* [out, callee] */ ArrayOf<Byte> ** ppRandomBits);

private:
    // TODO: Add your private member variables here.
};

}
}
}
}
}
}

#endif // __CRANDOMBITSSUPPLIERHELPER_H__
