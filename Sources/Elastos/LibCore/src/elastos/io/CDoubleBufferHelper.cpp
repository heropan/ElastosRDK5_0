
#include "CDoubleBufferHelper.h"
#include "DoubleBuffer.h"

namespace Elastos {
namespace IO {

ECode CDoubleBufferHelper::Allocate(
    /* [in] */ Int32 capacity,
    /* [out] */ IDoubleBuffer** doubleBuf)
{
    return DoubleBuffer::Allocate(capacity, doubleBuf);
}

ECode CDoubleBufferHelper::WrapArray(
    /* [in] */ ArrayOf<Double>* array,
    /* [out] */ IDoubleBuffer** doubleBuf)
{
    return DoubleBuffer::WrapArray(array, doubleBuf);
}

ECode CDoubleBufferHelper::WrapArray(
    /* [in] */ ArrayOf<Double>* array,
    /* [in] */ Int32 start,
    /* [in] */ Int32 doubleCount,
    /* [out] */ IDoubleBuffer** doubleBuf)
{
    return DoubleBuffer::WrapArray(array, start, doubleCount, doubleBuf);
}

} // namespace IO
} // namespace Elastos
