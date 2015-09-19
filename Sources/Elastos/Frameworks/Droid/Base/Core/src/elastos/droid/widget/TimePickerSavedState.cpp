
#include "widget/TimePickerSavedState.h"

TimePickerSavedState::TimePickerSavedState(
    /* [in] */ IParcelable* superState, 
    /* [in] */ Int32 hour, 
    /* [in] */ Int32 minute)
{
    super(superState);
    mHour = hour;
    mMinute = minute;
}

TimePickerSavedState::TimePickerSavedState(
    /* [in] */ IParcel* in)
{
    super(in);
    mHour = in.readInt();
    mMinute = in.readInt();
}

Int32 TimePickerSavedState::GetHour() 
{
    return mHour;
}

Int32 TimePickerSavedState::GetMinute() 
{
    return mMinute;
}

ECode TimePickerSavedState::WriteToParcel(
    /* [in] */ IParcel* dest, 
    /* [in] */ Int32 flags)
{
    super.writeToParcel(dest, flags);
    dest.writeInt(mHour);
    dest.writeInt(mMinute);
}

