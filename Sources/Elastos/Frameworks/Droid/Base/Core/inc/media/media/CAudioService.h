
#ifndef __CAUDIOSERVICE_H__
#define __CAUDIOSERVICE_H__

#include "_CAudioService.h"
#include "ext/frameworkext.h"
#include <elastos/utility/etl/List.h>
#include <elastos/Stack.h>
#include <elastos/utility/etl/HashMap.h>
#include "os/HandlerBase.h"
#include "database/ContentObserver.h"
#include "content/BroadcastReceiver.h"

using Elastos::Utility::Etl::List;
using Elastos::Utility::Etl::HashMap;
using Elastos::IO::IPrintWriter;
using Elastos::IO::IFileDescriptor;
using Elastos::Droid::App::IPendingIntent;
using Elastos::Droid::App::IKeyguardManager;
using Elastos::Droid::Content::IIntent;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::IContentResolver;
using Elastos::Droid::Content::IBroadcastReceiver;
using Elastos::Droid::Content::BroadcastReceiver;
using Elastos::Droid::Content::IComponentName;
using Elastos::Droid::Os::HandlerBase;
using Elastos::Droid::Os::IBinder;
using Elastos::Droid::Os::IBundle;
using Elastos::Droid::Os::IPowerManagerWakeLock;
using Elastos::Droid::Os::IRemoteCallbackList;
using Elastos::Droid::Database::IContentObserver;
using Elastos::Droid::Database::ContentObserver;
using Elastos::Droid::Bluetooth::IBluetoothDevice;
using Elastos::Droid::Bluetooth::IBluetoothHeadset;
using Elastos::Droid::Bluetooth::IBluetoothProfileServiceListener;
using Elastos::Droid::View::IKeyEvent;
using Elastos::Droid::Widget::IVolumePanel;
using Elastos::Utility::Stack;

namespace Elastos {
namespace Droid {
namespace Media {

CarClass(CAudioService)
{
public:
    ///////////////////////////////////////////////////////////////////////////
    // Inner classes
    ///////////////////////////////////////////////////////////////////////////

    class VolumeStreamState
        : public IInterface
        , public ElRefBase
    {
    friend class CAudioService;

    private:
        class VolumeDeathHandler
            : public ElRefBase
            , public IProxyDeathRecipient
        {
        friend class CAudioService;

        public:
            CAR_INTERFACE_DECL();

            VolumeDeathHandler(
                /* [in] */ IBinder* cb,
                /* [in] */ VolumeStreamState* owner,
                /* [in] */ CAudioService* service);

            // must be called while synchronized on parent VolumeStreamState
            CARAPI Mute(
                /* [in] */ Boolean state);

            /*implements IProxyDeathRecipient*/
            CARAPI ProxyDied();

        private:
            AutoPtr<IBinder> mICallback; // To be notified of client's death

            Int32 mMuteCount; // Number of active mutes for this client

            VolumeStreamState* mVolumeStreamState;
            CAudioService* mAudioService;
        };

    public:
        CAR_INTERFACE_DECL();

        VolumeStreamState(
            /* [in] */ const String& settingName,
            /* [in] */ Int32 streamType,
            /* [in] */ CAudioService* owner);

        CARAPI GetSettingNameForDevice(
            /* [in] */ Boolean lastAudible,
            /* [in] */ Int32 device,
            /* [out] */ String* result);

        CARAPI ReadSettings();

        CARAPI ApplyDeviceVolume(
            /* [in] */ Int32 device);

        CARAPI ApplyAllVolumes();

        CARAPI AdjustIndex(
            /* [in] */ Int32 deltaIndex,
            /* [in] */ Int32 device,
            /* [out] */ Boolean* result);

        CARAPI SetIndex(
            /* [in] */ Int32 index,
            /* [in] */ Int32 device,
            /* [in] */ Boolean lastAudible,
            /* [out] */ Boolean* result);

        CARAPI GetIndex(
            /* [in] */ Int32 device,
            /* [in] */ Boolean lastAudible,
            /* [out] */ Int32* result);

        CARAPI SetLastAudibleIndex(
            /* [in] */ Int32 index,
            /* [in] */ Int32 device);

        CARAPI AdjustLastAudibleIndex(
            /* [in] */ Int32 deltaIndex,
            /* [in] */ Int32 device);

        CARAPI GetMaxIndex(
            /* [out] */ Int32* result);

        CARAPI SetAllIndexes(
            /* [in] */ VolumeStreamState* srcStream,
            /* [in] */ Boolean lastAudible);

        CARAPI SetAllIndexesToMax();

        CARAPI Mute(
            /* [in] */ IBinder* cb,
            /* [in] */ Boolean state);

        CARAPI GetStreamType(
            /* [out] */ Int32* result);

        CARAPI_(String) ToString();

    private:
        CARAPI_(Int32) GetValidIndex(
            /* [in] */ Int32 index);

        CARAPI_(Int32) MuteCount();

        CARAPI_(AutoPtr<VolumeDeathHandler>) GetDeathHandler(
            /* [in] */ IBinder* cb,
            /* [in] */ Boolean state);

    private:
        Int32 mStreamType;

        String mVolumeIndexSettingName;
        String mLastAudibleVolumeIndexSettingName;
        Int32 mIndexMax;

//        private final ConcurrentHashMap<Integer, Integer> mIndex = new ConcurrentHashMap<Integer, Integer>(8, 0.75f, 4);
        /*const*/ HashMap<Int32, Int32> mIndex;
        Object mIndexLock;

//        private final ConcurrentHashMap<Integer, Integer> mLastAudibleIndex = new ConcurrentHashMap<Integer, Integer>(8, 0.75f, 4);
        /*const*/ HashMap<Int32, Int32> mLastAudibleIndex;
        Object mLastAudibleIndexLock;

        List< AutoPtr<VolumeDeathHandler> > mDeathHandlers; //handles mute/solo clients death

        /*add by chenjd, 2013-09-04
        *fix AndroidBug about mute, see class define of VolumeDeathHandler for detail.
        */
        Int32 mMuteCount;

        Object mThisLock;

        CAudioService* mAudioService;
    };

private:
    class ForceControlStreamClient
        : public ElRefBase
        , public IProxyDeathRecipient
    {
    public:
        CAR_INTERFACE_DECL();

        ForceControlStreamClient(
            /* [in] */ IBinder* cb,
            /* [in] */ CAudioService* owner);

        /*implements IProxyDeathRecipient*/
        CARAPI ProxyDied();

        CARAPI_(void) ReleaseResources();

    private:
        AutoPtr<IBinder> mCb; // To be notified of client's death

        CAudioService* mAudioService;
    };

    class SetModeDeathHandler
        : public ElRefBase
        , public IProxyDeathRecipient
    {
    public:
        CAR_INTERFACE_DECL();

        SetModeDeathHandler(
            /* [in] */ IBinder* cb,
            /* [in] */ Int32 pid,
            /* [in] */ CAudioService* owner);

        /*implements IProxyDeathRecipient*/
        CARAPI ProxyDied();

        CARAPI_(Int32) GetPid();

        CARAPI_(void) SetMode(
            /* [in] */ Int32 mode);

        CARAPI_(Int32) GetMode();

        CARAPI_(AutoPtr<IBinder>) GetBinder();

    private:
        AutoPtr<IBinder> mCb; // To be notified of client's death
        Int32 mPid;
        Int32 mMode; // = AudioSystem.MODE_NORMAL; // Current mode set by this client

        CAudioService* mAudioService;
    };

    class SoundPoolListenerThread
        : public ThreadBase
    {
    public:
        SoundPoolListenerThread(
            /* [in] */ CAudioService* owner);

        //@Override /*implements Thread*/
        virtual CARAPI Run();

    private:
        CAudioService* mAudioService;
    };

    class SoundPoolCallback
        : public ElRefBase
        , public IOnLoadCompleteListener //defines in ISoundPool
    {
    public:
        CAR_INTERFACE_DECL();

        SoundPoolCallback(
            /* [in] */ CAudioService* owner);

        CARAPI_(Int32) Status();

        CARAPI_(void) SetLastSample(
            /* [in] */ Int32 sample);

        /*implements IOnLoadCompleteListener*/
        CARAPI OnLoadComplete(
            /* [in] */ ISoundPool* soundPool,
            /* [in] */ Int32 sampleId,
            /* [in] */ Int32 status);

    private:
        Int32 mStatus;
        Int32 mLastSample;

        CAudioService* mAudioService;
    };

    class ScoClient
        : public ElRefBase
        , public IProxyDeathRecipient
    {
    public:
        CAR_INTERFACE_DECL();

        ScoClient(
            /* [in] */ IBinder* cb,
            /* [in] */ CAudioService* owner);

        /*implements IProxyDeathRecipient*/
        CARAPI ProxyDied();

        CARAPI_(void) IncCount();

        CARAPI_(void) DecCount();

        CARAPI_(void) ClearCount(
            /* [in] */ Boolean stopSco);

        CARAPI_(Int32) GetCount();

        CARAPI_(AutoPtr<IBinder>) GetBinder();

        CARAPI_(Int32) GetPid();

        CARAPI_(Int32) TotalCount();

    private:
        CARAPI_(void) RequestScoState(
            /* [in] */ Int32 state);

    private:
        AutoPtr<IBinder> mCb; // To be notified of client's death
        Int32 mCreatorPid;
        Int32 mStartcount; // number of SCO connections started by this client

        CAudioService* mAudioService;
    };

    /** Thread that handles native AudioSystem control. */
    class AudioSystemThread
        : public ThreadBase
    {
    public:
        AudioSystemThread(
            /* [in] */ CAudioService* owner);

        //@Override /*implements Thread*/
        virtual CARAPI Run();

    private:
        CAudioService* mAudioService;
    };

    /** Handles internal volume messages in separate volume thread. */
    class AudioHandler
        : public HandlerBase
    {
    friend class CAudioService;

    public:
        AudioHandler(
            /* [in] */ CAudioService* owner);

        ~AudioHandler();

        //@Override
        CARAPI HandleMessage(
            /* [in] */ IMessage* msg);

    private:
        CARAPI_(void) SetDeviceVolume(
            /* [in] */ VolumeStreamState* streamState,
            /* [in] */ Int32 device);

        CARAPI_(void) SetAllVolumes(
            /* [in] */ VolumeStreamState* streamState);

        CARAPI_(void) PersistVolume(
            /* [in] */ VolumeStreamState* streamState,
            /* [in] */ Int32 persistType,
            /* [in] */ Int32 device);

        CARAPI_(void) PersistRingerMode(
            /* [in] */ Int32 ringerMode);

        CARAPI_(void) PlaySoundEffect(
            /* [in] */ Int32 effectType,
            /* [in] */ Int32 volume);

        CARAPI_(void) OnHandlePersistMediaButtonReceiver(
            /* [in] */ IComponentName* receiver);

        CARAPI_(void) CleanupPlayer(
            /* [in] */ IMediaPlayer* mp);

        CARAPI_(void) SetForceUse(
            /* [in] */ Int32 usage,
            /* [in] */ Int32 config);

    private:
        CAudioService* mAudioService;
    };

    class SettingsObserver : public ContentObserver
    {
    public:
        SettingsObserver(
            /* [in] */ CAudioService* owner);

        //Override //implements IContentObserver
        CARAPI OnChange(
            /* [in] */ Boolean selfChange);

    private:
        CAudioService* mAudioService;
    };

    /**
     * Receiver for misc intent broadcasts the Phone app cares about.
     */
    class AudioServiceBroadcastReceiver : public BroadcastReceiver
    {
    public:
        AudioServiceBroadcastReceiver(
            /* [in] */ CAudioService* owner);

        //Override
        CARAPI OnReceive(
            /* [in] */ IContext* context,
            /* [in] */ IIntent* intent);

        CARAPI ToString(
            /* [out] */ String* info)
        {
            VALIDATE_NOT_NULL(info);
            *info = String("CAudioService::AudioServiceBroadcastReceiver: ");
            (*info).AppendFormat("%p", this);
            return NOERROR;
        }
    private:
        CAudioService* mAudioService;
    };

    class KeyEventDoneBroadcastReceiver : public BroadcastReceiver
    {
    public:
        KeyEventDoneBroadcastReceiver(
            /* [in] */ CAudioService* owner);

        //Override
        CARAPI OnReceive(
            /* [in] */ IContext* context,
            /* [in] */ IIntent* intent);

        CARAPI ToString(
            /* [out] */ String* info)
        {
            VALIDATE_NOT_NULL(info);
            *info = String("CAudioService::KeyEventDoneBroadcastReceiver: ");
            (*info).AppendFormat("%p", this);
            return NOERROR;
        }
    private:
        CAudioService* mAudioService;
    };

    /**
     * Inner class to monitor audio focus client deaths, and remove them from the audio focus
     * stack if necessary.
     */
    class AudioFocusDeathHandler
        : public ElRefBase
        , public IProxyDeathRecipient
    {
    public:
        CAR_INTERFACE_DECL();

        AudioFocusDeathHandler(
            /* [in] */ IBinder* cb,
            /* [in] */ CAudioService* owner);

        /*implements IProxyDeathRecipient*/
        CARAPI ProxyDied();

        CARAPI_(AutoPtr<IBinder>) GetBinder();

    private:
        AutoPtr<IBinder> mCb; // To be notified of client's death

        CAudioService* mAudioService;
    };

    /*static*/
    class FocusStackEntry : public ElRefBase
    {
    public:
        FocusStackEntry();

        FocusStackEntry(
            /* [in] */ Int32 streamType,
            /* [in] */ Int32 duration,
            /* [in] */ IIAudioFocusDispatcher* afl,
            /* [in] */ IBinder* source,
            /* [in] */ const String& id,
            /* [in] */ AudioFocusDeathHandler* hdlr,
            /* [in] */ const String& pn,
            /* [in] */ Int32 uid);

        ~FocusStackEntry();

        CARAPI_(void) UnlinkToDeath();

    protected:
        //@overrode
        CARAPI_(void) Finalize();

    public:
        Int32 mStreamType; // = -1; // no stream type
        AutoPtr<IIAudioFocusDispatcher> mFocusDispatcher; // = null;
        AutoPtr<IBinder> mSourceRef; // = null;
        String mClientId;
        Int32 mFocusChangeType;
        AutoPtr<AudioFocusDeathHandler> mHandler;
        String mPackageName;
        Int32 mCallingUid;
    };

    /**
     * Inner class to monitor remote control client deaths, and remove the client for the
     * remote control stack if necessary.
     */
    class RcClientDeathHandler
        : public ElRefBase
        , public IProxyDeathRecipient
    {
    friend class CAudioService;

    public:
        CAR_INTERFACE_DECL();

        RcClientDeathHandler(
            /* [in] */ IBinder* cb,
            /* [in] */ IPendingIntent* pi,
            /* [in] */ CAudioService* owner);

        /*implements IProxyDeathRecipient*/
        CARAPI ProxyDied();

        CARAPI_(AutoPtr<IBinder>) GetBinder();

    private:
        AutoPtr<IBinder> mCb; // To be notified of client's death
        AutoPtr<IPendingIntent> mMediaIntent;

        CAudioService* mAudioService;
    };

    class RemotePlaybackState : public ElRefBase
    {
    friend class CAudioService;

    private:
        RemotePlaybackState(
            /* [in] */ Int32 id,
            /* [in] */ Int32 vol,
            /* [in] */ Int32 volMax);

    private:
        Int32 mRccId;
        Int32 mVolume;
        Int32 mVolumeMax;
        Int32 mVolumeHandling;
    };

    /*static*/
    class RemoteControlStackEntry
        : public IInterface
        , public ElRefBase
    {
    public:
        /** precondition: mediaIntent != null, eventReceiver != null */
        RemoteControlStackEntry(
            /* [in] */ IPendingIntent* mediaIntent,
            /* [in] */ IComponentName* eventReceiver);

        ~RemoteControlStackEntry();

        CAR_INTERFACE_DECL()

        CARAPI_(void) ResetPlaybackInfo();

        CARAPI_(void) UnlinkToRcClientDeath();

    protected:
        //@Override
        CARAPI_(void) Finalize();

    public:
        Int32 mRccId; // = IRemoteControlClient::RCSE_ID_UNREGISTERED;

        /**
         * The target for the ACTION_MEDIA_BUTTON events.
         * Always non null.
         */
        AutoPtr<IPendingIntent> mMediaIntent;

        /**
         * The registered media button event receiver.
         * Always non null.
         */
        AutoPtr<IComponentName> mReceiverComponent;

        String mCallingPackageName;
        Int32 mCallingUid;

        /**
         * Provides access to the information to display on the remote control.
         * May be null (when a media button event receiver is registered,
         *     but no remote control client has been registered) */
        AutoPtr<IIRemoteControlClient> mRcClient;

        AutoPtr<RcClientDeathHandler> mRcClientDeathHandler;

        /**
         * Information only used for non-local playback
         */
        Int32 mPlaybackType;
        Int32 mPlaybackVolume;
        Int32 mPlaybackVolumeMax;
        Int32 mPlaybackVolumeHandling;
        Int32 mPlaybackStream;
        Int32 mPlaybackState;
        AutoPtr<IIRemoteVolumeObserver> mRemoteVolumeObs;
    };

    /**
     * Inner class to monitor remote control display deaths, and unregister them from the list
     * of displays if necessary.
     */
    class RcDisplayDeathHandler
        : public ElRefBase
        , public IProxyDeathRecipient
    {
    public:
        CAR_INTERFACE_DECL();

        RcDisplayDeathHandler(
            /* [in] */ IBinder* b,
            /* [in] */ CAudioService* owner);

        /*implements IProxyDeathRecipient*/
        CARAPI ProxyDied();

        CARAPI_(void) UnlinkToRcDisplayDeath();

    private:
        AutoPtr<IBinder> mCb; // To be notified of client's death

        CAudioService* mAudioService;
    };

    class MediaPlayerOnCompletionListener
        : public ElRefBase
        , public IMediaPlayerOnCompletionListener
    {
    public:
        CAR_INTERFACE_DECL();

        MediaPlayerOnCompletionListener(
            /* [in] */ AudioHandler* owner);

        OnCompletion(
            /* [in] */ IMediaPlayer* mp);

    private:
        AudioHandler* mAudioService;
    };

    class MediaPlayerOnErrorListener
        : public ElRefBase
        , public IMediaPlayerOnErrorListener
    {
    public:
        CAR_INTERFACE_DECL();

        MediaPlayerOnErrorListener(
            /* [in] */ AudioHandler* owner);

        CARAPI OnError(
            /* [in] */ IMediaPlayer* mp,
            /* [in] */ Int32 what,
            /* [in] */ Int32 extra,
            /* [out] */ Boolean* result);

    private:
        AudioHandler* mAudioHandler;
    };

    class AudioSystemErrorCallback
       : public ElRefBase
       , public IAudioSystemErrorCallback
    {
    public:
        CAR_INTERFACE_DECL()

        AudioSystemErrorCallback(
            /* [in] */ CAudioService* owner);

        CARAPI OnError(
           /* [in] */ Int32 error);
    private:
        CAudioService* mAudioService;
    };

public:
    CAudioService();

    ~CAudioService();

    ///////////////////////////////////////////////////////////////////////////
    // Construction
    ///////////////////////////////////////////////////////////////////////////
    /** @hide */
    CARAPI constructor(
        /* [in] */ IContext* context);

    ///////////////////////////////////////////////////////////////////////////
    // IPC methods
    ///////////////////////////////////////////////////////////////////////////
    /** @see AudioManager#adjustVolume(int, int) */
    CARAPI AdjustVolume(
        /* [in] */ Int32 direction,
        /* [in] */ Int32 flags);

    /** @see AudioManager#adjustLocalOrRemoteStreamVolume(int, int) with current assumption
     *  on streamType: fixed to STREAM_MUSIC */
    CARAPI AdjustLocalOrRemoteStreamVolume(
        /* [in] */ Int32 streamType,
        /* [in] */ Int32 direction);

    /** @see AudioManager#adjustVolume(int, int, int) */
    CARAPI AdjustSuggestedStreamVolume(
        /* [in] */ Int32 direction,
        /* [in] */ Int32 suggestedStreamType,
        /* [in] */ Int32 flags);

    /** @see AudioManager#adjustStreamVolume(int, int, int) */
    CARAPI AdjustStreamVolume(
        /* [in] */ Int32 streamType,
        /* [in] */ Int32 direction,
        /* [in] */ Int32 flags);

    /** @see AudioManager#adjustMasterVolume(int) */
    CARAPI AdjustMasterVolume(
        /* [in] */ Int32 steps,
        /* [in] */ Int32 flags);

    /** @see AudioManager#setStreamVolume(int, int, int) */
    CARAPI SetStreamVolume(
        /* [in] */ Int32 streamType,
        /* [in] */ Int32 index,
        /* [in] */ Int32 flags);

    /** @see AudioManager#forceVolumeControlStream(int) */
    CARAPI ForceVolumeControlStream(
        /* [in] */ Int32 streamType,
        /* [in] */ IBinder* cb);

    /** @see AudioManager#setStreamSolo(int, boolean) */
    CARAPI SetStreamSolo(
        /* [in] */ Int32 streamType,
        /* [in] */ Boolean state,
        /* [in] */ IBinder* cb);

    /** @see AudioManager#setStreamMute(int, boolean) */
    CARAPI SetStreamMute(
        /* [in] */ Int32 streamType,
        /* [in] */ Boolean state,
        /* [in] */ IBinder* cb);

    /** get stream mute state. */
    CARAPI IsStreamMute(
        /* [in] */ Int32 streamType,
        /* [out] */ Boolean* result);

    /** @see AudioManager#setMasterMute(boolean, IBinder) */
    CARAPI SetMasterMute(
        /* [in] */ Boolean state,
        /* [in] */ Int32 flags,
        /* [in] */ IBinder* cb);

    /** get master mute state. */
    CARAPI IsMasterMute(
        /* [out] */ Boolean* result);

    /** @see AudioManager#getStreamVolume(int) */
    CARAPI GetStreamVolume(
        /* [in] */ Int32 streamType,
        /* [out] */ Int32* result);

    CARAPI GetMasterVolume(
        /* [out] */ Int32* result);

    CARAPI SetMasterVolume(
        /* [in] */ Int32 volume,
        /* [in] */ Int32 flags);

    /** @see AudioManager#getStreamMaxVolume(int) */
    CARAPI GetStreamMaxVolume(
        /* [in] */ Int32 streamType,
        /* [out] */ Int32* result);

    CARAPI GetMasterMaxVolume(
        /* [out] */ Int32* result);

    /** Get last audible volume before stream was muted. */
    CARAPI GetLastAudibleStreamVolume(
        /* [in] */ Int32 streamType,
        /* [out] */ Int32* result);

    /** Get last audible master volume before it was muted. */
    CARAPI GetLastAudibleMasterVolume(
        /* [out] */ Int32* result);

    /** @see AudioManager#getMasterStreamType(int) */
    CARAPI GetMasterStreamType(
        /* [out] */ Int32* result);


    /** @see AudioManager#getRingerMode() */
    CARAPI GetRingerMode(
        /* [out] */ Int32* result);

    /** @see AudioManager#setRingerMode(int) */
    CARAPI SetRingerMode(
        /* [in] */ Int32 ringerMode);

    /** @see AudioManager#shouldVibrate(int) */
    CARAPI ShouldVibrate(
        /* [in] */ Int32 vibrateType,
        /* [out] */ Boolean* result);

    /** @see AudioManager#getVibrateSetting(int) */
    CARAPI GetVibrateSetting(
        /* [in] */ Int32 vibrateType,
        /* [out] */ Int32* result);

    /** @see AudioManager#setVibrateSetting(int, int) */
    CARAPI SetVibrateSetting(
        /* [in] */ Int32 vibrateType,
        /* [in] */ Int32 vibrateSetting);

    /** @see AudioManager#setMode(int) */
    CARAPI SetMode(
        /* [in] */ Int32 mode,
        /* [in] */ IBinder* cb);

    /** @see AudioManager#getMode() */
    CARAPI GetMode(
        /* [out] */ Int32* result);

    /** @see AudioManager#playSoundEffect(int) */
    CARAPI PlaySoundEffect(
        /* [in] */ Int32 effectType);

    /** @see AudioManager#playSoundEffect(int, float) */
    CARAPI PlaySoundEffectVolume(
        /* [in] */ Int32 effectType,
        /* [in] */ Float volume);

    /**
     * Loads samples into the soundpool.
     * This method must be called at when sound effects are enabled
     */
    CARAPI LoadSoundEffects(
        /* [out] */ Boolean* result);

    /**
     *  Unloads samples from the sound pool.
     *  This method can be called to free some memory when
     *  sound effects are disabled.
     */
    CARAPI UnloadSoundEffects();

    /** @see AudioManager#reloadAudioSettings() */
    CARAPI ReloadAudioSettings();

    /** @see AudioManager#setSpeakerphoneOn() */
    CARAPI SetSpeakerphoneOn(
        /* [in] */ Boolean on);

    /** @see AudioManager#isSpeakerphoneOn() */
    CARAPI IsSpeakerphoneOn(
        /* [out] */ Boolean* result);

    /** @see AudioManager#setBluetoothScoOn() */
    CARAPI SetBluetoothScoOn(
        /* [in] */ Boolean on);

    /** @see AudioManager#isBluetoothScoOn() */
    CARAPI IsBluetoothScoOn(
        /* [out] */ Boolean* result);

    /** @see AudioManager#setBluetoothA2dpOn() */
    CARAPI SetBluetoothA2dpOn(
        /* [in] */ Boolean on);

    /** @see AudioManager#isBluetoothA2dpOn() */
    CARAPI IsBluetoothA2dpOn(
        /* [out] */ Boolean* result);

    /** @see AudioManager#startBluetoothSco() */
    CARAPI StartBluetoothSco(
        /* [in] */ IBinder* cb);

    /** @see AudioManager#stopBluetoothSco() */
    CARAPI StopBluetoothSco(
        /* [in] */ IBinder* cb);

    CARAPI ClearAllScoClients(
        /* [in] */ Int32 exceptPid,
        /* [in] */ Boolean stopSco);

    /** see AudioManager.setRemoteSubmixOn(boolean on) */
    CARAPI SetRemoteSubmixOn(
        /* [in] */ Boolean on,
        /* [in] */ Int32 address);

    CARAPI IsStreamAffectedByRingerMode(
        /* [in] */ Int32 streamType,
        /* [out] */ Boolean* result);

    CARAPI IsStreamAffectedByMute(
        /* [in] */ Int32 streamType,
        /* [out] */ Boolean* result);

    CARAPI SetWiredDeviceConnectionState(
        /* [in] */ Int32 device,
        /* [in] */ Int32 state,
        /* [in] */ const String& name);

    CARAPI SetBluetoothA2dpDeviceConnectionState(
        /* [in] */ IBluetoothDevice* device,
        /* [in] */ Int32 state,
        /* [out] */ Int32* result);

    /** @see AudioManager#requestAudioFocus(IAudioFocusDispatcher, int, int) */
    CARAPI RequestAudioFocus(
        /* [in] */ Int32 mainStreamType,
        /* [in] */ Int32 durationHint,
        /* [in] */ IBinder* cb,
        /* [in] */ IIAudioFocusDispatcher* fd,
        /* [in] */ const String& clientId,
        /* [in] */ const String& callingPackageName,
        /* [out] */ Int32* result);

    /** @see AudioManager#abandonAudioFocus(IAudioFocusDispatcher) */
    CARAPI AbandonAudioFocus(
        /* [in] */ IIAudioFocusDispatcher* fl,
        /* [in] */ const String& clientId,
        /* [out] */ Int32* result);

    CARAPI UnregisterAudioFocusClient(
        /* [in] */ const String& clientId);

    //==========================================================================================
    // RemoteControl
    //==========================================================================================
    CARAPI DispatchMediaKeyEvent(
        /* [in] */ IKeyEvent* keyEvent);

    CARAPI DispatchMediaKeyEventUnderWakelock(
        /* [in] */ IKeyEvent* keyEvent);

    CARAPI OnSendFinished(
        /* [in] */ IPendingIntent* pendingIntent,
        /* [in] */ IIntent* intent,
        /* [in] */ Int32 resultCode,
        /* [in] */ const String& resultData,
        /* [in] */ IBundle* resultExtras);

    /**
     * see AudioManager.registerMediaButtonIntent(PendingIntent pi, ComponentName c)
     * precondition: mediaIntent != null, target != null
     */
    CARAPI RegisterMediaButtonIntent(
        /* [in] */ IPendingIntent* mediaIntent,
        /* [in] */ IComponentName* eventReceiver);

    /**
     * see AudioManager.unregisterMediaButtonIntent(PendingIntent mediaIntent)
     * precondition: mediaIntent != null, eventReceiver != null
     */
    CARAPI UnregisterMediaButtonIntent(
        /* [in] */ IPendingIntent* mediaIntent,
        /* [in] */ IComponentName* eventReceiver);

    /**
     * see AudioManager.registerMediaButtonEventReceiverForCalls(ComponentName c)
     * precondition: c != null
     */
    CARAPI RegisterMediaButtonEventReceiverForCalls(
        /* [in] */ IComponentName* c);

    /**
     * see AudioManager.unregisterMediaButtonEventReceiverForCalls()
     */
    CARAPI UnregisterMediaButtonEventReceiverForCalls();

    /**
     * see AudioManager.registerRemoteControlClient(ComponentName eventReceiver, ...)
     * @return the unique ID of the RemoteControlStackEntry associated with the RemoteControlClient
     * Note: using this method with rcClient == null is a way to "disable" the IRemoteControlClient
     *     without modifying the RC stack, but while still causing the display to refresh (will
     *     become blank as a result of this)
     */
    CARAPI RegisterRemoteControlClient(
        /* [in] */ IPendingIntent* mediaIntent,
        /* [in] */ IIRemoteControlClient* rcClient,
        /* [in] */ const String& callingPackageName,
        /* [out] */ Int32* result);

     /**
      * see AudioManager.unregisterRemoteControlClient(PendingIntent pi, ...)
      * rcClient is guaranteed non-null
      */
    CARAPI UnregisterRemoteControlClient(
        /* [in] */ IPendingIntent* mediaIntent,
        /* [in] */ IIRemoteControlClient* rcClient);

    /**
     * Register an IRemoteControlDisplay.
     * Notify all IRemoteControlClient of the new display and cause the RemoteControlClient
     * at the top of the stack to update the new display with its information.
     * Since only one IRemoteControlDisplay is supported, this will unregister the previous display.
     * @param rcd the IRemoteControlDisplay to register. No effect if null.
     */
    CARAPI RegisterRemoteControlDisplay(
        /* [in] */ IIRemoteControlDisplay* rcd);

    /**
     * Unregister an IRemoteControlDisplay.
     * Since only one IRemoteControlDisplay is supported, this has no effect if the one to
     *    unregister is not the current one.
     * @param rcd the IRemoteControlDisplay to unregister. No effect if null.
     */
    CARAPI UnregisterRemoteControlDisplay(
        /* [in] */ IIRemoteControlDisplay* rcd);

    CARAPI RemoteControlDisplayUsesBitmapSize(
        /* [in] */ IIRemoteControlDisplay* rcd,
        /* [in] */ Int32 w,
        /* [in] */ Int32 h);

    CARAPI SetPlaybackInfoForRcc(
        /* [in] */ Int32 rccId,
        /* [in] */ Int32 what,
        /* [in] */ Int32 value);

    CARAPI RegisterRemoteVolumeObserverForRcc(
        /* [in] */ Int32 rccId,
        /* [in] */ IIRemoteVolumeObserver* rvo);

    CARAPI GetRemoteStreamMaxVolume(
        /* [out] */ Int32* result);

    CARAPI GetRemoteStreamVolume(
        /* [out]*/ Int32* result);

    CARAPI SetRemoteStreamVolume(
        /* [in] */ Int32 vol);

    // Handles request to override default use of A2DP for media.
    CARAPI SetBluetoothA2dpOnInt(
        /* [in] */ Boolean on);

     //@Override
    CARAPI SetRingtonePlayer(
        /* [in] */ IIRingtonePlayer* player);

    //@Override
    CARAPI GetRingtonePlayer(
        /* [out] */ IIRingtonePlayer** result);

    //@Override
    CARAPI StartWatchingRoutes(
        /* [in] */ IIAudioRoutesObserver* observer,
        /* [out] */ IAudioRoutesInfo** result);

    CARAPI DisableSafeMediaVolume();

    // called by android.hardware.Camera to populate CameraInfo.canDisableShutterSound
    CARAPI IsCameraSoundForced(
        /* [out] */ Boolean* result);

    /**
     * @see #setVibrateSetting(int, int)
     */
    static CARAPI GetValueForVibrateSetting(
        /* [in] */ Int32 existingValue,
        /* [in] */ Int32 vibrateType,
        /* [in] */ Int32 vibrateSetting,
        /* [out] */ Int32* result);

    //CARAPI_(void) Wait();

    //CARAPI_(void) NotifyAll();

protected:
    //@Override
    CARAPI ToString(
        /* [out] */ String* result);
    // CARAPI_(void) Dump(
    //     /* [in] */ IFileDescriptor* fd,
    //     /* [in] */ IPrintWriter* pw,
    //     /* [in] */ ArrayOf<String>* args);

private:
    CARAPI_(void) CreateAudioSystemThread();

    /** Waits for the volume handler to be created by the other thread. */
    CARAPI_(void) WaitForAudioHandlerCreation();

    CARAPI_(void) CheckAllAliasStreamVolumes();

    CARAPI_(void) CreateStreamStates();

    CARAPI_(String) DumpStreamStates();

    CARAPI_(void) UpdateStreamVolumeAlias(
        /* [in] */ Boolean updateVolumes);

    CARAPI_(void) ReadDockAudioSettings(
        /* [in] */ IContentResolver* cr);

    CARAPI_(void) ReadPersistedSettings();

    CARAPI_(Int32) RescaleIndex(
        /* [in] */ Int32 index,
        /* [in] */ Int32 srcStream,
        /* [in] */ Int32 dstStream);

    CARAPI_(Int32) FindVolumeDelta(
        /* [in] */ Int32 direction,
        /* [in] */ Int32 volume);

    CARAPI_(void) SendBroadcastToAll(
        /* [in] */ IIntent* intent);

    CARAPI_(void) SendStickyBroadcastToAll(
        /* [in] */ IIntent* intent);

    // UI update and Broadcast Intent
    CARAPI_(void) SendVolumeUpdate(
        /* [in] */ Int32 streamType,
        /* [in] */ Int32 oldIndex,
        /* [in] */ Int32 index,
        /* [in] */ Int32 flags);

    // UI update and Broadcast Intent
    CARAPI_(void) SendMasterVolumeUpdate(
        /* [in] */ Int32 flags,
        /* [in] */ Int32 oldVolume,
        /* [in] */ Int32 newVolume);

    // UI update and Broadcast Intent
    CARAPI_(void) SendMasterMuteUpdate(
        /* [in] */ Boolean muted,
        /* [in] */ Int32 flags);

    CARAPI_(void) BroadcastMasterMuteStatus(
        /* [in] */ Boolean muted);

    /**
     * Sets the stream state's index, and posts a message to set system volume.
     * This will not call out to the UI. Assumes a valid stream type.
     *
     * @param streamType Type of the stream
     * @param index Desired volume index of the stream
     * @param force If true, set the volume even if the desired volume is same
     * as the current volume.
     * @param lastAudible If true, stores new index as last audible one
     */
    CARAPI_(void) SetStreamVolumeInt(
        /* [in] */ Int32 streamType,
        /* [in] */ Int32 index,
        /* [in] */ Int32 device,
        /* [in] */ Boolean force,
        /* [in] */ Boolean lastAudible);

    CARAPI_(void) DoSetMasterVolume(
        /* [in] */ Float volume,
        /* [in] */ Int32 flags);

    CARAPI EnsureValidRingerMode(
        /* [in] */ Int32 ringerMode);

    CARAPI_(void) SetRingerModeInt(
        /* [in] */ Int32 ringerMode,
        /* [in] */ Boolean persist);

    CARAPI_(void) RestoreMasterVolume();

    // must be called synchronized on mSetModeDeathHandlers
    // setModeInt() returns a valid PID if the audio mode was successfully set to
    // any mode other than NORMAL.
    CARAPI_(Int32) SetModeInt(
        /* [in] */ Int32 mode,
        /* [in] */ IBinder* cb,
        /* [in] */ Int32 pid);

    CARAPI_(void) ReadAudioSettings(
        /* [in] */ Boolean userSwitch);

    CARAPI_(void) CheckScoAudioState();

    CARAPI_(AutoPtr<ScoClient>) GetScoClient(
        /* [in] */ IBinder* cb,
        /* [in] */ Boolean create);

    CARAPI_(Boolean) GetBluetoothHeadset();

    CARAPI_(void) DisconnectBluetoothSco(
        /* [in] */ Int32 exceptPid);

    CARAPI_(void) ResetBluetoothSco();

    CARAPI_(void) BroadcastScoConnectionState(
        /* [in] */ Int32 state);

    CARAPI_(void) OnSetRsxConnectionState(
        /* [in] */ Int32 available,
        /* [in] */ Int32 address);

    CARAPI_(void) OnCheckMusicActive();

    CARAPI_(void) OnConfigureSafeVolume(
        /* [in] */ Boolean force);

    ///////////////////////////////////////////////////////////////////////////
    // Internal methods
    ///////////////////////////////////////////////////////////////////////////
    /**
     * Checks if the adjustment should change ringer mode instead of just
     * adjusting volume. If so, this will set the proper ringer mode and volume
     * indices on the stream states.
     */
    CARAPI_(Boolean) CheckForRingerModeChange(
        /* [in] */ Int32 oldIndex,
        /* [in] */ Int32 direction,
        /* [in] */ Int32 step);

    CARAPI_(Boolean) IsStreamMutedByRingerMode(
        /* [in] */ Int32 streamType);

    CARAPI EnsureValidDirection(
        /* [in] */ Int32 direction);

    CARAPI EnsureValidSteps(
        /* [in] */ Int32 steps);

    CARAPI EnsureValidStreamType(
        /* [in] */ Int32 streamType);

    CARAPI_(Boolean) IsInCommunication();

    CARAPI_(Int32) GetActiveStreamType(
        /* [in] */ Int32 suggestedStreamType);

    CARAPI_(void) BroadcastRingerMode(
        /* [in] */ Int32 ringerMode);

    CARAPI_(void) BroadcastVibrateSetting(
        /* [in] */ Int32 vibrateType);

    // Message helper methods
    /**
     * Queue a message on the given handler's message queue, after acquiring the service wake lock.
     * Note that the wake lock needs to be released after the message has been handled.
     */
    CARAPI_(void) QueueMsgUnderWakeLock(
        /* [in] */ Int32 msg,
        /* [in] */ Int32 arg1,
        /* [in] */ Int32 arg2,
        /* [in] */ IInterface* obj,
        /* [in] */ Int32 delay);

    CARAPI SendMsg(
        /* [in] */ Int32 msg,
        /* [in] */ Int32 existingMsgPolicy,
        /* [in] */ Int32 arg1,
        /* [in] */ Int32 arg2,
        /* [in] */ IInterface* obj,
        /* [in] */ Int32 delay);

    /* package */ CARAPI_(Boolean) CheckAudioSettingsPermission(
        /* [in] */ const String& method);

    CARAPI_(Int32) GetDeviceForStream(
        /* [in] */ Int32 stream);

    // must be called synchronized on mConnectedDevices
    CARAPI_(void) MakeA2dpDeviceAvailable(
        /* [in] */ const String& address);

    CARAPI_(void) OnSendBecomingNoisyIntent();

    // must be called synchronized on mConnectedDevices
    CARAPI_(void) MakeA2dpDeviceUnavailableNow(
        /* [in] */ const String& address);

    // must be called synchronized on mConnectedDevices
    CARAPI_(void) MakeA2dpDeviceUnavailableLater(
        /* [in] */ const String& address);

    // must be called synchronized on mConnectedDevices
    CARAPI_(void) CancelA2dpDeviceTimeout();

    // must be called synchronized on mConnectedDevices
    CARAPI_(Boolean) HasScheduledA2dpDockTimeout();

    CARAPI_(void) OnSetA2dpConnectionState(
        /* [in] */ IBluetoothDevice* btDevice,
        /* [in] */ Int32 state);

    CARAPI_(Boolean) HandleDeviceConnection(
        /* [in] */ Boolean connected,
        /* [in] */ Int32 device,
        /* [in] */ const String& params);

    CARAPI_(Int32) CheckSendBecomingNoisyIntent(
        /* [in] */ Int32 device,
        /* [in] */ Int32 state);

    CARAPI_(void) SendDeviceConnectionIntent(
        /* [in] */ Int32 device,
        /* [in] */ Int32 state,
        /* [in] */ const String& name);

    CARAPI_(void) OnSetWiredDeviceConnectionState(
        /* [in] */ Int32 device,
        /* [in] */ Int32 state,
        /* [in] */ const String& name);

    /**
     * Discard the current audio focus owner.
     * Notify top of audio focus stack that it lost focus (regardless of possibility to reassign
     * focus), remove it from the stack, and clear the remote control display.
     */
    CARAPI_(void) DiscardAudioFocusOwner();

    CARAPI_(void) NotifyTopOfAudioFocusStack();

    /**
     * Helper function:
     * Display in the log the current entries in the audio focus stack
     */
    CARAPI_(String) DumpFocusStack();

    /**
     * Helper function:
     * Remove a focus listener from the focus stack.
     * @param focusListenerToRemove the focus listener
     * @param signal if TRUE and the listener was at the top of the focus stack, i.e. it was holding
     *   focus, notify the next item in the stack it gained focus.
     */
    CARAPI_(void) RemoveFocusStackEntry(
        /* [in] */ const String& clientToRemove,
        /* [in] */ Boolean signal);

    /**
     * Helper function:
     * Called synchronized on mAudioFocusLock
     * Remove focus listeners from the focus stack for a particular client when it has died.
     */
    CARAPI_(void) RemoveFocusStackEntryForClient(
        /* [in] */ IBinder* cb);

    /**
     * Helper function:
     * Returns TRUE if the system is in a state where the focus can be reevaluated, false otherwise.
     */
    CARAPI_(Boolean) CanReassignAudioFocus();

    CARAPI_(void) FilterMediaKeyEvent(
        /* [in] */ IKeyEvent* keyEvent,
        /* [in] */ Boolean needWakeLock);

    /**
     * Handles the dispatching of the media button events to the telephony package.
     * Precondition: mMediaReceiverForCalls != null
     * @param keyEvent a non-null KeyEvent whose key code is one of the supported media buttons
     * @param needWakeLock true if a PARTIAL_WAKE_LOCK needs to be held while this key event
     *     is dispatched.
     */
    CARAPI_(void) DispatchMediaKeyEventForCalls(
        /* [in] */ IKeyEvent* keyEvent,
        /* [in] */ Boolean needWakeLock);

    /**
     * Handles the dispatching of the media button events to one of the registered listeners,
     * or if there was none, broadcast an ACTION_MEDIA_BUTTON intent to the rest of the system.
     * @param keyEvent a non-null KeyEvent whose key code is one of the supported media buttons
     * @param needWakeLock true if a PARTIAL_WAKE_LOCK needs to be held while this key event
     *     is dispatched.
     */
    CARAPI_(void) DispatchMediaKeyEvent(
        /* [in] */ IKeyEvent* keyEvent,
        /* [in] */ Boolean needWakeLock);

    /**
     * Filter key events that may be used for voice-based interactions
     * @param keyEvent a non-null KeyEvent whose key code is that of one of the supported
     *    media buttons that can be used to trigger voice-based interactions.
     * @param needWakeLock true if a PARTIAL_WAKE_LOCK needs to be held while this key event
     *     is dispatched.
     */
    CARAPI_(void) FilterVoiceInputKeyEvent(
        /* [in] */ IKeyEvent* keyEvent,
        /* [in] */ Boolean needWakeLock);

    CARAPI_(void) SendSimulatedMediaButtonEvent(
        /* [in] */ IKeyEvent* originalKeyEvent,
        /* [in] */ Boolean needWakeLock);

    /*static*/
    static CARAPI_(Boolean) IsValidMediaKeyEvent(
        /* [in] */ IKeyEvent* keyEvent);

    /**
     * Checks whether the given key code is one that can trigger the launch of voice-based
     *   interactions.
     * @param keyCode the key code associated with the key event
     * @return true if the key is one of the supported voice-based interaction triggers
     */
    /*static*/
    static CARAPI_(Boolean) IsValidVoiceInputKeyCode(
        /* [in] */ Int32 keyCode);

    /**
     * Tell the system to start voice-based interactions / voice commands
     */
    CARAPI_(void) StartVoiceBasedInteractions(
        /* [in] */ Boolean needWakeLock);

    /**
     * Helper function:
     * Display in the log the current entries in the remote control focus stack
     */
    CARAPI_(String) DumpRCStack();

    /**
     * Helper function:
     * Display in the log the current entries in the remote control stack, focusing
     * on RemoteControlClient data
     */
    CARAPI_(String) DumpRCCStack();

    /**
     * Helper function:
     * Remove any entry in the remote control stack that has the same package name as packageName
     * Pre-condition: packageName != null
     */
    CARAPI_(void) RemoveMediaButtonReceiverForPackage(
        /* [in] */ const String& packageName);

    /**
     * Helper function:
     * Restore remote control receiver from the system settings.
     */
    CARAPI_(void) RestoreMediaButtonReceiver();

    /**
     * Helper function:
     * Set the new remote control receiver at the top of the RC focus stack.
     * precondition: mediaIntent != null, target != null
     */
    CARAPI_(void) PushMediaButtonReceiver(
        /* [in] */ IPendingIntent* mediaIntent,
        /* [in] */ IComponentName* target);

    /**
     * Helper function:
     * Remove the remote control receiver from the RC focus stack.
     * precondition: pi != null
     */
    CARAPI_(void) RemoveMediaButtonReceiver(
        /* [in] */ IPendingIntent* pi);

    /**
     * Helper function:
     * Called synchronized on mRCStack
     */
    CARAPI_(Boolean) IsCurrentRcController(
        /* [in] */ IPendingIntent* pi);

    //==========================================================================================
    // Remote control display / client
    //==========================================================================================
    /**
     * Update the remote control displays with the new "focused" client generation
     */
    CARAPI_(void) SetNewRcClientOnDisplays_syncRcsCurrc(
        /* [in] */ Int32 newClientGeneration,
        /* [in] */ IPendingIntent* newMediaIntent,
        /* [in] */ Boolean clearing);

    /**
     * Update the remote control clients with the new "focused" client generation
     */
    CARAPI_(void) SetNewRcClientGenerationOnClients_syncRcsCurrc(
        /* [in]*/ Int32 newClientGeneration);

    /**
     * Update the displays and clients with the new "focused" client generation and name
     * @param newClientGeneration the new generation value matching a client update
     * @param newClientEventReceiver the media button event receiver associated with the client.
     *    May be null, which implies there is no registered media button event receiver.
     * @param clearing true if the new client generation value maps to a remote control update
     *    where the display should be cleared.
     */
    CARAPI_(void) SetNewRcClient_syncRcsCurrc(
        /* [in] */ Int32 newClientGeneration,
        /* [in] */ IPendingIntent* newMediaIntent,
        /* [in] */ Boolean clearing);

    /**
     * Called when processing MSG_RCDISPLAY_CLEAR event
     */
    CARAPI_(void) OnRcDisplayClear();

    /**
     * Called when processing MSG_RCDISPLAY_UPDATE event
     */
    CARAPI_(void) OnRcDisplayUpdate(
        /* [in] */ RemoteControlStackEntry* rcse,
        /* [in] */ Int32 flags /* USED ?*/);

    /**
     * Helper function:
     * Called synchronized on mRCStack
     */
    CARAPI_(void) ClearRemoteControlDisplay_syncAfRcs();

    /**
     * Helper function for code readability: only to be called from
     *    checkUpdateRemoteControlDisplay_syncAfRcs() which checks the preconditions for
     *    this method.
     * Preconditions:
     *    - called synchronized mAudioFocusLock then on mRCStack
     *    - mRCStack.isEmpty() is false
     */
    CARAPI_(void) UpdateRemoteControlDisplay_syncAfRcs(
        /* [in] */ Int32 infoChangedFlags);

    /**
     * Helper function:
     * Called synchronized on mAudioFocusLock, then mRCStack
     * Check whether the remote control display should be updated, triggers the update if required
     * @param infoChangedFlags the flags corresponding to the remote control client information
     *     that has changed, if applicable (checking for the update conditions might trigger a
     *     clear, rather than an update event).
     */
    CARAPI_(void) CheckUpdateRemoteControlDisplay_syncAfRcs(
        /* [in] */ Int32 infoChangedFlags);

    CARAPI_(void) RcDisplay_stopDeathMonitor_syncRcStack();

    CARAPI_(void) RcDisplay_startDeathMonitor_syncRcStack();

    // handler for MSG_RCC_NEW_PLAYBACK_INFO
    CARAPI_(void) OnNewPlaybackInfoForRcc(
        /* [in] */ Int32 rccId,
        /* [in] */ Int32 key,
        /* [in] */ Int32 value);

    // handler for MSG_RCC_NEW_VOLUME_OBS
    CARAPI_(void) OnRegisterVolumeObserverForRcc(
        /* [in] */ Int32 rccId,
        /* [in] */ IIRemoteVolumeObserver* rvo);

    /**
     * Checks if a remote client is active on the supplied stream type. Update the remote stream
     * volume state if found and playing
     * @param streamType
     * @return false if no remote playing is currently playing
     */
    CARAPI_(Boolean) CheckUpdateRemoteStateIfActive(
        /* [in] */ Int32 streamType);

    /**
     * Returns true if the given playback state is considered "active", i.e. it describes a state
     * where playback is happening, or about to
     * @param playState the playback state to evaluate
     * @return true if active, false otherwise (inactive or unknown)
     */
    /*static*/
    static CARAPI_(Boolean) IsPlaystateActive(
        /* [in] */ Int32 playState);

    CARAPI_(void) AdjustRemoteVolume(
        /* [in] */ Int32 streamType,
        /* [in] */ Int32 direction,
        /* [in] */ Int32 flags);

    CARAPI_(void) SendVolumeUpdateToRemote(
        /* [in] */ Int32 rccId,
        /* [in] */ Int32 direction);

    /**
     * Call to make AudioService reevaluate whether it's in a mode where remote players should
     * have their volume controlled. In this implementation this is only to reset whether
     * VolumePanel should display remote volumes
     */
    CARAPI_(void) PostReevaluateRemote();

    CARAPI_(void) OnReevaluateRemote();


    //==========================================================================================
    // Device orientation
    //==========================================================================================
    /**
     * Handles device configuration changes that may map to a change in the orientation.
     * This feature is optional, and is defined by the definition and value of the
     * "ro.audio.monitorOrientation" system property.
     */
    CARAPI_(void) HandleConfigurationChanged(
        /* [in] */ IContext* context);

    CARAPI_(void) HandleBootCompleted(
        /* [in] */ IContext* context);

    CARAPI_(void) SetOrientationForAudioSystem();

    CARAPI_(void) SetSafeMediaVolumeEnabled(
        /* [in] */ Boolean on);

    CARAPI_(void) EnforceSafeMediaVolume();

    CARAPI_(Boolean) CheckSafeMediaVolume(
        /* [in] */ Int32 streamType,
        /* [in] */ Int32 index,
        /* [in] */ Int32 device);

    CARAPI_(String) DumpRingerMode();

protected:
    /** Debug remote control client/display feature */
    static const Boolean DEBUG_RC;

    /** Debug volumes */
    static const Boolean DEBUG_VOL;

private:
    static const String TAG; // = "AudioService";

    /** How long to delay before persisting a change in volume/ringer mode. */
    static const Int32 PERSIST_DELAY;// = 500;

    AutoPtr<IContext> mContext;
    AutoPtr<IContentResolver> mContentResolver;
    Boolean mVoiceCapable;

    /** The UI */
    AutoPtr<IVolumePanel> mVolumePanel;

    // sendMsg() flags
    /** If the msg is already queued, replace it with this one. */
    static const Int32 SENDMSG_REPLACE;// = 0;

    /** If the msg is already queued, ignore this one and leave the old. */
    static const Int32 SENDMSG_NOOP;// = 1;

    /** If the msg is already queued, queue this one and leave the old. */
    static const Int32 SENDMSG_QUEUE;// = 2;

    // AudioHandler message
    static const Int32 MSG_SET_DEVICE_VOLUME;// = 0;
    static const Int32 MSG_PERSIST_VOLUME;// = 1;
    static const Int32 MSG_PERSIST_MASTER_VOLUME;// = 2;
    static const Int32 MSG_PERSIST_RINGER_MODE;// = 3;
    static const Int32 MSG_MEDIA_SERVER_DIED;// = 4;
    static const Int32 MSG_MEDIA_SERVER_STARTED;// = 5;
    static const Int32 MSG_PLAY_SOUND_EFFECT;// = 6;
    static const Int32 MSG_BTA2DP_DOCK_TIMEOUT;// = 7;
    static const Int32 MSG_LOAD_SOUND_EFFECTS;// = 8;
    static const Int32 MSG_SET_FORCE_USE;// = 9;
    static const Int32 MSG_PERSIST_MEDIABUTTONRECEIVER;// = 10;
    static const Int32 MSG_BT_HEADSET_CNCT_FAILED;// = 11;
    static const Int32 MSG_RCDISPLAY_CLEAR;// = 12;
    static const Int32 MSG_RCDISPLAY_UPDATE;// = 13;
    static const Int32 MSG_SET_ALL_VOLUMES;// = 14;
    static const Int32 MSG_PERSIST_MASTER_VOLUME_MUTE;// = 15;
    static const Int32 MSG_REPORT_NEW_ROUTES;// = 16;
    static const Int32 MSG_REEVALUATE_REMOTE;// = 17;
    static const Int32 MSG_RCC_NEW_PLAYBACK_INFO;// = 18;
    static const Int32 MSG_RCC_NEW_VOLUME_OBS;// = 19;
    static const Int32 MSG_SET_FORCE_BT_A2DP_USE;// = 20;

    // start of messages handled under wakelock
    //   these messages can only be queued, i.e. sent with queueMsgUnderWakeLock(),
    //   and not with sendMsg(..., ..., SENDMSG_QUEUE, ...)
    static const Int32 MSG_SET_WIRED_DEVICE_CONNECTION_STATE;// = 21;
    static const Int32 MSG_SET_A2DP_CONNECTION_STATE;// = 22;

    // end of messages handled under wakelock
    static const Int32 MSG_SET_RSX_CONNECTION_STATE;// = 23; // change remote submix connection
    static const Int32 MSG_CHECK_MUSIC_ACTIVE;// = 24;
    static const Int32 MSG_BROADCAST_AUDIO_BECOMING_NOISY;// = 25;
    static const Int32 MSG_CONFIGURE_SAFE_MEDIA_VOLUME;// = 26;
    static const Int32 MSG_CONFIGURE_SAFE_MEDIA_VOLUME_FORCED;// = 27;

    // flags for MSG_PERSIST_VOLUME indicating if current and/or last audible volume should be
    // persisted
    static const Int32 PERSIST_CURRENT;// = 0x1;
    static const Int32 PERSIST_LAST_AUDIBLE;// = 0x2;

    static const Int32 BTA2DP_DOCK_TIMEOUT_MILLIS;// = 8000;
    // Timeout for connection to bluetooth headset service
    static const Int32 BT_HEADSET_CNCT_TIMEOUT_MS;// = 3000;

    /** @see AudioSystemThread */
    AutoPtr<AudioSystemThread> mAudioSystemThread;

    /** @see AudioHandler */
    AutoPtr<IHandler> mAudioHandler;
    Object mThisLock;

    /** @see VolumeStreamState */
    AutoPtr< ArrayOf<VolumeStreamState*> > mStreamStates;
    Object mStreamStatesLock;

    AutoPtr<SettingsObserver> mSettingsObserver;

    Int32 mMode;

    // protects mRingerMode
    Object mSettingsLock;

    Boolean mMediaServerOk;

    AutoPtr<ISoundPool> mSoundPool;
    Object mSoundEffectsLock;

    static const Int32 NUM_SOUNDPOOL_CHANNELS;// = 4;

    // Internally master volume is a float in the 0.0 - 1.0 range,
    // but to support integer based AudioManager API we translate it to 0 - 100
    static const Int32 MAX_MASTER_VOLUME;// = 100;

    // Maximum volume adjust steps allowed in a single batch call.
    static const Int32 MAX_BATCH_VOLUME_ADJUST_STEPS;// = 4;

    /* Sound effect file names  */
    static const String SOUND_EFFECTS_PATH; // = "/media/audio/ui/";

//    static const AutoPtr< ArrayOf<String> > SOUND_EFFECT_FILES;
    static const String SOUND_EFFECT_FILES[];

    /* Sound effect file name mapping sound effect id (AudioManager.FX_xxx) to
     * file index in SOUND_EFFECT_FILES[] (first column) and indicating if effect
     * uses soundpool (second column) */
//    AutoPtr< ArrayOf< AutoPtr< ArrayOf<Int32> > > > SOUND_EFFECT_FILES_MAP;
    Int32 SOUND_EFFECT_FILES_MAP[9][2];
    // = {0, -1},  // FX_KEY_CLICK
    //   {0, -1},  // FX_FOCUS_NAVIGATION_UP
    //   {0, -1},  // FX_FOCUS_NAVIGATION_DOWN
    //   {0, -1},  // FX_FOCUS_NAVIGATION_LEFT
    //   {0, -1},  // FX_FOCUS_NAVIGATION_RIGHT
    //   {1, -1},  // FX_KEYPRESS_STANDARD
    //   {2, -1},  // FX_KEYPRESS_SPACEBAR
    //   {3, -1},  // FX_FOCUS_DELETE
    //   {4, -1}   // FX_FOCUS_RETURN

    /** @hide Maximum volume index values for audio streams */
//    AutoPtr< ArrayOf<Int32> > MAX_STREAM_VOLUME;
    Int32 MAX_STREAM_VOLUME[10];
    // = 5,  // STREAM_VOICE_CALL
    //   7,  // STREAM_SYSTEM
    //   7,  // STREAM_RING
    //   15, // STREAM_MUSIC
    //   7,  // STREAM_ALARM
    //   7,  // STREAM_NOTIFICATION
    //   15, // STREAM_BLUETOOTH_SCO
    //   7,  // STREAM_SYSTEM_ENFORCED
    //   15, // STREAM_DTMF
    //   15  // STREAM_TTS

    /* mStreamVolumeAlias[] indicates for each stream if it uses the volume settings
     * of another stream: This avoids multiplying the volume settings for hidden
     * stream types that follow other stream behavior for volume settings
     * NOTE: do not create loops in aliases!
     * Some streams alias to different streams according to device category (phone or tablet) or
     * use case (in call s off call...).See updateStreamVolumeAlias() for more details
     *  mStreamVolumeAlias contains the default aliases for a voice capable device (phone) and
     *  STREAM_VOLUME_ALIAS_NON_VOICE for a non voice capable device (tablet).*/
//    AutoPtr< ArrayOf<Int32> > STREAM_VOLUME_ALIAS;
    Int32 STREAM_VOLUME_ALIAS[10];
    // = AudioSystem.STREAM_VOICE_CALL,      // STREAM_VOICE_CALL
    //   AudioSystem.STREAM_RING,            // STREAM_SYSTEM
    //   AudioSystem.STREAM_RING,            // STREAM_RING
    //   AudioSystem.STREAM_MUSIC,           // STREAM_MUSIC
    //   AudioSystem.STREAM_ALARM,           // STREAM_ALARM
    //   AudioSystem.STREAM_RING,            // STREAM_NOTIFICATION
    //   AudioSystem.STREAM_BLUETOOTH_SCO,   // STREAM_BLUETOOTH_SCO
    //   AudioSystem.STREAM_RING,            // STREAM_SYSTEM_ENFORCED
    //   AudioSystem.STREAM_RING,            // STREAM_DTMF
    //   AudioSystem.STREAM_MUSIC            // STREAM_TTS

//    AutoPtr< ArrayOf<Int32> > STREAM_VOLUME_ALIAS_NON_VOICE;
    Int32 STREAM_VOLUME_ALIAS_NON_VOICE[10];
    // = AudioSystem.STREAM_VOICE_CALL,      // STREAM_VOICE_CALL
    //    AudioSystem.STREAM_MUSIC,           // STREAM_SYSTEM
    //    AudioSystem.STREAM_RING,            // STREAM_RING
    //    AudioSystem.STREAM_MUSIC,           // STREAM_MUSIC
    //    AudioSystem.STREAM_ALARM,           // STREAM_ALARM
    //    AudioSystem.STREAM_RING,            // STREAM_NOTIFICATION
    //    AudioSystem.STREAM_BLUETOOTH_SCO,   // STREAM_BLUETOOTH_SCO
    //    AudioSystem.STREAM_MUSIC,           // STREAM_SYSTEM_ENFORCED
    //    AudioSystem.STREAM_MUSIC,           // STREAM_DTMF
    //    AudioSystem.STREAM_MUSIC            // STREAM_TTS

    AutoPtr< ArrayOf<Int32> > mStreamVolumeAlias;

    // stream names used by dumpStreamStates()
//    AutoPtr< ArrayOf<String> > STREAM_NAMES;
    String STREAM_NAMES[10];
    // = "STREAM_VOICE_CALL",
    //   "STREAM_SYSTEM",
    //   "STREAM_RING",
    //   "STREAM_MUSIC",
    //   "STREAM_ALARM",
    //   "STREAM_NOTIFICATION",
    //   "STREAM_BLUETOOTH_SCO",
    //   "STREAM_SYSTEM_ENFORCED",
    //   "STREAM_DTMF",
    //   "STREAM_TTS"

    AutoPtr<IAudioSystemErrorCallback> mAudioSystemCallback; // = new AudioSystem.ErrorCallback();

    /**
     * Current ringer mode from one of {@link AudioManager#RINGER_MODE_NORMAL},
     * {@link AudioManager#RINGER_MODE_SILENT}, or
     * {@link AudioManager#RINGER_MODE_VIBRATE}.
     */
    // protected by mSettingsLock
    Int32 mRingerMode;

    /** @see System#MODE_RINGER_STREAMS_AFFECTED */
    Int32 mRingerModeAffectedStreams;

    // Streams currently muted by ringer mode
    Int32 mRingerModeMutedStreams;

    /** @see System#MUTE_STREAMS_AFFECTED */
    Int32 mMuteAffectedStreams;

    /**
     * NOTE: setVibrateSetting(), getVibrateSetting(), shouldVibrate() are deprecated.
     * mVibrateSetting is just maintained during deprecation period but vibration policy is
     * now only controlled by mHasVibrator and mRingerMode
     */
    Int32 mVibrateSetting;

    // Is there a vibrator
    Boolean mHasVibrator;

    // Broadcast receiver for device connections intent broadcasts
    AutoPtr<IBroadcastReceiver> mReceiver; // = new AudioServiceBroadcastReceiver();

    // Used to alter media button redirection when the phone is ringing.
    Boolean mIsRinging; // = FALSE;

    // Devices currently connected
    HashMap<Int32, String> mConnectedDevices; // = new HashMap <Integer, String>();
    Object mConnectedDevicesLock;

    // Forced device usage for communications
    Int32 mForcedUseForComm;

    // True if we have master volume support
    Boolean mUseMasterVolume;

    AutoPtr< ArrayOf<Int32> > mMasterVolumeRamp;

    // List of binder death handlers for setMode() client processes.
    // The last process to have called setMode() is at the top of the list.
    List< AutoPtr<SetModeDeathHandler> > mSetModeDeathHandlers; // = new ArrayList <SetModeDeathHandler>();
    Object mSetModeDeathHandlersLock;

    // List of clients having issued a SCO start request
    List< AutoPtr<ScoClient> > mScoClients; // = new ArrayList <ScoClient>();
    Object mScoClientsLock;

    // BluetoothHeadset API to control SCO connection
    AutoPtr<IBluetoothHeadset> mBluetoothHeadset;


    // Bluetooth headset device
    AutoPtr<IBluetoothDevice> mBluetoothHeadsetDevice;

    // Indicate if SCO audio connection is currently active and if the initiator is
    // audio service (internal) or bluetooth headset (external)
    Int32 mScoAudioState;

    // SCO audio state is not active
    static const Int32 SCO_STATE_INACTIVE;// = 0;
    // SCO audio activation request waiting for headset service to connect
    static const Int32 SCO_STATE_ACTIVATE_REQ;// = 1;
    // SCO audio state is active or starting due to a local request to start a virtual call
    static const Int32 SCO_STATE_ACTIVE_INTERNAL;// = 3;
    // SCO audio deactivation request waiting for headset service to connect
    static const Int32 SCO_STATE_DEACTIVATE_REQ;// = 5;

    // SCO audio state is active due to an action in BT handsfree (either voice recognition or
    // in call audio)
    static const Int32 SCO_STATE_ACTIVE_EXTERNAL;// = 2;
    // Deactivation request for all SCO connections (initiated by audio mode change)
    // waiting for headset service to connect
    static const Int32 SCO_STATE_DEACTIVATE_EXT_REQ;// = 4;

    // Current connection state indicated by bluetooth headset
    Int32 mScoConnectionState;

    // true if boot sequence has been completed
    Boolean mBootCompleted;

    // listener for SoundPool sample load completion indication
    AutoPtr<SoundPoolCallback> mSoundPoolCallBack;

    // thread for SoundPool listener
    AutoPtr<SoundPoolListenerThread> mSoundPoolListenerThread;

    // message looper for SoundPool listener
    AutoPtr<ILooper> mSoundPoolLooper; // = null;

    // volume applied to sound played with playSoundEffect()
    static Int32 sSoundEffectVolumeDb;

    // getActiveStreamType() will return:
    // - STREAM_NOTIFICATION on tablets during this period after a notification stopped
    // - STREAM_MUSIC on phones during this period after music or talkback/voice search prompt
    // stopped
    static const Int32 DEFAULT_STREAM_TYPE_OVERRIDE_DELAY_MS;// = 5000;

    // previous volume adjustment direction received by checkForRingerModeChange()
    Int32 mPrevVolDirection; // = AudioManager.ADJUST_SAME;

    // Keyguard manager proxy
    AutoPtr<IKeyguardManager> mKeyguardManager;

    // mVolumeControlStream is set by VolumePanel to temporarily force the stream type which volume
    // is controlled by Vol keys.
    Int32 mVolumeControlStream; // = -1;
    Object mForceControlStreamLock;

    // VolumePanel is currently the only client of forceVolumeControlStream() and runs in system
    // server process so in theory it is not necessary to monitor the client death.
    // However it is good to be ready for future evolutions.
    AutoPtr<ForceControlStreamClient> mForceControlStreamClient; // = null;

    // Used to play ringtones outside system_server
    AutoPtr<IIRingtonePlayer> mRingtonePlayer; // private volatile IRingtonePlayer mRingtonePlayer;

    Int32 mDeviceOrientation; // = Configuration.ORIENTATION_UNDEFINED;

    // Request to override default use of A2DP for media.
    Boolean mBluetoothA2dpEnabled;
    Object mBluetoothA2dpEnabledLock;

    // Monitoring of audio routes.  Protected by mCurAudioRoutes.
    AutoPtr<IAudioRoutesInfo> mCurAudioRoutes ; // = new AudioRoutesInfo();
    Object mCurAudioRoutesLock;

    AutoPtr<IRemoteCallbackList> mRoutesObservers ; // = new RemoteCallbackList<IAudioRoutesObserver>();

    // Devices for which the volume is fixed and VolumePanel slider should be disabled
    Int32 mFixedVolumeDevices;
    // = AudioSystem.DEVICE_OUT_AUX_DIGITAL |
    //   AudioSystem.DEVICE_OUT_DGTL_DOCK_HEADSET |
    //   AudioSystem.DEVICE_OUT_ANLG_DOCK_HEADSET |
    //   AudioSystem.DEVICE_OUT_ALL_USB;

    Boolean mMonitorOrientation;

    Boolean mDockAudioMediaEnabled; // = true;

    Int32 mDockState; // Intent.EXTRA_DOCK_STATE_UNDOCKED;

    AutoPtr<IBluetoothProfileServiceListener> mBluetoothProfileServiceListener; // = new BluetoothProfile.ServiceListener();

    // Devices which removal triggers intent ACTION_AUDIO_BECOMING_NOISY. The intent is only
    // sent if none of these devices is connected.
    Int32 mBecomingNoisyIntentDevices;
    // = AudioSystem.DEVICE_OUT_WIRED_HEADSET | AudioSystem.DEVICE_OUT_WIRED_HEADPHONE |
    //   AudioSystem.DEVICE_OUT_ALL_A2DP | AudioSystem.DEVICE_OUT_AUX_DIGITAL |
    //   AudioSystem.DEVICE_OUT_ANLG_DOCK_HEADSET | AudioSystem.DEVICE_OUT_DGTL_DOCK_HEADSET |
    //   AudioSystem.DEVICE_OUT_ALL_USB;

    /* cache of the address of the last dock the device was connected to */
    String mDockAddress;

    //==========================================================================================
    // AudioFocus
    //==========================================================================================
    /* constant to identify focus stack entry that is used to hold the focus while the phone
     * is ringing or during a call. Used by com.android.internal.telephony.CallManager when
     * entering and exiting calls.
     */
    static Object mAudioFocusLock;

    static Object mRingingLock;

    //AutoPtr<IPhoneStateListener> mPhoneStateListener; // = new PhoneStateListener()

    //private final Stack<FocusStackEntry> mFocusStack = new Stack<FocusStackEntry>();

    /**
     * The different actions performed in response to a voice button key event.
     */
    static const Int32 VOICEBUTTON_ACTION_DISCARD_CURRENT_KEY_PRESS;// = 1;
    static const Int32 VOICEBUTTON_ACTION_START_VOICE_INPUT;// = 2;
    static const Int32 VOICEBUTTON_ACTION_SIMULATE_KEY_PRESS;// = 3;

    Object mVoiceEventLock ;
    Boolean mVoiceButtonDown;
    Boolean mVoiceButtonHandled;

    AutoPtr<IPowerManagerWakeLock> mMediaEventWakeLock;

    static const Int32 WAKELOCK_RELEASE_ON_FINISHED = 1980; //magic number

    // only set when wakelock was acquired, no need to check value when received
    static const String EXTRA_WAKELOCK_ACQUIRED; // = "android.media.AudioService.WAKELOCK_ACQUIRED";

    AutoPtr<IBroadcastReceiver> mKeyEventDone; // = new BroadcastReceiver();

    Object mCurrentRcLock;

    /**
     * The one remote control client which will receive a request for display information.
     * This object may be null.
     * Access protected by mCurrentRcLock.
     */
    AutoPtr<IIRemoteControlClient> mCurrentRcClient; // = null;

    static const Int32 RC_INFO_NONE = 0;
    static const Int32 RC_INFO_ALL = 15;
    // = RemoteControlClient.FLAG_INFORMATION_REQUEST_ALBUM_ART |
    //   RemoteControlClient.FLAG_INFORMATION_REQUEST_KEY_MEDIA |
    //   RemoteControlClient.FLAG_INFORMATION_REQUEST_METADATA |
    //   RemoteControlClient.FLAG_INFORMATION_REQUEST_PLAYSTATE;

    /**
     * A monotonically increasing generation counter for mCurrentRcClient.
     * Only accessed with a lock on mCurrentRcLock.
     * No value wrap-around issues as we only act on equal values.
     */
    Int32 mCurrentRcClientGen; // = 0;

    /**
     * A global counter for RemoteControlClient identifiers
     */
    static Int32 sLastRccId; // = 0;

    /**
     * Internal cache for the playback information of the RemoteControlClient whose volume gets to
     * be controlled by the volume keys ("main"), so we don't have to iterate over the RC stack
     * every time we need this info.
     */
    AutoPtr<RemotePlaybackState> mMainRemote;
    Object mMainRemoteLock;

    /**
     * Indicates whether the "main" RemoteControlClient is considered active.
     * Use synchronized on mMainRemote.
     */
    Boolean mMainRemoteIsActive;

    /**
     * Indicates whether there is remote playback going on. True even if there is no "active"
     * remote playback (mMainRemoteIsActive is false), but a RemoteControlClient has declared it
     * handles remote playback.
     * Use synchronized on mMainRemote.
     */
    Boolean mHasRemotePlayback;

    /**
     *  The stack of remote control event receivers.
     *  Code sections and methods that modify the remote control event receiver stack are
     *  synchronized on mRCStack, but also BEFORE on mFocusLock as any change in either
     *  stack, audio focus or RC, can lead to a change in the remote control display
     */
//    Stack< AutoPtr<RemoteControlStackEntry> > mRCStack; // = new Stack<RemoteControlStackEntry>();
    Object mRCStackLock;

    /**
     * The component the telephony package can register so telephony calls have priority to
     * handle media button events
     */
    AutoPtr<IComponentName> mMediaReceiverForCalls; // = null;

    /**
     * The remote control displays.
     * Access synchronized on mRCStack
     * NOTE: Only one IRemoteControlDisplay supported in this implementation
     */
    AutoPtr<IIRemoteControlDisplay> mRcDisplay;
    AutoPtr<RcDisplayDeathHandler> mRcDisplayDeathHandler;
    Int32 mArtworkExpectedWidth; // = -1;
    Int32 mArtworkExpectedHeight; // = -1;

    //==========================================================================================
    // Safe media volume management.
    // MUSIC stream volume level is limited when headphones are connected according to safety
    // regulation. When the user attempts to raise the volume above the limit, a warning is
    // displayed and the user has to acknowlegde before the volume is actually changed.
    // The volume index corresponding to the limit is stored in config_safe_media_volume_index
    // property. Platforms with a different limit must set this property accordingly in their
    // overlay.
    //==========================================================================================

    // mSafeMediaVolumeState indicates whether the media volume is limited over headphones.
    // It is SAFE_MEDIA_VOLUME_NOT_CONFIGURED at boot time until a network service is connected
    // or the configure time is elapsed. It is then set to SAFE_MEDIA_VOLUME_ACTIVE or
    // SAFE_MEDIA_VOLUME_DISABLED according to country option. If not SAFE_MEDIA_VOLUME_DISABLED, it
    // can be set to SAFE_MEDIA_VOLUME_INACTIVE by calling AudioService.disableSafeMediaVolume()
    // (when user opts out).
    const Int32 SAFE_MEDIA_VOLUME_NOT_CONFIGURED; // = 0;
    const Int32 SAFE_MEDIA_VOLUME_DISABLED; // = 1;
    const Int32 SAFE_MEDIA_VOLUME_INACTIVE; // = 2;
    const Int32 SAFE_MEDIA_VOLUME_ACTIVE; // = 3;

    Int32 mSafeMediaVolumeState;
    Object mSafeMediaVolumeStateLock;

    Int32 mMcc; // = 0;

    // mSafeMediaVolumeIndex is the cached value of config_safe_media_volume_index property
    Int32 mSafeMediaVolumeIndex;

    // mSafeMediaVolumeDevices lists the devices for which safe media volume is enforced,
    Int32 mSafeMediaVolumeDevices;
    // = AudioSystem.DEVICE_OUT_WIRED_HEADSET |
    //   AudioSystem.DEVICE_OUT_WIRED_HEADPHONE;

    // mMusicActiveMs is the cumulative time of music activity since safe volume was disabled.
    // When this time reaches UNSAFE_VOLUME_MUSIC_ACTIVE_MS_MAX, the safe media volume is re-enabled
    // automatically. mMusicActiveMs is rounded to a multiple of MUSIC_ACTIVE_POLL_PERIOD_MS.
    Int32 mMusicActiveMs;

    static const Int32 UNSAFE_VOLUME_MUSIC_ACTIVE_MS_MAX = (20 * 3600 * 1000); // 20 hours
    static const Int32 MUSIC_ACTIVE_POLL_PERIOD_MS = 60000;  // 1 minute polling interval
    static const Int32 SAFE_VOLUME_CONFIGURE_TIMEOUT_MS = 30000;  // 30s after boot completed

    //==========================================================================================
    // Camera shutter sound policy.
    // config_camera_sound_forced configuration option in config.xml defines if the camera shutter
    // sound is forced (sound even if the device is in silent mode) or not. This option is false by
    // default and can be overridden by country specific overlay in values-mccXXX/config.xml.
    //==========================================================================================
    // cached value of com.android.internal.R.bool.config_camera_sound_forced
    Boolean mCameraSoundForced;
    Object mCameraSoundForcedLock;

//    static AutoPtr< ArrayOf<String> > RINGER_MODE_NAMES;
    static String RINGER_MODE_NAMES[3];
    // = "SILENT",
    //   "VIBRATE",
    //   "NORMAL"

};

} // namespace Media
} // namepsace Droid
} // namespace Elastos

#endif // __CAUDIOSERVICE_H__
