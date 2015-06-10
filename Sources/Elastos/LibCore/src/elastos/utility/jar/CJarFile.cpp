#include "elastos/List.h"
#include "CJarEntry.h"
#include "CJarFile.h"
#include "CManifest.h"
#include "CStreams.h"
#include <cutils/log.h>

using Elastos::Utility::List;
using Elastos::IO::FilterInputStream;
using Elastos::IO::IStreams;
using Elastos::IO::CStreams;
using Elastos::IO::EIID_IInputStream;

namespace Elastos {
namespace Utility {
namespace Jar {

const String CJarFile::META_DIR("META-INF/");

CJarFile::JarFileInputStream::JarFileInputStream(
    /* [in] */ IInputStream* is,
    /* [in] */ IZipEntry* ze,
    /* [in] */ JarVerifier::VerifierEntry* e)
    : mDone(FALSE)
{
    FilterInputStream::constructor(is);
    mZipEntry = ze;
    mZipEntry->GetSize(&mCount);
    mEntry = e;
}

ECode CJarFile::JarFileInputStream::Read(
    /* [out] */ Int32* val)
{
    VALIDATE_NOT_NULL(val)
    if (mDone) {
        *val = -1;
        return NOERROR;
    }
    if (mCount > 0) {
        FilterInputStream::Read(val);
        if (*val != -1) {
            FAIL_RETURN(mEntry->Write(val))
            mCount--;
        } else {
            mCount = 0;
        }
        if (mCount == 0) {
            mDone = TRUE;
            FAIL_RETURN(mEntry->Verify())
        }
        return NOERROR;
    } else {
        mDone = TRUE;
        FAIL_RETURN(mEntry->Verify())
        *val = -1;
        return NOERROR;
    }
}

ECode CJarFile::JarFileInputStream::Read(
    /* [in] */ ArrayOf<Byte>* buf,
    /* [in] */ Int32 off,
    /* [in] */ Int32 nbytes,
    /* [out] */ Int32* val)
{
    VALIDATE_NOT_NULL(val)
    if (mDone) {
        *val = -1;
        return NOERROR;
    }
    if (mCount > 0) {
        FAIL_RETURN(FilterInputStream::Read(buf, off, nbytes, val))
        if (*val != -1) {
            Int32 size = *val;
            if (mCount < size) {
                size = (Int32) mCount;
            }
            FAIL_RETURN(mEntry->Write(*buf, off, size))
            mCount -= size;
        } else {
            mCount = 0;
        }
        if (mCount == 0) {
            mDone = TRUE;
            FAIL_RETURN(mEntry->Verify())
        }
        return NOERROR;
    } else {
        mDone = TRUE;
        FAIL_RETURN(mEntry->Verify())
        *val = -1;
        return NOERROR;
    }
}

ECode CJarFile::JarFileInputStream::Available(
    /* [out] */ Int32* val)
{
    VALIDATE_NOT_NULL(val)
    if (mDone) {
        *val = 0;
        return NOERROR;
    }
    return FilterInputStream::Available(val);
}

ECode CJarFile::JarFileInputStream::Skip(
    /* [in] */ Int64 byteCount,
    /* [out] */ Int64* val)
{
    VALIDATE_NOT_NULL(val)
    AutoPtr<IStreams> stream;
    FAIL_RETURN(CStreams::AcquireSingleton((IStreams**)&stream))
    return stream->SkipByReading((IInputStream*)IInputStream::Probe(this), byteCount, val);
}

CAR_INTERFACE_IMPL(CJarFile, ZipFile, IJarFile)

CAR_OBJECT_IMPL(CJarFile)

CJarFile::CJarFile()
    : mClosed(FALSE)
{}

ECode CJarFile::Close()
{
    FAIL_RETURN(ZipFile::Close())
    mClosed = TRUE;
    return NOERROR;
}

ECode CJarFile::GetEntry(
    /* [in] */ const String& entryName,
    /* [out] */ IZipEntry** entry)
{
    VALIDATE_NOT_NULL(entry)
    AutoPtr<IZipEntry> ze;
    FAIL_RETURN(ZipFile::GetEntry(entryName, (IZipEntry**)&ze));
    if (!ze) {
        *entry = ze;
        return NOERROR;
    }
    AutoPtr<IJarEntry> je;
    FAIL_RETURN(CJarEntry::New((IZipEntry*)ze.Get(), (IJarEntry**)&je));
    ((CJarEntry*)je.Get())->mParentJar = THIS_PROBE(IJarFile);
    *entry = je.Get();
    REFCOUNT_ADD(*entry)
    return NOERROR;
}

ECode CJarFile::GetInputStream(
    /* [in] */ IZipEntry* ze,
    /* [out] */ IInputStream** is)
{
    VALIDATE_NOT_NULL(is)
    *is = NULL;
    AutoPtr<IManifest> mf;
    if (mManifestEntry != NULL) {
        GetManifest((IManifest**)&mf);
    }

    if (mVerifier != NULL) {
        if (mf == NULL) {
            GetManifest((IManifest**)&mf);
        }
        FAIL_RETURN(mVerifier->SetManifest(mf))

        if (mManifest != NULL) {
            FAIL_RETURN(((CManifest*)mManifest.Get())->GetMainAttributesEnd(&mVerifier->mMainAttributesEnd))
        }

        Boolean isValid;
        if (mVerifier->ReadCertificates(&isValid), isValid) {
            mVerifier->RemoveMetaEntries();

            if (mManifest != NULL) {
                ((CManifest*)mManifest.Get())->RemoveChunks();
            }

            Boolean isSigned;
            if ((mVerifier->IsSignedJar(&isSigned), !isSigned)) {
                mVerifier = NULL;
            }
        }
    }

    AutoPtr<IInputStream> in;
    ZipFile::GetInputStream(ze, (IInputStream**)&in);
    if (in == NULL) {
        return NOERROR;
    }

    Int64 size;
    if (mVerifier == NULL || (ze->GetSize(&size), size) == -1) {
        *is = in;
        REFCOUNT_ADD(*is)
        return NOERROR;
    }

    AutoPtr<JarVerifier::VerifierEntry> entry;
    String name;
    ze->GetName(&name);
    mVerifier->InitEntry(name, (JarVerifier::VerifierEntry**)&entry);
    if (entry == NULL) {
        *is = in;
        REFCOUNT_ADD(*is)
        return NOERROR;
    }

    AutoPtr<IInputStream> ret = (IInputStream*)new CJarFile::JarFileInputStream(in, ze, entry);
    *is = ret;
    REFCOUNT_ADD(*is)
    return NOERROR;
}

ECode CJarFile::GetJarEntry(
    /* [in] */ const String& name,
    /* [out] */ IJarEntry** jarEntry)
{
    VALIDATE_NOT_NULL(jarEntry)
    AutoPtr<IZipEntry> ze;
    FAIL_RETURN(GetEntry(name, (IZipEntry**)&ze))
    *jarEntry = ze.Get();
    REFCOUNT_ADD(*jarEntry)
    return NOERROR;
}

ECode CJarFile::GetManifest(
    /* [out] */ IManifest** manifest)
{
    VALIDATE_NOT_NULL(manifest)
    *manifest = NULL;

    if (mClosed) {
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    if (mManifest != NULL) {
        *manifest = mManifest;
        REFCOUNT_ADD(*manifest)
        return NOERROR;
    }

    AutoPtr<IInputStream> is;
    FAIL_RETURN(ZipFile::GetInputStream(mManifestEntry, (IInputStream**)&is));

    if (mVerifier != NULL) {
        String name;
        AutoPtr<ArrayOf<Byte> > meta;
        AutoPtr<IStreams> stream;
        mManifestEntry->GetName(&name);
        CStreams::AcquireSingleton((IStreams**)&stream);
        stream->ReadFully(is, (ArrayOf<Byte>**)&meta);
        mVerifier->AddMetaEntry(name, meta);
        is = NULL;
        ZipFile::GetInputStream(mManifestEntry, (IInputStream**)&is);
    }

    mManifest = CManifest::Create(is, mVerifier != NULL);
    REFCOUNT_ADD(mManifest)
    ECode ec = is->Close();
    FAIL_GOTO(ec, label)
    mManifestEntry = NULL;

label:
    if (ec == E_NULL_POINTER_EXCEPTION) {
        mManifestEntry = NULL;
    }
    else if (FAILED(ec)) {
        return ec;
    }

    *manifest = mManifest;
    REFCOUNT_ADD(*manifest)
    return ec;
}

ECode CJarFile::constructor(
    /* [in] */ IFile* file)
{
    // TODO: Add your code here
    return constructor(file, TRUE);
}

ECode CJarFile::constructor(
    /* [in] */ IFile * file,
    /* [in] */ Boolean verify)
{
    // TODO: Add your code here
    FAIL_RETURN(ZipFile::constructor(file))
    if (verify) {
        String path;
        mVerifier = new JarVerifier((file->GetPath(&path), path));
    }
    return ReadMetaEntries();
}

ECode CJarFile::constructor(
    /* [in] */ IFile* file,
    /* [in] */ Boolean verify,
    /* [in] */ Int32 mode)
{
    FAIL_RETURN(ZipFile::constructor(file))
    if (verify) {
        String path;
        mVerifier = new JarVerifier((file->GetPath(&path), path));
    }
    return ReadMetaEntries();
}

ECode CJarFile::constructor(
    /* [in] */ const String& filename)
{
    return constructor(filename, TRUE);
}

ECode CJarFile::constructor(
    /* [in] */ const String& filename,
    /* [in] */ Boolean verify)
{
    FAIL_RETURN(ZipFile::constructor(filename))
    if (verify) {
        mVerifier = new JarVerifier(filename);
    }
    return ReadMetaEntries();
}

/**
 * Called by the JarFile constructors, this method reads the contents of the
 * file's META-INF/ directory and picks out the MANIFEST.MF file and
 * verifier signature files if they exist. Any signature files found are
 * registered with the verifier.
 *
 * @throws IOException
 *             if there is a problem reading the jar file entries.
 */
ECode CJarFile::ReadMetaEntries()
{
    // Get all meta directory entries
    AutoPtr<ArrayOf<IZipEntry*> > metaEntries;
    FAIL_RETURN(GetMetaEntriesImpl((ArrayOf<IZipEntry*>**)&metaEntries))
    if (metaEntries == NULL) {
        mVerifier = NULL;
        return NOERROR;
    }
    Boolean bSigned = FALSE;
    Boolean bEndsWith;
    String entryName;
    Int32 eleNum = metaEntries->GetLength();
    for (Int32 i = 0; i < eleNum; ++i) {
        (*metaEntries)[i]->GetName(&entryName);
        // Is this the entry for META-INF/MANIFEST.MF ?
        if (mManifestEntry == NULL && entryName.EqualsIgnoreCase(IJarFile::MANIFEST_NAME)) {
            mManifestEntry = (*metaEntries)[i];
            // If there is no verifier then we don't need to look any further.
            if (mVerifier == NULL) {
                break;
            }
        }
        else {
            // Is this an entry that the verifier needs?
            if (mVerifier != NULL
                && ((EndsWithIgnoreCase(entryName, String(".SF"), &bEndsWith), bEndsWith)
                    || (EndsWithIgnoreCase(entryName, String(".DSA"), &bEndsWith), bEndsWith)
                    || (EndsWithIgnoreCase(entryName, String(".RSA"), &bEndsWith), bEndsWith))) {
                bSigned = TRUE;
                AutoPtr<IInputStream> is;
                FAIL_RETURN(ZipFile::GetInputStream((*metaEntries)[i], (IInputStream**)&is))
                AutoPtr<IStreams> stream;
                CStreams::AcquireSingleton((IStreams**)&stream);
                AutoPtr<ArrayOf<Byte> > buf;
                FAIL_RETURN(stream->ReadFully(is, (ArrayOf<Byte>**)&buf))
                FAIL_RETURN(mVerifier->AddMetaEntry(entryName, buf))
            }
        }
    }

    // If there were no signature files, then no verifier work to do.
    if (!bSigned) {
        mVerifier = NULL;
    }
    return NOERROR;
}

ECode CJarFile::EndsWithIgnoreCase(
    /* [in] */ const String& s,
    /* [in] */ const String& suffix,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    *result = s.RegionMatches(s.GetLength() - suffix.GetLength(), suffix, 0, suffix.GetLength());
    return NOERROR;
}

/**
 * Returns all the ZipEntry's that relate to files in the
 * JAR's META-INF directory.
 *
 * @return the list of ZipEntry's or {@code null} if there are none.
 */
ECode CJarFile::GetMetaEntriesImpl (
    /* [out] */ ArrayOf<IZipEntry*> **zipEntries)
{
    AutoPtr<IObjectContainer> entries;
    FAIL_RETURN(GetEntries((IObjectContainer**)&entries))
    AutoPtr<List<AutoPtr<IZipEntry> > > list = new List<AutoPtr<IZipEntry> >();
    AutoPtr<IObjectEnumerator> enumerator;
    entries->GetObjectEnumerator((IObjectEnumerator**)&enumerator);
    Boolean hasNext = FALSE;
    AutoPtr<IZipEntry> element;
    Int32 i = 0;
    while (enumerator->MoveNext(&hasNext), hasNext)
    {
        element = NULL;
        enumerator->Current((IInterface**)&element);
        String name;
        if ((element->GetName(&name), name).StartWith(META_DIR)
            && name.GetLength() > META_DIR.GetLength()) {
            list->PushBack(element);
        }
    }
    if (list->IsEmpty()) {
        *zipEntries = NULL;
        return NOERROR;
    }
    AutoPtr<ArrayOf<IZipEntry*> > result = ArrayOf<IZipEntry*>::Alloc(list->GetSize());
    Int32 index = 0;
    for (List<AutoPtr<IZipEntry> >::Iterator it = list->Begin();
        it != list->End(); ++it) {
        result->Set(index++, *it);
    }
    *zipEntries = result;
    REFCOUNT_ADD(*zipEntries)
    return NOERROR;
}

} // namespace Jar
} // namespace Utility
} // namespace Elastos

