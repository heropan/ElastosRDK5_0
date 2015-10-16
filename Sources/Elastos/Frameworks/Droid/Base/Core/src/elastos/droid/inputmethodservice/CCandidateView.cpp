
#include "elastos/droid/inputmethodservice/CCandidateView.h"
// #include "elastos/droid/graphics/CPaint.h"
#include "elastos/droid/R.h"

using Elastos::Droid::R;
// using Elastos::Droid::View::EIID_View;
using Elastos::Droid::Content::Res::IResources;

namespace Elastos {
namespace Droid {
namespace InputMethodService {

const Int32 CCandidateView::OUT_OF_BOUNDS;
const Int32 CCandidateView::MAX_SUGGESTIONS;
const Int32 CCandidateView::SCROLL_PIXELS;
const Int32 CCandidateView::X_GAP;
AutoPtr< ArrayOf<ICharSequence*> > CCandidateView::EMPTY_LIST = ArrayOf<ICharSequence*>::Alloc(0);

CCandidateView::SelfSimpleOnGestureListener::SelfSimpleOnGestureListener(
    /* [in] */ CCandidateView* host)
    : mHost(host)
{}

CCandidateView::SelfSimpleOnGestureListener::~SelfSimpleOnGestureListener()
{}

Boolean CCandidateView::SelfSimpleOnGestureListener::OnScroll(
    /* [in] */ IMotionEvent* e1,
    /* [in] */ IMotionEvent* e2,
    /* [in] */ Float distanceX,
    /* [in] */ Float distanceY)
{
    assert(0 && "TODO");
    // assert(mHost != NULL);
    // mHost->mScrolled = TRUE;
    // Int32 sx = mHost->GetScrollX();
    // sx += distanceX;
    // if (sx < 0) {
    //     sx = 0;
    // }
    // if (sx + mHost->GetWidth() > mHost->mTotalWidth) {
    //     sx -= distanceX;
    // }
    // mHost->mTargetScrollX = sx;
    // mHost->ScrollTo(sx, mHost->GetScrollY());
    // mHost->Invalidate();
    return TRUE;
}

CAR_OBJECT_IMPL(CCandidateView);
CAR_INTERFACE_IMPL(CCandidateView, Object/*View*/, ICandidateView);
CCandidateView::CCandidateView()
    : mSuggestions(NULL)
    , mSelectedIndex(0)
    , mTouchX(OUT_OF_BOUNDS)
    , mTypedWordValid(FALSE)
    , mWordWidth(ArrayOf<Int32>::Alloc(MAX_SUGGESTIONS))
    , mWordX(ArrayOf<Int32>::Alloc(MAX_SUGGESTIONS))
    , mColorNormal(0)
    , mColorRecommended(0)
    , mColorOther(0)
    , mVerticalPadding(0)
    , mScrolled(FALSE)
    , mTargetScrollX(0)
    , mTotalWidth(0)
    //, mGestureDetector(NULL)
{}

CCandidateView::~CCandidateView()
{
    // if (mGestureDetector != NULL) {
    //     delete mGestureDetector;
    // }
}

ECode CCandidateView::constructor(
    /* [in] */ IContext * context)
{
    PRINT_ENTER_LEAVE("CCandidateView::constructor");
    assert(context != NULL);
    assert(0 && "TODO");
    // FAIL_RETURN(View::constructor(context));
    // AutoPtr<IResources> r;
    // FAIL_RETURN(context->GetResources((IResources**)&r));
    // r->GetDrawable(R::drawable::list_selector_background,
    //         (IDrawable**)&mSelectionHighlight);

    // AutoPtr< ArrayOf<Int32> > states = ArrayOf<Int32>::Alloc(4);
    // (*states)[0] = R::attr::state_enabled;
    // (*states)[1] = R::attr::state_focused;
    // (*states)[2] = R::attr::state_window_focused;
    // (*states)[3] = R::attr::state_pressed;
    // Boolean temp = FALSE;
    // mSelectionHighlight->SetState(states, &temp);

    // Int32 color = 0;
    // r->GetColor(0x7f050003 /*R.color.candidate_background*/, &color);
    // SetBackgroundColor(color);

    // r->GetColor(0x7f050000 /*R.color.candidate_normal*/, &mColorNormal);
    // r->GetColor(0x7f050001 /*R.color.candidate_recommended*/, &mColorRecommended);
    // r->GetColor(0x7f050002 /*R.color.candidate_other*/, &mColorOther);
    // r->GetDimensionPixelSize(0x7f060002 /*R.dimen.candidate_vertical_padding*/, &mVerticalPadding);

    // CPaint::New((IPaint**)&mPaint);
    // mPaint->SetColor(mColorNormal);
    // mPaint->SetAntiAlias(TRUE);
    // Int32 tmp = 0;
    // r->GetDimensionPixelSize(0x7f060001 /*R.dimen.candidate_font_height*/, &tmp);
    // mPaint->SetTextSize(tmp);
    // mPaint->SetStrokeWidth(0);

    // //mGestureDetector = new GestureDetector(NULL, new SelfSimpleOnGestureListener(this));
    // SetHorizontalFadingEdgeEnabled(TRUE);
    // SetWillNotDraw(FALSE);
    // SetHorizontalScrollBarEnabled(FALSE);
    // SetVerticalScrollBarEnabled(FALSE);
    return NOERROR;
}

ECode CCandidateView::SetService(
    /* [in] */ ISoftKeyboard* listener)
{
    mService = listener;
    return NOERROR;
}

ECode CCandidateView::ComputeHorizontalScrollRange(
    /* [out] */ Int32* range)
{
    assert(range != NULL);
    *range = mTotalWidth;
    return NOERROR;
}

void CCandidateView::OnMeasure(
    /* [in] */ Int32 widthMeasureSpec,
    /* [in] */ Int32 heightMeasureSpec)
{
    assert(0 && "TODO");
    // Int32 measuredWidth = ResolveSize(50, widthMeasureSpec);

    // // Get the desired height of the icon menu view (last row of items does
    // // not have a divider below)
    // AutoPtr<IRect> padding;
    // CRect::New((IRect**)&padding);
    // Boolean isPadding = FALSE;
    // mSelectionHighlight->GetPadding(padding, &isPadding);
    // Float size;
    // Int32 top = 0, bottom = 0;
    // mPaint->GetTextSize(&size);
    // padding->GetTop(&top);
    // padding->GetBottom(&bottom);
    // Int32 desiredHeight = (Int32)size + mVerticalPadding + top + bottom;

    // // Maximum possible width and desired height
    // SetMeasuredDimension(measuredWidth,
    //         ResolveSize(desiredHeight, heightMeasureSpec));
}

void CCandidateView::OnDraw(
    /* [in] */ ICanvas* canvas)
{
    assert(0 && "TODO");
    // if (canvas != NULL) {
    //     View::OnDraw(canvas);
    // }
    // mTotalWidth = 0;
    // if (mSuggestions == NULL) return;

    // if (mBgPadding == NULL) {
    //     CRect::New(0, 0, 0, 0, (IRect**)&mBgPadding);
    //     AutoPtr<IDrawable> bg = GetBackground();
    //     if (bg != NULL) {
    //         Boolean isPadding = FALSE;
    //         bg->GetPadding(mBgPadding, &isPadding);
    //     }
    // }
    // Int32 x = 0;
    // Int32 count = mSuggestions->GetLength() ;
    // Int32 height = GetHeight();
    // Int32 touchX = mTouchX;
    // Int32 scrollX = GetScrollX();
    // Boolean scrolled = mScrolled;
    // Boolean typedWordValid = mTypedWordValid;
    // Float textSize = 0, ascent = 0;
    // mPaint->GetTextSize((Float*)&textSize);
    // mPaint->Ascent((Float*)&ascent);
    // Int32 y = (Int32) (((height - textSize) / 2) - ascent);

    // Int32 top;
    // mBgPadding->GetTop(&top);
    // for (Int32 i = 0; i < count; i++) {
    //     ICharSequence* text = (*mSuggestions)[i];
    //     String suggestion;
    //     if (text != NULL) {
    //         text->ToString(&suggestion);
    //     }
    //     Float textWidth = 0;
    //     mPaint->MeasureText(suggestion, &textWidth);
    //     Int32 wordWidth = (Int32) textWidth + X_GAP * 2;

    //     (*mWordX)[i] = x;
    //     (*mWordWidth)[i] = wordWidth;
    //     mPaint->SetColor(mColorNormal);
    //     if (touchX + scrollX >= x && touchX + scrollX < x + wordWidth && !scrolled) {
    //         if (canvas != NULL) {
    //             canvas->Translate(x, 0);
    //             mSelectionHighlight->SetBounds(0, top, wordWidth, height);
    //             mSelectionHighlight->Draw(canvas);
    //             canvas->Translate(-x, 0);
    //         }
    //         mSelectedIndex = i;
    //     }

    //     if (canvas != NULL) {
    //         if ((i == 1 && !typedWordValid) || (i == 0 && typedWordValid)) {
    //             mPaint->SetFakeBoldText(TRUE);
    //             mPaint->SetColor(mColorRecommended);
    //         }
    //         else if (i != 0) {
    //             mPaint->SetColor(mColorOther);
    //         }
    //         canvas->DrawText(suggestion, x + X_GAP, y, mPaint);
    //         mPaint->SetColor(mColorOther);
    //         canvas->DrawLine(x + wordWidth + 0.5f, top,
    //                 x + wordWidth + 0.5f, height + 1, mPaint);
    //         mPaint->SetFakeBoldText(FALSE);
    //     }
    //     x += wordWidth;
    // }
    // mTotalWidth = x;
    // if (mTargetScrollX != GetScrollX()) {
    //     ScrollToTarget();
    // }
}

void CCandidateView::ScrollToTarget()
{
    assert(0 && "TODO");
    // Int32 sx = GetScrollX();
    // if (mTargetScrollX > sx) {
    //     sx += SCROLL_PIXELS;
    //     if (sx >= mTargetScrollX) {
    //         sx = mTargetScrollX;
    //         RequestLayout();
    //     }
    // }
    // else {
    //     sx -= SCROLL_PIXELS;
    //     if (sx <= mTargetScrollX) {
    //         sx = mTargetScrollX;
    //         RequestLayout();
    //     }
    // }
    // ScrollTo(sx, GetScrollY());
    // Invalidate();
}

ECode CCandidateView::SetSuggestions(
    /* [in] */ ArrayOf<ICharSequence*>* suggestions,
    /* [in] */ Boolean completions,
    /* [in] */ Boolean typedWordValid)
{
    assert(0 && "TODO");
    // Clear();
    // if (suggestions != NULL) {
    //     mSuggestions = suggestions->Clone();
    // }
    // mTypedWordValid = typedWordValid;
    // ScrollTo(0, 0);
    // mTargetScrollX = 0;
    // // Compute the total width
    // OnDraw(NULL);
    // Invalidate();
    // RequestLayout();
    return NOERROR;
}

ECode CCandidateView::Clear()
{
    mSuggestions = EMPTY_LIST;
    mTouchX = OUT_OF_BOUNDS;
    mSelectedIndex = -1;
    assert(0 && "TODO");
    // Invalidate();
    return NOERROR;
}

Boolean CCandidateView::OnTouchEvent(
    /* [in] */ IMotionEvent* event)
{
    //TODO
    // if (mGestureDetector->OnTouchEvent(event)) {
    //     return TRUE;
    // }

    Int32 action = 0, x = 0, y = 0;
    event->GetAction(&action);
    Float fx, fy;
    event->GetX(&fx);
    event->GetY(&fy);
    x = (Int32)fx;
    y = (Int32)fy;
    mTouchX = x;

    switch (action) {
    case IMotionEvent::ACTION_DOWN:
        mScrolled = FALSE;
        assert(0 && "TODO");
        // Invalidate();
        break;
    case IMotionEvent::ACTION_MOVE:
        if (y <= 0) {
            // Fling up!?
            if (mSelectedIndex >= 0) {
                mService->PickSuggestionManually(mSelectedIndex);
                mSelectedIndex = -1;
            }
        }
        assert(0 && "TODO");
        // Invalidate();
        break;
    case IMotionEvent::ACTION_UP:
        if (!mScrolled) {
            if (mSelectedIndex >= 0) {
                mService->PickSuggestionManually(mSelectedIndex);
            }
        }
        mSelectedIndex = -1;
        assert(0 && "TODO");
        // RemoveHighlight();
        // RequestLayout();
        break;
    }
    return TRUE;
}

void CCandidateView::TakeSuggestionAt(
    /* [in] */ Float x)
{
    mTouchX = (Int32)x;
    // To detect candidate
    OnDraw(NULL);
    if (mSelectedIndex >= 0) {
        mService->PickSuggestionManually(mSelectedIndex);
    }
    assert(0 && "TODO");
    // Invalidate();
}

void CCandidateView::RemoveHighlight()
{
    mTouchX = OUT_OF_BOUNDS;
    assert(0 && "TODO");
    // Invalidate();
}

} // namespace InputMethodService
} // namespace Droid
} // namespace Elastos
