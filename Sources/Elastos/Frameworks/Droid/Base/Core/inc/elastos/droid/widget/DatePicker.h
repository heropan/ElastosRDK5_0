
#ifndef __ELASTOS_DROID_WIDGET_DATEPICKER_H__
#define __ELASTOS_DROID_WIDGET_DATEPICKER_H__

#include "elastos/droid/ext/frameworkext.h"
#include "widget/FrameLayout.h"
#include "view/ViewBaseSavedState.h"
#include <Elastos.CoreLibrary.h>
#include <Elastos.CoreLibrary.h>
#include <R.h>


using Libcore::ICU::ILocale;
using Elastos::Utility::ICalendar;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Utility::IAttributeSet;
using Elastos::Droid::Widget::FrameLayout;
using Elastos::Utility::Etl::HashMap;
using Elastos::Text::IDateFormat;
using Elastos::Droid::Widget::INumberPickerOnValueChangeListener;
using Elastos::Droid::Widget::IOnDateChangedListener;
using Elastos::Droid::View::IViewBaseSavedState;
using Elastos::Droid::View::ViewBaseSavedState;
using Elastos::Droid::View::EIID_IViewBaseSavedState;

namespace Elastos {
namespace Droid {
namespace Widget {

class DatePicker : public Elastos::Droid::Widget::FrameLayout
{
public:
    class DatePickerOnChangeListener
    : public ElRefBase
    , public INumberPickerOnValueChangeListener
    {
    public:
        DatePickerOnChangeListener(
            /* [in] */ DatePicker* host);

        CAR_INTERFACE_DECL()

        CARAPI OnValueChange(
            /* [in] */ INumberPicker* picker,
            /* [in] */ Int32 oldVal,
            /* [in] */ Int32 newVal);
    private:
        DatePicker* mHost;
    };

    class DatePickerOnDateChangeListener
    : public ElRefBase
    , public IOnDateChangeListener
    {
    public:
        DatePickerOnDateChangeListener(
            /* [in] */ DatePicker* host);

        CAR_INTERFACE_DECL()

        OnSelectedDayChange(
            /* [in] */ ICalendarView* view,
            /* [in] */ Int32 year,
            /* [in] */ Int32 month,
            /* [in] */ Int32 dayOfMonth);

    private:
        DatePicker* mHost;
    };

private:
    class DatePickerSavedState
    : public ElRefBase
    , public IViewBaseSavedState
    , public ViewBaseSavedState
    {
    public:
        DatePickerSavedState(
            /* [in] */ IParcel* in);

        DatePickerSavedState(
            /* [in] */ IParcelable* superState,
            /* [in] */ Int32 year,
            /* [in] */ Int32 month,
            /* [in] */ Int32 day);

        CAR_INTERFACE_DECL()

        CARAPI WriteToParcel(
            /* [in] */ IParcel* dest);

        CARAPI GetSuperState(
            /* [out] */ IParcelable** state);

    private:
        Int32 mYear;
        Int32 mMonth;
        Int32 mDay;
    };

public:
    DatePicker(
            /* [in] */ IContext* context,
            /* [in] */ IAttributeSet* attrs = NULL,
            /* [in] */ Int32 defStyle = R::attr::datePickerStyle);

    CARAPI_(Int64) GetMinDate();

    virtual CARAPI SetMinDate(
        /* [in] */ Int64 minDate);

    CARAPI_(Int64) GetMaxDate();

    virtual CARAPI SetMaxDate(
        /* [in] */ Int64 maxDate);

    virtual CARAPI SetEnabled(
        /* [in] */ Boolean enabled);

    virtual CARAPI_(Boolean) IsEnabled();

    virtual CARAPI_(Boolean) DispatchPopulateAccessibilityEvent(
        /* [in] */ IAccessibilityEvent* event);

    virtual CARAPI OnPopulateAccessibilityEvent(
        /* [in] */ IAccessibilityEvent* event);

    virtual CARAPI OnInitializeAccessibilityEvent(
        /* [in] */ IAccessibilityEvent* event);

    virtual CARAPI OnInitializeAccessibilityNodeInfo(
        /* [in] */ IAccessibilityNodeInfo* info);

    CARAPI_(Boolean) GetCalendarViewShown();

    CARAPI_(AutoPtr<ICalendarView>) GetCalendarView();

    virtual CARAPI SetCalendarViewShown(
        /* [in] */ Boolean shown);

    CARAPI_(Boolean) GetSpinnersShown();

    virtual CARAPI SetSpinnersShown(
        /* [in] */ Boolean shown);

    virtual CARAPI UpdateDate(
        /* [in] */ Int32 year,
        /* [in] */ Int32 month,
        /* [in] */ Int32 dayOfMonth);

    CARAPI Init(
        /* [in] */ Int32 year,
        /* [in] */ Int32 monthOfYear,
        /* [in] */ Int32 dayOfMonth,
        /* [in] */ IOnDateChangedListener* onDateChangedListener);

    CARAPI_(Int32) GetYear();

    CARAPI_(Int32) GetMonth();

    CARAPI_(Int32) GetDayOfMonth();

protected:
    DatePicker();

    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs = NULL,
        /* [in] */ Int32 defStyle = R::attr::datePickerStyle);

    virtual CARAPI_(void) OnConfigurationChanged(
        /* [in] */ IConfiguration* newConfig);

    /*protected void dispatchRestoreInstanceState(SparseArray<IParcelable> container);*/
    virtual CARAPI DispatchRestoreInstanceState(
            /* [in] */ IObjectInt32Map* container);

    virtual CARAPI_(AutoPtr<IParcelable>) OnSaveInstanceState();

    virtual CARAPI_(void) OnRestoreInstanceState(
            /* [in] */ IParcelable* state);

private:
    CARAPI SetCurrentLocale(
            /* [in] */ ILocale* locale);

    CARAPI_(AutoPtr<ICalendar>) GetCalendarForLocale(
            /* [in] */ ICalendar* oldCalendar,
            /* [in] */ ILocale* locale);

    CARAPI ReorderSpinners();

    CARAPI SetCurrentLocale();

    CARAPI_(Boolean) ParseDate(
            /* [in] */ const String& date,
            /* [in] */ ICalendar* outDate);

    CARAPI_(Boolean) IsNewDate(
            /* [in] */ Int32 year,
            /* [in] */ Int32 month,
            /* [in] */ Int32 dayOfMonth);

    CARAPI SetDate(
            /* [in] */ Int32 year,
            /* [in] */ Int32 month,
            /* [in] */ Int32 dayOfMonth);

    CARAPI UpdateSpinners();

    CARAPI UpdateCalendarView();

    CARAPI NotifyDateChanged();

    CARAPI SetImeOptions(
            /* [in] */ INumberPicker* spinner,
            /* [in] */ Int32 spinnerCount,
            /* [in] */ Int32 spinnerIndex);

    CARAPI SetContentDescriptions();

    CARAPI TrySetContentDescription(
            /* [in] */ IView* root,
            /* [in] */ Int32 viewId,
            /* [in] */ Int32 contDescResId);

    CARAPI UpdateInputState();

private:

    const static String DATE_FORMAT;

    const static Int32 DEFAULT_START_YEAR = 1900;

    const static Int32 DEFAULT_END_YEAR = 2100;

    const static Boolean DEFAULT_CALENDAR_VIEW_SHOWN = TRUE;

    const static Boolean DEFAULT_SPINNERS_SHOWN = TRUE;

    const static Boolean DEFAULT_ENABLED_STATE = TRUE;

    const static String DATEPICKER_NAME;

    AutoPtr<ILinearLayout> mSpinners;

    AutoPtr<INumberPicker> mDaySpinner;

    AutoPtr<INumberPicker> mMonthSpinner;

    AutoPtr<INumberPicker> mYearSpinner;

    AutoPtr<IEditText> mDaySpinnerInput;

    AutoPtr<IEditText> mMonthSpinnerInput;

    AutoPtr<IEditText> mYearSpinnerInput;

    AutoPtr<ICalendarView> mCalendarView;

    AutoPtr<ILocale> mCurrentLocale;

    AutoPtr<IOnDateChangedListener> mOnDateChangedListener;

    AutoPtr< ArrayOf<String> > mShortMonths;

    AutoPtr<IDateFormat> mDateFormat;

    Int32 mNumberOfMonths;

    AutoPtr<ICalendar> mTempDate;

    AutoPtr<ICalendar> mMinDate;

    AutoPtr<ICalendar> mMaxDate;

    AutoPtr<ICalendar> mCurrentDate;

    Boolean mIsEnabled;
};

} // namespace Widget
} // namespace Droid
} // namespace Elastos

#endif
