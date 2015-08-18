#ifndef __ELASTOS_DROID_VIEW_MENU_CACTIONMENUPRESENTERSAVEDSTATE_H__
#define __ELASTOS_DROID_VIEW_MENU_CACTIONMENUPRESENTERSAVEDSTATE_H__

#include "_Elastos_Droid_View_Menu_CActionMenuPresenterSavedState.h"

namespace Elastos {
namespace Droid {
namespace View {
namespace Menu {

CarClass(CActionMenuPresenterSavedState)
{
public:
    CActionMenuPresenterSavedState();

    CARAPI constructor();

    CARAPI ReadFromParcel(
        /* [in] */ IParcel *source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel *dest);

    CARAPI SetOpenSubMenuId(
        /* [in] */ Int32 menuId);

    CARAPI GetOpenSubMenuId(
        /* [in] */ Int32* menuId);

private:
    Int32 mOpenSubMenuId;

};

}// namespace Menu
}// namespace View
}// namespace Droid
}// namespace Elastos
#endif
