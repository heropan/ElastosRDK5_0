
#include "CFileWriter.h"
#include "CFileOutputStream.h"

namespace Elastos {
namespace IO {

CAR_OBJECT_IMPL(CFileWriter)

ECode CFileWriter::constructor(
    /* [in] */ IFile* file)
{
    AutoPtr<IFileOutputStream> fileOutputStream;
    ASSERT_SUCCEEDED(CFileOutputStream::New(file, (IFileOutputStream**)&fileOutputStream));
    if (fileOutputStream && fileOutputStream->Probe(EIID_IOutputStream) != NULL) {
        return FileWriter::constructor((IOutputStream*)fileOutputStream->Probe(EIID_IOutputStream));
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
        return FileWriter::constructor((IOutputStream*)fileOutputStream->Probe(EIID_IOutputStream));
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
        return FileWriter::constructor((IOutputStream*)fileOutputStream->Probe(EIID_IOutputStream));
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
        return FileWriter::constructor((IOutputStream*)fileOutputStream->Probe(EIID_IOutputStream));
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
        return FileWriter::constructor((IOutputStream*)fileOutputStream->Probe(EIID_IOutputStream));
    }
    else {
        return E_IO_EXCEPTION;
    }
}

} // namespace IO
} // namespace Elastos
