#ifndef __ELASTOS_IO_CNIOACCESSHELPER_H__
#define __ELASTOS_IO_CNIOACCESSHELPER_H__

#include "_Elastos_IO_CNIOAccessHelper.h"

namespace Elastos {
namespace IO {

CarClass(CNIOAccessHelper)
{
public:
    /**
     * Returns the underlying native pointer to the data of the given
     * Buffer starting at the Buffer's current position, or 0 if the
     * Buffer is not backed by native heap storage. Note that this is
     * different than what the Harmony implementation calls a "base
     * address."
     *
     * @param Buffer b the Buffer to be queried
     * @return the native pointer to the Buffer's data at its current
     * position, or 0 if there is none
     */
    CARAPI GetBasePointer(
        /* [in] */ IBuffer* b,
        /* [out] */ Int64* pointer);

    /**
     * Returns the number of bytes remaining in the given Buffer. That is,
     * this scales <code>remaining()</code> by the byte-size of elements
     * of this Buffer.
     *
     * @param Buffer b the Buffer to be queried
     * @return the number of remaining bytes
     */
    CARAPI GetRemainingBytes(
        /* [in] */ IBuffer* b,
        /* [out] */ Int32* remaining);


    /**
     * Returns the offset in bytes from the start of the underlying
     * Java array object containing the data of the given Buffer to
     * the actual start of the data. This method is only meaningful if
     * getBaseArray() returns non-null.
     *
     * @param Buffer b the Buffer to be queried
     * @return the data offset in bytes to the start of this Buffer's data
     */
    CARAPI GetBaseArrayOffset(
        /* [in] */ IBuffer* b,
        /* [out] */ Int32* offset);
};

} // namespace IO
} // namespace Elastos

#endif