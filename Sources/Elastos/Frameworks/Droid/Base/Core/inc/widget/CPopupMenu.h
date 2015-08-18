#ifndef __ELASTOS_DROID_WIDGET_CPOPUPMENU_H__
#define __ELASTOS_DROID_WIDGET_CPOPUPMENU_H__

#include "_Elastos_Droid_Widget_CPopupMenu.h"
#include "widget/PopupMenu.h"

using Elastos::Droid::Content::IContext;
using Elastos::Droid::View::IView;
using Elastos::Droid::View::IMenu;
using Elastos::Droid::View::IMenuInflater;
using Elastos::Droid::Widget::IPopupMenuOnMenuItemClickListener;

namespace Elastos {
namespace Droid {
namespace Widget {

CarClass(CPopupMenu), public PopupMenu
{
public:
    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IView* anchor);

    CARAPI GetMenu(
        /* [out] */ IMenu** menu);

    CARAPI GetMenuInflater(
        /* [out] */ IMenuInflater** flater);

    CARAPI Inflate(
        /* [in] */ Int32 menuRes);

    CARAPI Show();

    CARAPI Dismiss();

    CARAPI SetOnMenuItemClickListener(
        /* [in] */ IPopupMenuOnMenuItemClickListener* listener);

    CARAPI SetOnDismissListener(
        /* [in] */ IPopupMenuOnDismissListener* listener);

    CARAPI OnMenuItemSelected(
        /* [in] */ IMenuBuilder* menu,
        /* [in] */ IMenuItem* item,
        /* [out] */ Boolean* rst);

    CARAPI OnMenuModeChange(
        /* [in] */ IMenuBuilder* menu);

    CARAPI OnCloseMenu(
        /* [in] */ IMenuBuilder* menu,
        /* [in] */ Boolean allMenusAreClosing);

    CARAPI OnOpenSubMenu(
        /* [in] */ IMenuBuilder* subMenu,
        /* [out] */ Boolean* rst);
};

} // namespace Widget
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_WIDGET_CPOPUPMENU_H__
