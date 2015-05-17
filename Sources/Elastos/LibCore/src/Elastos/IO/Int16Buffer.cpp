
#include "cmdef.h"
#include "Int16Buffer.h"
#include "ReadWriteInt16ArrayBuffer.h"

namespace Elastos {
namespace IO {

//  {AE6AA98E-18B9-4de2-A2DA-C030CEF90746}
extern "C" const InterfaceID EIID_Int16Buffer =
     { 0xae6aa98e, 0x18b9, 0x4de2, { 0xa2, 0xda, 0xc0, 0x30, 0xce, 0xf9, 0x7, 0x46 } };

Int16Buffer::Int16Buffer(
    /* [in] */ Int32 capacity)
    : Buffer(1, capacity, NULL)
{}

ECode Int16Buffer::Allocate(
    /* [in] */ Int32 capacity,
    /* [out] */ IInt16Buffer** buf)
{
    VALIDATE_NOT_NULL(buf);

    if (capacity < 0) {
        // throw new IllegalArgumentException("capacity < 0: " + capacity);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    *buf = (IInt16Buffer*)new ReadWriteInt16ArrayBuffer(capacity);
    INTERFACE_ADDREF(*buf)
    return NOERROR;
}

ECode Int16Buffer::WrapArray(
    /* [in] */ ArrayOf<Int16>* array,
    /* [out] */ IInt16Buffer** buf)
{
    return WrapArrayEx(array, 0, array->GetLength(), buf);
}

ECode Int16Buffer::WrapArrayEx(
    /* [in] */ ArrayOf<Int16>* array,
    /* [in] */ Int32 start,
    /* [in] */ Int32 int16Count,
    /* [out] */ IInt16Buffer** buf)
{
    VALIDATE_NOT_NULL(buf);
    Int32 arrayLength = array->GetLength();
    if ((start | int16Count) < 0 || start > arrayLength || arrayLength - start < int16Count) {
        // throw new ArrayIndexOutOfBoundsException(arrayLength, offset,
        //         count);
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    AutoPtr<ReadWriteInt16ArrayBuffer> rwBuf = new ReadWriteInt16ArrayBuffer(array);
    rwBuf->mPosition = start;
    rwBuf->mLimit = start + int16Count;
    *buf = (IInt16Buffer*)rwBuf.Get();
    INTERFACE_ADDREF(*buf)
    return NOERROR;
}

ECode Int16Buffer::GetArray(
    /* [out, callee] */ ArrayOf<Int16>** array)
{
    return ProtectedArray(array);
}

ECode Int16Buffer::GetArrayOffset(
    /* [out] */ Int32* offset)
{
    return ProtectedArrayOffset(offset);
}

ECode Int16Buffer::CompareTo(
    /* [in] */ IInt16Buffer* otherBuffer,
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
    Int16 thisInt16 = 0;
    Int16 otherInt16 = 0;
    while (compareRemaining > 0) {
        GetInt16Ex(thisPos, &thisInt16);
        otherBuffer->GetInt16Ex(otherPos, &otherInt16);
        // checks for Int16 and NaN inequality
        if (thisInt16 != otherInt16) {
            *result = thisInt16 < otherInt16 ? -1 : 1;
            return NOERROR;
        }
        thisPos++;
        otherPos++;
        compareRemaining--;
    }
    *result = remaining - otherRemaining;
    return NOERROR;
}

ECode Int16Buffer::Equals(
    /* [in] */ IInterface* other,
    /* [out] */ Boolean* rst)
{
    VALIDATE_NOT_NULL(rst)

    if (IInt16Buffer::Probe(other) == NULL) {
        *rst = FALSE;
        return NOERROR;
    }

    AutoPtr<IInt16Buffer> otherBuffer = (IInt16Buffer*) other;
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
    Int16 thisValue = 0;
    Int16 otherValue = 0;
    while (equalSoFar && (myPosition < mLimit)) {
        FAIL_RETURN(GetInt16Ex(myPosition++, &thisValue))
        FAIL_RETURN(otherBuffer->GetInt16Ex(otherPosition++, &otherValue))
        equalSoFar = thisValue == otherValue;
    }
    *rst = equalSoFar;
    return NOERROR;
}

ECode Int16Buffer::GetInt16s(
    /* [out] */ ArrayOf<Int16>* dst)
{
    return GetInt16sEx(dst, 0, dst->GetLength());
}

ECode Int16Buffer::GetInt16sEx(
    /* [out] */ ArrayOf<Int16>* dst,
    /* [in] */ Int32 dstOffset,
    /* [in] */ Int32 int16Count)
{
    VALIDATE_NOT_NULL(dst);
    Int32 arrayLength = dst->GetLength();
    if ((dstOffset | int16Count) < 0 || dstOffset > arrayLength || arrayLength - dstOffset < int16Count) {
        // throw new ArrayIndexOutOfBoundsException(arrayLength, offset,
        //         count);
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    Int32 remaining = 0;
    GetRemaining(&remaining);
    if (int16Count > remaining) {
        // throw new BufferUnderflowException();
        return E_BUFFER_UNDER_FLOW_EXCEPTION;
    }
    for (Int32 i = dstOffset; i < dstOffset + int16Count; ++i) {
        GetInt16(&(*dst)[i]);
    }

    return NOERROR;
}

ECode Int16Buffer::HasArray(
    /* [out] */ Boolean* hasArray)
{
    return ProtectedHasArray(hasArray);
}

ECode Int16Buffer::PutInt16s(
    /* [in] */ const ArrayOf<Int16>& src)
{
    return PutInt16sEx(src, 0, src.GetLength());
}

ECode Int16Buffer::PutInt16sEx(
    /* [in] */ const ArrayOf<Int16>& src,
    /* [in] */ Int32 srcOffset,
    /* [in] */ Int32 int16Count)
{
    Int32 arrayLength = src.GetLength();
    if ((srcOffset | int16Count) < 0 || srcOffset > arrayLength || arrayLength - srcOffset < int16Count) {
        // throw new ArrayIndexOutOfBoundsException(arrayLength, offset,
        //         count);
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    Int32 remaining = 0;
    GetRemaining(&remaining);
    if (int16Count > remaining) {
        // throw new BufferOverflowException();
        return E_BUFFER_OVER_FLOW_EXCEPTION;
    }
    for (Int32 i = srcOffset; i < srcOffset + int16Count; ++i) {
        PutInt16(src[i]);
    }
    return NOERROR;
}

ECode Int16Buffer::PutInt16Buffer(
    /* [in] */ IInt16Buffer* src)
{
    if (src == (IInt16Buffer*)this->Probe(EIID_IInt16Buffer)) {
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

    AutoPtr< ArrayOf<Int16> > contents = ArrayOf<Int16>::Alloc(srcRemaining);
    FAIL_RETURN(src->GetInt16s(contents))
    return PutInt16s(*contents);
}

} // namespace IO
} // namespace Elastos
