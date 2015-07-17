
#ifndef __INPUTDIALOGCONTAINER_H__
#define __INPUTDIALOGCONTAINER_H__

// import android.app.AlertDialog;
// import android.app.DatePickerDialog.OnDateSetListener;
// import android.content.Context;
// import android.content.DialogInterface;
// import android.content.DialogInterface.OnDismissListener;
// import android.text.format.DateFormat;
// import android.view.View;
// import android.widget.AdapterView;
// import android.widget.DatePicker;
// import android.widget.ListView;
// import android.widget.TimePicker;

// import org.chromium.base.ApiCompatibilityUtils;
// import org.chromium.content.R;
// import org.chromium.content.browser.input.DateTimePickerDialog.OnDateTimeSetListener;
// import org.chromium.content.browser.input.MultiFieldTimePickerDialog.OnMultiFieldTimeSetListener;

// import java.util.Arrays;
// import java.util.Calendar;
// import java.util.Date;
// import java.util.GregorianCalendar;
// import java.util.TimeZone;
// import java.util.concurrent.TimeUnit;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Content {
namespace Browser {
namespace Input {

/**
 * Opens the approprate date/time picker dialog for the given dialog type.
 */
class InputDialogContainer
{
public:
    class InputActionDelegate
    {
    public:
        CARAPI_(void) CancelDateTimeDialog() = 0;

        CARAPI_(void) ReplaceDateTime(
            /* [in] */ Double value) = 0;
    };

private:
    class InnerAdapterViewOnItemClickListener
        : public Object
        , public IAdapterViewOnItemClickListener
    {
    public:
        InnerAdapterViewOnItemClickListener(
            /* [in] */ InputDialogContainer* owner,
            /* [in] */ Int32 dialogType,
            /* [in] */ Double dialogValue,
            /* [in] */ Double min,
            /* [in] */ Double max,
            /* [in] */ Double step,
            /* [in] */ DateTimeSuggestionListAdapter* adapter);

        CARAPI OnItemClick(
            /* [in] */ IAdapterView* parent,
            /* [in] */ IView* view,
            /* [in] */ Int32 position,
            /* [in] */ Int64 id);

    private:
        InputDialogContainer* mOwner;
        Int32 mDialogType;
        Double mDialogValue;
        Double mMin;
        Double mMax;
        Double mStep;
        DateTimeSuggestionListAdapter* mAdapter;
    };

    class InnerDialogInterfaceOnClickListener
        : public Object
        , public IDialogInterfaceOnClickListener
    {
    public:
        InnerDialogInterfaceOnClickListener(
            /* [in] */ InputDialogContainer* owner);

        CARAPI OnClick(
            /* [in] */ IDialogInterface* dialog,
            /* [in] */ Int32 which);

    private:
        InputDialogContainer* mOwner;
    };

    class InnerDialogInterfaceOnDismissListener
        : public Object
        , public IDialogInterfaceOnDismissListener
    {
    public:
        InnerDialogInterfaceOnDismissListener(
            /* [in] */ InputDialogContainer* owner);

        CARAPI OnDismiss(
            /* [in] */ IDialogInterface* dialog);

    private:
        InputDialogContainer* mOwner;
    };

    class SetButtonDialogInterfaceOnClickListener
        : public Object
        , public IDialogInterfaceOnClickListener
    {
    public:
        SetButtonDialogInterfaceOnClickListener(
            /* [in] */ InputDialogContainer* owner);

        CARAPI OnClick(
            /* [in] */ IDialogInterface* dialog,
            /* [in] */ Int32 which);

    private:
        InputDialogContainer* mOwner;
    };

    class SetOnDismissListenerDialogInterfaceOnDismissListener
        : public Object
        , public IDialogInterfaceOnDismissListener
    {
    public:
        SetOnDismissListenerDialogInterfaceOnDismissListener(
            /* [in] */ InputDialogContainer* owner);

        CARAPI OnDismiss(
            /* [in] */ IDialogInterface* dialog);

    private:
        InputDialogContainer* mOwner;
    };

    class DateListener
        : public Object
        , public IOnDateSetListener
    {
    public:
        DateListener(
            /* [in] */ InputDialogContainer* owner,
            /* [in] */ Int32 dialogType);

        //@Override
        CARAPI OnDateSet(
            /* [in] */ IDatePicker* view,
            /* [in] */ Int32 year,
            /* [in] */ Int32 month,
            /* [in] */ Int32 monthDay);

    private:
        InputDialogContainer* mOwner;
        const Int32 mDialogType;
    };

    class FullTimeListener
        : public Object
        , public IOnMultiFieldTimeSetListener
    {
    public:
        FullTimeListener(
            /* [in] */ InputDialogContainer* owner,
            /* [in] */ Int32 dialogType);

        //@Override
        CARAPI OnTimeSet(
            /* [in] */ Int32 hourOfDay,
            /* [in] */ Int32 minute,
            /* [in] */ Int32 second,
            /* [in] */ Int32 milli);

    private:
        InputDialogContainer* mOwner;
        const Int32 mDialogType;
    };

    class DateTimeListener
        : public Object
        , public IOnDateTimeSetListener
    {
    public:
        DateTimeListener(
            /* [in] */ InputDialogContainer* owner,
            /* [in] */ Int32 dialogType);

        //@Override
        CARAPI OnDateTimeSet(
            /* [in] */ IDatePicker* dateView,
            /* [in] */ ITimePicker* timeView,
            /* [in] */ Int32 year,
            /* [in] */ Int32 month,
            /* [in] */ Int32 monthDay,
            /* [in] */ Int32 hourOfDay,
            /* [in] */ Int32 minute);

    private:
        InputDialogContainer* mOwner;
        const Boolean mLocal;
        const Int32 mDialogType;
    };

    class MonthOrWeekListener
        : public Object,
        , public TwoFieldDatePickerDialog::OnValueSetListener
    {
    public:
        MonthOrWeekListener(
            /* [in] */ InputDialogContainer* owner,
            /* [in] */ Int32 dialogType);

        //@Override
        CARAPI_(void) OnValueSet(
            /* [in] */ Int32 year,
            /* [in] */ Int32 positionInYear);

    private:
        InputDialogContainer* mOwner;
        const Int32 mDialogType;
    };

public:
    InputDialogContainer(
        /* [in] */ IContext* context,
        /* [in] */ InputActionDelegate* inputActionDelegate);

    static CARAPI_(void) InitializeInputTypes(
        /* [in] */ Int32 textInputTypeDate,
        /* [in] */ Int32 textInputTypeDateTime,
        /* [in] */ Int32 textInputTypeDateTimeLocal,
        /* [in] */ Int32 textInputTypeMonth,
        /* [in] */ Int32 textInputTypeTime,
        /* [in] */ Int32 textInputTypeWeek);

    static CARAPI_(Boolean) IsDialogInputType(
        /* [in] */ Int32 type);

    CARAPI_(void) ShowPickerDialog(
        /* [in] */ Int32 dialogType,
        /* [in] */ Double dialogValue,
        /* [in] */ Double min,
        /* [in] */ Double max,
        /* [in] */ Double step);

    CARAPI_(void) ShowSuggestionDialog(
        /* [in] */ Int32 dialogType,
        /* [in] */ Double dialogValue,
        /* [in] */ Double min,
        /* [in] */ Double max,
        /* [in] */ Double step,
        /* [in] */ ArrayOf<DateTimeSuggestion>* suggestions);

    CARAPI_(void) ShowDialog(
        /* [in] */ Int32 type,
        /* [in] */ Double value,
        /* [in] */ Double min,
        /* [in] */ Double max,
        /* [in] */ Double step,
        /* [in] */ ArrayOf<DateTimeSuggestion>* suggestions);

    CARAPI_(void) ShowPickerDialog(
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
        /* [in] */ Double step);

    CARAPI_(Boolean) IsDialogShowing();

    CARAPI_(void) DismissDialog();

protected:
    CARAPI_(void) SetFieldDateTimeValue(
        /* [in] */ Int32 dialogType,
        /* [in] */ Int32 year,
        /* [in] */ Int32 month,
        /* [in] */ Int32 monthDay,
        /* [in] */ Int32 hourOfDay,
        /* [in] */ Int32 minute,
        /* [in] */ Int32 second,
        /* [in] */ Int32 millis,
        /* [in] */ Int32 week);

private:
    static Int32 sTextInputTypeDate;
    static Int32 sTextInputTypeDateTime;
    static Int32 sTextInputTypeDateTimeLocal;
    static Int32 sTextInputTypeMonth;
    static Int32 sTextInputTypeTime;
    static Int32 sTextInputTypeWeek;

    const AutoPtr<IContext> mContext;

    // Prevents sending two notifications (from onClick and from onDismiss)
    Boolean mDialogAlreadyDismissed;

    AutoPtr<IAlertDialog> mDialog;
    const AutoPtr<InputActionDelegate> mInputActionDelegate;
};

} // namespace Input
} // namespace Browser
} // namespace Content
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif//__INPUTDIALOGCONTAINER_H__
