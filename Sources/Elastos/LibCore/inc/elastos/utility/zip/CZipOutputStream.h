
#ifndef __ELASTOS_UTILITY_CZIPOUTPUTSTREAM_H__
#define __ELASTOS_UTILITY_CZIPOUTPUTSTREAM_H__

#include "_Elastos_Utility_Zip_CZipOutputStream.h"
#include "ZipOutputStream.h"

namespace Elastos {
namespace Utility {
namespace Zip {

CarClass(CZipOutputStream) , public ZipOutputStream
{
public:
    CAR_OBJECT_DECL()

    /**
     * Constructs a new {@code ZipOutputStream} with the specified output
     * stream.
     *
     * @param p1
     *            the {@code OutputStream} to write the data to.
     */
    CARAPI constructor(
        /* [in] */ IOutputStream* os);

    CARAPI GetLock(
        /* [out] */ IInterface** lockobj);
};

} // namespace Zip
} // namespace Utility
} // namespace Elastos

#endif //__ELASTOS_UTILITY_CZIPOUTPUTSTREAM_H__
