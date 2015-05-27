
#include "coredef.h"
#include "CUnsafeByteSequence.h"

namespace Elastos {
namespace IO{

ECode CUnsafeByteSequence::constructor(
    /* [in] */ Int32 capacity)
{
    mBytes = ArrayOf<Byte>::Alloc(capacity);
    return NOERROR;
}

ECode CUnsafeByteSequence::Size(
    /* [out]*/ Int32* size)
{
    *size = mCount;
    return NOERROR;
}

ECode CUnsafeByteSequence::Rewind()
{
    mCount = 0;
    return NOERROR;
}

ECode CUnsafeByteSequence::Write(
    /* [in] */ ArrayOf<Byte>* buffer,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length)
{
    if (mCount + length >= mBytes->GetLength()) {
        AutoPtr<ArrayOf<Byte> > newBytes = ArrayOf<Byte>::Alloc((mCount + length) * 2); // speed up
        memcpy(newBytes->GetPayload(), mBytes->GetPayload(), mCount);
        // newBytes->Copy(mBytes);
        mBytes = newBytes;
    }

    memcpy(mBytes->GetPayload() + mCount, buffer->GetPayload() + offset, length); // speed up
    // mBytes->Copy(mCount, buffer, offset, length); // speed up
    mCount += length;
    return NOERROR;
}

ECode CUnsafeByteSequence::Write(
    /* [in] */ Int32 b)
{
    if (mCount == mBytes->GetLength()) {
        AutoPtr<ArrayOf<Byte> > newBytes =  ArrayOf<Byte>::Alloc(mCount * 2);
        memcpy(newBytes->GetPayload(), mBytes->GetPayload(), mCount); // speed up
        // newBytes->Copy(mBytes);
        mBytes = newBytes;
    }

    (*mBytes)[mCount++] = (Byte)b;
    return NOERROR;
}

ECode CUnsafeByteSequence::ToByteArray(
    /* [out, callee] */ ArrayOf<Byte>** result)
{
    if (mCount == mBytes->GetLength()) {
        *result = mBytes;
        INTERFACE_ADDREF(*result);
        return NOERROR;
    }

    AutoPtr<ArrayOf<Byte> > newBytes = ArrayOf<Byte>::Alloc(mCount);
    newBytes->Copy(mBytes, 0, mCount);
    *result = newBytes;
    INTERFACE_ADDREF(*result);
    return NOERROR;
}

ECode CUnsafeByteSequence::ToString(
    /* [out] */ String* result)
{
    const char* temp = (const char*)mBytes->GetPayload();

    *result = String(temp, mCount);
    return NOERROR;
}

} // namespace IO
} // namespace Elastos
