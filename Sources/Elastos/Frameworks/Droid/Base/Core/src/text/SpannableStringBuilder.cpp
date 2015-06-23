
#include "text/SpannableStringBuilder.h"
#include "text/CSpannableStringBuilder.h"
#include "text/TextUtils.h"
#include "util/ArrayUtils.h"
#include <elastos/core/StringBuffer.h>
#include "text/Selection.h"
#include <elastos/core/Math.h>
#include <elastos/core/Character.h>

using Elastos::Core::Character;
using Elastos::Core::IComparable;
using Elastos::Core::CStringWrapper;
using Elastos::Core::StringBuffer;
using Elastos::Core::EIID_ICharSequence;
using Elastos::Droid::Internal::Utility::ArrayUtils;

namespace Elastos {
namespace Droid {
namespace Text {

AutoPtr< ArrayOf<IInputFilter*> > SpannableStringBuilder::NO_FILTERS = ArrayOf<IInputFilter*>::Alloc(0);

const Int32 SpannableStringBuilder::MARK;
const Int32 SpannableStringBuilder::POINT;
const Int32 SpannableStringBuilder::PARAGRAPH;

const Int32 SpannableStringBuilder::START_MASK;
const Int32 SpannableStringBuilder::END_MASK;
const Int32 SpannableStringBuilder::START_SHIFT;

const Int32 SpannableStringBuilder::SPAN_START_AT_START;
const Int32 SpannableStringBuilder::SPAN_START_AT_END;
const Int32 SpannableStringBuilder::SPAN_END_AT_START;
const Int32 SpannableStringBuilder::SPAN_END_AT_END;
const Int32 SpannableStringBuilder::SPAN_START_END_MASK;

static AutoPtr<ICharSequence> CreateEmptyCs()
{
    AutoPtr<ICharSequence> cs;
    CStringWrapper::New(String(""), (ICharSequence**)&cs);
    return cs;
}

AutoPtr<ICharSequence> SpannableStringBuilder::EMPTY_CS = CreateEmptyCs();

SpannableStringBuilder::SpannableStringBuilder()
    : mGapStart(0)
    , mGapLength(0)
    , mSpanCount(0)
    , mSpanCountBeforeAdd(0)
{
    Init();
}

SpannableStringBuilder::SpannableStringBuilder(
    /* [in] */ ICharSequence* text)
{
    Init(text);
}

SpannableStringBuilder::SpannableStringBuilder(
    /* [in] */ ICharSequence* text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    Init(text, start, end);
}

SpannableStringBuilder::~SpannableStringBuilder()
{
}

ECode SpannableStringBuilder::Init()
{
    Init(EMPTY_CS);
    return NOERROR;
}

ECode SpannableStringBuilder::Init(
    /* [in] */ ICharSequence* text)
{
    VALIDATE_NOT_NULL(text);
    Int32 len;
    text->GetLength(&len);
    Init(text, 0, len);

    return NOERROR;
}

ECode SpannableStringBuilder::Init(
    /* [in] */ ICharSequence* text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    VALIDATE_NOT_NULL(text);
    Int32 textLen = 0;
    text->GetLength(&textLen);
    if (start < 0 || end < 0 || start > end ||
        end - start > textLen) {
        return E_STRING_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    mFilters = NO_FILTERS;
    Int32 srclen = end - start;

    Int32 len = ArrayUtils::IdealInt32ArraySize(srclen + 1);
    mText = ArrayOf<Char32>::Alloc(len);
    mGapStart = srclen;
    mGapLength = len - srclen;

    for (Int32 i = start; i < end; i++) {
        Char32 ch;
        text->GetCharAt(i, &ch);
        (*mText)[i - start] = ch;
    }

    mSpanCount = 0;
    Int32 alloc = ArrayUtils::IdealInt32ArraySize(0);
    mSpans = ArrayOf<IInterface*>::Alloc(alloc);
    mSpanStarts = ArrayOf<Int32>::Alloc(alloc);
    mSpanEnds = ArrayOf<Int32>::Alloc(alloc);
    mSpanFlags = ArrayOf<Int32>::Alloc(alloc);

    if (ISpanned::Probe(text)) {
        AutoPtr<ISpanned> sp = ISpanned::Probe(text);
        AutoPtr< ArrayOf<IInterface*> > spans = NULL;
        sp->GetSpans(start, end, EIID_IInterface, (ArrayOf<IInterface*>**)&spans);

        for (Int32 i = 0; i < spans->GetLength(); i++) {
            if (INoCopySpan::Probe((*spans)[i])) {
                continue;
            }

            Int32 st;
            sp->GetSpanStart((*spans)[i], &st);
            st -= start;
            Int32 en;
            sp->GetSpanEnd((*spans)[i], &en);
            en -= start;
            Int32 fl;
            sp->GetSpanFlags((*spans)[i], &fl);

            if (st < 0)
                st = 0;
            if (st > end - start)
                st = end - start;

            if (en < 0)
                en = 0;
            if (en > end - start)
                en = end - start;

            SetSpan(FALSE, (*spans)[i], st, en, fl);
        }
    }

    return NOERROR;
}

AutoPtr<ISpannableStringBuilder> SpannableStringBuilder::ValueOf(
    /* [in] */ ICharSequence* source)
{
    AutoPtr<ISpannableStringBuilder> ssb;
    if (ISpannableStringBuilder::Probe(source)) {
        ssb = ISpannableStringBuilder::Probe(source);
    } else {
        CSpannableStringBuilder::New(source, (ISpannableStringBuilder**)&ssb);
    }
    return ssb;
}

ECode SpannableStringBuilder::GetCharAt(
    /* [in] */ Int32 where,
    /* [out] */ Char32* c)
{
    VALIDATE_NOT_NULL(c);
    *c = '\0';

    Int32 len = GetLength();
    if (where < 0) {
        //throw new IndexOutOfBoundsException("GetCharAt: " + where + " < 0");
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    else if (where >= len) {
        //throw new IndexOutOfBoundsException("GetCharAt: " + where +
        //                                    " >= GetLength " + len);
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    if (where >= mGapStart) {
        *c = (*mText)[where + mGapLength];
    } else {
        *c = (*mText)[where];
    }
    return NOERROR;
}

Int32 SpannableStringBuilder::GetLength()
{
    return mText->GetLength() - mGapLength;
}

ECode SpannableStringBuilder::Insert(
    /* [in] */ Int32 where,
    /* [in] */ ICharSequence* tb,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    return Replace(where, where, tb, start, end);
}

ECode SpannableStringBuilder::Insert(
    /* [in] */ Int32 where,
    /* [in] */ ICharSequence* tb)
{
    assert(tb);
    Int32 len;
    tb->GetLength(&len);
    return Replace(where, where, tb, 0, len);
}

ECode SpannableStringBuilder::Delete(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    FAIL_RETURN(Replace(start, end, EMPTY_CS, 0, 0));
    if (mGapLength > 2 * GetLength()) {
        FAIL_RETURN(ResizeFor(GetLength()));
    }

    return NOERROR;
}

ECode SpannableStringBuilder::Clear()
{
    return Replace(0, GetLength(), EMPTY_CS, 0, 0);
}

ECode SpannableStringBuilder::ClearSpans()
{
    for (Int32 i = mSpanCount - 1; i >= 0; i--) {
        AutoPtr<IInterface> what = (*mSpans)[i];
        Int32 ostart = (*mSpanStarts)[i];
        Int32 oend = (*mSpanEnds)[i];

        if (ostart > mGapStart) {
            ostart -= mGapLength;
        }

        if (oend > mGapStart) {
            oend -= mGapLength;
        }

        mSpanCount = i;
        mSpans->Set(i, NULL);

        SendSpanRemoved(what, ostart, oend);
    }

    return NOERROR;
}

ECode SpannableStringBuilder::Append(
    /* [in] */ ICharSequence* text)
{
    VALIDATE_NOT_NULL(text);
    Int32 length = GetLength();
    Int32 tLen;
    text->GetLength(&tLen);
    return Replace(length, length, text, 0, tLen);
}

ECode SpannableStringBuilder::Append(
    /* [in] */ ICharSequence* text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    VALIDATE_NOT_NULL(text);
    Int32 length = GetLength();
    return Replace(length, length, text, start, end);
}

ECode SpannableStringBuilder::Append(
    /* [in] */ Char32 text)
{
    VALIDATE_NOT_NULL(text);
    String str("");
    str.Append(text);
    AutoPtr<ICharSequence> cs;
    CStringWrapper::New(str, (ICharSequence**)&cs);
    return Append(cs);
}

ECode SpannableStringBuilder::Replace(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ ICharSequence* tb)
{
    VALIDATE_NOT_NULL(tb);
    Int32 len;
    tb->GetLength(&len);
    return Replace(start, end, tb, 0, len);
}

ECode SpannableStringBuilder::Replace(
    /* [in] */ const Int32 start,
    /* [in] */ const Int32 end,
    /* [in] */ ICharSequence* _tb,
    /* [in] */ Int32 tbstart,
    /* [in] */ Int32 tbend)
{
    FAIL_RETURN(CheckRange(String("replace"), start, end));
    FAIL_RETURN(CheckRange(_tb, tbstart, tbend));
    Int32 filtercount = mFilters->GetLength();
    AutoPtr<ICharSequence> tb = _tb;
    for (Int32 i = 0; i < filtercount; i++) {
        AutoPtr<ICharSequence> repl;
        AutoPtr<ISpanned> span = (ISpanned*)this->Probe(EIID_ISpanned);
        (*mFilters)[i]->Filter(tb, tbstart, tbend, span, start, end, (ICharSequence**)&repl);
        if (repl != NULL) {
            tb = repl;
            tbstart = 0;
            repl->GetLength(&tbend);
        }
    }

    Int32 origLen = end - start;
    Int32 newLen = tbend - tbstart;

    if (origLen == 0 && newLen == 0 && !HasNonExclusiveExclusiveSpanAt(tb, tbstart)) {
        // This is a no-op iif there are no spans in tb that would be added (with a 0-length)
        // Early exit so that the text watchers do not get notified
        return NOERROR;
    }

    AutoPtr<ArrayOf<IInterface*> > tempWatchers = GetSpans(start, start + origLen, EIID_ITextWatcher);
    AutoPtr<ArrayOf<ITextWatcher*> > textWatchers = ArrayOf<ITextWatcher*>::Alloc(tempWatchers->GetLength());
    for(Int32 i = 0; i < tempWatchers->GetLength(); i++)
    {
        AutoPtr<ITextWatcher> temp;
        if(temp = ITextWatcher::Probe((*tempWatchers)[i]))
        {
            textWatchers->Set(i, temp);
        }
    }
    SendBeforeTextChanged(textWatchers, start, origLen, newLen);

    // Try to keep the cursor / selection at the same relative position during
    // a text replacement. If replaced or replacement text length is zero, this
    // is already taken care of.
    Boolean adjustSelection = origLen != 0 && newLen != 0;
    Int32 selectionStart = 0;
    Int32 selectionEnd = 0;
    if (adjustSelection) {
        selectionStart = Selection::GetSelectionStart((ICharSequence*)this->Probe(EIID_ICharSequence));
        selectionEnd = Selection::GetSelectionEnd((ICharSequence*)this->Probe(EIID_ICharSequence));
    }
    Change(start, end, tb, tbstart, tbend);

    if (adjustSelection) {
        if (selectionStart > start && selectionStart < end) {
            Int32 offset = (selectionStart - start) * newLen / origLen;
            selectionStart = start + offset;

            SetSpan(FALSE, Selection::SELECTION_START, selectionStart, selectionStart,
                    ISpanned::SPAN_POINT_POINT);
        }
        if (selectionEnd > start && selectionEnd < end) {
            Int32 offset = (selectionEnd - start) * newLen / origLen;
            selectionEnd = start + offset;

            SetSpan(FALSE, Selection::SELECTION_END, selectionEnd, selectionEnd,
                    ISpanned::SPAN_POINT_POINT);
        }
    }

    SendTextChanged(textWatchers, start, origLen, newLen);
    SendAfterTextChanged(textWatchers);

    // Span watchers need to be called after text watchers, which may update the layout
    SendToSpanWatchers(start, end, newLen - origLen);

    return NOERROR;
}

ECode SpannableStringBuilder::SetSpan(
    /* [in] */ IInterface* what,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ Int32 flags)
{
    return SetSpan(TRUE, what, start, end, flags);
}

Boolean SpannableStringBuilder::IsSpanEquals(
    /* [in] */ IInterface* lhs,
    /* [in] */ IInterface* rhs)
{
    assert(lhs != NULL);
    if (!rhs)
        return FALSE;

    AutoPtr<IComparable> cpr = (IComparable*)(lhs->Probe(Elastos::Core::EIID_IComparable));
    if (cpr) {
        Int32 result;
        cpr->CompareTo(rhs, &result);
        return (result == 0);
    }
    else if (lhs->Probe(EIID_IInterface) == rhs->Probe(EIID_IInterface)) {
        return TRUE;
    }
    return FALSE;
}

ECode SpannableStringBuilder::RemoveSpan(
    /* [in] */ IInterface* what)
{
    for (Int32 i = mSpanCount - 1; i >= 0; i--) {

        if ((*mSpans)[i] == what) {
            RemoveSpan(i);
            return NOERROR;
        }
    }
    return NOERROR;
}

Int32 SpannableStringBuilder::GetSpanStart(
    /* [in] */ IInterface* what)
{
    Int32 count = mSpanCount;
    for (Int32 i = count - 1; i >= 0; i--) {
        if ((*mSpans)[i] == what) {
            Int32 where = (*mSpanStarts)[i];

            if (where > mGapStart) {
                where -= mGapLength;
            }

            return where;
        }
    }

    return -1;
}

Int32 SpannableStringBuilder::GetSpanEnd(
    /* [in] */ IInterface* what)
{
    Int32 count = mSpanCount;
    for (Int32 i = count - 1; i >= 0; i--) {
        if ((*mSpans)[i] == what) {
            Int32 where = (*mSpanEnds)[i];

            if (where > mGapStart) {
                where -= mGapLength;
            }

            return where;
        }
    }

    return -1;
}

Int32 SpannableStringBuilder::GetSpanFlags(
    /* [in] */ IInterface* what)
{
    Int32 count = mSpanCount;
    for (Int32 i = count - 1; i >= 0; i--) {
        if ((*mSpans)[i] == what) {
            return (*mSpanFlags)[i];
        }
    }

    return 0;
}

AutoPtr<ArrayOf<IInterface*> > SpannableStringBuilder::GetSpans(
    /* [in] */ Int32 queryStart,
    /* [in] */ Int32 queryEnd,
    /* [in] */ const InterfaceID& kind)
{
    Int32 spanCount = mSpanCount;
    AutoPtr< ArrayOf<IInterface*> > spans = mSpans;
    AutoPtr< ArrayOf<Int32> > starts = mSpanStarts;
    AutoPtr< ArrayOf<Int32> > ends = mSpanEnds;
    AutoPtr< ArrayOf<Int32> > flags = mSpanFlags;
    Int32 gapstart = mGapStart;
    Int32 gaplen = mGapLength;

    Int32 count = 0;
    AutoPtr< ArrayOf<IInterface*> > ret;
    AutoPtr<IInterface> ret1;

    for (Int32 i = 0; i < spanCount; i++) {
        Int32 spanStart = (*starts)[i];
        if (spanStart > gapstart) {
            spanStart -= gaplen;
        }

        if (spanStart > queryEnd) {
            continue;
        }

        Int32 spanEnd = (*ends)[i];
        if (spanEnd > gapstart) {
            spanEnd -= gaplen;
        }

        if (spanEnd < queryStart) {
            continue;
        }

        if (spanStart != spanEnd && queryStart != queryEnd) {
            if (spanStart == queryEnd) {
                continue;
            }

            if (spanEnd == queryStart) {
                continue;
            }
        }

        AutoPtr<IInterface> curSpan = (*spans)[i];
        // Expensive test, should be performed after the previous tests
        if ((curSpan == NULL) || (NULL == curSpan->Probe(kind))) {
            continue;
        }

        if (count == 0) {
            // Safe conversion thanks to the isInstance test above
            ret1 = curSpan;
            count++;
        } else {
            if (count == 1) {
                // Safe conversion, but requires a suppressWarning
                ret = ArrayOf<IInterface*>::Alloc(spanCount - i + 1);
                ret->Set(0, ret1);
            }

            Int32 prio = (*flags)[i] & ISpanned::SPAN_PRIORITY;
            if (prio != 0) {
                Int32 j = 0;

                for (j = 0; j < count; j++) {
                    Int32 p = GetSpanFlags((*ret)[j]) & ISpanned::SPAN_PRIORITY;
                    if (prio > p) {
                        break;
                    }
                }

                Int32 cc = count - j;
                for (Int32 k = cc - 1; k >= 0; --k) {
                    ret->Set(j + k + 1, (*ret)[j + k]);
                }

                // Safe conversion thanks to the isInstance test above
                ret->Set(j, curSpan);
                count++;
            } else {
                // Safe conversion thanks to the isInstance test above
                ret->Set(count++, curSpan);
            }
        }
    }

    if (count == 0) {
        ret = ArrayOf<IInterface*>::Alloc(0);
        return ret;
    }

    if (count == 1) {
        // Safe conversion, but requires a suppressWarning
        ret = ArrayOf<IInterface*>::Alloc(1);
        ret->Set(0, ret1);
        return ret;
    }

    if (count == ret->GetLength()) {
        return ret;
    }

    // Safe conversion, but requires a suppressWarning
    AutoPtr< ArrayOf<IInterface*> > nret = ArrayOf<IInterface*>::Alloc(count);
    nret->Copy(ret, 0, count);
    return nret;
}

Int32 SpannableStringBuilder::NextSpanTransition(
    /* [in] */ Int32 start,
    /* [in] */ Int32 limit,
    /* [in] */ const InterfaceID& kind)
{
    Int32 count = mSpanCount;
    Int32 gapstart = mGapStart;
    Int32 gaplen = mGapLength;

    //if (kind == NULL) {
    //    kind = Object.class;
    //}

    for (Int32 i = 0; i < count; i++) {
        Int32 st = (*mSpanStarts)[i];
        Int32 en = (*mSpanEnds)[i];

        if (st > gapstart) {
            st -= gaplen;
        }

        if (en > gapstart) {
            en -= gaplen;
        }

        if (st > start && st < limit && (*mSpans)[i]->Probe(kind) != NULL) {
            limit = st;
        }

        if (en > start && en < limit && (*mSpans)[i]->Probe(kind) != NULL) {
            limit = en;
        }
    }

    return limit;
}

ECode SpannableStringBuilder::SubSequence(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out] */ ICharSequence** cs)
{
    AutoPtr<ISpannableStringBuilder> sub;
    FAIL_RETURN(CSpannableStringBuilder::New(
        (ICharSequence*)this->Probe(Elastos::Core::EIID_ICharSequence), start, end,
        (ISpannableStringBuilder**)&sub));
    *cs = ICharSequence::Probe(sub);
    REFCOUNT_ADD(*cs);
    return NOERROR;
}

ECode SpannableStringBuilder::GetChars(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ ArrayOf<Char32>* dest,
    /* [in] */ Int32 destoff)
{
    VALIDATE_NOT_NULL(dest);
    FAIL_RETURN(CheckRange(String("getchars"), start, end));

    if (end <= mGapStart) {
        Int32 ret = dest->Copy(destoff, mText, start, end - start);
        return ret == -1 ? E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION : NOERROR;
    }
    else if (start >= mGapStart) {
        Int32 ret = dest->Copy(destoff, mText, start + mGapLength, end - start);
        return ret == -1 ? E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION : NOERROR;
    }
    else {
        Int32 ret = dest->Copy(destoff, mText, start, mGapStart - start);
        if (ret == -1) return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
        ret = dest->Copy(destoff + (mGapStart - start), mText, mGapStart + mGapLength, end - mGapStart);
        return ret == -1 ? E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION : NOERROR;
    }
}

String SpannableStringBuilder::ToString()
{
    Int32 len = GetLength();
    if (len == 0) return String("");
    AutoPtr< ArrayOf<Char32> > buf = ArrayOf<Char32>::Alloc(len);
    GetChars(0, len, buf, 0);
    return String(*buf);
}

String SpannableStringBuilder::Substring(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    AutoPtr< ArrayOf<Char32> > buf = ArrayOf<Char32>::Alloc(end - start);
    GetChars(start, end, buf, 0);
    return String(*buf);
}

ECode SpannableStringBuilder::DrawText(
    /* [in] */ ICanvas* c,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ IPaint* p)
{
    assert(c != NULL);

    FAIL_RETURN(CheckRange(String("drawText"), start, end));

    if (end <= mGapStart) {
        c->DrawText(*mText, start, end - start, x, y, p);
    } else if (start >= mGapStart) {
        c->DrawText(*mText, start + mGapLength, end - start, x, y, p);
    } else {
        AutoPtr< ArrayOf<Char32> > buf = TextUtils::Obtain(end - start);

        GetChars(start, end, buf, 0);
        c->DrawText(*buf, 0, end - start, x, y, p);
        TextUtils::Recycle(buf);
    }

    return NOERROR;
}

ECode SpannableStringBuilder::DrawTextRun(
    /* [in] */ ICanvas* c,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ Int32 contextStart,
    /* [in] */ Int32 contextEnd,
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ Int32 flags,
    /* [in] */ IPaint* p)
{
    assert(c != NULL);

    FAIL_RETURN(CheckRange(String("drawTextRun"), start, end));

    Int32 contextLen = contextEnd - contextStart;
    Int32 len = end - start;
    if (contextEnd <= mGapStart) {
        c->DrawTextRun(*mText, start, len, contextStart, contextLen, x, y, flags, p);
    } else if (contextStart >= mGapStart) {
        c->DrawTextRun(*mText, start + mGapLength, len, contextStart + mGapLength,
                contextLen, x, y, flags, p);
    } else {
        AutoPtr< ArrayOf<Char32> > buf = TextUtils::Obtain(contextLen);
        GetChars(contextStart, contextEnd, buf, 0);
        c->DrawTextRun(*buf, start - contextStart, len, 0, contextLen, x, y, flags, p);
        TextUtils::Recycle(buf);
    }

    return NOERROR;
}

ECode SpannableStringBuilder::MeasureText(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ IPaint* p,
    /* [out] */ Float* width)
{
    assert(width != NULL && p != NULL);
    FAIL_RETURN(CheckRange(String("measureText"), start, end));

    if (end <= mGapStart) {
        p->MeasureText(*mText, start, end - start, width);
    } else if (start >= mGapStart) {
        p->MeasureText(*mText, start + mGapLength, end - start, width);
    } else {
        AutoPtr< ArrayOf<Char32> > buf = TextUtils::Obtain(end - start);
        GetChars(start, end, buf, 0);

        p->MeasureText(*buf, 0, end - start, width);
        TextUtils::Recycle(buf);
    }

    return NOERROR;
}

ECode SpannableStringBuilder::GetTextWidths(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ ArrayOf<Float>* widths,
    /* [in] */ IPaint* p,
    /* [out] */ Int32* count)
{
    assert(count != NULL && p != NULL);
    FAIL_RETURN(CheckRange(String("getTextWidths"), start, end));

    if (end <= mGapStart) {
        p->GetTextWidths(*mText, start, end - start, widths, count);
    } else if (start >= mGapStart) {
        p->GetTextWidths(*mText, start + mGapLength, end - start, widths, count);
    } else {
        AutoPtr< ArrayOf<Char32> > buf = TextUtils::Obtain(end - start);
        GetChars(start, end, buf, 0);

        p->GetTextWidths(*buf, 0, end - start, widths, count);
        TextUtils::Recycle(buf);
    }

    return NOERROR;
}

Float SpannableStringBuilder::GetTextRunAdvances(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ Int32 contextStart,
    /* [in] */ Int32 contextEnd,
    /* [in] */ Int32 flags,
    /* [in] */ ArrayOf<Float>* advances,
    /* [in] */ Int32 advancesPos,
    /* [in] */ IPaint* p)
{
    assert(p != NULL);

    Float ret;

    Int32 contextLen = contextEnd - contextStart;
    Int32 len = end - start;

    if (end <= mGapStart) {
        p->GetTextRunAdvances(*mText, start, len, contextStart, contextLen,
                flags, advances, advancesPos, &ret);
    } else if (start >= mGapStart) {
        p->GetTextRunAdvances(*mText, start + mGapLength, len,
                contextStart + mGapLength, contextLen, flags, advances,
                advancesPos, &ret);
    } else {
        AutoPtr< ArrayOf<Char32> > buf = TextUtils::Obtain(contextLen);
        GetChars(contextStart, contextEnd, buf, 0);

        p->GetTextRunAdvances(*buf, start - contextStart, len,
               0, contextLen, flags, advances, advancesPos, &ret);
        TextUtils::Recycle(buf);
    }

    return ret;
}

Float SpannableStringBuilder::GetTextRunAdvances(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ Int32 contextStart,
    /* [in] */ Int32 contextEnd,
    /* [in] */ Int32 flags,
    /* [in] */ ArrayOf<Float>* advances,
    /* [in] */ Int32 advancesPos,
    /* [in] */ IPaint* p,
    /* [in] */ Int32 reserved)
{
    assert(p != NULL);

    Float ret = 0.f;

    Int32 contextLen = contextEnd - contextStart;
    Int32 len = end - start;

    if (end <= mGapStart) {
        p->GetTextRunAdvances(*mText, start, len, contextStart, contextLen,
                flags, advances, advancesPos, reserved, &ret);
    } else if (start >= mGapStart) {
        p->GetTextRunAdvances(*mText, start + mGapLength, len,
                contextStart + mGapLength, contextLen, flags, advances, advancesPos, reserved, &ret);
    } else {
        AutoPtr< ArrayOf<Char32> > buf = TextUtils::Obtain(contextLen);
        GetChars(contextStart, contextEnd, buf, 0);
        p->GetTextRunAdvances(*buf, start - contextStart, len,
               0, contextLen, flags, advances, advancesPos, reserved, &ret);
        TextUtils::Recycle(buf);
    }

    return ret;
}

Int32 SpannableStringBuilder::GetTextRunCursor(
    /* [in] */ Int32 contextStart,
    /* [in] */ Int32 contextEnd,
    /* [in] */ Int32 flags,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 cursorOpt,
    /* [in] */ IPaint* p)
{
    assert(p != NULL);

    Int32 ret = 0;

    Int32 contextLen = contextEnd - contextStart;
    if (contextEnd <= mGapStart) {
        p->GetTextRunCursor(*mText, contextStart, contextLen,
                flags, offset, cursorOpt, &ret);
    } else if (contextStart >= mGapStart) {
        p->GetTextRunCursor(*mText, contextStart + mGapLength, contextLen,
                flags, offset + mGapLength, cursorOpt, &ret);
        ret -= mGapLength;
    } else {
        AutoPtr< ArrayOf<Char32> > buf = TextUtils::Obtain(contextLen);
        GetChars(contextStart, contextEnd, buf, 0);
        p->GetTextRunCursor(*buf, 0, contextLen, flags, offset - contextStart, cursorOpt, &ret);
        ret += contextStart;
        TextUtils::Recycle(buf);
    }

    return ret;
}

ECode SpannableStringBuilder::SetFilters(
        /* [in] */ ArrayOf<IInputFilter*>* filters)
{
    if (filters == NULL) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    mFilters = filters;
    return NOERROR;
}

AutoPtr< ArrayOf<IInputFilter*> > SpannableStringBuilder::GetFilters()
{
    return mFilters;
}

ECode SpannableStringBuilder::ResizeFor(
    /* [in] */ Int32 size)
{
    Int32 oldLength = mText->GetLength();
    Int32 newLength = ArrayUtils::IdealCharArraySize(size + 1);
    Int32 delta = newLength - oldLength;
    if (delta == 0) return NOERROR;

    AutoPtr< ArrayOf<Char32> > newText = ArrayOf<Char32>::Alloc(newLength);
    newText->Copy(mText, mGapStart);
    Int32 after = oldLength - (mGapStart + mGapLength);
    newText->Copy(newLength - after, mText, oldLength - after, after);
    mText = newText;

    mGapLength += delta;
    if (mGapLength < 1) {
        // new Exception("mGapLength < 1").printStackTrace();
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    for (Int32 i = 0; i < mSpanCount; i++) {
        if ((*mSpanStarts)[i] > mGapStart) (*mSpanStarts)[i] += delta;
        if ((*mSpanEnds)[i] > mGapStart) (*mSpanEnds)[i] += delta;
    }

    return NOERROR;
}

void SpannableStringBuilder::MoveGapTo(
    /* [in] */ Int32 where)
{
    if (where == mGapStart)
        return;

    Boolean atEnd = (where == GetLength());

    if (where < mGapStart) {
        Int32 overlap = mGapStart - where;
        AutoPtr<ArrayOf<Char32> > newArray = mText->Clone();
        mText->Copy(mGapStart + mGapLength - overlap, newArray, where, overlap);
    } else /* where > mGapStart */ {
        Int32 overlap = where - mGapStart;
        AutoPtr<ArrayOf<Char32> > newArray = mText->Clone();
        mText->Copy(mGapStart, newArray, where + mGapLength - overlap, overlap);
    }

    // XXX be more clever
    for (Int32 i = 0; i < mSpanCount; i++) {
        Int32 start = (*mSpanStarts)[i];
        Int32 end = (*mSpanEnds)[i];

        if (start > mGapStart) {
            start -= mGapLength;
        }

        if (start > where) {
            start += mGapLength;
        } else if (start == where) {
            Int32 flag = ((*mSpanFlags)[i] & START_MASK) >> START_SHIFT;

            if (flag == POINT || (atEnd && flag == PARAGRAPH))
                start += mGapLength;
        }

        if (end > mGapStart) {
            end -= mGapLength;
        }

        if (end > where) {
            end += mGapLength;
        } else if (end == where) {
            Int32 flag = ((*mSpanFlags)[i] & END_MASK);

            if (flag == POINT || (atEnd && flag == PARAGRAPH)) {
                end += mGapLength;
            }
        }

        (*mSpanStarts)[i] = start;
        (*mSpanEnds)[i] = end;
    }

    mGapStart = where;
}

ECode SpannableStringBuilder::Change(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ ICharSequence* cs,
    /* [in] */ Int32 csStart,
    /* [in] */ Int32 csEnd)
{
    assert(cs != NULL);

    // Can be negative
    Int32 replacedLength = end - start;
    Int32 replacementLength = csEnd - csStart;
    Int32 nbNewChars = replacementLength - replacedLength;

    for (Int32 i = mSpanCount - 1; i >= 0; i--) {
        Int32 spanStart = (*mSpanStarts)[i];
        if (spanStart > mGapStart) {
            spanStart -= mGapLength;
        }

        Int32 spanEnd = (*mSpanEnds)[i];
        if (spanEnd > mGapStart) {
            spanEnd -= mGapLength;
        }

        if (( (*mSpanFlags)[i] & ISpanned::SPAN_PARAGRAPH) == ISpanned::SPAN_PARAGRAPH) {
            Int32 ost = spanStart;
            Int32 oen = spanEnd;
            Int32 clen = GetLength();

            if (spanStart > start && spanStart <= end) {
                Char32 c;
                for (spanStart = end; spanStart < clen; spanStart++)
                {
                    GetCharAt(spanStart - 1, &c);
                    if (spanStart > end && c == '\n')
                        break;
                }
            }

            if (spanEnd > start && spanEnd <= end) {
                Char32 c;
                for (spanEnd = end; spanEnd < clen; spanEnd++)
                {
                    GetCharAt(spanEnd - 1, &c);
                    if (spanEnd > end && c == '\n')
                        break;
                }
            }

            if (spanStart != ost || spanEnd != oen)
                SetSpan(FALSE, (*mSpans)[i], spanStart, spanEnd, (*mSpanFlags)[i]);
        }

        Int32 flags = 0;
        if (spanStart == start) flags |= SPAN_START_AT_START;
        else if (spanStart == end + nbNewChars) flags |= SPAN_START_AT_END;
        if (spanEnd == start) flags |= SPAN_END_AT_START;
        else if (spanEnd == end + nbNewChars) flags |= SPAN_END_AT_END;
        (*mSpanFlags)[i] |= flags;
    }

    MoveGapTo(end);

    if (nbNewChars >= mGapLength) {
        FAIL_RETURN(ResizeFor(mText->GetLength() + nbNewChars - mGapLength));
    }

    Boolean textIsRemoved = replacementLength == 0;
    // The removal pass needs to be done before the gap is updated in order to broadcast the
    // correct previous positions to the correct intersecting SpanWatchers
    if (replacedLength > 0) { // no need for span fixup on pure insertion
        // A for loop will not work because the array is being modified
        // Do not iterate in reverse to keep the SpanWatchers notified in ordering
        // Also, a removed SpanWatcher should not get notified of removed spans located
        // further in the span array.
        Int32 i = 0;
        while (i < mSpanCount) {
            if (( (*mSpanFlags)[i] & ISpanned::SPAN_EXCLUSIVE_EXCLUSIVE) ==
                    ISpanned::SPAN_EXCLUSIVE_EXCLUSIVE &&
                    (*mSpanStarts)[i] >= start && (*mSpanStarts)[i] < mGapStart + mGapLength &&
                    (*mSpanEnds)[i] >= start && (*mSpanEnds)[i] < mGapStart + mGapLength &&
                    // This condition indicates that the span would become empty
                    (textIsRemoved || (*mSpanStarts)[i] > start || (*mSpanEnds)[i] < mGapStart)) {
                RemoveSpan(i);
                continue; // do not increment i, spans will be shifted left in the array
            }

            i++;
        }
    }

    mGapStart += nbNewChars;
    mGapLength -= nbNewChars;

    if (mGapLength < 1) {
        // new Exception("mGapLength < 1").printStackTrace();
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    TextUtils::GetChars(cs, csStart, csEnd, mText, start);

    if (replacedLength > 0) { // no need for span fixup on pure insertion
        Boolean atEnd = (mGapStart + mGapLength == mText->GetLength());

        for (Int32 i = 0; i < mSpanCount; i++) {
            Int32 startFlag = ((*mSpanFlags)[i] & START_MASK) >> START_SHIFT;
            (*mSpanStarts)[i] = UpdatedIntervalBound((*mSpanStarts)[i], start, nbNewChars, startFlag,
                    atEnd, textIsRemoved);

            Int32 endFlag = ((*mSpanFlags)[i] & END_MASK);
            (*mSpanEnds)[i] = UpdatedIntervalBound((*mSpanEnds)[i], start, nbNewChars, endFlag,
                    atEnd, textIsRemoved);
        }
    }

    mSpanCountBeforeAdd = mSpanCount;

    if (cs->Probe(EIID_ISpanned)) {
        AutoPtr<ISpanned> sp = (ISpanned*) cs;
        AutoPtr< ArrayOf<IInterface*> > spans;
        sp->GetSpans(csStart, csEnd, EIID_IInterface, (ArrayOf<IInterface*>**)&spans);
        Int32 st, en;
        for (Int32 i = 0; i < spans->GetLength(); i++) {
            sp->GetSpanStart((*spans)[i], &st);
            sp->GetSpanEnd((*spans)[i], &en);

            if (st < csStart) st = csStart;
            if (en > csEnd) en = csEnd;

            // Add span only if this object is not yet used as a span in this string
            if (GetSpanStart((*spans)[i]) < 0) {
                Int32 flag;
                sp->GetSpanFlags((*spans)[i], &flag);
                SetSpan(FALSE, (*spans)[i], st - csStart + start,
                         en - csStart + start, flag);
            }
        }
    }

    return NOERROR;
}

Int32 SpannableStringBuilder::UpdatedIntervalBound(
    /* [in] */ Int32 offset,
    /* [in] */ Int32 start,
    /* [in] */ Int32 nbNewChars,
    /* [in] */ Int32 flag,
    /* [in] */ Boolean atEnd,
    /* [in] */ Boolean textIsRemoved)
{
    if (offset >= start && offset < mGapStart + mGapLength) {
        if (flag == POINT) {
            // A POINT located inside the replaced range should be moved to the end of the
            // replaced text.
            // The exception is when the point is at the start of the range and we are doing a
            // text replacement (as opposed to a deletion): the point stays there.
            if (textIsRemoved || offset > start) {
                return mGapStart + mGapLength;
            }
        } else {
            if (flag == PARAGRAPH) {
                if (atEnd) {
                    return mGapStart + mGapLength;
                }
            } else { // MARK
                // MARKs should be moved to the start, with the exception of a mark located at
                // the end of the range (which will be < mGapStart + mGapLength since mGapLength
                // is > 0, which should stay 'unchanged' at the end of the replaced text.
                if (textIsRemoved || offset < mGapStart - nbNewChars) {
                    return start;
                } else {
                    // Move to the end of replaced text (needed if nbNewChars != 0)
                    return mGapStart;
                }
            }
        }
    }

    return offset;
}

void SpannableStringBuilder::RemoveSpan(
    /* [in] */ Int32 i)
{
    AutoPtr<IInterface> object = (*mSpans)[i];

    Int32 start = (*mSpanStarts)[i];
    Int32 end = (*mSpanEnds)[i];

    if (start > mGapStart) start -= mGapLength;
    if (end > mGapStart) end -= mGapLength;

    Int32 count = mSpanCount - (i + 1);
    mSpans->Copy(i, mSpans, i + 1, count);
    mSpanStarts->Copy(i, mSpanStarts, i + 1, count);
    mSpanEnds->Copy(i, mSpanEnds, i + 1, count);
    mSpanFlags->Copy(i, mSpanFlags, i + 1, count);

    mSpanCount--;
    mSpans->Set(mSpanCount, NULL);
    SendSpanRemoved(object, start, end);
}

Boolean SpannableStringBuilder::HasNonExclusiveExclusiveSpanAt(
    /* [in] */ ICharSequence* text,
    /* [in] */ Int32 offset)
{
    if (text != NULL && text->Probe(EIID_ISpanned)) {
        AutoPtr<ISpanned> spanned = (ISpanned*) text;
        AutoPtr< ArrayOf<IInterface*> > spans;
        spanned->GetSpans(offset, offset, EIID_IInterface, (ArrayOf<IInterface*>**)&spans);
        Int32 length = spans->GetLength();
        for (Int32 i = 0; i < length; i++) {
            AutoPtr<IInterface> span = (*spans)[i];
            Int32 flags;
            spanned->GetSpanFlags(span, &flags);
            if (flags != ISpanned::SPAN_EXCLUSIVE_EXCLUSIVE) return TRUE;
        }
    }

    return FALSE;
}

void SpannableStringBuilder::SendToSpanWatchers(
    /* [in] */ Int32 replaceStart,
    /* [in] */ Int32 replaceEnd,
    /* [in] */ Int32 nbNewChars)
{
    for (Int32 i = 0; i < mSpanCountBeforeAdd; i++) {
        Int32 spanStart = (*mSpanStarts)[i];
        Int32 spanEnd = (*mSpanEnds)[i];
        if (spanStart > mGapStart) spanStart -= mGapLength;
        if (spanEnd > mGapStart) spanEnd -= mGapLength;
        Int32 spanFlags = (*mSpanFlags)[i];

        Int32 newReplaceEnd = replaceEnd + nbNewChars;
        Boolean spanChanged = FALSE;

        Int32 previousSpanStart = spanStart;
        if (spanStart > newReplaceEnd) {
            if (nbNewChars != 0) {
                previousSpanStart -= nbNewChars;
                spanChanged = TRUE;
            }
        } else if (spanStart >= replaceStart) {
            // No change if span start was already at replace interval boundaries before replace
            if ((spanStart != replaceStart ||
                    ((spanFlags & SPAN_START_AT_START) != SPAN_START_AT_START)) &&
                    (spanStart != newReplaceEnd ||
                    ((spanFlags & SPAN_START_AT_END) != SPAN_START_AT_END))) {
                // TODO A correct previousSpanStart cannot be computed at this point.
                // It would require to save all the previous spans' positions before the replace
                // Using an invalid -1 value to convey this would break the broacast range
                spanChanged = TRUE;
            }
        }

        Int32 previousSpanEnd = spanEnd;
        if (spanEnd > newReplaceEnd) {
            if (nbNewChars != 0) {
                previousSpanEnd -= nbNewChars;
                spanChanged = TRUE;
            }
        } else if (spanEnd >= replaceStart) {
            // No change if span start was already at replace interval boundaries before replace
            if ((spanEnd != replaceStart ||
                    ((spanFlags & SPAN_END_AT_START) != SPAN_END_AT_START)) &&
                    (spanEnd != newReplaceEnd ||
                    ((spanFlags & SPAN_END_AT_END) != SPAN_END_AT_END))) {
                // TODO same as above for previousSpanEnd
                spanChanged = TRUE;
            }
        }

        if (spanChanged) {
            SendSpanChanged((*mSpans)[i], previousSpanStart, previousSpanEnd, spanStart, spanEnd);
        }
        (*mSpanFlags)[i] &= ~SPAN_START_END_MASK;
    }

    // The spans starting at mIntermediateSpanCount were added from the replacement text
    for (Int32 i = mSpanCountBeforeAdd; i < mSpanCount; i++) {
        Int32 spanStart = (*mSpanStarts)[i];
        Int32 spanEnd = (*mSpanEnds)[i];
        if (spanStart > mGapStart) spanStart -= mGapLength;
        if (spanEnd > mGapStart) spanEnd -= mGapLength;
        SendSpanAdded((*mSpans)[i], spanStart, spanEnd);
    }
}

void SpannableStringBuilder::SendBeforeTextChanged(
    /* [in] */ ArrayOf<ITextWatcher*>* watchers,
    /* [in] */ Int32 start,
    /* [in] */ Int32 before,
    /* [in] */ Int32 after)
{
    AutoPtr<ISpannable> sp = (ISpannable*)this->Probe(EIID_ISpannable);
    assert(watchers != NULL);

    Int32 n = watchers->GetLength();

    for (Int32 i = 0; i < n; i++) {
        (*watchers)[i]->BeforeTextChanged((ICharSequence*)this->Probe(EIID_ICharSequence),
                start, before, after);
    }
}

void SpannableStringBuilder::SendTextChanged(
    /* [in] */ ArrayOf<ITextWatcher*>* watchers,
    /* [in] */ Int32 start,
    /* [in] */ Int32 before,
    /* [in] */ Int32 after)
{
    assert(watchers != NULL);

    Int32 n = watchers->GetLength();

    for (Int32 i = 0; i < n; i++) {
        (*watchers)[i]->OnTextChanged((ICharSequence*)this->Probe(EIID_ICharSequence),
            start, before, after);
    }
}

void SpannableStringBuilder::SendAfterTextChanged(
    /* [in] */ ArrayOf<ITextWatcher*>* watchers)
{
    assert(watchers != NULL);

    Int32 n = watchers->GetLength();

    for (Int32 i = 0; i < n; i++) {
        (*watchers)[i]->AfterTextChanged((IEditable*)this->Probe(EIID_IEditable));
    }
}

void SpannableStringBuilder::SendSpanAdded(
    /* [in] */ IInterface* what,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    AutoPtr< ArrayOf<IInterface*> > recip = GetSpans(start, end, EIID_ISpanWatcher);
    Int32 n = recip->GetLength();
    for (Int32 i = 0; i < n; i++) {
        AutoPtr<ISpanWatcher> sw = (ISpanWatcher*)((*recip)[i]->Probe(EIID_ISpanWatcher));
        sw->OnSpanAdded(THIS_PROBE(ISpannable), what, start, end);
    }
}

void SpannableStringBuilder::SendSpanRemoved(
    /* [in] */ IInterface* what,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    AutoPtr< ArrayOf<IInterface*> > recip = GetSpans(start, end, EIID_ISpanWatcher);
    Int32 n = recip->GetLength();
    for (Int32 i = 0; i < n; i++) {
        AutoPtr<ISpanWatcher> sw = (ISpanWatcher*)((*recip)[i]->Probe(EIID_ISpanWatcher));
        sw->OnSpanRemoved(THIS_PROBE(ISpannable), what, start, end);
    }
}

void SpannableStringBuilder::SendSpanChanged(
    /* [in] */ IInterface* what,
    /* [in] */ Int32 oldStart,
    /* [in] */ Int32 oldEnd,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    // The bounds of a possible SpanWatcher are guaranteed to be set before this method is
    // called, so that the order of the span does not affect this broadcast.
    Int32 rStart = Elastos::Core::Math::Min(oldStart, start);
    Int32 rEnd = Elastos::Core::Math::Min(Elastos::Core::Math::Max(oldEnd, end), GetLength());
    AutoPtr< ArrayOf<IInterface*> > recip = GetSpans(rStart, rEnd, EIID_ISpanWatcher);

    Int32 n = recip->GetLength();
    for (Int32 i = 0; i < n; i++) {
        AutoPtr<ISpanWatcher> sw = (ISpanWatcher*)((*recip)[i]->Probe(EIID_ISpanWatcher));
        sw->OnSpanChanged(THIS_PROBE(ISpannable), what, oldStart, oldEnd, start, end);
    }
}

String SpannableStringBuilder::Region(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    StringBuffer tmp("(");

    tmp += start;
    tmp += " ... ";
    tmp += end;
    tmp += ")";

    return tmp.ToString();
}

ECode SpannableStringBuilder::CheckRange(
    /* [in] */ const String& operation,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    if (end < start) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    Int32 len = GetLength();
    if (start > len || end > len) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    if (start < 0 || end < 0) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    return NOERROR;
}

ECode SpannableStringBuilder::CheckRange(
    /* [in] */ ICharSequence* tb,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    if (NULL == tb) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (end < start) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    Int32 len = 0;
    tb->GetLength(&len);
    if (start < 0 || start > len || end < 0 || end > len) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    return NOERROR;
}

ECode SpannableStringBuilder::SetSpan(
    /* [in] */ Boolean send,
    /* [in] */ IInterface* what,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ Int32 flags)
{
    FAIL_RETURN(CheckRange(String("setSpan"), start, end));
    Int32 flagsStart = (flags & START_MASK) >> START_SHIFT;
    if (flagsStart == PARAGRAPH) {
        if (start != 0 && start != GetLength()) {
            Char32 c;
            GetCharAt(start - 1, &c);

            if (c != '\n') {
                // throw new RuntimeException("PARAGRAPH span must start at paragraph boundary");
                return E_RUNTIME_EXCEPTION;
            }
        }
    }

    Int32 flagsEnd = flags & END_MASK;
    if (flagsEnd == PARAGRAPH) {
        if (end != 0 && end != GetLength()) {
            Char32 c;
            GetCharAt(end - 1, &c);

            if (c != '\n') {
                // throw new RuntimeException("PARAGRAPH span must end at paragraph boundary");
                return E_RUNTIME_EXCEPTION;
            }
        }
    }

    // 0-length Spanned.SPAN_EXCLUSIVE_EXCLUSIVE
    if (flagsStart == POINT && flagsEnd == MARK && start == end) {
//        if (send) Log.e("SpannableStringBuilder",
//                "SPAN_EXCLUSIVE_EXCLUSIVE spans cannot have a zero length");
        // Silently ignore invalid spans when they are created from this class.
        // This avoids the duplication of the above test code before all the
        // calls to setSpan that are done in this class
        return NOERROR;
    }

    Int32 nstart = start;
    Int32 nend = end;

    if (start > mGapStart) {
        start += mGapLength;
    } else if (start == mGapStart) {
        if (flagsStart == POINT || (flagsStart == PARAGRAPH && start == GetLength())) {
            start += mGapLength;
        }
    }

    if (end > mGapStart) {
        end += mGapLength;
    } else if (end == mGapStart) {
        if (flagsEnd == POINT || (flagsEnd == PARAGRAPH && end == GetLength())) {
            end += mGapLength;
        }
    }

    Int32 count = mSpanCount;
    for (Int32 i = 0; i < count; i++) {
        if ((*mSpans)[i] == what) {
            Int32 ostart = (*mSpanStarts)[i];
            Int32 oend = (*mSpanEnds)[i];

            if (ostart > mGapStart) {
                ostart -= mGapLength;
            }
            if (oend > mGapStart) {
                oend -= mGapLength;
            }

            (*mSpanStarts)[i] = start;
            (*mSpanEnds)[i] = end;
            (*mSpanFlags)[i] = flags;

            if (send) {
                SendSpanChanged(what, ostart, oend, nstart, nend);
            }

            return NOERROR;
        }
    }

    if (mSpanCount + 1 >= mSpans->GetLength()) {
        Int32 newsize = ArrayUtils::IdealInt32ArraySize(mSpanCount + 1);
        AutoPtr<ArrayOf<IInterface*> > newspans = ArrayOf<IInterface*>::Alloc(newsize);
        AutoPtr<ArrayOf<Int32> > newspanstarts = ArrayOf<Int32>::Alloc(newsize);
        AutoPtr<ArrayOf<Int32> > newspanends = ArrayOf<Int32>::Alloc(newsize);
        AutoPtr<ArrayOf<Int32> > newspanflags = ArrayOf<Int32>::Alloc(newsize);

        newspans->Copy(mSpans, 0, mSpanCount);

        assert(mSpanStarts != NULL);
        newspanstarts->Copy(mSpanStarts, 0, mSpanCount);

        newspanends->Copy(mSpanEnds, 0, mSpanCount);

        newspanflags->Copy(mSpanFlags, 0, mSpanCount);

        mSpans = newspans;
        mSpanStarts = newspanstarts;
        mSpanEnds = newspanends;

        mSpanFlags = newspanflags;
    }

    mSpans->Set(mSpanCount, what);
    (*mSpanStarts)[mSpanCount] = start;

    (*mSpanEnds)[mSpanCount] = end;
    (*mSpanFlags)[mSpanCount] = flags;
    mSpanCount++;

    if (send) {
        SendSpanAdded(what, nstart, nend);
    }

    return NOERROR;
}

} // namespace Text
} // namepsace Droid
} // namespace Elastos
