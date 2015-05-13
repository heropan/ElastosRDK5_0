#ifndef __CNUMBERPICKER_H_
#define __CNUMBERPICKER_H_

#include "_CNumberPicker.h"
#include "widget/NumberPicker.h"
#include "view/ViewMacro.h"
#include "view/ViewGroupLayoutParamsMacro.h"
#include "widget/LinearLayoutMacro.h"

namespace Elastos{
namespace Droid{
namespace Widget{

CarClass(CNumberPicker), public NumberPicker
{
public:
    IVIEW_METHODS_DECL()
    IVIEWGROUP_METHODS_DECL()
    IVIEWPARENT_METHODS_DECL()
    IVIEWMANAGER_METHODS_DECL()
    IDRAWABLECALLBACK_METHODS_DECL()
    IKEYEVENTCALLBACK_METHODS_DECL()
    IACCESSIBILITYEVENTSOURCE_METHODS_DECL()
    ILINEARLAYOUT_METHODS_DECL()

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI constructor(
        /* [in] */ IContext* context);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle);

public:
    CARAPI SetOnValueChangedListener(
        /* [in] */ INumberPickerOnValueChangeListener* onValueChangedListener);

    CARAPI SetOnScrollListener(
        /* [in] */ INumberPickerOnScrollListener* onScrollListener);

    CARAPI SetFormatter(
        /* [in] */ INumberPickerFormatter* formatter);

    CARAPI SetValue(
        /* [in] */ Int32 value);

    CARAPI GetWrapSelectorWheel(
        /* [out] */ Boolean* res);

    CARAPI SetWrapSelectorWheel(
        /* [in] */ Boolean wrapSelectorWheel);

    CARAPI SetOnLongPressUpdateInterval(
        /* [in] */ Int64 intervalMillis);

    CARAPI GetValue(
        /* [out] */ Int32* value);

    CARAPI GetMinValue(
        /* [out] */ Int32* MinValue);

    CARAPI SetMinValue(
        /* [in] */ Int32 MinValue);

    CARAPI GetMaxValue(
        /* [out] */ Int32* MaxValue);

    CARAPI SetMaxValue(
        /* [in] */ Int32 maxValue);

    CARAPI GetDisplayedValues(
        /* [out, callee] */ ArrayOf<String>** displayedValues);

    CARAPI SetDisplayedValues(
        /* [in] */ ArrayOf<String>* displayedValues);
};

}// namespace Widget
}// namespace Droid
}// namespace Elastos

#endif //__CNUMBERPICKER_H_
