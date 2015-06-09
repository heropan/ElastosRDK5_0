
#ifndef __ELASTOS_UTILITY_CJARINPUTSTREAM_H__
#define __ELASTOS_UTILITY_CJARINPUTSTREAM_H__

#include "_Elastos_Utility_Jar_CJarInputStream.h"
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

CarClass(CJarInputStream)
    , public ZipInputStream
    , public IJarInputStream
{
public:
    CAR_INTERFACE_DECL()

    CAR_OBJECT_DECL()

    CJarInputStream();

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

#endif // __ELASTOS_UTILITY_CJARINPUTSTREAM_H__
