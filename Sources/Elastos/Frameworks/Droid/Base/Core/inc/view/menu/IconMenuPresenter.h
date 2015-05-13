
#ifndef  __ICONMENUPRESENTER_H__
#define  __ICONMENUPRESENTER_H__

#include "view/menu/BaseMenuPresenter.h"
#include "view/menu/MenuDialogHelper.h"

using Elastos::ElRefBase;
using Elastos::AutoPtr;
using Elastos::Utility::CObjectInt32Map;
using Elastos::Utility::IObjectInt32Map;
using Elastos::Droid::Os::IBundle;
using Elastos::Droid::View::Menu::IIconMenuItemView;

namespace Elastos {
namespace Droid {
namespace View {
namespace Menu {

/**
 * MenuPresenter for the classic "six-pack" icon menu.
 */
class IconMenuPresenter : public BaseMenuPresenter
{
protected:
    class SubMenuPresenterCallback
        : public ElRefBase
        , public IMenuPresenterCallback
    {
    public:
        SubMenuPresenterCallback(
            /* [in] */ IconMenuPresenter* host);

        CARAPI_(PInterface) Probe(
            /* [in] */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        //@Override
        CARAPI OnCloseMenu(
            /* [in] */ IMenuBuilder* menu,
            /* [in] */ Boolean allMenusAreClosing);

        // /@Override
        CARAPI OnOpenSubMenu(
            /* [in] */ IMenuBuilder* subMenu,
            /* [out] */ Boolean* result);

    private:
        IconMenuPresenter*  mHost;
    };

public:
    IconMenuPresenter(
        /* [in] */ IContext* context);

    //@Override
    virtual CARAPI InitForMenu(
        /* [in] */ IContext* context,
        /* [in] */ IMenuBuilder* menu);

    //@Override
    virtual CARAPI BindItemView(
        /* [in] */ IMenuItemImpl* item,
        /* [in] */ IMenuItemView* itemView);

    //@Override
    virtual CARAPI_(Boolean) ShouldIncludeItem(
        /* [in] */ Int32 childIndex,
        /* [in] */ IMenuItemImpl* item);

    //@Override
    virtual CARAPI OnSubMenuSelected(
        /* [in] */ ISubMenuBuilder* subMenu,
        /* [out] */ Boolean* result);

    //@Override
    virtual CARAPI UpdateMenuView(
        /* [in] */ Boolean cleared);

    virtual CARAPI_(Int32) GetNumActualItemsShown();

    virtual CARAPI SaveHierarchyState(
        /* [in] */ IBundle* outState);

    virtual CARAPI RestoreHierarchyState(
        /* [in] */ IBundle* inState);

    //@Override
    virtual CARAPI OnSaveInstanceState(
        /* [out] */ IParcelable** parcel);

    //@Override
    virtual CARAPI OnRestoreInstanceState(
        /* [in] */ IParcelable* state);

protected:
    IconMenuPresenter();

    CARAPI Init(
        /* [in] */ IContext* ctx);

    //@Override
    virtual CARAPI_(void) AddItemView(
        /* [in] */ IView* itemView,
        /* [in] */ Int32 childIndex);

    //@Override
    virtual CARAPI_(Boolean) FilterLeftoverView(
        /* [in] */ IViewGroup* parent,
        /* [in] */ Int32 childIndex);

protected:
    Int32 mOpenSubMenuId;
    AutoPtr<SubMenuPresenterCallback> mSubMenuPresenterCallback;
    AutoPtr<IMenuDialogHelper> mOpenSubMenu;

private:
    AutoPtr<IIconMenuItemView> mMoreView;
    Int32 mMaxItems;

    static String VIEWS_TAG;
    static String OPEN_SUBMENU_KEY;

};

} // namespace Menu
} // namespace View
} // namespace Droid
} // namespace Elastos

#endif  //__ICONMENUPRESENTER_H__
