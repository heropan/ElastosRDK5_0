#include "text/method/Touch.h"
#include "text/method/MetaKeyKeyListener.h"
#include "view/CViewConfiguration.h"
#include <elastos/core/Math.h>

using Elastos::Droid::View::CViewConfiguration;
using Elastos::Droid::View::IKeyEvent;

namespace Elastos {
namespace Droid {
namespace Text {
namespace Method {
/*****************************Touch::DragState*****************************/
PInterface Touch::DragState::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)(ITouchDragState*)this;
    }
    else if (riid == EIID_ITouchDragState) {
        return (ITouchDragState*)this;
    }
    else if (riid == EIID_INoCopySpan) {
        return (INoCopySpan*)this;
    }
    return NULL;
}

UInt32 Touch::DragState::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 Touch::DragState::Release()
{
    return ElRefBase::Release();
}

ECode Touch::DragState::GetInterfaceID(
    /* [in] */ IInterface* object,
    /* [out] */ InterfaceID* iID)
{
    VALIDATE_NOT_NULL(iID);
    if (iID == NULL) {
        return E_INVALID_ARGUMENT;
    }

    if (object == (IInterface*)(ITouchDragState*)this) {
        *iID = EIID_ITouchDragState;
    }
    else if (object == (IInterface*)(INoCopySpan*)this) {
        *iID = EIID_INoCopySpan;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

Touch::DragState::DragState(
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ Int32 scrollX,
    /* [in] */ Int32 scrollY)
    : mFarEnough(FALSE)
    , mUsed(FALSE)
{
    mX = x;
    mY = y;
    mScrollX = scrollX;
    mScrollY = scrollY;
}

/*****************************Touch*****************************/
CAR_INTERFACE_IMPL(Touch, Object, ITouch)

Touch::Touch(){}

void Touch::ScrollTo(
    /* [in] */ ITextView* widget,
    /* [in] */ ILayout* layout,
    /* [in] */ Int32 x,
    /* [in] */ Int32 y)
{
    Int32 wTotalPaddingLeft, wTotalPaddingRight, wWidth;

    const Int32 horizontalPadding = (widget->GetTotalPaddingLeft(&wTotalPaddingLeft), wTotalPaddingLeft) + (widget->GetTotalPaddingRight(&wTotalPaddingRight), wTotalPaddingRight);
    const Int32 availableWidth = (widget->GetWidth(&wWidth), wWidth) - horizontalPadding;

    Int32 top;
    layout->GetLineForVertical(y, &top);
    LayoutAlignment a;
    layout->GetParagraphAlignment(top, &a);
    Int32 lParagraphDirection;
    layout->GetParagraphDirection(top, &lParagraphDirection);
    Boolean ltr = lParagraphDirection > 0;

    Int32 left, right;
    Boolean bHorizontallyScrolling = FALSE;
    if ((widget->GetHorizontallyScrolling(&bHorizontallyScrolling), bHorizontallyScrolling)) {
        Int32 wTotalPaddingTop, wTotalPaddingBottom, wHeight;
        const Int32 verticalPadding = (widget->GetTotalPaddingTop(&wTotalPaddingTop), wTotalPaddingTop) + (widget->GetTotalPaddingBottom(&wTotalPaddingBottom), wTotalPaddingBottom);
        Int32 bottom;
        layout->GetLineForVertical(y + (widget->GetHeight(&wHeight), wHeight) - verticalPadding, &bottom);

        left = Integer_MAX_VALUE;
        right = 0;

        for (Int32 i = top; i <= bottom; i++) {
            Float lLineLeft, lLineRight;
            left = (Int32) Elastos::Core::Math::Min((Float)left, (layout->GetLineLeft(i, &lLineLeft), lLineLeft));
            right = (Int32) Elastos::Core::Math::Max((Float)right, (layout->GetLineRight(i, &lLineRight), lLineRight));
        }
    } else {
        left = 0;
        right = availableWidth;
    }

    const Int32 actualWidth = right - left;

    if (actualWidth < availableWidth) {
        if (a == LayoutAlignment.ALIGN_CENTER) {
            x = left - ((availableWidth - actualWidth) / 2);
        } else if ((ltr && (a == LayoutAlignment.ALIGN_OPPOSITE))
                    || (!ltr && (a == LayoutAlignment.ALIGN_NORMAL))
                    || (a == LayoutAlignment.ALIGN_RIGHT)) {
            // align_opposite does NOT mean align_right, we need the paragraph
            // direction to resolve it to left or right
            x = left - (availableWidth - actualWidth);
        } else {
            x = left;
        }
    } else {
        x = Elastos::Core::Math::Min(x, right - availableWidth);
        x = Elastos::Core::Math::Max(x, left);
    }

    widget->ScrollTo(x, y);
}

Boolean Touch::OnTouchEvent(
    /* [in] */ ITextView* widget,
    /* [in] */ ISpannable* buffer,
    /* [in] */ IMotionEvent* event)
{
    AutoPtr< ArrayOf< IInterface* > > ds;
    Int32 actionMasked;
    switch ((event->GetActionMasked(&actionMasked), actionMasked)) {
        case IMotionEvent::ACTION_DOWN: {
            Int32 bufLen = 0;
            buffer->GetLength(&bufLen);
            buffer->GetSpans(0, bufLen, EIID_ITouchDragState, (ArrayOf< IInterface* >**)&ds);

            for (Int32 i = 0; i < ds->GetLength(); i++) {
                buffer->RemoveSpan((*ds)[i]);
            }

            Float eX,eY;
            Int32 wScrollX,wScrollY;
            AutoPtr<DragState> state = new DragState((event->GetX(&eX), eX), (event->GetY(&eY), eY),
                    (widget->GetScrollX(&wScrollX), wScrollX), (widget->GetScrollY(&wScrollY), wScrollY));
            buffer->SetSpan(state, 0, 0, ISpannable::SPAN_MARK_MARK);
            return TRUE;
        }

        case IMotionEvent::ACTION_UP: {
            Int32 bufLen = 0;
            buffer->GetLength(&bufLen);
            buffer->GetSpans(0, bufLen, EIID_ITouchDragState, (ArrayOf< IInterface* >**)&ds);

            for (Int32 i = 0; i < ds->GetLength(); i++) {
                AutoPtr<DragState> state = (DragState*)(*ds)[i];
                AutoPtr<IInterface> obj = state->Probe(EIID_IInterface);
                buffer->RemoveSpan(obj);
            }

            if (ds->GetLength() > 0 && ((DragState*)((*ds)[0]))->mUsed) {
                return TRUE;
            } else {
                return FALSE;
            }
        }

        case IMotionEvent::ACTION_MOVE: {
            Int32 bufLen = 0;
            buffer->GetLength(&bufLen);
            buffer->GetSpans(0, bufLen, EIID_ITouchDragState, (ArrayOf< IInterface* >**)&ds);

            if (ds->GetLength() > 0) {
                ((DragState*)((*ds)[0]))->mIsSelectionStarted = FALSE;
                if ( ((DragState*)((*ds)[0]))->mFarEnough == FALSE ) {
                    AutoPtr<IContext> context;
                    Int32 slop;
                    CViewConfiguration::Get((widget->GetContext((IContext**)&context), context))->GetScaledTouchSlop(&slop);

                    Float eX,eY;

                    if (Elastos::Core::Math::Abs((event->GetX(&eX), eX) - ((DragState*)((*ds)[0]))->mX) >= slop ||
                        Elastos::Core::Math::Abs((event->GetY(&eY), eY) - ((DragState*)((*ds)[0]))->mY) >= slop) {
                        ((DragState*)((*ds)[0]))->mFarEnough = TRUE;
                    }
                }

                if ( ((DragState*)((*ds)[0]))->mFarEnough ) {
                    ((DragState*)((*ds)[0]))->mUsed = TRUE;
                    Int32 metaState;
                    Boolean cap = ((event->GetMetaState(&metaState), metaState) & IKeyEvent::META_SHIFT_ON) != 0
                            || MetaKeyKeyListener::GetMetaState(buffer,
                                    IMetaKeyKeyListener::META_SHIFT_ON) == 1
                            || MetaKeyKeyListener::GetMetaState(buffer,
                                    IMetaKeyKeyListener::META_SELECTING) != 0;
                    Float dx;
                    Float dy;
                    Float ex,ey;
                    if (cap) {
                        // if we're selecting, we want the scroll to go in
                        // the direction of the drag
                        dx = (event->GetX(&ex), ex) - ((DragState*)((*ds)[0]))->mX;
                        dy = (event->GetY(&ey), ey) - ((DragState*)((*ds)[0]))->mY;
                    } else {
                        dx = ((DragState*)((*ds)[0]))->mX - (event->GetX(&ex), ex);
                        dy = ((DragState*)((*ds)[0]))->mY - (event->GetY(&ey), ey);
                    }
                    ((DragState*)((*ds)[0]))->mX = (event->GetX(&ex), ex);
                    ((DragState*)((*ds)[0]))->mY = (event->GetY(&ey), ey);

                    Int32 wScrollX, wScrollY;
                    Int32 nx = (widget->GetScrollX(&wScrollX), wScrollX) + (Int32) dx;
                    Int32 ny = (widget->GetScrollY(&wScrollY), wScrollY) + (Int32) dy;

                    Int32 wTotalPaddingTop, wTotalPaddingBottom;
                    Int32 padding = (widget->GetTotalPaddingTop(&wTotalPaddingTop), wTotalPaddingTop) + (widget->GetTotalPaddingBottom(&wTotalPaddingBottom), wTotalPaddingBottom);
                    AutoPtr<ILayout> layout;
                    widget->GetLayout((ILayout**)&layout);

                    Int32 lHeight, wHeight;
                    ny = Elastos::Core::Math::Min(ny, (layout->GetHeight(&lHeight), lHeight) - ((widget->GetHeight(&wHeight), wHeight) - padding));
                    ny = Elastos::Core::Math::Max(ny, 0);

                    Int32 oldX;
                    widget->GetScrollX(&oldX);
                    Int32 oldY;
                    widget->GetScrollY(&oldY);

                    ScrollTo(widget, layout, nx, ny);

                    // If we actually scrolled, then cancel the up action.
                    Int32 nowX, nowY;
                    if (oldX != (widget->GetScrollX(&nowX), nowX) || oldY != (widget->GetScrollY(&nowY), nowY)) {
                        widget->CancelLongPress();
                    }

                    return TRUE;
                }
            }
        }
    }

    return FALSE;
}

Int32 Touch::GetInitialScrollX(
    /* [in] */ ITextView* widget,
    /* [in] */ ISpannable* buffer)
{
    AutoPtr< ArrayOf< IInterface* > > ds;
    Int32 bufLen = 0;
    buffer->GetLength(&bufLen);
    buffer->GetSpans(0, bufLen, EIID_ITouchDragState, (ArrayOf< IInterface* >**)&ds);
    assert(ds != NULL);

    return ds->GetLength() > 0 ? ((DragState*)((*ds)[0]))->mScrollX : -1;
}

Int32 Touch::GetInitialScrollY(
    /* [in] */ ITextView* widget,
    /* [in] */ ISpannable* buffer)
{
    AutoPtr< ArrayOf< IInterface* > > ds;
    Int32 bufLen = 0;
    buffer->GetLength(&bufLen);
    buffer->GetSpans(0, bufLen, EIID_ITouchDragState, (ArrayOf< IInterface* >**)&ds);
    assert(ds != NULL);

    return ((ds != NULL) && (ds->GetLength() > 0)) ? ((DragState*)((*ds)[0]))->mScrollY : -1;
}

Boolean Touch::IsActivelySelecting(
    /* [in] */ ISpannable* buffer)
{
    ArrayOf<AutoPtr<IDragState> > ds;
    Int32 len;
    buffer->GetLength(&len);
    buffer->GetSpans(0, len, EIID_IDragState, (ArrayOf<IInterface*>**)&ds);

    return ds->GetLength() > 0 && ((DragState*)((*ds)[0]))->mIsActivelySelecting;
}

Boolean Touch::IsSelectionStarted(
    /* [in] */ ISpannable* buffer)
{
    ArrayOf<AutoPtr<IDragState> > ds;
    Int32 len;
    buffer->GetLength(&len);
    buffer->GetSpans(0, len, EIID_IDragState, (ArrayOf<IInterface*>**)&ds);

    return ds->GetLength() > 0 && ((DragState*)((*ds)[0]))->mIsSelectionStarted;
}

} // namespace Method
} // namespace Text
} // namepsace Droid
} // namespace Elastos
