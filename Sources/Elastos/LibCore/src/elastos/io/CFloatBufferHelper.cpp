
#include "CFloatBufferHelper.h"
#include "FloatBuffer.h"

namespace Elastos {
namespace IO {

ECode CFloatBufferHelper::Allocate(
    /* [in] */ Int32 capacity,
    /* [out] */ IFloatBuffer** floatBuffer)
{
    return FloatBuffer::Allocate(capacity, floatBuffer);
}

ECode CFloatBufferHelper::WrapArray(
    /* [in] */ ArrayOf<Float>* array,
    /* [out] */ IFloatBuffer** floatBuffer)
{
    return FloatBuffer::WrapArray(array, floatBuffer);
}

ECode CFloatBufferHelper::WrapArray(
    /* [in] */ ArrayOf<Float>* array,
    /* [in] */ Int32 start,
    /* [in] */ Int32 floatCount,
    /* [out] */ IFloatBuffer** floatBuffer)
{
    return FloatBuffer::WrapArrayEx(array, start, floatCount, floatBuffer);
}

} // namespace IO
} // namespace Elastos

