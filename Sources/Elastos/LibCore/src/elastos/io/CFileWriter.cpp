
#include "coredef.h"
#include "CFileWriter.h"
#include "CFileOutputStream.h"

namespace Elastos {
namespace IO {

ECode CFileWriter::constructor(
    /* [in] */ IFile* file)
{
    AutoPtr<IFileOutputStream> fileOutputStream;
    ASSERT_SUCCEEDED(CFileOutputStream::New(file, (IFileOutputStream**)&fileOutputStream));
    if (fileOutputStream && fileOutputStream->Probe(EIID_IOutputStream) != NULL) {
        return Init((IOutputStream*)fileOutputStream->Probe(EIID_IOutputStream));
    }
    else {
        return E_IO_EXCEPTION;
    }
}

ECode CFileWriter::constructor(
    /* [in] */ IFile* file,
    /* [in] */ Boolean append)
{
    AutoPtr<IFileOutputStream> fileOutputStream;
    ASSERT_SUCCEEDED(CFileOutputStream::New(file, append, (IFileOutputStream**)&fileOutputStream));
    if (fileOutputStream && fileOutputStream->Probe(EIID_IOutputStream) != NULL) {
        return Init((IOutputStream*)fileOutputStream->Probe(EIID_IOutputStream));
    }
    else {
        return E_IO_EXCEPTION;
    }
}

ECode CFileWriter::constructor(
    /* [in] */ IFileDescriptor* fd)
{
    AutoPtr<IFileOutputStream> fileOutputStream;
    ASSERT_SUCCEEDED(CFileOutputStream::New(fd, (IFileOutputStream**)&fileOutputStream));
    if (fileOutputStream && fileOutputStream->Probe(EIID_IOutputStream) != NULL) {
        return Init((IOutputStream*)fileOutputStream->Probe(EIID_IOutputStream));
    }
    else {
        return E_IO_EXCEPTION;
    }
}

ECode CFileWriter::constructor(
    /* [in] */ const String& filename)
{
    AutoPtr<IFileOutputStream> fileOutputStream;
    ASSERT_SUCCEEDED(CFileOutputStream::New(filename, (IFileOutputStream**)&fileOutputStream));
    if (fileOutputStream && fileOutputStream->Probe(EIID_IOutputStream) != NULL) {
        return Init((IOutputStream*)fileOutputStream->Probe(EIID_IOutputStream));
    }
    else {
        return E_IO_EXCEPTION;
    }
}

ECode CFileWriter::constructor(
    /* [in] */ const String& filename,
    /* [in] */ Boolean append)
{
    AutoPtr<IFileOutputStream> fileOutputStream;
    ASSERT_SUCCEEDED(CFileOutputStream::New(filename, append, (IFileOutputStream**)&fileOutputStream));
    if (fileOutputStream && fileOutputStream->Probe(EIID_IOutputStream) != NULL) {
        return Init((IOutputStream*)fileOutputStream->Probe(EIID_IOutputStream));
    }
    else {
        return E_IO_EXCEPTION;
    }
}

ECode CFileWriter::Close()
{
    return FileWriter::Close();
}

ECode CFileWriter::Flush()
{
    return FileWriter::Flush();
}

ECode CFileWriter::Write(
    /* [in] */ Int32 oneChar32)
{
    return FileWriter::Write(oneChar32);
}

ECode CFileWriter::WriteChars(
    /* [in] */ const ArrayOf<Char32>& buffer)
{
    return FileWriter::WriteChars(buffer);
}

ECode CFileWriter::WriteChars(
    /* [in] */ const ArrayOf<Char32>& buffer,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count)
{
    return FileWriter::WriteChars(buffer, offset, count);
}

ECode CFileWriter::WriteString(
    /* [in] */ const String& str)
{
    return FileWriter::WriteString(str);
}

ECode CFileWriter::WriteString(
    /* [in] */ const String& str,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count)
{
    return FileWriter::WriteString(str, offset, count);
}

ECode CFileWriter::AppendChar(
    /* [in] */ Char32 c)
{
    return FileWriter::AppendChar(c);
}

ECode CFileWriter::AppendCharSequence(
    /* [in] */ ICharSequence* csq)
{
    return FileWriter::AppendCharSequence(csq);
}

ECode CFileWriter::AppendCharSequence(
    /* [in] */ ICharSequence* csq,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    return FileWriter::AppendCharSequence(csq, start, end);
}

ECode CFileWriter::CheckError(
    /* [out] */ Boolean* hasError)
{
    return FileWriter::CheckError(hasError);
}

ECode CFileWriter::GetEncoding(
    /* [out] */ String* encoding)
{
    return FileWriter::GetEncoding(encoding);
}

ECode CFileWriter::GetLock(
    /* [out] */ IInterface** lockobj)
{
    VALIDATE_NOT_NULL(lockobj);

    AutoPtr<IInterface> obj = FileWriter::GetLock();
    *lockobj = obj;
    REFCOUNT_ADD(*lockobj);
    return NOERROR;
}

} // namespace IO
} // namespace Elastos
