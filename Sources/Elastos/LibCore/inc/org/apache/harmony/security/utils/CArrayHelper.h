
#ifndef __CARRAYHELPER_H__
#define __CARRAYHELPER_H__

#include "_CArrayHelper.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Security {
namespace Utils {

CarClass(CArrayHelper)
{
public:
    CARAPI GetBytesAsString(
        /* [in] */ ArrayOf<Byte> * pData,
        /* [out] */ String * pStr);

    CARAPI ToString(
        /* [in] */ ArrayOf<Byte> * pArray,
        /* [in] */ const String& prefix,
        /* [out] */ String * pStr);

private:
    // TODO: Add your private member variables here.
};

}
}
}
}
}

#endif // __CARRAYHELPER_H__
