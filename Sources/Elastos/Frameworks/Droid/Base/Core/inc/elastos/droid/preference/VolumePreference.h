
#ifndef __ELASTOS_DROID_PREFERENCE_VOLUMEPREFERENCE_H__
#define __ELASTOS_DROID_PREFERENCE_VOLUMEPREFERENCE_H__

#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/preference/SeekBarDialogPreference.h"
#include "elastos/droid/database/ContentObserver.h"
#include "elastos/droid/os/Runnable.h"
#include <elastos/core/Object.h>

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
    , public ISeekBarVolumizerCallback
    , public IVolumePreference
{
public:
    VolumePreference(){}

    virtual ~VolumePreference(){}

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyleAttr,
        /* [in] */ Int32 defStyleRes);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyleAttr);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CAR_INTERFACE_DECL()

    CARAPI SetStreamType(
        /* [in] */ Int32 streamType);

    CARAPI OnKey(
        /* [in] */ IView* v,
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event,
        /* [out] */ Boolean* result);

    CARAPI OnActivityStop();

    CARAPI OnSampleStarting(
        /* [in] */ ISeekBarVolumizer* volumizer);

protected:
    CARAPI OnBindDialogView(
        /* [in] */ IView* view);

    CARAPI OnDialogClosed(
        /* [in] */ Boolean positiveResult);

    CARAPI OnSaveInstanceState(
        /* [out] */ IParcelable** state);

    CARAPI OnRestoreInstanceState(
        /* [in] */ IParcelable* state);

private:
    CARAPI_(void) Cleanup();
public:
    const static String TAG;
private:
    Int32 mStreamType;
    AutoPtr<ISeekBarVolumizer> mSeekBarVolumizer;

public:
    class VolumeStore
        : public Object
        , public IVolumePreferenceVolumeStore
    {
        friend class VolumePreference;
    public:
        VolumeStore()
            : volume(-1)
            , originalVolume(-1)
        {}
        VolumeStore(
            /* [in] */ VolumePreference* owner)
            : volume(-1)
            , originalVolume(-1)
            , mOwner(owner)
        {}
        CAR_INTERFACE_DECL()
    public:
        Int32 volume;
        Int32 originalVolume;
        VolumePreference* mOwner;
    };

};

}
}
}

#endif //__ELASTOS_DROID_PREFERENCE_VOLUMEPREFERENCE_H__
