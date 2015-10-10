
#ifndef __ELASTOS_DROID_MEDIA_CAUDIOMANAGER_H__
#define __ELASTOS_DROID_MEDIA_CAUDIOMANAGER_H__

#include "_Elastos_Droid_Media_CAudioManager.h"
#include "elastos/droid/ext/frameworkext.h"
#include "os/HandlerBase.h"
#include <elastos/utility/etl/HashMap.h>

using Elastos::Droid::Os::HandlerBase;
using Elastos::Droid::Os::IBinder;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::IComponentName;
using Elastos::Droid::View::IKeyEvent;
using Elastos::Droid::App::IPendingIntent;
using Elastos::Droid::Bluetooth::IBluetoothDevice;
using Elastos::Utility::Etl::HashMap;

namespace Elastos {
namespace Droid {
namespace Media {

/**
 * AudioManager provides access to volume and ringer mode control.
 * <p>
 * Use <code>Context.getSystemService(Context.AUDIO_SERVICE)</code> to get
 * an instance of this class.
 */
CarClass(CAudioManager)
{
friend class CAudioManagerAudioFocusDispatcher;

private:
    /**
     * Helper class to handle the forwarding of audio focus events to the appropriate listener
     */
    class FocusEventHandlerDelegate : public ElRefBase
    {
    private:
        class MyHandler : public HandlerBase
        {
        public:
            MyHandler(
                /* [in] */ ILooper* looper,
                /* [in] */ FocusEventHandlerDelegate* host)
                : HandlerBase(looper)
                , mHost(host)
            {}

            CARAPI HandleMessage(
                /* [in] */ IMessage* msg);
        private:
            FocusEventHandlerDelegate* mHost;
        };

    public:
        FocusEventHandlerDelegate(
            /* [in] */ CAudioManager* owner);

        CARAPI_(AutoPtr<IHandler>) GetHandler();

    private:
        AutoPtr<IHandler> mHandler;

        CAudioManager* mOwner;
    };

public:
    CAudioManager();

    CARAPI constructor(
        /* [in] */ IContext* context);

    /**
     * @hide
     */
    CARAPI PreDispatchKeyEvent(
        /* [in] */ IKeyEvent* event,
        /* [in] */ Int32 stream);

    /**
     * @hide
     */
    CARAPI HandleKeyDown(
        /* [in] */ IKeyEvent* event,
        /* [in] */ Int32 stream);

    /**
     * @hide
     */
    CARAPI HandleKeyUp(
        /* [in] */ IKeyEvent* event,
        /* [in] */ Int32 stream);

    /**
     * Adjusts the volume of a particular stream by one step in a direction.
     * <p>
     * This method should only be used by applications that replace the platform-wide
     * management of audio settings or the main telephony application.
     *
     * @param streamType The stream type to adjust. One of {@link #STREAM_VOICE_CALL},
     * {@link #STREAM_SYSTEM}, {@link #STREAM_RING}, {@link #STREAM_MUSIC} or
     * {@link #STREAM_ALARM}
     * @param direction The direction to adjust the volume. One of
     *            {@link #ADJUST_LOWER}, {@link #ADJUST_RAISE}, or
     *            {@link #ADJUST_SAME}.
     * @param flags One or more flags.
     * @see #adjustVolume(Int32, Int32)
     * @see #setStreamVolume(Int32, Int32, Int32)
     */
    CARAPI AdjustStreamVolume(
        /* [in] */ Int32 streamType,
        /* [in] */ Int32 direction,
        /* [in] */ Int32 flags);

    /**
     * Adjusts the volume of the most relevant stream. For example, if a call is
     * active, it will have the highest priority regardless of if the in-call
     * screen is showing. Another example, if music is playing in the background
     * and a call is not active, the music stream will be adjusted.
     * <p>
     * This method should only be used by applications that replace the platform-wide
     * management of audio settings or the main telephony application.
     *
     * @param direction The direction to adjust the volume. One of
     *            {@link #ADJUST_LOWER}, {@link #ADJUST_RAISE}, or
     *            {@link #ADJUST_SAME}.
     * @param flags One or more flags.
     * @see #adjustSuggestedStreamVolume(Int32, Int32, Int32)
     * @see #adjustStreamVolume(Int32, Int32, Int32)
     * @see #setStreamVolume(Int32, Int32, Int32)
     */
    CARAPI AdjustVolume(
        /* [in] */ Int32 direction,
        /* [in] */ Int32 flags);

    /**
     * Adjusts the volume of the most relevant stream, or the given fallback
     * stream.
     * <p>
     * This method should only be used by applications that replace the platform-wide
     * management of audio settings or the main telephony application.
     *
     * @param direction The direction to adjust the volume. One of
     *            {@link #ADJUST_LOWER}, {@link #ADJUST_RAISE}, or
     *            {@link #ADJUST_SAME}.
     * @param suggestedStreamType The stream type that will be used if there
     *            isn't a relevant stream. {@link #USE_DEFAULT_STREAM_TYPE} is valid here.
     * @param flags One or more flags.
     * @see #adjustVolume(Int32, Int32)
     * @see #adjustStreamVolume(Int32, Int32, Int32)
     * @see #setStreamVolume(Int32, Int32, Int32)
     */
    CARAPI AdjustSuggestedStreamVolume(
        /* [in] */ Int32 direction,
        /* [in] */ Int32 suggestedStreamType,
        /* [in] */ Int32 flags);

    /**
     * Adjusts the master volume for the device's audio amplifier.
     * <p>
     *
     * @param steps The number of volume steps to adjust. A positive
     *            value will raise the volume.
     * @param flags One or more flags.
     * @hide
     */
    CARAPI AdjustMasterVolume(
        /* [in] */ Int32 steps,
        /* [in] */ Int32 flags);

    /**
     * Returns the current ringtone mode.
     *
     * @return The current ringtone mode, one of {@link #RINGER_MODE_NORMAL},
     *         {@link #RINGER_MODE_SILENT}, or {@link #RINGER_MODE_VIBRATE}.
     * @see #setRingerMode(Int32)
     */
    CARAPI GetRingerMode(
        /* [out] */ Int32* result);

    /**
     * Returns the maximum volume index for a particular stream.
     *
     * @param streamType The stream type whose maximum volume index is returned.
     * @return The maximum valid volume index for the stream.
     * @see #getStreamVolume(Int32)
     */
    CARAPI GetStreamMaxVolume(
        /* [in] */ Int32 streamType,
        /* [out] */ Int32* result);

    /**
     * Returns the current volume index for a particular stream.
     *
     * @param streamType The stream type whose volume index is returned.
     * @return The current volume index for the stream.
     * @see #getStreamMaxVolume(Int32)
     * @see #setStreamVolume(Int32, Int32, Int32)
     */
    CARAPI GetStreamVolume(
        /* [in] */ Int32 streamType,
        /* [out] */ Int32* result);

    /**
     * Get last audible volume before stream was muted.
     *
     * @hide
     */
    CARAPI GetLastAudibleStreamVolume(
        /* [in] */ Int32 streamType,
        /* [out] */ Int32* result);

    /**
     * Get the stream type whose volume is driving the UI sounds volume.
     * UI sounds are screen lock/unlock, camera shutter, key clicks...
     * @hide
     */
    CARAPI GetMasterStreamType(
        /* [out] */ Int32* result);

    /**
     * Sets the ringer mode.
     * <p>
     * Silent mode will mute the volume and will not vibrate. Vibrate mode will
     * mute the volume and vibrate. Normal mode will be audible and may vibrate
     * according to user settings.
     *
     * @param ringerMode The ringer mode, one of {@link #RINGER_MODE_NORMAL},
     *            {@link #RINGER_MODE_SILENT}, or {@link #RINGER_MODE_VIBRATE}.
     * @see #getRingerMode()
     */
    CARAPI SetRingerMode(
        /* [in] */ Int32 ringerMode);

    /**
     * Sets the volume index for a particular stream.
     *
     * @param streamType The stream whose volume index should be set.
     * @param index The volume index to set. See
     *            {@link #getStreamMaxVolume(Int32)} for the largest valid value.
     * @param flags One or more flags.
     * @see #getStreamMaxVolume(Int32)
     * @see #getStreamVolume(Int32)
     */
    CARAPI SetStreamVolume(
        /* [in] */ Int32 streamType,
        /* [in] */ Int32 index,
        /* [in] */ Int32 flags);

    /**
     * Returns the maximum volume index for master volume.
     *
     * @hide
     */
    CARAPI GetMasterMaxVolume(
        /* [out] */ Int32* result);

    /**
     * Returns the current volume index for master volume.
     *
     * @return The current volume index for master volume.
     * @hide
     */
    CARAPI GetMasterVolume(
        /* [out] */ Int32* result);

    /**
     * Get last audible volume before master volume was muted.
     *
     * @hide
     */
    CARAPI GetLastAudibleMasterVolume(
        /* [out] */ Int32* result);

    /**
     * Sets the volume index for master volume.
     *
     * @param index The volume index to set. See
     *            {@link #getMasterMaxVolume(Int32)} for the largest valid value.
     * @param flags One or more flags.
     * @see #getMasterMaxVolume(Int32)
     * @see #getMasterVolume(Int32)
     * @hide
     */
    CARAPI SetMasterVolume(
        /* [in] */ Int32 index,
        /* [in] */ Int32 flags);

    /**
     * Solo or unsolo a particular stream. All other streams are muted.
     * <p>
     * The solo command is protected against client process death: if a process
     * with an active solo request on a stream dies, all streams that were muted
     * because of this request will be unmuted automatically.
     * <p>
     * The solo requests for a given stream are cumulative: the AudioManager
     * can receive several solo requests from one or more clients and the stream
     * will be unsoloed only when the same number of unsolo requests are received.
     * <p>
     * For a better user experience, applications MUST unsolo a soloed stream
     * in onPause() and solo is again in onResume() if appropriate.
     *
     * @param streamType The stream to be soloed/unsoloed.
     * @param state The required solo state: TRUE for solo ON, FALSE for solo OFF
     */
    CARAPI SetStreamSolo(
        /* [in] */ Int32 streamType,
        /* [in] */ Boolean state);

    /**
     * Mute or unmute an audio stream.
     * <p>
     * The mute command is protected against client process death: if a process
     * with an active mute request on a stream dies, this stream will be unmuted
     * automatically.
     * <p>
     * The mute requests for a given stream are cumulative: the AudioManager
     * can receive several mute requests from one or more clients and the stream
     * will be unmuted only when the same number of unmute requests are received.
     * <p>
     * For a better user experience, applications MUST unmute a muted stream
     * in onPause() and mute is again in onResume() if appropriate.
     * <p>
     * This method should only be used by applications that replace the platform-wide
     * management of audio settings or the main telephony application.
     *
     * @param streamType The stream to be muted/unmuted.
     * @param state The required mute state: TRUE for mute ON, FALSE for mute OFF
     */
    CARAPI SetStreamMute(
        /* [in] */ Int32 streamType,
        /* [in] */ Boolean state);

    /**
     * get stream mute state.
     *
     * @hide
     */
    CARAPI IsStreamMute(
        /* [in] */ Int32 streamType,
        /* [out] */ Boolean* result);

    /**
     * set master mute state.
     *
     * @hide
     */
    CARAPI SetMasterMute(
        /* [in] */ Boolean state);

    /**
     * set master mute state with optional flags.
     *
     * @hide
     */
    CARAPI SetMasterMute(
        /* [in] */ Boolean state,
        /* [in] */ Int32 flags);

    /**
     * get master mute state.
     *
     * @hide
     */
    CARAPI IsMasterMute(
        /* [out] */ Boolean* result);

    /**
     * forces the stream controlled by hard volume keys
     * specifying streamType == -1 releases control to the
     * logic.
     *
     * @hide
     */
    CARAPI ForceVolumeControlStream(
        /* [in] */ Int32 streamType);

    /**
     * Returns whether a particular type should vibrate according to user
     * settings and the current ringer mode.
     * <p>
     * This shouldn't be needed by most clients that use notifications to
     * vibrate. The notification manager will not vibrate if the policy doesn't
     * allow it, so the client should always set a vibrate pattern and let the
     * notification manager control whether or not to actually vibrate.
     *
     * @param vibrateType The type of vibrate. One of
     *            {@link #VIBRATE_TYPE_NOTIFICATION} or
     *            {@link #VIBRATE_TYPE_RINGER}.
     * @return Whether the type should vibrate at the instant this method is
     *         called.
     * @see #setVibrateSetting(Int32, Int32)
     * @see #getVibrateSetting(Int32)
     */
    CARAPI ShouldVibrate(
        /* [in] */ Int32 vibrateType,
        /* [out] */ Boolean* result);

    /**
     * Returns whether the user's vibrate setting for a vibrate type.
     * <p>
     * This shouldn't be needed by most clients that want to vibrate, instead
     * see {@link #shouldVibrate(Int32)}.
     *
     * @param vibrateType The type of vibrate. One of
     *            {@link #VIBRATE_TYPE_NOTIFICATION} or
     *            {@link #VIBRATE_TYPE_RINGER}.
     * @return The vibrate setting, one of {@link #VIBRATE_SETTING_ON},
     *         {@link #VIBRATE_SETTING_OFF}, or
     *         {@link #VIBRATE_SETTING_ONLY_SILENT}.
     * @see #setVibrateSetting(Int32, Int32)
     * @see #shouldVibrate(Int32)
     */
    CARAPI GetVibrateSetting(
        /* [in] */ Int32 vibrateType,
        /* [out] */ Int32* result);

    /**
     * Sets the setting for when the vibrate type should vibrate.
     * <p>
     * This method should only be used by applications that replace the platform-wide
     * management of audio settings or the main telephony application.
     *
     * @param vibrateType The type of vibrate. One of
     *            {@link #VIBRATE_TYPE_NOTIFICATION} or
     *            {@link #VIBRATE_TYPE_RINGER}.
     * @param vibrateSetting The vibrate setting, one of
     *            {@link #VIBRATE_SETTING_ON},
     *            {@link #VIBRATE_SETTING_OFF}, or
     *            {@link #VIBRATE_SETTING_ONLY_SILENT}.
     * @see #getVibrateSetting(Int32)
     * @see #shouldVibrate(Int32)
     */
    CARAPI SetVibrateSetting(
        /* [in] */ Int32 vibrateType,
        /* [in] */ Int32 vibrateSetting);

    /**
     * Sets the speakerphone on or off.
     * <p>
     * This method should only be used by applications that replace the platform-wide
     * management of audio settings or the main telephony application.
     *
     * @param on set <var>TRUE</var> to turn on speakerphone;
     *           <var>FALSE</var> to turn it off
     */
    CARAPI SetSpeakerphoneOn(
        /* [in] */ Boolean on);

    /**
     * Checks whether the speakerphone is on or off.
     *
     * @return TRUE if speakerphone is on, FALSE if it's off
     */
    CARAPI IsSpeakerphoneOn(
        /* [out] */ Boolean* result);

    //====================================================================
    // Bluetooth SCO control
    /**
     * Indicates if current platform supports use of SCO for off call use cases.
     * Application wanted to use bluetooth SCO audio when the phone is not in call
     * must first call thsi method to make sure that the platform supports this
     * feature.
     * @return TRUE if bluetooth SCO can be used for audio when not in call
     *         FALSE otherwise
     * @see #startBluetoothSco()
     */
    CARAPI IsBluetoothScoAvailableOffCall(
        /* [out] */ Boolean* result);

    /**
     * Start bluetooth SCO audio connection.
     * <p>Requires Permission:
     *   {@link android.Manifest.permission#MODIFY_AUDIO_SETTINGS}.
     * <p>This method can be used by applications wanting to send and received audio
     * to/from a bluetooth SCO headset while the phone is not in call.
     * <p>As the SCO connection establishment can take several seconds,
     * applications should not rely on the connection to be available when the method
     * returns but instead register to receive the intent {@link #ACTION_SCO_AUDIO_STATE_CHANGED}
     * and wait for the state to be {@link #SCO_AUDIO_STATE_CONNECTED}.
     * <p>As the connection is not guaranteed to succeed, applications must wait for this intent with
     * a timeout.
     * <p>When finished with the SCO connection or if the establishment times out,
     * the application must call {@link #stopBluetoothSco()} to clear the request and turn
     * down the bluetooth connection.
     * <p>Even if a SCO connection is established, the following restrictions apply on audio
     * output streams so that they can be routed to SCO headset:
     * - the stream type must be {@link #STREAM_VOICE_CALL}
     * - the format must be mono
     * - the sampling must be 16kHz or 8kHz
     * <p>The following restrictions apply on input streams:
     * - the format must be mono
     * - the sampling must be 8kHz
     *
     * <p>Note that the phone application always has the priority on the usage of the SCO
     * connection for telephony. If this method is called while the phone is in call
     * it will be ignored. Similarly, if a call is received or sent while an application
     * is using the SCO connection, the connection will be lost for the application and NOT
     * returned automatically when the call ends.
     * @see #stopBluetoothSco()
     * @see #ACTION_SCO_AUDIO_STATE_CHANGED
     */
    CARAPI StartBluetoothSco();

    /**
     * Stop bluetooth SCO audio connection.
     * <p>Requires Permission:
     *   {@link android.Manifest.permission#MODIFY_AUDIO_SETTINGS}.
     * <p>This method must be called by applications having requested the use of
     * bluetooth SCO audio with {@link #startBluetoothSco()}
     * when finished with the SCO connection or if the establishment times out.
     * @see #startBluetoothSco()
     */
    CARAPI StopBluetoothSco();

    /**
     * Request use of Bluetooth SCO headset for communications.
     * <p>
     * This method should only be used by applications that replace the platform-wide
     * management of audio settings or the main telephony application.
     *
     * @param on set <var>TRUE</var> to use bluetooth SCO for communications;
     *               <var>FALSE</var> to not use bluetooth SCO for communications
     */
    CARAPI SetBluetoothScoOn(
        /* [in] */ Boolean on);

    /**
     * Checks whether communications use Bluetooth SCO.
     *
     * @return TRUE if SCO is used for communications;
     *         FALSE if otherwise
     */
    CARAPI IsBluetoothScoOn(
        /* [out] */ Boolean* result);

    /**
     * @param on set <var>TRUE</var> to route A2DP audio to/from Bluetooth
     *           headset; <var>FALSE</var> disable A2DP audio
     * @deprecated Do not use.
     */
    //@Deprecated
    CARAPI SetBluetoothA2dpOn(
        /* [in] */ Boolean on);

    /**
     * Checks whether A2DP audio routing to the Bluetooth headset is on or off.
     *
     * @return TRUE if A2DP audio is being routed to/from Bluetooth headset;
     *         FALSE if otherwise
     */
    CARAPI IsBluetoothA2dpOn(
        /* [out] */ Boolean* result);

    /**
     * @hide
     * Signals whether remote submix audio rerouting is enabled.
     */
    CARAPI SetRemoteSubmixOn(
        /* [in] */ Boolean on,
        /* [in] */ Int32 address);

    /**
     * Sets audio routing to the wired headset on or off.
     *
     * @param on set <var>TRUE</var> to route audio to/from wired
     *           headset; <var>FALSE</var> disable wired headset audio
     * @deprecated Do not use.
     */
    //@Deprecated
    CARAPI SetWiredHeadsetOn(
        /* [in] */ Boolean on);

    /**
     * Checks whether audio routing to the wired headset is on or off.
     *
     * @return TRUE if audio is being routed to/from wired headset;
     *         FALSE if otherwise
     */
    CARAPI IsWiredHeadsetOn(
        /* [out] */ Boolean* result);

    /**
     * Sets the microphone mute on or off.
     * <p>
     * This method should only be used by applications that replace the platform-wide
     * management of audio settings or the main telephony application.
     *
     * @param on set <var>TRUE</var> to mute the microphone;
     *           <var>FALSE</var> to turn mute off
     */
    CARAPI SetMicrophoneMute(
        /* [in] */ Boolean on);

    /**
     * Checks whether the microphone mute is on or off.
     *
     * @return TRUE if microphone is muted, FALSE if it's not
     */
    CARAPI IsMicrophoneMute(
        /* [out] */ Boolean* result);

    /**
     * Sets the audio mode.
     * <p>
     * The audio mode encompasses audio routing AND the behavior of
     * the telephony layer. Therefore this method should only be used by applications that
     * replace the platform-wide management of audio settings or the main telephony application.
     * In particular, the {@link #MODE_IN_CALL} mode should only be used by the telephony
     * application when it places a phone call, as it will cause signals from the radio layer
     * to feed the platform mixer.
     *
     * @param mode  the requested audio mode (NORMAL, RINGTONE, or IN_CALL).
     *              Informs the HAL about the current audio state so that
     *              it can route the audio appropriately.
     */
    CARAPI SetMode(
        /* [in] */ Int32 mode);

    /**
     * Returns the current audio mode.
     *
     * @return      the current audio mode (NORMAL, RINGTONE, or IN_CALL).
     *              Returns the current current audio state from the HAL.
     */
    CARAPI GetMode(
        /* [out] */ Int32* result);

    /**
     * Sets the audio routing for a specified mode
     *
     * @param mode   audio mode to change route. E.g., MODE_RINGTONE.
     * @param routes bit vector of routes requested, created from one or
     *               more of ROUTE_xxx types. Set bits indicate that route should be on
     * @param mask   bit vector of routes to change, created from one or more of
     * ROUTE_xxx types. Unset bits indicate the route should be left unchanged
     *
     * @deprecated   Do not set audio routing directly, use setSpeakerphoneOn(),
     * setBluetoothScoOn() methods instead.
     */
    //@Deprecated
    CARAPI SetRouting(
        /* [in] */ Int32 mode,
        /* [in] */ Int32 routes,
        /* [in] */ Int32 mask);

    /**
     * Returns the current audio routing bit vector for a specified mode.
     *
     * @param mode audio mode to get route (e.g., MODE_RINGTONE)
     * @return an audio route bit vector that can be compared with ROUTE_xxx
     * bits
     * @deprecated   Do not query audio routing directly, use isSpeakerphoneOn(),
     * isBluetoothScoOn(), isBluetoothA2dpOn() and isWiredHeadsetOn() methods instead.
     */
    //@Deprecated
    CARAPI GetRouting(
        /* [in] */ Int32 mode,
        /* [out] */ Int32* result);

    /**
     * Checks whether any music is active.
     *
     * @return TRUE if any music tracks are active.
     */
    CARAPI IsMusicActive(
        /* [out] */ Boolean* result);

    /**
     * @hide
     * Checks whether speech recognition is active
     * @return true if a recording with source {@link MediaRecorder.AudioSource#VOICE_RECOGNITION}
     *    is underway.
     */
    CARAPI IsSpeechRecognitionActive(
        /* [out] */ Boolean* result);

    /**
     * @hide
     * If the stream is active locally or remotely, adjust its volume according to the enforced
     * priority rules.
     * Note: only AudioManager.STREAM_MUSIC is supported at the moment
     */
    CARAPI AdjustLocalOrRemoteStreamVolume(
        /* [in] */ Int32 streamType,
        /* [in] */ Int32 direction);

    /*
     * Sets a generic audio configuration parameter. The use of these parameters
     * are platform dependant, see libaudio
     *
     * ** Temporary interface - DO NOT USE
     *
     * TODO: Replace with a more generic key:value get/set mechanism
     *
     * param key   name of parameter to set. Must not be NULL.
     * param value value of parameter. Must not be NULL.
     */
    /**
     * @hide
     * @deprecated Use {@link #setPrameters(String)} instead
     */
    //@Deprecated
    CARAPI SetParameter(
        /* [in] */ const String& key,
        /* [in] */ const String& value);

    /**
     * Sets a variable number of parameter values to audio hardware.
     *
     * @param keyValuePairs list of parameters key value pairs in the form:
     *    key1=value1;key2=value2;...
     *
     */
    CARAPI SetParameters(
        /* [in] */ const String& keyValuePairs);

    /**
     * Sets a varaible number of parameter values to audio hardware.
     *
     * @param keys list of parameters
     * @return list of parameters key value pairs in the form:
     *    key1=value1;key2=value2;...
     */
    CARAPI GetParameters(
        /* [in] */ const String& keys,
        /* [out] */ String* parameters);

    /**
     * Plays a sound effect (Key clicks, lid open/close...)
     * @param effectType The type of sound effect. One of
     *            {@link #FX_KEY_CLICK},
     *            {@link #FX_FOCUS_NAVIGATION_UP},
     *            {@link #FX_FOCUS_NAVIGATION_DOWN},
     *            {@link #FX_FOCUS_NAVIGATION_LEFT},
     *            {@link #FX_FOCUS_NAVIGATION_RIGHT},
     *            {@link #FX_KEYPRESS_STANDARD},
     *            {@link #FX_KEYPRESS_SPACEBAR},
     *            {@link #FX_KEYPRESS_DELETE},
     *            {@link #FX_KEYPRESS_RETURN},
     * NOTE: This version uses the UI settings to determine
     * whether sounds are heard or not.
     */
    CARAPI PlaySoundEffect(
        /* [in] */ Int32 effectType);

    /**
     * Plays a sound effect (Key clicks, lid open/close...)
     * @param effectType The type of sound effect. One of
     *            {@link #FX_KEY_CLICK},
     *            {@link #FX_FOCUS_NAVIGATION_UP},
     *            {@link #FX_FOCUS_NAVIGATION_DOWN},
     *            {@link #FX_FOCUS_NAVIGATION_LEFT},
     *            {@link #FX_FOCUS_NAVIGATION_RIGHT},
     *            {@link #FX_KEYPRESS_STANDARD},
     *            {@link #FX_KEYPRESS_SPACEBAR},
     *            {@link #FX_KEYPRESS_DELETE},
     *            {@link #FX_KEYPRESS_RETURN},
     * @param volume Sound effect volume.
     * The volume value is a raw scalar so UI controls should be scaled logarithmically.
     * If a volume of -1 is specified, the AudioManager.STREAM_MUSIC stream volume minus 3dB will be used.
     * NOTE: This version is for applications that have their own
     * settings panel for enabling and controlling volume.
     */
    CARAPI PlaySoundEffect(
        /* [in] */ Int32 effectType,
        /* [in] */ Float volume);

    /**
     *  Load Sound effects.
     *  This method must be called when sound effects are enabled.
     */
    CARAPI LoadSoundEffects();

    /**
     *  Unload Sound effects.
     *  This method can be called to free some memory when
     *  sound effects are disabled.
     */
    CARAPI UnloadSoundEffects();

    /**
     * @hide
     * Registers a listener to be called when audio focus changes. Calling this method is optional
     * before calling {@link #requestAudioFocus(IOnAudioFocusChangeListener, Int32, Int32)}, as it
     * will register the listener as well if it wasn't registered already.
     * @param l the listener to be notified of audio focus changes.
     */
    CARAPI RegisterAudioFocusListener(
        /* [in] */ IOnAudioFocusChangeListener* l);

    /**
     * @hide
     * Causes the specified listener to not be called anymore when focus is gained or lost.
     * @param l the listener to unregister.
     */
    CARAPI UnregisterAudioFocusListener(
        /* [in] */ IOnAudioFocusChangeListener* l);

    /**
     *  Request audio focus.
     *  Send a request to obtain the audio focus
     *  @param l the listener to be notified of audio focus changes
     *  @param streamType the main audio stream type affected by the focus request
     *  @param durationHint use {@link #AUDIOFOCUS_GAIN_TRANSIENT} to indicate this focus request
     *      is temporary, and focus will be abandonned shortly. Examples of transient requests are
     *      for the playback of driving directions, or notifications sounds.
     *      Use {@link #AUDIOFOCUS_GAIN_TRANSIENT_MAY_DUCK} to indicate also that it's ok for
     *      the previous focus owner to keep playing if it ducks its audio output.
     *      Use {@link #AUDIOFOCUS_GAIN} for a focus request of unknown duration such
     *      as the playback of a song or a video.
     *  @return {@link #AUDIOFOCUS_REQUEST_FAILED} or {@link #AUDIOFOCUS_REQUEST_GRANTED}
     */
    CARAPI RequestAudioFocus(
        /* [in] */ IOnAudioFocusChangeListener* l,
        /* [in] */ Int32 streamType,
        /* [in] */ Int32 durationHint,
        /* [out] */ Int32* result);

    /**
     * @hide
     * Used internally by telephony package to request audio focus. Will cause the focus request
     * to be associated with the "voice communication" identifier only used in AudioService
     * to identify this use case.
     * @param streamType use STREAM_RING for focus requests when ringing, VOICE_CALL for
     *    the establishment of the call
     * @param durationHint the type of focus request. AUDIOFOCUS_GAIN_TRANSIENT is recommended so
     *    media applications resume after a call
     */
    CARAPI RequestAudioFocusForCall(
        /* [in] */ Int32 streamType,
        /* [in] */ Int32 durationHint);

    /**
     * @hide
     * Used internally by telephony package to abandon audio focus, typically after a call or
     * when ringing ends and the call is rejected or not answered.
     * Should match one or more calls to {@link #requestAudioFocusForCall(Int32, Int32)}.
     */
    CARAPI AbandonAudioFocusForCall();

    /**
     *  Abandon audio focus. Causes the previous focus owner, if any, to receive focus.
     *  @param l the listener with which focus was requested.
     *  @return {@link #AUDIOFOCUS_REQUEST_FAILED} or {@link #AUDIOFOCUS_REQUEST_GRANTED}
     */
    CARAPI AbandonAudioFocus(
        /* [in] */ IOnAudioFocusChangeListener* l,
        /* [out] */ Int32* result);

    //====================================================================
    // Remote Control
    /**
     * Register a component to be the sole receiver of MEDIA_BUTTON intents.
     * @param eventReceiver identifier of a {@link android.content.BroadcastReceiver}
     *      that will receive the media button intent. This broadcast receiver must be declared
     *      in the application manifest.
     */
    CARAPI RegisterMediaButtonEventReceiver(
        /* [in] */ IComponentName* eventReceiver);

    /**
     * @hide
     * no-op if (pi == null) or (eventReceiver == null)
     */
    CARAPI RegisterMediaButtonIntent(
        /* [in] */ IPendingIntent* pi,
        /* [in] */ IComponentName* eventReceiver);

    /**
     * @hide
     * Used internally by telephony package to register an intent receiver for ACTION_MEDIA_BUTTON.
     * @param eventReceiver the component that will receive the media button key events,
     *          no-op if eventReceiver is null
     */
    CARAPI RegisterMediaButtonEventReceiverForCalls(
        /* [in] */ IComponentName* eventReceiver);

    /**
     * @hide
     */
    CARAPI UnregisterMediaButtonEventReceiverForCalls();

    /**
     * Unregister the receiver of MEDIA_BUTTON intents.
     * @param eventReceiver identifier of a {@link android.content.BroadcastReceiver}
     *      that was registered with {@link #registerMediaButtonEventReceiver(ComponentName)}.
     */
    CARAPI UnregisterMediaButtonEventReceiver(
        /* [in] */ IComponentName* eventReceiver);

    /**
     * @hide
     */
    CARAPI UnregisterMediaButtonIntent(
        /* [in] */ IPendingIntent* pi,
        /* [in] */ IComponentName* eventReceiver);

    /**
     * Registers the remote control client for providing information to display on the remote
     * controls.
     * @param rcClient The remote control client from which remote controls will receive
     *      information to display.
     * @see RemoteControlClient
     */
    CARAPI RegisterRemoteControlClient(
        /* [in] */ IRemoteControlClient* rcClient);

    /**
     * Unregisters the remote control client that was providing information to display on the
     * remote controls.
     * @param rcClient The remote control client to unregister.
     * @see #registerRemoteControlClient(RemoteControlClient)
     */
    CARAPI UnregisterRemoteControlClient(
        /* [in] */ IRemoteControlClient* rcClient);

    /**
     * @hide
     * Registers a remote control display that will be sent information by remote control clients.
     * @param rcd
     */
    CARAPI RegisterRemoteControlDisplay(
        /* [in] */ IIRemoteControlDisplay* rcd);

    /**
     * @hide
     * Unregisters a remote control display that was sent information by remote control clients.
     * @param rcd
     */
    CARAPI UnregisterRemoteControlDisplay(
        /* [in] */ IIRemoteControlDisplay* rcd);

    /**
     * @hide
     * Sets the artwork size a remote control display expects when receiving bitmaps.
     * @param rcd
     * @param w the maximum width of the expected bitmap. Negative values indicate it is
     *   useless to send artwork.
     * @param h the maximum height of the expected bitmap. Negative values indicate it is
     *   useless to send artwork.
     */
    CARAPI RemoteControlDisplayUsesBitmapSize(
        /* [in] */ IIRemoteControlDisplay* rcd,
        /* [in] */ Int32 w,
        /* [in] */ Int32 h);
    /**
     *  @hide
     *  Reload audio settings. This method is called by Settings backup
     *  agent when audio settings are restored and causes the AudioService
     *  to read and apply restored settings.
     */
    CARAPI ReloadAudioSettings();

    /**
     * Checks whether the phone is in silent mode, with or without vibrate.
     *
     * @return true if phone is in silent mode, with or without vibrate.
     *
     * @see #getRingerMode()
     *
     * @hide pending API Council approval
     */
    CARAPI IsSilentMode(
        /* [out] */ Boolean* result);

    /**
     * Return the enabled devices for the specified output stream type.
     *
     * @param streamType The stream type to query. One of
     *            {@link #STREAM_VOICE_CALL},
     *            {@link #STREAM_SYSTEM},
     *            {@link #STREAM_RING},
     *            {@link #STREAM_MUSIC},
     *            {@link #STREAM_ALARM},
     *            {@link #STREAM_NOTIFICATION},
     *            {@link #STREAM_DTMF}.
     *
     * @return The bit-mask "or" of audio output device codes for all enabled devices on this
     *         stream. Zero or more of
     *            {@link #DEVICE_OUT_EARPIECE},
     *            {@link #DEVICE_OUT_SPEAKER},
     *            {@link #DEVICE_OUT_WIRED_HEADSET},
     *            {@link #DEVICE_OUT_WIRED_HEADPHONE},
     *            {@link #DEVICE_OUT_BLUETOOTH_SCO},
     *            {@link #DEVICE_OUT_BLUETOOTH_SCO_HEADSET},
     *            {@link #DEVICE_OUT_BLUETOOTH_SCO_CARKIT},
     *            {@link #DEVICE_OUT_BLUETOOTH_A2DP},
     *            {@link #DEVICE_OUT_BLUETOOTH_A2DP_HEADPHONES},
     *            {@link #DEVICE_OUT_BLUETOOTH_A2DP_SPEAKER},
     *            {@link #DEVICE_OUT_AUX_DIGITAL},
     *            {@link #DEVICE_OUT_ANLG_DOCK_HEADSET},
     *            {@link #DEVICE_OUT_DGTL_DOCK_HEADSET}.
     *            {@link #DEVICE_OUT_DEFAULT} is not used here.
     *
     * The implementation may support additional device codes beyond those listed, so
     * the application should ignore any bits which it does not recognize.
     * Note that the information may be imprecise when the implementation
     * cannot distinguish whether a particular device is enabled.
     *
     * {@hide}
     */
    CARAPI GetDevicesForStream(
        /* [in] */ Int32 streamType,
        /* [out] */ Int32* result);

     /**
     * Indicate wired accessory connection state change.
     * @param device type of device connected/disconnected (AudioManager.DEVICE_OUT_xxx)
     * @param state  new connection state: 1 connected, 0 disconnected
     * @param name   device name
     * {@hide}
     */
    CARAPI SetWiredDeviceConnectionState(
        /* [in] */ Int32 device,
        /* [in] */ Int32 state,
        /* [in] */ const String& name);

     /**
     * Indicate A2DP sink connection state change.
     * @param device Bluetooth device connected/disconnected
     * @param state  new connection state (BluetoothProfile.STATE_xxx)
     * @return a delay in ms that the caller should wait before broadcasting
     * BluetoothA2dp.ACTION_CONNECTION_STATE_CHANGED intent.
     * {@hide}
     */
    CARAPI SetBluetoothA2dpDeviceConnectionState(
        /* [in] */ IBluetoothDevice* device,
        /* [in] */ Int32 state,
        /* [out] */ Int32* result);

    /** {@hide} */
    CARAPI GetRingtonePlayer(
        /* [out] */ IIRingtonePlayer** result);

    /**
     * Returns the value of the property with the specified key.
     * @param key One of the strings corresponding to a property key: either
     *            {@link #PROPERTY_OUTPUT_SAMPLE_RATE} or
     *            {@link #PROPERTY_OUTPUT_FRAMES_PER_BUFFER}
     * @return A string representing the associated value for that property key,
     *         or null if there is no value for that key.
     */
    CARAPI GetProperty(
        /* [in] */ const String& key,
        /* [out] */ String* result);

    /**
     * Checks valid ringer mode values.
     *
     * @return true if the ringer mode indicated is valid, false otherwise.
     *
     * @see #setRingerMode(Int32)
     * @hide
     */
    static CARAPI IsValidRingerMode(
        /* [in] */ Int32 ringerMode,
        /* [out] */ Boolean* result);

    /**
    * @hide
    */
    static CARAPI SetHdmiAvailable(
        /* [in] */ Boolean val);

    static CARAPI GetHdmiAvailable(
        /* [out] */ Boolean* result);

    /**
    * @hide
    */
    static CARAPI GetHdmiExpected(
        /* [out] */ Boolean* result);

    /**
    * @hide
    */
    static CARAPI SetHdmiExpected(
        /* [in] */ Boolean val);

    /* get audio devices list(the devices which are supported)
    *  @param devType  the audio device type,maybe in/out
    */
    CARAPI GetAudioDevices(
        /* [in] */ const String& devType,
        /* [out, callle] */ ArrayOf<String>** result);

    /* get current active devices */
    /* @param devType  the audio device type
    *  @return the current list of the active device
    */
    CARAPI GetActiveAudioDevices(
        /* [in] */ const String& devType,
        /* [out, callle] */ ArrayOf<String>** result);

    /* set audio devices active
     * @param devices
     * @param state  must be AUDIO_INPUT_ACTIVE or AUDIO_OUTPUT_ACTIVE
     */
    CARAPI SetAudioDeviceActive(
        /* [in] */ const ArrayOf<String>& devices,
        /* [in] */ const String& state);

private:
    static CARAPI_(AutoPtr<IIAudioService>) GetService();

    /**
     * Settings has an in memory cache, so this is fast.
     */
    CARAPI_(Boolean) QuerySoundEffectsEnabled();

    CARAPI_(AutoPtr<IOnAudioFocusChangeListener>) FindFocusListener(
        /* [in] */ const String& id);

    CARAPI_(String) GetIdForAudioFocusListener(
        /* [in] */ IOnAudioFocusChangeListener* l);

    static CARAPI_(void) WriteFileVal(
        /* [in] */ const String& filename,
        /* [in] */ Int32 avail);

    static CARAPI_(Int32) ReadFileVal(
        /* [in] */ const String& filename);

public:
    /**  @hide Default volume index values for audio streams */
    static const Int32 DEFAULT_STREAM_VOLUME[];

private:
    static const String TAG;
    static const Boolean DEBUG;

    static const String hdmiExpected ;// = "audio.hdmi.expected";
    static const String hdmiAvailable ; // = "audio.hdmi.available";

    AutoPtr<IContext> mContext;
    Int64 mVolumeKeyUpTime;
    Boolean mUseMasterVolume;
    Boolean mUseVolumeKeySounds;

  // maximum valid ringer mode value. Values must start from 0 and be contiguous.
    static const Int32 RINGER_MODE_MAX;

    static AutoPtr<IIAudioService> sService;

    /**
     * Map to convert focus event listener IDs, as used in the AudioService audio focus stack,
     * to actual listener objects.
     */
    HashMap<String, AutoPtr<IOnAudioFocusChangeListener> > mAudioFocusIdListenerMap;

    /**
     * Lock to prevent concurrent changes to the list of focus listeners for this AudioManager
     * instance.
     */
    Object mFocusListenerLock;

    /**
     * Handler for audio focus events coming from the audio service.
     */
    AutoPtr<FocusEventHandlerDelegate> mAudioFocusEventHandlerDelegate;

    AutoPtr<IIAudioFocusDispatcher> mAudioFocusDispatcher;

    /**
     * {@hide}
     */
    AutoPtr<IBinder> mICallBack;
};

} // namespace Media
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_MEDIA_CAUDIOMANAGER_H__
