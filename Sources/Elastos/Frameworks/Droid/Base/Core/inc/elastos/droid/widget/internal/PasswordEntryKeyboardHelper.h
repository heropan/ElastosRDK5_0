
#ifndef __ELASTOS_DROID_WIDGET_INTERNAL_PASSWORDENTRYKEYBOARDHELPER_H__
#define __ELASTOS_DROID_WIDGET_INTERNAL_PASSWORDENTRYKEYBOARDHELPER_H__

#define LAYOUTS_LENGTH 5

#include "ext/frameworkext.h"
#include "widget/internal/CPasswordEntryKeyboard.h"

using Elastos::Core::ICharSequence;
using Elastos::Droid::View::IView;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::InputMethodService::IKeyboardView;

namespace Elastos {
namespace Droid {
namespace Widget {
namespace Internal {

class PasswordEntryKeyboardHelper
{
public:
    PasswordEntryKeyboardHelper(
        /* [in] */ IContext* context,
        /* [in] */ IKeyboardView* keyboardView,
        /* [in] */ IView* targetView);

    PasswordEntryKeyboardHelper(
        /* [in] */ IContext* context,
        /* [in] */ IKeyboardView* keyboardView,
        /* [in] */ IView* targetView,
        /* [in] */ Boolean useFullScreenWidth);

    PasswordEntryKeyboardHelper(
        /* [in] */ IContext* context,
        /* [in] */ IKeyboardView* keyboardView,
        /* [in] */ IView* targetView,
        /* [in] */ Boolean useFullScreenWidth,
        /* [in] */ ArrayOf<Int32>* layouts);

    virtual CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid) = 0;

    virtual CARAPI CreateKeyboards();

    virtual CARAPI SetEnableHaptics(
        /* [in] */ Boolean enabled);

    virtual CARAPI_(Boolean) IsAlpha();

    virtual CARAPI SetKeyboardMode(
        /* [in] */ Int32 mode);

    virtual CARAPI SendDownUpKeyEvents(
        /* [in] */ Int32 keyEventCode);

    virtual CARAPI OnKey(
        /* [in] */ Int32 primaryCode,
        /* [in] */ ArrayOf<Int32>* keyCodes);

    /**
     * Sets and enables vibrate pattern.  If id is 0 (or can't be loaded), vibrate is disabled.
     * @param id resource id for array containing vibrate pattern.
     */
    virtual CARAPI SetVibratePattern(
        /* [in] */ Int32 id);

    virtual CARAPI HandleBackspace();

    virtual CARAPI OnPress(
        /* [in] */ Int32 primaryCode);

    virtual CARAPI OnRelease(
        /* [in] */ Int32 primaryCode);

    virtual CARAPI OnText(
        /* [in] */ ICharSequence* text);

    virtual CARAPI SwipeDown();

    virtual CARAPI SwipeLeft();

    virtual CARAPI SwipeRight();

    virtual CARAPI SwipeUp();

protected:
    PasswordEntryKeyboardHelper();

    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ IKeyboardView* keyboardView,
        /* [in] */ IView* targetView,
        /* [in] */ Boolean useFullScreenWidth = FALSE,
        /* [in] */ ArrayOf<Int32>* layouts = NULL);

public:
    static const Int32 KEYBOARD_MODE_ALPHA = 0;
    static const Int32 KEYBOARD_MODE_NUMERIC = 1;

private:
    CARAPI_(void) CreateKeyboardsWithSpecificSize(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height);

    CARAPI_(void) CreateKeyboardsWithDefaultWidth();

    CARAPI_(void) SendKeyEventsToTarget(
        /* [in] */ Int32 character);

    CARAPI_(void) HandleModeChange();

    CARAPI_(void) HandleShift();

    CARAPI_(void) HandleCharacter(
        /* [in] */ Int32 primaryCode,
        /* [in] */ ArrayOf<Int32>* keyCodes);

    CARAPI_(void) HandleClose();

    CARAPI_(void) PerformHapticFeedback();

    static const Int32 KEYBOARD_STATE_NORMAL = 0;
    static const Int32 KEYBOARD_STATE_SHIFTED = 1;
    static const Int32 KEYBOARD_STATE_CAPSLOCK = 2;
    static const String TAG;
    Int32 mKeyboardMode;
    Int32 mKeyboardState;
    AutoPtr<CPasswordEntryKeyboard> mQwertyKeyboard;
    AutoPtr<CPasswordEntryKeyboard> mQwertyKeyboardShifted;
    AutoPtr<CPasswordEntryKeyboard> mSymbolsKeyboard;
    AutoPtr<CPasswordEntryKeyboard> mSymbolsKeyboardShifted;
    AutoPtr<CPasswordEntryKeyboard> mNumericKeyboard;
    AutoPtr<IContext> mContext;
    AutoPtr<IView> mTargetView;
    AutoPtr<IKeyboardView> mKeyboardView;
    AutoPtr<ArrayOf<Int64> > mVibratePattern;
    Boolean mEnableHaptics;

    static const Int32 NUMERIC = 0;
    static const Int32 QWERTY = 1;
    static const Int32 QWERTY_SHIFTED = 2;
    static const Int32 SYMBOLS = 3;
    static const Int32 SYMBOLS_SHIFTED = 4;

    AutoPtr<ArrayOf<Int32> > mLayouts;


    Boolean mUsingScreenWidth;
};

}// namespace Internal
}// namespace Widget
}// namespace Droid
}// namespace Elastos
#endif
