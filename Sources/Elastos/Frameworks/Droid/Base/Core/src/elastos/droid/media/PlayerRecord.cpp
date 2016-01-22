#include "elastos/droid/media/PlayerRecord.h"

namespace Elastos{
namespace Droid {
namespace Media {
//================================================================================
//                      PlayerRecord::RccPlaybackState
//================================================================================
PlayerRecord::RccPlaybackState()
    : mState(0)
    , mPositionMs(0)
    , mSpeed(0)
{}

PlayerRecord::~RccPlaybackState()
{}

ECode PlayerRecord::RccPlaybackState::constructor(
    /* [in] */ Int32 state,
    /* [in] */ Long positionMs,
    /* [in] */ Float speed)
{
    mState = state;
    mPositionMs = positionMs;
    mSpeed = speed;
    return NOERROR;
}

ECode PlayerRecord::RccPlaybackState::Reset()
{
    mState = IRemoteControlClient::PLAYSTATE_STOPPED;
    mPositionMs = IRemoteControlClient::PLAYBACK_POSITION_INVALID;
    mSpeed = IRemoteControlClient::PLAYBACK_SPEED_1X;
    return NOERROR;
}

ECode PlayerRecord::RccPlaybackState::ToString(
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);
    *result = StateToString() + String(", ") + PosToString() + String(", ") + mSpeed + String("X");
    return NOERROR;
}

String PlayerRecord::RccPlaybackState::PosToString()
{
    if (mPositionMs == IRemoteControlClient::PLAYBACK_POSITION_INVALID) {
        return String("PLAYBACK_POSITION_INVALID");
    } else if (mPositionMs == IRemoteControlClient::PLAYBACK_POSITION_ALWAYS_UNKNOWN) {
        return String("PLAYBACK_POSITION_ALWAYS_UNKNOWN");
    } else {
        return StringUtils::ToString(mPositionMs) + String("ms");
    }
}

String PlayerRecord::RccPlaybackState::StateToString()
{
    switch (mState) {
        case IRemoteControlClient::PLAYSTATE_NONE:
            return String("PLAYSTATE_NONE");
        case IRemoteControlClient::PLAYSTATE_STOPPED:
            return String("PLAYSTATE_STOPPED");
        case IRemoteControlClient::PLAYSTATE_PAUSED:
            return String("PLAYSTATE_PAUSED");
        case IRemoteControlClient::PLAYSTATE_PLAYING:
            return String("PLAYSTATE_PLAYING");
        case IRemoteControlClient::PLAYSTATE_FAST_FORWARDING:
            return String("PLAYSTATE_FAST_FORWARDING");
        case IRemoteControlClient::PLAYSTATE_REWINDING:
            return String("PLAYSTATE_REWINDING");
        case IRemoteControlClient::PLAYSTATE_SKIPPING_FORWARDS:
            return String("PLAYSTATE_SKIPPING_FORWARDS");
        case IRemoteControlClient::PLAYSTATE_SKIPPING_BACKWARDS:
            return String("PLAYSTATE_SKIPPING_BACKWARDS");
        case IRemoteControlClient::PLAYSTATE_BUFFERING:
            return String("PLAYSTATE_BUFFERING");
        case RemoteControlClient.PLAYSTATE_ERROR:
            return String("PLAYSTATE_ERROR");
        default:
            return String("[invalid playstate]");
    }
}

//================================================================================
//                      PlayerRecord::RemotePlaybackState
//================================================================================
PlayerRecord::RemotePlaybackState::RemotePlaybackState()
    ：mRccId(0)
    , mVolume(0)
    , mVolumeMax(0)
    , mVolumeHandling(0)
{}

PlayerRecord::RemotePlaybackState::~RemotePlaybackState()
{}

ECode PlayerRecord::RemotePlaybackState::constructor(
    /* [in] */ Int32 id,
    /* [in] */ Int32 vol,
    /* [in] */ Int32 volMax)
{
    mRccId = id;
    mVolume = vol;
    mVolumeMax = volMax;
    mVolumeHandling = IRemoteControlClient::DEFAULT_PLAYBACK_VOLUME_HANDLING;
    return NOERROR;
}

//================================================================================
//                      PlayerRecord::RcClientDeathHandler
//================================================================================
PlayerRecord::RcClientDeathHandler::RcClientDeathHandler()
{}

PlayerRecord::RcClientDeathHandler::~RcClientDeathHandler()
{}

ECode PlayerRecord::RcClientDeathHandler::constructor(
    /* [in] */ IBinder* cb,
    /* [in] */ IPendingIntent* pi)
{
    mCb = cb;
    mMediaIntent = pi;
    return NOERROR;
}

ECode PlayerRecord::RcClientDeathHandler::BinderDied()
{
    Slogger::W(TAG, "  RemoteControlClient died");
    // remote control client died, make sure the displays don't use it anymore
    //  by setting its remote control client to null
    sController->RegisterRemoteControlClient(mMediaIntent, NULL/*rcClient*/, NULL/*ignored*/);
    // the dead client was maybe handling remote playback, the controller should reevaluate
    return sController->PostReevaluateRemote();
}

ECode PlayerRecord::RcClientDeathHandler::GetBinder(
    /* [out] */ IBinder** result)
{
    VALIDATE_NOT_NULL(result);
    *result = mCb;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

//================================================================================
//                      PlayerRecord
//================================================================================
PlayerRecord::PlayerRecord()
{}

PlayerRecord::~PlayerRecord()
{}

CAR_INTERFACE_IMPL(PlayerRecord, Object, IProxyDeathRecipient);

ECode PlayerRecord::Dump(
    /* [in] */ IPrintWriter* pw,
    /* [in] */ Boolean registrationInfo)
{
    VALIDATE_NOT_NULL(pw);
    if (registrationInfo) {
        pw->Println(String("  pi: ") + mMediaIntent +
                String(" -- pack: ") + mCallingPackageName +
                String("  -- ercvr: ") + mReceiverComponent +
                String("  -- client: ") + mRcClient +
                String("  -- uid: ") + mCallingUid +
                String("  -- type: ") + mPlaybackType +
                String("  state: ") + mPlaybackState);
    } else {
        // emphasis on state
        pw->Println(String("  uid: ") + mCallingUid +
                String("  -- id: ") + mRccId +
                String("  -- type: ") + mPlaybackType +
                String("  -- state: ") + mPlaybackState +
                String("  -- vol handling: ") + mPlaybackVolumeHandling +
                String("  -- vol: ") + mPlaybackVolume +
                String("  -- volMax: ") + mPlaybackVolumeMax +
                "  -- volObs: " + mRemoteVolumeObs);
    }

}

ECode PlayerRecord::ResetPlaybackInfo()
{
    mPlaybackType = IRemoteControlClient::PLAYBACK_TYPE_LOCAL;
    mPlaybackVolume = IRemoteControlClient::DEFAULT_PLAYBACK_VOLUME;
    mPlaybackVolumeMax = IRemoteControlClient::DEFAULT_PLAYBACK_VOLUME;
    mPlaybackVolumeHandling = IRemoteControlClient::DEFAULT_PLAYBACK_VOLUME_HANDLING;
    mPlaybackStream = IAudioManager::STREAM_MUSIC;
    mPlaybackState->Reset();
    mRemoteVolumeObs = NULL;
    return NOERROR;
}

ECode PlayerRecord::UnlinkToRcClientDeath()
{
    if ((mRcClientDeathHandler != NULL) && (mRcClientDeathHandler.mCb != NULL)) {
        // try {
            mRcClientDeathHandler.mCb->UnlinkToDeath(mRcClientDeathHandler, 0);
            mRcClientDeathHandler = NULL;
        // } catch (java.util.NoSuchElementException e) {
            // not much we can do here
            // Log.e(TAG, "Error in unlinkToRcClientDeath()", e);
        // }
    }
    return NOERROR;
}

ECode PlayerRecord::Destroy()
{
    UnlinkToRcClientDeath();
    if (mToken != NULL) {
        mToken->UnlinkToDeath(this, 0);
        mToken = NULL;
    }
    return NOERROR;
}

ECode PlayerRecord::BinderDied()
{
    return sController->UnregisterMediaButtonIntentAsync(mMediaIntent);
}

ECode PlayerRecord::SetMediaFocusControl(
    /* [in] */ IMediaFocusControl* mfc)
{
    sController = mfc;
    return NOERROR;
}

ECode PlayerRecord::constructor(
    /* [in] */ IPendingIntent* mediaIntent,
    /* [in] */ IComponentName* eventReceiver,
    /* [in] */ IBinder* token)
{
    ECode ec = NOERROR;
    mMediaIntent = mediaIntent;
    mReceiverComponent = eventReceiver;
    mToken = token;
    mCallingUid = -1;
    mRcClient = NULL;
    mRccId = ++sLastRccId;
    AutoPtr<RccPlaybackState> rp = new RccPlaybackState();
    rp->constructor(
        IRemoteControlClient::PLAYSTATE_STOPPED,
        IRemoteControlClient::PLAYBACK_POSITION_INVALID,
        IRemoteControlClient::PLAYBACK_SPEED_1X);
    ResetPlaybackInfo();
    if (mToken != NULL) {
        // try {
            ec = mToken->LinkToDeath(this, 0);
            if (ec == (ECode)E_REMOTE_EXCEPTION) {
               return sController->UnregisterMediaButtonIntentAsync(mMediaIntent);
            }
        // } catch (RemoteException e) {
            // sController.unregisterMediaButtonIntentAsync(mMediaIntent);
        // }
    }
    return NOERROR;
}

ECode PlayerRecord::GetRccId(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    *result = mRccId;
    return NOERROR;
}

ECode PlayerRecord::GetRcc(
    /* [out] */ IRemoteControlClient** result)
{
    VALIDATE_NOT_NULL(result);
    *result = mRcClient;
    REFCOUNT_ADD(*result);
    return NOERROR;
}


ECode PlayerRecord::GetMediaButtonReceiver(
    /* [out] */ IComponentName** result)
{
    VALIDATE_NOT_NULL(result);
    *result = mReceiverComponent;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

ECode PlayerRecord::GetMediaButtonIntent(
    /* [out] */ IPendingIntent** result)
{
    VALIDATE_NOT_NULL(result);
    *result = mMediaIntent;
    return NOERROR;
}

ECode PlayerRecord::HasMatchingMediaButtonIntent(
    /* [in] */ IPendingIntent* pi,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    if (mToken != NULL) {
        // return mMediaIntent.equals(pi);
        if (mReceiverComponent != NULL) {
            // return mReceiverComponent.equals(pi.getIntent().getComponent());
        } else {
            *result = FALSE;
            return NOERROR;
        }
    }

}

ECode PlayerRecord::IsPlaybackActive(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return MediaFocusControl::IsPlaystateActive(/*mPlaybackState.mState*/, result);
}

ECode PlayerRecord::ResetControllerInfoForRcc(
    /* [in] */ IRemoteControlClient* rcClient,
    /* [in] */ const String& callingPackageName,
    /* [in] */ Int32 uid)
{
    // already had a remote control client?
    if (mRcClientDeathHandler != NULL) {
        // stop monitoring the old client's death
        UnlinkToRcClientDeath();
    }
    // save the new remote control client
    mRcClient = rcClient;
    mCallingPackageName = callingPackageName;
    mCallingUid = uid;
    if (rcClient == NULL) {
        // here mcse.mRcClientDeathHandler is NULL;
        return ResetPlaybackInfo();
    } else {
        AutoPtr<IBinder> b;
        mRcClient->AsBinder((IBinder**)&b);
        AutoPtr<RcClientDeathHandler> rcdh = new RcClientDeathHandler();
        rcdh->constructor(b, mMediaIntent);
        // try {
        ECode ec = b->LinkToDeath(rcdh, 0);
        // } catch (RemoteException e) {
            // remote control client is DOA, disqualify it
        if (ec == (ECode)E_REMOTE_EXCEPTION) {
            Slogger::W(TAG, "registerRemoteControlClient() has a dead client %p", b);
            mRcClient = NULL;
        }
        // }
        mRcClientDeathHandler = rcdh;
    }
    return NOERROR;
}

ECode PlayerRecord::ResetControllerInfoForNoRcc()
{
    // stop monitoring the RCC death
    UnlinkToRcClientDeath();
    // reset the RCC-related fields
    mRcClient = NULL;
    mCallingPackageName = NULL;
    return NOERROR;
}

ECode PlayerRecord::Finalize()
{
    Destroy(); // unlink exception handled inside method
    // super.finalize();
    return NOERROR;
}

} // namespace Elastos
} // namespace Droid
} // namespace Media