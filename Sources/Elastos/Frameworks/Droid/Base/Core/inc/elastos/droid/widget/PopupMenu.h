
#ifndef __ELASTOS_DROID_WIDGET_POPUPMENU_H__
#define __ELASTOS_DROID_WIDGET_POPUPMENU_H__

#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/view/menu/CMenuPopupHelper.h"

using Elastos::Droid::Content::IContext;
using Elastos::Droid::View::IView;
using Elastos::Droid::View::IMenu;
using Elastos::Droid::View::IMenuInflater;
using Elastos::Droid::View::IMenuItem;
using Elastos::Droid::View::Menu::IMenuBuilder;
using Elastos::Droid::View::Menu::ISubMenuBuilder;
using Elastos::Droid::View::Menu::IMenuPopupHelper;
using Elastos::Droid::View::Menu::CMenuPopupHelper;
using Elastos::Droid::View::Menu::IMenuBuilderCallback;
using Elastos::Droid::View::Menu::IMenuPresenterCallback;

namespace Elastos {
namespace Droid {
namespace Widget {

class PopupMenu
{
private:
    class MyCallback
        : public IMenuBuilderCallback
        , public IMenuPresenterCallback
        , public ElRefBase
    {
    public:
        MyCallback(
            /* [in] */ PopupMenu* owner)
            : mOwner(owner)
        {}

        CAR_INTERFACE_DECL()

        CARAPI OnMenuItemSelected(
            /* [in] */ IMenuBuilder* menu,
            /* [in] */ IMenuItem* item,
            /* [out] */ Boolean* state);

        CARAPI OnMenuModeChange(
            /* [in] */ IMenuBuilder* menu);

        CARAPI OnOpenSubMenu(
            /* [in] */ IMenuBuilder* subMenu,
            /* [out] */ Boolean* handle);

        CARAPI OnCloseMenu(
            /* [in] */ IMenuBuilder* menu,
            /* [in] */ Boolean allMenusAreClosing);

    private:
        PopupMenu* mOwner;
    };

public:
    PopupMenu(
        /* [in] */ IContext* context,
        /* [in] */ IView* anchor);

    virtual CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid) = 0;

    virtual CARAPI_(AutoPtr<IMenu>) GetMenu();

    virtual CARAPI_(AutoPtr<IMenuInflater>) GetMenuInflater();

    virtual CARAPI Inflate(
        /* [in] */ Int32 menuRes);

    virtual CARAPI Show();

    virtual CARAPI Dismiss();

    virtual CARAPI SetOnMenuItemClickListener(
        /* [in] */ IPopupMenuOnMenuItemClickListener* listener);

    virtual CARAPI SetOnDismissListener(
        /* [in] */ IPopupMenuOnDismissListener* listener);

    virtual CARAPI_(Boolean) OnMenuItemSelected(
        /* [in] */ IMenuBuilder* menu,
        /* [in] */ IMenuItem* item);

    virtual CARAPI OnCloseMenu(
        /* [in] */ IMenuBuilder* menu,
        /* [in] */ Boolean allMenusAreClosing);

    virtual CARAPI_(Boolean) OnOpenSubMenu(
        /* [in] */ IMenuBuilder* subMenu);

    virtual CARAPI OnCloseSubMenu(
        /* [in] */ ISubMenuBuilder* menu);

    virtual CARAPI OnMenuModeChange(
        /* [in] */ IMenuBuilder* menu);

protected:
    PopupMenu();

    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ IView* anchor);

private:
    AutoPtr<IContext> mContext;
    AutoPtr<IMenuBuilder> mMenu;
    AutoPtr<IView> mAnchor;
    AutoPtr<IMenuPopupHelper> mPopup;
    AutoPtr<IPopupMenuOnMenuItemClickListener> mMenuItemClickListener;
    AutoPtr<IPopupMenuOnDismissListener> mDismissListener;
};

} // namespace Widget
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_WIDGET_POPUPMENU_H__
