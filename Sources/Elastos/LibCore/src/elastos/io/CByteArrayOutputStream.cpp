
#include "coredef.h"
#include "CByteArrayOutputStream.h"

namespace Elastos {
namespace IO {

ECode CByteArrayOutputStream::Close()
{
    return ByteArrayOutputStream::Close();
}

ECode CByteArrayOutputStream::Reset()
{
    Object::Autolock lock(mLock);

    return ByteArrayOutputStream::Reset();
}

ECode CByteArrayOutputStream::GetSize(
    /* [out] */ Int32* size)
{
    VALIDATE_NOT_NULL(size);

    return ByteArrayOutputStream::GetSize(size);
}

ECode CByteArrayOutputStream::ToByteArray(
    /* [out, callee] */ ArrayOf<Byte>** bytes)
{
    VALIDATE_NOT_NULL(bytes);

    Object::Autolock lock(mLock);

    return ByteArrayOutputStream::ToByteArray(bytes);
}

ECode CByteArrayOutputStream::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);

    return ByteArrayOutputStream::ToString(str);
}

ECode CByteArrayOutputStream::ToString(
    /* [in] */ const String& enc,
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);

    return ByteArrayOutputStream::ToStringEx(enc, str);
}

ECode CByteArrayOutputStream::WriteBytes(
    /* [in] */ const ArrayOf<Byte>& buffer,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count)
{
    Object::Autolock lock(mLock);

    return ByteArrayOutputStream::WriteBytesEx(buffer, offset, count);
}

ECode CByteArrayOutputStream::Write(
    /* [in] */ Int32 oneByte)
{
    Object::Autolock lock(mLock);

    return ByteArrayOutputStream::Write(oneByte);
}

ECode CByteArrayOutputStream::WriteTo(
    /* [in] */ IOutputStream* os)
{
    VALIDATE_NOT_NULL(os);

    Object::Autolock lock(mLock);

    return ByteArrayOutputStream::WriteTo(os);
}

ECode CByteArrayOutputStream::Flush()
{
    return ByteArrayOutputStream::Flush();
}

ECode CByteArrayOutputStream::WriteBytes(
    /* [in] */ const ArrayOf<Byte>& buffer)
{
    Object::Autolock lock(mLock);

    return ByteArrayOutputStream::WriteBytes(buffer);
}

ECode CByteArrayOutputStream::CheckError(
    /* [out] */ Boolean* hasError)
{
    VALIDATE_NOT_NULL(hasError);

    return ByteArrayOutputStream::CheckError(hasError);
}

PInterface CByteArrayOutputStream::Probe(
    /* [in] */ REIID riid)
{
    return _CByteArrayOutputStream::Probe(riid);
}

ECode CByteArrayOutputStream::constructor()
{
    return ByteArrayOutputStream::Init(32);
}

ECode CByteArrayOutputStream::constructor(
    /* [in] */ Int32 size)
{
    return ByteArrayOutputStream::Init(size);
}

ECode CByteArrayOutputStream::GetLock(
    /* [out] */ IInterface** lockobj)
{
    VALIDATE_NOT_NULL(lockobj);

    AutoPtr<IInterface> obj = ByteArrayOutputStream::GetLock();
    *lockobj = obj;
    INTERFACE_ADDREF(*lockobj);
    return NOERROR;
}

} // namespace IO
} // namespace Elastos
