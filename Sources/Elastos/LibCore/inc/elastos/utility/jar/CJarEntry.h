
#ifndef __ELASTOS_UTILITY_CJARENTRY_H__
#define __ELASTOS_UTILITY_CJARENTRY_H__

#include "_Elastos_Utility_Jar_CJarEntry.h"
#include "ZipEntry.h"
#include "List.h"

using Elastos::Utility::Etl::List;
using Elastos::Utility::Zip::IZipEntry;
using Elastos::Utility::Zip::ZipEntry;
using Elastos::Security::ICodeSigner;
using Elastos::Security::Cert::ICertificate;
using Elastos::Security::Cert::ICertificateFactory;

namespace Elastos {
namespace Utility {
namespace Jar {

CarClass(CJarEntry)
    , public ZipEntry
    , public IJarEntry
{
public:
    CAR_INTERFACE_DECL()

    CAR_OBJECT_DECL()

    CJarEntry();

    CARAPI GetComment(
        /* [out] */ String* comment);

    CARAPI GetCompressedSize(
        /* [out] */ Int64* size);

    CARAPI GetCrc(
        /* [out] */ Int64* checksum);

    CARAPI GetExtra(
        /* [out, callee] */ ArrayOf<Byte> ** extra);

    CARAPI GetMethod(
        /* [out] */ Int32* method);

    CARAPI GetName(
        /* [out] */ String* name);

    CARAPI GetSize(
        /* [out] */ Int64* size);

    CARAPI GetTime(
        /* [out] */ Int64* timeInMil);

    CARAPI IsDirectory(
        /* [out] */ Boolean* isDirectory);

    CARAPI GetAttributes(
        /* [out] */ IAttributes** attrib);

    CARAPI constructor(
        /* [in] */ const String& name);

    CARAPI constructor(
        /* [in] */ IZipEntry* zipEntry);

    CARAPI constructor(
        /* [in] */ IJarEntry* jarEntry);

    CARAPI GetCertificates(
        /* [out, callee] */ ArrayOf<ICertificate*>** certificates);

    CARAPI SetAttributes(
        /* [in] */ IAttributes* attrib);

    CARAPI GetCodeSigners(
        /* [out, callee] */ ArrayOf<ICodeSigner*>** codeSigner);

private:
    CARAPI GetCodeSigners(
    /* [in] */ ArrayOf<ICertificate*>* certs,
    /* [out, callee] */ ArrayOf<ICodeSigner*>** codeSigners);

    CARAPI AddCodeSigner(
    /* [in] */ List<AutoPtr<ICodeSigner> >* asigners,
    /* [in] */ List<AutoPtr<ICertificate> >* list);

public:
    AutoPtr<IJarFile> mParentJar;
    AutoPtr<ArrayOf<ICodeSigner*> > mSigners;

private:
    AutoPtr<IAttributes> mAttributes;
    // Cached factory used to build CertPath-s in <code>getCodeSigners()</code>.
    AutoPtr<ICertificateFactory> mFactory;
    Boolean mIsFactoryChecked;
};

} // namespace Jar
} // namespace Utility
} // namespace Elastos

#endif // __ELASTOS_UTILITY_CJARENTRY_H__
