
#ifndef __MONTHPICKERDIALOG_H__
#define __MONTHPICKERDIALOG_H__

// import android.content.Context;

// import org.chromium.content.R;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Content {
namespace Browser {
namespace Input {

class MonthPickerDialog : public TwoFieldDatePickerDialog
{
public:
    /**
     * @param context The context the dialog is to run in.
     * @param callBack How the parent is notified that the date is set.
     * @param year The initial year of the dialog.
     * @param monthOfYear The initial month of the dialog.
     */
    MonthPickerDialog(
        /* [in] */ IContext* context,
        /* [in] */ OnValueSetListener* callBack,
        /* [in] */ Int32 year,
        /* [in] */ Int32 monthOfYear,
        /* [in] */ Double minMonth,
        /* [in] */ Double maxMonth);

    /**
     * Gets the {@link MonthPicker} contained in this dialog.
     *
     * @return The calendar view.
     */
    CARAPI_(AutoPtr<IMonthPicker>) GetMonthPicker();

protected:
    //@Override
    CARAPI_(AutoPtr<TwoFieldDatePicker>) CreatePicker(
        /* [in] */ IContext* context,
        /* [in] */ Double minValue,
        /* [in] */ Double maxValue);
};

} // namespace Input
} // namespace Browser
} // namespace Content
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif//__MONTHPICKERDIALOG_H__
