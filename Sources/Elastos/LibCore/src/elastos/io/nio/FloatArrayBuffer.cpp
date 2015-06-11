
#include "FloatArrayBuffer.h"
#include "CByteOrderHelper.h"

namespace Elastos {
namespace IO {

FloatArrayBuffer::FloatArrayBuffer(
    /* [in] */ ArrayOf<Float>* array)
    : FloatBuffer(array->GetLength())
    , mBackingArray(array)
    , mOffset(0)
{}

FloatArrayBuffer::FloatArrayBuffer(
    /* [in] */ Int32 capacity)
    : FloatBuffer(capacity)
    , mBackingArray(ArrayOf<Float>::Alloc(capacity))
    , mOffset(0)
{}

FloatArrayBuffer::FloatArrayBuffer(
    /* [in] */ Int32 capacity,
    /* [in] */ ArrayOf<Float>* backingArray,
    /* [in] */ Int32 offset)
    : FloatBuffer(capacity)
    , mBackingArray(backingArray)
    , mOffset(offset)
{}

ECode FloatArrayBuffer::GetFloat(
    /* [out] */ Float* value)
{
    VALIDATE_NOT_NULL(value);
    if (mPosition == mLimit) {
        // throw new BufferUnderflowException();
        return E_BUFFER_UNDER_FLOW_EXCEPTION;
    }
    *value = (*mBackingArray)[mOffset + mPosition++];
    return NOERROR;
}

ECode FloatArrayBuffer::GetFloat(
    /* [in] */ Int32 index,
    /* [out] */ Float* value)
{
    VALIDATE_NOT_NULL(value);
    FAIL_RETURN(CheckIndex(index))
    *value = (*mBackingArray)[mOffset + index];
    return NOERROR;
}

ECode FloatArrayBuffer::GetFloats(
    /* [out] */ ArrayOf<Float>* dst,
    /* [in] */ Int32 dstOffset,
    /* [in] */ Int32 floatCount)
{
    VALIDATE_NOT_NULL(dst);
    Int32 remaining = 0;
    GetRemaining(&remaining);
    if (floatCount > remaining) {
        // throw new BufferUnderflowException();
        return E_BUFFER_UNDER_FLOW_EXCEPTION;
    }

    dst->Copy(dstOffset, mBackingArray, mOffset + mPosition, floatCount);
    mPosition += floatCount;
    return NOERROR;
}

ECode FloatArrayBuffer::IsDirect(
    /* [out] */ Boolean* isDirect)
{
    VALIDATE_NOT_NULL(isDirect);
    *isDirect = FALSE;
    return NOERROR;
}

ECode FloatArrayBuffer::GetOrder(
   /* [out] */ ByteOrder* byteOrder)
{
    AutoPtr<IByteOrderHelper> helper;
    ASSERT_SUCCEEDED(CByteOrderHelper::AcquireSingleton((IByteOrderHelper**)&helper))
    return helper->GetNativeOrder(byteOrder);
}

} // namespace IO
} // namespace Elastos
