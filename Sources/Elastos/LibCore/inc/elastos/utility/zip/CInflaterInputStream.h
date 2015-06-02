
#ifndef __ELASTOS_UTILITY_CINFLATERINPUTSTREAM_H__
#define __ELASTOS_UTILITY_CINFLATERINPUTSTREAM_H__

#include "_Elastos_Utility_Zip_CInflaterInputStream.h"
#include "InflaterInputStream.h"

namespace Elastos {
namespace Utility {
namespace Zip {

CarClass(CInflaterInputStream)
    , public InflaterInputStream
{
public:
    CAR_OBJECT_DECL()

    CARAPI GetLock(
        /* [out] */ IInterface** lockobj);
};

} // namespace Zip
} // namespace Utility
} // namespace Elastos

#endif //__ELASTOS_UTILITY_CINFLATERINPUTSTREAM_H__
