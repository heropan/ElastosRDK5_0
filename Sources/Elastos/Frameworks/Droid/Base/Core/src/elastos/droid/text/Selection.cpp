
#include "elastos/droid/text/Selection.h"
#include <elastos/core/Math.h>
#include <stdio.h>

namespace Elastos {
namespace Droid {
namespace Text {

AutoPtr<IInterface> Selection::SELECTION_START = new Selection::SelectionObject();
AutoPtr<IInterface> Selection::SELECTION_END = new Selection::SelectionObject();

PInterface Selection::SelectionObject::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)this;
    }
    else if (riid == EIID_INoCopySpan) {
        return (INoCopySpan*)this;
    }

    return NULL;
}

UInt32 Selection::SelectionObject::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 Selection::SelectionObject::Release()
{
    return ElRefBase::Release();
}

ECode Selection::SelectionObject::GetInterfaceID(
    /* [in] */ IInterface* object,
    /* [out] */ InterfaceID* IID)
{
    VALIDATE_NOT_NULL(IID);

    if (object == (INoCopySpan*)this) {
        *IID = EIID_INoCopySpan;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

/*
 * Retrieving the selection
 */

/**
 * Return the offset of the selection anchor or cursor, or -1 if
 * there is no selection or cursor.
 */
Int32 Selection::GetSelectionStart(
    /* [in] */ ICharSequence* text)
{
    if (text && ISpanned::Probe(text)) {
        Int32 start = 0;
        ISpanned::Probe(text)->GetSpanStart(SELECTION_START, &start);
        return start;
    }
    else
        return -1;
}

/**
 * Return the offset of the selection edge or cursor, or -1 if
 * there is no selection or cursor.
 */
Int32 Selection::GetSelectionEnd(
    /* [in] */ ICharSequence* text)
{
    if (text && ISpanned::Probe(text)) {
        Int32 end;
        ISpanned::Probe(text)->GetSpanStart(SELECTION_END, &end);
        return end;
    }
    else
        return -1;
}

/*
 * Setting the selection
 */

/**
 * Set the selection anchor to <code>start</code> and the selection edge
 * to <code>stop</code>.
 */
void Selection::SetSelection(
    /* [in] */ ISpannable* text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 stop)
{
    Int32 ostart = GetSelectionStart(text);
    Int32 oend = GetSelectionEnd(text);

    if (ostart != start || oend != stop) {
        text->SetSpan(
            SELECTION_START, start, start,
            ISpanned::SPAN_POINT_POINT | ISpanned::SPAN_INTERMEDIATE);
        text->SetSpan(
            SELECTION_END, stop, stop, ISpanned::SPAN_POINT_POINT);
    }
}

/**
 * Move the cursor to offset <code>index</code>.
 */
void Selection::SetSelection(
    /* [in] */ ISpannable* text,
    /* [in] */ Int32 index)
{
    SetSelection(text, index, index);
}

/**
 * Select the entire text.
 */
void Selection::SelectAll(
    /* [in] */ ISpannable* text)
{
    Int32 len;
    text->GetLength(&len);
    SetSelection(text, 0, len);
}

/**
 * Move the selection edge to offset <code>index</code>.
 */
void Selection::ExtendSelection(
    /* [in] */ ISpannable* text,
    /* [in] */ Int32 index)
{
    Int32 start;
    text->GetSpanStart(SELECTION_END, &start);
    if (start != index)
        text->SetSpan(SELECTION_END, index, index, ISpanned::SPAN_POINT_POINT);
}

/**
 * Remove the selection or cursor, if any, from the text.
 */
void Selection::RemoveSelection(
    /* [in] */ ISpannable* text)
{
    text->RemoveSpan(SELECTION_START);
    text->RemoveSpan(SELECTION_END);
}

/*
 * Moving the selection within the layout
 */

/**
 * Move the cursor to the buffer offset physically above the current
 * offset, or return FALSE if the cursor is already on the top line.
 */
Boolean Selection::MoveUp(
    /* [in] */ ISpannable* text,
    /* [in] */ ILayout* layout)
{
    Int32 start = GetSelectionStart(text);
    Int32 end = GetSelectionEnd(text);

    if (start != end) {
        Int32 min = Elastos::Core::Math::Min(start, end);
        Int32 max = Elastos::Core::Math::Max(start, end);

        SetSelection(text, min);

        Int32 len;
        if (min == 0 && max == (text->GetLength(&len), len)) {
            return FALSE;
        }

        return TRUE;
    }
    else {
        Int32 line;
        layout->GetLineForOffset(end, &line);

        if (line > 0) {
            Int32 move;
            Int32 direction1, direction2;
            layout->GetParagraphDirection(line, &direction1);
            layout->GetParagraphDirection(line - 1, &direction2);
            if (direction1 == direction2) {
                Float h;
                layout->GetPrimaryHorizontal(end, &h);
                layout->GetOffsetForHorizontal(line - 1, h, &move);
            }
            else {
                layout->GetLineStart(line - 1, &move);
            }

            SetSelection(text, move);
            return TRUE;
        }
    }

    return FALSE;
}

/**
 * Move the cursor to the buffer offset physically below the current
 * offset, or return FALSE if the cursor is already on the bottom line.
 */
Boolean Selection::MoveDown(
    /* [in] */ ISpannable* text,
    /* [in] */ ILayout* layout)
{
    Int32 start = GetSelectionStart(text);
    Int32 end = GetSelectionEnd(text);

    if (start != end) {
        Int32 min = Elastos::Core::Math::Min(start, end);
        Int32 max = Elastos::Core::Math::Max(start, end);

        SetSelection(text, max);

        Int32 len;
        if (min == 0 && max == (text->GetLength(&len), len)) {
            return FALSE;
        }

        return TRUE;
    }
    else {
        Int32 line;
        layout->GetLineForOffset(end, &line);

        Int32 lineCount;
        layout->GetLineCount(&lineCount);
        if (line < lineCount - 1) {
            Int32 move;
            Int32 direction1, direction2;
            layout->GetParagraphDirection(line, &direction1);
            layout->GetParagraphDirection(line + 1, &direction2);
            if (direction1 ==direction2) {
                Float h;
                layout->GetPrimaryHorizontal(end, &h);
                layout->GetOffsetForHorizontal(line + 1, h, &move);
            }
            else {
                layout->GetLineStart(line + 1, &move);
            }

            SetSelection(text, move);
            return TRUE;
        }
    }

    return FALSE;
}

/**
 * Move the cursor to the buffer offset physically to the left of
 * the current offset, or return FALSE if the cursor is already
 * at the left edge of the line and there is not another line to move it to.
 */
Boolean Selection::MoveLeft(
    /* [in] */ ISpannable* text,
    /* [in] */ ILayout* layout)
{
    Int32 start = GetSelectionStart(text);
    Int32 end = GetSelectionEnd(text);

    if (start != end) {
        SetSelection(text, ChooseHorizontal(layout, -1, start, end));
        return TRUE;
    }
    else {
        Int32 to;
        layout->GetOffsetToLeftOf(end, &to);
        if (to != end) {
            SetSelection(text, to);
            return TRUE;
        }
    }

    return FALSE;
}

/**
 * Move the cursor to the buffer offset physically to the right of
 * the current offset, or return FALSE if the cursor is already at
 * at the right edge of the line and there is not another line
 * to move it to.
 */
Boolean Selection::MoveRight(
    /* [in] */ ISpannable* text,
    /* [in] */ ILayout* layout)
{
    Int32 start = GetSelectionStart(text);
    Int32 end = GetSelectionEnd(text);

    if (start != end) {
        SetSelection(text, ChooseHorizontal(layout, 1, start, end));
        return TRUE;
    }
    else {
        Int32 to;
        layout->GetOffsetToRightOf(end, &to);
        if (to != end) {
            SetSelection(text, to);
            return TRUE;
        }
    }

    return FALSE;
}

/**
 * Move the selection end to the buffer offset physically above
 * the current selection end.
 */
Boolean Selection::ExtendUp(
    /* [in] */ ISpannable* text,
    /* [in] */ ILayout* layout)
{
    Int32 end = GetSelectionEnd(text);
    Int32 line;
    layout->GetLineForOffset(end, &line);

    if (line > 0) {
        Int32 move;
        Int32 direction1, direction2;
        layout->GetParagraphDirection(line, &direction1);
        layout->GetParagraphDirection(line - 1, &direction2);
        if (direction1 == direction2) {
            Float h;
            layout->GetPrimaryHorizontal(end, &h);
            layout->GetOffsetForHorizontal(line - 1, h, &move);
        }
        else {
            layout->GetLineStart(line - 1, &move);
        }

        ExtendSelection(text, move);
        return TRUE;
    }
    else if (end != 0) {
        ExtendSelection(text, 0);
        return TRUE;
    }

    return TRUE;
}

/**
 * Move the selection end to the buffer offset physically below
 * the current selection end.
 */
Boolean Selection::ExtendDown(
    /* [in] */ ISpannable* text,
    /* [in] */ ILayout* layout)
{
    Int32 end = GetSelectionEnd(text);
    Int32 line;
    layout->GetLineForOffset(end, &line);
    Int32 len;
    Int32 lineCount;
    layout->GetLineCount(&lineCount);
    if (line < lineCount - 1) {
        Int32 move;
        Int32 direction1, direction2;
        layout->GetParagraphDirection(line, &direction1);
        layout->GetParagraphDirection(line + 1, &direction2);
        if (direction1 == direction2) {
            Float h;
            layout->GetPrimaryHorizontal(end, &h);
            layout->GetOffsetForHorizontal(line + 1, h, &move);
        }
        else {
            layout->GetLineStart(line + 1, &move);
        }

        ExtendSelection(text, move);
        return TRUE;
    }
    else if (end != (text->GetLength(&len), len)) {
        ExtendSelection(text, len);
        return TRUE;
    }

    return TRUE;
}

/**
 * Move the selection end to the buffer offset physically to the left of
 * the current selection end.
 */
Boolean Selection::ExtendLeft(
    /* [in] */ ISpannable* text,
    /* [in] */ ILayout* layout)
{
    Int32 end = GetSelectionEnd(text);
    Int32 to;
    layout->GetOffsetToLeftOf(end, &to);

    if (to != end) {
        ExtendSelection(text, to);
        return TRUE;
    }

    return TRUE;
}

/**
 * Move the selection end to the buffer offset physically to the right of
 * the current selection end.
 */
Boolean Selection::ExtendRight(
    /* [in] */ ISpannable* text,
    /* [in] */ ILayout* layout)
{
    Int32 end = GetSelectionEnd(text);
    Int32 to;
    layout->GetOffsetToRightOf(end, &to);

    if (to != end) {
        ExtendSelection(text, to);
        return TRUE;
    }

    return TRUE;
}

Boolean Selection::ExtendToLeftEdge(
    /* [in] */ ISpannable* text,
    /* [in] */ ILayout* layout)
{
    Int32 where = FindEdge(text, layout, -1);
    ExtendSelection(text, where);
    return TRUE;
}

Boolean Selection::ExtendToRightEdge(
    /* [in] */ ISpannable* text,
    /* [in] */ ILayout* layout)
{
    Int32 where = FindEdge(text, layout, 1);
    ExtendSelection(text, where);
    return TRUE;
}

Boolean Selection::MoveToLeftEdge(
    /* [in] */ ISpannable* text,
    /* [in] */ ILayout* layout)
{
    Int32 where = FindEdge(text, layout, -1);
    SetSelection(text, where);
    return TRUE;
}

Boolean Selection::MoveToRightEdge(
    /* [in] */ ISpannable* text,
    /* [in] */ ILayout* layout)
{
    Int32 where = FindEdge(text, layout, 1);
    SetSelection(text, where);
    return TRUE;
}

/** {@hide} */
Boolean Selection::MoveToPreceding(
    /* [in] */ ISpannable* text,
    /* [in] */ ISelectionPositionIterator* iter,
    /* [in] */ Boolean extendSelection)
{
    assert(iter != NULL);

    Int32 offset;
    iter->Preceding(GetSelectionEnd(text), &offset);
//    if (offset != ISelectionPositionIterator::DONE) {
        if (extendSelection) {
            ExtendSelection(text, offset);
        } else {
            SetSelection(text, offset);
        }
//    }
    return TRUE;
}

/** {@hide} */
Boolean Selection::MoveToFollowing(
    /* [in] */ ISpannable* text,
    /* [in] */ ISelectionPositionIterator* iter,
    /* [in] */ Boolean extendSelection)
{
    assert(iter != NULL);

    Int32 offset;
    iter->Following(GetSelectionEnd(text), &offset);
//    if (offset != ISelectionPositionIterator::DONE) {
        if (extendSelection) {
            ExtendSelection(text, offset);
        } else {
            SetSelection(text, offset);
        }
//    }
    return TRUE;
}

Int32 Selection::FindEdge(
    /* [in] */ ISpannable* text,
    /* [in] */ ILayout* layout,
    /* [in] */ Int32 dir)
{
    Int32 pt = GetSelectionEnd(text);
    Int32 line;
    layout->GetLineForOffset(pt, &line);
    Int32 pdir;
    layout->GetParagraphDirection(line, &pdir);

    if (dir * pdir < 0) {
        Int32 start;
        layout->GetLineStart(line, &start);
        return start;
    }
    else {
        Int32 end;
        layout->GetLineEnd(line, &end);
        Int32 lineCount;
        layout->GetLineCount(&lineCount);
        if (line == lineCount - 1)
            return end;
        else
            return end - 1;
    }
}

Int32 Selection::ChooseHorizontal(
    /* [in] */ ILayout* layout,
    /* [in] */ Int32 direction,
    /* [in] */ Int32 off1,
    /* [in] */ Int32 off2)
{
    Int32 line1;
    layout->GetLineForOffset(off1, &line1);
    Int32 line2;
    layout->GetLineForOffset(off2, &line2);

    if (line1 == line2) {
        // same line, so it goes by pure physical direction

        Float h1, h2;
        layout->GetPrimaryHorizontal(off1, &h1);
        layout->GetPrimaryHorizontal(off2, &h2);

        if (direction < 0) {
            // to left
            if (h1 < h2)
                return off1;
            else
                return off2;
        }
        else {
            // to right
            if (h1 > h2)
                return off1;
            else
                return off2;
        }
    }
    else {
        // different line, so which line is "left" and which is "right"
        // depends upon the directionality of the text

        // This only checks at one end, but it's not clear what the
        // right thing to do is if the ends don't agree.  Even if it
        // is wrong it should still not be too bad.
        Int32 line;
        layout->GetLineForOffset(off1, &line);
        Int32 textdir;
        layout->GetParagraphDirection(line, &textdir);

        if (textdir == direction) {
            return Elastos::Core::Math::Max(off1, off2);
        } else {
            return Elastos::Core::Math::Min(off1, off2);
        }
    }
}

} // namespace Text
} // namepsace Droid
} // namespace Elastos