
#include "elastos/coredef.h"
#include "NativeCrypto.h"
#include <elastos/core/UniquePtr.h>

#include <openssl/asn1t.h>
#include <openssl/dsa.h>
#include <openssl/engine.h>
#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/rsa.h>
#include <openssl/ssl.h>
#include <openssl/x509v3.h>

using Elastos::Core::UniquePtr;

namespace Org {
namespace Conscrypt {

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

struct DSA_Delete {
    void operator()(DSA* p) const {
        DSA_free(p);
    }
};
typedef UniquePtr<DSA, DSA_Delete> Unique_DSA;

struct EVP_PKEY_Delete {
    void operator()(EVP_PKEY* p) const {
        EVP_PKEY_free(p);
    }
};
typedef UniquePtr<EVP_PKEY, EVP_PKEY_Delete> Unique_EVP_PKEY;

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
static void freeOpenSslErrorState()
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

    freeOpenSslErrorState();
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
        freeOpenSslErrorState();
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
    freeOpenSslErrorState();

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

} // namespace Conscrypt
} // namespace Org
