
#ifndef __CKEYCHARACTERMAP_H__
#define __CKEYCHARACTERMAP_H__

#include "_CKeyCharacterMap.h"
#include "ext/frameworkext.h"
#include <elastos/HashMap.h>
#include <elastos/Mutex.h>
#include <androidfw/KeyCharacterMap.h>

using Elastos::Utility::HashMap;
using Elastos::Core::IStringBuilder;
using Elastos::Core::Threading::Mutex;

namespace Elastos {
namespace Droid {
namespace View {

/**
 * Describes the keys provided by a keyboard device and their associated labels.
 */
CarClass(CKeyCharacterMap)
{
private:
    static const char* TAG;
    /* Characters used to display placeholders for dead keys. */
    static const Int32 ACCENT_ACUTE = 0x00B4;
    static const Int32 ACCENT_BREVE = 0x02D8;
    static const Int32 ACCENT_CARON = 0x02C7;
    static const Int32 ACCENT_CEDILLA = 0x00B8;
    static const Int32 ACCENT_CIRCUMFLEX = 0x02C6;
    static const Int32 ACCENT_COMMA_ABOVE = 0x1FBD;
    static const Int32 ACCENT_COMMA_ABOVE_RIGHT = 0x02BC;
    static const Int32 ACCENT_DOT_ABOVE = 0x02D9;
    static const Int32 ACCENT_DOT_BELOW = '.'; // approximate
    static const Int32 ACCENT_DOUBLE_ACUTE = 0x02DD;
    static const Int32 ACCENT_GRAVE = 0x02CB;
    static const Int32 ACCENT_HOOK_ABOVE = 0x02C0;
    static const Int32 ACCENT_HORN = '\''; // approximate
    static const Int32 ACCENT_MACRON = 0x00AF;
    static const Int32 ACCENT_MACRON_BELOW = 0x02CD;
    static const Int32 ACCENT_OGONEK = 0x02DB;
    static const Int32 ACCENT_REVERSED_COMMA_ABOVE = 0x02BD;
    static const Int32 ACCENT_RING_ABOVE = 0x02DA;
    static const Int32 ACCENT_STROKE = '-'; // approximate
    static const Int32 ACCENT_TILDE = 0x02DC;
    static const Int32 ACCENT_TURNED_COMMA_ABOVE = 0x02BB;
    static const Int32 ACCENT_UMLAUT = 0x00A8;
    static const Int32 ACCENT_VERTICAL_LINE_ABOVE = 0x02C8;
    static const Int32 ACCENT_VERTICAL_LINE_BELOW = 0x02CC;

    /* Legacy dead key display characters used in previous versions of the API.
     * We still support these characters by mapping them to their non-legacy version. */
    static const Int32 ACCENT_GRAVE_LEGACY = '`';
    static const Int32 ACCENT_CIRCUMFLEX_LEGACY = '^';
    static const Int32 ACCENT_TILDE_LEGACY = '~';

    /**
     * Maps Unicode combining diacritical to display-form dead key.
     */
    static HashMap<Int32, Int32> sCombiningToAccent;
    static HashMap<Int32, Int32> sAccentToCombining;

    /**
     * Maps combinations of (display-form) combining key and second character
     * to combined output character.
     * These mappings are derived from the Unicode NFC tables as needed.
     */
    static HashMap<Int32, Int32> sDeadKeyCache;
    static Mutex sDeadKeyCacheLock;
    static const AutoPtr<IStringBuilder> sDeadKeyBuilder;

    static const Boolean mIsStaticInited;

    static Boolean InitStatic();

    static void AddCombining(
        /* [in] */Int32 combining,
        /* [in] */Int32 accent);

    static void AddDeadKey(
        /* [in] */Int32 accent,
        /* [in] */Int32 c,
        /* [in] */Int32 result);

public:
    /**
     * Specifies a substitute key code and meta state as a fallback action
     * for an unhandled key.
     * @hide
     */
    class FallbackAction : public ElRefBase, public IFallbackAction
    {
    private:
        FallbackAction();

    public:
        CAR_INTERFACE_DECL();

        static AutoPtr<FallbackAction> Obtain();

        CARAPI Recycle();

    private:
        static Int32 MAX_RECYCLED;
        static Mutex sRecycleLock;
        static AutoPtr<FallbackAction> sRecycleBin;
        static Int32 sRecycledCount;

        AutoPtr<FallbackAction> mNext;

    public:
        Int32 mKeyCode;
        Int32 mMetaState;
    };

    /**
     * Describes the character mappings associated with a key.
     *
     * @deprecated instead use {@link KeyCharacterMap#getDisplayLabel(Int32)},
     * {@link KeyCharacterMap#getNumber(Int32)} and {@link KeyCharacterMap#get(Int32, Int32)}.
     */
    //@Deprecated
    class KeyData : public ElRefBase, public IKeyData
    {
    public:
        CAR_INTERFACE_DECL();

        KeyData();

    public:
        /**
         * The display label (see {@link #getDisplayLabel}).
         */
        Char32 mDisplayLabel;
        /**
         * The "number" value (see {@link #getNumber}).
         */
        Char32 mNumber;
        /**
         * The character that will be generated in various meta states
         * (the same ones used for {@link #get} and defined as
         * {@link KeyEvent#META_SHIFT_ON} and {@link KeyEvent#META_ALT_ON}).
         *      <table>
         *          <tr><th>Index</th><th align="left">Value</th></tr>
         *          <tr><td>0</td><td>no modifiers</td></tr>
         *          <tr><td>1</td><td>caps</td></tr>
         *          <tr><td>2</td><td>alt</td></tr>
         *          <tr><td>3</td><td>caps + alt</td></tr>
         *      </table>
         */
        AutoPtr<ArrayOf<Char32> > mMeta;
    };

private:
    class NativeKeyCharacterMap
    {
    public:
        NativeKeyCharacterMap(int32_t deviceId, const android::sp<android::KeyCharacterMap>& map) :
            mDeviceId(deviceId), mMap(map)
        {
        }

        ~NativeKeyCharacterMap() {
        }

        inline int32_t getDeviceId() const {
            return mDeviceId;
        }

        inline const android::sp<android::KeyCharacterMap>& getMap() const {
            return mMap;
        }

    private:
        int32_t mDeviceId;
        android::sp<android::KeyCharacterMap> mMap;
    };

public:
    /**
     * Loads the key character maps for the keyboard with the specified device id.
     *
     * @param deviceId The device id of the keyboard.
     * @return The associated key character map.
     * @throws {@link UnavailableException} if the key character map
     * could not be loaded because it was malformed or the default key character map
     * is missing from the system.
     */
    static CARAPI Load(
        /* [in] */ Int32 deviceId,
        /* [out] */ IKeyCharacterMap** kcm);

    /**
     * Queries the framework about whether any physical keys exist on the
     * any keyboard attached to the device that are capable of producing the given key code.
     *
     * @param keyCode The key code to query.
     * @return True if at least one attached keyboard supports the specified key code.
     */
    static CARAPI DeviceHasKey(
        /* [in] */ Int32 keyCode,
        /* [out] */ Boolean* hasKey);

    /**
     * Get the character that is produced by putting accent on the character c.
     * For example, getDeadChar('`', 'e') returns &egrave;.
     *
     * @param accent The accent character.  eg. '`'
     * @param c The basic character.
     * @return The combined character, or 0 if the characters cannot be combined.
     */
    static CARAPI_(Int32) GetDeadChar(
        /* [in] */ Int32 accent,
        /* [in] */ Int32 c);

    /**
     * Queries the framework about whether any physical keys exist on the
     * any keyboard attached to the device that are capable of producing the given
     * array of key codes.
     *
     * @param keyCodes The array of key codes to query.
     * @return A new array of the same size as the key codes array whose elements
     * are set to true if at least one attached keyboard supports the corresponding key code
     * at the same index in the key codes array.
     */
    static CARAPI DeviceHasKeys(
        /* [in] */ const ArrayOf<Int32>& keyCodes,
        /* [out, callee]] */ ArrayOf<Boolean>** hasKeys);

public:
    CKeyCharacterMap();

    ~CKeyCharacterMap();

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IParcel* in);

    // Called from Native
    CARAPI constructor(
        /* [in] */ Int32 deviceId,
        /* [in] */ Int32 map);

    /**
     * Gets the Unicode character generated by the specified key and meta
     * key state combination.
     * <p>
     * Returns the Unicode character that the specified key would produce
     * when the specified meta bits (see {@link MetaKeyKeyListener})
     * were active.
     * </p><p>
     * Returns 0 if the key is not one that is used to type Unicode
     * characters.
     * </p><p>
     * If the return value has bit {@link #COMBINING_ACCENT} set, the
     * key is a "dead key" that should be combined with another to
     * actually produce a character -- see {@link #getDeadChar} --
     * after masking with {@link #COMBINING_ACCENT_MASK}.
     * </p>
     *
     * @param keyCode The key code.
     * @param metaState The meta key modifier state.
     * @return The associated character or combining accent, or 0 if none.
     */
    CARAPI Get(
        /* [in] */ Int32 keyCode,
        /* [in] */ Int32 metaState,
        /* [out] */ Int32* value);

    /**
     * Gets the fallback action to perform if the application does not
     * handle the specified key.
     * <p>
     * When an application does not handle a particular key, the system may
     * translate the key to an alternate fallback key (specified in the
     * fallback action) and dispatch it to the application.
     * The event containing the fallback key is flagged
     * with {@link KeyEvent#FLAG_FALLBACK}.
     * </p>
     *
     * @param keyCode The key code.
     * @param metaState The meta key modifier state.
     * @return The fallback action, or NULL if none.  Remember to recycle the fallback action.
     *
     * @hide
     */
    CARAPI GetFallbackAction(
        /* [in] */ Int32 keyCode,
        /* [in] */ Int32 metaState,
        /* [out] */ IFallbackAction** fallbackAction);

    /**
     * Gets the number or symbol associated with the key.
     * <p>
     * The character value is returned, not the numeric value.
     * If the key is not a number, but is a symbol, the symbol is retuned.
     * </p><p>
     * This method is intended to to support dial pads and other numeric or
     * symbolic entry on keyboards where certain keys serve dual function
     * as alphabetic and symbolic keys.  This method returns the number
     * or symbol associated with the key independent of whether the user
     * has pressed the required modifier.
     * </p><p>
     * For example, on one particular keyboard the keys on the top QWERTY row generate
     * numbers when ALT is pressed such that ALT-Q maps to '1'.  So for that keyboard
     * when {@link #getNumber} is called with {@link KeyEvent#KEYCODE_Q} it returns '1'
     * so that the user can type numbers without pressing ALT when it makes sense.
     * </p>
     *
     * @param keyCode The key code.
     * @return The associated numeric or symbolic character, or 0 if none.
     */
    CARAPI GetNumber(
        /* [in] */ Int32 keyCode,
        /* [out] */ Char32* number);

    /**
     * Gets the first character in the character array that can be generated
     * by the specified key code.
     * <p>
     * This is a convenience function that returns the same value as
     * {@link #getMatch(Int32,Char32[],Int32) getMatch(keyCode, chars, 0)}.
     * </p>
     *
     * @param keyCode The keycode.
     * @param chars The array of matching characters to consider.
     * @return The matching associated character, or 0 if none.
     */
    CARAPI GetMatch(
        /* [in] */ Int32 keyCode,
        /* [in] */ const ArrayOf<Char32>& chars,
        /* [out] */ Char32* match);

    /**
     * Gets the first character in the character array that can be generated
     * by the specified key code.  If there are multiple choices, prefers
     * the one that would be generated with the specified meta key modifier state.
     *
     * @param keyCode The key code.
     * @param chars The array of matching characters to consider.
     * @param metaState The preferred meta key modifier state.
     * @return The matching associated character, or 0 if none.
     */
    CARAPI GetMatchEx(
        /* [in] */ Int32 keyCode,
        /* [in] */ const ArrayOf<Char32>& chars,
        /* [in] */ Int32 metaState,
        /* [out] */ Char32* match);

    /**
     * Gets the primary character for this key.
     * In other words, the label that is physically printed on it.
     *
     * @param keyCode The key code.
     * @return The display label character, or 0 if none (eg. for non-printing keys).
     */
    CARAPI GetDisplayLabel(
        /* [in] */ Int32 keyCode,
        /* [out] */ Char32* label);

    /**
     * Get the character conversion data for a given key code.
     *
     * @param keyCode The keyCode to query.
     * @param results A {@link KeyData} instance that will be filled with the results.
     * @return True if the key was mapped.  If the key was not mapped, results is not modified.
     *
     * @deprecated instead use {@link KeyCharacterMap#getDisplayLabel(Int32)},
     * {@link KeyCharacterMap#getNumber(Int32)} or {@link KeyCharacterMap#get(Int32, Int32)}.
     */
    //@Deprecated
    CARAPI GetKeyData(
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyData* results,
        /* [out] */ Boolean* res);

    /**
     * Get an array of KeyEvent objects that if put into the input stream
     * could plausibly generate the provided sequence of characters.  It is
     * not guaranteed that the sequence is the only way to generate these
     * events or that it is optimal.
     * <p>
     * This function is primarily offered for instrumentation and testing purposes.
     * It may fail to map characters to key codes.  In particular, the key character
     * map for the {@link #BUILT_IN_KEYBOARD built-in keyboard} device id may be empty.
     * Consider using the key character map associated with the
     * {@link #VIRTUAL_KEYBOARD virtual keyboard} device id instead.
     * </p><p>
     * For robust text entry, do not use this function.  Instead construct a
     * {@link KeyEvent} with action code {@link KeyEvent#ACTION_MULTIPLE} that contains
     * the desired string using {@link KeyEvent#KeyEvent(long, String, Int32, Int32)}.
     * </p>
     *
     * @param chars The sequence of characters to generate.
     * @return An array of {@link KeyEvent} objects, or NULL if the given Char32 array
     *         can not be generated using the current key character map.
     */
    CARAPI GetEvents(
        /* [in] */ ArrayOf<Char32>* chars,
        /* [out, callee] */ ArrayOf<IKeyEvent*>** keyEvents);

    /**
     * Returns true if the specified key produces a glyph.
     *
     * @param keyCode The key code.
     * @return True if the key is a printing key.
     */
    CARAPI IsPrintingKey(
        /* [in] */ Int32 keyCode,
        /* [out] */ Boolean* res);

    /**
     * Gets the keyboard type.
     * Returns {@link #NUMERIC}, {@link #PREDICTIVE}, {@link #ALPHA}, {@link #FULL}
     * or {@link #SPECIAL_FUNCTION}.
     * <p>
     * Different keyboard types have different semantics.  Refer to the documentation
     * associated with the keyboard type constants for details.
     * </p>
     *
     * @return The keyboard type.
     */
    CARAPI GetKeyboardType(
        /* [out] */ Int32* type);

    /**
     * Gets a constant that describes the behavior of this keyboard's modifier keys
     * such as {@link KeyEvent#KEYCODE_SHIFT_LEFT}.
     * <p>
     * Currently there are two behaviors that may be combined:
     * </p>
     * <ul>
     * <li>Chorded behavior: When the modifier key is pressed together with one or more
     * character keys, the keyboard inserts the modified keys and
     * then resets the modifier state when the modifier key is released.</li>
     * <li>Toggled behavior: When the modifier key is pressed and released on its own
     * it first toggles into a latched state.  When latched, the modifier will apply
     * to next character key that is pressed and will then reset itself to the initial state.
     * If the modifier is already latched and the modifier key is pressed and release on
     * its own again, then it toggles into a locked state.  When locked, the modifier will
     * apply to all subsequent character keys that are pressed until unlocked by pressing
     * the modifier key on its own one more time to reset it to the initial state.
     * Toggled behavior is useful for small profile keyboards designed for thumb typing.
     * </ul>
     * <p>
     * This function currently returns {@link #MODIFIER_BEHAVIOR_CHORDED} when the
     * {@link #getKeyboardType() keyboard type} is {@link #FULL} or {@link #SPECIAL_FUNCTION} and
     * {@link #MODIFIER_BEHAVIOR_CHORDED_OR_TOGGLED} otherwise.
     * In the future, the function may also take into account global keyboard
     * accessibility settings, other user preferences, or new device capabilities.
     * </p>
     *
     * @return The modifier behavior for this keyboard.
     *
     * @see {@link #MODIFIER_BEHAVIOR_CHORDED}
     * @see {@link #MODIFIER_BEHAVIOR_CHORDED_OR_TOGGLED}
     */
    CARAPI GetModifierBehavior(
        /* [out] */ Int32* behavior);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* in);

    //@Override
    CARAPI WriteToParcel(
        /* [in] */ IParcel* out);

    CARAPI GetMap(
        /* [in] */ Int32* map);

private:
    CARAPI_(void) NativeReadFromParcel(
        /* [in] */ IParcel* in);

    CARAPI_(void) NativeWriteToParcel(
        /* [in] */ IParcel* out);

    CARAPI_(Char32) NativeGetCharacter(
        /* [in] */ Int32 keyCode,
        /* [in] */ Int32 metaState);

    CARAPI_(Boolean) NativeGetFallbackAction(
        /* [in] */ Int32 keyCode,
        /* [in] */ Int32 metaState,
        /* [in] */ FallbackAction* outFallbackAction);

    CARAPI_(Char32) NativeGetNumber(
        /* [in] */ Int32 keyCode);

    CARAPI_(Char32) NativeGetMatch(
        /* [in] */ Int32 keyCode,
        /* [in] */ const ArrayOf<Char32>& chars,
        /* [in] */ Int32 metaState);

    CARAPI_(Char32) NativeGetDisplayLabel(
        /* [in] */ Int32 keyCode);

    CARAPI_(Int32) NativeGetKeyboardType();

    CARAPI_(AutoPtr<ArrayOf<IKeyEvent*> >) NativeGetEvents(
        /* [in] */ const ArrayOf<Char32>& chars);

private:
    NativeKeyCharacterMap* mMap;
};

}   //namespace View
}   //namespace Droid
}   //namespace Elastos

#endif //__CKEYCHARACTERMAP_H__
