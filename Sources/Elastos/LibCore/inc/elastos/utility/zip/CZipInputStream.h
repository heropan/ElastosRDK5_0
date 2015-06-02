
#ifndef __ELASTOS_UTILITY_CZIPINPUTSTREAM_H__
#define __ELASTOS_UTILITY_CZIPINPUTSTREAM_H__


#include "_Elastos_Utility_Zip_CZipInputStream.h"
#include "ZipInputStream.h"

namespace Elastos {
namespace Utility {
namespace Zip {

CarClass(CZipInputStream) , public ZipInputStream
{
public:
    CAR_OBJECT_DECL()

    CARAPI GetLock(
        /* [out] */ IInterface** lockobj);
};

} // namespace Zip
} // namespace Utility
} // namespace Elastos

#endif //__ELASTOS_UTILITY_CZIPINPUTSTREAM_H__
