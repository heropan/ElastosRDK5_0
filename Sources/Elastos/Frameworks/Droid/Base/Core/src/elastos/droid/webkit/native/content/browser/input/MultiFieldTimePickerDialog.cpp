
namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Content {
namespace Browser {
namespace Input {

//==================================================================
//            MultiFieldTimePickerDialog::NumberFormatter
//==================================================================

MultiFieldTimePickerDialog::NumberFormatter::NumberFormatter(
    /* [in] */ const String& format)
    : mFormat(format)
{
}

//@Override
MultiFieldTimePickerDialog::NumberFormatter::Format(
    /* [in] */ Int32 value,
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);
    *str = String::Format(mFormat, value);
    return NOERROR;
}

//==================================================================
//                 MultiFieldTimePickerDialog
//==================================================================

const Int32 MultiFieldTimePickerDialog::SECOND_IN_MILLIS;
const Int32 MultiFieldTimePickerDialog::MINUTE_IN_MILLIS;
const Int32 MultiFieldTimePickerDialog::HOUR_IN_MILLIS;

MultiFieldTimePickerDialog::MultiFieldTimePickerDialog(
    /* [in] */ IContext* context,
    /* [in] */ Int32 theme,
    /* [in] */ Int32 hour,
    /* [in] */ Int32 minute,
    /* [in] */ Int32 second,
    /* [in] */ Int32 milli,
    /* [in] */ Int32 min,
    /* [in] */ Int32 max,
    /* [in] */ Int32 step,
    /* [in] */ Boolean is24hourFormat,
    /* [in] */ OnMultiFieldTimeSetListener* listener)
{
    super(context, theme);
    mListener = listener;
    mStep = step;
    mIs24hourFormat = is24hourFormat;

    if (min >= max) {
        min = 0;
        max = 24 * HOUR_IN_MILLIS - 1;
    }

    if (step < 0 || step >= 24 * HOUR_IN_MILLIS) {
        step = MINUTE_IN_MILLIS;
    }

    AutoPtr<ILayoutInflater> inflater;
    context->GetSystemService(
                    IContext::LAYOUT_INFLATER_SERVICE,
                    (ILayoutInflater**)&inflater);
    AutoPtr<IView> view;
    inflater->Inflate(R::layout::multi_field_time_picker_dialog, NULL,
            (IView**)&view);
    SetView(view);

    view->FindViewById(R::id::hour, (IView**)&mHourSpinner);
    view->FindViewById(R::id::minute, (IView**)&mMinuteSpinner);
    view->FindViewById(R::id::second, (IView**)&mSecSpinner);
    view->FindViewById(R::id::milli, (IView**)&mMilliSpinner);
    view->FindViewById(R::id::ampm, (IView**)&mAmPmSpinner);

    Int32 minHour = min / HOUR_IN_MILLIS;
    Int32 maxHour = max / HOUR_IN_MILLIS;
    min -= minHour * HOUR_IN_MILLIS;
    max -= maxHour * HOUR_IN_MILLIS;

    if (minHour == maxHour) {
        mHourSpinner->SetEnabled(FALSE);
        hour = minHour;
    }

    if (is24hourFormat) {
        mAmPmSpinner->SetVisibility(IView::GONE);
    }
    else {
        Int32 minAmPm = minHour / 12;
        Int32 maxAmPm = maxHour / 12;
        Int32 amPm = hour / 12;
        mAmPmSpinner->SetMinValue(minAmPm);
        mAmPmSpinner->SetMaxValue(maxAmPm);
        String am, pm;
        context->GetString(R::string::time_picker_dialog_am, &am);
        context->GetString(R::string::time_picker_dialog_pm, &pm);
        AutoPtr< ArrayOf<String> > strs = ArrayOf<String>::Alloc(2);
        (*strs)[0] = am;
        (*strs)[1] = pm;
        mAmPmSpinner->SetDisplayedValues(strs);

        hour %= 12;
        if (hour == 0) {
            hour = 12;
        }

        if (minAmPm == maxAmPm) {
            mAmPmSpinner->SetEnabled(FALSE);
            amPm = minAmPm;

            minHour %= 12;
            maxHour %= 12;
            if (minHour == 0 && maxHour == 0) {
                minHour = 12;
                maxHour = 12;
            } else if (minHour == 0) {
                minHour = maxHour;
                maxHour = 12;
            } else if (maxHour == 0) {
                maxHour = 12;
            }
        }
        else {
            minHour = 1;
            maxHour = 12;
        }
        mAmPmSpinner->SetValue(amPm);
    }

    if (minHour == maxHour) {
        mHourSpinner->SetEnabled(FALSE);
    }

    mHourSpinner->SetMinValue(minHour);
    mHourSpinner->SetMaxValue(maxHour);
    mHourSpinner->SetValue(hour);

    AutoPtr<NumberFormatter> twoDigitPaddingFormatter = new NumberFormatter("%02d");

    Int32 minMinute = min / MINUTE_IN_MILLIS;
    Int32 maxMinute = max / MINUTE_IN_MILLIS;
    min -= minMinute * MINUTE_IN_MILLIS;
    max -= maxMinute * MINUTE_IN_MILLIS;

    if (minHour == maxHour) {
        mMinuteSpinner->SetMinValue(minMinute);
        mMinuteSpinner->SetMaxValue(maxMinute);
        if (minMinute == maxMinute) {
            // Set this otherwise the box is empty until you stroke it.
            String format;
            TwoDigitPaddingFormatter::Format(minMinute, &format);
            AutoPtr< ArrayOf<String> > array = ArrayOf<String>::Alloc(1);
            (*array)[0] = format;

            mMinuteSpinner->SetDisplayedValues(array);
            mMinuteSpinner->SetEnabled(FALSE);
            minute = minMinute;
        }
    }
    else {
        mMinuteSpinner->SetMinValue(0);
        mMinuteSpinner->SetMaxValue(59);
    }

    if (step >= HOUR_IN_MILLIS) {
        mMinuteSpinner->SetEnabled(FALSE);
    }

    mMinuteSpinner->SetValue(minute);
    mMinuteSpinner->SetFormatter(twoDigitPaddingFormatter);

    if (step >= MINUTE_IN_MILLIS) {
        // Remove the ':' in front of the second spinner as well.
        AutoPtr<IView> secondColon;
        view->FindViewById(R::id::second_colon, (IView**)&secondColon);
        secondColon->SetVisibility(IView::GONE);
        mSecSpinner->SetVisibility(IView::GONE);
    }

    Int32 minSecond = min / SECOND_IN_MILLIS;
    Int32 maxSecond = max / SECOND_IN_MILLIS;
    min -= minSecond * SECOND_IN_MILLIS;
    max -= maxSecond * SECOND_IN_MILLIS;

    if (minHour == maxHour && minMinute == maxMinute) {
        mSecSpinner.setMinValue(minSecond);
        mSecSpinner.setMaxValue(maxSecond);
        if (minSecond == maxSecond) {
            // Set this otherwise the box is empty until you stroke it.
            mSecSpinner.setDisplayedValues(
                new String[] { twoDigitPaddingFormatter.format(minSecond) });
            mSecSpinner.setEnabled(false);
            second = minSecond;
        }
    } else {
        mSecSpinner.setMinValue(0);
        mSecSpinner.setMaxValue(59);
    }

    mSecSpinner.setValue(second);
    mSecSpinner.setFormatter(twoDigitPaddingFormatter);

    if (step >= SECOND_IN_MILLIS) {
        // Remove the '.' in front of the milli spinner as well.
        AutoPtr<IView> secondDot;
        view->FindViewById(R::id::second_dot);
        secondDot->SetVisibility(IView::GONE);
        mMilliSpinner->SetVisibility(IView::GONE);
    }

    // Round to the nearest step.
    milli = ((milli + step / 2) / step) * step;
    if (step == 1 || step == 10 || step == 100) {
        if (minHour == maxHour && minMinute == maxMinute &&
            minSecond == maxSecond) {
            mMilliSpinner->SetMinValue(min / step);
            mMilliSpinner->SetMaxValue(max / step);

            if (min == max) {
                mMilliSpinner->SetEnabled(FALSE);
                milli = min;
            }
        }
        else {
            mMilliSpinner->SetMinValue(0);
            mMilliSpinner->SetMaxValue(999 / step);
        }

        if (step == 1) {
            AutoPtr<NumberFormatter> format = new NumberFormatter(String("%03d"));
            mMilliSpinner->SetFormatter(format);
        }
        else if (step == 10) {
            AutoPtr<NumberFormatter> format = new NumberFormatter("%02d");
            mMilliSpinner->SetFormatter(format);
        }
        else if (step == 100) {
            AutoPtr<NumberFormatter> format = new NumberFormatter("%d");
            mMilliSpinner->SetFormatter(format);
        }
        mMilliSpinner->SetValue(milli / step);
        mBaseMilli = 0;
    }
    else if (step < SECOND_IN_MILLIS) {
        // Non-decimal step value.
        ArrayList<String> strValue = new ArrayList<String>();
        for (Int32 i = min; i < max; i += step) {
            strValue->Add(String::Format(String("%03d"), i));
        }
        mMilliSpinner->SetMinValue(0);
        mMilliSpinner->SetMaxValue(strValue.size() - 1);
        mMilliSpinner->SetValue((milli - min) / step);
        mMilliSpinner->SetDisplayedValues(
            strValue.toArray(new String[strValue.size()]));
        mBaseMilli = min;
    }
    else {
        mBaseMilli = 0;
    }
}

//@Override
ECode MultiFieldTimePickerDialog::OnClick(
    /* [in] */ IDialogInterface* dialog,
    /* [in] */ Int32 which)
{
    NotifyDateSet();
    return NOERROR;
}

void MultiFieldTimePickerDialog::NotifyDateSet()
{
    Int32 hour;
    mHourSpinner->GetValue(&hour);
    Int32 minute;
    mMinuteSpinner->GetValue(&minute);
    Int32 sec;
    mSecSpinner->GetValue(&sec);
    Int32 spinnerValue;
    mMilliSpinner->GetValue(&spinnerValue);
    Int32 milli = spinnerValue * mStep + mBaseMilli;
    if (!mIs24hourFormat) {
        Int32 ampm;
        mAmPmSpinner->GetValue(&ampm);
        if (hour == 12) {
            hour = 0;
        }
        hour += ampm * 12;
    }

    mListener->OnTimeSet(hour, minute, sec, milli);
}

} // namespace Input
} // namespace Browser
} // namespace Content
} // namespace Webkit
} // namespace Droid
} // namespace Elastos
