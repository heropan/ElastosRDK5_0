
#ifndef __ELASTOS_DROID_TEXT_CSPANNABLESTRINGBUILDER_H__
#define __ELASTOS_DROID_TEXT_CSPANNABLESTRINGBUILDER_H__

#include "_CSpannableStringBuilder.h"
#include "text/SpannableStringBuilder.h"

namespace Elastos {
namespace Droid {
namespace Text {

CarClass(CSpannableStringBuilder), public SpannableStringBuilder
{
public:
    CARAPI GetLength(
        /* [out] */ Int32* number);

    CARAPI GetCharAt(
        /* [in] */ Int32 index,
        /* [out] */ Char32* c);

    CARAPI SubSequence(
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [out] */ ICharSequence** csq);

    CARAPI ToString(
        /* [out] */ String* str);

    /**
     * Return a String containing a copy of the chars in this buffer, limited to the
     * [start, end[ range.
     * @hide
     */
    CARAPI Substring(
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [out] */ String* str);

    CARAPI GetSpans(
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ const InterfaceID& type,
        /* [out, callee] */ ArrayOf<IInterface*>** objs);

    CARAPI GetSpanStart(
        /* [in] */ IInterface* tag,
        /* [out] */ Int32* start);

    CARAPI GetSpanEnd(
        /* [in] */ IInterface* tag,
        /* [out] */ Int32* end);

    CARAPI GetSpanFlags(
        /* [in] */ IInterface* tag,
        /* [out] */ Int32* flags);

    CARAPI NextSpanTransition(
        /* [in] */ Int32 start,
        /* [in] */ Int32 limit,
        /* [in] */ const InterfaceID& type,
        /* [out] */ Int32* offset);

    CARAPI SetSpan(
        /* [in] */ IInterface* what,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ Int32 flags);

    CARAPI RemoveSpan(
        /* [in] */ IInterface* what);

    CARAPI Replace(
        /* [in] */ Int32 st,
        /* [in] */ Int32 en,
        /* [in] */ ICharSequence* source,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    CARAPI Replace(
        /* [in] */ Int32 st,
        /* [in] */ Int32 en,
        /* [in] */ ICharSequence* text);

    CARAPI Insert(
        /* [in] */ Int32 where,
        /* [in] */ ICharSequence* text,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    CARAPI Insert(
        /* [in] */ Int32 where,
        /* [in] */ ICharSequence* text);

    CARAPI Delete(
        /* [in] */ Int32 st,
        /* [in] */ Int32 en);

    CARAPI Append(
        /* [in] */ ICharSequence* text);

    CARAPI Append(
        /* [in] */ ICharSequence* text,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    CARAPI Append(
        /* [in] */ Char32 text);

    CARAPI Clear();

    CARAPI ClearSpans();

    CARAPI SetFilters(
        /* [in] */ ArrayOf<IInputFilter*>* filters);

    CARAPI GetFilters(
        /* [out, callee] */ ArrayOf<IInputFilter*>** filters);

    CARAPI GetChars(
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [out] */ ArrayOf<Char32>* dest,
        /* [in] */ Int32 destoff);

    CARAPI DrawText(
        /* [in] */ ICanvas* c,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ Float x,
        /* [in] */ Float y,
        /* [in] */ IPaint* p);

    /**
     * Don't call this yourself -- exists for Canvas to use internally.
     * {@hide}
     */
    CARAPI DrawTextRun(
        /* [in] */ ICanvas* c,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ Int32 contextStart,
        /* [in] */ Int32 contextEnd,
        /* [in] */ Float x,
        /* [in] */ Float y,
        /* [in] */ Int32 flags,
        /* [in] */ IPaint* p);

    CARAPI MeasureText(
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ IPaint* p,
        /* [out] */ Float* width);

    CARAPI GetTextWidths(
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [out] */ ArrayOf<Float>* widths,
        /* [in] */ IPaint* p,
        /* [out] */ Int32* count);

    /**
     * Don't call this yourself -- exists for Paint to use internally.
     * {@hide}
     */
    CARAPI GetTextRunAdvances(
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ Int32 contextStart,
        /* [in] */ Int32 contextEnd,
        /* [in] */ Int32 flags,
        /* [in] */ ArrayOf<Float>* advances,
        /* [in] */ Int32 advancesPos,
        /* [in] */ IPaint* p,
        /* [out] */ Float* result);

    /**
     * Don't call this yourself -- exists for Paint to use internally.
     * {@hide}
     */
    CARAPI GetTextRunAdvances(
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ Int32 contextStart,
        /* [in] */ Int32 contextEnd,
        /* [in] */ Int32 flags,
        /* [in] */ ArrayOf<Float>* advances,
        /* [in] */ Int32 advancesPos,
        /* [in] */ IPaint* p,
        /* [in] */ Int32 reserved,
        /* [out] */ Float* result);

    /**
     * Returns the next cursor position in the run.  This avoids placing the cursor between
     * surrogates, between characters that form conjuncts, between base characters and combining
     * marks, or within a reordering cluster.
     *
     * <p>The context is the shaping context for cursor movement, generally the bounds of the metric
     * span enclosing the cursor in the direction of movement.
     * <code>contextStart</code>, <code>contextEnd</code> and <code>offset</code> are relative to
     * the start of the string.</p>
     *
     * <p>If cursorOpt is CURSOR_AT and the offset is not a valid cursor position,
     * this returns -1.  Otherwise this will never return a value before contextStart or after
     * contextEnd.</p>
     *
     * @param contextStart the start index of the context
     * @param contextEnd the (non-inclusive) end index of the context
     * @param flags either DIRECTION_RTL or DIRECTION_LTR
     * @param offset the cursor position to move from
     * @param cursorOpt how to move the cursor, one of CURSOR_AFTER,
     * CURSOR_AT_OR_AFTER, CURSOR_BEFORE,
     * CURSOR_AT_OR_BEFORE, or CURSOR_AT
     * @param p the Paint object that is requesting this information
     * @return the offset of the next position, or -1
     * @deprecated This is an internal method, refrain from using it in your code
     */
    //@Deprecated
    CARAPI GetTextRunCursor(
        /* [in] */ Int32 contextStart,
        /* [in] */ Int32 contextEnd,
        /* [in] */ Int32 flags,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 cursorOpt,
        /* [in] */ IPaint* p,
        /* [out] */ Int32* cursor);

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ ICharSequence* source);

    CARAPI constructor(
        /* [in] */ ICharSequence* source,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);
};

} // namespace Text
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_TEXT_CSPANNABLESTRINGBUILDER_H__
