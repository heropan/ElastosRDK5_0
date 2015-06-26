
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

AutoPtr<IInterface> NIOAccess::GetBaseArray(
    /* [in] */ IBuffer* b)
{
    Boolean isflag = FALSE;
    b->HasArray(&isflag);
    AutoPtr<IInterface> outface;
    if (isflag) {
        assert(0 && "TODO");
        // b->GetArray((IInterface**)&outface);
    }
    else {
        outface = NULL;
    }
    return outface;
}

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
