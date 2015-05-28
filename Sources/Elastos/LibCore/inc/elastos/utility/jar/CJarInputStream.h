
#ifndef __CJARINPUTSTREAM_H__
#define __CJARINPUTSTREAM_H__

#include "_CJarInputStream.h"
#ifdef ELASTOS_CORELIBRARY
#include "Elastos.CoreLibrary_server.h"
#else
#include "Elastos.CoreLibrary.h"
#endif
#include "ZipInputStream.h"
#include "JarVerifier.h"

using Elastos::Utility::Zip::ZipInputStream;
using Elastos::Utility::Zip::IZipEntry;
using Elastos::IO::IOutputStream;
using Elastos::IO::IOutputStream;
using Elastos::Core::ICharSequence;

namespace Elastos {
namespace Utility {
namespace Jar {

CarClass(CJarInputStream), public ZipInputStream
{
public:
    CJarInputStream();

    CARAPI Close();

    CARAPI Available(
        /* [out] */ Int32* number);

    CARAPI Mark(
        /* [in] */ Int32 readLimit);

    CARAPI IsMarkSupported(
        /* [out] */ Boolean* supported);

    CARAPI Read(
        /* [out] */ Int32* value);

    CARAPI ReadBytes(
        /* [out] */ ArrayOf<Byte>* buffer,
        /* [out] */ Int32* number);

    CARAPI ReadBytes(
        /* [out] */ ArrayOf<Byte>* buffer,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 length,
        /* [out] */ Int32* number);

    CARAPI Reset();

    CARAPI Skip(
        /* [in] */ Int64 byteCount,
        /* [out] */ Int64* number);

    CARAPI CloseEntry();

    CARAPI GetNextEntry(
        /* [out] */ IZipEntry** entry);

    CARAPI GetManifest(
        /* [out] */ IManifest** manifest);

    CARAPI GetNextJarEntry(
        /* [out] */ IJarEntry** jarEntry);

    CARAPI constructor(
        /* [in] */ IInputStream* stream,
        /* [in] */ Boolean verify);

    CARAPI constructor(
        /* [in] */ IInputStream* stream);

    CARAPI GetLock(
        /* [out] */ IInterface** lockobj);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

private:
    AutoPtr<IManifest> mManifest;

    Boolean mEos;

    AutoPtr<IJarEntry> mEntry;

    AutoPtr<IJarEntry> mJarEntry;

    Boolean mIsMeta;

    AutoPtr<JarVerifier> mVerifier;

    AutoPtr<IOutputStream> mVerStream;
};

} // namespace Jar
} // namespace Utility
} // namespace Elastos

#endif // __CJARINPUTSTREAM_H__
