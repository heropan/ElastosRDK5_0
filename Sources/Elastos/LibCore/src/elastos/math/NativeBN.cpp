#include <NativeBN.h>
#include <cmdef.h>
#include <elastos/UniquePtr.h>
#include <openssl/bn.h>
#include <openssl/crypto.h>
#include <openssl/err.h>

namespace Elastos {
namespace Math {

using Elastos::Core::UniquePtr;

struct BN_CTX_Deleter {
    void operator()(BN_CTX* p) const {
        BN_CTX_free(p);
    }
};
typedef UniquePtr<BN_CTX, BN_CTX_Deleter> Unique_BN_CTX;

Int32 NativeBN::ERR_get_error()
{
    return ::ERR_get_error();
}

String NativeBN::ERR_error_string(Int32 e)
{
    char* errStr = ::ERR_error_string(e, NULL);
    return String(errStr);
}

Int32 NativeBN::BN_new()
{
    return (Int32)::BN_new();
}

void NativeBN::BN_free(Int32 ah)
{
    BIGNUM* a = reinterpret_cast<BIGNUM*>(ah);
    return ::BN_free(a);
}

Int32 NativeBN::BN_cmp(Int32 ah, Int32 bh)
{
    BIGNUM* a = reinterpret_cast<BIGNUM*>(ah);
    BIGNUM* b = reinterpret_cast<BIGNUM*>(bh);
    return ::BN_cmp(a, b);
}

Boolean NativeBN::BN_copy(Int32 toh, Int32 fromh)
{
    BIGNUM* to = reinterpret_cast<BIGNUM*>(toh);
    BIGNUM* from = reinterpret_cast<BIGNUM*>(fromh);
    return (NULL != ::BN_copy(to, from));
}

Boolean NativeBN::PutLongInt(Int32 a, Int64 dw)
{
    if (dw >= 0)
        return PutULongInt(a, dw, FALSE);
    else
        return PutULongInt(a, -dw, TRUE);
}

Boolean NativeBN::PutULongInt(Int32 ah, Int64 dw, Boolean neg)
{
    UInt32 hi = dw >> 32; // This shifts without sign extension.
    Int32 lo = (Int32)dw; // This truncates implicitely.

    BIGNUM* a = reinterpret_cast<BIGNUM*>(ah);

    // cf. litEndInts2bn:
    bn_check_top(a);
    if (bn_wexpand(a, 2) != NULL) {
        a->d[0] = lo;
        a->d[1] = hi;
        a->top = 2;
        a->neg = neg;
        bn_correct_top(a);
        return TRUE;
    }

    return FALSE;
}

Int32 NativeBN::BN_dec2bn(Int32 ah, const String& str)
{
    if (str.IsNull())
        return -1;

    BIGNUM* a = reinterpret_cast<BIGNUM*>(ah);
    return ::BN_dec2bn(&a, str);
}

Int32 NativeBN::BN_hex2bn(Int32 ah, const String& str)
{
    if (str.IsNull())
        return -1;

    BIGNUM* a = reinterpret_cast<BIGNUM*>(ah);
    return ::BN_hex2bn(&a, str);
}

Boolean NativeBN::BN_bin2bn(const ArrayOf<Byte>& bytes, Boolean neg, Int32 reth)
{
    Int32 len = bytes.GetLength();
    if (len == 0) {
        return FALSE;
    }

    BIGNUM* ret = reinterpret_cast<BIGNUM*>(reth);
    Boolean success = (::BN_bin2bn(reinterpret_cast<const unsigned char*>(bytes.GetPayload()), len, ret) != NULL);
    if (success && neg) {
        ::BN_set_negative(ret, 1);
    }
    return success;
}

Boolean NativeBN::LitEndInts2bn(const ArrayOf<Int32>& ints, Boolean neg, Int32 reth)
{
    Int32 len = ints.GetLength();
    BIGNUM* ret = reinterpret_cast<BIGNUM*>(reth);
    bn_check_top(ret);
    if (len > 0) {
        //STATIC_ASSERT(sizeof(BN_ULONG) == sizeof(jint), BN_ULONG_not_32_bit);
        const BN_ULONG* tmpInts = reinterpret_cast<const BN_ULONG*>(ints.GetPayload());
        if ((tmpInts != NULL) && (bn_wexpand(ret, len) != NULL)) {
            Int32 i = len; do { i--; ret->d[i] = tmpInts[i]; } while (i > 0);
            ret->top = len;
            ret->neg = neg;
            // need to call this due to clear byte at top if avoiding
            // having the top bit set (-ve number)
            // Basically get rid of top zero ints:
            bn_correct_top(ret);
            return TRUE;
        }
        else {
            return FALSE;
        }
    }
    else { // (len = 0) means value = 0 and sign will be 0, too.
        ret->top = 0;
        return TRUE;
    }
}

Boolean NativeBN::TwosComp2bn(const ArrayOf<Byte>& bytes, Int32 reth)
{
    Int32 bytesLen = bytes.GetLength();
    BIGNUM* ret = reinterpret_cast<BIGNUM*>(reth);
    if (bytesLen == 0) {
        return FALSE;
    }

    Boolean success;
    const unsigned char* s = reinterpret_cast<const unsigned char*>(bytes.GetPayload());
    if ((bytes[0] & 0X80) == 0) { // Positive value!
        //
        // We can use the existing BN implementation for unsigned big endian bytes:
        //
        success = (::BN_bin2bn(s, bytesLen, ret) != NULL);
        ::BN_set_negative(ret, FALSE);
    }
    else { // Negative value!
        //
        // We need to apply two's complement:
        //
        success = negBigEndianBytes2bn(s, bytesLen, reth);
        ::BN_set_negative(ret, TRUE);
    }
    return success;
}

Int64 NativeBN::LongInt(Int32 ah)
{
    BIGNUM* a = reinterpret_cast<BIGNUM*>(ah);
    bn_check_top(a);
    Int32 intLen = a->top;
    BN_ULONG* d = a->d;
    switch (intLen) {
    case 0:
        return 0;
    case 1:
        if (!a->neg) return d[0] & 0X00000000FFFFFFFFLL;
        else return -(d[0] & 0X00000000FFFFFFFFLL);
    default:
        if (!a->neg) return ((long long)d[1] << 32) | (d[0] & 0XFFFFFFFFLL);
        else return -(((long long)d[1] << 32) | (d[0] & 0XFFFFFFFFLL));
    }
}

char* NativeBN::leadingZerosTrimmed(char* s)
{
    char* p = s;
    if (*p == '-') {
        p++;
        while ((*p == '0') && (*(p + 1) != 0)) { p++; }
        p--;
        *p = '-';
    } else {
        while ((*p == '0') && (*(p + 1) != 0)) { p++; }
    }
    return p;
}

String NativeBN::BN_bn2dec(Int32 ah)
{
    BIGNUM* a = reinterpret_cast<BIGNUM*>(ah);
    char* tmpStr;
    char* retStr;
    tmpStr = ::BN_bn2dec(a);
    if (tmpStr != NULL) {
        retStr = leadingZerosTrimmed(tmpStr);
        String returnJString(retStr);
        OPENSSL_free(tmpStr);
        return returnJString;
    }

    return String();
}

String NativeBN::BN_bn2h(Int32 ah)
{
    BIGNUM* a = reinterpret_cast<BIGNUM*>(ah);
    char* tmpStr;
    char* retStr;
    tmpStr = ::BN_bn2h(a);
    if (tmpStr != NULL) {
        retStr = leadingZerosTrimmed(tmpStr);
        String returnJString(retStr);
        OPENSSL_free(tmpStr);
        return returnJString;
    }

    return String();
}

AutoPtr<ArrayOf<Byte> > NativeBN::BN_bn2bin(Int32 ah)
{
    BIGNUM* a = reinterpret_cast<BIGNUM*>(ah);
    AutoPtr<ArrayOf<Byte> > result = NULL;
    Int32 size = BN_num_bytes(a);
    if (size <= 0) {
        return result;
    }

    result = ArrayOf<Byte>::Alloc(size);
    ::BN_bn2bin(a, reinterpret_cast<unsigned char*>(result->GetPayload()));
    return result;
}

AutoPtr<ArrayOf<Int32> > NativeBN::Bn2litEndInts(Int32 ah)
{
    BIGNUM* a = reinterpret_cast<BIGNUM*>(ah);
    AutoPtr<ArrayOf<Int32> > result = NULL;

    bn_check_top(a);
    Int32 len = a->top;
    if (len == 0) {
        return result;
    }

    result = ArrayOf<Int32>::Alloc(len);
    BN_ULONG* ulongs = reinterpret_cast<BN_ULONG*>(result->GetPayload());
    if (ulongs == NULL) {
        return result;
    }

    Int32 i = len; do { i--; ulongs[i] = a->d[i]; } while (i > 0);
    return result;
}

// Returns -1, 0, 1 AND NOT Boolean.
// #define NativeBN::BN_is_negative(a) ((a)->neg != 0)
Int32 NativeBN::Sign(Int32 ah)
{
    BIGNUM* a = reinterpret_cast<BIGNUM*>(ah);
    if (BN_is_zero(a)) return 0;
    else if (BN_is_negative(a)) return -1;
    else return 1;
}

void NativeBN::BN_set_negative(Int32 ah, Int32 n)
{
    BIGNUM* a = reinterpret_cast<BIGNUM*>(ah);
    ::BN_set_negative(a, n);
}

Int32 NativeBN::BitLength(Int32 ah)
{
    BIGNUM* a = reinterpret_cast<BIGNUM*>(ah);
    bn_check_top(a);
    Int32 intLen = a->top;
    if (intLen == 0) return 0;
    BN_ULONG* d = a->d;
    Int32 i = intLen - 1;
    BN_ULONG msd = d[i]; // most significant digit
    if (a->neg) {
        // Handle negative values correctly:
        // i.e. decrement the msd if all other digits are 0:
        // while ((i > 0) && (d[i] != 0)) { i--; }
        do { i--; } while (!((i < 0) || (d[i] != 0)));
        if (i < 0) msd--; // Only if all lower significant digits are 0 we decrement the most significant one.
    }
    return (intLen - 1) * 32 + ::BN_num_bits_word(msd);
}

Boolean NativeBN::BN_is_bit_set(Int32 ah, Int32 n)
{
    BIGNUM* a = reinterpret_cast<BIGNUM*>(ah);
    return ::BN_is_bit_set(a, n);
}

// Returns Boolean success.
// op: 0 = reset;1 = set; -1 = flip
// uses NativeBN::BN_set_bit(), NativeBN::BN_clear_bit() and NativeBN::BN_is_bit_set()
Boolean NativeBN::ModifyBit(Int32 ah, Int32 n, Int32 op)
{
    BIGNUM* a = reinterpret_cast<BIGNUM*>(ah);
    switch (op) {
    case 1: return ::BN_set_bit(a, n);
    case 0: return ::BN_clear_bit(a, n);
    case -1:
        if (::BN_is_bit_set(a, n)) return ::BN_clear_bit(a, n);
        else return ::BN_set_bit(a, n);
    }
    return FALSE;
}

Boolean NativeBN::BN_shift(Int32 rh, Int32 ah, Int32 n)
{
    BIGNUM* a = reinterpret_cast<BIGNUM*>(ah);
    BIGNUM* r = reinterpret_cast<BIGNUM*>(rh);
    return (n >= 0) ? ::BN_lshift(r, a, n) : ::BN_rshift(r, a, -n);
}

Boolean NativeBN::BN_add_word(Int32 ah, Int32 w)
{
    BIGNUM* a = reinterpret_cast<BIGNUM*>(ah);
    return ::BN_add_word(a, w);
}

Boolean NativeBN::BN_sub_word(Int32 ah, Int32 w)
{
    BIGNUM* a = reinterpret_cast<BIGNUM*>(ah);
    return ::BN_sub_word(a, w);
}

Boolean  NativeBN::BN_mul_word(Int32 ah, Int32 w)
{
    BIGNUM* a = reinterpret_cast<BIGNUM*>(ah);
    return ::BN_mul_word(a, w);
}

Int32 NativeBN::BN_div_word(Int32 ah, Int32 w)
{
    BIGNUM* a = reinterpret_cast<BIGNUM*>(ah);
    return ::BN_div_word(a, w);
}

Int32 NativeBN::BN_mod_word(Int32 ah, Int32 w)
{
    BIGNUM* a = reinterpret_cast<BIGNUM*>(ah);
    return ::BN_mod_word(a, w);
}

Boolean NativeBN::BN_add(Int32 rh, Int32 ah, Int32 bh)
{
    BIGNUM* a = reinterpret_cast<BIGNUM*>(ah);
    BIGNUM* b = reinterpret_cast<BIGNUM*>(bh);
    BIGNUM* r = reinterpret_cast<BIGNUM*>(rh);
    return ::BN_add(r, a, b);
}

Boolean NativeBN::BN_sub(Int32 rh, Int32 ah, Int32 bh)
{
    BIGNUM* a = reinterpret_cast<BIGNUM*>(ah);
    BIGNUM* b = reinterpret_cast<BIGNUM*>(bh);
    BIGNUM* r = reinterpret_cast<BIGNUM*>(rh);
    return ::BN_sub(r, a, b);
}

Boolean NativeBN::BN_gcd(Int32 rh, Int32 ah, Int32 bh)
{
    BIGNUM* a = reinterpret_cast<BIGNUM*>(ah);
    BIGNUM* b = reinterpret_cast<BIGNUM*>(bh);
    BIGNUM* r = reinterpret_cast<BIGNUM*>(rh);
    Unique_BN_CTX ctx(BN_CTX_new());
    return ::BN_gcd(r, a, b, ctx.get());
}

Boolean NativeBN::BN_mul(Int32 rh, Int32 ah, Int32 bh)
{
    BIGNUM* a = reinterpret_cast<BIGNUM*>(ah);
    BIGNUM* b = reinterpret_cast<BIGNUM*>(bh);
    BIGNUM* r = reinterpret_cast<BIGNUM*>(rh);
    Unique_BN_CTX ctx(BN_CTX_new());
    return ::BN_mul(r, a, b, ctx.get());
}

Boolean NativeBN::BN_exp(Int32 rh, Int32 ah, Int32 ph)
{
    BIGNUM* a = reinterpret_cast<BIGNUM*>(ah);
    BIGNUM* p = reinterpret_cast<BIGNUM*>(ph);
    BIGNUM* r = reinterpret_cast<BIGNUM*>(rh);
    Unique_BN_CTX ctx(BN_CTX_new());
    return ::BN_exp(r, a, p, ctx.get());
}

Boolean NativeBN::BN_div(Int32 dvh, Int32 remh, Int32 mh, Int32 dh)
{
    BIGNUM* dv = reinterpret_cast<BIGNUM*>(dvh);
    BIGNUM* rem = reinterpret_cast<BIGNUM*>(remh);
    BIGNUM* m = reinterpret_cast<BIGNUM*>(mh);
    BIGNUM* d = reinterpret_cast<BIGNUM*>(dh);
    Unique_BN_CTX ctx(BN_CTX_new());
    return ::BN_div(dv, rem, m, d, ctx.get());
}

Boolean NativeBN::BN_nnmod(Int32 rh, Int32 ah, Int32 mh)
{
    BIGNUM* a = reinterpret_cast<BIGNUM*>(ah);
    BIGNUM* m = reinterpret_cast<BIGNUM*>(mh);
    BIGNUM* r = reinterpret_cast<BIGNUM*>(rh);
    Unique_BN_CTX ctx(BN_CTX_new());
    return ::BN_nnmod(r, a, m, ctx.get());
}

Boolean NativeBN::BN_mod_exp(Int32 rh, Int32 ah, Int32 ph, Int32 mh)
{
    BIGNUM* a = reinterpret_cast<BIGNUM*>(ah);
    BIGNUM* m = reinterpret_cast<BIGNUM*>(mh);
    BIGNUM* p = reinterpret_cast<BIGNUM*>(ph);
    BIGNUM* r = reinterpret_cast<BIGNUM*>(rh);
    Unique_BN_CTX ctx(BN_CTX_new());
    return ::BN_mod_exp(r, a, p, m, ctx.get());
}

Boolean NativeBN::BN_mod_inverse(Int32 reth, Int32 ah, Int32 nh)
{
    BIGNUM* a = reinterpret_cast<BIGNUM*>(ah);
    BIGNUM* n = reinterpret_cast<BIGNUM*>(nh);
    BIGNUM* ret = reinterpret_cast<BIGNUM*>(reth);
    Unique_BN_CTX ctx(BN_CTX_new());
    return (::BN_mod_inverse(ret, a, n, ctx.get()) != NULL);
}

Boolean NativeBN::BN_generate_prime_(
            Int32 reth, Int32 bits, Boolean safe,
            Int32 addh, Int32 remh, Int32 cb)
{
    BIGNUM* ret = reinterpret_cast<BIGNUM*>(reth);
    BIGNUM* add = reinterpret_cast<BIGNUM*>(addh);
    BIGNUM* rem = reinterpret_cast<BIGNUM*>(remh);
    return ::BN_generate_prime_(ret, bits, safe, add, rem, reinterpret_cast<BN_GENCB*>(cb));
}

Boolean NativeBN::BN_is_prime_(Int32 ph, Int32 nchecks, Int32 cb)
{
    BIGNUM* p = reinterpret_cast<BIGNUM*>(ph);
    Unique_BN_CTX ctx(BN_CTX_new());
    return ::BN_is_prime_(p, nchecks, ctx.get(), reinterpret_cast<BN_GENCB*>(cb));
}

#define BYTES2INT(bytes, k) \
 (  (bytes[k + 3] & 0xFF) \
  | (bytes[k + 2] & 0xFF) << 8 \
  | (bytes[k + 1] & 0xFF) << 16 \
  | (bytes[k + 0] & 0xFF) << 24 )

Boolean NativeBN::negBigEndianBytes2bn(const unsigned char* bytes, Int32 bytesLen, Int32 reth)
{
    BIGNUM* ret = reinterpret_cast<BIGNUM*>(reth);
    // We rely on: (BN_BITS2 == 32), i.e. BN_ULONG is unsigned Int32 and has 4 bytes:
    bn_check_top(ret);
    // FIXME: assert bytesLen > 0
    Int32 intLen = (bytesLen + 3) / 4;
    Int32 firstNonzeroDigit = -2;
    if (bn_wexpand(ret, intLen) != NULL) {
        BN_ULONG* d = ret->d;
        BN_ULONG di;
        ret->top = intLen;
        Int32 highBytes = bytesLen % 4;
        Int32 k = bytesLen;
        // Put bytes to the int array starting from the end of the byte array
        Int32 i = 0;
        while (k > highBytes) {
            k -= 4;
            di = BYTES2INT(bytes, k);
            if (di != 0) {
                d[i] = -di;
                firstNonzeroDigit = i;
                i++;
                while (k > highBytes) {
                    k -= 4;
                    d[i] = ~BYTES2INT(bytes, k);
                    i++;
                }
                break;
            } else {
                d[i] = 0;
                i++;
            }
        }
        if (highBytes != 0) {
            di = -1;
            // Put the first bytes in the highest element of the int array
            if (firstNonzeroDigit != -2) {
                for (k = 0; k < highBytes; k++) {
                    di = (di << 8) | (bytes[k] & 0xFF);
                }
                d[i] = ~di;
            } else {
                for (k = 0; k < highBytes; k++) {
                    di = (di << 8) | (bytes[k] & 0xFF);
                }
                d[i] = -di;
            }
        }
        return TRUE;
    }
    else
        return FALSE;
}

} // namespace Math
} // namespace Elastos
