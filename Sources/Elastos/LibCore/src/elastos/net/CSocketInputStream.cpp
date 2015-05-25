
#include "CSocketInputStream.h"
#include <cmdef.h>

namespace Elastos {
namespace Net {

ECode CSocketInputStream::Available(
    /* [out] */ Int32* number)
{
    return mSocket->Available(number);
}

ECode CSocketInputStream::Close()
{
    return mSocket->Close();;
}

ECode CSocketInputStream::Mark(
    /* [in] */ Int32 readLimit)
{
    return E_NOT_IMPLEMENTED;
}

ECode CSocketInputStream::IsMarkSupported(
    /* [out] */ Boolean* pSupported)
{
    return E_NOT_IMPLEMENTED;
}

ECode CSocketInputStream::Read(
    /* [out] */ Int32* value)
{
    AutoPtr<ArrayOf<Byte> > buffer = ArrayOf<Byte>::Alloc(1);
    Int32 result;
    mSocket->Read(buffer, 0, 1, &result);
    if (result == -1) {
        *value = -1;
    } else {
        *value = (*buffer)[0] & 0xFF;
    }
    return NOERROR;
}

ECode CSocketInputStream::ReadBytes(
    /* [out] */ ArrayOf<Byte> * buffer,
    /* [out] */ Int32* number)
{
    return ReadBytesEx(buffer, 0, buffer->GetLength(), number);
}

ECode CSocketInputStream::ReadBytesEx(
    /* [out] */ ArrayOf<Byte> * buffer,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [out] */ Int32* number)
{
    if (buffer == NULL) {
        return E_INVALID_ARGUMENT;
    }

    if (length == 0) {
        *number = 0;
        return NOERROR;
    }

    if (0 > offset || offset >= buffer->GetLength()) {
        return E_BUFFER_UNDER_FLOW_EXCEPTION;
    }
    if (0 > length || offset + length > buffer->GetLength()) {
        return E_BUFFER_UNDER_FLOW_EXCEPTION;
    }

    return mSocket->Read(buffer, offset, length, number);
}

ECode CSocketInputStream::Reset()
{
    return E_NOT_IMPLEMENTED;
}

ECode CSocketInputStream::Skip(
    /* [in] */ Int64 count,
    /* [out] */ Int64* number)
{
    if (count == 0) {
        *number = 0;
    } else {

    }
    return NOERROR;
}

ECode CSocketInputStream::constructor(
    /* [in] */ ISocketImpl* impl)
{
    mSocket = (PlainSocketImpl*) impl;
    return NOERROR;
}

PInterface CSocketInputStream::Probe(
    /* [in] */ REIID riid)
{
    return _CSocketInputStream::Probe(riid);
}

ECode CSocketInputStream::GetLock(
    /* [out] */ IInterface** lockobj)
{
    VALIDATE_NOT_NULL(lockobj);

    AutoPtr<IInterface> obj = InputStream::GetLock();
    *lockobj = obj;
    INTERFACE_ADDREF(*lockobj);
    return NOERROR;
}

} // namespace Net
} // namespace Elastos
