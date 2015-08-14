#ifndef __ELASTOS_DROID_VIEW_MENU_CICONMENUPRESENTER_H__
#define __ELASTOS_DROID_VIEW_MENU_CICONMENUPRESENTER_H__
#include "_CIconMenuPresenter.h"
#include "view/menu/IconMenuPresenter.h"

using Elastos::Droid::Content::IContext;
using Elastos::Droid::View::IView;
using Elastos::Droid::View::IViewGroup;
using Elastos::Droid::Os::IBundle;
using Elastos::Droid::View::Menu::IconMenuPresenter;

namespace Elastos {
namespace Droid {
namespace View {
namespace Menu {

CarClass(CIconMenuPresenter) , public IconMenuPresenter
{
public:
    CIconMenuPresenter();

    CARAPI constructor(
        /* [in] */ IContext* context);

    virtual CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI InitForMenu(
        /* [in] */ IContext* context,
        /* [in] */ IMenuBuilder* menu);

    CARAPI GetMenuView(
        /* [in] */ IViewGroup* root,
        /* [out] */ IMenuView** view);

    CARAPI UpdateMenuView(
        /* [in] */ Boolean cleared);

    CARAPI SetCallback(
        /* [in] */ IMenuPresenterCallback* cb);

    CARAPI OnSubMenuSelected(
        /* [in] */ ISubMenuBuilder* subMenu,
        /* [out] */ Boolean* handled);

    CARAPI OnCloseMenu(
        /* [in] */ IMenuBuilder* menu,
        /* [in] */ Boolean allMenusAreClosing);

    CARAPI FlagActionItems(
        /* [out] */ Boolean* shown);

    CARAPI ExpandItemActionView(
        /* [in] */ IMenuBuilder* menu,
        /* [in] */ IMenuItemImpl* item,
        /* [out] */ Boolean* expanded);

    CARAPI CollapseItemActionView(
        /* [in] */ IMenuBuilder* menu,
        /* [in] */ IMenuItemImpl* item,
        /* [out] */ Boolean* collapsed);

    CARAPI GetId(
        /* [out] */ Int32* id);

    CARAPI OnSaveInstanceState(
        /* [out] */ IParcelable** pa);

    CARAPI OnRestoreInstanceState(
        /* [in] */ IParcelable* state);

    CARAPI CreateItemView(
        /* [in] */ IViewGroup* parent,
        /* [out] */ IMenuItemView** view);

    CARAPI GetItemView(
        /* [in] */ IMenuItemImpl* item,
        /* [in] */ IView* convertView,
        /* [in] */ IViewGroup* parent,
        /* [out] */ IView** v);

    CARAPI BindItemView(
        /* [in] */ IMenuItemImpl* item,
        /* [in] */ IMenuItemView* itemView);

    CARAPI ShouldIncludeItem(
        /* [in] */ Int32 childIndex,
        /* [in] */ IMenuItemImpl* item,
        /* [out] */ Boolean* res);

    CARAPI SetId(
        /* [in] */ Int32 id);

    CARAPI GetNumActualItemsShown(
        /* [out] */ Int32* rst);

    CARAPI SaveHierarchyState(
        /* [in] */ IBundle* outState);

    CARAPI RestoreHierarchyState(
        /* [in] */ IBundle* inState);
};

} // namespace Menu
} // namespace View
} // namespace Droid
} // namespace Elastos

#endif
