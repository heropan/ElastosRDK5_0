
#include "CZipOutputStream.h"


namespace Elastos {
namespace Utility {
namespace Zip {

ECode CZipOutputStream::Close()
{
    return ZipOutputStream::Close();
}

ECode CZipOutputStream::Flush()
{
    return ZipOutputStream::Flush();
}

ECode CZipOutputStream::Write(
    /* [in] */ Int32 oneByte)
{
    return ZipOutputStream::Write(oneByte);
}

ECode CZipOutputStream::WriteBytes(
    /* [in] */ const ArrayOf<Byte>& buffer)
{
    return ZipOutputStream::WriteBytes(buffer);
}

ECode CZipOutputStream::WriteBytesEx(
    /* [in] */ const ArrayOf<Byte>& buffer,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count)
{
    return ZipOutputStream::WriteBytesEx(buffer, offset, count);
}

ECode CZipOutputStream::CheckError(
    /* [out] */ Boolean* hasError)
{
    VALIDATE_NOT_NULL(hasError);
    return ZipOutputStream::CheckError(hasError);
}

ECode CZipOutputStream::Finish()
{
    return ZipOutputStream::Finish();
}

ECode CZipOutputStream::CloseEntry()
{
    return ZipOutputStream::CloseEntry();
}

ECode CZipOutputStream::PutNextEntry(
    /* [in] */ IZipEntry* ze)
{
    return ZipOutputStream::PutNextEntry(ze);
}

ECode CZipOutputStream::SetComment(
    /* [in] */ const String& comment)
{
    return ZipOutputStream::SetComment(comment);
}

ECode CZipOutputStream::SetLevel(
    /* [in] */ Int32 level)
{
    return ZipOutputStream::SetLevel(level);
}

ECode CZipOutputStream::SetMethod(
    /* [in] */ Int32 method)
{
    return ZipOutputStream::SetMethod(method);
}

ECode CZipOutputStream::constructor(
    /* [in] */ IOutputStream* os)
{
    return ZipOutputStream::Init(os);
}

ECode CZipOutputStream::GetLock(
    /* [out] */ IInterface** lockobj)
{
    VALIDATE_NOT_NULL(lockobj);

    AutoPtr<IInterface> obj = ZipOutputStream::GetLock();
    *lockobj = obj;
    INTERFACE_ADDREF(*lockobj);
    return NOERROR;
}

PInterface CZipOutputStream::Probe(
    /* [in] */ REIID riid)
{
    return _CZipOutputStream::Probe(riid);
}

} // namespace Zip
} // namespace Utility
} // namespace Elastos
