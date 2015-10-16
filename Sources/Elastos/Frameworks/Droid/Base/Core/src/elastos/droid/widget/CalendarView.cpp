#include "CalendarView.h"
#include "elastos/droid/utility/CTypedValueHelper.h"
#include "elastos/droid/text/format/DateUtils.h"
#include "elastos/droid/widget/CAbsListViewLayoutParams.h"
#include "elastos/droid/view/CGestureDetector.h"
#include "elastos/droid/R.h"
#include "Elastos.Core.h"
#include "elastos/droid/graphics/CPaint.h"
#include <elastos/core/StringUtils.h>
#include "elastos/droid/widget/CCalendarView.h"

using Elastos::Core::ISystem;
using Elastos::Core::CSystem;
using Elastos::Core::StringUtils;
using Elastos::Core::ICloneable;
using Elastos::Droid::Utility::IDisplayMetrics;
using Elastos::Droid::Utility::ITypedValue;
using Elastos::Droid::Utility::CTypedValueHelper;
using Elastos::Droid::Utility::ITypedValueHelper;
using Elastos::Droid::View::ILayoutInflater;
using Elastos::Droid::View::IOnGestureListener;
using Elastos::Droid::View::CGestureDetector;
using Elastos::Droid::Text::Format::DateUtils;
using Elastos::Droid::Text::Format::IDateUtils;
using Elastos::Droid::Graphics::PaintStyle;
using Elastos::Droid::Graphics::PaintAlign;
using Elastos::Text::CSimpleDateFormat;
using Elastos::Text::ISimpleDateFormat;
using Elastos::Droid::Graphics::CPaint;
using Elastos::Droid::R;
using Elastos::Droid::View::EIID_View;
using Elastos::Droid::Widget::CCalendarView;

namespace Elastos {
namespace Droid {
namespace Widget {

const String CalendarView::TAG("CalendarView");
const String CalendarView::DATE_FORMAT = String("MM/dd/yyyy");
const String CalendarView::DEFAULT_MIN_DATE = String("01/01/1900");
const String CalendarView::DEFAULT_MAX_DATE = String("01/01/2100");

CAR_INTERFACE_IMPL(CalendarView::SetUpAdapterDataSetObserver, IDataSetObserver)
CAR_INTERFACE_IMPL(CalendarView::SetUpListViewAbsListViewOnScrollListener, IAbsListViewOnScrollListener)

/*............................ScrollStateRunnable Start............................*/

CalendarView::ScrollStateRunnable::ScrollStateRunnable(
    /* [in] */ ICalendarView* host)
    : mView(NULL)
{
    AutoPtr<IWeakReferenceSource> wrs = (IWeakReferenceSource*)host->Probe(EIID_IWeakReferenceSource);
    assert(wrs.Get() != NULL);
    wrs->GetWeakReference((IWeakReference**)&mWeakHost);
}

ECode CalendarView::ScrollStateRunnable::DoScrollStateChange(
    /* [in] */ IAbsListView* view,
    /* [in] */ Int32 scrollState)
{
    AutoPtr<ICalendarView> alv;
    mWeakHost->Resolve(EIID_ICalendarView, (IInterface**)&alv);
    if (alv.Get() == NULL) {
        return NOERROR;
    }

    CalendarView* cv = (CalendarView*)(CCalendarView*)alv.Get();

    mView = view;
    mNewState = scrollState;
    cv->RemoveCallbacks(this);
    cv->PostDelayed(this, CalendarView::SCROLL_CHANGE_DELAY);

    return NOERROR;
}

ECode CalendarView::ScrollStateRunnable::Run()
{
    AutoPtr<ICalendarView> alv;
    mWeakHost->Resolve(EIID_ICalendarView, (IInterface**)&alv);
    if (alv.Get() == NULL) {
        return NOERROR;
    }

    CalendarView* cv = (CalendarView*)(CCalendarView*)alv.Get();
    cv->mCurrentScrollState = mNewState;
    // Fix the position after a scroll or a fling ends
    if (mNewState == IAbsListViewOnScrollListener::SCROLL_STATE_IDLE
            && cv->mPreviousScrollState != IAbsListViewOnScrollListener::SCROLL_STATE_IDLE) {
        AutoPtr<IView> child;
        mView->GetChildAt(0, (IView**)&child);
        if (child == NULL) {
            // The view is no longer visible, just return
            return NOERROR;
        }
        Int32 bottom;
        child->GetBottom(&bottom);
        Int32 dist = bottom - cv->mListScrollTopOffset;
        if (dist > cv->mListScrollTopOffset) {
            if (cv->mIsScrollingUp) {
                Int32 height;
                child->GetHeight(&height);
                mView->SmoothScrollBy(dist - height, CalendarView::ADJUSTMENT_SCROLL_DURATION);
            } else {
                mView->SmoothScrollBy(dist, CalendarView::ADJUSTMENT_SCROLL_DURATION);
            }
        }
    }
    cv->mPreviousScrollState = mNewState;
    return NOERROR;
}
/*............................ScrollStateRunnable End..............................*/


/*............................_WeeksAdapter Start..............................*/

CalendarView::_WeeksAdapter::_WeeksAdapter(
    /* [in] */ IContext* context,
    /* [in] */ CalendarView* host)
   : mHost(host)
   , mSelectedWeek(0)
   , mFocusedMonth(0)
   , mTotalWeekCount(0)
{
    AutoPtr<ICalendarHelper> helper;
    CCalendarHelper::AcquireSingleton((ICalendarHelper**)&helper);
    helper->GetInstance((ICalendar**)&mSelectedDate);

    mHost->mContext = context;
    AutoPtr<IOnGestureListener> listener = new CalendarGestureListener();
    CGestureDetector::New(mHost->mContext, listener, (IGestureDetector**)&mGestureDetector);
    Init();
}

ECode CalendarView::_WeeksAdapter::Init()
{
    mSelectedWeek = mHost->GetWeeksSinceMinDate(mSelectedDate);
    mTotalWeekCount = mHost->GetWeeksSinceMinDate(mHost->mMaxDate);
    Int32 minDayOfWeek, maxDayOfWeek;
    mHost->mMinDate->Get(ICalendar::DAY_OF_WEEK, &minDayOfWeek);
    mHost->mMaxDate->Get(ICalendar::DAY_OF_WEEK, &maxDayOfWeek);
    if(minDayOfWeek != mHost->mFirstDayOfWeek
            || maxDayOfWeek != mHost->mFirstDayOfWeek)
    {
        mTotalWeekCount++;
    }
    return NOERROR;
}

ECode CalendarView::_WeeksAdapter::SetSelectedDay(
    /* [in] */ ICalendar* selectedDay)
{
    Int32 sDay, sYear, mDay, mYear;
    selectedDay->Get(ICalendar::DAY_OF_YEAR, &sDay);
    selectedDay->Get(ICalendar::YEAR, &sYear);
    mSelectedDate->Get(ICalendar::DAY_OF_YEAR, &mDay);
    mSelectedDate->Get(ICalendar::YEAR, &mYear);
    if(sDay == mDay && sYear == mYear)
    {
        return NOERROR;
    }
    Int64 millis;
    selectedDay->GetTimeInMillis(&millis);
    mSelectedDate->SetTimeInMillis(millis);
    mSelectedWeek = mHost->GetWeeksSinceMinDate(mSelectedDate);
    mSelectedDate->Get(ICalendar::MONTH, &mFocusedMonth);
    NotifyDataSetChanged();
    return NOERROR;
}

AutoPtr<ICalendar> CalendarView::_WeeksAdapter::GetSelectedDay()
{
    return mSelectedDate;
}

//@Override
Int32 CalendarView::_WeeksAdapter::GetCount()
{
    return mTotalWeekCount;
}

//@Override
AutoPtr<IInterface> CalendarView::_WeeksAdapter::GetItem(
    /* [in] */Int32 position)
{
    return NULL;
}

//@Override
Int64 CalendarView::_WeeksAdapter::GetItemId(
    /* [in] */ Int32 position)
{
    return position;
}

//@Override
AutoPtr<IView> CalendarView::_WeeksAdapter::GetView(
    /* [in] */ Int32 position,
    /* [in] */ IView* convertView,
    /* [in] */ IViewGroup* parent)
{
    AutoPtr<_WeekView> weekView;
    if (convertView != NULL) {
        weekView = (_WeekView*)(WeekView*) convertView;
    } else {
        weekView = (_WeekView*)(new WeekView(mHost->mContext, mHost));
        AutoPtr<IAbsListViewLayoutParams> params;
        CAbsListViewLayoutParams::New(IViewGroupLayoutParams::WRAP_CONTENT,
                    IViewGroupLayoutParams::WRAP_CONTENT, (IAbsListViewLayoutParams**)&params);
        weekView->SetLayoutParams(params);
        weekView->SetClickable(TRUE);
        weekView->SetOnTouchListener(((WeeksAdapter*)this));
    }
    Int32 selectedWeekDay;
    if(mSelectedWeek == position)
    {
        mSelectedDate->Get(ICalendar::DAY_OF_WEEK, &selectedWeekDay);
    }else {
        selectedWeekDay = -1;
    }
    weekView->Init(position, selectedWeekDay, mFocusedMonth);
    return (IView*)(WeekView*)(weekView.Get());
}

/**
 * Changes which month is in focus and updates the view.
 *
 * @param month The month to show as in focus [0-11]
 */
ECode CalendarView::_WeeksAdapter::SetFocusMonth(
    /* [in] */ Int32 month)
{
    if (mFocusedMonth == month) {
        return NOERROR;
    }
    mFocusedMonth = month;
    NotifyDataSetChanged();
    return NOERROR;
}

//@Override
ECode CalendarView::_WeeksAdapter::OnTouch(
    /* [in] */ IView* v,
    /* [in] */ IMotionEvent* event,
    /* [in] */ Boolean* result)
{
    Boolean enabled, touchResult;
    if ((mHost->mListView->IsEnabled(&enabled), enabled) &&
            (mGestureDetector->OnTouchEvent(event, &touchResult), touchResult))
    {
        AutoPtr<WeekView> weekView = (WeekView*) v;
        // if we cannot find a day for the given location we are done
        Float x;
        event->GetX(&x);
        if (!weekView->GetDayFromLocation(x, mHost->mTempDate))
        {
            *result = TRUE;
            return NOERROR;
        }
        // it is possible that the touched day is outside the valid range
        // we draw whole weeks but range end can fall not on the week end
        Boolean before, after;
        if ((mHost->mTempDate->IsBefore(mHost->mMinDate, &before), before) ||
                (mHost->mTempDate->IsAfter(mHost->mMaxDate, &after), after)) {
            *result = TRUE;
            return NOERROR;
        }
        OnDateTapped(mHost->mTempDate);
        *result = TRUE;
        return NOERROR;
    }
    *result = FALSE;
    return NOERROR;
}

/**
 * Maintains the same hour/min/sec but moves the day to the tapped day.
 *
 * @param day The day that was tapped
 */
ECode CalendarView::_WeeksAdapter::OnDateTapped(
    /* [in] */ ICalendar* day)
{
    SetSelectedDay(day);
    mHost->SetMonthDisplayed(day);
    return NOERROR;
}

/*............................_WeeksAdapter End................................*/


/*............................WeeksAdapter Start..............................*/

PInterface CalendarView::WeeksAdapter::Probe(
    /* [in] */ REIID riid)
{
    if(riid == EIID_IInterface) {
        return (IInterface*)(IViewOnTouchListener*)this;
    } else if(riid == EIID_IViewOnTouchListener) {
        return (IViewOnTouchListener*)this;
    } else if(riid == EIID_IBaseAdapter) {
        return (IBaseAdapter*)this;
    } else if(riid == EIID_ISpinnerAdapter) {
        return (ISpinnerAdapter*)this;
    } else if(riid == EIID_IAdapter) {
        return (IAdapter*)(IBaseAdapter*)this;
    } else if (riid == EIID_IListAdapter) {
        return (IListAdapter*)(IBaseAdapter*)this;
    } else {
        return NULL;
    }
}

CalendarView::WeeksAdapter::WeeksAdapter(
    /* [in] */ IContext* context,
    /* [in] */ CalendarView* host)
         : _WeeksAdapter(context, host)
{

}

UInt32 CalendarView::WeeksAdapter::AddRef()
{
    return _WeeksAdapter::ElRefBase::AddRef();
}

UInt32 CalendarView::WeeksAdapter::Release()
{
    return _WeeksAdapter::ElRefBase::Release();
}

ECode CalendarView::WeeksAdapter::GetInterfaceID(
    /* [in] */ IInterface *object,
    /* [out] */ InterfaceID *pIID)
{
    if (object == (IInterface*)(IViewOnTouchListener *)this) {
        *pIID = EIID_IViewOnTouchListener;
    }else if (object == (IInterface*)(IBaseAdapter *)this) {
        *pIID = EIID_IBaseAdapter;
    }else if (object == (IInterface*)(ISpinnerAdapter *)this) {
        *pIID = EIID_ISpinnerAdapter;
    }else if (object == (IInterface*)(IAdapter*)(IBaseAdapter *)this) {
        *pIID = EIID_IAdapter;
    }else {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return NOERROR;
}

ECode CalendarView::WeeksAdapter::AreAllItemsEnabled(
    /* [out] */ Boolean* enabled)
{
    *enabled = _WeeksAdapter::AreAllItemsEnabled();
    return NOERROR;
}

ECode CalendarView::WeeksAdapter::IsEnabled(
    /* [in] */ Int32 position,
    /* [out] */ Boolean* enabled)
{
    *enabled = _WeeksAdapter::IsEnabled(position);
    return NOERROR;
}

ECode CalendarView::WeeksAdapter::OnTouch(
    /* [in] */ IView* v,
    /* [in] */ IMotionEvent* event,
    /* [out] */ Boolean* result)
{
    return _WeeksAdapter::OnTouch(v, event, result);
}

ECode CalendarView::WeeksAdapter::NotifyDataSetChanged()
{
    return _WeeksAdapter::NotifyDataSetChanged();
}

ECode CalendarView::WeeksAdapter::NotifyDataSetInvalidated()
{
    return _WeeksAdapter::NotifyDataSetInvalidated();
}

ECode CalendarView::WeeksAdapter::GetDropDownView(
    /* [in] */ Int32 position,
    /* [in] */ IView* convertView,
    /* [in] */ IViewGroup* parent,
    /* [out] */ IView** view)
{
    AutoPtr<IView> temp =  _WeeksAdapter::GetDropDownView(position, convertView, parent);
    *view = temp;
    REFCOUNT_ADD(*view);
    return NOERROR;
}

ECode CalendarView::WeeksAdapter::RegisterDataSetObserver(
    /* [in] */ IDataSetObserver* observer)
{
    return _WeeksAdapter::RegisterDataSetObserver(observer);
}

ECode CalendarView::WeeksAdapter::UnregisterDataSetObserver(
    /* [in] */ IDataSetObserver* observer)
{
    return _WeeksAdapter::UnregisterDataSetObserver(observer);
}

ECode CalendarView::WeeksAdapter::GetCount(
    /* [out] */ Int32* count)
{
    *count = _WeeksAdapter::GetCount();
    return NOERROR;
}

ECode CalendarView::WeeksAdapter::GetItem(
    /* [in] */ Int32 position,
    /* [out] */ IInterface** item)
{
    AutoPtr<IInterface> temp = _WeeksAdapter::GetItem(position);
    *item = temp;
    REFCOUNT_ADD(*item);
    return NOERROR;
}

ECode CalendarView::WeeksAdapter::GetItemId(
    /* [in] */ Int32 position,
    /* [out] */ Int64* itemId)
{
    *itemId = _WeeksAdapter::GetItemId(position);
    return NOERROR;
}

ECode CalendarView::WeeksAdapter::HasStableIds(
    /* [out] */ Boolean* hasStableIds)
{
    *hasStableIds = _WeeksAdapter::HasStableIds();
    return NOERROR;
}

ECode CalendarView::WeeksAdapter::GetView(
    /* [in] */ Int32 position,
    /* [in] */ IView* convertView,
    /* [in] */ IViewGroup* parent,
    /* [out] */ IView** view)
{
    AutoPtr<IView> temp = _WeeksAdapter::GetView(position, convertView, parent);
    *view = temp;
    REFCOUNT_ADD(*view);
    return NOERROR;
}

ECode CalendarView::WeeksAdapter::GetItemViewType(
    /* [in] */ Int32 position,
    /* [out] */ Int32* viewType)
{
    *viewType = _WeeksAdapter::GetItemViewType(position);
    return NOERROR;
}

ECode CalendarView::WeeksAdapter::GetViewTypeCount(
    /* [out] */ Int32* count)
{
    *count = _WeeksAdapter::GetViewTypeCount();
    return NOERROR;
}

ECode CalendarView::WeeksAdapter::IsEmpty(
     /* [out] */ Boolean* isEmpty)
{
    *isEmpty = _WeeksAdapter::IsEmpty();
    return NOERROR;
}
/*............................WeeksAdapter End................................*/


/*............................_WeekView Start..............................*/
CalendarView::_WeekView::_WeekView(
    /* [in] */ IContext* context,
    /* [in] */ CalendarView* host)
     : View(context)
     , mHost(host)
     , mHasFocusedDay(FALSE)
     , mHasUnfocusedDay(FALSE)
     , mMonthOfFirstWeekDay(-1)
     , mLastWeekDayMonth(-1)
     , mWeek(-1)
     , mWidth(0)
     , mHeight(0)
     , mHasSelectedDay(FALSE)
     , mSelectedDay(-1)
     , mNumCells(0)
     , mSelectedLeft(-1)
     , mSelectedRight(-1)
{
    // Sets up any standard paints that will be used
    CRect::New((IRect**)&mTempRect);
    CPaint::New((IPaint**)&mDrawPaint);
    CPaint::New((IPaint**)&mMonthNumDrawPaint);
    InitilaizePaints();
}

ECode CalendarView::_WeekView::Init(
    /* [in] */ Int32 weekNumber,
    /* [in] */ Int32 selectedWeekDay,
    /* [in] */ Int32 focusedMonth)
{
    mSelectedDay = selectedWeekDay;
    mHasSelectedDay = mSelectedDay != -1;
    mNumCells = mHost->mShowWeekNumber ? mHost->mDaysPerWeek + 1 : mHost->mDaysPerWeek;
    mWeek = weekNumber;
    Int64 millis;
    mHost->mMinDate->GetTimeInMillis(&millis);
    mHost->mTempDate->SetTimeInMillis(millis);

    mHost->mTempDate->Add(ICalendar::WEEK_OF_YEAR, mWeek);
    mHost->mTempDate->SetFirstDayOfWeek(mHost->mFirstDayOfWeek);

    // Allocate space for caching the day numbers and focus values
    mDayNumbers = ArrayOf<String>::Alloc(mNumCells);
    mFocusDay = ArrayOf<Boolean>::Alloc(mNumCells);

    // If we're showing the week number calculate it based on Monday
    Int32 i = 0;
    if (mHost->mShowWeekNumber) {
        Int32 value;
        mHost->mTempDate->Get(ICalendar::WEEK_OF_YEAR, &value);
        String sv = StringUtils::ToString(value);
        (*mDayNumbers)[0] = sv;
        i++;
    }

    // Now adjust our starting day based on the start day of the week
    Int32 dayOfWeek;
    mHost->mTempDate->Get(ICalendar::DAY_OF_WEEK, &dayOfWeek);
    Int32 diff = mHost->mFirstDayOfWeek - dayOfWeek;
    mHost->mTempDate->Add(ICalendar::DAY_OF_MONTH, diff);

    if(mFirstDay != NULL) mFirstDay = NULL;

    AutoPtr<IInterface> clone;
    ICloneable::Probe(mHost->mTempDate)->Clone((IInterface**)&clone);
    mFirstDay = ICalendar::Probe(clone);
    mHost->mTempDate->Get(ICalendar::MONTH, &mMonthOfFirstWeekDay);

    mHasUnfocusedDay = TRUE;
    for (; i < mNumCells; i++) {
        Int32 month;
        mHost->mTempDate->Get(ICalendar::MONTH, &month);
        Boolean isFocusedDay = (month == focusedMonth);
        (*mFocusDay)[i] = isFocusedDay;
        mHasFocusedDay |= isFocusedDay;
        mHasUnfocusedDay &= !isFocusedDay;
        // do not draw dates outside the valid range to avoid user confusion
        Boolean before, after;
        if ((mHost->mTempDate->IsBefore(mHost->mMinDate, &before), before)
                || (mHost->mTempDate->IsAfter(mHost->mMaxDate, &after), after)) {
            (*mDayNumbers)[i] = String("");
        } else {
//          TODO
//            mDayNumbers[i] = String.format(Locale.getDefault(), "%d",
//                    mTempDate.get(Calendar.DAY_OF_MONTH));
            Int32 dayOfMonth = 0;
            mHost->mTempDate->Get(ICalendar::DAY_OF_MONTH, &dayOfMonth);
            (*mDayNumbers)[i] = StringUtils::ToString(dayOfMonth);
        }
        mHost->mTempDate->Add(ICalendar::DAY_OF_MONTH, 1);
    }
    // We do one extra add at the end of the loop, if that pushed us to
    // new month undo it
    Int32 value;
    mHost->mTempDate->Get(ICalendar::DAY_OF_MONTH, &value);
    if (value == 1) {
        mHost->mTempDate->Add(ICalendar::DAY_OF_MONTH, -1);
    }
    mHost->mTempDate->Get(ICalendar::MONTH, &mLastWeekDayMonth);

    UpdateSelectionPositions();
    return NOERROR;
}

Int32 CalendarView::_WeekView::GetMonthOfFirstWeekDay()
{
    return mMonthOfFirstWeekDay;
}

Int32 CalendarView::_WeekView::GetMonthOfLastWeekDay()
{
    return mLastWeekDayMonth;
}

AutoPtr<ICalendar> CalendarView::_WeekView::GetFirstDay()
{
    return mFirstDay;
}

Boolean CalendarView::_WeekView::GetDayFromLocation(
    /* [in] */ Float x,
    /* [in, out] */ ICalendar* outCalendar)
{
    Boolean isLayoutRtl = IsLayoutRtl();

    Int32 start;
    Int32 end;

    if (isLayoutRtl) {
        start = 0;
        end = mHost->mShowWeekNumber ? mWidth - mWidth / mNumCells : mWidth;
    } else {
        start = mHost->mShowWeekNumber ? mWidth / mNumCells : 0;
        end = mWidth;
    }

    if (x < start || x > end) {
        outCalendar->Clear();
        return FALSE;
    }

    // Selection is (x - start) / (pixels/day) which is (x - start) * day / pixels
    Int32 dayPosition = (Int32) ((x - start) * mHost->mDaysPerWeek / (end - start));

    if (isLayoutRtl) {
        dayPosition = mHost->mDaysPerWeek - 1 - dayPosition;
    }
    Int64 millis;
    mFirstDay->GetTimeInMillis(&millis);
    outCalendar->SetTimeInMillis(millis);
    outCalendar->Add(ICalendar::DAY_OF_MONTH, dayPosition);

    return TRUE;
}

void CalendarView::_WeekView::OnDraw(
    /* [in] */ ICanvas* canvas)
{
    DrawBackground(canvas);
    DrawWeekNumbersAndDates(canvas);
    DrawWeekSeparators(canvas);
    DrawSelectedDateVerticalBars(canvas);
}


void CalendarView::_WeekView::OnSizeChanged(
    /* [in] */ Int32 w,
    /* [in] */ Int32 h,
    /* [in] */ Int32 oldw,
    /* [in] */ Int32 oldh)
{
    mWidth = w;
    UpdateSelectionPositions();
}

void CalendarView::_WeekView::OnMeasure(
    /* [in] */ Int32 widthMeasureSpec,
    /* [in] */ Int32 heightMeasureSpec)
{
    Int32 height, paddingTop, paddingBottom;
    mHost->mListView->GetHeight(&height);
    mHost->mListView->GetPaddingTop(&paddingTop);
    mHost->mListView->GetPaddingBottom(&paddingBottom);

    mHeight = (height - paddingTop - paddingBottom) / mHost->mShownWeekCount;
    SetMeasuredDimension(View::MeasureSpec::GetSize(widthMeasureSpec), mHeight);
}

void CalendarView::_WeekView::InitilaizePaints()
{
    mDrawPaint->SetFakeBoldText(FALSE);
    mDrawPaint->SetAntiAlias(TRUE);
    mDrawPaint->SetStyle(PaintStyle_FILL);
    mMonthNumDrawPaint->SetFakeBoldText(TRUE);
    mMonthNumDrawPaint->SetAntiAlias(TRUE);
    mMonthNumDrawPaint->SetStyle(PaintStyle_FILL);
    mMonthNumDrawPaint->SetTextAlign(PaintAlign_CENTER);
    mMonthNumDrawPaint->SetTextSize(mHost->mDateTextSize);
}

void CalendarView::_WeekView::DrawBackground(
    /* [in] */ ICanvas* canvas)
{
    if (!mHasSelectedDay) {
        return;
    }
    mDrawPaint->SetColor(mHost->mSelectedWeekBackgroundColor);

    ((CRect*)mTempRect.Get())->mTop = mHost->mWeekSeperatorLineWidth;
    ((CRect*)mTempRect.Get())->mBottom = mHeight;

    Boolean isLayoutRtl = IsLayoutRtl();

    if (isLayoutRtl) {
        ((CRect*)mTempRect.Get())->mLeft = 0;
        ((CRect*)mTempRect.Get())->mRight = mSelectedLeft - 2;
    } else {
        ((CRect*)mTempRect.Get())->mLeft = mHost->mShowWeekNumber ? mWidth / mNumCells : 0;
        ((CRect*)mTempRect.Get())->mRight = mSelectedLeft - 2;
    }
    canvas->DrawRect(mTempRect, mDrawPaint);

    if (isLayoutRtl) {
        ((CRect*)mTempRect.Get())->mLeft = mSelectedRight + 3;
        ((CRect*)mTempRect.Get())->mRight = mHost->mShowWeekNumber ? mWidth - mWidth / mNumCells : mWidth;
    } else {
        ((CRect*)mTempRect.Get())->mLeft = mSelectedRight + 3;
        ((CRect*)mTempRect.Get())->mRight = mWidth;
    }
    canvas->DrawRect(mTempRect, mDrawPaint);
}

void CalendarView::_WeekView::DrawWeekNumbersAndDates(
    /* [in] */ ICanvas* canvas)
{
    Float textHeight ;
    mDrawPaint->GetTextSize(&textHeight);
    Int32 y = (Int32) ((mHeight + textHeight) / 2) - mHost->mWeekSeperatorLineWidth;
    Int32 nDays = mNumCells;
    Int32 divisor = 2 * nDays;

    mDrawPaint->SetTextAlign(PaintAlign_CENTER);
    mDrawPaint->SetTextSize(mHost->mDateTextSize);

    Int32 i = 0;

    if (IsLayoutRtl()) {
        for (; i < nDays - 1; i++) {
            mMonthNumDrawPaint->SetColor((*mFocusDay)[i] ? mHost->mFocusedMonthDateColor
                    : mHost->mUnfocusedMonthDateColor);
            Int32 x = (2 * i + 1) * mWidth / divisor;
            canvas->DrawText((*mDayNumbers)[nDays - 1 - i], x, y, mMonthNumDrawPaint);
        }
        if (mHost->mShowWeekNumber) {
            mDrawPaint->SetColor(mHost->mWeekNumberColor);
            Int32 x = mWidth - mWidth / divisor;
            canvas->DrawText((*mDayNumbers)[0], x, y, mDrawPaint);
        }
    } else {
        if (mHost->mShowWeekNumber) {
            mDrawPaint->SetColor(mHost->mWeekNumberColor);
            Int32 x = mWidth / divisor;
            canvas->DrawText((*mDayNumbers)[0], x, y, mDrawPaint);
            i++;
        }
        for (; i < nDays; i++) {
            mMonthNumDrawPaint->SetColor((*mFocusDay)[i] ? mHost->mFocusedMonthDateColor
                    : mHost->mUnfocusedMonthDateColor);
            Int32 x = (2 * i + 1) * mWidth / divisor;
            canvas->DrawText((*mDayNumbers)[i], x, y, mMonthNumDrawPaint);
        }
    }
}

void CalendarView::_WeekView::DrawWeekSeparators(
    /* [in] */ ICanvas* canvas)
{
    // If it is the topmost fully visible child do not draw separator line
    Int32 firstFullyVisiblePosition;
    mHost->mListView->GetFirstVisiblePosition(&firstFullyVisiblePosition);
    AutoPtr<IView> v;
    mHost->mListView->GetChildAt(0, (IView**)&v);
    Int32 top;
    v->GetTop(&top);
    if (top < 0) {
        firstFullyVisiblePosition++;
    }
    if (firstFullyVisiblePosition == mWeek) {
        return;
    }
    mDrawPaint->SetColor(mHost->mWeekSeparatorLineColor);
    mDrawPaint->SetStrokeWidth(mHost->mWeekSeperatorLineWidth);
    Float startX;
    Float stopX;
    if (IsLayoutRtl()) {
        startX = 0;
        stopX = mHost->mShowWeekNumber ? mWidth - mWidth / mNumCells : mWidth;
    } else {
        startX = mHost->mShowWeekNumber ? mWidth / mNumCells : 0;
        stopX = mWidth;
    }
    canvas->DrawLine(startX, 0, stopX, 0, mDrawPaint);
}

void CalendarView::_WeekView::DrawSelectedDateVerticalBars(
    /* [in] */ ICanvas* canvas)
{
    if (!mHasSelectedDay) {
        return;
    }
    mHost->mSelectedDateVerticalBar->SetBounds(mSelectedLeft - mHost->mSelectedDateVerticalBarWidth / 2,
            mHost->mWeekSeperatorLineWidth,
            mSelectedLeft +  mHost->mSelectedDateVerticalBarWidth / 2, mHeight);
    mHost->mSelectedDateVerticalBar->Draw(canvas);
    mHost->mSelectedDateVerticalBar->SetBounds(mSelectedRight - mHost->mSelectedDateVerticalBarWidth / 2,
            mHost->mWeekSeperatorLineWidth,
            mSelectedRight + mHost->mSelectedDateVerticalBarWidth / 2, mHeight);
    mHost-> mSelectedDateVerticalBar->Draw(canvas);
}

void CalendarView::_WeekView::UpdateSelectionPositions()
{
    if (mHasSelectedDay) {
        Boolean isLayoutRtl = IsLayoutRtl();
        Int32 selectedPosition = mSelectedDay - mHost->mFirstDayOfWeek;
        if (selectedPosition < 0) {
            selectedPosition += 7;
        }
        if (mHost->mShowWeekNumber && !isLayoutRtl) {
            selectedPosition++;
        }
        if (isLayoutRtl) {
            mSelectedLeft = (mHost->mDaysPerWeek - 1 - selectedPosition) * mWidth / mNumCells;

        } else {
            mSelectedLeft = selectedPosition * mWidth / mNumCells;
        }
        mSelectedRight = mSelectedLeft + mWidth / mNumCells;
    }
}
/*............................_WeekView End................................*/


/*............................WeekView Start..............................*/
IVIEW_METHODS_IMPL(CalendarView::WeekView, CalendarView::_WeekView)

IDRAWABLECALLBACK_METHODS_IMPL(CalendarView::WeekView, CalendarView::_WeekView)

IKEYEVENTCALLBACK_METHODS_IMPL(CalendarView::WeekView, CalendarView::_WeekView)

IACCESSIBILITYEVENTSOURCE_METHODS_IMPL(CalendarView::WeekView, CalendarView::_WeekView)

CalendarView::WeekView::WeekView(
    /* [in] */ IContext* context,
    /* [in] */ CalendarView* host)
    : _WeekView(context, host)
{
}

PInterface CalendarView::WeekView::Probe(
    /* [in] */ REIID riid)
{
    if(riid == EIID_IInterface) {
        return (IInterface*)(IView*)this;

    } else if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);

    } else if(riid == EIID_IView) {
        return (IInterface*)(IView*)this;

    } else if(riid == EIID_IDrawableCallback) {
        return (IInterface*)(IDrawableCallback*)this;

    } else if(riid == EIID_IKeyEventCallback) {
        return (IInterface*)(IKeyEventCallback*)this;
    }
    else if (riid == EIID_IWeakReferenceSource) {
        return (IInterface*)(IWeakReferenceSource*)this;
    }
    else if(riid == EIID_IAccessibilityEventSource) {
        return (IInterface*)(IAccessibilityEventSource*)this;

    } else {
        return NULL;
    }
}

UInt32 CalendarView::WeekView::AddRef()
{
    return WeekView::_WeekView::AddRef();
}

UInt32 CalendarView::WeekView::Release()
{
    return WeekView::_WeekView::Release();
}

ECode CalendarView::WeekView::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    if (pObject == (IInterface*)(IView*)this) {
        *pIID = EIID_IView;
    }
    else if (pObject == (IInterface*)(IDrawableCallback *)this) {
        *pIID = EIID_IDrawableCallback;
    }
    else if (pObject == (IInterface*)(IKeyEventCallback *)this) {
        *pIID = EIID_IKeyEventCallback;
    }
    else if (pObject == (IInterface*)(IAccessibilityEventSource*)this) {
        *pIID = EIID_IAccessibilityEventSource;
    }
    else if (pObject == (IInterface*)(IWeakReferenceSource*)this) {
        *pIID = EIID_IWeakReferenceSource;
    }
    else {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return NOERROR;
}

ECode CalendarView::WeekView::GetWeakReference(
    /* [out] */ IWeakReference** weakReference)
{
    VALIDATE_NOT_NULL(weakReference)
    *weakReference = new WeakReferenceImpl(Probe(EIID_IInterface), CreateWeak(this));
    REFCOUNT_ADD(*weakReference)
    return NOERROR;
}
/*............................WeekView End................................*/


/*............................SetUpAdapterDataSetObserver Start..............................*/
CalendarView::SetUpAdapterDataSetObserver::SetUpAdapterDataSetObserver(
    /* [in] */ CalendarView* host)
    : mHost(host)
{

}

ECode CalendarView::SetUpAdapterDataSetObserver::OnChanged()
{
    if (mHost->mOnDateChangeListener != NULL) {
        AutoPtr<ICalendar> selectedDay = mHost->mAdapter->GetSelectedDay();
        Int32 year, month, dayOfMonth;
        selectedDay->Get(ICalendar::YEAR, &year);
        selectedDay->Get(ICalendar::MONTH, &month);
        selectedDay->Get(ICalendar::DAY_OF_MONTH, &dayOfMonth);

        mHost->mOnDateChangeListener->OnSelectedDayChange(
                (ICalendarView*)(mHost->Probe(EIID_ICalendarView)), year, month, dayOfMonth);
    }
    return NOERROR;
}

ECode CalendarView::SetUpAdapterDataSetObserver::OnInvalidated()
{
    return NOERROR;
}
/*............................SetUpAdapterDataSetObserver End..............................*/


/*............................SetUpListViewAbsListViewOnScrollListener Start..............................*/

CalendarView::SetUpListViewAbsListViewOnScrollListener::SetUpListViewAbsListViewOnScrollListener(
    /* [in] */ CalendarView* host) : mHost(host)
{}

ECode CalendarView::SetUpListViewAbsListViewOnScrollListener::OnScrollStateChanged(
    /* [in] */ IAbsListView* view,
    /* [in] */ Int32 scrollState)
{
    mHost->OnScrollStateChanged(view, scrollState);
    return NOERROR;
}

ECode CalendarView::SetUpListViewAbsListViewOnScrollListener::OnScroll(
    /* [in] */ IAbsListView* view,
    /* [in] */ Int32 firstVisibleItem,
    /* [in] */ Int32 visibleItemCount,
    /* [in] */ Int32 totalItemCount)
{
    mHost->OnScroll(view, firstVisibleItem, visibleItemCount, totalItemCount);
    return NOERROR;
}

/*............................SetUpListViewAbsListViewOnScrollListener End..............................*/
CalendarView::CalendarView()
    : mWeekSeperatorLineWidth(0)
    , mDateTextSize(0)
    , mSelectedDateVerticalBarWidth(0)
    , mSelectedWeekBackgroundColor(0)
    , mFocusedMonthDateColor(0)
    , mUnfocusedMonthDateColor(0)
    , mWeekSeparatorLineColor(0)
    , mWeekNumberColor(0)
    , mWeekDayTextAppearanceResId(0)
    , mDateTextAppearanceResId(0)
    , mListScrollTopOffset(2)
    , mWeekMinVisibleHeight(12)
    , mBottomBuffer(20)
    , mShownWeekCount(0)
    , mShowWeekNumber(FALSE)
    , mDaysPerWeek(7)
    , mFriction(0.05f)
    , mVelocityScale(0.333f)
    , mFirstDayOfWeek(0)
    , mCurrentMonthDisplayed(0)
    , mPreviousScrollPosition(0)
    , mIsScrollingUp(FALSE)
    , mPreviousScrollState(IAbsListViewOnScrollListener::SCROLL_STATE_IDLE)
    , mCurrentScrollState(IAbsListViewOnScrollListener::SCROLL_STATE_IDLE)
{
    AutoPtr<ISimpleDateFormat> sp;
    CSimpleDateFormat::New(DATE_FORMAT, (ISimpleDateFormat**)&sp);
    mDateFormat = sp;
}

ECode CalendarView::Init(
    /* [in] */ IContext* context)
{
    return Init(context, NULL);
}

ECode CalendarView::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return Init(context, attrs, 0);
}

ECode CalendarView::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    FrameLayout::Init(context, attrs, 0);
    InitFromSource(context, attrs, defStyle);
    return NOERROR;
}

CalendarView::CalendarView(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
    : FrameLayout(context, attrs, 0)
    , mWeekSeperatorLineWidth(0)
    , mDateTextSize(0)
    , mSelectedDateVerticalBarWidth(0)
    , mSelectedWeekBackgroundColor(0)
    , mFocusedMonthDateColor(0)
    , mUnfocusedMonthDateColor(0)
    , mWeekSeparatorLineColor(0)
    , mWeekNumberColor(0)
    , mWeekDayTextAppearanceResId(0)
    , mDateTextAppearanceResId(0)
    , mListScrollTopOffset(2)
    , mWeekMinVisibleHeight(12)
    , mBottomBuffer(20)
    , mShownWeekCount(0)
    , mShowWeekNumber(FALSE)
    , mDaysPerWeek(7)
    , mFriction(0.05f)
    , mVelocityScale(0.333f)
    , mFirstDayOfWeek(0)
    , mCurrentMonthDisplayed(0)
    , mPreviousScrollPosition(0)
    , mIsScrollingUp(FALSE)
    , mPreviousScrollState(IAbsListViewOnScrollListener::SCROLL_STATE_IDLE)
    , mCurrentScrollState(IAbsListViewOnScrollListener::SCROLL_STATE_IDLE)
{
    AutoPtr<ISimpleDateFormat> sp;
    CSimpleDateFormat::New(DATE_FORMAT, (ISimpleDateFormat**)&sp);
    mDateFormat = sp;
    InitFromSource(context, attrs, defStyle);

}

ECode CalendarView::InitFromSource(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    mScrollStateChangedRunnable = new ScrollStateRunnable((ICalendarView*)this->Probe(EIID_ICalendarView));
    AutoPtr<ILocaleHelper> localeHelper;
    CLocaleHelper::AcquireSingleton((ILocaleHelper**)&localeHelper);
    AutoPtr<ILocale> locale;
    localeHelper->GetDefault((ILocale**)&locale);

    SetCurrentLocale(locale);

    AutoPtr<ArrayOf<Int32> > attrId = ArrayOf<Int32>::Alloc(
                    const_cast<Int32*>(R::styleable::CalendarView),
                    ARRAY_SIZE(R::styleable::CalendarView));
    AutoPtr<ITypedArray> a;
    FAIL_RETURN(context->ObtainStyledAttributes(
            attrs, attrId, R::attr::calendarViewStyle, 0, (ITypedArray**)&a));
    a->GetBoolean(R::styleable::CalendarView_showWeekNumber, DEFAULT_SHOW_WEEK_NUMBER, &mShowWeekNumber);
    AutoPtr<ILocaleDataHelper> dataHelper;
    CLocaleDataHelper::AcquireSingleton((ILocaleDataHelper**)&dataHelper);

    AutoPtr<ILocaleData> data;
    dataHelper->Get(locale, (ILocaleData**)&data);

    AutoPtr<IInteger32> integer;
    data->GetFirstDayOfWeek((IInteger32**)&integer);
    Int32 value;
    integer->GetValue(&value);
    a->GetInt32(R::styleable::CalendarView_firstDayOfWeek, value, &mFirstDayOfWeek);
    String minDate;
    a->GetString(R::styleable::CalendarView_minDate, &minDate);
    if(minDate.IsEmpty() || !ParseDate(minDate, mMinDate))
    {
        ParseDate(DEFAULT_MIN_DATE, mMinDate);
    }
    String maxDate;
    a->GetString(R::styleable::CalendarView_maxDate, &maxDate);
    if(maxDate.IsEmpty() || !ParseDate(maxDate, mMaxDate))
    {
        ParseDate(DEFAULT_MAX_DATE, mMaxDate);
    }
    Boolean before;
    mMaxDate->IsBefore(mMinDate, &before);
    if(before)
    {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    a->GetInt32(R::styleable::CalendarView_shownWeekCount, DEFAULT_SHOWN_WEEK_COUNT, &mShownWeekCount);

    a->GetColor(R::styleable::CalendarView_selectedWeekBackgroundColor, 0, &mSelectedWeekBackgroundColor);
    a->GetColor(R::styleable::CalendarView_focusedMonthDateColor, 0, &mFocusedMonthDateColor);
    a->GetColor(R::styleable::CalendarView_unfocusedMonthDateColor, 0, &mUnfocusedMonthDateColor);
    a->GetColor(R::styleable::CalendarView_weekSeparatorLineColor, 0, &mWeekSeparatorLineColor);
    a->GetColor(R::styleable::CalendarView_weekNumberColor, 0, &mWeekNumberColor);
    a->GetDrawable(R::styleable::CalendarView_selectedDateVerticalBar, (IDrawable**)&mSelectedDateVerticalBar);
    a->GetResourceId(R::styleable::CalendarView_dateTextAppearance, R::style::TextAppearance_Small, &mDateTextAppearanceResId);
    UpdateDateTextSize();

    a->GetResourceId(
            R::styleable::CalendarView_weekDayTextAppearance,
            DEFAULT_WEEK_DAY_TEXT_APPEARANCE_RES_ID, &mWeekDayTextAppearanceResId);
    a->Recycle();

    AutoPtr<IDisplayMetrics> displayMetrics;
    GetResources()->GetDisplayMetrics((IDisplayMetrics**)&displayMetrics);

    Float temp;
    AutoPtr<ITypedValueHelper> helper;
    CTypedValueHelper::AcquireSingleton((ITypedValueHelper**)&helper);
    helper->ApplyDimension(ITypedValue::COMPLEX_UNIT_DIP,
                UNSCALED_WEEK_MIN_VISIBLE_HEIGHT, displayMetrics, &temp);
    mWeekMinVisibleHeight = (Int32) temp;

    helper->ApplyDimension(ITypedValue::COMPLEX_UNIT_DIP,
            UNSCALED_LIST_SCROLL_TOP_OFFSET, displayMetrics, &temp);
    mListScrollTopOffset = (Int32)temp;
    helper->ApplyDimension(ITypedValue::COMPLEX_UNIT_DIP,
            UNSCALED_BOTTOM_BUFFER, displayMetrics, &temp);
    mBottomBuffer = (Int32)temp;

    helper->ApplyDimension(ITypedValue::COMPLEX_UNIT_DIP,
            UNSCALED_SELECTED_DATE_VERTICAL_BAR_WIDTH, displayMetrics, &temp);
    mSelectedDateVerticalBarWidth = (Int32)temp;
    helper->ApplyDimension(ITypedValue::COMPLEX_UNIT_DIP,
            UNSCALED_WEEK_SEPARATOR_LINE_WIDTH, displayMetrics, &temp);
    mWeekSeperatorLineWidth = (Int32)temp;
    AutoPtr<IInterface> inter;
    mContext->GetSystemService(IContext::LAYOUT_INFLATER_SERVICE,(IInterface**)&inter);
    AutoPtr<ILayoutInflater> layoutInflater = ILayoutInflater::Probe(inter);
    AutoPtr<IView> content;
    layoutInflater->Inflate(R::layout::calendar_view, NULL, FALSE, (IView**)&content);
    AddView(content);
    AutoPtr<IView> vList = FindViewById(R::id::list);
    mListView = IListView::Probe(vList);
    content->FindViewById(R::id::day_names, (IView**)&mDayNamesHeader);
    content->FindViewById(R::id::month_name, (IView**)&mMonthName);
    SetUpHeader();
    SetUpListView();
    SetUpAdapter();
    // go to today or whichever is close to today min or max date
    AutoPtr<ISystem> system;
    Elastos::Core::CSystem::AcquireSingleton((ISystem**)&system);
    Int64 now;
    system->GetCurrentTimeMillis(&now);
    mTempDate->SetTimeInMillis(now);
    if (mTempDate->IsBefore(mMinDate, &before), before) {
        FAIL_RETURN(GoTo(mMinDate, FALSE, TRUE, TRUE));
    } else if (mMaxDate->IsBefore(mTempDate, &before), before) {
        FAIL_RETURN(GoTo(mMaxDate, FALSE, TRUE, TRUE));
    } else {
        FAIL_RETURN(GoTo(mTempDate, FALSE, TRUE, TRUE));
    }
    Invalidate();
    return NOERROR;
}

/**
 * Sets the number of weeks to be shown.
 *
 * @param count The shown week count.
 *
 * @attr ref android.R.styleable#CalendarView_shownWeekCount
 */
ECode CalendarView::SetShownWeekCount(
    /* [in] */ Int32 count)
{
    if (mShownWeekCount != count) {
        mShownWeekCount = count;
        FAIL_RETURN(Invalidate());
    }
    return NOERROR;
}

/**
 * Gets the number of weeks to be shown.
 *
 * @return The shown week count.
 *
 * @attr ref android.R.styleable#CalendarView_shownWeekCount
 */
Int32 CalendarView::GetShownWeekCount()
{
    return mShownWeekCount;
}

/**
 * Sets the background color for the selected week.
 *
 * @param color The week background color.
 *
 * @attr ref android.R.styleable#CalendarView_selectedWeekBackgroundColor
 */
ECode CalendarView::SetSelectedWeekBackgroundColor(
    /* [in] */ Int32 color)
{
    if (mSelectedWeekBackgroundColor != color) {
        mSelectedWeekBackgroundColor = color;
        Int32 childCount;
        FAIL_RETURN(mListView->GetChildCount(&childCount));
        for (Int32 i = 0; i < childCount; i++) {
            AutoPtr<IView> v;
            FAIL_RETURN(mListView->GetChildAt(i, (IView**)&v));
            AutoPtr<_WeekView> weekView = (_WeekView*)(WeekView*)v.Get();

            if (weekView->mHasSelectedDay) {
                FAIL_RETURN(weekView->Invalidate());
            }
        }
    }
    return NOERROR;
}

/**
 * Gets the background color for the selected week.
 *
 * @return The week background color.
 *
 * @attr ref android.R.styleable#CalendarView_selectedWeekBackgroundColor
 */
Int32 CalendarView::GetSelectedWeekBackgroundColor()
{
    return mSelectedWeekBackgroundColor;
}

/**
 * Sets the color for the dates of the focused month.
 *
 * @param color The focused month date color.
 *
 * @attr ref android.R.styleable#CalendarView_focusedMonthDateColor
 */
ECode CalendarView::SetFocusedMonthDateColor(
    /* [in] */ Int32 color)
{
    if (mFocusedMonthDateColor != color) {
        mFocusedMonthDateColor = color;
        Int32 childCount;
        FAIL_RETURN(mListView->GetChildCount(&childCount));
        for (Int32 i = 0; i < childCount; i++) {
            AutoPtr<IView> v;
            mListView->GetChildAt(i, (IView**)&v);
            AutoPtr<_WeekView> weekView = (_WeekView*)(WeekView*)v.Get();
            if (weekView->mHasFocusedDay) {
                FAIL_RETURN(weekView->Invalidate());
            }
        }
    }
    return NOERROR;
}

/**
 * Gets the color for the dates in the focused month.
 *
 * @return The focused month date color.
 *
 * @attr ref android.R.styleable#CalendarView_focusedMonthDateColor
 */
Int32 CalendarView::GetFocusedMonthDateColor()
{
    return mFocusedMonthDateColor;
}

/**
 * Sets the color for the dates of a not focused month.
 *
 * @param color A not focused month date color.
 *
 * @attr ref android.R.styleable#CalendarView_unfocusedMonthDateColor
 */
ECode CalendarView::SetUnfocusedMonthDateColor(
    /* [in] */ Int32 color)
{
    if (mUnfocusedMonthDateColor != color) {
        mUnfocusedMonthDateColor = color;
        Int32 childCount;
        FAIL_RETURN(mListView->GetChildCount(&childCount));
        for (Int32 i = 0; i < childCount; i++) {
            AutoPtr<IView> v;
            mListView->GetChildAt(i, (IView**)&v);
            AutoPtr<_WeekView> weekView = (_WeekView*)(WeekView*)v.Get();
            if (weekView->mHasUnfocusedDay) {
                FAIL_RETURN(weekView->Invalidate());
            }
        }
    }
    return NOERROR;
}

/**
 * Gets the color for the dates in a not focused month.
 *
 * @return A not focused month date color.
 *
 * @attr ref android.R.styleable#CalendarView_unfocusedMonthDateColor
 */
Int32 CalendarView::GetUnfocusedMonthDateColor()
{
    return mUnfocusedMonthDateColor;
}

/**
 * Sets the color for the week numbers.
 *
 * @param color The week number color.
 *
 * @attr ref android.R.styleable#CalendarView_weekNumberColor
 */
ECode CalendarView::SetWeekNumberColor(
    /* [in] */ Int32 color)
{
    if (mWeekNumberColor != color) {
        mWeekNumberColor = color;
        if (mShowWeekNumber) {
            FAIL_RETURN(InvalidateAllWeekViews());
        }
    }
    return NOERROR;
}

/**
 * Gets the color for the week numbers.
 *
 * @return The week number color.
 *
 * @attr ref android.R.styleable#CalendarView_weekNumberColor
 */
Int32 CalendarView::GetWeekNumberColor() {
    return mWeekNumberColor;
}

/**
 * Sets the color for the separator line between weeks.
 *
 * @param color The week separator color.
 *
 * @attr ref android.R.styleable#CalendarView_weekSeparatorLineColor
 */
ECode CalendarView::SetWeekSeparatorLineColor(
    /* [in] */ Int32 color)
{
    if (mWeekSeparatorLineColor != color) {
        mWeekSeparatorLineColor = color;
        FAIL_RETURN(InvalidateAllWeekViews());
    }
    return NOERROR;
}

/**
 * Gets the color for the separator line between weeks.
 *
 * @return The week separator color.
 *
 * @attr ref android.R.styleable#CalendarView_weekSeparatorLineColor
 */
Int32 CalendarView::GetWeekSeparatorLineColor() {
    return mWeekSeparatorLineColor;
}

/**
 * Sets the drawable for the vertical bar shown at the beginning and at
 * the end of the selected date.
 *
 * @param resourceId The vertical bar drawable resource id.
 *
 * @attr ref android.R.styleable#CalendarView_selectedDateVerticalBar
 */
ECode CalendarView::SetSelectedDateVerticalBar(
    /* [in] */ Int32 resourceId)
{

    AutoPtr<IDrawable> drawable;
    FAIL_RETURN(GetResources()->GetDrawable(resourceId, (IDrawable**)&drawable));
    FAIL_RETURN(SetSelectedDateVerticalBar(drawable));
    return NOERROR;
}

/**
 * Sets the drawable for the vertical bar shown at the beginning and at
 * the end of the selected date.
 *
 * @param drawable The vertical bar drawable.
 *
 * @attr ref android.R.styleable#CalendarView_selectedDateVerticalBar
 */
ECode CalendarView::SetSelectedDateVerticalBar(
    /* [in] */ IDrawable* drawable)
{
    if (mSelectedDateVerticalBar.Get() != drawable) {
        mSelectedDateVerticalBar = drawable;
        Int32 childCount ;
        mListView->GetChildCount(&childCount);
        for (Int32 i = 0; i < childCount; i++) {
            AutoPtr<IView> v;
            mListView->GetChildAt(i, (IView**)&v);
            AutoPtr<_WeekView> weekView = (_WeekView*)(WeekView*)v.Get();
            if (weekView->mHasSelectedDay) {
                weekView->Invalidate();
            }
        }
    }
    return NOERROR;
}

/**
 * Gets the drawable for the vertical bar shown at the beginning and at
 * the end of the selected date.
 *
 * @return The vertical bar drawable.
 */
AutoPtr<IDrawable> CalendarView::GetSelectedDateVerticalBar()
{
    return mSelectedDateVerticalBar;
}

/**
 * Sets the text appearance for the week day abbreviation of the calendar header.
 *
 * @param resourceId The text appearance resource id.
 *
 * @attr ref android.R.styleable#CalendarView_weekDayTextAppearance
 */
ECode CalendarView::SetWeekDayTextAppearance(
    /* [in] */ Int32 resourceId)
{
    if (mWeekDayTextAppearanceResId != resourceId)
    {
        mWeekDayTextAppearanceResId = resourceId;
        SetUpHeader();
    }
    return NOERROR;
}

/**
 * Gets the text appearance for the week day abbreviation of the calendar header.
 *
 * @return The text appearance resource id.
 *
 * @attr ref android.R.styleable#CalendarView_weekDayTextAppearance
 */
Int32 CalendarView::GetWeekDayTextAppearance()
{
    return mWeekDayTextAppearanceResId;
}

/**
 * Sets the text appearance for the calendar dates.
 *
 * @param resourceId The text appearance resource id.
 *
 * @attr ref android.R.styleable#CalendarView_dateTextAppearance
 */
ECode CalendarView::SetDateTextAppearance(
    /* [in] */ Int32 resourceId)
{
    if (mDateTextAppearanceResId != resourceId)
    {
        mDateTextAppearanceResId = resourceId;
        UpdateDateTextSize();
        InvalidateAllWeekViews();
    }
    return NOERROR;
}

/**
 * Gets the text appearance for the calendar dates.
 *
 * @return The text appearance resource id.
 *
 * @attr ref android.R.styleable#CalendarView_dateTextAppearance
 */
Int32 CalendarView::GetDateTextAppearance() {
    return mDateTextAppearanceResId;
}

//@Override
ECode CalendarView::SetEnabled(
    /* [in] */ Boolean enabled)
{
    return mListView->SetEnabled(enabled);
}

//@Override
Boolean CalendarView::IsEnabled() {
    Boolean enable;
    mListView->IsEnabled(&enable);
    return enable;
}

//@Override
void CalendarView::OnConfigurationChanged(
    /* [in] */ IConfiguration* newConfig)
{
    FrameLayout::OnConfigurationChanged(newConfig);
    AutoPtr<ILocale> locale;
    newConfig->GetLocale((ILocale**)&locale);
    SetCurrentLocale(locale);
}

//@Override
ECode CalendarView::OnInitializeAccessibilityEvent(
    /* [in] */ IAccessibilityEvent* event)
{
    FAIL_RETURN(FrameLayout::OnInitializeAccessibilityEvent(event));
    String classNameStr("CalendarView");
    AutoPtr<ICharSequence> className;
    FAIL_RETURN(CStringWrapper::New(classNameStr, (ICharSequence**)&className));
    return event->SetClassName(className);
}

//@Override
ECode CalendarView::OnInitializeAccessibilityNodeInfo(
    /* [in] */ IAccessibilityNodeInfo* info)
{
    FAIL_RETURN(FrameLayout::OnInitializeAccessibilityNodeInfo(info));
    String classNameStr("CalendarView");
    AutoPtr<ICharSequence> className;
    FAIL_RETURN(CStringWrapper::New(classNameStr, (ICharSequence**)&className));
    return info->SetClassName(className);
}

/**
 * Gets the minimal date supported by this {@link CalendarView} in milliseconds
 * since January 1, 1970 00:00:00 in {@link TimeZone#getDefault()} time
 * zone.
 * <p>
 * Note: The default minimal date is 01/01/1900.
 * <p>
 *
 * @return The minimal supported date.
 *
 * @attr ref android.R.styleable#CalendarView_minDate
 */
Int64 CalendarView::GetMinDate()
{
    Int64 millis;
    mMinDate->GetTimeInMillis(&millis);
    return millis;
}

/**
 * Sets the minimal date supported by this {@link CalendarView} in milliseconds
 * since January 1, 1970 00:00:00 in {@link TimeZone#getDefault()} time
 * zone.
 *
 * @param minDate The minimal supported date.
 *
 * @attr ref android.R.styleable#CalendarView_minDate
 */
ECode CalendarView::SetMinDate(
    /* [in] */ Int64 minDate)
{
    mTempDate->SetTimeInMillis(minDate);
    if (IsSameDate(mTempDate, mMinDate))
    {
        return NOERROR;
    }
    mMinDate->SetTimeInMillis(minDate);
    // make sure the current date is not earlier than
    // the new min date since the latter is used for
    // calculating the indices in the adapter thus
    // avoiding out of bounds error
    //zhangjingcheng,TODO
    AutoPtr<ICalendar> date = mAdapter->mSelectedDate;

    Boolean before;
    if (date->IsBefore(mMinDate, &before), before) {
        mAdapter->SetSelectedDay(mMinDate);
    }
    // reinitialize the adapter since its range depends on min date
    mAdapter->Init();
    if (date->IsBefore(mMinDate, &before), before) {
        Int64 mills;
        mTempDate->GetTimeInMillis(&mills);
        SetDate(mills);
    } else {
        // we go to the current date to force the ListView to query its
        // adapter for the shown views since we have changed the adapter
        // range and the base from which the later calculates item indices
        // note that calling setDate will not work since the date is the same
        FAIL_RETURN(GoTo(date, FALSE, TRUE, FALSE));
    }
    return NOERROR;
}

/**
 * Gets the maximal date supported by this {@link CalendarView} in milliseconds
 * since January 1, 1970 00:00:00 in {@link TimeZone#getDefault()} time
 * zone.
 * <p>
 * Note: The default maximal date is 01/01/2100.
 * <p>
 *
 * @return The maximal supported date.
 *
 * @attr ref android.R.styleable#CalendarView_maxDate
 */
Int64 CalendarView::GetMaxDate()
{
    Int64 millis;
    mMaxDate->GetTimeInMillis(&millis);
    return millis;
}

/**
 * Sets the maximal date supported by this {@link CalendarView} in milliseconds
 * since January 1, 1970 00:00:00 in {@link TimeZone#getDefault()} time
 * zone.
 *
 * @param maxDate The maximal supported date.
 *
 * @attr ref android.R.styleable#CalendarView_maxDate
 */
ECode CalendarView::SetMaxDate(
    /* [in] */ Int64 maxDate)
{
    mTempDate->SetTimeInMillis(maxDate);
    if (IsSameDate(mTempDate, mMaxDate))
    {
        return NOERROR;
    }
    mMaxDate->SetTimeInMillis(maxDate);
    // reinitialize the adapter since its range depends on max date
    mAdapter->Init();
    //zhangjingcheng,TODO
    AutoPtr<ICalendar> date = mAdapter->mSelectedDate;
    Boolean after;
    if (date->IsAfter(mMaxDate, &after), after)
    {
        Int64 millis;
        mMaxDate->GetTimeInMillis(&millis);
        SetDate(millis);
    } else {
        // we go to the current date to force the ListView to query its
        // adapter for the shown views since we have changed the adapter
        // range and the base from which the later calculates item indices
        // note that calling setDate will not work since the date is the same
        FAIL_RETURN(GoTo(date, FALSE, TRUE, FALSE));
    }
    return NOERROR;
}

/**
 * Sets whether to show the week number.
 *
 * @param showWeekNumber True to show the week number.
 *
 * @attr ref android.R.styleable#CalendarView_showWeekNumber
 */
ECode CalendarView::SetShowWeekNumber(
    /* [in] */ Boolean showWeekNumber)
{
    if (mShowWeekNumber == showWeekNumber) {
        return NOERROR;
    }
    mShowWeekNumber = showWeekNumber;
    FAIL_RETURN(mAdapter->NotifyDataSetChanged());
    FAIL_RETURN(SetUpHeader());
    return NOERROR;
}

/**
 * Gets whether to show the week number.
 *
 * @return True if showing the week number.
 *
 * @attr ref android.R.styleable#CalendarView_showWeekNumber
 */
Boolean CalendarView::GetShowWeekNumber()
{
    return mShowWeekNumber;
}

/**
 * Gets the first day of week.
 *
 * @return The first day of the week conforming to the {@link CalendarView}
 *         APIs.
 * @see Calendar#MONDAY
 * @see Calendar#TUESDAY
 * @see Calendar#WEDNESDAY
 * @see Calendar#THURSDAY
 * @see Calendar#FRIDAY
 * @see Calendar#SATURDAY
 * @see Calendar#SUNDAY
 *
 * @attr ref android.R.styleable#CalendarView_firstDayOfWeek
 */
Int32 CalendarView::GetFirstDayOfWeek()
{
    return mFirstDayOfWeek;
}

/**
 * Sets the first day of week.
 *
 * @param firstDayOfWeek The first day of the week conforming to the
 *            {@link CalendarView} APIs.
 * @see Calendar#MONDAY
 * @see Calendar#TUESDAY
 * @see Calendar#WEDNESDAY
 * @see Calendar#THURSDAY
 * @see Calendar#FRIDAY
 * @see Calendar#SATURDAY
 * @see Calendar#SUNDAY
 *
 * @attr ref android.R.styleable#CalendarView_firstDayOfWeek
 */
ECode CalendarView::SetFirstDayOfWeek(
    /* [in] */ Int32 firstDayOfWeek)
{
    if (mFirstDayOfWeek == firstDayOfWeek) {
        return NOERROR;
    }
    mFirstDayOfWeek = firstDayOfWeek;
    FAIL_RETURN(mAdapter->Init());
    FAIL_RETURN(mAdapter->NotifyDataSetChanged());
    FAIL_RETURN(SetUpHeader());
    return NOERROR;
}

/**
 * Sets the listener to be notified upon selected date change.
 *
 * @param listener The listener to be notified.
 */
ECode CalendarView::SetOnDateChangeListener(
    /* [in] */ IOnDateChangeListener* listener)
{
    mOnDateChangeListener = listener;
    return NOERROR;
}

/**
 * Gets the selected date in milliseconds since January 1, 1970 00:00:00 in
 * {@link TimeZone#getDefault()} time zone.
 *
 * @return The selected date.
 */
Int64 CalendarView::GetDate()
{
    Int64 millis;
    mAdapter->mSelectedDate->GetTimeInMillis(&millis);
    return millis;
}

/**
 * Sets the selected date in milliseconds since January 1, 1970 00:00:00 in
 * {@link TimeZone#getDefault()} time zone.
 *
 * @param date The selected date.
 *
 * @throws IllegalArgumentException of the provided date is before the
 *        minimal or after the maximal date.
 *
 * @see #setDate(long, boolean, boolean)
 * @see #setMinDate(long)
 * @see #setMaxDate(long)
 */
ECode CalendarView::SetDate(
    /* [in] */ Int64 date)
{
    return SetDate(date, FALSE, FALSE);
}

/**
 * Sets the selected date in milliseconds since January 1, 1970 00:00:00 in
 * {@link TimeZone#getDefault()} time zone.
 *
 * @param date The date.
 * @param animate Whether to animate the scroll to the current date.
 * @param center Whether to center the current date even if it is already visible.
 *
 * @throws IllegalArgumentException of the provided date is before the
 *        minimal or after the maximal date.
 *
 * @see #setMinDate(long)
 * @see #setMaxDate(long)
 */
ECode CalendarView::SetDate(
     /* [in] */ Int64 date,
     /* [in] */ Boolean animate,
     /* [in] */ Boolean center)
{
    mTempDate->SetTimeInMillis(date);
    if (IsSameDate(mTempDate, mAdapter->mSelectedDate)) {
        return NOERROR;
    }
    Int32 flags = IDateUtils::FORMAT_SHOW_DATE | IDateUtils::FORMAT_NO_MONTH_DAY
            | IDateUtils::FORMAT_SHOW_YEAR;
    Int64 millis;
    mTempDate->GetTimeInMillis(&millis);
    String newMonthName = DateUtils::FormatDateRange(mContext, millis, millis, flags);
    AutoPtr<ICharSequence> t;
    CStringWrapper::New(newMonthName, (ICharSequence**)&t);
    mMonthName->SetText(t);
    return GoTo(mTempDate, animate, TRUE, center);
}

ECode CalendarView::InvalidateAllWeekViews()
{
    Int32 childCount;
    mListView->GetChildCount(&childCount);
    for (Int32 i = 0; i < childCount; i++) {
        AutoPtr<IView> view;
        mListView->GetChildAt(i, (IView**)&view);
        view->Invalidate();
    }
    return NOERROR;
}

ECode CalendarView::UpdateDateTextSize()
{
    AutoPtr<ArrayOf<Int32> > attrId = ArrayOf<Int32>::Alloc(
                    const_cast<Int32*>(R::styleable::TextAppearance),
                    ARRAY_SIZE(R::styleable::TextAppearance));

    AutoPtr<ITypedArray> dateTextAppearance;
    GetContext()->ObtainStyledAttributes(mDateTextAppearanceResId, attrId, (ITypedArray**)&dateTextAppearance);
    dateTextAppearance->GetDimensionPixelSize(
            R::styleable::TextAppearance_textSize, DEFAULT_DATE_TEXT_SIZE, &mDateTextSize);
    dateTextAppearance->Recycle();
    return NOERROR;
}


/**
 * Sets the current locale.
 *
 * @param locale The current locale.
 */
ECode CalendarView::SetCurrentLocale(
    /* [in] */ ILocale* locale)
{
    Boolean equals;
    if (locale->Equals(mCurrentLocale, &equals), equals) {
        return NOERROR;
    }

    mCurrentLocale = locale;

    mTempDate = GetCalendarForLocale(mTempDate, locale);
    mFirstDayOfMonth = GetCalendarForLocale(mFirstDayOfMonth, locale);
    mMinDate = GetCalendarForLocale(mMinDate, locale);
    mMaxDate = GetCalendarForLocale(mMaxDate, locale);
    return NOERROR;
}

/**
 * Gets a calendar for locale bootstrapped with the value of a given calendar.
 *
 * @param oldCalendar The old calendar.
 * @param locale The locale.
 */
AutoPtr<ICalendar> CalendarView::GetCalendarForLocale(
    /* [in] */ ICalendar* oldCalendar,
    /* [in] */ ILocale* locale)
{
    AutoPtr<ICalendarHelper> helper;
    CCalendarHelper::AcquireSingleton((ICalendarHelper**)&helper);

    if (oldCalendar == NULL) {
        AutoPtr<ICalendar> instance;
        helper->GetInstance(locale, (ICalendar**)&instance);
        return instance;
    } else {
        Int64 currentTimeMillis;
        oldCalendar->GetTimeInMillis(&currentTimeMillis);
        AutoPtr<ICalendar> newCalendar;
        helper->GetInstance(locale, (ICalendar**)&newCalendar);
        newCalendar->SetTimeInMillis(currentTimeMillis);
        return newCalendar;
    }
}

/**
 * @return True if the <code>firstDate</code> is the same as the <code>
 * secondDate</code>.
 */
Boolean CalendarView::IsSameDate(
    /* [in] */ ICalendar* firstDate,
    /* [in] */ ICalendar* secondDate)
{
    Int32 fDay, sDay, fYear, sYear;
    firstDate->Get(ICalendar::DAY_OF_YEAR, &fDay);
    secondDate->Get(ICalendar::DAY_OF_YEAR, &sDay);
    firstDate->Get(ICalendar::YEAR, &fYear);
    secondDate->Get(ICalendar::YEAR, &sYear);
    return ((fDay == sDay) && (fYear == sYear));
}

/**
 * Creates a new adapter if necessary and sets up its parameters.
 */
ECode CalendarView::SetUpAdapter()
{
    if (mAdapter == NULL) {
        mAdapter = new WeeksAdapter(GetContext(), this);
        AutoPtr<SetUpAdapterDataSetObserver> observer = new SetUpAdapterDataSetObserver(this);
        mAdapter->RegisterDataSetObserver(observer);
        mListView->SetAdapter((IAdapter*)mAdapter->Probe(EIID_IAdapter));
    }
    // refresh the view with the new parameters
    mAdapter->NotifyDataSetChanged();
    return NOERROR;
}

/**
 * Sets up the strings to be used by the header.
 */
ECode CalendarView::SetUpHeader()
{
    mDayLabels = ArrayOf<String>::Alloc(mDaysPerWeek);
    for (Int32 i = mFirstDayOfWeek, count = mFirstDayOfWeek + mDaysPerWeek; i < count; i++) {
        Int32 calendarDay = (i > ICalendar::SATURDAY) ? i - ICalendar::SATURDAY : i;
        (*mDayLabels)[i - mFirstDayOfWeek] = DateUtils::GetDayOfWeekString(calendarDay,
                IDateUtils::LENGTH_SHORTEST);
    }


    AutoPtr<ITextView> label;
    mDayNamesHeader->GetChildAt(0, (IView**)&label);

    if (mShowWeekNumber) {
        label->SetVisibility(IView::VISIBLE);
    } else {
        label->SetVisibility(IView::GONE);
    }

    Int32 count;
    mDayNamesHeader->GetChildCount(&count);
    for (Int32 i = 1; i < count; i++) {
        label = NULL;
        mDayNamesHeader->GetChildAt(i, (IView**)&label);
        if (mWeekDayTextAppearanceResId > -1) {
            label->SetTextAppearance(mContext, mWeekDayTextAppearanceResId);
        }
        if (i < mDaysPerWeek + 1) {
            AutoPtr<ICharSequence> t;
            FAIL_RETURN(CStringWrapper::New((*mDayLabels)[i - 1], (ICharSequence**)&t));
            label->SetText(t);
            label->SetVisibility(IView::VISIBLE);
        } else {
            label->SetVisibility(IView::GONE);
        }
    }
    mDayNamesHeader->Invalidate();
    return NOERROR;
}

/**
 * Sets all the required fields for the list view.
 */
ECode CalendarView::SetUpListView()
{
    // Configure the listview
    mListView->SetDivider(NULL);
    mListView->SetItemsCanFocus(TRUE);
    mListView->SetVerticalScrollBarEnabled(FALSE);
    AutoPtr<SetUpListViewAbsListViewOnScrollListener> l = new SetUpListViewAbsListViewOnScrollListener(this);
    mListView->SetOnScrollListener(l);
    // Make the scrolling behavior nicer
    mListView->SetFriction(mFriction);
    mListView->SetVelocityScale(mVelocityScale);
    return NOERROR;
}

/**
 * This moves to the specified time in the view. If the time is not already
 * in range it will move the list so that the first of the month containing
 * the time is at the top of the view. If the new time is already in view
 * the list will not be scrolled unless forceScroll is true. This time may
 * optionally be highlighted as selected as well.
 *
 * @param date The time to move to.
 * @param animate Whether to scroll to the given time or just redraw at the
 *            new location.
 * @param setSelected Whether to set the given time as selected.
 * @param forceScroll Whether to recenter even if the time is already
 *            visible.
 *
 * @throws IllegalArgumentException of the provided date is before the
 *        range start of after the range end.
 */
ECode CalendarView::GoTo(
    /* [in] */ ICalendar* date,
    /* [in] */ Boolean animate,
    /* [in] */ Boolean setSelected,
    /* [in] */ Boolean forceScroll)
{
    Boolean before, after;
    if ((date->IsBefore(mMinDate, &before), before) || (date->IsAfter(mMaxDate, &after), after)) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    // Find the first and last entirely visible weeks
    Int32 firstFullyVisiblePosition;
    mListView->GetFirstVisiblePosition(&firstFullyVisiblePosition);
    AutoPtr<IView> firstChild;
    mListView->GetChildAt(0, (IView**)&firstChild);
    if (firstChild != NULL)
    {
        Int32 top;
        firstChild->GetTop(&top);
        if (top < 0) {
            firstFullyVisiblePosition++;
        }
    }
    Int32 lastFullyVisiblePosition = firstFullyVisiblePosition + mShownWeekCount - 1;
    if (firstChild != NULL)
    {
        Int32 top;
        firstChild->GetTop(&top);
        if (top > mBottomBuffer) {
            lastFullyVisiblePosition--;
        }
    }
    if (setSelected) {
        mAdapter->SetSelectedDay(date);
    }
    // Get the week we're going to
    Int32 position = GetWeeksSinceMinDate(date);
    // Check if the selected day is now outside of our visible range
    // and if so scroll to the month that contains it
    if (position < firstFullyVisiblePosition || position > lastFullyVisiblePosition || forceScroll)
    {
        Int64 millis;
        date->GetTimeInMillis(&millis);
        mFirstDayOfMonth->SetTimeInMillis(millis);
        mFirstDayOfMonth->Set(ICalendar::DAY_OF_MONTH, 1);
        SetMonthDisplayed(mFirstDayOfMonth);
        // the earliest time we can scroll to is the min date
        if (mFirstDayOfMonth->IsBefore(mMinDate, &before), before)
        {
            position = 0;
        }
        else {
            position = GetWeeksSinceMinDate(mFirstDayOfMonth);
        }
        mPreviousScrollState = IAbsListViewOnScrollListener::SCROLL_STATE_FLING;
        if (animate) {
            mListView->SmoothScrollToPositionFromTop(position, mListScrollTopOffset,
                    GOTO_SCROLL_DURATION);
        } else {
            mListView->SetSelectionFromTop(position, mListScrollTopOffset);
            // Perform any after scroll operations that are needed
            OnScrollStateChanged(mListView, IAbsListViewOnScrollListener::SCROLL_STATE_IDLE);
        }
    } else if (setSelected) {
        // Otherwise just set the selection
        SetMonthDisplayed(date);
    }
    return NOERROR;
}

/**
 * Parses the given <code>date</code> and in case of success sets
 * the result to the <code>outDate</code>.
 *
 * @return True if the date was parsed.
 */
Boolean CalendarView::ParseDate(
    /* [in] */ const String& date,
    /* [in] */ ICalendar* outDate)
{
    AutoPtr<IDate> d;
    mDateFormat->Parse(date, (IDate**)&d);
    ECode e = outDate->SetTime(d);
    if(FAILED(e))
    {
//            Log.w(TAG, String("Date: ") + date + String(" not in format: ") + DATE_FORMAT);
        return FALSE;
    }else {
        return TRUE;
    }
}

/**
 * Called when a <code>view</code> transitions to a new <code>scrollState
 * </code>.
 */
ECode CalendarView::OnScrollStateChanged(
    /* [in] */ IAbsListView* view,
    /* [in] */ Int32 scrollState)
{
    return mScrollStateChangedRunnable->DoScrollStateChange(view, scrollState);
}

/**
 * Updates the title and selected month if the <code>view</code> has moved to a new
 * month.
 */
ECode CalendarView::OnScroll(
    /* [in] */ IAbsListView* view,
    /* [in] */ Int32 firstVisibleItem,
    /* [in] */ Int32 visibleItemCount,
    /* [in] */ Int32 totalItemCount)
{
    AutoPtr<IView> v;
    view->GetChildAt(0, (IView**)&v);
    AutoPtr<_WeekView> child = (_WeekView*)(WeekView*)v.Get();

    if (child == NULL) {
        return NOERROR;
    }

    // Figure out where we are
    Int32 position;
    view->GetFirstVisiblePosition(&position);
    Int64 currScroll = position * child->GetHeight() - child->GetBottom();

    // If we have moved since our last call update the direction
    if (currScroll < mPreviousScrollPosition) {
        mIsScrollingUp = TRUE;
    } else if (currScroll > mPreviousScrollPosition) {
        mIsScrollingUp = FALSE;
    } else {
        return NOERROR;
    }

    // Use some hysteresis for checking which month to highlight. This
    // causes the month to transition when two full weeks of a month are
    // visible when scrolling up, and when the first day in a month reaches
    // the top of the screen when scrolling down.
    Int32 offset = child->GetBottom() < mWeekMinVisibleHeight ? 1 : 0;
    AutoPtr<IView> vChild;
    if (mIsScrollingUp) {
        view->GetChildAt(SCROLL_HYST_WEEKS + offset, (IView**)&vChild);
        child = (_WeekView*)(WeekView*)vChild.Get();
    } else if (offset != 0) {
        view->GetChildAt(offset, (IView**)&vChild);
        child = (_WeekView*)(WeekView*)vChild.Get();
    }

    // Find out which month we're moving into
    Int32 month;
    if (mIsScrollingUp) {
        month = child->GetMonthOfFirstWeekDay();
    } else {
        month = child->GetMonthOfLastWeekDay();
    }

    // And how it relates to our current highlighted month
    Int32 monthDiff;
    if (mCurrentMonthDisplayed == 11 && month == 0) {
        monthDiff = 1;
    } else if (mCurrentMonthDisplayed == 0 && month == 11) {
        monthDiff = -1;
    } else {
        monthDiff = month - mCurrentMonthDisplayed;
    }

    // Only switch months if we're scrolling away from the currently
    // selected month
    if ((!mIsScrollingUp && monthDiff > 0) || (mIsScrollingUp && monthDiff < 0)) {
        AutoPtr<ICalendar> firstDay;
        firstDay = child->GetFirstDay();
        if (mIsScrollingUp) {
            firstDay->Add(ICalendar::DAY_OF_MONTH, -DAYS_PER_WEEK);
        } else {
            firstDay->Add(ICalendar::DAY_OF_MONTH, DAYS_PER_WEEK);
        }
        SetMonthDisplayed(firstDay);
    }
    mPreviousScrollPosition = currScroll;
    mPreviousScrollState = mCurrentScrollState;
    return NOERROR;
}

/**
 * Sets the month displayed at the top of this view based on time. Override
 * to add custom events when the title is changed.
 *
 * @param calendar A day in the new focus month.
 */
ECode CalendarView::SetMonthDisplayed(
    /* [in] */ ICalendar* calendar)
{
    Int32 newMonthDisplayed;
    calendar->Get(ICalendar::MONTH, &newMonthDisplayed);

    if (mCurrentMonthDisplayed != newMonthDisplayed) {
        mCurrentMonthDisplayed = newMonthDisplayed;
        mAdapter->SetFocusMonth(mCurrentMonthDisplayed);
        Int32 flags = IDateUtils::FORMAT_SHOW_DATE | IDateUtils::FORMAT_NO_MONTH_DAY
                | IDateUtils::FORMAT_SHOW_YEAR;
        Int64 millis;
        calendar->GetTimeInMillis(&millis);
        String newMonthName = DateUtils::FormatDateRange(mContext, millis, millis, flags);
        AutoPtr<ICharSequence> text;
        //TODO:
        //CStringWrapper::New(newMonthName, (ICharSequence**)&text);
        CStringWrapper::New(String("Month::Year"), (ICharSequence**)&text);

        mMonthName->SetText(text);
        mMonthName->Invalidate();
    }
    return NOERROR;
}

/**
 * @return Returns the number of weeks between the current <code>date</code>
 *         and the <code>mMinDate</code>.
 */
Int32 CalendarView::GetWeeksSinceMinDate(
    /* [in] */ ICalendar* date)
{
    Boolean before;
    if (date->IsBefore(mMinDate, &before), before) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    Int64 endTimeMillis;
    date->GetTimeInMillis(&endTimeMillis);
    AutoPtr<ITimeZone> timeZoneForDate;
    date->GetTimeZone((ITimeZone**)&timeZoneForDate);
    Int32 offset;
    timeZoneForDate->GetOffset(endTimeMillis, &offset);
    endTimeMillis += offset;

    Int64 startTimeMillis;
    mMinDate->GetTimeInMillis(&startTimeMillis);
    AutoPtr<ITimeZone> timeZoneForMinDate;
    mMinDate->GetTimeZone((ITimeZone**)&timeZoneForMinDate);
    timeZoneForMinDate->GetOffset(startTimeMillis, &offset);
    startTimeMillis += offset;

    Int32 dayOfWeek;
    mMinDate->Get(ICalendar::DAY_OF_WEEK, &dayOfWeek);
    Int64 dayOffsetMillis;
    dayOffsetMillis = (dayOfWeek - mFirstDayOfWeek)
            * MILLIS_IN_DAY;
    return (Int32) ((endTimeMillis - startTimeMillis + dayOffsetMillis) / MILLIS_IN_WEEK);
}

//==============================================================================
//                     CalendarView::CalendarGestureListener
//==============================================================================
CalendarView::CalendarGestureListener::CalendarGestureListener()
{

}

ECode CalendarView::CalendarGestureListener::OnSingleTapUp(
    /* [in] */ IMotionEvent* e,
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);
    *res = TRUE;
    return NOERROR;
}

}// namespace Widget
}// namespace Droid
}// namespace Elastos
