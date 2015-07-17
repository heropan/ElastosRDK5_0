
namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Content {
namespace Browser {
namespace Input {

//==================================================================
//    InputDialogContainer::InnerAdapterViewOnItemClickListener
//==================================================================

InputDialogContainer::InnerAdapterViewOnItemClickListener::InnerAdapterViewOnItemClickListener(
    /* [in] */ InputDialogContainer* owner,
    /* [in] */ Int32 dialogType,
    /* [in] */ Double dialogValue,
    /* [in] */ Double min,
    /* [in] */ Double max,
    /* [in] */ Double step,
    /* [in] */ DateTimeSuggestionListAdapter* adapter)
    : mOwner(owner)
    , mDialogType(dialogType)
    , mDialogValue(dialogValue)
    , mMin(min)
    , mMax(max)
    , mStep(step)
    , mAdapter(adapter)
{
}

ECode InputDialogContainer::InnerAdapterViewOnItemClickListener::OnItemClick(
    /* [in] */ IAdapterView* parent,
    /* [in] */ IView* view,
    /* [in] */ Int32 position,
    /* [in] */ Int64 id)
{
    if (position == mAdapter->GetCount() - 1) {
        mOwner->DismissDialog();
        mOwner->ShowPickerDialog(mMdialogType, mDialogValue, mMin, mMax, mStep);
    }
    else {
        Double suggestionValue = mAdapter->GetItem(mPosition).value();
        mOwner->mInputActionDelegate->ReplaceDateTime(suggestionValue);
        mOwner->DismissDialog();
        mOwner->mDialogAlreadyDismissed = TRUE;
    }

    return NOERROR;
}

//=========================================================================
//        InputDialogContainer::InnerDialogInterfaceOnClickListener
//=========================================================================

InputDialogContainer::InnerDialogInterfaceOnClickListener::InnerDialogInterfaceOnClickListener(
    /* [in] */ InputDialogContainer* owner)
    : mOwner(owner)
{
}

ECode InputDialogContainer::InnerDialogInterfaceOnClickListener::OnClick(
    /* [in] */ IDialogInterface* dialog,
    /* [in] */ Int32 which)
{
    mOwner->DismissDialog();
    return NOERROR;
}

//=========================================================================
//        InputDialogContainer::InnerDialogInterfaceOnDismissListener
//=========================================================================

InputDialogContainer::InnerDialogInterfaceOnDismissListener::InnerDialogInterfaceOnDismissListener(
    /* [in] */ InputDialogContainer* owner)
    : mOwner(owner)
{
}

ECode InputDialogContainer::InnerDialogInterfaceOnDismissListener::OnDismiss(
    /* [in] */ IDialogInterface* dialog)
{
    if (mOwner->mDialog == dialog && !mOwner->mDialogAlreadyDismissed) {
        mOwner->mDialogAlreadyDismissed = TRUE;
        mOwner->mInputActionDelegate->CancelDateTimeDialog();
    }

    return NOERROR;
}

//=============================================================================
//       InputDialogContainer::SetButtonDialogInterfaceOnClickListener
//=============================================================================

InputDialogContainer::SetButtonDialogInterfaceOnClickListener::ISetButtonDialogInterfaceOnClickListener(
    /* [in] */ InputDialogContainer* owner)
    : mOwner(owner)
{
}

ECode InputDialogContainer::SetButtonDialogInterfaceOnClickListener::OnClick(
    /* [in] */ IDialogInterface* dialog,
    /* [in] */ Int32 which)
{
    mOwner->mDialogAlreadyDismissed = TRUE;
    mOwner->mInputActionDelegate->ReplaceDateTime(Double.NaN);
    return NOERROR;
}

//================================================================================================
//         InputDialogContainer::SetOnDismissListenerDialogInterfaceOnDismissListener
//================================================================================================

InputDialogContainer::SetOnDismissListenerDialogInterfaceOnDismissListener::SetOnDismissListenerDialogInterfaceOnDismissListener(
    /* [in] */ InputDialogContainer* owner)
    : mOwner(owner)
{
}

ECode InputDialogContainer::SetOnDismissListenerDialogInterfaceOnDismissListener::OnDismiss(
    /* [in] */ IDialogInterface* dialog)
{
    if (!mOwner->mDialogAlreadyDismissed) {
        mOwner->mDialogAlreadyDismissed = TRUE;
        mOwner->mInputActionDelegate->CancelDateTimeDialog();
    }

    return NOERROR;
}

//==================================================================
//              InputDialogContainer::DateListener
//==================================================================

InputDialogContainer::DateListener::DateListener(
    /* [in] */ InputDialogContainer* owner,
    /* [in] */ Int32 dialogType)
    : mOwner(owner)
    , mDialogType(dialogType)
{
}

//@Override
ECode InputDialogContainer::DateListener::OnDateSet(
    /* [in] */ IDatePicker* view,
    /* [in] */ Int32 year,
    /* [in] */ Int32 month,
    /* [in] */ Int32 monthDay)
{
    mOwner->SetFieldDateTimeValue(mDialogType, year, month, monthDay, 0, 0, 0, 0, 0);
    return NOERROR;
}

//==================================================================
//              InputDialogContainer::FullTimeListener
//==================================================================

InputDialogContainer::FullTimeListener::FullTimeListener(
    /* [in] */ InputDialogContainer* owner,
    /* [in] */ Int32 dialogType)
    : mOwner(owner)
    , mDialogType(dialogType)
{
}

//@Override
ECode InputDialogContainer::FullTimeListener::OnTimeSet(
    /* [in] */ Int32 hourOfDay,
    /* [in] */ Int32 minute,
    /* [in] */ Int32 second,
    /* [in] */ Int32 milli)
{
    mOwner->SetFieldDateTimeValue(mDialogType, 0, 0, 0, hourOfDay, minute, second, milli, 0);
    return NOERROR;
}

//==================================================================
//             InputDialogContainer::DateTimeListener
//==================================================================

private class DateTimeListener implements OnDateTimeSetListener {
private final boolean mLocal;
private final int mDialogType;

InputDialogContainer::DateTimeListener::DateTimeListener(
    /* [in] */ InputDialogContainer* owner,
    /* [in] */ Int32 dialogType)
    : mOwner(owner)
    , mLocal(dialogType == sTextInputTypeDateTimeLocal)
    , mDialogType(dialogType)
{
}

//@Override
ECode InputDialogContainer::DateTimeListener::OnDateTimeSet(
    /* [in] */ IDatePicker* dateView,
    /* [in] */ ITimePicker* timeView,
    /* [in] */ Int32 year,
    /* [in] */ Int32 month,
    /* [in] */ Int32 monthDay,
    /* [in] */ Int32 hourOfDay,
    /* [in] */ Int32 minute)
{
    SetFieldDateTimeValue(mDialogType, year, month, monthDay, hourOfDay, minute, 0, 0, 0);
    return NOERROR;
}

//==================================================================
//           InputDialogContainer::MonthOrWeekListener
//==================================================================

InputDialogContainer::MonthOrWeekListener::MonthOrWeekListener(
    /* [in] */ InputDialogContainer* owner,
    /* [in] */ Int32 dialogType)
    : mOwner(owner)
    , mDialogType(dialogType)
{
}

//@Override
void InputDialogContainer::MonthOrWeekListener::OnValueSet(
    /* [in] */ Int32 year,
    /* [in] */ Int32 positionInYear)
{
    if (mDialogType == sTextInputTypeMonth) {
        SetFieldDateTimeValue(mDialogType, year, positionInYear, 0, 0, 0, 0, 0, 0);
    }
    else {
        SetFieldDateTimeValue(mDialogType, year, 0, 0, 0, 0, 0, 0, positionInYear);
    }
}

//==================================================================
//                       InputDialogContainer
//==================================================================

Int32 InputDialogContainer::sTextInputTypeDate = 0;
Int32 InputDialogContainer::sTextInputTypeDateTime = 0;
Int32 InputDialogContainer::sTextInputTypeDateTimeLocal = 0;
Int32 InputDialogContainer::sTextInputTypeMonth = 0;
Int32 InputDialogContainer::sTextInputTypeTime = 0;
Int32 InputDialogContainer::sTextInputTypeWeek = 0;

InputDialogContainer::InputDialogContainer(
    /* [in] */ IContext* context,
    /* [in] */ InputActionDelegate inputActionDelegate)
    : mDialogAlreadyDismissed(FALSE)
    , mContext(context)
    , mInputActionDelegate(inputActionDelegate)
{
}

void InputDialogContainer::InitializeInputTypes(
    /* [in] */ Int32 textInputTypeDate,
    /* [in] */ Int32 textInputTypeDateTime,
    /* [in] */ Int32 textInputTypeDateTimeLocal,
    /* [in] */ Int32 textInputTypeMonth,
    /* [in] */ Int32 textInputTypeTime,
    /* [in] */ Int32 textInputTypeWeek)
{
    sTextInputTypeDate = textInputTypeDate;
    sTextInputTypeDateTime = textInputTypeDateTime;
    sTextInputTypeDateTimeLocal = textInputTypeDateTimeLocal;
    sTextInputTypeMonth = textInputTypeMonth;
    sTextInputTypeTime = textInputTypeTime;
    sTextInputTypeWeek = textInputTypeWeek;
}

Boolean InputDialogContainer::IsDialogInputType(
    /* [in] */ Int32 type)
{
    return type == sTextInputTypeDate || type == sTextInputTypeTime
            || type == sTextInputTypeDateTime || type == sTextInputTypeDateTimeLocal
            || type == sTextInputTypeMonth || type == sTextInputTypeWeek;
}

void InputDialogContainer::ShowPickerDialog(
    /* [in] */ Int32 dialogType,
    /* [in] */ Double dialogValue,
    /* [in] */ Double min,
    /* [in] */ Double max,
    /* [in] */ Double step)
{
    AutoPtr<ICalendar> cal;
    // |dialogValue|, |min|, |max| mean different things depending on the |dialogType|.
    // For input type=month is the number of months since 1970.
    // For input type=time it is milliseconds since midnight.
    // For other types they are just milliseconds since 1970.
    // If |dialogValue| is NaN it means an empty value. We will show the current time.
    if (Double.isNaN(dialogValue)) {
        AutoPtr<ICalendarHelper> helper;
        CCalendarHelper::Acq((ICalendarHelper**)&helper);
        helper->GetInstance((ICalendar**)&cal);
        cal->Set(ICalendar::MILLISECOND, 0);
    }
    else {
        if (dialogType == sTextInputTypeMonth) {
            cal = MonthPicker::CreateDateFromValue(dialogValue);
        }
        else if (dialogType == sTextInputTypeWeek) {
            cal = WeekPicker.createDateFromValue(dialogValue);
        }
        else {
            AutoPtr<IGregorianCalendar> gregorianCalendar;
            CGregorianCalendar::New(TimeZone::GetTimeZone(String("UTC")), (IGregorianCalendar**)&gregorianCalendar);
            // According to the HTML spec we only use the Gregorian calendar
            // so we ignore the Julian/Gregorian transition.
            AutoPtr<IDate> date;
            CDate::New(Long.MIN_VALUE, (IDate**)&date);
            gregorianCalendar->SetGregorianChange(date);
            gregorianCalendar->SetTimeInMillis((Int64) dialogValue);
            cal =  gregorianCalendar;
        }
    }
    if (dialogType == sTextInputTypeDate) {
        Int32 year, month, dayOfMonth;
        cal->Get(ICalendar::YEAR, &year);
        cal->Get(ICalendar::MONTH, &month);
        cal->Get(ICalendar::DAY_OF_MONTH, &dayOfMonth);
        ShowPickerDialog(dialogType,
                year,
                month,
                dayOfMonth,
                0, 0, 0, 0, 0, min, max, step);
    }
    else if (dialogType == sTextInputTypeTime) {
        Int32 hourOfDay, minute;
        cal->Get(ICalendar::HOUR_OF_DAY, &hourOfDay);
        cal->Get(ICalendar::MINUTE, &minute);
        ShowPickerDialog(dialogType, 0, 0, 0,
                hourOfDay,
                minute,
                0, 0, 0, min, max, step);
    }
    else if (dialogType == sTextInputTypeDateTime ||
            dialogType == sTextInputTypeDateTimeLocal) {
        Int32 year, month, dayOfMonth, hourOfDay, second;
        cal->Get(ICalendar::YEAR, &year);
        cal->Get(ICalendar::MONTH, &month);
        cal->Get(ICalendar::DAY_OF_MONTH, &dayOfMonth);
        cal->Get(ICalendar::HOUR_OF_DAY, &hourOfDay);
        cal->Get(ICalendar::MINUTE, &minute);
        cal->Get(ICalendar::SECOND, &second);
        Int64 millisecond;
        cal->Get(ICalendar::MILLISECOND, &millisecond);
        ShowPickerDialog(dialogType,
                year,
                month,
                dayOfMonth,
                hourOfDay,
                minute,
                second,
                millisecond,
                0, min, max, step);
    }
    else if (dialogType == sTextInputTypeMonth) {
        Int32 year, month;
        cal->Get(ICalendar::YEAR, &year);
        cal->Get(ICalendar::MONTH, &month);
        ShowPickerDialog(dialogType, year, month, 0,
                0, 0, 0, 0, 0, min, max, step);
    }
    else if (dialogType == sTextInputTypeWeek) {
        Int32 year = WeekPicker::GetISOWeekYearForDate(cal);
        Int32 week = WeekPicker::GetWeekForDate(cal);
        ShowPickerDialog(dialogType, year, 0, 0, 0, 0, 0, 0, week, min, max, step);
    }
}

void InputDialogContainer::ShowSuggestionDialog(
    /* [in] */ Int32 dialogType,
    /* [in] */ Double dialogValue,
    /* [in] */ Double min,
    /* [in] */ Double max,
    /* [in] */ Double step,
    /* [in] */ ArrayOf<DateTimeSuggestion>* suggestions)
{
    AutoPtr<IListView> suggestionListView;
    CListView::New(mContext, (IListView**)&suggestionListView);
    AutoPtr<IList> list;
    Arrays::AsList(suggestions, (IList**)&list);
    const AutoPtr<DateTimeSuggestionListAdapter> adapter =
        new DateTimeSuggestionListAdapter(mContext, list);
    suggestionListView->SetAdapter(adapter);
    AutoPtr<AdapterViewOnItemClickListener> listener = new InnerAdapterViewOnItemClickListener(
           this, dialogType, dialogValue, min, max, step, adapter);
    suggestionListView->SetOnItemClickListener(listener);

    Int32 dialogTitleId = R::string::date_picker_dialog_title;
    if (dialogType == sTextInputTypeTime) {
        dialogTitleId = R::string::time_picker_dialog_title;
    }
    else if (dialogType == sTextInputTypeDateTime ||
            dialogType == sTextInputTypeDateTimeLocal) {
        dialogTitleId = R::string::date_time_picker_dialog_title;
    }
    else if (dialogType == sTextInputTypeMonth) {
        dialogTitleId = R::string::month_picker_dialog_title;
    }
    else if (dialogType == sTextInputTypeWeek) {
        dialogTitleId = R::string::week_picker_dialog_title;
    }

    AutoPtr<IAlertDialogBuilder> dialogBuilder;
    CAlertDialogBuilder::New(mContext, (IAlertDialogBuilder**)&dialogBuilder);
    dialogBuilder->SetTitle(dialogTitleId);
    dialogBuilder->SetView(suggestionListView);
    AutoPtr<ICharSequence> text;
    mContext->GetText(android::R::string::cancel, (ICharSequence**)&text);
    AutoPtr<IDialogInterfaceOnClickListener> clickListener =  new InnerDialogInterfaceOnClickListener(this);
    dialogBuilder->SetNegativeButton(text, clickListener);
    dialogBuilder->Create((IAlertDialog**)&mDialog);

    AutoPtr<InnerDialogInterfaceOnDismissListener> dismissListener = new InnerDialogInterfaceOnDismissListener(this);
    mDialog->SetOnDismissListener(dismissListener);
    mDialogAlreadyDismissed = FALSE;
    mDialog->Show();
}

void InputDialogContainer::ShowDialog(
    /* [in] */ Int32 type,
    /* [in] */ Double value,
    /* [in] */ Double min,
    /* [in] */ Double max,
    /* [in] */ Double step,
    /* [in] */ ArrayOf<DateTimeSuggestion>* suggestions)
{
    // When the web page asks to show a dialog while there is one already open,
    // dismiss the old one.
    DismissDialog();
    if (suggestions == NULL) {
        ShowPickerDialog(type, value, min, max, step);
    }
    else {
        ShowSuggestionDialog(type, value, min, max, step, suggestions);
    }
}

void InputDialogContainer::ShowPickerDialog(
    /* [in] */ Int32 dialogType,
    /* [in] */ Int32 year,
    /* [in] */ Int32 month,
    /* [in] */ Int32 monthDay,
    /* [in] */ Int32 hourOfDay,
    /* [in] */ Int32 minute,
    /* [in] */ Int32 second,
    /* [in] */ Int32 millis,
    /* [in] */ Int32 week,
    /* [in] */ Double min,
    /* [in] */ Double max,
    /* [in] */ Double step)
{
    if (IsDialogShowing()) mDialog->Dismiss();

    Int32 stepTime = (Int32) step;

    if (dialogType == sTextInputTypeDate) {
        AutoPtr<DateListener> dateListener = new DateListener(dialogType);
        AutoPtr<ChromeDatePickerDialog> dialog = new ChromeDatePickerDialog(mContext,
                dateListener,
                year, month, monthDay);
        DateDialogNormalizer::normalize(dialog->GetDatePicker(), dialog,
                year, month, monthDay,
                0, 0,
                (Int64) min, (Int64) max);

        AutoPtr<ICharSequence> text;
        mContext->GetText(R::string::date_picker_dialog_title, (ICharSequence**)&text);
        dialog->SetTitle(text);
        mDialog = dialog;
    }
    else if (dialogType == sTextInputTypeTime) {
        AutoPtr<FullTimeListener> listener = FullTimeListener(dialogType);
        mDialog = new MultiFieldTimePickerDialog(
            mContext, 0 /* theme */ ,
            hourOfDay, minute, second, millis,
            (Int32) min, (Int32) max, stepTime,
            DateFormat::Is24HourFormat(mContext),
            listener);
    }
    else if (dialogType == sTextInputTypeDateTime ||
            dialogType == sTextInputTypeDateTimeLocal) {
        AutoPtr<DateTimeListener> listener = new DateTimeListener(dialogType);
        mDialog = new DateTimePickerDialog(mContext,
                listener,
                year, month, monthDay,
                hourOfDay, minute,
                DateFormat::Is24HourFormat(mContext), min, max);
    }
    else if (dialogType == sTextInputTypeMonth) {
        AutoPtr<MonthOrWeekListener> listener = new MonthOrWeekListener(this, dialogType);
        mDialog = new MonthPickerDialog(mContext, listener,
                year, month, min, max);
    }
    else if (dialogType == sTextInputTypeWeek) {
        AutoPtr<MonthOrWeekListener> listener = new MonthOrWeekListener(dialogType);
        mDialog = new WeekPickerDialog(mContext, listener,
                year, week, min, max);
    }

    if (ApiCompatibilityUtils::DatePickerRequiresAccept()) {
        AutoPtr<ICharSequence> text;
        mContext->GetText(R::string::date_picker_dialog_set, (ICharSequence**)&text);
        mDialog->SetButton(IDialogInterface::BUTTON_POSITIVE,
                text,
                (IDialogInterfaceOnClickListener*) mDialog);
    }

    AutoPtr<ICharSequence> text1;
    mContext->GetText(android::R::string::cancel, (ICharSequence**)&text1);
    mDialog->SetButton(IDialogInterface::BUTTON_NEGATIVE,
            text1,
            (IDialogInterfaceOnClickListener*) NULL);

    AutoPtr<ICharSequence> text2;
    mContext->GetText(R::string::date_picker_dialog_clear, (ICharSequence**)&text2);
    AutoPtr<IDialogInterfaceOnClickListener> clickListener = new SetButtonDialogInterfaceOnClickListener(this);
    mDialog->SetButton(IDialogInterface::BUTTON_NEUTRAL,
            text2,
            clickListener);

    AutoPtr<SetOnDismissListenerDialogInterfaceOnDismissListener> onDismissListener =
            new SetOnDismissListenerDialogInterfaceOnDismissListener(this);
    mDialog->SetOnDismissListener(onDismissListener);

    mDialogAlreadyDismissed = FALSE;
    mDialog->Show();
}

Boolean InputDialogContainer::IsDialogShowing()
{
    Boolean bFlag = FALSE;
    return mDialog != NULL && (mDialog->IsShowing(&bFlag), bFlag);
}

void InputDialogContainer::DismissDialog()
{
    if (IsDialogShowing()) {
        mDialog->Dismiss();
    }
}

void InputDialogContainer::SetFieldDateTimeValue(
    /* [in] */ Int32 dialogType,
    /* [in] */ Int32 year,
    /* [in] */ Int32 month,
    /* [in] */ Int32 monthDay,
    /* [in] */ Int32 hourOfDay,
    /* [in] */ Int32 minute,
    /* [in] */ Int32 second,
    /* [in] */ Int32 millis,
    /* [in] */ Int32 week)
{
    // Prevents more than one callback being sent to the native
    // side when the dialog triggers multiple events.
    if (mDialogAlreadyDismissed)
        return;

    mDialogAlreadyDismissed = TRUE;

    if (dialogType == sTextInputTypeMonth) {
        mInputActionDelegate->ReplaceDateTime((year - 1970) * 12 + month);
    }
    else if (dialogType == sTextInputTypeWeek) {
        mInputActionDelegate->ReplaceDateTime(
              WeekPicker::CreateDateFromWeek(year, week)->GetTimeInMillis());
    }
    else if (dialogType == sTextInputTypeTime) {
        mInputActionDelegate->ReplaceDateTime(TimeUnit.HOURS.toMillis(hourOfDay) +
                                             TimeUnit.MINUTES.toMillis(minute) +
                                             TimeUnit.SECONDS.toMillis(second) +
                                             millis);
    }
    else {
        AutoPtr<ICalendarHelper> helper;
        CCalendarHelper::AcquireSingleton((ICalendarHelper**)&helper);
        AutoPtr<ICalendar> cal;
        helper->GetInstance(TimeZone::GetTimeZone(String("UTC")), (ICalendar**)&cal);
        cal->Clear();
        cal->Set(ICalendar::YEAR, year);
        cal->Set(ICalendar::MONTH, month);
        cal->Set(ICalendar::DAY_OF_MONTH, monthDay);
        cal->Set(ICalendar::HOUR_OF_DAY, hourOfDay);
        cal->Set(ICalendar::MINUTE, minute);
        cal->Set(ICalendar::SECOND, second);
        cal->Set(ICalendar::MILLISECOND, millis);
        Int64 timeInMillis;
        cal->GetTimeInMillis(&timeInMillis);
        mInputActionDelegate->ReplaceDateTime(timeInMillis);
    }
}

} // namespace Input
} // namespace Browser
} // namespace Content
} // namespace Webkit
} // namespace Droid
} // namespace Elastos
