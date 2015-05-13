
#include "cmdef.h"
#include "CBufferedOutputStream.h"

namespace Elastos {
namespace IO {

ECode CBufferedOutputStream::Close()
{
    Object::Autolock lock(mLock);

    return BufferedOutputStream::Close();
}

ECode CBufferedOutputStream::Flush()
{
    Object::Autolock lock(mLock);

    return BufferedOutputStream::Flush();
}

ECode CBufferedOutputStream::Write(
    /* [in] */ Int32 oneByte)
{
    Object::Autolock lock(mLock);

    return BufferedOutputStream::Write(oneByte);
}

ECode CBufferedOutputStream::WriteBytes(
    /* [in] */ const ArrayOf<Byte> & buffer)
{
    return BufferedOutputStream::WriteBytes(buffer);
}

ECode CBufferedOutputStream::WriteBytesEx(
    /* [in] */ const ArrayOf<Byte>& buffer,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count)
{
    Object::Autolock lock(mLock);

    return BufferedOutputStream::WriteBytesEx(buffer, offset, count);
}

ECode CBufferedOutputStream::CheckError(
    /* [out] */ Boolean* hasError)
{
    VALIDATE_NOT_NULL(hasError);

    return BufferedOutputStream::CheckError(hasError);
}

PInterface CBufferedOutputStream::Probe(
    /* [in] */ REIID riid)
{
    return _CBufferedOutputStream::Probe(riid);
}

ECode CBufferedOutputStream::constructor(
    /* [in] */ IOutputStream* out)
{
    VALIDATE_NOT_NULL(out);

    return BufferedOutputStream::Init(out, 8192);
}

ECode CBufferedOutputStream::constructor(
    /* [in] */ IOutputStream* out,
    /* [in] */ Int32 size)
{
    VALIDATE_NOT_NULL(out);

    return BufferedOutputStream::Init(out, size);
}

ECode CBufferedOutputStream::GetLock(
    /* [out] */ IInterface** lockobj)
{
    VALIDATE_NOT_NULL(lockobj);

    AutoPtr<IInterface> obj = BufferedOutputStream::GetLock();
    *lockobj = obj;
    INTERFACE_ADDREF(*lockobj);
    return NOERROR;
}

} // namespace IO
} // namespace Elastos
