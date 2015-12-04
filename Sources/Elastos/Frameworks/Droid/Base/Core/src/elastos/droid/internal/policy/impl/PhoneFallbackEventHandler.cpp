
#include "elastos/droid/internal/policy/impl/PhoneFallbackEventHandler.h"

using Elastos::Droid::View::EIID_IFallbackEventHandler;

namespace Elastos {
namespace Droid {
namespace Internal {
namespace Policy {
namespace Impl {

//=====================================================================
//                      PhoneFallbackEventHandler
//=====================================================================
CAR_INTERFACE_IMPL_2(PhoneFallbackEventHandler, Object, IPhoneFallbackEventHandler, IFallbackEventHandler)
String PhoneFallbackEventHandler::TAG("PhoneFallbackEventHandler");
const Boolean PhoneFallbackEventHandler::DEBUG = FALSE;

PhoneFallbackEventHandler::PhoneFallbackEventHandler()
{
}

CARAPI PhoneFallbackEventHandler::constructor(
    /* [in] */ IContext* context)
{
    // ==================before translated======================
    // mContext = context;
    return NOERROR;
}

ECode PhoneFallbackEventHandler::SetView(
    /* [in] */ IView* v)
{
    VALIDATE_NOT_NULL(v);
    // ==================before translated======================
    // mView = v;
    assert(0);
    return NOERROR;
}

ECode PhoneFallbackEventHandler::PreDispatchKeyEvent(
    /* [in] */ IKeyEvent* event)
{
    VALIDATE_NOT_NULL(event);
    // ==================before translated======================
    // getAudioManager().preDispatchKeyEvent(event, AudioManager.USE_DEFAULT_STREAM_TYPE);
    assert(0);
    return NOERROR;
}

ECode PhoneFallbackEventHandler::DispatchKeyEvent(
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(event);
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    //
    // final int action = event.getAction();
    // final int keyCode = event.getKeyCode();
    //
    // if (action == KeyEvent.ACTION_DOWN) {
    //     return onKeyDown(keyCode, event);
    // } else {
    //     return onKeyUp(keyCode, event);
    // }
    assert(0);
    return NOERROR;
}

ECode PhoneFallbackEventHandler::OnKeyDown(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(event);
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // /* ****************************************************************************
    //  * HOW TO DECIDE WHERE YOUR KEY HANDLING GOES.
    //  * See the comment in PhoneWindow.onKeyDown
    //  * ****************************************************************************/
    // final KeyEvent.DispatcherState dispatcher = mView.getKeyDispatcherState();
    //
    // switch (keyCode) {
    //     case KeyEvent.KEYCODE_VOLUME_UP:
    //     case KeyEvent.KEYCODE_VOLUME_DOWN:
    //     case KeyEvent.KEYCODE_VOLUME_MUTE: {
    //         MediaSessionLegacyHelper.getHelper(mContext).sendVolumeKeyEvent(event, false);
    //         return true;
    //     }
    //
    //
    //     case KeyEvent.KEYCODE_MEDIA_PLAY:
    //     case KeyEvent.KEYCODE_MEDIA_PAUSE:
    //     case KeyEvent.KEYCODE_MEDIA_PLAY_PAUSE:
    //         /* Suppress PLAY/PAUSE toggle when phone is ringing or in-call
    //          * to avoid music playback */
    //         if (getTelephonyManager().getCallState() != TelephonyManager.CALL_STATE_IDLE) {
    //             return true;  // suppress key event
    //         }
    //     case KeyEvent.KEYCODE_MUTE:
    //     case KeyEvent.KEYCODE_HEADSETHOOK:
    //     case KeyEvent.KEYCODE_MEDIA_STOP:
    //     case KeyEvent.KEYCODE_MEDIA_NEXT:
    //     case KeyEvent.KEYCODE_MEDIA_PREVIOUS:
    //     case KeyEvent.KEYCODE_MEDIA_REWIND:
    //     case KeyEvent.KEYCODE_MEDIA_RECORD:
    //     case KeyEvent.KEYCODE_MEDIA_FAST_FORWARD:
    //     case KeyEvent.KEYCODE_MEDIA_AUDIO_TRACK: {
    //         handleMediaKeyEvent(event);
    //         return true;
    //     }
    //
    //     case KeyEvent.KEYCODE_CALL: {
    //         if (getKeyguardManager().inKeyguardRestrictedInputMode() || dispatcher == null) {
    //             break;
    //         }
    //         if (event.getRepeatCount() == 0) {
    //             dispatcher.startTracking(event, this);
    //         } else if (event.isLongPress() && dispatcher.isTracking(event)) {
    //             dispatcher.performedLongPress(event);
    //             mView.performHapticFeedback(HapticFeedbackConstants.LONG_PRESS);
    //             // launch the VoiceDialer
    //             Intent intent = new Intent(Intent.ACTION_VOICE_COMMAND);
    //             intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
    //             try {
    //                 sendCloseSystemWindows();
    //                 mContext.startActivity(intent);
    //             } catch (ActivityNotFoundException e) {
    //                 startCallActivity();
    //             }
    //         }
    //         return true;
    //     }
    //
    //     case KeyEvent.KEYCODE_CAMERA: {
    //         if (getKeyguardManager().inKeyguardRestrictedInputMode() || dispatcher == null) {
    //             break;
    //         }
    //         if (event.getRepeatCount() == 0) {
    //             dispatcher.startTracking(event, this);
    //         } else if (event.isLongPress() && dispatcher.isTracking(event)) {
    //             dispatcher.performedLongPress(event);
    //             mView.performHapticFeedback(HapticFeedbackConstants.LONG_PRESS);
    //             sendCloseSystemWindows();
    //             // Broadcast an intent that the Camera button was longpressed
    //             Intent intent = new Intent(Intent.ACTION_CAMERA_BUTTON, null);
    //             intent.putExtra(Intent.EXTRA_KEY_EVENT, event);
    //             mContext.sendOrderedBroadcastAsUser(intent, UserHandle.CURRENT_OR_SELF,
    //                     null, null, null, 0, null, null);
    //         }
    //         return true;
    //     }
    //
    //     case KeyEvent.KEYCODE_SEARCH: {
    //         if (getKeyguardManager().inKeyguardRestrictedInputMode() || dispatcher == null) {
    //             break;
    //         }
    //         if (event.getRepeatCount() == 0) {
    //             dispatcher.startTracking(event, this);
    //         } else if (event.isLongPress() && dispatcher.isTracking(event)) {
    //             Configuration config = mContext.getResources().getConfiguration();
    //             if (config.keyboard == Configuration.KEYBOARD_NOKEYS
    //                     || config.hardKeyboardHidden == Configuration.HARDKEYBOARDHIDDEN_YES) {
    //                 // launch the search activity
    //                 Intent intent = new Intent(Intent.ACTION_SEARCH_LONG_PRESS);
    //                 intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
    //                 try {
    //                     mView.performHapticFeedback(HapticFeedbackConstants.LONG_PRESS);
    //                     sendCloseSystemWindows();
    //                     getSearchManager().stopSearch();
    //                     mContext.startActivity(intent);
    //                     // Only clear this if we successfully start the
    //                     // activity; otherwise we will allow the normal short
    //                     // press action to be performed.
    //                     dispatcher.performedLongPress(event);
    //                     return true;
    //                 } catch (ActivityNotFoundException e) {
    //                     // Ignore
    //                 }
    //             }
    //         }
    //         break;
    //     }
    // }
    // return false;
    assert(0);
    return NOERROR;
}

ECode PhoneFallbackEventHandler::OnKeyUp(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(event);
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // if (DEBUG) {
    //     Slog.d(TAG, "up " + keyCode);
    // }
    // final KeyEvent.DispatcherState dispatcher = mView.getKeyDispatcherState();
    // if (dispatcher != null) {
    //     dispatcher.handleUpEvent(event);
    // }
    //
    // switch (keyCode) {
    //     case KeyEvent.KEYCODE_VOLUME_UP:
    //     case KeyEvent.KEYCODE_VOLUME_DOWN:
    //     case KeyEvent.KEYCODE_VOLUME_MUTE: {
    //         if (!event.isCanceled()) {
    //             MediaSessionLegacyHelper.getHelper(mContext).sendVolumeKeyEvent(event, false);
    //         }
    //         return true;
    //     }
    //
    //     case KeyEvent.KEYCODE_HEADSETHOOK:
    //     case KeyEvent.KEYCODE_MUTE:
    //     case KeyEvent.KEYCODE_MEDIA_PLAY:
    //     case KeyEvent.KEYCODE_MEDIA_PAUSE:
    //     case KeyEvent.KEYCODE_MEDIA_PLAY_PAUSE:
    //     case KeyEvent.KEYCODE_MEDIA_STOP:
    //     case KeyEvent.KEYCODE_MEDIA_NEXT:
    //     case KeyEvent.KEYCODE_MEDIA_PREVIOUS:
    //     case KeyEvent.KEYCODE_MEDIA_REWIND:
    //     case KeyEvent.KEYCODE_MEDIA_RECORD:
    //     case KeyEvent.KEYCODE_MEDIA_FAST_FORWARD:
    //     case KeyEvent.KEYCODE_MEDIA_AUDIO_TRACK: {
    //         handleMediaKeyEvent(event);
    //         return true;
    //     }
    //
    //     case KeyEvent.KEYCODE_CAMERA: {
    //         if (getKeyguardManager().inKeyguardRestrictedInputMode()) {
    //             break;
    //         }
    //         if (event.isTracking() && !event.isCanceled()) {
    //             // Add short press behavior here if desired
    //         }
    //         return true;
    //     }
    //
    //     case KeyEvent.KEYCODE_CALL: {
    //         if (getKeyguardManager().inKeyguardRestrictedInputMode()) {
    //             break;
    //         }
    //         if (event.isTracking() && !event.isCanceled()) {
    //             startCallActivity();
    //         }
    //         return true;
    //     }
    // }
    // return false;
    assert(0);
    return NOERROR;
}

ECode PhoneFallbackEventHandler::StartCallActivity()
{
    // ==================before translated======================
    // sendCloseSystemWindows();
    // Intent intent = new Intent(Intent.ACTION_CALL_BUTTON);
    // intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
    // try {
    //     mContext.startActivity(intent);
    // } catch (ActivityNotFoundException e) {
    //     Slog.w(TAG, "No activity found for android.intent.action.CALL_BUTTON.");
    // }
    assert(0);
    return NOERROR;
}

ECode PhoneFallbackEventHandler::GetSearchManager(
    /* [out] */ ISearchManager** result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // if (mSearchManager == null) {
    //     mSearchManager = (SearchManager) mContext.getSystemService(Context.SEARCH_SERVICE);
    // }
    // return mSearchManager;
    assert(0);
    return NOERROR;
}

ECode PhoneFallbackEventHandler::GetTelephonyManager(
    /* [out] */ ITelephonyManager** result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // if (mTelephonyManager == null) {
    //     mTelephonyManager = (TelephonyManager)mContext.getSystemService(
    //             Context.TELEPHONY_SERVICE);
    // }
    // return mTelephonyManager;
    assert(0);
    return NOERROR;
}

ECode PhoneFallbackEventHandler::GetKeyguardManager(
    /* [out] */ IKeyguardManager** result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // if (mKeyguardManager == null) {
    //     mKeyguardManager = (KeyguardManager)mContext.getSystemService(Context.KEYGUARD_SERVICE);
    // }
    // return mKeyguardManager;
    assert(0);
    return NOERROR;
}

ECode PhoneFallbackEventHandler::GetAudioManager(
    /* [out] */ IAudioManager** result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // if (mAudioManager == null) {
    //     mAudioManager = (AudioManager)mContext.getSystemService(Context.AUDIO_SERVICE);
    // }
    // return mAudioManager;
    assert(0);
    return NOERROR;
}

ECode PhoneFallbackEventHandler::SendCloseSystemWindows()
{
    // ==================before translated======================
    // PhoneWindowManager.sendCloseSystemWindows(mContext, null);
    assert(0);
    return NOERROR;
}

void PhoneFallbackEventHandler::HandleMediaKeyEvent(
    /* [in] */ IKeyEvent* keyEvent)
{
    // ==================before translated======================
    // MediaSessionLegacyHelper.getHelper(mContext).sendMediaButtonEvent(keyEvent, false);
    assert(0);
}

} // namespace Impl
} // namespace Policy
} // namespace Internal
} // namespace Droid
} // namespace Elastos


