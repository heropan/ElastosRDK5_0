
#include "elastos/droid/widget/internal/PasswordEntryKeyboardHelper.h"
#include "elastos/droid/view/CKeyCharacterMap.h"
#include "elastos/droid/view/View.h"
#include "elastos/droid/view/CKeyEvent.h"
#include "elastos/droid/provider/Settings.h"
#include "elastos/droid/os/SystemClock.h"
#include "elastos/droid/view/ViewRootImpl.h"

using Elastos::Droid::Os::SystemClock;
using Elastos::Droid::Provider::Settings;
using Elastos::Droid::Provider::ISettingsSystem;
using Elastos::Droid::View::View;
using Elastos::Droid::View::EIID_View;
using Elastos::Droid::View::IKeyEvent;
using Elastos::Droid::View::CKeyEvent;
using Elastos::Droid::View::IKeyCharacterMap;
using Elastos::Droid::View::ViewRootImpl;
using Elastos::Droid::View::CKeyCharacterMap;
using Elastos::Droid::View::IViewGroupLayoutParams;
using Elastos::Droid::View::IHapticFeedbackConstants;
using Elastos::Droid::InputMethodService::IKeyboard;
using Elastos::Droid::InputMethodService::EIID_IKeyboard;
using Elastos::Droid::InputMethodService::IOnKeyboardActionListener;
using Elastos::Droid::InputMethodService::EIID_IOnKeyboardActionListener;

namespace Elastos {
namespace Droid {
namespace Widget {
namespace Internal {

const String PasswordEntryKeyboardHelper::TAG("PasswordEntryKeyboardHelper");
const Int32 PasswordEntryKeyboardHelper::KEYBOARD_STATE_NORMAL;
const Int32 PasswordEntryKeyboardHelper::KEYBOARD_STATE_SHIFTED;
const Int32 PasswordEntryKeyboardHelper::KEYBOARD_STATE_CAPSLOCK;
const Int32 PasswordEntryKeyboardHelper::NUMERIC;
const Int32 PasswordEntryKeyboardHelper::QWERTY;
const Int32 PasswordEntryKeyboardHelper::QWERTY_SHIFTED;
const Int32 PasswordEntryKeyboardHelper::SYMBOLS;
const Int32 PasswordEntryKeyboardHelper::SYMBOLS_SHIFTED;

PasswordEntryKeyboardHelper::PasswordEntryKeyboardHelper()
    : mKeyboardMode(KEYBOARD_MODE_ALPHA)
    , mKeyboardState(KEYBOARD_STATE_NORMAL)
    , mEnableHaptics(FALSE)
    , mUsingScreenWidth(FALSE)
{
    mLayouts = ArrayOf<Int32>::Alloc(LAYOUTS_LENGTH);
    (*mLayouts)[0] = R::xml::password_kbd_numeric;
    (*mLayouts)[1] = R::xml::password_kbd_qwerty;
    (*mLayouts)[2] = R::xml::password_kbd_qwerty_shifted;
    (*mLayouts)[3] = R::xml::password_kbd_symbols;
    (*mLayouts)[4] = R::xml::password_kbd_symbols_shift;
}
PasswordEntryKeyboardHelper::PasswordEntryKeyboardHelper(
    /* [in] */ IContext* context,
    /* [in] */ IKeyboardView* keyboardView,
    /* [in] */ IView* targetView)
    : mKeyboardMode(KEYBOARD_MODE_ALPHA)
    , mKeyboardState(KEYBOARD_STATE_NORMAL)
    , mEnableHaptics(FALSE)
    , mUsingScreenWidth(FALSE)
{
    mLayouts = ArrayOf<Int32>::Alloc(LAYOUTS_LENGTH);
    (*mLayouts)[0] = R::xml::password_kbd_numeric;
    (*mLayouts)[1] = R::xml::password_kbd_qwerty;
    (*mLayouts)[2] = R::xml::password_kbd_qwerty_shifted;
    (*mLayouts)[3] = R::xml::password_kbd_symbols;
    (*mLayouts)[4] = R::xml::password_kbd_symbols_shift;

    ASSERT_SUCCEEDED(Init(context, keyboardView, targetView, TRUE, NULL));
}

PasswordEntryKeyboardHelper::PasswordEntryKeyboardHelper(
    /* [in] */ IContext* context,
    /* [in] */ IKeyboardView* keyboardView,
    /* [in] */ IView* targetView,
    /* [in] */ Boolean useFullScreenWidth)
    : mKeyboardMode(KEYBOARD_MODE_ALPHA)
    , mKeyboardState(KEYBOARD_STATE_NORMAL)
    , mEnableHaptics(FALSE)
    , mUsingScreenWidth(FALSE)
{
    mLayouts = ArrayOf<Int32>::Alloc(LAYOUTS_LENGTH);
    (*mLayouts)[0] = R::xml::password_kbd_numeric;
    (*mLayouts)[1] = R::xml::password_kbd_qwerty;
    (*mLayouts)[2] = R::xml::password_kbd_qwerty_shifted;
    (*mLayouts)[3] = R::xml::password_kbd_symbols;
    (*mLayouts)[4] = R::xml::password_kbd_symbols_shift;

    ASSERT_SUCCEEDED(Init(context, keyboardView, targetView, useFullScreenWidth, NULL));
}

PasswordEntryKeyboardHelper::PasswordEntryKeyboardHelper(
    /* [in] */ IContext* context,
    /* [in] */ IKeyboardView* keyboardView,
    /* [in] */ IView* targetView,
    /* [in] */ Boolean useFullScreenWidth,
    /* [in] */ ArrayOf<Int32>* layouts)
    : mKeyboardMode(KEYBOARD_MODE_ALPHA)
    , mKeyboardState(KEYBOARD_STATE_NORMAL)
    , mEnableHaptics(FALSE)
    , mUsingScreenWidth(FALSE)
{
    mLayouts = ArrayOf<Int32>::Alloc(LAYOUTS_LENGTH);
    (*mLayouts)[0] = R::xml::password_kbd_numeric;
    (*mLayouts)[1] = R::xml::password_kbd_qwerty;
    (*mLayouts)[2] = R::xml::password_kbd_qwerty_shifted;
    (*mLayouts)[3] = R::xml::password_kbd_symbols;
    (*mLayouts)[4] = R::xml::password_kbd_symbols_shift;

    ASSERT_SUCCEEDED(Init(context, keyboardView, targetView, useFullScreenWidth, layouts));
}

CARAPI PasswordEntryKeyboardHelper::Init(
    /* [in] */ IContext* context,
    /* [in] */ IKeyboardView* keyboardView,
    /* [in] */ IView* targetView,
    /* [in] */ Boolean useFullScreenWidth,
    /* [in] */ ArrayOf<Int32>* layouts)
{
    mContext = context;
    mTargetView = targetView;
    mKeyboardView = keyboardView;
    mKeyboardView->SetOnKeyboardActionListener(THIS_PROBE(IOnKeyboardActionListener));
    mUsingScreenWidth = useFullScreenWidth;
    if (layouts != NULL) {
        if (layouts->GetLength() != mLayouts->GetLength()) {
            return E_RUNTIME_EXCEPTION;
            //throw new RuntimeException("Wrong number of layouts");
        }
        for (Int32 i = 0; i < mLayouts->GetLength(); i++) {
            (*mLayouts)[i] = (*layouts)[i];
        }
    }
    CreateKeyboards();
    return NOERROR;
}

ECode PasswordEntryKeyboardHelper::CreateKeyboards()
{
    AutoPtr<IViewGroupLayoutParams> lp;
    mKeyboardView->GetLayoutParams((IViewGroupLayoutParams**)&lp);
    Int32 width, height;
    lp->GetWidth(&width);
    lp->GetHeight(&height);

    if (mUsingScreenWidth || (width == IViewGroupLayoutParams::MATCH_PARENT)) {
        CreateKeyboardsWithDefaultWidth();
    } else {
        CreateKeyboardsWithSpecificSize(width, height);
    }
    return NOERROR;
}

ECode PasswordEntryKeyboardHelper::SetEnableHaptics(
    /* [in] */ Boolean enabled)
{
    mEnableHaptics = enabled;

    return NOERROR;
}

Boolean PasswordEntryKeyboardHelper::IsAlpha()
{
    return mKeyboardMode == KEYBOARD_MODE_ALPHA;
}

void PasswordEntryKeyboardHelper::CreateKeyboardsWithSpecificSize(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    mNumericKeyboard = NULL;
    CPasswordEntryKeyboard::NewByFriend(mContext, (*mLayouts)[NUMERIC], 0,
        width, height, (CPasswordEntryKeyboard**)&mNumericKeyboard);
    mQwertyKeyboard = NULL;
    CPasswordEntryKeyboard::NewByFriend(mContext, (*mLayouts)[QWERTY], R::id::mode_normal,
        width, height, (CPasswordEntryKeyboard**)&mQwertyKeyboard);
    mQwertyKeyboard->EnableShiftLock();

    mQwertyKeyboardShifted = NULL;
    CPasswordEntryKeyboard::NewByFriend(mContext, (*mLayouts)[QWERTY_SHIFTED],
        R::id::mode_normal, width, height, (CPasswordEntryKeyboard**)&mQwertyKeyboardShifted);
    mQwertyKeyboardShifted->EnableShiftLock();
    Boolean rst;
    mQwertyKeyboardShifted->SetShifted(TRUE, &rst); // always shifted.

    mSymbolsKeyboard = NULL;
    CPasswordEntryKeyboard::NewByFriend(mContext, (*mLayouts)[SYMBOLS], 0, width, height, (CPasswordEntryKeyboard**)&mSymbolsKeyboard);
    mSymbolsKeyboard->EnableShiftLock();

    mSymbolsKeyboardShifted = NULL;
    CPasswordEntryKeyboard::NewByFriend(mContext, (*mLayouts)[SYMBOLS_SHIFTED], 0,
            width, height, (CPasswordEntryKeyboard**)&mSymbolsKeyboardShifted);
    mSymbolsKeyboardShifted->EnableShiftLock();
    mSymbolsKeyboardShifted->SetShifted(TRUE, &rst); // always shifted
}

void PasswordEntryKeyboardHelper::CreateKeyboardsWithDefaultWidth()
{
    mNumericKeyboard = NULL;
    CPasswordEntryKeyboard::NewByFriend(mContext, (*mLayouts)[NUMERIC],
        (CPasswordEntryKeyboard**)&mNumericKeyboard);
    mQwertyKeyboard = NULL;
    CPasswordEntryKeyboard::NewByFriend(mContext, (*mLayouts)[QWERTY],
        R::id::mode_normal, (CPasswordEntryKeyboard**)&mQwertyKeyboard);
    mQwertyKeyboard->EnableShiftLock();

    mQwertyKeyboardShifted = NULL;
    CPasswordEntryKeyboard::NewByFriend(mContext, (*mLayouts)[QWERTY_SHIFTED],
        R::id::mode_normal, (CPasswordEntryKeyboard**)&mQwertyKeyboardShifted);
    mQwertyKeyboardShifted->EnableShiftLock();
    Boolean rst;
    mQwertyKeyboardShifted->SetShifted(TRUE, &rst); // always shifted.

    mSymbolsKeyboard = NULL;
    CPasswordEntryKeyboard::NewByFriend(mContext, (*mLayouts)[SYMBOLS], (CPasswordEntryKeyboard**)&mSymbolsKeyboard);
    mSymbolsKeyboard->EnableShiftLock();

    mSymbolsKeyboardShifted = NULL;
    CPasswordEntryKeyboard::NewByFriend(mContext, (*mLayouts)[SYMBOLS_SHIFTED], (CPasswordEntryKeyboard**)&mSymbolsKeyboardShifted);
    mSymbolsKeyboardShifted->EnableShiftLock();
    mSymbolsKeyboardShifted->SetShifted(TRUE, &rst); // always shifted
}

ECode PasswordEntryKeyboardHelper::SetKeyboardMode(
    /* [in] */ Int32 mode)
{
    switch (mode) {
        case KEYBOARD_MODE_ALPHA:
        {
            mKeyboardView->SetKeyboard((IKeyboard*)(mQwertyKeyboard->Probe(EIID_IKeyboard)));
            mKeyboardState = KEYBOARD_STATE_NORMAL;
            AutoPtr<IContentResolver> cr;
            mContext->GetContentResolver((IContentResolver**)&cr);
            Boolean visiblePassword;
            Int32 value;
            FAIL_RETURN(Settings::System::GetInt32(
                    cr, ISettingsSystem::TEXT_SHOW_PASSWORD, 1, &value))
            visiblePassword = value != 0;
            Boolean enablePreview = FALSE; // TODO: grab from configuration
            mKeyboardView->SetPreviewEnabled(visiblePassword && enablePreview);
            break;
        }
        case KEYBOARD_MODE_NUMERIC:
            mKeyboardView->SetKeyboard((IKeyboard*)(mNumericKeyboard->Probe(EIID_IKeyboard)));
            mKeyboardState = KEYBOARD_STATE_NORMAL;
            mKeyboardView->SetPreviewEnabled(FALSE); // never show popup for numeric keypad
            break;
    }
    mKeyboardMode = mode;
    return NOERROR;
}

void PasswordEntryKeyboardHelper::SendKeyEventsToTarget(
    /* [in] */ Int32 character)
{
    AutoPtr<ViewRootImpl> viewRootImpl =
        reinterpret_cast<Elastos::Droid::View::View*>((mTargetView)->Probe(EIID_View))->GetViewRootImpl();
    AutoPtr<ArrayOf<IKeyEvent*> > events;
    AutoPtr<IKeyCharacterMap> kcm;
    CKeyCharacterMap::Load(IKeyCharacterMap::VIRTUAL_KEYBOARD, (IKeyCharacterMap**)&kcm);
    AutoPtr<ArrayOf<Char32> > param = ArrayOf<Char32>::Alloc(1);
    (*param)[0] = character;
    kcm->GetEvents(param, (ArrayOf<IKeyEvent*>**)&events);
    if (events != NULL) {
        Int32 N = events->GetLength();
        for (Int32 i = 0; i < N; i++) {
            AutoPtr<IKeyEvent> event = (*events)[i];
            Int32 flags;
            event->GetFlags(&flags);
            AutoPtr<IKeyEvent> newEvent;
            CKeyEvent::ChangeFlags(event, flags
                | IKeyEvent::FLAG_SOFT_KEYBOARD | IKeyEvent::FLAG_KEEP_TOUCH_MODE, (IKeyEvent**)&newEvent);
            viewRootImpl->DispatchKey(newEvent);
        }
    }
}

ECode PasswordEntryKeyboardHelper::SendDownUpKeyEvents(
    /* [in] */ Int32 keyEventCode)
{
    Int64 eventTime = SystemClock::GetUptimeMillis();
    AutoPtr<ViewRootImpl> viewRootImpl =
        reinterpret_cast<Elastos::Droid::View::View*>((mTargetView)->Probe(EIID_View))->GetViewRootImpl();
    AutoPtr<IKeyEvent> keyEvent;
    CKeyEvent::New(eventTime, eventTime, IKeyEvent::ACTION_DOWN, keyEventCode, 0, 0,
                        IKeyCharacterMap::VIRTUAL_KEYBOARD, 0,
                    IKeyEvent::FLAG_SOFT_KEYBOARD|IKeyEvent::FLAG_KEEP_TOUCH_MODE, (IKeyEvent**)&keyEvent);
    viewRootImpl->DispatchKeyFromIme(keyEvent);
    AutoPtr<IKeyEvent> keyEvent2;
    CKeyEvent::New(eventTime, eventTime, IKeyEvent::ACTION_UP, keyEventCode, 0, 0,
        IKeyCharacterMap::VIRTUAL_KEYBOARD, 0,
        IKeyEvent::FLAG_SOFT_KEYBOARD|IKeyEvent::FLAG_KEEP_TOUCH_MODE, (IKeyEvent**)&keyEvent2);
    viewRootImpl->DispatchKeyFromIme(keyEvent2);
    return NOERROR;
}

ECode PasswordEntryKeyboardHelper::OnKey(
    /* [in] */ Int32 primaryCode,
    /* [in] */ ArrayOf<Int32>* keyCodes)
{
    if (primaryCode == IKeyboard::KEYCODE_DELETE) {
        HandleBackspace();
    } else if (primaryCode == IKeyboard::KEYCODE_SHIFT) {
        HandleShift();
    } else if (primaryCode == IKeyboard::KEYCODE_CANCEL) {
        HandleClose();
        return NOERROR;
    } else if (primaryCode == IKeyboard::KEYCODE_MODE_CHANGE && mKeyboardView != NULL) {
        HandleModeChange();
    } else {
        HandleCharacter(primaryCode, keyCodes);
        // Switch back to old keyboard if we're not in capslock mode
        if (mKeyboardState == KEYBOARD_STATE_SHIFTED) {
            // skip to the unlocked state
            mKeyboardState = KEYBOARD_STATE_SHIFTED;
            HandleShift();
        }
    }

    return NOERROR;
}

/**
 * Sets and enables vibrate pattern.  If id is 0 (or can't be loaded), vibrate is disabled.
 * @param id resource id for array containing vibrate pattern.
 */
ECode PasswordEntryKeyboardHelper::SetVibratePattern(
    /* [in] */ Int32 id)
{
    AutoPtr<ArrayOf<Int32> > tmpArray;
    //try {
        AutoPtr<IResources> rs;
        mContext->GetResources((IResources**)&rs);
        rs->GetIntArray(id, (ArrayOf<Int32>**)&tmpArray);
    /*} catch (Resources.NotFoundException e) {
        if (id != 0) {
            Log.e(TAG, "Vibrate pattern missing", e);
        }
    }*/
    if (tmpArray == NULL) {
        mVibratePattern = NULL;
        return NOERROR;
    }
    mVibratePattern = ArrayOf<Int64>::Alloc(tmpArray->GetLength());
    for (Int32 i = 0; i < tmpArray->GetLength(); i++) {
        (*mVibratePattern)[i] = (*tmpArray)[i];
    }
    return NOERROR;
}

void PasswordEntryKeyboardHelper::HandleModeChange()
{
    AutoPtr<IKeyboard> current;
    mKeyboardView->GetKeyboard((IKeyboard**)&current);

    AutoPtr<IKeyboard> next = NULL;
    if ((IKeyboard*)(mQwertyKeyboard->Probe(EIID_IKeyboard)) == current ||
            (IKeyboard*)(mQwertyKeyboardShifted->Probe(EIID_IKeyboard)) == current) {
        next = (IKeyboard*)(mSymbolsKeyboard->Probe(EIID_IKeyboard));
    }
    else if ((IKeyboard*)(mSymbolsKeyboard->Probe(EIID_IKeyboard)) == current ||
            (IKeyboard*)(mSymbolsKeyboardShifted->Probe(EIID_IKeyboard)) == current) {
        next = (IKeyboard*)(mQwertyKeyboard->Probe(EIID_IKeyboard));
    }
    if (next != NULL) {
        mKeyboardView->SetKeyboard(next);
        mKeyboardState = KEYBOARD_STATE_NORMAL;
    }
}

ECode PasswordEntryKeyboardHelper::HandleBackspace()
{
    SendDownUpKeyEvents(IKeyEvent::KEYCODE_DEL);
    PerformHapticFeedback();
    return NOERROR;
}

void PasswordEntryKeyboardHelper::HandleShift()
{
    if (mKeyboardView == NULL) {
        return;
    }
    AutoPtr<IKeyboard> current;
    mKeyboardView->GetKeyboard((IKeyboard**)&current);
    AutoPtr<CPasswordEntryKeyboard> next = NULL;
    Boolean isAlphaMode = (IKeyboard*)(mQwertyKeyboard->Probe(EIID_IKeyboard)) == current
            || (IKeyboard*)(mQwertyKeyboardShifted->Probe(EIID_IKeyboard)) == current;
    if (mKeyboardState == KEYBOARD_STATE_NORMAL) {
        mKeyboardState = isAlphaMode ? KEYBOARD_STATE_SHIFTED : KEYBOARD_STATE_CAPSLOCK;
        next = isAlphaMode ? mQwertyKeyboardShifted : mSymbolsKeyboardShifted;
    } else if (mKeyboardState == KEYBOARD_STATE_SHIFTED) {
        mKeyboardState = KEYBOARD_STATE_CAPSLOCK;
        next = isAlphaMode ? mQwertyKeyboardShifted : mSymbolsKeyboardShifted;
    } else if (mKeyboardState == KEYBOARD_STATE_CAPSLOCK) {
        mKeyboardState = KEYBOARD_STATE_NORMAL;
        next = isAlphaMode ? mQwertyKeyboard : mSymbolsKeyboard;
    }
    if (next != NULL) {
        if ((IKeyboard*)(next->Probe(EIID_IKeyboard)) != current) {
            mKeyboardView->SetKeyboard((IKeyboard*)(next->Probe(EIID_IKeyboard)));
        }
        next->SetShiftLocked(mKeyboardState == KEYBOARD_STATE_CAPSLOCK);
        Boolean rst;
        mKeyboardView->SetShifted(mKeyboardState != KEYBOARD_STATE_NORMAL, &rst);
    }
}

void PasswordEntryKeyboardHelper::HandleCharacter(
    /* [in] */ Int32 primaryCode,
    /* [in] */ ArrayOf<Int32>* keyCodes)
{
    // Maybe turn off shift if not in capslock mode.
    Boolean isShifted;
    mKeyboardView->IsShifted(&isShifted);
    if (isShifted && primaryCode != ' ' && primaryCode != '\n') {
        if(primaryCode >= 97 && primaryCode <= 122)
            primaryCode += 32;
    }
    SendKeyEventsToTarget(primaryCode);
}

void PasswordEntryKeyboardHelper::HandleClose()
{

}

ECode PasswordEntryKeyboardHelper::OnPress(
    /* [in] */ Int32 primaryCode)
{
    PerformHapticFeedback();

    return NOERROR;
}

void PasswordEntryKeyboardHelper::PerformHapticFeedback()
{
    if (mEnableHaptics) {
        Boolean rst;
        mKeyboardView->PerformHapticFeedback(IHapticFeedbackConstants::VIRTUAL_KEY,
                IHapticFeedbackConstants::FLAG_IGNORE_VIEW_SETTING
                | IHapticFeedbackConstants::FLAG_IGNORE_GLOBAL_SETTING, &rst);
    }
}

ECode PasswordEntryKeyboardHelper::OnRelease(
    /* [in] */ Int32 primaryCode)
{
    return NOERROR;
}

ECode PasswordEntryKeyboardHelper::OnText(
    /* [in] */ ICharSequence* text)
{
    return NOERROR;
}

ECode PasswordEntryKeyboardHelper::SwipeDown()
{
    return NOERROR;
}

ECode PasswordEntryKeyboardHelper::SwipeLeft()
{
    return NOERROR;
}

ECode PasswordEntryKeyboardHelper::SwipeRight()
{
    return NOERROR;
}

ECode PasswordEntryKeyboardHelper::SwipeUp()
{
    return NOERROR;
}

}// namespace Internal
}// namespace Widget
}// namespace Droid
}// namespace Elastos
