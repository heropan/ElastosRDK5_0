#ifndef __ELASTOS_DROID_VIEW_MENU_CACTIONMENUITEM_H__
#define __ELASTOS_DROID_VIEW_MENU_CACTIONMENUITEM_H__

#include "elastos/droid/ext/frameworkext.h"
#include "view/menu/ActionMenuItem.h"
#include "_Elastos_Droid_View_Menu_CActionMenuItem.h"

using Elastos::Core::ICharSequence;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::IIntent;
using Elastos::Droid::Graphics::Drawable::IDrawable;
using Elastos::Droid::View::IMenuItem;


namespace Elastos {
namespace Droid {
namespace View {
namespace Menu {


CarClass(CActionMenuItem) , public ActionMenuItem
{
public:
    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ Int32 group,
        /* [in] */ Int32 id,
        /* [in] */ Int32 categoryOrder,
        /* [in] */ Int32 ordering,
        /* [in] */ ICharSequence* title);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI GetAlphabeticShortcut(
        /* [out] */ Char32* shortcut);

    CARAPI GetGroupId(
        /* [out] */ Int32* id);

    CARAPI GetIcon(
        /* [out] */ IDrawable** icon);

    CARAPI GetIntent(
        /* [out] */ IIntent** intent);

    CARAPI GetItemId(
        /* [out] */ Int32* id);

    CARAPI GetMenuInfo(
        /* [out] */ IContextMenuInfo** info);

    CARAPI GetNumericShortcut(
        /* [out] */ Char32* shortcut);

    CARAPI GetOrder(
        /* [out] */ Int32* ordering);

    CARAPI GetSubMenu(
        /* [out] */ ISubMenu** menu);

    CARAPI GetTitle(
        /* [out] */ ICharSequence** title);

    CARAPI GetTitleCondensed(
        /* [out] */ ICharSequence** title);

    CARAPI HasSubMenu(
        /* [out] */ Boolean* has);

    CARAPI IsCheckable(
        /* [out] */ Boolean* checkable);

    CARAPI IsChecked(
        /* [out] */ Boolean* checked);

    CARAPI IsEnabled(
        /* [out] */ Boolean* enabled);

    CARAPI IsVisible(
        /* [out] */ Boolean* visible);

    CARAPI SetAlphabeticShortcut(
        /* [in] */ Char32 alphaChar);

    CARAPI SetCheckable(
        /* [in] */ Boolean checkable);

    CARAPI SetExclusiveCheckable(
        /* [in] */ Boolean exclusive);

    CARAPI SetChecked(
        /* [in] */ Boolean checked);

    CARAPI SetEnabled(
        /* [in] */ Boolean enabled);

    CARAPI SetIcon(
        /* [in] */ IDrawable* icon);

    CARAPI SetIcon(
        /* [in] */ Int32 iconRes);

    CARAPI SetIntent(
        /* [in] */ IIntent* intent);

    CARAPI SetNumericShortcut(
        /* [in] */ Char32 numericChar);

    CARAPI SetOnMenuItemClickListener(
        /* [in] */ IOnMenuItemClickListener* menuItemClickListener);

    CARAPI SetShortcut(
        /* [in] */ Char32 numericChar,
        /* [in] */ Char32 alphaChar);

    CARAPI SetTitle(
        /* [in] */ ICharSequence* title);

    CARAPI SetTitle(
        /* [in] */ Int32 title);

    CARAPI SetTitleCondensed(
        /* [in] */ ICharSequence* title);

    CARAPI SetVisible(
        /* [in] */ Boolean visible);

    CARAPI Invoke(
        /* [out] */ Boolean* res);

    CARAPI SetShowAsAction(
        /* [in] */ Int32 show);

    CARAPI SetActionView(
        /* [in] */ IView* actionView);

    CARAPI GetActionView(
        /* [out] */ IView** view);

    //@Override
    CARAPI SetActionView(
        /* [in] */ Int32 resId);

    //@Override
    CARAPI GetActionProvider(
        /* [out] */ IActionProvider** provider);

    //@Override
    CARAPI SetActionProvider(
        /* [in] */ IActionProvider* actionProvider);

    //@Override
    CARAPI SetShowAsActionFlags(
        /* [in] */ Int32 actionEnum);

    //@Override
    CARAPI ExpandActionView(
        /* [out] */ Boolean* result);

    //@Override
    CARAPI CollapseActionView(
        /* [out] */ Boolean* result);

    //@Override
    CARAPI IsActionViewExpanded(
        /* [out] */ Boolean* result);

    //@Override
    CARAPI SetOnActionExpandListener(
        /* [in] */ IOnActionExpandListener* listener);
};

} // namespace Menu
} // namespace View
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_VIEW_MENU_CACTIONMENUITEM_H__
