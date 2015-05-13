
#ifndef __VOLUMEPREFERENCE_H__
#define __VOLUMEPREFERENCE_H__

#include "SeekBarDialogPreference.h"
#include "database/ContentObserver.h"
#include "os/Runnable.h"
#include <elrefbase.h>

using Elastos::Droid::Database::IContentObserver;
using Elastos::Droid::Database::ContentObserver;
using Elastos::Droid::Media::IAudioManager;
using Elastos::Droid::Media::IRingtone;
using Elastos::Droid::Net::IUri;
using Elastos::Droid::Os::Runnable;
using Elastos::Droid::Os::IHandler;
using Elastos::Droid::View::IViewOnKeyListener;
using Elastos::Droid::Widget::ISeekBarOnSeekBarChangeListener;

namespace Elastos {
namespace Droid {
namespace Preference {

/**
 * @hide
 */
class VolumePreference
    : public SeekBarDialogPreference
    , public IPreferenceManagerOnActivityStopListener
    , public IViewOnKeyListener
{
public:
    class VolumeStore : public ElRefBase
    {
    public:
        VolumeStore()
            : mVolume(-1)
            , mOriginalVolume(-1)
        {}

    public:
        Int32 mVolume;
        Int32 mOriginalVolume;
    };

    /**
     * Turns a {@link SeekBar} into a volume control.
     */
    class SeekBarVolumizer
        : public ISeekBarOnSeekBarChangeListener
        , public Runnable
    {
    private:
        class MyContentObserver : public ContentObserver
        {
        public:
            MyContentObserver(
                /* [in] */ IHandler* handler,
                /* [in] */ SeekBarVolumizer* host);

            CARAPI OnChange(
                /* [in] */ Boolean selfChange);

        protected:
            SeekBarVolumizer* mHost;
        };

    public:
        SeekBarVolumizer(
            /* [in] */ IContext* context,
            /* [in] */ ISeekBar* seekBar,
            /* [in] */ Int32 streamType,
            /* [in] */ VolumePreference* host);

        SeekBarVolumizer(
            /* [in] */ IContext* context,
            /* [in] */ ISeekBar* seekBar,
            /* [in] */ Int32 streamType,
            /* [in] */ IUri* defaultUri,
            /* [in] */ VolumePreference* host);

        CAR_INTERFACE_DECL()

        CARAPI_(void) Stop();

        CARAPI_(void) RevertVolume();

        CARAPI OnProgressChanged(
            /* [in] */ ISeekBar* seekBar,
            /* [in] */ Int32 progress,
            /* [in] */ Boolean fromTouch);

        CARAPI OnStartTrackingTouch(
            /* [in] */ ISeekBar* seekBar);

        CARAPI OnStopTrackingTouch(
            /* [in] */ ISeekBar* seekBar);

        CARAPI Run();

        CARAPI_(Boolean) IsSamplePlaying();

        CARAPI_(void) StartSample();

        CARAPI_(void) StopSample();

        CARAPI_(AutoPtr<ISeekBar>) GetSeekBar();

        CARAPI_(void) ChangeVolumeBy(
            /* [in] */ Int32 amount);

        CARAPI_(void) MuteVolume();

        CARAPI_(void) OnSaveInstanceState(
            /* [in] */ VolumeStore* volumeStore);

        CARAPI_(void) OnRestoreInstanceState(
            /* [in] */ VolumeStore* volumeStore);

    private:
        CARAPI_(void) Init(
            /* [in] */ IContext* context,
            /* [in] */ ISeekBar* seekBar,
            /* [in] */ Int32 streamType,
            /* [in] */ IUri* defaultUri);

        CARAPI_(void) InitSeekBar(
            /* [in] */ ISeekBar* seekBar,
            /* [in] */ IUri* defaultUri);

        CARAPI_(void) PostSetVolume(
            /* [in] */ Int32 progress);

    private:
        AutoPtr<IContext> mContext;
        AutoPtr<IHandler> mHandler;

        AutoPtr<IAudioManager> mAudioManager;
        Int32 mStreamType;
        Int32 mOriginalStreamVolume;
        AutoPtr<IRingtone> mRingtone;

        Int32 mLastProgress;
        AutoPtr<ISeekBar> mSeekBar;
        Int32 mVolumeBeforeMute;

        AutoPtr<IContentObserver> mVolumeObserver;

        AutoPtr<VolumePreference> mHost;
    };

public:
    VolumePreference(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CAR_INTERFACE_DECL()

    CARAPI_(void) SetStreamType(
        /* [in] */ Int32 streamType);

    CARAPI OnKey(
        /* [in] */ IView* v,
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event,
        /* [out] */ Boolean* result);

    CARAPI OnActivityStop();

protected:
    CARAPI OnBindDialogView(
        /* [in] */ IView* view);

    CARAPI OnDialogClosed(
        /* [in] */ Boolean positiveResult);

    CARAPI OnSampleStarting(
        /* [in] */ SeekBarVolumizer* volumizer);

    CARAPI OnSaveInstanceState(
        /* [out] */ IParcelable** state);

    CARAPI OnRestoreInstanceState(
        /* [in] */ IParcelable* state);

private:
    /**
     * Do clean up.  This can be called multiple times!
     */
    CARAPI_(void) Cleanup();

private:
    static const String TAG;

    Int32 mStreamType;

    /** May be null if the dialog isn't visible. */
    AutoPtr<SeekBarVolumizer> mSeekBarVolumizer;
};

}
}
}

#endif // __CVOLUMEPREFERENCE_H__
