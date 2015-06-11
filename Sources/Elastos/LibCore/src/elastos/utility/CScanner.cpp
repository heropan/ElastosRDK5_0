
#include "CScanner.h"
#include "Pattern.h"
#include "CLocale.h"
#include "Charset.h"
#include "IoUtils.h"
#include "CStringReader.h"
#include "CFileInputStream.h"
#include "CInputStreamReader.h"
#include "CStringWrapper.h"
#include "CBigDecimal.h"
#include "CBigInteger.h"
#include "CByte.h"
#include "CDouble.h"
#include "CFloat.h"
#include "CInteger16.h"
#include "CInteger32.h"
#include "CInteger64.h"
#include "Character.h"
#include "CharBuffer.h"
#include "NumberFormat.h"
#include <Math.h>
#include <StringUtils.h>

using Elastos::Core::ICharSequence;
using Elastos::Core::CStringWrapper;
using Elastos::Core::StringUtils;
using Elastos::Core::IByte;
using Elastos::Core::CByte;
using Elastos::Core::IDouble;
using Elastos::Core::CDouble;
using Elastos::Core::IFloat;
using Elastos::Core::CFloat;
using Elastos::Core::IInteger16;
using Elastos::Core::CInteger16;
using Elastos::Core::IInteger32;
using Elastos::Core::CInteger32;
using Elastos::Core::IInteger64;
using Elastos::Core::CInteger64;
using Elastos::Core::Character;
using Elastos::Utility::Regex::Pattern;
using Elastos::IO::IFileInputStream;
using Elastos::IO::CFileInputStream;
using Elastos::IO::IInputStreamReader;
using Elastos::IO::CInputStreamReader;
using Elastos::IO::IoUtils;
using Elastos::IO::ICloseable;
using Elastos::IO::IStringReader;
using Elastos::IO::CStringReader;
using Elastos::IO::CharBuffer;
using Elastos::IO::Charset::ICharset;
using Elastos::IO::Charset::Charset;
using Elastos::Math::CBigDecimal;
using Elastos::Math::IBigInteger;
using Elastos::Math::CBigInteger;
using Elastos::Text::INumberFormat;
using Elastos::Text::NumberFormat;
using Elastos::Text::IDecimalFormatSymbols;
using Libcore::ICU::CLocale;

namespace Elastos {
namespace Utility {

const AutoPtr<IPattern> CScanner::DEFAULT_DELIMITER;

const AutoPtr<IPattern> CScanner::BOOLEAN_PATTERN;

const AutoPtr<IPattern> CScanner::LINE_TERMINATOR;

const AutoPtr<IPattern> CScanner::MULTI_LINE_TERMINATOR;

const AutoPtr<IPattern> CScanner::LINE_PATTERN;

const AutoPtr<IPattern> CScanner::ANY_PATTERN;

const Int32 CScanner::DIPLOID;

const Int32 CScanner::DEFAULT_RADIX;

const Int32 CScanner::DEFAULT_TRUNK_SIZE;

CAR_INTERFACE_IMPL_2(CScanner, Object, IScanner, IIterator)

CAR_OBJECT_IMPL(CScanner)

Boolean CScanner::StaticInit()
{
    Pattern::Compile(String("\\p{javaWhitespace}+"), (IPattern**)&DEFAULT_DELIMITER);
    Pattern::Compile(String("TRUE|FALSE"), IPattern::CASE_INSENSITIVE, (IPattern**)&BOOLEAN_PATTERN);
    Pattern::Compile(String("(?s).*"), (IPattern**)&ANY_PATTERN);

    String NL = String("\n|\r\n|\r|\u0085|\u2028|\u2029");
    Pattern::Compile(NL, (IPattern**)&LINE_TERMINATOR);
    Pattern::Compile(String("(") + NL + String(")+"), (IPattern**)&MULTI_LINE_TERMINATOR);
    Pattern::Compile(String(".*(") + NL + String(")|.+(") + NL + String(")?"), (IPattern**)&LINE_PATTERN);
    return TRUE;
}

Boolean CScanner::sStaticflag = StaticInit();

CScanner::CScanner()
    : mDelimiter(DEFAULT_DELIMITER)
    , mIntegerRadix(DEFAULT_RADIX)
    , mFindStartIndex(0)
    , mPreStartIndex(mFindStartIndex)
    , mBufferLength(0)
    , mClosed(FALSE)
    , mLastIOException(NOERROR)
    , mMatchSuccessful(FALSE)
    , mInputExhausted(FALSE)
    , mCachehasNextIndex(-1)
{
    mLocale = CLocale::GetDefault();
}

ECode CScanner::constructor(
    /* [in] */ IFile* filesrc)
{
    AutoPtr<ICharset> outset;
    Charset::DefaultCharset((ICharset**)&outset);
    String outname;
    outset->GetName(&outname);
    return this->constructor(filesrc, outname);
}

ECode CScanner::constructor(
    /* [in] */ IFile* filesrc,
    /* [in] */ const String& charsetName)
{
    if (filesrc == NULL) {
        // throw new NullPointerException("filesrc == NULL");
        return E_NULL_POINTER_EXCEPTION;
    }
    AutoPtr<IFileInputStream> fis;
    FAIL_RETURN(CFileInputStream::New(filesrc, (IFileInputStream**)&fis));
    if (charsetName.IsNull()) {
        // throw new IllegalArgumentException("charsetName == NULL");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    // try {
    AutoPtr<IInputStreamReader> isr;
    ECode ec = CInputStreamReader::New(fis, charsetName, (IInputStreamReader**)&isr);
    // } catch (UnsupportedEncodingException e) {
    if (ec != NOERROR) {
        IoUtils::CloseQuietly(ICloseable::Probe(fis));
        // throw new IllegalArgumentException(e.getMessage());
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    // }
    mInput = IReadable::Probe(isr);
    return Initialization();
}

ECode CScanner::constructor(
    /* [in] */ const String& strsrc)
{
    AutoPtr<IStringReader> sr;
    FAIL_RETURN(CStringReader::New(strsrc, (IStringReader**)&sr));
    mInput = IReadable::Probe(sr);
    return Initialization();
}

ECode CScanner::constructor(
    /* [in] */ IInputStream* inputsrc)
{
    AutoPtr<ICharset> outset;
    Charset::DefaultCharset((ICharset**)&outset);
    String outname;
    outset->GetName(&outname);
    return this->constructor(inputsrc, outname);
}

ECode CScanner::constructor(
    /* [in] */ IInputStream* inputsrc,
    /* [in] */ const String& charsetName)
{
    if (inputsrc == NULL) {
        // throw new NullPointerException("inputsrc == NULL");
        return E_NULL_POINTER_EXCEPTION;
    }
    // try {
    AutoPtr<IInputStreamReader> isr;
    FAIL_RETURN(CInputStreamReader::New(inputsrc, charsetName, (IInputStreamReader**)&isr));
    mInput = IReadable::Probe(isr);
    // } catch (UnsupportedEncodingException e) {
        // throw new IllegalArgumentException(e.getMessage());
    // }
    return Initialization();
}

ECode CScanner::constructor(
    /* [in] */ IReadable* readsrc)
{
    if (readsrc == NULL) {
        // throw new NullPointerException("readsrc == NULL");
        return E_NULL_POINTER_EXCEPTION;
    }
    mInput = readsrc;
    return Initialization();
}

ECode CScanner::constructor(
    /* [in] */ IReadableByteChannel* rbcsrc)
{
    AutoPtr<ICharset> outset;
    Charset::DefaultCharset((ICharset**)&outset);
    String outname;
    outset->GetName(&outname);
    return this->constructor(rbcsrc, outname);
}

ECode CScanner::constructor(
    /* [in] */ IReadableByteChannel* rbcsrc,
    /* [in] */ const String& charsetName)
{
    if (rbcsrc == NULL) {
        // throw new NullPointerException("rbcsrc == NULL");
        return E_NULL_POINTER_EXCEPTION;
    }
    if (charsetName.IsNull()) {
        // throw new IllegalArgumentException("charsetName == NULL");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    assert(0 && "TODO");
    // input = Channels.newReader(rbcsrc, charsetName);
    return Initialization();
}

ECode CScanner::HasNext(
    /* [out] */ Boolean* result)
{
    return HasNext(ANY_PATTERN, result);
}

ECode CScanner::Next(
    /* [out] */ IInterface** object)
{
    VALIDATE_NOT_NULL(object)

    String str;
    FAIL_RETURN(Next(ANY_PATTERN, &str));
    AutoPtr<ICharSequence> sq;
    CStringWrapper::New(str, (ICharSequence**)&sq);
    *object = sq;
    REFCOUNT_ADD(*object);
    return NOERROR;
}

ECode CScanner::Remove()
{
    // throw new UnsupportedOperationException();
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

ECode CScanner::Close()
{
    if (mClosed) {
        return NOERROR;
    }
    if (ICloseable::Probe(mInput)) {
        // try {
        mLastIOException = ICloseable::Probe(mInput)->Close();
        // } catch (IOException e) {
            // mLastIOException = e;
        // }
    }
    mClosed = TRUE;
    return NOERROR;
}

ECode CScanner::Delimiter(
    /* [out] */ IPattern** outpat)
{
    VALIDATE_NOT_NULL(outpat)

    *outpat = mDelimiter;
    REFCOUNT_ADD(*outpat)
    return NOERROR;
}

ECode CScanner::FindInLine(
    /* [in] */ IPattern* pattern,
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str)

    FAIL_RETURN(CheckClosed());
    FAIL_RETURN(CheckNull(pattern));
    Int32 horizonLineSeparator = 0;

    mMatcher->UsePattern(MULTI_LINE_TERMINATOR);
    mMatcher->Region(mFindStartIndex, mBufferLength);

    Boolean findComplete = FALSE;
    Int32 terminatorLength = 0;
    while (!findComplete) {
        if (mMatcher->Find(&findComplete), findComplete) {
            mMatcher->Start(&horizonLineSeparator);
            mMatcher->End(&terminatorLength);
            terminatorLength -= horizonLineSeparator;
            findComplete = TRUE;
        }
        else {
            if (!mInputExhausted) {
                ReadMore();
                ResetMatcher();
            }
            else {
                horizonLineSeparator = mBufferLength;
                findComplete = TRUE;
            }
        }
    }

    mMatcher->UsePattern(pattern);

    /*
     * TODO The following 2 statements are used to deal with regex's bug.
     * java.util.regex.Matcher.region(Int32 start, Int32 end) implementation
     * does not have any effects when called. They will be removed once the
     * bug is fixed.
     */
    Int32 oldLimit = 0;
    mBuffer->GetLimit(&oldLimit);
    // Considering the look ahead feature, the line terminator should be involved as RI
    mBuffer->SetLimit(horizonLineSeparator + terminatorLength);
    // ========== To deal with regex bug ====================

    // Considering the look ahead feature, the line terminator should be involved as RI
    mMatcher->Region(mFindStartIndex, horizonLineSeparator + terminatorLength);
    if (mMatcher->Find(&findComplete), findComplete) {
        // The scanner advances past the input that matched
        mMatcher->End(&mFindStartIndex);
        // If the matched pattern is immediately followed by line
        // terminator.
        Int32 endlength = 0;
        if (horizonLineSeparator == (mMatcher->End(&endlength), endlength)) {
            mFindStartIndex += terminatorLength;
        }
        // the line terminator itself should not be a part of
        // the match result according to the Spec

        if (horizonLineSeparator != mBufferLength
                && (horizonLineSeparator + terminatorLength == (mMatcher->End(&endlength), endlength))) {
            // ========== To deal with regex bug ====================
            mBuffer->SetLimit(oldLimit);
            // ========== To deal with regex bug ====================

            mMatchSuccessful = FALSE;
            *str = String(NULL);
            return NOERROR;
        }
        mMatchSuccessful = TRUE;

        // ========== To deal with regex bug ====================
        mBuffer->SetLimit(oldLimit);
        // ========== To deal with regex bug ====================

        return mMatcher->Group(str);
    }

    // ========== To deal with regex bug ====================
    mBuffer->SetLimit(oldLimit);
    // ========== To deal with regex bug ====================

    mMatchSuccessful = FALSE;
    *str = String(NULL);
    return NOERROR;
}

ECode CScanner::FindInLine(
    /* [in] */ const String& pattern,
    /* [out] */ String* str)
{
    AutoPtr<IPattern> outpat;
    Pattern::Compile(pattern, (IPattern**)&outpat);
    return FindInLine(outpat, str);
}

ECode CScanner::FindWithinHorizon(
    /* [in] */ IPattern* pattern,
    /* [in] */ Int32 horizon,
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str)

    FAIL_RETURN(CheckClosed());
    FAIL_RETURN(CheckNull(pattern));
    if (horizon < 0) {
        // throw new IllegalArgumentException("horizon < 0");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    mMatcher->UsePattern(pattern);

    String result;
    Int32 findEndIndex = 0;
    Int32 horizonEndIndex = 0;
    if (horizon == 0) {
        horizonEndIndex = Elastos::Core::Math::INT32_MAX_VALUE;
    }
    else {
        horizonEndIndex = mFindStartIndex + horizon;
    }
    while (TRUE) {
        findEndIndex = mBufferLength;

        // If horizon > 0, then search up to
        // min( mBufferLength, mFindStartIndex + horizon).
        // Otherwise search until readable is exhausted.
        findEndIndex = Elastos::Core::Math::Min(horizonEndIndex, mBufferLength);
        // If horizon == 0, consider horizon as always outside buffer.
        Boolean isHorizonInBuffer = (horizonEndIndex <= mBufferLength);
        // First, try to find pattern within buffer. If pattern can not be
        // found in buffer, then expand the buffer and try again,
        // util horizonEndIndex is exceeded or no more input left.
        mMatcher->Region(mFindStartIndex, findEndIndex);
        Boolean isflag = FALSE;
        if (mMatcher->Find(&isflag), isflag) {
            if (isHorizonInBuffer || mInputExhausted) {
                mMatcher->Group(&result);
                break;
            }
        }
        else {
            // Pattern is not found in buffer while horizonEndIndex is
            // within buffer, or input is exhausted. Under this situation,
            // it can be judged that find fails.
            if (isHorizonInBuffer || mInputExhausted) {
                break;
            }
        }

        // Expand buffer and reset matcher if needed.
        if (!mInputExhausted) {
            ReadMore();
            ResetMatcher();
        }
    }
    if (!result.IsNull()) {
        mMatcher->End(&mFindStartIndex);
        mMatchSuccessful = TRUE;
    }
    else {
        mMatchSuccessful = FALSE;
    }
    *str = result;
    return NOERROR;
}

ECode CScanner::FindWithinHorizon(
    /* [in] */ const String& pattern,
    /* [in] */ Int32 horizon,
    /* [out] */ String* str)
{
    AutoPtr<IPattern> outpat;
    Pattern::Compile(pattern, (IPattern**)&outpat);
    return FindWithinHorizon(outpat, horizon, str);
}

ECode CScanner::HasNext(
    /* [in] */ IPattern* pattern,
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value)

    FAIL_RETURN(CheckClosed());
    FAIL_RETURN(CheckNull(pattern));
    mMatchSuccessful = FALSE;
    SaveCurrentStatus();
    // if the next token exists, set the match region, otherwise return
    // FALSE
    if (!SetTokenRegion()) {
        RecoverPreviousStatus();
        *value = FALSE;
        return NOERROR;
    }
    mMatcher->UsePattern(pattern);
    Boolean hasNext = FALSE;
    // check whether next token matches the specified pattern
    if (mMatcher->Matches(&hasNext), hasNext) {
        mCachehasNextIndex = mFindStartIndex;
        mMatchSuccessful = TRUE;
        hasNext = TRUE;
    }
    RecoverPreviousStatus();
    *value = hasNext;
    return NOERROR;
}

ECode CScanner::HasNext(
    /* [in] */ const String& pattern,
    /* [out] */ Boolean* value)
{
    AutoPtr<IPattern> outpat;
    Pattern::Compile(pattern, (IPattern**)&outpat);
    return HasNext(outpat, value);
}

ECode CScanner::HasNextBigDecimal(
    /* [out] */ Boolean* value)
{
    AutoPtr<IPattern> floatPattern = GetFloatPattern();
    Boolean isBigDecimalValue = FALSE;
    if (HasNext(floatPattern, &isBigDecimalValue), isBigDecimalValue) {
        String floatString;
        mMatcher->Group(&floatString);
        floatString = RemoveLocaleInfoFromFloat(floatString);
        // try {
        AutoPtr<IBigDecimal> outbd;
        ECode ec = CBigDecimal::New(floatString, (IBigDecimal**)&outbd);
        // } catch (NumberFormatException e) {
        if (ec == E_NUMBER_FORMAT_EXCEPTION) {
            mMatchSuccessful = FALSE;
        }
        // }
        if (ec == NOERROR) {
            isBigDecimalValue = TRUE;
            mCacheHasNextValue = outbd;
        }

    }
    *value = isBigDecimalValue;
    return NOERROR;
}

ECode CScanner::HasNextBigInteger(
    /* [out] */ Boolean* value)
{
    return HasNextBigInteger(mIntegerRadix, value);
}

ECode CScanner::HasNextBigInteger(
    /* [in] */ Int32 radix,
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value)

    AutoPtr<IPattern> integerPattern = GetIntegerPattern(radix);
    Boolean isBigIntegerValue = FALSE;
    if (HasNext(integerPattern, &isBigIntegerValue), isBigIntegerValue) {
        String intString;
        mMatcher->Group(&intString);
        intString = RemoveLocaleInfo(intString, INT);
        // try {
        AutoPtr<IBigInteger> outbi;
        ECode ec = CBigInteger::New(intString, radix, (IBigInteger**)&outbi);
        // } catch (NumberFormatException e) {
        if (ec == E_NUMBER_FORMAT_EXCEPTION) {
            mMatchSuccessful = FALSE;
        }
        // }
        if (ec == NOERROR) {
            isBigIntegerValue = TRUE;
            mCacheHasNextValue = outbi;
        }

    }
    *value = isBigIntegerValue;
    return NOERROR;
}

ECode CScanner::HasNextBoolean(
    /* [out] */ Boolean* value)
{
    return HasNext(BOOLEAN_PATTERN, value);
}

ECode CScanner::HasNextByte(
    /* [out] */ Boolean* value)
{
    return HasNextByte(mIntegerRadix, value);
}

ECode CScanner::HasNextByte(
    /* [in] */ Int32 radix,
    /* [out] */ Boolean* value)
{
    AutoPtr<IPattern> integerPattern = GetIntegerPattern(radix);
    Boolean isByteValue = FALSE;
    if (HasNext(integerPattern, &isByteValue), isByteValue) {
        String intString;
        mMatcher->Group(&intString);
        intString = RemoveLocaleInfo(intString, INT);
        // try {
        Int32 outbyte = StringUtils::ParseInt32(intString, radix);
        AutoPtr<IByte> inbyte;
        ECode ec = CByte::New((Byte)outbyte, (IByte**)&inbyte);
        // } catch (NumberFormatException e) {
        if (ec == E_NUMBER_FORMAT_EXCEPTION) {
            mMatchSuccessful = FALSE;
        }
        // }
        if (ec == NOERROR){
            mCacheHasNextValue = inbyte;
            isByteValue = TRUE;
        }

    }
    *value = isByteValue;
    return NOERROR;
}

ECode CScanner::HasNextDouble(
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value)

    AutoPtr<IPattern> floatPattern = GetFloatPattern();
    Boolean isDoubleValue = FALSE;
    if (HasNext(floatPattern, &isDoubleValue), isDoubleValue) {
        String floatString;
        mMatcher->Group(&floatString);
        floatString = RemoveLocaleInfoFromFloat(floatString);
        // try {
        Double outdouble = StringUtils::ParseDouble(floatString);
        AutoPtr<IDouble> indouble;
        ECode ec = CDouble::New(outdouble, (IDouble**)&indouble);
        // } catch (NumberFormatException e) {
        if (ec == E_NUMBER_FORMAT_EXCEPTION) {
            mMatchSuccessful = FALSE;
        }
        // }
        if (ec == NOERROR) {
            mCacheHasNextValue = indouble;
            isDoubleValue = TRUE;
        }
    }
    *value = isDoubleValue;
    return NOERROR;
}

ECode CScanner::HasNextFloat(
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value)

    AutoPtr<IPattern> floatPattern = GetFloatPattern();
    Boolean isFloatValue = FALSE;
    if (HasNext(floatPattern, &isFloatValue), isFloatValue) {
        String floatString;
        mMatcher->Group(&floatString);
        floatString = RemoveLocaleInfoFromFloat(floatString);
        // try {
        Float outfloat = StringUtils::ParseFloat(floatString);
        AutoPtr<IFloat> infloat;
        ECode ec = CFloat::New(outfloat, (IFloat**)&infloat);
        if (ec == NOERROR) {
            mCacheHasNextValue = infloat;
            isFloatValue = TRUE;
        }
        // } catch (NumberFormatException e) {
        if (ec == E_NUMBER_FORMAT_EXCEPTION) {
            mMatchSuccessful = FALSE;
        }
        // }
    }
    *value = isFloatValue;
    return NOERROR;
}

ECode CScanner::HasNextInt32(
    /* [out] */ Boolean* value)
{
    return HasNextInt32(mIntegerRadix, value);
}

ECode CScanner::HasNextInt32(
    /* [in] */ Int32 radix,
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value)

    AutoPtr<IPattern> integerPattern = GetIntegerPattern(radix);
    Boolean isIntValue = FALSE;
    if (HasNext(integerPattern, &isIntValue), isIntValue) {
        String intString;
        mMatcher->Group(&intString);
        intString = RemoveLocaleInfo(intString, INT);
        // try {
        Int32 outint = StringUtils::ParseInt32(intString, radix);
        AutoPtr<IInteger32> inint;
        ECode ec = CInteger32::New(outint, (IInteger32**)&inint);
        if (ec == NOERROR) {
            mCacheHasNextValue = inint;
            isIntValue = TRUE;
        }
        // } catch (NumberFormatException e) {
        if (ec == E_NUMBER_FORMAT_EXCEPTION) {
            mMatchSuccessful = FALSE;
        }
        // }
    }
    *value = isIntValue;
    return NOERROR;
}

ECode CScanner::HasNextLine(
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value)

    FAIL_RETURN(CheckClosed());
    mMatcher->UsePattern(LINE_PATTERN);
    mMatcher->Region(mFindStartIndex, mBufferLength);

    Boolean hasNextLine = FALSE;
    while (TRUE) {
        if (mMatcher->Find(&hasNextLine), hasNextLine) {
            Int32 intend = 0;
            if (mInputExhausted || (mMatcher->End(&intend), intend) != mBufferLength) {
                mMatchSuccessful = TRUE;
                hasNextLine = TRUE;
                break;
            }
        }
        else {
            if (mInputExhausted) {
                mMatchSuccessful = FALSE;
                break;
            }
        }
        if (!mInputExhausted) {
            ReadMore();
            ResetMatcher();
        }
    }
    *value = hasNextLine;
    return NOERROR;
}

ECode CScanner::HasNextInt64(
    /* [out] */ Boolean* value)
{
    return HasNextInt64(mIntegerRadix, value);
}

ECode CScanner::HasNextInt64(
    /* [in] */ Int32 radix,
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value)

    AutoPtr<IPattern> integerPattern = GetIntegerPattern(radix);
    Boolean isLongValue = FALSE;
    if (HasNext(integerPattern, &isLongValue), isLongValue) {
        String intString;
        mMatcher->Group(&intString);
        intString = RemoveLocaleInfo(intString, INT);
        // try {
        Int64 outint = StringUtils::Parse(intString, radix);
        AutoPtr<IInteger64> inint;
        ECode ec = CInteger64::New(outint, (IInteger64**)&inint);
        if (ec == NOERROR) {
            mCacheHasNextValue = inint;
            isLongValue = TRUE;
        }
        // } catch (NumberFormatException e) {
        if (ec == E_NUMBER_FORMAT_EXCEPTION) {
            mMatchSuccessful = FALSE;
        }
        // }
    }
    *value = isLongValue;
    return NOERROR;
}

ECode CScanner::HasNextInt16(
    /* [out] */ Boolean* value)
{
    return HasNextInt16(mIntegerRadix, value);
}

ECode CScanner::HasNextInt16(
    /* [in] */ Int32 radix,
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value)

    AutoPtr<IPattern> integerPattern = GetIntegerPattern(radix);
    Boolean isShortValue = FALSE;
    if (HasNext(integerPattern, &isShortValue), isShortValue) {
        String intString;
        mMatcher->Group(&intString);
        intString = RemoveLocaleInfo(intString, INT);
        // try {
        Int16 outint = StringUtils::ParseInt16(intString, radix);
        AutoPtr<IInteger16> inint;
        ECode ec = CInteger16::New(outint, (IInteger16**)&inint);
        if (ec == NOERROR) {
            mCacheHasNextValue = inint;
            isShortValue = TRUE;
        }
        // } catch (NumberFormatException e) {
        if (ec == E_NUMBER_FORMAT_EXCEPTION) {
            mMatchSuccessful = FALSE;
        }
        // }
    }
    *value = isShortValue;
    return NOERROR;
}

ECode CScanner::IoException()
{
    return mLastIOException;
}

ECode CScanner::GetLocale(
    /* [out] */ ILocale** locale)
{
    VALIDATE_NOT_NULL(locale)

    *locale = mLocale;
    REFCOUNT_ADD(*locale)
    return NOERROR;
}

ECode CScanner::GetMatch(
    /* [out] */ IMatchResult** outmatch)
{
    if (!mMatchSuccessful) {
        // throw new IllegalStateException();
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    return mMatcher->ToMatchResult(outmatch);
}

ECode CScanner::Next(
    /* [in] */ IPattern* pattern,
    /* [out] */ String* str)
{
    FAIL_RETURN(CheckClosed());
    FAIL_RETURN(CheckNull(pattern));
    mMatchSuccessful = FALSE;
    SaveCurrentStatus();
    if (!SetTokenRegion()) {
        RecoverPreviousStatus();
        // if setting match region fails
        // throw new NoSuchElementException();
        return E_NO_SUCH_ELEMENT_EXCEPTION;
    }
    mMatcher->UsePattern(pattern);
    Boolean isflag = FALSE;
    if (!(mMatcher->Matches(&isflag), isflag)) {
        RecoverPreviousStatus();
        // throw new InputMismatchException();
        return E_INPUT_MISMATCH_EXCEPTION;
    }
    mMatchSuccessful = TRUE;
    return mMatcher->Group(str);
}

ECode CScanner::Next(
    /* [in] */ const String& pattern,
    /* [out] */ String* str)
{
    AutoPtr<IPattern> outpat;
    Pattern::Compile(pattern, (IPattern**)&outpat);
    return Next(outpat, str);
}

ECode CScanner::NextBigDecimal(
    /* [out] */ IBigDecimal** outbig)
{
    VALIDATE_NOT_NULL(outbig)

    FAIL_RETURN(CheckClosed());
    AutoPtr<IInterface> obj = mCacheHasNextValue;
    mCacheHasNextValue = NULL;
    if (IBigDecimal::Probe(obj)) {
        mFindStartIndex = mCachehasNextIndex;
        *outbig = IBigDecimal::Probe(obj);
        REFCOUNT_ADD(*outbig);
        return NOERROR;
    }
    AutoPtr<IPattern> floatPattern = GetFloatPattern();
    String floatString;
    Next(floatPattern, &floatString);
    floatString = RemoveLocaleInfoFromFloat(floatString);
    AutoPtr<IBigDecimal> bigDecimalValue;
    // try {
    ECode ec = CBigDecimal::New(floatString, (IBigDecimal**)&bigDecimalValue);
    // } catch (NumberFormatException e) {
    if (ec == E_NUMBER_FORMAT_EXCEPTION) {
        mMatchSuccessful = FALSE;
        RecoverPreviousStatus();
        // throw new InputMismatchException();
        return E_INPUT_MISMATCH_EXCEPTION;
    }
    // }
    *outbig = bigDecimalValue;
    REFCOUNT_ADD(*outbig)
    return NOERROR;
}

ECode CScanner::NextBigInteger(
    /* [out] */ IBigInteger** outbig)
{
    return NextBigInteger(mIntegerRadix, outbig);
}

ECode CScanner::NextBigInteger(
    /* [in] */ Int32 radix,
    /* [out] */ IBigInteger** outbig)
{
    VALIDATE_NOT_NULL(outbig)

    FAIL_RETURN(CheckClosed());
    AutoPtr<IInterface> obj = mCacheHasNextValue;
    mCacheHasNextValue = NULL;
    if (IBigInteger::Probe(obj)) {
        mFindStartIndex = mCachehasNextIndex;
        *outbig = IBigInteger::Probe(obj);
        REFCOUNT_ADD(*outbig);
        return NOERROR;
    }
    AutoPtr<IPattern> integerPattern = GetIntegerPattern(radix);
    String intString;
    Next(integerPattern, &intString);
    intString = RemoveLocaleInfo(intString, INT);
    AutoPtr<IBigInteger> bigIntegerValue;
    // try {
    ECode ec = CBigInteger::New(intString, radix, (IBigInteger**)&bigIntegerValue);
    // } catch (NumberFormatException e) {
    if (ec == E_NUMBER_FORMAT_EXCEPTION) {
        mMatchSuccessful = FALSE;
        RecoverPreviousStatus();
        // throw new InputMismatchException();
        return E_INPUT_MISMATCH_EXCEPTION;
    }
    // }
    *outbig = bigIntegerValue;
    REFCOUNT_ADD(*outbig)
    return NOERROR;
}

ECode CScanner::NextBoolean(
    /* [out] */ Boolean* value)
{
    String outstr;
    Next(BOOLEAN_PATTERN, &outstr);
    *value = String("true").EqualsIgnoreCase(outstr);
    return NOERROR;
}

ECode CScanner::NextByte(
    /* [out] */ Byte* value)
{
    return NextByte(mIntegerRadix, value);
}

ECode CScanner::NextByte(
    /* [in] */ Int32 radix,
    /* [out] */ Byte* value)
{
    VALIDATE_NOT_NULL(value)

    FAIL_RETURN(CheckClosed());
    AutoPtr<IInterface> obj = mCacheHasNextValue;
    mCacheHasNextValue = NULL;
    if (IByte::Probe(obj)) {
        mFindStartIndex = mCachehasNextIndex;
        return IByte::Probe(obj)->GetValue(value);
    }
    AutoPtr<IPattern> integerPattern = GetIntegerPattern(radix);
    String intString;
    Next(integerPattern, &intString);
    intString = RemoveLocaleInfo(intString, INT);
    // try {
    Int32 byteValue = 0;
    ECode ec = StringUtils::ParseInt32(intString, radix, &byteValue);
    // } catch (NumberFormatException e) {
    if (ec == E_NUMBER_FORMAT_EXCEPTION) {
        mMatchSuccessful = FALSE;
        RecoverPreviousStatus();
        // throw new InputMismatchException();
        return E_INPUT_MISMATCH_EXCEPTION;
    }
    // }
    *value = (Byte)byteValue;
    return NOERROR;
}

ECode CScanner::NextDouble(
    /* [out] */ Double* value)
{
    VALIDATE_NOT_NULL(value)

    FAIL_RETURN(CheckClosed());
    AutoPtr<IInterface> obj = mCacheHasNextValue;
    mCacheHasNextValue = NULL;
    if (IDouble::Probe(obj)) {
        mFindStartIndex = mCachehasNextIndex;
        return IDouble::Probe(obj)->GetValue(value);
    }
    AutoPtr<IPattern> floatPattern = GetFloatPattern();
    String floatString;
    Next(floatPattern, &floatString);
    floatString = RemoveLocaleInfoFromFloat(floatString);
    Double doubleValue = 0;
    // try {
    ECode ec = StringUtils::ParseDouble(floatString, &doubleValue);
    // } catch (NumberFormatException e) {
    if (ec == E_NUMBER_FORMAT_EXCEPTION) {
        mMatchSuccessful = FALSE;
        RecoverPreviousStatus();
        // throw new InputMismatchException();
        return E_INPUT_MISMATCH_EXCEPTION;
    }
    // }
    *value = doubleValue;
    return NOERROR;
}

ECode CScanner::NextFloat(
    /* [out] */ Float* value)
{
    FAIL_RETURN(CheckClosed());
    AutoPtr<IInterface> obj = mCacheHasNextValue;
    mCacheHasNextValue = NULL;
    if (IFloat::Probe(obj)) {
        mFindStartIndex = mCachehasNextIndex;
        return IFloat::Probe(obj)->GetValue(value);
    }
    AutoPtr<IPattern> floatPattern = GetFloatPattern();
    String floatString;
    Next(floatPattern, &floatString);
    floatString = RemoveLocaleInfoFromFloat(floatString);
    Float floatValue = 0;
    // try {
    ECode ec = StringUtils::ParseFloat(floatString, &floatValue);
    // } catch (NumberFormatException e) {
    if (ec == E_NUMBER_FORMAT_EXCEPTION) {
        mMatchSuccessful = FALSE;
        RecoverPreviousStatus();
        // throw new InputMismatchException();
        return E_INPUT_MISMATCH_EXCEPTION;
    }
    // }
    *value = floatValue;
    return NOERROR;
}

ECode CScanner::NextInt32(
    /* [out] */ Int32* value)
{
    return NextInt32(mIntegerRadix, value);
}

ECode CScanner::NextInt32(
    /* [in] */ Int32 radix,
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value)

    FAIL_RETURN(CheckClosed());
    AutoPtr<IInterface> obj = mCacheHasNextValue;
    mCacheHasNextValue = NULL;
    if (IInteger32::Probe(obj)) {
        mFindStartIndex = mCachehasNextIndex;
        return IInteger32::Probe(obj)->GetValue(value);
    }
    AutoPtr<IPattern> integerPattern = GetIntegerPattern(radix);
    String intString;
    Next(integerPattern, &intString);
    intString = RemoveLocaleInfo(intString, INT);
    Int32 intValue = 0;
    // try {
    ECode ec = StringUtils::ParseInt32(intString, radix, &intValue);
    // } catch (NumberFormatException e) {
    if (ec == E_NUMBER_FORMAT_EXCEPTION) {
        mMatchSuccessful = FALSE;
        RecoverPreviousStatus();
        // throw new InputMismatchException();
        return E_INPUT_MISMATCH_EXCEPTION;
    }
    // }
    *value = intValue;
    return NOERROR;
}

ECode CScanner::NextLine(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str)

    FAIL_RETURN(CheckClosed());

    mMatcher->UsePattern(LINE_PATTERN);
    mMatcher->Region(mFindStartIndex, mBufferLength);
    String result;
    Boolean isflag = FALSE;
    while (TRUE) {
        if (mMatcher->Find(&isflag), isflag) {
            Int32 endvalue = 0;
            if (mInputExhausted || (mMatcher->End(&endvalue), endvalue) != mBufferLength
                    || mBufferLength < (mBuffer->GetCapacity(&endvalue), endvalue)) {
                mMatchSuccessful = TRUE;
                mMatcher->End(&mFindStartIndex);
                mMatcher->Group(&result);
                break;
            }
        }
        else {
            if (mInputExhausted) {
                mMatchSuccessful = FALSE;
                // throw new NoSuchElementException();
                return E_NO_SUCH_ELEMENT_EXCEPTION;
            }
        }
        if (!mInputExhausted) {
            ReadMore();
            ResetMatcher();
        }
    }
    // Find text without line terminator here.
    if (result != NULL) {
        AutoPtr<IMatcher> terminatorMatcher;
        LINE_TERMINATOR->Matcher(result, (IMatcher**)&terminatorMatcher);
        if (terminatorMatcher->Find(&isflag), isflag) {
            Int32 startvalue = 0;
            terminatorMatcher->Start(&startvalue);
            result = result.Substring(0, startvalue);
        }
    }
    *str = result;
    return NOERROR;
}

ECode CScanner::NextInt64(
    /* [out] */ Int64* value)
{
    return NextInt64(mIntegerRadix, value);
}

ECode CScanner::NextInt64(
    /* [in] */ Int32 radix,
    /* [out] */ Int64* value)
{
    VALIDATE_NOT_NULL(value)

    FAIL_RETURN(CheckClosed());
    AutoPtr<IInterface> obj = mCacheHasNextValue;
    mCacheHasNextValue = NULL;
    if (IInteger64::Probe(obj)) {
        mFindStartIndex = mCachehasNextIndex;
        return IInteger64::Probe(obj)->GetValue(value);
    }
    AutoPtr<IPattern> integerPattern = GetIntegerPattern(radix);
    String intString;
    Next(integerPattern, &intString);
    intString = RemoveLocaleInfo(intString, INT);
    Int64 longValue = 0;
    // try {
    ECode ec = StringUtils::Parse(intString, radix, &longValue);
    // } catch (NumberFormatException e) {
    if (ec == E_NUMBER_FORMAT_EXCEPTION) {
        mMatchSuccessful = FALSE;
        RecoverPreviousStatus();
        // throw new InputMismatchException();
        return E_INPUT_MISMATCH_EXCEPTION;
    }
    // }
    *value = longValue;
    return NOERROR;
}

ECode CScanner::NextInt16(
    /* [out] */ Int16* value)
{
    return NextInt16(mIntegerRadix, value);
}

ECode CScanner::NextInt16(
    /* [in] */ Int32 radix,
    /* [out] */ Int16* value)
{
    VALIDATE_NOT_NULL(value)

    FAIL_RETURN(CheckClosed());
    AutoPtr<IInterface> obj = mCacheHasNextValue;
    mCacheHasNextValue = NULL;
    if (IInteger16::Probe(obj)) {
        mFindStartIndex = mCachehasNextIndex;
        return IInteger16::Probe(obj)->GetValue(value);
    }
    AutoPtr<IPattern> integerPattern = GetIntegerPattern(radix);
    String intString;
    Next(integerPattern, &intString);
    intString = RemoveLocaleInfo(intString, INT);
    Int16 shortValue = 0;
    // try {
    ECode ec = StringUtils::ParseInt16(intString, radix, &shortValue);
    // } catch (NumberFormatException e) {
    if (ec == E_NUMBER_FORMAT_EXCEPTION) {
        mMatchSuccessful = FALSE;
        RecoverPreviousStatus();
        // throw new InputMismatchException();
        return E_INPUT_MISMATCH_EXCEPTION;
    }
    // }
    *value = shortValue;
    return NOERROR;
}

ECode CScanner::Radix(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value)

    *value = mIntegerRadix;
    return NOERROR;
}

ECode CScanner::Skip(
    /* [in] */ IPattern* pattern,
    /* [out] */ IScanner** outscan)
{
    VALIDATE_NOT_NULL(outscan)

    FAIL_RETURN(CheckClosed());
    FAIL_RETURN(CheckNull(pattern));
    mMatcher->UsePattern(pattern);
    mMatcher->Region(mFindStartIndex, mBufferLength);
    while (TRUE) {
        Boolean isflag = FALSE;
        if (mMatcher->LookingAt(&isflag), isflag) {
            Int32 endvalue = 0;
            mMatcher->End(&endvalue);
            Boolean matchInBuffer = endvalue < mBufferLength
                    || (endvalue == mBufferLength && mInputExhausted);
            if (matchInBuffer) {
                mMatchSuccessful = TRUE;
                mMatcher->End(&mFindStartIndex);
                break;
            }
        }
        else {
            if (mInputExhausted) {
                mMatchSuccessful = FALSE;
                // throw new NoSuchElementException();
                return E_NO_SUCH_ELEMENT_EXCEPTION;
            }
        }
        if (!mInputExhausted) {
            ReadMore();
            ResetMatcher();
        }
    }
    *outscan = (IScanner*) this->Probe(EIID_IScanner);
    REFCOUNT_ADD(*outscan)
    return NOERROR;
}

ECode CScanner::Skip(
    /* [in] */ const String& pattern,
    /* [out] */ IScanner** outscan)
{
    AutoPtr<IPattern> outpat;
    Pattern::Compile(pattern, (IPattern**)&outpat);
    return Skip(outpat, outscan);
}

ECode CScanner::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str)

    assert(0 && "TODO");
    // return getClass().getName() +
    //         "[mDelimiter=" + mDelimiter +
    //         ",mFindStartIndex=" + mFindStartIndex +
    //         ",mMatchSuccessful=" + mMatchSuccessful +
    //         ",mClosed=" + mClosed +
    //         "]";
    return NOERROR;
}

ECode CScanner::UseDelimiter(
    /* [in] */ IPattern* pattern,
    /* [out] */ IScanner** outscan)
{
    VALIDATE_NOT_NULL(outscan)

    mDelimiter = pattern;
    *outscan = (IScanner*) this->Probe(EIID_IScanner);
    REFCOUNT_ADD(*outscan)
    return NOERROR;
}

ECode CScanner::UseDelimiter(
    /* [in] */ const String& pattern,
    /* [out] */ IScanner** outscan)
{
    AutoPtr<IPattern> outpat;
    Pattern::Compile(pattern, (IPattern**)&outpat);
    return UseDelimiter(outpat, outscan);
}

ECode CScanner::UseLocale(
    /* [in] */ ILocale* l,
    /* [out] */ IScanner** outscan)
{
    VALIDATE_NOT_NULL(outscan)

    if (l == NULL) {
        // throw new NullPointerException("l == NULL");
        return E_NULL_POINTER_EXCEPTION;
    }
    mLocale = l;
    *outscan = (IScanner*) this->Probe(EIID_IScanner);
    REFCOUNT_ADD(*outscan)
    return NOERROR;
}

ECode CScanner::UseRadix(
    /* [in] */ Int32 radix,
    /* [out] */ IScanner** outscan)
{
    VALIDATE_NOT_NULL(outscan)

    FAIL_RETURN(CheckRadix(radix));
    mIntegerRadix = radix;
    *outscan = (IScanner*) this->Probe(EIID_IScanner);
    REFCOUNT_ADD(*outscan)
    return NOERROR;
}

ECode CScanner::Reset(
    /* [out] */ IScanner** outscan)
{
    VALIDATE_NOT_NULL(outscan)

    mDelimiter = DEFAULT_DELIMITER;
    mLocale = CLocale::GetDefault();
    mIntegerRadix = 10;
    *outscan = (IScanner*) this->Probe(EIID_IScanner);
    REFCOUNT_ADD(*outscan)
    return NOERROR;
}

ECode CScanner::CheckRadix(
    /* [in] */ Int32 radix)
{
    if (radix < Character::MIN_RADIX || radix > Character::MAX_RADIX) {
        // throw new IllegalArgumentException("Invalid radix: " + radix);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return NOERROR;
}

ECode CScanner::Initialization()
{
    FAIL_RETURN(CharBuffer::Allocate(DEFAULT_TRUNK_SIZE, (ICharBuffer**)&mBuffer));
    mBuffer->SetLimit(0);
    String outstr;
    mBuffer->ToString(&outstr);
    return mDelimiter->Matcher(outstr, (IMatcher**)&mMatcher);
}

ECode CScanner::CheckClosed()
{
    if (mClosed) {
        // throw new IllegalStateException();
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    return NOERROR;
}

ECode CScanner::CheckNull(
    /* [in] */ IPattern* pattern)
{
    if (pattern == NULL) {
        // throw new NullPointerException("pattern == NULL");
        return E_NULL_POINTER_EXCEPTION;
    }
    return NOERROR;
}

ECode CScanner::ResetMatcher()
{
    String outstr;
    mBuffer->ToString(&outstr);
    if (mMatcher == NULL) {
        mDelimiter->Matcher(outstr, (IMatcher**)&mMatcher);
    }
    else {
        AutoPtr<ICharSequence> sq;
        CStringWrapper::New(outstr, (ICharSequence**)&sq);
        mMatcher->Reset(sq);
    }
    mMatcher->Region(mFindStartIndex, mBufferLength);
    return NOERROR;
}

ECode CScanner::SaveCurrentStatus()
{
    mPreStartIndex = mFindStartIndex;
    return NOERROR;
}

ECode CScanner::RecoverPreviousStatus()
{
    mFindStartIndex = mPreStartIndex;
    return NOERROR;
}

AutoPtr<IPattern> CScanner::GetIntegerPattern(
    /* [in] */ Int32 radix)
{
    CheckRadix(radix);
    AutoPtr<INumberFormat> nf;
    NumberFormat::GetInstance(mLocale, (INumberFormat**)&nf);
    mDecimalFormat = IDecimalFormat::Probe(nf);

    String allAvailableDigits = String("0123456789abcdefghijklmnopqrstuvwxyz");
    String ASCIIDigit = allAvailableDigits.Substring(0, radix);
    String nonZeroASCIIDigit = allAvailableDigits.Substring(1, radix);

    StringBuilder digit("((?i)[");
    digit += ASCIIDigit;
    digit += "]|\\p{javaDigit})";
    StringBuilder nonZeroDigit("((?i)[");
    nonZeroDigit += nonZeroASCIIDigit;
    nonZeroDigit += "]|([\\p{javaDigit}&&[^0]]))";
    AutoPtr<StringBuilder> numeral = GetNumeral(&digit, &nonZeroDigit);

    StringBuilder integer("(([-+]?(");
    integer += *numeral;
    integer += ")))|(";
    integer += *(AddPositiveSign(numeral));
    integer += ")|(";
    integer += *(AddNegativeSign(numeral));
    integer += ")";

    AutoPtr<IPattern> integerPattern;
    Pattern::Compile(integer.ToString(), (IPattern**)&integerPattern);
    return integerPattern;
}

AutoPtr<IPattern> CScanner::GetFloatPattern()
{
    AutoPtr<INumberFormat> numfor;
    NumberFormat::GetInstance(mLocale, (INumberFormat**)&numfor);
    mDecimalFormat = IDecimalFormat::Probe(numfor);

    StringBuilder digit("([0-9]|(\\p{javaDigit}))");
    StringBuilder nonZeroDigit("[\\p{javaDigit}&&[^0]]");
    AutoPtr<StringBuilder> numeral = GetNumeral(&digit, &nonZeroDigit);

    AutoPtr<IDecimalFormatSymbols> dfs;
    mDecimalFormat->GetDecimalFormatSymbols((IDecimalFormatSymbols**)&dfs);
    Char32 outchar;
    dfs->GetDecimalSeparator(&outchar);
    String decimalSeparator = String("\\") + outchar;
    StringBuilder decimalNumeral("(");
    decimalNumeral += *numeral;
    decimalNumeral += "|";
    decimalNumeral += *numeral;
    decimalNumeral += decimalSeparator;
    decimalNumeral += digit;
    decimalNumeral += "*+|";
    decimalNumeral += decimalSeparator;
    decimalNumeral += digit;
    decimalNumeral += "++)";
    StringBuilder exponent("([eE][+-]?");
    exponent += digit;
    exponent += "+)?";

    StringBuilder decimal("(([-+]?");
    decimal += decimalNumeral;
    decimal += "(";
    decimal += exponent;
    decimal += "?)";
    decimal += ")|(";
    decimal += *(AddPositiveSign(&decimalNumeral));
    decimal += "(";
    decimal += exponent;
    decimal += "?)";
    decimal += ")|(";
    decimal += *(AddNegativeSign(&decimalNumeral));
    decimal += "(";
    decimal += exponent;
    decimal += "?)";
    decimal += "))";

    StringBuilder hexFloat("([-+]?0[xX][0-9a-fA-F]*");
    hexFloat += "\\.";
    hexFloat += "[0-9a-fA-F]+([pP][-+]?[0-9]+)?)";
    String localNaN;
    dfs->GetNaN(&localNaN);
    String localeInfinity;
    dfs->GetInfinity(&localeInfinity);
    StringBuilder nonNumber("(NaN|\\Q");
    nonNumber += localNaN;
    nonNumber += "\\E|Infinity|\\Q";
    nonNumber += localeInfinity;
    nonNumber += "\\E)";
    StringBuilder singedNonNumber("((([-+]?(");
    singedNonNumber += nonNumber;
    singedNonNumber += ")))|(";
    singedNonNumber += *(AddPositiveSign(&nonNumber));
    singedNonNumber += ")|(";
    singedNonNumber += *(AddNegativeSign(&nonNumber));
    singedNonNumber += "))";;

    StringBuilder floatString;
    floatString += decimal;
    floatString += "|";
    floatString += hexFloat;
    floatString += "|";
    floatString += singedNonNumber;
    AutoPtr<IPattern> floatPattern;
    Pattern::Compile(floatString.ToString(), (IPattern**)&floatPattern);
    return floatPattern;
}

AutoPtr<StringBuilder> CScanner::GetNumeral(
    /* [in] */ StringBuilder* digit,
    /* [in] */ StringBuilder* nonZeroDigit)
{
    AutoPtr<IDecimalFormatSymbols> dfs;
    mDecimalFormat->GetDecimalFormatSymbols((IDecimalFormatSymbols**)&dfs);
    String groupSeparator;
    Char32 outchar;
    dfs->GetGroupingSeparator(&outchar);
    groupSeparator = String("\\") + outchar;
    StringBuilder groupedNumeral("(");
    groupedNumeral += *nonZeroDigit;
    groupedNumeral += *digit;
    groupedNumeral += "?";
    groupedNumeral += *digit;
    groupedNumeral += "?(";
    groupedNumeral += groupSeparator;
    groupedNumeral += *digit;
    groupedNumeral += *digit;
    groupedNumeral += *digit;
    groupedNumeral += ")+)";
    AutoPtr<StringBuilder> numeral = new StringBuilder("((");
    *numeral += *digit;
    *numeral += "++)|";
    *numeral += groupedNumeral;
    *numeral += ")";
    return numeral;
}

AutoPtr<StringBuilder> CScanner::AddPositiveSign(
    /* [in] */ StringBuilder* unSignNumeral)
{
    String positivePrefix("");
    String positiveSuffix("");
    String gpp;
    mDecimalFormat->GetPositivePrefix(&gpp);
    if (!gpp.IsEmpty()) {
        positivePrefix = String("\\Q") + gpp + "\\E";
    }
    String gps;
    mDecimalFormat->GetPositiveSuffix(&gps);
    if (!gps.IsEmpty()) {
        positiveSuffix = String("\\Q") + gps + "\\E";
    }
    AutoPtr<StringBuilder> signedNumeral = new StringBuilder();
    *signedNumeral += positivePrefix;
    *signedNumeral += *unSignNumeral;
    *signedNumeral += positiveSuffix;
    return signedNumeral;
}

AutoPtr<StringBuilder> CScanner::AddNegativeSign(
    /* [in] */ StringBuilder* unSignNumeral)
{
    String negativePrefix("");
    String negativeSuffix("");
    String gnp;
    mDecimalFormat->GetNegativePrefix(&gnp);
    if (!gnp.IsEmpty()) {
        negativePrefix = String("\\Q") + gnp + "\\E";
    }
    String gns;
    mDecimalFormat->GetNegativeSuffix(&gns);
    if (!gns.IsEmpty()) {
        negativeSuffix = String("\\Q") + gns + "\\E";
    }
    AutoPtr<StringBuilder> signedNumeral = new StringBuilder();
    *signedNumeral += negativePrefix;
    *signedNumeral += *unSignNumeral;
    *signedNumeral += negativeSuffix;
    return signedNumeral;
}

String CScanner::RemoveLocaleInfoFromFloat(
    /* [in] */ const String& floatString)
{
    // If the token is HexFloat
    if (-1 != floatString.IndexOf('x') || -1 != floatString.IndexOf('X')) {
        return floatString;
    }

    Int32 exponentIndex;
    String decimalNumeralString;
    String exponentString;
    // If the token is scientific notation
    if (-1 != (exponentIndex = floatString.IndexOf('e'))
            || -1 != (exponentIndex = floatString.IndexOf('E'))) {
        decimalNumeralString = floatString.Substring(0, exponentIndex);
        exponentString = floatString.Substring(exponentIndex + 1,
                floatString.GetLength());
        decimalNumeralString = RemoveLocaleInfo(decimalNumeralString, FLOAT);
        return decimalNumeralString + String("e") + exponentString;
    }
    return RemoveLocaleInfo(floatString, FLOAT);
}

String CScanner::RemoveLocaleInfo(
    /* [in] */ const String& token,
    /* [in] */ DataType type)
{
    StringBuilder tokenBuilder(token);
    Boolean negative = RemoveLocaleSign(&tokenBuilder);
    // Remove group separator
    AutoPtr<IDecimalFormatSymbols> dfs;
    mDecimalFormat->GetDecimalFormatSymbols((IDecimalFormatSymbols**)&dfs);
    Char32 outchar;
    dfs->GetGroupingSeparator(&outchar);
    String groupSeparator;
    groupSeparator += outchar;
    Int32 separatorIndex = -1;
    while (-1 != (tokenBuilder.IndexOf(groupSeparator, &separatorIndex), separatorIndex)) {
        tokenBuilder.Delete(separatorIndex, separatorIndex + 1);
    }
    // Remove decimal separator
    dfs->GetDecimalSeparator(&outchar);
    String decimalSeparator;
    decimalSeparator += outchar;
    tokenBuilder.IndexOf(decimalSeparator, &separatorIndex);
    StringBuilder result("");
    if (INT == type) {
        for (Int32 i = 0; i < tokenBuilder.GetLength(); i++) {
            if (-1 != Character::ToDigit(tokenBuilder.GetChar(i),
                    Character::MAX_RADIX)) {
                result += tokenBuilder.GetChar(i);
            }
        }
    }
    if (FLOAT == type) {
        String strnan;
        if (tokenBuilder.ToString().Equals((dfs->GetNaN(&strnan), strnan))) {
            result += "NaN";
        }
        else if (tokenBuilder.ToString().Equals((dfs->GetInfinity(&strnan), strnan))) {
            result += "Infinity";
        }
        else {
            for (Int32 i = 0; i < tokenBuilder.GetLength(); i++) {
                if (-1 != Character::ToDigit(tokenBuilder.GetChar(i), 10)) {
                    result += Character::ToDigit(tokenBuilder.GetChar(i), 10);
                }
            }
        }
    }
    // Token is NaN or Infinity
    if (result.GetLength() == 0) {
        result += tokenBuilder;
    }
    if (-1 != separatorIndex) {
        result.InsertChar(separatorIndex, ".");
    }
    // If input is negative
    if (negative) {
        result.InsertChar(0, '-');
    }
    return result.ToString();
}

Boolean CScanner::RemoveLocaleSign(
    /* [in] */ StringBuilder* tokenBuilder)
{
    String positivePrefix;
    mDecimalFormat->GetPositivePrefix(&positivePrefix);
    String positiveSuffix;
    mDecimalFormat->GetPositiveSuffix(&positiveSuffix);
    String negativePrefix;
    mDecimalFormat->GetNegativePrefix(&negativePrefix);
    String negativeSuffix;
    mDecimalFormat->GetNegativeSuffix(&negativeSuffix);

    Int32 indexvalue = 0;
    if ((tokenBuilder->IndexOf(String("+"), &indexvalue), indexvalue) == 0) {
        tokenBuilder->Delete(0, 1);
    }
    if (!positivePrefix.IsEmpty() && (tokenBuilder->IndexOf(positivePrefix, &indexvalue), indexvalue) == 0) {
        tokenBuilder->Delete(0, positivePrefix.GetLength());
    }
    if (!positiveSuffix.IsEmpty()
            && -1 != (tokenBuilder->IndexOf(positiveSuffix, &indexvalue), indexvalue)) {
        tokenBuilder->Delete(
                tokenBuilder->GetLength() - positiveSuffix.GetLength(),
                tokenBuilder->GetLength());
    }
    Boolean negative = FALSE;
    if (tokenBuilder->IndexOf(String("-"), &indexvalue), indexvalue == 0) {
        tokenBuilder->Delete(0, 1);
        negative = TRUE;
    }
    if (!negativePrefix.IsEmpty() && (tokenBuilder->IndexOf(negativePrefix, &indexvalue), indexvalue) == 0) {
        tokenBuilder->Delete(0, negativePrefix.GetLength());
        negative = TRUE;
    }
    if (!negativeSuffix.IsEmpty()
            && -1 != (tokenBuilder->IndexOf(negativeSuffix, &indexvalue), indexvalue)) {
        tokenBuilder->Delete(
                tokenBuilder->GetLength() - negativeSuffix.GetLength(),
                tokenBuilder->GetLength());
        negative = TRUE;
    }
    return negative;
}

Boolean CScanner::SetTokenRegion()
{
    // The position where token begins
    Int32 tokenStartIndex = 0;
    // The position where token ends
    Int32 tokenEndIndex = 0;
    // Use mDelimiter pattern
    mMatcher->UsePattern(mDelimiter);
    mMatcher->Region(mFindStartIndex, mBufferLength);

    tokenStartIndex = FindPreDelimiter();
    if (SetHeadTokenRegion(tokenStartIndex)) {
        return TRUE;
    }
    tokenEndIndex = FindPostDelimiter();
    // If the second mDelimiter is not found
    if (-1 == tokenEndIndex) {
        // Just first Delimiter Exists
        if (mFindStartIndex == mBufferLength) {
            return FALSE;
        }
        tokenEndIndex = mBufferLength;
        mFindStartIndex = mBufferLength;
    }

    mMatcher->Region(tokenStartIndex, tokenEndIndex);
    return TRUE;
}

Int32 CScanner::FindPreDelimiter()
{
    Int32 tokenStartIndex;
    Boolean findComplete = FALSE;
    while (!findComplete) {
        if (mMatcher->Find(&findComplete), findComplete) {
            findComplete = TRUE;
            // If just mDelimiter remains
            Int32 startvalue =0;
            if ((mMatcher->Start(&startvalue), startvalue) == mFindStartIndex
                    && (mMatcher->End(&startvalue), startvalue) == mBufferLength) {
                // If more input resource exists
                if (!mInputExhausted) {
                    ReadMore();
                    ResetMatcher();
                    findComplete = FALSE;
                }
            }
        }
        else {
            if (!mInputExhausted) {
                ReadMore();
                ResetMatcher();
            }
            else {
                return -1;
            }
        }
    }
    mMatcher->End(&tokenStartIndex);
    mMatcher->End(&mFindStartIndex);
    return tokenStartIndex;
}

Boolean CScanner::SetHeadTokenRegion(
    /* [in] */ Int32 findIndex)
{
    Int32 tokenStartIndex;
    Int32 tokenEndIndex;
    Boolean setSuccess = FALSE;
    // If no mDelimiter exists, but something exites in this scanner
    if (-1 == findIndex && mPreStartIndex != mBufferLength) {
        tokenStartIndex = mPreStartIndex;
        tokenEndIndex = mBufferLength;
        mFindStartIndex = mBufferLength;
        mMatcher->Region(tokenStartIndex, tokenEndIndex);
        setSuccess = TRUE;
    }
    Int32 startvalue = 0;
    // If the first mDelimiter of scanner is not at the find start position
    if (-1 != findIndex && mPreStartIndex != (mMatcher->Start(&startvalue), startvalue)) {
        tokenStartIndex = mPreStartIndex;
        mMatcher->Start(&tokenEndIndex);
        mMatcher->Start(&mFindStartIndex);
        // set match region and return
        mMatcher->Region(tokenStartIndex, tokenEndIndex);
        setSuccess = TRUE;
    }
    return setSuccess;
}

Int32 CScanner::FindPostDelimiter()
{
    Int32 tokenEndIndex = 0;
    Boolean findComplete = FALSE;
    while (!findComplete) {
        if (mMatcher->Find(&findComplete), findComplete) {
            findComplete = TRUE;
            Int32 endvalue = 0;
            if ((mMatcher->Start(&tokenEndIndex), tokenEndIndex) == mFindStartIndex
                    && tokenEndIndex == (mMatcher->End(&endvalue), endvalue)) {
                findComplete = FALSE;
            }
        }
        else {
            if (!mInputExhausted) {
                ReadMore();
                ResetMatcher();
            }
            else {
                return -1;
            }
        }
    }
    mMatcher->Start(&tokenEndIndex);
    mMatcher->Start(&mFindStartIndex);
    return tokenEndIndex;
}

ECode CScanner::ReadMore()
{
    Int32 oldPosition = 0;
    mBuffer->GetPosition(&oldPosition);
    Int32 oldBufferLength = mBufferLength;
    // Increase capacity if empty space is not enough
    Int32 capacitylen = 0;
    if (mBufferLength >= (mBuffer->GetCapacity(&capacitylen), capacitylen)) {
        ExpandBuffer();
    }

    // Read input resource
    Int32 readCount = 0;
    // try {
    mBuffer->SetLimit(capacitylen);
    mBuffer->SetPosition(oldBufferLength);
    assert(0 && "TODO");
    ECode ec = NOERROR;
    // while ((readCount = mInput->Read(buffer)) == 0) {
    //     // nothing to do here
    // }
    // } catch (IOException e) {
    if (ec == E_IO_EXCEPTION) {
        // Consider the scenario: readable puts 4 chars into
        // buffer and then an IOException is thrown out. In this case,
        // buffer is
        // actually grown, but readable.read() will never return.
        mBuffer->GetPosition(&mBufferLength);
        /*
         * Uses -1 to record IOException occurring, and no more input can be
         * read.
         */
        readCount = -1;
        mLastIOException = ec;
    }
    // }

    mBuffer->Flip();
    mBuffer->SetPosition(oldPosition);
    if (-1 == readCount) {
        mInputExhausted = TRUE;
    }
    else {
        mBufferLength = readCount + mBufferLength;
    }
    return NOERROR;
}

ECode CScanner::ExpandBuffer()
{
    Int32 oldPosition = 0;
    mBuffer->GetPosition(&oldPosition);
    Int32 oldCapacity = 0;
    mBuffer->GetCapacity(&oldCapacity);
    Int32 oldLimit = 0;
    mBuffer->GetLimit(&oldLimit);
    Int32 newCapacity = oldCapacity * DIPLOID;
    AutoPtr< ArrayOf<Char32> > newBuffer = ArrayOf<Char32>::Alloc(newCapacity);
    AutoPtr< ArrayOf<Char32> > outarr;
    mBuffer->GetArray((ArrayOf<Char32>**)&outarr);
    newBuffer->Copy(0, outarr, 0, oldLimit);
    CharBuffer::WrapArray(newBuffer, 0, newCapacity, (ICharBuffer**)&mBuffer);
    mBuffer->SetPosition(oldPosition);
    mBuffer->SetLimit(oldLimit);
    return NOERROR;
}

} // namespace Utility
} // namespace Elastos

