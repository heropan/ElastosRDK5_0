#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/widget/DatePicker.h"
#include "elastos/droid/text/format/DateUtils.h"
#include "elastos/droid/text/TextUtils.h"
#include "elastos/droid/widget/NumberPicker.h"
#include "elastos/droid/text/format/DateUtils.h"
#include "elastos/droid/text/format/DateFormat.h"
#include "elastos/droid/view/inputmethod/CInputMethodManager.h"
#include "elastos/droid/R.h"

using Elastos::Core::ISystem;
using Elastos::Core::CSystem;
using Elastos::Core::ICharSequence;
using Elastos::Core::CStringWrapper;
using Elastos::Droid::Text::TextUtils;
using Elastos::Droid::Text::Format::IDateUtils;
using Elastos::Droid::Text::Format::DateUtils;
using Elastos::Droid::Text::Format::DateUtils;
using Elastos::Droid::Text::Format::DateFormat;
using Elastos::Droid::View::ILayoutInflater;
using Elastos::Droid::View::IViewGroup;
using Elastos::Droid::View::EIID_IViewGroup;
using Elastos::Droid::View::InputMethod::IInputMethodManager;
using Elastos::Droid::View::InputMethod::CInputMethodManager;
using Elastos::Droid::Widget::NumberPicker;
using Elastos::Utility::ILocaleHelper;
using Elastos::Utility::CLocaleHelper;
using Elastos::Text::CSimpleDateFormat;
using Elastos::Text::ISimpleDateFormat;
using Elastos::Utility::ICalendarHelper;
using Elastos::Utility::CCalendarHelper;
using Elastos::Utility::IDate;

namespace Elastos {
namespace Droid {
namespace Widget {

const String DatePicker::DATE_FORMAT = String("MM/dd/yyyy");
const Int32 DatePicker::DEFAULT_START_YEAR;
const Int32 DatePicker::DEFAULT_END_YEAR;
const Boolean DatePicker::DEFAULT_CALENDAR_VIEW_SHOWN;
const Boolean DatePicker::DEFAULT_SPINNERS_SHOWN;
const Boolean DatePicker::DEFAULT_ENABLED_STATE;
const String DatePicker::DATEPICKER_NAME = String("DatePicker");

CAR_INTERFACE_IMPL(DatePicker::DatePickerOnChangeListener, INumberPickerOnValueChangeListener)
CAR_INTERFACE_IMPL(DatePicker::DatePickerOnDateChangeListener, IOnDateChangeListener)
CAR_INTERFACE_IMPL(DatePicker::DatePickerSavedState, IViewBaseSavedState)

DatePicker::DatePicker() :
    mIsEnabled(TRUE)
{
    CSimpleDateFormat::New(DATE_FORMAT, (ISimpleDateFormat**)&mDateFormat);
}

DatePicker::DatePicker(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle) :
    mIsEnabled(TRUE)
{
    CSimpleDateFormat::New(DATE_FORMAT, (ISimpleDateFormat**)&mDateFormat);
    Init(context, attrs, defStyle);
}

ECode DatePicker::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    FrameLayout::Init(context, attrs, defStyle);
    AutoPtr<ILocaleHelper> helper;
    CLocaleHelper::AcquireSingleton((ILocaleHelper**)&helper);
    AutoPtr<ILocale> locale;
    helper->GetDefault((ILocale**)&locale);
    SetCurrentLocale(locale);
    AutoPtr<ArrayOf<Int32> > attrIds = ArrayOf<Int32>::Alloc(
            const_cast<Int32 *>(R::styleable::DatePicker),
            ARRAY_SIZE(R::styleable::DatePicker));
    AutoPtr<ITypedArray> a;
    FAIL_RETURN(context->ObtainStyledAttributes(attrs, attrIds, defStyle, 0, (ITypedArray**)&a));

    Boolean spinnersShown = FALSE;
    a->GetBoolean(R::styleable::DatePicker_spinnersShown, DEFAULT_SPINNERS_SHOWN, &spinnersShown);

    Boolean calendarViewShown = FALSE;
    a->GetBoolean(R::styleable::DatePicker_calendarViewShown, DEFAULT_CALENDAR_VIEW_SHOWN, &calendarViewShown);
    Int32 startYear = 0;
    a->GetInt32(R::styleable::DatePicker_startYear, DEFAULT_START_YEAR, &startYear);
    Int32 endYear = 0;
    a->GetInt32(R::styleable::DatePicker_endYear, DEFAULT_END_YEAR, &endYear);

    String minDate;
    a->GetString(R::styleable::DatePicker_minDate, &minDate);
    String maxDate;
    a->GetString(R::styleable::DatePicker_maxDate, &maxDate);
    Int32 layoutResourceId = 0;
    a->GetResourceId(R::styleable::DatePicker_internalLayout, R::layout::date_picker, &layoutResourceId);
    a->Recycle();
    AutoPtr<IInterface> inter;
    context->GetSystemService(IContext::LAYOUT_INFLATER_SERVICE, (IInterface**)&inter);
    AutoPtr<ILayoutInflater> inflater = ILayoutInflater::Probe(inter);

    AutoPtr<IView> view;
    inflater->Inflate(layoutResourceId, (IViewGroup*)this->Probe(EIID_IViewGroup), (IView**)&view);
    AutoPtr<DatePickerOnChangeListener> onChangeListener = new DatePickerOnChangeListener(this);
    mSpinners = ILinearLayout::Probe(FindViewById(R::id::pickers));
    mCalendarView = ICalendarView::Probe(FindViewById(R::id::calendar_view));
    AutoPtr<DatePickerOnDateChangeListener> listener = new DatePickerOnDateChangeListener(this);
    mCalendarView->SetOnDateChangeListener(listener);

    mDaySpinner = INumberPicker::Probe(FindViewById(R::id::day));
    mDaySpinner->SetFormatter(NumberPicker::GetTwoDigitFormatter());
    mDaySpinner->SetOnLongPressUpdateInterval(100);
    mDaySpinner->SetOnValueChangedListener(onChangeListener);
    AutoPtr<IView> dayInput;
    mDaySpinner->FindViewById(R::id::numberpicker_input, (IView**)&dayInput);
    mDaySpinnerInput = IEditText::Probe(dayInput);

    mMonthSpinner = INumberPicker::Probe(FindViewById(R::id::month));
    mMonthSpinner->SetMinValue(0);
    mMonthSpinner->SetMaxValue(mNumberOfMonths - 1);
    mMonthSpinner->SetDisplayedValues(mShortMonths);
    mMonthSpinner->SetOnLongPressUpdateInterval(200);
    mMonthSpinner->SetOnValueChangedListener(onChangeListener);
    AutoPtr<IView> monthInput;
    mMonthSpinner->FindViewById(R::id::numberpicker_input, (IView**)&monthInput);
    mMonthSpinnerInput = IEditText::Probe(monthInput);

    mYearSpinner = INumberPicker::Probe(FindViewById(R::id::year));
    mYearSpinner->SetOnLongPressUpdateInterval(100);
    mYearSpinner->SetOnValueChangedListener(onChangeListener);
    AutoPtr<IView> yearInput;
    mYearSpinner->FindViewById(R::id::numberpicker_input, (IView**)&yearInput);
    mYearSpinnerInput = IEditText::Probe(yearInput);

    if (!spinnersShown && !calendarViewShown) {
        SetSpinnersShown(TRUE);
    } else {
        SetSpinnersShown(spinnersShown);
        SetCalendarViewShown(calendarViewShown);
    }
    mTempDate->Clear();
    if (!TextUtils::IsEmpty(minDate)) {
        if (!ParseDate(minDate, mTempDate)) {
            mTempDate->Set(startYear, 0, 1);
        }
    } else {
        mTempDate->Set(startYear, 0, 1);
    }
    Int64 minTimeInMillis = 0;
    mTempDate->GetTimeInMillis(&minTimeInMillis);
    SetMinDate(minTimeInMillis);
    mTempDate->Clear();
    if (!TextUtils::IsEmpty(maxDate)) {
        if (!ParseDate(maxDate, mTempDate)) {
            mTempDate->Set(endYear, 11, 31);
        }
    } else {
        mTempDate->Set(endYear, 11, 31);
    }
    Int64 maxTimeInMillis = 0;
    mTempDate->GetTimeInMillis(&maxTimeInMillis);
    SetMaxDate(maxTimeInMillis);
    mCurrentDate->SetTimeInMillis(maxTimeInMillis);
    Int32 year = 0, month = 0, day = 0;
    mCurrentDate->Get(ICalendar::YEAR, &year);
    mCurrentDate->Get(ICalendar::MONTH, &month);
    mCurrentDate->Get(ICalendar::DAY_OF_MONTH, &day);
    Init(year, month, day, NULL);

    ReorderSpinners();

    SetContentDescriptions();
    if (GetImportantForAccessibility() == IView::IMPORTANT_FOR_ACCESSIBILITY_AUTO) {
        SetImportantForAccessibility(IView::IMPORTANT_FOR_ACCESSIBILITY_YES);
    }
    return NOERROR;
}

Int64 DatePicker::GetMinDate()
{
    Int64 minDate = 0;
    mCalendarView->GetMinDate(&minDate);
    return minDate;
}

ECode DatePicker::SetMinDate(
    /* [in] */ Int64 minDate)
{
    mTempDate->SetTimeInMillis(minDate);

    Int32 tempDateYear = 0, minDateYear = 0, tempDateDay = 0, minDateDay = 0;
    mTempDate->Get(ICalendar::YEAR, &tempDateYear);
    mTempDate->Get(ICalendar::DAY_OF_YEAR, &tempDateDay);
    mMinDate->Get(ICalendar::YEAR, &minDateYear);
    mMinDate->Get(ICalendar::DAY_OF_YEAR, &minDateDay);
    if((tempDateYear == minDateYear) && (tempDateDay != minDateDay)) return NOERROR;

    mMinDate->SetTimeInMillis(minDate);
    mCalendarView->SetMinDate(minDate);

    Boolean result = FALSE;
    mCurrentDate->IsBefore(mMinDate, &result);
    if(result) {
        Int64 time = 0;
        mMinDate->GetTimeInMillis(&time);
        mCurrentDate->SetTimeInMillis(time);
        UpdateCalendarView();
    }
    UpdateSpinners();
    return NOERROR;
}

Int64 DatePicker::GetMaxDate()
{
    Int64 maxDate = 0;
    mCalendarView->GetMaxDate(&maxDate);
    return maxDate;
}

ECode DatePicker::SetMaxDate(
    /* [in] */ Int64 maxDate)
{
    mTempDate->SetTimeInMillis(maxDate);

    Int32 tempDateYear = 0, maxDateYear = 0, tempDateDay = 0, maxDateDay = 0;
    mTempDate->Get(ICalendar::YEAR, &tempDateYear);
    mTempDate->Get(ICalendar::DAY_OF_YEAR, &tempDateDay);
    mMaxDate->Get(ICalendar::YEAR, &maxDateYear);
    mMaxDate->Get(ICalendar::DAY_OF_YEAR, &maxDateDay);
    if((tempDateYear == maxDateYear) && (tempDateDay != maxDateDay)) {
        return NOERROR;
    }
    mMaxDate->SetTimeInMillis(maxDate);
    mCalendarView->SetMaxDate(maxDate);

    Boolean result = FALSE;
    mCurrentDate->IsAfter(mMaxDate, &result);
    if(result) {
        Int64 time = 0;
        mMaxDate->GetTimeInMillis(&time);
        mCurrentDate->SetTimeInMillis(time);
        UpdateCalendarView();
    }
    UpdateSpinners();
    return NOERROR;
}

ECode DatePicker::SetEnabled(
    /* [in] */ Boolean enabled)
{
    if(mIsEnabled == enabled) return NOERROR;
    FrameLayout::SetEnabled(enabled);
    mDaySpinner->SetEnabled(enabled);
    mMonthSpinner->SetEnabled(enabled);
    mYearSpinner->SetEnabled(enabled);
    mCalendarView->SetEnabled(enabled);
    mIsEnabled = enabled;

    return NOERROR;
}

Boolean DatePicker::IsEnabled()
{
    return mIsEnabled;
}

Boolean DatePicker::DispatchPopulateAccessibilityEvent(
    /* [in] */ IAccessibilityEvent* event)
{
    OnPopulateAccessibilityEvent(event);
    return TRUE;
}

ECode DatePicker::OnPopulateAccessibilityEvent(
    /* [in] */ IAccessibilityEvent* event)
{
    FrameLayout::OnPopulateAccessibilityEvent(event);
    const Int32 flags = IDateUtils::FORMAT_SHOW_TIME | IDateUtils::FORMAT_SHOW_YEAR;
    AutoPtr<IDateUtils> dateUtils;
    Int64 timeInMillis = 0;
    mCurrentDate->GetTimeInMillis(&timeInMillis);
    String selectedDateUtterance = DateUtils::FormatDateTime(mContext, timeInMillis, flags);
    AutoPtr<IObjectContainer> container;
    event->GetText((IObjectContainer**)&container);

    AutoPtr<ICharSequence> seq;
    FAIL_RETURN(CStringWrapper::New(selectedDateUtterance, (ICharSequence**)&seq));
    container->Add(seq);
    return NOERROR;
}

ECode DatePicker::OnInitializeAccessibilityEvent(
    /* [in] */ IAccessibilityEvent* event)
{
    FrameLayout::OnInitializeAccessibilityEvent(event);
    AutoPtr<ICharSequence> seq;
    FAIL_RETURN(CStringWrapper::New(DATEPICKER_NAME, (ICharSequence**)&seq));
    event->SetClassName(seq);
    return NOERROR;
}

ECode DatePicker::OnInitializeAccessibilityNodeInfo(
    /* [in] */ IAccessibilityNodeInfo* info)
{
    FrameLayout::OnInitializeAccessibilityNodeInfo(info);
    AutoPtr<ICharSequence> seq;
    FAIL_RETURN(CStringWrapper::New(DATEPICKER_NAME, (ICharSequence**)&seq));
    info->SetClassName(seq);
    return NOERROR;
}

void DatePicker::OnConfigurationChanged(
    /* [in] */ IConfiguration* newConfig)
{
    FrameLayout::OnConfigurationChanged(newConfig);
    AutoPtr<ILocale> locale;
    newConfig->GetLocale((ILocale**)&locale);
    SetCurrentLocale(locale);
}

Boolean DatePicker::GetCalendarViewShown()
{
    Boolean calendarViewShown;
    mCalendarView->IsShown(&calendarViewShown);
    return calendarViewShown;
}

AutoPtr<ICalendarView> DatePicker::GetCalendarView()
{
    return mCalendarView;
}

ECode DatePicker::SetCalendarViewShown(
    /* [in] */ Boolean shown)
{
    if(shown){
        mCalendarView->SetVisibility(IView::VISIBLE);
    } else{
        mCalendarView->SetVisibility(IView::GONE);
    }
    return NOERROR;
}

Boolean DatePicker::GetSpinnersShown()
{
    Boolean SpinnersShown;
    mSpinners->IsShown(&SpinnersShown);
    return SpinnersShown;
}


ECode DatePicker::SetSpinnersShown(
    /* [in] */ Boolean shown)
{
    if(shown){
        mSpinners->SetVisibility(IView::VISIBLE);
    } else{
        mSpinners->SetVisibility(IView::GONE);
    }
    return NOERROR;
}

ECode DatePicker::SetCurrentLocale(
    /* [in] */ ILocale* locale)
{
    Boolean result = FALSE;
    locale->Equals(mCurrentLocale, &result);
    if(result) return NOERROR;

    mCurrentLocale = locale;
    mTempDate = GetCalendarForLocale(mTempDate, locale);
    mMinDate = GetCalendarForLocale(mMinDate, locale);
    mMaxDate = GetCalendarForLocale(mMaxDate, locale);
    mCurrentDate = GetCalendarForLocale(mCurrentDate, locale);

    mTempDate->GetActualMaximum(ICalendar::MONTH, &mNumberOfMonths);
    mNumberOfMonths += 1;

    mShortMonths = ArrayOf<String>::Alloc(mNumberOfMonths);
    for(Int32 i = 0; i < mNumberOfMonths; i++) {
        (*mShortMonths)[i] = DateUtils::GetMonthString(ICalendar::JANUARY + i,
                IDateUtils::LENGTH_MEDIUM);
    }
    return NOERROR;
}

AutoPtr<ICalendar> DatePicker::GetCalendarForLocale(
    /* [in] */ ICalendar* oldCalendar,
    /* [in] */ ILocale* locale)
{
    AutoPtr<ICalendarHelper> helper;
    CCalendarHelper::AcquireSingleton((ICalendarHelper**)&helper);
    if(!oldCalendar) {
        AutoPtr<ICalendar> calendar;
        helper->GetInstance(locale, (ICalendar**)&calendar);
        return calendar;
    } else {
        Int64 currentTimeMillis;
        oldCalendar->GetTimeInMillis(&currentTimeMillis);
        AutoPtr<ICalendar> calendar;
        helper->GetInstance(locale, (ICalendar**)&calendar);
        calendar->SetTimeInMillis(currentTimeMillis);
        return calendar;
    }
}

ECode DatePicker::ReorderSpinners()
{
    mSpinners->RemoveAllViews();
    AutoPtr< ArrayOf<Char32> > order = DateFormat::GetDateFormatOrder(GetContext());
    const Int32 spinnerCount = order->GetLength();
    for(Int32 i = 0; i < spinnerCount; i++) {
        switch ((Int32)((*order)[i])) {
        case Elastos::Droid::Text::Format::IDateFormat::DATE:
            mSpinners->AddView(mDaySpinner);
            SetImeOptions(mDaySpinner, spinnerCount, i);
            break;
        case Elastos::Droid::Text::Format::IDateFormat::MONTH:
            mSpinners->AddView(mMonthSpinner);
            SetImeOptions(mMonthSpinner, spinnerCount, i);
            break;
        case Elastos::Droid::Text::Format::IDateFormat::YEAR:
            mSpinners->AddView(mYearSpinner);
            SetImeOptions(mYearSpinner, spinnerCount, i);
            break;
        default:
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
            break;
        }
    }
    return NOERROR;
}

ECode DatePicker::UpdateDate(
    /* [in] */ Int32 year,
    /* [in] */ Int32 month,
    /* [in] */ Int32 dayOfMonth)
{
    if(!IsNewDate(year, month, dayOfMonth)){
        return NOERROR;
    }
    SetDate(year, month, dayOfMonth);
    UpdateSpinners();
    UpdateCalendarView();
    NotifyDateChanged();
    return NOERROR;
}

ECode DatePicker::DispatchRestoreInstanceState(
    /* [in] */ IObjectInt32Map* container)
{
    DispatchThawSelfOnly(container);
    return NOERROR;
}

AutoPtr<IParcelable> DatePicker::OnSaveInstanceState()
{
    AutoPtr<IParcelable> superState = FrameLayout::OnSaveInstanceState();

    /*AutoPtr<DatePickerSavedState> state = new DatePickerSavedState(superState, GetYear(), GetMonth(), GetDayOfMonth());
    AutoPtr<IParcelable> p = IParcelable::Probe(state);*/
    return NULL;
}

void DatePicker::OnRestoreInstanceState(
        /* [in] */ IParcelable* state)
{
}

ECode DatePicker::Init(
    /* [in] */ Int32 year,
    /* [in] */ Int32 monthOfYear,
    /* [in] */ Int32 dayOfMonth,
    /* [in] */ IOnDateChangedListener* onDateChangedListener)
{
    SetDate(year, monthOfYear, dayOfMonth);
    UpdateSpinners();
    UpdateCalendarView();
    mOnDateChangedListener = onDateChangedListener;
    return NOERROR;
}

Boolean DatePicker::ParseDate(
    /* [in] */ const String& date,
    /* [in] */ ICalendar* outDate)
{
    AutoPtr<IDate> toDate;
    mDateFormat->Parse(date, (IDate**)&toDate);
    //try {
    outDate->SetTime(toDate);
    return TRUE;
    //    } catch (ParseException e) {
    //        Log.w(LOG_TAG, "Date: " + date + " not in format: " + DATE_FORMAT);
    //       return false;
    //    }
}

Boolean DatePicker::IsNewDate(
    /* [in] */ Int32 year,
    /* [in] */ Int32 month,
    /* [in] */ Int32 dayOfMonth)
{
    Int32 newYear = 0, newMonth = 0, newDayOfMonth = 0;
    mCurrentDate->Get(ICalendar::YEAR, &newYear);
    mCurrentDate->Get(ICalendar::MONTH, &newMonth);
    mCurrentDate->Get(ICalendar::DAY_OF_MONTH, &newDayOfMonth);
    return (newYear != year || newMonth != month || newDayOfMonth != dayOfMonth);
}

ECode DatePicker::SetDate(
    /* [in] */ Int32 year,
    /* [in] */ Int32 month,
    /* [in] */ Int32 dayOfMonth)
{
    mCurrentDate->Set(year, month, dayOfMonth);
    Boolean beforeMin = FALSE, afterMax = FALSE;
    mCurrentDate->IsBefore(mMinDate, &beforeMin);
    mCurrentDate->IsAfter(mMaxDate, &afterMax);
    if(beforeMin) {
        Int64 min = 0;
        mMinDate->GetTimeInMillis(&min);
        mCurrentDate->SetTimeInMillis(min);
    } else if(afterMax) {
        Int64 max = 0;
        mMaxDate->GetTimeInMillis(&max);
        mCurrentDate->SetTimeInMillis(max);
    }
    return NOERROR;
}

ECode DatePicker::UpdateSpinners()
{
    Boolean min, max;
    mCurrentDate->Equals(mMinDate, &min);
    mCurrentDate->Equals(mMaxDate, &max);
    Int32 minDayValue, maxDayValue, minMonthValue, maxMonthValue;
    if(min) {
        mCurrentDate->Get(ICalendar::DAY_OF_MONTH, &minDayValue);
        mCurrentDate->GetActualMaximum(ICalendar::DAY_OF_MONTH, &maxDayValue);
        mDaySpinner->SetMinValue(minDayValue);
        mDaySpinner->SetMaxValue(maxDayValue);
        mDaySpinner->SetWrapSelectorWheel(FALSE);
        mMonthSpinner->SetDisplayedValues(NULL);
        mCurrentDate->Get(ICalendar::MONTH, &minMonthValue);
        mCurrentDate->GetActualMaximum(ICalendar::MONTH, &maxMonthValue);
        mMonthSpinner->SetMinValue(minMonthValue);
        mMonthSpinner->SetMaxValue(maxMonthValue);
        mMonthSpinner->SetWrapSelectorWheel(FALSE);
    } else if(max) {
        mCurrentDate->GetActualMinimum(ICalendar::DAY_OF_MONTH, &minDayValue);
        mCurrentDate->Get(ICalendar::DAY_OF_MONTH, &maxDayValue);
        mDaySpinner->SetMinValue(minDayValue);
        mDaySpinner->SetMaxValue(maxDayValue);
        mDaySpinner->SetWrapSelectorWheel(FALSE);
        mMonthSpinner->SetDisplayedValues(NULL);
        mCurrentDate->GetActualMinimum(ICalendar::MONTH, &minMonthValue);
        mCurrentDate->Get(ICalendar::MONTH, &maxMonthValue);
        mMonthSpinner->SetMinValue(minMonthValue);
        mMonthSpinner->SetMaxValue(maxMonthValue);
        mMonthSpinner->SetWrapSelectorWheel(FALSE);
    } else {
        mDaySpinner->SetMaxValue(1);
        mCurrentDate->GetActualMaximum(ICalendar::DAY_OF_MONTH, &maxDayValue);
        mDaySpinner->SetMaxValue(maxDayValue);
        mDaySpinner->SetWrapSelectorWheel(TRUE);
        mMonthSpinner->SetDisplayedValues(NULL);
        mMonthSpinner->SetMinValue(0);
        mMonthSpinner->SetMaxValue(11);
        mMonthSpinner->SetWrapSelectorWheel(TRUE);
    }

    Int32 start = 0, end = 0;
    mMonthSpinner->GetMinValue(&start);
    mMonthSpinner->GetMaxValue(&end);
    end += 1;

    AutoPtr< ArrayOf<String> > disPlayedValues = ArrayOf<String>::Alloc(end - start);
    if (start < 0 && start > mShortMonths->GetLength()) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    } else if (start > end) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    if (end - start < mShortMonths->GetLength() - start) {
        disPlayedValues->Copy(start, mShortMonths, end - start);
    } else {
        disPlayedValues->Copy(start, mShortMonths, mShortMonths->GetLength() - start);
    }

    mMonthSpinner->SetDisplayedValues(disPlayedValues);

    Int32 yearValue, monthValue, dayVelue, minYearValue, maxYearValue;
    mMinDate->Get(ICalendar::YEAR, &minYearValue);
    mMaxDate->Get(ICalendar::YEAR, &maxYearValue);
    mYearSpinner->SetMinValue(minYearValue);
    mYearSpinner->SetMaxValue(maxYearValue);
    mYearSpinner->SetWrapSelectorWheel(FALSE);

    mCurrentDate->Get(ICalendar::YEAR, &yearValue);
    mCurrentDate->Get(ICalendar::MONTH, &monthValue);
    mCurrentDate->Get(ICalendar::DAY_OF_MONTH, &dayVelue);
    mYearSpinner->SetValue(yearValue);
    mMonthSpinner->SetValue(monthValue);
    mDaySpinner->SetValue(dayVelue);

    return NOERROR;
}

ECode DatePicker::UpdateCalendarView()
{
    Int64 timeInMillis;
    mCurrentDate->GetTimeInMillis(&timeInMillis);
    mCalendarView->SetDate(timeInMillis, FALSE, FALSE);
    return NOERROR;
}

Int32 DatePicker::GetYear()
{
    Int32 year;
    mCurrentDate->Get(ICalendar::YEAR, &year);
    return year;
}

Int32 DatePicker::GetMonth()
{
    Int32 month;
    mCurrentDate->Get(ICalendar::MONTH, &month);
    return month;
}

Int32 DatePicker::GetDayOfMonth()
{
    Int32 day;
    mCurrentDate->Get(ICalendar::DAY_OF_MONTH, &day);
    return day;
}

ECode DatePicker::NotifyDateChanged()
{
    SendAccessibilityEvent(IAccessibilityEvent::TYPE_VIEW_SELECTED);
    if(mOnDateChangedListener != NULL) {
        mOnDateChangedListener->OnDateChanged((IDatePicker*)this->Probe(EIID_IDatePicker), GetYear(), GetMonth(), GetDayOfMonth());
    }

    return NOERROR;
}

ECode DatePicker::SetImeOptions(
        /* [in] */ INumberPicker* spinner,
        /* [in] */ Int32 spinnerCount,
        /* [in] */ Int32 spinnerIndex)
{
    Int32 imeOptions;
    if(spinnerIndex < spinnerCount - 1) {
        imeOptions = IEditorInfo::IME_ACTION_NEXT;
    } else {
        imeOptions = IEditorInfo::IME_ACTION_DONE;
    }
    AutoPtr<ITextView> input;
    AutoPtr<IView> view = FindViewById(R::id::numberpicker_input);
    input = (ITextView*)view->Probe(EIID_ITextView);
    if(input != NULL) {
        input->SetImeOptions(imeOptions);
    }
    return NOERROR;
}

ECode DatePicker::SetContentDescriptions()
{
    TrySetContentDescription(mDaySpinner, R::id::increment, R::string::date_picker_increment_day_button);
    TrySetContentDescription(mDaySpinner, R::id::increment, R::string::date_picker_decrement_day_button);
    TrySetContentDescription(mDaySpinner, R::id::increment, R::string::date_picker_increment_month_button);
    TrySetContentDescription(mDaySpinner, R::id::increment, R::string::date_picker_decrement_month_button);
    TrySetContentDescription(mDaySpinner, R::id::increment, R::string::date_picker_increment_year_button);
    TrySetContentDescription(mDaySpinner, R::id::increment, R::string::date_picker_decrement_year_button);

    return NOERROR;
}

ECode DatePicker::TrySetContentDescription(
    /* [in] */ IView* root,
    /* [in] */ Int32 viewId,
    /* [in] */ Int32 contDescResId)
{
    AutoPtr<IView> target;
    root->FindViewById(viewId, (IView**)&target);
    if(target != NULL) {
        AutoPtr<IContext> context;
        context = GetContext();
        String res;
        context->GetString(contDescResId, &res);
        AutoPtr<ICharSequence> seq;
        FAIL_RETURN(CStringWrapper::New(res, (ICharSequence**)&seq));
        target->SetContentDescription(seq);
    }
    return NOERROR;
}

ECode DatePicker::UpdateInputState()
{
    AutoPtr<IInputMethodManager> inputMethodManager = CInputMethodManager::PeekInstance();
    if (inputMethodManager) {
        Boolean yearActive = FALSE, monthActive = FALSE, dayActive = FALSE;
        inputMethodManager->IsActive(mYearSpinnerInput, &yearActive);
        inputMethodManager->IsActive(mMonthSpinnerInput, &monthActive);
        inputMethodManager->IsActive(mDaySpinnerInput, &dayActive);
        Boolean res = FALSE;
        if (yearActive) {
            mYearSpinnerInput->ClearFocus();
            inputMethodManager->HideSoftInputFromWindow(GetWindowToken(), 0, &res);
        } else if (monthActive) {
            mMonthSpinnerInput->ClearFocus();
            inputMethodManager->HideSoftInputFromWindow(GetWindowToken(), 0, &res);
        } else if (dayActive) {
            mDaySpinnerInput->ClearFocus();
            inputMethodManager->HideSoftInputFromWindow(GetWindowToken(), 0, &res);
        }
    }
    return NOERROR;
}

// ==================================================================
//                DatePicker::DatePickerOnChangeListener
// ==================================================================

DatePicker::DatePickerOnChangeListener::DatePickerOnChangeListener(
    /* [in] */ DatePicker* host) :
    mHost(host)
{
}

ECode DatePicker::DatePickerOnChangeListener::OnValueChange(
    /* [in] */ INumberPicker* picker,
    /* [in] */ Int32 oldVal,
    /* [in] */ Int32 newVal)
{
    mHost->UpdateInputState();
    Int64 millis = 0;
    mHost->mCurrentDate->GetTimeInMillis(&millis);
    mHost->mTempDate->SetTimeInMillis(millis);
    if (picker == mHost->mDaySpinner) {
        Int32 maxDayOfMonth = 0;
        mHost->mTempDate->GetActualMaximum(ICalendar::DAY_OF_MONTH, &maxDayOfMonth);
        if (oldVal == maxDayOfMonth && newVal == 1) {
            mHost->mTempDate->Add(ICalendar::DAY_OF_MONTH, 1);
        } else if (oldVal == 1 && newVal == maxDayOfMonth) {
            mHost->mTempDate->Add(ICalendar::DAY_OF_MONTH, -1);
        } else {
            mHost->mTempDate->Add(ICalendar::DAY_OF_MONTH, newVal - oldVal);
        }
    } else if (picker == mHost->mMonthSpinner) {
        if (oldVal == 11 && newVal == 0) {
            mHost->mTempDate->Add(ICalendar::MONTH, 1);
        } else if (oldVal == 0 && newVal == 11) {
            mHost->mTempDate->Add(ICalendar::MONTH, -1);
        } else {
            mHost->mTempDate->Add(ICalendar::MONTH, newVal - oldVal);
        }
    } else if (picker == mHost->mYearSpinner) {
        mHost->mTempDate->Set(ICalendar::YEAR, newVal);

    } else {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    Int32 year = 0, month = 0, day = 0;
    mHost->mTempDate->Get(ICalendar::YEAR, &year);
    mHost->mTempDate->Get(ICalendar::MONTH, &month);
    mHost->mTempDate->Get(ICalendar::DAY_OF_MONTH, &day);
    mHost->SetDate(year, month, day);
    mHost->UpdateSpinners();
    mHost->UpdateCalendarView();
    mHost->NotifyDateChanged();
    return NOERROR;
}

// ==================================================================
//                DatePicker::DatePickerOnDateChangedListener
// ==================================================================

DatePicker::DatePickerOnDateChangeListener::DatePickerOnDateChangeListener(
    /* [in] */ DatePicker* host) :
    mHost(host)
{
}

ECode DatePicker::DatePickerOnDateChangeListener::OnSelectedDayChange(
    /* [in] */ ICalendarView* view,
    /* [in] */ Int32 year,
    /* [in] */ Int32 month,
    /* [in] */ Int32 dayOfMonth)
{
    mHost->SetDate(year, month, dayOfMonth);
    mHost->UpdateSpinners();
    mHost->NotifyDateChanged();
    return NOERROR;
}

// ==================================================================
//                DatePicker::DatePickerSavedState
// ==================================================================

DatePicker::DatePickerSavedState::DatePickerSavedState(
    /* [in] */ IParcel* in) :
    ViewBaseSavedState(in)
{
    in->ReadInt32(&mYear);
    in->ReadInt32(&mMonth);
    in->ReadInt32(&mDay);
}

DatePicker::DatePickerSavedState::DatePickerSavedState(
    /* [in] */ IParcelable* superState,
    /* [in] */ Int32 year,
    /* [in] */ Int32 month,
    /* [in] */ Int32 day) :
    ViewBaseSavedState(superState)
{
    mYear = year;
    mMonth = month;
    mDay = day;
}


ECode DatePicker::DatePickerSavedState::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    ViewBaseSavedState::WriteToParcel(dest);
    dest->WriteInt32(mYear);
    dest->WriteInt32(mMonth);
    dest->WriteInt32(mDay);
    return NOERROR;
}

ECode DatePicker::DatePickerSavedState::GetSuperState(
    /* [out] */ IParcelable** state)
{
    VALIDATE_NOT_NULL(state);
    AutoPtr<IParcelable> parcelable = ViewBaseSavedState::GetSuperState();
    *state = parcelable;
    REFCOUNT_ADD(*state);
    return NOERROR;
}

} // namespace Widget
} // namespace Droid
} // namespace Elastos

