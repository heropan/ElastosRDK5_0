
#include "CFileReader.h"
#include "CFileInputStream.h"

namespace Elastos {
namespace IO {

CAR_OBJECT_IMPL(CFileReader)

ECode CFileReader::constructor(
    /* [in] */ IFile* file)
{
    AutoPtr<IFileInputStream> fileInputstream;
    ASSERT_SUCCEEDED(CFileInputStream::New(file, (IFileInputStream**)&fileInputstream));
    if (fileInputstream && fileInputstream->Probe(EIID_IInputStream) != NULL) {
        return FileReader::constructor((IInputStream*)fileInputstream->Probe(EIID_IInputStream));
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
        return FileReader::constructor((IInputStream*)fileInputstream->Probe(EIID_IInputStream));
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
        return FileReader::constructor((IInputStream*)fileInputstream->Probe(EIID_IInputStream));
    }
    else {
        return E_FILE_NOT_FOUND_EXCEPTION;
    }
}

} // namespace IO
} // namespace Elastos
