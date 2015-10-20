

#ifndef __ELASTOS_DROID_PREFERENCE_SEEKBARVOLUMIZER_H__
#define  __ELASTOS_DROID_PREFERENCE_SEEKBARVOLUMIZER_H__

#include "elastos/droid/ext/frameworkext.h"
#include <elastos/core/Object.h>
#include "elastos/droid/database/ContentObserver.h"
#include "elastos/droid/content/BroadcastReceiver.h"
#include "elastos/droid/os/Handler.h"

using Elastos::Droid::Widget::ISeekBarOnSeekBarChangeListener;
using Elastos::Droid::Widget::ISeekBar;
using Elastos::Droid::Os::IHandlerCallback;
using Elastos::Droid::Media::IRingtone;
using Elastos::Droid::Media::IAudioManager;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Os::IHandler;
using Elastos::Droid::Net::IUri;
using Elastos::Droid::Database::ContentObserver;
using Elastos::Droid::Database::IContentObserver;
using Elastos::Droid::Content::BroadcastReceiver;
using Elastos::Droid::Content::IContentResolver;
using Elastos::Droid::Os::IMessage;
using Elastos::Droid::Os::Handler;
using Elastos::Droid::Content::IIntent;

namespace Elastos {
namespace Droid {
namespace Preference {

class SeekBarVolumizer
    : public Object
    , public ISeekBarVolumizer
    //, public ISeekBarOnSeekBarChangeListener
    //, public IHandlerCallback
{
public:
    SeekBarVolumizer();

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ Int32 streamType,
        /* [in] */ IUri* defaultUri,
        /* [in] */ ISeekBarVolumizerCallback* callback);

    CAR_INTERFACE_DECL()

    virtual CARAPI SetSeekBar(
        /* [in] */ ISeekBar* seekBar);

    CARAPI HandleMessage(
        /* [in] */ IMessage* msg,
        /* [out] */ Boolean* result);

    virtual CARAPI PostStopSample();

    virtual CARAPI Stop();

    virtual CARAPI RevertVolume();

    virtual CARAPI OnProgressChanged(
        /* [in] */ ISeekBar* seekBar,
        /* [in] */ Int32 progress,
        /* [in] */ Boolean fromTouch);

    virtual CARAPI PostSetVolume(
        /* [in] */ Int32 progress);

    virtual CARAPI OnStartTrackingTouch(
        /* [in] */ ISeekBar* seekBar);

    virtual CARAPI OnStopTrackingTouch(
        /* [in] */ ISeekBar* seekBar);

    virtual CARAPI IsSamplePlaying(
        /* [out] */ Boolean* result);

    virtual CARAPI StartSample();

    virtual CARAPI StopSample();

    virtual CARAPI GetSeekBar(
        /* [out] */ ISeekBar** seekBar);

    virtual CARAPI ChangeVolumeBy(
        /* [in] */ Int32 amount);

    virtual CARAPI MuteVolume();

    virtual CARAPI OnSaveInstanceState(
        /* [in] */ IVolumePreferenceVolumeStore* volumeStore);

    virtual CARAPI OnRestoreInstanceState(
        /* [in] */ IVolumePreferenceVolumeStore* volumeStore);

private:
    CARAPI_(void) OnInitSample();
    CARAPI_(void) PostStartSample();
    CARAPI_(void) OnStartSample();
    CARAPI_(void) OnStopSample();

private:
    class SeekBarVolumizerH
        : public Handler
    {
        friend class SeekBarVolumizer;
    public:
        SeekBarVolumizerH(
            /* [in] */ SeekBarVolumizer* owner);

        CARAPI HandleMessage(
            /* [in] */ IMessage* msg);

        CARAPI PostUpdateSlider(
            /* [in] */ Int32 volume);

    private:
        const static Int32 UPDATE_SLIDER = 1;
        SeekBarVolumizer* mOwner;
    };

    class SeekBarVolumizerObserver
        : public ContentObserver
    {
        friend class SeekBarVolumizer;
    public:
        SeekBarVolumizerObserver(
            /* [in] */ IHandler* handler,
            /* [in] */ SeekBarVolumizer* owner);

        //@Override
        CARAPI OnChange(
            /* [in] */ Boolean selfChange);
    private:
        SeekBarVolumizer* mOwner;
    };

    class SeekBarVolumizerReceiver : public BroadcastReceiver
    {
        friend class SeekBarVolumizer;
    public:
        SeekBarVolumizerReceiver(
            /* [in] */ SeekBarVolumizer* owner);

        CARAPI_(void) SetListening(
            /* [in] */ Boolean listening);

        //@Override
        CARAPI OnReceive(
            /* [in] */ IContext* context,
            /* [in] */ IIntent* intent);
    private:
        Boolean mListening;
        SeekBarVolumizer* mOwner;
    };

private:
    const static String TAG;
    AutoPtr<IContext> mContext;
    AutoPtr<IHandler> mHandler;
    AutoPtr<SeekBarVolumizerH> mUiHandler;
    AutoPtr<ISeekBarVolumizerCallback> mCallback;
    AutoPtr<IUri> mDefaultUri;
    AutoPtr<IAudioManager> mAudioManager;
    Int32 mStreamType;
    Int32 mMaxStreamVolume;
    AutoPtr<SeekBarVolumizerReceiver> mReceiver;
    AutoPtr<SeekBarVolumizerObserver> mVolumeObserver;

    Int32 mOriginalStreamVolume;
    AutoPtr<IRingtone> mRingtone;
    Int32 mLastProgress;
    AutoPtr<ISeekBar> mSeekBar;
    Int32 mVolumeBeforeMute;

    const static Int32 MSG_SET_STREAM_VOLUME = 0;
    const static Int32 MSG_START_SAMPLE = 1;
    const static Int32 MSG_STOP_SAMPLE = 2;
    const static Int32 MSG_INIT_SAMPLE = 3;
    const static Int32 CHECK_RINGTONE_PLAYBACK_DELAY_MS = 1000;

};

} // Preference
} // Droid
} // Elastos

#endif  // __ELASTOS_DROID_PREFERENCE_SEEKBARVOLUMIZER_H__