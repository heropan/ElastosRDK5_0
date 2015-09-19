
#ifndef __ELASTOS_DROID_VIEW_CKEYEVENTHELPER_H__
#define __ELASTOS_DROID_VIEW_CKEYEVENTHELPER_H__

#include "_Elastos_Droid_View_CKeyEventHelper.h"

namespace Elastos {
namespace Droid {
namespace View {

CarClass(CKeyEventHelper)
{
public:
    /**
     * Returns the maximum keycode.
     */
    CARAPI GetMaxKeyCode(
        /* [out] */ Int32* maxKeyCode);

    /**
     * Get the character that is produced by putting accent on the character
     * c.
     * For example, getDeadChar('`', 'e') returns &egrave;.
     */
    CARAPI GetDeadChar(
        /* [in] */ Int32 accent,
        /* [in] */ Int32 c,
        /* [out] */ Int32* deadChar);

    /**
     * Obtains a (potentially recycled) key event.
     *
     * @hide
     */
    CARAPI Obtain(
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
        /* [in] */ const String& characters,
        /* [out] */ IKeyEvent** keyEvent);

    /**
     * Obtains a (potentially recycled) copy of another key event.
     *
     * @hide
     */
    CARAPI Obtain(
        /* [in] */ IKeyEvent* otherEvent,
        /* [out] */ IKeyEvent** keyEvent);

    /**
     * Create a new key event that is the same as the given one, but whose
     * event time and repeat count are replaced with the given value.
     *
     * @param event The existing event to be copied.  This is not modified.
     * @param eventTime The new event time
     * (in {@link android.os.SystemClock#uptimeMillis}) of the event.
     * @param newRepeat The new repeat count of the event.
     */
    CARAPI ChangeTimeRepeat(
        /* [in] */ IKeyEvent* event,
        /* [in] */ Int64 eventTime,
        /* [in] */ Int32 newRepeat,
        /* [out] */ IKeyEvent** newEvent);

    /**
     * Create a new key event that is the same as the given one, but whose
     * event time and repeat count are replaced with the given value.
     *
     * @param event The existing event to be copied.  This is not modified.
     * @param eventTime The new event time
     * (in {@link android.os.SystemClock#uptimeMillis}) of the event.
     * @param newRepeat The new repeat count of the event.
     * @param newFlags New flags for the event, replacing the entire value
     * in the original event.
     */
    CARAPI ChangeTimeRepeat(
        /* [in] */ IKeyEvent* event,
        /* [in] */ Int64 eventTime,
        /* [in] */ Int32 newRepeat,
        /* [in] */ Int32 newFlags,
        /* [out] */ IKeyEvent** newEvent);

    /**
     * Create a new key event that is the same as the given one, but whose
     * action is replaced with the given value.
     *
     * @param event The existing event to be copied.  This is not modified.
     * @param action The new action code of the event.
     */
    CARAPI ChangeAction(
        /* [in] */ IKeyEvent* event,
        /* [in] */ Int32 action,
        /* [out] */ IKeyEvent** newEvent);

    /**
     * Create a new key event that is the same as the given one, but whose
     * flags are replaced with the given value.
     *
     * @param event The existing event to be copied.  This is not modified.
     * @param flags The new flags constant.
     */
    CARAPI ChangeFlags(
        /* [in] */ IKeyEvent* event,
        /* [in] */ Int32 flags,
        /* [out] */ IKeyEvent** newEvent);

    /**
     * Returns true if the specified keycode is a gamepad button.
     * @return True if the keycode is a gamepad button, such as {@link #KEYCODE_BUTTON_A}.
     */
    CARAPI IsGamepadButton(
        /* [in] */ Int32 keyCode,
        /* [out] */ Boolean* res);

    /**
     * Gets a mask that includes all valid modifier key meta state bits.
     * <p>
     * For the purposes of this function, {@link #KEYCODE_CAPS_LOCK},
     * {@link #KEYCODE_SCROLL_LOCK}, and {@link #KEYCODE_NUM_LOCK} are
     * not considered modifier keys.  Consequently, the mask specifically excludes
     * {@link #META_CAPS_LOCK_ON}, {@link #META_SCROLL_LOCK_ON} and {@link #META_NUM_LOCK_ON}.
     * </p>
     *
     * @return The modifier meta state mask which is a combination of
     * {@link #META_SHIFT_ON}, {@link #META_SHIFT_LEFT_ON}, {@link #META_SHIFT_RIGHT_ON},
     * {@link #META_ALT_ON}, {@link #META_ALT_LEFT_ON}, {@link #META_ALT_RIGHT_ON},
     * {@link #META_CTRL_ON}, {@link #META_CTRL_LEFT_ON}, {@link #META_CTRL_RIGHT_ON},
     * {@link #META_META_ON}, {@link #META_META_LEFT_ON}, {@link #META_META_RIGHT_ON},
     * {@link #META_SYM_ON}, {@link #META_FUNCTION_ON}.
     */
    CARAPI GetModifierMetaStateMask(
        /* [out] */ Int32* mask);

    /**
     * Returns true if this key code is a modifier key.
     * <p>
     * For the purposes of this function, {@link #KEYCODE_CAPS_LOCK},
     * {@link #KEYCODE_SCROLL_LOCK}, and {@link #KEYCODE_NUM_LOCK} are
     * not considered modifier keys.  Consequently, this function return false
     * for those keys.
     * </p>
     *
     * @return True if the key code is one of
     * {@link #KEYCODE_SHIFT_LEFT} {@link #KEYCODE_SHIFT_RIGHT},
     * {@link #KEYCODE_ALT_LEFT}, {@link #KEYCODE_ALT_RIGHT},
     * {@link #KEYCODE_CTRL_LEFT}, {@link #KEYCODE_CTRL_RIGHT},
     * {@link #KEYCODE_META_LEFT}, or {@link #KEYCODE_META_RIGHT},
     * {@link #KEYCODE_SYM}, {@link #KEYCODE_NUM}, {@link #KEYCODE_FUNCTION}.
     */
    CARAPI IsModifierKey(
        /* [in] */ Int32 keyCode,
        /* [out] */ Boolean* res);

    /**
     * Normalizes the specified meta state.
     * <p>
     * The meta state is normalized such that if either the left or right modifier meta state
     * bits are set then the result will also include the universal bit for that modifier.
     * </p><p>
     * If the specified meta state contains {@link #META_ALT_LEFT_ON} then
     * the result will also contain {@link #META_ALT_ON} in addition to {@link #META_ALT_LEFT_ON}
     * and the other bits that were specified in the input.  The same is process is
     * performed for shift, control and meta.
     * </p><p>
     * If the specified meta state contains synthetic meta states defined by
     * {@link MetaKeyKeyListener}, then those states are translated here and the original
     * synthetic meta states are removed from the result.
     * {@link MetaKeyKeyListener#META_CAP_LOCKED} is translated to {@link #META_CAPS_LOCK_ON}.
     * {@link MetaKeyKeyListener#META_ALT_LOCKED} is translated to {@link #META_ALT_ON}.
     * {@link MetaKeyKeyListener#META_SYM_LOCKED} is translated to {@link #META_SYM_ON}.
     * </p><p>
     * Undefined meta state bits are removed.
     * </p>
     *
     * @param metaState The meta state.
     * @return The normalized meta state.
     */
    CARAPI NormalizeMetaState(
        /* [in] */ Int32 metaState,
        /* [out] */ Int32* normalizeMetaState);

    /**
     * Returns true if no modifiers keys are pressed according to the specified meta state.
     * <p>
     * For the purposes of this function, {@link #KEYCODE_CAPS_LOCK},
     * {@link #KEYCODE_SCROLL_LOCK}, and {@link #KEYCODE_NUM_LOCK} are
     * not considered modifier keys.  Consequently, this function ignores
     * {@link #META_CAPS_LOCK_ON}, {@link #META_SCROLL_LOCK_ON} and {@link #META_NUM_LOCK_ON}.
     * </p><p>
     * The meta state is normalized prior to comparison using {@link #normalizeMetaState(int)}.
     * </p>
     *
     * @param metaState The meta state to consider.
     * @return True if no modifier keys are pressed.
     * @see #hasNoModifiers()
     */
    CARAPI MetaStateHasNoModifiers(
        /* [in] */ Int32 metaState,
        /* [out] */ Boolean* res);

    /**
     * Returns true if only the specified modifier keys are pressed according to
     * the specified meta state.  Returns false if a different combination of modifier
     * keys are pressed.
     * <p>
     * For the purposes of this function, {@link #KEYCODE_CAPS_LOCK},
     * {@link #KEYCODE_SCROLL_LOCK}, and {@link #KEYCODE_NUM_LOCK} are
     * not considered modifier keys.  Consequently, this function ignores
     * {@link #META_CAPS_LOCK_ON}, {@link #META_SCROLL_LOCK_ON} and {@link #META_NUM_LOCK_ON}.
     * </p><p>
     * If the specified modifier mask includes directional modifiers, such as
     * {@link #META_SHIFT_LEFT_ON}, then this method ensures that the
     * modifier is pressed on that side.
     * If the specified modifier mask includes non-directional modifiers, such as
     * {@link #META_SHIFT_ON}, then this method ensures that the modifier
     * is pressed on either side.
     * If the specified modifier mask includes both directional and non-directional modifiers
     * for the same type of key, such as {@link #META_SHIFT_ON} and {@link #META_SHIFT_LEFT_ON},
     * then this method throws an illegal argument exception.
     * </p>
     *
     * @param metaState The meta state to consider.
     * @param modifiers The meta state of the modifier keys to check.  May be a combination
     * of modifier meta states as defined by {@link #getModifierMetaStateMask()}.  May be 0 to
     * ensure that no modifier keys are pressed.
     * @return True if only the specified modifier keys are pressed.
     * @throws IllegalArgumentException if the modifiers parameter contains invalid modifiers
     * @see #hasModifiers
     */
    CARAPI MetaStateHasModifiers(
        /* [in] */ Int32 metaState,
        /* [in] */ Int32 modifiers,
        /* [out] */ Boolean* res);

    /**
     * Returns a string that represents the symbolic name of the specified action
     * such as "ACTION_DOWN", or an equivalent numeric constant such as "35" if unknown.
     *
     * @param action The action.
     * @return The symbolic name of the specified action.
     * @hide
     */
    CARAPI ActionToString(
        /* [in] */ Int32 action,
        /* [out] */ String* str);

    /**
     * Returns a string that represents the symbolic name of the specified keycode
     * such as "KEYCODE_A", "KEYCODE_DPAD_UP", or an equivalent numeric constant
     * such as "1001" if unknown.
     *
     * @param keyCode The key code.
     * @return The symbolic name of the specified keycode.
     *
     * @see KeyCharacterMap#getDisplayLabel
     */
    CARAPI KeyCodeToString(
        /* [in] */ Int32 keyCode,
        /* [out] */ String* str);

    /**
     * Gets a keycode by its symbolic name such as "KEYCODE_A" or an equivalent
     * numeric constant such as "1001".
     *
     * @param symbolicName The symbolic name of the keycode.
     * @return The keycode or {@link #KEYCODE_UNKNOWN} if not found.
     * @see #keycodeToString
     */
    CARAPI KeyCodeFromString(
        /* [in] */ const String& symbolicName,
        /* [out] */ Int32* keyCode);

    /**
     * Returns a string that represents the symbolic name of the specified combined meta
     * key modifier state flags such as "0", "META_SHIFT_ON",
     * "META_ALT_ON|META_SHIFT_ON" or an equivalent numeric constant such as "0x10000000"
     * if unknown.
     *
     * @param metaState The meta state.
     * @return The symbolic name of the specified combined meta state flags.
     * @hide
     */
    CARAPI MetaStateToString(
        /* [in] */ Int32 metaState,
        /* [out] */ String* str);

    /** @hide */
    CARAPI CreateFromParcelBody(
        /* [in] */ IParcel* in,
        /* [out] */ IKeyEvent** newEvent);
};

}   //namespace View
}   //namespace Droid
}   //namespace Elastos
#endif //__ELASTOS_DROID_VIEW_CKEYEVENTHELPER_H__
