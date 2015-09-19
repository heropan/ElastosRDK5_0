
#include "NIOAccess.h"
#include "Buffer.h"

namespace Elastos {
namespace IO {

Int64 NIOAccess::GetBasePointer(
    /* [in] */ IBuffer* b)
{
    VALIDATE_NOT_NULL(b)
    Buffer* buf = (Buffer*)b;
    if (buf == NULL || buf->mEffectiveDirectAddress == 0) {
        return 0L;
    }
    return buf->mEffectiveDirectAddress + (buf->mPosition << buf->mElementSizeShift);
}

AutoPtr<IInterface> NIOAccess::GetBaseArray(
    /* [in] */ IBuffer* b)
{
    assert(b);
    Boolean isflag = FALSE;
    b->HasArray(&isflag);
    if (isflag) {
        AutoPtr<IArrayOf> outface;
        b->GetArray((IArrayOf**)&outface);
        return outface;
    }

    return NULL;
}

Int32 NIOAccess::GetBaseArrayOffset(
    /* [in] */ IBuffer* b)
{
    VALIDATE_NOT_NULL(b)
    Boolean hasArray;
    b->HasArray(&hasArray);
    if (hasArray) {
        Int32 offset, position;
        Buffer* buf = (Buffer*)b;
        b->GetArrayOffset(&offset);
        b->GetPosition(&position);
        return (offset + position) << buf->mElementSizeShift;
    }

    return 0;
}

} // namespace IO
} // namespace Elastos
