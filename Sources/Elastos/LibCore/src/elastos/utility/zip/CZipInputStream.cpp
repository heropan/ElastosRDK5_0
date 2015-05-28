
#include "CZipInputStream.h"

namespace Elastos {
namespace Utility {
namespace Zip {


PInterface CZipInputStream::Probe(
    /* [in] */ REIID riid)
{
    return _CZipInputStream::Probe(riid);
}

ECode CZipInputStream::GetLock(
    /* [out] */ IInterface** lockobj)
{
    VALIDATE_NOT_NULL(lockobj);

    AutoPtr<IInterface> obj = ZipInputStream::GetLock();
    *lockobj = obj;
    INTERFACE_ADDREF(*lockobj);
    return NOERROR;
}

ECode CZipInputStream::Available(
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);
    return ZipInputStream::Available(number);
}

ECode CZipInputStream::Close()
{
    return ZipInputStream::Close();
}

ECode CZipInputStream::Mark(
    /* [in] */ Int32 readLimit)
{
    return ZipInputStream::Mark(readLimit);
}

ECode CZipInputStream::IsMarkSupported(
    /* [out] */ Boolean* supported)
{
    VALIDATE_NOT_NULL(supported);
    return ZipInputStream::IsMarkSupported(supported);
}

ECode CZipInputStream::Read(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    return ZipInputStream::Read(value);
}

ECode CZipInputStream::ReadBytes(
    /* [out] */ ArrayOf<Byte>* buffer,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);
    return ZipInputStream::ReadBytes(buffer, number);
}

ECode CZipInputStream::ReadBytes(
    /* [out] */ ArrayOf<Byte>* buffer,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);
    return ZipInputStream::ReadBytes(buffer, offset, length, number);
}

ECode CZipInputStream::Reset()
{
    return ZipInputStream::Reset();
}

ECode CZipInputStream::Skip(
    /* [in] */ Int64 count,
    /* [out] */ Int64* number)
{
    VALIDATE_NOT_NULL(number);
    return ZipInputStream::Skip(count, number);
}

ECode CZipInputStream::CloseEntry()
{
    return ZipInputStream::CloseEntry();
}

ECode CZipInputStream::GetNextEntry(
    /* [out] */ IZipEntry** entry)
{
    return ZipInputStream::GetNextEntry(entry);
}

ECode CZipInputStream::constructor(
    /* [in] */ IInputStream* stream)
{
    return ZipInputStream::Init(stream);
}

} // namespace Zip
} // namespace Utility
} // namespace Elastos
