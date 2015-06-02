
#include "CZipEntry.h"

namespace Elastos {
namespace Utility {
namespace Zip {

CAR_OBJECT_IMPL(CZipEntry)

ECode CZipEntry::GetComment(
    /* [out] */ String* comment)
{
    VALIDATE_NOT_NULL(comment);
    *comment = ZipEntry::GetComment();
    return NOERROR;
}

ECode CZipEntry::GetCompressedSize(
    /* [out] */ Int64* size)
{
    VALIDATE_NOT_NULL(size);
    *size = ZipEntry::GetCompressedSize();
    return NOERROR;
}

ECode CZipEntry::GetCrc(
    /* [out] */ Int64* checksum)
{
    VALIDATE_NOT_NULL(checksum);
    *checksum = ZipEntry::GetCrc();
    return NOERROR;
}

ECode CZipEntry::GetExtra(
    /* [out, callee] */ ArrayOf<Byte>** extra)
{
    VALIDATE_NOT_NULL(extra);
    AutoPtr<ArrayOf<Byte> > temp = ZipEntry::GetExtra();
    *extra = temp;
    REFCOUNT_ADD(*extra)
    return NOERROR;
}

ECode CZipEntry::GetMethod(
    /* [out] */ Int32* method)
{
    VALIDATE_NOT_NULL(method);
    *method = ZipEntry::GetMethod();
    return NOERROR;
}

ECode CZipEntry::GetName(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);
    *name = ZipEntry::GetName();
    return NOERROR;
}

ECode CZipEntry::GetSize(
    /* [out] */ Int64* size)
{
    VALIDATE_NOT_NULL(size);
    *size = ZipEntry::GetSize();
    return NOERROR;
}

ECode CZipEntry::GetTime(
    /* [out] */ Int64* time)
{
    VALIDATE_NOT_NULL(time);
    *time = ZipEntry::GetTime();
    return NOERROR;
}

ECode CZipEntry::IsDirectory(
    /* [out] */ Boolean* isDirectory)
{
    VALIDATE_NOT_NULL(isDirectory);
    *isDirectory = ZipEntry::IsDirectory();
    return NOERROR;
}

} // namespace Zip
} // namespace Utility
} // namespace Elastos
