
#ifndef __CKEYCHARACTERMAP_HELPERH__
#define __CKEYCHARACTERMAP_HELPERH__

#include "_CKeyCharacterMapHelper.h"

namespace Elastos {
namespace Droid {
namespace View {

CarClass(CKeyCharacterMapHelper)
{
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
    CARAPI Load(
        /* [in] */ Int32 deviceId,
        /* [out] */ IKeyCharacterMap** kcm);

    /**
     * Queries the framework about whether any physical keys exist on the
     * any keyboard attached to the device that are capable of producing the given key code.
     *
     * @param keyCode The key code to query.
     * @return True if at least one attached keyboard supports the specified key code.
     */
    CARAPI DeviceHasKey(
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
    CARAPI GetDeadChar(
        /* [in] */ Int32 accent,
        /* [in] */ Int32 c,
        /* [out] */ Int32* deadChar);

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
    CARAPI DeviceHasKeys(
        /* [in] */ const ArrayOf<Int32>& keyCodes,
        /* [in] */ ArrayOf<Boolean>** hasKeys);
};

}   //namespace View
}   //namespace Droid
}   //namespace Elastos

#endif //__CKEYCHARACTERMAPHELPER_H__
