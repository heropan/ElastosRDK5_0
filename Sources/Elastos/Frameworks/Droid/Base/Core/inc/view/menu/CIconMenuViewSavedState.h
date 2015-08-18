#ifndef __ELASTOS_DROID_VIEW_MENU_CICONMENUVIEWSAVEDSTATE_H__
#define __ELASTOS_DROID_VIEW_MENU_CICONMENUVIEWSAVEDSTATE_H__
#include "_Elastos_Droid_View_Menu_CIconMenuViewSavedState.h"
#include "view/ViewBaseSavedState.h"

using Elastos::Droid::View::ViewBaseSavedState;

namespace Elastos {
namespace Droid {
namespace View {
namespace Menu {

CarClass(CIconMenuViewSavedState) , public ViewBaseSavedState
{
public:

    CIconMenuViewSavedState();

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IParcelable* superState,
        /* [in] */ Int32 focusedPosition);

    CARAPI GetSuperState(
        /* [out] */ IParcelable** superState);

    CARAPI SetFocusedPosition(
        /* [in] */ Int32 focusedPosition);

    CARAPI GetFocusedPosition(
        /* [out] */ Int32* focusedPosition);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel *source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel *dest);
private:
    Int32 mFocusedPosition;
};

} // namespace Menu
} // namespace View
} // namepsace Droid
} // namespace Elastos
#endif
