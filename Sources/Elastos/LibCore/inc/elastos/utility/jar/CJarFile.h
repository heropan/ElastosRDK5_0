
#ifndef __CJARFILE_H__
#define __CJARFILE_H__

#include "_CJarFile.h"
#ifdef ELASTOS_CORELIBRARY
#include "Elastos.CoreLibrary_server.h"
#else
#include "Elastos.CoreLibrary.h"
#endif
#include "ZipFile.h"
#include "JarVerifier.h"

using Elastos::Utility::Zip::ZipFile;
using Elastos::Utility::Zip::IZipEntry;
using Elastos::IO::IInputStream;
using Elastos::IO::IFile;

namespace Elastos {
namespace Utility {
namespace Jar {

CarClass(CJarFile), public ZipFile
{
public:
    class JarFileInputStream
        : public FilterInputStream
        , public IInputStream
        , public ElLightRefBase {
    public:
        JarFileInputStream(
            /* [in] */ IInputStream* is,
            /* [in] */ IZipEntry* ze,
            /* [in] */ JarVerifier::VerifierEntry* e);

        //late following interfaces could be replaced with a macro
        CAR_INTERFACE_DECL()

        CARAPI Available(
            /* [out] */ Int32* val);

        CARAPI Mark(
            /* [in] */ Int32 readLimit);

        CARAPI IsMarkSupported(
            /* [out] */ Boolean* supported);

        CARAPI Read(
            /* [out] */ Int32* val);

        CARAPI ReadBytes(
            /* [out] */ ArrayOf<Byte>* buffer,
            /* [out] */ Int32* number);

        CARAPI ReadBytes(
            /* [in] */ ArrayOf<Byte>* buf,
            /* [in] */ Int32 off,
            /* [in] */ Int32 nbytes,
            /* [out] */ Int32* val);

        CARAPI Reset();

        CARAPI Skip(
            /* [in] */ Int64 byteCount,
            /* [out] */ Int64* val);

        CARAPI Close();

    private:
        Int64 mCount;
        AutoPtr<IZipEntry> mZipEntry;
        AutoPtr<JarVerifier::VerifierEntry> mEntry;
        Boolean mDone;
    };

    CJarFile();

    CARAPI Close();

    CARAPI GetEntries(
        /* [out] */ IObjectContainer** entries);

    CARAPI GetEntry(
        /* [in] */ const String& entryName,
        /* [out] */ IZipEntry** entry);

    CARAPI GetInputStream(
        /* [in] */ IZipEntry* entry,
        /* [out] */ IInputStream** is);

    CARAPI GetName(
        /* [out] */ String* name);

    CARAPI GetSize(
        /* [out] */ Int32* size);

    CARAPI GetJarEntry(
        /* [in] */ const String& name,
        /* [out] */ IJarEntry** jarEntry);

    CARAPI GetManifest(
        /* [out] */ IManifest** manifest);

    CARAPI constructor(
        /* [in] */ IFile* file);

    CARAPI constructor(
        /* [in] */ IFile* file,
        /* [in] */ Boolean verify);

    CARAPI constructor(
        /* [in] */ IFile* file,
        /* [in] */ Boolean verify,
        /* [in] */ Int32 mode);

    CARAPI constructor(
        /* [in] */ const String& filename);

    CARAPI constructor(
        /* [in] */ const String& filename,
        /* [in] */ Boolean verify);

private:
    CARAPI ReadMetaEntries();
    static CARAPI EndsWithIgnoreCase(
        /* [in] */ const String& s,
        /* [in] */ const String& suffix,
        /* [out] */ Boolean* result);
    CARAPI GetMetaEntriesImpl(
    /* [out, callee] */ ArrayOf<IZipEntry*>** zipEntries);

public:
    // The directory containing the manifest.
    static const String META_DIR;
    AutoPtr<JarVerifier> mVerifier;

private:
    // The manifest after it has been read from the JAR.
    AutoPtr<IManifest> mManifest;

    // The entry for the MANIFEST.MF file before it is read.
    AutoPtr<IZipEntry> mManifestEntry;

    Boolean mClosed;
};

} // namespace Jar
} // namespace Utility
} // namespace Elastos

#endif // __CJARFILE_H__
