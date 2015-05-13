
#include "cmdef.h"
#include "CFilterOutputStream.h"

namespace Elastos {
namespace IO {

CFilterOutputStream::CFilterOutputStream()
{
}

CFilterOutputStream::~CFilterOutputStream()
{
}

ECode CFilterOutputStream::Close()
{
    return FilterOutputStream::Close();
}

ECode CFilterOutputStream::Flush()
{
    return FilterOutputStream::Flush();
}

ECode CFilterOutputStream::Write(
    /* [in] */ Int32 oneByte)
{
    return FilterOutputStream::Write(oneByte);
}

ECode CFilterOutputStream::WriteBytes(
    /* [in] */ const ArrayOf<Byte>& buffer)
{
    return FilterOutputStream::WriteBytes(buffer);
}

ECode CFilterOutputStream::WriteBytesEx(
    /* [in] */ const ArrayOf<Byte>& buffer,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count)
{
    return FilterOutputStream::WriteBytesEx(buffer, offset, count);
}

ECode CFilterOutputStream::CheckError(
    /* [out] */ Boolean* hasError)
{
    VALIDATE_NOT_NULL(hasError);

    return FilterOutputStream::CheckError(hasError);
}

PInterface CFilterOutputStream::Probe(
    /* [in] */ REIID riid)
{
    return _CFilterOutputStream::Probe(riid);
}

ECode CFilterOutputStream::constructor(
    /* [in] */ IOutputStream* out)
{
    VALIDATE_NOT_NULL(out);

    return FilterOutputStream::Init(out);
}

ECode CFilterOutputStream::GetLock(
    /* [out] */ IInterface** lockobj)
{
    VALIDATE_NOT_NULL(lockobj);

    AutoPtr<IInterface> obj = FilterOutputStream::GetLock();
    *lockobj = obj;
    INTERFACE_ADDREF(*lockobj);
    return NOERROR;
}

} // namespace IO
} // namespace Elastos
