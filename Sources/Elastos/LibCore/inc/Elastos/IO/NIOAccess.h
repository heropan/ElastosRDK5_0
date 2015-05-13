#ifndef __NIOAccess_H__
#define __NIOAccess_H__

#include <elastos.h>
#include <Elastos.Core_server.h>

namespace Elastos {
namespace IO {

/**
 * This class is used via JNI by code in frameworks/base/.
 */
class NIOAccess
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
    static CARAPI_(Int64) GetBasePointer(
        /* [in] */ IBuffer* b);

    /**
     * Returns the number of bytes remaining in the given Buffer. That is,
     * this scales <code>remaining()</code> by the byte-size of elements
     * of this Buffer.
     *
     * @param Buffer b the Buffer to be queried
     * @return the number of remaining bytes
     */
    static CARAPI_(Int32) GetRemainingBytes(
        /* [in] */ IBuffer* b);

    /**
     * Returns the underlying Java array containing the data of the
     * given Buffer, or null if the Buffer is not backed by a Java array.
     *
     * @param Buffer b the Buffer to be queried
     * @return the Java array containing the Buffer's data, or null if
     * there is none
     */
    //static Object getBaseArray(Buffer b);

    /**
     * Returns the offset in bytes from the start of the underlying
     * Java array object containing the data of the given Buffer to
     * the actual start of the data. This method is only meaningful if
     * getBaseArray() returns non-null.
     *
     * @param Buffer b the Buffer to be queried
     * @return the data offset in bytes to the start of this Buffer's data
     */
    static CARAPI_(Int32) GetBaseArrayOffset(
        /* [in] */ IBuffer* b);
};

} // namespace IO
} // namespace Elastos

#endif // __NIOAccess_H__
