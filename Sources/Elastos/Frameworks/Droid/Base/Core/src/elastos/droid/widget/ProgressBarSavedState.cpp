
#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/widget/ProgressBarSavedState.h"

namespace Elastos {
namespace Droid {
namespace Widget {

ProgressBarSavedState::ProgressBarSavedState()
    : mProgress(0)
    , mSecondaryProgress(0)
{
}

ProgressBarSavedState::ProgressBarSavedState(
    /* [in] */ IParcelable* superState)
    : ViewBaseSavedState(superState)
    , mProgress(0)
    , mSecondaryProgress(0)
{
}

ECode ProgressBarSavedState::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    ViewBaseSavedState::WriteToParcel(dest);

    dest->WriteInt32(mProgress);
    dest->WriteInt32(mSecondaryProgress);

    return NOERROR;
}

ECode ProgressBarSavedState::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    ViewBaseSavedState::ReadFromParcel(source);

    source->ReadInt32(&mProgress);
    source->ReadInt32(&mSecondaryProgress);

    return NOERROR;
}

ECode ProgressBarSavedState::Init()
{
    return ViewBaseSavedState::Init();
}

ECode ProgressBarSavedState::Init(
    /* [in] */ IParcelable* superState)
{
    return ViewBaseSavedState::Init(superState);
}

}// namespace Widget
}// namespace Droid
}// namespace Elastos
