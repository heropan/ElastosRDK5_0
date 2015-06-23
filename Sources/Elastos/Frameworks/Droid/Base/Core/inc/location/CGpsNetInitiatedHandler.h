
#ifndef __GPSNETINITIATEDHANDLER_H__
#define __GPSNETINITIATEDHANDLER_H__
#include "_CGpsNetInitiatedHandler.h"
#include "ext/frameworkext.h"

using Elastos::Core::Mutex;
using Elastos::Droid::Os::IBundle;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::IIntent;
using Elastos::Droid::App::INotification;

namespace Elastos {
namespace Droid {
namespace Location {

/**
 * A GPS Network-initiated Handler class used by LocationManager.
 *
 * {@hide}
 */
CarClass(CGpsNetInitiatedHandler)
{
public:
    CGpsNetInitiatedHandler();

    CGpsNetInitiatedHandler(
        /* [in] */ IContext* context);

    // Handles NI events from HAL
    CARAPI HandleNiNotification(
        /* [in] */ IGpsNiNotification* notif);

    CARAPI constructor(
        /* [in] */ IContext* context);

    // Converts a string (or Hex string) to a char array
    static CARAPI_(AutoPtr<ArrayOf<Byte> >) StringToByteArray(
        /* [in] */ const String& original,
        /* [in] */ Boolean isHex);

    /**
     * Unpacks an byte array containing 7-bit packed characters into a String.
     *
     * @param input a 7-bit packed char array
     * @return the unpacked String
     */
    static CARAPI_(String) DecodeGSMPackedString(
        /* [in] */ ArrayOf<Byte>* input);

    static CARAPI_(String) DecodeUTF8String(
        /* [in] */ ArrayOf<Byte>* input);

    static CARAPI_(String) DecodeUCS2String(
        /* [in] */ ArrayOf<Byte>* input);

    // change this to configure dialog display (for verification)
    static CARAPI_(String) GetDialogTitle(
        /* [in] */ IGpsNiNotification* notif,
        /* [in] */ IContext* context);

private:

    // Sets the NI notification.
    /*synchronized*/
    CARAPI_(void) SetNiNotification(
        /* [in] */ IGpsNiNotification* notif);

    // Opens the notification dialog and waits for user input
    CARAPI_(void) OpenNiDialog(
        /* [in] */ IGpsNiNotification* notif);

    // Construct the intent for bringing up the dialog activity, which shows the
    // notification and takes user input
    CARAPI_(AutoPtr<IIntent>) GetDlgIntent(
        /* [in] */ IGpsNiNotification* notif);

    /** Decode NI string
     *
     * @param original   The text string to be decoded
     * @param isHex      Specifies whether the content of the string has been encoded as a Hex string. Encoding
     *                   a string as Hex can allow zeros inside the coded text.
     * @param coding     Specifies the coding scheme of the string, such as GSM, UTF8, UCS2, etc. This coding scheme
     *                      needs to match those used passed to HAL from the native GPS driver. Decoding is done according
     *                   to the <code> coding </code>, after a Hex string is decoded. Generally, if the
     *                   notification strings don't need further decoding, <code> coding </code> encoding can be
     *                   set to -1, and <code> isHex </code> can be false.
     * @return the decoded string
     */
    static CARAPI_(String) DecodeString(
        /* [in] */ const String& original,
        /* [in] */ Boolean isHex,
        /* [in] */ Int32 coding);

    // change this to configure notification display
    static CARAPI_(String) GetNotifTicker(
        /* [in] */ IGpsNiNotification* notif,
        /* [in] */ IContext* context);

    // change this to configure notification display
    static CARAPI_(String) GetNotifTitle(
        /* [in] */ IGpsNiNotification* notif,
        /* [in] */ IContext* context);

    // change this to configure notification display
    static CARAPI_(String) GetNotifMessage(
        /* [in] */ IGpsNiNotification* notif,
        /* [in] */ IContext* context);

    // change this to configure dialog display (for verification)
    static CARAPI_(String) GetDialogMessage(
        /* [in] */ IGpsNiNotification* notif,
        /* [in] */ IContext* context);

    static CARAPI_(String) GsmAlphabet_Gsm7BitPackedToString(
        /* [in] */ ArrayOf<Byte>* pdu,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 lengthSeptets,
        /* [in] */ Int32 numPaddingBits);

private:
    static const String TAG;// = "GpsNetInitiatedHandler";

    static const String sLanguage; // this static value defined in GsmAlphabet.java
    static const String sShift; // this static value defined in GsmAlphabet.java
    static const Byte GSM_EXTENDED_ESCAPE;

    static const Boolean DEBUG;// = TRUE;
    static const Boolean VERBOSE;// = FALSE;

    AutoPtr<IContext> mContext;

    // parent gps location provider
    AutoPtr<ILocationManager> mLocationManager;

    // configuration of notificaiton behavior
    Boolean mPlaySounds;// = false;
    Boolean mPopupImmediately;// = true;

    // Set to true if string from HAL is encoded as Hex, e.g., "3F0039"
    static Boolean mIsHexInput;// = TRUE;

    /**
     * The notification that is shown when a network-initiated notification
     * (and verification) event is received.
     * <p>
     * This is lazily created, so use {@link #setNINotification()}.
     */
    AutoPtr<INotification> mNiNotification;

    Mutex mLock;
};

} // namespace Location
} // namepsace Droid
} // namespace Elastos

#endif //__GPSNETINITIATEDHANDLER_H__
