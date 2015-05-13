#ifndef __CACTIONMENUPRESENTER_H__
#define __CACTIONMENUPRESENTER_H__
#include "_CActionMenuPresenter.h"
#include "ActionMenuPresenter.h"

using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::Res::IConfiguration;
using Elastos::Droid::View::IView;
using Elastos::Droid::View::IViewGroup;

namespace Elastos {
namespace Droid {
namespace View {
namespace Menu {

CarClass(CActionMenuPresenter) , public ActionMenuPresenter
{
public:

    CActionMenuPresenter();

    CARAPI constructor(
        /* [in] */ IContext* ctx);

    virtual CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI OnSubUiVisibilityChanged(
        /* [in] */ Boolean isVisible);

    CARAPI OnConfigurationChanged(
        /* [in] */ IConfiguration* newConfig);

    CARAPI SetWidthLimit(
        /* [in] */ Int32 width,
        /* [in] */ Boolean strict);

    CARAPI SetReserveOverflow(
        /* [in] */ Boolean reserveOverflow);

    CARAPI SetItemLimit(
        /* [in] */ Int32 itemCount);

    CARAPI SetExpandedActionViewsExclusive(
        /* [in] */ Boolean isExclusive);

    CARAPI ShowOverflowMenu(
        /* [out] */ Boolean* res);

    CARAPI HideOverflowMenu(
        /* [out] */ Boolean* res);

    CARAPI DismissPopupMenus(
        /* [out] */ Boolean* res);

    CARAPI HideSubMenus(
        /* [out] */ Boolean* res);

    CARAPI IsOverflowMenuShowing(
        /* [out] */ Boolean* res);

    CARAPI IsOverflowReserved(
        /* [out] */ Boolean* res);

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
};

} // namespace Menu
} // namespace View
} // namespace Droid
} // namespace Elastos
#endif
