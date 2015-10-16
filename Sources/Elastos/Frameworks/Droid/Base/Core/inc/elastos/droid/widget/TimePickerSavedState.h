
#ifndef __TIMEPICKERSAVEDSTATE_H__
#define __TIMEPICKERSAVEDSTATE_H__

#include "elastos/droid/view/ViewBaseSavedState.h"

/**
 * Used to save / restore state of time picker
 */
class TimePickerSavedState : public ViewBaseSavedState
{
public:
    virtual CARAPI_(Int32) GetHour();

    virtual CARAPI_(Int32) GetMinute();

    virtual CARAPI WriteToParcel(
        /* [in] */ IParcel* dest,
        /* [in] */ Int32 flags);


    /*public static final Parcelable.Creator<SavedState> CREATOR = new Creator<SavedState>() {
        public SavedState createFromParcel(Parcel in) {
            return new SavedState(in);
        }

        public SavedState[] newArray(int size) {
            return new SavedState[size];
        }
    };*/

private:
    TimePickerSavedState(
        /* [in] */ IParcelable* superState,
        /* [in] */ Int32 hour,
        /* [in] */ Int32 minute);

    TimePickerSavedState(
        /* [in] */ IParcel* in);

    Int32 mHour;

    Int32 mMinute;
};

#endif
