
#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/widget/AbsSpinnerSavedState.h"

namespace Elastos {
namespace Droid {
namespace Widget {

AbsSpinnerSavedState::AbsSpinnerSavedState()
    : mSelectedId(-1)
    , mPosition(0)
{
}

AbsSpinnerSavedState::AbsSpinnerSavedState(
    /* [in] */ IParcelable* superState)
    : ViewBaseSavedState(superState)
    , mSelectedId(-1)
    , mPosition(0)
{
}

ECode AbsSpinnerSavedState::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    ViewBaseSavedState::WriteToParcel(dest);

    dest->WriteInt64(mSelectedId);
    dest->WriteInt32(mPosition);

    return NOERROR;
}

ECode AbsSpinnerSavedState::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    ViewBaseSavedState::ReadFromParcel(source);

    source->ReadInt64(&mSelectedId);
    source->ReadInt32(&mPosition);

    return NOERROR;
}

ECode AbsSpinnerSavedState::Init()
{
    return ViewBaseSavedState::Init();
}

ECode AbsSpinnerSavedState::Init(
    /* [in] */ IParcelable* superState)
{
    return ViewBaseSavedState::Init(superState);
}

}// namespace Widget
}// namespace Droid
}// namespace Elastos
