#ifndef __CPASSWORDENTRYKEYBOARDVIEW_H__
#define __CPASSWORDENTRYKEYBOARDVIEW_H__

#include "_CPasswordEntryKeyboardView.h"
#include "PasswordEntryKeyboardView.h"
#include "view/ViewMacro.h"

namespace Elastos {
namespace Droid {
namespace Widget {
namespace Internal {

CarClass(CPasswordEntryKeyboardView) , public PasswordEntryKeyboardView
{
public:
    IVIEW_METHODS_DECL()
    IDRAWABLECALLBACK_METHODS_DECL()
    IKEYEVENTCALLBACK_METHODS_DECL()
    IACCESSIBILITYEVENTSOURCE_METHODS_DECL()

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CPasswordEntryKeyboardView();

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle);

    CARAPI OnClick(
        /* [in] */ IView* v);

    CARAPI SetOnKeyboardActionListener(
        /* [in] */ IOnKeyboardActionListener* listener);

    CARAPI SetKeyboard(
        /* [in] */ IKeyboard* keyboard);

    CARAPI GetKeyboard(
        /* [out] */ IKeyboard** keyboard);

    CARAPI SetShifted(
        /* [in] */ Boolean shifted,
        /* [out] */ Boolean* res);

    CARAPI IsShifted(
        /* [out] */ Boolean* shifted);

    CARAPI SetPreviewEnabled(
        /* [in] */ Boolean previewEnabled);

    CARAPI IsPreviewEnabled(
        /* [out] */ Boolean* res);

    CARAPI SetVerticalCorrection(
        /* [in] */ Int32 verticalOffset);

    CARAPI SetPopupParent(
        /* [in] */ IView* v);

    CARAPI SetPopupOffset(
        /* [in] */ Int32 x,
        /* [in] */ Int32 y);

    CARAPI SetProximityCorrectionEnabled(
        /* [in] */ Boolean enabled);

    CARAPI IsProximityCorrectionEnabled(
        /* [out] */ Boolean* res);

    CARAPI InvalidateAllKeys();

    CARAPI InvalidateKey(
        /* [in] */ Int32 keyIndex);

    CARAPI Closing();

    CARAPI HandleBack(
        /* [out] */ Boolean* res);
};

}// namespace Internal
}// namespace Widget
}// namespace Droid
}// namespace Elastos
#endif