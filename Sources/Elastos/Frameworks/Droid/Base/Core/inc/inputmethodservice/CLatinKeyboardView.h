
#ifndef __ELASTOS_DROID_INPUTMETHODSERVICE_CLATINKEYBOARDVIEW_H__
#define  __ELASTOS_DROID_INPUTMETHODSERVICE_CLATINKEYBOARDVIEW_H__

#include "_Elastos_Droid_InputMethodService_CLatinKeyboardView.h"

#include "inputmethodservice/KeyboardView.h"

namespace Elastos {
namespace Droid {
namespace InputMethodService {

CarClass(CLatinKeyboardView), public KeyboardView
{
public:
    CARAPI constructor(
        /* [in] */ IContext* ctx,
        /* [in] */ IAttributeSet* attrs);

    CARAPI constructor(
        /* [in] */ IContext* ctx,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    IVIEW_METHODS_DECL()

    IDRAWABLECALLBACK_METHODS_DECL()

    IKEYEVENTCALLBACK_METHODS_DECL()

    IACCESSIBILITYEVENTSOURCE_METHODS_DECL()

    CARAPI OnLongPress(
        /* [in] */ IKeyboardKey* popupKey,
        /* [out] */ Boolean* res);

    CARAPI SetOnKeyboardActionListener(
        /* [in] */ IOnKeyboardActionListener* listener);

    /**
     * Returns the {@link IOnKeyboardActionListener} object.
     * @return the listener attached to this keyboard
     */
    CARAPI GetOnKeyboardActionListener(
        /* [out] */ IOnKeyboardActionListener** listener);

    /**
     * Attaches a keyboard to this view. The keyboard can be switched at any time and the
     * view will re-layout itself to accommodate the keyboard.
     * @see IKeyboard
     * @see #getKeyboard()
     * @param keyboard the keyboard to display in this view
     */
    CARAPI SetKeyboard(
        /* [in] */ IKeyboard* keyboard);

    /**
     * Returns the current keyboard being displayed by this view.
     * @return the currently attached keyboard
     * @see #setKeyboard(IKeyboard)
     */
    CARAPI GetKeyboard(
        /* [out] */ IKeyboard** keyboard);

    /**
     * Sets the state of the shift key of the keyboard, if any.
     * @param shifted whether or not to enable the state of the shift key
     * @return TRUE if the shift key state changed, FALSE if there was no change
     * @see KeyboardView#isShifted()
     */
    CARAPI SetShifted(
        /* [in] */ Boolean shifted,
        /* [out] */ Boolean* res);

    /**
     * Returns the state of the shift key of the keyboard, if any.
     * @return TRUE if the shift is in a pressed state, FALSE otherwise. If there is
     * no shift key on the keyboard or there is no keyboard attached, it returns FALSE.
     * @see KeyboardView#setShifted(Boolean)
     */
    CARAPI IsShifted(
        /* [out] */ Boolean* shifted);

    /**
     * Enables or disables the key feedback popup. This is a popup that shows a magnified
     * version of the depressed key. By default the preview is enabled.
     * @param previewEnabled whether or not to enable the key feedback popup
     * @see #isPreviewEnabled()
     */
    CARAPI SetPreviewEnabled(
        /* [in] */ Boolean previewEnabled);

    /**
     * Returns the enabled state of the key feedback popup.
     * @return whether or not the key feedback popup is enabled
     * @see #setPreviewEnabled(Boolean)
     */
    CARAPI IsPreviewEnabled(
        /* [out] */ Boolean* res);

    CARAPI SetVerticalCorrection(
        /* [in] */ Int32 verticalOffset);

    CARAPI SetPopupParent(
        /* [in] */ IView* v);

    CARAPI SetPopupOffset(
        /* [in] */ Int32 x,
        /* [in] */ Int32 y);

    /**
     * When enabled, calls to {@link IOnKeyboardActionListener#onKey} will include key
     * codes for adjacent keys.  When disabled, only the primary key code will be
     * reported.
     * @param enabled whether or not the proximity correction is enabled
     */
    CARAPI SetProximityCorrectionEnabled(
        /* [in] */ Boolean enabled);

    /**
     * Returns TRUE if proximity correction is enabled.
     */
    CARAPI IsProximityCorrectionEnabled(
        /* [out] */ Boolean* res);

    /**
     * Popup keyboard close button clicked.
     * @hide
     */
    CARAPI OnClick(
        /* [in] */ IView* v);

    CARAPI InvalidateAllKeys();

    CARAPI InvalidateKey(
        /* [in] */ Int32 keyIndex);

    CARAPI Closing();

    CARAPI HandleBack(
        /* [out] */ Boolean* res);
};

} // namespace InputMethodService
} // namespace Droid
} // namespace Elastos

#endif  //__ELASTOS_DROID_INPUTMETHODSERVICE_CLATINKEYBOARDVIEW_H__
