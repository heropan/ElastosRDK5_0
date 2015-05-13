
#include "widget/TimePicker.h"
#include "widget/NumberPicker.h"
#include "text/format/DateUtils.h"
#include "view/inputmethod/CInputMethodManager.h"

using Libcore::ICU::ILocaleHelper;
using Libcore::ICU::CLocaleHelper;
using Elastos::Core::CInteger32;
using Elastos::Core::CStringWrapper;
using Elastos::Droid::Text::Format::IDateUtils;
using Elastos::Droid::Text::Format::DateUtils;
using Elastos::Droid::View::EIID_IViewOnClickListener;
using Elastos::Droid::View::EIID_IViewGroup;
using Elastos::Droid::View::InputMethod::CInputMethodManager;
using Elastos::Droid::Widget::NumberPicker;
using Elastos::Text::IDateFormatSymbols;
using Elastos::Text::CDateFormatSymbols;
using Elastos::Utility::ICalendarHelper;
using Elastos::Utility::CCalendarHelper;


namespace Elastos {
namespace Droid {
namespace Widget {

const Boolean TimePicker::DEFAULT_ENABLED_STATE;
const Int32 TimePicker::HOURS_IN_HALF_DAY;

TimePicker::TimePicker(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
    : mIsEnabled(TRUE)

{
    NO_OP_CHANGE_LISTENER = new TimePickerOnTimeChangedListener();
    Init(context, attrs, defStyle);
}

TimePicker::TimePicker()
    : mIsEnabled(TRUE)
{
    PEL("TimePicker::TimePicker")
    NO_OP_CHANGE_LISTENER = new TimePickerOnTimeChangedListener();
}

ECode TimePicker::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    PEL("TimePicker::Init")
    FrameLayout::Init(context, attrs, defStyle);

    AutoPtr<ILocaleHelper> helper;
    CLocaleHelper::AcquireSingleton((ILocaleHelper**)&helper);
    AutoPtr<ILocale> def;
    helper->GetDefault((ILocale**)&def);
    SetCurrentLocale(def);

    AutoPtr<ArrayOf<Int32> > attrIds = ArrayOf<Int32>::Alloc(
            const_cast<Int32 *>(R::styleable::TimePicker),
            ARRAY_SIZE(R::styleable::TimePicker));
    AutoPtr<ITypedArray> a;
    FAIL_RETURN(context->ObtainStyledAttributesEx3(attrs, attrIds, defStyle, 0, (ITypedArray**)&a));
    Int32 layoutResourceId = 0;
    a->GetResourceId(R::styleable::TimePicker_internalLayout, R::layout::time_picker, &layoutResourceId);
    a->Recycle();

    AutoPtr<IInterface> inter;
    context->GetSystemService(IContext::LAYOUT_INFLATER_SERVICE, (IInterface**)&inter);
    AutoPtr<ILayoutInflater> infalter = ILayoutInflater::Probe(inter);
    AutoPtr<IView> v;
    infalter->InflateEx2(layoutResourceId, (IViewGroup*)this->Probe(EIID_IViewGroup), TRUE, (IView**)&v);

    mHourSpinner = INumberPicker::Probe(FindViewById(R::id::hour));
    AutoPtr<HourChangeListener> hourChangeListener = new HourChangeListener(this);
    mHourSpinner->SetOnValueChangedListener(hourChangeListener);

    AutoPtr<IView> hourSpinnerInput;
    mHourSpinner->FindViewById(R::id::numberpicker_input, (IView**)&hourSpinnerInput);
    mHourSpinnerInput = IEditText::Probe(hourSpinnerInput);
    mHourSpinnerInput->SetImeOptions(IEditorInfo::IME_ACTION_NEXT);

    mDivider = ITextView::Probe(FindViewById(R::id::divider));
    if (mDivider) {
        mDivider->SetTextEx3(R::string::time_picker_separator);
    }

    mMinuteSpinner = INumberPicker::Probe(FindViewById(R::id::minute));
    mMinuteSpinner->SetMinValue(0);
    mMinuteSpinner->SetMaxValue(59);
    mMinuteSpinner->SetOnLongPressUpdateInterval(100);
    mMinuteSpinner->SetFormatter(NumberPicker::GetTwoDigitFormatter());
    AutoPtr<MinuteChangeListener> minuteChangeListener = new MinuteChangeListener(this);
    mMinuteSpinner->SetOnValueChangedListener(minuteChangeListener);
    AutoPtr<IView> minuteSpinnerInput;
    mMinuteSpinner->FindViewById(R::id::numberpicker_input, (IView**)&minuteSpinnerInput);
    mMinuteSpinnerInput = IEditText::Probe(minuteSpinnerInput);
    mMinuteSpinnerInput->SetImeOptions(IEditorInfo::IME_ACTION_NEXT);
    AutoPtr<IDateFormatSymbols> symbols;
    CDateFormatSymbols::New((IDateFormatSymbols**)&symbols);
    symbols->GetAmPmStrings((ArrayOf<String>**)&mAmPmStrings);

    AutoPtr<IView> amPmView = FindViewById(R::id::amPm);
    if (IButton::Probe(amPmView)) {
        mAmPmSpinner = NULL;
        mAmPmSpinnerInput = NULL;
        mAmPmButton = IButton::Probe(amPmView);
        AutoPtr<AmPmClickListener> clickListener = new AmPmClickListener(this);
        mAmPmButton->SetOnClickListener(clickListener);
    } else {
        mAmPmButton = NULL;
        mAmPmSpinner = INumberPicker::Probe(amPmView);
        mAmPmSpinner->SetMinValue(0);
        mAmPmSpinner->SetMaxValue(1);
        mAmPmSpinner->SetDisplayedValues(mAmPmStrings);
        AutoPtr<AmPmChangeListener> changeListener = new AmPmChangeListener(this);
        mAmPmSpinner->SetOnValueChangedListener(changeListener);

        AutoPtr<IView> mAmPmView;
        mAmPmSpinner->FindViewById(R::id::numberpicker_input, (IView**)&mAmPmView);
        mAmPmSpinnerInput = IEditText::Probe(mAmPmView);
        mAmPmSpinnerInput->SetImeOptions(IEditorInfo::IME_ACTION_DONE);
    }

    UpdateHourControl();
    UpdateAmPmControl();

    SetOnTimeChangedListener(NO_OP_CHANGE_LISTENER);
    Int32 hour = 0, minute = 0;
    mTempCalendar->Get(ICalendar::HOUR_OF_DAY, &hour);
    AutoPtr<IInteger32> timeHour;
    CInteger32::New(hour, (IInteger32**)&timeHour);
    SetCurrentHour(timeHour);
    mTempCalendar->Get(ICalendar::MINUTE, &minute);
    AutoPtr<IInteger32> timeMinute;
    CInteger32::New(hour, (IInteger32**)&timeMinute);
    SetCurrentHour(timeMinute);

    if (!IsEnabled()) {
        SetEnabled(FALSE);
    }

    SetContentDescriptions();

    if (GetImportantForAccessibility() == IView::IMPORTANT_FOR_ACCESSIBILITY_AUTO) {
        SetImportantForAccessibility(IView::IMPORTANT_FOR_ACCESSIBILITY_YES);
    }

    return NOERROR;
}

ECode TimePicker::SetEnabled(
    /* [in] */ Boolean enabled)
{
    PEL("TimePicker::SetEnabled")
    if (mIsEnabled == enabled) {
        return NOERROR;
    }
    FrameLayout::SetEnabled(enabled);
    mMinuteSpinner->SetEnabled(enabled);
    if (mDivider) {
        mDivider->SetEnabled(enabled);
    }
    mHourSpinner->SetEnabled(enabled);
    if (mAmPmSpinner) {
        mAmPmSpinner->SetEnabled(enabled);
    } else {
        mAmPmButton->SetEnabled(enabled);
    }
    mIsEnabled = enabled;

    return NOERROR;
}

Boolean TimePicker::IsEnabled()
{
    PEL("TimePicker::IsEnabled")
    return mIsEnabled;
}

ECode TimePicker::SetOnTimeChangedListener(
    /* [in] */ IOnTimeChangedListener* onTimeChangedListener)
{
    PEL("TimePicker::SetOnTimeChangedListener")
    mOnTimeChangedListener = onTimeChangedListener;
    return NOERROR;
}

AutoPtr<IInteger32> TimePicker::GetCurrentHour()
{
    PEL("TimePicker::GetCurrentHour")
    Int32 currentHour = 0;
    mHourSpinner->GetValue(&currentHour);
    AutoPtr<IInteger32> res;
    if (Is24HourView()) {
        CInteger32::New(currentHour, (IInteger32**)&res);
    } else if (mIsAm) {
        CInteger32::New(currentHour % HOURS_IN_HALF_DAY, (IInteger32**)&res);
    } else {
        CInteger32::New((currentHour % HOURS_IN_HALF_DAY) + HOURS_IN_HALF_DAY, (IInteger32**)&res);
    }

    return res;
}

ECode TimePicker::SetCurrentHour(
    /* [in] */ IInteger32* currentHour)
{
    PEL("TimePicker::SetCurrentHour")
    Int32 current = 0, old = 0;
    GetCurrentHour()->GetValue(&old);
    if (currentHour == NULL || (currentHour->GetValue(&current), current)
        == old) {
        return NOERROR;
    }
    if (!Is24HourView()) {
        if(current >= HOURS_IN_HALF_DAY) {
            mIsAm = FALSE;
            if(current > HOURS_IN_HALF_DAY) {
                current = current - HOURS_IN_HALF_DAY;
            }
        } else {
            mIsAm = TRUE;
            if (current == 0) {
                current = HOURS_IN_HALF_DAY;
            }
        }
        UpdateAmPmControl();
    }
    mHourSpinner->SetValue(current);
    OnTimeChanged();

    return NOERROR;
}

ECode TimePicker::SetIs24HourView(
    /* [in] */ Boolean is24HourView)
{
    PEL("TimePicker::SetIs24HourView")
    if (mIs24HourView == is24HourView) {
        return NOERROR;
    }
    mIs24HourView = is24HourView;
    AutoPtr<IInteger32> currentHour = GetCurrentHour();
    UpdateHourControl();
    SetCurrentHour(currentHour);
    UpdateAmPmControl();

    return NOERROR;
}

Boolean TimePicker::Is24HourView()
{
    PEL("TimePicker::Is24HourView")
    return mIs24HourView;
}

CARAPI_(AutoPtr<IInteger32>) TimePicker::GetCurrentMinute()
{
    PEL("TimePicker::GetCurrentMinute")
    Int32 value = 0;
    mMinuteSpinner->GetValue(&value);
    AutoPtr<IInteger32> res;
    CInteger32::New(value, (IInteger32**)&res);
    return res;
}

ECode TimePicker::SetCurrentMinute(
    /* [in] */ IInteger32* currentMinute)
{
    PEL("TimePicker::SetCurrentMinute")
    Int32 current = 0, old = 0;
    GetCurrentMinute()->GetValue(&old);
    if (currentMinute == NULL || (currentMinute->GetValue(&current), current) == old) {
        return NOERROR;
    }
    mMinuteSpinner->SetValue(current);
    OnTimeChanged();
    return NOERROR;
}

ECode TimePicker::GetBaseline(
    /* [out] */ Int32* baseline)
{
    PEL("TimePicker::GetBaseline")
    VALIDATE_NOT_NULL(baseline);
    mHourSpinner->GetBaseline(baseline);
    return NOERROR;
}

Boolean TimePicker::DispatchPopulateAccessibilityEvent(
    /* [in] */ IAccessibilityEvent* event)
{
    PEL("TimePicker::DispatchPopulateAccessibilityEvent")
    OnPopulateAccessibilityEvent(event);
    return TRUE;
}

ECode TimePicker::OnPopulateAccessibilityEvent(
    /* [in] */ IAccessibilityEvent* event)
{
    PEL("TimePicker::OnPopulateAccessibilityEvent")
    FrameLayout::OnPopulateAccessibilityEvent(event);
    Int32 flags = IDateUtils::FORMAT_SHOW_TIME;
    if (mIs24HourView) {
        flags |= IDateUtils::FORMAT_24HOUR;
    } else {
        flags |= IDateUtils::FORMAT_12HOUR;
    }

    AutoPtr<IInteger32> hour = GetCurrentHour();
    AutoPtr<IInteger32> minute = GetCurrentMinute();
    Int32 currentHour = 0, currentMinute = 0;
    hour->GetValue(&currentHour);
    minute->GetValue(&currentMinute);
    mTempCalendar->Set(ICalendar::HOUR_OF_DAY, currentHour);
    mTempCalendar->Set(ICalendar::MINUTE, currentMinute);
    Int64 timeInMillis = 0;
    mTempCalendar->GetTimeInMillis(&timeInMillis);
    String selectedDateUtterance = DateUtils::FormatDateTime(mContext, timeInMillis, flags);
    AutoPtr<IObjectContainer> container;
    event->GetText((IObjectContainer**)&container);
    AutoPtr<ICharSequence> seq;
    CStringWrapper::New(selectedDateUtterance, (ICharSequence**)&seq);
    container->Add(seq);
    return NOERROR;
}

ECode TimePicker::OnInitializeAccessibilityEvent(
    /* [in] */ IAccessibilityEvent* event)
{
    PEL("TimePicker::OnInitializeAccessibilityEvent")
    FrameLayout::OnInitializeAccessibilityEvent(event);
    AutoPtr<ICharSequence> seq;
    FAIL_RETURN(CStringWrapper::New(String("CTimePicker"), (ICharSequence**)&seq));
    event->SetClassName(seq);
    return NOERROR;
}

ECode TimePicker::OnInitializeAccessibilityNodeInfo(
    /* [in] */ IAccessibilityNodeInfo* info)
{
    PEL("TimePicker::OnInitializeAccessibilityNodeInfo")
    FrameLayout::OnInitializeAccessibilityNodeInfo(info);
    AutoPtr<ICharSequence> seq;
    FAIL_RETURN(CStringWrapper::New(String("CTimePicker"), (ICharSequence**)&seq));
    info->SetClassName(seq);
    return NOERROR;
}

void TimePicker::OnConfigurationChanged(
    /* [in] */ IConfiguration* newConfig)
{
    PEL("TimePicker::OnConfigurationChanged")
    FrameLayout::OnConfigurationChanged(newConfig);
    AutoPtr<ILocale> locale;
    newConfig->GetLocale((ILocale**)&locale);
    SetCurrentLocale(locale);
}

AutoPtr<IParcelable> TimePicker::OnSaveInstanceState()
{
    PEL("TimePicker::OnSaveInstanceState")
    AutoPtr<IParcelable> superState = FrameLayout::OnSaveInstanceState();
    //return new SavedState(superState, getCurrentHour(), getCurrentMinute());
    return NULL;
}

void TimePicker::OnRestoreInstanceState(
    /* [in] */ IParcelable* state)
{
    PEL("TimePicker::OnRestoreInstanceState")
    /*SavedState ss = (SavedState) state;
    super.onRestoreInstanceState(ss.getSuperState());
    setCurrentHour(ss.getHour());
    setCurrentMinute(ss.getMinute());*/
}

ECode TimePicker::SetCurrentLocale(
    /* [in] */ ILocale* locale)
{
    PEL("TimePicker::SetCurrentLocale")
    Boolean res = FALSE;
    locale->Equals(mCurrentLocale, &res);
    if (res) {
        return NOERROR;
    }
    mCurrentLocale = locale;
    AutoPtr<ICalendarHelper> helper;
    CCalendarHelper::AcquireSingleton((ICalendarHelper**)&helper);
    helper->GetInstanceEx(locale, (ICalendar**)&mTempCalendar);

    return NOERROR;
}

ECode TimePicker::UpdateHourControl()
{
    PEL("TimePicker::UpdateHourControl")
    if (Is24HourView()) {
        mHourSpinner->SetMinValue(0);
        mHourSpinner->SetMaxValue(23);
        mHourSpinner->SetFormatter(NumberPicker::GetTwoDigitFormatter());
    } else {
        mHourSpinner->SetMinValue(1);
        mHourSpinner->SetMaxValue(12);
        mHourSpinner->SetFormatter(NULL);
    }
    return NOERROR;
}

ECode TimePicker::UpdateAmPmControl()
{
    PEL("TimePicker::UpdateAmPmControl")
    if (Is24HourView()) {
        if (mAmPmSpinner) {
            mAmPmSpinner->SetVisibility(IView::GONE);
        } else {
            mAmPmButton->SetVisibility(IView::GONE);
        }
    } else {
        Int32 index = mIsAm ? 0 : 1;//ICalendar::AM : ICalendar::PM;
        if (mAmPmSpinner) {
            mAmPmSpinner->SetValue(index);
            mAmPmSpinner->SetVisibility(IView::VISIBLE);
        } else {
            AutoPtr<ICharSequence> seq;
            FAIL_RETURN(CStringWrapper::New((*mAmPmStrings)[index], (ICharSequence**)&seq));
            mAmPmButton->SetText(seq);
            mAmPmButton->SetVisibility(IView::VISIBLE);
        }
    }
    SendAccessibilityEvent(IAccessibilityEvent::TYPE_VIEW_SELECTED);
    return NOERROR;
}

ECode TimePicker::OnTimeChanged()
{
    PEL("TimePicker::OnTimeChanged")
    SendAccessibilityEvent(IAccessibilityEvent::TYPE_VIEW_SELECTED);
    if (mOnTimeChangedListener.Get() != NULL) {
        AutoPtr<IInteger32> currentHour = GetCurrentHour();
        Int32 hour = 0;
        currentHour->GetValue(&hour);

        AutoPtr<IInteger32> currentMinute = GetCurrentMinute();
        Int32 minute = 0;
        currentMinute->GetValue(&minute);

        mOnTimeChangedListener->OnTimeChanged((ITimePicker*)this->Probe(EIID_ITimePicker), hour, minute);
    }
    return NOERROR;
}

ECode TimePicker::SetContentDescriptions()
{
    PEL("TimePicker::SetContentDescriptions")
    TrySetContentDescription(mMinuteSpinner, R::id::increment,
        R::string::time_picker_increment_minute_button);
    TrySetContentDescription(mMinuteSpinner, R::id::decrement,
        R::string::time_picker_decrement_minute_button);
    TrySetContentDescription(mHourSpinner, R::id::increment,
        R::string::time_picker_increment_hour_button);
    TrySetContentDescription(mHourSpinner, R::id::decrement,
        R::string::time_picker_decrement_hour_button);

    if (mAmPmSpinner) {
        TrySetContentDescription(mAmPmSpinner, R::id::increment,
            R::string::time_picker_increment_set_pm_button);
        TrySetContentDescription(mAmPmSpinner, R::id::decrement,
            R::string::time_picker_decrement_set_am_button);
    }
    return NOERROR;
}

ECode TimePicker::TrySetContentDescription(
    /* [in] */ IView* root,
    /* [in] */ Int32 viewId,
    /* [in] */ Int32 contDescResId)
{
    PEL("TimePicker::TrySetContentDescription")
    AutoPtr<IView> target;
    root->FindViewById(viewId, (IView**)&target);

    if(target) {
        String str;
        mContext->GetString(contDescResId, &str);
        AutoPtr<ICharSequence> seq;
        FAIL_RETURN(CStringWrapper::New(str, (ICharSequence**)&seq));
        target->SetContentDescription(seq);
    }
    return NOERROR;
}

ECode TimePicker::UpdateInputState()
{
    PEL("TimePicker::UpdateInputState")
    AutoPtr<IInputMethodManager> inputMethodManager = CInputMethodManager::PeekInstance();
    if (inputMethodManager) {
        Boolean hourActive = FALSE, minuteActive = FALSE, amPmActice = FALSE, res = FALSE;
        inputMethodManager->IsActive(mHourSpinnerInput, &hourActive);
        inputMethodManager->IsActive(mMinuteSpinnerInput, &minuteActive);
        inputMethodManager->IsActive(mAmPmSpinnerInput, &amPmActice);
        if (hourActive) {
            mHourSpinnerInput->ClearFocus();
            inputMethodManager->HideSoftInputFromWindow(GetWindowToken(), 0, &res);
        } else if (minuteActive) {
            mMinuteSpinnerInput->ClearFocus();
            inputMethodManager->HideSoftInputFromWindow(GetWindowToken(), 0, &res);
        } else if (amPmActice) {
            mAmPmSpinnerInput->ClearFocus();
            inputMethodManager->HideSoftInputFromWindow(GetWindowToken(), 0, &res);
        }
    }

    return NOERROR;
}

//============================================================
//          TimePicker::TimePickerOnTimeChangedListener
//============================================================
CAR_INTERFACE_IMPL(TimePicker::TimePickerOnTimeChangedListener, IOnTimeChangedListener)

TimePicker::TimePickerOnTimeChangedListener::TimePickerOnTimeChangedListener()
{
    PEL("TimePickerOnTimeChangedListener::TimePickerOnTimeChangedListener")
}

ECode TimePicker::TimePickerOnTimeChangedListener::OnTimeChanged(
    /* [in] */ ITimePicker* view,
    /* [in] */ Int32 hourOfDay,
    /* [in] */ Int32 minute)
{
    PEL("TimePickerOnTimeChangedListener::OnTimeChanged")
    //do nothing
    return NOERROR;
}

//============================================================
//          TimePicker::HourChangeListener
//============================================================
CAR_INTERFACE_IMPL(TimePicker::HourChangeListener, INumberPickerOnValueChangeListener)

TimePicker::HourChangeListener::HourChangeListener(
    /* [in] */ TimePicker* host)
    : mHost(host)
{
    PEL("HourChangeListener::HourChangeListener")
}

ECode TimePicker::HourChangeListener::OnValueChange(
    /* [in] */ INumberPicker* picker,
    /* [in] */ Int32 oldVal,
    /* [in] */ Int32 newVal)
{
    PEL("HourChangeListener::OnValueChange")
    mHost->UpdateInputState();
    if (!mHost->Is24HourView()) {
        if ((oldVal == mHost->HOURS_IN_HALF_DAY -1 && newVal == mHost->HOURS_IN_HALF_DAY)
            || (oldVal == mHost->HOURS_IN_HALF_DAY && newVal == mHost->HOURS_IN_HALF_DAY -1)) {
            mHost->mIsAm = !mHost->mIsAm;
            mHost->UpdateAmPmControl();
        }
    }
    mHost->OnTimeChanged();
    return NOERROR;
}

//============================================================
//          TimePicker::MinuteChangeListener
//============================================================
CAR_INTERFACE_IMPL(TimePicker::MinuteChangeListener, INumberPickerOnValueChangeListener)

TimePicker::MinuteChangeListener::MinuteChangeListener(
    /* [in] */ TimePicker* host)
    : mHost(host)
{
    PEL("MinuteChangeListener::MinuteChangeListener")
}

ECode TimePicker::MinuteChangeListener::OnValueChange(
    /* [in] */ INumberPicker* picker,
    /* [in] */ Int32 oldVal,
    /* [in] */ Int32 newVal)
{
    PEL("MinuteChangeListener::OnValueChange")
    mHost->UpdateInputState();
    Int32 minValue = 0, maxValue = 0;
    mHost->mMinuteSpinner->GetMinValue(&minValue);
    mHost->mMinuteSpinner->GetMaxValue(&maxValue);
    if (oldVal == maxValue && newVal == minValue) {
        Int32 newHour = 0;
        mHost->mHourSpinner->GetValue(&newHour);
        ++newHour;
        if (!mHost->Is24HourView() && newHour == mHost->HOURS_IN_HALF_DAY) {
            mHost->mIsAm = !mHost->mIsAm;
            mHost->UpdateAmPmControl();
        }
        mHost->mHourSpinner->SetValue(newHour);
    } else if (oldVal == minValue && newVal == maxValue) {
        Int32 newHour = 0;
        mHost->mHourSpinner->GetValue(&newHour);
        --newHour;
        if (!mHost->Is24HourView() && newHour == HOURS_IN_HALF_DAY - 1) {
            mHost->mIsAm = !mHost->mIsAm;
            mHost->UpdateAmPmControl();
        }
        mHost->mHourSpinner->SetValue(newHour);
    }
    mHost->OnTimeChanged();
    return NOERROR;
}

//============================================================
//          TimePicker::AmPmChangeListener
//============================================================
CAR_INTERFACE_IMPL(TimePicker::AmPmChangeListener, INumberPickerOnValueChangeListener)

TimePicker::AmPmChangeListener::AmPmChangeListener(
    /* [in] */ TimePicker* host)
    : mHost(host)
{
    PEL("AmPmChangeListener::AmPmChangeListener")
}

ECode TimePicker::AmPmChangeListener::OnValueChange(
    /* [in] */ INumberPicker* picker,
    /* [in] */ Int32 oldVal,
    /* [in] */ Int32 newVal)
{
    PEL("AmPmChangeListener::OnValueChange")
    Boolean res = FALSE;
    mHost->UpdateInputState();
    picker->RequestFocus(&res);
    mHost->mIsAm = !mHost->mIsAm;
    mHost->UpdateAmPmControl();
    mHost->OnTimeChanged();
    return NOERROR;
}

//============================================================
//          TimePicker::AmPmClickListener
//============================================================
CAR_INTERFACE_IMPL(TimePicker::AmPmClickListener, IViewOnClickListener)

TimePicker::AmPmClickListener::AmPmClickListener(
    /* [in] */ TimePicker* host)
    : mHost(host)
{
    PEL("AmPmChangeListener::AmPmClickListener")
}

ECode TimePicker::AmPmClickListener::OnClick(
    /* [in] */ IView* v)
{
    PEL("AmPmChangeListener::OnClick")
    Boolean res = FALSE;
    v->RequestFocus(&res);
    mHost->mIsAm = !mHost->mIsAm;
    mHost->UpdateAmPmControl();
    mHost->OnTimeChanged();
    return NOERROR;
}


} // namespace Widget
} // namepsace Droid
} // namespace Elastos

