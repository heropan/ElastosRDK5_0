
#include "widget/FastScroller.h"
#include <elastos/core/Math.h>
#include "os/SystemClock.h"
#include "os/Build.h"
#include "os/CHandler.h"
#include "graphics/CRectF.h"
#include "graphics/CPaint.h"
#include "view/CMotionEvent.h"
#include "view/CViewConfiguration.h"
#include "widget/AbsListView.h"
#include "widget/CExpandableListConnector.h"

using Elastos::Core::EIID_ICharSequence;
using Elastos::Core::ICharSequence;
using Elastos::Core::CStringWrapper;
using Elastos::Droid::R;
using Elastos::Droid::Os::SystemClock;
using Elastos::Droid::Os::IHandler;
using Elastos::Droid::Os::CHandler;
using Elastos::Droid::Os::Build;
using Elastos::Droid::Content::Pm::IApplicationInfo;
using Elastos::Droid::Content::Res::IResourcesTheme;
using Elastos::Droid::Content::Res::IColorStateList;
using Elastos::Droid::Graphics::Drawable::INinePatchDrawable;
using Elastos::Droid::View::CMotionEvent;
using Elastos::Droid::View::CViewConfiguration;

namespace Elastos {
namespace Droid {
namespace Widget {

//==============================================================================
//              FastScroller::DeferStartDragRunnable
//==============================================================================

FastScroller::DeferStartDragRunnable::DeferStartDragRunnable(
    /* [in] */ FastScroller* host)
    : mHost(host)
{
}

ECode FastScroller::DeferStartDragRunnable::Run()
{
    if (mHost->mList->mIsAttached) {
        mHost->BeginDrag();

        Int32 viewHeight = mHost->mList->GetHeight();
        // Jitter
        Int32 newThumbY = (Int32) mHost->mInitialTouchY - mHost->mThumbH + 10;
        if (newThumbY < 0) {
            newThumbY = 0;
        } else if (newThumbY + mHost->mThumbH > viewHeight) {
            newThumbY = viewHeight - mHost->mThumbH;
        }
        mHost->mThumbY = newThumbY;
        mHost->ScrollTo((Float) mHost->mThumbY / (viewHeight - mHost->mThumbH));
    }

    mHost->mPendingDrag = FALSE;
    return NOERROR;
}

//==============================================================================
//              FastScroller::ScrollFade
//==============================================================================

const Int32 FastScroller::ScrollFade::ALPHA_MAX;
const Int64 FastScroller::ScrollFade::FADE_DURATION;

FastScroller::ScrollFade::ScrollFade(
    /* [in] */ FastScroller* host)
    : mHost(host)
{}

void FastScroller::ScrollFade::StartFade()
{
    mFadeDuration = FADE_DURATION;
    mStartTime = SystemClock::GetUptimeMillis();
    mHost->SetState(STATE_EXIT);
}

Int32 FastScroller::ScrollFade::GetAlpha()
{
    if (mHost->GetState() != mHost->STATE_EXIT) {
        return ALPHA_MAX;
    }
    Int32 alpha;
    Int64 now = SystemClock::GetUptimeMillis();
    if (now > mStartTime + mFadeDuration) {
        alpha = 0;
    }
    else {
        alpha = (Int32)(ALPHA_MAX - ((now - mStartTime) * ALPHA_MAX) / mFadeDuration);
    }

    return alpha;
}

ECode FastScroller::ScrollFade::Run()
{
    if (mHost->GetState() != mHost->STATE_EXIT) {
        StartFade();
        return NOERROR;
    }

    if (GetAlpha() > 0) {
        mHost->mList->Invalidate();
    }
    else {
        mHost->SetState(mHost->STATE_NONE);
    }

    return NOERROR;
}

//==============================================================================
//              FastScroller
//==============================================================================

static AutoPtr<ArrayOf<Int32> > InitPRESSED_STATES()
{
    AutoPtr<ArrayOf<Int32> > array = ArrayOf<Int32>::Alloc(1);
    (*array)[0] = R::attr::state_pressed;
    return array;
}

static AutoPtr<ArrayOf<Int32> > InitATTRS()
{
    AutoPtr<ArrayOf<Int32> > array = ArrayOf<Int32>::Alloc(6);
    (*array)[0] = R::attr::fastScrollTextColor;
    (*array)[1] = R::attr::fastScrollThumbDrawable;
    (*array)[2] = R::attr::fastScrollTrackDrawable;
    (*array)[3] = R::attr::fastScrollPreviewBackgroundLeft;
    (*array)[4] = R::attr::fastScrollPreviewBackgroundRight;
    (*array)[5] = R::attr::fastScrollOverlayPosition;
    return array;
}

const Int32 FastScroller::MIN_PAGES;
const Int32 FastScroller::STATE_NONE;
const Int32 FastScroller::STATE_ENTER;
const Int32 FastScroller::STATE_VISIBLE;
const Int32 FastScroller::STATE_DRAGGING;
const Int32 FastScroller::STATE_EXIT;

AutoPtr<ArrayOf<Int32> > FastScroller::PRESSED_STATES = InitPRESSED_STATES();
AutoPtr<ArrayOf<Int32> > FastScroller::DEFAULT_STATES = ArrayOf<Int32>::Alloc(0);
AutoPtr<ArrayOf<Int32> > FastScroller::ATTRS = InitATTRS();

const Int32 FastScroller::TEXT_COLOR;
const Int32 FastScroller::THUMB_DRAWABLE;
const Int32 FastScroller::TRACK_DRAWABLE;
const Int32 FastScroller::PREVIEW_BACKGROUND_LEFT;
const Int32 FastScroller::PREVIEW_BACKGROUND_RIGHT;
const Int32 FastScroller::OVERLAY_POSITION;
const Int32 FastScroller::OVERLAY_FLOATING;
const Int32 FastScroller::OVERLAY_AT_THUMB;
const Int32 FastScroller::FADE_TIMEOUT;
const Int32 FastScroller::PENDING_DRAG_DELAY;

FastScroller::FastScroller(
    /* [in] */ IContext* context,
    /* [in] */ AbsListView* listView)
    : mList(listView)
    , mThumbH(0)
    , mThumbW(0)
    , mThumbY(0)
    , mOverlaySize(0)
    , mScrollCompleted(FALSE)
    , mVisibleItem(0)
    , mListOffset(0)
    , mItemCount(-1)
    , mLongList(FALSE)
    , mDrawOverlay(FALSE)
    , mState(0)
    , mChangedBounds(FALSE)
    , mPosition(0)
    , mAlwaysShow(FALSE)
    , mOverlayPosition(0)
    , mMatchDragPosition(FALSE)
    , mInitialTouchY(0)
    , mPendingDrag(FALSE)
    , mScaledTouchSlop(0)
{
    Init(context);
}

void FastScroller::SetAlwaysShow(
    /* [in] */ Boolean alwaysShow)
{
    mAlwaysShow = alwaysShow;
    if (alwaysShow) {
        RemoveCallbacks();
        SetState(STATE_VISIBLE);
    } else if (mState == STATE_VISIBLE) {
        PostDelayed(FADE_TIMEOUT);
    }
}

Boolean FastScroller::IsAlwaysShowEnabled()
{
    return mAlwaysShow;
}

void FastScroller::RefreshDrawableState()
{
    AutoPtr<ArrayOf<Int32> > state = mState == STATE_DRAGGING ? PRESSED_STATES : DEFAULT_STATES;

    Boolean isStateFul;
    if (mThumbDrawable != NULL && (mThumbDrawable->IsStateful(&isStateFul), isStateFul)) {
        mThumbDrawable->SetState(state, &isStateFul);
    }
    if (mTrackDrawable != NULL && (mTrackDrawable->IsStateful(&isStateFul), isStateFul)) {
        mTrackDrawable->SetState(state, &isStateFul);
    }
}

void FastScroller::SetScrollbarPosition(
    /* [in] */ Int32 position)
{
    if (position == IView::SCROLLBAR_POSITION_DEFAULT) {
        Boolean isLayoutRtl = mList->IsLayoutRtl();
        position = isLayoutRtl ?
            IView::SCROLLBAR_POSITION_LEFT : IView::SCROLLBAR_POSITION_RIGHT;
    }
    mPosition = position;
    switch (position) {
        default:
        case IView::SCROLLBAR_POSITION_RIGHT:
            mOverlayDrawable = mOverlayDrawableRight;
            break;
        case IView::SCROLLBAR_POSITION_LEFT:
            mOverlayDrawable = mOverlayDrawableLeft;
            break;
    }
}

Int32 FastScroller::GetWidth()
{
    return mThumbW;
}

void FastScroller::SetState(
    /* [in] */ Int32 state)
{
    switch (state) {
    case STATE_NONE:
        RemoveCallbacks();
        mList->Invalidate();
        break;
    case STATE_VISIBLE:
        if (mState != STATE_VISIBLE) { // Optimization
            ResetThumbPos();
        }
        // Fall through
    case STATE_DRAGGING:
        RemoveCallbacks();
        break;
    case STATE_EXIT:
        {
            Int32 viewWidth = mList->GetWidth();
            mList->Invalidate(
                viewWidth - mThumbW, mThumbY, viewWidth, mThumbY + mThumbH);
        }
        break;
    default:
        break;
    }
    mState = state;

    RefreshDrawableState();
}

Int32 FastScroller::GetState()
{
    return mState;
}

void FastScroller::ResetThumbPos()
{
    Int32 viewWidth = mList->GetWidth();
    // Bounds are always top right. Y coordinate get's translated during draw
    switch (mPosition) {
            case IView::SCROLLBAR_POSITION_RIGHT:
                mThumbDrawable->SetBounds(viewWidth - mThumbW, 0, viewWidth, mThumbH);
                break;
            case IView::SCROLLBAR_POSITION_LEFT:
                mThumbDrawable->SetBounds(0, 0, mThumbW, mThumbH);
                break;
    }

    mThumbDrawable->SetAlpha(ScrollFade::ALPHA_MAX);
}

void FastScroller::UseThumbDrawable(
    /* [in] */ IContext* context,
    /* [in] */ IDrawable* drawable)
{
    mThumbDrawable = drawable;
    if (INinePatchDrawable::Probe(drawable)) {
        AutoPtr<IResources> resources;
        context->GetResources((IResources**)&resources);
        resources->GetDimensionPixelSize(
            R::dimen::fastscroll_thumb_width, &mThumbW);
        resources->GetDimensionPixelSize(
            R::dimen::fastscroll_thumb_height, &mThumbH);
    }
    else
    {
        drawable->GetIntrinsicWidth(&mThumbW);
        drawable->GetIntrinsicHeight(&mThumbH);
    }
    mChangedBounds = TRUE;
}

void FastScroller::Init(
    /* [in] */ IContext* context)
{
    CHandler::New((IHandler**)&mHandler);

    // Get both the scrollbar states drawables
    AutoPtr<IResourcesTheme> theme;
    context->GetTheme((IResourcesTheme**)&theme);
    AutoPtr<ITypedArray> ta;
    theme->ObtainStyledAttributes(ATTRS, (ITypedArray**)&ta);
    AutoPtr<IDrawable> thumb;
    ta->GetDrawable(THUMB_DRAWABLE, (IDrawable**)&thumb);
    UseThumbDrawable(context, thumb);
    ta->GetDrawable(TRACK_DRAWABLE, (IDrawable**)&mTrackDrawable);

    ta->GetDrawable(PREVIEW_BACKGROUND_LEFT, (IDrawable**)&mOverlayDrawableLeft);
    ta->GetDrawable(PREVIEW_BACKGROUND_RIGHT, (IDrawable**)&mOverlayDrawableRight);
    ta->GetInt32(OVERLAY_POSITION, OVERLAY_FLOATING, &mOverlayPosition);

    mScrollCompleted = TRUE;

    GetSectionsFromIndexer();

    AutoPtr<IResources> res;
    context->GetResources((IResources**)&res);
    res->GetDimensionPixelSize(R::dimen::fastscroll_overlay_size, &mOverlaySize);

    ASSERT_SUCCEEDED(CRectF::New((IRectF**)&mOverlayPos));
    mScrollFade = new ScrollFade(this);
    mDeferStartDrag = new DeferStartDragRunnable(this);
    ASSERT_SUCCEEDED(CPaint::New((IPaint**)&mPaint));
    mPaint->SetAntiAlias(TRUE);
    mPaint->SetTextAlign(PaintAlign_CENTER);
    mPaint->SetTextSize(mOverlaySize / 2);

    AutoPtr<IColorStateList> textColor;
    ta->GetColorStateList(TEXT_COLOR, (IColorStateList**)&textColor);

    Int32 textColorNormal;
    textColor->GetDefaultColor(&textColorNormal);
    mPaint->SetColor(textColorNormal);
    mPaint->SetStyle(PaintStyle_FILL_AND_STROKE);

    // to show mOverlayDrawable properly
    if (mList->GetWidth() > 0 && mList->GetHeight() > 0) {
        OnSizeChanged(mList->GetWidth(), mList->GetHeight(), 0, 0);
    }

    mState = STATE_NONE;
    RefreshDrawableState();

    ta->Recycle();

    AutoPtr<CViewConfiguration> vc = CViewConfiguration::Get(context);
    vc->GetScaledTouchSlop(&mScaledTouchSlop);

    AutoPtr<IApplicationInfo> appInfo;
    context->GetApplicationInfo((IApplicationInfo**)&appInfo);
    Int32 version;
    appInfo->GetTargetSdkVersion(&version);
    mMatchDragPosition = version >= Build::VERSION_CODES::HONEYCOMB;

    SetScrollbarPosition(mList->GetVerticalScrollbarPosition());
}

void FastScroller::Stop()
{
    SetState(STATE_NONE);
}

Boolean FastScroller::IsVisible()
{
    return !(mState == STATE_NONE);
}

void FastScroller::Draw(
    /* [in] */ ICanvas* canvas)
{
    if (mState == STATE_NONE) {
        // No need to draw anything
        return;
    }

    Int32 y = mThumbY;
    Int32 viewWidth = mList->GetWidth();
    Boolean temp;
    Int32 alpha = -1;
    if (mState == STATE_EXIT) {
        alpha = mScrollFade->GetAlpha();
        if (alpha < ScrollFade::ALPHA_MAX / 2) {
            mThumbDrawable->SetAlpha(alpha * 2);
        }

        Int32 left = 0;
        switch (mPosition) {
            case IView::SCROLLBAR_POSITION_RIGHT:
                left = viewWidth - (mThumbW * alpha) / ScrollFade::ALPHA_MAX;
                break;
            case IView::SCROLLBAR_POSITION_LEFT:
                left = -mThumbW + (mThumbW * alpha) / ScrollFade::ALPHA_MAX;
                break;
        }
        mThumbDrawable->SetBounds(left, 0, left + mThumbW, mThumbH);

        mChangedBounds = TRUE;
    }

    if (mTrackDrawable != NULL) {
        AutoPtr<IRect> thumbBounds;
        mThumbDrawable->GetBounds((IRect**)&thumbBounds);
        Int32 l, t, r, b;
        thumbBounds->Get(&l, &t, &r, &b);
        Int32 left = l;
        Int32 halfThumbHeight = (b - t) / 2;
        Int32 trackWidth;
        mTrackDrawable->GetIntrinsicWidth(&trackWidth);
        Int32 trackLeft = (left + mThumbW / 2) - trackWidth / 2;
        mTrackDrawable->SetBounds(trackLeft, halfThumbHeight,
            trackLeft + trackWidth, mList->GetHeight() - halfThumbHeight);
        mTrackDrawable->Draw(canvas);
    }

    canvas->Translate(0, y);
    mThumbDrawable->Draw(canvas);
    canvas->Translate(0, -y);

    // If user is dragging the scroll bar, draw the alphabet overlay
    if (mState == STATE_DRAGGING && mDrawOverlay) {
        if (mOverlayPosition == OVERLAY_AT_THUMB) {
            AutoPtr<IRect> thumbBounds;
            mThumbDrawable->GetBounds((IRect**)&thumbBounds);
            Int32 l, t, r, b;
            thumbBounds->Get(&l, &t, &r, &b);

            Int32 left = 0;
            switch (mPosition) {
                default:
                case IView::SCROLLBAR_POSITION_RIGHT:
                    left = Elastos::Core::Math::Max(0, l - mThumbW - mOverlaySize);
                    break;
                case IView::SCROLLBAR_POSITION_LEFT:
                    left = Elastos::Core::Math::Min(r + mThumbW, mList->GetWidth() - mOverlaySize);
                    break;
            }

            Int32 top = Elastos::Core::Math::Max(0,
                Elastos::Core::Math::Min(y + (mThumbH - mOverlaySize) / 2, mList->GetHeight() - mOverlaySize));

            CRectF* pos = (CRectF*)mOverlayPos.Get();
            pos->mLeft = left;
            pos->mRight = left + mOverlaySize;
            pos->mTop = top;
            pos->mBottom = top + mOverlaySize;
            if (mOverlayDrawable != NULL) {
                mOverlayDrawable->SetBounds(left, top, left + mOverlaySize, top + mOverlaySize);
            }
        }

        mOverlayDrawable->Draw(canvas);
        AutoPtr<IPaint> paint = mPaint;
        Float descent;
        paint->Descent(&descent);

        Float lf, tf, rf, bf;
        mOverlayPos->Get(&lf, &tf, &rf, &bf);

        mOverlayDrawable->GetPadding(mTmpRect, &temp);
        Int32 l, t, r, b;
        mTmpRect->Get(&l, &t, &r, &b);

        Int32 hOff = (r - l) / 2;
        Int32 vOff = (b - t) / 2;
        canvas->DrawText(mSectionText, (Int32) (lf + rf) / 2 - hOff,
            (Int32) (bf + tf) / 2 + mOverlaySize / 4 - descent - vOff, paint);
    } else if (mState == STATE_EXIT) {
        if (alpha == 0) { // Done with exit
            SetState(STATE_NONE);
        } else if (mTrackDrawable != NULL) {
            mList->Invalidate(viewWidth - mThumbW, 0, viewWidth, mList->GetHeight());
        } else {
            mList->Invalidate(viewWidth - mThumbW, y, viewWidth, y + mThumbH);
        }
    }
}

void FastScroller::OnSizeChanged(
    /* [in] */ Int32 w,
    /* [in] */ Int32 h,
    /* [in] */ Int32 oldw,
    /* [in] */ Int32 oldh)
{
    if (mThumbDrawable != NULL) {
        switch (mPosition) {
            default:
            case IView::SCROLLBAR_POSITION_RIGHT:
                mThumbDrawable->SetBounds(w - mThumbW, 0, w, mThumbH);
                break;
            case IView::SCROLLBAR_POSITION_LEFT:
                mThumbDrawable->SetBounds(0, 0, mThumbW, mThumbH);
                break;
        }
    }

    if (mOverlayPosition == OVERLAY_FLOATING) {
        Float left = (w - mOverlaySize) / 2;
        Float top = h / 10; // 10% from top
        CRectF* pos = (CRectF*)mOverlayPos.Get();
        pos->mLeft = left;
        pos->mRight = left + mOverlaySize;
        pos->mTop = top;
        pos->mBottom = top + mOverlaySize;
        if (mOverlayDrawable != NULL) {
            mOverlayDrawable->SetBounds(left, top, left + mOverlaySize, top + mOverlaySize);
        }
    }
}

void FastScroller::OnItemCountChanged(
    /* [in] */ Int32 oldCount,
    /* [in] */ Int32 newCount)
{
    if (mAlwaysShow) {
        mLongList = TRUE;
    }
}

void FastScroller::OnScroll(
    /* [in] */ IAbsListView* view,
    /* [in] */ Int32 firstVisibleItem,
    /* [in] */ Int32 visibleItemCount,
    /* [in] */ Int32 totalItemCount)
{
    // Are there enough pages to require fast scroll? Recompute only if total count changes
    if (mItemCount != totalItemCount && visibleItemCount > 0) {
        mItemCount = totalItemCount;
        mLongList = mItemCount / visibleItemCount >= MIN_PAGES;
    }

    if (mAlwaysShow) {
        mLongList = TRUE;
    }

    if (!mLongList) {
        if (mState != STATE_NONE) {
            SetState(STATE_NONE);
        }
        return;
    }

    if (totalItemCount - visibleItemCount > 0 && mState != STATE_DRAGGING ) {
        mThumbY = GetThumbPositionForListPosition(firstVisibleItem, visibleItemCount,
            totalItemCount);

        if (mChangedBounds) {
            ResetThumbPos();
            mChangedBounds = FALSE;
        }
    }
    mScrollCompleted = TRUE;
    if (firstVisibleItem == mVisibleItem) {
        return;
    }
    mVisibleItem = firstVisibleItem;
    if (mState != STATE_DRAGGING) {
        SetState(STATE_VISIBLE);

        if (!mAlwaysShow) {
            PostDelayed(FADE_TIMEOUT);
        }
    }
}

AutoPtr<ISectionIndexer> FastScroller::GetSectionIndexer()
{
    return mSectionIndexer;
}

AutoPtr<ArrayOf<IInterface*> > FastScroller::GetSections()
{
    if (mListAdapter == NULL && mList != NULL) {
        GetSectionsFromIndexer();
    }

    return mSections;
}

void FastScroller::GetSectionsFromIndexer()
{
    AutoPtr<IAdapter> adapter;
    ((IAdapterView*)(mList->Probe(EIID_IAdapterView)))->GetAdapter((IAdapter**)&adapter);
    mSectionIndexer = NULL;
    IHeaderViewListAdapter* headerAdapter = IHeaderViewListAdapter::Probe(adapter);
    if (headerAdapter) {
        headerAdapter->GetHeadersCount(&mListOffset);
        adapter = NULL;
        IWrapperListAdapter::Probe(headerAdapter)->GetWrappedAdapter((IListAdapter**)&adapter);
    }

    IExpandableListConnector* connector = IExpandableListConnector::Probe(adapter);
    if (connector) {
        AutoPtr<IExpandableListAdapter> expAdapter = ((CExpandableListConnector*)connector)->GetAdapter();
        if (ISectionIndexer::Probe(expAdapter)) {
            mSectionIndexer = ISectionIndexer::Probe(expAdapter);
            mListAdapter = (IBaseAdapter*)adapter->Probe(EIID_IBaseAdapter);
            mSections = NULL;
            mSectionIndexer->GetSections((ArrayOf<IInterface*>**)&mSections);
        }
    }
    else {
        mListAdapter = IBaseAdapter::Probe(adapter);
        mSections = NULL;

        ISectionIndexer* sectionIndexer = ISectionIndexer::Probe(adapter);
        if (sectionIndexer) {
            mSectionIndexer = sectionIndexer;
            mSectionIndexer->GetSections((ArrayOf<IInterface*>**)&mSections);
        }

        if (mSections == NULL) {
            mSections = ArrayOf<IInterface*>::Alloc(1);
            AutoPtr<ICharSequence> seq;
            CStringWrapper::New(String(" "), (ICharSequence**)&seq);
            mSections->Set(0, seq);
        }
    }
}

void FastScroller::OnSectionsChanged()
{
    mListAdapter = NULL;
}

void FastScroller::ScrollTo(
    /* [in] */ Float position)
{
    Int32 count = mList->GetCount();
    mScrollCompleted = FALSE;
    Float fThreshold = (1.0f / count) / 8;
    Int32 sectionIndex;
    if (mSections->GetLength() > 1) {
        Int32 nSections = mSections->GetLength();
        Int32 section = (Int32)(position * nSections);
        if (section >= nSections) {
            section = nSections - 1;
        }

        Int32 exactSection = section;
        sectionIndex = section;
        Int32 index;
        mSectionIndexer->GetPositionForSection(section, &index);

        // Given the expected section and index, the following code will
        // try to account for missing sections (no names starting with..)
        // It will compute the scroll space of surrounding empty sections
        // and interpolate the currently visible letter's range across the
        // available space, so that there is always some list movement while
        // the user moves the thumb.
        //
        Int32 nextIndex = count;
        Int32 prevIndex = index;
        Int32 prevSection = section;
        Int32 nextSection = section + 1;
        // Assume the next section is unique
        if (section < nSections - 1) {
            mSectionIndexer->GetPositionForSection(section + 1, &nextIndex);
        }

        // Find the previous index if we're slicing the previous section
        if (nextIndex == index) {
            // Non-existent letter
            while (section > 0) {
                section--;
                mSectionIndexer->GetPositionForSection(section, &prevIndex);
                if (prevIndex != index) {
                    prevSection = section;
                    sectionIndex = section;
                    break;
                }
                else if (section == 0) {
                    // When section reaches 0 here, sectionIndex must follow it.
                    // Assuming mSectionIndexer.getPositionForSection(0) == 0.
                    sectionIndex = 0;
                    break;
                }
            }
        }

        // Find the next index, in case the assumed next index is not
        // unique. For instance, if there is no P, then request for P's
        // position actually returns Q's. So we need to look ahead to make
        // sure that there is really a Q at Q's position. If not, move
        // further down...
        Int32 nextNextSection = nextSection + 1;
        Int32 nextNextPosition;
        mSectionIndexer->GetPositionForSection(nextNextSection, &nextNextPosition);
        while (nextNextSection < nSections &&
               nextNextPosition == nextIndex) {
            nextNextSection++;
            nextSection++;
            mSectionIndexer->GetPositionForSection(nextNextSection, &nextNextPosition);
        }
        // Compute the beginning and ending scroll range percentage of the
        // currently visible letter. This could be equal to or greater than
        // (1 / nSections).
        Float fPrev = (Float)prevSection / nSections;
        Float fNext = (Float)nextSection / nSections;
        if (prevSection == exactSection && position - fPrev < fThreshold) {
            index = prevIndex;
        }
        else {
            index = prevIndex + (Int32)((nextIndex - prevIndex) * (position - fPrev)
                / (fNext - fPrev));
        }
        // Don't overflow
        if (index > count - 1) {
            index = count - 1;
        }

        if (mList->Probe(EIID_IExpandableListView)) {
            IExpandableListView* expList = (IExpandableListView*)(mList->Probe(EIID_IExpandableListView));
            Int32 pos = ExpandableListView::GetPackedPositionForGroup(index + mListOffset);
            Int32 flastListPos;
            expList->GetFlatListPosition(pos, &flastListPos);
            expList->SetSelectionFromTop(flastListPos, 0);
        }
        else if (mList->Probe(EIID_IListView)) {
            IListView* list = (IListView*)mList->Probe(EIID_IListView);
            list->SetSelectionFromTop(index + mListOffset, 0);
        }
        else {
            mList->SetSelection(index + mListOffset);
        }
    }
    else {
        Int32 index = (Int32)(position * count);
        // Don't overflow
        if (index > count - 1) index = count - 1;

        if (mList->Probe(EIID_IExpandableListView)) {
            IExpandableListView* expList = (IExpandableListView*)(mList->Probe(EIID_IExpandableListView));
            Int32 pos = ExpandableListView::GetPackedPositionForGroup(index + mListOffset);
            Int32 flastListPos;
            expList->GetFlatListPosition(pos, &flastListPos);
            expList->SetSelectionFromTop(flastListPos, 0);
        }
        else if (mList->Probe(EIID_IListView)) {
            IListView* list = (IListView*)mList->Probe(EIID_IListView);
            list->SetSelectionFromTop(index + mListOffset, 0);
        }
        else {
            mList->SetSelection(index + mListOffset);
        }

        sectionIndex = -1;
    }

    if (sectionIndex >= 0) {
        AutoPtr<ICharSequence> seq = (ICharSequence*)(*mSections)[sectionIndex]->Probe(EIID_ICharSequence);
        assert(seq != NULL);
        seq->ToString(&mSectionText);
        String text = mSectionText ;
        mDrawOverlay = (text.GetLength() != 1 || text.GetChar(0) != ' ') &&
                sectionIndex < mSections->GetLength();
    }
    else {
        mDrawOverlay = FALSE;
    }
}

Int32 FastScroller::GetThumbPositionForListPosition(
    /* [in] */ Int32 firstVisibleItem,
    /* [in] */ Int32 visibleItemCount,
    /* [in] */ Int32 totalItemCount)
{
    if (mSectionIndexer == NULL || mListAdapter == NULL) {
        GetSectionsFromIndexer();
    }
    if (mSectionIndexer == NULL || !mMatchDragPosition) {
        return ((mList->GetHeight() - mThumbH) * firstVisibleItem)
            / (totalItemCount - visibleItemCount);
    }

    firstVisibleItem -= mListOffset;
    if (firstVisibleItem < 0) {
        return 0;
    }
    totalItemCount -= mListOffset;

    Int32 trackHeight = mList->GetHeight() - mThumbH;

    Int32 section, sectionPos, nextSectionPos;
    mSectionIndexer->GetSectionForPosition(firstVisibleItem, &section);
    mSectionIndexer->GetPositionForSection(section, &sectionPos);
    mSectionIndexer->GetPositionForSection(section + 1, &nextSectionPos);
    Int32 sectionCount = mSections->GetLength();
    Int32 positionsInSection = nextSectionPos - sectionPos;

    AutoPtr<IView> child = mList->GetChildAt(0);
    Int32 top, height;
    child->GetTop(&top);
    child->GetHeight(&height);
    Float incrementalPos = child == NULL ? 0 : firstVisibleItem +
        (Float) (mList->GetPaddingTop() - top) / height;
    Float posWithinSection = (incrementalPos - sectionPos) / positionsInSection;
    Int32 result = (Int32) ((section + posWithinSection) / sectionCount * trackHeight);

    // Fake out the scrollbar for the last item. Since the section indexer won't
    // ever actually move the list in this end space, make scrolling across the last item
    // account for whatever space is remaining.
    if (firstVisibleItem > 0 && firstVisibleItem + visibleItemCount == totalItemCount) {
        AutoPtr<IView> lastChild = mList->GetChildAt(visibleItemCount - 1);
        lastChild->GetTop(&top);
        lastChild->GetHeight(&height);
        Float lastItemVisible = (Float) (mList->GetHeight() - mList->GetPaddingBottom()
            - top) / height;
        result += (trackHeight - result) * lastItemVisible;
    }

    return result;
}

void FastScroller::CancelFling()
{
    // Cancel the list fling
    AutoPtr<CMotionEvent> cancelFling;
    CMotionEvent::Obtain(
        0, 0, IMotionEvent::ACTION_CANCEL, 0, 0, 0,
        (CMotionEvent**)&cancelFling);
    mList->OnTouchEvent(cancelFling);
    cancelFling->Recycle();
}

void FastScroller::CancelPendingDrag()
{
    mList->RemoveCallbacks(mDeferStartDrag);
    mPendingDrag = FALSE;
}

void FastScroller::StartPendingDrag()
{
    mPendingDrag = TRUE;
    mList->PostDelayed(mDeferStartDrag, PENDING_DRAG_DELAY);
}

void FastScroller::BeginDrag()
{
    SetState(STATE_DRAGGING);
    if (mListAdapter == NULL && mList != NULL) {
        GetSectionsFromIndexer();
    }
    if (mList != NULL) {
        mList->RequestDisallowInterceptTouchEvent(TRUE);
        mList->ReportScrollStateChange(IAbsListViewOnScrollListener::SCROLL_STATE_TOUCH_SCROLL);
    }

    CancelFling();
}

Boolean FastScroller::OnInterceptTouchEvent(
    /* [in] */ IMotionEvent* ev)
{
    Int32 mask;
    ev->GetActionMasked(&mask);
    Float x, y;
    ev->GetX(&x);
    ev->GetY(&y);
    switch (mask) {
        case IMotionEvent::ACTION_DOWN:
            if (mState > STATE_NONE && IsPointInside(x, y)) {
                if (!mList->IsInScrollingContainer()) {
                    BeginDrag();
                    return TRUE;
                }
                mInitialTouchY = y;
                StartPendingDrag();
            }
            break;
        case IMotionEvent::ACTION_UP:
        case IMotionEvent::ACTION_CANCEL:
            CancelPendingDrag();
            break;
    }
    return FALSE;
}

Boolean FastScroller::OnTouchEvent(
    /* [in] */ IMotionEvent* me)
{
    if (mState == STATE_NONE) {
        return FALSE;
    }

    Int32 action;
    me->GetAction(&action);
    Float fx, fy;
    me->GetX(&fx);
    me->GetY(&fy);

    if (action == IMotionEvent::ACTION_DOWN) {
        if (IsPointInside(fx, fy)) {
            SetState(STATE_DRAGGING);
            if (!mList->IsInScrollingContainer()) {
                BeginDrag();
                return TRUE;
            }
            mInitialTouchY = fy;
            StartPendingDrag();
        }
    }
    else if (action == IMotionEvent::ACTION_UP) { // don't add ACTION_CANCEL here
        if (mPendingDrag) {
            // Allow a tap to scroll.
            BeginDrag();

            Int32 viewHeight = mList->GetHeight();
            // Jitter
            Int32 newThumbY = (Int32) fy - mThumbH + 10;
            if (newThumbY < 0) {
                newThumbY = 0;
            } else if (newThumbY + mThumbH > viewHeight) {
                newThumbY = viewHeight - mThumbH;
            }
            mThumbY = newThumbY;
            ScrollTo((Float) mThumbY / (viewHeight - mThumbH));

            CancelPendingDrag();
            // Will hit the STATE_DRAGGING check below
        }
        if (mState == STATE_DRAGGING) {
            if (mList != NULL) {
                // ViewGroup does the right thing already, but there might
                // be other classes that don't properly reset on touch-up,
                // so do this explicitly just in case.
                mList->RequestDisallowInterceptTouchEvent(FALSE);
                mList->ReportScrollStateChange(IAbsListViewOnScrollListener::SCROLL_STATE_IDLE);
            }

            SetState(STATE_VISIBLE);

            RemoveCallbacks();
            if (!mAlwaysShow) {
                PostDelayed(1000);
            }
            return TRUE;
        }
    }
    else if (action == IMotionEvent::ACTION_MOVE) {
        if (mPendingDrag) {
            if (Elastos::Core::Math::Abs(fy - mInitialTouchY) > mScaledTouchSlop) {
                SetState(STATE_DRAGGING);
                if (mListAdapter == NULL && mList != NULL) {
                    GetSectionsFromIndexer();
                }
                if (mList != NULL) {
                    mList->RequestDisallowInterceptTouchEvent(TRUE);
                    mList->ReportScrollStateChange(IAbsListViewOnScrollListener::SCROLL_STATE_TOUCH_SCROLL);
                }

                CancelFling();
                CancelPendingDrag();
                // Will hit the STATE_DRAGGING check below
            }
        }
        if (mState == STATE_DRAGGING) {
            Int32 viewHeight = mList->GetHeight();
            // Jitter
            Int32 newThumbY = (Int32)fy - mThumbH + 10;
            if (newThumbY < 0) {
                newThumbY = 0;
            }
            else if (newThumbY + mThumbH > viewHeight) {
                newThumbY = viewHeight - mThumbH;
            }

            if (Elastos::Core::Math::Abs(mThumbY - newThumbY) < 2) {
                return TRUE;
            }
            mThumbY = newThumbY;
            // If the previous scrollTo is still pending
            if (mScrollCompleted) {
                ScrollTo((Float)mThumbY / (viewHeight - mThumbH));
            }
            return TRUE;
        }
    }
    else if (action == IMotionEvent::ACTION_CANCEL) {
        CancelPendingDrag();
    }
    return FALSE;
}

Boolean FastScroller::IsPointInside(
    /* [in] */ Float x,
    /* [in] */ Float y)
{
    Boolean inTrack = FALSE;
    switch (mPosition) {
        default:
        case IView::SCROLLBAR_POSITION_RIGHT:
            inTrack = x > mList->GetWidth() - mThumbW;
            break;
        case IView::SCROLLBAR_POSITION_LEFT:
            inTrack = x < mThumbW;
            break;
    }

    // Allow taps in the track to start moving.
    return inTrack && ((mTrackDrawable != NULL) || (y >= mThumbY && y <= mThumbY + mThumbH));
}

ECode FastScroller::RemoveCallbacks()
{
    return mHandler->RemoveCallbacks(mScrollFade);
}

Boolean FastScroller::PostDelayed(
    /* [in] */ Int64 delayMillis)
{
    if (delayMillis < 0 ) {
        delayMillis = 0;
    }

    Boolean result;
    return mHandler->PostDelayed(mScrollFade, delayMillis, &result);
}

}// namespace Widget
}// namespace Droid
}// namespace Elastos
