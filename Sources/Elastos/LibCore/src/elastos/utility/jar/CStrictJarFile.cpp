
#include "CStrictJarFile.h"
#include "CCloseGuardHelper.h"
#include "CRandomAccessFile.h"
#include "CManifest.h"
#include "CArrayOf.h"
#include "CJarFile.h"
#include "IoUtils.h"
#include "CZipFile.h"
#include "CInflater.h"
#include "Math.h"
#include "CZipEntry.h"
#include "CStreams.h"

using Elastos::Core::CArrayOf;
using Elastos::Core::ICloseGuardHelper;
using Elastos::Core::CCloseGuardHelper;
using Elastos::IO::CRandomAccessFile;
using Elastos::IO::ICloseable;
using Libcore::IO::IoUtils;
using Libcore::IO::IStreams;
using Libcore::IO::CStreams;
using Elastos::Utility::Zip::CZipFile;
using Elastos::Utility::Zip::CZipEntry;
using Elastos::Utility::Zip::IInflater;
using Elastos::Utility::Zip::CInflater;

namespace Elastos {
namespace Utility {
namespace Jar {

//====================================================================
// CStrictJarFile::EntryIterator
//====================================================================
CAR_INTERFACE_IMPL(CStrictJarFile::EntryIterator, Object, IIterator)

CStrictJarFile::EntryIterator::EntryIterator(
    /* [in] */ Int64 nativeHandle,
    /* [in] */ const String& prefix)
{
    CStrictJarFile::NativeStartIteration(nativeHandle, prefix, &mIterationHandle);
}

ECode CStrictJarFile::EntryIterator::GetNext(
    /* [out] */ IInterface** obj)
{
    VALIDATE_NOT_NULL(obj)

    if (mNextEntry != NULL) {
        *obj = mNextEntry.Get();
        REFCOUNT_ADD(*obj)
        mNextEntry = NULL;
        return NOERROR;
    }

    AutoPtr<IZipEntry> ze;
    FAIL_RETURN(CStrictJarFile::NativeNextEntry(mIterationHandle, (IZipEntry**)&ze))
    *obj = ze.Get();
    REFCOUNT_ADD(*obj)
    return NOERROR;
}

ECode CStrictJarFile::EntryIterator::HasNext(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    *result = FALSE;

    if (mNextEntry != NULL) {
        *result = TRUE;
        return NOERROR;
    }

    AutoPtr<IZipEntry> ze;
    FAIL_RETURN(CStrictJarFile::NativeNextEntry(mIterationHandle, (IZipEntry**)&ze))
    if (ze == NULL) {
        *result = FALSE;
        return NOERROR;
    }

    mNextEntry = ze;
    *result = TRUE;
    return NOERROR;
}

ECode CStrictJarFile::EntryIterator::Remove()
{
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

//====================================================================
// CStrictJarFile
//====================================================================
CAR_INTERFACE_IMPL(CStrictJarFile, Object, IStrictJarFile)

CAR_OBJECT_IMPL(CStrictJarFile)

CStrictJarFile::CStrictJarFile()
    : mNativeHandle(0)
    , mIsSigned(FALSE)
    , mClosed(FALSE)
{}

ECode CStrictJarFile::constructor(
    /* [in] */ const String& fileName)
{
    AutoPtr<ICloseGuardHelper> helper;
    CCloseGuardHelper::AcquireSingleton((ICloseGuardHelper**)&helper);
    helper->Get((ICloseGuard**)&mGuard);

    FAIL_RETURN(NativeOpenJarFile(fileName, &mNativeHandle))
    CRandomAccessFile::New(fileName, String("r"), (IRandomAccessFile**)&mRaf);

    ECode ec = NOERROR;
    // try {
    // Read the MANIFEST and signature files up front and try to
    // parse them. We never want to accept a JAR File with broken signatures
    // or manifests, so it's best to throw as early as possible.
    AutoPtr<HashMap<String, AutoPtr<ArrayOf<Byte> > > > metaEntries;
    metaEntries = GetMetaEntries();
    HashMap<String, AutoPtr<ArrayOf<Byte> > >::Iterator it;
    it = metaEntries->Find(IJarFile::MANIFEST_NAME);
    AutoPtr<ArrayOf<Byte> > bytes;
    if (it != metaEntries->End()) {
        bytes = it->mSecond;
    }

    CManifest::New((IManifest**)&mManifest);
    FAIL_GOTO(((CManifest*)mManifest.Get())->constructor(bytes, TRUE), _EXIT_)
    mVerifier = new JarVerifier(fileName, mManifest, metaEntries);

    FAIL_GOTO(mVerifier->ReadCertificates(&mIsSigned), _EXIT_)
    if (mIsSigned)
        FAIL_GOTO(mVerifier->IsSignedJar(&mIsSigned), _EXIT_)
    // } catch (IOException ioe) {
_EXIT_:
    NativeClose(mNativeHandle);
    if (FAILED(ec)) {
        return ec;
    }
    // }

    mGuard->Open(String("close"));
    return NOERROR;
}

ECode CStrictJarFile::GetManifest(
    /* [out] */ IManifest** manifest)
{
    VALIDATE_NOT_NULL(manifest)
    *manifest = mManifest;
    REFCOUNT_ADD(*manifest)
    return NOERROR;
}

ECode CStrictJarFile::GetIterator(
    /* [out] */ IIterator** it)
{
    VALIDATE_NOT_NULL(it)
    AutoPtr<EntryIterator> tmp = new EntryIterator(mNativeHandle, String(""));
    *it = tmp;
    REFCOUNT_ADD(*it);
    return NOERROR;
}

ECode CStrictJarFile::FindEntry(
    /* [in] */ const String& name,
    /* [out] */ IZipEntry** ze)
{
    return NativeFindEntry(mNativeHandle, name, ze);
}

ECode CStrictJarFile::GetCertificateChains(
    /* [in] */ IZipEntry* ze,
    /* [out, callee] */ ArrayOf<IArrayOf *> ** certificate)
{
    VALIDATE_NOT_NULL(certificate)
    *certificate = NULL;
    VALIDATE_NOT_NULL(ze)

    if (mIsSigned) {
        String name;
        ze->GetName(&name);
        AutoPtr<ArrayOf<AutoPtr<ArrayOf<ICertificate*> > > > arr;
        arr = mVerifier->GetCertificateChains(name);

        if (arr) {
            AutoPtr<ArrayOf<IArrayOf *> > certs = ArrayOf<IArrayOf *>::Alloc(arr->GetLength());
            for (Int32 i = 0; i < arr->GetLength(); ++i) {
                ArrayOf<ICertificate*> * subArr = (*arr)[i];
                AutoPtr<IArrayOf> ao;
                CArrayOf::New(subArr->GetLength(), (IArrayOf**)&ao);
                for (Int32 j = 0; j < subArr->GetLength(); ++j) {
                    ao->Set(j, TO_IINTERFACE((*subArr)[j]));
                }
                certs->Set(i, ao);
            }

            *certificate = certs;
            REFCOUNT_ADD(*certificate)
        }
    }
    return NOERROR;
}

ECode CStrictJarFile::GetCertificates(
    /* [in] */ IZipEntry* ze,
    /* [out, callee] */ ArrayOf<ICertificate *> ** certificate)
{
    VALIDATE_NOT_NULL(certificate)
    *certificate = NULL;

    if (mIsSigned) {
        String name;
        ze->GetName(&name);
        AutoPtr<ArrayOf<AutoPtr<ArrayOf<ICertificate*> > > > certChains;
        certChains = mVerifier->GetCertificateChains(name);

        // Measure number of certs.
        Int32 count = 0;
        for (Int32 i = 0; i < certChains->GetLength(); ++i) {
            count += (*certChains)[i]->GetLength();
        }

        // Create new array and copy all the certs into it.
        AutoPtr<ArrayOf<ICertificate*> > certs = ArrayOf<ICertificate*>::Alloc(count);
        count = 0;
        for (Int32 i = 0; i < certChains->GetLength(); ++i) {
            certs->Copy(count, (*certChains)[i], 0, (*certChains)[i]->GetLength());
            count += (*certChains)[i]->GetLength();
        }

        *certificate = certs;
        REFCOUNT_ADD(*certificate)
    }

    return NOERROR;
}

ECode CStrictJarFile::GetInputStream(
    /* [in] */ IZipEntry* ze,
    /* [out] */ IInputStream** result)
{
    VALIDATE_NOT_NULL(result)
    *result = NULL;

    AutoPtr<IInputStream> is;
    GetZipInputStream(ze, (IInputStream**)&is);

    if (mIsSigned) {
        VALIDATE_NOT_NULL(ze)

        AutoPtr<JarVerifier::VerifierEntry> entry;
        String name;
        ze->GetName(&name);
         mVerifier->InitEntry(name, (JarVerifier::VerifierEntry**)&entry);
        if (entry == NULL) {
            *result = is;
            REFCOUNT_ADD(*result)
            return NOERROR;
        }

        Int64 size;
        ze->GetSize(&size);
        AutoPtr<CJarFile::JarFileInputStream> jfis = new CJarFile::JarFileInputStream();
        jfis->constructor(is, size, entry);
        *result = (IInputStream*)jfis.Get();
        REFCOUNT_ADD(*result)
        return NOERROR;
    }

    *result = is;
    REFCOUNT_ADD(*result)
    return NOERROR;
}

ECode CStrictJarFile::Close()
{
    if (!mClosed) {
        mGuard->Close();

        NativeClose(mNativeHandle);
        IoUtils::CloseQuietly(ICloseable::Probe(mRaf));
        mClosed = TRUE;
    }
    return NOERROR;
}

ECode CStrictJarFile::GetZipInputStream(
    /* [in] */ IZipEntry* ze,
    /* [out] */ IInputStream** is)
{
    VALIDATE_NOT_NULL(is)
    *is = NULL;

    Int32 method;
    ze->GetMethod(&method);
    Int64 offset, size;
    ((CZipEntry*)ze)->GetDataOffset(&offset);
    ze->GetSize(&size);

    if (method == IZipEntry::STORED) {
        AutoPtr<CZipFile::RAFStream> wrapped = new CZipFile::RAFStream(mRaf, offset, offset + size);
        *is = (IInputStream*)wrapped.Get();
        REFCOUNT_ADD(*is)
        return NOERROR;
    }
    else {
        Int64 csize;
        ze->GetCompressedSize(&csize);
        AutoPtr<CZipFile::RAFStream> wrapped = new CZipFile::RAFStream(
            mRaf, offset, offset + csize);

        using Elastos::Core::Math;
        Int32 bufSize = Math::Max(1024, (Int32) Math::Min(size, 65535L));

        AutoPtr<IInflater> inf;
        CInflater::New(TRUE, (IInflater**)&inf);

        AutoPtr<CZipFile::ZipInflaterInputStream> zis;
        zis = new CZipFile::ZipInflaterInputStream(wrapped, inf, bufSize, ze);
         *is = (IInputStream*)zis.Get();
        REFCOUNT_ADD(*is)
        return NOERROR;
    }
    return NOERROR;
}

AutoPtr<HashMap<String, AutoPtr<ArrayOf<Byte> > > > CStrictJarFile::GetMetaEntries()
{
    AutoPtr<HashMap<String, AutoPtr<ArrayOf<Byte> > > > metaEntries;
    metaEntries = new HashMap<String, AutoPtr<ArrayOf<Byte> > >();

    AutoPtr<IStreams> streams;
    CStreams::AcquireSingleton((IStreams**)&streams);

    AutoPtr<IIterator> entryIterator = new EntryIterator(mNativeHandle, String("META-INF/"));
    Boolean hasNext;
    while (entryIterator->HasNext(&hasNext), hasNext) {
        AutoPtr<IInterface> obj;
        entryIterator->GetNext((IInterface**)&obj);
        AutoPtr<IZipEntry> entry = IZipEntry::Probe(obj);
        String name;
        entry->GetName(&name);

        AutoPtr<IInputStream> is;
        GetInputStream(entry, (IInputStream**)&is);
        AutoPtr<ArrayOf<Byte> > bytes;
        ECode ec = streams->ReadFully(is, (ArrayOf<Byte>**)&bytes);
        if (FAILED(ec)) {
            ALOGE("CStrictJarFile::ReadMetaEntries failed!");
            break;
        }
        (*metaEntries)[name] = bytes;
    }

    return metaEntries;
}

ECode CStrictJarFile::NativeOpenJarFile(
    /* [in] */ const String& fileName,
    /* [out] */ Int64* result)
{
    return NOERROR;
}

ECode CStrictJarFile::NativeStartIteration(
    /* [in] */ Int64 nativeHandle,
    /* [in] */ const String& prefix,
    /* [out] */ Int64* result)
{
    return NOERROR;
}

ECode CStrictJarFile::NativeNextEntry(
    /* [in] */ Int64 mIterationHandle,
    /* [out] */ IZipEntry** ze)
{
    return NOERROR;
}

ECode CStrictJarFile::NativeFindEntry(
    /* [in] */ Int64 nativeHandle,
    /* [in] */ const String& entryName,
    /* [out] */ IZipEntry** ze)
{
    return NOERROR;
}

ECode CStrictJarFile::NativeClose(
    /* [in] */ Int64 nativeHandle)
{
    return NOERROR;
}

} // namespace Jar
} // namespace Utility
} // namespace Elastos
