
#include "coredef.h"
#include "FloatBuffer.h"
#include "ReadWriteFloatArrayBuffer.h"

namespace Elastos {
namespace IO {

//  {F4FEE9E7-CAA5-4d2e-9171-A96EB3CCB046}
extern "C" const InterfaceID EIID_FloatBuffer =
    { 0xf4fee9e7, 0xcaa5, 0x4d2e, { 0x91, 0x71, 0xa9, 0x6e, 0xb3, 0xcc, 0xb0, 0x46 } };

FloatBuffer::FloatBuffer(
    /* [in] */ Int32 capacity)
    : Buffer(2, capacity, NULL)
{}

ECode FloatBuffer::Allocate(
    /* [in] */ Int32 capacity,
    /* [out] */ IFloatBuffer** buf)
{
    VALIDATE_NOT_NULL(buf);

    if (capacity < 0) {
        // throw new IllegalArgumentException("capacity < 0: " + capacity);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    *buf = (IFloatBuffer*)new ReadWriteFloatArrayBuffer(capacity);
    REFCOUNT_ADD(*buf)
    return NOERROR;
}

ECode FloatBuffer::WrapArray(
    /* [in] */ ArrayOf<Float>* array,
    /* [out] */ IFloatBuffer** buf)
{
    return WrapArray(array, 0, array->GetLength(), buf);
}

ECode FloatBuffer::WrapArray(
    /* [in] */ ArrayOf<Float>* array,
    /* [in] */ Int32 start,
    /* [in] */ Int32 floatCount,
    /* [out] */ IFloatBuffer** buf)
{
    VALIDATE_NOT_NULL(buf);

    Int32 arrayLength = array->GetLength();
    if ((start | floatCount) < 0 || start > arrayLength || arrayLength - start < floatCount) {
        // throw new ArrayIndexOutOfBoundsException(arrayLength, offset,
        //         count);
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    AutoPtr<ReadWriteFloatArrayBuffer> rwBuf = new ReadWriteFloatArrayBuffer(array);
    rwBuf->mPosition = start;
    rwBuf->mLimit = start + floatCount;
    *buf = (IFloatBuffer*)rwBuf.Get();
    REFCOUNT_ADD(*buf)
    return NOERROR;
}

ECode FloatBuffer::GetArray(
    /* [out, callee] */ ArrayOf<Float>** array)
{
    return ProtectedArray(array);
}

ECode FloatBuffer::GetArrayOffset(
    /* [out] */ Int32* offset)
{
    return ProtectedArrayOffset(offset);
}

ECode FloatBuffer::CompareTo(
    /* [in] */ IFloatBuffer* otherBuffer,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    Int32 remaining = 0;
    Int32 otherRemaining = 0;
    GetRemaining(&remaining);
    otherBuffer->GetRemaining(&otherRemaining);
    Int32 compareRemaining = (remaining < otherRemaining) ? remaining : otherRemaining;
    Int32 thisPos = mPosition;
    Int32 otherPos = 0;
    otherBuffer->GetPosition(&otherPos);
    Float thisFloat = 0.0f;
    Float otherFloat = 0.0f;
    while (compareRemaining > 0) {
        GetFloat(thisPos, &thisFloat);
        otherBuffer->GetFloat(otherPos, &otherFloat);
        // checks for Float and NaN inequality
        if ((thisFloat != otherFloat)
                && ((thisFloat == thisFloat) || (otherFloat == otherFloat))) {
            *result = thisFloat < otherFloat ? -1 : 1;
            return NOERROR;
        }
        thisPos++;
        otherPos++;
        compareRemaining--;
    }
    *result = remaining - otherRemaining;
    return NOERROR;
}

ECode FloatBuffer::Equals(
    /* [in] */ IInterface* other,
    /* [out] */ Boolean* rst)
{
    VALIDATE_NOT_NULL(rst)

    if (IFloatBuffer::Probe(other) == NULL) {
        *rst = FALSE;
        return NOERROR;
    }

    AutoPtr<IFloatBuffer> otherBuffer = (IFloatBuffer*) other;
    Int32 thisRemaining = 0;
    Int32 otherRemaining = 0;
    GetRemaining(&thisRemaining);
    otherBuffer->GetRemaining(&otherRemaining);
    if (thisRemaining != otherRemaining) {
        *rst = FALSE;
        return NOERROR;
    }

    Int32 myPosition = mPosition;
    Int32 otherPosition = 0;
    otherBuffer->GetPosition(&otherPosition);
    Boolean equalSoFar = TRUE;
    Float thisValue = 0.0f;
    Float otherValue = 0.0f;
    while (equalSoFar && (myPosition < mLimit)) {
        FAIL_RETURN(GetFloat(myPosition++, &thisValue))
        FAIL_RETURN(otherBuffer->GetFloat(otherPosition++, &otherValue))
        equalSoFar = thisValue == otherValue || (thisValue != thisValue && otherValue != otherValue);
    }
    *rst = equalSoFar;
    return NOERROR;
}

ECode FloatBuffer::GetFloats(
    /* [out] */ ArrayOf<Float>* dst)
{
    return GetFloats(dst, 0, dst->GetLength());
}

ECode FloatBuffer::GetFloats(
    /* [out] */ ArrayOf<Float>* dst,
    /* [in] */ Int32 dstOffset,
    /* [in] */ Int32 floatCount)
{
    VALIDATE_NOT_NULL(dst);
    Int32 arrayLength = dst->GetLength();
    if ((dstOffset | floatCount) < 0 || dstOffset > arrayLength || arrayLength - dstOffset < floatCount) {
        // throw new ArrayIndexOutOfBoundsException(arrayLength, offset,
        //         count);
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    Int32 remaining = 0;
    GetRemaining(&remaining);
    if (floatCount > remaining) {
        // throw new BufferUnderflowException();
        return E_BUFFER_UNDER_FLOW_EXCEPTION;
    }

    for (Int32 i = dstOffset; i < dstOffset + floatCount; ++i) {
        GetFloat(&(*dst)[i]);
    }
    return NOERROR;
}

ECode FloatBuffer::HasArray(
    /* [out] */ Boolean* hasArray)
{
    return ProtectedHasArray(hasArray);
}

ECode FloatBuffer::PutFloats(
    /* [in] */ const ArrayOf<Float>& src)
{
    return PutFloats(src, 0, src.GetLength());
}

ECode FloatBuffer::PutFloats(
    /* [in] */ const ArrayOf<Float>& src,
    /* [in] */ Int32 srcOffset,
    /* [in] */ Int32 floatCount)
{
    Int32 arrayLength = src.GetLength();
    if ((srcOffset | floatCount) < 0 || srcOffset > arrayLength || arrayLength - srcOffset < floatCount) {
        // throw new ArrayIndexOutOfBoundsException(arrayLength, offset,
        //         count);
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    Int32 remaining = 0;
    GetRemaining(&remaining);
    if (floatCount > remaining) {
        // throw new BufferOverflowException();
        return E_BUFFER_OVER_FLOW_EXCEPTION;
    }

    for (Int32 i = srcOffset; i < srcOffset + floatCount; ++i) {
        PutFloat(src[i]);
    }
    return NOERROR;
}

ECode FloatBuffer::PutFloatBuffer(
    /* [in] */ IFloatBuffer* src)
{
    if (src == (IFloatBuffer*)this->Probe(EIID_IFloatBuffer)) {
        // throw new IllegalArgumentException("src == this");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    Int32 srcRemaining = 0;
    Int32 remaining = 0;
    src->GetRemaining(&srcRemaining);
    GetRemaining(&remaining);
    if (srcRemaining > remaining) {
        // throw new BufferOverflowException();
        return E_BUFFER_OVER_FLOW_EXCEPTION;
    }

    AutoPtr< ArrayOf<Float> > contents = ArrayOf<Float>::Alloc(srcRemaining);
    FAIL_RETURN(src->GetFloats(contents))
    return PutFloats(*contents);
}

} // namespace IO
} // namespace Elastos
