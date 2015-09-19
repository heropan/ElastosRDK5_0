
#ifndef __ELASTOS_DROID_WEBKIT_CONTENT_BROWSER_INPUT_DATETIMEPICKERDIALOG_H__
#define __ELASTOS_DROID_WEBKIT_CONTENT_BROWSER_INPUT_DATETIMEPICKERDIALOG_H__

// import android.app.AlertDialog;
// import android.content.Context;
// import android.content.DialogInterface;
// import android.content.DialogInterface.OnClickListener;
// import android.text.format.Time;
// import android.view.LayoutInflater;
// import android.view.View;
// import android.widget.DatePicker;
// import android.widget.DatePicker.OnDateChangedListener;
// import android.widget.TimePicker;
// import android.widget.TimePicker.OnTimeChangedListener;

// import org.chromium.content.R;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Content {
namespace Browser {
namespace Input {

class DateTimePickerDialog
    : public IAlertDialog
    , public IOnClickListener
    , public IOnDateChangedListener
    , public IOnTimeChangedListener
{
public:
    /**
     * The callback used to indicate the user is done filling in the date.
     */
    class OnDateTimeSetListener
    {
    public:
        /**
         * @param dateView The DatePicker view associated with this listener.
         * @param timeView The TimePicker view associated with this listener.
         * @param year The year that was set.
         * @param monthOfYear The month that was set (0-11) for compatibility
         *            with {@link java.util.Calendar}.
         * @param dayOfMonth The day of the month that was set.
         * @param hourOfDay The hour that was set.
         * @param minute The minute that was set.
         */
        virtual CARAPI_(void) OnDateTimeSet(
            /* [in] */ IDatePicker* dateView,
            /* [in] */ ITimePicker* timeView,
            /* [in] */ Int32 year,
            /* [in] */ Int32 monthOfYear,
            /* [in] */ Int32 dayOfMonth,
            /* [in] */ Int32 hourOfDay,
            /* [in] */ Int32 minute) = 0;
    };

public:
    /**
     * @param context The context the dialog is to run in.
     * @param callBack How the parent is notified that the date is set.
     * @param year The initial year of the dialog.
     * @param monthOfYear The initial month of the dialog.
     * @param dayOfMonth The initial day of the dialog.
     */
    DateTimePickerDialog(
        /* [in] */ IContext* context,
        /* [in] */ IOnDateTimeSetListener* callBack,
        /* [in] */ Int32 year,
        /* [in] */ Int32 monthOfYear,
        /* [in] */ Int32 dayOfMonth,
        /* [in] */ Int32 hourOfDay,
        /* [in] */ Int32 minute,
        /* [in] */ Boolean is24HourView,
        /* [in] */ Double min,
        /* [in] */ Double max);

    //@Override
    CARAPI OnClick(
        /* [in] */ IDialogInterface* dialog,
        /* [in] */ Int32 which);

    //@Override
    CARAPI OnDateChanged(
        /* [in] */ IDatePicker* view,
        /* [in] */ Int32 year,
        /* [in] */ Int32 month,
        /* [in] */ Int32 day);

    //@Override
    CARAPI OnTimeChanged(
        /* [in] */ ITimePicker* view,
        /* [in] */ Int32 hourOfDay,
        /* [in] */ Int32 minute);

    /**
     * Sets the current date.
     *
     * @param year The date year.
     * @param monthOfYear The date month.
     * @param dayOfMonth The date day of month.
     */
    CARAPI_(void) UpdateDateTime(
        /* [in] */ Int32 year,
        /* [in] */ Int32 monthOfYear,
        /* [in] */ Int32 dayOfMonth,
        /* [in] */ Int32 hourOfDay,
        /* [in] */ Int32 minutOfHour);

private:
    CARAPI_(void) TryNotifyDateTimeSet();

private:
    const AutoPtr<DatePicker> mDatePicker;
    const AutoPtr<TimePicker> mTimePicker;
    const AutoPtr<IOnDateTimeSetListener> mCallBack;

    const Int64 mMinTimeMillis;
    const Int64 mMaxTimeMillis;
}

} // namespace Input
} // namespace Browser
} // namespace Content
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif//__ELASTOS_DROID_WEBKIT_CONTENT_BROWSER_INPUT_DATETIMEPICKERDIALOG_H__
