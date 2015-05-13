#include "view/menu/CIconMenuViewSavedState.h"

namespace Elastos {
namespace Droid {
namespace View {
namespace Menu {

CIconMenuViewSavedState::CIconMenuViewSavedState()
    : mFocusedPosition(0)
{}

ECode CIconMenuViewSavedState::constructor()
{
    return NOERROR;
}

ECode CIconMenuViewSavedState::constructor(
    /* [in] */ IParcelable* superState,
    /* [in] */ Int32 focusedPosition)
{
    ViewBaseSavedState::Init(superState);
    mFocusedPosition = focusedPosition;
    return NOERROR;
}

ECode CIconMenuViewSavedState::GetSuperState(
    /* [out] */ IParcelable** superState)
{
    AutoPtr<IParcelable> temp = ViewBaseSavedState::GetSuperState();
    *superState = temp;
    INTERFACE_ADDREF(*superState);
    return NOERROR;
}

ECode CIconMenuViewSavedState::SetFocusedPosition(
    /* [in] */ Int32 focusedPosition)
{
    mFocusedPosition = focusedPosition;
    return NOERROR;
}

ECode CIconMenuViewSavedState::GetFocusedPosition(
    /* [out] */ Int32* focusedPosition)
{
    *focusedPosition = mFocusedPosition;
    return NOERROR;
}

ECode CIconMenuViewSavedState::ReadFromParcel(
    /* [in] */ IParcel *source)
{
    ViewBaseSavedState::ReadFromParcel(source);
    source->ReadInt32(&mFocusedPosition);
    return NOERROR;
}

ECode CIconMenuViewSavedState::WriteToParcel(
    /* [in] */ IParcel *dest)
{
    ViewBaseSavedState::WriteToParcel(dest);
    dest->WriteInt32(mFocusedPosition);
    return NOERROR;
}

} // namespace Menu
} // namespace View
} // namepsace Droid
} // namespace Elastos
