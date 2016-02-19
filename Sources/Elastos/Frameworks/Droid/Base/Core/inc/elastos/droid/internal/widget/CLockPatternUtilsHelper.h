#ifndef __ELASTOS_DROID_INTERNAL_WIDGET_CLOCKPATTERNUTILSHELPER_H__
#define __ELASTOS_DROID_INTERNAL_WIDGET_CLOCKPATTERNUTILSHELPER_H__

#include "_Elastos_Droid_Internal_Widget_CLockPatternUtilsHelper.h"
#include <elastos/core/Singleton.h>

using Elastos::Utility::IList;

namespace Elastos {
namespace Droid {
namespace Internal {
namespace Widget {

CarClass(CLockPatternUtilsHelper)
    , public Singleton
    , public ILockPatternUtilsHelper
{
public:
    CAR_INTERFACE_DECL()

    CAR_SINGLETON_DECL()

    /*
     * Returns the maximum length of a sequential characters.  A sequence is defined as
     * monotonically increasing characters with a constant interval or the same character repeated.
     *
     * For example:
     * maxLengthSequence("1234") == 4
     * maxLengthSequence("1234abc") == 4
     * maxLengthSequence("aabc") == 3
     * maxLengthSequence("qwertyuio") == 1
     * maxLengthSequence("@ABC") == 3
     * maxLengthSequence(";;;;") == 4 (anything that repeats)
     * maxLengthSequence(":;<=>") == 1  (ordered, but not composed of alphas or digits)
     *
     * @param string the pass
     * @return the number of sequential letters or digits
     */
    CARAPI MaxLengthSequence(
            /* [in] */ const String& string,
        /* [out] */ Int32* len);

    /**
     * Gets whether the device is encrypted.
     *
     * @return Whether the device is encrypted.
     */
    CARAPI IsDeviceEncrypted(
        /* [out] */ Boolean* isDeviceEncrypted);

    /**
     * Determine if the device supports encryption, even if it's set to default. This
     * differs from isDeviceEncrypted() in that it returns true even if the device is
     * encrypted with the default password.
     * @return true if device encryption is enabled
     */
    CARAPI IsDeviceEncryptionEnabled(
        /* [out] */ Boolean* isDeviceEncryptionEnabled);

    /**
     * Deserialize a pattern.
     * @param string The pattern serialized with {@link #patternToString}
     * @return The pattern.
     */
    CARAPI StringToPattern(
        /* [in] */ const String& string,
        /* [out] */ IList** list);

    /**
     * Serialize a pattern.
     * @param pattern The pattern.
     * @return The pattern in string form.
     */
    CARAPI PatternToString(
        /* [in] */ IList* pattern,
        /* [out] */ String* str);

    /*
     * Generate an SHA-1 hash for the pattern. Not the most secure, but it is
     * at least a second level of protection. First level is that the file
     * is in a location only readable by the system process.
     * @param pattern the gesture pattern.
     * @return the hash of the pattern in a byte array.
     */
    CARAPI PatternToHash(
        /* [in] */ IList* pattern,
        /* [out, callee] */ ArrayOf<Byte>** arr);

    CARAPI IsSafeModeEnabled(
        /* [out] */ Boolean* result);
};

}// namespace Widget
}// namespace Internal
}// namespace Droid
}// namespace Elastos

#endif // __ELASTOS_DROID_INTERNAL_WIDGET_CLOCKPATTERNUTILSHELPER_H__
