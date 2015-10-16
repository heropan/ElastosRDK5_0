#include "elastos/droid/view/menu/CActionMenuPresenterSavedState.h"

namespace Elastos {
namespace Droid {
namespace View {
namespace Menu {

CActionMenuPresenterSavedState::CActionMenuPresenterSavedState()
    : mOpenSubMenuId(0)
{}

ECode CActionMenuPresenterSavedState::constructor()
{
    return NOERROR;
}

ECode CActionMenuPresenterSavedState::ReadFromParcel(
    /* [in] */ IParcel *source)
{
    source->ReadInt32(&mOpenSubMenuId);
    return NOERROR;
}

ECode CActionMenuPresenterSavedState::WriteToParcel(
    /* [in] */ IParcel *dest)
{
    dest->WriteInt32(mOpenSubMenuId);
    return NOERROR;
}

ECode CActionMenuPresenterSavedState::SetOpenSubMenuId(
        /* [in] */ Int32 menuId)
{
    mOpenSubMenuId = menuId;
    return NOERROR;
}

ECode CActionMenuPresenterSavedState::GetOpenSubMenuId(
        /* [in] */ Int32* menuId)
{
    *menuId = mOpenSubMenuId;
    return NOERROR;
}

}// namespace Menu
}// namespace View
}// namespace Droid
}// namespace Elastos
