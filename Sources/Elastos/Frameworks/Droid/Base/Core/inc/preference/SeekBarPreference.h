
#ifndef __ELASTOS_DROID_PREFERENCE_SEEKBARPREFERENCE_H__
#define __ELASTOS_DROID_PREFERENCE_SEEKBARPREFERENCE_H__

#include "Preference.h"

using Elastos::Droid::Widget::ISeekBar;
using Elastos::Droid::Widget::ISeekBarOnSeekBarChangeListener;

namespace Elastos {
namespace Droid {
namespace Preference {

/**
 * @hide
 */
class SeekBarPreference
    : public Preference
    , public ISeekBarOnSeekBarChangeListener
{
public:
    SeekBarPreference(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle);

    SeekBarPreference(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    SeekBarPreference(
        /* [in] */ IContext* context);

    CAR_INTERFACE_DECL()

    CARAPI GetSummary(
        /* [out] */ ICharSequence** summary);

    CARAPI OnKey(
        /* [in] */ IView* v,
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event,
        /* [out] */ Boolean* result);

    CARAPI_(void) SetMax(
        /* [in] */ Int32 max);

    CARAPI_(void) SetProgress(
        /* [in] */ Int32 progress);

    CARAPI_(Int32) GetProgress();

protected:
    CARAPI OnBindView(
        /* [in] */ IView* view);

    CARAPI OnSetInitialValue(
        /* [in] */ Boolean restoreValue,
        /* [in] */ IInterface* defaultValue);

    CARAPI OnGetDefaultValue(
        /* [in] */ ITypedArray* a,
        /* [in] */ Int32 index,
        /* [out] */ IInterface** value);

    /**
     * Persist the seekBar's progress value if callChangeListener
     * returns true, otherwise set the seekBar's progress to the stored value
     */
    CARAPI_(void) SyncProgress(
        /* [in] */ ISeekBar* seekBar);

    CARAPI OnProgressChanged(
            /* [in] */ ISeekBar* seekBar,
            /* [in] */ Int32 progress,
            /* [in] */ Boolean fromUser);

    CARAPI OnStartTrackingTouch(
        /* [in] */ ISeekBar* seekBar);

    CARAPI OnStopTrackingTouch(
        /* [in] */ ISeekBar* seekBar);

    CARAPI OnSaveInstanceState(
        /* [out] */ IParcelable** state);

    CARAPI OnRestoreInstanceState(
        /* [in] */ IParcelable* state);

private:
    CARAPI_(void) Init(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle);

    CARAPI_(void) SetProgress(
        /* [in] */ Int32 progress,
        /* [in] */ Boolean notifyChanged);

private:
    Int32 mProgress;
    Int32 mMax;
    Boolean mTrackingTouch;
};

}
}
}

#endif // __SeekBarPreference_H__
