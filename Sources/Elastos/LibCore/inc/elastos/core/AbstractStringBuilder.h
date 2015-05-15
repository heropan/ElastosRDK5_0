#ifndef __ABSTRACTSTRINGBUILDER_H__
#define __ABSTRACTSTRINGBUILDER_H__

#ifdef ELASTOS_CORE
#include "Elastos.Core_server.h"
#else
#include "Elastos.Core.h"
#endif

namespace Elastos {
namespace Core {

class IntegralToString;

/**
 * A modifiable {@link CharSequence sequence of characters} for use in creating
 * and modifying Strings. This class is intended as a base class for
 * {@link StringBuffer} and {@link StringBuilder}.
 *
 * @see StringBuffer
 * @see StringBuilder
 * @since 1.5
 *
 */
class AbstractStringBuilder
{
protected:
    AbstractStringBuilder();

    AbstractStringBuilder(
        /* [in] */ Int32 capacity);

    AbstractStringBuilder(
        /* [in] */ const String& str);

    virtual ~AbstractStringBuilder();

    CARAPI SetLengthO(
        /* [in] */ Int32 length);

    CARAPI_(Int32) GetLengthO();

    CARAPI_(Int32) GetByteCountO();

    CARAPI_(Int32) GetCapacityO();

    CARAPI EnsureCapacityO(
        /* [in] */ Int32 min);

    CARAPI EnlargeBuffer(
        /* [in] */ Int32 min);

    CARAPI TrimToSizeO();

    CARAPI SetCharAtO(
        /* [in] */ Int32 index,
        /* [in] */ Char32 ch);

    CARAPI GetCharO(
        /* [in] */ Int32 index,
        /* [out] */ Char32* result);

    CARAPI GetCharsO(
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ ArrayOf<Char32>* dst,
        /* [in] */ Int32 dstStart);

    CARAPI SubstringO(
        /* [in] */ Int32 start,
        /* [out] */ String* string);

    CARAPI SubstringExO(
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [out] */ String* string);

    CARAPI SubSequenceO(
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [out] */ ICharSequence** seq);

    CARAPI IndexOfO(
        /* [in] */ const String& string,
        /* [out] */ Int32* index);

    CARAPI IndexOfExO(
        /* [in] */ const String& string,
        /* [in] */ Int32 start,
        /* [out] */ Int32* index);

    CARAPI LastIndexOfO(
        /* [in] */ const String& string,
        /* [out] */ Int32* index);

    CARAPI LastIndexOfExO(
        /* [in] */ const String& string,
        /* [in] */ Int32 start,
        /* [out] */ Int32* index);

    CARAPI AppendONULL();

    CARAPI AppendOCStr(
        /* [in] */ char const* cstr);

    CARAPI AppendOBoolean(
        /* [in] */ Boolean b);

    CARAPI AppendOInt32(
        /* [in] */ Int32 i);

    CARAPI AppendOInt64(
        /* [in] */ Int64 l);

    CARAPI AppendOFloat(
        /* [in] */ Float f);

    CARAPI AppendODouble(
        /* [in] */ Double d);

    CARAPI AppendOChars(
        /* [in] */ const ArrayOf<Char32>& chars);

    CARAPI AppendOCharsEx(
        /* [in] */ const ArrayOf<Char32>& chars,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 length);

    CARAPI AppendOChar(
        /* [in] */ Char32 ch);

    CARAPI AppendOString(
        /* [in] */ const String& string);

    CARAPI AppendOCharSequence(
        /* [in] */ ICharSequence* csq);

    CARAPI AppendOCharSequenceEx(
        /* [in] */ ICharSequence* csq,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    CARAPI AppendOObject(
        /* [in] */ IInterface* obj);

    CARAPI InsertOBoolean(
        /* [in] */ Int32 offset,
        /* [in] */ Boolean b);

    CARAPI InsertOInt32(
        /* [in] */ Int32 offset,
        /* [in] */ Int32 i);

    CARAPI InsertOInt64(
        /* [in] */ Int32 offset,
        /* [in] */ Int64 l);

    CARAPI InsertOFloat(
        /* [in] */ Int32 offset,
        /* [in] */ Float f);

    CARAPI InsertODouble(
        /* [in] */ Int32 offset,
        /* [in] */ Double d);

    CARAPI InsertOChar(
        /* [in] */ Int32 offset,
        /* [in] */ Char32 c);

    CARAPI InsertOChars(
        /* [in] */ Int32 offset,
        /* [in] */ const ArrayOf<Char32>& chars);

    CARAPI InsertOCharsEx(
        /* [in] */ Int32 offset,
        /* [in] */ const ArrayOf<Char32>& chars,
        /* [in] */ Int32 start,
        /* [in] */ Int32 length);

    CARAPI InsertOString(
        /* [in] */ Int32 offset,
        /* [in] */ const String& str);

    CARAPI InsertOCharSequence(
        /* [in] */ Int32 offset,
        /* [in] */ ICharSequence* s);

    CARAPI InsertOCharSequenceEx(
        /* [in] */ Int32 offset,
        /* [in] */ ICharSequence* s,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    CARAPI InsertOObject(
        /* [in] */ Int32 offset,
        /* [in] */ IInterface* obj);

    CARAPI ReplaceO(
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ const String& string);

    CARAPI ReverseO();

    CARAPI DeleteO(
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    CARAPI DeleteOChar(
        /* [in] */ Int32 index);

    CARAPI ToASCIICaseString(
        /* [in] */ Boolean isUpperCase);
private:
    /**
     * @brief constracts AbstractStringBuilder with s.
     *
     * Constract a string buffer with a constant characters in it and the specified initial step.
     * @param s A constant character pointer argument ,it contains the initial contents of the string buffer.
     */
    CARAPI_(void) Init(
        /* [in] */ const char* s,
        /* [in] */ Int32 step);

    /**
     * @brief Increases the capacity of the string buffer.
     * @param size An integer value representing the size want to increase for the string buffer.
     */
    CARAPI_(void) Break(
        /* [in] */ Int32 size);

    CARAPI_(void) GenerateNewBuffer(
        /* [in] */ Int32 capacity);

    CARAPI_(void) DbgPrint();

    // see UTF8.cpp
    inline Boolean IsASCII(char c) { return !(c & ~0x7F); }
    inline Int32 UTF8SequenceLengthNonASCII(char b0)
    {
        if ((b0 & 0xC0) != 0xC0)
            return 0;
        if ((b0 & 0xE0) == 0xC0)
            return 2;
        if ((b0 & 0xF0) == 0xE0)
            return 3;
        if ((b0 & 0xF8) == 0xF0)
            return 4;
        return 0;
    }
    inline Int32 UTF8SequenceLength(char b0)
    {
        return IsASCII(b0) ? 1 : UTF8SequenceLengthNonASCII(b0);
    }

private:
    friend class IntegralToString;

    /**
     * @var mByteCount
     * @brief The number of bits used to represent the length of a StringBuffer object.
     */
    Int32   mByteCount;// length in java

    /**
     * @var mCharCount
     * @brief The number of unicode code point used to represent the length of a StringBuffer object.
     */
    Int32 mCharCount;
    volatile Boolean mIsCounted;

    /**
     * @var mString
     * @brief The character pointer used to contain a string value.
     */
    char*  mString;

    /**
     * @var mStep
     * @brief The Integer value used to define a step how long a string buffer would increase for one time.
     */
    Int32   mStep;

    /**
     * @var mCapacity
     * @brief Integer value used to define the capacity of a string buffer.
     */
    Int32   mCapacity;

    Boolean mShared;
};

} // namespace Core
} // namespace Elastos

#endif // __ABSTRACTSTRINGBUILDER_H__
