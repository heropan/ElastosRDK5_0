
#ifndef  __CKEYBOARDVIEW_H__
#define  __CKEYBOARDVIEW_H__

#include "_CKeyboardView.h"
#include "inputmethodservice/KeyboardView.h"


namespace Elastos {
namespace Droid {
namespace InputMethodService {

/**
 * A view that renders a virtual {@link Keyboard}. It handles rendering of keys and
 * detecting key presses and touch movements.
 *
 * @attr ref android.R.styleable#KeyboardView_keyBackground
 * @attr ref android.R.styleable#KeyboardView_keyPreviewLayout
 * @attr ref android.R.styleable#KeyboardView_keyPreviewOffset
 * @attr ref android.R.styleable#KeyboardView_labelTextSize
 * @attr ref android.R.styleable#KeyboardView_keyTextSize
 * @attr ref android.R.styleable#KeyboardView_keyTextColor
 * @attr ref android.R.styleable#KeyboardView_verticalCorrection
 * @attr ref android.R.styleable#KeyboardView_popupLayout
 */
CarClass(CKeyboardView), public KeyboardView
{
public:
    IVIEW_METHODS_DECL()

    IDRAWABLECALLBACK_METHODS_DECL()

    IKEYEVENTCALLBACK_METHODS_DECL()

    IACCESSIBILITYEVENTSOURCE_METHODS_DECL()

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI OnClick(
        /* [in] */ IView* v);

    CARAPI SetOnKeyboardActionListener(
        /* [in] */ IOnKeyboardActionListener* listener);

    CARAPI GetOnKeyboardActionListener(
        /* [out] */ IOnKeyboardActionListener** listener);

    CARAPI Closing();

    CARAPI HandleBack(
        /* [out] */ Boolean* res);

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
};

} // namespace InputMethodService
} // namespace Droid
} // namespace Elastos

#endif  //__KEYBOARDVIEW_H__
