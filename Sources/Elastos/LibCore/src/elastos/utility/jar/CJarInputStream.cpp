
#include "CJarInputStream.h"
#include "CJarFile.h"
#include "CByteArrayOutputStream.h"
#include "CManifest.h"
#include "CAttributes.h"
#include "CStringWrapper.h"
#include "CJarEntry.h"
#include "JarVerifier.h"

using Elastos::IO::CByteArrayOutputStream;
using Elastos::Core::CStringWrapper;

namespace Elastos {
namespace Utility {
namespace Jar {

CJarInputStream::CJarInputStream()
    : mEos(FALSE), mIsMeta(FALSE)
{}

ECode CJarInputStream::Close()
{
    return ZipInputStream::Close();
}

ECode CJarInputStream::Available(
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number)
    return ZipInputStream::Available(number);
}

ECode CJarInputStream::Mark(
    /* [in] */ Int32 readLimit)
{
    return ZipInputStream::Mark(readLimit);
}

ECode CJarInputStream::IsMarkSupported(
    /* [out] */ Boolean* supported)
{
    VALIDATE_NOT_NULL(supported)
    return ZipInputStream::IsMarkSupported(supported);
}

ECode CJarInputStream::Read(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value)
    return ZipInputStream::Read(value);
}

ECode CJarInputStream::ReadBytes(
    /* [out] */ ArrayOf<Byte>* buffer,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(buffer)
    VALIDATE_NOT_NULL(number)
    return ZipInputStream::ReadBytes(buffer, number);
}

ECode CJarInputStream::ReadBytes(
    /* [out] */ ArrayOf<Byte>* buffer,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(buffer)
    VALIDATE_NOT_NULL(number)
    return ZipInputStream::ReadBytes(buffer, offset, length, number);
}

ECode CJarInputStream::Reset()
{
    return ZipInputStream::Reset();
}

ECode CJarInputStream::Skip(
    /* [in] */ Int64 byteCount,
    /* [out] */ Int64* number)
{
    VALIDATE_NOT_NULL(number)
    return ZipInputStream::Skip(byteCount, number);
}

ECode CJarInputStream::CloseEntry()
{
    return ZipInputStream::CloseEntry();
}

ECode CJarInputStream::GetNextEntry(
    /* [out] */ IZipEntry** entry)
{
    VALIDATE_NOT_NULL(entry)
    if (mEntry != NULL) {
        mJarEntry = mEntry;
        mEntry = NULL;
        ((CJarEntry*)mJarEntry.Get())->SetAttributes(NULL);
    } else {
        mJarEntry = NULL;
        ZipInputStream::GetNextEntry((IZipEntry**)&mJarEntry);
        if (!mJarEntry) {
            *entry = NULL;
            return NOERROR;
        }
        if (mVerifier != NULL) {
            String name;
            mJarEntry->GetName(&name);
            name = name.ToUpperCase();
            mIsMeta = name.StartWith(CJarFile::META_DIR);
            if (mIsMeta) {
                mVerStream = NULL;
                FAIL_RETURN(CByteArrayOutputStream::New((IOutputStream**)&mVerStream))
            } else {
                AutoPtr<JarVerifier::VerifierEntry> ve;
                FAIL_RETURN(mVerifier->InitEntry(name, (JarVerifier::VerifierEntry**)&ve))
                mVerStream = ve;
            }
        }
    }
    mEos = FALSE;
    *entry = mJarEntry;
    INTERFACE_ADDREF(*entry)
    return NOERROR;
}

ECode CJarInputStream::GetManifest(
    /* [out] */ IManifest** manifest)
{
    VALIDATE_NOT_NULL(manifest)
    *manifest = mManifest;
    INTERFACE_ADDREF(*manifest)
    return NOERROR;
}

ECode CJarInputStream::GetNextJarEntry(
    /* [out] */ IJarEntry** jarEntry)
{
    VALIDATE_NOT_NULL(jarEntry)
    return GetNextEntry((IZipEntry**)jarEntry);
}

ECode CJarInputStream::constructor(
    /* [in] */ IInputStream* stream,
    /* [in] */ Boolean verify)
{
    FAIL_RETURN(ZipInputStream::Init(stream))
    if (verify) {
        mVerifier = new JarVerifier(String("JarInputStream"));
    }
    mEntry = NULL;
    if ((GetNextJarEntry((IJarEntry**)&mEntry), mEntry) == NULL) {
        return NOERROR;
    }
    String name;
    if ((mEntry->GetName(&name), name).EqualsIgnoreCase(CJarFile::META_DIR)) {
        mEntry = NULL;
        CloseEntry();
        mEntry = NULL;
        GetNextJarEntry((IJarEntry**)&mEntry);
    }
    if ((mEntry->GetName(&name), name).EqualsIgnoreCase(IJarFile::MANIFEST_NAME)) {
        mEntry = NULL;
        mManifest = CManifest::Create(this, verify);
        FAIL_RETURN(CloseEntry())
        if (verify) {
            mVerifier->SetManifest(mManifest);
            if (mManifest != NULL) {
                ((CManifest*)mManifest.Get())->GetMainAttributesEnd(&mVerifier->mMainAttributesEnd);
            }
        }
    } else {
        AutoPtr<IAttributes> temp;
        CAttributes::New(3, (IAttributes**)&temp);
        AutoPtr<ICharSequence> cs;
        CStringWrapper::New(String("hidden"), (ICharSequence**)&cs);
        AutoPtr<IInterface> outface;
        ((CAttributes*)temp.Get())->mMap->Put(cs, NULL, (IInterface**)&outface);
        ((CJarEntry*)mEntry.Get())->SetAttributes(temp);
        /*
         * if not from the first entry, we will not get enough
         * information,so no verify will be taken out.
         */
         mVerifier = NULL;
    }
    return NOERROR;
}

ECode CJarInputStream::constructor(
    /* [in] */ IInputStream* stream)
{
    return constructor(stream, TRUE);
}

PInterface CJarInputStream::Probe(
        /* in */ REIID id)
{
    return _CJarInputStream::Probe(id);
}

ECode CJarInputStream::GetLock(
    /* [out] */ IInterface** lockobj)
{
    VALIDATE_NOT_NULL(lockobj);

    AutoPtr<IInterface> obj = ZipInputStream::GetLock();
    *lockobj = obj;
    INTERFACE_ADDREF(*lockobj);
    return NOERROR;
}

} // namespace Jar
} // namespace Utility
} // namespace Elastos
