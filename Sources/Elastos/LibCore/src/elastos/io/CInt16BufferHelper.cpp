
#include "CInt16BufferHelper.h"
#include "Int16Buffer.h"

namespace Elastos {
namespace IO {

ECode CInt16BufferHelper::Allocate(
    /* [in] */ Int32 capacity,
    /* [out] */ IInt16Buffer** int16Buffer)
{
    return Int16Buffer::Allocate(capacity, int16Buffer);
}

ECode CInt16BufferHelper::WrapArray(
    /* [in] */ ArrayOf<Int16>* array,
    /* [out] */ IInt16Buffer** int16Buffer)
{
    return Int16Buffer::WrapArray(array, int16Buffer);
}

ECode CInt16BufferHelper::WrapArray(
    /* [in] */ ArrayOf<Int16>* array,
    /* [in] */ Int32 start,
    /* [in] */ Int32 int16Count,
    /* [out] */ IInt16Buffer** int16Buffer)
{
    return Int16Buffer::WrapArray(array, start, int16Count, int16Buffer);
}

} // namespace IO
} // namespace Elastos
