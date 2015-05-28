#include <elastos.h>
#include "JarVerifier.h"
#include "CName.h"
#include "CAttributes.h"
#include "InitManifest.h"
#include "CManifest.h"
#include "CArrayList.h"
#include "CVector.h"
#include "CMessageDigestHelper.h"
#include "CBase64.h"
#include "CHashMap.h"
#include <cutils/log.h>

using Elastos::IO::EIID_IOutputStream;
using Elastos::Utility::CArrayList;
using Elastos::Utility::CVector;
using Elastos::Utility::CHashMap;
using Elastos::Utility::ISet;
using Elastos::Security::IMessageDigestHelper;
using Elastos::Security::CMessageDigestHelper;
using Libcore::IO::IBase64;
using Libcore::IO::CBase64;
// using Org::Apache::Harmony::Security::Utils::IJarUtils;

namespace Elastos {
namespace Utility {
namespace Jar {

CAR_INTERFACE_IMPL_LIGHT(JarVerifier::VerifierEntry, IOutputStream)

JarVerifier::VerifierEntry::VerifierEntry(
    /* [in] */ const String& name,
    /* [in] */ IMessageDigest* digest,
    /* [in] */ ArrayOf<Byte>* hash,
    /* [in] */ ArrayOf<ICertificate*>* certificates,
    /* [in] */ JarVerifier* host)
    : mName(name)
    , mDigest(digest)
    , mHash(hash)
    , mCertificates(certificates)
    , mHost(host)
{
}

ECode JarVerifier::VerifierEntry::Write(
    /* in */ Int32 value)
{
    return mDigest->Update((Byte)value);
}

ECode JarVerifier::VerifierEntry::WriteBytes(
            /* [in] */ const ArrayOf<Byte>& buffer)
{
    return OutputStream::WriteBytes(buffer);
}

ECode JarVerifier::VerifierEntry::WriteBytes(
    /* in */ const ArrayOf<Byte>& buf,
    /* in */ Int32 off,
    /* in */ Int32 nbytes)
{
    return mDigest->Update(buf, off, nbytes);
}

ECode JarVerifier::VerifierEntry::Verify()
{
    AutoPtr<ArrayOf<Byte> > d;
    mDigest->Digest((ArrayOf<Byte>**)&d);
    AutoPtr<IMessageDigestHelper> helper;
    CMessageDigestHelper::AcquireSingleton((IMessageDigestHelper**)&helper);
    Boolean isEqual;
    AutoPtr<IBase64> bs64;
    CBase64::AcquireSingleton((IBase64**)&bs64);
    AutoPtr<ArrayOf<Byte> > rst;
    bs64->Decode(mHash, 0, (ArrayOf<Byte>**)&rst);
    helper->IsEqual(d, rst, &isEqual);
    if (!isEqual) {
        return E_SECURITY_EXCEPTION;
    }
    (*(mHost->GetVerifiedEntry()))[mName] = mCertificates;
    return NOERROR;
}

ECode JarVerifier::VerifierEntry::CheckError(
            /* [out] */ Boolean* hasError)
{
    return OutputStream::CheckError(hasError);
}

ECode JarVerifier::VerifierEntry::Close()
{
    return OutputStream::Close();
}

ECode JarVerifier::VerifierEntry::GetLock(
    /* [out] */ IInterface** lockobj)
{
    VALIDATE_NOT_NULL(lockobj);
    AutoPtr<IInterface> obj = OutputStream::GetLock();
    *lockobj = obj;
    REFCOUNT_ADD(*lockobj);
    return NOERROR;
}

JarVerifier::JarVerifier()
    : mMainAttributesEnd(0)
    , mMetaEntries(new HashMap<String, AutoPtr<ArrayOf<Byte> > >(5))
    , mSignatures(new HashMap<String, AutoPtr<HashMap<String, AutoPtr<IAttributes> > > >(5))
    , mCertificates(new HashMap<String, AutoPtr<ArrayOf<ICertificate*> > >(5))
    , mVerifiedEntries(new HashMap<String, AutoPtr<ArrayOf<ICertificate*> > >())
{
}

/**
 * Constructs and returns a new instance of {@code JarVerifier}.
 *
 * @param name
 *            the name of the JAR file being verified.
 */
JarVerifier::JarVerifier(
    /* int */ const String& name)
    : mMainAttributesEnd(0)
    , mMetaEntries(new HashMap<String, AutoPtr<ArrayOf<Byte> > >(5))
    , mSignatures(new HashMap<String, AutoPtr<HashMap<String, AutoPtr<IAttributes> > > >(5))
    , mCertificates(new HashMap<String, AutoPtr<ArrayOf<ICertificate*> > >(5))
    , mVerifiedEntries(new HashMap<String, AutoPtr<ArrayOf<ICertificate*> > >())
    , mJarName(name)
{
}

JarVerifier::~JarVerifier()
{
    mVerifiedEntries = NULL;
    mCertificates = NULL;
    mMetaEntries = NULL;
    mSignatures = NULL;
}

/**
 * Invoked for each new JAR entry read operation from the input
 * stream. This method constructs and returns a new {@link VerifierEntry}
 * which contains the certificates used to sign the entry and its hash value
 * as specified in the JAR MANIFEST format.
 *
 * @param name
 *            the name of an entry in a JAR file which is <b>not</b> in the
 *            {@code META-INF} directory.
 * @return a new instance of {@link VerifierEntry} which can be used by
 *         callers as an {@link OutputStream}.
 */
ECode JarVerifier::InitEntry(
    /* in */ const String& name,
    /* out */ VerifierEntry** entry)
{
    // If no manifest is present by the time an entry is found,
    // verification cannot occur. If no signature files have
    // been found, do not verify.
    if (mMan == NULL || mSignatures->IsEmpty()) {
        *entry = NULL;
        return NOERROR;
    }

    AutoPtr<IAttributes> attributes;
    mMan->GetAttributes(name, (IAttributes**)&attributes);
    // entry has no digest
    if (attributes == NULL) {
        *entry = NULL;
        return NOERROR;
    }

    AutoPtr<IArrayList> certs;
    CArrayList::New((IArrayList**)&certs);
    HashMap<String, AutoPtr<HashMap<String, AutoPtr<IAttributes> > > >::Iterator it;
    for (it = mSignatures->Begin(); it != mSignatures->End(); ++it) {
        //HashMap<String, AutoPtr<IAttributes> >* hm =
        AutoPtr<IAttributes> attrib = (*(it->mSecond))[name];
        if (attrib != NULL) {
            // Found an entry for entry name in .SF file
            String signatureFile = it->mFirst;
            AutoPtr<IVector> vt;
            GetSignerCertificates(signatureFile, mCertificates, (IVector**)&vt);
            Boolean rst;
            certs->AddAll(ICollection::Probe(vt.Get()), &rst);
        }
    }

    // entry is not signed
    Boolean isEmpty;
    if ((certs->IsEmpty(&isEmpty), isEmpty)) {
        *entry = NULL;
        return NOERROR;
    }
    AutoPtr<ArrayOf<IInterface*> > tmp;
    certs->ToArray((ArrayOf<PInterface>**)&tmp);
    AutoPtr<ArrayOf<ICertificate*> > certificatesArray = ArrayOf<ICertificate*>::Alloc(tmp->GetLength());
    for (Int32 i = 0; i < tmp->GetLength(); ++i) {
        certificatesArray->Set(i, ICertificate::Probe((*tmp)[i]));
    }

    String algorithms;
    attributes->GetValue(String("Digest-Algorithms"), &algorithms);
    if (algorithms.IsNull()) {
        algorithms = "SHA SHA1";
    }
    StringTokenizer* tokens = new StringTokenizer(algorithms);
    while (tokens->HasMoreTokens()) {
        String algorithm = (String)(const char*)tokens->NextToken();
        String hash;
        attributes->GetValue(algorithm + "-Digest", &hash);
        if (hash.IsNull()) {
            continue;
        }
        UInt32 bytes = hash.GetByteLength();
        AutoPtr<ArrayOf<Byte> > hashBytes = ArrayOf<Byte>::Alloc(bytes);
        FAIL_RETURN(hashBytes->Copy((Byte*)const_cast<char*>(hash.string()), bytes))
        AutoPtr<IMessageDigest> md;
        AutoPtr<IMessageDigestHelper> helper;
        CMessageDigestHelper::AcquireSingleton((IMessageDigestHelper**)&helper);
        ECode ec = helper->GetInstance(algorithm, (IMessageDigest**)&md);
        if (ec == E_NO_SUCH_ALGORITHM_EXCEPTION) {
            //ignore
        }
        if (FAILED(ec)) {
            delete tokens;
            return ec;
        }
        AutoPtr<VerifierEntry> ret = new VerifierEntry(name, md,
            hashBytes, certificatesArray, this);
        *entry = ret;
        REFCOUNT_ADD(*entry)
        delete tokens;
        return NOERROR;
    }
    delete tokens;
    *entry = NULL;
    return NOERROR;
}

/**
 * Add a new meta entry to the internal collection of data held on each JAR
 * entry in the {@code META-INF} directory including the manifest
 * file itself. Files associated with the signing of a JAR would also be
 * added to this collection.
 *
 * @param name
 *            the name of the file located in the {@code META-INF}
 *            directory.
 * @param buf
 *            the file bytes for the file called {@code name}.
 * @see #removeMetaEntries()
 */
ECode JarVerifier::AddMetaEntry(
    /* in */ const String& name,
    /* in */ ArrayOf<Byte>* buf)
{
    (*mMetaEntries)[name.ToUpperCase()] = buf;
    return NOERROR;
}

/**
 * If the associated JAR file is signed, check on the validity of all of the
 * known signatures.
 *
 * @return {@code true} if the associated JAR is signed and an internal
 *         check verifies the validity of the signature(s). {@code false} if
 *         the associated JAR file has no entries at all in its {@code
 *         META-INF} directory. This situation is indicative of an invalid
 *         JAR file.
 *         <p>
 *         Will also return {@code true} if the JAR file is <i>not</i>
 *         signed.
 * @throws SecurityException
 *             if the JAR file is signed and it is determined that a
 *             signature block file contains an invalid signature for the
 *             corresponding signature file.
 */
ECode JarVerifier::ReadCertificates(
    /* out */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = NULL;

    Object::Autolock lock(mLock);
    VALIDATE_NOT_NULL(result)
    if (mMetaEntries == NULL) {
        return NOERROR;
    }

    String key;
    HashMap<String, AutoPtr<ArrayOf<Byte> > >::Iterator it = mMetaEntries->Begin();
    for (; it != mMetaEntries->End();) {
        key = it->mFirst;
        if (key.EndWith(".DSA") || key.EndWith(".RSA")) {
            FAIL_RETURN(VerifyCertificate(key))
            // Check for recursive class load
            if (mMetaEntries == NULL) {
                return NOERROR;
            }
            mMetaEntries->Erase(it++);
        }
        else {
            it++;
        }
    }

    *result = TRUE;
    return NOERROR;
}

ECode JarVerifier::VerifyCertificate(
    /* in */ const String& certFile)
{
    // Found Digital Sig, .SF should already have been read
    String signatureFile = certFile.Substring(0, certFile.LastIndexOf('.'))
        + ".SF";
    AutoPtr<ArrayOf<Byte> > sfBytes;
    sfBytes = (*mMetaEntries)[signatureFile];
    if (sfBytes == NULL) {
        return NOERROR;
    }

    AutoPtr<ArrayOf<Byte> > manifest = (*mMetaEntries)[IJarFile::MANIFEST_NAME];
    // Manifest entry is required for any verifications.
    if (manifest == NULL) {
        return NOERROR;
    }

    AutoPtr<ArrayOf<Byte> > sBlockBytes = (*mMetaEntries)[certFile];
    /*
        try {
        Certificate[] signerCertChain = JarUtils.verifySignature(
                new ByteArrayInputStream(sfBytes),
                new ByteArrayInputStream(sBlockBytes));

         // Recursive call in loading security provider related class which
         // is in a signed JAR.
        if (metaEntries == null) {
            return;
        }
        if (signerCertChain != null) {
            certificates.put(signatureFile, signerCertChain);
        }
        } catch (IOException e) {
            return;
        } catch (GeneralSecurityException e) {
            throw failedVerification(jarName, signatureFile);
        }
    */

    AutoPtr<ArrayOf<ICertificate*> > signerCertChain;

    // AutoPtr<IJarUtils> jarUtils;
    // CJarUtils::AcquireSingleton((IJarUtils**)&jarUtils);
    // AutoPtr<IByteArrayInputStream> sfi, sbi;
    // CByteArrayInputStream::New(sfBytes, (IByteArrayInputStream**)&sfi);
    // CByteArrayInputStream::New(sBlockBytes, (IByteArrayInputStream**)&sbi);
    // jarUtils->VerifySignature(sfi, sbi, (ArrayOf<ICertificate*>**)&signerCertChain);

    // Recursive call in loading security provider related class which
    // is in a signed JAR.
    if (mMetaEntries == NULL) {
        return NOERROR;
    }
    if (signerCertChain != NULL) {
        (*mCertificates)[signatureFile] = signerCertChain;
    }

    // Verify manifest hash in .sf file
    AutoPtr<IAttributes> attributes;
    FAIL_RETURN(CAttributes::New((IAttributes**)&attributes))
    AutoPtr<IMap> _entries;
    CHashMap::New((IMap**)&_entries);
    AutoPtr<InitManifest> im = new InitManifest(sfBytes, attributes, CName::SIGNATURE_VERSION);
    FAIL_RETURN(im->InitEntries(_entries, NULL))

    //convert _entries to entries;
    AutoPtr<HashMap<String, AutoPtr<IAttributes> > > entries = new HashMap<String, AutoPtr<IAttributes> >();
    AutoPtr<ISet> entrySet;
    _entries->EntrySet((ISet**)&entrySet);
    AutoPtr<IIterator> it;
    entrySet->GetIterator((IIterator**)&it);

    Boolean hasNext;
    String strKey;
    IMapEntry* mapEnty;
    for (; (it->HasNext(&hasNext), hasNext);) {
        AutoPtr<IInterface> next, key, value;
        it->Next((IInterface**)&next);
        mapEnty = IMapEntry::Probe(next);
        mapEnty->GetKey((IInterface**)&key);
        mapEnty->GetValue((IInterface**)&value);
        ICharSequence::Probe(key)->ToString(&strKey);
        (*entries)[strKey] = IAttributes::Probe(value);
    }

    Boolean createdBySigntool = FALSE;
    String createdBy;
    FAIL_RETURN(attributes->GetValue(String("Created-By"), &createdBy))
    if (createdBy != NULL) {
        createdBySigntool = createdBy.IndexOf("signtool") != -1;
    }

    // Use .SF to verify the mainAttributes of the manifest
    // If there is no -Digest-Manifest-Main-Attributes entry in .SF
    // file, such as those created before java 1.5, then we ignore
    // such verification.
    if (mMainAttributesEnd > 0 && !createdBySigntool) {
        String digestAttribute = String("-Digest-Manifest-Main-Attributes");
        Boolean bVerified;
        if (!(Verify(attributes, digestAttribute, manifest, 0, mMainAttributesEnd, FALSE, TRUE, &bVerified), bVerified)) {
            //throw failedVerification(mJarName, signatureFile);
            return E_SECURITY_EXCEPTION;
        }
    }

    // Use .SF to verify the whole manifest.
    String digestAttribute = createdBySigntool ? String("-Digest")
            : String("-Digest-Manifest");
    Boolean result;
    if (Verify(attributes, digestAttribute, manifest, 0, manifest->GetLength(),
        FALSE, FALSE, &result), !result) {
        HashMap<String, AutoPtr<IAttributes> >::Iterator it = entries->Begin();
        while (it != entries->End()) {

            AutoPtr<CManifest::Chunk> chunk;
            ((CManifest*)mMan.Get())->GetChunk(it->mFirst, (CManifest::Chunk**)&chunk);
            if (chunk == NULL) {
                return NOERROR;
            }
            if (Verify(it->mSecond, String("-Digest"), manifest,
                    chunk->mStart, chunk->mEnd, createdBySigntool, FALSE, &result), !result) {
                return E_SECURITY_EXCEPTION;
            }
            ++it;
        }
    }

    (*mMetaEntries)[signatureFile] = NULL;
    (*mSignatures)[signatureFile] = entries;
    return NOERROR;
}

ECode JarVerifier::Verify(
        /* in */ IAttributes* attributes,
        /* in */ const String& entry,
        /* in */ ArrayOf<Byte>* data,
        /* in */ Int32 start,
        /* in */ Int32 end,
        /* in */ Boolean ignoreSecondEndline,
        /* in */ Boolean ignorable,
        /* out */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    String algorithms;
    attributes->GetValue(String("Digest-Algorithms"), &algorithms);
    if (algorithms == NULL) {
        algorithms = "SHA SHA1";
    }
    StringTokenizer* tokens = new StringTokenizer(algorithms);
    while (tokens->HasMoreTokens()) {
        String algorithm = (String)(const char*)tokens->NextToken();
        String hash;
        attributes->GetValue(algorithm + entry, &hash);
        if (hash == NULL) {
            continue;
        }

        AutoPtr<IMessageDigest> md;
        AutoPtr<IMessageDigestHelper> helper;
        CMessageDigestHelper::AcquireSingleton((IMessageDigestHelper**)&helper);
        ECode ec = helper->GetInstance(algorithm, (IMessageDigest**)&md);
        if (E_NO_SUCH_ALGORITHM_EXCEPTION == ec) {
            continue;
        }
        delete tokens;
        FAIL_RETURN(ec)
        if (ignoreSecondEndline && (*data)[end - 1] == '\n'
            && (*data)[end - 2] == '\n') {
            md->Update(data, start, end - 1 - start);
        }
        else {
            md->Update(data, start, end - start);
        }
        AutoPtr<ArrayOf<Byte> > b;
        md->Digest((ArrayOf<Byte>**)&b);
        AutoPtr<ArrayOf<Byte> > hashBytes = ArrayOf<Byte>::Alloc(hash.GetByteLength());
        hashBytes->Copy(hash.string(), hash.GetByteLength());
        AutoPtr<IBase64> base64;
        CBase64::AcquireSingleton((IBase64**)&base64);
        AutoPtr<ArrayOf<Byte> > outData;
        base64->Decode(hashBytes, (ArrayOf<Byte>**)&outData);
        return helper->IsEqual(b, outData, result);

    }
    delete tokens;
    *result = ignorable;
    return NOERROR;
}

/**
 * Returns all of the {@link java.security.cert.Certificate} instances that
 * were used to verify the signature on the JAR entry called
 * {@code name}.
 *
 * @param name
 *            the name of a JAR entry.
 * @return an array of {@link java.security.cert.Certificate}.
 */
ECode JarVerifier::GetCertificates(
    /* [in] */ const String& name,
    /* [out, callee] */ ArrayOf<ICertificate*>** certificates)
{
    VALIDATE_NOT_NULL(certificates)
    AutoPtr<ArrayOf<ICertificate*> > verifiedCerts = (*mVerifiedEntries)[name];
    if (verifiedCerts == NULL) {
        *certificates = NULL;
        return NOERROR;
    }
    *certificates = verifiedCerts->Clone();
    REFCOUNT_ADD(*certificates)
    return NOERROR;
}

/**
 * Remove all entries from the internal collection of data held about each
 * JAR entry in the {@code META-INF} directory.
 *
 * @see #addMetaEntry(String, byte[])
 */
ECode JarVerifier::RemoveMetaEntries()
{
    mMetaEntries = NULL;
    return NOERROR;
}

/**
 * Returns a {@code Vector} of all of the
 * {@link java.security.cert.Certificate}s that are associated with the
 * signing of the named signature file.
 *
 * @param signatureFileName
 *            the name of a signature file.
 * @param certificates
 *            a {@code Map} of all of the certificate chains discovered so
 *            far while attempting to verify the JAR that contains the
 *            signature file {@code signatureFileName}. This object is
 *            previously set in the course of one or more calls to
 *            {@link #verifyJarSignatureFile(String, String, String, Map, Map)}
 *            where it was passed as the last argument.
 * @return all of the {@code Certificate} entries for the signer of the JAR
 *         whose actions led to the creation of the named signature file.
 */
ECode JarVerifier::GetSignerCertificates(
    /* [in] */ const String& signatureFileName,
    /* [in] */ HashMap<String, AutoPtr<ArrayOf<ICertificate*> > >* certificates,
    /* [out] */ IVector** certs)
{
    VALIDATE_NOT_NULL(certs)
    AutoPtr<IVector> result;
    CVector::New((IVector**)&result);
    AutoPtr<ArrayOf<ICertificate*> > certChain = (*certificates)[signatureFileName];
    if (certChain != NULL) {
        for (Int32 i = 0; i < certChain->GetLength(); ++i) {
            result->AddElement((*certChain)[i]);
        }
    }
    *certs = result;
    REFCOUNT_ADD(*certs)
    return NOERROR;
}


ECode JarVerifier::SetManifest(
        /* in */ IManifest* mf)
{
    mMan = mf;
    return NOERROR;
}

/**
 * Returns a <code>boolean</code> indication of whether or not the
 * associated jar file is signed.
 *
 * @return {@code true} if the JAR is signed, {@code false}
 *         otherwise.
 */
ECode JarVerifier::IsSignedJar(
    /* [out] */ Boolean* isSigned)
{
    VALIDATE_NOT_NULL(isSigned)
    *isSigned = mCertificates->GetSize() > 0;
    return NOERROR;
}

HashMap<String, AutoPtr<ArrayOf<ICertificate*> > >* JarVerifier::GetVerifiedEntry()
{
    return mVerifiedEntries;
}

} // namespace Jar
} // namespace Utility
} // namespace Elastos
