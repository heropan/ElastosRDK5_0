#include "text/method/LinkMovementMethod.h"
#include "text/Selection.h"
//#include "os/ElObject.h"
#include "view/CKeyEvent.h"
#include <elastos/core/Math.h>
//#include <elastos/Core/Object.h>

//using Elastos::Core::Object;
using Elastos::Droid::Text::Selection;
using Elastos::Droid::Text::Style::EIID_IClickableSpan;
using Elastos::Droid::Text::Style::IClickableSpan;
using Elastos::Droid::View::CKeyEvent;
using Elastos::Droid::View::IView;

namespace Elastos {
namespace Droid {
namespace Text {
namespace Method {

const Int32 LinkMovementMethod::CLICK = 1;
const Int32 LinkMovementMethod::UP = 2;
const Int32 LinkMovementMethod::DOWN = 3;

AutoPtr<IInterface> LinkMovementMethod::FROM_BELOW /*= (IInterface*)(new Object())*/;//new ElObject();;// = new NoCopySpan.Concrete();

Boolean LinkMovementMethod::HandleMovementKey(
    /* [in] */ ITextView* widget,
    /* [in] */ ISpannable* buffer,
    /* [in] */ Int32 keyCode,
    /* [in] */ Int32 movementMetaState,
    /* [in] */ IKeyEvent* event)
{
    switch (keyCode) {
        case IKeyEvent::KEYCODE_DPAD_CENTER:
        case IKeyEvent::KEYCODE_ENTER:
            if (CKeyEvent::MetaStateHasNoModifiers(movementMetaState)) {
                Int32 action, repeatCount;
                if ((event->GetAction(&action), action) == IKeyEvent::ACTION_DOWN &&
                        (event->GetRepeatCount(&repeatCount), repeatCount) == 0 && Action(CLICK, widget, buffer)) {
                    return TRUE;
                }
            }
            break;
    }
    return ScrollingMovementMethod::HandleMovementKey(widget, buffer, keyCode, movementMetaState, event);
}

Boolean LinkMovementMethod::Up(
    /* [in] */ ITextView* widget,
    /* [in] */ ISpannable* buffer)
{
    if (Action(UP, widget, buffer)) {
        return TRUE;
    }
    return ScrollingMovementMethod::Up(widget, buffer);
}

Boolean LinkMovementMethod::Down(
    /* [in] */ ITextView* widget,
    /* [in] */ ISpannable* buffer)
{
    if (Action(DOWN, widget, buffer)) {
        return TRUE;
    }

    return ScrollingMovementMethod::Down(widget, buffer);
}

Boolean LinkMovementMethod::Left(
    /* [in] */ ITextView* widget,
    /* [in] */ ISpannable* buffer)
{
    if (Action(UP, widget, buffer)) {
        return TRUE;
    }

    return ScrollingMovementMethod::Left(widget, buffer);
}

Boolean LinkMovementMethod::Right(
    /* [in] */ ITextView* widget,
    /* [in] */ ISpannable* buffer)
{
    if (Action(DOWN, widget, buffer)) {
        return TRUE;
    }

    return ScrollingMovementMethod::Right(widget, buffer);
}

Boolean LinkMovementMethod::Action(
    /* [in] */ Int32 what,
    /* [in] */ ITextView* widget,
    /* [in] */ ISpannable* buffer)
{
printf("==[%s]--==[%s]--==[%d] \n", __FILE__, __FUNCTION__, __LINE__);
    AutoPtr<ILayout> layout;
    widget->GetLayout((ILayout**)&layout);
    Int32 wTotalpaddingTop, wTotalPaddingBottom, wScrollY, wHeight;
    widget->GetTotalPaddingTop(&wTotalpaddingTop);
    widget->GetTotalPaddingBottom(&wTotalPaddingBottom);
    widget->GetScrollY(&wScrollY);
    widget->GetHeight(&wHeight);

    Int32 padding = wTotalpaddingTop +
                      wTotalPaddingBottom;
    Int32 areatop = wScrollY;
    Int32 areabot = areatop + wHeight - padding;

    Int32 linetop;
    layout->GetLineForVertical(areatop, &linetop);
    Int32 linebot;
    layout->GetLineForVertical(areabot, &linebot);

    Int32 first;
    layout->GetLineStart(linetop, &first);
    Int32 last;
    layout->GetLineEnd(linebot, &last);

    AutoPtr< ArrayOf< IClickableSpan* > > candidates;
    buffer->GetSpans(first, last, EIID_IClickableSpan, (ArrayOf< IInterface* >**)&candidates);

    Int32 a = Selection::GetSelectionStart(buffer);
    Int32 b = Selection::GetSelectionEnd(buffer);

    Int32 selStart = Elastos::Core::Math::Min(a, b);
    Int32 selEnd = Elastos::Core::Math::Max(a, b);

    if (selStart < 0) {
        Int32 spanStart;
        if ((buffer->GetSpanStart(FROM_BELOW, &spanStart), spanStart) >= 0) {
            Int32 bufLen;
            buffer->GetLength(&bufLen);
            selStart = selEnd = bufLen;
        }
    }

    if (selStart > last)
        selStart = selEnd = Integer_MAX_VALUE;
    if (selEnd < first)
        selStart = selEnd = -1;

    switch (what) {
        case CLICK:{
            if (selStart == selEnd) {
                return FALSE;
            }

            AutoPtr< ArrayOf< IClickableSpan* > > link;
            buffer->GetSpans(selStart, selEnd, EIID_IClickableSpan, (ArrayOf< IInterface* >**)&link);

            if (link->GetLength() != 1)
                return FALSE;

            (*link)[0]->OnClick((IView*)widget);
            break;
        }

        case UP:{
            Int32 beststart, bestend;

            beststart = -1;
            bestend = -1;

            for (Int32 i = 0; i < candidates->GetLength(); i++) {
                Int32 end;
                buffer->GetSpanEnd((*candidates)[i], &end);

                if (end < selEnd || selStart == selEnd) {
                    if (end > bestend) {
                        buffer->GetSpanStart((*candidates)[i], &beststart);
                        bestend = end;
                    }
                }
            }

            if (beststart >= 0) {
                Selection::SetSelection(buffer, bestend, beststart);
                return TRUE;
            }

            break;
        }

        case DOWN:{
            Int32 beststart, bestend;
            beststart = Integer_MAX_VALUE;
            bestend = Integer_MAX_VALUE;

            for (Int32 i = 0; i < candidates->GetLength(); i++) {
                Int32 start;
                buffer->GetSpanStart( (*candidates)[i], &start);

                if (start > selStart || selStart == selEnd) {
                    if (start < beststart) {
                        beststart = start;
                        buffer->GetSpanEnd( (*candidates)[i], &bestend);
                    }
                }
            }

            if (bestend < Integer_MAX_VALUE) {
                Selection::SetSelection(buffer, beststart, bestend);
                return TRUE;
            }

            break;
        }
    }

    return FALSE;
}

Boolean LinkMovementMethod::OnTouchEvent(
    /* [in] */ ITextView* widget,
    /* [in] */ ISpannable* buffer,
    /* [in] */ IMotionEvent* event)
{
    Int32 action;
    event->GetAction(&action);

    if (action == IMotionEvent::ACTION_UP ||
        action == IMotionEvent::ACTION_DOWN) {
        Float fX,fY;
        event->GetX(&fX);
        event->GetY(&fY);
        Int32 x = (Int32) fX;
        Int32 y = (Int32) fY;

        Int32 wTotalPaddingLeft, wTotalPaddingTop, wScrollX, wScrollY;

        x -= (widget->GetTotalPaddingLeft(&wTotalPaddingLeft), wTotalPaddingLeft);
        y -= (widget->GetTotalPaddingTop(&wTotalPaddingTop), wTotalPaddingTop);

        x += (widget->GetScrollX(&wScrollX), wScrollX);
        y += (widget->GetScrollY(&wScrollY), wScrollY);

        AutoPtr<ILayout> layout;
        widget->GetLayout((ILayout**)&layout);
        Int32 line;
        layout->GetLineForVertical(y, &line);
        Int32 off;
        layout->GetOffsetForHorizontal(line, x, &off);

        AutoPtr< ArrayOf< IClickableSpan* > > link;
        buffer->GetSpans(off, off,  EIID_IClickableSpan, (ArrayOf< IInterface* >**)&link);

        if (link->GetLength() != 0) {
            if (action == IMotionEvent::ACTION_UP) {
                (*link)[0]->OnClick((IView*)widget);
            } else if (action == IMotionEvent::ACTION_DOWN) {
                Int32 spanStart, spanEnd;
                buffer->GetSpanStart((IInterface*)((*link)[0]), &spanStart);
                buffer->GetSpanEnd((IInterface*)((*link)[0]), &spanEnd);
                Selection::SetSelection(buffer, spanStart, spanEnd);
            }

            return TRUE;
        } else {
            Selection::RemoveSelection(buffer);
        }
    }
    return ScrollingMovementMethod::OnTouchEvent(widget, buffer, event);
}

void LinkMovementMethod::Initialize(
    /* [in] */ ITextView* widget,
    /* [in] */ ISpannable* text)
{
    Selection::RemoveSelection(text);
    text->RemoveSpan(FROM_BELOW);
}

void LinkMovementMethod::OnTakeFocus(
    /* [in] */ ITextView* view,
    /* [in] */ ISpannable* text,
    /* [in] */ Int32 dir)
{
    Selection::RemoveSelection(text);

    if ((dir & IView::FOCUS_BACKWARD) != 0) {
        text->SetSpan(FROM_BELOW, 0, 0, ISpannable::SPAN_POINT_POINT);
    } else {
        text->RemoveSpan(FROM_BELOW);
    }
}


} // namespace Method
} // namespace Text
} // namepsace Droid
} // namespace Elastos