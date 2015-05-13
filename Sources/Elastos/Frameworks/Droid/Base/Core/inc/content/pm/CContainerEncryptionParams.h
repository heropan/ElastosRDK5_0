
#ifndef __CCONTAINERENCRYPTIONPARAMS_H__
#define __CCONTAINERENCRYPTIONPARAMS_H__

#include "_CContainerEncryptionParams.h"

using namespace Elastos::Core;

namespace Elastos {
namespace Droid {
namespace Content {
namespace Pm {

CarClass(CContainerEncryptionParams)
{
public:
    CARAPI constructor(
        /* [in] */ const String& encryptionAlgorithm,
        /* [in] */ Elastos::Security::Spec::IAlgorithmParameterSpec* encryptionSpec,
        /* [in] */ Elastos::Crypto::ISecretKey* encryptionKey);

    CARAPI constructor(
        /* [in] */ const String& encryptionAlgorithm,
        /* [in] */ Elastos::Security::Spec::IAlgorithmParameterSpec* encryptionSpec,
        /* [in] */ Elastos::Crypto::ISecretKey* encryptionKey,
        /* [in] */ const String& macAlgorithm,
        /* [in] */ Elastos::Security::Spec::IAlgorithmParameterSpec* macSpec,
        /* [in] */ Elastos::Crypto::ISecretKey* macKey,
        /* [in] */ ArrayOf<Byte>* macTag,
        /* [in] */ Int64 authenticatedDataStart,
        /* [in] */ Int64 encryptedDataStart,
        /* [in] */ Int64 dataEnd);

    CARAPI GetEncryptionAlgorithm(
        /* [out] */ String* ealgo);

    CARAPI GetEncryptionSpec(
        /* [out] */ Elastos::Security::Spec::IAlgorithmParameterSpec** spec);

    CARAPI GetEncryptionKey(
        /* [out] */ Elastos::Crypto::ISecretKey** key);

    CARAPI GetMacAlgorithm(
        /* [out] */ String* malgo);

    CARAPI GetMacSpec(
        /* [out] */ Elastos::Security::Spec::IAlgorithmParameterSpec** spec);

    CARAPI GetMacKey(
        /* [out] */ Elastos::Crypto::ISecretKey** key);

    CARAPI GetMacTag(
        /* [out, callee] */ ArrayOf<Byte>** tag);

    CARAPI GetAuthenticatedDataStart(
        /* [out] */ Int64* aDataStart);

    CARAPI GetEncryptedDataStart(
        /* [out] */ Int64* eDataStart);

    CARAPI GetDataEnd(
        /* [out] */ Int64* dataEnd);

    CARAPI Equals(
        /* [in] */ IInterface* o,
        /* [out] */ Boolean* isEquals);

    CARAPI GetHashCode(
        /* [out] */ Int32* code);

    CARAPI ToString(
        /* [out] */ String* str);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

private:
    CARAPI constructor(
        /* [in] */ IParcel* source);

    CARAPI Init(
        /* [in] */ const String& encryptionAlgorithm,
        /* [in] */ Elastos::Security::Spec::IAlgorithmParameterSpec* encryptionSpec,
        /* [in] */ Elastos::Crypto::ISecretKey* encryptionKey,
        /* [in] */ const String& macAlgorithm,
        /* [in] */ Elastos::Security::Spec::IAlgorithmParameterSpec* macSpec,
        /* [in] */ Elastos::Crypto::ISecretKey* macKey,
        /* [in] */ ArrayOf<Byte>* macTag,
        /* [in] */ Int64 authenticatedDataStart,
        /* [in] */ Int64 encryptedDataStart,
        /* [in] */ Int64 dataEnd);

private:
    /** What we print out first when toString() is called. */
    static const String TO_STRING_PREFIX;

    /**
     * Parameter type for parceling that indicates the next parameters are
     * IvParameters.
     */
    static const Int32 ENC_PARAMS_IV_PARAMETERS;

    /** Parameter type for paceling that indicates there are no MAC parameters. */
    static const Int32 MAC_PARAMS_NONE;

    /** The encryption algorithm used. */
    String mEncryptionAlgorithm;

    /** The parameter spec to be used for encryption. */
    AutoPtr<Elastos::Security::Spec::IIvParameterSpec> mEncryptionSpec;

    /** Secret key to be used for decryption. */
    AutoPtr<Elastos::Crypto::ISecretKey> mEncryptionKey;

    /** Algorithm name for the MAC to be used. */
    String mMacAlgorithm;

    /** The parameter spec to be used for the MAC tag authentication. */
    AutoPtr<Elastos::Security::Spec::IAlgorithmParameterSpec> mMacSpec;

    /** Secret key to be used for MAC tag authentication. */
    AutoPtr<Elastos::Crypto::ISecretKey> mMacKey;

    /** MAC tag authenticating the data in the container. */
    AutoPtr<ArrayOf<Byte> > mMacTag;

    /** Offset into file where authenticated (e.g., MAC protected) data begins. */
    Int64 mAuthenticatedDataStart;

    /** Offset into file where encrypted data begins. */
    Int64 mEncryptedDataStart;

    /**
     * Offset into file for the end of encrypted data (and, by extension,
     * authenticated data) in file.
     */
    Int64 mDataEnd;
};

}
}
}
}

#endif // __CCONTAINERENCRYPTIONPARAMS_H__
