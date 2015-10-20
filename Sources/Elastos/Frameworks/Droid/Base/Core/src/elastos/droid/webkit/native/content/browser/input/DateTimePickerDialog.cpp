
#include "elastos/droid/webkit/native/content/browser/input/DateTimePickerDialog.h"
#include "elastos/droid/webkit/native/content/browser/input/DateDialogNormalizer.h"

using Elastos::Core::ICharSequence;
using Elastos::Core::IInteger32;
// TODO using Elastos::Core::CInteger32;
using Elastos::Droid::App::EIID_IAlertDialog;
using Elastos::Droid::Content::EIID_IDialogInterfaceOnClickListener;
using Elastos::Droid::Os::IMessage;
using Elastos::Droid::View::IView;
using Elastos::Droid::View::EIID_IView;
using Elastos::Droid::Widget::EIID_IOnTimeChangedListener;
using Elastos::Droid::Widget::EIID_IDatePickerOnDateChangedListener;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Content {
namespace Browser {
namespace Input {

CAR_INTERFACE_IMPL_4(DateTimePickerDialog, Object, IAlertDialog,
        IDialogInterfaceOnClickListener, IDatePickerOnDateChangedListener, IOnTimeChangedListener);

/**
 * @param context The context the dialog is to run in.
 * @param callBack How the parent is notified that the date is set.
 * @param year The initial year of the dialog.
 * @param monthOfYear The initial month of the dialog.
 * @param dayOfMonth The initial day of the dialog.
 */
DateTimePickerDialog::DateTimePickerDialog(
    /* [in] */ IContext* context,
    /* [in] */ DateTimePickerDialog::OnDateTimeSetListener* callBack,
    /* [in] */ Int32 year,
    /* [in] */ Int32 monthOfYear,
    /* [in] */ Int32 dayOfMonth,
    /* [in] */ Int32 hourOfDay,
    /* [in] */ Int32 minute,
    /* [in] */ Boolean is24HourView,
    /* [in] */ Double min,
    /* [in] */ Double max)
    // TODO : AlertDialog(context, 0)
    : mMinTimeMillis((Int64)min)
    , mMaxTimeMillis((Int64)max)
    , mCallBack(callBack)
{
    AutoPtr<ICharSequence> setCS;
    assert(0);
    // TODO
    // context->GetText(
    //         R::string::date_picker_dialog_set, (ICharSequence**)&setCS);
    SetButton(IDialogInterface::BUTTON_POSITIVE, setCS, this);
    AutoPtr<ICharSequence> cancelCS;
    assert(0);
    // TODO
    // context->GetText(android::R::string::cancel, (ICharSequence**)&cancelCS);
    SetButton(IDialogInterface::BUTTON_NEGATIVE, cancelCS,
            (IMessage*)NULL);
    SetIcon(0);
    AutoPtr<ICharSequence> titleCS;
    assert(0);
    // TODO
    // context->GetText(R::string::date_time_picker_dialog_title, (ICharSequence**)&titleCS);
    // SetTitle(titleCS);

    AutoPtr<ILayoutInflater> inflater;
    context->GetSystemService(IContext::LAYOUT_INFLATER_SERVICE, (IInterface**)&inflater);
    AutoPtr<IView> view;
    assert(0);
    // TODO
    // inflater->Inflate(R::layout::date_time_picker_dialog, NULL, (IView**)&view);
    SetView(view);
    assert(0);
    // TODO
    // view->FindViewById(R::id::date_picker, (IView**)&mDatePicker);
    DateDialogNormalizer::Normalize(mDatePicker, this,
            year, monthOfYear, dayOfMonth, hourOfDay, minute, mMinTimeMillis, mMaxTimeMillis);

    assert(0);
    // TODO
    // view->FindViewById(R::id::time_picker, (IView**)&mTimePicker);
    mTimePicker->SetIs24HourView(is24HourView);
    AutoPtr<IInteger32> iHourOfDay;
    assert(0);
    // TODO
    // CInteger32::New(hourOfDay, (IInteger32**)&iHourOfDay);
    mTimePicker->SetCurrentHour(iHourOfDay);
    AutoPtr<IInteger32> iMinute;
    assert(0);
    // TODO
    // CInteger32::New(minute, (IInteger32**)&iMinute);
    mTimePicker->SetCurrentMinute(iMinute);
    mTimePicker->SetOnTimeChangedListener(this);
    AutoPtr<IInteger32> iCurrentHour, iCurrentMinute;
    mTimePicker->GetCurrentHour((IInteger32**)&iCurrentHour);
    mTimePicker->GetCurrentMinute((IInteger32**)&iCurrentMinute);
    Int32 currentHour, currentMinute;
    iCurrentHour->GetValue(&currentHour);
    iCurrentMinute->GetValue(&currentMinute);
    OnTimeChanged(mTimePicker, currentHour,
            currentMinute);
}

//@Override
ECode DateTimePickerDialog::OnClick(
    /* [in] */ IDialogInterface* dialog,
    /* [in] */ Int32 which)
{
    return TryNotifyDateTimeSet();
}

ECode DateTimePickerDialog::TryNotifyDateTimeSet()
{
    if (mCallBack != NULL) {
        AutoPtr<IView> view = (IView*)mDatePicker->Probe(EIID_IView);
        view->ClearFocus();
        Int32 year, month, dayOfMonth, currentHour, currentMinute;
        mDatePicker->GetYear(&year);
        mDatePicker->GetMonth(&month);
        mDatePicker->GetDayOfMonth(&dayOfMonth);
        AutoPtr<IInteger32> iCurrentHour;
        mTimePicker->GetCurrentHour((IInteger32**)&iCurrentHour);
        iCurrentHour->GetValue(&currentHour);
        AutoPtr<IInteger32> iCurrentMinute;
        mTimePicker->GetCurrentMinute((IInteger32**)&iCurrentMinute);
        iCurrentMinute->GetValue(&currentMinute);
        mCallBack->OnDateTimeSet(mDatePicker, mTimePicker, year,
                month, dayOfMonth, currentHour, currentMinute);
    }

    return NOERROR;
}

//@Override
ECode DateTimePickerDialog::OnDateChanged(
    /* [in] */ IDatePicker* view,
    /* [in] */ Int32 year,
    /* [in] */ Int32 month,
    /* [in] */ Int32 day)
{
    // Signal a time change so the max/min checks can be applied.
    if (mTimePicker != NULL) {
        Int32 hour, minute;
        AutoPtr<IInteger32> iHour;
        mTimePicker->GetCurrentHour((IInteger32**)&iHour);
        iHour->GetValue(&hour);
        AutoPtr<IInteger32> iMinute;
        mTimePicker->GetCurrentMinute((IInteger32**)&iMinute);
        iMinute->GetValue(&minute);
        OnTimeChanged(mTimePicker, hour, minute);
    }

    return NOERROR;
}

//@Override
ECode DateTimePickerDialog::OnTimeChanged(
    /* [in] */ ITimePicker* view,
    /* [in] */ Int32 hourOfDay,
    /* [in] */ Int32 minute)
{
    AutoPtr<ITime> time;
    assert(0);
    // TODO
    // CTime::New((ITime**)&time);
    Int32 currentMinute, currentHour, dayOfMonth, month, year;
    AutoPtr<IInteger32> iCurrentMinute;
    mTimePicker->GetCurrentMinute((IInteger32**)&iCurrentMinute);
    iCurrentMinute->GetValue(&currentMinute);
    AutoPtr<IInteger32> iCurrentHour;
    mTimePicker->GetCurrentHour((IInteger32**)&iCurrentHour);
    iCurrentHour->GetValue(&currentHour);
    mDatePicker->GetDayOfMonth(&dayOfMonth);
    mDatePicker->GetMonth(&month);
    mDatePicker->GetYear(&year);
    time->Set(0, currentMinute,
            currentHour, dayOfMonth,
            month, year);

    Int64 millis;
    time->ToMillis(TRUE, &millis);
    if (millis < mMinTimeMillis) {
        time->Set(mMinTimeMillis);
    }
    else if (millis > mMaxTimeMillis) {
        time->Set(mMaxTimeMillis);
    }

    Int32 tHour, tMinute;
    time->GetHour(&tHour);
    time->GetMinute(&tMinute);
    AutoPtr<IInteger32> itHour;
    assert(0);
    // TODO
    // CInteger32::New(tHour, (IInteger32**)&itHour);
    mTimePicker->SetCurrentHour(itHour);
    AutoPtr<IInteger32> itMinute;
    assert(0);
    // TODO
    // CInteger32::New(tMinute, (IInteger32**)&itMinute);
    mTimePicker->SetCurrentMinute(itMinute);

    return NOERROR;
}

/**
 * Sets the current date.
 *
 * @param year The date year.
 * @param monthOfYear The date month.
 * @param dayOfMonth The date day of month.
 */
void DateTimePickerDialog::UpdateDateTime(
    /* [in] */ Int32 year,
    /* [in] */ Int32 monthOfYear,
    /* [in] */ Int32 dayOfMonth,
    /* [in] */ Int32 hourOfDay,
    /* [in] */ Int32 minuteOfHour)
{
    mDatePicker->UpdateDate(year, monthOfYear, dayOfMonth);
    AutoPtr<IInteger32> iHourOfDay;
    assert(0);
    // TODO
    // CInteger32::New(hourOfDay, (IInteger32**)&iHourOfDay);
    mTimePicker->SetCurrentHour(iHourOfDay);
    AutoPtr<IInteger32> iMinuteOfHour;
    assert(0);
    // TODO
    // CInteger32::New(minuteOfHour, (IInteger32**)&iMinuteOfHour);
    mTimePicker->SetCurrentMinute(iMinuteOfHour);
}

} // namespace Input
} // namespace Browser
} // namespace Content
} // namespace Webkit
} // namespace Droid
} // namespace Elastos
