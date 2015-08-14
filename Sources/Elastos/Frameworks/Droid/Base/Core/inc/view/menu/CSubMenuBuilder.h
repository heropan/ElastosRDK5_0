
#ifndef __ELASTOS_DROID_VIEW_MENU_CSUBMENUBUILDER_H__
#define __ELASTOS_DROID_VIEW_MENU_CSUBMENUBUILDER_H__

#include "view/menu/SubMenuBuilderBase.h"
#include "_CSubMenuBuilder.h"




namespace Elastos {
namespace Droid {
namespace View {
namespace Menu {

/**
 * The model for a sub menu, which is an extension of the menu.  Most methods are proxied to
 * the parent menu.
 */
CarClass(CSubMenuBuilder) , public SubMenuBuilderBase
{
public:
    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IMenuBuilder* parentMenu,
        /* [in] */ IMenuItemImpl* item);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    IMENU_METHODS_DECL();
    IMENUBUILDER_METHODS_DECL();

    CARAPI GetItem(
        /* [out] */ IMenuItem** menuItem);

    CARAPI SetIcon(
        /* [in] */ IDrawable* icon);

    CARAPI SetIcon(
        /* [in] */ Int32 iconRes);

    CARAPI SetHeaderIcon(
        /* [in] */ IDrawable* icon);

    CARAPI SetHeaderIcon(
        /* [in] */ Int32 iconRes);

    CARAPI SetHeaderTitle(
        /* [in] */ ICharSequence* title);

    CARAPI SetHeaderTitle(
        /* [in] */ Int32 titleRes);

    CARAPI SetHeaderView(
        /* [in] */ IView* view);

    CARAPI GetParentMenu(
        /* [out] */ IMenu** menu);
};

} // namespace Menu
} // namespace View
} // namespace Droid
} // namespace Elastos

#endif  //__ELASTOS_DROID_VIEW_MENU_CSUBMENUBUILDER_H__
