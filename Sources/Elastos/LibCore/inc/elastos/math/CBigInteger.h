#ifndef __MATH_CBIGINTEGER_H__
#define __MATH_CBIGINTEGER_H__

#include "__Elastos_Math__CBigInteger.h"
#include <elstring.h>
#include <elastos/Mutex.h>
#include <BigInt.h>
#include "Object.h"

using Elastos::Core::Threading::Mutex;

namespace Elastos {
namespace Math {

class BitLevel;
class Logical;
class Primality;
class Conversion;
class Multiplication;

CarClass(CBigInteger)
    : public Object
    , public IBigInteger
    , public INumber
    , public IComparable
{
public:
    CBigInteger();

    CAR_OBJECT_DECL()

    CAR_INTERFACE_DECL()

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ Int32 sign,
        /* [in] */ Int32 numberLength,
        /* [in] */ const ArrayOf<Int32>& digits);

    CARAPI constructor(
        /* [in] */ Int32 sign,
        /* [in] */ Int64 value);

    CARAPI constructor(
        /* [in] */ const String& decStr);

    CARAPI constructor(
        /* [in] */ const String& decStr,
        /* [in] */ Int32 radix);

    CARAPI constructor(
        /* [in] */ Int32 signum,
        /* [in] */ const ArrayOf<Byte>& magnitude);

    CARAPI constructor(
        /* [in] */ const ArrayOf<Byte>& value);

    /** Returns a {@code BigInteger} whose value is equal to {@code value}. */
    static CARAPI ValueOf(
        /* [in] */ Int64 value,
        /* [out] */ IBigInteger** result);

    /**
     * Returns the two's complement representation of this {@code BigInteger} in
     * a byte array.
     */
    CARAPI ToByteArray(
        /* [out, callee] */ ArrayOf<Byte>** bytes);

    /**
     * Returns a {@code BigInteger} whose value is the absolute value of {@code
     * this}.
     */
    CARAPI Abs(
        /* [out] */ IBigInteger** result);

    /**
     * Returns a {@code BigInteger} whose value is the {@code -this}.
     */
    CARAPI Negate(
        /* [out] */ IBigInteger** result);

    /**
     * Returns a {@code BigInteger} whose value is {@code this + value}.
     */
    CARAPI Add(
        /* [in] */ IBigInteger* value,
        /* [out] */ IBigInteger** result);

    /**
     * Returns a {@code BigInteger} whose value is {@code this - value}.
     */
    CARAPI Subtract(
        /* [in] */ IBigInteger* value,
        /* [out] */ IBigInteger** result);

    /**
     * Returns the sign of this {@code BigInteger}.
     *
     * @return {@code -1} if {@code this < 0}, {@code 0} if {@code this == 0},
     *     {@code 1} if {@code this > 0}.
     */
    CARAPI GetSignum(
        /* [out] */ Int32* sign);

    /**
     * Returns a {@code BigInteger} whose value is {@code this >> n}. For
     * negative arguments, the result is also negative. The shift distance may
     * be negative which means that {@code this} is shifted left.
     *
     * <p><b>Implementation Note:</b> Usage of this method on negative values is
     * not recommended as the current implementation is not efficient.
     *
     * @param n shift distance
     * @return {@code this >> n} if {@code n >= 0}; {@code this << (-n)}
     *     otherwise
     */
    CARAPI ShiftRight(
        /* [in] */ Int32 n,
        /* [out] */ IBigInteger** result);

    /**
     * Returns a {@code BigInteger} whose value is {@code this << n}. The
     * result is equivalent to {@code this * pow(2, n)} if n >= 0. The shift
     * distance may be negative which means that {@code this} is shifted right.
     * The result then corresponds to {@code floor(this / pow(2, -n))}.
     *
     * <p><b>Implementation Note:</b> Usage of this method on negative values is
     * not recommended as the current implementation is not efficient.
     *
     * @param n shift distance.
     * @return {@code this << n} if {@code n >= 0}; {@code this >> (-n)}.
     *     otherwise
     */
    CARAPI ShiftLeft(
        /* [in] */ Int32 n,
        /* [out] */ IBigInteger** result);

    CARAPI ShiftLeftOneBit(
        /* [out] */ IBigInteger** result);

    /**
     * Returns the length of the value's two's complement representation without
     * leading zeros for positive numbers / without leading ones for negative
     * values.
     *
     * <p>The two's complement representation of {@code this} will be at least
     * {@code bitLength() + 1} bits long.
     *
     * <p>The value will fit into an {@code Int32} if {@code bitLength() < 32} or
     * into a {@code long} if {@code bitLength() < 64}.
     *
     * @return the length of the minimal two's complement representation for
     *     {@code this} without the sign bit.
     */
    CARAPI BitLength(
        /* [out] */ Int32* length);

    /**
     * Tests whether the bit at position n in {@code this} is set. The result is
     * equivalent to {@code this & pow(2, n) != 0}.
     *
     * <p><b>Implementation Note:</b> Usage of this method is not recommended as
     * the current implementation is not efficient.
     *
     * @param n position where the bit in {@code this} has to be inspected.
     * @throws ArithmeticException if {@code n < 0}.
     */
    CARAPI TestBit(
        /* [in] */ Int32 n,
        /* [out] */ Boolean* result);

    /**
     * Returns a {@code BigInteger} which has the same binary representation
     * as {@code this} but with the bit at position n set. The result is
     * equivalent to {@code this | pow(2, n)}.
     *
     * <p><b>Implementation Note:</b> Usage of this method is not recommended as
     * the current implementation is not efficient.
     *
     * @param n position where the bit in {@code this} has to be set.
     * @throws ArithmeticException if {@code n < 0}.
     */
    CARAPI SetBit(
        /* [in] */ Int32 n,
        /* [out] */ IBigInteger** result);

    /**
     * Returns a {@code BigInteger} which has the same binary representation
     * as {@code this} but with the bit at position n cleared. The result is
     * equivalent to {@code this & ~pow(2, n)}.
     *
     * <p><b>Implementation Note:</b> Usage of this method is not recommended as
     * the current implementation is not efficient.
     *
     * @param n position where the bit in {@code this} has to be cleared.
     * @throws ArithmeticException if {@code n < 0}.
     */
    CARAPI ClearBit(
        /* [in] */ Int32 n,
        /* [out] */ IBigInteger** result);

    /**
     * Returns a {@code BigInteger} which has the same binary representation
     * as {@code this} but with the bit at position n flipped. The result is
     * equivalent to {@code this ^ pow(2, n)}.
     *
     * <p><b>Implementation Note:</b> Usage of this method is not recommended as
     * the current implementation is not efficient.
     *
     * @param n position where the bit in {@code this} has to be flipped.
     * @throws ArithmeticException if {@code n < 0}.
     */
    CARAPI FlipBit(
        /* [in] */ Int32 n,
        /* [out] */ IBigInteger** result);

    /**
     * Returns the position of the lowest set bit in the two's complement
     * representation of this {@code BigInteger}. If all bits are zero (this==0)
     * then -1 is returned as result.
     *
     * <p><b>Implementation Note:</b> Usage of this method is not recommended as
     * the current implementation is not efficient.
     */
    CARAPI GetLowestSetBit(
        /* [out] */ Int32* position);

    /**
     * Returns the number of bits in the two's complement representation of
     * {@code this} which differ from the sign bit. If {@code this} is negative,
     * the result is equivalent to the number of bits set in the two's
     * complement representation of {@code -this - 1}.
     *
     * <p>Use {@code bitLength(0)} to find the length of the binary value in
     * bits.
     *
     * <p><b>Implementation Note:</b> Usage of this method is not recommended as
     * the current implementation is not efficient.
     */
    CARAPI BitCount(
        /* [out] */ Int32* number);

    /**
     * Returns a {@code BigInteger} whose value is {@code ~this}. The result
     * of this operation is {@code -this-1}.
     *
     * <p><b>Implementation Note:</b> Usage of this method is not recommended as
     * the current implementation is not efficient.
     */
    CARAPI Not(
        /* [out] */ IBigInteger** result);

    /**
     * Returns a {@code BigInteger} whose value is {@code this & value}.
     *
     * <p><b>Implementation Note:</b> Usage of this method is not recommended
     * as the current implementation is not efficient.
     *
     * @param value value to be and'ed with {@code this}.
     * @throws NullPointerException if {@code value == null}.
     */
    CARAPI And(
        /* [in] */ IBigInteger* value,
        /* [out] */ IBigInteger** result);

    /**
     * Returns a {@code BigInteger} whose value is {@code this | value}.
     *
     * <p><b>Implementation Note:</b> Usage of this method is not recommended as
     * the current implementation is not efficient.
     *
     * @param value value to be or'ed with {@code this}.
     * @throws NullPointerException if {@code value == null}.
     */
    CARAPI Or(
        /* [in] */ IBigInteger* value,
        /* [out] */ IBigInteger** result);

    /**
     * Returns a {@code BigInteger} whose value is {@code this ^ value}.
     *
     * <p><b>Implementation Note:</b> Usage of this method is not recommended as
     * the current implementation is not efficient.
     *
     * @param value value to be xor'ed with {@code this}
     * @throws NullPointerException if {@code value == null}
     */
    CARAPI Xor(
        /* [in] */ IBigInteger* value,
        /* [out] */ IBigInteger** result);

    /**
     * Returns a {@code BigInteger} whose value is {@code this & ~value}.
     * Evaluating {@code x.andNot(value)} returns the same result as {@code
     * x.and(value.not())}.
     *
     * <p><b>Implementation Note:</b> Usage of this method is not recommended
     * as the current implementation is not efficient.
     *
     * @param value value to be not'ed and then and'ed with {@code this}.
     * @throws NullPointerException if {@code value == null}.
     */
    CARAPI AndNot(
        /* [in] */ IBigInteger* value,
        /* [out] */ IBigInteger** result);

    /**
     * Compares this {@code BigInteger} with {@code value}. Returns {@code -1}
     * if {@code this < value}, {@code 0} if {@code this == value} and {@code 1}
     * if {@code this > value}, .
     *
     * @param value value to be compared with {@code this}.
     * @throws NullPointerException if {@code value == null}.
     */
    CARAPI CompareTo(
        /* [in] */ IInterface* other,
        /* [out] */ Int32* result);

    /**
     * Returns the minimum of this {@code BigInteger} and {@code value}.
     *
     * @param value value to be used to compute the minimum with {@code this}.
     * @throws NullPointerException if {@code value == null}.
     */
    CARAPI Min(
        /* [in] */ IBigInteger* value,
        /* [out] */ IBigInteger** result);

    /**
     * Returns the maximum of this {@code BigInteger} and {@code value}.
     *
     * @param value value to be used to compute the maximum with {@code this}
     * @throws NullPointerException if {@code value == null}
     */
    CARAPI Max(
        /* [in] */ IBigInteger* value,
        /* [out] */ IBigInteger** result);

    /**
     * Returns a {@code BigInteger} whose value is greatest common divisor
     * of {@code this} and {@code value}. If {@code this == 0} and {@code
     * value == 0} then zero is returned, otherwise the result is positive.
     *
     * @param value value with which the greatest common divisor is computed.
     * @throws NullPointerException if {@code value == null}.
     */
    CARAPI Gcd(
        /* [in] */ IBigInteger* value,
        /* [out] */ IBigInteger** result);

    /**
     * Returns a {@code BigInteger} whose value is {@code this * value}.
     *
     * @throws NullPointerException if {@code value == null}.
     */
    CARAPI Multiply(
        /* [in] */ IBigInteger* value,
        /* [out] */ IBigInteger** result);

    /**
     * Returns a {@code BigInteger} whose value is {@code pow(this, exp)}.
     *
     * @throws ArithmeticException if {@code exp < 0}.
     */
    CARAPI Pow(
        /* [in] */ Int32 exp,
        /* [out] */ IBigInteger** result);

    /**
     * Returns a two element {@code BigInteger} array containing
     * {@code this / divisor} at index 0 and {@code this % divisor} at index 1.
     *
     * @param divisor value by which {@code this} is divided.
     * @throws NullPointerException if {@code divisor == null}.
     * @throws ArithmeticException if {@code divisor == 0}.
     * @see #divide
     * @see #remainder
     */
    CARAPI DivideAndRemainder(
       /* [in] */ IBigInteger* divisor,
       /* [out, callee] */ ArrayOf<IBigInteger*>** bigIntegerArray);

    /**
     * Returns a {@code BigInteger} whose value is {@code this / divisor}.
     *
     * @param divisor value by which {@code this} is divided.
     * @return {@code this / divisor}.
     * @throws NullPointerException if {@code divisor == null}.
     * @throws ArithmeticException if {@code divisor == 0}.
     */
    CARAPI Divide(
        /* [in] */ IBigInteger* divisor,
        /* [out] */ IBigInteger** result);

    /**
     * Returns a {@code BigInteger} whose value is {@code this % divisor}.
     * Regarding signs this methods has the same behavior as the % operator on
     * ints: the sign of the remainder is the same as the sign of this.
     *
     * @param divisor value by which {@code this} is divided.
     * @throws NullPointerException if {@code divisor == null}.
     * @throws ArithmeticException if {@code divisor == 0}.
     */
    CARAPI Remainder(
        /* [in] */ IBigInteger* divisor,
        /* [out] */ IBigInteger** result);

    /**
     * Returns a {@code BigInteger} whose value is {@code 1/this mod m}. The
     * modulus {@code m} must be positive. The result is guaranteed to be in the
     * interval {@code [0, m)} (0 inclusive, m exclusive). If {@code this} is
     * not relatively prime to m, then an exception is thrown.
     *
     * @param m the modulus.
     * @throws NullPointerException if {@code m == null}
     * @throws ArithmeticException if {@code m < 0 or} if {@code this} is not
     *     relatively prime to {@code m}
     */
    CARAPI ModInverse(
        /* [in] */ IBigInteger* divisor,
        /* [out] */ IBigInteger** result);

    /**
     * Returns a {@code BigInteger} whose value is {@code
     * pow(this, exponent) mod m}. The modulus {@code m} must be positive. The
     * result is guaranteed to be in the interval {@code [0, m)} (0 inclusive,
     * m exclusive). If the exponent is negative, then {@code
     * pow(this.modInverse(m), -exponent) mod m} is computed. The inverse of
     * this only exists if {@code this} is relatively prime to m, otherwise an
     * exception is thrown.
     *
     * @param exponent the exponent.
     * @param m the modulus.
     * @throws NullPointerException if {@code m == null} or {@code exponent ==
     *     null}.
     * @throws ArithmeticException if {@code m < 0} or if {@code exponent<0} and
     *     this is not relatively prime to {@code m}.
     */
    CARAPI ModPow(
        /* [in] */ IBigInteger* exponent,
        /* [in] */ IBigInteger* m,
        /* [out] */ IBigInteger** result);

    /**
     * Returns a {@code BigInteger} whose value is {@code this mod m}. The
     * modulus {@code m} must be positive. The result is guaranteed to be in the
     * interval {@code [0, m)} (0 inclusive, m exclusive). The behavior of this
     * function is not equivalent to the behavior of the % operator defined for
     * the built-in {@code Int32}'s.
     *
     * @param m the modulus.
     * @return {@code this mod m}.
     * @throws NullPointerException if {@code m == null}.
     * @throws ArithmeticException if {@code m < 0}.
     */
    CARAPI Mod(
        /* [in] */ IBigInteger* m,
        /* [out] */ IBigInteger** result);

    /**
     * Tests whether this {@code BigInteger} is probably prime. If {@code true}
     * is returned, then this is prime with a probability beyond
     * {@code 1 - 1/pow(2, certainty)}. If {@code false} is returned, then this
     * is definitely composite. If the argument {@code certainty} <= 0, then
     * this method returns true.
     *
     * @param certainty tolerated primality uncertainty.
     * @return {@code true}, if {@code this} is probably prime, {@code false}
     *     otherwise.
     */
    CARAPI IsProbablePrime(
        /* [in] */ Int32 certainty,
        /* [out] */ Boolean* result);

    /**
     * Returns the smallest integer x > {@code this} which is probably prime as
     * a {@code BigInteger} instance. The probability that the returned {@code
     * BigInteger} is prime is beyond {@code 1 - 1/pow(2, 80)}.
     *
     * @return smallest integer > {@code this} which is probably prime.
     * @throws ArithmeticException if {@code this < 0}.
     */
    CARAPI NextProbablePrime(
        /* [out] */ IBigInteger** result);

    /**
     * Returns a string representation of this {@code BigInteger} in decimal
     * form.
     */
    CARAPI ToString(
        /* [out] */ String* string);

    /**
     * Returns a string containing a string representation of this {@code
     * BigInteger} with base radix. If {@code radix < Character.MIN_RADIX} or
     * {@code radix > Character.MAX_RADIX} then a decimal representation is
     * returned. The characters of the string representation are generated with
     * method {@code Character.forDigit}.
     *
     * @param radix base to be used for the string representation.
     */
    CARAPI ToString(
        /* [in] */ Int32 radix,
        /* [out] */ String* string);

    /*=================================INumber================================*/

    CARAPI ByteValue(
        /* [out] */ Byte* result);

    /**
     * Returns this {@code BigInteger} as an int value. If {@code this} is too
     * big to be represented as an int, then {@code this % (1 << 32)} is
     * returned.
     */
    CARAPI Int16Value(
        /* [out] */ Int16* result);

    CARAPI Int32Value(
        /* [out] */ Int32* result);

    /**
     * Returns this {@code BigInteger} as a long value. If {@code this} is too
     * big to be represented as a long, then {@code this % pow(2, 64)} is
     * returned.
     */
    CARAPI Int64Value(
        /* [out] */ Int64* result);

    /**
     * Returns this {@code BigInteger} as a float. If {@code this} is too big to
     * be represented as a float, then {@code Float.POSITIVE_INFINITY} or
     * {@code Float.NEGATIVE_INFINITY} is returned. Note that not all integers
     * in the range {@code [-Float.MAX_VALUE, Float.MAX_VALUE]} can be exactly
     * represented as a float.
     */
    CARAPI FloatValue(
        /* [out] */ Float* result);

    /**
     * Returns this {@code BigInteger} as a double. If {@code this} is too big
     * to be represented as a double, then {@code Double.POSITIVE_INFINITY} or
     * {@code Double.NEGATIVE_INFINITY} is returned. Note that not all integers
     * in the range {@code [-Double.MAX_VALUE, Double.MAX_VALUE]} can be exactly
     * represented as a double.
     */
    CARAPI DoubleValue(
        /* [out] */ Double* result);

    CARAPI Equals(
        /* [in] */ IInterface* other,
        /* [out] */ Boolean* result);

    CARAPI GetHashCode(
        /* [out] */ Int32* hashCode);

    static CARAPI NewFromBigInt(
        /* [in] */ BigInt* bigInt,
        /* [out] */ IBigInteger** result);

    CARAPI_(AutoPtr<BigInt>) GetBigInt();

private:
    CARAPI_(void) SetBigInt(
        /* [in] */ BigInt* bigInt);

    CARAPI_(void) SetJavaRepresentation(
        /* [in] */ Int32 sign,
        /* [in] */ Int32 numberLength,
        /* [in] */ const ArrayOf<Int32>& digits);

    CARAPI_(void) PrepareJavaRepresentation();

    /**
     * Returns the two's complement representation of this BigInteger in a byte
     * array.
     *
     * @return two's complement representation of {@code this}
     */
    CARAPI TwosComplement(
        /* [out, callee] */ ArrayOf<Byte>** bytes);

    CARAPI_(Int32) GetFirstNonzeroDigit();

    CARAPI_(Int32) GetDigit(
        /* [in] */ Int32 index);

    static CARAPI_(void) ParseFromString(
        /* [in] */ CBigInteger& bi,
        /* [in] */ const String& value,
        /* [in] */ Int32 radix);

    static CARAPI_(Int32) MultiplyByInt(
        /* [in] */ ArrayOf<Int32>& res,
        /* [in] */ ArrayOf<Int32>& a,
        /* [in] */ Int32 aSize,
        /* [in] */ Int32 factor);

    static CARAPI_(Int32) InplaceAdd(
        /* [in] */ ArrayOf<Int32>& a,
        /* [in] */ Int32 aSize,
        /* [in] */ Int32 addend);

public:
    /** The {@code BigInteger} constant 0. */
    static AutoPtr<IBigInteger> ZERO;

    /** The {@code BigInteger} constant 1. */
    static AutoPtr<IBigInteger> ONE;

    /** The {@code BigInteger} constant 10. */
    static AutoPtr<IBigInteger> TEN;

    /** The {@code BigInteger} constant -1. */
    static AutoPtr<IBigInteger> MINUS_ONE;

private:
    friend class BitLevel;
    friend class Logical;
    friend class Primality;
    friend class Conversion;
    friend class Multiplication;

    AutoPtr<BigInt> mBigInt;
    Boolean mNativeIsValid;
    Boolean mJavaIsValid;

    /** The magnitude of this in the little-endian representation. */
    //Int32[] digits;
    AutoPtr< ArrayOf<Int32> > mDigits;

    /**
     * The length of this in measured in ints. Can be less than
     * digits.length().
     */
    Int32 mNumberLength;

    Int32 mSign;
    Int32 mFirstNonzeroDigit;

    /** sign field, used for serialization. */
    Int32 mSignum;

    /** absolute value field, used for serialization */
    //byte[] magnitude;

    /** Cache for the hash code. */
    Int32 mHashCode;

    Mutex mLock;
};

} // namespace Math
} // namespace Elastos

#endif // __MATH_CBIGINTEGER_H__
