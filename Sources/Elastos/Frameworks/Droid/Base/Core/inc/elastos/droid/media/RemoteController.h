#ifndef _ELASTOS_DROID_MEDIA_REMOTECONTROLLER_H__
#define _ELASTOS_DROID_MEDIA_REMOTECONTROLLER_H__

#include "Elastos.Droid.Media.h"
#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/media/session/CMediaController.h"
#include "elastos/droid/media/MediaMetadataEditor.h"
#include <elastos/core/Object.h>

namespace Elastos{
namespace Droid {
namespace Media {

class RemoteController
    : public Object
    , public IRemoteController
{
private:
    //==================================================
    // Implementation of IRemoteControlDisplay interface
    class RcDisplay
        : public Object
        , public IIRemoteControlDisplay
    {
    public:
        RcDisplay(
            /* [in] */ RemoteController* rc);

        ~RcDisplay();

        CAR_INTERFACE_DECL()

        CARAPI SetCurrentClientId(
            /* [in] */ Int32 genId,
            /* [in] */ IPendingIntent* clientMediaIntent,
            /* [in] */ Boolean clearing);

        CARAPI SetEnabled(
            /* [in] */ Boolean enabled);

        CARAPI SetPlaybackState(
            /* [in] */ Int32 genId,
            /* [in] */ Int32 state,
            /* [in] */ Int64 stateChangeTimeMs,
            /* [in] */ Int64 currentPosMs,
            /* [in] */ Float speed);

        CARAPI SetTransportControlInfo(
            /* [in] */ Int32 genId,
            /* [in] */ Int32 transportControlFlags,
            /* [in] */ Int32 posCapabilities);

        CARAPI SetMetadata(
            /* [in] */ Int32 genId,
            /* [in] */ IBundle* metadata);

        CARAPI SetArtwork(
            /* [in] */ Int32 genId,
            /* [in] */ IBitmap* artwork);

        CARAPI SetAllMetadata(
            /* [in] */ Int32 genId,
            /* [in] */ IBundle* metadata,
            /* [in] */ IBitmap* artwork);
    private:
        AutoPtr<IWeakReference> mController;
    };

    class MediaControllerCallback
        : public Object
        , public IMediaControllerCallback
    {
    public:
        MediaControllerCallback(
            /* [in] */ RemoteController* host);

        ~MediaControllerCallback();

        CAR_INTERFACE_DECL()

        CARAPI OnPlaybackStateChanged(
            /* [in] */ IPlaybackState* state);

        CARAPI OnMetadataChanged(
            /* [in] */ IMediaMetadata* metadata);

    private:
        RemoteController* mHost;
    };

    /**
     * Listens for changes to the active session stack and replaces the
     * currently tracked session if it has changed.
     */
    class TopTransportSessionListener
        : public Object
        , public IMediaSessionManagerOnActiveSessionsChangedListener
    {
    public:
        TopTransportSessionListener();

        ~TopTransportSessionListener();

        CAR_INTERFACE_DECL()

        CARAPI OnActiveSessionsChanged(
            /* [in] */ List<AutoPtr<CMediaController> >* controllers);
    };

    class EventHandler
        : public Handler
    {
        EventHandler(
            /* [in] */ IRemoteController* rc,
            /* [in] */ ILooper* looper);

        ~EventHandler();

        CAR_INTERFACE_DECL()

        CARAPI HandleMessage(
            /* [in] */ IMessage* msg);
    };

    class PlaybackInfo
        : public Object
    {
    public:
        PlaybackInfo(
            /* [in] */ Int32 state,
            /* [in] */ Int64 stateChangeTimeMs,
            /* [in] */ Int64 currentPosMs,
            /* [in] */ Float speed);

        ~PlaybackInfo();

    public:
        Int32 mState;
        Int64 mStateChangeTimeMs;
        Int64 mCurrentPosMs;
        Float mSpeed;
    };

public:
    class MetadataEditor
        : public MediaMetadataEditor
    {
    public:
        CARAPI Apply();

        CAR_INTERFACE_DECL()

    protected:
        /**
         * @hide
         */
        MetadataEditor();

        CARAPI constructor();

        CARAPI constructor(
            /* [in] */ IBundle* metadata,
            /* [in] */ Int64 editableKeys);

    private:
        CARAPPI_(void) CleanupBitmapFromBundle(
            /* [in] */ Int32 key);
    };

public:
    RemoteController();

    virtual ~RemoteController();

    CAR_INTERFACE_DECL()

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IRemoteControllerOnClientUpdateListener* updateListener);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IRemoteControllerOnClientUpdateListener* updateListener,
        /* [in] */ ILooper* looper);

    CARAPI GetRemoteControlClientPackageName(
        /* [out] */ String* result);

    CARAPI GetEstimatedMediaPosition(
        /* [out] */ Int64* result);

    CARAPI SendMediaKeyEvent(
        /* [in] */ IKeyEvent* keyEvent,
        /* [out] */ Boolean* result);

    CARAPI SeekTo(
        /* [in] */ Int64 timeMs,
        /* [out] */ Boolean* result);

    CARAPI SetArtworkConfiguration(
        /* [in] */ Boolean wantBitmap,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [out] */ Boolean* result)；

    CARAPI SetArtworkConfiguration(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [out] */ Boolean* result);

    CARAPI ClearArtworkConfiguration(
        /* [out] */ Boolean* result);

    CARAPI SetSynchronizationMode(
        /* [in] */ Int32 sync,
        /* [out] */ Boolean* result);

    CARAPI EditMetadata(
        /* [out] */ IMediaMetadataEditor** result);

    CARAPI StartListeningToSessions();

    CARAPI StopListeningToSessions();

    CARAPI SetIsRegistered(
        /* [in] */ Boolean registered);

    CARAPI GetRcDisplay(
        /* [out] */ IRcDisplay** result);

    CARAPI GetArtworkSize(
        /* [out, callee] */ ArrayOf<Int32>** result);

    CARAPI GetUpdateListener(
        /* [out] */ IRemoteControllerOnClientUpdateListener** result);

private:
    static CARAPI_(void) SendMsg(
        /* [in] */ IHandler* handler,
        /* [in] */ Int32 msg,
        /* [in] */ Int32 existingMsgPolicy,
        /* [in] */ Int32 arg1,
        /* [in] */ Int32 arg2,
        /* [in] */ IInterface* obj,
        /* [in] */ Int32 delayMs);

    CARAPI_(void) OnNewPendingIntent(
        /* [in] */ Int32 genId,
        /* [in] */ IPendingIntent* pi);

    CARAPI_(void) OnNewPlaybackInfo(
        /* [in] */ Int32 genId,
        /* [in] */ PlaybackInfo* pi);

    CARAPI_(void) OnNewTransportInfo(
        /* [in] */ Int32 genId,
        /* [in] */ Int32 transportControlFlags);

    CARAPI_(void) OnNewMetadata(
        /* [in] */ Int32 genId,
        /* [in] */ IBundle* metadata);

    CARAPI_(void) OnClientChange(
        /* [in] */ Int32 genId,
        /* [in] */ Boolean clearing);

    CARAPI_(void) OnDisplayEnable(
        /* [in] */ boolean enabled);

    CARAPI_(void) UpdateController(
        /* [in] */ CMediaController* controller);

    CARAPI_(void) OnNewPlaybackState(
        /* [in] */ PlaybackState* state);

    CARAPI_(void) onNewMediaMetadata(
        /* [in] */ MediaMetadata* metadata);

private:
    const static Int32 MAX_BITMAP_DIMENSION;
    const static Int32 TRANSPORT_UNKNOWN;
    const static String TAG;
    const static Boolean DEBUG;
    const static Boolean USE_SESSIONS;
    const static Object mGenLock;
    const static Object mInfoLock;

    AutoPtr<RcDisplay> mRcd;
    AutoPtr<IContext> mContext;
    AutoPtr<IAudioManager> mAudioManager;
    Int32 mMaxBitmapDimension;
    AutoPtr<MetadataEditor> mMetadataEditor;

    AutoPtr<MediaSessionManager> mSessionManager;
    AutoPtr<IMediaSessionManagerOnActiveSessionsChangedListener> mSessionListener;
    AutoPtr<IMediaControllerCallback> mSessionCb;

    /**
     * Synchronized on mGenLock
     */
    Int32 mClientGenerationIdCurrent;

    /**
     * Synchronized on mInfoLock
     */
    Boolean mIsRegistered;
    AutoPtr<IPendingIntent> mClientPendingIntentCurrent;
    AutoPtr<IRemoteControllerOnClientUpdateListener> mOnClientUpdateListener;
    AutoPtr<PlaybackInfo> mLastPlaybackInfo;
    Int32 mArtworkWidth;
    Int32 mArtworkHeight;
    Boolean mEnabled;
    // synchronized on mInfoLock, for USE_SESSION apis.
    AutoPtr<MediaController> mCurrentSession;

    //==================================================
    // Event handling
    AutoPtr<EventHandler> mEventHandler;
    const static Int32 MSG_NEW_PENDING_INTENT;
    const static Int32 MSG_NEW_PLAYBACK_INFO;
    const static Int32 MSG_NEW_TRANSPORT_INFO;
    const static Int32 MSG_NEW_METADATA; // msg always has non-null obj parameter
    const static Int32 MSG_CLIENT_CHANGE;
    const static Int32 MSG_DISPLAY_ENABLE;
    const static Int32 MSG_NEW_PLAYBACK_STATE;
    const static Int32 MSG_NEW_MEDIA_METADATA;

    /** If the msg is already queued, replace it with this one. */
    static const Int32 SENDMSG_REPLACE;
    /** If the msg is already queued, ignore this one and leave the old. */
    static const Int32 SENDMSG_NOOP;
    /** If the msg is already queued, queue this one and leave the old. */
    static const Int32 SENDMSG_QUEUE;
};

} // namespace Elastos
} // namespace Droid
} // namespace Media

#endif // _ELASTOS_DROID_MEDIA_REMOTECONTROLLER_H__
