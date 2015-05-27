
#include "CInt64BufferHelper.h"
#include "Int64Buffer.h"

namespace Elastos {
namespace IO {

ECode CInt64BufferHelper::Allocate(
    /* [in] */ Int32 capacity,
    /* [out] */ IInt64Buffer** int64Buffer)
{
    return Int64Buffer::Allocate(capacity, int64Buffer);
}

ECode CInt64BufferHelper::WrapArray(
    /* [in] */ ArrayOf<Int64>* array,
    /* [out] */ IInt64Buffer** int64Buffer)
{
    return Int64Buffer::WrapArray(array, int64Buffer);
}

ECode CInt64BufferHelper::WrapArray(
    /* [in] */ ArrayOf<Int64>* array,
    /* [in] */ Int32 start,
    /* [in] */ Int32 int64Count,
    /* [out] */ IInt64Buffer** int64Buffer)
{
    return Int64Buffer::WrapArrayEx(array, start, int64Count, int64Buffer);
}

} // namespace IO
} // namespace Elastos

