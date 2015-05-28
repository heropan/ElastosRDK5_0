#ifndef __JARVERIFIER_H__
#define __JARVERIFIER_H__

#ifdef ELASTOS_CORELIBRARY
#include "Elastos.CoreLibrary_server.h"
#else
#include "Elastos.CoreLibrary.h"
#endif
#include "elastos/HashMap.h"
#include <elastos.h>
#include "OutputStream.h"
#include <elastos/Object.h>

using Elastos::Utility::IVector;
using Elastos::Utility::HashMap;
using Elastos::Core::Object;
using Elastos::IO::IOutputStream;
using Elastos::IO::OutputStream;
using Elastos::Security::Cert::ICertificate;
using Elastos::Security::IMessageDigest;

namespace Elastos {
namespace Utility {
namespace Jar {

class JarVerifier : public ElLightRefBase {
public:
    /**
    * Stores and a hash and a message digest and verifies that massage digest
    * matches the hash.
    */
    class VerifierEntry :
        public IOutputStream,
        public OutputStream,
        public ElLightRefBase {
    public:
        CAR_INTERFACE_DECL()

        VerifierEntry(
            /* [in] */ const String& name,
            /* [in] */ IMessageDigest* digest,
            /* [in] */ ArrayOf<Byte>* hash,
            /* [in] */ ArrayOf<ICertificate*>* certificates,
            /* [in] */ JarVerifier* host);

        /**
         * Updates a digest with one byte.
         */
        CARAPI Write(
            /* in */ Int32 value);

        CARAPI WriteBytes(
            /* [in] */ const ArrayOf<Byte>& buffer);

        /**
         * Updates a digest with byte array.
         */
        CARAPI WriteBytes(
            /* [in] */ const ArrayOf<Byte>& buffer,
            /* [in] */ Int32 offset,
            /* [in] */ Int32 count);

        /**
         * Verifies that the digests stored in the manifest match the decrypted
         * digests from the .SF file. This indicates the validity of the
         * signing, not the integrity of the file, as it's digest must be
         * calculated and verified when its contents are read.
         *
         * @throws SecurityException
         *             if the digest value stored in the manifest does <i>not</i>
         *             agree with the decrypted digest as recovered from the
         *             <code>.SF</code> file.
         */
        CARAPI Verify();

        CARAPI CheckError(
            /* [out] */ Boolean* hasError);

        CARAPI Close();

        CARAPI GetLock(
            /* [out] */ IInterface** lockobj);
    private:
        String mName;

        AutoPtr<IMessageDigest> mDigest;

        AutoPtr<ArrayOf<Byte> > mHash;

        AutoPtr<ArrayOf<ICertificate*> > mCertificates;

        JarVerifier* mHost;
    };

    static CARAPI GetSignerCertificates(
        /* [in] */ const String& signatureFileName,
        /* [in] */ HashMap<String, AutoPtr<ArrayOf<ICertificate*> > >* certificates,
        /* [out] */ IVector** certs);

    JarVerifier();
    JarVerifier(
        /* int */ const String& name);
    ~JarVerifier();
    CARAPI InitEntry(
        /* in */ const String& name,
        /* out */ VerifierEntry** entry);
    CARAPI AddMetaEntry(
        /* in */ const String& name,
        /* in */ ArrayOf<Byte>* buf);

    CARAPI ReadCertificates(
        /* out */ Boolean* result);

    CARAPI RemoveMetaEntries();

    /**
     * Associate this verifier with the specified {@link Manifest} object.
     *
     * @param mf
     *            a {@code java.util.jar.Manifest} object.
     */
    CARAPI SetManifest(
        /* in */ IManifest* mf);

    CARAPI IsSignedJar(
    /* [out] */ Boolean* isSigned);

    CARAPI GetCertificates(
    /* [in] */ const String& name,
    /* [out, callee] */ ArrayOf<ICertificate*>** certificates);

    HashMap<String, AutoPtr<ArrayOf<ICertificate*> > >* GetVerifiedEntry();

private:
    CARAPI VerifyCertificate(
        /* in */ const String& certFile);

    CARAPI Verify(
        /* in */ IAttributes* attributes,
        /* in */ const String& entry,
        /* in */ ArrayOf<Byte>* data,
        /* in */ Int32 start,
        /* in */ Int32 end,
        /* in */ Boolean ignoreSecondEndline,
        /* in */ Boolean ignorable,
        /* out */ Boolean* result);

public:
    Int32 mMainAttributesEnd;
private:
    const String mJarName;
    AutoPtr<IManifest> mMan;

    AutoPtr<HashMap<String, AutoPtr<ArrayOf<Byte> > > > mMetaEntries;

    typedef HashMap<String, AutoPtr<IAttributes> > StringAttributesMap;
    typedef typename StringAttributesMap::Iterator StringAttributesMapIterator;

    AutoPtr<HashMap<String, AutoPtr<StringAttributesMap> > > mSignatures;
    AutoPtr<HashMap<String, AutoPtr<ArrayOf<ICertificate*> > > > mCertificates;
    AutoPtr<HashMap<String, AutoPtr<ArrayOf<ICertificate*> > > > mVerifiedEntries;

    Object mLock;
};

} // namespace Jar
} // namespace Utility
} // namespace Elastos

#endif //end __JARVERIFIER_H__
