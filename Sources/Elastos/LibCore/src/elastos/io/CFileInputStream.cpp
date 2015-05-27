
#include "coredef.h"
#include "CFileInputStream.h"

namespace Elastos {
namespace IO {

ECode CFileInputStream::Available(
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);

    return FileInputStream::Available(number);
}

ECode CFileInputStream::Close()
{
    return FileInputStream::Close();
}

ECode CFileInputStream::GetChannel(
    /* [out] */ IFileChannel** channel)
{
    VALIDATE_NOT_NULL(channel);

    return FileInputStream::GetChannel(channel);
}

ECode CFileInputStream::GetFD(
    /* [out] */ IFileDescriptor** fd)
{
    VALIDATE_NOT_NULL(fd);

    return FileInputStream::GetFD(fd);
}

ECode CFileInputStream::Mark(
    /* [in] */ Int32 readLimit)
{
    return FileInputStream::Mark(readLimit);
}

ECode CFileInputStream::IsMarkSupported(
    /* [out] */ Boolean* supported)
{
    VALIDATE_NOT_NULL(supported);

    return FileInputStream::IsMarkSupported(supported);
}

ECode CFileInputStream::Read(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);

    return FileInputStream::Read(value);
}

ECode CFileInputStream::ReadBytes(
    /* [out] */ ArrayOf<Byte>* buffer,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(buffer);
    VALIDATE_NOT_NULL(number);

    return FileInputStream::ReadBytes(buffer, number);
}

ECode CFileInputStream::ReadBytes(
    /* [out] */ ArrayOf<Byte>* buffer,
    /* [in] */ Int32 byteOffset,
    /* [in] */ Int32 byteCount,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(buffer);
    VALIDATE_NOT_NULL(number);

    return FileInputStream::ReadBytesEx(buffer, byteOffset, byteCount, number);
}

ECode CFileInputStream::Reset()
{
    return FileInputStream::Reset();
}

ECode CFileInputStream::Skip(
    /* [in] */ Int64 byteCount,
    /* [out] */ Int64* number)
{
    VALIDATE_NOT_NULL(number);

    return FileInputStream::Skip(byteCount, number);
}

ECode CFileInputStream::constructor(
    /* [in] */ IFile* file)
{
    VALIDATE_NOT_NULL(file);

    return FileInputStream::Init(file);
}

ECode CFileInputStream::constructor(
    /* [in] */ IFileDescriptor* fd)
{
    VALIDATE_NOT_NULL(fd);

    return FileInputStream::Init(fd);
}

ECode CFileInputStream::constructor(
    /* [in] */ const String& fileName)
{
    return FileInputStream::Init(fileName);
}

PInterface CFileInputStream::Probe(
    /* [in] */ REIID riid)
{
    return _CFileInputStream::Probe(riid);
}

ECode CFileInputStream::GetLock(
    /* [out] */ IInterface** lockobj)
{
    VALIDATE_NOT_NULL(lockobj);

    AutoPtr<IInterface> obj = FileInputStream::GetLock();
    *lockobj = obj;
    INTERFACE_ADDREF(*lockobj);
    return NOERROR;
}

} // namespace IO
} // namespace Elastos
