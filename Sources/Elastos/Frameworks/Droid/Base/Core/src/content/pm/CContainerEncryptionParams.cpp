
#include "content/pm/CContainerEncryptionParams.h"

namespace Elastos {
namespace Droid {
namespace Content {
namespace Pm {


const String TO_STRING_PREFIX = String("ContainerEncryptionParams{");

const Int32 CContainerEncryptionParams::ENC_PARAMS_IV_PARAMETERS = 1;

const Int32 CContainerEncryptionParams::MAC_PARAMS_NONE = 1;

CAR_INTERFACE_IMPL(CContainerEncryptionParams, Object, IContainerEncryptionParams, IParcelable)

CAR_OBJECT_IMPL(CContainerEncryptionParams)

ECode CContainerEncryptionParams::constructor()
{
    return NOERROR;
}

ECode CContainerEncryptionParams::constructor(
    /* [in] */ const String& encryptionAlgorithm,
    /* [in] */ Elastos::Security::Spec::IAlgorithmParameterSpec* encryptionSpec,
    /* [in] */ Elastos::Crypto::ISecretKey* encryptionKey)
{
    return Init(encryptionAlgorithm, encryptionSpec, encryptionKey,
                 String(NULL), NULL, NULL, NULL, -1, -1, -1);;
}

ECode CContainerEncryptionParams::constructor(
    /* [in] */ const String& encryptionAlgorithm,
    /* [in] */ Elastos::Security::Spec::IAlgorithmParameterSpec* encryptionSpec,
    /* [in] */ Elastos::Crypto::ISecretKey* encryptionKey,
    /* [in] */ const String& macAlgorithm,
    /* [in] */ Elastos::Security::Spec::IAlgorithmParameterSpec* macSpec,
    /* [in] */ Elastos::Crypto::ISecretKey* macKey,
    /* [in] */ ArrayOf<Byte>* macTag,
    /* [in] */ Int64 authenticatedDataStart,
    /* [in] */ Int64 encryptedDataStart,
    /* [in] */ Int64 dataEnd)
{
    return Init(encryptionAlgorithm, encryptionSpec, encryptionKey, macAlgorithm,
                 macSpec, macKey, macTag, authenticatedDataStart, encryptedDataStart, dataEnd);
}

ECode CContainerEncryptionParams::constructor(
    /* [in] */ IParcel* source)
{
    // mEncryptionAlgorithm = source.readString();
    // final int encParamType = source.readInt();
    // final byte[] encParamsEncoded = source.createByteArray();
    // mEncryptionKey = (SecretKey) source.readSerializable();

    // mMacAlgorithm = source.readString();
    // final int macParamType = source.readInt();
    // source.createByteArray(); // byte[] macParamsEncoded
    // mMacKey = (SecretKey) source.readSerializable();

    // mMacTag = source.createByteArray();

    // mAuthenticatedDataStart = source.readLong();
    // mEncryptedDataStart = source.readLong();
    // mDataEnd = source.readLong();

    // switch (encParamType) {
    //     case ENC_PARAMS_IV_PARAMETERS:
    //         mEncryptionSpec = new IvParameterSpec(encParamsEncoded);
    //         break;
    //     default:
    //         throw new InvalidAlgorithmParameterException("Unknown parameter type "
    //                 + encParamType);
    // }

    // switch (macParamType) {
    //     case MAC_PARAMS_NONE:
    //         mMacSpec = null;
    //         break;
    //     default:
    //         throw new InvalidAlgorithmParameterException("Unknown parameter type "
    //                 + macParamType);
    // }

    // if (mEncryptionKey == null) {
    //     throw new NullPointerException("encryptionKey == null");
    // }

    return E_NOT_IMPLEMENTED;
}

ECode CContainerEncryptionParams::GetEncryptionAlgorithm(
    /* [out] */ String* ealgo)
{
    VALIDATE_NOT_NULL(ealgo);
    *ealgo = mEncryptionAlgorithm;
    return NOERROR;
}

ECode CContainerEncryptionParams::GetEncryptionSpec(
    /* [out] */ Elastos::Security::Spec::IAlgorithmParameterSpec** spec)
{
    // VALIDATE_NOT_NULL(spec);
    // *spec = (Elastos::Security::Spec::IIvParameterSpec*)mEncryptionSpec;
    // return NOERROR;
}

ECode CContainerEncryptionParams::GetEncryptionKey(
    /* [out] */ Elastos::Crypto::ISecretKey** key)
{
    VALIDATE_NOT_NULL(key);
    *key = mEncryptionKey;
    return NOERROR;
}

ECode CContainerEncryptionParams::GetMacAlgorithm(
    /* [out] */ String* malgo)
{
    VALIDATE_NOT_NULL(malgo);
    *malgo = mMacAlgorithm;
    return NOERROR;
}

ECode CContainerEncryptionParams::GetMacSpec(
    /* [out] */ Elastos::Security::Spec::IAlgorithmParameterSpec** spec)
{
    VALIDATE_NOT_NULL(spec);
    *spec = mMacSpec;
    return NOERROR;
}

ECode CContainerEncryptionParams::GetMacKey(
    /* [out] */ Elastos::Crypto::ISecretKey** key)
{
    VALIDATE_NOT_NULL(key);
    *key = mMacKey;
    return NOERROR;
}

ECode CContainerEncryptionParams::GetMacTag(
    /* [out, callee] */ ArrayOf<Byte>** tag)
{
    VALIDATE_NOT_NULL(tag);
    *tag = mMacTag;
    REFCOUNT_ADD(*tag);
    return NOERROR;
}

ECode CContainerEncryptionParams::GetAuthenticatedDataStart(
    /* [out] */ Int64* aDataStart)
{
    VALIDATE_NOT_NULL(aDataStart);
    *aDataStart = mAuthenticatedDataStart;
    return NOERROR;
}

ECode CContainerEncryptionParams::GetEncryptedDataStart(
    /* [out] */ Int64* eDataStart)
{
    VALIDATE_NOT_NULL(eDataStart);
    *eDataStart = mEncryptedDataStart;
    return NOERROR;
}

ECode CContainerEncryptionParams::GetDataEnd(
    /* [out] */ Int64* dataEnd)
{
    VALIDATE_NOT_NULL(dataEnd);
    *dataEnd = mDataEnd;
    return NOERROR;
}

ECode CContainerEncryptionParams::Equals(
    /* [in] */ IInterface* o,
    /* [out] */ Boolean* isEquals)
{
    VALIDATE_NOT_NULL(o);
    VALIDATE_NOT_NULL(isEquals);
    return E_NOT_IMPLEMENTED;
}

ECode CContainerEncryptionParams::GetHashCode(
    /* [out] */ Int32* code)
{
    VALIDATE_NOT_NULL(code);
    return E_NOT_IMPLEMENTED;
}

ECode CContainerEncryptionParams::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);
    return E_NOT_IMPLEMENTED;
}

ECode CContainerEncryptionParams::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    VALIDATE_NOT_NULL(source);
    return E_NOT_IMPLEMENTED;
}

ECode CContainerEncryptionParams::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    VALIDATE_NOT_NULL(dest);

    dest->WriteString(mEncryptionAlgorithm);
    dest->WriteInt32(ENC_PARAMS_IV_PARAMETERS);
    // dest->WriteArrayOf(mEncryptionSpec->getIV());
    dest->WriteInterfacePtr(mEncryptionKey);

    dest->WriteString(mMacAlgorithm);
    dest->WriteInt32(MAC_PARAMS_NONE);
    // dest->WriteByteArray(new byte[0]);
    dest->WriteInterfacePtr(mMacKey);

    dest->WriteArrayOf((Handle32)mMacTag.Get());

    dest->WriteInt64(mAuthenticatedDataStart);
    dest->WriteInt64(mEncryptedDataStart);
    dest->WriteInt64(mDataEnd);

    return NOERROR;
}

ECode CContainerEncryptionParams::Init(
    /* [in] */ const String& encryptionAlgorithm,
    /* [in] */ Elastos::Security::Spec::IAlgorithmParameterSpec* encryptionSpec,
    /* [in] */ Elastos::Crypto::ISecretKey* encryptionKey,
    /* [in] */ const String& macAlgorithm,
    /* [in] */ Elastos::Security::Spec::IAlgorithmParameterSpec* macSpec,
    /* [in] */ Elastos::Crypto::ISecretKey* macKey,
    /* [in] */ ArrayOf<Byte>* macTag,
    /* [in] */ Int64 authenticatedDataStart,
    /* [in] */ Int64 encryptedDataStart,
    /* [in] */ Int64 dataEnd)
{
    // if (TextUtils.isEmpty(encryptionAlgorithm)) {
    //     throw new NullPointerException("algorithm == null");
    // } else if (encryptionSpec == null) {
    //     throw new NullPointerException("encryptionSpec == null");
    // } else if (encryptionKey == null) {
    //     throw new NullPointerException("encryptionKey == null");
    // }

    // if (!TextUtils.isEmpty(macAlgorithm)) {
    //     if (macKey == null) {
    //         throw new NullPointerException("macKey == null");
    //     }
    // }

    // if (!(encryptionSpec instanceof IvParameterSpec)) {
    //     throw new InvalidAlgorithmParameterException(
    //             "Unknown parameter spec class; must be IvParameters");
    // }

    mEncryptionAlgorithm = encryptionAlgorithm;
    mEncryptionSpec = (Elastos::Security::Spec::IIvParameterSpec*)encryptionSpec;
    mEncryptionKey = encryptionKey;

    mMacAlgorithm = macAlgorithm;
    mMacSpec = macSpec;
    mMacKey = macKey;
    mMacTag = macTag;

    mAuthenticatedDataStart = authenticatedDataStart;
    mEncryptedDataStart = encryptedDataStart;
    mDataEnd = dataEnd;

    return NOERROR;
}

}
}
}
}

