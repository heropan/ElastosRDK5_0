
#include "coredef.h"
#include "CFileReader.h"
#include "CFileInputStream.h"

namespace Elastos {
namespace IO {

ECode CFileReader::constructor(
    /* [in] */ IFile* file)
{
    AutoPtr<IFileInputStream> fileInputstream;
    ASSERT_SUCCEEDED(CFileInputStream::New(file, (IFileInputStream**)&fileInputstream));
    if (fileInputstream && fileInputstream->Probe(EIID_IInputStream) != NULL) {
        return Init((IInputStream*)fileInputstream->Probe(EIID_IInputStream));
    }
    else {
        return E_FILE_NOT_FOUND_EXCEPTION;
    }
}

ECode CFileReader::constructor(
    /* [in] */ IFileDescriptor* fd)
{
    AutoPtr<IFileInputStream> fileInputstream;
    ASSERT_SUCCEEDED(CFileInputStream::New(fd, (IFileInputStream**)&fileInputstream));
    if (fileInputstream && fileInputstream->Probe(EIID_IInputStream) != NULL) {
        return Init((IInputStream*)fileInputstream->Probe(EIID_IInputStream));
    }
    else {
        return E_FILE_NOT_FOUND_EXCEPTION;
    }
}

ECode CFileReader::constructor(
    /* [in] */ const String& filename)
{
    AutoPtr<IFileInputStream> fileInputstream;
    ASSERT_SUCCEEDED(CFileInputStream::New(filename, (IFileInputStream**)&fileInputstream));
    if (fileInputstream && fileInputstream->Probe(EIID_IInputStream) != NULL) {
        return Init((IInputStream*)fileInputstream->Probe(EIID_IInputStream));
    }
    else {
        return E_FILE_NOT_FOUND_EXCEPTION;
    }
}

ECode CFileReader::Close()
{
    return FileReader::Close();
}

ECode CFileReader::Mark(
    /* [in] */ Int32 readLimit)
{
    return FileReader::Mark(readLimit);
}

ECode CFileReader::IsMarkSupported(
    /* [out] */ Boolean* supported)
{
    return FileReader::IsMarkSupported(supported);
}

ECode CFileReader::Read(
    /* [out] */ Int32* value)
{
    return FileReader::Read(value);
}

ECode CFileReader::ReadChars(
    /* [out] */ ArrayOf<Char32>* buffer,
    /* [out] */ Int32* number)
{
    return FileReader::ReadChars(buffer, number);
}

ECode CFileReader::ReadChars(
    /* [out] */ ArrayOf<Char32>* buffer,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count,
    /* [out] */ Int32* number)
{
    return FileReader::ReadCharsEx(buffer, offset, count, number);
}

ECode CFileReader::IsReady(
    /* [out] */ Boolean* ready)
{
    return FileReader::IsReady(ready);
}

ECode CFileReader::Reset()
{
    return FileReader::Reset();
}

ECode CFileReader::Skip(
    /* [in] */ Int64 count,
    /* [out] */ Int64* number)
{
    return FileReader::Skip(count, number);
}

ECode CFileReader::ReadCharBuffer(
    /* [in] */ ICharBuffer* target,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);

    return FileReader::ReadCharBuffer(target, number);
}

ECode CFileReader::GetEncoding(
    /* [out] */ String* encoding)
{
    return FileReader::GetEncoding(encoding);
}

ECode CFileReader::GetLock(
    /* [out] */ IInterface** lockobj)
{
    VALIDATE_NOT_NULL(lockobj);

    AutoPtr<IInterface> obj = FileReader::GetLock();
    *lockobj = obj;
    INTERFACE_ADDREF(*lockobj);
    return NOERROR;
}

} // namespace IO
} // namespace Elastos
