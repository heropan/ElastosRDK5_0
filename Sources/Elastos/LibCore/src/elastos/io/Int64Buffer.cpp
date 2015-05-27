
#include "coredef.h"
#include "Int64Buffer.h"
#include "ReadWriteInt64ArrayBuffer.h"

namespace Elastos {
namespace IO {

//  {286E2FAC-3118-4904-8D87-AD0C30BF47C7}
extern "C" const InterfaceID EIID_Int64Buffer =
     { 0x286e2fac, 0x3118, 0x4904, { 0x8d, 0x87, 0xad, 0xc, 0x30, 0xbf, 0x47, 0xc7 } };

Int64Buffer::Int64Buffer(
    /* [in] */ Int32 capacity)
    : Buffer(3, capacity, NULL)
{}

ECode Int64Buffer::Allocate(
    /* [in] */ Int32 capacity,
    /* [out] */ IInt64Buffer** buf)
{
    VALIDATE_NOT_NULL(buf);

    if (capacity < 0) {
        // throw new IllegalArgumentException("capacity < 0: " + capacity);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    *buf = (IInt64Buffer*)new ReadWriteInt64ArrayBuffer(capacity);
    INTERFACE_ADDREF(*buf)
    return NOERROR;
}

ECode Int64Buffer::WrapArray(
    /* [in] */ ArrayOf<Int64>* array,
    /* [out] */ IInt64Buffer** buf)
{
    return WrapArrayEx(array, 0, array->GetLength(), buf);
}

ECode Int64Buffer::WrapArray(
    /* [in] */ ArrayOf<Int64>* array,
    /* [in] */ Int32 start,
    /* [in] */ Int32 int64Count,
    /* [out] */ IInt64Buffer** buf)
{
    VALIDATE_NOT_NULL(buf);
    Int32 arrayLength = array->GetLength();
    if ((start | int64Count) < 0 || start > arrayLength || arrayLength - start < int64Count) {
        // throw new ArrayIndexOutOfBoundsException(arrayLength, offset,
        //         count);
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    AutoPtr<ReadWriteInt64ArrayBuffer> rwBuf = new ReadWriteInt64ArrayBuffer(array);
    rwBuf->mPosition = start;
    rwBuf->mLimit = start + int64Count;
    *buf = (IInt64Buffer*)rwBuf.Get();
    INTERFACE_ADDREF(*buf)
    return NOERROR;
}

ECode Int64Buffer::GetArray(
    /* [out, callee] */ ArrayOf<Int64>** array)
{
    return ProtectedArray(array);
}

ECode Int64Buffer::GetArrayOffset(
    /* [out] */ Int32* offset)
{
    return ProtectedArrayOffset(offset);
}

ECode Int64Buffer::CompareTo(
    /* [in] */ IInt64Buffer* otherBuffer,
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
    Int64 thisInt64 = 0;
    Int64 otherInt64 = 0;
    while (compareRemaining > 0) {
        GetInt64Ex(thisPos, &thisInt64);
        otherBuffer->GetInt64Ex(otherPos, &otherInt64);
        // checks for Int64 and NaN inequality
        if (thisInt64 != otherInt64) {
            *result = thisInt64 < otherInt64 ? -1 : 1;
            return NOERROR;
        }
        thisPos++;
        otherPos++;
        compareRemaining--;
    }
    *result = remaining - otherRemaining;
    return NOERROR;
}

ECode Int64Buffer::Equals(
    /* [in] */ IInterface* other,
    /* [out] */ Boolean* rst)
{
    VALIDATE_NOT_NULL(rst)

    if (IInt64Buffer::Probe(other) == NULL) {
        *rst = FALSE;
        return NOERROR;
    }

    AutoPtr<IInt64Buffer> otherBuffer = (IInt64Buffer*) other;
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
    Int64 thisValue = 0;
    Int64 otherValue = 0;
    while (equalSoFar && (myPosition < mLimit)) {
        FAIL_RETURN(GetInt64Ex(myPosition++, &thisValue))
        FAIL_RETURN(otherBuffer->GetInt64Ex(otherPosition++, &otherValue))
        equalSoFar = thisValue == otherValue;
    }
    *rst = equalSoFar;
    return NOERROR;
}

ECode Int64Buffer::GetInt64s(
    /* [out] */ ArrayOf<Int64>* dst)
{
    return GetInt64sEx(dst, 0, dst->GetLength());
}

ECode Int64Buffer::GetInt64s(
    /* [out] */ ArrayOf<Int64>* dst,
    /* [in] */ Int32 dstOffset,
    /* [in] */ Int32 int64Count)
{
    VALIDATE_NOT_NULL(dst);
    Int32 arrayLength = dst->GetLength();
    if ((dstOffset | int64Count) < 0 || dstOffset > arrayLength || arrayLength - dstOffset < int64Count) {
        // throw new ArrayIndexOutOfBoundsException(arrayLength, offset,
        //         count);
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    Int32 remaining = 0;
    GetRemaining(&remaining);
    if (int64Count > remaining) {
        // throw new BufferUnderflowException();
        return E_BUFFER_UNDER_FLOW_EXCEPTION;
    }

    for (Int32 i = dstOffset; i < dstOffset + int64Count; ++i) {
        GetInt64(&(*dst)[i]);
    }
    return NOERROR;
}

ECode Int64Buffer::HasArray(
    /* [out] */ Boolean* hasArray)
{
    return ProtectedHasArray(hasArray);
}

ECode Int64Buffer::PutInt64s(
    /* [in] */ const ArrayOf<Int64>& src)
{
    return PutInt64sEx(src, 0, src.GetLength());
}

ECode Int64Buffer::PutInt64s(
    /* [in] */ const ArrayOf<Int64>& src,
    /* [in] */ Int32 srcOffset,
    /* [in] */ Int32 int64Count)
{
    Int32 arrayLength = src.GetLength();
    if ((srcOffset | int64Count) < 0 || srcOffset > arrayLength || arrayLength - srcOffset < int64Count) {
        // throw new ArrayIndexOutOfBoundsException(arrayLength, offset,
        //         count);
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    Int32 remaining = 0;
    GetRemaining(&remaining);
    if (int64Count > remaining) {
        // throw new BufferOverflowException();
        return E_BUFFER_OVER_FLOW_EXCEPTION;
    }

    for (Int32 i = srcOffset; i < srcOffset + int64Count; ++i) {
        PutInt64(src[i]);
    }
    return NOERROR;
}

ECode Int64Buffer::PutInt64Buffer(
    /* [in] */ IInt64Buffer* src)
{
    if (src == (IInt64Buffer*)this->Probe(EIID_IInt64Buffer)) {
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

    AutoPtr< ArrayOf<Int64> > contents = ArrayOf<Int64>::Alloc(srcRemaining);
    FAIL_RETURN(src->GetInt64s(contents))
    return PutInt64s(*contents);
}

} // namespace IO
} // namespace Elastos
