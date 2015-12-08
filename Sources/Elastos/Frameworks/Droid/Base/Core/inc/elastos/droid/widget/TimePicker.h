
#ifndef __ELASTOS_DROID_WIDGET_TIMEPICKER_H__
#define __ELASTOS_DROID_WIDGET_TIMEPICKER_H__

#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/widget/FrameLayout.h"
#include <R.h>

using Elastos::Utility::ILocale;
using Elastos::Core::IInteger32;
using Elastos::Utility::ICalendar;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::Res::IConfiguration;
using Elastos::Droid::View::IViewOnClickListener;
using Elastos::Droid::View::IView;
using Elastos::Droid::View::Accessibility::IAccessibilityEvent;
using Elastos::Droid::View::Accessibility::IAccessibilityNodeInfo;
using Elastos::Droid::Utility::IAttributeSet;
using Elastos::Droid::Widget::IOnTimeChangedListener;
using Elastos::Droid::Widget::INumberPickerOnValueChangeListener;
using Elastos::Droid::Widget::ITimePicker;
using Elastos::Droid::Widget::INumberPicker;
using Elastos::Droid::Widget::IEditText;
using Elastos::Droid::Widget::ITextView;
using Elastos::Droid::Widget::IButton;

namespace Elastos {
namespace Droid {
namespace Widget {

class TimePicker : public FrameLayout
{
private:
    class TimePickerOnTimeChangedListener
    : public IOnTimeChangedListener
    , public ElRefBase
    {
    public:
        TimePickerOnTimeChangedListener();

        CAR_INTERFACE_DECL();

        CARAPI OnTimeChanged(
            /* [in] */ ITimePicker* view,
            /* [in] */ Int32 hourOfDay,
            /* [in] */ Int32 minute);
    };

    class HourChangeListener
        : public INumberPickerOnValueChangeListener
        , public ElRefBase
    {
    public:
        HourChangeListener(
            /* [in] */ TimePicker* host);

        CAR_INTERFACE_DECL();

        CARAPI OnValueChange(
            /* [in] */ INumberPicker* picker,
            /* [in] */ Int32 oldVal,
            /* [in] */ Int32 newVal);

    private:
        TimePicker* mHost;
    };

    class MinuteChangeListener
        : public INumberPickerOnValueChangeListener
        , public ElRefBase
    {
    public:
        MinuteChangeListener(
            /* [in] */ TimePicker* host);

        CAR_INTERFACE_DECL();

        CARAPI OnValueChange(
            /* [in] */ INumberPicker* picker,
            /* [in] */ Int32 oldVal,
            /* [in] */ Int32 newVal);

    private:
        TimePicker* mHost;
    };

    class AmPmChangeListener
        : public INumberPickerOnValueChangeListener
        , public ElRefBase
    {
    public:
        AmPmChangeListener(
            /* [in] */ TimePicker* host);

        CAR_INTERFACE_DECL();

        CARAPI OnValueChange(
            /* [in] */ INumberPicker* picker,
            /* [in] */ Int32 oldVal,
            /* [in] */ Int32 newVal);

    private:
        TimePicker* mHost;
    };

    class AmPmClickListener
        : public IViewOnClickListener
        , public ElRefBase
    {
    public:
        AmPmClickListener(
            /* [in] */ TimePicker* host);

        CAR_INTERFACE_DECL();

        CARAPI OnClick(
            /* [in] */ IView* v);

    private:
        TimePicker* mHost;
    };

public:
    TimePicker(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs = NULL,
        /* [in] */ Int32 defStyle = R::attr::timePickerStyle);

    CARAPI SetEnabled(
        /* [in] */ Boolean enabled);

    CARAPI_(Boolean) IsEnabled();

    CARAPI SetOnTimeChangedListener(
        /* [in] */ IOnTimeChangedListener* onTimeChangedListener);

    CARAPI_(Int32) GetCurrentHour();

    CARAPI SetCurrentHour(
        /* [in] */ Int32 currentHour);

    CARAPI SetIs24HourView(
        /* [in] */ Boolean is24HourView);

    CARAPI_(Boolean) Is24HourView();

    CARAPI_(Int32) GetCurrentMinute();

    CARAPI SetCurrentMinute(
        /* [in] */ Int32 currentMinute);

    CARAPI GetBaseline(
        /* [out] */ Int32* baseline);

    CARAPI_(Boolean) DispatchPopulateAccessibilityEvent(
        /* [in] */ IAccessibilityEvent* event);

    CARAPI OnPopulateAccessibilityEvent(
        /* [in] */ IAccessibilityEvent* event);

    CARAPI OnInitializeAccessibilityEvent(
        /* [in] */ IAccessibilityEvent* event);

    CARAPI OnInitializeAccessibilityNodeInfo(
        /* [in] */ IAccessibilityNodeInfo* info);



protected:
    TimePicker();

    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs = NULL,
        /* [in] */ Int32 defStyle = R::attr::timePickerStyle);

    CARAPI_(void) OnConfigurationChanged(
        /* [in] */ IConfiguration* newConfig);

    CARAPI_(AutoPtr<IParcelable>) OnSaveInstanceState();

    CARAPI_(void) OnRestoreInstanceState(
        /* [in] */ IParcelable* state);

private:
    CARAPI SetCurrentLocale(
        /* [in] */ ILocale* locale);

    CARAPI UpdateHourControl();

    CARAPI UpdateAmPmControl();

    CARAPI OnTimeChanged();

    CARAPI SetContentDescriptions();

    CARAPI TrySetContentDescription(
        /* [in] */ IView* root,
        /* [in] */ Int32 viewId,
        /* [in] */ Int32 contDescResId);

    CARAPI UpdateInputState();


private:
    static const Boolean DEFAULT_ENABLED_STATE = TRUE;
    static const Int32 HOURS_IN_HALF_DAY = 12;

    AutoPtr<IOnTimeChangedListener> NO_OP_CHANGE_LISTENER;

    Boolean mIs24HourView;
    Boolean mIsAm;

    AutoPtr<INumberPicker> mHourSpinner;
    AutoPtr<INumberPicker> mMinuteSpinner;
    AutoPtr<INumberPicker> mAmPmSpinner;
    AutoPtr<IEditText> mHourSpinnerInput;
    AutoPtr<IEditText> mMinuteSpinnerInput;
    AutoPtr<IEditText> mAmPmSpinnerInput;
    AutoPtr<ITextView> mDivider;
    AutoPtr<IButton> mAmPmButton;

    AutoPtr< ArrayOf<String> > mAmPmStrings;

    Boolean mIsEnabled;
    AutoPtr<IOnTimeChangedListener> mOnTimeChangedListener;
    AutoPtr<ICalendar> mTempCalendar;
    AutoPtr<ILocale> mCurrentLocale;



};

} // namespace Widget
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_WIDGET_TIMEPICKER_H__
