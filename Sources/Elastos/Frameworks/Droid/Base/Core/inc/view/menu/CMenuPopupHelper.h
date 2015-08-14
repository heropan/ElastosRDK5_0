#ifndef __ELASTOS_DROID_VIEW_MENU_CMENUPOPUPHELPER_H__
#define __ELASTOS_DROID_VIEW_MENU_CMENUPOPUPHELPER_H__

#include "_CMenuPopupHelper.h"
#include "view/menu/MenuPopupHelper.h"

using Elastos::Droid::Content::IContext;
using Elastos::Droid::Widget::IAdapterView;

namespace Elastos {
namespace Droid {
namespace View {
namespace Menu {

CarClass(CMenuPopupHelper) , public MenuPopupHelper
{
public:
    CMenuPopupHelper();

    CARAPI constructor(
        /* [in] */IContext* ctx,
        /* [in] */IMenuBuilder* menu);

    CARAPI constructor(
        /* [in] */IContext* ctx,
        /* [in] */IMenuBuilder* menu,
        /* [in] */IView* anchorView);

    CARAPI constructor(
        /* [in] */IContext* ctx,
        /* [in] */IMenuBuilder* menu,
        /* [in] */IView* anchorView,
        /* [in] */Boolean overflowOnly);

    virtual CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI SetAnchorView(
        /* [in] */ IView* anchor);

    CARAPI SetForceShowIcon(
        /* [in] */ Boolean forceShow);

    CARAPI Show();

    CARAPI TryShow(
        /* [out] */ Boolean* rst);

    CARAPI Dismiss();

    CARAPI IsShowing(
        /* [out] */ Boolean* rst);

    CARAPI OnItemClick(
        /* [in] */ IAdapterView* parent,
        /* [in] */ IView* view,
        /* [in] */ Int32 position,
        /* [in] */ Int64 id);

    CARAPI OnKey(
        /* [in] */ IView* v,
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event,
        /* [out] */ Boolean* result);

    CARAPI OnGlobalLayout();

    CARAPI OnDismiss();

    CARAPI OnViewAttachedToWindow(
                /* [in] */ IView* v);

    CARAPI OnViewDetachedFromWindow(
        /* [in] */ IView* v);

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
} // namepsace Droid
} // namespace Elastos
#endif
