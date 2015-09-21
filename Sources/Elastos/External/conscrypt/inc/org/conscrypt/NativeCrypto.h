
#ifndef __NATIVECRYPTO_H__
#define __NATIVECRYPTO_H__

#include "Org.Conscrypt_server.h"

using Elastos::Security::Interfaces::IDSAPrivateKey;
using Elastos::Security::Interfaces::IECPrivateKey;
using Elastos::Security::Interfaces::IRSAPrivateKey;

namespace Org {
namespace Conscrypt {

class NativeCrypto
{
public:
    static CARAPI ENGINE_load_dynamic();

    static CARAPI ENGINE_by_id(
        /* [in] */ const String& id,
        /* [out] */ Int64* result);

    static CARAPI ENGINE_add(
        /* [in] */ Int64 engineRef,
        /* [out] */ Int32* result);

    static CARAPI ENGINE_init(
        /* [in] */ Int64 engineRef,
        /* [out] */ Int32* result);

    static CARAPI ENGINE_finish(
        /* [in] */ Int64 engineRef,
        /* [out] */ Int32* result);

    static CARAPI ENGINE_free(
        /* [in] */ Int64 engineRef,
        /* [out] */ Int32* result);

    static CARAPI ENGINE_load_private_key(
        /* [in] */ Int64 engineRef,
        /* [in] */ const String& key_id,
        /* [out] */ Int64* result);

    static CARAPI ENGINE_get_id(
        /* [in] */ Int64 engineRef,
        /* [out] */ String* result);

    static CARAPI ENGINE_ctrl_cmd_string(
        /* [in] */ Int64 engineRef,
        /* [in] */ const String& cmd,
        /* [in] */ const String& arg,
        /* [in] */ Int32 cmd_optional,
        /* [out] */ Int32* result);

    static CARAPI EVP_PKEY_new_DSA(
        /* [in] */ ArrayOf<Byte>* p,
        /* [in] */ ArrayOf<Byte>* q,
        /* [in] */ ArrayOf<Byte>* g,
        /* [in] */ ArrayOf<Byte>* pub_key,
        /* [in] */ ArrayOf<Byte>* priv_key,
        /* [out] */ Int64* result);

    static CARAPI EVP_PKEY_new_RSA(
        /* [in] */ ArrayOf<Byte>* n,
        /* [in] */ ArrayOf<Byte>* e,
        /* [in] */ ArrayOf<Byte>* d,
        /* [in] */ ArrayOf<Byte>* p,
        /* [in] */ ArrayOf<Byte>* q,
        /* [in] */ ArrayOf<Byte>* dmp1,
        /* [in] */ ArrayOf<Byte>* dmq1,
        /* [in] */ ArrayOf<Byte>* iqmp,
        /* [out] */ Int64* result);

    static CARAPI EVP_PKEY_new_mac_key(
        /* [in] */ Int32 type,
        /* [in] */ ArrayOf<Byte>* key,
        /* [out] */ Int64* result);

    static CARAPI EVP_PKEY_size(
        /* [in] */ Int64 pkeyRef,
        /* [out] */ Int32* result);

    static CARAPI EVP_PKEY_type(
        /* [in] */ Int64 pkeyRef,
        /* [out] */ Int32* result);

    static CARAPI EVP_PKEY_print_public(
        /* [in] */ Int64 pkeyRef,
        /* [out] */ String* result);

    static CARAPI EVP_PKEY_print_private(
        /* [in] */ Int64 pkeyRef,
        /* [out] */ String* result);

    static CARAPI EVP_PKEY_free(
        /* [in] */ Int64 pkeyRef);

    static CARAPI EVP_PKEY_cmp(
        /* [in] */ Int64 pkey1Ref,
        /* [in] */ Int64 pkey2Ref,
        /* [out] */ Int32* result);

    static CARAPI I2d_PKCS8_PRIV_KEY_INFO(
        /* [in] */ Int64 pkeyRef,
        /* [out, callee] */ ArrayOf<Byte>** result);

    static CARAPI D2i_PKCS8_PRIV_KEY_INFO(
        /* [in] */ ArrayOf<Byte>* keyBytes,
        /* [out] */ Int64* result);

    static CARAPI I2d_PUBKEY(
        /* [in] */ Int64 pkeyRef,
        /* [out, callee] */ ArrayOf<Byte>** result);

    static CARAPI D2i_PUBKEY(
        /* [in] */ ArrayOf<Byte>* data,
        /* [out] */ Int64* result);

    static CARAPI GetRSAPrivateKeyWrapper(
        /* [in] */ IRSAPrivateKey* key,
        /* [in] */ ArrayOf<Byte>* modulus,
        /* [out] */ Int64* result);

    static CARAPI GetDSAPrivateKeyWrapper(
        /* [in] */ IDSAPrivateKey* key,
        /* [out] */ Int64* result);

    static CARAPI GetECPrivateKeyWrapper(
        /* [in] */ IECPrivateKey* key,
        /* [in] */ Int64 ecGroupRef,
        /* [out] */ Int64* result);

    static CARAPI RSA_generate_key_ex(
        /* [in] */ Int32 modulusBits,
        /* [in] */ ArrayOf<Byte>* publicExponent,
        /* [out] */ Int64* result);

    static CARAPI RSA_size(
        /* [in] */ Int64 pkeyRef,
        /* [out] */ Int32* result);

    static CARAPI RSA_private_encrypt(
        /* [in] */ Int32 flen,
        /* [in] */ ArrayOf<Byte>* from,
        /* [in] */ ArrayOf<Byte>* to,
        /* [in] */ Int64 pkeyRef,
        /* [in] */ Int32 padding,
        /* [out] */ Int32* result);

    static CARAPI RSA_public_decrypt(
        /* [in] */ Int32 flen,
        /* [in] */ ArrayOf<Byte>* from,
        /* [in] */ ArrayOf<Byte>* to,
        /* [in] */ Int64 pkeyRef,
        /* [in] */ Int32 padding,
        /* [out] */ Int32* result);

    static CARAPI RSA_public_encrypt(
        /* [in] */ Int32 flen,
        /* [in] */ ArrayOf<Byte>* from,
        /* [in] */ ArrayOf<Byte>* to,
        /* [in] */ Int64 pkeyRef,
        /* [in] */ Int32 padding,
        /* [out] */ Int32* result);

    static CARAPI RSA_private_decrypt(
        /* [in] */ Int32 flen,
        /* [in] */ ArrayOf<Byte>* from,
        /* [in] */ ArrayOf<Byte>* to,
        /* [in] */ Int64 pkeyRef,
        /* [in] */ Int32 padding,
        /* [out] */ Int32* result);

    static CARAPI Get_RSA_public_params(
        /* [in] */ Int64 pkeyRef,
        /* [out, callee] */ ArrayOf<Byte>** n,
        /* [out, callee] */ ArrayOf<Byte>** e);

    static CARAPI Get_RSA_private_params(
        /* [in] */ Int64 pkeyRef,
        /* [out, callee] */ ArrayOf<Byte>** n,
        /* [out, callee] */ ArrayOf<Byte>** e,
        /* [out, callee] */ ArrayOf<Byte>** d,
        /* [out, callee] */ ArrayOf<Byte>** p,
        /* [out, callee] */ ArrayOf<Byte>** q,
        /* [out, callee] */ ArrayOf<Byte>** dmp1,
        /* [out, callee] */ ArrayOf<Byte>** dmq1,
        /* [out, callee] */ ArrayOf<Byte>** iqmp);

    static CARAPI DSA_generate_key(
        /* [in] */ Int32 primeBits,
        /* [in] */ ArrayOf<Byte>* seed,
        /* [in] */ ArrayOf<Byte>* g,
        /* [in] */ ArrayOf<Byte>* p,
        /* [in] */ ArrayOf<Byte>* q,
        /* [out] */ Int64* result);

    static CARAPI Get_DSA_params(
        /* [in] */ Int64 pkeyRef,
        /* [out, callee] */ ArrayOf<Byte>** g,
        /* [out, callee] */ ArrayOf<Byte>** p,
        /* [out, callee] */ ArrayOf<Byte>** q,
        /* [out, callee] */ ArrayOf<Byte>** ypub,
        /* [out, callee] */ ArrayOf<Byte>** xpriv);

    static CARAPI Set_DSA_flag_nonce_from_hash(
        /* [in] */ Int64 pkeyRef);

    static CARAPI I2d_RSAPublicKey(
        /* [in] */ Int64 pkeyRef,
        /* [out, callee] */ ArrayOf<Byte>** result);

    static CARAPI I2d_RSAPrivateKey(
        /* [in] */ Int64 pkeyRef,
        /* [out, callee] */ ArrayOf<Byte>** result);

    static CARAPI I2d_DSAPublicKey(
        /* [in] */ Int64 pkeyRef,
        /* [out, callee] */ ArrayOf<Byte>** result);

    static CARAPI I2d_DSAPrivateKey(
        /* [in] */ Int64 pkeyRef,
        /* [out, callee] */ ArrayOf<Byte>** result);

    static CARAPI EVP_PKEY_new_DH(
        /* [in] */ ArrayOf<Byte>* p,
        /* [in] */ ArrayOf<Byte>* g,
        /* [in] */ ArrayOf<Byte>* pub_key,
        /* [in] */ ArrayOf<Byte>* priv_key,
        /* [out] */ Int64* result);

    static CARAPI DH_generate_parameters_ex(
        /* [in] */ Int32 primeBits,
        /* [in] */ Int64 generator,
        /* [out] */ Int64* result);

    static CARAPI DH_generate_key(
        /* [in] */ Int64 pkeyRef);

    static CARAPI Get_DH_params(
        /* [in] */ Int64 pkeyRef,
        /* [out, callee] */ ArrayOf<Byte>** p,
        /* [out, callee] */ ArrayOf<Byte>** g,
        /* [out, callee] */ ArrayOf<Byte>** ypub,
        /* [out, callee] */ ArrayOf<Byte>** xpriv);

    static CARAPI EVP_PKEY_new_EC_KEY(
        /* [in] */ Int64 groupRef,
        /* [in] */ Int64 pubkeyRef,
        /* [in] */ ArrayOf<Byte>* keyBytes,
        /* [out] */ Int64* result);

    static CARAPI EC_GROUP_new_by_curve_name(
        /* [in] */ const String& curveName,
        /* [out] */ Int64* result);

    static CARAPI EC_GROUP_new_curve(
        /* [in] */ Int32 type,
        /* [in] */ ArrayOf<Byte>* p,
        /* [in] */ ArrayOf<Byte>* a,
        /* [in] */ ArrayOf<Byte>* b,
        /* [out] */ Int64* result);

    static CARAPI EC_GROUP_dup(
        /* [in] */ Int64 groupRef,
        /* [out] */ Int64* result);

    static CARAPI EC_GROUP_set_asn1_flag(
        /* [in] */ Int64 groupRef,
        /* [in] */ Int32 flag);

    static CARAPI EC_GROUP_set_point_conversion_form(
        /* [in] */ Int64 groupRef,
        /* [in] */ Int32 form);

    static CARAPI EC_GROUP_get_curve_name(
        /* [in] */ Int64 groupRef,
        /* [out] */ String* result);

    static CARAPI EC_GROUP_get_curve(
        /* [in] */ Int64 groupRef,
        /* [out, callee] */ ArrayOf<Byte>** p,
        /* [out, callee] */ ArrayOf<Byte>** a,
        /* [out, callee] */ ArrayOf<Byte>** b);

    static CARAPI EC_GROUP_clear_free(
        /* [in] */ Int64 groupRef);

    static CARAPI EC_GROUP_cmp(
        /* [in] */ Int64 group1Ref,
        /* [in] */ Int64 group2Ref,
        /* [out] */ Boolean* result);

    static CARAPI EC_GROUP_set_generator(
        /* [in] */ Int64 groupRef,
        /* [in] */ Int64 pointRef,
        /* [in] */ ArrayOf<Byte>* nBytes,
        /* [in] */ ArrayOf<Byte>* hBytes);

    static CARAPI EC_GROUP_get_generator(
        /* [in] */ Int64 groupRef,
        /* [out] */ Int64* result);

    static CARAPI Get_EC_GROUP_type(
        /* [in] */ Int64 groupRef,
        /* [out] */ Int32* result);

    static CARAPI EC_GROUP_get_order(
        /* [in] */ Int64 groupRef,
        /* [out, callee] */ ArrayOf<Byte>** result);

    static CARAPI EC_GROUP_get_degree(
        /* [in] */ Int64 groupRef,
        /* [out] */ Int32* result);

    static CARAPI EC_GROUP_get_cofactor(
        /* [in] */ Int64 groupRef,
        /* [out, callee] */ ArrayOf<Byte>** result);

    static CARAPI EC_POINT_new(
        /* [in] */ Int64 groupRef,
        /* [out] */ Int64* result);

    static CARAPI EC_POINT_clear_free(
        /* [in] */ Int64 pointRef);

    static CARAPI EC_POINT_cmp(
        /* [in] */ Int64 groupRef,
        /* [in] */ Int64 point1Ref,
        /* [in] */ Int64 point2Ref,
        /* [out] */ Boolean* result);

    static CARAPI EC_POINT_get_affine_coordinates(
        /* [in] */ Int64 groupRef,
        /* [in] */ Int64 pointRef,
        /* [out, callee] */ ArrayOf<Byte>** xBytes,
        /* [out, callee] */ ArrayOf<Byte>** yBytes);

    static CARAPI EC_POINT_set_affine_coordinates(
        /* [in] */ Int64 groupRef,
        /* [in] */ Int64 pointRef,
        /* [in] */ ArrayOf<Byte>* xBytes,
        /* [in] */ ArrayOf<Byte>* yBytes);

    static CARAPI EC_KEY_generate_key(
        /* [in] */ Int64 groupRef,
        /* [out] */ Int64* result);

    static CARAPI EC_KEY_get0_group(
        /* [in] */ Int64 pkeyRef,
        /* [out] */ Int64* result);

    static CARAPI EC_KEY_get_private_key(
        /* [in] */ Int64 pkeyRef,
        /* [out, callee] */ ArrayOf<Byte>** result);

    static CARAPI EC_KEY_get_public_key(
        /* [in] */ Int64 pkeyRef,
        /* [out] */ Int64* result);

    static CARAPI EC_KEY_set_nonce_from_hash(
        /* [in] */ Int64 pkeyRef,
        /* [in] */ Boolean enabled);

    static CARAPI ECDH_compute_key(
        /* [in] */ ArrayOf<Byte>* outArray,
        /* [in] */ Int32 outOffset,
        /* [in] */ Int64 publicKeyRef,
        /* [in] */ Int64 privateKeyRef,
        /* [out] */ Int32* result);

private:
    static const Boolean sInitialized;

private:
    static CARAPI_(Boolean) Clinit();
};

} // namespace Conscrypt
} // namespace Org

#endif //__NATIVECRYPTO_H__
