
#include "coredef.h"
#include "CFileOutputStream.h"

namespace Elastos {
namespace IO {

CFileOutputStream::~CFileOutputStream()
{}

ECode CFileOutputStream::Close()
{
    return FileOutputStream::Close();
}

ECode CFileOutputStream::GetChannel(
    /* [out] */ IFileChannel** channel)
{
    VALIDATE_NOT_NULL(channel);

    return FileOutputStream::GetChannel(channel);
}

ECode CFileOutputStream::GetFD(
    /* [out] */ IFileDescriptor** fd)
{
    VALIDATE_NOT_NULL(fd);

    return FileOutputStream::GetFD(fd);
}

ECode CFileOutputStream::Flush()
{
    return FileOutputStream::Flush();
}

ECode CFileOutputStream::Write(
    /* [in] */ Int32 oneByte)
{
    return FileOutputStream::Write(oneByte);
}

ECode CFileOutputStream::WriteBytes(
    /* [in] */ const ArrayOf<Byte>& buffer)
{
    return FileOutputStream::WriteBytes(buffer);
}

ECode CFileOutputStream::WriteBytes(
    /* [in] */ const ArrayOf<Byte>& buffer,
    /* [in] */ Int32 byteOffset,
    /* [in] */ Int32 byteCount)
{
    return FileOutputStream::WriteBytesEx(buffer, byteOffset, byteCount);
}

ECode CFileOutputStream::CheckError(
    /* [out] */ Boolean* hasError)
{
    VALIDATE_NOT_NULL(hasError);

    return FileOutputStream::CheckError(hasError);
}

PInterface CFileOutputStream::Probe(
    /* [in] */ REIID riid)
{
    return _CFileOutputStream::Probe(riid);
}

ECode CFileOutputStream::constructor(
    /* [in] */ IFile* file)
{
    return FileOutputStream::Init(file);
}

ECode CFileOutputStream::constructor(
    /* [in] */ IFile* file,
    /* [in] */ Boolean append)
{
    return FileOutputStream::Init(file, append);
}

ECode CFileOutputStream::constructor(
    /* [in] */ IFileDescriptor* fd)
{
    return FileOutputStream::Init(fd);
}

ECode CFileOutputStream::constructor(
    /* [in] */ const String& fileName)
{
    return FileOutputStream::Init(fileName);
}

ECode CFileOutputStream::constructor(
    /* [in] */ const String& fileName,
    /* [in] */ Boolean append)
{
    return FileOutputStream::Init(fileName, append);
}

ECode CFileOutputStream::GetLock(
    /* [out] */ IInterface** lockobj)
{
    VALIDATE_NOT_NULL(lockobj);

    AutoPtr<IInterface> obj = FileOutputStream::GetLock();
    *lockobj = obj;
    INTERFACE_ADDREF(*lockobj);
    return NOERROR;
}

} // namespace IO
} // namespace Elastos
