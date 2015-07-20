
#include "ext/frameworkext.h"
#include "text/TextUtils.h"
#include <elastos/core/Character.h>
#include <elastos/core/StringBuilder.h>
#include "R.h"
#include "text/MeasuredText.h"
#include "text/TextDirectionHeuristics.h"
#ifdef DROID_CORE
#include "text/CSpannedString.h"
#include "text/CSpannableString.h"
#include "text/CSpannableStringBuilder.h"
#include "text/CAnnotation.h"
#include "text/style/CAlignmentSpanStandard.h"
#include "text/style/CForegroundColorSpan.h"
#include "text/style/CRelativeSizeSpan.h"
#include "text/style/CScaleXSpan.h"
#include "text/style/CStrikethroughSpan.h"
#include "text/style/CUnderlineSpan.h"
#include "text/style/CStyleSpan.h"
#include "text/style/CBulletSpan.h"
#include "text/style/CQuoteSpan.h"
#include "text/style/CLeadingMarginSpanStandard.h"
#include "text/style/CURLSpan.h"
#include "text/style/CBackgroundColorSpan.h"
#include "text/style/CTypefaceSpan.h"
#include "text/style/CSuperscriptSpan.h"
#include "text/style/CSubscriptSpan.h"
#include "text/style/CAbsoluteSizeSpan.h"
#include "text/style/CTextAppearanceSpan.h"
#include "text/style/CSuggestionSpan.h"
#include "text/style/CSpellCheckSpan.h"
#include "text/style/CSuggestionRangeSpan.h"
#include "text/style/CEasyEditSpan.h"
#include "text/style/CLocaleSpan.h"
#include "content/res/CResources.h"
#include "content/res/CResourcesHelper.h"
#endif

using Elastos::Core::Character;
using Elastos::Core::CStringWrapper;
using Elastos::Core::StringBuilder;
using Elastos::Core::IStringBuffer;
using Elastos::Core::EIID_ICharSequence;
using Elastos::Core::IStringBuilder;
using Elastos::Droid::R;
using namespace Elastos::Droid::Text::Style;
using Elastos::Droid::Content::Res::IResources;
using Elastos::Droid::Content::Res::CResources;
using Elastos::Droid::Content::Res::IResourcesHelper;
using Elastos::Droid::Content::Res::CResourcesHelper;
using Elastos::Droid::View::IView;
using Libcore::ICU::IICUUtil;
using Libcore::ICU::CICUUtil;
using Libcore::ICU::ILocaleHelper;
using Libcore::ICU::CLocaleHelper;

namespace Elastos {
namespace Droid {
namespace Text {

const Char32 TextUtils::FIRST_RIGHT_TO_LEFT;
const Char32 TextUtils::ZWNBS_CHAR;

Mutex TextUtils::sLock;
AutoPtr< ArrayOf<Char32> > TextUtils::sTemp;
AutoPtr< ArrayOf<String> > TextUtils::EMPTY_STRING_ARRAY = ArrayOf<String>::Alloc(0);
String TextUtils::ARAB_SCRIPT_SUBTAG = String("Arab");
String TextUtils::HEBR_SCRIPT_SUBTAG = String("Hebr");

/**
 * Initializes the splitter. setString may be called later.
 * @param delimiter the delimeter on which to split
 */
TextUtils::SimpleStringSplitter::SimpleStringSplitter(
    /* [in] */ Char32 delimiter)
    : mDelimiter(delimiter)
    , mPosition(0)
    , mLength(0)
{}

/**
 * Sets the string to split
 * @param string the string to split
 */
ECode TextUtils::SimpleStringSplitter::SetString(
    /* [in] */ const String& string)
{
    mString = string;
    mPosition = 0;
    mLength = mString.GetLength();
    return NOERROR;
}

//        public Iterator<String> iterator();

ECode TextUtils::SimpleStringSplitter::HasNext(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = mPosition < mLength;
    return NOERROR;
}

ECode TextUtils::SimpleStringSplitter::Next(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);

    Int32 end = mString.IndexOf(mDelimiter, mPosition);
    if (end == -1) {
        end = mLength;
    }
    if (mPosition > end) {
        *str = NULL;
        return E_STRING_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    *str = mString.Substring(mPosition, end);
    mPosition = end + 1; // Skip the delimiter.
    return NOERROR;
}

ECode TextUtils::SimpleStringSplitter::Remove()
{
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

void TextUtils::GetChars(
    /* [in] */ ICharSequence* s,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ ArrayOf<Char32>* dest,
    /* [in] */ Int32 destoff)
{
    assert(s != NULL);
    String str;
    if (IStringBuffer::Probe(s)) {
        IStringBuffer* sb = IStringBuffer::Probe(s);
        sb->GetChars(start, end, dest, destoff);
        sb->ToString(&str);
    }
    else if (IStringBuilder::Probe(s)) {
        IStringBuilder* sb = IStringBuilder::Probe(s);
        sb->GetChars(start, end, dest, destoff);
        sb->ToString(&str);

    }
    else if (IGetChars::Probe(s)) {
        IGetChars* sb = IGetChars::Probe(s);
        sb->GetChars(start, end, dest, destoff);
        sb->ToString(&str);

    }
    else {
        for (Int32 i = start; i < end; i++) {
            Char32 ch;
            s->GetCharAt(i, &ch);
            (*dest)[destoff++] = ch;
        }
    }
}

Int32 TextUtils::IndexOf(
    /* [in] */ ICharSequence* s,
    /* [in] */ Char32 ch)
{
    return IndexOf(s, ch, 0);
}

Int32 TextUtils::IndexOf(
    /* [in] */ ICharSequence* s,
    /* [in] */ Char32 ch,
    /* [in] */ Int32 start)
{
    Int32 len;
    s->GetLength(&len);
    return IndexOf(s, ch, start, len);

}

Int32 TextUtils::IndexOf(
    /* [in] */ ICharSequence* s,
    /* [in] */ Char32 ch,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    assert(s != NULL);

    if (IGetChars::Probe(s) || IStringBuffer::Probe(s) ||
        IStringBuilder::Probe(s) /* TODO || c == String.class */) {
        const Int32 INDEX_INCREMENT = 500;
        AutoPtr< ArrayOf<Char32> > temp = Obtain(INDEX_INCREMENT);

        while (start < end) {
            Int32 segend = start + INDEX_INCREMENT;
            if (segend > end)
                segend = end;

            GetChars(s, start, segend, temp, 0);

            Int32 count = segend - start;
            for (Int32 i = 0; i < count; i++) {
                if ((*temp)[i] == ch) {
                    Recycle(temp);
                    return i + start;
                }
            }

            start = segend;
        }

        Recycle(temp);
        return -1;
    }

    for (Int32 i = start; i < end; i++) {
        Char32 ch_;
        s->GetCharAt(i, &ch_);
        if (ch_ == ch) {
            return i;
        }
    }

    return -1;
}

Int32 TextUtils::LastIndexOf(
    /* [in] */ ICharSequence* s,
    /* [in] */ Char32 ch)
{
    Int32 len;
    s->GetLength(&len);
    return LastIndexOf(s, ch, len - 1);
}

Int32 TextUtils::LastIndexOf(
    /* [in] */ ICharSequence* s,
    /* [in] */ Char32 ch,
    /* [in] */ Int32 last)
{
    // assert(0);
//    Class<? extends CharSequence> c = s.getClass();
//
   // if (c == String.class)
   //     return ((String) s).lastIndexOf(ch, last);
    String str;
    s->ToString(&str);
    Int32 index = str.LastIndexOf(ch, last);
    if (index != -1) {
        return index;
    }

    return LastIndexOf(s, ch, 0, last);
}

Int32 TextUtils::LastIndexOf(
    /* [in] */ ICharSequence* s,
    /* [in] */ Char32 ch,
    /* [in] */ Int32 start,
    /* [in] */ Int32 last)
{
    if (last < 0) {
        return -1;
    }
    Int32 len;
    s->GetLength(&len);
    if (last >= len) {
        last = len - 1;
    }

    Int32 end = last + 1;

//    assert(0);
//    Class<? extends CharSequence> c = s.getClass();
//
   if (IGetChars::Probe(s) != NULL || IStringBuffer::Probe(s) != NULL ||
       IStringBuilder::Probe(s) != NULL/* || c == String.class*/) {
       const Int32 INDEX_INCREMENT = 500;
       AutoPtr<ArrayOf<Char32> > temp = Obtain(INDEX_INCREMENT);

       while (start < end) {
           Int32 segstart = end - INDEX_INCREMENT;
           if (segstart < start)
               segstart = start;

           GetChars(s, segstart, end, temp, 0);

           Int32 count = end - segstart;
           for (Int32 i = count - 1; i >= 0; i--) {
               if ((*temp)[i] == ch) {
                   Recycle(temp);
                   return i + segstart;
               }
           }

           end = segstart;
       }

       Recycle(temp);
       return -1;
   }

    for (Int32 i = end - 1; i >= start; i--) {
        Char32 ch_;
        s->GetCharAt(i, &ch_);
        if (ch_ == ch) {
            return i;
        }
    }

    return -1;
}

Int32 TextUtils::IndexOf(
    /* [in] */ ICharSequence* s,
    /* [in] */ ICharSequence* needle)
{
    Int32 len;
    s->GetLength(&len);
    return IndexOf(s, needle, 0, len);
}

Int32 TextUtils::IndexOf(
    /* [in] */ ICharSequence* s,
    /* [in] */ ICharSequence* needle,
    /* [in] */ Int32 start)
{
    Int32 len;
    s->GetLength(&len);
    return IndexOf(s, needle, start, len);
}

Int32 TextUtils::IndexOf(
    /* [in] */ ICharSequence* s,
    /* [in] */ ICharSequence* needle,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    Int32 nlen;
    needle->GetLength(&nlen);
    if (nlen == 0) {
        return start;
    }

    Char32 ch;
    needle->GetCharAt(0, &ch);

    for (;;) {
        start = IndexOf(s, ch, start);
        if (start > end - nlen) {
            break;
        }

        if (start < 0) {
            return -1;
        }

        if (RegionMatches(s, start, needle, 0, nlen)) {
            return start;
        }

        start++;
    }
    return -1;
}

Boolean TextUtils::RegionMatches(
    /* [in] */ ICharSequence* one,
    /* [in] */ Int32 toffset,
    /* [in] */ ICharSequence* two,
    /* [in] */ Int32 ooffset,
    /* [in] */ Int32 len)
{
    AutoPtr< ArrayOf<Char32> > temp = Obtain(2 * len);

   GetChars(one, toffset, toffset + len, temp, 0);
   GetChars(two, ooffset, ooffset + len, temp, len);

    Boolean match = TRUE;
    for (Int32 i = 0; i < len; i++) {
        if ((*temp)[i] != (*temp)[i + len]) {
            match = FALSE;
            break;
        }
    }

    Recycle(temp);
    return match;
}

/**
 * Create a new String object containing the given range of characters
 * from the source string.  This is different than simply calling
 * {@link CharSequence#subSequence(int, int) CharSequence.subSequence}
 * in that it does not preserve any style runs in the source sequence,
 * allowing a more efficient implementation.
 */
String TextUtils::Substring(
    /* [in] */ ICharSequence* source,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    if (IStringBuilder::Probe(source) != NULL) {
        IStringBuilder* builder = IStringBuilder::Probe(source);
        String str;
        builder->Substring(start, end, &str);
        return str;
    }
    else if (IStringBuffer::Probe(source) != NULL) {
        IStringBuffer* builder = IStringBuffer::Probe(source);
        String str;
        builder->Substring(start, end, &str);
        return str;
    }

    AutoPtr<ArrayOf<Char32> > temp = Obtain(end - start);
    GetChars(source, start, end, temp, 0);
    String ret = String(*temp, 0, end - start);
    Recycle(temp);

    return ret;
}

/**
 * Returns list of multiple {@link CharSequence} joined into a single
 * {@link CharSequence} separated by localized delimiter such as ", ".
 *
 * @hide
 */
//    public static CharSequence join(Iterable<CharSequence> list);

/**
 * Returns a string containing the tokens joined by delimiters.
 * @param tokens an array objects to be joined. Strings will be formed from
 *     the objects by calling object.toString().
 */
String TextUtils::Join(
    /* [in] */ ICharSequence* delimiter,
    /* [in] */ IObjectContainer* tokens)
{
    StringBuilder sb;
    Boolean firstTime = TRUE;
    AutoPtr<IObjectEnumerator> enumerator;
    tokens->GetObjectEnumerator((IObjectEnumerator**)&enumerator);
    Boolean hasNext = FALSE;
    while (enumerator->MoveNext(&hasNext), hasNext) {
        AutoPtr<IInterface> token;
        enumerator->Current((IInterface**)&token);
        if (firstTime) {
            firstTime = FALSE;
        } else {
            sb.AppendCharSequence(delimiter);
        }
        sb.AppendObject(token);
    }
    return sb.ToString();
}

/**
 * Returns a string containing the tokens joined by delimiters.
 * @param tokens an array objects to be joined. Strings will be formed from
 *     the objects by calling object.toString().
 */
//    public static String join(CharSequence delimiter, Iterable tokens);

/**
 * String.split() returns [''] when the string to be split is empty. This returns []. This does
 * not remove any empty strings from the result. For example split("a,", ","  ) returns {"a", ""}.
 *
 * @param text the string to split
 * @param expression the regular expression to match
 * @return an array of strings. The array will be empty if text is empty
 *
 * @throws NullPointerException if expression or text is NULL
 */
//    public static String[] split(String text, String expression);

/**
 * Splits a string on a pattern. String.split() returns [''] when the string to be
 * split is empty. This returns []. This does not remove any empty strings from the result.
 * @param text the string to split
 * @param pattern the regular expression to match
 * @return an array of strings. The array will be empty if text is empty
 *
 * @throws NullPointerException if expression or text is NULL
 */
//    public static String[] split(String text, Pattern pattern);

AutoPtr<ICharSequence> TextUtils::StringOrSpannedString(
    /* [in] */ ICharSequence* source)
{
    if (source == NULL)
        return NULL;

    if (ISpannedString::Probe(source)) {
        return source;
    } else if (ISpanned::Probe(source)) {
        AutoPtr<ISpannedString> ss;
        CSpannedString::New(source, (ISpannedString**)&ss);
        AutoPtr<ICharSequence> result = ICharSequence::Probe(ss.Get());
        return result;
    }

    String str;
    source->ToString(&str);
    AutoPtr<ICharSequence> result;
    CStringWrapper::New(str, (ICharSequence**)&result);
    return result;
}

/**
 * Returns true if the string is NULL or 0-length.
 * @param str the string to be examined
 * @return true if str is NULL or zero length
 */
Boolean TextUtils::IsEmpty(
    /* [in] */ ICharSequence* str)
{
    if (str == NULL) return TRUE;

    Int32 len;
    str->GetLength(&len);
    if (len == 0) return TRUE;

    return FALSE;
}

Boolean TextUtils::IsEmpty(
    /* [in] */ const String& str)
{
    return str.IsNullOrEmpty();
}

/**
 * Returns the length that the specified CharSequence would have if
 * spaces and control characters were trimmed from the start and end,
 * as by {@link String#trim}.
 */
Int32 TextUtils::GetTrimmedLength(
    /* [in] */ ICharSequence* s)
{
    Int32 len;
    s->GetLength(&len);

    Char32 ch;
    Int32 start = 0;
    while (start < len && (s->GetCharAt(start, &ch), ch <= ' ')) {
        start++;
    }

    Int32 end = len;
    while (end > start && (s->GetCharAt(end - 1, &ch), ch <= ' ')) {
        end--;
    }

    return end - start;
}

/**
 * Returns true if a and b are equal, including if they are both NULL.
 * <p><i>Note: In platform versions 1.1 and earlier, this method only worked well if
 * both the arguments were instances of String.</i></p>
 * @param a first CharSequence to check
 * @param b second CharSequence to check
 * @return true if a and b are equal
 */
Boolean TextUtils::Equals(
    /* [in] */ ICharSequence* a,
    /* [in] */ ICharSequence* b)
{
    if (a == b) return TRUE;
    Int32 len1, len2;
    if (a != NULL && b != NULL && (a->GetLength(&len1), b->GetLength(&len2), len1 == len2)) {
//        if (a instanceof String && b instanceof String) {
//            return a.equals(b);
//        } else {
            for (Int32 i = 0; i < len1; i++) {
                Char32 ch1, ch2;
                a->GetCharAt(i, &ch1);
                b->GetCharAt(i, &ch2);
                if (ch1 != ch2) return FALSE;
            }
            return TRUE;
//        }
    }
    return FALSE;
}

Boolean TextUtils::Equals(
    /* [in] */ const String& a,
    /* [in] */ const String& b)
{
    if (a.IsNull() && b.IsNull())
        return TRUE;

    if (a.IsNull() || b.IsNull())
        return FALSE;

    return a.Equals(b);
}

// XXX currently this only reverses chars, not spans
AutoPtr<ICharSequence> TextUtils::GetReverse(
    /* [in] */ ICharSequence* source,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    assert(0);
    //    return new Reverser(source, start, end);
    return NULL;
}

/**
 * Flatten a CharSequence and whatever styles can be copied across processes
 * into the parcel.
 */
ECode TextUtils::WriteToParcel(
    /* [in] */ ICharSequence* cs,
    /* [in] */ IParcel* p)
{
    ISpanned* sp = ISpanned::Probe(cs);
    if (sp) {
        FAIL_RETURN(p->WriteInt32(0));
        String str;
        cs->ToString(&str);
        FAIL_RETURN(p->WriteString(str));


        AutoPtr<ISpanned> sp = ISpanned::Probe(cs);
        AutoPtr<ArrayOf<IInterface*> > os;
        Int32 len;
        cs->GetLength(&len);
        sp->GetSpans(0, len, EIID_IInterface, (ArrayOf<IInterface*>**)&os);

        // note to people adding to this: check more specific types
        // before more generic types.  also notice that it uses
        // "if" instead of "else if" where there are interfaces
        // so one object can be several.

        len = os->GetLength();
        for (Int32 i = 0; i < len; i++) {
            AutoPtr<IInterface> o = (*os)[i];
            AutoPtr<IInterface> prop = (*os)[i];

            AutoPtr<ICharacterStyle> temp = ICharacterStyle::Probe(prop);
            if (temp != NULL) {
                prop = NULL;
                temp->GetUnderlying((ICharacterStyle**)&prop);
            }

            AutoPtr<IParcelableSpan> ps = IParcelableSpan::Probe(prop);
            if (ps != NULL) {
                Int32 typeId;
                ps->GetSpanTypeId(&typeId);
                FAIL_RETURN(p->WriteInt32(typeId));
                FAIL_RETURN(IParcelable::Probe(ps)->WriteToParcel(p));
                FAIL_RETURN(WriteWhere(p, sp, o));
            }
        }

        FAIL_RETURN(p->WriteInt32(0));
   }
   else {
       FAIL_RETURN(p->WriteInt32(1));
       if (cs != NULL) {
            String str;
            cs->ToString(&str);
            FAIL_RETURN(p->WriteString(str));
       }
       else {
            FAIL_RETURN(p->WriteString(String(NULL)));
       }
   }

   return NOERROR;
}

ECode TextUtils::CHAR_SEQUENCE_CREATOR::CreateFromParcel(
    /* [in] */ IParcel* p,
    /* [out] */ ICharSequence** csq)
{
    VALIDATE_NOT_NULL(csq);
    *csq = NULL;

    AutoPtr<ICharSequence> cs;
    Int32 kind;
    FAIL_RETURN(p->ReadInt32(&kind));

    String string;
    FAIL_RETURN(p->ReadString(&string));
    if (string.IsNull()) {
        return NOERROR;
    }

    CStringWrapper::New(string, (ICharSequence**)&cs);
    if (kind == 1) {
        *csq = cs;
        REFCOUNT_ADD(*csq);
        return NOERROR;
    }

    AutoPtr<ISpannableString> sp;
    CSpannableString::New(cs, (ISpannableString**)&sp);

    while (TRUE) {
        FAIL_RETURN(p->ReadInt32(&kind));

        if (kind == 0)
            break;

        AutoPtr<IInterface> object;
        switch (kind) {
        case ITextUtils::ALIGNMENT_SPAN:
            CAlignmentSpanStandard::New(p, (IAlignmentSpan**)&object);
            FAIL_RETURN(ReadSpan(p, ISpannable::Probe(sp), object));
            break;

        case ITextUtils::FOREGROUND_COLOR_SPAN:
            CForegroundColorSpan::New(p, (IForegroundColorSpan**)&object);
            FAIL_RETURN(ReadSpan(p, ISpannable::Probe(sp), object));
            break;

        case ITextUtils::RELATIVE_SIZE_SPAN:
            CRelativeSizeSpan::New(p, (IRelativeSizeSpan**)&object);
            FAIL_RETURN(ReadSpan(p, ISpannable::Probe(sp), object));
            break;

        case ITextUtils::SCALE_X_SPAN:
            CScaleXSpan::New(p, (IScaleXSpan**)&object);
            FAIL_RETURN(ReadSpan(p, ISpannable::Probe(sp), object));
            break;

        case ITextUtils::STRIKETHROUGH_SPAN:
            CStrikethroughSpan::New(p, (IStrikethroughSpan**)&object);
            FAIL_RETURN(ReadSpan(p, ISpannable::Probe(sp), object));
            break;

        case ITextUtils::UNDERLINE_SPAN:
            CUnderlineSpan::New(p, (IUnderlineSpan**)&object);
            FAIL_RETURN(ReadSpan(p, ISpannable::Probe(sp), object));
            break;

        case ITextUtils::STYLE_SPAN:
            CStyleSpan::New(p, (IStyleSpan**)&object);
            FAIL_RETURN(ReadSpan(p, ISpannable::Probe(sp), object));
            break;

        case ITextUtils::BULLET_SPAN:
            CBulletSpan::New(p, (IBulletSpan**)&object);
            FAIL_RETURN(ReadSpan(p, ISpannable::Probe(sp), object));
            break;

        case ITextUtils::QUOTE_SPAN:
            CQuoteSpan::New(p, (IQuoteSpan**)&object);
            FAIL_RETURN(ReadSpan(p, ISpannable::Probe(sp), object));
            break;

        case ITextUtils::LEADING_MARGIN_SPAN:
            CLeadingMarginSpanStandard::New(p, (ILeadingMarginSpan**)&object);
            FAIL_RETURN(ReadSpan(p, ISpannable::Probe(sp), object));
        break;

        case ITextUtils::URL_SPAN:
            CURLSpan::New(p, (IURLSpan**)&object);
            FAIL_RETURN(ReadSpan(p, ISpannable::Probe(sp), object));
            break;

        case ITextUtils::BACKGROUND_COLOR_SPAN:
            CBackgroundColorSpan::New(p, (IBackgroundColorSpan**)&object);
            FAIL_RETURN(ReadSpan(p, ISpannable::Probe(sp), object));
            break;

        case ITextUtils::TYPEFACE_SPAN:
            CTypefaceSpan::New(p, (ITypefaceSpan**)&object);
            FAIL_RETURN(ReadSpan(p, ISpannable::Probe(sp), object));
            break;

        case ITextUtils::SUPERSCRIPT_SPAN:
            CSuperscriptSpan::New(p, (ISuperscriptSpan**)&object);
            FAIL_RETURN(ReadSpan(p, ISpannable::Probe(sp), object));
            break;

        case ITextUtils::SUBSCRIPT_SPAN:
            CSubscriptSpan::New(p, (ISubscriptSpan**)&object);
            FAIL_RETURN(ReadSpan(p, ISpannable::Probe(sp), object));
            break;

        case ITextUtils::ABSOLUTE_SIZE_SPAN:
            CAbsoluteSizeSpan::New(p, (IAbsoluteSizeSpan**)&object);
            FAIL_RETURN(ReadSpan(p, ISpannable::Probe(sp), object));
            break;

        case ITextUtils::TEXT_APPEARANCE_SPAN:
            CTextAppearanceSpan::New(p, (ITextAppearanceSpan**)&object);
            FAIL_RETURN(ReadSpan(p, ISpannable::Probe(sp), object));
            break;

        case ITextUtils::ANNOTATION:
            CAnnotation::New(p, (IAnnotation**)&object);
            FAIL_RETURN(ReadSpan(p, ISpannable::Probe(sp), object));
            break;

        case ITextUtils::SUGGESTION_SPAN:
            CSuggestionSpan::New(p, (ISuggestionSpan**)&object);
            FAIL_RETURN(ReadSpan(p, ISpannable::Probe(sp), object));
            break;

        case ITextUtils::SPELL_CHECK_SPAN:
            CSpellCheckSpan::New(p, (ISpellCheckSpan**)&object);
            FAIL_RETURN(ReadSpan(p, ISpannable::Probe(sp), object));
            break;

        case ITextUtils::SUGGESTION_RANGE_SPAN:
            CSuggestionRangeSpan::New(p, (ISuggestionRangeSpan**)&object);
            FAIL_RETURN(ReadSpan(p, ISpannable::Probe(sp), object));
            break;

        case ITextUtils::EASY_EDIT_SPAN:
            CEasyEditSpan::New((IEasyEditSpan**)&object);
            FAIL_RETURN(ReadSpan(p, ISpannable::Probe(sp), object));
            break;

        case ITextUtils::LOCALE_SPAN:
            CLocaleSpan::New(p, (ILocaleSpan**)&object);
            FAIL_RETURN(ReadSpan(p, ISpannable::Probe(sp), object));
            break;

        default:
            // throw new RuntimeException("bogus span encoding " + kind);
            return E_RUNTIME_EXCEPTION;
        }
    }

    *csq = ICharSequence::Probe(sp);
    REFCOUNT_ADD(*csq);
    return NOERROR;
}

AutoPtr<ArrayOf<ICharSequence*> > TextUtils::CHAR_SEQUENCE_CREATOR::NewArray(
    /* [in] */ Int32 size)
{
    AutoPtr<ArrayOf<ICharSequence*> > array = ArrayOf<ICharSequence*>::Alloc(size);
    return array;
}

/**
 * Debugging tool to print the spans in a CharSequence.  The output will
 * be printed one span per line.  If the CharSequence is not a Spanned,
 * then the entire string will be printed on a single line.
 */
//    public static void dumpSpans(CharSequence cs, Printer printer, String prefix);

/**
 * Return a new CharSequence in which each of the source strings is
 * replaced by the corresponding element of the destinations.
 */
//    public static CharSequence replace(CharSequence template,
//                                       String[] sources,
//                                       CharSequence[] destinations);

/**
 * Replace instances of "^1", "^2", etc. in the
 * <code>template</code> CharSequence with the corresponding
 * <code>values</code>.  "^^" is used to produce a single caret in
 * the output.  Only up to 9 replacement values are supported,
 * "^10" will be produce the first replacement value followed by a
 * '0'.
 *
 * @param template the input text containing "^1"-style
 * placeholder values.  This object is not modified; a copy is
 * returned.
 *
 * @param values CharSequences substituted into the template.  The
 * first is substituted for "^1", the second for "^2", and so on.
 *
 * @return the new CharSequence produced by doing the replacement
 *
 * @throws IllegalArgumentException if the template requests a
 * value that was not provided, or if more than 9 values are
 * provided.
 */
//    public static CharSequence expandTemplate(CharSequence template,
//                                              CharSequence... values);

Int32 TextUtils::GetOffsetBefore(
    /* [in] */ ICharSequence* text,
    /* [in] */ Int32 offset)
{
    if (offset == 0) {
        return 0;
    }
    if (offset == 1) {
        return 0;
    }

    Char32 c;
    text->GetCharAt(offset - 1, &c);

    if (c >= 0xDC00/*'\uDC00'*/ && c <= 0xDFFF/*'\uDFFF'*/) {
        Char32 c1;
        text->GetCharAt(offset - 2, &c1);

        if (c1 >= 0xD800/*'\uD800'*/ && c1 <= 0xDBFF/*'\uDBFF'*/) {
            offset -= 2;
        }
        else {
            offset -= 1;
        }
    }
    else {
        offset -= 1;
    }

    if (ISpanned::Probe(text) != NULL) {
        AutoPtr< ArrayOf<IInterface*> > spans;
        AutoPtr<ISpanned> spanned = ISpanned::Probe(text);
        spanned->GetSpans(offset, offset, EIID_IReplacementSpan, (ArrayOf<IInterface*>**)&spans);

        for (Int32 i = 0; i < spans->GetLength(); i++) {
            Int32 start, end;
            spanned->GetSpanStart((*spans)[i], &start);
            spanned->GetSpanEnd((*spans)[i], &end);

            if (start < offset && end > offset) {
                offset = start;
            }
        }
    }

    return offset;
}

Int32 TextUtils::GetOffsetAfter(
    /* [in] */ ICharSequence* text,
    /* [in] */ Int32 offset)
{
    Int32 len;
    text->GetLength(&len);

    if (offset == len) {
        return len;
    }
    if (offset == len - 1) {
        return len;
    }

    Char32 c;
    text->GetCharAt(offset, &c);

    if (c >= 0xD800/*'\uD800'*/ && c <= 0xDBFF/*'\uDBFF'*/) {
        Char32 c1;
        text->GetCharAt(offset + 1, &c1);

        if (c1 >= 0xDC00/*'\uDC00'*/ && c1 <= 0xDFFF/*'\uDFFF'*/) {
            offset += 2;
        }
        else {
            offset += 1;
        }
    }
    else {
        offset += 1;
    }

    if (ISpanned::Probe(text) != NULL) {
        AutoPtr< ArrayOf<IInterface*> > spans;
        AutoPtr<ISpanned> spanned = ISpanned::Probe(text);
        spanned->GetSpans(offset, offset, EIID_IReplacementSpan, (ArrayOf<IInterface*>**)&spans);

        for (Int32 i = 0; i < spans->GetLength(); i++) {
            Int32 start, end;
            spanned->GetSpanStart((*spans)[i], &start);
            spanned->GetSpanEnd((*spans)[i], &end);

            if (start < offset && end > offset) {
                offset = end;
            }
        }
    }

    return offset;
}

/**
 * Copies the spans from the region <code>start...end</code> in
 * <code>source</code> to the region
 * <code>destoff...destoff+end-start</code> in <code>dest</code>.
 * Spans in <code>source</code> that begin before <code>start</code>
 * or end after <code>end</code> but overlap this range are trimmed
 * as if they began at <code>start</code> or ended at <code>end</code>.
 *
 * @throws IndexOutOfBoundsException if any of the copied spans
 * are out of range in <code>dest</code>.
 */
void TextUtils::CopySpansFrom(
    /* [in] */ ISpanned* source,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ const InterfaceID& kind,
    /* [in] */ ISpannable* dest,
    /* [in] */ Int32 destoff)
{
//    if (kind == NULL) {
//        kind = Object.class;
//    }
    AutoPtr< ArrayOf<IInterface*> > spans;
    source->GetSpans(start, end, kind, (ArrayOf<IInterface*>**)&spans);

    for (Int32 i = 0; i < spans->GetLength(); i++) {
        Int32 st, en, fl;
        source->GetSpanStart((*spans)[i], &st);
        source->GetSpanEnd((*spans)[i], &en);
        source->GetSpanFlags((*spans)[i], &fl);

        if (st < start) {
            st = start;
        }
        if (en > end) {
            en = end;
        }

        dest->SetSpan((*spans)[i], st - start + destoff,
                en - start + destoff, fl);
    }
}

/**
 * Returns the original text if it fits in the specified width
 * given the properties of the specified Paint,
 * or, if it does not fit, a truncated
 * copy with ellipsis character added at the specified edge or center.
 */
AutoPtr<ICharSequence> TextUtils::Ellipsize(
    /* [in] */ ICharSequence* text,
    /* [in] */ ITextPaint* p,
    /* [in] */ Float avail,
    /* [in] */ TextUtilsTruncateAt where)
{
    return Ellipsize(text, p, avail, where, FALSE, NULL);
}

/**
 * Returns the original text if it fits in the specified width
 * given the properties of the specified Paint,
 * or, if it does not fit, a copy with ellipsis character added
 * at the specified edge or center.
 * If <code>preserveLength</code> is specified, the returned copy
 * will be padded with zero-width spaces to preserve the original
 * length and offsets instead of truncating.
 * If <code>callback</code> is non-NULL, it will be called to
 * report the start and end of the ellipsized range.  TextDirection
 * is determined by the first strong directional character.
 */
AutoPtr<ICharSequence> TextUtils::Ellipsize(
    /* [in] */ ICharSequence* text,
    /* [in] */ ITextPaint* paint,
    /* [in] */ Float avail,
    /* [in] */ TextUtilsTruncateAt where,
    /* [in] */ Boolean preserveLength,
    /* [in] */ ITextUtilsEllipsizeCallback* callback)
{
    AutoPtr<IResourcesHelper> helper;
    CResourcesHelper::AcquireSingleton((IResourcesHelper**)&helper);
    AutoPtr<IResources> resources;
    helper->GetSystem((IResources**)&resources);

    String ellipsis;
    if (where == TextUtilsTruncateAt_END_SMALL)  {
        resources->GetString(R::string::ellipsis_two_dots, &ellipsis);
    }
    else {
        resources->GetString(R::string::ellipsis, &ellipsis);
    }

    AutoPtr<ICharSequence> rst = Ellipsize(text, paint, avail, where, preserveLength, callback,
            TextDirectionHeuristics::FIRSTSTRONG_LTR,
            ellipsis);
    if(rst == NULL) {
        String sNull("");
        CStringWrapper::New(sNull, (ICharSequence**)&rst);
    }
    return rst;
}

/**
 * Returns the original text if it fits in the specified width
 * given the properties of the specified Paint,
 * or, if it does not fit, a copy with ellipsis character added
 * at the specified edge or center.
 * If <code>preserveLength</code> is specified, the returned copy
 * will be padded with zero-width spaces to preserve the original
 * length and offsets instead of truncating.
 * If <code>callback</code> is non-NULL, it will be called to
 * report the start and end of the ellipsized range.
 *
 * @hide
 */
AutoPtr<ICharSequence> TextUtils::Ellipsize(
   /* [in] */ ICharSequence* text,
   /* [in] */ ITextPaint* paint,
   /* [in] */ Float avail,
   /* [in] */ TextUtilsTruncateAt where,
   /* [in] */ Boolean preserveLength,
   /* [in] */ ITextUtilsEllipsizeCallback* callback,
   /* [in] */ ITextDirectionHeuristic* textDir,
   /* [in] */ const String& ellipsis)
{
    assert(text != NULL);

    Int32 len;
    text->GetLength(&len);

    AutoPtr<MeasuredText> mt = MeasuredText::Obtain();
    //try {
        Float width = SetPara(mt, paint, text, 0, len, textDir);

        if (width <= avail) {
            if (callback != NULL) {
                callback->Ellipsized(0, 0);
            }
            MeasuredText::Recycle(mt);
            return text;
        }

        // XXX assumes ellipsis string does not require shaping and
        // is unaffected by style
        Float ellipsiswid;
        paint->MeasureText(ellipsis, &ellipsiswid);
        avail -= ellipsiswid;

        Int32 left = 0;
        Int32 right = len;
        if (avail < 0) {
            // it all goes
        } else if (where == TextUtilsTruncateAt_START) {
            right = len - mt->BreakText(len, FALSE, avail);
        } else if (where == TextUtilsTruncateAt_END
            || where == TextUtilsTruncateAt_END_SMALL) {
            left = mt->BreakText(len, TRUE, avail);
        } else {
            right = len - mt->BreakText(len, FALSE, avail / 2);
            avail -= mt->Measure(right, len);
            left = mt->BreakText(right, TRUE, avail);
        }

        if (callback != NULL) {
            callback->Ellipsized(left, right);
        }

        AutoPtr<ArrayOf<Char32> > buf = mt->mChars;
        AutoPtr<ISpanned> sp = ISpanned::Probe(text);

        Int32 remaining = len - (right - left);
        if (preserveLength) {
            if (remaining > 0) { // else eliminate the ellipsis too
               (*buf)[left++] = ellipsis.GetChar(0);
            }
            for (Int32 i = left; i < right; i++) {
               (*buf)[i] = ZWNBS_CHAR;
            }

            StringBuilder sb;
            sb.AppendChars(*buf, 0, len);
            AutoPtr<ICharSequence> s = sb.ToCharSequence();
            if (sp == NULL) {
                MeasuredText::Recycle(mt);
                return s;
            }

            AutoPtr<ISpannableString> ss;
            CSpannableString::New(s, (ISpannableString**)&ss);
            CopySpansFrom(sp, 0, len, EIID_IInterface, ss, 0);
            MeasuredText::Recycle(mt);
            return ss;
        }

        if (remaining == 0) {
            AutoPtr<ICharSequence> seq;
            CStringWrapper::New(String(""), (ICharSequence**)&seq);
            MeasuredText::Recycle(mt);
            return seq;
        }

        if (sp == NULL) {
            Int32 length = remaining + ellipsis.GetByteLength();
            StringBuilder sb(length);
            sb.AppendChars(*buf, 0, left);
            sb.AppendString(ellipsis);
            sb.AppendChars(*buf, right, len - right);
            MeasuredText::Recycle(mt);
            return sb.ToCharSequence();
        }

        AutoPtr<ISpannableStringBuilder> ssb;
        CSpannableStringBuilder::New((ISpannableStringBuilder**)&ssb);
        ssb->Append(text, 0, left);
        AutoPtr<ICharSequence> elpSeq;
        CStringWrapper::New(ellipsis, (ICharSequence**)&elpSeq);
        ssb->Append(elpSeq);
        ssb->Append(text, right, len);
        MeasuredText::Recycle(mt);
        return ssb;
   //} finally {
      // MeasuredText.recycle(mt);
   //}
}

/**
 * Converts a CharSequence of the comma-separated form "Andy, Bob,
 * Charles, David" that is too wide to fit into the specified width
 * into one like "Andy, Bob, 2 more".
 *
 * @param text the text to truncate
 * @param p the Paint with which to measure the text
 * @param avail the horizontal width available for the text
 * @param oneMore the string for "1 more" in the current locale
 * @param more the string for "%d more" in the current locale
 */
//    public static CharSequence commaEllipsize(CharSequence text,
//                                              TextPaint p, float avail,
//                                              String oneMore,
//                                              String more);
/**
 * @hide
 */
//    public static CharSequence commaEllipsize(CharSequence text, TextPaint p,
//         float avail, String oneMore, String more, TextDirectionHeuristic textDir);

/* package */
Boolean TextUtils::DoesNotNeedBidi(
    /* [ini] */ ICharSequence* s,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    assert(s != NULL);
    for (Int32 i = start; i < end; i++) {
        Char32 c;
        s->GetCharAt(i, &c);
        if (c >= FIRST_RIGHT_TO_LEFT) {
            return FALSE;
        }
    }
    return TRUE;
}

/* package */
Boolean TextUtils::DoesNotNeedBidi(
    /* [in] */ ArrayOf<Char32>* text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 len)
{
    assert(text != NULL);
    for (Int32 i = start, e = i + len; i < e; i++) {
        if ((*text)[i] >= FIRST_RIGHT_TO_LEFT) {
            return FALSE;
        }
    }
    return TRUE;
}

Int32 TextUtils::IdealCharArraySize(
    /*[in] */ Int32 size)
{
    Int32 need = size * 4;
    Int32 result = need;
    for (Int32 i = 4; i < 32; i++) {
        if (need <= (1 << i) - 12) {
            result = (1 << i) - 12;
            break;
        }
    }

    return result / 4;
}

/* package */
AutoPtr< ArrayOf<Char32> > TextUtils::Obtain(
    /* [in] */ Int32 len)
{
    assert(len >= 0);
    AutoPtr< ArrayOf<Char32> > buf;
    {
        AutoLock lock(sLock);
        buf = sTemp;
        sTemp = NULL;
    }

    if (buf == NULL || buf->GetLength() < len) {
        buf = ArrayOf<Char32>::Alloc(IdealCharArraySize(len));
    }

    return buf;
}

/* package */
void TextUtils::Recycle(
    /* [in] */ ArrayOf<Char32>* temp)
{
    if (temp && temp->GetLength() > 1000) {
        return;
    }

    AutoLock lock(sLock);
    sTemp = temp;
}

/**
 * Html-encode the string.
 * @param s the string to be encoded
 * @return the encoded string
 */
//    public static String htmlEncode(String s);

/**
 * Returns a CharSequence concatenating the specified CharSequences,
 * retaining their spans if any.
 */
AutoPtr<ICharSequence> TextUtils::Concat(
    /* [in] */ ArrayOf<ICharSequence*>* text)
{
    AutoPtr<ICharSequence> result;
    Int32 len = text->GetLength();
    if (len == 0) {
        CStringWrapper::New(String(""), (ICharSequence**)&result);
        return result;
    }
    if (len == 1) {
        return (*text)[0];
    }

    Boolean spanned = FALSE;
    for (Int32 i = 0; i < len; ++i) {
        if (ISpanned::Probe((*text)[i])) {
            spanned = TRUE;
            break;
        }
    }

    StringBuilder sb;
    for (Int32 i = 0; i < len; ++i) {
        sb.AppendCharSequence((*text)[i]);
    }

    if (!spanned) {
        CStringWrapper::New(sb.ToString(), (ICharSequence**)&result);
        return result;
    }
    AutoPtr<ISpannableString> ss;
    AutoPtr<ICharSequence> s = sb.ToCharSequence();
    CSpannableString::New(s, (ISpannableString**)&ss);
    Int32 off = 0;
    Int32 iLen;
    for (Int32 i = 0; i < len; ++i) {
        (*text)[i]->GetLength(&iLen);
        if (ISpanned::Probe((*text)[i])) {
            CopySpansFrom(ISpanned::Probe((*text)[i]), 0, iLen, EIID_IInterface , ss, off);
        }

        off += iLen;
    }
    AutoPtr<ISpannedString> span;
    CSpannedString::New(ss,(ISpannedString**)&span);
    result = ICharSequence::Probe(span.Get());
    return result;
}

/**
 * Returns whether the given CharSequence contains any printable characters.
 */
//    public static boolean isGraphic(CharSequence str);

/**
 * Returns whether this character is a printable character.
 */
//    public static boolean isGraphic(char c);
/**
 * Returns whether the given CharSequence contains only digits.
 */
//    public static boolean isDigitsOnly(CharSequence str);

/**
 * @hide
 */
//    public static boolean isPrintableAscii(final char c);

/**
 * @hide
 */
//    public static boolean isPrintableAsciiOnly(final CharSequence str);

/**
 * Determine what caps mode should be in effect at the current offset in
 * the text.  Only the mode bits set in <var>reqModes</var> will be
 * checked.  Note that the caps mode flags here are explicitly defined
 * to match those in {@link InputType}.
 *
 * @param cs The text that should be checked for caps modes.
 * @param off Location in the text at which to check.
 * @param reqModes The modes to be checked: may be any combination of
 * {@link #CAP_MODE_CHARACTERS}, {@link #CAP_MODE_WORDS}, and
 * {@link #CAP_MODE_SENTENCES}.
 *
 * @return Returns the actual capitalization modes that can be in effect
 * at the current position, which is any combination of
 * {@link #CAP_MODE_CHARACTERS}, {@link #CAP_MODE_WORDS}, and
 * {@link #CAP_MODE_SENTENCES}.
 */
Int32 TextUtils::GetCapsMode(
    /* [in] */ ICharSequence* cs,
    /* [in] */ Int32 off,
    /* [in] */ Int32 reqModes)
{
    if (off < 0) {
        return 0;
    }

    Int32 i;
    Char32 c;
    Int32 mode = 0;

    if ((reqModes & ITextUtils::CAP_MODE_CHARACTERS) != 0) {
        mode |= ITextUtils::CAP_MODE_CHARACTERS;
    }
    if ((reqModes & (ITextUtils::CAP_MODE_WORDS | ITextUtils::CAP_MODE_SENTENCES)) == 0) {
        return mode;
    }

    // Back over allowed opening punctuation.

    for (i = off; i > 0; i--) {
        cs->GetCharAt(i - 1, &c);

        if (c != '"' && c != '\'' &&
            Character::GetType(c) != Character::START_PUNCTUATION) {
            break;
        }
    }

    // Start of paragraph, with optional whitespace.

    Int32 j = i;
    while (j > 0 && ((cs->GetCharAt(j - 1, &c), c) == ' ' || c == '\t')) {
        j--;
    }
    if (j == 0 || (cs->GetCharAt(j - 1, &c), c) == '\n') {
        return mode | ITextUtils::CAP_MODE_WORDS;
    }

    // Or start of word if we are that style.

    if ((reqModes & ITextUtils::CAP_MODE_SENTENCES) == 0) {
        if (i != j) mode |= ITextUtils::CAP_MODE_WORDS;
        return mode;
    }

    // There must be a space if not the start of paragraph.

    if (i == j) {
        return mode;
    }

    // Back over allowed closing punctuation.

    for (; j > 0; j--) {
        cs->GetCharAt(j - 1, &c);

        if (c != '"' && c != '\'' &&
            Character::GetType(c) != Character::END_PUNCTUATION) {
            break;
        }
    }

    if (j > 0) {
        cs->GetCharAt(j - 1, &c);

        if (c == '.' || c == '?' || c == '!') {
            // Do not capitalize if the word ends with a period but
            // also contains a period, in which case it is an abbreviation.

            if (c == '.') {
                for (Int32 k = j - 2; k >= 0; k--) {
                    cs->GetCharAt(k, &c);

                    if (c == '.') {
                        return mode;
                    }

                    if (!Character::IsLetter(c)) {
                        break;
                    }
                }
            }

            return mode | ITextUtils::CAP_MODE_SENTENCES;
        }
    }

    return mode;
}

Boolean TextUtils::DelimitedStringContains(
    /* [in] */ const String& delimitedString,
    /* [in] */ Char32 delimiter,
    /* [in] */ const String& item)
{
    if (IsEmpty(delimitedString) || IsEmpty(item)) {
        return FALSE;
    }
    Int32 pos = -1;
    Int32 length = delimitedString.GetLength();
    while ((pos = delimitedString.IndexOf(item, pos + 1)) != -1) {
        if (pos > 0 && delimitedString.GetChar(pos - 1) != delimiter) {
            continue;
        }
        Int32 expectedDelimiterPos = pos + item.GetLength();
        if (expectedDelimiterPos == length) {
            // Match at end of string.
            return TRUE;
        }
        if (delimitedString.GetChar(expectedDelimiterPos) == delimiter) {
            return TRUE;
        }
    }
    return FALSE;
}

/**
 * Removes empty spans from the <code>spans</code> array.
 *
 * When parsing a Spanned using {@link Spanned#nextSpanTransition(int, int, Class)}, empty spans
 * will (correctly) create span transitions, and calling getSpans on a slice of text bounded by
 * one of these transitions will (correctly) include the empty overlapping span.
 *
 * However, these empty spans should not be taken into account when layouting or rendering the
 * string and this method provides a way to filter getSpans' results accordingly.
 *
 * @param spans A list of spans retrieved using {@link Spanned#getSpans(int, int, Class)} from
 * the <code>spanned</code>
 * @param spanned The Spanned from which spans were extracted
 * @return A subset of spans where empty spans ({@link Spanned#getSpanStart(Object)}  ==
 * {@link Spanned#getSpanEnd(Object)} have been removed. The initial order is preserved
 * @hide
 */
AutoPtr< ArrayOf<IInterface*> > TextUtils::RemoveEmptySpans(
    /* [in] */ ArrayOf<IInterface*>* spans,
    /* [in] */ ISpanned* spanned,
    /* [in] */ const InterfaceID& klass)
{
    assert(spans != NULL);
    assert(spanned != NULL);

    AutoPtr< ArrayOf<IInterface*> > copy;
    Int32 count = 0;

    for (Int32 i = 0; i < spans->GetLength(); i++) {
        AutoPtr<IInterface> span = (*spans)[i];
        Int32 start, end;
        spanned->GetSpanStart(span, &start);
        spanned->GetSpanEnd(span, &end);

        if (start == end) {
            if (copy == NULL) {
                assert(0);
//                copy = (T[]) Array.newInstance(klass, spans.length - 1);
//                System.arraycopy(spans, 0, copy, 0, i);
                count = i;
            }
        }
        else {
            if (copy != NULL) {
                copy->Set(count, span);
                count++;
            }
        }
    }

    if (copy != NULL) {
        assert(0);
//        T[] result = (T[]) Array.newInstance(klass, count);
//        System.arraycopy(copy, 0, result, 0, count);
        return NULL;
    }
    else {
        return spans;
    }
}

/**
 * Pack 2 int values into a long, useful as a return value for a range
 * @see #unpackRangeStartFromLong(long)
 * @see #unpackRangeEndFromLong(long)
 * @hide
 */
Int64 TextUtils::PackRangeInInt64(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    return (((Int64) start) << 32) | end;
}

/**
 * Get the start value from a range packed in a long by {@link #packRangeInLong(int, int)}
 * @see #unpackRangeEndFromLong(long)
 * @see #packRangeInLong(int, int)
 * @hide
 */
Int32 TextUtils::UnpackRangeStartFromInt64(
    /* [in] */ Int64 range)
{
    return (Int32)(((UInt64)range) >> 32);
}

/**
 * Get the end value from a range packed in a long by {@link #packRangeInLong(int, int)}
 * @see #unpackRangeStartFromLong(long)
 * @see #packRangeInLong(int, int)
 * @hide
 */
Int32 TextUtils::UnpackRangeEndFromInt64(
    /* [in] */ Int64 range)
{
    return (Int32) (range & 0x00000000FFFFFFFFll);
}

/**
 * Return the layout direction for a given Locale
 *
 * @param locale the Locale for which we want the layout direction. Can be NULL.
 * @return the layout direction. This may be one of:
 * {@link android.view.View#LAYOUT_DIRECTION_LTR} or
 * {@link android.view.View#LAYOUT_DIRECTION_RTL}.
 *
 * Be careful: this code will need to be updated when vertical scripts will be supported
 */
Int32 TextUtils::GetLayoutDirectionFromLocale(
    /* [in] */ ILocale* locale)
{
    AutoPtr<ILocale> root;
    AutoPtr<ILocaleHelper> helper;
    CLocaleHelper::AcquireSingleton((ILocaleHelper**)&helper);
    helper->GetROOT((ILocale**)&root);
    Boolean result;
    if (locale != NULL && !(locale->Equals(root, &result), result)) {
        AutoPtr<IICUUtil> icuHelper;
        String scriptSubtag;
        CICUUtil::AcquireSingleton((IICUUtil**)&icuHelper);
        locale->ToString(&scriptSubtag);
        icuHelper->AddLikelySubtags(scriptSubtag, &scriptSubtag);
        icuHelper->GetScript(scriptSubtag, &scriptSubtag);

        if (scriptSubtag.IsNull()) {
            return GetLayoutDirectionFromFirstChar(locale);
        }

        if (scriptSubtag.EqualsIgnoreCase(ARAB_SCRIPT_SUBTAG) ||
            scriptSubtag.EqualsIgnoreCase(HEBR_SCRIPT_SUBTAG)) {
            return IView::LAYOUT_DIRECTION_RTL;
        }
    }

    return IView::LAYOUT_DIRECTION_LTR;
}

ECode TextUtils::WriteWhere(
    /* [in] */ IParcel* p,
    /* [in] */ ISpanned* sp,
    /* [in] */ IInterface* o)
{
    Int32 start, end, flags;
    sp->GetSpanStart(o, &start);
    FAIL_RETURN(p->WriteInt32(start));
    sp->GetSpanEnd(o, &end);
    FAIL_RETURN(p->WriteInt32(end));
    sp->GetSpanFlags(o, &flags);
    FAIL_RETURN(p->WriteInt32(flags));
    return NOERROR;
}

ECode TextUtils::ReadSpan(
    /* [in] */ IParcel* p,
    /* [in] */ ISpannable* sp,
    /* [in] */ IInterface* o)
{
    Int32 start, end, flags;
    FAIL_RETURN(p->ReadInt32(&start));
    FAIL_RETURN(p->ReadInt32(&end));
    FAIL_RETURN(p->ReadInt32(&flags));
    return sp->SetSpan(o, start, end, flags);
}

Float TextUtils::SetPara(
   /* [in] */ MeasuredText* mt,
   /* [in] */ ITextPaint* paint,
   /* [in] */ ICharSequence* text,
   /* [in] */ Int32 start,
   /* [in] */ Int32 end,
   /* [in] */ ITextDirectionHeuristic* textDir)
{
    assert(mt != NULL && text != NULL);
    mt->SetPara(text, start, end, textDir);

    Float width;
    AutoPtr<ISpanned> sp = ISpanned::Probe(text);

    Int32 len = end - start;
    if (sp == NULL) {
        width = mt->AddStyleRun(paint, len, NULL);
    }
    else {
        width = 0;
        Int32 spanEnd;
        for (Int32 spanStart = 0; spanStart < len; spanStart = spanEnd) {
            sp->NextSpanTransition(spanStart, len, EIID_IMetricAffectingSpan, &spanEnd);
            AutoPtr<ArrayOf<IInterface*> > spans, spansResult;//IMetricAffectingSpan
            sp->GetSpans(spanStart, spanEnd, EIID_IMetricAffectingSpan, (ArrayOf<IInterface*>**)&spans);
            spansResult = TextUtils::RemoveEmptySpans(spans, sp, EIID_IMetricAffectingSpan);
            width += mt->AddStyleRun(paint, spansResult, spanEnd - spanStart, NULL);
        }
    }

    return width;
}

/**
 * Fallback algorithm to detect the locale direction. Rely on the fist char of the
 * localized locale name. This will not work if the localized locale name is in English
 * (this is the case for ICU 4.4 and "Urdu" script)
 *
 * @param locale
 * @return the layout direction. This may be one of:
 * {@link View#LAYOUT_DIRECTION_LTR} or
 * {@link View#LAYOUT_DIRECTION_RTL}.
 *
 * Be careful: this code will need to be updated when vertical scripts will be supported
 *
 * @hide
 */
Int32 TextUtils::GetLayoutDirectionFromFirstChar(
    /* [in] */ ILocale* locale)
{
    String displayName;
    locale->GetDisplayName(locale, &displayName);
    const Byte directionality = Character::GetDirectionality(displayName.GetChar(0));
    if (directionality == Character::DIRECTIONALITY_RIGHT_TO_LEFT ||
        directionality == Character::DIRECTIONALITY_RIGHT_TO_LEFT_ARABIC) {
        return IView::LAYOUT_DIRECTION_RTL;
    }
    return IView::LAYOUT_DIRECTION_LTR;
}

} // namespace Text
} // namepsace Droid
} // namespace Elastos
