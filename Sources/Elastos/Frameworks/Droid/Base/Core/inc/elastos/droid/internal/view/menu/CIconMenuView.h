
#ifndef __ELASTOS_DROID_INTERNAL_VIEW_MENU_CICONMENUVIEW_H__
#define __ELASTOS_DROID_INTERNAL_VIEW_MENU_CICONMENUVIEW_H__

#include "_Elastos_Droid_Internal_View_Menu_CIconMenuView.h"
#include "elastos/droid/internal/view/menu/IconMenuView.h"

namespace Elastos {
namespace Droid {
namespace Internal {
namespace View {
namespace Menu {

CarClass(CIconMenuView), public IconMenuView
{
public:
    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI_(PInterface) Probe(
        /* [in]  */ REIID riid);

    IVIEW_METHODS_DECL()
    IVIEWGROUP_METHODS_DECL()
    IVIEWPARENT_METHODS_DECL()
    IVIEWMANAGER_METHODS_DECL()
    IDRAWABLECALLBACK_METHODS_DECL()
    IKEYEVENTCALLBACK_METHODS_DECL()
    IACCESSIBILITYEVENTSOURCE_METHODS_DECL()

    CARAPI GetLayout(
        /* [out, callee] */ ArrayOf<Int32>** layout);

    CARAPI GetLayoutNumRows(
        /* [out] */ Int32* num);

    CARAPI Initialize(
        /* [in] */ IMenuBuilder* menu);

    CARAPI GetWindowAnimations(
        /* [out] */ Int32* animations);

    CARAPI InvokeItem(
        /* [in] */ IMenuItemImpl* item,
        /* [out] */ Boolean* state);

    CARAPI Run();
};

} // namespace Menu
} // namespace View
} // namespace Internal
} // namespace Droid
} // namespace Elastos

#endif  // __ELASTOS_DROID_INTERNAL_VIEW_MENU_CICONMENUVIEW_H__
