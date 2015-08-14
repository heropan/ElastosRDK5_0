#ifndef __ELASTOS_DROID_WIDGET_INTERNAL_CPASSWORDENTRYKEYBOARDHELPER_H__
#define __ELASTOS_DROID_WIDGET_INTERNAL_CPASSWORDENTRYKEYBOARDHELPER_H__

#include "_CPasswordEntryKeyboardHelper.h"
#include "widget/internal/PasswordEntryKeyboardHelper.h"

namespace Elastos {
namespace Droid {
namespace Widget {
namespace Internal {

CarClass(CPasswordEntryKeyboardHelper) , public PasswordEntryKeyboardHelper
{
public:
    CPasswordEntryKeyboardHelper();

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IKeyboardView* keyboardView,
        /* [in] */ IView* targetView);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IKeyboardView* keyboardView,
        /* [in] */ IView* targetView,
        /* [in] */ Boolean useFullScreenWidth);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IKeyboardView* keyboardView,
        /* [in] */ IView* targetView,
        /* [in] */ Boolean useFullScreenWidth,
        /* [in] */ ArrayOf<Int32>* layouts);

    virtual CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI CreateKeyboards();

    CARAPI SetEnableHaptics(
        /* [in] */ Boolean enabled);

    CARAPI IsAlpha(
        /* [out] */ Boolean* alpha);

    CARAPI SetKeyboardMode(
        /* [in] */ Int32 mode);

    CARAPI SendDownUpKeyEvents(
        /* [in] */ Int32 keyEventCode);

    CARAPI OnKey(
        /* [in] */ Int32 primaryCode,
        /* [in] */ ArrayOf<Int32>* keyCodes);

    CARAPI SetVibratePattern(
        /* [in] */ Int32 id);

    CARAPI HandleBackspace();

    CARAPI OnPress(
        /* [in] */ Int32 primaryCode);

    CARAPI OnRelease(
        /* [in] */ Int32 primaryCode);

    CARAPI OnText(
        /* [in] */ ICharSequence* text);

    CARAPI SwipeDown();

    CARAPI SwipeLeft();

    CARAPI SwipeRight();

    CARAPI SwipeUp();
};

}// namespace Internal
}// namespace Widget
}// namespace Droid
}// namespace Elastos

#endif
