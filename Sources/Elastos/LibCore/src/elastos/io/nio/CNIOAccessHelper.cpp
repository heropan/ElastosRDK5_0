#include "CNIOAccessHelper.h"
#include "NIOAccess.h"

namespace Elastos {
namespace IO {

ECode CNIOAccessHelper::GetBasePointer(
    /* [in] */ IBuffer* b,
    /* [out] */ Int64* pointer)
{
    VALIDATE_NOT_NULL(pointer)
    *pointer = NIOAccess::GetBasePointer(b);
    return NOERROR;
}

ECode CNIOAccessHelper::GetRemainingBytes(
    /* [in] */ IBuffer* b,
    /* [out] */ Int32* remaining)
{
    VALIDATE_NOT_NULL(remaining)
    *remaining = NIOAccess::GetRemainingBytes(b);
    return NOERROR;
}


ECode CNIOAccessHelper::GetBaseArrayOffset(
    /* [in] */ IBuffer* b,
    /* [out] */ Int32* offset)
{
    VALIDATE_NOT_NULL(offset)
    *offset = NIOAccess::GetBaseArrayOffset(b);
    return NOERROR;
}

} // namespace IO
} // namespace Elastos