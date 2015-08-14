#ifndef __ELASTOS_DROID_VIEW_MENU_CLISTMENUPRESENTER_H__
#define __ELASTOS_DROID_VIEW_MENU_CLISTMENUPRESENTER_H__

#include "_CListMenuPresenter.h"
#include "view/menu/ListMenuPresenter.h"

using Elastos::Droid::Content::IContext;
using Elastos::Droid::Widget::IAdapterView;
using Elastos::Droid::Widget::IListAdapter;
using Elastos::Droid::Os::IBundle;

namespace Elastos {
namespace Droid {
namespace View {
namespace Menu {

CarClass(CListMenuPresenter) , public ListMenuPresenter
{
public:
    CListMenuPresenter();

    virtual CARAPI_(PInterface) Probe(
        /* [in]  */ REIID riid);

    CARAPI constructor(
        /* [in] */ IContext* ctx,
        /* [in] */ Int32 itemLayoutRes);

    CARAPI constructor(
        /* [in] */ Int32 itemLayoutRes,
        /* [in] */ Int32 themeRes);

    CARAPI OnItemClick(
        /* [in] */ IAdapterView* parent,
        /* [in] */ IView* view,
        /* [in] */ Int32 position,
        /* [in] */ Int64 id);

    CARAPI GetAdapter(
        /* [out] */ IListAdapter** apt);

    CARAPI GetItemIndexOffset(
        /* [out] */ Int32* offset);

    CARAPI SetItemIndexOffset(
        /* [in] */ Int32 offset);

    CARAPI ExpandItemActionView(
        /* [in] */ IMenuBuilder* menu,
        /* [in] */ IMenuItemImpl* item,
        /* [out] */ Boolean* rst);

    CARAPI CollapseItemActionView(
        /* [in] */ IMenuBuilder* menu,
        /* [in] */ IMenuItemImpl* item,
        /* [out] */ Boolean* rst);

    CARAPI SaveHierarchyState(
        /* [in] */ IBundle* outState);

    CARAPI RestoreHierarchyState(
        /* [in] */ IBundle* outState);

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

    CARAPI GetId(
        /* [out] */ Int32* id);

    CARAPI OnSaveInstanceState(
        /* [out] */ IParcelable** pa);

    CARAPI OnRestoreInstanceState(
        /* [in] */ IParcelable* state);


};

} // namespace Menu
} // namespace View
} // namepsace Droid
} // namespace Elastos
#endif
