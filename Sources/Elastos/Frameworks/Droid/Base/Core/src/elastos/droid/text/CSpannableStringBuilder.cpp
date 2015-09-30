
#include "elastos/droid/text/CSpannableStringBuilder.h"

namespace Elastos {
namespace Droid {
namespace Text {

ECode CSpannableStringBuilder::GetLength(
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);
    *number = SpannableStringBuilder::GetLength();

    return NOERROR;
}

ECode CSpannableStringBuilder::GetCharAt(
    /* [in] */ Int32 index,
    /* [out] */ Char32* c)
{
    VALIDATE_NOT_NULL(c);
    return SpannableStringBuilder::GetCharAt(index, c);
}

ECode CSpannableStringBuilder::SubSequence(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out] */ ICharSequence** csq)
{
    VALIDATE_NOT_NULL(csq);
    return SpannableStringBuilder::SubSequence(start, end, csq);
}

ECode CSpannableStringBuilder::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);
    *str = SpannableStringBuilder::ToString();

    return NOERROR;
}

ECode CSpannableStringBuilder::Substring(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);
    *str = SpannableStringBuilder::Substring(start, end);

    return NOERROR;
}

ECode CSpannableStringBuilder::GetSpans(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ const InterfaceID& type,
    /* [out, callee] */ ArrayOf<IInterface *>** objs)
{
    VALIDATE_NOT_NULL(objs);
    AutoPtr<ArrayOf<IInterface *> > list = SpannableStringBuilder::GetSpans(start, end, type);
    *objs = list.Get();
    REFCOUNT_ADD(*objs);

    return NOERROR;
}

ECode CSpannableStringBuilder::GetSpanStart(
    /* [in] */ IInterface* tag,
    /* [out] */ Int32* start)
{
    VALIDATE_NOT_NULL(start);
    *start = SpannableStringBuilder::GetSpanStart(tag);

    return NOERROR;
}

ECode CSpannableStringBuilder::GetSpanEnd(
    /* [in] */ IInterface* tag,
    /* [out] */ Int32* end)
{
    VALIDATE_NOT_NULL(end);
    *end = SpannableStringBuilder::GetSpanEnd(tag);

    return NOERROR;
}

ECode CSpannableStringBuilder::GetSpanFlags(
    /* [in] */ IInterface* tag,
    /* [out] */ Int32* flags)
{
    VALIDATE_NOT_NULL(flags);
    *flags = SpannableStringBuilder::GetSpanFlags(tag);

    return NOERROR;
}

ECode CSpannableStringBuilder::NextSpanTransition(
    /* [in] */ Int32 start,
    /* [in] */ Int32 limit,
    /* [in] */ const InterfaceID& type,
    /* [out] */ Int32* offset)
{
    VALIDATE_NOT_NULL(offset);
    *offset = SpannableStringBuilder::NextSpanTransition(start, limit, type);

    return NOERROR;
}

ECode CSpannableStringBuilder::SetSpan(
    /* [in] */ IInterface* what,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ Int32 flags)
{
    return SpannableStringBuilder::SetSpan(what, start, end, flags);
}

ECode CSpannableStringBuilder::RemoveSpan(
    /* [in] */ IInterface* what)
{
    return SpannableStringBuilder::RemoveSpan(what);
}

ECode CSpannableStringBuilder::Replace(
    /* [in] */ Int32 st,
    /* [in] */ Int32 en,
    /* [in] */ ICharSequence* source,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    return SpannableStringBuilder::Replace(st, en, source, start, end);
}

ECode CSpannableStringBuilder::Replace(
    /* [in] */ Int32 st,
    /* [in] */ Int32 en,
    /* [in] */ ICharSequence* text)
{
    return SpannableStringBuilder::Replace(st, en, text);
}

ECode CSpannableStringBuilder::Insert(
    /* [in] */ Int32 where,
    /* [in] */ ICharSequence* text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    return SpannableStringBuilder::Insert(where, text, start, end);
}

ECode CSpannableStringBuilder::Insert(
    /* [in] */ Int32 where,
    /* [in] */ ICharSequence* text)
{
    return SpannableStringBuilder::Insert(where, text);
}

ECode CSpannableStringBuilder::Delete(
    /* [in] */ Int32 st,
    /* [in] */ Int32 en)
{
    return SpannableStringBuilder::Delete(st, en);
}

ECode CSpannableStringBuilder::Append(
    /* [in] */ ICharSequence* text)
{
    return SpannableStringBuilder::Append(text);
}

ECode CSpannableStringBuilder::Append(
    /* [in] */ ICharSequence* text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    return SpannableStringBuilder::Append(text, start, end);
}

ECode CSpannableStringBuilder::Append(
    /* [in] */ Char32 text)
{
    return SpannableStringBuilder::Append(text);
}

ECode CSpannableStringBuilder::Clear()
{
    return SpannableStringBuilder::Clear();
}

ECode CSpannableStringBuilder::ClearSpans()
{
    return SpannableStringBuilder::ClearSpans();
}

ECode CSpannableStringBuilder::SetFilters(
    /* [in] */ ArrayOf<IInputFilter*>* filters)
{
    return SpannableStringBuilder::SetFilters(filters);
}

ECode CSpannableStringBuilder::GetFilters(
    /* [out, callee] */ ArrayOf<IInputFilter*>** filters)
{
    VALIDATE_NOT_NULL(filters);
    AutoPtr< ArrayOf<IInputFilter*> > objs = SpannableStringBuilder::GetFilters();
    *filters = objs;
    REFCOUNT_ADD(*filters);
    return NOERROR;
}

ECode CSpannableStringBuilder::GetChars(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out] */ ArrayOf<Char32>* dest,
    /* [in] */ Int32 destoff)
{
    VALIDATE_NOT_NULL(dest);
    return SpannableStringBuilder::GetChars(start, end, dest, destoff);
}

ECode CSpannableStringBuilder::DrawText(
    /* [in] */ ICanvas* c,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ IPaint* p)
{
    return SpannableStringBuilder::DrawText(c, start, end, x, y, p);
}

ECode CSpannableStringBuilder::DrawTextRun(
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
    return SpannableStringBuilder::DrawTextRun(c, start, end, contextStart,
                 contextEnd, x, y, flags, p);
}

ECode CSpannableStringBuilder::MeasureText(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ IPaint* p,
    /* [out] */ Float* width)
{
    VALIDATE_NOT_NULL(width);
    return SpannableStringBuilder::MeasureText(start, end, p, width);
}

ECode CSpannableStringBuilder::GetTextWidths(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out] */ ArrayOf<Float>* widths,
    /* [in] */ IPaint* p,
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(widths);
    VALIDATE_NOT_NULL(count);
    return SpannableStringBuilder::GetTextWidths(start, end, widths, p, count);
}

ECode CSpannableStringBuilder::GetTextRunAdvances(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ Int32 contextStart,
    /* [in] */ Int32 contextEnd,
    /* [in] */ Int32 flags,
    /* [in] */ ArrayOf<Float>* advances,
    /* [in] */ Int32 advancesPos,
    /* [in] */ IPaint* p,
    /* [out] */ Float* result)
{
    VALIDATE_NOT_NULL(result);
    *result = SpannableStringBuilder::GetTextRunAdvances(start, end,
        contextStart, contextEnd, flags, advances, advancesPos, p);

    return NOERROR;
}

ECode CSpannableStringBuilder::GetTextRunAdvances(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ Int32 contextStart,
    /* [in] */ Int32 contextEnd,
    /* [in] */ Int32 flags,
    /* [in] */ ArrayOf<Float>* advances,
    /* [in] */ Int32 advancesPos,
    /* [in] */ IPaint* p,
    /* [in] */ Int32 reserved,
    /* [out] */ Float* result)
{
    VALIDATE_NOT_NULL(result);
    *result = SpannableStringBuilder::GetTextRunAdvances(start, end,
        contextStart, contextEnd, flags, advances, advancesPos, p, reserved);

    return NOERROR;
}

ECode CSpannableStringBuilder::GetTextRunCursor(
    /* [in] */ Int32 contextStart,
    /* [in] */ Int32 contextEnd,
    /* [in] */ Int32 flags,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 cursorOpt,
    /* [in] */ IPaint* p,
    /* [out] */ Int32* cursor)
{
    VALIDATE_NOT_NULL(cursor);
    *cursor = SpannableStringBuilder::GetTextRunCursor(contextStart,
        contextEnd, flags, offset, cursorOpt, p);

    return NOERROR;
}

ECode CSpannableStringBuilder::constructor()
{
    return SpannableStringBuilder::Init();
}

ECode CSpannableStringBuilder::constructor(
    /* [in] */ ICharSequence* source)
{
    return SpannableStringBuilder::Init(source);
}

ECode CSpannableStringBuilder::constructor(
    /* [in] */ ICharSequence* source,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    return SpannableStringBuilder::Init(source, start, end);
}

PInterface CSpannableStringBuilder::Probe(
    /* [in] */ REIID riid)
{
    return _CSpannableStringBuilder::Probe(riid);
}

} // namespace Text
} // namepsace Droid
} // namespace Elastos
