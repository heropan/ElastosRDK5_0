
#include "CJarOutputStream.h"

using Elastos::Utility::Zip::CZipEntry;

namespace Elastos {
namespace Utility {
namespace Jar {


ECode CJarOutputStream::Close()
{
    return ZipOutputStream::Close();
}

ECode CJarOutputStream::Flush()
{
    return ZipOutputStream::Flush();
}

ECode CJarOutputStream::Write(
    /* [in] */ Int32 oneByte)
{
    return ZipOutputStream::Write(oneByte);
}

ECode CJarOutputStream::WriteBytes(
    /* [in] */ const ArrayOf<Byte>& buffer)
{
    return ZipOutputStream::WriteBytes(buffer);
}

ECode CJarOutputStream::WriteBytes(
    /* [in] */ const ArrayOf<Byte>& buffer,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count)
{
    return ZipOutputStream::WriteBytes(buffer, offset, count);
}

ECode CJarOutputStream::CheckError(
    /* [out] */ Boolean* hasError)
{
    VALIDATE_NOT_NULL(hasError)
    return ZipOutputStream::CheckError(hasError);
}

ECode CJarOutputStream::Finish()
{
    return ZipOutputStream::Finish();
}

ECode CJarOutputStream::CloseEntry()
{
    return ZipOutputStream::CloseEntry();
}

ECode CJarOutputStream::PutNextEntry(
    /* [in] */ IZipEntry* ze)
{
    return ZipOutputStream::PutNextEntry(ze);
}

ECode CJarOutputStream::SetComment(
    /* [in] */ const String& comment)
{
    return ZipOutputStream::SetComment(comment);
}

ECode CJarOutputStream::SetLevel(
    /* [in] */ Int32 level)
{
    return ZipOutputStream::SetLevel(level);
}

ECode CJarOutputStream::SetMethod(
    /* [in] */ Int32 method)
{
    return ZipOutputStream::SetMethod(method);
}

ECode CJarOutputStream::constructor(
    /* [in] */ IOutputStream* os,
    /* [in] */ IManifest* manifest)
{
    FAIL_RETURN(ZipOutputStream::Init(os))
    if (manifest == NULL) {
        return E_NULL_POINTER_EXCEPTION;
    }
    mManifest = manifest;
    AutoPtr<IZipEntry> ze;
    CZipEntry::New(IJarFile::MANIFEST_NAME, (IZipEntry**)&ze);
    FAIL_RETURN(PutNextEntry(ze))
    FAIL_RETURN(mManifest->Write(this))
    CloseEntry();
    return NOERROR;
}

ECode CJarOutputStream::constructor(
    /* [in] */ IOutputStream* os)
{
    return ZipOutputStream::Init(os);
}

ECode CJarOutputStream::GetLock(
    /* [out] */ IInterface** lockobj)
{
    VALIDATE_NOT_NULL(lockobj);

    AutoPtr<IInterface> obj = ZipOutputStream::GetLock();
    *lockobj = obj;
    INTERFACE_ADDREF(*lockobj);
    return NOERROR;
}

PInterface CJarOutputStream::Probe(
        /* in */ REIID id)
{
    return _CJarOutputStream::Probe(id);
}

} // namespace Jar
} // namespace Utility
} // namespace Elastos

