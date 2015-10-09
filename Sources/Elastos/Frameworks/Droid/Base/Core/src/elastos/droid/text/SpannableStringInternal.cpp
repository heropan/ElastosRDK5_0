
#include "elastos/droid/text/SpannableStringInternal.h"
#include <util/ArrayUtils.h>
#include <elastos/core/Math.h>
#include <elastos/core/StringBuffer.h>

using Elastos::Core::IComparable;
using Elastos::Core::StringBuffer;
using Elastos::Droid::Internal::Utility::ArrayUtils;

namespace Elastos {
namespace Droid {
namespace Text {

/*const*/ AutoPtr< ArrayOf<IInterface*> > SpannableStringInternal::EMPTY;// = ArrayOf<IInterface*>::Alloc(0);
const Int32 SpannableStringInternal::START;
const Int32 SpannableStringInternal::END;
const Int32 SpannableStringInternal::FLAGS;
const Int32 SpannableStringInternal::COLUMNS;

SpannableStringInternal::SpannableStringInternal()
{}

SpannableStringInternal::SpannableStringInternal(
    /* [in] */ ICharSequence* source,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    ASSERT_SUCCEEDED(Init(source, start, end));
}

SpannableStringInternal::~SpannableStringInternal()
{
    mSpans = NULL;
    mSpanData = NULL;
}

Int32 SpannableStringInternal::GetLength()
{
    return mText.GetLength();
}

Char32 SpannableStringInternal::GetCharAt(
    /* [in] */ Int32 i)
{
    return mText.GetChar(i);
}

ECode SpannableStringInternal::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);
    *str = mText;
    return NOERROR;
}

ECode SpannableStringInternal::GetChars(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out] */ ArrayOf<Char32>* dest,
    /* [in] */ Int32 off)
{
    AutoPtr< ArrayOf<Char32> > ans = mText.GetChars();
    if (ans->GetLength() > 0)
        dest->Copy(off, ans, start, end - start + 1);

    return NOERROR;
}

/* package */
ECode SpannableStringInternal::SetSpan(
    /* [in] */ IInterface* what,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ Int32 flags)
{
    Int32 nstart = start;
    Int32 nend = end;

    FAIL_RETURN(CheckRange("SetSpan", start, end));

    if ((flags & ISpanned::SPAN_PARAGRAPH) == ISpanned::SPAN_PARAGRAPH) {
        if (start != 0 && start != GetLength()) {
            Char32 c = GetCharAt(start - 1);

            if (c != '\n') {
                ELDBG("PARAGRAPH span must start at paragraph boundary (%d follows %c", start, c);
                return E_RUNTIME_EXCEPTION;
            }
        }

        if (end != 0 && end != GetLength()) {
            Char32 c = GetCharAt(end - 1);

            if (c != '\n') {
                ELDBG("PARAGRAPH span must end at paragraph boundary (%d follows %c", end, c);
                return E_RUNTIME_EXCEPTION;
            }
        }
    }

    Int32 count = mSpanCount;
    for (Int32 i = 0; i < count; i++) {
        if (IsSpanEquals((*mSpans)[i], what)) {
            Int32 ostart = (*mSpanData)[i * COLUMNS + START];
            Int32 oend = (*mSpanData)[i * COLUMNS + END];

            (*mSpanData)[i * COLUMNS + START] = start;
            (*mSpanData)[i * COLUMNS + END] = end;
            (*mSpanData)[i * COLUMNS + FLAGS] = flags;

            SendSpanChanged(what, ostart, oend, nstart, nend);
            return NOERROR;
        }
    }

    if (mSpanCount + 1 >= mSpans->GetLength()) {
        AutoPtr<ArrayOf<IInterface*> > newtags = ArrayUtils::NewUnpaddedObjectArray(GrowingArrayUtils::GrowSize(mSpanCount));
        AutoPtr<ArrayOf<Int32> > newdata = ArrayOf<Int32>::Alloc(newtags->GetLength() * 3);

        newtags->Copy(0, mSpans, 0, mSpanCount);
        newdata->Copy(0, mSpanData, 0, mSpanCount * 3);

        mSpans = newtags;
        mSpanData = newdata;
    }

    mSpans->Set(mSpanCount, what);
    (*mSpanData)[mSpanCount * COLUMNS + START] = start;
    (*mSpanData)[mSpanCount * COLUMNS + END] = end;
    (*mSpanData)[mSpanCount * COLUMNS + FLAGS] = flags;
    mSpanCount++;

    if (this->Probe(EIID_ISpannable) != NULL) {
        SendSpanAdded(what, nstart, nend);
    }
    return NOERROR;
}

/* package */
ECode SpannableStringInternal::RemoveSpan(
    /* [in] */ IInterface* what)
{
    Int32 count = mSpanCount;
    for (Int32 i = count - 1; i >= 0; i--) {
        if (IsSpanEquals((*mSpans)[i], what)) {
            Int32 ostart = (*mSpanData)[i * COLUMNS + START];
            Int32 oend = (*mSpanData)[i * COLUMNS + END];

            Int32 c = count - (i + 1);

            mSpans->Set(i, NULL);

            mSpans->Copy(i, mSpans, i + 1, c);
            mSpanData->Copy(i * COLUMNS, mSpanData, (i + 1) * COLUMNS, c * COLUMNS);

            mSpanCount--;

            SendSpanRemoved(what, ostart, oend);
            return NOERROR;
        }
    }
    return NOERROR;
}

Boolean SpannableStringInternal::IsSpanEquals(
    /* [in] */ IInterface* lhs,
    /* [in] */ IInterface* rhs)
{
    if (lhs == NULL && rhs == NULL) {
        return TRUE;
    }

    if ((lhs == NULL && rhs != NULL) || (lhs != NULL && rhs == NULL)) {
        return FALSE;
    }

    IComparable* cpr = (IComparable*)(lhs->Probe(Elastos::Core::EIID_IComparable));
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

Int32 SpannableStringInternal::GetSpanStart(
    /* [in] */ IInterface* what)
{
    Int32 count = mSpanCount;
    for (Int32 i = count - 1; i >= 0; i--) {
        if (IsSpanEquals((*mSpans)[i], what)) {
            return (*mSpanData)[i * COLUMNS + START];
        }
    }

    return -1;
}

Int32 SpannableStringInternal::GetSpanEnd(
    /* [in] */ IInterface* what)
{
    Int32 count = mSpanCount;
    for (Int32 i = count - 1; i >= 0; i--) {
        if (IsSpanEquals((*mSpans)[i], what)) {
            return (*mSpanData)[i * COLUMNS + END];
        }
    }

    return -1;
}

Int32 SpannableStringInternal::GetSpanFlags(
    /* [in] */ IInterface* what)
{
    Int32 count = mSpanCount;
    for (Int32 i = count - 1; i >= 0; i--) {
        if (IsSpanEquals((*mSpans)[i], what)) {
            return (*mSpanData)[i * COLUMNS + FLAGS];
        }
    }

    return 0;
}

ECode SpannableStringInternal::GetSpans(
    /* [in] */ Int32 queryStart,
    /* [in] */ Int32 queryEnd,
    /* [in] */ const InterfaceID& kind,
    /* [out, callee] */ ArrayOf<IInterface*>** objs)
{
    assert(objs != NULL);
    *objs = NULL;

    Int32 count = 0;

    Int32 spanCount = mSpanCount;
    AutoPtr<ArrayOf<IInterface*> > ret;
    AutoPtr<IInterface> ret1;

    for (Int32 i = 0; i < spanCount; i++) {
        if (((*mSpans)[i] == NULL) || ((*mSpans)[i]->Probe(kind) == NULL)) {
            continue;
        }

        Int32 spanStart = (*mSpanData)[i * COLUMNS + START];
        Int32 spanEnd = (*mSpanData)[i * COLUMNS + END];

        if (spanStart > queryEnd) {
            continue;
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

        if (count == 0) {
            ret1 = (*mSpans)[i];
            count++;
        }
        else {
            if (count == 1) {
                ret = ArrayOf<IInterface*>::Alloc(spanCount - i + 1);
                ret->Set(0, ret1);
            }

            Int32 prio = (*mSpanData)[i * COLUMNS + FLAGS] & ISpanned::SPAN_PRIORITY;
            if (prio != 0) {
                Int32 j;

                for (j = 0; j < count; j++) {
                    Int32 p = GetSpanFlags((*ret)[j]) & ISpanned::SPAN_PRIORITY;

                    if (prio > p) {
                        break;
                    }
                }

                ret->Copy(j + 1, ret, j, count - j);
                ret->Set(j, (*mSpans)[i]);
                count++;
            } else {
                ret->Set(count++, (*mSpans)[i]);
            }
        }
    }

    if (count == 0) {
        *objs = ArrayOf<IInterface*>::Alloc(0);
        REFCOUNT_ADD(*objs);
        return NOERROR;
    }
    else if (count == 1) {
        *objs = ArrayOf<IInterface*>::Alloc(1);
        (*objs)->Set(0, ret1);
        REFCOUNT_ADD(*objs);
        return NOERROR;
    }
    else if (count == ret->GetLength()) {
        *objs = ret.Get();
        REFCOUNT_ADD(*objs);
        return NOERROR;
    }

    *objs = ArrayOf<IInterface*>::Alloc(count);
    (*objs)->Copy(0, ret, 0, count);
    REFCOUNT_ADD(*objs);
    return NOERROR;
}

Int32 SpannableStringInternal::NextSpanTransition(
    /* [in] */ Int32 start,
    /* [in] */ Int32 limit,
    /* [in] */ const InterfaceID& kind)
{
    Int32 count = mSpanCount;
//    if (kind == null) {
//        kind = Object.class;
//    }

    for (Int32 i = 0; i < count; i++) {
        Int32 st = (*mSpanData)[i * COLUMNS + START];
        Int32 en = (*mSpanData)[i * COLUMNS + END];

        if (st > start && st < limit && (*mSpans)[i]->Probe(kind) != NULL) {
            limit = st;
        }
        if (en > start && en < limit && (*mSpans)[i]->Probe(kind) != NULL) {
            limit = en;
        }
    }

    return limit;
}

void SpannableStringInternal::SendSpanAdded(
    /* [in] */ IInterface* what,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    AutoPtr<ArrayOf<IInterface*> > recip;
    GetSpans(start, end, EIID_ISpanWatcher, (ArrayOf<IInterface*>**)&recip);

    Int32 N = recip->GetLength();
    for (Int32 i = 0; i < N; i++) {
        ISpanWatcher* span = ISpanWatcher::Probe((*recip)[i]);
        span->OnSpanAdded(THIS_PROBE(ISpannable), what, start, end);
    }
}

void SpannableStringInternal::SendSpanRemoved(
    /* [in] */ IInterface* what,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    AutoPtr<ArrayOf<IInterface*> > recip;
    GetSpans(start, end, EIID_ISpanWatcher, (ArrayOf<IInterface*>**)&recip);

    Int32 N = recip->GetLength();
    for (int i = 0; i < N; i++) {
        ISpanWatcher* span = ISpanWatcher::Probe((*recip)[i]);
        span->OnSpanRemoved(THIS_PROBE(ISpannable), what, start, end);
    }
}

void SpannableStringInternal::SendSpanChanged(
    /* [in] */ IInterface* what,
    /* [in] */ Int32 s,
    /* [in] */ Int32 e,
    /* [in] */ Int32 st,
    /* [in] */ Int32 en)
{
    using Elastos::Core::Math;
    AutoPtr<ArrayOf<IInterface*> > recip;
    GetSpans(Math::Min(s, st), Math::Max(e, en), EIID_ISpanWatcher, (ArrayOf<IInterface*>**)&recip);

    Int32 N = recip->GetLength();
    for (int i = 0; i < N; i++) {
        ISpanWatcher* span = ISpanWatcher::Probe((*recip)[i]);
        span->OnSpanChanged(THIS_PROBE(ISpannable), what, s, e, st, en);
    }
}

void SpannableStringInternal::Region(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out] */ String* str)
{
    assert(str != NULL);
    StringBuffer sb("(");
    sb += start;
    sb += " ... ";
    sb += end;
    sb += ")";
    *str = sb.ToString();
}

ECode SpannableStringInternal::CheckRange(
    /* [in] */ const char* operation,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    if (end < start) {
//        throw new IndexOutOfBoundsException(operation + " " +
//                                            region(start, end) +
//                                            " has end before start");
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    Int32 len = GetLength();

    if (start > len || end > len) {
//        throw new IndexOutOfBoundsException(operation + " " +
//                                            region(start, end) +
//                                            " ends beyond length " + len);
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    if (start < 0 || end < 0) {
//        throw new IndexOutOfBoundsException(operation + " " +
//                                            region(start, end) +
//                                            " starts before 0");
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    return NOERROR;
}

ECode SpannableStringInternal::Init(
    /* [in] */ ICharSequence* source,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    VALIDATE_NOT_NULL(source);

    Int32 len;
    source->GetLength(&len);
    if (start == 0 && end == len) {
        source->ToString(&mText);
    }
    else {
        String str;
        source->ToString(&str);
        mText = str.Substring(start, end);
    }

    mSpans = EmptyArray::OBJECT;
    mSpanData = EmptyArray::INT32;

    if (ISpanned::Probe(source)) {
        AutoPtr<ISpanned> sp = ISpanned::Probe(source);
        AutoPtr<ArrayOf<IInterface*> > spans;
        sp->GetSpans(start, end, EIID_IInterface, (ArrayOf<IInterface*>**)&spans);
        if (spans) {
            Int32 st, en, fl;
            for (Int32 i = 0; i < spans->GetLength(); i++) {
                sp->GetSpanStart((*spans)[i], &st);
                sp->GetSpanEnd((*spans)[i], &en);
                sp->GetSpanFlags((*spans)[i], &fl);

                if (st < start) {
                    st = start;
                }
                if (en > end) {
                    en = end;
                }
                SetSpan((*spans)[i], st - start, en - start, fl);
            }
        }
    }
    return NOERROR;
}

// Same as SpannableStringBuilder
// @Override
ECode SpannableStringInternal::Equals(
    /* [in] */ IInterface* o,
    /* [out] */ Boolean* result)
{
    assert(0 && "TODO");
    if (o instanceof Spanned &&
            toString().equals(o.toString())) {
        Spanned other = (Spanned) o;
        // Check span data
        Object[] otherSpans = other.getSpans(0, other.length(), Object.class);
        if (mSpanCount == otherSpans.length) {
            for (int i = 0; i < mSpanCount; ++i) {
                Object thisSpan = mSpans[i];
                Object otherSpan = otherSpans[i];
                if (thisSpan == this) {
                    if (other != otherSpan ||
                            getSpanStart(thisSpan) != other.getSpanStart(otherSpan) ||
                            getSpanEnd(thisSpan) != other.getSpanEnd(otherSpan) ||
                            getSpanFlags(thisSpan) != other.getSpanFlags(otherSpan)) {
                        return false;
                    }
                } else if (!thisSpan.equals(otherSpan) ||
                        getSpanStart(thisSpan) != other.getSpanStart(otherSpan) ||
                        getSpanEnd(thisSpan) != other.getSpanEnd(otherSpan) ||
                        getSpanFlags(thisSpan) != other.getSpanFlags(otherSpan)) {
                    return false;
                }
            }
            return true;
        }
    }
    return false;
}

// Same as SpannableStringBuilder
ECode SpannableStringInternal::GetHashCode(
    /* [out] */ Int32* result)
{
    assert(0 && "TODO");
    // int hash = toString().hashCode();
    // hash = hash * 31 + mSpanCount;
    // for (int i = 0; i < mSpanCount; ++i) {
    //     Object span = mSpans[i];
    //     if (span != this) {
    //         hash = hash * 31 + span.hashCode();
    //     }
    //     hash = hash * 31 + getSpanStart(span);
    //     hash = hash * 31 + getSpanEnd(span);
    //     hash = hash * 31 + getSpanFlags(span);
    // }
    // return hash;
    return NOERROR;
}

} // namespace Text
} // namepsace Droid
} // namespace Elastos
