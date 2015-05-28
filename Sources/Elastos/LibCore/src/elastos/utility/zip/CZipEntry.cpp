
#include "CZipEntry.h"

namespace Elastos {
namespace Utility {
namespace Zip {


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
    INTERFACE_ADDREF(*extra)
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

ECode CZipEntry::SetComment(
    /* [in] */ const String& comment)
{
    return ZipEntry::SetComment(comment);
}

ECode CZipEntry::SetCompressedSize(
    /* [in] */ Int64 size)
{
    return ZipEntry::SetCompressedSize(size);
}

ECode CZipEntry::SetCrc(
    /* [in] */ Int64 checksum)
{
    return ZipEntry::SetCrc(checksum);
}

ECode CZipEntry::SetExtra(
    /* [in] */ ArrayOf<Byte>* data)
{
    return ZipEntry::SetExtra(data);
}

ECode CZipEntry::SetMethod(
    /* [in] */ Int32 method)
{
    return ZipEntry::SetMethod(method);
}

ECode CZipEntry::SetSize(
    /* [in] */ Int64 size)
{
    return ZipEntry::SetSize(size);
}

ECode CZipEntry::SetTime(
    /* [in] */ Int64 time)
{
    return ZipEntry::SetTime(time);
}

ECode CZipEntry::constructor(
    /* [in] */ const String& name)
{
    return ZipEntry::Init(name);
}

ECode CZipEntry::constructor(
    /* [in] */ IZipEntry* ze)
{
    return ZipEntry::Init((CZipEntry*)ze);
}

ECode CZipEntry::constructor(
    /* in */ const ArrayOf<Byte>& hdrBuf,
    /* in */ IInputStream* in)
{
    return ZipEntry::Init(hdrBuf, in);
}

PInterface CZipEntry::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_ZipEntry) {
        return reinterpret_cast<PInterface>((ZipEntry*)this);
    }

    return _CZipEntry::Probe(riid);
}

} // namespace Zip
} // namespace Utility
} // namespace Elastos
