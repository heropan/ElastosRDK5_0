
#include "view/CKeyEvent.h"
#include "view/CInputDevice.h"
#include <androidfw/Input.h>
#include <elastos/core/StringBuilder.h>
#include <elastos/utility/logging/Logger.h>
#include <elastos/core/StringUtils.h>

using Elastos::Core::Mutex;
using Elastos::Core::StringUtils;
using Elastos::Core::StringBuilder;
using Elastos::Utility::Etl::HashMap;
using Elastos::Utility::Logging::Logger;
//using Elastos::Droid::Text::Method::IMetaKeyKeyListener;

namespace Elastos {
namespace Droid {
namespace View {

const Boolean CKeyEvent::DEBUG = FALSE;
const char* CKeyEvent::TAG = "KeyEvent";
const Int32 CKeyEvent::LAST_KEYCODE;

HashMap<Int32, String> CKeyEvent::KEYCODE_SYMBOLIC_NAMES;
const String CKeyEvent::META_SYMBOLIC_NAMES[] = {
    String("META_SHIFT_ON"),
    String("META_ALT_ON"),
    String("META_SYM_ON"),
    String("META_FUNCTION_ON"),
    String("META_ALT_LEFT_ON"),
    String("META_ALT_RIGHT_ON"),
    String("META_SHIFT_LEFT_ON"),
    String("META_SHIFT_RIGHT_ON"),
    String("META_CAP_LOCKED"),
    String("META_ALT_LOCKED"),
    String("META_SYM_LOCKED"),
    String("0x00000800"),
    String("META_CTRL_ON"),
    String("META_CTRL_LEFT_ON"),
    String("META_CTRL_RIGHT_ON"),
    String("0x00008000"),
    String("META_META_ON"),
    String("META_META_LEFT_ON"),
    String("META_META_RIGHT_ON"),
    String("0x00080000"),
    String("META_CAPS_LOCK_ON"),
    String("META_NUM_LOCK_ON"),
    String("META_SCROLL_LOCK_ON"),
    String("0x00800000"),
    String("0x01000000"),
    String("0x02000000"),
    String("0x04000000"),
    String("0x08000000"),
    String("0x10000000"),
    String("0x20000000"),
    String("0x40000000"),
    String("0x80000000"),
};

const Int32 CKeyEvent::MAX_RECYCLED;
Mutex CKeyEvent::gRecyclerLock;

Int32 CKeyEvent::gRecyclerUsed = 0;
AutoPtr<CKeyEvent> CKeyEvent::gRecyclerTop;

const Int32 CKeyEvent::META_MODIFIER_MASK;
const Int32 CKeyEvent::META_LOCK_MASK;
const Int32 CKeyEvent::META_ALL_MASK;
const Int32 CKeyEvent::META_SYNTHETIC_MASK;
const Int32 CKeyEvent::META_INVALID_MODIFIER_MASK;

const Boolean CKeyEvent::mIsStaticInited = CKeyEvent::InitStatic();

Boolean CKeyEvent::InitStatic()
{
    PopulateKeycodeSymbolicNames();
    return TRUE;
}

void CKeyEvent::PopulateKeycodeSymbolicNames()
{
    HashMap<Int32, String>& names = KEYCODE_SYMBOLIC_NAMES;
    names[KEYCODE_UNKNOWN] = String("KEYCODE_UNKNOWN");
    names[KEYCODE_SOFT_LEFT] = String("KEYCODE_SOFT_LEFT");
    names[KEYCODE_SOFT_RIGHT] = String("KEYCODE_SOFT_RIGHT");
    names[KEYCODE_HOME] = String("KEYCODE_HOME");
    names[KEYCODE_BACK] = String("KEYCODE_BACK");
    names[KEYCODE_CALL] = String("KEYCODE_CALL");
    names[KEYCODE_ENDCALL] = String("KEYCODE_ENDCALL");
    names[KEYCODE_0] = String("KEYCODE_0");
    names[KEYCODE_1] = String("KEYCODE_1");
    names[KEYCODE_2] = String("KEYCODE_2");
    names[KEYCODE_3] = String("KEYCODE_3");
    names[KEYCODE_4] = String("KEYCODE_4");
    names[KEYCODE_5] = String("KEYCODE_5");
    names[KEYCODE_6] = String("KEYCODE_6");
    names[KEYCODE_7] = String("KEYCODE_7");
    names[KEYCODE_8] = String("KEYCODE_8");
    names[KEYCODE_9] = String("KEYCODE_9");
    names[KEYCODE_STAR] = String("KEYCODE_STAR");
    names[KEYCODE_POUND] = String("KEYCODE_POUND");
    names[KEYCODE_DPAD_UP] = String("KEYCODE_DPAD_UP");
    names[KEYCODE_DPAD_DOWN] = String("KEYCODE_DPAD_DOWN");
    names[KEYCODE_DPAD_LEFT] = String("KEYCODE_DPAD_LEFT");
    names[KEYCODE_DPAD_RIGHT] = String("KEYCODE_DPAD_RIGHT");
    names[KEYCODE_DPAD_CENTER] = String("KEYCODE_DPAD_CENTER");
    names[KEYCODE_VOLUME_UP] = String("KEYCODE_VOLUME_UP");
    names[KEYCODE_VOLUME_DOWN] = String("KEYCODE_VOLUME_DOWN");
    names[KEYCODE_POWER] = String("KEYCODE_POWER");
    names[KEYCODE_CAMERA] = String("KEYCODE_CAMERA");
    names[KEYCODE_CLEAR] = String("KEYCODE_CLEAR");
    names[KEYCODE_A] = String("KEYCODE_A");
    names[KEYCODE_B] = String("KEYCODE_B");
    names[KEYCODE_C] = String("KEYCODE_C");
    names[KEYCODE_D] = String("KEYCODE_D");
    names[KEYCODE_E] = String("KEYCODE_E");
    names[KEYCODE_F] = String("KEYCODE_F");
    names[KEYCODE_G] = String("KEYCODE_G");
    names[KEYCODE_H] = String("KEYCODE_H");
    names[KEYCODE_I] = String("KEYCODE_I");
    names[KEYCODE_J] = String("KEYCODE_J");
    names[KEYCODE_K] = String("KEYCODE_K");
    names[KEYCODE_L] = String("KEYCODE_L");
    names[KEYCODE_M] = String("KEYCODE_M");
    names[KEYCODE_N] = String("KEYCODE_N");
    names[KEYCODE_O] = String("KEYCODE_O");
    names[KEYCODE_P] = String("KEYCODE_P");
    names[KEYCODE_Q] = String("KEYCODE_Q");
    names[KEYCODE_R] = String("KEYCODE_R");
    names[KEYCODE_S] = String("KEYCODE_S");
    names[KEYCODE_T] = String("KEYCODE_T");
    names[KEYCODE_U] = String("KEYCODE_U");
    names[KEYCODE_V] = String("KEYCODE_V");
    names[KEYCODE_W] = String("KEYCODE_W");
    names[KEYCODE_X] = String("KEYCODE_X");
    names[KEYCODE_Y] = String("KEYCODE_Y");
    names[KEYCODE_Z] = String("KEYCODE_Z");
    names[KEYCODE_COMMA] = String("KEYCODE_COMMA");
    names[KEYCODE_PERIOD] = String("KEYCODE_PERIOD");
    names[KEYCODE_ALT_LEFT] = String("KEYCODE_ALT_LEFT");
    names[KEYCODE_ALT_RIGHT] = String("KEYCODE_ALT_RIGHT");
    names[KEYCODE_SHIFT_LEFT] = String("KEYCODE_SHIFT_LEFT");
    names[KEYCODE_SHIFT_RIGHT] = String("KEYCODE_SHIFT_RIGHT");
    names[KEYCODE_TAB] = String("KEYCODE_TAB");
    names[KEYCODE_SPACE] = String("KEYCODE_SPACE");
    names[KEYCODE_SYM] = String("KEYCODE_SYM");
    names[KEYCODE_EXPLORER] = String("KEYCODE_EXPLORER");
    names[KEYCODE_ENVELOPE] = String("KEYCODE_ENVELOPE");
    names[KEYCODE_ENTER] = String("KEYCODE_ENTER");
    names[KEYCODE_DEL] = String("KEYCODE_DEL");
    names[KEYCODE_GRAVE] = String("KEYCODE_GRAVE");
    names[KEYCODE_MINUS] = String("KEYCODE_MINUS");
    names[KEYCODE_EQUALS] = String("KEYCODE_EQUALS");
    names[KEYCODE_LEFT_BRACKET] = String("KEYCODE_LEFT_BRACKET");
    names[KEYCODE_RIGHT_BRACKET] = String("KEYCODE_RIGHT_BRACKET");
    names[KEYCODE_BACKSLASH] = String("KEYCODE_BACKSLASH");
    names[KEYCODE_SEMICOLON] = String("KEYCODE_SEMICOLON");
    names[KEYCODE_APOSTROPHE] = String("KEYCODE_APOSTROPHE");
    names[KEYCODE_SLASH] = String("KEYCODE_SLASH");
    names[KEYCODE_AT] = String("KEYCODE_AT");
    names[KEYCODE_NUM] = String("KEYCODE_NUM");
    names[KEYCODE_HEADSETHOOK] = String("KEYCODE_HEADSETHOOK");
    names[KEYCODE_FOCUS] = String("KEYCODE_FOCUS");
    names[KEYCODE_PLUS] = String("KEYCODE_PLUS");
    names[KEYCODE_MENU] = String("KEYCODE_MENU");
    names[KEYCODE_NOTIFICATION] = String("KEYCODE_NOTIFICATION");
    names[KEYCODE_SEARCH] = String("KEYCODE_SEARCH");
    names[KEYCODE_MEDIA_PLAY_PAUSE] = String("KEYCODE_MEDIA_PLAY_PAUSE");
    names[KEYCODE_MEDIA_STOP] = String("KEYCODE_MEDIA_STOP");
    names[KEYCODE_MEDIA_NEXT] = String("KEYCODE_MEDIA_NEXT");
    names[KEYCODE_MEDIA_PREVIOUS] = String("KEYCODE_MEDIA_PREVIOUS");
    names[KEYCODE_MEDIA_REWIND] = String("KEYCODE_MEDIA_REWIND");
    names[KEYCODE_MEDIA_FAST_FORWARD] = String("KEYCODE_MEDIA_FAST_FORWARD");
    names[KEYCODE_MUTE] = String("KEYCODE_MUTE");
    names[KEYCODE_PAGE_UP] = String("KEYCODE_PAGE_UP");
    names[KEYCODE_PAGE_DOWN] = String("KEYCODE_PAGE_DOWN");
    names[KEYCODE_PICTSYMBOLS] = String("KEYCODE_PICTSYMBOLS");
    names[KEYCODE_SWITCH_CHARSET] = String("KEYCODE_SWITCH_CHARSET");
    names[KEYCODE_BUTTON_A] = String("KEYCODE_BUTTON_A");
    names[KEYCODE_BUTTON_B] = String("KEYCODE_BUTTON_B");
    names[KEYCODE_BUTTON_C] = String("KEYCODE_BUTTON_C");
    names[KEYCODE_BUTTON_X] = String("KEYCODE_BUTTON_X");
    names[KEYCODE_BUTTON_Y] = String("KEYCODE_BUTTON_Y");
    names[KEYCODE_BUTTON_Z] = String("KEYCODE_BUTTON_Z");
    names[KEYCODE_BUTTON_L1] = String("KEYCODE_BUTTON_L1");
    names[KEYCODE_BUTTON_R1] = String("KEYCODE_BUTTON_R1");
    names[KEYCODE_BUTTON_L2] = String("KEYCODE_BUTTON_L2");
    names[KEYCODE_BUTTON_R2] = String("KEYCODE_BUTTON_R2");
    names[KEYCODE_BUTTON_THUMBL] = String("KEYCODE_BUTTON_THUMBL");
    names[KEYCODE_BUTTON_THUMBR] = String("KEYCODE_BUTTON_THUMBR");
    names[KEYCODE_BUTTON_START] = String("KEYCODE_BUTTON_START");
    names[KEYCODE_BUTTON_SELECT] = String("KEYCODE_BUTTON_SELECT");
    names[KEYCODE_BUTTON_MODE] = String("KEYCODE_BUTTON_MODE");
    names[KEYCODE_ESCAPE] = String("KEYCODE_ESCAPE");
    names[KEYCODE_FORWARD_DEL] = String("KEYCODE_FORWARD_DEL");
    names[KEYCODE_CTRL_LEFT] = String("KEYCODE_CTRL_LEFT");
    names[KEYCODE_CTRL_RIGHT] = String("KEYCODE_CTRL_RIGHT");
    names[KEYCODE_CAPS_LOCK] = String("KEYCODE_CAPS_LOCK");
    names[KEYCODE_SCROLL_LOCK] = String("KEYCODE_SCROLL_LOCK");
    names[KEYCODE_META_LEFT] = String("KEYCODE_META_LEFT");
    names[KEYCODE_META_RIGHT] = String("KEYCODE_META_RIGHT");
    names[KEYCODE_FUNCTION] = String("KEYCODE_FUNCTION");
    names[KEYCODE_SYSRQ] = String("KEYCODE_SYSRQ");
    names[KEYCODE_BREAK] = String("KEYCODE_BREAK");
    names[KEYCODE_MOVE_HOME] = String("KEYCODE_MOVE_HOME");
    names[KEYCODE_MOVE_END] = String("KEYCODE_MOVE_END");
    names[KEYCODE_INSERT] = String("KEYCODE_INSERT");
    names[KEYCODE_FORWARD] = String("KEYCODE_FORWARD");
    names[KEYCODE_MEDIA_PLAY] = String("KEYCODE_MEDIA_PLAY");
    names[KEYCODE_MEDIA_PAUSE] = String("KEYCODE_MEDIA_PAUSE");
    names[KEYCODE_MEDIA_CLOSE] = String("KEYCODE_MEDIA_CLOSE");
    names[KEYCODE_MEDIA_EJECT] = String("KEYCODE_MEDIA_EJECT");
    names[KEYCODE_MEDIA_RECORD] = String("KEYCODE_MEDIA_RECORD");
    names[KEYCODE_F1] = String("KEYCODE_F1");
    names[KEYCODE_F2] = String("KEYCODE_F2");
    names[KEYCODE_F3] = String("KEYCODE_F3");
    names[KEYCODE_F4] = String("KEYCODE_F4");
    names[KEYCODE_F5] = String("KEYCODE_F5");
    names[KEYCODE_F6] = String("KEYCODE_F6");
    names[KEYCODE_F7] = String("KEYCODE_F7");
    names[KEYCODE_F8] = String("KEYCODE_F8");
    names[KEYCODE_F9] = String("KEYCODE_F9");
    names[KEYCODE_F10] = String("KEYCODE_F10");
    names[KEYCODE_F11] = String("KEYCODE_F11");
    names[KEYCODE_F12] = String("KEYCODE_F12");
    names[KEYCODE_NUM_LOCK] = String("KEYCODE_NUM_LOCK");
    names[KEYCODE_NUMPAD_0] = String("KEYCODE_NUMPAD_0");
    names[KEYCODE_NUMPAD_1] = String("KEYCODE_NUMPAD_1");
    names[KEYCODE_NUMPAD_2] = String("KEYCODE_NUMPAD_2");
    names[KEYCODE_NUMPAD_3] = String("KEYCODE_NUMPAD_3");
    names[KEYCODE_NUMPAD_4] = String("KEYCODE_NUMPAD_4");
    names[KEYCODE_NUMPAD_5] = String("KEYCODE_NUMPAD_5");
    names[KEYCODE_NUMPAD_6] = String("KEYCODE_NUMPAD_6");
    names[KEYCODE_NUMPAD_7] = String("KEYCODE_NUMPAD_7");
    names[KEYCODE_NUMPAD_8] = String("KEYCODE_NUMPAD_8");
    names[KEYCODE_NUMPAD_9] = String("KEYCODE_NUMPAD_9");
    names[KEYCODE_NUMPAD_DIVIDE] = String("KEYCODE_NUMPAD_DIVIDE");
    names[KEYCODE_NUMPAD_MULTIPLY] = String("KEYCODE_NUMPAD_MULTIPLY");
    names[KEYCODE_NUMPAD_SUBTRACT] = String("KEYCODE_NUMPAD_SUBTRACT");
    names[KEYCODE_NUMPAD_ADD] = String("KEYCODE_NUMPAD_ADD");
    names[KEYCODE_NUMPAD_DOT] = String("KEYCODE_NUMPAD_DOT");
    names[KEYCODE_NUMPAD_COMMA] = String("KEYCODE_NUMPAD_COMMA");
    names[KEYCODE_NUMPAD_ENTER] = String("KEYCODE_NUMPAD_ENTER");
    names[KEYCODE_NUMPAD_EQUALS] = String("KEYCODE_NUMPAD_EQUALS");
    names[KEYCODE_NUMPAD_LEFT_PAREN] = String("KEYCODE_NUMPAD_LEFT_PAREN");
    names[KEYCODE_NUMPAD_RIGHT_PAREN] = String("KEYCODE_NUMPAD_RIGHT_PAREN");
    names[KEYCODE_VOLUME_MUTE] = String("KEYCODE_VOLUME_MUTE");
    names[KEYCODE_INFO] = String("KEYCODE_INFO");
    names[KEYCODE_CHANNEL_UP] = String("KEYCODE_CHANNEL_UP");
    names[KEYCODE_CHANNEL_DOWN] = String("KEYCODE_CHANNEL_DOWN");
    names[KEYCODE_ZOOM_IN] = String("KEYCODE_ZOOM_IN");
    names[KEYCODE_ZOOM_OUT] = String("KEYCODE_ZOOM_OUT");
    names[KEYCODE_TV] = String("KEYCODE_TV");
    names[KEYCODE_WINDOW] = String("KEYCODE_WINDOW");
    names[KEYCODE_GUIDE] = String("KEYCODE_GUIDE");
    names[KEYCODE_DVR] = String("KEYCODE_DVR");
    names[KEYCODE_BOOKMARK] = String("KEYCODE_BOOKMARK");
    names[KEYCODE_CAPTIONS] = String("KEYCODE_CAPTIONS");
    names[KEYCODE_SETTINGS] = String("KEYCODE_SETTINGS");
    names[KEYCODE_TV_POWER] = String("KEYCODE_TV_POWER");
    names[KEYCODE_TV_INPUT] = String("KEYCODE_TV_INPUT");
    names[KEYCODE_STB_INPUT] = String("KEYCODE_STB_INPUT");
    names[KEYCODE_STB_POWER] = String("KEYCODE_STB_POWER");
    names[KEYCODE_AVR_POWER] = String("KEYCODE_AVR_POWER");
    names[KEYCODE_AVR_INPUT] = String("KEYCODE_AVR_INPUT");
    names[KEYCODE_PROG_RED] = String("KEYCODE_PROG_RED");
    names[KEYCODE_PROG_GREEN] = String("KEYCODE_PROG_GREEN");
    names[KEYCODE_PROG_YELLOW] = String("KEYCODE_PROG_YELLOW");
    names[KEYCODE_PROG_BLUE] = String("KEYCODE_PROG_BLUE");
    names[KEYCODE_APP_SWITCH] = String("KEYCODE_APP_SWITCH");
    names[KEYCODE_BUTTON_1] = String("KEYCODE_BUTTON_1");
    names[KEYCODE_BUTTON_2] = String("KEYCODE_BUTTON_2");
    names[KEYCODE_BUTTON_3] = String("KEYCODE_BUTTON_3");
    names[KEYCODE_BUTTON_4] = String("KEYCODE_BUTTON_4");
    names[KEYCODE_BUTTON_5] = String("KEYCODE_BUTTON_5");
    names[KEYCODE_BUTTON_6] = String("KEYCODE_BUTTON_6");
    names[KEYCODE_BUTTON_7] = String("KEYCODE_BUTTON_7");
    names[KEYCODE_BUTTON_8] = String("KEYCODE_BUTTON_8");
    names[KEYCODE_BUTTON_9] = String("KEYCODE_BUTTON_9");
    names[KEYCODE_BUTTON_10] = String("KEYCODE_BUTTON_10");
    names[KEYCODE_BUTTON_11] = String("KEYCODE_BUTTON_11");
    names[KEYCODE_BUTTON_12] = String("KEYCODE_BUTTON_12");
    names[KEYCODE_BUTTON_13] = String("KEYCODE_BUTTON_13");
    names[KEYCODE_BUTTON_14] = String("KEYCODE_BUTTON_14");
    names[KEYCODE_BUTTON_15] = String("KEYCODE_BUTTON_15");
    names[KEYCODE_BUTTON_16] = String("KEYCODE_BUTTON_16");
    names[KEYCODE_LANGUAGE_SWITCH] = String("KEYCODE_LANGUAGE_SWITCH");
    names[KEYCODE_MANNER_MODE] = String("KEYCODE_MANNER_MODE");
    names[KEYCODE_3D_MODE] = String("KEYCODE_3D_MODE");
    names[KEYCODE_CONTACTS] = String("KEYCODE_CONTACTS");
    names[KEYCODE_CALENDAR] = String("KEYCODE_CALENDAR");
    names[KEYCODE_MUSIC] = String("KEYCODE_MUSIC");
    names[KEYCODE_CALCULATOR] = String("KEYCODE_CALCULATOR");
    names[KEYCODE_ZENKAKU_HANKAKU] = String("KEYCODE_ZENKAKU_HANKAKU");
    names[KEYCODE_EISU] = String("KEYCODE_EISU");
    names[KEYCODE_MUHENKAN] = String("KEYCODE_MUHENKAN");
    names[KEYCODE_HENKAN] = String("KEYCODE_HENKAN");
    names[KEYCODE_KATAKANA_HIRAGANA] = String("KEYCODE_KATAKANA_HIRAGANA");
    names[KEYCODE_YEN] = String("KEYCODE_YEN");
    names[KEYCODE_RO] = String("KEYCODE_RO");
    names[KEYCODE_KANA] = String("KEYCODE_KANA");
    names[KEYCODE_ASSIST] = String("KEYCODE_ASSIST");

    /* add by Gary. start {{----------------------------------- */
    /* 2011-10-27 */
    /* add some new keys for TVD */
    names[KEYCODE_TV_SYSTEM] = String("KEYCODE_TV_SYSTEM");
    names[KEYCODE_GOTO] = String("KEYCODE_GOTO");
    names[KEYCODE_SUBTITLE] = String("KEYCODE_SUBTITLE");
    names[KEYCODE_AUDIO] = String("KEYCODE_AUDIO");
    names[KEYCODE_ZOOM] = String("KEYCODE_ZOOM");
    names[KEYCODE_HELP] = String("KEYCODE_HELP");
    names[KEYCODE_FAVOURITE] = String("KEYCODE_FAVOURITE");
    names[KEYCODE_LOOP] = String("KEYCODE_LOOP");
    names[KEYCODE_EXPAND] = String("KEYCODE_EXPAND");
    names[KEYCODE_MOUSE] = String("KEYCODE_MOUSE");
    names[KEYCODE_MOVIE] = String("KEYCODE_MOVIE");
    names[KEYCODE_APPS] = String("KEYCODE_APPS");
    names[KEYCODE_BROWSER] = String("KEYCODE_BROWSER");
    /* add by Gary. end   -----------------------------------}} */
}

Int32 CKeyEvent::GetMaxKeyCode()
{
    return LAST_KEYCODE;
}

Int32 CKeyEvent::GetDeadChar(
    /* [in] */ Int32 accent,
    /* [in] */ Int32 c)
{
    return CKeyCharacterMap::GetDeadChar(accent, c);
}

ECode CKeyEvent::constructor()
{
    return NOERROR;
}

ECode CKeyEvent::constructor(
    /* [in] */ Int32 action,
    /* [in] */ Int32 code)
{
    mAction = action;
    mKeyCode = code;
    mRepeatCount = 0;
    mDeviceId = CKeyCharacterMap::VIRTUAL_KEYBOARD;

    return NOERROR;
}

ECode CKeyEvent::constructor(
    /* [in] */ Int64 downTime,
    /* [in] */ Int64 eventTime,
    /* [in] */ Int32 action,
    /* [in] */ Int32 code,
    /* [in] */ Int32 repeat)
{
    mDownTime = downTime;
    mEventTime = eventTime;
    mAction = action;
    mKeyCode = code;
    mRepeatCount = repeat;
    mDeviceId = CKeyCharacterMap::VIRTUAL_KEYBOARD;

    return NOERROR;
}

ECode CKeyEvent::constructor(
    /* [in] */ Int64 downTime,
    /* [in] */ Int64 eventTime,
    /* [in] */ Int32 action,
    /* [in] */ Int32 code,
    /* [in] */ Int32 repeat,
    /* [in] */ Int32 metaState)
{
    mDownTime = downTime;
    mEventTime = eventTime;
    mAction = action;
    mKeyCode = code;
    mRepeatCount = repeat;
    mMetaState = metaState;
    mDeviceId = CKeyCharacterMap::VIRTUAL_KEYBOARD;

    return NOERROR;
}

ECode CKeyEvent::constructor(
    /* [in] */ Int64 downTime,
    /* [in] */ Int64 eventTime,
    /* [in] */ Int32 action,
    /* [in] */ Int32 code,
    /* [in] */ Int32 repeat,
    /* [in] */ Int32 metaState,
    /* [in] */ Int32 deviceId,
    /* [in] */ Int32 scancode)
{
    mDownTime = downTime;
    mEventTime = eventTime;
    mAction = action;
    mKeyCode = code;
    mRepeatCount = repeat;
    mMetaState = metaState;
    mDeviceId = deviceId;
    mScanCode = scancode;

    return NOERROR;
}

ECode CKeyEvent::constructor(
    /* [in] */ Int64 downTime,
    /* [in] */ Int64 eventTime,
    /* [in] */ Int32 action,
    /* [in] */ Int32 code,
    /* [in] */ Int32 repeat,
    /* [in] */ Int32 metaState,
    /* [in] */ Int32 deviceId,
    /* [in] */ Int32 scancode,
    /* [in] */ Int32 flags)
{
    mDownTime = downTime;
    mEventTime = eventTime;
    mAction = action;
    mKeyCode = code;
    mRepeatCount = repeat;
    mMetaState = metaState;
    mDeviceId = deviceId;
    mScanCode = scancode;
    mFlags = flags;

    return NOERROR;
}

ECode CKeyEvent::constructor(
    /* [in] */ Int64 downTime,
    /* [in] */ Int64 eventTime,
    /* [in] */ Int32 action,
    /* [in] */ Int32 code,
    /* [in] */ Int32 repeat,
    /* [in] */ Int32 metaState,
    /* [in] */ Int32 deviceId,
    /* [in] */ Int32 scancode,
    /* [in] */ Int32 flags,
    /* [in] */ Int32 source)
{
    mDownTime = downTime;
    mEventTime = eventTime;
    mAction = action;
    mKeyCode = code;
    mRepeatCount = repeat;
    mMetaState = metaState;
    mDeviceId = deviceId;
    mScanCode = scancode;
    mFlags = flags;
    mSource = source;

    return NOERROR;
}

ECode CKeyEvent::constructor(
    /* [in] */ Int64 time,
    /* [in] */ const String& characters,
    /* [in] */ Int32 deviceId,
    /* [in] */ Int32 flags)
{
    mDownTime = time;
    mEventTime = time;
    mCharacters = characters;
    mAction = ACTION_MULTIPLE;
    mKeyCode = KEYCODE_UNKNOWN;
    mRepeatCount = 0;
    mDeviceId = deviceId;
    mFlags = flags;
    mSource = IInputDevice::SOURCE_KEYBOARD;

    return NOERROR;
}

ECode CKeyEvent::constructor(
    /* [in] */ IKeyEvent* origEvent)
{
    if (origEvent == NULL) {
        return E_INVALID_ARGUMENT;
    }

    CKeyEvent* event = (CKeyEvent*)origEvent;
    mDownTime = event->mDownTime;
    mEventTime = event->mEventTime;
    mAction = event->mAction;
    mKeyCode = event->mKeyCode;
    mRepeatCount = event->mRepeatCount;
    mMetaState = event->mMetaState;
    mDeviceId = event->mDeviceId;
    mSource = event->mSource;
    mScanCode = event->mScanCode;
    mFlags = event->mFlags;
    mCharacters = event->mCharacters;

    return NOERROR;
}

ECode CKeyEvent::constructor(
    /* [in] */ IKeyEvent* origEvent,
    /* [in] */ Int64 eventTime,
    /* [in] */ Int32 newRepeat)
{
    if (origEvent == NULL) {
        return E_INVALID_ARGUMENT;
    }

    CKeyEvent* event = (CKeyEvent*)origEvent;
    mDownTime = event->mDownTime;
    mEventTime = eventTime;
    mAction = event->mAction;
    mKeyCode = event->mKeyCode;
    mRepeatCount = newRepeat;
    mMetaState = event->mMetaState;
    mDeviceId = event->mDeviceId;
    mSource = event->mSource;
    mScanCode = event->mScanCode;
    mFlags = event->mFlags;
    mCharacters = event->mCharacters;

    return NOERROR;
}

ECode CKeyEvent::constructor(
    /* [in] */ IKeyEvent* origEvent,
    /* [in] */ Int32 action)
{
    if (origEvent == NULL) {
        return E_INVALID_ARGUMENT;
    }

    CKeyEvent* event = (CKeyEvent*)origEvent;
    mDownTime = event->mDownTime;
    mEventTime = event->mEventTime;
    mAction = action;
    mKeyCode = event->mKeyCode;
    mRepeatCount = event->mRepeatCount;
    mMetaState = event->mMetaState;
    mDeviceId = event->mDeviceId;
    mSource = event->mSource;
    mScanCode = event->mScanCode;
    mFlags = event->mFlags;
    // Don't copy mCharacters, since one way or the other we'll lose it
    // when changing the action.

    return NOERROR;
}

ECode CKeyEvent::constructor(
    /* [in] */ IParcel* in)
{
    return ReadFromParcel(in);
}

AutoPtr<CKeyEvent> CKeyEvent::Obtain()
{
    AutoPtr<CKeyEvent> ev;
    {
        Mutex::Autolock lock(gRecyclerLock);

        ev = gRecyclerTop;
        if (ev == NULL) {
            CKeyEvent::NewByFriend((CKeyEvent**)&ev);
            return ev;
        }
        gRecyclerTop = ev->mNext;
        gRecyclerUsed -= 1;
    }
    ev->mNext = NULL;
    ev->PrepareForReuse();

    return ev;
}

AutoPtr<CKeyEvent> CKeyEvent::Obtain(
    /* [in] */ Int64 downTime,
    /* [in] */ Int64 eventTime,
    /* [in] */ Int32 action,
    /* [in] */ Int32 code,
    /* [in] */ Int32 repeat,
    /* [in] */ Int32 metaState,
    /* [in] */ Int32 deviceId,
    /* [in] */ Int32 scancode,
    /* [in] */ Int32 flags,
    /* [in] */ Int32 source,
    /* [in] */ const String& characters)
{
    AutoPtr<CKeyEvent> ev = Obtain();
    ev->mDownTime = downTime;
    ev->mEventTime = eventTime;
    ev->mAction = action;
    ev->mKeyCode = code;
    ev->mRepeatCount = repeat;
    ev->mMetaState = metaState;
    ev->mDeviceId = deviceId;
    ev->mScanCode = scancode;
    ev->mFlags = flags;
    ev->mSource = source;
    ev->mCharacters = characters;
    return ev;
}

AutoPtr<CKeyEvent> CKeyEvent::Obtain(
    /* [in] */ IKeyEvent* otherEvent)
{
    CKeyEvent* other = (CKeyEvent*)otherEvent;
    AutoPtr<CKeyEvent> ev = Obtain();
    ev->mDownTime = other->mDownTime;
    ev->mEventTime = other->mEventTime;
    ev->mAction = other->mAction;
    ev->mKeyCode = other->mKeyCode;
    ev->mRepeatCount = other->mRepeatCount;
    ev->mMetaState = other->mMetaState;
    ev->mDeviceId = other->mDeviceId;
    ev->mScanCode = other->mScanCode;
    ev->mFlags = other->mFlags;
    ev->mSource = other->mSource;
    ev->mCharacters = other->mCharacters;
    return ev;
}

ECode CKeyEvent::Copy(
    /* [out] */ IInputEvent** event)
{
    VALIDATE_NOT_NULL(event);
    AutoPtr<IInputEvent> temp = (IInputEvent*)Obtain(this);
    *event = temp;
    REFCOUNT_ADD(*event);
    return NOERROR;
}

ECode CKeyEvent::Recycle()
{
    InputEvent::Recycle();
    mCharacters = NULL;

    Mutex::Autolock lock(gRecyclerLock);

    if (gRecyclerUsed < MAX_RECYCLED) {
        gRecyclerUsed++;
        mNext = gRecyclerTop;
        gRecyclerTop = this;
    }

    return NOERROR;
}

ECode CKeyEvent::RecycleIfNeededAfterDispatch()
{
    // Do nothing.
    return NOERROR;
}

ECode CKeyEvent::ChangeTimeRepeat(
    /* [in] */ IKeyEvent* event,
    /* [in] */ Int64 eventTime,
    /* [in] */ Int32 newRepeat,
    /* [out] */ IKeyEvent** newEvent)
{
    VALIDATE_NOT_NULL(newEvent);
    return CKeyEvent::New(event, eventTime, newRepeat, newEvent);
}

ECode CKeyEvent::ChangeTimeRepeat(
    /* [in] */ IKeyEvent* event,
    /* [in] */ Int64 eventTime,
    /* [in] */ Int32 newRepeat,
    /* [in] */ Int32 newFlags,
    /* [out] */ IKeyEvent** newEvent)
{
    VALIDATE_NOT_NULL(newEvent);
    ECode ec = CKeyEvent::New(event, eventTime, newRepeat, newEvent);
    if (FAILED(ec)) {
        return ec;
    }

    ((CKeyEvent*)*newEvent)->mEventTime = eventTime;
    ((CKeyEvent*)*newEvent)->mRepeatCount = newRepeat;
    ((CKeyEvent*)*newEvent)->mFlags = newFlags;

    return NOERROR;
}

ECode CKeyEvent::ChangeAction(
    /* [in] */ IKeyEvent* event,
    /* [in] */ Int32 action,
    /* [out] */ IKeyEvent** newEvent)
{
    VALIDATE_NOT_NULL(newEvent);
    return CKeyEvent::New(event, action, newEvent);
}

ECode CKeyEvent::ChangeFlags(
    /* [in] */ IKeyEvent* event,
    /* [in] */ Int32 flags,
    /* [out] */ IKeyEvent** newEvent)
{
    VALIDATE_NOT_NULL(newEvent);
    ECode ec = CKeyEvent::New(event, newEvent);
    if (FAILED(ec)) {
        return ec;
    }

    ((CKeyEvent*)*newEvent)->mFlags = flags;

    return NOERROR;
}

ECode CKeyEvent::IsTainted(
    /* [out] */ Boolean* isTainted)
{
    VALIDATE_NOT_NULL(isTainted);
    *isTainted = (mFlags & FLAG_TAINTED) != 0;

    return NOERROR;
}

ECode CKeyEvent::SetTainted(
    /* [in] */ Boolean tainted)
{
    mFlags = tainted ? mFlags | FLAG_TAINTED : mFlags & ~FLAG_TAINTED;

    return NOERROR;
}

ECode CKeyEvent::IsDown(
    /* [out] */ Boolean* isDown)
{
    VALIDATE_NOT_NULL(isDown);

    *isDown = (mAction == ACTION_DOWN);

    return NOERROR;
}

ECode CKeyEvent::IsSystem(
    /* [out] */ Boolean* isSystem)
{
    VALIDATE_NOT_NULL(isSystem);

    *isSystem = android::KeyEvent::isSystemKey(mKeyCode);

    return NOERROR;
}

ECode CKeyEvent::HasDefaultAction(
    /* [out] */ Boolean* hasDefaultAction)
{
    VALIDATE_NOT_NULL(hasDefaultAction);

    *hasDefaultAction = android::KeyEvent::hasDefaultAction(mKeyCode);

    return NOERROR;
}

Boolean CKeyEvent::IsGamepadButton(
    /* [in] */ Int32 keyCode)
{
    switch (keyCode) {
        case IKeyEvent::KEYCODE_BUTTON_A:
        case IKeyEvent::KEYCODE_BUTTON_B:
        case IKeyEvent::KEYCODE_BUTTON_C:
        case IKeyEvent::KEYCODE_BUTTON_X:
        case IKeyEvent::KEYCODE_BUTTON_Y:
        case IKeyEvent::KEYCODE_BUTTON_Z:
        case IKeyEvent::KEYCODE_BUTTON_L1:
        case IKeyEvent::KEYCODE_BUTTON_R1:
        case IKeyEvent::KEYCODE_BUTTON_L2:
        case IKeyEvent::KEYCODE_BUTTON_R2:
        case IKeyEvent::KEYCODE_BUTTON_THUMBL:
        case IKeyEvent::KEYCODE_BUTTON_THUMBR:
        case IKeyEvent::KEYCODE_BUTTON_START:
        case IKeyEvent::KEYCODE_BUTTON_SELECT:
        case IKeyEvent::KEYCODE_BUTTON_MODE:
        case IKeyEvent::KEYCODE_BUTTON_1:
        case IKeyEvent::KEYCODE_BUTTON_2:
        case IKeyEvent::KEYCODE_BUTTON_3:
        case IKeyEvent::KEYCODE_BUTTON_4:
        case IKeyEvent::KEYCODE_BUTTON_5:
        case IKeyEvent::KEYCODE_BUTTON_6:
        case IKeyEvent::KEYCODE_BUTTON_7:
        case IKeyEvent::KEYCODE_BUTTON_8:
        case IKeyEvent::KEYCODE_BUTTON_9:
        case IKeyEvent::KEYCODE_BUTTON_10:
        case IKeyEvent::KEYCODE_BUTTON_11:
        case IKeyEvent::KEYCODE_BUTTON_12:
        case IKeyEvent::KEYCODE_BUTTON_13:
        case IKeyEvent::KEYCODE_BUTTON_14:
        case IKeyEvent::KEYCODE_BUTTON_15:
        case IKeyEvent::KEYCODE_BUTTON_16:
            return TRUE;
        default:
            return FALSE;
    }
}

ECode CKeyEvent::GetDeviceId(
    /* [out] */ Int32* deviceId)
{
    VALIDATE_NOT_NULL(deviceId);
    *deviceId = mDeviceId;

    return NOERROR;
}

ECode CKeyEvent::GetDevice(
    /* [out] */ IInputDevice** device)
{
    VALIDATE_NOT_NULL(device)
    return InputEvent::GetDevice(device);
}

ECode CKeyEvent::GetSource(
    /* [out] */ Int32* source)
{
    VALIDATE_NOT_NULL(source)
    *source = mSource;

    return NOERROR;
}

ECode CKeyEvent::SetSource(
    /* [in] */ Int32 source)
{
    mSource = source;
    return NOERROR;
}

ECode CKeyEvent::GetMetaState(
    /* [out] */ Int32* metaState)
{
    VALIDATE_NOT_NULL(metaState);

    *metaState = mMetaState;

    return NOERROR;
}

ECode CKeyEvent::GetModifiers(
    /* [out] */ Int32* modifiers)
{
    VALIDATE_NOT_NULL(modifiers);
    *modifiers = NormalizeMetaState(mMetaState) & META_MODIFIER_MASK;

    return NOERROR;
}

ECode CKeyEvent::GetFlags(
    /* [out] */ Int32* flags)
{
    VALIDATE_NOT_NULL(flags);

    *flags = mFlags;

    return NOERROR;
}

Int32 CKeyEvent::GetModifierMetaStateMask()
{
    return META_MODIFIER_MASK;
}

Boolean CKeyEvent::IsModifierKey(
    /* [in] */ Int32 keyCode)
{
    switch (keyCode) {
        case KEYCODE_SHIFT_LEFT:
        case KEYCODE_SHIFT_RIGHT:
        case KEYCODE_ALT_LEFT:
        case KEYCODE_ALT_RIGHT:
        case KEYCODE_CTRL_LEFT:
        case KEYCODE_CTRL_RIGHT:
        case KEYCODE_META_LEFT:
        case KEYCODE_META_RIGHT:
        case KEYCODE_SYM:
        case KEYCODE_NUM:
        case KEYCODE_FUNCTION:
            return TRUE;
        default:
            return FALSE;
    }
}

Int32 CKeyEvent::NormalizeMetaState(
    /* [in] */ Int32 metaState)
{
    if ((metaState & (META_SHIFT_LEFT_ON | META_SHIFT_RIGHT_ON)) != 0) {
        metaState |= META_SHIFT_ON;
    }
    if ((metaState & (META_ALT_LEFT_ON | META_ALT_RIGHT_ON)) != 0) {
        metaState |= META_ALT_ON;
    }
    if ((metaState & (META_CTRL_LEFT_ON | META_CTRL_RIGHT_ON)) != 0) {
        metaState |= META_CTRL_ON;
    }
    if ((metaState & (META_META_LEFT_ON | META_META_RIGHT_ON)) != 0) {
        metaState |= META_META_ON;
    }
    // if ((metaState & IMetaKeyKeyListener::META_CAP_LOCKED) != 0) {
    //     metaState |= META_CAPS_LOCK_ON;
    // }
    // if ((metaState & IMetaKeyKeyListener::META_ALT_LOCKED) != 0) {
    //     metaState |= META_ALT_ON;
    // }
    // if ((metaState & IMetaKeyKeyListener::META_SYM_LOCKED) != 0) {
    //     metaState |= META_SYM_ON;
    // }
    return metaState & META_ALL_MASK;
}

Boolean CKeyEvent::MetaStateHasNoModifiers(
    /* [in] */ Int32 metaState)
{
    return (NormalizeMetaState(metaState) & META_MODIFIER_MASK) == 0;
}

ECode CKeyEvent::MetaStateHasModifiers(
    /* [in] */ Int32 metaState,
    /* [in] */ Int32 modifiers,
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);
    // Note: For forward compatibility, we allow the parameter to contain meta states
    //       that we do not recognize but we explicitly disallow meta states that
    //       are not valid modifiers.
    if ((modifiers & META_INVALID_MODIFIER_MASK) != 0) {
        Logger::E(TAG, String("modifiers must not contain ")
            + "META_CAPS_LOCK_ON, META_NUM_LOCK_ON, META_SCROLL_LOCK_ON, "
            + "META_CAP_LOCKED, META_ALT_LOCKED, META_SYM_LOCKED, "
            + "or META_SELECTING");

        return E_ILLEGAL_ARGUMENT_EXCEPTION ;
    }

    metaState = NormalizeMetaState(metaState) & META_MODIFIER_MASK;
    FAIL_RETURN(MetaStateFilterDirectionalModifiers(metaState, modifiers,
        META_SHIFT_ON, META_SHIFT_LEFT_ON, META_SHIFT_RIGHT_ON, &metaState));
    FAIL_RETURN(MetaStateFilterDirectionalModifiers(metaState, modifiers,
        META_ALT_ON, META_ALT_LEFT_ON, META_ALT_RIGHT_ON, &metaState));
    FAIL_RETURN(MetaStateFilterDirectionalModifiers(metaState, modifiers,
        META_CTRL_ON, META_CTRL_LEFT_ON, META_CTRL_RIGHT_ON, &metaState));
    FAIL_RETURN(MetaStateFilterDirectionalModifiers(metaState, modifiers,
        META_META_ON, META_META_LEFT_ON, META_META_RIGHT_ON, &metaState));
    *res = metaState == modifiers;
    return NOERROR;
}

ECode CKeyEvent::MetaStateFilterDirectionalModifiers(
    /* [in] */ Int32 metaState,
    /* [in] */ Int32 modifiers,
    /* [in] */ Int32 basic,
    /* [in] */ Int32 left,
    /* [in] */ Int32 right,
    /* [out] */ Int32* ret)
{
    VALIDATE_NOT_NULL(ret);
    Boolean wantBasic = (modifiers & basic) != 0;
    Int32 directional = left | right;
    Boolean wantLeftOrRight = (modifiers & directional) != 0;

    if (wantBasic) {
        if (wantLeftOrRight) {
            Logger::E(TAG, String("modifiers must not contain ")
                + MetaStateToString(basic) + " combined with "
                + MetaStateToString(left) + " or " + MetaStateToString(right));
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
        }
        *ret = metaState & ~directional;
    }
    else if (wantLeftOrRight) {
        *ret = metaState & ~basic;
    }
    else {
        *ret = metaState;
    }

    return NOERROR;
}

ECode CKeyEvent::HasNoModifiers(
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);
    *res = MetaStateHasNoModifiers(mMetaState);

    return NOERROR;
}

ECode CKeyEvent::HasModifiers(
    /* [in] */ Int32 modifiers,
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);
    return MetaStateHasModifiers(mMetaState, modifiers, res);
}

ECode CKeyEvent::IsAltPressed(
    /* [out] */ Boolean* isAltPressed)
{
    VALIDATE_NOT_NULL(isAltPressed);
    *isAltPressed = (mMetaState & META_ALT_ON) != 0;

    return NOERROR;
}

ECode CKeyEvent::IsShiftPressed(
    /* [out] */ Boolean* isShiftPressed)
{
    VALIDATE_NOT_NULL(isShiftPressed);
    *isShiftPressed = (mMetaState & META_SHIFT_ON) != 0;

    return NOERROR;
}

ECode CKeyEvent::IsSymPressed(
    /* [out] */ Boolean* isSymPressed)
{
    VALIDATE_NOT_NULL(isSymPressed);
    *isSymPressed = (mMetaState & META_SYM_ON) != 0;

    return NOERROR;
}

ECode CKeyEvent::IsCtrlPressed(
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);
    *res = (mMetaState & META_CTRL_ON) != 0;

    return NOERROR;
}

ECode CKeyEvent::IsMetaPressed(
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);
    *res = (mMetaState & META_META_ON) != 0;

    return NOERROR;
}

ECode CKeyEvent::IsFunctionPressed(
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);
    *res = (mMetaState & META_FUNCTION_ON) != 0;

    return NOERROR;
}

ECode CKeyEvent::IsCapsLockOn(
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);
    *res = (mMetaState & META_CAPS_LOCK_ON) != 0;

    return NOERROR;
}

ECode CKeyEvent::IsNumLockOn(
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);
    *res = (mMetaState & META_NUM_LOCK_ON) != 0;

    return NOERROR;
}

ECode CKeyEvent::IsScrollLockOn(
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);
    *res = (mMetaState & META_SCROLL_LOCK_ON) != 0;

    return NOERROR;
}

ECode CKeyEvent::GetAction(
    /* [out] */ Int32* action)
{
    VALIDATE_NOT_NULL(action);
    *action = mAction;

    return NOERROR;
}

ECode CKeyEvent::IsCanceled(
    /* [out] */ Boolean* isCanceled)
{
    VALIDATE_NOT_NULL(isCanceled);
    *isCanceled = (mFlags&FLAG_CANCELED) != 0;

    return NOERROR;
}

ECode CKeyEvent::StartTracking()
{
    mFlags |= FLAG_START_TRACKING;

    return NOERROR;
}

ECode CKeyEvent::IsTracking(
    /* [out] */ Boolean* isTracking)
{
    VALIDATE_NOT_NULL(isTracking);
    *isTracking = (mFlags&FLAG_TRACKING) != 0;

    return NOERROR;
}

ECode CKeyEvent::IsLongPress(
    /* [out] */ Boolean* isLongPress)
{
    VALIDATE_NOT_NULL(isLongPress);
    *isLongPress = (mFlags&FLAG_LONG_PRESS) != 0;

    return NOERROR;
}

ECode CKeyEvent::GetKeyCode(
    /* [out] */ Int32* keyCode)
{
    VALIDATE_NOT_NULL(keyCode);
    *keyCode = mKeyCode;

    return NOERROR;
}

ECode CKeyEvent::GetCharacters(
    /* [out] */ String* characters)
{
    VALIDATE_NOT_NULL(characters);
    *characters = mCharacters;

    return NOERROR;
}

ECode CKeyEvent::GetScanCode(
    /* [out] */ Int32* scanCode)
{
    VALIDATE_NOT_NULL(scanCode);
    *scanCode = mScanCode;

    return NOERROR;
}

ECode CKeyEvent::GetRepeatCount(
    /* [out] */ Int32* repeatCount)
{
    VALIDATE_NOT_NULL(repeatCount);
    *repeatCount = mRepeatCount;

    return NOERROR;
}

ECode CKeyEvent::GetDownTime(
    /* [out] */ Int64* downTime)
{
    VALIDATE_NOT_NULL(downTime);
    *downTime = mDownTime;

    return NOERROR;
}

ECode CKeyEvent::GetEventTime(
    /* [out] */ Int64* eventTime)
{
    VALIDATE_NOT_NULL(eventTime);
    *eventTime = mEventTime;

    return NOERROR;
}

ECode CKeyEvent::GetEventTimeNano(
    /* [out] */ Int64* eventTimeNano)
{
    VALIDATE_NOT_NULL(eventTimeNano);
    *eventTimeNano = mEventTime * 1000000L;

    return NOERROR;
}

ECode CKeyEvent::GetSequenceNumber(
        /* [out] */ Int32* seq)
{
    VALIDATE_NOT_NULL(seq);
    return InputEvent::GetSequenceNumber(seq);
}

ECode CKeyEvent::GetKeyboardDevice(
    /* [out] */ Int32* deviceId)
{
    VALIDATE_NOT_NULL(deviceId);
    *deviceId = mDeviceId;

    return NOERROR;
}

ECode CKeyEvent::GetKeyCharacterMap(
    /* [out] */ IKeyCharacterMap** kcm)
{
    return CKeyCharacterMap::Load(mDeviceId, kcm);
}

ECode CKeyEvent::GetDisplayLabel(
    /* [out] */ Char32* displayLabel)
{
    VALIDATE_NOT_NULL(displayLabel);

    AutoPtr<IKeyCharacterMap> kcm;
    FAIL_RETURN(GetKeyCharacterMap((IKeyCharacterMap**)&kcm));

    return kcm->GetDisplayLabel(mKeyCode, displayLabel);
}

ECode CKeyEvent::GetUnicodeChar(
    /* [out] */ Int32* unicodeChar)
{
    return GetUnicodeChar(mMetaState, unicodeChar);
}

ECode CKeyEvent::GetUnicodeChar(
    /* [in] */ Int32 metaState,
    /* [out] */ Int32* unicodeChar)
{
    VALIDATE_NOT_NULL(unicodeChar);
    AutoPtr<IKeyCharacterMap> kcm;
    FAIL_RETURN(GetKeyCharacterMap((IKeyCharacterMap**)&kcm));

    return kcm->Get(mKeyCode, metaState, unicodeChar);
}

ECode CKeyEvent::GetKeyData(
    /* [in] */ IKeyData* keyData,
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);
    AutoPtr<IKeyCharacterMap> kcm;
    FAIL_RETURN(GetKeyCharacterMap((IKeyCharacterMap**)&kcm));

    return kcm->GetKeyData(mKeyCode, keyData, res);
}

ECode CKeyEvent::GetMatch(
    /* [in] */ const ArrayOf<Char32>& chars,
    /* [out] */ Char32* match)
{
    return GetMatch(chars, 0, match);
}

ECode CKeyEvent::GetMatch(
    /* [in] */ const ArrayOf<Char32>& chars,
    /* [in] */ Int32 modifiers,
    /* [out] */ Char32* match)
{
    VALIDATE_NOT_NULL(match);
    AutoPtr<IKeyCharacterMap> kcm;
    FAIL_RETURN(GetKeyCharacterMap((IKeyCharacterMap**)&kcm));

    return kcm->GetMatch(mKeyCode, chars, modifiers, match);
}

ECode CKeyEvent::GetNumber(
    /* [out] */ Char32* ch)
{
    VALIDATE_NOT_NULL(ch);
    AutoPtr<IKeyCharacterMap> kcm;
    FAIL_RETURN(GetKeyCharacterMap((IKeyCharacterMap**)&kcm));

    return kcm->GetNumber(mKeyCode, ch);
}

ECode CKeyEvent::IsPrintingKey(
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);
    AutoPtr<IKeyCharacterMap> kcm;
    FAIL_RETURN(GetKeyCharacterMap((IKeyCharacterMap**)&kcm));

    return kcm->IsPrintingKey(mKeyCode, res);
}

ECode CKeyEvent::Dispatch(
    /* [in] */ IKeyEventCallback* receiver,
    /* [out] */ Boolean* res)
{
    return Dispatch(receiver, NULL, NULL, res);
}

ECode CKeyEvent::Dispatch(
    /* [in] */ IKeyEventCallback* receiver,
    /* [in] */ IDispatcherState* state,
    /* [in] */ IInterface* target,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    if (DEBUG) {
        Logger::D(TAG, "Key down to %p in %p, mAction:%d", target, state, mAction);
    }

    Boolean res = FALSE;
    switch (mAction) {
    case ACTION_DOWN:
        {
            mFlags &= ~FLAG_START_TRACKING;

            receiver->OnKeyDown(mKeyCode, this, &res);
            if (state != NULL) {
                Boolean isLongPress;
                Boolean isTracking;

                if (res && mRepeatCount == 0
                    && (mFlags & FLAG_START_TRACKING) != 0) {
                    if (DEBUG)
                        Logger::D(TAG, "  Start tracking!");

                    state->StartTracking(this, target);
                }
                else if ((IsLongPress(&isLongPress), isLongPress)
                    && (state->IsTracking(this, &isTracking), isTracking)) {
                    receiver->OnKeyLongPress(mKeyCode, this, &res);
                    if (res) {
                        if (DEBUG) {
                            Logger::D(TAG, "  Clear from Int64 press!");
                        }

                        state->PerformedLongPress(this);
                    }
                }
            }
        }
        break;
    case ACTION_UP:
        {
            if (state != NULL) {
                state->HandleUpEvent(this);
            }

            FAIL_RETURN(receiver->OnKeyUp(mKeyCode, this, &res));
        }
        break;
    case ACTION_MULTIPLE:
        {
            Int32 count = mRepeatCount;
            Int32 code = mKeyCode;

            receiver->OnKeyMultiple(code, count, this, &res);
            if (res) {
                break;
            }

            if (code != KEYCODE_UNKNOWN) {
                mAction = ACTION_DOWN;
                mRepeatCount = 0;

                Boolean handled = FALSE;
                receiver->OnKeyDown(code, this, &handled);
                if (handled) {
                    mAction = ACTION_UP;
                    receiver->OnKeyUp(code, this, &res);
                }
                mAction = ACTION_MULTIPLE;
                mRepeatCount = count;
                res = handled;
            }
        }
        break;
    default:
        break;
    }

    *result = res;
    return NOERROR;
}

String CKeyEvent::ToString()
{
    AutoPtr<StringBuilder> msg = new StringBuilder();
    msg->AppendCStr("KeyEvent { action=");
    msg->AppendString(ActionToString(mAction));
    msg->AppendCStr(", keyCode=");
    msg->AppendString(KeyCodeToString(mKeyCode));
    msg->AppendCStr(", scanCode=");
    msg->AppendInt32(mScanCode);
    if (!mCharacters.IsNull()) {
        msg->AppendCStr(", characters=\"");
        msg->AppendString(mCharacters);
        msg->AppendCStr("\"");
    }
    msg->AppendCStr(", metaState=");
    msg->AppendString(MetaStateToString(mMetaState));
    msg->AppendCStr(", flags=0x");
    msg->AppendString(StringUtils::Int32ToString(mFlags, 16));
    msg->AppendCStr(", repeatCount=");
    msg->AppendInt32(mRepeatCount);
    msg->AppendCStr(", eventTime=");
    msg->AppendInt32(mEventTime);
    msg->AppendCStr(", downTime=");
    msg->AppendInt32(mDownTime);
    msg->AppendCStr(", deviceId=");
    msg->AppendInt32(mDeviceId);
    msg->AppendCStr(", source=0x");
    msg->AppendString(StringUtils::Int32ToString(mSource, 16));
    msg->AppendCStr(" }");
    return msg->ToString();
}

String CKeyEvent::ActionToString(
    /* [in] */ Int32 action)
{
    switch (action) {
        case ACTION_DOWN:
            return String("ACTION_DOWN");
        case ACTION_UP:
            return String("ACTION_UP");
        case ACTION_MULTIPLE:
            return String("ACTION_MULTIPLE");
        default:
            return StringUtils::Int32ToString(action);
    }
}

String CKeyEvent::KeyCodeToString(
    /* [in] */ Int32 keyCode)
{
    String symbolicName;
    HashMap<Int32, String>::Iterator find = KEYCODE_SYMBOLIC_NAMES.Find(keyCode);
    if (find != KEYCODE_SYMBOLIC_NAMES.End()) {
        symbolicName = find->mSecond;
    }

    return symbolicName.IsNull() ? StringUtils::Int32ToString(keyCode) : symbolicName;
}

ECode CKeyEvent::KeyCodeFromString(
    /* [in] */ const String& symbolicName,
    /* [out] */ Int32* keyCode)
{
    VALIDATE_NOT_NULL(keyCode);

    if (symbolicName.IsNull()) {
        Logger::E(TAG, "symbolicName must not be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    HashMap<Int32, String>::Iterator iter = KEYCODE_SYMBOLIC_NAMES.Begin();
    for (Int32 i = 0; iter != KEYCODE_SYMBOLIC_NAMES.End(); ++iter, i++) {
        if (symbolicName.Equals(iter->mSecond)) {
            *keyCode = i;
            return NOERROR;
        }
    }

    *keyCode = StringUtils::ParseInt32(symbolicName);

    return NOERROR;
}

String CKeyEvent::MetaStateToString(
    /* [in] */ Int32 metaState)
{
    if (metaState == 0) {
        return String("0");
    }
    AutoPtr<StringBuilder> result;
    Int32 i = 0;
    while (metaState != 0) {
        Boolean isSet = (metaState & 1) != 0;
        metaState = (metaState >> 1) & 0x7FFFFFFF; // unsigned shift!
        if (isSet) {
            String name = META_SYMBOLIC_NAMES[i];
            if (result == NULL) {
                if (metaState == 0) {
                    return name;
                }
                result = new StringBuilder(name);
            }
            else {
                result->AppendChar('|');
                result->AppendString(name);
            }
        }
        i += 1;
    }
    return result->ToString();
}

ECode CKeyEvent::CreateFromParcelBody(
    /* [in] */ IParcel* in,
    /* [out] */ IKeyEvent** newEvent)
{
    return CKeyEvent::New(in, newEvent);
}

ECode CKeyEvent::ReadFromParcel(
    /* [in] */ IParcel *source)
{
    VALIDATE_NOT_NULL(source);

    Int32 token;
    FAIL_RETURN(source->ReadInt32(&token));

    FAIL_RETURN(source->ReadInt32(&mDeviceId));
    FAIL_RETURN(source->ReadInt32(&mSource));
    FAIL_RETURN(source->ReadInt32(&mAction));
    FAIL_RETURN(source->ReadInt32(&mKeyCode));
    FAIL_RETURN(source->ReadInt32(&mRepeatCount));
    FAIL_RETURN(source->ReadInt32(&mMetaState));
    FAIL_RETURN(source->ReadInt32(&mScanCode));
    FAIL_RETURN(source->ReadInt32(&mFlags));
    FAIL_RETURN(source->ReadInt64(&mDownTime));
    FAIL_RETURN(source->ReadInt64(&mEventTime));

    return NOERROR;
}

ECode CKeyEvent::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    VALIDATE_NOT_NULL(dest);

    FAIL_RETURN(dest->WriteInt32(PARCEL_TOKEN_KEY_EVENT));

    FAIL_RETURN(dest->WriteInt32(mDeviceId));
    FAIL_RETURN(dest->WriteInt32(mSource));
    FAIL_RETURN(dest->WriteInt32(mAction));
    FAIL_RETURN(dest->WriteInt32(mKeyCode));
    FAIL_RETURN(dest->WriteInt32(mRepeatCount));
    FAIL_RETURN(dest->WriteInt32(mMetaState));
    FAIL_RETURN(dest->WriteInt32(mScanCode));
    FAIL_RETURN(dest->WriteInt32(mFlags));
    FAIL_RETURN(dest->WriteInt64(mDownTime));
    FAIL_RETURN(dest->WriteInt64(mEventTime));

    return NOERROR;
}

}   //namespace View
}   //namespace Droid
}   //namespace Elastos
