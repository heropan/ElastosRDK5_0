
#include "coredef.h"
#include "DoubleBuffer.h"
#include "ReadWriteDoubleArrayBuffer.h"

namespace Elastos {
namespace IO {

// {6548CB4D-07DE-41e9-9AF8-8625E6DF3F08}
extern "C" const InterfaceID EIID_DoubleBuffer =
    { 0x6548cb4d, 0x7de, 0x41e9, { 0x9a, 0xf8, 0x86, 0x25, 0xe6, 0xdf, 0x3f, 0x8 } };

DoubleBuffer::DoubleBuffer(
    /* [in] */ Int32 capacity)
    : Buffer(3, capacity, NULL)
{}

ECode DoubleBuffer::Allocate(
    /* [in] */ Int32 capacity,
    /* [out] */ IDoubleBuffer** buf)
{
    VALIDATE_NOT_NULL(buf);

    if (capacity < 0) {
        // throw new IllegalArgumentException("capacity < 0: " + capacity);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    *buf = (IDoubleBuffer*)new ReadWriteDoubleArrayBuffer(capacity);
    REFCOUNT_ADD(*buf)
    return NOERROR;
}

ECode DoubleBuffer::Wrap(
    /* [in] */ ArrayOf<Double>* array,
    /* [out] */ IDoubleBuffer** buf)
{
    return Wrap(array, 0, array->GetLength(), buf);
}

ECode DoubleBuffer::Wrap(
    /* [in] */ ArrayOf<Double>* array,
    /* [in] */ Int32 start,
    /* [in] */ Int32 doubleCount,
    /* [out] */ IDoubleBuffer** buf)
{
    VALIDATE_NOT_NULL(buf);
    Int32 arrayLength = array->GetLength();
    if ((start | doubleCount) < 0 || start > arrayLength || arrayLength - start < doubleCount) {
        // throw new ArrayIndexOutOfBoundsException(arrayLength, offset,
        //         count);
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    AutoPtr<ReadWriteDoubleArrayBuffer> rwbuf = new ReadWriteDoubleArrayBuffer(array);
    rwbuf->mPosition = start;
    rwbuf->mLimit = start + doubleCount;
    *buf = (IDoubleBuffer*)rwbuf.Get();
    REFCOUNT_ADD(*buf)
    return NOERROR;
}

ECode DoubleBuffer::GetArray(
    /* [out, callee] */ ArrayOf<Double>** array)
{
    return ProtectedArray(array);
}

ECode DoubleBuffer::GetArrayOffset(
    /* [out] */ Int32* offset)
{
    return ProtectedArrayOffset(offset);
}

ECode DoubleBuffer::CompareTo(
    /* [in] */  IDoubleBuffer* otherBuffer,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(otherBuffer);
    VALIDATE_NOT_NULL(result);

    Int32 remaining = 0;
    Int32 otherRemaining = 0;
    GetRemaining(&remaining);
    otherBuffer->GetRemaining(&otherRemaining);
    Int32 compareRemaining = (remaining < otherRemaining) ? remaining : otherRemaining;

    Int32 thisPos = mPosition;
    Int32 otherPos = 0;
    otherBuffer->GetPosition(&otherPos);
    Double thisDouble, otherDouble;
    while (compareRemaining > 0) {
        GetDouble(thisPos, &thisDouble);
        otherBuffer->GetDouble(otherPos, &otherDouble);
        if ((thisDouble != otherDouble)
                && ((thisDouble == thisDouble) || (otherDouble == otherDouble))) {
            *result = thisDouble < otherDouble ? -1 : 1;
            return NOERROR;
        }
        thisPos++;
        otherPos++;
        compareRemaining--;
    }

    *result = remaining - otherRemaining;
    return NOERROR;
}

ECode DoubleBuffer::Equals(
    /* [in] */ IInterface* other,
    /* [out] */ Boolean* rst)
{
    VALIDATE_NOT_NULL(rst);
    *rst = FALSE;
    VALIDATE_NOT_NULL(other);

    DoubleBuffer* otherObj = (DoubleBuffer*)(other->Probe(EIID_DoubleBuffer));

    if (otherObj == NULL) return NOERROR;

    Int32 remaining = 0;
    Int32 otherRemaining = 0;
    GetRemaining(&remaining);
    otherObj->GetRemaining(&otherRemaining);
    if (remaining != otherRemaining) {
        *rst = FALSE;
        return NOERROR;
    }

    Int32 otherPosition = 0;
    Int32 myPosition = mPosition;
    otherObj->GetPosition(&otherPosition);
    Boolean equalSoFar = TRUE;
    Double thisValue = 0.0;
    Double otherValue = 0.0;
    while (equalSoFar && (mPosition < mLimit)) {
        this->GetDouble(myPosition++, &thisValue);
        otherObj->GetDouble(otherPosition++, &otherValue);
        equalSoFar = (thisValue == otherValue) ||
                     (thisValue != thisValue && otherValue != otherValue);
    }

    *rst = equalSoFar;
    return NOERROR;
}

ECode DoubleBuffer::GetDoubles(
    /* [out] */ ArrayOf<Double>* dst)
{
    return GetDoubles(dst, 0, dst->GetLength());
}

ECode DoubleBuffer::GetDoubles(
    /* [out] */ ArrayOf<Double>* dst,
    /* [in] */ Int32 dstOffset,
    /* [in] */ Int32 doubleCount)
{
    VALIDATE_NOT_NULL(dst);
    Int32 arrayLength = dst->GetLength();
    if ((dstOffset | doubleCount) < 0 || dstOffset > arrayLength || arrayLength - dstOffset < doubleCount) {
        // throw new ArrayIndexOutOfBoundsException(arrayLength, offset,
        //         count);
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    Int32 remaining = 0;
    GetRemaining(&remaining);
    if (doubleCount > remaining) {
        // throw new BufferUnderflowException();
        return E_BUFFER_UNDER_FLOW_EXCEPTION;
    }
    for (Int32 i = dstOffset; i < dstOffset + doubleCount; i++) {
        GetDouble(&(*dst)[i]);
    }

    return NOERROR;
}

ECode DoubleBuffer::HasArray(
    /* [out] */ Boolean* hasArray)
{
    return ProtectedHasArray(hasArray);
}

ECode DoubleBuffer::PutDoubles(
    /* [in] */ const ArrayOf<Double>& src)
{
    return PutDoubles(src, 0, src.GetLength());
}

ECode DoubleBuffer::PutDoubles(
    /* [in] */ const ArrayOf<Double>& src,
    /* [in] */ Int32 srcOffset,
    /* [in] */ Int32 doubleCount)
{
    Int32 arrayLength = src.GetLength();
    if ((srcOffset | doubleCount) < 0 || srcOffset > arrayLength || arrayLength - srcOffset < doubleCount) {
        // throw new ArrayIndexOutOfBoundsException(arrayLength, offset,
        //         count);
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    Int32 remaining = 0;
    GetRemaining(&remaining);
    if (doubleCount > remaining) {
        // throw new BufferOverflowException();
        return E_BUFFER_OVER_FLOW_EXCEPTION;
    }
    for (Int32 i = srcOffset; i < srcOffset + doubleCount; i++) {
        PutDouble(src[i]);
    }
    return NOERROR;
}

ECode DoubleBuffer::PutDoubleBuffer(
    /* [in] */ IDoubleBuffer* src)
{
    if (src == (IDoubleBuffer*)this->Probe(EIID_IDoubleBuffer)) {
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

    AutoPtr< ArrayOf<Double> > doubles = ArrayOf<Double>::Alloc(srcRemaining);
    FAIL_RETURN(src->GetDoubles(doubles))
    return PutDoubles(*doubles);
}

} // namespace IO
} // namespace Elastos
