
#include "elastos/coredef.h"
#include "NativeCrypto.h"
#include "CryptoUpcalls.h"
#include <elastos/core/UniquePtr.h>
#include <elastos/utility/logging/Logger.h>

#include <arpa/inet.h>

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

using Elastos::Core::CArrayOf;
using Elastos::Core::CByte;
using Elastos::Core::CInteger32;
using Elastos::Core::CString;
using Elastos::Core::EIID_IByte;
using Elastos::Core::IByte;
using Elastos::Core::ICharSequence;
using Elastos::Core::IInteger32;
using Elastos::Core::UniquePtr;
using Elastos::IO::IFlushable;
using Elastos::IO::IInputStream;
using Elastos::Security::CMessageDigestHelper;
using Elastos::Security::IMessageDigest;
using Elastos::Security::IMessageDigestHelper;
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

struct OPENSSL_Delete {
    void operator()(void* p) const {
        OPENSSL_free(p);
    }
};
typedef UniquePtr<unsigned char, OPENSSL_Delete> Unique_OPENSSL_str;

struct BIO_Delete {
    void operator()(BIO* p) const {
        BIO_free_all(p);
    }
};
typedef UniquePtr<BIO, BIO_Delete> Unique_BIO;

struct BIGNUM_Delete {
    void operator()(BIGNUM* p) const {
        BN_free(p);
    }
};
typedef UniquePtr<BIGNUM, BIGNUM_Delete> Unique_BIGNUM;

struct ASN1_INTEGER_Delete {
    void operator()(ASN1_INTEGER* p) const {
        ASN1_INTEGER_free(p);
    }
};
typedef UniquePtr<ASN1_INTEGER, ASN1_INTEGER_Delete> Unique_ASN1_INTEGER;

struct DH_Delete {
    void operator()(DH* p) const {
        DH_free(p);
    }
};
typedef UniquePtr<DH, DH_Delete> Unique_DH;

struct DSA_Delete {
    void operator()(DSA* p) const {
        DSA_free(p);
    }
};
typedef UniquePtr<DSA, DSA_Delete> Unique_DSA;

struct EC_POINT_Delete {
    void operator()(EC_POINT* p) const {
        EC_POINT_clear_free(p);
    }
};
typedef UniquePtr<EC_POINT, EC_POINT_Delete> Unique_EC_POINT;

struct EC_KEY_Delete {
    void operator()(EC_KEY* p) const {
        EC_KEY_free(p);
    }
};
typedef UniquePtr<EC_KEY, EC_KEY_Delete> Unique_EC_KEY;

struct EVP_MD_CTX_Delete {
    void operator()(EVP_MD_CTX* p) const {
        EVP_MD_CTX_destroy(p);
    }
};
typedef UniquePtr<EVP_MD_CTX, EVP_MD_CTX_Delete> Unique_EVP_MD_CTX;

struct EVP_CIPHER_CTX_Delete {
    void operator()(EVP_CIPHER_CTX* p) const {
        EVP_CIPHER_CTX_free(p);
    }
};
typedef UniquePtr<EVP_CIPHER_CTX, EVP_CIPHER_CTX_Delete> Unique_EVP_CIPHER_CTX;

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

struct ASN1_BIT_STRING_Delete {
    void operator()(ASN1_BIT_STRING* p) const {
        ASN1_BIT_STRING_free(p);
    }
};
typedef UniquePtr<ASN1_BIT_STRING, ASN1_BIT_STRING_Delete> Unique_ASN1_BIT_STRING;

struct ASN1_OBJECT_Delete {
    void operator()(ASN1_OBJECT* p) const {
        ASN1_OBJECT_free(p);
    }
};
typedef UniquePtr<ASN1_OBJECT, ASN1_OBJECT_Delete> Unique_ASN1_OBJECT;

struct ASN1_GENERALIZEDTIME_Delete {
    void operator()(ASN1_GENERALIZEDTIME* p) const {
        ASN1_GENERALIZEDTIME_free(p);
    }
};
typedef UniquePtr<ASN1_GENERALIZEDTIME, ASN1_GENERALIZEDTIME_Delete> Unique_ASN1_GENERALIZEDTIME;

struct PKCS7_Delete {
    void operator()(PKCS7* p) const {
        PKCS7_free(p);
    }
};
typedef UniquePtr<PKCS7, PKCS7_Delete> Unique_PKCS7;

struct sk_X509_Delete {
    void operator()(STACK_OF(X509)* p) const {
        sk_X509_pop_free(p, X509_free);
    }
};
typedef UniquePtr<STACK_OF(X509), sk_X509_Delete> Unique_sk_X509;

struct sk_ASN1_OBJECT_Delete {
    void operator()(STACK_OF(ASN1_OBJECT)* p) const {
        sk_ASN1_OBJECT_pop_free(p, ASN1_OBJECT_free);
    }
};
typedef UniquePtr<STACK_OF(ASN1_OBJECT), sk_ASN1_OBJECT_Delete> Unique_sk_ASN1_OBJECT;

struct sk_GENERAL_NAME_Delete {
    void operator()(STACK_OF(GENERAL_NAME)* p) const {
        sk_GENERAL_NAME_pop_free(p, GENERAL_NAME_free);
    }
};
typedef UniquePtr<STACK_OF(GENERAL_NAME), sk_GENERAL_NAME_Delete> Unique_sk_GENERAL_NAME;

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

template<typename T>
static T* FromContextObject(IOpenSSLDigestContext* contextObject, ECode* ec) {
    *ec = NOERROR;
    Int64 ctxRef;
    IOpenSSLNativeReference::Probe(contextObject)->GetNativeContext(&ctxRef);
    T* ref = reinterpret_cast<T*>(ctxRef);
    if (ref == NULL) {
        NATIVE_TRACE("ctx == null");
        *ec = E_NULL_POINTER_EXCEPTION;
    }
    return ref;
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
 * Converts an OpenSSL BIGNUM to a Java byte[] array in two's complement.
 */
static ECode BignumToArray(const BIGNUM* source, const char* sourceName, ArrayOf<Byte>** result)
{
    NATIVE_TRACE("bignumToArray(%p, %s)", source, sourceName);

    if (source == NULL) {
        *result = NULL;
        return E_NULL_POINTER_EXCEPTION;
    }

    size_t numBytes = BN_num_bytes(source) + 1;
    AutoPtr< ArrayOf<Byte> > bytes = ArrayOf<Byte>::Alloc(numBytes);

    unsigned char* tmp = reinterpret_cast<unsigned char*>(bytes->GetPayload());
    if (BN_num_bytes(source) > 0 && BN_bn2bin(source, tmp + 1) <= 0) {
        *result = NULL;
        return ThrowExceptionIfNecessary("bignumToArray");
    }

    // Set the sign and convert to two's complement if necessary for the Java code.
    if (BN_is_negative(source)) {
        bool carry = true;
        for (ssize_t i = numBytes - 1; i >= 0; i--) {
            tmp[i] ^= 0xFF;
            if (carry) {
                carry = (++tmp[i]) == 0;
            }
        }
        *tmp |= 0x80;
    }
    else {
        *tmp = 0x00;
    }

    NATIVE_TRACE("bignumToArray(%p, %s) => %p", source, sourceName, bytes);
    *result = bytes;
    REFCOUNT_ADD(*result);
    return NOERROR;
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

template<typename T, T* (*d2i_func)(T**, const unsigned char**, long)>
T* ByteArrayToASN1(ArrayOf<Byte>* byteArray)
{
    if (byteArray == NULL) {
        NATIVE_TRACE("ByteArrayToASN1(%p) => using byte array failed", byteArray);
        return NULL;
    }

    const unsigned char* tmp = reinterpret_cast<const unsigned char*>(byteArray->GetPayload());
    return d2i_func(NULL, &tmp, byteArray->GetLength());
}

/**
 * Converts ASN.1 BIT STRING to a jbooleanArray.
 */
ECode ASN1BitStringToBooleanArray(ASN1_BIT_STRING* bitStr, ArrayOf<Boolean>** result)
{
    int size = bitStr->length * 8;
    if (bitStr->flags & ASN1_STRING_FLAG_BITS_LEFT) {
        size -= bitStr->flags & 0x07;
    }

    AutoPtr< ArrayOf<Boolean> > bitsRef = ArrayOf<Boolean>::Alloc(size);

    for (int i = 0; i < size; i++) {
        (*bitsRef)[i] = ASN1_BIT_STRING_get_bit(bitStr, i);
    }

    *result = bitsRef;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

/**
 * To avoid the round-trip to ASN.1 and back in X509_dup, we just up the reference count.
 */
static X509* X509_dup_nocopy(X509* x509)
{
    if (x509 == NULL) {
        return NULL;
    }
    CRYPTO_add(&x509->references, 1, CRYPTO_LOCK_X509);
    return x509;
}

/**
 * BIO for InputStream
 */
class BIO_Stream
{
public:
    BIO_Stream(IInterface* stream)
        : mStream(stream)
        , mEof(FALSE)
    {}

    ~BIO_Stream() {}

    Boolean IsEof() const
    {
        NATIVE_TRACE("isEof? %s", mEof ? "yes" : "no");
        return mEof;
    }

    Int32 Flush()
    {
        ECode ec = IFlushable::Probe(mStream)->Flush();
        if (FAILED(ec)) return -1;

        return 1;
    }

protected:
    AutoPtr<IInterface> GetStream()
    {
        return mStream;
    }

    void SetEof(Boolean eof)
    {
        mEof = eof;
    }

private:
    AutoPtr<IInterface> mStream;
    Boolean mEof;
};

class BIO_InputStream : public BIO_Stream
{
public:
    BIO_InputStream(IInterface* stream)
        : BIO_Stream(stream)
    {}

    Int32 Read(char *buf, Int32 len)
    {
        AutoPtr< ArrayOf<Byte> > bytes = ArrayOf<Byte>::Alloc(len);
        Int32 read;
        ECode ec = IInputStream::Probe(GetStream())->Read(bytes, &read);
        if (FAILED(ec)) {
            NATIVE_TRACE("BIO_InputStream::read failed call to InputStream#read");
            return -1;
        }

        /* Java uses -1 to indicate EOF condition. */
        if (read == -1) {
            SetEof(TRUE);
            read = 0;
        }
        else if (read > 0) {
            memcpy(buf, bytes->GetPayload(), read);
        }

        return read;
    }

    Int32 Gets(char *buf, Int32 len)
    {
        if (len > PEM_LINE_LENGTH) {
            len = PEM_LINE_LENGTH;
        }

        AutoPtr< ArrayOf<Byte> > bytes = ArrayOf<Byte>::Alloc(len - 1);
        Int32 read;
        ECode ec = IOpenSSLBIOInputStream::Probe(GetStream())->Gets(bytes, &read);
        if (FAILED(ec)) {
            NATIVE_TRACE("BIO_InputStream::read failed call to InputStream#read");
            return -1;
        }

        /* Java uses -1 to indicate EOF condition. */
        if (read == -1) {
            SetEof(TRUE);
            read = 0;
        }
        else if (read > 0) {
            memcpy(buf, bytes->GetPayload(), read);
        }

        buf[read] = '\0';
        NATIVE_TRACE("BIO::gets \"%s\"", buf);
        return read;
    }

public:
    /** Length of PEM-encoded line (64) plus CR plus NULL */
    static const Int32 PEM_LINE_LENGTH = 66;
};

class BIO_OutputStream : public BIO_Stream
{
public:
    BIO_OutputStream(IInterface* stream)
        : BIO_Stream(stream)
    {}

    Int32 Write(const char *buf, Int32 len)
    {
        AutoPtr< ArrayOf<Byte> > bytes = ArrayOf<Byte>::Alloc(len);
        bytes->Copy(reinterpret_cast<const Byte*>(buf), len);

        ECode ec = IOutputStream::Probe(GetStream())->Write(bytes);
        if (FAILED(ec)) {
            NATIVE_TRACE("BIO_OutputStream::write => failed call to OutputStream#write");
            return -1;
        }

        return len;
    }
};

static int bio_stream_create(BIO *b)
{
    b->init = 1;
    b->num = 0;
    b->ptr = NULL;
    b->flags = 0;
    return 1;
}

static int bio_stream_destroy(BIO *b)
{
    if (b == NULL) {
        return 0;
    }

    if (b->ptr != NULL) {
        delete static_cast<BIO_Stream*>(b->ptr);
        b->ptr = NULL;
    }

    b->init = 0;
    b->flags = 0;
    return 1;
}

static int bio_stream_read(BIO *b, char *buf, int len)
{
    BIO_clear_retry_flags(b);
    BIO_InputStream* stream = static_cast<BIO_InputStream*>(b->ptr);
    int ret = stream->Read(buf, len);
    if (ret == 0) {
        // EOF is indicated by -1 with a BIO flag.
        BIO_set_retry_read(b);
        return -1;
    }
    return ret;
}

static int bio_stream_write(BIO *b, const char *buf, int len)
{
    BIO_clear_retry_flags(b);
    BIO_OutputStream* stream = static_cast<BIO_OutputStream*>(b->ptr);
    return stream->Write(buf, len);
}

static int bio_stream_puts(BIO *b, const char *buf)
{
    BIO_OutputStream* stream = static_cast<BIO_OutputStream*>(b->ptr);
    return stream->Write(buf, strlen(buf));
}

static int bio_stream_gets(BIO *b, char *buf, int len)
{
    BIO_InputStream* stream = static_cast<BIO_InputStream*>(b->ptr);
    return stream->Gets(buf, len);
}

static void bio_stream_assign(BIO *b, BIO_Stream* stream)
{
    b->ptr = static_cast<void*>(stream);
}

static long bio_stream_ctrl(BIO *b, int cmd, long, void *)
{
    BIO_Stream* stream = static_cast<BIO_Stream*>(b->ptr);

    switch (cmd) {
    case BIO_CTRL_EOF:
        return stream->IsEof() ? 1 : 0;
    case BIO_CTRL_FLUSH:
        return stream->Flush();
    default:
        return 0;
    }
}

static BIO_METHOD stream_bio_method = {
        ( 100 | 0x0400 ), /* source/sink BIO */
        "InputStream/OutputStream BIO",
        bio_stream_write, /* bio_write */
        bio_stream_read, /* bio_read */
        bio_stream_puts, /* bio_puts */
        bio_stream_gets, /* bio_gets */
        bio_stream_ctrl, /* bio_ctrl */
        bio_stream_create, /* bio_create */
        bio_stream_destroy, /* bio_free */
        NULL, /* no bio_callback_ctrl */
};

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
    ::BIO_write(buffer.get(), "\0", 1);

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
    ::BIO_write(buffer.get(), "\0", 1);

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

ECode NativeCrypto::RSA_generate_key_ex(
    /* [in] */ Int32 modulusBits,
    /* [in] */ ArrayOf<Byte>* publicExponent,
    /* [out] */ Int64* result)
{
    VALIDATE_NOT_NULL(result);

    NATIVE_TRACE("RSA_generate_key_ex(%d, %p)", modulusBits, publicExponent);

    ECode ec;
    BIGNUM* eRef = NULL;
    if (!ArrayToBignum(publicExponent, &eRef, &ec)) {
        *result = 0;
        return ec;
    }
    Unique_BIGNUM e(eRef);

    Unique_RSA rsa(RSA_new());
    if (rsa.get() == NULL) {
        *result = 0;
        return E_OUT_OF_MEMORY_ERROR;
    }

    if (::RSA_generate_key_ex(rsa.get(), modulusBits, e.get(), NULL) < 0) {
        *result = 0;
        return ThrowExceptionIfNecessary("RSA_generate_key_ex");
    }

    Unique_EVP_PKEY pkey(EVP_PKEY_new());
    if (pkey.get() == NULL) {
        *result = 0;
        return ThrowRuntimeException("RSA_generate_key_ex failed");
    }

    if (EVP_PKEY_assign_RSA(pkey.get(), rsa.get()) != 1) {
        *result = 0;
        return ThrowRuntimeException("RSA_generate_key_ex failed");
    }

    OWNERSHIP_TRANSFERRED(rsa);
    NATIVE_TRACE("RSA_generate_key_ex(n=%d, e=%p) => %p", modulusBits, publicExponent, pkey.get());
    *result = reinterpret_cast<uintptr_t>(pkey.release());
    return NOERROR;
}

ECode NativeCrypto::RSA_size(
    /* [in] */ Int64 pkeyRef,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    EVP_PKEY* pkey = reinterpret_cast<EVP_PKEY*>(pkeyRef);
    NATIVE_TRACE("RSA_size(%p)", pkey);

    if (pkey == NULL) {
        *result = 0;
        return E_NULL_POINTER_EXCEPTION;
    }

    Unique_RSA rsa(EVP_PKEY_get1_RSA(pkey));
    if (rsa.get() == NULL) {
        *result = 0;
        return ThrowRuntimeException("RSA_size failed");
    }

    *result = static_cast<Int32>(::RSA_size(rsa.get()));
    return NOERROR;
}

typedef int RSACryptOperation(int flen, const unsigned char* from, unsigned char* to, RSA* rsa,
                              int padding);

static ECode RSA_crypt_operation(RSACryptOperation operation,
        const char* caller __attribute__ ((unused)), Int32 flen,
        ArrayOf<Byte>* from, ArrayOf<Byte>* to, Int64 pkeyRef, Int32 padding, Int32* result)
{
    EVP_PKEY* pkey = reinterpret_cast<EVP_PKEY*>(pkeyRef);
    NATIVE_TRACE("%s(%d, %p, %p, %p)", caller, flen, from, to, pkey);

    if (pkey == NULL) {
        *result = -1;
        return E_NULL_POINTER_EXCEPTION;
    }

    Unique_RSA rsa(EVP_PKEY_get1_RSA(pkey));
    if (rsa.get() == NULL) {
        *result = -1;
        return NOERROR;
    }

    if (from == NULL) {
        *result = -1;
        return NOERROR;
    }

    if (to == NULL) {
        *result = -1;
        return NOERROR;
    }

    int resultSize = operation(static_cast<int>(flen),
            reinterpret_cast<const unsigned char*>(from->GetPayload()),
            reinterpret_cast<unsigned char*>(to->GetPayload()), rsa.get(), padding);
    if (resultSize == -1) {
        NATIVE_TRACE("%s => failed", caller);
        *result = -1;
        return ThrowExceptionIfNecessary("RSA_crypt_operation");
    }

    NATIVE_TRACE("%s(%d, %p, %p, %p) => %d", caller, flen, from, to, pkey,
            resultSize);
    *result = static_cast<Int32>(resultSize);
    return NOERROR;
}

ECode NativeCrypto::RSA_private_encrypt(
    /* [in] */ Int32 flen,
    /* [in] */ ArrayOf<Byte>* from,
    /* [in] */ ArrayOf<Byte>* to,
    /* [in] */ Int64 pkeyRef,
    /* [in] */ Int32 padding,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    return RSA_crypt_operation(::RSA_private_encrypt, __FUNCTION__,
                               flen, from, to, pkeyRef, padding, result);
}

ECode NativeCrypto::RSA_public_decrypt(
    /* [in] */ Int32 flen,
    /* [in] */ ArrayOf<Byte>* from,
    /* [in] */ ArrayOf<Byte>* to,
    /* [in] */ Int64 pkeyRef,
    /* [in] */ Int32 padding,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    return RSA_crypt_operation(::RSA_public_decrypt, __FUNCTION__,
                               flen, from, to, pkeyRef, padding, result);
}

ECode NativeCrypto::RSA_public_encrypt(
    /* [in] */ Int32 flen,
    /* [in] */ ArrayOf<Byte>* from,
    /* [in] */ ArrayOf<Byte>* to,
    /* [in] */ Int64 pkeyRef,
    /* [in] */ Int32 padding,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    return RSA_crypt_operation(::RSA_public_encrypt, __FUNCTION__,
                               flen, from, to, pkeyRef, padding, result);
}

ECode NativeCrypto::RSA_private_decrypt(
    /* [in] */ Int32 flen,
    /* [in] */ ArrayOf<Byte>* from,
    /* [in] */ ArrayOf<Byte>* to,
    /* [in] */ Int64 pkeyRef,
    /* [in] */ Int32 padding,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    return RSA_crypt_operation(::RSA_private_decrypt, __FUNCTION__,
                               flen, from, to, pkeyRef, padding, result);
}

ECode NativeCrypto::Get_RSA_public_params(
    /* [in] */ Int64 pkeyRef,
    /* [out, callee] */ ArrayOf<Byte>** n,
    /* [out, callee] */ ArrayOf<Byte>** e)
{
    VALIDATE_NOT_NULL(n);
    VALIDATE_NOT_NULL(e);

    EVP_PKEY* pkey = reinterpret_cast<EVP_PKEY*>(pkeyRef);
    NATIVE_TRACE("get_RSA_public_params(%p)", pkey);

    if (pkey == NULL) {
        *n = NULL;
        *e = NULL;
        return E_NULL_POINTER_EXCEPTION;
    }

    Unique_RSA rsa(EVP_PKEY_get1_RSA(pkey));
    if (rsa.get() == NULL) {
        *n = NULL;
        *e = NULL;
        return ThrowExceptionIfNecessary("get_RSA_public_params failed");
    }

    ECode ec = BignumToArray(rsa->n, "n", n);
    if (FAILED(ec)) {
        *n = NULL;
        *e = NULL;
        return ec;
    }

    ec = BignumToArray(rsa->e, "e", e);
    if (FAILED(ec)) {
        *n = NULL;
        *e = NULL;
        return ec;
    }

    return NOERROR;
}

ECode NativeCrypto::Get_RSA_private_params(
    /* [in] */ Int64 pkeyRef,
    /* [out, callee] */ ArrayOf<Byte>** n,
    /* [out, callee] */ ArrayOf<Byte>** e,
    /* [out, callee] */ ArrayOf<Byte>** d,
    /* [out, callee] */ ArrayOf<Byte>** p,
    /* [out, callee] */ ArrayOf<Byte>** q,
    /* [out, callee] */ ArrayOf<Byte>** dmp1,
    /* [out, callee] */ ArrayOf<Byte>** dmq1,
    /* [out, callee] */ ArrayOf<Byte>** iqmp)
{
    VALIDATE_NOT_NULL(n && e && d && p && q && dmp1 && dmq1 && iqmp);

    EVP_PKEY* pkey = reinterpret_cast<EVP_PKEY*>(pkeyRef);
    NATIVE_TRACE("get_RSA_public_params(%p)", pkey);

    if (pkey == NULL) {
        *n = *e = *d = *p = *q = *dmp1 = *dmq1 = *iqmp = NULL;
        return E_NULL_POINTER_EXCEPTION;
    }

    Unique_RSA rsa(EVP_PKEY_get1_RSA(pkey));
    if (rsa.get() == NULL) {
        *n = *e = *d = *p = *q = *dmp1 = *dmq1 = *iqmp = NULL;
        return ThrowExceptionIfNecessary("get_RSA_public_params failed");
    }

    ECode ec = BignumToArray(rsa->n, "n", n);
    if (FAILED(ec)) {
        *n = *e = *d = *p = *q = *dmp1 = *dmq1 = *iqmp = NULL;
        return ec;
    }

    *e = *d = *p = *q = *dmp1 = *dmq1 = *iqmp = NULL;

    if (rsa->e != NULL) {
        ec = BignumToArray(rsa->e, "e", e);
        if (FAILED(ec)) {
            *n = *e = *d = *p = *q = *dmp1 = *dmq1 = *iqmp = NULL;
            return ec;
        }
    }

    if (rsa->d != NULL) {
        ec = BignumToArray(rsa->d, "d", d);
        if (FAILED(ec)) {
            *n = *e = *d = *p = *q = *dmp1 = *dmq1 = *iqmp = NULL;
            return ec;
        }
    }

    if (rsa->p != NULL) {
        ec = BignumToArray(rsa->p, "p", p);
        if (FAILED(ec)) {
            *n = *e = *d = *p = *q = *dmp1 = *dmq1 = *iqmp = NULL;
            return ec;
        }
    }

    if (rsa->q != NULL) {
        ec = BignumToArray(rsa->q, "q", q);
        if (FAILED(ec)) {
            *n = *e = *d = *p = *q = *dmp1 = *dmq1 = *iqmp = NULL;
            return ec;
        }
    }

    if (rsa->dmp1 != NULL) {
        ec = BignumToArray(rsa->dmp1, "dmp1", dmp1);
        if (FAILED(ec)) {
            *n = *e = *d = *p = *q = *dmp1 = *dmq1 = *iqmp = NULL;
            return ec;
        }
    }

    if (rsa->dmq1 != NULL) {
        ec = BignumToArray(rsa->dmq1, "dmq1", dmq1);
        if (FAILED(ec)) {
            *n = *e = *d = *p = *q = *dmp1 = *dmq1 = *iqmp = NULL;
            return ec;
        }
    }

    if (rsa->iqmp != NULL) {
        ec = BignumToArray(rsa->iqmp, "iqmp", iqmp);
        if (FAILED(ec)) {
            *n = *e = *d = *p = *q = *dmp1 = *dmq1 = *iqmp = NULL;
            return ec;
        }
    }

    return NOERROR;
}

ECode NativeCrypto::DSA_generate_key(
    /* [in] */ Int32 primeBits,
    /* [in] */ ArrayOf<Byte>* seed,
    /* [in] */ ArrayOf<Byte>* g,
    /* [in] */ ArrayOf<Byte>* p,
    /* [in] */ ArrayOf<Byte>* q,
    /* [out] */ Int64* result)
{
    VALIDATE_NOT_NULL(result);

    NATIVE_TRACE("DSA_generate_key(%d, %p, %p, %p, %p)", primeBits, seed, g, p, q);

    UniquePtr<unsigned char[]> seedPtr;
    unsigned long seedSize = 0;
    if (seed != NULL) {
        seedSize = seed->GetLength();
        seedPtr.reset(new unsigned char[seedSize]);

        memcpy(seedPtr.get(), seed->GetPayload(), seedSize);
    }

    Unique_DSA dsa(DSA_new());
    if (dsa.get() == NULL) {
        NATIVE_TRACE("DSA_generate_key failed");
        FreeOpenSslErrorState();
        *result = 0;
        return E_OUT_OF_MEMORY_ERROR;
    }

    if (g != NULL && p != NULL && q != NULL) {
        NATIVE_TRACE("DSA_generate_key parameters specified");

        ECode ec;
        if (!ArrayToBignum(g, &dsa->g, &ec)) {
            *result = 0;
            return ec;
        }

        if (!ArrayToBignum(p, &dsa->p, &ec)) {
            *result = 0;
            return ec;
        }

        if (!ArrayToBignum(q, &dsa->q, &ec)) {
            *result = 0;
            return ec;
        }
    }
    else {
        NATIVE_TRACE("DSA_generate_key generating parameters");

        if (!DSA_generate_parameters_ex(dsa.get(), primeBits, seedPtr.get(), seedSize, NULL, NULL, NULL)) {
            NATIVE_TRACE("DSA_generate_key => param generation failed");
            *result = 0;
            return ThrowExceptionIfNecessary("NativeCrypto_DSA_generate_parameters_ex failed");
        }
    }

    if (!::DSA_generate_key(dsa.get())) {
        NATIVE_TRACE("DSA_generate_key failed");
        *result = 0;
        return ThrowExceptionIfNecessary("NativeCrypto_DSA_generate_key failed");
    }

    Unique_EVP_PKEY pkey(EVP_PKEY_new());
    if (pkey.get() == NULL) {
        NATIVE_TRACE("DSA_generate_key failed");
        FreeOpenSslErrorState();
        *result = 0;
        return ThrowRuntimeException("NativeCrypto_DSA_generate_key failed");
    }

    if (EVP_PKEY_assign_DSA(pkey.get(), dsa.get()) != 1) {
        NATIVE_TRACE("DSA_generate_key failed");
        *result = 0;
        return ThrowExceptionIfNecessary("NativeCrypto_DSA_generate_key failed");
    }

    OWNERSHIP_TRANSFERRED(dsa);
    NATIVE_TRACE("DSA_generate_key(n=%d, e=%p) => %p", primeBits, seedPtr.get(), pkey.get());
    *result = reinterpret_cast<uintptr_t>(pkey.release());
    return NOERROR;
}

ECode NativeCrypto::Get_DSA_params(
    /* [in] */ Int64 pkeyRef,
    /* [out, callee] */ ArrayOf<Byte>** g,
    /* [out, callee] */ ArrayOf<Byte>** p,
    /* [out, callee] */ ArrayOf<Byte>** q,
    /* [out, callee] */ ArrayOf<Byte>** ypub,
    /* [out, callee] */ ArrayOf<Byte>** xpriv)
{
    VALIDATE_NOT_NULL(g && p && q && ypub && xpriv);

    EVP_PKEY* pkey = reinterpret_cast<EVP_PKEY*>(pkeyRef);
    NATIVE_TRACE("get_DSA_params(%p)", pkey);

    if (pkey == NULL) {
        *g = *p = *q = *ypub = *xpriv = NULL;
        return E_NULL_POINTER_EXCEPTION;
    }

    Unique_DSA dsa(EVP_PKEY_get1_DSA(pkey));
    if (dsa.get() == NULL) {
        *g = *p = *q = *ypub = *xpriv = NULL;
        return ThrowExceptionIfNecessary("get_DSA_params failed");
    }

    *g = *p = *q = *ypub = *xpriv = NULL;

    if (dsa->g != NULL) {
        ECode ec = BignumToArray(dsa->g, "g", g);
        if (FAILED(ec)) {
            *g = *p = *q = *ypub = *xpriv = NULL;
            return ec;
        }
    }

    if (dsa->p != NULL) {
        ECode ec = BignumToArray(dsa->p, "p", p);
        if (FAILED(ec)) {
            *g = *p = *q = *ypub = *xpriv = NULL;
            return ec;
        }
    }

    if (dsa->q != NULL) {
        ECode ec = BignumToArray(dsa->q, "q", q);
        if (FAILED(ec)) {
            *g = *p = *q = *ypub = *xpriv = NULL;
            return ec;
        }
    }

    if (dsa->pub_key != NULL) {
        ECode ec = BignumToArray(dsa->pub_key, "pub_key", ypub);
        if (FAILED(ec)) {
            *g = *p = *q = *ypub = *xpriv = NULL;
            return ec;
        }
    }

    if (dsa->priv_key != NULL) {
        ECode ec = BignumToArray(dsa->priv_key, "priv_key", xpriv);
        if (FAILED(ec)) {
            *g = *p = *q = *ypub = *xpriv = NULL;
            return ec;
        }
    }

    return NOERROR;
}

ECode NativeCrypto::Set_DSA_flag_nonce_from_hash(
    /* [in] */ Int64 pkeyRef)
{
    EVP_PKEY* pkey = reinterpret_cast<EVP_PKEY*>(pkeyRef);
    NATIVE_TRACE("set_DSA_flag_nonce_from_hash(%p)", pkey);

    if (pkey == NULL) {
        return E_NULL_POINTER_EXCEPTION;
    }

    Unique_DSA dsa(EVP_PKEY_get1_DSA(pkey));
    if (dsa.get() == NULL) {
        return ThrowExceptionIfNecessary("set_DSA_flag_nonce_from_hash failed");
    }

    dsa->flags |= DSA_FLAG_NONCE_FROM_HASH;
    return NOERROR;
}

ECode NativeCrypto::I2d_RSAPublicKey(
    /* [in] */ Int64 pkeyRef,
    /* [out, callee] */ ArrayOf<Byte>** result)
{
    VALIDATE_NOT_NULL(result);

    assert(0);
    return NOERROR;
}

ECode NativeCrypto::I2d_RSAPrivateKey(
    /* [in] */ Int64 pkeyRef,
    /* [out, callee] */ ArrayOf<Byte>** result)
{
    VALIDATE_NOT_NULL(result);

    assert(0);
    return NOERROR;
}

ECode NativeCrypto::I2d_DSAPublicKey(
    /* [in] */ Int64 pkeyRef,
    /* [out, callee] */ ArrayOf<Byte>** result)
{
    VALIDATE_NOT_NULL(result);

    assert(0);
    return NOERROR;
}

ECode NativeCrypto::I2d_DSAPrivateKey(
    /* [in] */ Int64 pkeyRef,
    /* [out, callee] */ ArrayOf<Byte>** result)
{
    VALIDATE_NOT_NULL(result);

    assert(0);
    return NOERROR;
}

ECode NativeCrypto::EVP_PKEY_new_DH(
    /* [in] */ ArrayOf<Byte>* p,
    /* [in] */ ArrayOf<Byte>* g,
    /* [in] */ ArrayOf<Byte>* pub_key,
    /* [in] */ ArrayOf<Byte>* priv_key,
    /* [out] */ Int64* result)
{
    VALIDATE_NOT_NULL(result);

    NATIVE_TRACE("EVP_PKEY_new_DH(p=%p, g=%p, pub_key=%p, priv_key=%p)",
              p, g, pub_key, priv_key);

    Unique_DH dh(DH_new());
    if (dh.get() == NULL) {
        *result = 0;
        return ThrowRuntimeException("DH_new failed");
    }

    ECode ec;
    if (!ArrayToBignum(p, &dh->p, &ec)) {
        *result = 0;
        return ec;
    }

    if (!ArrayToBignum(g, &dh->g, &ec)) {
        *result = 0;
        return ec;
    }

    if (pub_key != NULL && !ArrayToBignum(pub_key, &dh->pub_key, &ec)) {
        *result = 0;
        return ec;
    }

    if (priv_key != NULL && !ArrayToBignum(priv_key, &dh->priv_key, &ec)) {
        *result = 0;
        return ec;
    }

    if (dh->p == NULL || dh->g == NULL
            || (pub_key != NULL && dh->pub_key == NULL)
            || (priv_key != NULL && dh->priv_key == NULL)) {
        *result = 0;
        return ThrowRuntimeException("Unable to convert BigInteger to BIGNUM");
    }

    /* The public key can be recovered if the private key is available. */
    if (dh->pub_key == NULL && dh->priv_key != NULL) {
        if (!::DH_generate_key(dh.get())) {
            *result = 0;
            return ThrowRuntimeException("EVP_PKEY_new_DH failed during pub_key generation");
        }
    }

    Unique_EVP_PKEY pkey(EVP_PKEY_new());
    if (pkey.get() == NULL) {
        *result = 0;
        return ThrowRuntimeException("EVP_PKEY_new failed");
    }
    if (EVP_PKEY_assign_DH(pkey.get(), dh.get()) != 1) {
        *result = 0;
        return ThrowRuntimeException("EVP_PKEY_assign_DH failed");
    }
    OWNERSHIP_TRANSFERRED(dh);
    NATIVE_TRACE("EVP_PKEY_new_DH(p=%p, g=%p, pub_key=%p, priv_key=%p) => %p",
              p, g, pub_key, priv_key, pkey.get());
    *result = reinterpret_cast<Int64>(pkey.release());
    return NOERROR;
}

ECode NativeCrypto::DH_generate_parameters_ex(
    /* [in] */ Int32 primeBits,
    /* [in] */ Int64 generator,
    /* [out] */ Int64* result)
{
    VALIDATE_NOT_NULL(result);

    NATIVE_TRACE("DH_generate_parameters_ex(%d, %d)", primeBits, generator);

    Unique_DH dh(DH_new());
    if (dh.get() == NULL) {
        NATIVE_TRACE("DH_generate_parameters_ex failed");
        FreeOpenSslErrorState();
        *result = 0;
        return E_OUT_OF_MEMORY_ERROR;
    }

    NATIVE_TRACE("DH_generate_parameters_ex generating parameters");

    if (!::DH_generate_parameters_ex(dh.get(), primeBits, generator, NULL)) {
        NATIVE_TRACE("DH_generate_parameters_ex => param generation failed");
        *result = 0;
        return ThrowExceptionIfNecessary("NativeCrypto_DH_generate_parameters_ex failed");
    }

    Unique_EVP_PKEY pkey(EVP_PKEY_new());
    if (pkey.get() == NULL) {
        NATIVE_TRACE("DH_generate_parameters_ex failed");
        FreeOpenSslErrorState();
        *result = 0;
        return ThrowRuntimeException("NativeCrypto_DH_generate_parameters_ex failed");
    }

    if (EVP_PKEY_assign_DH(pkey.get(), dh.get()) != 1) {
        NATIVE_TRACE("DH_generate_parameters_ex failed");
        *result = 0;
        return ThrowExceptionIfNecessary("NativeCrypto_DH_generate_parameters_ex failed");
    }

    OWNERSHIP_TRANSFERRED(dh);
    NATIVE_TRACE("DH_generate_parameters_ex(n=%d, g=%d) => %p", primeBits, generator, pkey.get());
    *result = reinterpret_cast<uintptr_t>(pkey.release());
    return NOERROR;
}

ECode NativeCrypto::DH_generate_key(
    /* [in] */ Int64 pkeyRef)
{
    NATIVE_TRACE("DH_generate_key(%p)", pkeyRef);
    EVP_PKEY* pkey = reinterpret_cast<EVP_PKEY*>(pkeyRef);

    if (pkey == NULL) {
        return E_NULL_POINTER_EXCEPTION;
    }

    Unique_DH dh(EVP_PKEY_get1_DH(pkey));
    if (dh.get() == NULL) {
        NATIVE_TRACE("DH_generate_key failed");
        FreeOpenSslErrorState();
        return ThrowExceptionIfNecessary("Unable to get DH key");
    }

    if (!::DH_generate_key(dh.get())) {
        NATIVE_TRACE("DH_generate_key failed");
        return ThrowExceptionIfNecessary("NativeCrypto_DH_generate_key failed");
    }

    return NOERROR;
}

ECode NativeCrypto::Get_DH_params(
    /* [in] */ Int64 pkeyRef,
    /* [out, callee] */ ArrayOf<Byte>** p,
    /* [out, callee] */ ArrayOf<Byte>** g,
    /* [out, callee] */ ArrayOf<Byte>** ypub,
    /* [out, callee] */ ArrayOf<Byte>** xpriv)
{
    VALIDATE_NOT_NULL(p && g && ypub && xpriv);

    EVP_PKEY* pkey = reinterpret_cast<EVP_PKEY*>(pkeyRef);
    NATIVE_TRACE("get_DH_params(%p)", pkey);

    if (pkey == NULL) {
        *p = *g = *ypub = *xpriv = NULL;
        return E_NULL_POINTER_EXCEPTION;
    }

    Unique_DH dh(EVP_PKEY_get1_DH(pkey));
    if (dh.get() == NULL) {
        *p = *g = *ypub = *xpriv = NULL;
        return ThrowExceptionIfNecessary("get_DH_params failed");
    }

    *p = *g = *ypub = *xpriv = NULL;

    if (dh->p != NULL) {
        ECode ec = BignumToArray(dh->p, "p", p);
        if (FAILED(ec)) {
            *p = *g = *ypub = *xpriv = NULL;
            return ec;
        }
    }

    if (dh->g != NULL) {
        ECode ec = BignumToArray(dh->g, "g", g);
        if (FAILED(ec)) {
            *p = *g = *ypub = *xpriv = NULL;
            return ec;
        }
    }

    if (dh->pub_key != NULL) {
        ECode ec = BignumToArray(dh->pub_key, "pub_key", ypub);
        if (FAILED(ec)) {
            *p = *g = *ypub = *xpriv = NULL;
            return ec;
        }
    }

    if (dh->priv_key != NULL) {
        ECode ec = BignumToArray(dh->priv_key, "priv_key", xpriv);
        if (FAILED(ec)) {
            *p = *g = *ypub = *xpriv = NULL;
            return ec;
        }
    }

    return NOERROR;
}

ECode NativeCrypto::EVP_PKEY_new_EC_KEY(
    /* [in] */ Int64 groupRef,
    /* [in] */ Int64 pubkeyRef,
    /* [in] */ ArrayOf<Byte>* keyBytes,
    /* [out] */ Int64* result)
{
    VALIDATE_NOT_NULL(result);

    const EC_GROUP* group = reinterpret_cast<const EC_GROUP*>(groupRef);
    const EC_POINT* pubkey = reinterpret_cast<const EC_POINT*>(pubkeyRef);
    NATIVE_TRACE("EVP_PKEY_new_EC_KEY(%p, %p, %p)", group, pubkey, keyBytes);

    Unique_BIGNUM key(NULL);
    if (keyBytes != NULL) {
        ECode ec;
        BIGNUM* keyRef = NULL;
        if (!ArrayToBignum(keyBytes, &keyRef, &ec)) {
            *result = 0;
            return ec;
        }
        key.reset(keyRef);
    }

    Unique_EC_KEY eckey(EC_KEY_new());
    if (eckey.get() == NULL) {
        *result = 0;
        return ThrowRuntimeException("EC_KEY_new failed");
    }

    if (EC_KEY_set_group(eckey.get(), group) != 1) {
        NATIVE_TRACE("EVP_PKEY_new_EC_KEY(%p, %p, %p) > EC_KEY_set_group failed", group, pubkey,
                keyBytes);
        *result = 0;
        return ThrowExceptionIfNecessary("EC_KEY_set_group");
    }

    if (pubkey != NULL) {
        if (EC_KEY_set_public_key(eckey.get(), pubkey) != 1) {
            NATIVE_TRACE("EVP_PKEY_new_EC_KEY(%p, %p, %p) => EC_KEY_set_private_key failed", group,
                    pubkey, keyBytes);
            *result = 0;
            return ThrowExceptionIfNecessary("EC_KEY_set_public_key");
        }
    }

    if (key.get() != NULL) {
        if (EC_KEY_set_private_key(eckey.get(), key.get()) != 1) {
            NATIVE_TRACE("EVP_PKEY_new_EC_KEY(%p, %p, %p) => EC_KEY_set_private_key failed", group,
                    pubkey, keyBytes);
            *result = 0;
            return ThrowExceptionIfNecessary("EC_KEY_set_private_key");
        }
        if (pubkey == NULL) {
            Unique_EC_POINT calcPubkey(::EC_POINT_new(group));
            if (!EC_POINT_mul(group, calcPubkey.get(), key.get(), NULL, NULL, NULL)) {
                NATIVE_TRACE("EVP_PKEY_new_EC_KEY(%p, %p, %p) => can't calulate public key", group,
                        pubkey, keyBytes);
                *result = 0;
                return ThrowExceptionIfNecessary("EC_KEY_set_private_key");
            }
            EC_KEY_set_public_key(eckey.get(), calcPubkey.get());
        }
    }

    if (!EC_KEY_check_key(eckey.get())) {
        NATIVE_TRACE("EVP_KEY_new_EC_KEY(%p, %p, %p) => invalid key created", group, pubkey, keyBytes);
        *result = 0;
        return ThrowExceptionIfNecessary("EC_KEY_check_key");
    }

    Unique_EVP_PKEY pkey(EVP_PKEY_new());
    if (pkey.get() == NULL) {
        NATIVE_TRACE("EVP_PKEY_new_EC(%p, %p, %p) => threw error", group, pubkey, keyBytes);
        *result = 0;
        return ThrowExceptionIfNecessary("EVP_PKEY_new failed");
    }
    if (EVP_PKEY_assign_EC_KEY(pkey.get(), eckey.get()) != 1) {
        NATIVE_TRACE("EVP_PKEY_new_EC(%p, %p, %p) => threw error", group, pubkey, keyBytes);
        *result = 0;
        return ThrowRuntimeException("EVP_PKEY_assign_EC_KEY failed");
    }
    OWNERSHIP_TRANSFERRED(eckey);

    NATIVE_TRACE("EVP_PKEY_new_EC_KEY(%p, %p, %p) => %p", group, pubkey, keyBytes, pkey.get());
    *result = reinterpret_cast<uintptr_t>(pkey.release());
    return NOERROR;
}

#define EC_CURVE_GFP 1
#define EC_CURVE_GF2M 2

/**
 * Return group type or 0 if unknown group.
 * EC_GROUP_GFP or EC_GROUP_GF2M
 */
static int get_EC_GROUP_type(const EC_GROUP* group)
{
    const EC_METHOD* method = EC_GROUP_method_of(group);
    if (method == EC_GFp_nist_method()
                || method == EC_GFp_mont_method()
                || method == EC_GFp_simple_method()) {
        return EC_CURVE_GFP;
    }
    else if (method == EC_GF2m_simple_method()) {
        return EC_CURVE_GF2M;
    }

    return 0;
}

ECode NativeCrypto::EC_GROUP_new_by_curve_name(
    /* [in] */ const String& curveName,
    /* [out] */ Int64* result)
{
    VALIDATE_NOT_NULL(result);

    NATIVE_TRACE("EC_GROUP_new_by_curve_name()");

    if (curveName.IsNull()) {
        *result = 0;
        return NOERROR;
    }
    NATIVE_TRACE("EC_GROUP_new_by_curve_name(%s)", curveName.string());

    int nid = OBJ_sn2nid(curveName.string());
    if (nid == NID_undef) {
        NATIVE_TRACE("EC_GROUP_new_by_curve_name(%s) => unknown NID name", curveName.string());
        *result = 0;
        return NOERROR;
    }

    EC_GROUP* group = ::EC_GROUP_new_by_curve_name(nid);
    if (group == NULL) {
        NATIVE_TRACE("EC_GROUP_new_by_curve_name(%s) => unknown NID %d", curveName.string(), nid);
        FreeOpenSslErrorState();
        *result = 0;
        return NOERROR;
    }

    NATIVE_TRACE("EC_GROUP_new_by_curve_name(%s) => %p", curveName.string(), group);
    *result = reinterpret_cast<uintptr_t>(group);
    return NOERROR;
}

ECode NativeCrypto::EC_GROUP_new_curve(
    /* [in] */ Int32 type,
    /* [in] */ ArrayOf<Byte>* pBytes,
    /* [in] */ ArrayOf<Byte>* aBytes,
    /* [in] */ ArrayOf<Byte>* bBytes,
    /* [out] */ Int64* result)
{
    VALIDATE_NOT_NULL(result);

    NATIVE_TRACE("EC_GROUP_new_curve(%d, %p, %p, %p)", type, pBytes, aBytes, bBytes);

    ECode ec;
    BIGNUM* pRef = NULL;
    if (!ArrayToBignum(pBytes, &pRef, &ec)) {
        *result = 0;
        return ec;
    }
    Unique_BIGNUM p(pRef);

    BIGNUM* aRef = NULL;
    if (!ArrayToBignum(aBytes, &aRef, &ec)) {
        *result = 0;
        return ec;
    }
    Unique_BIGNUM a(aRef);

    BIGNUM* bRef = NULL;
    if (!ArrayToBignum(bBytes, &bRef, &ec)) {
        *result = 0;
        return ec;
    }
    Unique_BIGNUM b(bRef);

    EC_GROUP* group;
    switch (type) {
    case EC_CURVE_GFP:
        group = ::EC_GROUP_new_curve_GFp(p.get(), a.get(), b.get(), (BN_CTX*) NULL);
        break;
    case EC_CURVE_GF2M:
        group = ::EC_GROUP_new_curve_GF2m(p.get(), a.get(), b.get(), (BN_CTX*) NULL);
        break;
    default:
        *result = 0;
        return ThrowRuntimeException("invalid group");
    }

    if (group == NULL) {
        *result = 0;
        return ThrowExceptionIfNecessary("EC_GROUP_new_curve");
    }

    NATIVE_TRACE("EC_GROUP_new_curve(%d, %p, %p, %p) => %p", type, pBytes, aBytes, bBytes, group);
    *result = reinterpret_cast<uintptr_t>(group);
    return NOERROR;
}

ECode NativeCrypto::EC_GROUP_dup(
    /* [in] */ Int64 groupRef,
    /* [out] */ Int64* result)
{
    VALIDATE_NOT_NULL(result);

    const EC_GROUP* group = reinterpret_cast<const EC_GROUP*>(groupRef);
    NATIVE_TRACE("EC_GROUP_dup(%p)", group);

    if (group == NULL) {
         NATIVE_TRACE("EC_GROUP_dup => group == NULL");
         *result = 0;
         return E_NULL_POINTER_EXCEPTION;
     }

     EC_GROUP* groupDup = ::EC_GROUP_dup(group);
     NATIVE_TRACE("EC_GROUP_dup(%p) => %p", group, groupDup);
     *result = reinterpret_cast<uintptr_t>(groupDup);
     return NOERROR;
}

ECode NativeCrypto::EC_GROUP_set_asn1_flag(
    /* [in] */ Int64 groupRef,
    /* [in] */ Int32 flag)
{
    EC_GROUP* group = reinterpret_cast<EC_GROUP*>(groupRef);
    NATIVE_TRACE("EC_GROUP_set_asn1_flag(%p, %d)", group, flag);

    if (group == NULL) {
        NATIVE_TRACE("EC_GROUP_set_asn1_flag => group == NULL");
        return E_NULL_POINTER_EXCEPTION;
    }

    ::EC_GROUP_set_asn1_flag(group, flag);
    NATIVE_TRACE("EC_GROUP_set_asn1_flag(%p, %d) => success", group, flag);
    return NOERROR;
}

ECode NativeCrypto::EC_GROUP_set_point_conversion_form(
    /* [in] */ Int64 groupRef,
    /* [in] */ Int32 form)
{
    EC_GROUP* group = reinterpret_cast<EC_GROUP*>(groupRef);
    NATIVE_TRACE("EC_GROUP_set_point_conversion_form(%p, %d)", group, form);

    if (group == NULL) {
        NATIVE_TRACE("EC_GROUP_set_point_conversion_form => group == NULL");
        return E_NULL_POINTER_EXCEPTION;
    }

    ::EC_GROUP_set_point_conversion_form(group, static_cast<point_conversion_form_t>(form));
    NATIVE_TRACE("EC_GROUP_set_point_conversion_form(%p, %d) => success", group, form);
    return NOERROR;
}

ECode NativeCrypto::EC_GROUP_get_curve_name(
    /* [in] */ Int64 groupRef,
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);

    const EC_GROUP* group = reinterpret_cast<const EC_GROUP*>(groupRef);
    NATIVE_TRACE("EC_GROUP_get_curve_name(%p)", group);

    if (group == NULL) {
        NATIVE_TRACE("EC_GROUP_get_curve_name => group == NULL");
        *result = NULL;
        return E_NULL_POINTER_EXCEPTION;
    }

    int nid = ::EC_GROUP_get_curve_name(group);
    if (nid == NID_undef) {
        NATIVE_TRACE("EC_GROUP_get_curve_name(%p) => unnamed curve", group);
        *result = NULL;
        return NOERROR;
    }

    const char* shortName = OBJ_nid2sn(nid);
    NATIVE_TRACE("EC_GROUP_get_curve_name(%p) => \"%s\"", group, shortName);
    *result = shortName;
    return NOERROR;
}

ECode NativeCrypto::EC_GROUP_get_curve(
    /* [in] */ Int64 groupRef,
    /* [out, callee] */ ArrayOf<Byte>** pBytes,
    /* [out, callee] */ ArrayOf<Byte>** aBytes,
    /* [out, callee] */ ArrayOf<Byte>** bBytes)
{
    VALIDATE_NOT_NULL(pBytes && aBytes && bBytes);

    const EC_GROUP* group = reinterpret_cast<const EC_GROUP*>(groupRef);
    NATIVE_TRACE("EC_GROUP_get_curve(%p)", group);

    Unique_BIGNUM p(BN_new());
    Unique_BIGNUM a(BN_new());
    Unique_BIGNUM b(BN_new());

    int ret;
    switch (get_EC_GROUP_type(group)) {
    case EC_CURVE_GFP:
        ret = EC_GROUP_get_curve_GFp(group, p.get(), a.get(), b.get(), (BN_CTX*) NULL);
        break;
    case EC_CURVE_GF2M:
        ret = EC_GROUP_get_curve_GF2m(group, p.get(), a.get(), b.get(), (BN_CTX*)NULL);
        break;
    default:
        *pBytes = *aBytes = *bBytes = NULL;
        return ThrowRuntimeException("invalid group");
    }
    if (ret != 1) {
        *pBytes = *aBytes = *bBytes = NULL;
        return ThrowExceptionIfNecessary("EC_GROUP_get_curve");
    }

    ECode ec = BignumToArray(p.get(), "p", pBytes);
    if (FAILED(ec)) {
        *pBytes = *aBytes = *bBytes = NULL;
        return ec;
    }

    ec = BignumToArray(a.get(), "a", aBytes);
    if (FAILED(ec)) {
        *pBytes = *aBytes = *bBytes = NULL;
        return ec;
    }

    ec = BignumToArray(b.get(), "b", bBytes);
    if (FAILED(ec)) {
        *pBytes = *aBytes = *bBytes = NULL;
        return ec;
    }

    NATIVE_TRACE("EC_GROUP_get_curve(%p) => %p, %p, %p", group, *pBytes, *aBytes, *bBytes);
    return NOERROR;
}

ECode NativeCrypto::EC_GROUP_clear_free(
    /* [in] */ Int64 groupRef)
{
    EC_GROUP* group = reinterpret_cast<EC_GROUP*>(groupRef);
    NATIVE_TRACE("EC_GROUP_clear_free(%p)", group);

    if (group == NULL) {
        NATIVE_TRACE("EC_GROUP_clear_free => group == NULL");
        return E_NULL_POINTER_EXCEPTION;
    }

    ::EC_GROUP_clear_free(group);
    NATIVE_TRACE("EC_GROUP_clear_free(%p) => success", group);
    return NOERROR;
}

ECode NativeCrypto::EC_GROUP_cmp(
    /* [in] */ Int64 group1Ref,
    /* [in] */ Int64 group2Ref,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    const EC_GROUP* group1 = reinterpret_cast<const EC_GROUP*>(group1Ref);
    const EC_GROUP* group2 = reinterpret_cast<const EC_GROUP*>(group2Ref);
    NATIVE_TRACE("EC_GROUP_cmp(%p, %p)", group1, group2);

    if (group1 == NULL || group2 == NULL) {
        NATIVE_TRACE("EC_GROUP_cmp(%p, %p) => group1 == null || group2 == null", group1, group2);
        *result = FALSE;
        return E_NULL_POINTER_EXCEPTION;
    }

    int ret = ::EC_GROUP_cmp(group1, group2, (BN_CTX*)NULL);

    NATIVE_TRACE("ECP_GROUP_cmp(%p, %p) => %d", group1, group2, ret);
    *result = ret == 0;
    return NOERROR;
}

ECode NativeCrypto::EC_GROUP_set_generator(
    /* [in] */ Int64 groupRef,
    /* [in] */ Int64 pointRef,
    /* [in] */ ArrayOf<Byte>* nBytes,
    /* [in] */ ArrayOf<Byte>* hBytes)
{
    EC_GROUP* group = reinterpret_cast<EC_GROUP*>(groupRef);
    const EC_POINT* point = reinterpret_cast<const EC_POINT*>(pointRef);
    NATIVE_TRACE("EC_GROUP_set_generator(%p, %p, %p, %p)", group, point, nBytes, hBytes);

    if (group == NULL || point == NULL) {
        NATIVE_TRACE("EC_GROUP_set_generator(%p, %p, %p, %p) => group == null || point == null",
                group, point, nBytes, hBytes);
        return E_NULL_POINTER_EXCEPTION;
    }

    ECode ec;
    BIGNUM* nRef = NULL;
    if (!ArrayToBignum(nBytes, &nRef, &ec)) {
        return ec;
    }
    Unique_BIGNUM n(nRef);

    BIGNUM* hRef = NULL;
    if (!ArrayToBignum(hBytes, &hRef, &ec)) {
        return ec;
    }
    Unique_BIGNUM h(hRef);

    int ret = ::EC_GROUP_set_generator(group, point, n.get(), h.get());
    if (ret == 0) {
        return ThrowExceptionIfNecessary("EC_GROUP_set_generator");
    }

    NATIVE_TRACE("EC_GROUP_set_generator(%p, %p, %p, %p) => %d", group, point, nBytes, hBytes, ret);
    return NOERROR;
}

ECode NativeCrypto::EC_GROUP_get_generator(
    /* [in] */ Int64 groupRef,
    /* [out] */ Int64* result)
{
    VALIDATE_NOT_NULL(result);

    const EC_GROUP* group = reinterpret_cast<const EC_GROUP*>(groupRef);
    NATIVE_TRACE("EC_GROUP_get_generator(%p)", group);

    if (group == NULL) {
        NATIVE_TRACE("EC_POINT_get_generator(%p) => group == null", group);
        *result = 0;
        return E_NULL_POINTER_EXCEPTION;
    }

    const EC_POINT* generator = EC_GROUP_get0_generator(group);

    Unique_EC_POINT dup(EC_POINT_dup(generator, group));
    if (dup.get() == NULL) {
        NATIVE_TRACE("EC_GROUP_get_generator(%p) => oom error", group);
        *result = 0;
        return E_OUT_OF_MEMORY_ERROR;
    }

    NATIVE_TRACE("EC_GROUP_get_generator(%p) => %p", group, dup.get());
    *result = reinterpret_cast<uintptr_t>(dup.release());
    return NOERROR;
}

ECode NativeCrypto::Get_EC_GROUP_type(
    /* [in] */ Int64 groupRef,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    const EC_GROUP* group = reinterpret_cast<const EC_GROUP*>(groupRef);
    NATIVE_TRACE("get_EC_GROUP_type(%p)", group);

    int type = get_EC_GROUP_type(group);
    if (type == 0) {
        NATIVE_TRACE("get_EC_GROUP_type(%p) => curve type", group);
        *result = 0;
        return ThrowRuntimeException("unknown curve type");
    }
    else {
        NATIVE_TRACE("get_EC_GROUP_type(%p) => %d", group, type);
    }
    *result = type;
    return NOERROR;
}

ECode NativeCrypto::EC_GROUP_get_order(
    /* [in] */ Int64 groupRef,
    /* [out, callee] */ ArrayOf<Byte>** result)
{
    VALIDATE_NOT_NULL(result);

    const EC_GROUP* group = reinterpret_cast<const EC_GROUP*>(groupRef);
    NATIVE_TRACE("EC_GROUP_get_order(%p)", group);

    Unique_BIGNUM order(BN_new());
    if (order.get() == NULL) {
        NATIVE_TRACE("EC_GROUP_get_order(%p) => can't create BN", group);
        *result = NULL;
        return E_OUT_OF_MEMORY_ERROR;
    }

    if (::EC_GROUP_get_order(group, order.get(), NULL) != 1) {
        NATIVE_TRACE("EC_GROUP_get_order(%p) => threw error", group);
        *result = NULL;
        return ThrowExceptionIfNecessary("EC_GROUP_get_order");
    }

    ECode ec = BignumToArray(order.get(), "order", result);
    if (FAILED(ec)) {
        *result = NULL;
        return ec;
    }

    NATIVE_TRACE("EC_GROUP_get_order(%p) => %p", group, *result);
    return NOERROR;
}

ECode NativeCrypto::EC_GROUP_get_degree(
    /* [in] */ Int64 groupRef,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    const EC_GROUP* group = reinterpret_cast<const EC_GROUP*>(groupRef);
    NATIVE_TRACE("EC_GROUP_get_degree(%p)", group);

    int degree = ::EC_GROUP_get_degree(group);
    if (degree == 0) {
      NATIVE_TRACE("EC_GROUP_get_degree(%p) => unsupported", group);
      *result = 0;
      return ThrowRuntimeException("not supported");
    }

    NATIVE_TRACE("EC_GROUP_get_degree(%p) => %d", group, degree);
    *result = degree;
    return NOERROR;
}

ECode NativeCrypto::EC_GROUP_get_cofactor(
    /* [in] */ Int64 groupRef,
    /* [out, callee] */ ArrayOf<Byte>** result)
{
    VALIDATE_NOT_NULL(result);

    const EC_GROUP* group = reinterpret_cast<const EC_GROUP*>(groupRef);
    NATIVE_TRACE("EC_GROUP_get_cofactor(%p)", group);

    Unique_BIGNUM cofactor(BN_new());
    if (cofactor.get() == NULL) {
        NATIVE_TRACE("EC_GROUP_get_cofactor(%p) => can't create BN", group);
        *result = NULL;
        return E_OUT_OF_MEMORY_ERROR;
    }

    if (::EC_GROUP_get_cofactor(group, cofactor.get(), NULL) != 1) {
        NATIVE_TRACE("EC_GROUP_get_cofactor(%p) => threw error", group);
        *result = NULL;
        return ThrowExceptionIfNecessary("EC_GROUP_get_cofactor");
    }

    ECode ec = BignumToArray(cofactor.get(), "cofactor", result);
    if (FAILED(ec)) {
        *result = NULL;
        return ec;
    }

    NATIVE_TRACE("EC_GROUP_get_cofactor(%p) => %p", group, *result);
    return NOERROR;
}

ECode NativeCrypto::EC_POINT_new(
    /* [in] */ Int64 groupRef,
    /* [out] */ Int64* result)
{
    VALIDATE_NOT_NULL(result);

    const EC_GROUP* group = reinterpret_cast<const EC_GROUP*>(groupRef);
    NATIVE_TRACE("EC_POINT_new(%p)", group);

    if (group == NULL) {
        NATIVE_TRACE("EC_POINT_new(%p) => group == null", group);
        *result = 0;
        return E_NULL_POINTER_EXCEPTION;
    }

    EC_POINT* point = ::EC_POINT_new(group);
    if (point == NULL) {
        *result = 0;
        return E_OUT_OF_MEMORY_ERROR;
    }

    *result = reinterpret_cast<uintptr_t>(point);
    return NOERROR;
}

ECode NativeCrypto::EC_POINT_clear_free(
    /* [in] */ Int64 pointRef)
{
    EC_POINT* point = reinterpret_cast<EC_POINT*>(pointRef);
    NATIVE_TRACE("EC_POINT_clear_free(%p)", point);

    if (point == NULL) {
        NATIVE_TRACE("EC_POINT_clear_free => point == NULL");
        return E_NULL_POINTER_EXCEPTION;
    }

    ::EC_POINT_clear_free(point);
    NATIVE_TRACE("EC_POINT_clear_free(%p) => success", point);
    return NOERROR;
}

ECode NativeCrypto::EC_POINT_cmp(
    /* [in] */ Int64 groupRef,
    /* [in] */ Int64 point1Ref,
    /* [in] */ Int64 point2Ref,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    const EC_GROUP* group = reinterpret_cast<const EC_GROUP*>(groupRef);
    const EC_POINT* point1 = reinterpret_cast<const EC_POINT*>(point1Ref);
    const EC_POINT* point2 = reinterpret_cast<const EC_POINT*>(point2Ref);
    NATIVE_TRACE("EC_POINT_cmp(%p, %p, %p)", group, point1, point2);

    if (group == NULL || point1 == NULL || point2 == NULL) {
        NATIVE_TRACE("EC_POINT_cmp(%p, %p, %p) => group == null || point1 == null || point2 == null",
                group, point1, point2);
        *result = FALSE;
        return E_NULL_POINTER_EXCEPTION;
    }

    int ret = ::EC_POINT_cmp(group, point1, point2, (BN_CTX*)NULL);

    NATIVE_TRACE("ECP_GROUP_cmp(%p, %p) => %d", point1, point2, ret);
    *result = ret == 0;
    return NOERROR;
}

ECode NativeCrypto::EC_POINT_get_affine_coordinates(
    /* [in] */ Int64 groupRef,
    /* [in] */ Int64 pointRef,
    /* [out, callee] */ ArrayOf<Byte>** xBytes,
    /* [out, callee] */ ArrayOf<Byte>** yBytes)
{
    VALIDATE_NOT_NULL(xBytes);
    VALIDATE_NOT_NULL(yBytes);

    const EC_GROUP* group = reinterpret_cast<const EC_GROUP*>(groupRef);
    const EC_POINT* point = reinterpret_cast<const EC_POINT*>(pointRef);
    NATIVE_TRACE("EC_POINT_get_affine_coordinates(%p, %p)", group, point);

    Unique_BIGNUM x(BN_new());
    Unique_BIGNUM y(BN_new());

    int ret;
    switch (get_EC_GROUP_type(group)) {
    case EC_CURVE_GFP:
        ret = EC_POINT_get_affine_coordinates_GFp(group, point, x.get(), y.get(), NULL);
        break;
    case EC_CURVE_GF2M:
        ret = EC_POINT_get_affine_coordinates_GF2m(group, point, x.get(), y.get(), NULL);
        break;
    default:
        *xBytes = *yBytes = NULL;
        return ThrowRuntimeException("invalid curve type");
    }
    if (ret != 1) {
        NATIVE_TRACE("EC_POINT_get_affine_coordinates(%p, %p)", group, point);
        *xBytes = *yBytes = NULL;
        return ThrowExceptionIfNecessary("EC_POINT_get_affine_coordinates");
    }

    ECode ec = BignumToArray(x.get(), "x", xBytes);
    if (FAILED(ec)) {
        *xBytes = *yBytes = NULL;
        return ec;
    }

    ec = BignumToArray(y.get(), "y", yBytes);
    if (FAILED(ec)) {
        *xBytes = *yBytes = NULL;
        return ec;
    }

    NATIVE_TRACE("EC_POINT_get_affine_coordinates(%p, %p) => %p, %p", group, point, *xBytes, *yBytes);
    return NOERROR;
}

ECode NativeCrypto::EC_POINT_set_affine_coordinates(
    /* [in] */ Int64 groupRef,
    /* [in] */ Int64 pointRef,
    /* [in] */ ArrayOf<Byte>* xBytes,
    /* [in] */ ArrayOf<Byte>* yBytes)
{
    const EC_GROUP* group = reinterpret_cast<const EC_GROUP*>(groupRef);
    EC_POINT* point = reinterpret_cast<EC_POINT*>(pointRef);
    NATIVE_TRACE("EC_POINT_set_affine_coordinates(%p, %p, %p, %p)", group, point, xBytes,
            yBytes);

    if (group == NULL || point == NULL) {
        NATIVE_TRACE("EC_POINT_set_affine_coordinates(%p, %p, %p, %p) => group == null || point == null",
                group, point, xBytes, yBytes);
        return E_NULL_POINTER_EXCEPTION;
    }

    ECode ec;
    BIGNUM* xRef = NULL;
    if (!ArrayToBignum(xBytes, &xRef, &ec)) {
        return ec;
    }
    Unique_BIGNUM x(xRef);

    BIGNUM* yRef = NULL;
    if (!ArrayToBignum(yBytes, &yRef, &ec)) {
        return ec;
    }
    Unique_BIGNUM y(yRef);

    int ret;
    switch (get_EC_GROUP_type(group)) {
    case EC_CURVE_GFP:
        ret = EC_POINT_set_affine_coordinates_GFp(group, point, x.get(), y.get(), NULL);
        break;
    case EC_CURVE_GF2M:
        ret = EC_POINT_set_affine_coordinates_GF2m(group, point, x.get(), y.get(), NULL);
        break;
    default:
        return ThrowRuntimeException("invalid curve type");
    }

    if (ret != 1) {
        return ThrowExceptionIfNecessary("EC_POINT_set_affine_coordinates");
    }

    NATIVE_TRACE("EC_POINT_set_affine_coordinates(%p, %p, %p, %p) => %d", group, point,
            xBytes, yBytes, ret);
    return NOERROR;
}

ECode NativeCrypto::EC_KEY_generate_key(
    /* [in] */ Int64 groupRef,
    /* [out] */ Int64* result)
{
    VALIDATE_NOT_NULL(result);

    const EC_GROUP* group = reinterpret_cast<const EC_GROUP*>(groupRef);
    NATIVE_TRACE("EC_KEY_generate_key(%p)", group);

    Unique_EC_KEY eckey(EC_KEY_new());
    if (eckey.get() == NULL) {
        NATIVE_TRACE("EC_KEY_generate_key(%p) => EC_KEY_new() oom", group);
        *result = 0;
        return E_OUT_OF_MEMORY_ERROR;
    }

    if (EC_KEY_set_group(eckey.get(), group) != 1) {
        NATIVE_TRACE("EC_KEY_generate_key(%p) => EC_KEY_set_group error", group);
        *result = 0;
        return ThrowExceptionIfNecessary("EC_KEY_set_group");
    }

    if (::EC_KEY_generate_key(eckey.get()) != 1) {
        NATIVE_TRACE("EC_KEY_generate_key(%p) => EC_KEY_generate_key error", group);
        *result = 0;
        return ThrowExceptionIfNecessary("EC_KEY_set_group");
    }

    Unique_EVP_PKEY pkey(EVP_PKEY_new());
    if (pkey.get() == NULL) {
        NATIVE_TRACE("EC_KEY_generate_key(%p) => threw error", group);
        *result = 0;
        return ThrowExceptionIfNecessary("EC_KEY_generate_key");
    }
    if (EVP_PKEY_assign_EC_KEY(pkey.get(), eckey.get()) != 1) {
        *result = 0;
        return ThrowRuntimeException("EVP_PKEY_assign_EC_KEY failed");
    }
    OWNERSHIP_TRANSFERRED(eckey);

    NATIVE_TRACE("EC_KEY_generate_key(%p) => %p", group, pkey.get());
    *result = reinterpret_cast<uintptr_t>(pkey.release());
    return NOERROR;
}

ECode NativeCrypto::EC_KEY_get0_group(
    /* [in] */ Int64 pkeyRef,
    /* [out] */ Int64* result)
{
    VALIDATE_NOT_NULL(result);

    EVP_PKEY* pkey = reinterpret_cast<EVP_PKEY*>(pkeyRef);
    NATIVE_TRACE("EC_KEY_get0_group(%p)", pkey);

    if (pkey == NULL) {
        NATIVE_TRACE("EC_KEY_get0_group(%p) => pkey == null", pkey);
        *result = 0;
        return E_NULL_POINTER_EXCEPTION;
    }

    if (::EVP_PKEY_type(pkey->type) != EVP_PKEY_EC) {
        NATIVE_TRACE("EC_KEY_get0_group(%p) => not EC key (type == %d)", pkey,
                ::EVP_PKEY_type(pkey->type));
        *result = 0;
        return ThrowRuntimeException("not EC key");
    }

    const EC_GROUP* group = ::EC_KEY_get0_group(pkey->pkey.ec);
    NATIVE_TRACE("EC_KEY_get0_group(%p) => %p", pkey, group);
    *result = reinterpret_cast<uintptr_t>(group);
    return NOERROR;
}

ECode NativeCrypto::EC_KEY_get_private_key(
    /* [in] */ Int64 pkeyRef,
    /* [out, callee] */ ArrayOf<Byte>** result)
{
    VALIDATE_NOT_NULL(result);

    EVP_PKEY* pkey = reinterpret_cast<EVP_PKEY*>(pkeyRef);
    NATIVE_TRACE("EC_KEY_get_private_key(%p)", pkey);

    if (pkey == NULL) {
        *result = NULL;
        return E_NULL_POINTER_EXCEPTION;
    }

    Unique_EC_KEY eckey(EVP_PKEY_get1_EC_KEY(pkey));
    if (eckey.get() == NULL) {
        *result = NULL;
        return ThrowExceptionIfNecessary("EVP_PKEY_get1_EC_KEY");
    }

    const BIGNUM *privkey = EC_KEY_get0_private_key(eckey.get());

    ECode ec = BignumToArray(privkey, "privkey", result);
    if (FAILED(ec)) {
        NATIVE_TRACE("EC_KEY_get_private_key(%p) => threw error", pkey);
        *result = NULL;
        return ec;
    }

    NATIVE_TRACE("EC_KEY_get_private_key(%p) => %p", pkey, *result);
    return NOERROR;
}

ECode NativeCrypto::EC_KEY_get_public_key(
    /* [in] */ Int64 pkeyRef,
    /* [out] */ Int64* result)
{
    VALIDATE_NOT_NULL(result);

    EVP_PKEY* pkey = reinterpret_cast<EVP_PKEY*>(pkeyRef);
    NATIVE_TRACE("EC_KEY_get_public_key(%p)", pkey);

    if (pkey == NULL) {
        *result = 0;
        return E_NULL_POINTER_EXCEPTION;
    }

    Unique_EC_KEY eckey(EVP_PKEY_get1_EC_KEY(pkey));
    if (eckey.get() == NULL) {
        *result = 0;
        return ThrowExceptionIfNecessary("EVP_PKEY_get1_EC_KEY");
    }

    Unique_EC_POINT dup(EC_POINT_dup(EC_KEY_get0_public_key(eckey.get()),
            ::EC_KEY_get0_group(eckey.get())));
    if (dup.get() == NULL) {
        NATIVE_TRACE("EC_KEY_get_public_key(%p) => can't dup public key", pkey);
        *result = 0;
        return ThrowRuntimeException("EC_POINT_dup");
    }

    NATIVE_TRACE("EC_KEY_get_public_key(%p) => %p", pkey, dup.get());
    *result = reinterpret_cast<uintptr_t>(dup.release());
    return NOERROR;
}

ECode NativeCrypto::EC_KEY_set_nonce_from_hash(
    /* [in] */ Int64 pkeyRef,
    /* [in] */ Boolean enabled)
{
    EVP_PKEY* pkey = reinterpret_cast<EVP_PKEY*>(pkeyRef);
    NATIVE_TRACE("EC_KEY_set_nonce_from_hash(%p, %d)", pkey, enabled ? 1 : 0);

    if (pkey == NULL) {
        return E_NULL_POINTER_EXCEPTION;
    }

    Unique_EC_KEY eckey(EVP_PKEY_get1_EC_KEY(pkey));
    if (eckey.get() == NULL) {
        return ThrowExceptionIfNecessary("EVP_PKEY_get1_EC_KEY");
    }

    ::EC_KEY_set_nonce_from_hash(eckey.get(), enabled ? 1 : 0);
    return NOERROR;
}

ECode NativeCrypto::ECDH_compute_key(
    /* [in] */ ArrayOf<Byte>* outArray,
    /* [in] */ Int32 outOffset,
    /* [in] */ Int64 publicKeyRef,
    /* [in] */ Int64 privateKeyRef,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    EVP_PKEY* pubPkey = reinterpret_cast<EVP_PKEY*>(publicKeyRef);
    EVP_PKEY* privPkey = reinterpret_cast<EVP_PKEY*>(privateKeyRef);
    NATIVE_TRACE("ECDH_compute_key(%p, %d, %p, %p)", outArray, outOffset, pubPkey, privPkey);

    if (outArray == NULL) {
        NATIVE_TRACE("ECDH_compute_key(%p, %d, %p, %p) can't get output buffer",
                outArray, outOffset, pubPkey, privPkey);
        *result = -1;
        return NOERROR;
    }

    if ((outOffset < 0) || (outOffset >= outArray->GetLength())) {
        *result = -1;
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    if (pubPkey == NULL) {
        *result = -1;
        return E_NULL_POINTER_EXCEPTION;
    }

    Unique_EC_KEY pubkey(EVP_PKEY_get1_EC_KEY(pubPkey));
    if (pubkey.get() == NULL) {
        NATIVE_TRACE("ECDH_compute_key(%p) => can't get public key", pubPkey);
        *result = -1;
        return ThrowExceptionIfNecessary("EVP_PKEY_get1_EC_KEY public");
    }

    const EC_POINT* pubkeyPoint = EC_KEY_get0_public_key(pubkey.get());
    if (pubkeyPoint == NULL) {
        NATIVE_TRACE("ECDH_compute_key(%p) => can't get public key point", pubPkey);
        *result = -1;
        return ThrowExceptionIfNecessary("EVP_PKEY_get1_EC_KEY public");
    }

    if (privPkey == NULL) {
        *result = -1;
        return E_NULL_POINTER_EXCEPTION;
    }

    Unique_EC_KEY privkey(EVP_PKEY_get1_EC_KEY(privPkey));
    if (privkey.get() == NULL) {
        *result = -1;
        return ThrowExceptionIfNecessary("EVP_PKEY_get1_EC_KEY private");
    }

    int outputLength = ::ECDH_compute_key(
            &(*outArray)[outOffset],
            outArray->GetLength() - outOffset,
            pubkeyPoint,
            privkey.get(),
            NULL // No KDF
            );
    if (outputLength == -1) {
        *result = -1;
        return ThrowExceptionIfNecessary("ECDH_compute_key");
    }

    *result = outputLength;
    return NOERROR;
}

ECode NativeCrypto::EVP_get_digestbyname(
    /* [in] */ const String& algorithm,
    /* [out] */ Int64* result)
{
    VALIDATE_NOT_NULL(result);

    if (algorithm.IsNull()) {
        *result = -1;
        return E_NULL_POINTER_EXCEPTION;
    }

    NATIVE_TRACE("NativeCrypto_EVP_get_digestbyname(%s)", algorithm.string());

    const EVP_MD* evp_md = ::EVP_get_digestbyname(algorithm.string());
    if (evp_md == NULL) {
        *result = 0;
        return ThrowRuntimeException("Hash algorithm not found");
    }

    NATIVE_TRACE("NativeCrypto_EVP_get_digestbyname(%s) => %p", algorithm.string(), evp_md);
    *result = reinterpret_cast<uintptr_t>(evp_md);
    return NOERROR;
}

ECode NativeCrypto::EVP_MD_size(
    /* [in] */ Int64 evpMdRef,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    EVP_MD* evp_md = reinterpret_cast<EVP_MD*>(evpMdRef);
    NATIVE_TRACE("NativeCrypto_EVP_MD_size(%p)", evp_md);

    if (evp_md == NULL) {
        *result = -1;
        return E_NULL_POINTER_EXCEPTION;
    }

    int ret = ::EVP_MD_size(evp_md);
    NATIVE_TRACE("NativeCrypto_EVP_MD_size(%p) => %d", evp_md, ret);
    *result = ret;
    return NOERROR;
}

ECode NativeCrypto::EVP_MD_block_size(
    /* [in] */ Int64 evpMdRef,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    EVP_MD* evp_md = reinterpret_cast<EVP_MD*>(evpMdRef);
    NATIVE_TRACE("NativeCrypto_EVP_MD_block_size(%p)", evp_md);

    if (evp_md == NULL) {
        *result = -1;
        return E_NULL_POINTER_EXCEPTION;
    }

    int ret = ::EVP_MD_block_size(evp_md);
    NATIVE_TRACE("NativeCrypto_EVP_MD_block_size(%p) => %d", evp_md, ret);
    *result = ret;
    return NOERROR;
}

ECode NativeCrypto::EVP_MD_CTX_create(
    /* [out] */ Int64* result)
{
    VALIDATE_NOT_NULL(result);

    NATIVE_TRACE_MD("EVP_MD_CTX_create()");

    Unique_EVP_MD_CTX ctx(::EVP_MD_CTX_create());
    if (ctx.get() == NULL) {
        *result = 0;
        return E_OUT_OF_MEMORY_ERROR;
    }

    NATIVE_TRACE_MD("EVP_MD_CTX_create() => %p", ctx.get());
    *result = reinterpret_cast<uintptr_t>(ctx.release());
    return NOERROR;
}

ECode NativeCrypto::EVP_MD_CTX_init(
    /* [in] */ IOpenSSLDigestContext* ctxRef)
{
    ECode ec;
    EVP_MD_CTX* ctx = FromContextObject<EVP_MD_CTX>(ctxRef, &ec);
    NATIVE_TRACE_MD("EVP_MD_CTX_init(%p)", ctx);

    if (ctx != NULL) {
        ::EVP_MD_CTX_init(ctx);
    }
    return ec;
}

ECode NativeCrypto::EVP_MD_CTX_destroy(
    /* [in] */ Int64 ctxRef)
{
    EVP_MD_CTX* ctx = reinterpret_cast<EVP_MD_CTX*>(ctxRef);
    NATIVE_TRACE_MD("EVP_MD_CTX_destroy(%p)", ctx);

    if (ctx != NULL) {
        ::EVP_MD_CTX_destroy(ctx);
    }
    return NOERROR;
}

ECode NativeCrypto::EVP_MD_CTX_copy(
    /* [in] */ IOpenSSLDigestContext* dstCtxRef,
    /* [in] */ IOpenSSLDigestContext* srcCtxRef,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    ECode ec = NOERROR;
    EVP_MD_CTX* dst_ctx = FromContextObject<EVP_MD_CTX>(dstCtxRef, &ec);
    if (dst_ctx == NULL) {
        *result = 0;
        return ec;
    }
    const EVP_MD_CTX* src_ctx = FromContextObject<EVP_MD_CTX>(srcCtxRef, &ec);
    if (src_ctx == NULL) {
        *result = 0;
        return ec;
    }
    NATIVE_TRACE_MD("EVP_MD_CTX_copy(%p. %p)", dst_ctx, src_ctx);

    int ret = EVP_MD_CTX_copy_ex(dst_ctx, src_ctx);
    if (ret == 0) {
        FreeOpenSslErrorState();
        ec = ThrowRuntimeException("Unable to copy EVP_MD_CTX");
    }

    NATIVE_TRACE_MD("EVP_MD_CTX_copy(%p, %p) => %d", dst_ctx, src_ctx, ret);
    *result = ret;
    return ec;
}

static ECode EvpInit(IOpenSSLDigestContext* evpMdCtxRef, Int64 evpMdRef, const char* name,
        int (*init_func)(EVP_MD_CTX*, const EVP_MD*, ENGINE*), Int32* result)
{
    ECode ec = NOERROR;
    EVP_MD_CTX* ctx = FromContextObject<EVP_MD_CTX>(evpMdCtxRef, &ec);
    if (ctx == NULL) {
        *result = 0;
        return ec;
    }

    const EVP_MD* evp_md = reinterpret_cast<const EVP_MD*>(evpMdRef);
    if (evp_md == NULL) {
        *result = 0;
        return E_NULL_POINTER_EXCEPTION;
    }
    NATIVE_TRACE_MD("%s(%p, %p)", name, ctx, evp_md);

    int ok = init_func(ctx, evp_md, NULL);
    if (ok == 0) {
        ec = ThrowExceptionIfNecessary(name);
        if (FAILED(ec)) {
            NATIVE_TRACE("%s(%p) => threw exception", name, evp_md);
            *result = 0;
            return ec;
        }
    }
    NATIVE_TRACE_MD("%s(%p, %p) => %d", name, ctx, evp_md, ok);
    *result = ok;
    return NOERROR;
}

ECode NativeCrypto::EVP_DigestInit(
    /* [in] */ IOpenSSLDigestContext* evpMdCtxRef,
    /* [in] */ Int64 evpMdRef,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    return EvpInit(evpMdCtxRef, evpMdRef, "EVP_DigestInit", EVP_DigestInit_ex, result);
}

static ECode EvpUpdate(IOpenSSLDigestContext* evpMdCtxRef, ArrayOf<Byte>* inBytes, Int32 inOffset,
        Int32 inLength, const char *name, int (*update_func)(EVP_MD_CTX*, const void *,
        size_t))
{
    ECode ec = NOERROR;
    EVP_MD_CTX* mdCtx = FromContextObject<EVP_MD_CTX>(evpMdCtxRef, &ec);
    NATIVE_TRACE_MD("%s(%p, %p, %d, %d)", name, mdCtx, inBytes, inOffset, inLength);

    if (mdCtx == NULL) {
        return ec;
    }

    if (inOffset < 0 || inOffset > inBytes->GetLength()) {
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    const Int32 inEnd = inOffset + inLength;
    if (inLength < 0 || inEnd < 0 || inEnd > inBytes->GetLength()) {
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    const unsigned char *tmp = reinterpret_cast<const unsigned char *>(inBytes->GetPayload());
    if (!update_func(mdCtx, tmp + inOffset, inLength)) {
        NATIVE_TRACE("ctx=%p %s => threw exception", mdCtx, name);
        ec = ThrowExceptionIfNecessary(name);
    }

    NATIVE_TRACE_MD("%s(%p, %p, %d, %d) => success", name, mdCtx, inBytes, inOffset, inLength);
    return ec;
}

ECode NativeCrypto::EVP_DigestUpdate(
    /* [in] */ IOpenSSLDigestContext* evpMdCtxRef,
    /* [in] */ ArrayOf<Byte>* buffer,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length)
{
    return EvpUpdate(evpMdCtxRef, buffer, offset, length, "EVP_DigestUpdate",
            ::EVP_DigestUpdate);
}

ECode NativeCrypto::EVP_DigestFinal(
    /* [in] */ IOpenSSLDigestContext* ctxRef,
    /* [in] */ ArrayOf<Byte>* hash,
    /* [in] */ Int32 offset,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    ECode ec = NOERROR;
    EVP_MD_CTX* ctx = FromContextObject<EVP_MD_CTX>(ctxRef, &ec);
    NATIVE_TRACE_MD("EVP_DigestFinal(%p, %p, %d)", ctx, hash, offset);

    if (ctx == NULL) {
        *result = -1;
        return ec;
    }
    else if (hash == NULL) {
        *result = -1;
        return E_NULL_POINTER_EXCEPTION;
    }

    unsigned int bytesWritten = -1;
    int ok = EVP_DigestFinal_ex(ctx,
                             reinterpret_cast<unsigned char*>(hash->GetPayload() + offset),
                             &bytesWritten);
    if (ok == 0) {
        ec = ThrowExceptionIfNecessary("EVP_DigestFinal");
    }

    NATIVE_TRACE_MD("EVP_DigestFinal(%p, %p, %d) => %d", ctx, hash, offset, bytesWritten);
    *result = bytesWritten;
    return ec;
}

ECode NativeCrypto::EVP_DigestSignInit(
    /* [in] */ IOpenSSLDigestContext* evpMdCtxRef,
    /* [in] */ Int64 evpMdRef,
    /* [in] */ Int64 pkeyRef)
{
    ECode ec = NOERROR;
    EVP_MD_CTX* mdCtx = FromContextObject<EVP_MD_CTX>(evpMdCtxRef, &ec);
    if (mdCtx == NULL) {
         return ec;
    }

    const EVP_MD* md = reinterpret_cast<const EVP_MD*>(evpMdRef);
    EVP_PKEY* pkey = reinterpret_cast<EVP_PKEY*>(pkeyRef);
    NATIVE_TRACE("EVP_DigestSignInit(%p, %p, %p)", mdCtx, md, pkey);

    if (md == NULL || pkey == NULL) {
        return E_NULL_POINTER_EXCEPTION;
    }

    if (::EVP_DigestSignInit(mdCtx, (EVP_PKEY_CTX **) NULL, md, (ENGINE *) NULL, pkey) <= 0) {
        NATIVE_TRACE("ctx=%p EVP_DigestSignInit => threw exception", mdCtx);
        return ThrowExceptionIfNecessary("EVP_DigestSignInit");
    }

    NATIVE_TRACE("EVP_DigestSignInit(%p, %p, %p) => success", mdCtx, md, pkey);
    return NOERROR;
}

ECode NativeCrypto::EVP_DigestSignUpdate(
    /* [in] */ IOpenSSLDigestContext* evpMdCtxRef,
    /* [in] */ ArrayOf<Byte>* buffer,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length)
{
    return EvpUpdate(evpMdCtxRef, buffer, offset, length, "EVP_DigestSignUpdate",
            ::EVP_DigestUpdate);
}

ECode NativeCrypto::EVP_DigestSignFinal(
    /* [in] */ IOpenSSLDigestContext* evpMdCtxRef,
    /* [out, callee] */ ArrayOf<Byte>** result)
{
    VALIDATE_NOT_NULL(result);

    ECode ec = NOERROR;
    EVP_MD_CTX* mdCtx = FromContextObject<EVP_MD_CTX>(evpMdCtxRef, &ec);
    NATIVE_TRACE("EVP_DigestSignFinal(%p)", mdCtx);

    if (mdCtx == NULL) {
        *result = NULL;
        return ec;
    }

    const size_t expectedSize = EVP_MD_CTX_size(mdCtx);
    AutoPtr< ArrayOf<Byte> > outBytes = ArrayOf<Byte>::Alloc(expectedSize);
    unsigned char *tmp = reinterpret_cast<unsigned char*>(outBytes->GetPayload());
    size_t len;
    if (!::EVP_DigestSignFinal(mdCtx, tmp, &len)) {
        NATIVE_TRACE("ctx=%p EVP_DigestSignFinal => threw exception", mdCtx);
        *result = NULL;
        return ThrowExceptionIfNecessary("EVP_DigestSignFinal");
    }

    if (len != expectedSize) {
        *result = NULL;
        return ThrowRuntimeException("hash size unexpected");
    }

    NATIVE_TRACE("EVP_DigestSignFinal(%p) => %p", mdCtx, outBytes.Get());
    *result = outBytes;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

ECode NativeCrypto::EVP_SignInit(
    /* [in] */ IOpenSSLDigestContext* evpMdCtxRef,
    /* [in] */ Int64 evpMdRef,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    return EvpInit(evpMdCtxRef, evpMdRef, "EVP_SignInit", EVP_DigestInit_ex, result);
}

ECode NativeCrypto::EVP_SignUpdate(
    /* [in] */ IOpenSSLDigestContext* evpMdCtxRef,
    /* [in] */ ArrayOf<Byte>* buffer,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length)
{
    return EvpUpdate(evpMdCtxRef, buffer, offset, length, "EVP_SignUpdate",
            ::EVP_DigestUpdate);
}

ECode NativeCrypto::EVP_SignFinal(
    /* [in] */ IOpenSSLDigestContext* ctxRef,
    /* [in] */ ArrayOf<Byte>* signature,
    /* [in] */ Int32 offset,
    /* [in] */ Int64 pkeyRef,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    ECode ec = NOERROR;
    EVP_MD_CTX* ctx = FromContextObject<EVP_MD_CTX>(ctxRef, &ec);
    EVP_PKEY* pkey = reinterpret_cast<EVP_PKEY*>(pkeyRef);
    NATIVE_TRACE("NativeCrypto_EVP_SignFinal(%p, %p, %d, %p)", ctx, signature, offset, pkey);

    if (ctx == NULL) {
        *result = -1;
        return ec;
    }
    else if (pkey == NULL) {
        *result = -1;
        return E_NULL_POINTER_EXCEPTION;
    }

    if (signature == NULL) {
        *result = -1;
        return NOERROR;
    }
    unsigned int bytesWritten = -1;
    int ok = ::EVP_SignFinal(ctx,
                           reinterpret_cast<unsigned char*>(signature->GetPayload() + offset),
                           &bytesWritten,
                           pkey);
    if (ok == 0) {
        ec = ThrowExceptionIfNecessary("NativeCrypto_EVP_SignFinal");
    }
    NATIVE_TRACE("NativeCrypto_EVP_SignFinal(%p, %p, %d, %p) => %u",
              ctx, signature, offset, pkey, bytesWritten);

    *result = bytesWritten;
    return NOERROR;
}

ECode NativeCrypto::EVP_VerifyInit(
    /* [in] */ IOpenSSLDigestContext* evpMdCtxRef,
    /* [in] */ Int64 evpMdRef,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    return EvpInit(evpMdCtxRef, evpMdRef, "EVP_VerifyInit", EVP_DigestInit_ex, result);
}

ECode NativeCrypto::EVP_VerifyUpdate(
    /* [in] */ IOpenSSLDigestContext* ctxRef,
    /* [in] */ ArrayOf<Byte>* buffer,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length)
{
    ECode ec = NOERROR;
    EVP_MD_CTX* ctx = FromContextObject<EVP_MD_CTX>(ctxRef, &ec);
    NATIVE_TRACE("NativeCrypto_EVP_VerifyUpdate(%p, %p, %d, %d)", ctx, buffer, offset, length);

    if (ctx == NULL) {
        return ec;
    }
    else if (buffer == NULL) {
        return E_NULL_POINTER_EXCEPTION;
    }

    if (offset < 0 || length < 0) {
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    if (buffer == NULL) {
        return NOERROR;
    }
    if (buffer->GetLength() < offset + length) {
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    int ok = ::EVP_DigestUpdate(ctx,
                            reinterpret_cast<const unsigned char*>(buffer->GetPayload() + offset),
                            length);
    if (ok == 0) {
        ec = ThrowExceptionIfNecessary("NativeCrypto_EVP_VerifyUpdate");
    }
    return ec;
}

ECode NativeCrypto::EVP_VerifyFinal(
    /* [in] */ IOpenSSLDigestContext* ctxRef,
    /* [in] */ ArrayOf<Byte>* buffer,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [in] */ Int64 pkeyRef,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    ECode ec = NOERROR;
    EVP_MD_CTX* ctx = FromContextObject<EVP_MD_CTX>(ctxRef, &ec);
    EVP_PKEY* pkey = reinterpret_cast<EVP_PKEY*>(pkeyRef);
    NATIVE_TRACE("NativeCrypto_EVP_VerifyFinal(%p, %p, %d, %d, %p)",
            ctx, buffer, offset, length, pkey);

    if (ctx == NULL) {
        *result = -1;
        return ec;
    }
    else if (buffer == NULL || pkey == NULL) {
        *result = -1;
        return E_NULL_POINTER_EXCEPTION;
    }

    int ok = ::EVP_VerifyFinal(ctx,
                            reinterpret_cast<const unsigned char*>(buffer->GetPayload() + offset),
                            length,
                            pkey);
    if (ok < 0) {
        ec = ThrowExceptionIfNecessary("NativeCrypto_EVP_VerifyFinal");
    }

    /*
     * For DSA keys, OpenSSL appears to have a bug where it returns
     * errors for any result != 1. See dsa_ossl.c in dsa_do_verify
     */
    FreeOpenSslErrorState();

    NATIVE_TRACE("NativeCrypto_EVP_VerifyFinal(%p, %p, %d, %d, %p) => %d",
              ctx, buffer, offset, length, pkey, ok);

    *result = ok;
    return ec;
}

ECode NativeCrypto::EVP_get_cipherbyname(
    /* [in] */ const String& algorithm,
    /* [out] */ Int64* result)
{
    VALIDATE_NOT_NULL(result);

    NATIVE_TRACE("EVP_get_cipherbyname(%p)", algorithm);
    if (algorithm.IsNull()) {
        NATIVE_TRACE("EVP_get_cipherbyname => threw exception algorithm == null");
        *result = -1;
        return E_NULL_POINTER_EXCEPTION;
    }

    const EVP_CIPHER* evp_cipher = ::EVP_get_cipherbyname(algorithm.string());
    if (evp_cipher == NULL) {
        FreeOpenSslErrorState();
    }

    NATIVE_TRACE("EVP_get_cipherbyname(%s) => %p", algorithm.string(), evp_cipher);
    *result = reinterpret_cast<uintptr_t>(evp_cipher);
    return NOERROR;
}

ECode NativeCrypto::EVP_CipherInit_ex(
    /* [in] */ Int64 ctxRef,
    /* [in] */ Int64 evpCipherRef,
    /* [in] */ ArrayOf<Byte>* keyArray,
    /* [in] */ ArrayOf<Byte>* ivArray,
    /* [in] */ Boolean encrypting)
{
    EVP_CIPHER_CTX* ctx = reinterpret_cast<EVP_CIPHER_CTX*>(ctxRef);
    const EVP_CIPHER* evpCipher = reinterpret_cast<const EVP_CIPHER*>(evpCipherRef);
    NATIVE_TRACE("EVP_CipherInit_ex(%p, %p, %p, %p, %d)", ctx, evpCipher, keyArray, ivArray,
            encrypting ? 1 : 0);

    if (ctx == NULL) {
        NATIVE_TRACE("EVP_CipherUpdate => ctx == null");
        return E_NULL_POINTER_EXCEPTION;
    }

    // The key can be null if we need to set extra parameters.
    UniquePtr<unsigned char[]> keyPtr;
    if (keyArray != NULL) {
        keyPtr.reset(new unsigned char[keyArray->GetLength()]);
        memcpy(keyPtr.get(), keyArray->GetPayload(), keyArray->GetLength());
    }

    // The IV can be null if we're using ECB.
    UniquePtr<unsigned char[]> ivPtr;
    if (ivArray != NULL) {
        ivPtr.reset(new unsigned char[ivArray->GetLength()]);
        memcpy(ivPtr.get(), ivArray->GetPayload(), ivArray->GetLength());
    }

    if (!::EVP_CipherInit_ex(ctx, evpCipher, NULL, keyPtr.get(), ivPtr.get(), encrypting ? 1 : 0)) {
        NATIVE_TRACE("EVP_CipherInit_ex => error initializing cipher");
        return ThrowExceptionIfNecessary("EVP_CipherInit_ex");
    }

    NATIVE_TRACE("EVP_CipherInit_ex(%p, %p, %p, %p, %d) => success", ctx, evpCipher, keyArray, ivArray,
            encrypting ? 1 : 0);
    return NOERROR;
}

ECode NativeCrypto::EVP_CipherUpdate(
    /* [in] */ Int64 ctxRef,
    /* [in] */ ArrayOf<Byte>* outArray,
    /* [in] */ Int32 outOffset,
    /* [in] */ ArrayOf<Byte>* inArray,
    /* [in] */ Int32 inOffset,
    /* [in] */ Int32 inLength,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    EVP_CIPHER_CTX* ctx = reinterpret_cast<EVP_CIPHER_CTX*>(ctxRef);
    NATIVE_TRACE("EVP_CipherUpdate(%p, %p, %d, %p, %d)", ctx, outArray, outOffset, inArray, inOffset);

    if (ctx == NULL) {
        NATIVE_TRACE("ctx=%p EVP_CipherUpdate => ctx == null", ctx);
        return E_NULL_POINTER_EXCEPTION;
    }

    if (inArray == NULL) {
        *result = 0;
        return NOERROR;
    }
    if (inOffset + inLength > inArray->GetLength()) {
        *result = 0;
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    if (outArray == NULL) {
        *result = 0;
        return NOERROR;
    }
    if (outOffset + inLength > outArray->GetLength()) {
        *result = 0;
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    NATIVE_TRACE("ctx=%p EVP_CipherUpdate in=%p in.length=%d inOffset=%d inLength=%d out=%p out.length=%d outOffset=%d",
            ctx, inArray, inArray->GetLength(), inOffset, inLength, outArray, outArray->GetLength(), outOffset);

    unsigned char* out = reinterpret_cast<unsigned char*>(outArray->GetPayload());
    const unsigned char* in = reinterpret_cast<const unsigned char*>(inArray->GetPayload());

    int outl;
    if (!::EVP_CipherUpdate(ctx, out + outOffset, &outl, in + inOffset, inLength)) {
        NATIVE_TRACE("ctx=%p EVP_CipherUpdate => threw error", ctx);
        *result = 0;
        return ThrowExceptionIfNecessary("EVP_CipherUpdate");
    }

    NATIVE_TRACE("EVP_CipherUpdate(%p, %p, %d, %p, %d) => %d", ctx, outArray, outOffset, inArray,
            inOffset, outl);
    *result = outl;
    return NOERROR;
}

ECode NativeCrypto::EVP_CipherFinal_ex(
    /* [in] */ Int64 ctxRef,
    /* [in] */ ArrayOf<Byte>* outArray,
    /* [in] */ Int32 outOffset,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    EVP_CIPHER_CTX* ctx = reinterpret_cast<EVP_CIPHER_CTX*>(ctxRef);
    NATIVE_TRACE("EVP_CipherFinal_ex(%p, %p, %d)", ctx, outArray, outOffset);

    if (ctx == NULL) {
        NATIVE_TRACE("ctx=%p EVP_CipherFinal_ex => ctx == null", ctx);
        *result = 0;
        return E_NULL_POINTER_EXCEPTION;
    }

    if (outArray == NULL) {
        *result = 0;
        return NOERROR;
    }

    unsigned char* out = reinterpret_cast<unsigned char*>(outArray->GetPayload());

    int outl;
    if (!::EVP_CipherFinal_ex(ctx, out + outOffset, &outl)) {
        NATIVE_TRACE("ctx=%p EVP_CipherFinal_ex => threw error", ctx);
        *result = 0;
        return ThrowExceptionIfNecessary("EVP_CipherFinal_ex");
    }

    NATIVE_TRACE("EVP_CipherFinal(%p, %p, %d) => %d", ctx, outArray, outOffset, outl);
    *result = outl;
    return NOERROR;
}

ECode NativeCrypto::EVP_CIPHER_iv_length(
    /* [in] */ Int64 evpCipherRef,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    const EVP_CIPHER* evpCipher = reinterpret_cast<const EVP_CIPHER*>(evpCipherRef);
    NATIVE_TRACE("EVP_CIPHER_iv_length(%p)", evpCipher);

    if (evpCipher == NULL) {
        NATIVE_TRACE("EVP_CIPHER_iv_length => evpCipher == null");
        *result = 0;
        return E_NULL_POINTER_EXCEPTION;
    }

    const int ivLength = ::EVP_CIPHER_iv_length(evpCipher);
    NATIVE_TRACE("EVP_CIPHER_iv_length(%p) => %d", evpCipher, ivLength);
    *result = ivLength;
    return NOERROR;
}

ECode NativeCrypto::EVP_CIPHER_CTX_new(
    /* [out] */ Int64* result)
{
    VALIDATE_NOT_NULL(result);

    NATIVE_TRACE("EVP_CIPHER_CTX_new()");

    Unique_EVP_CIPHER_CTX ctx(::EVP_CIPHER_CTX_new());
    if (ctx.get() == NULL) {
        NATIVE_TRACE("EVP_CipherInit_ex => context allocation error");
        *result = 0;
        return E_OUT_OF_MEMORY_ERROR;
    }

    NATIVE_TRACE("EVP_CIPHER_CTX_new() => %p", ctx.get());
    *result = reinterpret_cast<uintptr_t>(ctx.release());
    return NOERROR;
}

ECode NativeCrypto::EVP_CIPHER_CTX_block_size(
    /* [in] */ Int64 ctxRef,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    EVP_CIPHER_CTX* ctx = reinterpret_cast<EVP_CIPHER_CTX*>(ctxRef);
    NATIVE_TRACE("EVP_CIPHER_CTX_block_size(%p)", ctx);

    if (ctx == NULL) {
        NATIVE_TRACE("ctx=%p EVP_CIPHER_CTX_block_size => ctx == null", ctx);
        *result = 0;
        return E_NULL_POINTER_EXCEPTION;
    }

    int blockSize = ::EVP_CIPHER_CTX_block_size(ctx);
    NATIVE_TRACE("EVP_CIPHER_CTX_block_size(%p) => %d", ctx, blockSize);
    *result = blockSize;
    return NOERROR;
}

ECode NativeCrypto::Get_EVP_CIPHER_CTX_buf_len(
    /* [in] */ Int64 ctxRef,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    EVP_CIPHER_CTX* ctx = reinterpret_cast<EVP_CIPHER_CTX*>(ctxRef);
    NATIVE_TRACE("get_EVP_CIPHER_CTX_buf_len(%p)", ctx);

    if (ctx == NULL) {
        NATIVE_TRACE("ctx=%p get_EVP_CIPHER_CTX_buf_len => ctx == null", ctx);
        *result = 0;
        return E_NULL_POINTER_EXCEPTION;
    }

    int buf_len = ctx->buf_len;
    NATIVE_TRACE("get_EVP_CIPHER_CTX_buf_len(%p) => %d", ctx, buf_len);
    *result = buf_len;
    return NOERROR;
}

ECode NativeCrypto::EVP_CIPHER_CTX_set_padding(
    /* [in] */ Int64 ctxRef,
    /* [in] */ Boolean enablePaddingBool)
{
    EVP_CIPHER_CTX* ctx = reinterpret_cast<EVP_CIPHER_CTX*>(ctxRef);
    int enablePadding = enablePaddingBool ? 1 : 0;
    NATIVE_TRACE("EVP_CIPHER_CTX_set_padding(%p, %d)", ctx, enablePadding);

    if (ctx == NULL) {
        NATIVE_TRACE("ctx=%p EVP_CIPHER_CTX_set_padding => ctx == null", ctx);
        return E_NULL_POINTER_EXCEPTION;
    }

    ::EVP_CIPHER_CTX_set_padding(ctx, enablePadding); // Not void, but always returns 1.
    NATIVE_TRACE("EVP_CIPHER_CTX_set_padding(%p, %d) => success", ctx, enablePadding);
    return NOERROR;
}

ECode NativeCrypto::EVP_CIPHER_CTX_set_key_length(
    /* [in] */ Int64 ctxRef,
    /* [in] */ Int32 keySizeBits)
{
    EVP_CIPHER_CTX* ctx = reinterpret_cast<EVP_CIPHER_CTX*>(ctxRef);
    NATIVE_TRACE("EVP_CIPHER_CTX_set_key_length(%p, %d)", ctx, keySizeBits);

    if (ctx == NULL) {
        NATIVE_TRACE("ctx=%p EVP_CIPHER_CTX_set_key_length => ctx == null", ctx);
        return E_NULL_POINTER_EXCEPTION;
    }

    if (!::EVP_CIPHER_CTX_set_key_length(ctx, keySizeBits)) {
        NATIVE_TRACE("NativeCrypto_EVP_CIPHER_CTX_set_key_length => threw error");
        return ThrowExceptionIfNecessary("NativeCrypto_EVP_CIPHER_CTX_set_key_length");
    }
    NATIVE_TRACE("EVP_CIPHER_CTX_set_key_length(%p, %d) => success", ctx, keySizeBits);
    return NOERROR;
}

ECode NativeCrypto::EVP_CIPHER_CTX_cleanup(
    /* [in] */ Int64 ctxRef)
{
    EVP_CIPHER_CTX* ctx = reinterpret_cast<EVP_CIPHER_CTX*>(ctxRef);
    NATIVE_TRACE("EVP_CIPHER_CTX_cleanup(%p)", ctx);

    if (ctx != NULL) {
        if (!::EVP_CIPHER_CTX_cleanup(ctx)) {
            NATIVE_TRACE("EVP_CIPHER_CTX_cleanup => threw error");
            return ThrowExceptionIfNecessary("EVP_CIPHER_CTX_cleanup");
        }
    }
    NATIVE_TRACE("EVP_CIPHER_CTX_cleanup(%p) => success", ctx);
    return NOERROR;
}

ECode NativeCrypto::RAND_seed(
    /* [in] */ ArrayOf<Byte>* seed)
{
    NATIVE_TRACE("NativeCrypto_RAND_seed seed=%p", seed);
    if (seed == NULL) {
        return NOERROR;
    }
    ::RAND_seed(seed->GetPayload(), seed->GetLength());
    return NOERROR;
}

ECode NativeCrypto::RAND_load_file(
    /* [in] */ const String& filename,
    /* [in] */ Int64 max_bytes,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    NATIVE_TRACE("NativeCrypto_RAND_load_file filename=%s max_bytes=%lld", filename.string(), max_bytes);
    if (filename.IsNull()) {
        *result = -1;
        return NOERROR;
    }
    int ret = ::RAND_load_file(filename.string(), max_bytes);
    NATIVE_TRACE("NativeCrypto_RAND_load_file file=%s => %d", filename.string(), ret);
    *result = ret;
    return NOERROR;
}

ECode NativeCrypto::RAND_bytes(
    /* [in] */ ArrayOf<Byte>* output)
{
    NATIVE_TRACE("NativeCrypto_RAND_bytes(%p)", output);

    if (output == NULL) {
        return NOERROR;
    }

    unsigned char* tmp = reinterpret_cast<unsigned char*>(output->GetPayload());
    if (::RAND_bytes(tmp, output->GetLength()) <= 0) {
        NATIVE_TRACE("tmp=%p NativeCrypto_RAND_bytes => threw error", tmp);
        return ThrowExceptionIfNecessary("NativeCrypto_RAND_bytes");
    }

    NATIVE_TRACE("NativeCrypto_RAND_bytes(%p) => success", output);
    return NOERROR;
}

ECode NativeCrypto::OBJ_txt2nid(
    /* [in] */ const String& oid,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    NATIVE_TRACE("OBJ_txt2nid");

    if (oid.IsNull()) {
        *result = 0;
        return NOERROR;
    }

    int nid = ::OBJ_txt2nid(oid.string());
    NATIVE_TRACE("OBJ_txt2nid(%s) => %d", oid.string(), nid);
    *result = nid;
    return NOERROR;
}

ECode NativeCrypto::OBJ_txt2nid_longName(
    /* [in] */ const String& oid,
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);

    NATIVE_TRACE("OBJ_txt2nid_longName");

    if (oid.IsNull()) {
        *result = NULL;
        return NOERROR;
    }

    NATIVE_TRACE("OBJ_txt2nid_longName(%s)", oid.string());

    int nid = ::OBJ_txt2nid(oid.string());
    if (nid == NID_undef) {
        NATIVE_TRACE("OBJ_txt2nid_longName(%s) => NID_undef", oid.string());
        FreeOpenSslErrorState();
        *result = NULL;
        return NOERROR;
    }

    const char* longName = ::OBJ_nid2ln(nid);
    NATIVE_TRACE("OBJ_txt2nid_longName(%s) => %s", oid.string(), longName);
    *result = longName;
    return NOERROR;
}

static ECode ASN1_OBJECT_to_OID_string(ASN1_OBJECT* obj, String* result)
{
    /*
     * The OBJ_obj2txt API doesn't "measure" if you pass in NULL as the buffer.
     * Just make a buffer that's large enough here. The documentation recommends
     * 80 characters.
     */
    char output[128];
    int ret = OBJ_obj2txt(output, sizeof(output), obj, 1);
    if (ret < 0) {
        *result = NULL;
        return ThrowExceptionIfNecessary("ASN1_OBJECT_to_OID_string");
    }
    else if (size_t(ret) >= sizeof(output)) {
        *result = NULL;
        return ThrowRuntimeException("ASN1_OBJECT_to_OID_string buffer too small");
    }

    NATIVE_TRACE("ASN1_OBJECT_to_OID_string(%p) => %s", obj, output);
    *result = output;
    return NOERROR;
}

ECode NativeCrypto::OBJ_txt2nid_oid(
    /* [in] */ const String& oid,
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);

    NATIVE_TRACE("OBJ_txt2nid_oid");

    if (oid.IsNull()) {
        *result = NULL;
        return NOERROR;
    }

    NATIVE_TRACE("OBJ_txt2nid_oid(%s)", oid.string());

    int nid = ::OBJ_txt2nid(oid.string());
    if (nid == NID_undef) {
        NATIVE_TRACE("OBJ_txt2nid_oid(%s) => NID_undef", oid.string());
        FreeOpenSslErrorState();
        *result = NULL;
        return NOERROR;
    }

    Unique_ASN1_OBJECT obj(::OBJ_nid2obj(nid));
    if (obj.get() == NULL) {
        *result = NULL;
        return ThrowExceptionIfNecessary("OBJ_nid2obj");
    }

    return ASN1_OBJECT_to_OID_string(obj.get(), result);
}

ECode NativeCrypto::X509_NAME_hash(
    /* [in] */ IX500Principal* principal,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    return X509_NAME_hash(principal, String("SHA1"), result);
}

ECode NativeCrypto::X509_NAME_hash_old(
    /* [in] */ IX500Principal* principal,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    return X509_NAME_hash(principal, String("MD5"), result);
}

ECode NativeCrypto::X509_NAME_hash(
    /* [in] */ IX500Principal* principal,
    /* [in] */ const String& algorithm,
    /* [out] */ Int32* result)
{
    AutoPtr<IMessageDigestHelper> helper;
    CMessageDigestHelper::AcquireSingleton((IMessageDigestHelper**)&helper);
    AutoPtr<IMessageDigest> instance;
    ECode ec = helper->GetInstance(algorithm, (IMessageDigest**)&instance);
    if (FAILED(ec)) return E_ASSERTION_ERROR;

    AutoPtr< ArrayOf<Byte> > encoded, digest;
    principal->GetEncoded((ArrayOf<Byte>**)&encoded);
    instance->Digest(encoded, (ArrayOf<Byte>**)&digest);
    Int32 offset = 0;
    *result = ((((*digest)[offset] & 0xff) <<  0) |
               (((*digest)[offset + 1] & 0xff) <<  8) |
               (((*digest)[offset + 2] & 0xff) << 16) |
               (((*digest)[offset + 3] & 0xff) << 24));
    return NOERROR;
}

static ECode X509_NAME_to_String(X509_NAME* name, unsigned long flags, String* result)
{
    NATIVE_TRACE("X509_NAME_to_String(%p)", name);

    Unique_BIO buffer(BIO_new(BIO_s_mem()));
    if (buffer.get() == NULL) {
        NATIVE_TRACE("X509_NAME_to_String(%p) => threw error", name);
        *result = NULL;
        return E_OUT_OF_MEMORY_ERROR;
    }

    /* Don't interpret the string. */
    flags &= ~(ASN1_STRFLGS_UTF8_CONVERT | ASN1_STRFLGS_ESC_MSB);

    /* Write in given format and null terminate. */
    ::X509_NAME_print_ex(buffer.get(), name, 0, flags);
    BIO_write(buffer.get(), "\0", 1);

    char *tmp;
    BIO_get_mem_data(buffer.get(), &tmp);
    NATIVE_TRACE("X509_NAME_to_String(%p) => \"%s\"", name, tmp);
    *result = tmp;
    return NOERROR;
}

ECode NativeCrypto::X509_NAME_print_ex(
    /* [in] */ Int64 x509NameRef,
    /* [in] */ Int64 jflags,
    /* [out] */ String* result)
{
    X509_NAME* x509name = reinterpret_cast<X509_NAME*>(static_cast<uintptr_t>(x509NameRef));
    unsigned long flags = static_cast<unsigned long>(jflags);
    NATIVE_TRACE("X509_NAME_print_ex(%p, %ld)", x509name, flags);

    if (x509name == NULL) {
        NATIVE_TRACE("X509_NAME_print_ex(%p, %ld) => x509name == null", x509name, flags);
        *result = NULL;
        return E_NULL_POINTER_EXCEPTION;
    }

    return X509_NAME_to_String(x509name, flags, result);
}

template <typename T, T* (*d2i_func)(BIO*, T**)>
static ECode d2i_ASN1Object_to_Int64(Int64 bioRef, Int64* result)
{
    BIO* bio = reinterpret_cast<BIO*>(static_cast<uintptr_t>(bioRef));
    NATIVE_TRACE("d2i_ASN1Object_to_jlong(%p)", bio);

    if (bio == NULL) {
        *result = 0;
        return E_NULL_POINTER_EXCEPTION;
    }

    T* x = d2i_func(bio, NULL);
    if (x == NULL) {
        *result = 0;
        return ThrowExceptionIfNecessary("d2i_ASN1Object_to_jlong");
    }

    *result = reinterpret_cast<uintptr_t>(x);
    return NOERROR;
}

ECode NativeCrypto::D2i_X509_bio(
    /* [in] */ Int64 bioRef,
    /* [out] */ Int64* result)
{
    VALIDATE_NOT_NULL(result);
    return d2i_ASN1Object_to_Int64<X509, d2i_X509_bio>(bioRef, result);
}

ECode NativeCrypto::D2i_X509(
    /* [in] */ ArrayOf<Byte>* certBytes,
    /* [out] */ Int64* result)
{
    X509* x = ByteArrayToASN1<X509, d2i_X509>(certBytes);
    *result = reinterpret_cast<uintptr_t>(x);
    return NOERROR;
}

template<typename T, T* (*PEM_read_func)(BIO*, T**, pem_password_cb*, void*)>
static ECode PEM_ASN1Object_to_Int64(Int64 bioRef, Int64* result)
{
    BIO* bio = reinterpret_cast<BIO*>(static_cast<uintptr_t>(bioRef));
    NATIVE_TRACE("PEM_ASN1Object_to_Int64(%p)", bio);

    if (bio == NULL) {
        NATIVE_TRACE("PEM_ASN1Object_to_Int64(%p) => bio == null", bio);
        *result = 0;
        return E_NULL_POINTER_EXCEPTION;
    }

    T* x = PEM_read_func(bio, NULL, NULL, NULL);
    if (x == NULL) {
        ECode ec = ThrowExceptionIfNecessary("PEM_ASN1Object_to_Int64");
        // Sometimes the PEM functions fail without pushing an error
        if (SUCCEEDED(ec)) {
            ec = ThrowRuntimeException("Failure parsing PEM");
        }
        NATIVE_TRACE("PEM_ASN1Object_to_Int64(%p) => threw exception", bio);
        *result = 0;
        return ec;
    }

    NATIVE_TRACE("PEM_ASN1Object_to_Int64(%p) => %p", bio, x);
    *result = reinterpret_cast<uintptr_t>(x);
    return NOERROR;
}

ECode NativeCrypto::PEM_read_bio_X509(
    /* [in] */ Int64 bioRef,
    /* [out] */ Int64* result)
{
    VALIDATE_NOT_NULL(result);
    NATIVE_TRACE("PEM_read_bio_X509(0x%llx)", bioRef);
    return PEM_ASN1Object_to_Int64<X509, ::PEM_read_bio_X509>(bioRef, result);
}

ECode NativeCrypto::I2d_X509(
    /* [in] */ Int64 x509Ref,
    /* [out, callee] */ ArrayOf<Byte>** result)
{
    VALIDATE_NOT_NULL(result);
    X509* x509 = reinterpret_cast<X509*>(static_cast<uintptr_t>(x509Ref));
    NATIVE_TRACE("i2d_X509(%p)", x509);
    return ASN1ToByteArray<X509, i2d_X509>(x509, result);
}

ECode NativeCrypto::I2d_X509_PUBKEY(
    /* [in] */ Int64 x509Ref,
    /* [out, callee] */ ArrayOf<Byte>** result)
{
    VALIDATE_NOT_NULL(result);
    X509* x509 = reinterpret_cast<X509*>(static_cast<uintptr_t>(x509Ref));
    NATIVE_TRACE("i2d_X509_PUBKEY(%p)", x509);
    return ASN1ToByteArray<X509_PUBKEY, i2d_X509_PUBKEY>(X509_get_X509_PUBKEY(x509), result);
}

ECode NativeCrypto::ASN1_seq_pack_X509(
    /* [in] */ ArrayOf<Int64>* certsArray,
    /* [out, callee] */ ArrayOf<Byte>** result)
{
    VALIDATE_NOT_NULL(result);

    NATIVE_TRACE("ASN1_seq_pack_X509(%p)", certsArray);
    if (certsArray == NULL) {
        NATIVE_TRACE("ASN1_seq_pack_X509(%p) => failed to get certs array", certsArray);
        *result = NULL;
        return NOERROR;
    }

    Unique_sk_X509 certStack(sk_X509_new_null());
    if (certStack.get() == NULL) {
        NATIVE_TRACE("ASN1_seq_pack_X509(%p) => failed to make cert stack", certsArray);
        *result = NULL;
        return NOERROR;
    }

    for (Int32 i = 0; i < certsArray->GetLength(); i++) {
        X509* x509 = reinterpret_cast<X509*>(static_cast<uintptr_t>((*certsArray)[i]));
        sk_X509_push(certStack.get(), X509_dup_nocopy(x509));
    }

    int len;
    Unique_OPENSSL_str encoded(ASN1_seq_pack(
                    reinterpret_cast<STACK_OF(OPENSSL_BLOCK)*>(
                            reinterpret_cast<uintptr_t>(certStack.get())),
                    reinterpret_cast<int (*)(void*, unsigned char**)>(i2d_X509), NULL, &len));
    if (encoded.get() == NULL) {
        NATIVE_TRACE("ASN1_seq_pack_X509(%p) => trouble encoding", certsArray);
        *result = NULL;
        return NOERROR;
    }

    AutoPtr< ArrayOf<Byte> > byteArray = ArrayOf<Byte>::Alloc(len);

    unsigned char* p = reinterpret_cast<unsigned char*>(byteArray->GetPayload());
    memcpy(p, encoded.get(), len);

    *result = byteArray;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

typedef STACK_OF(X509) PKIPATH;

ASN1_ITEM_TEMPLATE(PKIPATH) =
    ASN1_EX_TEMPLATE_TYPE(ASN1_TFLG_SEQUENCE_OF, 0, PkiPath, X509)
ASN1_ITEM_TEMPLATE_END(PKIPATH)

ECode NativeCrypto::ASN1_seq_unpack_X509_bio(
    /* [in] */ Int64 bioRef,
    /* [out, callee] */ ArrayOf<Int64>** result)
{
    VALIDATE_NOT_NULL(result);

    BIO* bio = reinterpret_cast<BIO*>(static_cast<uintptr_t>(bioRef));
    NATIVE_TRACE("ASN1_seq_unpack_X509_bio(%p)", bio);

    Unique_sk_X509 path((PKIPATH*) ASN1_item_d2i_bio(ASN1_ITEM_rptr(PKIPATH), bio, NULL));
    if (path.get() == NULL) {
        *result = NULL;
        return ThrowExceptionIfNecessary("ASN1_seq_unpack_X509_bio");
    }

    size_t size = sk_X509_num(path.get());

    AutoPtr< ArrayOf<Int64> > certArray = ArrayOf<Int64>::Alloc(size);
    for (size_t i = 0; i < size; i++) {
        X509* item = reinterpret_cast<X509*>(sk_X509_shift(path.get()));
        (*certArray)[i] = reinterpret_cast<uintptr_t>(item);
    }

    NATIVE_TRACE("ASN1_seq_unpack_X509_bio(%p) => returns %d items", bio, size);
    *result = certArray;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

ECode NativeCrypto::X509_free(
    /* [in] */ Int64 x509Ref)
{
    X509* x509 = reinterpret_cast<X509*>(static_cast<uintptr_t>(x509Ref));
    NATIVE_TRACE("X509_free(%p)", x509);

    if (x509 == NULL) {
        NATIVE_TRACE("X509_free(%p) => x509 == null", x509);
        return E_NULL_POINTER_EXCEPTION;
    }

    ::X509_free(x509);
    return NOERROR;
}

ECode NativeCrypto::X509_cmp(
    /* [in] */ Int64 x509Ref1,
    /* [in] */ Int64 x509Ref2,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    X509* x509_1 = reinterpret_cast<X509*>(static_cast<uintptr_t>(x509Ref1));
    X509* x509_2 = reinterpret_cast<X509*>(static_cast<uintptr_t>(x509Ref2));
    NATIVE_TRACE("X509_cmp(%p, %p)", x509_1, x509_2);

    if (x509_1 == NULL) {
        NATIVE_TRACE("X509_cmp(%p, %p) => x509_1 == null", x509_1, x509_2);
        *result = -1;
        return E_NULL_POINTER_EXCEPTION;
    }

    if (x509_2 == NULL) {
        NATIVE_TRACE("X509_cmp(%p, %p) => x509_2 == null", x509_1, x509_2);
        *result = -1;
        return E_NULL_POINTER_EXCEPTION;
    }

    int ret = ::X509_cmp(x509_1, x509_2);
    NATIVE_TRACE("X509_cmp(%p, %p) => %d", x509_1, x509_2, ret);
    *result = ret;
    return NOERROR;
}

ECode NativeCrypto::Get_X509_hashCode(
    /* [in] */ Int64 x509Ref,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    X509* x509 = reinterpret_cast<X509*>(static_cast<uintptr_t>(x509Ref));

    if (x509 == NULL) {
        NATIVE_TRACE("get_X509_hashCode(%p) => x509 == null", x509);
        *result = 0;
        return E_NULL_POINTER_EXCEPTION;
    }

    // Force caching extensions.
    X509_check_ca(x509);

    Int32 hashCode = 0;
    for (int i = 0; i < SHA_DIGEST_LENGTH; i++) {
        hashCode = 31 * hashCode + x509->sha1_hash[i];
    }
    *result = hashCode;
    return NOERROR;
}

ECode NativeCrypto::X509_print_ex(
    /* [in] */ Int64 bioRef,
    /* [in] */ Int64 x509Ref,
    /* [in] */ Int64 jnmflag,
    /* [in] */ Int64 jcertflag)
{
    BIO* bio = reinterpret_cast<BIO*>(static_cast<uintptr_t>(bioRef));
    X509* x509 = reinterpret_cast<X509*>(static_cast<uintptr_t>(x509Ref));
    long nmflag = static_cast<long>(jnmflag);
    long certflag = static_cast<long>(jcertflag);
    NATIVE_TRACE("X509_print_ex(%p, %p, %ld, %ld)", bio, x509, nmflag, certflag);

    if (bio == NULL) {
        NATIVE_TRACE("X509_print_ex(%p, %p, %ld, %ld) => bio == null", bio, x509, nmflag, certflag);
        return E_NULL_POINTER_EXCEPTION;
    }

    if (x509 == NULL) {
        NATIVE_TRACE("X509_print_ex(%p, %p, %ld, %ld) => x509 == null", bio, x509, nmflag, certflag);
        return E_NULL_POINTER_EXCEPTION;
    }

    if (!::X509_print_ex(bio, x509, nmflag, certflag)) {
        NATIVE_TRACE("X509_print_ex(%p, %p, %ld, %ld) => threw error", bio, x509, nmflag, certflag);
        return ThrowExceptionIfNecessary("X509_print_ex");
    }
    else {
        NATIVE_TRACE("X509_print_ex(%p, %p, %ld, %ld) => success", bio, x509, nmflag, certflag);
        return NOERROR;
    }
}

ECode NativeCrypto::X509_get_issuer_name(
    /* [in] */ Int64 x509Ref,
    /* [out, callee] */ ArrayOf<Byte>** result)
{
    VALIDATE_NOT_NULL(result);

    X509* x509 = reinterpret_cast<X509*>(static_cast<uintptr_t>(x509Ref));
    NATIVE_TRACE("X509_get_issuer_name(%p)", x509);
    return ASN1ToByteArray<X509_NAME, i2d_X509_NAME>(::X509_get_issuer_name(x509), result);
}

ECode NativeCrypto::X509_get_subject_name(
    /* [in] */ Int64 x509Ref,
    /* [out, callee] */ ArrayOf<Byte>** result)
{
    VALIDATE_NOT_NULL(result);

    X509* x509 = reinterpret_cast<X509*>(static_cast<uintptr_t>(x509Ref));
    NATIVE_TRACE("X509_get_subject_name(%p)", x509);
    return ASN1ToByteArray<X509_NAME, i2d_X509_NAME>(::X509_get_subject_name(x509), result);
}

ECode NativeCrypto::Get_X509_sig_alg_oid(
    /* [in] */ Int64 x509Ref,
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);

    X509* x509 = reinterpret_cast<X509*>(static_cast<uintptr_t>(x509Ref));
    NATIVE_TRACE("get_X509_sig_alg_oid(%p)", x509);

    if (x509 == NULL || x509->sig_alg == NULL) {
        NATIVE_TRACE("get_X509_sig_alg_oid(%p) => x509 == NULL", x509);
        *result = NULL;
        return E_NULL_POINTER_EXCEPTION;
    }

    return ASN1_OBJECT_to_OID_string(x509->sig_alg->algorithm, result);
}

ECode NativeCrypto::Get_X509_sig_alg_parameter(
    /* [in] */ Int64 x509Ref,
    /* [out, callee] */ ArrayOf<Byte>** result)
{
    VALIDATE_NOT_NULL(result);

    X509* x509 = reinterpret_cast<X509*>(static_cast<uintptr_t>(x509Ref));
    NATIVE_TRACE("get_X509_sig_alg_parameter(%p)", x509);

    if (x509 == NULL) {
        NATIVE_TRACE("get_X509_sig_alg_parameter(%p) => x509 == null", x509);
        *result = NULL;
        return E_NULL_POINTER_EXCEPTION;
    }

    if (x509->sig_alg->parameter == NULL) {
        NATIVE_TRACE("get_X509_sig_alg_parameter(%p) => null", x509);
        *result = NULL;
        return NOERROR;
    }

    return ASN1ToByteArray<ASN1_TYPE, i2d_ASN1_TYPE>(x509->sig_alg->parameter, result);
}

ECode NativeCrypto::Get_X509_issuerUID(
    /* [in] */ Int64 x509Ref,
    /* [out, callee] */ ArrayOf<Boolean>** result)
{
    VALIDATE_NOT_NULL(result);

    X509* x509 = reinterpret_cast<X509*>(static_cast<uintptr_t>(x509Ref));
    NATIVE_TRACE("get_X509_issuerUID(%p)", x509);

    if (x509 == NULL) {
        NATIVE_TRACE("get_X509_issuerUID(%p) => x509 == null", x509);
        *result = NULL;
        return E_NULL_POINTER_EXCEPTION;
    }

    if (x509->cert_info->issuerUID == NULL) {
        NATIVE_TRACE("get_X509_issuerUID(%p) => null", x509);
        *result = NULL;
        return NOERROR;
    }

    return ASN1BitStringToBooleanArray(x509->cert_info->issuerUID, result);
}

ECode NativeCrypto::Get_X509_subjectUID(
    /* [in] */ Int64 x509Ref,
    /* [out, callee] */ ArrayOf<Boolean>** result)
{
    VALIDATE_NOT_NULL(result);

    X509* x509 = reinterpret_cast<X509*>(static_cast<uintptr_t>(x509Ref));
    NATIVE_TRACE("get_X509_subjectUID(%p)", x509);

    if (x509 == NULL) {
        NATIVE_TRACE("get_X509_subjectUID(%p) => x509 == null", x509);
        *result = NULL;
        return E_NULL_POINTER_EXCEPTION;
    }

    if (x509->cert_info->subjectUID == NULL) {
        NATIVE_TRACE("get_X509_subjectUID(%p) => null", x509);
        *result = NULL;
        return NOERROR;
    }

    return ASN1BitStringToBooleanArray(x509->cert_info->subjectUID, result);
}

ECode NativeCrypto::X509_get_pubkey(
    /* [in] */ Int64 x509Ref,
    /* [out] */ Int64* result)
{
    VALIDATE_NOT_NULL(result);

    X509* x509 = reinterpret_cast<X509*>(static_cast<uintptr_t>(x509Ref));
    NATIVE_TRACE("X509_get_pubkey(%p)", x509);

    if (x509 == NULL) {
        NATIVE_TRACE("X509_get_pubkey(%p) => x509 == null", x509);
        *result = 0;
        return E_NULL_POINTER_EXCEPTION;
    }

    Unique_EVP_PKEY pkey(::X509_get_pubkey(x509));
    if (pkey.get() == NULL) {
        *result = 0;
        return ThrowExceptionIfNecessary("X509_get_pubkey");
    }

    NATIVE_TRACE("X509_get_pubkey(%p) => %p", x509, pkey.get());
    *result = reinterpret_cast<uintptr_t>(pkey.release());
    return NOERROR;
}

ECode NativeCrypto::Get_X509_pubkey_oid(
    /* [in] */ Int64 x509Ref,
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);

    X509* x509 = reinterpret_cast<X509*>(static_cast<uintptr_t>(x509Ref));
    NATIVE_TRACE("get_X509_pubkey_oid(%p)", x509);

    if (x509 == NULL) {
        NATIVE_TRACE("get_X509_pubkey_oid(%p) => x509 == null", x509);
        *result = NULL;
        return E_NULL_POINTER_EXCEPTION;
    }

    X509_PUBKEY* pubkey = X509_get_X509_PUBKEY(x509);
    return ASN1_OBJECT_to_OID_string(pubkey->algor->algorithm, result);
}

template<typename T, int (*get_ext_by_OBJ_func)(T*, ASN1_OBJECT*, int),
        X509_EXTENSION* (*get_ext_func)(T*, int)>
static X509_EXTENSION *X509Type_get_ext(T* x509Type, const String& oid, ECode* ec)
{
    NATIVE_TRACE("X509Type_get_ext(%p)", x509Type);

    if (x509Type == NULL) {
        *ec = E_NULL_POINTER_EXCEPTION;
        return NULL;
    }

    if (oid.IsNull()) {
        *ec = NOERROR;
        return NULL;
    }

    Unique_ASN1_OBJECT asn1(OBJ_txt2obj(oid.string(), 1));
    if (asn1.get() == NULL) {
        NATIVE_TRACE("X509Type_get_ext(%p, %s) => oid conversion failed", x509Type, oid.string());
        FreeOpenSslErrorState();
        *ec = NOERROR;
        return NULL;
    }

    int extIndex = get_ext_by_OBJ_func(x509Type, asn1.get(), -1);
    if (extIndex == -1) {
        NATIVE_TRACE("X509Type_get_ext(%p, %s) => ext not found", x509Type, oid.string());
        *ec = NOERROR;
        return NULL;
    }

    X509_EXTENSION* ext = get_ext_func(x509Type, extIndex);
    NATIVE_TRACE("X509Type_get_ext(%p, %s) => %p", x509Type, oid.string(), ext);
    *ec = NOERROR;
    return ext;
}

template<typename T, int (*get_ext_by_OBJ_func)(T*, ASN1_OBJECT*, int),
        X509_EXTENSION* (*get_ext_func)(T*, int)>
static ECode X509Type_get_ext_oid(T* x509Type, const String& oidString, ArrayOf<Byte>** result)
{
    ECode ec;
    X509_EXTENSION* ext = X509Type_get_ext<T, get_ext_by_OBJ_func, get_ext_func>(x509Type,
            oidString, &ec);
    if (ext == NULL) {
        NATIVE_TRACE("X509Type_get_ext_oid(%p, %s) => fetching extension failed", x509Type, oidString.string());
        *result = NULL;
        return ec;
    }

    NATIVE_TRACE("X509Type_get_ext_oid(%p, %s) => %p", x509Type, oidString.string(), ext->value);
    return ASN1ToByteArray<ASN1_OCTET_STRING, i2d_ASN1_OCTET_STRING>(ext->value, result);
}

ECode NativeCrypto::X509_get_ext_oid(
    /* [in] */ Int64 x509Ref,
    /* [in] */ const String& oid,
    /* [out, callee] */ ArrayOf<Byte>** result)
{
    VALIDATE_NOT_NULL(result);
    X509* x509 = reinterpret_cast<X509*>(static_cast<uintptr_t>(x509Ref));
    NATIVE_TRACE("X509_get_ext_oid(%p, %s)", x509, oid.string());
    return X509Type_get_ext_oid<X509, X509_get_ext_by_OBJ, X509_get_ext>(x509, oid, result);
}

template<typename T, int (*get_ext_by_critical_func)(T*, int, int), X509_EXTENSION* (*get_ext_func)(T*, int)>
static ECode get_X509Type_ext_oids(Int64 x509Ref, Int32 critical, ArrayOf<String>** result)
{
    T* x509 = reinterpret_cast<T*>(static_cast<uintptr_t>(x509Ref));
    NATIVE_TRACE("get_X509Type_ext_oids(%p, %d)", x509, critical);

    if (x509 == NULL) {
        NATIVE_TRACE("get_X509Type_ext_oids(%p, %d) => x509 == null", x509, critical);
        *result = NULL;
        return E_NULL_POINTER_EXCEPTION;
    }

    int lastPos = -1;
    int count = 0;
    while ((lastPos = get_ext_by_critical_func(x509, critical, lastPos)) != -1) {
        count++;
    }

    NATIVE_TRACE("get_X509Type_ext_oids(%p, %d) has %d entries", x509, critical, count);

    AutoPtr< ArrayOf<String> > joa = ArrayOf<String>::Alloc(count);

    lastPos = -1;
    count = 0;
    while ((lastPos = get_ext_by_critical_func(x509, critical, lastPos)) != -1) {
        X509_EXTENSION* ext = get_ext_func(x509, lastPos);

        String extOid;
        ECode ec = ASN1_OBJECT_to_OID_string(ext->object, &extOid);
        if (extOid.IsNull()) {
            NATIVE_TRACE("get_X509Type_ext_oids(%p) => couldn't get OID", x509);
            *result = NULL;
            return ec;
        }

        (*joa)[count++] = extOid;
    }

    NATIVE_TRACE("get_X509Type_ext_oids(%p, %d) => success", x509, critical);
    *result = joa;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

ECode NativeCrypto::Get_X509_ext_oids(
    /* [in] */ Int64 x509Ref,
    /* [in] */ Int32 critical,
    /* [out, callee] */ ArrayOf<String>** result)
{
    VALIDATE_NOT_NULL(result);
    NATIVE_TRACE("get_X509_ext_oids(0x%llx, %d)", x509Ref, critical);
    return get_X509Type_ext_oids<X509, X509_get_ext_by_critical, X509_get_ext>(x509Ref,
            critical, result);
}

/**
 * Converts GENERAL_NAME items to the output format expected in
 * X509Certificate#getSubjectAlternativeNames and
 * X509Certificate#getIssuerAlternativeNames return.
 */
static ECode GENERAL_NAME_to_object(GENERAL_NAME* gen, IInterface** result)
{
    switch (gen->type) {
    case GEN_EMAIL:
    case GEN_DNS:
    case GEN_URI: {
        // This must not be a T61String and must not contain NULLs.
        const char* data = reinterpret_cast<const char*>(ASN1_STRING_data(gen->d.ia5));
        ssize_t len = ASN1_STRING_length(gen->d.ia5);
        if ((len == static_cast<ssize_t>(strlen(data)))
                && (ASN1_PRINTABLE_type(ASN1_STRING_data(gen->d.ia5), len) != V_ASN1_T61STRING)) {
            NATIVE_TRACE("GENERAL_NAME_to_jobject(%p) => Email/DNS/URI \"%s\"", gen, data);
            String str(data);
            return CString::New(str, (ICharSequence**)result);
        }
        else {
            NATIVE_TRACE("GENERAL_NAME_to_jobject(%p) => Email/DNS/URI invalid", gen);
            *result = NULL;
            return E_CERTIFICATE_PARSING_EXCEPTION;
        }
    }
    case GEN_DIRNAME: {
        /* Write in RFC 2253 format */
        String str;
        ECode ec = X509_NAME_to_String(gen->d.directoryName, XN_FLAG_RFC2253, &str);
        if (FAILED(ec)) {
            *result = NULL;
            return ec;
        }
        return CString::New(str, (ICharSequence**)result);
    }
    case GEN_IPADD: {
        const void *ip = reinterpret_cast<const void *>(gen->d.ip->data);
        if (gen->d.ip->length == 4) {
            // IPv4
            UniquePtr<char[]> buffer(new char[INET_ADDRSTRLEN]);
            if (inet_ntop(AF_INET, ip, buffer.get(), INET_ADDRSTRLEN) != NULL) {
                NATIVE_TRACE("GENERAL_NAME_to_jobject(%p) => IPv4 %s", gen, buffer.get());
                String str(buffer.get());
                return CString::New(str, (ICharSequence**)result);
            }
            else {
                NATIVE_TRACE("GENERAL_NAME_to_jobject(%p) => IPv4 failed %s", gen, strerror(errno));
            }
        }
        else if (gen->d.ip->length == 16) {
            // IPv6
            UniquePtr<char[]> buffer(new char[INET6_ADDRSTRLEN]);
            if (inet_ntop(AF_INET6, ip, buffer.get(), INET6_ADDRSTRLEN) != NULL) {
                NATIVE_TRACE("GENERAL_NAME_to_jobject(%p) => IPv6 %s", gen, buffer.get());
                String str(buffer.get());
                return CString::New(str, (ICharSequence**)result);
            }
            else {
                NATIVE_TRACE("GENERAL_NAME_to_jobject(%p) => IPv6 failed %s", gen, strerror(errno));
            }
        }

        /* Invalid IP encodings are pruned out without throwing an exception. */
        *result = NULL;
        return NOERROR;
    }
    case GEN_RID: {
        String str;
        ECode ec = ASN1_OBJECT_to_OID_string(gen->d.registeredID, &str);
        if (FAILED(ec)) {
            *result = NULL;
            return ec;
        }
        return CString::New(str, (ICharSequence**)result);
    }
    case GEN_OTHERNAME:
    case GEN_X400:
    default: {
        AutoPtr< ArrayOf<Byte> > byteArray;
        ECode ec = ASN1ToByteArray<GENERAL_NAME, i2d_GENERAL_NAME>(gen, (ArrayOf<Byte>**)&byteArray);
        if (FAILED(ec)) {
            *result = NULL;
            return ec;
        }
        AutoPtr<IArrayOf> arrayObj;
        CArrayOf::New(EIID_IByte, byteArray->GetLength(), (IArrayOf**)&arrayObj);
        for (Int32 i = 0; i < byteArray->GetLength(); ++i) {
            AutoPtr<IByte> bObj;
            CByte::New((*byteArray)[i], (IByte**)&bObj);
            arrayObj->Set(i, bObj);
        }
        *result = arrayObj;
        REFCOUNT_ADD(*result);
        return NOERROR;
    }
    }

    *result = NULL;
    return NOERROR;
}

#define GN_STACK_SUBJECT_ALT_NAME 1
#define GN_STACK_ISSUER_ALT_NAME 2

ECode NativeCrypto::Get_X509_GENERAL_NAME_stack(
    /* [in] */ Int64 x509Ref,
    /* [in] */ Int32 type,
    /* [out, callee] */ ArrayOf<IArrayOf*>** result)
{
    VALIDATE_NOT_NULL(result);

    X509* x509 = reinterpret_cast<X509*>(static_cast<uintptr_t>(x509Ref));
    NATIVE_TRACE("get_X509_GENERAL_NAME_stack(%p, %d)", x509, type);

    if (x509 == NULL) {
        NATIVE_TRACE("get_X509_GENERAL_NAME_stack(%p, %d) => x509 == null", x509, type);
        *result = NULL;
        return E_NULL_POINTER_EXCEPTION;
    }

    X509_check_ca(x509);

    STACK_OF(GENERAL_NAME)* gn_stack;
    Unique_sk_GENERAL_NAME stackHolder;
    if (type == GN_STACK_SUBJECT_ALT_NAME) {
        gn_stack = x509->altname;
    }
    else if (type == GN_STACK_ISSUER_ALT_NAME) {
        stackHolder.reset(
                static_cast<STACK_OF(GENERAL_NAME)*>(X509_get_ext_d2i(x509, NID_issuer_alt_name,
                        NULL, NULL)));
        gn_stack = stackHolder.get();
    }
    else {
        NATIVE_TRACE("get_X509_GENERAL_NAME_stack(%p, %d) => unknown type", x509, type);
        *result = NULL;
        return NOERROR;
    }

    int count = sk_GENERAL_NAME_num(gn_stack);
    if (count <= 0) {
        NATIVE_TRACE("get_X509_GENERAL_NAME_stack(%p, %d) => null (no entries)", x509, type);
        *result = NULL;
        return NOERROR;
    }

    /*
     * Keep track of how many originally so we can ignore any invalid
     * values later.
     */
    const int origCount = count;

    AutoPtr< ArrayOf<IArrayOf*> > joa = ArrayOf<IArrayOf*>::Alloc(count);
    for (int i = 0, j = 0; i < origCount; i++, j++) {
        GENERAL_NAME* gen = sk_GENERAL_NAME_value(gn_stack, i);
        AutoPtr<IInterface> val;
        ECode ec = GENERAL_NAME_to_object(gen, (IInterface**)&val);
        if (FAILED(ec)) {
            NATIVE_TRACE("get_X509_GENERAL_NAME_stack(%p, %d) => threw exception parsing gen name",
                    x509, type);
            *result = NULL;
            return ec;
        }

        /*
         * If it's NULL, we'll have to skip this, reduce the number of total
         * entries, and fix up the array later.
         */
        if (val == NULL) {
            j--;
            count--;
            continue;
        }

        AutoPtr<IArrayOf> item;
        CArrayOf::New(EIID_IInterface, 2, (IArrayOf**)&item);

        AutoPtr<IInteger32> type;
        CInteger32::New(gen->type, (IInteger32**)&type);
        item->Set(0, type);
        item->Set(1, val);

        joa->Set(j, item);
    }

    if (count == 0) {
        NATIVE_TRACE("get_X509_GENERAL_NAME_stack(%p, %d) shrunk from %d to 0; returning NULL",
                x509, type, origCount);
        joa = NULL;
    }
    else if (origCount != count) {
        NATIVE_TRACE("get_X509_GENERAL_NAME_stack(%p, %d) shrunk from %d to %d", x509, type,
                origCount, count);

        AutoPtr< ArrayOf<IArrayOf*> > joa_copy = ArrayOf<IArrayOf*>::Alloc(count);

        for (int i = 0; i < count; i++) {
            AutoPtr<IArrayOf> item = (*joa)[i];
            joa_copy->Set(i, item);
        }

        joa = joa_copy;
    }

    NATIVE_TRACE("get_X509_GENERAL_NAME_stack(%p, %d) => %d entries", x509, type, count);
    *result = joa;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

ECode NativeCrypto::Get_X509_ex_kusage(
    /* [in] */ Int64 x509Ref,
    /* [out, callee] */ ArrayOf<Boolean>** result)
{
    VALIDATE_NOT_NULL(result);

    X509* x509 = reinterpret_cast<X509*>(static_cast<uintptr_t>(x509Ref));
    NATIVE_TRACE("get_X509_ex_kusage(%p)", x509);

    if (x509 == NULL) {
        NATIVE_TRACE("get_X509_ex_kusage(%p) => x509 == null", x509);
        *result = NULL;
        return E_NULL_POINTER_EXCEPTION;
    }

    Unique_ASN1_BIT_STRING bitStr(static_cast<ASN1_BIT_STRING*>(
            X509_get_ext_d2i(x509, NID_key_usage, NULL, NULL)));
    if (bitStr.get() == NULL) {
        NATIVE_TRACE("get_X509_ex_kusage(%p) => null", x509);
        *result = NULL;
        return NOERROR;
    }

    return ASN1BitStringToBooleanArray(bitStr.get(), result);
}

ECode NativeCrypto::Get_X509_ex_xkusage(
    /* [in] */ Int64 x509Ref,
    /* [out, callee] */ ArrayOf<String>** result)
{
    VALIDATE_NOT_NULL(result);

    X509* x509 = reinterpret_cast<X509*>(static_cast<uintptr_t>(x509Ref));
    NATIVE_TRACE("get_X509_ex_xkusage(%p)", x509);

    if (x509 == NULL) {
        NATIVE_TRACE("get_X509_ex_xkusage(%p) => x509 == null", x509);
        *result = NULL;
        return E_NULL_POINTER_EXCEPTION;
    }

    Unique_sk_ASN1_OBJECT objArray(static_cast<STACK_OF(ASN1_OBJECT)*>(
            X509_get_ext_d2i(x509, NID_ext_key_usage, NULL, NULL)));
    if (objArray.get() == NULL) {
        NATIVE_TRACE("get_X509_ex_xkusage(%p) => null", x509);
        *result = NULL;
        return NOERROR;
    }

    size_t size = sk_ASN1_OBJECT_num(objArray.get());
    AutoPtr< ArrayOf<String> > exKeyUsage = ArrayOf<String>::Alloc(size);

    for (size_t i = 0; i < size; i++) {
        String oidStr;
        ASN1_OBJECT_to_OID_string(
                sk_ASN1_OBJECT_value(objArray.get(), i), &oidStr);
        (*exKeyUsage)[i] = oidStr;
    }

    NATIVE_TRACE("get_X509_ex_xkusage(%p) => success (%d entries)", x509, size);
    *result = exKeyUsage;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

ECode NativeCrypto::Get_X509_ex_pathlen(
    /* [in] */ Int64 x509Ref,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    X509* x509 = reinterpret_cast<X509*>(static_cast<uintptr_t>(x509Ref));
    NATIVE_TRACE("get_X509_ex_pathlen(%p)", x509);

    if (x509 == NULL) {
        NATIVE_TRACE("get_X509_ex_pathlen(%p) => x509 == null", x509);
        *result = 0;
        return E_NULL_POINTER_EXCEPTION;
    }

    /* Just need to do this to cache the ex_* values. */
    X509_check_ca(x509);

    NATIVE_TRACE("get_X509_ex_pathlen(%p) => %ld", x509, x509->ex_pathlen);
    *result = x509->ex_pathlen;
    return NOERROR;
}

ECode NativeCrypto::X509_get_notBefore(
    /* [in] */ Int64 x509Ref,
    /* [out] */ Int64* result)
{
    VALIDATE_NOT_NULL(result);

    X509* x509 = reinterpret_cast<X509*>(static_cast<uintptr_t>(x509Ref));
    NATIVE_TRACE("X509_get_notBefore(%p)", x509);

    if (x509 == NULL) {
        NATIVE_TRACE("X509_get_notBefore(%p) => x509 == null", x509);
        *result = 0;
        return E_NULL_POINTER_EXCEPTION;
    }

    ASN1_TIME* notBefore = X509_GET_NOTBEFORE(x509);
    NATIVE_TRACE("X509_get_notBefore(%p) => %p", x509, notBefore);
    *result = reinterpret_cast<uintptr_t>(notBefore);
    return NOERROR;
}

ECode NativeCrypto::X509_get_notAfter(
    /* [in] */ Int64 x509Ref,
    /* [out] */ Int64* result)
{
    VALIDATE_NOT_NULL(result);

    X509* x509 = reinterpret_cast<X509*>(static_cast<uintptr_t>(x509Ref));
    NATIVE_TRACE("X509_get_notAfter(%p)", x509);

    if (x509 == NULL) {
        NATIVE_TRACE("X509_get_notAfter(%p) => x509 == null", x509);
        *result = 0;
        return E_NULL_POINTER_EXCEPTION;
    }

    ASN1_TIME* notAfter = X509_GET_NOTAFTER(x509);
    NATIVE_TRACE("X509_get_notAfter(%p) => %p", x509, notAfter);
    *result = reinterpret_cast<uintptr_t>(notAfter);
    return NOERROR;
}

ECode NativeCrypto::X509_get_version(
    /* [in] */ Int64 x509Ref,
    /* [out] */ Int64* result)
{
    VALIDATE_NOT_NULL(result);

    X509* x509 = reinterpret_cast<X509*>(static_cast<uintptr_t>(x509Ref));
    NATIVE_TRACE("X509_get_version(%p)", x509);

    long version = X509_GET_VERSION(x509);
    NATIVE_TRACE("X509_get_version(%p) => %ld", x509, version);
    *result = version;
    return NOERROR;
}

template<typename T>
static ECode get_X509Type_serialNumber(T* x509Type, ASN1_INTEGER* (*get_serial_func)(T*), ArrayOf<Byte>** result)
{
    NATIVE_TRACE("get_X509Type_serialNumber(%p)", x509Type);

    if (x509Type == NULL) {
        NATIVE_TRACE("get_X509Type_serialNumber(%p) => x509Type == null", x509Type);
        *result = NULL;
        return E_NULL_POINTER_EXCEPTION;
    }

    ASN1_INTEGER* serialNumber = get_serial_func(x509Type);
    Unique_BIGNUM serialBn(ASN1_INTEGER_to_BN(serialNumber, NULL));
    if (serialBn.get() == NULL) {
        NATIVE_TRACE("X509_get_serialNumber(%p) => threw exception", x509Type);
        *result = NULL;
        return NOERROR;
    }

    AutoPtr< ArrayOf<Byte> > serialArray;
    ECode ec = BignumToArray(serialBn.get(), "serialBn", (ArrayOf<Byte>**)&serialArray);
    if (FAILED(ec)) {
        NATIVE_TRACE("X509_get_serialNumber(%p) => threw exception", x509Type);
        *result = NULL;
        return ec;
    }

    NATIVE_TRACE("X509_get_serialNumber(%p) => %p", x509Type, serialArray.Get());
    *result = serialArray;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

ECode NativeCrypto::X509_get_serialNumber(
    /* [in] */ Int64 x509Ref,
    /* [out, callee] */ ArrayOf<Byte>** result)
{
    VALIDATE_NOT_NULL(result);

    X509* x509 = reinterpret_cast<X509*>(static_cast<uintptr_t>(x509Ref));
    NATIVE_TRACE("X509_get_serialNumber(%p)", x509);
    return get_X509Type_serialNumber<X509>(x509, ::X509_get_serialNumber, result);
}

ECode NativeCrypto::X509_verify(
    /* [in] */ Int64 x509Ref,
    /* [in] */ Int64 pkeyRef)
{
    X509* x509 = reinterpret_cast<X509*>(static_cast<uintptr_t>(x509Ref));
    EVP_PKEY* pkey = reinterpret_cast<EVP_PKEY*>(pkeyRef);
    NATIVE_TRACE("X509_verify(%p, %p)", x509, pkey);

    if (x509 == NULL) {
        NATIVE_TRACE("X509_verify(%p, %p) => x509 == null", x509, pkey);
        return E_NULL_POINTER_EXCEPTION;
    }

    if (pkey == NULL) {
        NATIVE_TRACE("X509_verify(%p, %p) => pkey == null", x509, pkey);
        return E_NULL_POINTER_EXCEPTION;
    }

    if (::X509_verify(x509, pkey) != 1) {
        NATIVE_TRACE("X509_verify(%p, %p) => verify failure", x509, pkey);
        return ThrowExceptionIfNecessary("X509_verify");
    }
    else {
        NATIVE_TRACE("X509_verify(%p, %p) => verify success", x509, pkey);
        return NOERROR;
    }
}

ECode NativeCrypto::Get_X509_cert_info_enc(
    /* [in] */ Int64 x509Ref,
    /* [out, callee] */ ArrayOf<Byte>** result)
{
    VALIDATE_NOT_NULL(result);
    X509* x509 = reinterpret_cast<X509*>(static_cast<uintptr_t>(x509Ref));
    NATIVE_TRACE("get_X509_cert_info_enc(%p)", x509);
    return ASN1ToByteArray<X509_CINF, i2d_X509_CINF>(x509->cert_info, result);
}

static void get_X509_signature(X509 *x509, ASN1_BIT_STRING** signature)
{
    *signature = x509->signature;
}

template<typename T>
static ECode get_X509Type_signature(T* x509Type, void (*get_signature_func)(T*, ASN1_BIT_STRING**), ArrayOf<Byte>** result)
{
    NATIVE_TRACE("get_X509Type_signature(%p)", x509Type);

    if (x509Type == NULL) {
        NATIVE_TRACE("get_X509Type_signature(%p) => x509Type == null", x509Type);
        *result = NULL;
        return E_NULL_POINTER_EXCEPTION;
    }

    ASN1_BIT_STRING* signature;
    get_signature_func(x509Type, &signature);

    AutoPtr< ArrayOf<Byte> > signatureArray = ArrayOf<Byte>::Alloc(signature->length);

    memcpy(signatureArray->GetPayload(), signature->data, signature->length);

    NATIVE_TRACE("get_X509Type_signature(%p) => %p (%d bytes)", x509Type, signatureArray.Get(),
            signature->length);
    *result = signatureArray;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

ECode NativeCrypto::Get_X509_signature(
    /* [in] */ Int64 x509Ref,
    /* [out, callee] */ ArrayOf<Byte>** result)
{
    VALIDATE_NOT_NULL(result);
    X509* x509 = reinterpret_cast<X509*>(static_cast<uintptr_t>(x509Ref));
    NATIVE_TRACE("get_X509_signature(%p)", x509);
    return get_X509Type_signature<X509>(x509, get_X509_signature, result);
}

ECode NativeCrypto::Get_X509_ex_flags(
    /* [in] */ Int64 x509Ref,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    X509* x509 = reinterpret_cast<X509*>(static_cast<uintptr_t>(x509Ref));
    NATIVE_TRACE("get_X509_ex_flags(%p)", x509);

    if (x509 == NULL) {
        NATIVE_TRACE("get_X509_ex_flags(%p) => x509 == null", x509);
        *result = 0;
        return E_NULL_POINTER_EXCEPTION;
    }

    X509_check_ca(x509);

    *result = x509->ex_flags;
    return NOERROR;
}

ECode NativeCrypto::X509_check_issued(
    /* [in] */ Int64 x509Ref1,
    /* [in] */ Int64 x509Ref2,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    X509* x509_1 = reinterpret_cast<X509*>(static_cast<uintptr_t>(x509Ref1));
    X509* x509_2 = reinterpret_cast<X509*>(static_cast<uintptr_t>(x509Ref2));
    NATIVE_TRACE("X509_check_issued(%p, %p)", x509_1, x509_2);

    int ret = ::X509_check_issued(x509_1, x509_2);
    NATIVE_TRACE("X509_check_issued(%p, %p) => %d", x509_1, x509_2, ret);
    *result = ret;
    return NOERROR;
}

static STACK_OF(X509)* PKCS7_get_certs(PKCS7* pkcs7)
{
    if (PKCS7_type_is_signed(pkcs7)) {
        return pkcs7->d.sign->cert;
    }
    else if (PKCS7_type_is_signedAndEnveloped(pkcs7)) {
        return pkcs7->d.signed_and_enveloped->cert;
    }
    else {
        NATIVE_TRACE("PKCS7_get_certs(%p) => unknown PKCS7 type", pkcs7);
        return NULL;
    }
}

static STACK_OF(X509_CRL)* PKCS7_get_CRLs(PKCS7* pkcs7)
{
    if (PKCS7_type_is_signed(pkcs7)) {
        return pkcs7->d.sign->crl;
    }
    else if (PKCS7_type_is_signedAndEnveloped(pkcs7)) {
        return pkcs7->d.signed_and_enveloped->crl;
    }
    else {
        NATIVE_TRACE("PKCS7_get_CRLs(%p) => unknown PKCS7 type", pkcs7);
        return NULL;
    }
}

template <typename T, typename T_stack>
static ECode PKCS7_to_ItemArray(T_stack* stack, T* (*dup_func)(T*), ArrayOf<Int64>** result)
{
    if (stack == NULL) {
        *result = NULL;
        return NOERROR;
    }

    size_t size = sk_num(reinterpret_cast<_STACK*>(stack));
    AutoPtr< ArrayOf<Int64> > ref_array = ArrayOf<Int64>::Alloc(size);
    for (size_t i = 0; i < size; i++) {
        T* item = reinterpret_cast<T*>(sk_value(reinterpret_cast<_STACK*>(stack), i));
        (*ref_array)[i] = reinterpret_cast<uintptr_t>(dup_func(item));
    }

    NATIVE_TRACE("PKCS7_to_ItemArray(%p) => %p [size=%d]", stack, ref_array.Get(), size);
    *result = ref_array;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

#define PKCS7_CERTS 1
#define PKCS7_CRLS 2

ECode NativeCrypto::D2i_PKCS7_bio(
    /* [in] */ Int64 bioRef,
    /* [in] */ Int32 which,
    /* [out, callee] */ ArrayOf<Int64>** result)
{
    VALIDATE_NOT_NULL(result);
    BIO* bio = reinterpret_cast<BIO*>(static_cast<uintptr_t>(bioRef));
    NATIVE_TRACE("d2i_PKCS7_bio(%p, %d)", bio, which);

    if (bio == NULL) {
        NATIVE_TRACE("d2i_PKCS7_bio(%p, %d) => bio == null", bio, which);
        *result = NULL;
        return E_NULL_POINTER_EXCEPTION;
    }

    Unique_PKCS7 pkcs7(d2i_PKCS7_bio(bio, NULL));
    if (pkcs7.get() == NULL) {
        NATIVE_TRACE("d2i_PKCS7_bio(%p, %d) => threw exception", bio, which);
        *result = 0;
        return ThrowExceptionIfNecessary("d2i_PKCS7_bio");
    }

    switch (which) {
    case PKCS7_CERTS:
        return PKCS7_to_ItemArray<X509, STACK_OF(X509)>(PKCS7_get_certs(pkcs7.get()), X509_dup, result);
    case PKCS7_CRLS:
        return PKCS7_to_ItemArray<X509_CRL, STACK_OF(X509_CRL)>(PKCS7_get_CRLs(pkcs7.get()),
                X509_CRL_dup, result);
    default:
        *result = NULL;
        return ThrowRuntimeException("unknown PKCS7 field");
    }
}

ECode NativeCrypto::I2d_PKCS7(
    /* [in] */ ArrayOf<Int64>* certsArray,
    /* [out, callee] */ ArrayOf<Byte>** result)
{
    VALIDATE_NOT_NULL(result);
    NATIVE_TRACE("i2d_PKCS7(%p)", certsArray);

    Unique_PKCS7 pkcs7(PKCS7_new());
    if (pkcs7.get() == NULL) {
        NATIVE_TRACE("i2d_PKCS7(%p) => pkcs7 == null", certsArray);
        *result = NULL;
        return E_NULL_POINTER_EXCEPTION;
    }

    if (PKCS7_set_type(pkcs7.get(), NID_pkcs7_signed) != 1) {
        *result = NULL;
        return ThrowExceptionIfNecessary("PKCS7_set_type");
    }

    for (Int32 i = 0; i < certsArray->GetLength(); i++) {
        X509* item = reinterpret_cast<X509*>((*certsArray)[i]);
        if (PKCS7_add_certificate(pkcs7.get(), item) != 1) {
            *result = NULL;
            return ThrowExceptionIfNecessary("i2d_PKCS7");
        }
    }

    NATIVE_TRACE("i2d_PKCS7(%p) => %d certs", certsArray, certsArray->GetLength());
    return ASN1ToByteArray<PKCS7, i2d_PKCS7>(pkcs7.get(), result);
}

ECode NativeCrypto::PEM_read_bio_PKCS7(
    /* [in] */ Int64 bioRef,
    /* [in] */ Int32 which,
    /* [out, callee] */ ArrayOf<Int64>** result)
{
    VALIDATE_NOT_NULL(result);
    BIO* bio = reinterpret_cast<BIO*>(static_cast<uintptr_t>(bioRef));
    NATIVE_TRACE("PEM_read_bio_PKCS7_CRLs(%p)", bio);

    if (bio == NULL) {
        NATIVE_TRACE("PEM_read_bio_PKCS7_CRLs(%p) => bio == null", bio);
        *result = 0;
        return E_NULL_POINTER_EXCEPTION;
    }

    Unique_PKCS7 pkcs7(::PEM_read_bio_PKCS7(bio, NULL, NULL, NULL));
    if (pkcs7.get() == NULL) {
        NATIVE_TRACE("PEM_read_bio_PKCS7_CRLs(%p) => threw exception", bio);
        *result = 0;
        return ThrowExceptionIfNecessary("PEM_read_bio_PKCS7_CRLs");
    }

    switch (which) {
    case PKCS7_CERTS:
        return PKCS7_to_ItemArray<X509, STACK_OF(X509)>(PKCS7_get_certs(pkcs7.get()), X509_dup, result);
    case PKCS7_CRLS:
        return PKCS7_to_ItemArray<X509_CRL, STACK_OF(X509_CRL)>(PKCS7_get_CRLs(pkcs7.get()),
                X509_CRL_dup, result);
    default:
        *result = NULL;
        return ThrowRuntimeException("unknown PKCS7 field");
    }
}

ECode NativeCrypto::D2i_X509_CRL_bio(
    /* [in] */ Int64 bioRef,
    /* [out] */ Int64* result)
{
    VALIDATE_NOT_NULL(result);
    return d2i_ASN1Object_to_Int64<X509_CRL, d2i_X509_CRL_bio>(bioRef, result);
}

ECode NativeCrypto::PEM_read_bio_X509_CRL(
    /* [in] */ Int64 bioRef,
    /* [out] */ Int64* result)
{
    VALIDATE_NOT_NULL(result);
    NATIVE_TRACE("PEM_read_bio_X509_CRL(0x%llx)", bioRef);
    return PEM_ASN1Object_to_Int64<X509_CRL, ::PEM_read_bio_X509_CRL>(bioRef, result);
}

ECode NativeCrypto::I2d_X509_CRL(
    /* [in] */ Int64 x509CrlRef,
    /* [out, callee] */ ArrayOf<Byte>** result)
{
    VALIDATE_NOT_NULL(result);
    X509_CRL* crl = reinterpret_cast<X509_CRL*>(static_cast<uintptr_t>(x509CrlRef));
    NATIVE_TRACE("i2d_X509_CRL(%p)", crl);
    return ASN1ToByteArray<X509_CRL, i2d_X509_CRL>(crl, result);
}

ECode NativeCrypto::X509_CRL_free(
    /* [in] */ Int64 x509CrlRef)
{
    X509_CRL* crl = reinterpret_cast<X509_CRL*>(static_cast<uintptr_t>(x509CrlRef));
    NATIVE_TRACE("X509_CRL_free(%p)", crl);

    if (crl == NULL) {
        NATIVE_TRACE("X509_CRL_free(%p) => crl == null", crl);
        return E_NULL_POINTER_EXCEPTION;
    }

    ::X509_CRL_free(crl);
    return NOERROR;
}

ECode NativeCrypto::X509_CRL_print(
    /* [in] */ Int64 bioRef,
    /* [in] */ Int64 x509CrlRef)
{
    BIO* bio = reinterpret_cast<BIO*>(static_cast<uintptr_t>(bioRef));
    X509_CRL* crl = reinterpret_cast<X509_CRL*>(static_cast<uintptr_t>(x509CrlRef));
    NATIVE_TRACE("X509_CRL_print(%p, %p)", bio, crl);

    if (bio == NULL) {
        NATIVE_TRACE("X509_CRL_print(%p, %p) => bio == null", bio, crl);
        return E_NULL_POINTER_EXCEPTION;
    }

    if (crl == NULL) {
        NATIVE_TRACE("X509_CRL_print(%p, %p) => crl == null", bio, crl);
        return E_NULL_POINTER_EXCEPTION;
    }

    if (!::X509_CRL_print(bio, crl)) {
        NATIVE_TRACE("X509_CRL_print(%p, %p) => threw error", bio, crl);
        return ThrowExceptionIfNecessary("X509_CRL_print");
    }
    else {
        NATIVE_TRACE("X509_CRL_print(%p, %p) => success", bio, crl);
        return NOERROR;
    }
}

ECode NativeCrypto::Get_X509_CRL_sig_alg_oid(
    /* [in] */ Int64 x509CrlRef,
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);
    X509_CRL* crl = reinterpret_cast<X509_CRL*>(static_cast<uintptr_t>(x509CrlRef));
    NATIVE_TRACE("get_X509_CRL_sig_alg_oid(%p)", crl);

    if (crl == NULL || crl->sig_alg == NULL) {
        NATIVE_TRACE("get_X509_CRL_sig_alg_oid(%p) => crl == NULL", crl);
        *result = NULL;
        return E_NULL_POINTER_EXCEPTION;
    }

    return ASN1_OBJECT_to_OID_string(crl->sig_alg->algorithm, result);
}

ECode NativeCrypto::Get_X509_CRL_sig_alg_parameter(
    /* [in] */ Int64 x509CrlRef,
    /* [out, callee] */ ArrayOf<Byte>** result)
{
    X509_CRL* crl = reinterpret_cast<X509_CRL*>(static_cast<uintptr_t>(x509CrlRef));
    NATIVE_TRACE("get_X509_CRL_sig_alg_parameter(%p)", crl);

    if (crl == NULL) {
        NATIVE_TRACE("get_X509_CRL_sig_alg_parameter(%p) => crl == null", crl);
        *result = NULL;
        return E_NULL_POINTER_EXCEPTION;
    }

    if (crl->sig_alg->parameter == NULL) {
        NATIVE_TRACE("get_X509_CRL_sig_alg_parameter(%p) => null", crl);
        *result = NULL;
        return NOERROR;
    }

    return ASN1ToByteArray<ASN1_TYPE, i2d_ASN1_TYPE>(crl->sig_alg->parameter, result);
}

ECode NativeCrypto::X509_CRL_get_issuer_name(
    /* [in] */ Int64 x509CrlRef,
    /* [out, callee] */ ArrayOf<Byte>** result)
{
    VALIDATE_NOT_NULL(result);
    X509_CRL* crl = reinterpret_cast<X509_CRL*>(static_cast<uintptr_t>(x509CrlRef));
    NATIVE_TRACE("X509_CRL_get_issuer_name(%p)", crl);
    return ASN1ToByteArray<X509_NAME, i2d_X509_NAME>(X509_CRL_get_issuer(crl), result);
}

ECode NativeCrypto::X509_CRL_get0_by_cert(
    /* [in] */ Int64 x509crlRef,
    /* [in] */ Int64 x509Ref,
    /* [out] */ Int64* result)
{
    VALIDATE_NOT_NULL(result);
    X509_CRL* x509crl = reinterpret_cast<X509_CRL*>(static_cast<uintptr_t>(x509crlRef));
    X509* x509 = reinterpret_cast<X509*>(static_cast<uintptr_t>(x509Ref));
    NATIVE_TRACE("X509_CRL_get0_by_cert(%p, %p)", x509crl, x509);

    if (x509crl == NULL) {
        NATIVE_TRACE("X509_CRL_get0_by_cert(%p, %p) => x509crl == null", x509crl, x509);
        *result = 0;
        return E_NULL_POINTER_EXCEPTION;
    }
    else if (x509 == NULL) {
        NATIVE_TRACE("X509_CRL_get0_by_cert(%p, %p) => x509 == null", x509crl, x509);
        *result = 0;
        return E_NULL_POINTER_EXCEPTION;
    }

    X509_REVOKED* revoked = NULL;
    int ret = ::X509_CRL_get0_by_cert(x509crl, &revoked, x509);
    if (ret == 0) {
        NATIVE_TRACE("X509_CRL_get0_by_cert(%p, %p) => none", x509crl, x509);
        *result = 0;
        return NOERROR;
    }

    NATIVE_TRACE("X509_CRL_get0_by_cert(%p, %p) => %p", x509crl, x509, revoked);
    *result = reinterpret_cast<uintptr_t>(revoked);
    return NOERROR;
}

ECode NativeCrypto::X509_CRL_get0_by_serial(
    /* [in] */ Int64 x509crlRef,
    /* [in] */ ArrayOf<Byte>* serialArray,
    /* [out] */ Int64* result)
{
    VALIDATE_NOT_NULL(result);
    X509_CRL* x509crl = reinterpret_cast<X509_CRL*>(static_cast<uintptr_t>(x509crlRef));
    NATIVE_TRACE("X509_CRL_get0_by_serial(%p, %p)", x509crl, serialArray);

    if (x509crl == NULL) {
        NATIVE_TRACE("X509_CRL_get0_by_serial(%p, %p) => crl == null", x509crl, serialArray);
        *result = 0;
        return E_NULL_POINTER_EXCEPTION;
    }

    Unique_BIGNUM serialBn(BN_new());
    if (serialBn.get() == NULL) {
        NATIVE_TRACE("X509_CRL_get0_by_serial(%p, %p) => BN allocation failed", x509crl, serialArray);
        *result = 0;
        return NOERROR;
    }

    ECode ec = NOERROR;
    BIGNUM* serialBare = serialBn.get();
    if (!ArrayToBignum(serialArray, &serialBare, &ec)) {
        if (SUCCEEDED(ec)) ec = E_NULL_POINTER_EXCEPTION;
        NATIVE_TRACE("X509_CRL_get0_by_serial(%p, %p) => BN conversion failed", x509crl, serialArray);
        *result = 0;
        return ec;
    }

    Unique_ASN1_INTEGER serialInteger(BN_to_ASN1_INTEGER(serialBn.get(), NULL));
    if (serialInteger.get() == NULL) {
        NATIVE_TRACE("X509_CRL_get0_by_serial(%p, %p) => BN conversion failed", x509crl, serialArray);
        *result = 0;
        return NOERROR;
    }

    X509_REVOKED* revoked = NULL;
    int ret = ::X509_CRL_get0_by_serial(x509crl, &revoked, serialInteger.get());
    if (ret == 0) {
        NATIVE_TRACE("X509_CRL_get0_by_serial(%p, %p) => none", x509crl, serialArray);
        *result = 0;
        return NOERROR;
    }

    NATIVE_TRACE("X509_CRL_get0_by_cert(%p, %p) => %p", x509crl, serialArray, revoked);
    *result = reinterpret_cast<uintptr_t>(revoked);
    return NOERROR;
}

/* This appears to be missing from OpenSSL. */
#if !defined(X509_revoked_dup)
X509_REVOKED* X509_revoked_dup(X509_REVOKED* x)
{
    return reinterpret_cast<X509_REVOKED*>(ASN1_item_dup(ASN1_ITEM_rptr(X509_REVOKED), x));
}
#endif

ECode NativeCrypto::X509_CRL_get_REVOKED(
    /* [in] */ Int64 x509crlRef,
    /* [out, callee] */ ArrayOf<Int64>** result)
{
    VALIDATE_NOT_NULL(result);
    X509_CRL* crl = reinterpret_cast<X509_CRL*>(static_cast<uintptr_t>(x509crlRef));
    NATIVE_TRACE("X509_CRL_get_REVOKED(%p)", crl);

    if (crl == NULL) {
        *result = NULL;
        return E_NULL_POINTER_EXCEPTION;
    }

    STACK_OF(X509_REVOKED)* stack = ((crl)->crl->revoked);
    if (stack == NULL) {
        NATIVE_TRACE("X509_CRL_get_REVOKED(%p) => stack is null", crl);
        *result = NULL;
        return NOERROR;
    }

    size_t size = sk_X509_REVOKED_num(stack);

    AutoPtr< ArrayOf<Int64> > revokedArray = ArrayOf<Int64>::Alloc(size);
    for (size_t i = 0; i < size; i++) {
        X509_REVOKED* item = reinterpret_cast<X509_REVOKED*>(sk_X509_REVOKED_value(stack, i));
        (*revokedArray)[i] = reinterpret_cast<uintptr_t>(X509_revoked_dup(item));
    }

    NATIVE_TRACE("X509_CRL_get_REVOKED(%p) => %p [size=%d]", stack, revokedArray.Get(), size);
    *result = revokedArray;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

ECode NativeCrypto::Get_X509_CRL_ext_oids(
    /* [in] */ Int64 x509crlRef,
    /* [in] */ Int32 critical,
    /* [out, callee] */ ArrayOf<String>** result)
{
    VALIDATE_NOT_NULL(result);
    NATIVE_TRACE("get_X509_CRL_ext_oids(0x%llx, %d)", x509crlRef, critical);
    return get_X509Type_ext_oids<X509_CRL, X509_CRL_get_ext_by_critical, ::X509_CRL_get_ext>(
            x509crlRef, critical, result);
}

ECode NativeCrypto::X509_CRL_get_ext_oid(
    /* [in] */ Int64 x509crlRef,
    /* [in] */ const String& oid,
    /* [out, callee] */ ArrayOf<Byte>** result)
{
    VALIDATE_NOT_NULL(result);
    X509_CRL* crl = reinterpret_cast<X509_CRL*>(static_cast<uintptr_t>(x509crlRef));
    NATIVE_TRACE("X509_CRL_get_ext_oid(%p, %s)", crl, oid.string());
    return X509Type_get_ext_oid<X509_CRL, X509_CRL_get_ext_by_OBJ, ::X509_CRL_get_ext>(crl,
            oid, result);
}

ECode NativeCrypto::X509_CRL_get_version(
    /* [in] */ Int64 x509crlRef,
    /* [out] */ Int64* result)
{
    VALIDATE_NOT_NULL(result);
    X509_CRL* crl = reinterpret_cast<X509_CRL*>(static_cast<uintptr_t>(x509crlRef));
    NATIVE_TRACE("X509_CRL_get_version(%p)", crl);

    long version = ::ASN1_INTEGER_get((crl)->crl->version);
    NATIVE_TRACE("X509_CRL_get_version(%p) => %ld", crl, version);
    *result = version;
    return NOERROR;
}

ECode NativeCrypto::X509_CRL_get_ext(
    /* [in] */ Int64 x509crlRef,
    /* [in] */ const String& oid,
    /* [out] */ Int64* result)
{
    VALIDATE_NOT_NULL(result);
    X509_CRL* crl = reinterpret_cast<X509_CRL*>(static_cast<uintptr_t>(x509crlRef));
    NATIVE_TRACE("X509_CRL_get_ext(%p, %p)", crl, oid);
    ECode ec = NOERROR;
    X509_EXTENSION* ext = X509Type_get_ext<X509_CRL, X509_CRL_get_ext_by_OBJ, ::X509_CRL_get_ext>(
            crl, oid, &ec);
    NATIVE_TRACE("X509_CRL_get_ext(%p, %p) => %p", crl, oid, ext);
    *result = reinterpret_cast<uintptr_t>(ext);
    return ec;
}

static void get_X509_CRL_signature(X509_CRL *crl, ASN1_BIT_STRING** signature)
{
    *signature = crl->signature;
}

ECode NativeCrypto::Get_X509_CRL_signature(
    /* [in] */ Int64 x509crlRef,
    /* [out, callee] */ ArrayOf<Byte>** result)
{
    VALIDATE_NOT_NULL(result);
    X509_CRL* crl = reinterpret_cast<X509_CRL*>(static_cast<uintptr_t>(x509crlRef));
    NATIVE_TRACE("get_X509_CRL_signature(%p)", crl);
    return get_X509Type_signature<X509_CRL>(crl, get_X509_CRL_signature, result);
}

ECode NativeCrypto::X509_CRL_verify(
    /* [in] */ Int64 x509crlRef,
    /* [in] */ Int64 pkeyRef)
{
    X509_CRL* crl = reinterpret_cast<X509_CRL*>(static_cast<uintptr_t>(x509crlRef));
    EVP_PKEY* pkey = reinterpret_cast<EVP_PKEY*>(pkeyRef);
    NATIVE_TRACE("X509_CRL_verify(%p, %p)", crl, pkey);

    if (crl == NULL) {
        NATIVE_TRACE("X509_CRL_verify(%p, %p) => crl == null", crl, pkey);
        return E_NULL_POINTER_EXCEPTION;
    }

    if (pkey == NULL) {
        NATIVE_TRACE("X509_CRL_verify(%p, %p) => pkey == null", crl, pkey);
        return E_NULL_POINTER_EXCEPTION;;
    }

    if (::X509_CRL_verify(crl, pkey) != 1) {
        NATIVE_TRACE("X509_CRL_verify(%p, %p) => verify failure", crl, pkey);
        return ThrowExceptionIfNecessary("X509_CRL_verify");
    }
    else {
        NATIVE_TRACE("X509_CRL_verify(%p, %p) => verify success", crl, pkey);
        return NOERROR;
    }
}

ECode NativeCrypto::Get_X509_CRL_crl_enc(
    /* [in] */ Int64 x509crlRef,
    /* [out, callee] */ ArrayOf<Byte>** result)
{
    VALIDATE_NOT_NULL(result);
    X509_CRL* crl = reinterpret_cast<X509_CRL*>(static_cast<uintptr_t>(x509crlRef));
    NATIVE_TRACE("get_X509_CRL_crl_enc(%p)", crl);
    return ASN1ToByteArray<X509_CRL_INFO, i2d_X509_CRL_INFO>(crl->crl, result);
}

ECode NativeCrypto::X509_CRL_get_lastUpdate(
    /* [in] */ Int64 x509crlRef,
    /* [out] */ Int64* result)
{
    VALIDATE_NOT_NULL(result);
    X509_CRL* crl = reinterpret_cast<X509_CRL*>(static_cast<uintptr_t>(x509crlRef));
    NATIVE_TRACE("X509_CRL_get_lastUpdate(%p)", crl);

    if (crl == NULL) {
        NATIVE_TRACE("X509_CRL_get_lastUpdate(%p) => crl == null", crl);
        *result = 0;
        return E_NULL_POINTER_EXCEPTION;
    }

    ASN1_TIME* lastUpdate = ((crl)->crl->lastUpdate);
    NATIVE_TRACE("X509_CRL_get_lastUpdate(%p) => %p", crl, lastUpdate);
    *result = reinterpret_cast<uintptr_t>(lastUpdate);
    return NOERROR;
}

ECode NativeCrypto::X509_CRL_get_nextUpdate(
    /* [in] */ Int64 x509crlRef,
    /* [out] */ Int64* result)
{
    VALIDATE_NOT_NULL(result);
    X509_CRL* crl = reinterpret_cast<X509_CRL*>(static_cast<uintptr_t>(x509crlRef));
    NATIVE_TRACE("X509_CRL_get_nextUpdate(%p)", crl);

    if (crl == NULL) {
        NATIVE_TRACE("X509_CRL_get_nextUpdate(%p) => crl == null", crl);
        *result = 0;
        return E_NULL_POINTER_EXCEPTION;
    }

    ASN1_TIME* nextUpdate = ((crl)->crl->nextUpdate);
    NATIVE_TRACE("X509_CRL_get_nextUpdate(%p) => %p", crl, nextUpdate);
    *result = reinterpret_cast<uintptr_t>(nextUpdate);
    return NOERROR;
}

ECode NativeCrypto::X509_REVOKED_dup(
    /* [in] */ Int64 x509RevokedRef,
    /* [out] */ Int64* result)
{
    VALIDATE_NOT_NULL(result);
    X509_REVOKED* revoked = reinterpret_cast<X509_REVOKED*>(static_cast<uintptr_t>(x509RevokedRef));
    NATIVE_TRACE("X509_REVOKED_dup(%p)", revoked);

    if (revoked == NULL) {
        NATIVE_TRACE("X509_REVOKED_dup(%p) => revoked == null", revoked);
        *result = 0;
        return E_NULL_POINTER_EXCEPTION;
    }

    X509_REVOKED* dup = X509_revoked_dup(revoked);
    NATIVE_TRACE("X509_REVOKED_dup(%p) => %p", revoked, dup);
    *result = reinterpret_cast<uintptr_t>(dup);
    return NOERROR;
}

ECode NativeCrypto::I2d_X509_REVOKED(
    /* [in] */ Int64 x509RevokedRef,
    /* [out, callee] */ ArrayOf<Byte>** result)
{
    VALIDATE_NOT_NULL(result);
    X509_REVOKED* x509Revoked =
            reinterpret_cast<X509_REVOKED*>(static_cast<uintptr_t>(x509RevokedRef));
    NATIVE_TRACE("i2d_X509_REVOKED(%p)", x509Revoked);
    return ASN1ToByteArray<X509_REVOKED, i2d_X509_REVOKED>(x509Revoked, result);
}

ECode NativeCrypto::Get_X509_REVOKED_ext_oids(
    /* [in] */ Int64 x509RevokedRef,
    /* [in] */ Int32 critical,
    /* [out, callee] */ ArrayOf<String>** result)
{
    VALIDATE_NOT_NULL(result);
    NATIVE_TRACE("get_X509_CRL_ext_oids(0x%llx, %d)", x509RevokedRef, critical);
    return get_X509Type_ext_oids<X509_REVOKED, X509_REVOKED_get_ext_by_critical,
            ::X509_REVOKED_get_ext>(x509RevokedRef, critical, result);
}

ECode NativeCrypto::X509_REVOKED_get_ext_oid(
    /* [in] */ Int64 x509RevokedRef,
    /* [in] */ const String& oid,
    /* [out, callee] */ ArrayOf<Byte>** result)
{
    VALIDATE_NOT_NULL(result);
    X509_REVOKED* revoked = reinterpret_cast<X509_REVOKED*>(static_cast<uintptr_t>(x509RevokedRef));
    NATIVE_TRACE("X509_REVOKED_get_ext_oid(%p, %p)", revoked, oid);
    return X509Type_get_ext_oid<X509_REVOKED, X509_REVOKED_get_ext_by_OBJ, ::X509_REVOKED_get_ext>(
            revoked, oid, result);
}

/* OpenSSL includes set_serialNumber but not get. */
#if !defined(X509_revoked_get_serialNumber)
static ASN1_INTEGER* X509_revoked_get_serialNumber(X509_REVOKED* x) {
    return x->serialNumber;
}
#endif

ECode NativeCrypto::X509_REVOKED_get_serialNumber(
    /* [in] */ Int64 x509RevokedRef,
    /* [out, callee] */ ArrayOf<Byte>** result)
{
    VALIDATE_NOT_NULL(result);
    X509_REVOKED* revoked = reinterpret_cast<X509_REVOKED*>(static_cast<uintptr_t>(x509RevokedRef));
    NATIVE_TRACE("X509_REVOKED_get_serialNumber(%p)", revoked);
    return get_X509Type_serialNumber<X509_REVOKED>(revoked, X509_revoked_get_serialNumber, result);
}

ECode NativeCrypto::X509_REVOKED_get_ext(
    /* [in] */ Int64 x509RevokedRef,
    /* [in] */ const String& oid,
    /* [out] */ Int64* result)
{
    VALIDATE_NOT_NULL(result);
    X509_REVOKED* revoked = reinterpret_cast<X509_REVOKED*>(static_cast<uintptr_t>(x509RevokedRef));
    NATIVE_TRACE("X509_REVOKED_get_ext(%p, %p)", revoked, oid);
    ECode ec = NOERROR;
    X509_EXTENSION* ext = X509Type_get_ext<X509_REVOKED, X509_REVOKED_get_ext_by_OBJ,
            ::X509_REVOKED_get_ext>(revoked, oid, &ec);
    NATIVE_TRACE("X509_REVOKED_get_ext(%p, %p) => %p", revoked, oid, ext);
    *result = reinterpret_cast<uintptr_t>(ext);
    return ec;
}

ECode NativeCrypto::Get_X509_REVOKED_revocationDate(
    /* [in] */ Int64 x509RevokedRef,
    /* [out] */ Int64* result)
{
    VALIDATE_NOT_NULL(result);
    X509_REVOKED* revoked = reinterpret_cast<X509_REVOKED*>(static_cast<uintptr_t>(x509RevokedRef));
    NATIVE_TRACE("get_X509_REVOKED_revocationDate(%p)", revoked);

    if (revoked == NULL) {
        NATIVE_TRACE("get_X509_REVOKED_revocationDate(%p) => revoked == null", revoked);
        *result = 0;
        return E_NULL_POINTER_EXCEPTION;
    }

    NATIVE_TRACE("get_X509_REVOKED_revocationDate(%p) => %p", revoked, revoked->revocationDate);
    *result = reinterpret_cast<uintptr_t>(revoked->revocationDate);
    return NOERROR;
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wwrite-strings"
ECode NativeCrypto::X509_REVOKED_print(
    /* [in] */ Int64 bioRef,
    /* [in] */ Int64 x509RevokedRef)
{
    BIO* bio = reinterpret_cast<BIO*>(static_cast<uintptr_t>(bioRef));
    X509_REVOKED* revoked = reinterpret_cast<X509_REVOKED*>(static_cast<uintptr_t>(x509RevokedRef));
    NATIVE_TRACE("X509_REVOKED_print(%p, %p)", bio, revoked);

    if (bio == NULL) {
        NATIVE_TRACE("X509_REVOKED_print(%p, %p) => bio == null", bio, revoked);
        return E_NULL_POINTER_EXCEPTION;
    }

    if (revoked == NULL) {
        NATIVE_TRACE("X509_REVOKED_print(%p, %p) => revoked == null", bio, revoked);
        return E_NULL_POINTER_EXCEPTION;
    }

    BIO_printf(bio, "Serial Number: ");
    i2a_ASN1_INTEGER(bio, revoked->serialNumber);
    BIO_printf(bio, "\nRevocation Date: ");
    ASN1_TIME_print(bio, revoked->revocationDate);
    BIO_printf(bio, "\n");
    X509V3_extensions_print(bio, "CRL entry extensions", revoked->extensions, 0, 0);
    return NOERROR;
}
#pragma GCC diagnostic pop

ECode NativeCrypto::X509_supported_extension(
    /* [in] */ Int64 x509ExtensionRef,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    X509_EXTENSION* ext = reinterpret_cast<X509_EXTENSION*>(static_cast<uintptr_t>(x509ExtensionRef));

    if (ext == NULL) {
        *result = 0;
        return E_NULL_POINTER_EXCEPTION;
    }

    *result = ::X509_supported_extension(ext);
    return NOERROR;
}

static inline void get_ASN1_TIME_data(char **data, int* output, size_t len)
{
    char c = **data;
    **data = '\0';
    *data -= len;
    *output = atoi(*data);
    *(*data + len) = c;
}

ECode NativeCrypto::ASN1_TIME_to_Calendar(
    /* [in] */ Int64 asn1TimeRef,
    /* [in] */ ICalendar* cal)
{
    ASN1_TIME* asn1Time = reinterpret_cast<ASN1_TIME*>(static_cast<uintptr_t>(asn1TimeRef));
    NATIVE_TRACE("ASN1_TIME_to_Calendar(%p, %p)", asn1Time, calendar);

    if (asn1Time == NULL) {
        return E_NULL_POINTER_EXCEPTION;
    }

    Unique_ASN1_GENERALIZEDTIME gen(ASN1_TIME_to_generalizedtime(asn1Time, NULL));
    if (gen.get() == NULL) {
        return E_NULL_POINTER_EXCEPTION;
    }

    if (gen->length < 14 || gen->data == NULL) {
        return E_NULL_POINTER_EXCEPTION;
    }

    int sec, min, hour, mday, mon, year;

    char *p = (char*) &gen->data[14];

    get_ASN1_TIME_data(&p, &sec, 2);
    get_ASN1_TIME_data(&p, &min, 2);
    get_ASN1_TIME_data(&p, &hour, 2);
    get_ASN1_TIME_data(&p, &mday, 2);
    get_ASN1_TIME_data(&p, &mon, 2);
    get_ASN1_TIME_data(&p, &year, 4);

    cal->Set(year, mon - 1, mday, hour, min, sec);
    return NOERROR;
}

ECode NativeCrypto::Create_BIO_InputStream(
    /* [in] */ IOpenSSLBIOInputStream* streamObj,
    /* [out] */ Int64* result)
{
    VALIDATE_NOT_NULL(result);
    NATIVE_TRACE("create_BIO_InputStream(%p)", streamObj);

    if (streamObj == NULL) {
        *result = 0;
        return E_NULL_POINTER_EXCEPTION;
    }

    Unique_BIO bio(BIO_new(&stream_bio_method));
    if (bio.get() == NULL) {
        *result = 0;
        return NOERROR;
    }

    bio_stream_assign(bio.get(), new BIO_InputStream(streamObj));

    NATIVE_TRACE("create_BIO_InputStream(%p) => %p", streamObj, bio.get());
    *result = reinterpret_cast<uintptr_t>(bio.release());
    return NOERROR;
}

ECode NativeCrypto::Create_BIO_OutputStream(
    /* [in] */ IOutputStream* streamObj,
    /* [out] */ Int64* result)
{
    VALIDATE_NOT_NULL(result);
    NATIVE_TRACE("create_BIO_OutputStream(%p)", streamObj);

    if (streamObj == NULL) {
        *result = 0;
        return E_NULL_POINTER_EXCEPTION;
    }

    Unique_BIO bio(BIO_new(&stream_bio_method));
    if (bio.get() == NULL) {
        *result = 0;
        return NOERROR;
    }

    bio_stream_assign(bio.get(), new BIO_OutputStream(streamObj));

    NATIVE_TRACE("create_BIO_OutputStream(%p) => %p", streamObj, bio.get());
    *result = reinterpret_cast<uintptr_t>(bio.release());
    return NOERROR;
}

ECode NativeCrypto::BIO_read(
    /* [in] */ Int64 bioRef,
    /* [in] */ ArrayOf<Byte>* outputBytes,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    BIO* bio = reinterpret_cast<BIO*>(static_cast<uintptr_t>(bioRef));
    NATIVE_TRACE("BIO_read(%p, %p)", bio, outputBytes);

    if (outputBytes == NULL) {
        NATIVE_TRACE("BIO_read(%p, %p) => output == null", bio, outputBytes);
        *result = 0;
        return E_NULL_POINTER_EXCEPTION;
    }

    int outputSize = outputBytes->GetLength();

    UniquePtr<unsigned char[]> buffer(new unsigned char[outputSize]);
    if (buffer.get() == NULL) {
        *result = 0;
        return E_OUT_OF_MEMORY_ERROR;
    }

    int read = ::BIO_read(bio, buffer.get(), outputSize);
    if (read <= 0) {
        NATIVE_TRACE("BIO_read(%p, %p) => threw IO exception", bio, outputBytes);
        *result = 0;
        return E_IO_EXCEPTION;
    }

    outputBytes->Copy(reinterpret_cast<Byte*>(buffer.get()), read);
    NATIVE_TRACE("BIO_read(%p, %p) => %d", bio, outBytes, read);
    *result = read;
    return NOERROR;
}

ECode NativeCrypto::BIO_write(
    /* [in] */ Int64 bioRef,
    /* [in] */ ArrayOf<Byte>* inputBytes,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length)
{
    BIO* bio = reinterpret_cast<BIO*>(static_cast<uintptr_t>(bioRef));
    NATIVE_TRACE("BIO_write(%p, %p, %d, %d)", bio, inputBytes, offset, length);

    if (inputBytes == NULL) {
        return E_NULL_POINTER_EXCEPTION;
    }

    if (offset < 0 || length < 0) {
        NATIVE_TRACE("BIO_write(%p, %p, %d, %d) => IOOB", bio, inputBytes, offset, length);
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    int inputSize = inputBytes->GetLength();
    if (inputSize < offset + length) {
        NATIVE_TRACE("BIO_write(%p, %p, %d, %d) => IOOB", bio, inputBytes, offset, length);
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    UniquePtr<unsigned char[]> buffer(new unsigned char[length]);
    if (buffer.get() == NULL) {
        return E_OUT_OF_MEMORY_ERROR;
    }

    memcpy(buffer.get(), inputBytes->GetPayload() + offset, length);
    if (::BIO_write(bio, buffer.get(), length) != length) {
        FreeOpenSslErrorState();
        NATIVE_TRACE("BIO_write(%p, %p, %d, %d) => IO error", bio, inputBytes, offset, length);
        return E_IO_EXCEPTION;
    }

    NATIVE_TRACE("BIO_write(%p, %p, %d, %d) => success", bio, inputBytes, offset, length);
    return NOERROR;
}

ECode NativeCrypto::BIO_free_all(
    /* [in] */ Int64 bioRef)
{
    BIO* bio = reinterpret_cast<BIO*>(static_cast<uintptr_t>(bioRef));
    NATIVE_TRACE("BIO_free_all(%p)", bio);

    if (bio == NULL) {
        return E_NULL_POINTER_EXCEPTION;
    }

    ::BIO_free_all(bio);
    return NOERROR;
}

} // namespace Conscrypt
} // namespace Org
