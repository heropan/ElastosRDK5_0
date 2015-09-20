
#include "elastos/coredef.h"
#include "NativeCrypto.h"
#include "CryptoUpcalls.h"
#include <elastos/core/UniquePtr.h>
#include <elastos/utility/logging/Logger.h>

#include <openssl/asn1t.h>
#include <openssl/dsa.h>
#include <openssl/engine.h>
#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/rsa.h>
#include <openssl/ssl.h>
#include <openssl/x509v3.h>
#include <openssl/crypto/ecdsa/ecs_locl.h>

using Elastos::Core::UniquePtr;
using Elastos::Security::IPrivateKey;
using Elastos::Utility::Logging::Logger;

namespace Org {
namespace Conscrypt {

#define LOG_TAG "NativeCrypto"

#undef WITH_NATIVE_TRACE
#undef WITH_NATIVE_TRACE_MD
#undef WITH_NATIVE_TRACE_DATA

/*
 * How to use this for debugging with Wireshark:
 *
 * 1. Pull lines from logcat to a file that looks like (without quotes):
 *     "RSA Session-ID:... Master-Key:..." <CR>
 *     "RSA Session-ID:... Master-Key:..." <CR>
 *     <etc>
 * 2. Start Wireshark
 * 3. Go to Edit -> Preferences -> SSL -> (Pre-)Master-Key log and fill in
 *    the file you put the lines in above.
 * 4. Follow the stream that corresponds to the desired "Session-ID" in
 *    the Server Hello.
 */
#undef WITH_NATIVE_TRACE_KEYS

#ifdef WITH_NATIVE_TRACE
#define NATIVE_TRACE(...) \
        ((void)ALOG(LOG_INFO, LOG_TAG "-jni", __VA_ARGS__));     \
/*
        ((void)printf("I/" LOG_TAG "-jni:"));         \
        ((void)printf(__VA_ARGS__));          \
        ((void)printf("\n"))
*/
#else
#define NATIVE_TRACE(...) ((void)0)
#endif
#ifdef WITH_NATIVE_TRACE_MD
#define NATIVE_TRACE_MD(...) \
        ((void)ALOG(LOG_INFO, LOG_TAG "-jni", __VA_ARGS__));
#else
#define NATIVE_TRACE_MD(...) ((void)0)
#endif

struct BIO_Delete {
    void operator()(BIO* p) const {
        BIO_free_all(p);
    }
};
typedef UniquePtr<BIO, BIO_Delete> Unique_BIO;

struct DSA_Delete {
    void operator()(DSA* p) const {
        DSA_free(p);
    }
};
typedef UniquePtr<DSA, DSA_Delete> Unique_DSA;

struct EC_KEY_Delete {
    void operator()(EC_KEY* p) const {
        EC_KEY_free(p);
    }
};
typedef UniquePtr<EC_KEY, EC_KEY_Delete> Unique_EC_KEY;

struct EVP_PKEY_Delete {
    void operator()(EVP_PKEY* p) const {
        EVP_PKEY_free(p);
    }
};
typedef UniquePtr<EVP_PKEY, EVP_PKEY_Delete> Unique_EVP_PKEY;

struct PKCS8_PRIV_KEY_INFO_Delete {
    void operator()(PKCS8_PRIV_KEY_INFO* p) const {
        PKCS8_PRIV_KEY_INFO_free(p);
    }
};
typedef UniquePtr<PKCS8_PRIV_KEY_INFO, PKCS8_PRIV_KEY_INFO_Delete> Unique_PKCS8_PRIV_KEY_INFO;

struct RSA_Delete {
    void operator()(RSA* p) const {
        RSA_free(p);
    }
};
typedef UniquePtr<RSA, RSA_Delete> Unique_RSA;

/**
 * Many OpenSSL APIs take ownership of an argument on success but don't free the argument
 * on failure. This means we need to tell our scoped pointers when we've transferred ownership,
 * without triggering a warning by not using the result of release().
 */
#define OWNERSHIP_TRANSFERRED(obj) \
    do { typeof (obj.release()) _dummy __attribute__((unused)) = obj.release(); } while(0)

/**
 * Frees the SSL error state.
 *
 * OpenSSL keeps an "error stack" per thread, and given that this code
 * can be called from arbitrary threads that we don't keep track of,
 * we err on the side of freeing the error state promptly (instead of,
 * say, at thread death).
 */
static void FreeOpenSslErrorState()
{
    ERR_clear_error();
    ERR_remove_state(0);
}

static ECode ThrowRuntimeException(const char* message)
{
    NATIVE_TRACE("ThrowRuntimeException %s", message);
    return E_RUNTIME_EXCEPTION;
}

/**
 * Throws a BadPaddingException with the given string as a message.
 */
static ECode ThrowBadPaddingException(const char* message)
{
    NATIVE_TRACE("ThrowBadPaddingException %s", message);
    return E_BAD_PADDING_EXCEPTION;
}

/**
 * Throws a SignatureException with the given string as a message.
 */
static ECode ThrowSignatureException(const char* message)
{
    NATIVE_TRACE("ThrowSignatureException %s", message);
    return E_SIGNATURE_EXCEPTION;
}

/**
 * Throws a InvalidKeyException with the given string as a message.
 */
static ECode ThrowInvalidKeyException(const char* message)
{
    NATIVE_TRACE("ThrowInvalidKeyException %s", message);
    return E_INVALID_KEY_EXCEPTION;
}

/**
 * Throws a SignatureException with the given string as a message.
 */
static ECode ThrowIllegalBlockSizeException(const char* message)
{
    NATIVE_TRACE("ThrowIllegalBlockSizeException %s", message);
    return E_ILLEGAL_BLOCK_SIZE_EXCEPTION;
}

/**
 * Throws a NoSuchAlgorithmException with the given string as a message.
 */
static ECode ThrowNoSuchAlgorithmException(const char* message)
{
    NATIVE_TRACE("ThrowUnknownAlgorithmException %s", message);
    return E_NO_SUCH_ALGORITHM_EXCEPTION;
}

static ECode ThrowForAsn1Error(int reason, const char *message)
{
    switch (reason) {
    case ASN1_R_UNABLE_TO_DECODE_RSA_KEY:
    case ASN1_R_UNABLE_TO_DECODE_RSA_PRIVATE_KEY:
    case ASN1_R_UNKNOWN_PUBLIC_KEY_TYPE:
    case ASN1_R_UNSUPPORTED_PUBLIC_KEY_TYPE:
    case ASN1_R_WRONG_PUBLIC_KEY_TYPE:
        return ThrowInvalidKeyException(message);
    case ASN1_R_UNKNOWN_MESSAGE_DIGEST_ALGORITHM:
        return ThrowNoSuchAlgorithmException(message);
    default:
        return ThrowRuntimeException(message);
    }
}

static ECode ThrowForEvpError(int reason, const char *message)
{
    switch (reason) {
    case EVP_R_BAD_DECRYPT:
        return ThrowBadPaddingException(message);
    case EVP_R_DATA_NOT_MULTIPLE_OF_BLOCK_LENGTH:
    case EVP_R_WRONG_FINAL_BLOCK_LENGTH:
        return ThrowIllegalBlockSizeException(message);
    case EVP_R_BAD_KEY_LENGTH:
    case EVP_R_BN_DECODE_ERROR:
    case EVP_R_BN_PUBKEY_ERROR:
    case EVP_R_INVALID_KEY_LENGTH:
    case EVP_R_MISSING_PARAMETERS:
    case EVP_R_UNSUPPORTED_KEY_SIZE:
    case EVP_R_UNSUPPORTED_KEYLENGTH:
        return ThrowInvalidKeyException(message);
    case EVP_R_WRONG_PUBLIC_KEY_TYPE:
        return ThrowSignatureException(message);
    case EVP_R_UNSUPPORTED_ALGORITHM:
        return ThrowNoSuchAlgorithmException(message);
    default:
        return ThrowRuntimeException(message);
    }
}

static ECode ThrowForRsaError(int reason, const char *message)
{
    switch (reason) {
    case RSA_R_BLOCK_TYPE_IS_NOT_01:
    case RSA_R_BLOCK_TYPE_IS_NOT_02:
        return ThrowBadPaddingException(message);
    case RSA_R_ALGORITHM_MISMATCH:
    case RSA_R_BAD_SIGNATURE:
    case RSA_R_DATA_GREATER_THAN_MOD_LEN:
    case RSA_R_DATA_TOO_LARGE_FOR_MODULUS:
    case RSA_R_INVALID_MESSAGE_LENGTH:
    case RSA_R_WRONG_SIGNATURE_LENGTH:
        return ThrowSignatureException(message);
    case RSA_R_UNKNOWN_ALGORITHM_TYPE:
        return ThrowNoSuchAlgorithmException(message);
    case RSA_R_MODULUS_TOO_LARGE:
    case RSA_R_NO_PUBLIC_EXPONENT:
        return ThrowInvalidKeyException(message);
    default:
        return ThrowRuntimeException(message);
    }
}

static ECode ThrowForX509Error(int reason, const char *message)
{
    switch (reason) {
    case X509_R_UNSUPPORTED_ALGORITHM:
        return ThrowNoSuchAlgorithmException(message);
    default:
        return ThrowRuntimeException(message);
    }
}

/*
 * Checks this thread's OpenSSL error queue and throws a RuntimeException if
 * necessary.
 *
 * @return true if an exception was thrown, false if not.
 */
static ECode ThrowExceptionIfNecessary(const char* location  __attribute__ ((unused)))
{
    const char* file;
    int line;
    const char* data;
    int flags;
    unsigned long error = ERR_get_error_line_data(&file, &line, &data, &flags);
    ECode ec = NOERROR;

    if (error != 0) {
        char message[256];
        ERR_error_string_n(error, message, sizeof(message));
        int library = ERR_GET_LIB(error);
        int reason = ERR_GET_REASON(error);
        NATIVE_TRACE("OpenSSL error in %s error=%lx library=%x reason=%x (%s:%d): %s %s",
                  location, error, library, reason, file, line, message,
                  (flags & ERR_TXT_STRING) ? data : "(no data)");
        switch (library) {
        case ERR_LIB_RSA:
            ec = ThrowForRsaError(reason, message);
            break;
        case ERR_LIB_ASN1:
            ec = ThrowForAsn1Error(reason, message);
            break;
        case ERR_LIB_EVP:
            ec = ThrowForEvpError(reason, message);
            break;
        case ERR_LIB_X509:
            ec = ThrowForX509Error(reason, message);
            break;
        case ERR_LIB_DSA:
            ec = ThrowInvalidKeyException(message);
            break;
        default:
            ec = ThrowRuntimeException(message);
            break;
        }
    }

    FreeOpenSslErrorState();
    return ec;
}

/**
 * Converts a Java byte[] two's complement to an OpenSSL BIGNUM. This will
 * allocate the BIGNUM if *dest == NULL. Returns true on success. If the
 * return value is false, there is a pending exception.
 */
static Boolean ArrayToBignum(ArrayOf<Byte>* source, BIGNUM** dest, ECode* ec)
{
    *ec = NOERROR;
    NATIVE_TRACE("ArrayToBignum(%p, %p)", source, dest);
    if (dest == NULL) {
        NATIVE_TRACE("ArrayToBignum(%p, %p) => dest is null!", source, dest);
        *ec = E_NULL_POINTER_EXCEPTION;
        return FALSE;
    }
    NATIVE_TRACE("ArrayToBignum(%p, %p) *dest == %p", source, dest, *dest);

    if (source == NULL) {
        NATIVE_TRACE("ArrayToBignum(%p, %p) => NULL", source, dest);
        return FALSE;
    }
    const unsigned char* tmp = reinterpret_cast<const unsigned char*>(source);
    size_t tmpSize = source->GetLength();

    /* if the array is empty, it is zero. */
    if (tmpSize == 0) {
        if (*dest == NULL) {
            *dest = BN_new();
        }
        BN_zero(*dest);
        return TRUE;
    }

    UniquePtr<unsigned char[]> twosComplement;
    Boolean negative = (tmp[0] & 0x80) != 0;
    if (negative) {
        // Need to convert to two's complement.
        twosComplement.reset(new unsigned char[tmpSize]);
        unsigned char* twosBytes = reinterpret_cast<unsigned char*>(twosComplement.get());
        memcpy(twosBytes, tmp, tmpSize);
        tmp = twosBytes;

        Boolean carry = true;
        for (ssize_t i = tmpSize - 1; i >= 0; i--) {
            twosBytes[i] ^= 0xFF;
            if (carry) {
                carry = (++twosBytes[i]) == 0;
            }
        }
    }
    BIGNUM *ret = BN_bin2bn(tmp, tmpSize, *dest);
    if (ret == NULL) {
        *ec = ThrowRuntimeException("Conversion to BIGNUM failed");
        NATIVE_TRACE("ArrayToBignum(%p, %p) => threw exception", source, dest);
        return FALSE;
    }
    BN_set_negative(ret, negative ? 1 : 0);

    *dest = ret;
    NATIVE_TRACE("ArrayToBignum(%p, %p) => *dest = %p", source, dest, ret);
    return TRUE;
}

/**
 * Converts various OpenSSL ASN.1 types to a jbyteArray with DER-encoded data
 * inside. The "i2d_func" function pointer is a function of the "i2d_<TYPE>"
 * from the OpenSSL ASN.1 API.
 */
template<typename T, int (*i2d_func)(T*, unsigned char**)>
ECode ASN1ToByteArray(
    /* [in] */ T* obj,
    /* [out, callee] */ ArrayOf<Byte>** result)
{
    if (obj == NULL) {
        NATIVE_TRACE("ASN1ToByteArray(%p) => null input", obj);
        *result = NULL;
        return E_NULL_POINTER_EXCEPTION;
    }

    int derLen = i2d_func(obj, NULL);
    if (derLen < 0) {
        NATIVE_TRACE("ASN1ToByteArray(%p) => measurement failed", obj);
        *result = NULL;
        return ThrowExceptionIfNecessary("ASN1ToByteArray");
    }

    AutoPtr< ArrayOf<Byte> > byteArray = ArrayOf<Byte>::Alloc(derLen);

    unsigned char* p = reinterpret_cast<unsigned char*>(byteArray->GetPayload());
    int ret = i2d_func(obj, &p);
    if (ret < 0) {
        NATIVE_TRACE("ASN1ToByteArray(%p) => final conversion failed", obj);
        *result = NULL;
        return ThrowExceptionIfNecessary("ASN1ToByteArray");
    }

    NATIVE_TRACE("ASN1ToByteArray(%p) => success (%d bytes written)", obj, ret);
    *result = byteArray;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

static AutoPtr< ArrayOf<Byte> > RawSignDigestWithPrivateKey(IPrivateKey* privateKey,
    const char* message, size_t message_len)
{
    AutoPtr< ArrayOf<Byte> > messageArray = ArrayOf<Byte>::Alloc(message_len);

    memcpy(messageArray->GetPayload(), message, message_len);

    AutoPtr< ArrayOf<Byte> > result;
    CryptoUpcalls::RawSignDigestWithPrivateKey(
            privateKey, messageArray, (ArrayOf<Byte>**)&result);
    return result;
}

static AutoPtr< ArrayOf<Byte> > RawCipherWithPrivateKey(IPrivateKey* privateKey,
    Boolean encrypt, const char* ciphertext, size_t ciphertext_len)
{
    AutoPtr< ArrayOf<Byte> > ciphertextArray = ArrayOf<Byte>::Alloc(ciphertext_len);

    memcpy(ciphertextArray->GetPayload(), ciphertext, ciphertext_len);

    AutoPtr< ArrayOf<Byte> > result;
    CryptoUpcalls::RawCipherWithPrivateKey(
            privateKey, encrypt, ciphertextArray, (ArrayOf<Byte>**)&result);
    return result;
}

// *********************************************
// From keystore_openssl.cpp in Chromium source.
// *********************************************

// Custom RSA_METHOD that uses the platform APIs.
// Note that for now, only signing through RSA_sign() is really supported.
// all other method pointers are either stubs returning errors, or no-ops.
// See <openssl/rsa.h> for exact declaration of RSA_METHOD.

int RsaMethodPubEnc(int flen,
                    const unsigned char* from,
                    unsigned char* to,
                    RSA* rsa,
                    int padding)
{
    RSAerr(RSA_F_RSA_PUBLIC_ENCRYPT, RSA_R_RSA_OPERATIONS_NOT_SUPPORTED);
    return -1;
}

int RsaMethodPubDec(int flen,
                    const unsigned char* from,
                    unsigned char* to,
                    RSA* rsa,
                    int padding)
{
    RSAerr(RSA_F_RSA_PUBLIC_DECRYPT, RSA_R_RSA_OPERATIONS_NOT_SUPPORTED);
    return -1;
}

// See RSA_eay_private_encrypt in
// third_party/openssl/openssl/crypto/rsa/rsa_eay.c for the default
// implementation of this function.
int RsaMethodPrivEnc(int flen,
                     const unsigned char *from,
                     unsigned char *to,
                     RSA *rsa,
                     int padding)
{
    if (padding != RSA_PKCS1_PADDING) {
        // TODO(davidben): If we need to, we can implement RSA_NO_PADDING
        // by using javax.crypto.Cipher and picking either the
        // "RSA/ECB/NoPadding" or "RSA/ECB/PKCS1Padding" transformation as
        // appropriate. I believe support for both of these was added in
        // the same Android version as the "NONEwithRSA"
        // java.security.Signature algorithm, so the same version checks
        // for GetRsaLegacyKey should work.
        RSAerr(RSA_F_RSA_PRIVATE_ENCRYPT, RSA_R_UNKNOWN_PADDING_TYPE);
        return -1;
    }

    // Retrieve private key JNI reference.
    IRSAPrivateKey* private_key = reinterpret_cast<IRSAPrivateKey*>(RSA_get_app_data(rsa));
    if (!private_key) {
        Logger::E(LOG_TAG, "Null JNI reference passed to RsaMethodPrivEnc!");
        RSAerr(RSA_F_RSA_PRIVATE_ENCRYPT, ERR_R_INTERNAL_ERROR);
        return -1;
    }

    // For RSA keys, this function behaves as RSA_private_encrypt with
    // PKCS#1 padding.
    AutoPtr< ArrayOf<Byte> > signature = RawSignDigestWithPrivateKey(
            IPrivateKey::Probe(private_key),
            reinterpret_cast<const char*>(from), flen);
    if (signature == NULL) {
        Logger::E(LOG_TAG, "Could not sign message in RsaMethodPrivEnc!");
        RSAerr(RSA_F_RSA_PRIVATE_ENCRYPT, ERR_R_INTERNAL_ERROR);
        return -1;
    }

    size_t expected_size = static_cast<size_t>(RSA_size(rsa));
    if ((size_t)signature->GetLength() > expected_size) {
        Logger::E(LOG_TAG, "RSA Signature size mismatch, actual: %zd, expected <= %zd", signature->GetLength(),
              expected_size);
        RSAerr(RSA_F_RSA_PRIVATE_ENCRYPT, ERR_R_INTERNAL_ERROR);
        return -1;
    }

    // Copy result to OpenSSL-provided buffer. rawSignDigestWithPrivateKey
    // should pad with leading 0s, but if it doesn't, pad the result.
    size_t zero_pad = expected_size - signature->GetLength();
    memset(to, 0, zero_pad);
    memcpy(to + zero_pad, signature->GetPayload(), signature->GetLength());

    return expected_size;
}

int RsaMethodPrivDec(int flen,
                     const unsigned char* from,
                     unsigned char* to,
                     RSA* rsa,
                     int padding)
{
    if (padding != RSA_PKCS1_PADDING) {
        RSAerr(RSA_F_RSA_PRIVATE_DECRYPT, RSA_R_UNKNOWN_PADDING_TYPE);
        return -1;
    }

    // Retrieve private key JNI reference.
    IRSAPrivateKey* private_key = reinterpret_cast<IRSAPrivateKey*>(RSA_get_app_data(rsa));
    if (!private_key) {
        Logger::E(LOG_TAG, "Null JNI reference passed to RsaMethodPrivDec!");
        RSAerr(RSA_F_RSA_PRIVATE_DECRYPT, ERR_R_INTERNAL_ERROR);
        return -1;
    }

    // For RSA keys, this function behaves as RSA_private_decrypt with
    // PKCS#1 padding.
    AutoPtr< ArrayOf<Byte> > cleartext = RawCipherWithPrivateKey(
            IPrivateKey::Probe(private_key), FALSE,
            reinterpret_cast<const char*>(from), flen);
    if (cleartext == NULL) {
        Logger::E(LOG_TAG, "Could not decrypt message in RsaMethodPrivDec!");
        RSAerr(RSA_F_RSA_PRIVATE_DECRYPT, ERR_R_INTERNAL_ERROR);
        return -1;
    }

    size_t expected_size = static_cast<size_t>(RSA_size(rsa));
    if ((size_t)cleartext->GetLength() > expected_size) {
        Logger::E(LOG_TAG, "RSA ciphertext size mismatch, actual: %zd, expected <= %zd", cleartext->GetLength(),
                expected_size);
        RSAerr(RSA_F_RSA_PRIVATE_DECRYPT, ERR_R_INTERNAL_ERROR);
        return -1;
    }

    // Copy result to OpenSSL-provided buffer.
    memcpy(to, cleartext->GetPayload(), cleartext->GetLength());

    return cleartext->GetLength();
}

int RsaMethodInit(RSA*)
{
    return 0;
}

int RsaMethodFinish(RSA* rsa)
{
    // Ensure the global JNI reference created with this wrapper is
    // properly destroyed with it.
    IRSAPrivateKey* key = reinterpret_cast<IRSAPrivateKey*>(RSA_get_app_data(rsa));
    if (key != NULL) {
        RSA_set_app_data(rsa, NULL);
        key->Release();
    }
    // Actual return value is ignored by OpenSSL. There are no docs
    // explaining what this is supposed to be.
    return 0;
}

const RSA_METHOD elastos_rsa_method = {
        /* .name = */ "Elastos signing-only RSA method",
        /* .rsa_pub_enc = */ RsaMethodPubEnc,
        /* .rsa_pub_dec = */ RsaMethodPubDec,
        /* .rsa_priv_enc = */ RsaMethodPrivEnc,
        /* .rsa_priv_dec = */ RsaMethodPrivDec,
        /* .rsa_mod_exp = */ NULL,
        /* .bn_mod_exp = */ NULL,
        /* .init = */ RsaMethodInit,
        /* .finish = */ RsaMethodFinish,
        // This flag is necessary to tell OpenSSL to avoid checking the content
        // (i.e. internal fields) of the private key. Otherwise, it will complain
        // it's not valid for the certificate.
        /* .flags = */ RSA_METHOD_FLAG_NO_CHECK,
        /* .app_data = */ NULL,
        /* .rsa_sign = */ NULL,
        /* .rsa_verify = */ NULL,
        /* .rsa_keygen = */ NULL,
};

// Custom DSA_METHOD that uses the platform APIs.
// Note that for now, only signing through DSA_sign() is really supported.
// all other method pointers are either stubs returning errors, or no-ops.
// See <openssl/dsa.h> for exact declaration of DSA_METHOD.
//
// Note: There is no DSA_set_app_data() and DSA_get_app_data() functions,
//       but RSA_set_app_data() is defined as a simple macro that calls
//       RSA_set_ex_data() with a hard-coded index of 0, so this code
//       does the same thing here.

DSA_SIG* DsaMethodDoSign(const unsigned char* dgst,
                         int dlen,
                         DSA* dsa)
{
    // Extract the JNI reference to the PrivateKey object.
    IDSAPrivateKey* private_key = reinterpret_cast<IDSAPrivateKey*>(DSA_get_ex_data(dsa, 0));
    if (private_key == NULL) return NULL;

    // Sign the message with it, calling platform APIs.
    AutoPtr< ArrayOf<Byte> > signature = RawSignDigestWithPrivateKey(
            IPrivateKey::Probe(private_key),
            reinterpret_cast<const char*>(dgst), dlen);
    if (signature == NULL) {
        Logger::E(LOG_TAG, "Could not sign message in DsaMethodDoSign!");
        return NULL;
    }

    // Note: With DSA, the actual signature might be smaller than DSA_size().
    size_t max_expected_size = static_cast<size_t>(DSA_size(dsa));
    if ((size_t)signature->GetLength() > max_expected_size) {
        Logger::E(LOG_TAG, "DSA Signature size mismatch, actual: %zd, expected <= %zd",
                signature->GetLength(), max_expected_size);
        return NULL;
    }

    // Convert the signature into a DSA_SIG object.
    const unsigned char* sigbuf = reinterpret_cast<const unsigned char*>(signature->GetPayload());
    int siglen = static_cast<size_t>(signature->GetLength());
    DSA_SIG* dsa_sig = d2i_DSA_SIG(NULL, &sigbuf, siglen);
    return dsa_sig;
}

int DsaMethodSignSetup(DSA* /* dsa */,
                       BN_CTX* /* ctx_in */,
                       BIGNUM** /* kinvp */,
                       BIGNUM** /* rp */,
                       const unsigned char* /* dgst */,
                       int /* dlen */)
{
    DSAerr(DSA_F_DSA_SIGN_SETUP, DSA_R_INVALID_DIGEST_TYPE);
    return -1;
}

int DsaMethodDoVerify(const unsigned char* /* dgst */,
                      int /* dgst_len */,
                      DSA_SIG* /* sig */,
                      DSA* /* dsa */)
{
    DSAerr(DSA_F_DSA_DO_VERIFY, DSA_R_INVALID_DIGEST_TYPE);
    return -1;
}

int DsaMethodFinish(DSA* dsa)
{
    // Free the global JNI reference that was created with this
    // wrapper key.
    IDSAPrivateKey* key = reinterpret_cast<IDSAPrivateKey*>(DSA_get_ex_data(dsa, 0));
    if (key != NULL) {
        DSA_set_ex_data(dsa, 0, NULL);
        key->Release();
    }
    // Actual return value is ignored by OpenSSL. There are no docs
    // explaining what this is supposed to be.
    return 0;
}

const DSA_METHOD elastos_dsa_method = {
        /* .name = */ "Elastos signing-only DSA method",
        /* .dsa_do_sign = */ DsaMethodDoSign,
        /* .dsa_sign_setup = */ DsaMethodSignSetup,
        /* .dsa_do_verify = */ DsaMethodDoVerify,
        /* .dsa_mod_exp = */ NULL,
        /* .bn_mod_exp = */ NULL,
        /* .init = */ NULL,  // nothing to do here.
        /* .finish = */ DsaMethodFinish,
        /* .flags = */ 0,
        /* .app_data = */ NULL,
        /* .dsa_paramgem = */ NULL,
        /* .dsa_keygen = */ NULL};

// Used to ensure that the global JNI reference associated with a custom
// EC_KEY + ECDSA_METHOD wrapper is released when its EX_DATA is destroyed
// (this function is called when EVP_PKEY_free() is called on the wrapper).
void ExDataFree(void* /* parent */,
                void* ptr,
                CRYPTO_EX_DATA* ad,
                int idx,
                long /* argl */,
                void* /* argp */)
{
    IECPrivateKey* private_key = reinterpret_cast<IECPrivateKey*>(ptr);
    if (private_key == NULL) return;

    CRYPTO_set_ex_data(ad, idx, NULL);
    private_key->Release();
}

int ExDataDup(CRYPTO_EX_DATA* /* to */,
              CRYPTO_EX_DATA* /* from */,
              void* /* from_d */,
              int /* idx */,
              long /* argl */,
              void* /* argp */)
{
    // This callback shall never be called with the current OpenSSL
    // implementation (the library only ever duplicates EX_DATA items
    // for SSL and BIO objects). But provide this to catch regressions
    // in the future.
    // Return value is currently ignored by OpenSSL.
    return 0;
}

class EcdsaExDataIndex
{
  public:
    int ex_data_index() { return ex_data_index_; }

    static EcdsaExDataIndex& Instance() {
        static EcdsaExDataIndex singleton;
        return singleton;
    }

  private:
    EcdsaExDataIndex() {
        ex_data_index_ = ECDSA_get_ex_new_index(0, NULL, NULL, ExDataDup, ExDataFree);
    }
    EcdsaExDataIndex(EcdsaExDataIndex const&);
    ~EcdsaExDataIndex() {}
    EcdsaExDataIndex& operator=(EcdsaExDataIndex const&);

    int ex_data_index_;
};

// Returns the index of the custom EX_DATA used to store the JNI reference.
int EcdsaGetExDataIndex(void)
{
    EcdsaExDataIndex& exData = EcdsaExDataIndex::Instance();
    return exData.ex_data_index();
}

ECDSA_SIG* EcdsaMethodDoSign(const unsigned char* dgst, int dgst_len, const BIGNUM* /* inv */,
                             const BIGNUM* /* rp */, EC_KEY* eckey)
{
    // Retrieve private key JNI reference.
    IECPrivateKey* private_key =
            reinterpret_cast<IECPrivateKey*>(ECDSA_get_ex_data(eckey, EcdsaGetExDataIndex()));
    if (!private_key) {
        Logger::E(LOG_TAG, "Null JNI reference passed to EcdsaMethodDoSign!");
        return NULL;
    }

    // Sign message with it through JNI.
    AutoPtr< ArrayOf<Byte> > signature = RawSignDigestWithPrivateKey(
            IPrivateKey::Probe(private_key),
            reinterpret_cast<const char*>(dgst), dgst_len);
    if (signature == NULL) {
        Logger::E(LOG_TAG, "Could not sign message in EcdsaMethodDoSign!");
        return NULL;
    }

    // Note: With ECDSA, the actual signature may be smaller than
    // ECDSA_size().
    size_t max_expected_size = static_cast<size_t>(ECDSA_size(eckey));
    if ((size_t)signature->GetLength() > max_expected_size) {
        Logger::E(LOG_TAG, "ECDSA Signature size mismatch, actual: %zd, expected <= %zd",
                signature->GetLength(), max_expected_size);
        return NULL;
    }

    // Convert signature to ECDSA_SIG object
    const unsigned char* sigbuf = reinterpret_cast<const unsigned char*>(signature->GetPayload());
    long siglen = static_cast<long>(signature->GetLength());
    return d2i_ECDSA_SIG(NULL, &sigbuf, siglen);
}

int EcdsaMethodSignSetup(EC_KEY* /* eckey */,
                         BN_CTX* /* ctx */,
                         BIGNUM** /* kinv */,
                         BIGNUM** /* r */,
                         const unsigned char*,
                         int)
{
    ECDSAerr(ECDSA_F_ECDSA_SIGN_SETUP, ECDSA_R_ERR_EC_LIB);
    return -1;
}

int EcdsaMethodDoVerify(const unsigned char* /* dgst */,
                        int /* dgst_len */,
                        const ECDSA_SIG* /* sig */,
                        EC_KEY* /* eckey */)
{
    ECDSAerr(ECDSA_F_ECDSA_DO_VERIFY, ECDSA_R_ERR_EC_LIB);
    return -1;
}

const ECDSA_METHOD elastos_ecdsa_method = {
        /* .name = */ "Elastos signing-only ECDSA method",
        /* .ecdsa_do_sign = */ EcdsaMethodDoSign,
        /* .ecdsa_sign_setup = */ EcdsaMethodSignSetup,
        /* .ecdsa_do_verify = */ EcdsaMethodDoVerify,
        /* .flags = */ 0,
        /* .app_data = */ NULL,
};

/**
 * OpenSSL locking support. Taken from the O'Reilly book by Viega et al., but I
 * suppose there are not many other ways to do this on a Linux system (modulo
 * isomorphism).
 */
#define MUTEX_TYPE pthread_mutex_t
#define MUTEX_SETUP(x) pthread_mutex_init(&(x), NULL)
#define MUTEX_CLEANUP(x) pthread_mutex_destroy(&(x))
#define MUTEX_LOCK(x) pthread_mutex_lock(&(x))
#define MUTEX_UNLOCK(x) pthread_mutex_unlock(&(x))
#define THREAD_ID pthread_self()
#define THROW_SSLEXCEPTION (-2)
#define THROW_SOCKETTIMEOUTEXCEPTION (-3)
#define THROWN_EXCEPTION (-4)

static MUTEX_TYPE* mutex_buf = NULL;

static void locking_function(int mode, int n, const char*, int)
{
    if (mode & CRYPTO_LOCK) {
        MUTEX_LOCK(mutex_buf[n]);
    }
    else {
        MUTEX_UNLOCK(mutex_buf[n]);
    }
}

static unsigned long id_function(void)
{
    return ((unsigned long)THREAD_ID);
}

int THREAD_setup(void)
{
    mutex_buf = new MUTEX_TYPE[CRYPTO_num_locks()];
    if (!mutex_buf) {
        return 0;
    }

    for (int i = 0; i < CRYPTO_num_locks(); ++i) {
        MUTEX_SETUP(mutex_buf[i]);
    }

    CRYPTO_set_id_callback(id_function);
    CRYPTO_set_locking_callback(locking_function);

    return 1;
}

int THREAD_cleanup(void)
{
    if (!mutex_buf) {
        return 0;
    }

    CRYPTO_set_id_callback(NULL);
    CRYPTO_set_locking_callback(NULL);

    for (int i = 0; i < CRYPTO_num_locks( ); i++) {
        MUTEX_CLEANUP(mutex_buf[i]);
    }

    free(mutex_buf);
    mutex_buf = NULL;

    return 1;
}

const Boolean NativeCrypto::sInitialized = Clinit();

/**
 * Initialization phase for every OpenSSL job: Loads the Error strings, the
 * crypto algorithms and reset the OpenSSL library
 */
Boolean NativeCrypto::Clinit()
{
    SSL_load_error_strings();
    ERR_load_crypto_strings();
    SSL_library_init();
    OpenSSL_add_all_algorithms();
    THREAD_setup();
    return TRUE;
}

ECode NativeCrypto::ENGINE_load_dynamic()
{
    NATIVE_TRACE("ENGINE_load_dynamic()");

    ::ENGINE_load_dynamic();
    return NOERROR;
}

ECode NativeCrypto::ENGINE_by_id(
    /* [in] */ const String& id,
    /* [out] */ Int64* result)
{
    VALIDATE_NOT_NULL(result);

    if (id.IsNull()) {
        NATIVE_TRACE("ENGINE_by_id => id == null");
        *result = 0;
        return NOERROR;
    }
    NATIVE_TRACE("ENGINE_by_id(\"%s\")", id.string());

    ENGINE* e = ::ENGINE_by_id(id.string());
    if (e == NULL) {
        FreeOpenSslErrorState();
    }

    NATIVE_TRACE("ENGINE_by_id(\"%s\") => %p", id.string(), e);
    *result = reinterpret_cast<uintptr_t>(e);
    return NOERROR;
}

ECode NativeCrypto::ENGINE_add(
    /* [in] */ Int64 engineRef,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    ENGINE* e = reinterpret_cast<ENGINE*>(static_cast<uintptr_t>(engineRef));
    NATIVE_TRACE("ENGINE_add(%p)", e);

    if (e == NULL) {
        *result = 0;
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    int ret = ::ENGINE_add(e);

    /*
     * We tolerate errors, because the most likely error is that
     * the ENGINE is already in the list.
     */
    FreeOpenSslErrorState();

    NATIVE_TRACE("ENGINE_add(%p) => %d", e, ret);
    *result = ret;
    return NOERROR;
}

ECode NativeCrypto::ENGINE_init(
    /* [in] */ Int64 engineRef,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    ENGINE* e = reinterpret_cast<ENGINE*>(static_cast<uintptr_t>(engineRef));
    NATIVE_TRACE("ENGINE_init(%p)", e);

    if (e == NULL) {
        *result = 0;
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    int ret = ::ENGINE_init(e);
    NATIVE_TRACE("ENGINE_init(%p) => %d", e, ret);
    *result = ret;
    return NOERROR;
}

ECode NativeCrypto::ENGINE_finish(
    /* [in] */ Int64 engineRef,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    ENGINE* e = reinterpret_cast<ENGINE*>(static_cast<uintptr_t>(engineRef));
    NATIVE_TRACE("ENGINE_finish(%p)", e);

    if (e == NULL) {
        *result = 0;
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    int ret = ::ENGINE_finish(e);
    NATIVE_TRACE("ENGINE_finish(%p) => %d", e, ret);
    *result = ret;
    return NOERROR;
}

ECode NativeCrypto::ENGINE_free(
    /* [in] */ Int64 engineRef,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    ENGINE* e = reinterpret_cast<ENGINE*>(static_cast<uintptr_t>(engineRef));
    NATIVE_TRACE("ENGINE_free(%p)", e);

    if (e == NULL) {
        *result = 0;
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    int ret = ::ENGINE_free(e);
    NATIVE_TRACE("ENGINE_free(%p) => %d", e, ret);
    *result = ret;
    return NOERROR;
}

ECode NativeCrypto::ENGINE_load_private_key(
    /* [in] */ Int64 engineRef,
    /* [in] */ const String& id,
    /* [out] */ Int64* result)
{
    VALIDATE_NOT_NULL(result);

    ENGINE* e = reinterpret_cast<ENGINE*>(static_cast<uintptr_t>(engineRef));

    if (id.IsNull()) {
        *result = 0;
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    Unique_EVP_PKEY pkey(::ENGINE_load_private_key(e, id.string(), NULL, NULL));
    if (pkey.get() == NULL) {
        *result = 0;
        return ThrowExceptionIfNecessary("ENGINE_load_private_key");
    }

    NATIVE_TRACE("ENGINE_load_private_key(%p, %s) => %p", e, is.string(), pkey.get());
    *result = reinterpret_cast<uintptr_t>(pkey.release());
    return NOERROR;
}

ECode NativeCrypto::ENGINE_get_id(
    /* [in] */ Int64 engineRef,
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);

    ENGINE* e = reinterpret_cast<ENGINE*>(static_cast<uintptr_t>(engineRef));
    NATIVE_TRACE("ENGINE_get_id(%p)", e);

    if (e == NULL) {
        NATIVE_TRACE("ENGINE_get_id(%p) => engine == null", e);
        *result = NULL;
        return E_NULL_POINTER_EXCEPTION;
    }

    const char *id = ::ENGINE_get_id(e);
    *result = id;

    NATIVE_TRACE("ENGINE_get_id(%p) => \"%s\"", e, id);
    return NOERROR;
}

ECode NativeCrypto::ENGINE_ctrl_cmd_string(
    /* [in] */ Int64 engineRef,
    /* [in] */ const String& cmd,
    /* [in] */ const String& arg,
    /* [in] */ Int32 cmd_optional,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    ENGINE* e = reinterpret_cast<ENGINE*>(static_cast<uintptr_t>(engineRef));
    NATIVE_TRACE("ENGINE_ctrl_cmd_string(%p, %s, %s, %d)", e, cmd.string(), arg.string(), cmd_optional);

    if (e == NULL) {
        NATIVE_TRACE("ENGINE_ctrl_cmd_string(%p, %s, %s, %d) => engine == null", e, cmd.string(), arg.string(),
                cmd_optional);
        *result = 0;
        return E_NULL_POINTER_EXCEPTION;
    }

    if (cmd.IsNull()) {
        *result = 0;
        return NOERROR;
    }

    const char* arg_c_str = NULL;
    if (!arg.IsNull()) {
        arg_c_str = arg.string();
        if (arg_c_str == NULL) {
            *result = 0;
            return NOERROR;
        }
    }
    NATIVE_TRACE("ENGINE_ctrl_cmd_string(%p, \"%s\", \"%s\", %d)", e, cmd.string(), arg_c_str,
            cmd_optional);

    int ret = ::ENGINE_ctrl_cmd_string(e, cmd.string(), arg_c_str, cmd_optional);
    if (ret != 1) {
        NATIVE_TRACE("ENGINE_ctrl_cmd_string(%p, \"%s\", \"%s\", %d) => threw error", e,
                cmd.string(), arg_c_str, cmd_optional);
        *result = 0;
        return ThrowExceptionIfNecessary("ENGINE_ctrl_cmd_string");
    }

    NATIVE_TRACE("ENGINE_ctrl_cmd_string(%p, \"%s\", \"%s\", %d) => %d", e, cmd.string(),
            arg_c_str, cmd_optional, ret);
    *result = ret;
    return NOERROR;
}

ECode NativeCrypto::EVP_PKEY_new_DSA(
    /* [in] */ ArrayOf<Byte>* p,
    /* [in] */ ArrayOf<Byte>* q,
    /* [in] */ ArrayOf<Byte>* g,
    /* [in] */ ArrayOf<Byte>* pub_key,
    /* [in] */ ArrayOf<Byte>* priv_key,
    /* [out] */ Int64* result)
{
    VALIDATE_NOT_NULL(result);

    NATIVE_TRACE("EVP_PKEY_new_DSA(p=%p, q=%p, g=%p, pub_key=%p, priv_key=%p)",
              p, q, g, pub_key, priv_key);

    Unique_DSA dsa(DSA_new());
    if (dsa.get() == NULL) {
        *result = 0;
        return ThrowRuntimeException("DSA_new failed");
    }

    ECode ec = NOERROR;
    if (!ArrayToBignum(p, &dsa->p, &ec)) {
        *result = 0;
        return ec;
    }

    if (!ArrayToBignum(q, &dsa->q, &ec)) {
        *result = 0;
        return ec;
    }

    if (!ArrayToBignum(g, &dsa->g, &ec)) {
        *result = 0;
        return ec;
    }

    if (pub_key != NULL && !ArrayToBignum(pub_key, &dsa->pub_key, &ec)) {
        *result = 0;
        return ec;
    }

    if (priv_key != NULL && !ArrayToBignum(priv_key, &dsa->priv_key, &ec)) {
        *result = 0;
        return ec;
    }

    if (dsa->p == NULL || dsa->q == NULL || dsa->g == NULL
            || (dsa->pub_key == NULL && dsa->priv_key == NULL)) {
        *result = 0;
        return ThrowRuntimeException("Unable to convert BigInteger to BIGNUM");
    }

    Unique_EVP_PKEY pkey(EVP_PKEY_new());
    if (pkey.get() == NULL) {
        *result = 0;
        return ThrowRuntimeException("EVP_PKEY_new failed");
    }
    if (EVP_PKEY_assign_DSA(pkey.get(), dsa.get()) != 1) {
        *result = 0;
        return ThrowRuntimeException("EVP_PKEY_assign_DSA failed");
    }
    OWNERSHIP_TRANSFERRED(dsa);
    NATIVE_TRACE("EVP_PKEY_new_DSA(p=%p, q=%p, g=%p, pub_key=%p, priv_key=%p) => %p",
              p, q, g, pub_key, priv_key, pkey.get());
    *result = reinterpret_cast<Int64>(pkey.release());
    return NOERROR;
}

ECode NativeCrypto::EVP_PKEY_new_RSA(
    /* [in] */ ArrayOf<Byte>* n,
    /* [in] */ ArrayOf<Byte>* e,
    /* [in] */ ArrayOf<Byte>* d,
    /* [in] */ ArrayOf<Byte>* p,
    /* [in] */ ArrayOf<Byte>* q,
    /* [in] */ ArrayOf<Byte>* dmp1,
    /* [in] */ ArrayOf<Byte>* dmq1,
    /* [in] */ ArrayOf<Byte>* iqmp,
    /* [out] */ Int64* result)
{
    VALIDATE_NOT_NULL(result);

    NATIVE_TRACE("EVP_PKEY_new_RSA(n=%p, e=%p, d=%p, p=%p, q=%p, dmp1=%p, dmq1=%p, iqmp=%p)",
            n, e, d, p, q, dmp1, dmq1, iqmp);

    Unique_RSA rsa(RSA_new());
    if (rsa.get() == NULL) {
        *result = 0;
        return ThrowRuntimeException("RSA_new failed");
    }

    if (e == NULL && d == NULL) {
        NATIVE_TRACE("EVP_PKEY_new_RSA => e == NULL && d == NULL");
        *result = 0;
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    ECode ec;
    if (!ArrayToBignum(n, &rsa->n, &ec)) {
        *result = 0;
        return ec;
    }

    if (e != NULL && !ArrayToBignum(e, &rsa->e, &ec)) {
        *result = 0;
        return ec;
    }

    if (d != NULL && !ArrayToBignum(d, &rsa->d, &ec)) {
        *result = 0;
        return ec;
    }

    if (p != NULL && !ArrayToBignum(p, &rsa->p, &ec)) {
        *result = 0;
        return ec;
    }

    if (q != NULL && !ArrayToBignum(q, &rsa->q, &ec)) {
        *result = 0;
        return ec;
    }

    if (dmp1 != NULL && !ArrayToBignum(dmp1, &rsa->dmp1, &ec)) {
        *result = 0;
        return ec;
    }

    if (dmq1 != NULL && !ArrayToBignum(dmq1, &rsa->dmq1, &ec)) {
        *result = 0;
        return ec;
    }

    if (iqmp != NULL && !ArrayToBignum(iqmp, &rsa->iqmp, &ec)) {
        *result = 0;
        return ec;
    }

#ifdef WITH_NATIVE_TRACE
    if (p != NULL && q != NULL) {
        int check = RSA_check_key(rsa.get());
        NATIVE_TRACE("EVP_PKEY_new_RSA(...) RSA_check_key returns %d", check);
    }
#endif

    if (rsa->n == NULL || (rsa->e == NULL && rsa->d == NULL)) {
        *result = 0;
        return ThrowRuntimeException("Unable to convert BigInteger to BIGNUM");
    }

    /*
     * If the private exponent is available, there is the potential to do signing
     * operations. If the public exponent is also available, OpenSSL will do RSA
     * blinding. Enable it if possible.
     */
    if (rsa->d != NULL) {
        if (rsa->e != NULL) {
            NATIVE_TRACE("EVP_PKEY_new_RSA(...) enabling RSA blinding => %p", rsa.get());
            RSA_blinding_on(rsa.get(), NULL);
        }
        else {
            NATIVE_TRACE("EVP_PKEY_new_RSA(...) disabling RSA blinding => %p", rsa.get());
            RSA_blinding_off(rsa.get());
        }
    }

    Unique_EVP_PKEY pkey(EVP_PKEY_new());
    if (pkey.get() == NULL) {
        *result = 0;
        return ThrowRuntimeException("EVP_PKEY_new failed");
    }
    if (EVP_PKEY_assign_RSA(pkey.get(), rsa.get()) != 1) {
        *result = 0;
        return ThrowRuntimeException("EVP_PKEY_new failed");
    }
    OWNERSHIP_TRANSFERRED(rsa);
    NATIVE_TRACE("EVP_PKEY_new_RSA(n=%p, e=%p, d=%p, p=%p, q=%p dmp1=%p, dmq1=%p, iqmp=%p) => %p",
            n, e, d, p, q, dmp1, dmq1, iqmp, pkey.get());
    *result = reinterpret_cast<uintptr_t>(pkey.release());
    return NOERROR;
}

ECode NativeCrypto::EVP_PKEY_new_mac_key(
    /* [in] */ Int32 type,
    /* [in] */ ArrayOf<Byte>* key,
    /* [out] */ Int64* result)
{
    VALIDATE_NOT_NULL(result);

    NATIVE_TRACE("EVP_PKEY_new_mac_key(%d, %p)", type, key);

    if (key == NULL) {
        *result = 0;
        return NOERROR;
    }

    const unsigned char* tmp = reinterpret_cast<const unsigned char*>(key->GetPayload());
    Unique_EVP_PKEY pkey(::EVP_PKEY_new_mac_key(type, (ENGINE *) NULL, tmp, key->GetLength()));
    if (pkey.get() == NULL) {
        NATIVE_TRACE("EVP_PKEY_new_mac_key(%d, %p) => threw error", type, key);
        *result = 0;
        return ThrowExceptionIfNecessary("ENGINE_load_private_key");
    }

    NATIVE_TRACE("EVP_PKEY_new_mac_key(%d, %p) => %p", type, key, pkey.get());
    *result = reinterpret_cast<uintptr_t>(pkey.release());
    return NOERROR;
}

ECode NativeCrypto::EVP_PKEY_size(
    /* [in] */ Int64 pkeyRef,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    EVP_PKEY* pkey = reinterpret_cast<EVP_PKEY*>(pkeyRef);
    NATIVE_TRACE("EVP_PKEY_size(%p)", pkey);

    if (pkey == NULL) {
        *result = -1;
        return E_NULL_POINTER_EXCEPTION;
    }

    int res = ::EVP_PKEY_size(pkey);
    NATIVE_TRACE("EVP_PKEY_size(%p) => %d", pkey, res);
    *result = res;
    return NOERROR;
}

ECode NativeCrypto::EVP_PKEY_type(
    /* [in] */ Int64 pkeyRef,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    EVP_PKEY* pkey = reinterpret_cast<EVP_PKEY*>(pkeyRef);
    NATIVE_TRACE("EVP_PKEY_type(%p)", pkey);

    if (pkey == NULL) {
        *result = -1;
        return E_NULL_POINTER_EXCEPTION;
    }

    int res = ::EVP_PKEY_type(pkey->type);
    NATIVE_TRACE("EVP_PKEY_type(%p) => %d", pkey, res);
    *result = res;
    return NOERROR;
}

ECode NativeCrypto::EVP_PKEY_print_public(
    /* [in] */ Int64 pkeyRef,
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);

    EVP_PKEY* pkey = reinterpret_cast<EVP_PKEY*>(pkeyRef);
    NATIVE_TRACE("EVP_PKEY_print_public(%p)", pkey);

    if (pkey == NULL) {
        *result = NULL;
        return E_NULL_POINTER_EXCEPTION;
    }

    Unique_BIO buffer(BIO_new(BIO_s_mem()));
    if (buffer.get() == NULL) {
        *result = NULL;
        return E_OUT_OF_MEMORY_ERROR;
    }

    if (::EVP_PKEY_print_public(buffer.get(), pkey, 0, (ASN1_PCTX*) NULL) != 1) {
        *result = NULL;
        return ThrowExceptionIfNecessary("EVP_PKEY_print_public");
    }
    // Null terminate this
    BIO_write(buffer.get(), "\0", 1);

    char *tmp;
    BIO_get_mem_data(buffer.get(), &tmp);
    *result = tmp;

    NATIVE_TRACE("EVP_PKEY_print_public(%p) => \"%s\"", pkey, (*result).string());
    return NOERROR;
}

ECode NativeCrypto::EVP_PKEY_print_private(
    /* [in] */ Int64 pkeyRef,
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);

    EVP_PKEY* pkey = reinterpret_cast<EVP_PKEY*>(pkeyRef);
    NATIVE_TRACE("EVP_PKEY_print_private(%p)", pkey);

    if (pkey == NULL) {
        *result = NULL;
        return E_NULL_POINTER_EXCEPTION;
    }

    Unique_BIO buffer(BIO_new(BIO_s_mem()));
    if (buffer.get() == NULL) {
        *result = NULL;
        return E_OUT_OF_MEMORY_ERROR;
    }

    if (::EVP_PKEY_print_private(buffer.get(), pkey, 0, (ASN1_PCTX*) NULL) != 1) {
        *result = NULL;
        return ThrowExceptionIfNecessary("EVP_PKEY_print_private");
    }
    // Null terminate this
    BIO_write(buffer.get(), "\0", 1);

    char *tmp;
    BIO_get_mem_data(buffer.get(), &tmp);
    *result = tmp;

    NATIVE_TRACE("EVP_PKEY_print_private(%p) => \"%s\"", pkey, (*result).string());
    return NOERROR;
}

ECode NativeCrypto::EVP_PKEY_free(
    /* [in] */ Int64 pkeyRef)
{
    EVP_PKEY* pkey = reinterpret_cast<EVP_PKEY*>(pkeyRef);
    NATIVE_TRACE("EVP_PKEY_free(%p)", pkey);

    if (pkey != NULL) {
        ::EVP_PKEY_free(pkey);
    }
    return NOERROR;
}

ECode NativeCrypto::EVP_PKEY_cmp(
    /* [in] */ Int64 pkey1Ref,
    /* [in] */ Int64 pkey2Ref,
    /* [out] */ Int32* result)
{
    EVP_PKEY* pkey1 = reinterpret_cast<EVP_PKEY*>(pkey1Ref);
    EVP_PKEY* pkey2 = reinterpret_cast<EVP_PKEY*>(pkey2Ref);
    NATIVE_TRACE("EVP_PKEY_cmp(%p, %p)", pkey1, pkey2);

    if (pkey1 == NULL) {
        NATIVE_TRACE("EVP_PKEY_cmp(%p, %p) => failed pkey1 == NULL", pkey1, pkey2);
        *result = -1;
        return E_NULL_POINTER_EXCEPTION;
    }
    else if (pkey2 == NULL) {
        NATIVE_TRACE("EVP_PKEY_cmp(%p, %p) => failed pkey2 == NULL", pkey1, pkey2);
        *result = -1;
        return E_NULL_POINTER_EXCEPTION;
    }

    int res = ::EVP_PKEY_cmp(pkey1, pkey2);
    NATIVE_TRACE("EVP_PKEY_cmp(%p, %p) => %d", pkey1, pkey2, res);
    *result = res;
    return NOERROR;
}

ECode NativeCrypto::I2d_PKCS8_PRIV_KEY_INFO(
    /* [in] */ Int64 pkeyRef,
    /* [out, callee] */ ArrayOf<Byte>** result)
{
    VALIDATE_NOT_NULL(result);

    EVP_PKEY* pkey = reinterpret_cast<EVP_PKEY*>(pkeyRef);
    NATIVE_TRACE("i2d_PKCS8_PRIV_KEY_INFO(%p)", pkey);

    if (pkey == NULL) {
        *result = NULL;
        return E_NULL_POINTER_EXCEPTION;
    }

    Unique_PKCS8_PRIV_KEY_INFO pkcs8(EVP_PKEY2PKCS8(pkey));
    if (pkcs8.get() == NULL) {
        NATIVE_TRACE("key=%p i2d_PKCS8_PRIV_KEY_INFO => error from key to PKCS8", pkey);
        *result = NULL;
        return ThrowExceptionIfNecessary("i2d_PKCS8_PRIV_KEY_INFO");
    }

    return ASN1ToByteArray<PKCS8_PRIV_KEY_INFO, i2d_PKCS8_PRIV_KEY_INFO>(pkcs8.get(), result);
}

ECode NativeCrypto::D2i_PKCS8_PRIV_KEY_INFO(
    /* [in] */ ArrayOf<Byte>* keyBytes,
    /* [out] */ Int64* result)
{
    VALIDATE_NOT_NULL(result);

    NATIVE_TRACE("d2i_PKCS8_PRIV_KEY_INFO(%p)", keyBytes);

    if (keyBytes == NULL) {
        NATIVE_TRACE("bytes=%p d2i_PKCS8_PRIV_KEY_INFO => threw exception", keyBytes);
        *result = 0;
        return NOERROR;
    }

    const unsigned char* tmp = reinterpret_cast<const unsigned char*>(keyBytes->GetPayload());
    Unique_PKCS8_PRIV_KEY_INFO pkcs8(::d2i_PKCS8_PRIV_KEY_INFO(NULL, &tmp, keyBytes->GetLength()));
    if (pkcs8.get() == NULL) {
        NATIVE_TRACE("ssl=%p d2i_PKCS8_PRIV_KEY_INFO => error from DER to PKCS8", keyBytes);
        *result = 0;
        return ThrowExceptionIfNecessary("d2i_PKCS8_PRIV_KEY_INFO");
    }

    Unique_EVP_PKEY pkey(EVP_PKCS82PKEY(pkcs8.get()));
    if (pkey.get() == NULL) {
        NATIVE_TRACE("ssl=%p d2i_PKCS8_PRIV_KEY_INFO => error from PKCS8 to key", keyBytes);
        *result = 0;
        return ThrowExceptionIfNecessary("d2i_PKCS8_PRIV_KEY_INFO");
    }

    NATIVE_TRACE("bytes=%p d2i_PKCS8_PRIV_KEY_INFO => %p", keyBytes, pkey.get());
    *result = reinterpret_cast<uintptr_t>(pkey.release());
    return NOERROR;
}

ECode NativeCrypto::I2d_PUBKEY(
    /* [in] */ Int64 pkeyRef,
    /* [out, callee] */ ArrayOf<Byte>** result)
{
    VALIDATE_NOT_NULL(result);

    EVP_PKEY* pkey = reinterpret_cast<EVP_PKEY*>(pkeyRef);
    NATIVE_TRACE("i2d_PUBKEY(%p)", pkey);
    return ASN1ToByteArray<EVP_PKEY, i2d_PUBKEY>(pkey, result);
}

ECode NativeCrypto::D2i_PUBKEY(
    /* [in] */ ArrayOf<Byte>* data,
    /* [out] */ Int64* result)
{
    VALIDATE_NOT_NULL(result);

    NATIVE_TRACE("d2i_PUBKEY(%p)", data);

    if (data == NULL) {
        NATIVE_TRACE("d2i_PUBKEY(%p) => threw error", data);
        *result = 0;
        return NOERROR;
    }

    const unsigned char* tmp = reinterpret_cast<const unsigned char*>(data->GetPayload());
    Unique_EVP_PKEY pkey(d2i_PUBKEY(NULL, &tmp, data->GetLength()));
    if (pkey.get() == NULL) {
        NATIVE_TRACE("bytes=%p d2i_PUBKEY => threw exception", data);
        *result = 0;
        return ThrowExceptionIfNecessary("d2i_PUBKEY");
    }

    *result = reinterpret_cast<uintptr_t>(pkey.release());
    return NOERROR;
}

ECode NativeCrypto::GetRSAPrivateKeyWrapper(
    /* [in] */ IRSAPrivateKey* key,
    /* [in] */ ArrayOf<Byte>* modulus,
    /* [out] */ Int64* result)
{
    VALIDATE_NOT_NULL(result);

    NATIVE_TRACE("getRSAPrivateKeyWrapper(%p, %p)", key, modulus);

    Unique_RSA rsa(RSA_new());
    if (rsa.get() == NULL) {
        *result = 0;
        return E_OUT_OF_MEMORY_ERROR;
    }

    RSA_set_method(rsa.get(), &elastos_rsa_method);

    ECode ec;
    if (!ArrayToBignum(modulus, &rsa->n, &ec)) {
        *result = 0;
        return ec;
    }

    key->AddRef();
    RSA_set_app_data(rsa.get(), key);

    Unique_EVP_PKEY pkey(EVP_PKEY_new());
    if (pkey.get() == NULL) {
        NATIVE_TRACE("getRSAPrivateKeyWrapper failed");
        ec = ThrowRuntimeException("getRSAPrivateKeyWrapper failed");
        FreeOpenSslErrorState();
        *result = 0;
        return ec;
    }

    if (EVP_PKEY_assign_RSA(pkey.get(), rsa.get()) != 1) {
        *result = 0;
        return ThrowRuntimeException("getRSAPrivateKeyWrapper failed");
    }
    OWNERSHIP_TRANSFERRED(rsa);
    *result = reinterpret_cast<uintptr_t>(pkey.release());
    return NOERROR;
}

ECode NativeCrypto::GetDSAPrivateKeyWrapper(
    /* [in] */ IDSAPrivateKey* key,
    /* [out] */ Int64* result)
{
    VALIDATE_NOT_NULL(result);

    NATIVE_TRACE("getDSAPrivateKeyWrapper(%p)", key);

    Unique_DSA dsa(DSA_new());
    if (dsa.get() == NULL) {
        *result = 0;
        return E_OUT_OF_MEMORY_ERROR;
    }

    DSA_set_method(dsa.get(), &elastos_dsa_method);
    key->AddRef();
    DSA_set_ex_data(dsa.get(), 0, key);

    Unique_EVP_PKEY pkey(EVP_PKEY_new());
    if (pkey.get() == NULL) {
        NATIVE_TRACE("getDSAPrivateKeyWrapper failed");
        FreeOpenSslErrorState();
        *result = 0;
        return ThrowRuntimeException("getDSAPrivateKeyWrapper failed");
    }

    if (EVP_PKEY_assign_DSA(pkey.get(), dsa.get()) != 1) {
        *result = 0;
        return ThrowRuntimeException("getDSAPrivateKeyWrapper failed");
    }
    OWNERSHIP_TRANSFERRED(dsa);
    *result = reinterpret_cast<uintptr_t>(pkey.release());
    return NOERROR;
}

ECode NativeCrypto::GetECPrivateKeyWrapper(
    /* [in] */ IECPrivateKey* key,
    /* [in] */ Int64 ecGroupRef,
    /* [out] */ Int64* result)
{
    VALIDATE_NOT_NULL(result);

    NATIVE_TRACE("getECPrivateKeyWrapper(%p, %p)", key, ecGroupRef);

    Unique_EC_KEY ecKey(EC_KEY_new());
    if (ecKey.get() == NULL) {
        *result = 0;
        return E_OUT_OF_MEMORY_ERROR;
    }

    const EC_GROUP* group = reinterpret_cast<const EC_GROUP*>(ecGroupRef);
    NATIVE_TRACE("EC_GROUP_get_curve_name(%p)", group);

    if (group == NULL) {
        NATIVE_TRACE("EC_GROUP_get_curve_name => group == NULL");
        *result = 0;
        return E_NULL_POINTER_EXCEPTION;
    }

    EC_KEY_set_group(ecKey.get(), group);

    ECDSA_set_method(ecKey.get(), &elastos_ecdsa_method);
    key->AddRef();
    ECDSA_set_ex_data(ecKey.get(), EcdsaGetExDataIndex(), key);

    Unique_EVP_PKEY pkey(EVP_PKEY_new());
    if (pkey.get() == NULL) {
        NATIVE_TRACE("getECPrivateKeyWrapper failed");
        FreeOpenSslErrorState();
        *result = 0;
        return ThrowRuntimeException("getECPrivateKeyWrapper failed");
    }

    if (EVP_PKEY_assign_EC_KEY(pkey.get(), ecKey.get()) != 1) {
        *result = 0;
        return ThrowRuntimeException("getECPrivateKeyWrapper failed");
    }
    OWNERSHIP_TRANSFERRED(ecKey);
    *result = reinterpret_cast<uintptr_t>(pkey.release());
    return NOERROR;
}

} // namespace Conscrypt
} // namespace Org
