
#ifndef __CERTIFICATE_H__
#define __CERTIFICATE_H__

#ifdef ELASTOS_CORELIBRARY
#include "Elastos.CoreLibrary_server.h"
#else
#include "Elastos.CoreLibrary.h"
#endif

using Elastos::IO::EIID_ISerializable;
using Elastos::IO::IInputStream;
using Elastos::IO::IObjectStreamField;
using Elastos::IO::ISerializable;

namespace Elastos {
namespace Security {
namespace Cert {

extern "C" const InterfaceID EIID_Certificate;

class Certificate {
public:
    /**
     * Returns the certificate type.
     *
     * @return the certificate type.
     */
    CARAPI GetType(
        /* [out] */ String* type) const;

    /**
     * Compares the argument to the certificate, and returns {@code true} if they
     * represent the <em>same</em> object using a class specific comparison. The
     * implementation in Object returns {@code true} only if the argument is the
     * exact same object as the callee (==).
     *
     * @param other
     *            the object to compare with this object.
     * @return {@code true} if the object is the same as this object, {@code
     *         false} if it is different from this object.
     * @see #hashCode
     */
    CARAPI Equals(
        /* [in] */ IInterface* other,
        /* [out] */ Boolean* equal);

    /**
     * Returns an integer hash code for the certificate. Any two objects which
     * return {@code true} when passed to {@code equals} must return the same
     * value for this method.
     *
     * @return the certificate's hash
     * @see #equals
     */
    CARAPI GetHashCode(
        /* [out] */ Int32* hashCode);

    /**
     * Returns the encoded representation for this certificate.
     *
     * @return the encoded representation for this certificate.
     * @throws CertificateEncodingException
     *             if the encoding fails.
     */
    virtual CARAPI GetEncoded(
        /* [out, callee] */ ArrayOf<Byte>** encode) = 0;

    /**
     * Verifies that this certificate was signed with the given public key.
     *
     * @param key
     *            PublicKey public key for which verification should be
     *            performed.
     * @throws CertificateException
     *             if encoding errors are detected.
     * @throws NoSuchAlgorithmException
     *             if an unsupported algorithm is detected.
     * @throws InvalidKeyException
     *             if an invalid key is detected.
     * @throws NoSuchProviderException
     *             if there is no default provider.
     * @throws SignatureException
     *             if signature errors are detected.
     */
    virtual CARAPI Verify(
        /* [in] */ IPublicKey* key) = 0;

    /**
     * Verifies that this certificate was signed with the given public key. It
     * Uses the signature algorithm given by the provider.
     *
     * @param key
     *            PublicKey public key for which verification should be
     *            performed.
     * @param sigProvider
     *            String the name of the signature provider.
     * @exception CertificateException
     *                if encoding errors are detected.
     * @exception NoSuchAlgorithmException
     *                if an unsupported algorithm is detected.
     * @exception InvalidKeyException
     *                if an invalid key is detected.
     * @exception NoSuchProviderException
     *                if the specified provider does not exists.
     * @exception SignatureException
     *                if signature errors are detected.
     */
    virtual CARAPI Verify(
        /* [in] */ IPublicKey* key,
        /* [in] */ const String& sigProvider) = 0;

    /**
     * Returns a string containing a concise, human-readable description of the
     * certificate.
     *
     * @return a printable representation for the certificate.
     */
    virtual CARAPI ToString(
        /* [out] */ String* str) = 0;

    /**
     * Returns the public key corresponding to this certificate.
     *
     * @return the public key corresponding to this certificate.
     */
    virtual CARAPI GetPublicKey(
        /* [out] */ IPublicKey** publicKey) = 0;
protected:
    /**
     * Creates a new {@code Certificate} with the specified type.
     *
     * @param type
     *        the certificate type.
     */
    Certificate(
        /* [in] */ const String& type);

    /**
     * Returns an alternate object to be serialized.
     *
     * @return the object to serialize.
     * @throws ObjectStreamException
     *             if the creation of the alternate object fails.
     */
    CARAPI WriteReplace(
        /* [out] */ IInterface** object);

    /**
     * The alternate {@code Serializable} class to be used for serialization and
     * deserialization of {@code Certificate} objects.
     */
    class CertificateRep
        : public ISerializable
        , public ElLightRefBase
    {
    public:
        CAR_INTERFACE_DECL()
        CertificateRep(
            /* [in] */ const String& type,
            /* [in] */ ArrayOf<Byte>* data);
    protected:
        /**
         * Creates a new {@code CertificateRep} instance with the specified
         * certificate type and encoded data.
         *
         * @param type
         *            the certificate type.
         * @param data
         *            the encoded data.
         */

        /**
         * Deserializes a {@code Certificate} from a serialized {@code
         * CertificateRep} object.
         *
         * @return the deserialized {@code Certificate}.
         * @throws ObjectStreamException
         *             if deserialization fails.
         */
        CARAPI ReadResolve(
            /* [out] */ IInterface** object);
    private:
        static const Int64 sSerialVersionUID;
        // The standard name of the certificate type
        const String mType;
        // The certificate data
        const AutoPtr<ArrayOf<Byte> > mData;
        // Force default serialization to use writeUnshared/readUnshared
        // for the certificate data
        static const AutoPtr<ArrayOf<IObjectStreamField*> > sSerialPersistentFields;
        /* = {
             new ObjectStreamField("type", String.class),
             new ObjectStreamField("data", byte[].class, true)
        };*/
    };

private:
    // The standard name of the certificate type
    const String mType;

};

} // end Cert
} // end Security
} // end Elastos

#endif // __CERTIFICATE_H__