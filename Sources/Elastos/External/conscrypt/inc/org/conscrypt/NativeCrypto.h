
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

private:
    static const Boolean sInitialized;

private:
    static CARAPI_(Boolean) Clinit();
};

} // namespace Conscrypt
} // namespace Org

#endif //__NATIVECRYPTO_H__
