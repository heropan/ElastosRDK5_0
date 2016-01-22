#ifndef _ELASTOS_DROID_MEDIA_PLAYERRECORD_H__
#define _ELASTOS_DROID_MEDIA_PLAYERRECORD_H__

#include "Elastos.Droid.Media.h"
#include "elastos/droid/ext/frameworkext.h"
#include <elastos/core/Object.h>

namespace Elastos{
namespace Droid {
namespace Media {

class PlayerRecord
	: public Object
	, public IPlayerRecord
	, public IProxyDeathRecipient
{
protected:
	class RccPlaybackState
        : public Object
        , public IRccPlaybackState
	{
	public:
		RccPlaybackState();

        virtual ~RccPlaybackState();

        CARAPI constructor(
            /* [in] */ Int32 state,
            /* [in] */ Long positionMs,
            /* [in] */ Float speed);

        CARAPI Reset();

        CARAPI ToString(
            /* [out] */ String* result);

    private:
        CARAPI_(String) PosToString();

        CARAPI_(String) StateToString();

    public:
        Int32 mState;
        Long mPositionMs;
        Float mSpeed;
	};

	class RemotePlaybackState
        : public Object
        , public IRemotePlaybackState
	{
	public:
		RemotePlaybackState();

        virtual ~RemotePlaybackState();

    protected:
        CARAPI constructor(
            /* [in] */ Int32 id,
            /* [in] */ Int32 vol,
            /* [in] */ Int32 volMax);
    public:
        Int32 mRccId;
        Int32 mVolume;
        Int32 mVolumeMax;
        Int32 mVolumeHandling;
	};

private:
    class RcClientDeathHandler
        : public Object
        , public IProxyDeathRecipient
    {
    public:
        RcClientDeathHandler();

        ~RcClientDeathHandler();

        CARAPI constructor(
            /* [in] */ IBinder* cb,
            /* [in] */ IPendingIntent* pi);

        CARAPI BinderDied();

        CARAPI GetBinder(
            /* [out] */ IBinder** result);

    private:
        AutoPtr<IBinder> mCb;
        AutoPtr<IPendingIntent> mMediaIntent;
    };

public:
	PlayerRecord();

	virtual ~PlayerRecord();

	CARAPI Dump(
		/* [in] */ IPrintWriter* pw,
		/* [in] */ Boolean registrationInfo);

	CARAPI ResetPlaybackInfo();

	CARAPI UnlinkToRcClientDeath();

	CARAPI Destroy();

	CARAPI BinderDied();

protected:
	static CARAPI SetMediaFocusControl(
		/* [in] */ IMediaFocusControl* mfc);

	CARAPI constructor(
		/* [in] */ IPendingIntent* mediaIntent,
		/* [in] */ IComponentName* eventReceiver,
		/* [in] */ IBinder* token);

	CARAPI GetRccId(
		/* [out] */ Int32* result);

	CARAPI GetRcc(
		/* [out] */ IRemoteControlClient** result);

	CARAPI GetMediaButtonReceiver(
		/* [out] */ IComponentName** result);

	CARAPI GetMediaButtonIntent(
		/* [out] */ IPendingIntent** result);

	CARAPI HasMatchingMediaButtonIntent(
		/* [in] */ PendingIntent pi,
		/* [out] */ Boolean* result);

	CARAPI IsPlaybackActive(
		/* [out] */ Boolean* result);

	CARAPI ResetControllerInfoForRcc(
		/* [in] */ IRemoteControlClient* rcClient,
        /* [in] */ const String& callingPackageName,
        /* [in] */ Int32 uid);

	CARAPI ResetControllerInfoForNoRcc();

	CARAPI Finalize();

public:
	static MediaFocusControl sController;
    /**
     * Information only used for non-local playback
     */
    //FIXME private?
    Int32 mPlaybackType;
    Int32 mPlaybackVolume;
    Int32 mPlaybackVolumeMax;
    Int32 mPlaybackVolumeHandling;
    Int32 mPlaybackStream;
    AutoPtr<IRccPlaybackState> mPlaybackState;
    AutoPtr<IIRemoteVolumeObserver> mRemoteVolumeObs;

private:
	// on purpose not using this classe's name, as it will only be used from MediaFocusControl
    static const String TAG = "MediaFocusControl";
    static const Boolean DEBUG = FALSE;

    /**
     * A global counter for RemoteControlClient identifiers
     */
    static Int32 sLastRccId = 0;

    /**
     * The target for the ACTION_MEDIA_BUTTON events.
     * Always non null. //FIXME verify
     */
    AutoPtr<IPendingIntent> mMediaIntent;
    /**
     * The registered media button event receiver.
     */
    AutoPtr<IComponentName> mReceiverComponent;

    Int32 mRccId = -1;

    /**
     * A non-null token implies this record tracks a "live" player whose death is being monitored.
     */
    AutoPtr<IBinder> mToken;
    String mCallingPackageName;
    Int32 mCallingUid;
    /**
     * Provides access to the information to display on the remote control.
     * May be null (when a media button event receiver is registered,
     *     but no remote control client has been registered) */
    AutoPtr<IRemoteControlClient> mRcClient;
    AutoPtr<IRcClientDeathHandler> mRcClientDeathHandler;
};

} // namespace Elastos
} // namespace Droid
} // namespace Media

#endif // _ELASTOS_DROID_MEDIA_PLAYERRECORD_H__
