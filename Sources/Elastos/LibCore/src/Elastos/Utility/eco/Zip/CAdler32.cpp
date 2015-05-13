
#include "cmdef.h"
#include "CAdler32.h"
#include <zlib.h>

namespace Elastos {
namespace Utility {
namespace Zip {


CAdler32::CAdler32()
    : mAdler(1ll)
{}

ECode CAdler32::GetValue(
    /* [out] */ Int64* value)
{
    VALIDATE_NOT_NULL(value);
    *value = mAdler;
    return NOERROR;
}

ECode CAdler32::Reset()
{
    mAdler = 1;
    return NOERROR;
}

ECode CAdler32::Update(
    /* [in] */ Int32 i)
{
    mAdler = UpdateByteImpl(i, mAdler);
    return NOERROR;
}

ECode CAdler32::UpdateEx(
    /* [in] */ const ArrayOf<Byte>& buf)
{
    return UpdateEx2(buf, 0, buf.GetLength());
}

ECode CAdler32::UpdateEx2(
    /* [in] */ const ArrayOf<Byte>& buf,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 byteCount)
{
    if ((offset | byteCount) < 0 || offset > buf.GetLength()
            || buf.GetLength() - offset < byteCount) {
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
//        throw new ArrayIndexOutOfBoundsException(arrayLength, offset,
//                count);
    }

    mAdler = UpdateImpl(buf, offset, byteCount, mAdler);
    return NOERROR;
}

Int64 CAdler32::UpdateImpl(
    /* [in] */ const ArrayOf<Byte>& buf,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 byteCount,
    /* [in] */ Int64 adler)
{
    return adler32(adler, reinterpret_cast<const Bytef*>(buf.GetPayload() + offset),
            byteCount);
}

Int64 CAdler32::UpdateByteImpl(
    /* [in] */ Int32 val,
    /* [in] */ Int64 adler)
{
    Bytef bytefVal = val;
    return adler32(adler, reinterpret_cast<const Bytef*>(&bytefVal), 1);
}


} // namespace Zip
} // namespace Utility
} // namespace Elastos
