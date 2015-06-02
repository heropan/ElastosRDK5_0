
#include "coredef.h"
#include "NIOAccess.h"
#include "Buffer.h"

namespace Elastos {
namespace IO {

Int64 NIOAccess::GetBasePointer(
    /* [in] */ IBuffer* b)
{
    VALIDATE_NOT_NULL(b)
    Buffer* buf = reinterpret_cast<Buffer*>(b->Probe(EIID_Buffer));
    if (buf == NULL || buf->mEffectiveDirectAddress == 0)
    {
        return 0L;
    }
    return buf->mEffectiveDirectAddress + (buf->mPosition << buf->mElementSizeShift);
}

/**
 * Returns the number of bytes remaining in the given Buffer. That is,
 * this scales <code>remaining()</code> by the byte-size of elements
 * of this Buffer.
 *
 * @param Buffer b the Buffer to be queried
 * @return the number of remaining bytes
 */
Int32 NIOAccess::GetRemainingBytes(
    /* [in] */ IBuffer* b)
{
    VALIDATE_NOT_NULL(b)
    Buffer* buf = NULL;
    if (b->Probe(EIID_Buffer) != NULL) {
        buf = reinterpret_cast<Buffer*>(b->Probe(EIID_Buffer));
        return (buf->mLimit - buf->mPosition) << buf->mElementSizeShift;
    }
    else {
        return -1;
    }
}

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
Int32 NIOAccess::GetBaseArrayOffset(
    /* [in] */ IBuffer* b)
{
    VALIDATE_NOT_NULL(b)
    Boolean hasArray;
    b->HasArray(&hasArray);
    Buffer* buf = reinterpret_cast<Buffer*>(b->Probe(EIID_Buffer));
    if (hasArray && buf != NULL) {
        Int32 offset, position;
        b->GetArrayOffset(&offset);
        b->GetPosition(&position);
        return (offset + position)<< buf->mElementSizeShift;
    }
    else {
        return 0;
    }
}

} // namespace IO
} // namespace Elastos
