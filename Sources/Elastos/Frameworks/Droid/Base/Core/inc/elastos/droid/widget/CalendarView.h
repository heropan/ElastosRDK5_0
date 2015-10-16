#ifndef __ELASTOS_DROID_WIDGET_CALENDARVIEW_H__
#define __ELASTOS_DROID_WIDGET_CALENDARVIEW_H__

#include "FrameLayout.h"
#include "BaseAdapter.h"

#include "elastos/droid/view/SimpleOnGestureListener.h"
#include "elastos/droid/os/Runnable.h"

using Libcore::ICU::ILocale;
using Libcore::ICU::ILocaleHelper;
using Libcore::ICU::CLocaleHelper;
using Libcore::ICU::ILocaleData;
using Libcore::ICU::ILocaleDataHelper;
using Libcore::ICU::CLocaleDataHelper;
using Elastos::Core::IInteger32;
using Elastos::Core::CInteger32;
using Elastos::Core::IRunnable;
using Elastos::Core::EIID_IRunnable;
using Elastos::Droid::Database::IDataSetObserver;
using Elastos::Droid::Database::EIID_IDataSetObserver;
using Elastos::Text::IDateFormat;
using Elastos::Utility::ICalendar;
using Elastos::Utility::IDate;
using Elastos::Utility::ICalendarHelper;
using Elastos::Utility::CCalendarHelper;
using Elastos::Utility::ITimeZone;
using Elastos::Core::CString;
using Elastos::Droid::Os::Runnable;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Utility::IAttributeSet;
using Elastos::Droid::Graphics::Drawable::IDrawable;
using Elastos::Droid::View::View;
using Elastos::Droid::View::IViewOnTouchListener;
using Elastos::Droid::View::EIID_IViewOnTouchListener;
using Elastos::Droid::View::IView;
using Elastos::Droid::View::EIID_IView;
using Elastos::Droid::View::IMotionEvent;
using Elastos::Droid::View::IGestureDetector;
using Elastos::Droid::Graphics::Drawable::IDrawableCallback;
using Elastos::Droid::Graphics::Drawable::EIID_IDrawableCallback;
using Elastos::Droid::View::IKeyEventCallback;
using Elastos::Droid::View::EIID_IKeyEventCallback;
using Elastos::Droid::View::Accessibility::IAccessibilityEventSource;
using Elastos::Droid::View::Accessibility::EIID_IAccessibilityEventSource;
using Elastos::Droid::View::Accessibility::IAccessibilityEvent;
using Elastos::Droid::Content::Res::IConfiguration;
using Elastos::Droid::Database::IDataSetObserver;
using Elastos::Core::CString;

namespace Elastos {
namespace Droid {
namespace Widget {

class CalendarView : public FrameLayout
{
public:
    CalendarView();

    CalendarView(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs = NULL,
        /* [in] */ Int32 defStyle = 0);

    virtual CARAPI SetShownWeekCount(
        /* [in] */ Int32 count);

    virtual CARAPI_(Int32) GetShownWeekCount();

    virtual CARAPI SetSelectedWeekBackgroundColor(
        /* [in] */ Int32 color);

    virtual CARAPI_(Int32) GetSelectedWeekBackgroundColor();

    virtual CARAPI SetFocusedMonthDateColor(
        /* [in] */ Int32 color);

    virtual CARAPI_(Int32) GetFocusedMonthDateColor();

    virtual CARAPI SetUnfocusedMonthDateColor(
        /* [in] */ Int32 color);

    virtual CARAPI_(Int32) GetUnfocusedMonthDateColor();

    virtual CARAPI SetWeekNumberColor(
        /* [in] */ Int32 color);

    virtual CARAPI_(Int32) GetWeekNumberColor();

    virtual CARAPI SetWeekSeparatorLineColor(
        /* [in] */ Int32 color);

    virtual CARAPI_(Int32) GetWeekSeparatorLineColor();

    virtual CARAPI SetSelectedDateVerticalBar(
        /* [in] */ Int32 resourceId);

    virtual CARAPI SetSelectedDateVerticalBar(
        /* [in] */ IDrawable* drawable);

    virtual CARAPI_(AutoPtr<IDrawable>) GetSelectedDateVerticalBar();

    virtual CARAPI SetWeekDayTextAppearance(
        /* [in] */ Int32 resourceId);

    virtual CARAPI_(Int32) GetWeekDayTextAppearance();

    virtual CARAPI SetDateTextAppearance(
            /* [in] */ Int32 resourceId);

    virtual CARAPI_(Int32) GetDateTextAppearance();

    virtual CARAPI SetEnabled(
        /* [in] */ Boolean enabled);

    virtual CARAPI_(Boolean) IsEnabled();

    virtual CARAPI OnInitializeAccessibilityEvent(
        /* [in] */ IAccessibilityEvent* event);

    virtual CARAPI OnInitializeAccessibilityNodeInfo(
        /* [in] */ IAccessibilityNodeInfo* info);

    virtual CARAPI_(Int64) GetMinDate();

    virtual CARAPI SetMinDate(
        /* [in] */ Int64 minDate);

    virtual CARAPI_(Int64) GetMaxDate();

    virtual CARAPI SetMaxDate(
            /* [in] */ Int64 maxDate);

    virtual CARAPI SetShowWeekNumber(
        /* [in] */ Boolean showWeekNumber);

    virtual CARAPI_(Boolean) GetShowWeekNumber();

    virtual CARAPI_(Int32) GetFirstDayOfWeek();

    virtual CARAPI SetFirstDayOfWeek(
        /* [in] */ Int32 firstDayOfWeek);

    virtual CARAPI SetOnDateChangeListener(
        /* [in] */ IOnDateChangeListener* listener);

    virtual CARAPI_(Int64) GetDate();

    virtual CARAPI SetDate(
        /* [in] */ Int64 date);

    virtual CARAPI SetDate(
        /* [in] */ Int64 date,
        /* [in] */ Boolean animate,
        /* [in] */ Boolean center);

protected:

    virtual CARAPI_(void) OnConfigurationChanged(
        /* [in] */ IConfiguration* newConfig);

    CARAPI Init(
        /* [in] */ IContext* context);

    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle);


private:
    CARAPI UpdateDateTextSize();

    CARAPI InvalidateAllWeekViews();

    CARAPI SetCurrentLocale(
        /* [in] */ ILocale* locale);

    CARAPI_(AutoPtr<ICalendar>) GetCalendarForLocale(
        /* [in] */ ICalendar* oldCalendar,
        /* [in] */ ILocale* locale);

    CARAPI_(Boolean) IsSameDate(
        /* [in] */ ICalendar* firstDate,
        /* [in] */ ICalendar* secondDate);

    CARAPI SetUpAdapter();

    CARAPI SetUpHeader();

    CARAPI SetUpListView();

    CARAPI GoTo(
        /* [in] */ ICalendar* date,
        /* [in] */ Boolean animate,
        /* [in] */ Boolean setSelected,
        /* [in] */ Boolean forceScroll);

    CARAPI_(Boolean) ParseDate(
        /* [in] */ const String& date,
        /* [in] */ ICalendar* outDate);

    CARAPI OnScrollStateChanged(
        /* [in] */ IAbsListView* view,
        /* [in] */ Int32 scrollState);

    CARAPI OnScroll(
        /* [in] */ IAbsListView* view,
        /* [in] */ Int32 firstVisibleItem,
        /* [in] */ Int32 visibleItemCount,
        /* [in] */ Int32 totalItemCount);

    CARAPI SetMonthDisplayed(
        /* [in] */ ICalendar* calendar);

    CARAPI_(Int32) GetWeeksSinceMinDate(
        /* [in] */ ICalendar* date);

    CARAPI InitFromSource(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle);

private:

    class ScrollStateRunnable
        : public Runnable
    {
    public:

        ScrollStateRunnable(
            /* [in] */ ICalendarView* host);

        CARAPI DoScrollStateChange(
            /* [in] */ IAbsListView* view,
            /* [in] */ Int32 scrollState);

        CARAPI Run();

    private:

        IAbsListView* mView;

        Int32 mNewState;
    protected:
        AutoPtr<IWeakReference> mWeakHost;
    };

    class _WeeksAdapter
        : public BaseAdapter
        , public ElRefBase
    {
    public:
        _WeeksAdapter(
            /* [in] */ IContext* context,
            /* [in] */ CalendarView* host);

        CARAPI SetSelectedDay(
            /* [in] */ ICalendar* selectedDay);

        CARAPI_(AutoPtr<ICalendar>) GetSelectedDay();

        virtual CARAPI_(Int32) GetCount();

        virtual CARAPI_(AutoPtr<IInterface>) GetItem(
            /* [in] */ Int32 position);

        virtual CARAPI_(Int64) GetItemId(
            /* [in] */ Int32 position);

        virtual CARAPI_(AutoPtr<IView>) GetView(
            /* [in] */ Int32 position,
            /* [in] */ IView* convertView,
            /* [in] */ IViewGroup* parent);

        CARAPI SetFocusMonth(
            /* [in] */ Int32 month);

        virtual CARAPI OnTouch(
            /* [in] */ IView* v,
            /* [in] */ IMotionEvent* event,
            /* [out] */ Boolean* result);

    private:

        CARAPI Init();

        CARAPI OnDateTapped(
            /* [in] */ ICalendar* day);

    protected:

        CalendarView* mHost;

    private:

        Int32 mSelectedWeek;

        AutoPtr<IGestureDetector> mGestureDetector;

        Int32 mFocusedMonth;

        AutoPtr<ICalendar> mSelectedDate;

        Int32 mTotalWeekCount;

        friend class CalendarView;
    };

    class WeeksAdapter
        : public _WeeksAdapter
        , public IViewOnTouchListener
        , public IBaseAdapter
        , public ISpinnerAdapter
    {
    public:

        WeeksAdapter(
            /* [in] */ IContext* context,
            /* [in] */ CalendarView* host);

        CARAPI_(PInterface) Probe(
            /* [in] */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        CARAPI AreAllItemsEnabled(
            /* [out] */ Boolean* enabled);

        CARAPI IsEnabled(
            /* [in] */ Int32 position,
            /* [out] */ Boolean* enabled);

        CARAPI OnTouch(
            /* [in] */ IView* v,
            /* [in] */ IMotionEvent* event,
            /* [out] */ Boolean* result);

        CARAPI NotifyDataSetChanged();

        CARAPI NotifyDataSetInvalidated();

        CARAPI GetDropDownView(
            /* [in] */ Int32 position,
            /* [in] */ IView* convertView,
            /* [in] */ IViewGroup* parent,
            /* [out] */ IView** view);

        CARAPI RegisterDataSetObserver(
            /* [in] */ IDataSetObserver* observer);

        CARAPI UnregisterDataSetObserver(
            /* [in] */ IDataSetObserver* observer);

        CARAPI GetCount(
            /* [out] */ Int32* count);

        CARAPI GetItem(
            /* [in] */ Int32 position,
            /* [out] */ IInterface** item);

        CARAPI GetItemId(
            /* [in] */ Int32 position,
            /* [out] */ Int64* itemId);

        CARAPI HasStableIds(
            /* [out] */ Boolean* hasStableIds);

        CARAPI GetView(
            /* [in] */ Int32 position,
            /* [in] */ IView* convertView,
            /* [in] */ IViewGroup* parent,
            /* [out] */ IView** view);

        CARAPI GetItemViewType(
            /* [in] */ Int32 position,
            /* [out] */ Int32* viewType);

        CARAPI GetViewTypeCount(
            /* [out] */ Int32* count);

        CARAPI IsEmpty(
             /* [out] */ Boolean* isEmpty);
    };

    class _WeekView
        : public ElRefBase
        , public View
    {
    public:
        _WeekView(
            /* [in] */ IContext* context,
            /* [in] */ CalendarView* host);

        CARAPI Init(
            /* [in] */ Int32 weekNumber,
            /* [in] */ Int32 selectedWeekDay,
            /* [in] */ Int32 focusedMonth);

        CARAPI_(Int32) GetMonthOfFirstWeekDay();

        CARAPI_(Int32) GetMonthOfLastWeekDay();

        CARAPI_(AutoPtr<ICalendar>) GetFirstDay();

        CARAPI_(Boolean) GetDayFromLocation(
            /* [in] */ Float x,
            /* [in, out] */ ICalendar* outCalendar);

    protected:

        virtual CARAPI_(void) OnDraw(
            /* [in] */ ICanvas* canvas);

        virtual CARAPI_(void) OnSizeChanged(
            /* [in] */ Int32 w,
            /* [in] */ Int32 h,
            /* [in] */ Int32 oldw,
            /* [in] */ Int32 oldh);

        virtual CARAPI_(void) OnMeasure(
            /* [in] */ Int32 widthMeasureSpec,
            /* [in] */ Int32 heightMeasureSpec);

    private:

        CARAPI_(void) InitilaizePaints();

        CARAPI_(void) DrawBackground(
            /* [in] */ ICanvas* canvas);

        CARAPI_(void) DrawWeekNumbersAndDates(
            /* [in] */ ICanvas* canvas);

        CARAPI_(void) DrawWeekSeparators(
            /* [in] */ ICanvas* canvas);

        CARAPI_(void) DrawSelectedDateVerticalBars(
            /* [in] */ ICanvas* canvas);

        CARAPI_(void) UpdateSelectionPositions();

    protected:
        CalendarView* mHost;

    private:

        AutoPtr<IRect> mTempRect;
        AutoPtr<IPaint> mDrawPaint;
        AutoPtr<IPaint> mMonthNumDrawPaint;
        AutoPtr< ArrayOf<String> > mDayNumbers;
        AutoPtr< ArrayOf<Boolean> > mFocusDay;
        Boolean mHasFocusedDay;
        Boolean mHasUnfocusedDay;
        AutoPtr<ICalendar> mFirstDay;
        Int32 mMonthOfFirstWeekDay;
        Int32 mLastWeekDayMonth;
        Int32 mWeek;
        Int32 mWidth;
        Int32 mHeight;
        Boolean mHasSelectedDay;
        Int32 mSelectedDay;
        Int32 mNumCells;
        Int32 mSelectedLeft;
        Int32 mSelectedRight;

        friend class CalendarView;
    };

    class WeekView
        : public _WeekView
        , public IView
        , public IDrawableCallback
        , public IKeyEventCallback
        , public IAccessibilityEventSource
        , public IWeakReferenceSource
    {
    public:

        IVIEW_METHODS_DECL()
        IDRAWABLECALLBACK_METHODS_DECL()
        IKEYEVENTCALLBACK_METHODS_DECL()
        IACCESSIBILITYEVENTSOURCE_METHODS_DECL()

        WeekView(
            /* [in] */ IContext* context,
            /* [in] */ CalendarView* host);

        CARAPI_(PInterface) Probe(
            /* [in] */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        CARAPI GetWeakReference(
            /* [out] */ IWeakReference** weakReference);
    };

    class SetUpAdapterDataSetObserver
            : public IDataSetObserver
            , public ElRefBase
    {
    public:
        CAR_INTERFACE_DECL()

        SetUpAdapterDataSetObserver(
            /* [in] */ CalendarView* host);
        CARAPI OnChanged();

        CARAPI OnInvalidated();
    private:
        CalendarView* mHost;
    };

    class SetUpListViewAbsListViewOnScrollListener
            : public IAbsListViewOnScrollListener
            , public ElRefBase
    {
    public:
        CAR_INTERFACE_DECL()

        SetUpListViewAbsListViewOnScrollListener(
            /* [in] */ CalendarView* host);

        CARAPI OnScrollStateChanged(
            /* [in] */ IAbsListView* view,
            /* [in] */ Int32 scrollState);

        CARAPI OnScroll(
            /* [in] */ IAbsListView* view,
            /* [in] */ Int32 firstVisibleItem,
            /* [in] */ Int32 visibleItemCount,
            /* [in] */ Int32 totalItemCount);
    private:
        CalendarView* mHost;
    };

    class CalendarGestureListener
        : public Elastos::Droid::View::SimpleOnGestureListener
    {
    public:
        CalendarGestureListener();

        CARAPI OnSingleTapUp(
            /* [in] */ IMotionEvent* e,
            /* [out] */ Boolean* res);
    };

private:
    /**
      * Tag for logging.
      */

     static const String TAG;

     /**
      * Default value whether to show week number.
      */
     static const Boolean DEFAULT_SHOW_WEEK_NUMBER = TRUE;

     /**
      * The number of milliseconds in a day.e
      */
     static const Int64 MILLIS_IN_DAY = 86400000L;

     /**
      * The number of day in a week.
      */
     static const Int32 DAYS_PER_WEEK = 7;

     /**
      * The number of milliseconds in a week.
      */
     static const Int64 MILLIS_IN_WEEK = DAYS_PER_WEEK * MILLIS_IN_DAY;

     /**
      * Affects when the month selection will change while scrolling upe
      */
     static const Int32 SCROLL_HYST_WEEKS = 2;

     /**
      * How long the GoTo fling animation should last.
      */
     static const Int32 GOTO_SCROLL_DURATION = 1000;

     /**
      * The duration of the adjustment upon a user scroll in milliseconds.
      */
     static const Int32 ADJUSTMENT_SCROLL_DURATION = 500;

     /**
      * How long to wait after receiving an onScrollStateChanged notification
      * before acting on it.
      */
     static const Int32 SCROLL_CHANGE_DELAY = 40;

     /**
      * String for parsing dates.
      */
     static const String DATE_FORMAT;// = String("MM/dd/yyyy");

     /**
      * The default minimal date.
      */
     static const String DEFAULT_MIN_DATE;// = String("01/01/1900");

     /**
      * The default maximal date.
      */
     static const String DEFAULT_MAX_DATE;// = String("01/01/2100");

     static const Int32 DEFAULT_SHOWN_WEEK_COUNT = 6;

     static const Int32 DEFAULT_DATE_TEXT_SIZE = 14;

     static const Int32 UNSCALED_SELECTED_DATE_VERTICAL_BAR_WIDTH = 6;

     static const Int32 UNSCALED_WEEK_MIN_VISIBLE_HEIGHT = 12;

     static const Int32 UNSCALED_LIST_SCROLL_TOP_OFFSET = 2;

     static const Int32 UNSCALED_BOTTOM_BUFFER = 20;

     static const Int32 UNSCALED_WEEK_SEPARATOR_LINE_WIDTH = 1;

     static const Int32 DEFAULT_WEEK_DAY_TEXT_APPEARANCE_RES_ID = -1;

private:

     Int32 mWeekSeperatorLineWidth;

     Int32 mDateTextSize;

     AutoPtr<IDrawable> mSelectedDateVerticalBar;

     Int32 mSelectedDateVerticalBarWidth;

     Int32 mSelectedWeekBackgroundColor;

     Int32 mFocusedMonthDateColor;

     Int32 mUnfocusedMonthDateColor;

     Int32 mWeekSeparatorLineColor;

     Int32 mWeekNumberColor;

     Int32 mWeekDayTextAppearanceResId;

     Int32 mDateTextAppearanceResId;

     /**
      * The top offset of the weeks list.
      */
     Int32 mListScrollTopOffset;

     /**
      * The visible height of a week view.
      */
     Int32 mWeekMinVisibleHeight;

     /**
      * The visible height of a week view.
      */
     Int32 mBottomBuffer;

     /**
      * The number of shown weeks.
      */
     Int32 mShownWeekCount;

     /**
      * Flag whether to show the week number.
      */
     Boolean mShowWeekNumber;

     /**
      * The number of day per week to be shown.
      */
     Int32 mDaysPerWeek;

     /**
      * The friction of the week list while flinging.
      */
     Float mFriction;

     /**
      * Scale for adjusting velocity of the week list while flinging.
      */
     Float mVelocityScale;

     /**
      * The adapter for the weeks list.
      */
     AutoPtr<WeeksAdapter> mAdapter;

     /**
      * The weeks list.
      */
     AutoPtr<IListView> mListView;

     /**
      * The name of the month to display.
      */
     AutoPtr<ITextView> mMonthName;

     /**
      * The header with week day names.
      */
     AutoPtr<IViewGroup> mDayNamesHeader;

     /**
      * Cached labels for the week names header.
      */
     AutoPtr<ArrayOf<String> > mDayLabels;

     /**
      * The first day of the week.
      */
     Int32 mFirstDayOfWeek;

     /**
      * Which month should be displayed/highlighted [0-11].
      */
     Int32 mCurrentMonthDisplayed;

     /**
      * Used for tracking during a scroll.
      */
     Int64 mPreviousScrollPosition;

     /**
      * Used for tracking which direction the view is scrolling.
      */
     Boolean mIsScrollingUp;

     /**
      * The previous scroll state of the weeks ListView.
      */
     Int32 mPreviousScrollState;

     /**
      * The current scroll state of the weeks ListView.
      */
     Int32 mCurrentScrollState;

     /**
      * Listener for changes in the selected day.
      */
     AutoPtr<IOnDateChangeListener> mOnDateChangeListener;

     /**
      * Command for adjusting the position after a scroll/fling.
      */
     AutoPtr<ScrollStateRunnable> mScrollStateChangedRunnable;

     /**
      * Temporary instance to avoid multiple instantiations.
      */
     AutoPtr<ICalendar> mTempDate;

     /**
      * The first day of the focused month.
      */
     AutoPtr<ICalendar> mFirstDayOfMonth;

     /**
      * The start date of the range supported by this picker.
      */
     AutoPtr<ICalendar> mMinDate;

     /**
      * The end date of the range supported by this picker.
      */
     AutoPtr<ICalendar> mMaxDate;

     /**
      * Date format for parsing dates.
      */
     AutoPtr<IDateFormat> mDateFormat;

     /**
      * The current locale.
      */
     AutoPtr<ILocale> mCurrentLocale;

};
}// namespace Widget
}// namespace Droid
}// namespace Elastos
#endif
