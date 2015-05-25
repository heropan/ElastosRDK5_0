
#include "cmdef.h"
#include "NativeBreakIterator.h"
#include "unicode/ubrk.h"
#include "ElStringByteSink.h"
#include "CStringCharacterIterator.h"
#include "elastos/StringBuilder.h"

using Elastos::Core::StringBuilder;
using Libcore::ICU::EIID_INativeBreakIterator;
using Libcore::ICU::NativeBreakIterator;
using Elastos::Text::IStringCharacterIterator;
using Elastos::Text::CStringCharacterIterator;
using Elastos::Text::EIID_ICharacterIterator;

namespace Libcore {
namespace ICU {

const Int32 NativeBreakIterator::BI_CHAR_INSTANCE = 1;
const Int32 NativeBreakIterator::BI_WORD_INSTANCE = 2;
const Int32 NativeBreakIterator::BI_LINE_INSTANCE = 3;
const Int32 NativeBreakIterator::BI_SENT_INSTANCE = 4;

CAR_INTERFACE_IMPL(NativeBreakIterator, INativeBreakIterator)

NativeBreakIterator::NativeBreakIterator(
    /* [in] */ Int32 address,
    /* [in] */ Int32 type)
    : mAddress(address)
    , mType(type)
{
    AutoPtr<CStringCharacterIterator> stringCharIter;
    ASSERT_SUCCEEDED(CStringCharacterIterator::NewByFriend(String(""),
            (CStringCharacterIterator**)&stringCharIter));
    mCharIter = (ICharacterIterator*)stringCharIter->Probe(EIID_ICharacterIterator);
}

NativeBreakIterator::~NativeBreakIterator()
{
    CloseBreakIteratorImpl(mAddress);
}


ECode NativeBreakIterator::Clone(
    /* [out] */ INativeBreakIterator ** outiter)
{
    VALIDATE_NOT_NULL(outiter);

    Int32 cloneAddr = CloneImpl(mAddress);
    AutoPtr<INativeBreakIterator> clone = new NativeBreakIterator(cloneAddr, mType);
    // The RI doesn't clone the CharacterIterator.
    clone->SetText(mCharIter);
    *outiter = clone;
    INTERFACE_ADDREF(*outiter);

    return NOERROR;
}

ECode NativeBreakIterator::Equals(
    /* [in] */ IInterface * object,
    /* [out] */ Boolean * value)
{
    VALIDATE_NOT_NULL(object)
    VALIDATE_NOT_NULL(value)

    if (object == (IInterface*)this->Probe(EIID_IInterface)) {
        *value = TRUE;
        return NOERROR;
    }
    AutoPtr<INativeBreakIterator> rhs = (INativeBreakIterator*)object->Probe(EIID_INativeBreakIterator);
    if (!rhs) {
        *value = FALSE;
        return NOERROR;
    }
    // TODO: is this sufficient? shouldn't we be checking the underlying rules?
    assert(0 && "TODO");
    // *value = mType == ((NativeBreakIterator*)rhs.Get())->mType && mCharIter.equals(rhs.charIter);
    return NOERROR;
}

ECode NativeBreakIterator::GetHashCode(
    /* [out] */ Int32 * value)
{
    VALIDATE_NOT_NULL(value)

    *value = 42; // No-one uses BreakIterator as a hash key.
    return NOERROR;
}

ECode NativeBreakIterator::Current(
    /* [out] */ Int32 * value)
{
    VALIDATE_NOT_NULL(value)

    *value = CurrentImpl(mAddress);
    return NOERROR;
}

ECode NativeBreakIterator::First(
    /* [out] */ Int32 * value)
{
    VALIDATE_NOT_NULL(value)

    *value = FirstImpl(mAddress);
    return NOERROR;
}

ECode NativeBreakIterator::Following(
    /* [in] */ Int32 offset,
    /* [out] */ Int32 * value)
{
    VALIDATE_NOT_NULL(value)

    *value = FollowingImpl(mAddress, offset);
    return NOERROR;
}

ECode NativeBreakIterator::GetText(
    /* [out] */ ICharacterIterator ** outiter)
{
    VALIDATE_NOT_NULL(outiter)

    Int32 newLoc = CurrentImpl(mAddress);
    Char32 c;
    mCharIter->SetIndex(newLoc, &c);
    *outiter = mCharIter;
    INTERFACE_ADDREF(*outiter);

    return NOERROR;
}

ECode NativeBreakIterator::Last(
    /* [out] */ Int32 * value)
{
    VALIDATE_NOT_NULL(value)

    *value = LastImpl(mAddress);
    return NOERROR;
}

ECode NativeBreakIterator::Next(
    /* [in] */ Int32 n,
    /* [out] */ Int32 * value)
{
    VALIDATE_NOT_NULL(value)

    *value = NextImpl(mAddress, n);
    return NOERROR;
}

ECode NativeBreakIterator::NextEx(
    /* [out] */ Int32 * value)
{
    VALIDATE_NOT_NULL(value)

    *value = NextImpl(mAddress, 1);
    return NOERROR;
}

ECode NativeBreakIterator::Previous(
    /* [out] */ Int32 * value)
{
    VALIDATE_NOT_NULL(value)

    *value = PreviousImpl(mAddress);
    return NOERROR;
}

ECode NativeBreakIterator::SetText(
    /* [in] */ ICharacterIterator * newText)
{
    StringBuilder sb;
    Char32 c;
    for (newText->First(&c); c != ICharacterIterator::DONE; newText->Next(&c)) {
        sb += c;
    }
    SetText(sb.ToString(), newText);
    return NOERROR;
}

ECode NativeBreakIterator::SetTextEx(
    /* [in] */ const String& newText)
{
    AutoPtr<IStringCharacterIterator> stringCharIter;
    ASSERT_SUCCEEDED(CStringCharacterIterator::New(newText, (IStringCharacterIterator**)&stringCharIter));
    SetText(newText, (ICharacterIterator*)stringCharIter);
    return NOERROR;
}

void NativeBreakIterator::SetText(
    /* [in] */ const String& s,
    /* [in] */ ICharacterIterator* it)
{
    mCharIter = it;
    SetTextImpl(mAddress, s);
}

ECode NativeBreakIterator::IsBoundary(
    /* [in] */ Int32 offset,
    /* [out] */ Boolean * value)
{
    VALIDATE_NOT_NULL(value)

    *value = IsBoundaryImpl(mAddress, offset);
    return NOERROR;
}

ECode NativeBreakIterator::Preceding(
    /* [in] */ Int32 offset,
    /* [out] */ Int32 * value)
{
    VALIDATE_NOT_NULL(value)

    *value = PrecedingImpl(mAddress, offset);
    return NOERROR;
}

AutoPtr<INativeBreakIterator> NativeBreakIterator::GetCharacterInstance(
    /* [in] */ ILocale* where)
{
    String str;
    where->ToString(&str);
    AutoPtr<INativeBreakIterator> nbi = new NativeBreakIterator(GetCharacterInstanceImpl(str), BI_CHAR_INSTANCE);
    return nbi;
}

AutoPtr<INativeBreakIterator> NativeBreakIterator::GetLineInstance(
    /* [in] */ ILocale* where)
{
    String str;
    where->ToString(&str);
    AutoPtr<INativeBreakIterator> nbi = new NativeBreakIterator(GetLineInstanceImpl(str), BI_LINE_INSTANCE);
    return nbi;
}

AutoPtr<INativeBreakIterator> NativeBreakIterator::GetSentenceInstance(
    /* [in] */ ILocale* where)
{
    String str;
    where->ToString(&str);
    AutoPtr<INativeBreakIterator> nbi = new NativeBreakIterator(GetSentenceInstanceImpl(str), BI_SENT_INSTANCE);
    return nbi;
}

AutoPtr<INativeBreakIterator> NativeBreakIterator::GetWordInstance(
    /* [in] */ ILocale* where)
{
    String str;
    where->ToString(&str);
    AutoPtr<INativeBreakIterator> nbi = new NativeBreakIterator(GetWordInstanceImpl(str), BI_WORD_INSTANCE);
    return nbi;
}

/**
 * ICU4C 4.6 doesn't let us update the pointers inside a UBreakIterator to track our char[] as it
 * moves around the heap. This class pins the char[] for the lifetime of the
 * java.text.BreakIterator. It also holds a global reference to the java.lang.String that owns the
 * char[] so that the char[] can't be GCed.
 */
class BreakIteratorPeer : ElRefBase
{
public:

    UInt32 AddRef()
    {
        return ElRefBase::AddRef();
    }

    UInt32 Release()
    {
        return ElRefBase::Release();
    }

    static AutoPtr<BreakIteratorPeer> fromAddress(
        /* [in] */ Int32 address)
    {
        return reinterpret_cast<BreakIteratorPeer*>(static_cast<uintptr_t>(address));
    }

    uintptr_t toAddress()
    {
        AddRef();
        return reinterpret_cast<uintptr_t>(this);
    }

    BreakIteratorPeer(
        /* [in] */ UBreakIterator* it)
        : mIt(it)
        , mString(String(NULL))
        , mChars(NULL)
    {}

    ECode setText(
        /* [in] */ const String& s)
    {
        releaseString();

        mString = s;
        mChars = mString.string();
        if (mChars == NULL) {
            return NOERROR;
        }

        size_t charCount = mString.GetLength();
        UErrorCode status = U_ZERO_ERROR;
        ubrk_setText(mIt, (const UChar*)mChars, charCount, &status);
        return maybeThrowIcuException(status);
    }

    AutoPtr<BreakIteratorPeer> clone()
    {
        UErrorCode status = U_ZERO_ERROR;
        Int32 bufferSize = U_BRK_SAFECLONE_BUFFERSIZE;
        UBreakIterator* it = ubrk_safeClone(mIt, NULL, &bufferSize, &status);
        if (FAILED(maybeThrowIcuException(status))) {
            return NULL;
        }
        AutoPtr<BreakIteratorPeer> result = new BreakIteratorPeer(it);
        if (!mString.IsNull()) {
            result->setText(mString);
        }
        return result;
    }

    void close() {
        if (mIt != NULL) {
            ubrk_close(mIt);
            mIt = NULL;
        }
        releaseString();
    }

    ~BreakIteratorPeer() {
        if (mIt != NULL || mString != NULL) {
            // LOG_ALWAYS_FATAL("BreakIteratorPeer deleted but not closed");
            Release();
        }
    }

    UBreakIterator* breakIterator()
    {
        return mIt;
    }

private:
    UBreakIterator* mIt;

    String mString;
    const char* mChars;

    /**
    * Checks if an error has occurred, throwing a suitable exception if so.
    * @param env JNI environment
    * @param errorCode code to determine if it is an error
    * @return 0 if errorCode is not an error, 1 if errorCode is an error, but the
    *         creation of the exception to be thrown fails
     * @exception thrown if errorCode represents an error
    */
    ECode maybeThrowIcuException(UErrorCode errorCode)
    {
    //    const char* message = u_errorName(errorCode);
        if (errorCode <= U_ZERO_ERROR || errorCode >= U_ERROR_LIMIT) {
            return NOERROR;
        }

        switch (errorCode) {
        case U_ILLEGAL_ARGUMENT_ERROR:
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
        case U_INDEX_OUTOFBOUNDS_ERROR:
        case U_BUFFER_OVERFLOW_ERROR:
            return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
        case U_UNSUPPORTED_ERROR:
            return E_UNSUPPORTED_OPERATION_EXCEPTION;
        default:
            return E_RUNTIME_EXCEPTION;
        }
    }

    void releaseString()
    {
        if (!mString.IsNull()) {
            mString = String(NULL);
            mChars = NULL;
        }
    }

    // Disallow copy and assignment.
    BreakIteratorPeer(const BreakIteratorPeer&);
    void operator=(const BreakIteratorPeer&);
};

static UBreakIterator* breakIterator(
    /* [in] */ Int32 address)
{
    AutoPtr<BreakIteratorPeer> peer = BreakIteratorPeer::fromAddress(address);
    return peer->breakIterator();
}

static Int32 makeIterator(
    /* [in] */ const String& locale,
    /* [in] */ UBreakIteratorType type)
{
    UErrorCode status = U_ZERO_ERROR;
    if (locale.IsNull()) {
        return 0;
    }
    UBreakIterator* it = ubrk_open(type, locale.string(), NULL, 0, &status);
    // if (FAILED(MaybeThrowIcuException(status))) {
    if (U_ZERO_ERROR < status && status < U_ERROR_LIMIT) {
        return 0;
    }
    return (new BreakIteratorPeer(it))->toAddress();
}

Int32 NativeBreakIterator::GetCharacterInstanceImpl(
    /* [in] */ const String& locale)
{
    return makeIterator(locale, UBRK_CHARACTER);
}

Int32 NativeBreakIterator::GetWordInstanceImpl(
    /* [in] */ const String& locale)
{
    return makeIterator(locale, UBRK_WORD);
}

Int32 NativeBreakIterator::GetLineInstanceImpl(
    /* [in] */ const String& locale)
{
    return makeIterator(locale, UBRK_LINE);
}

Int32 NativeBreakIterator::GetSentenceInstanceImpl(
    /* [in] */ const String& locale)
{
    return makeIterator(locale, UBRK_SENTENCE);
}

void NativeBreakIterator::CloseBreakIteratorImpl(
    /* [in] */ Int32 address)
{
    AutoPtr<BreakIteratorPeer> peer = BreakIteratorPeer::fromAddress(address);
    peer->close();
    //delete peer;
}

Int32 NativeBreakIterator::CloneImpl(
    /* [in] */ Int32 address)
{
    AutoPtr<BreakIteratorPeer> peer = BreakIteratorPeer::fromAddress(address);
    return peer->clone()->toAddress();
}

void NativeBreakIterator::SetTextImpl(
    /* [in] */ Int32 address,
    /* [in] */ const String& text)
{
    AutoPtr<BreakIteratorPeer> peer = BreakIteratorPeer::fromAddress(address);
    peer->setText(text);
}

Boolean NativeBreakIterator::IsBoundaryImpl(
    /* [in] */ Int32 address,
    /* [in] */ Int32 offset)
{
    return ubrk_isBoundary(breakIterator(address), offset);
}

Int32 NativeBreakIterator::NextImpl(
    /* [in] */ Int32 address,
    /* [in] */ Int32 n)
{
    UBreakIterator* bi = breakIterator(address);
    if (n < 0) {
        while (n++ < -1) {
            ubrk_previous(bi);
        }
        return ubrk_previous(bi);
    }
    else if (n == 0) {
        return ubrk_current(bi);
    }
    else {
        while (n-- > 1) {
            ubrk_next(bi);
        }
        return ubrk_next(bi);
    }
    return -1;
}

Int32 NativeBreakIterator::PrecedingImpl(
    /* [in] */ Int32 address,
    /* [in] */ Int32 offset)
{
    return ubrk_preceding(breakIterator(address), offset);
}

Int32 NativeBreakIterator::CurrentImpl(
    /* [in] */ Int32 address)
{
    return ubrk_current(breakIterator(address));
}

Int32 NativeBreakIterator::FirstImpl(
    /* [in] */ Int32 address)
{
    return ubrk_first(breakIterator(address));
}

Int32 NativeBreakIterator::FollowingImpl(
    /* [in] */ Int32 address,
    /* [in] */ Int32 offset)
{
    return ubrk_following(breakIterator(address), offset);
}

Int32 NativeBreakIterator::PreviousImpl(
    /* [in] */ Int32 address)
{
    return ubrk_previous(breakIterator(address));
}

Int32 NativeBreakIterator::LastImpl(
    /* [in] */ Int32 address)
{
    return ubrk_last(breakIterator(address));
}

} // namespace ICU
} // namespace Libcore
