#ifndef __ELASTOS_DROID_VIEW_MENU_CMENUITEMIMPL_H__
#define __ELASTOS_DROID_VIEW_MENU_CMENUITEMIMPL_H__

#include "_Elastos_Droid_View_Menu_CMenuItemImpl.h"
#include "view/menu/MenuItemImpl.h"

using Elastos::Core::ICharSequence;
using Elastos::Core::IRunnable;
using Elastos::Droid::Content::IIntent;
using Elastos::Droid::Graphics::Drawable::IDrawable;

namespace Elastos {
namespace Droid {
namespace View {
namespace Menu {

CarClass(CMenuItemImpl) , public MenuItemImpl
{
public:
    CMenuItemImpl();

    CARAPI constructor(
        /* [in] */ IMenuBuilder* menu,
        /* [in] */ Int32 group,
        /* [in] */ Int32 id,
        /* [in] */ Int32 categoryOrder,
        /* [in] */ Int32 ordering,
        /* [in] */ ICharSequence* title,
        /* [in] */ Int32 showAsAction);

    virtual CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI Invoke(
        /* [out] */ Boolean* handled);

    CARAPI GetOrdering(
        /* [out] */ Int32* ordering);

    CARAPI SetCallback(
        /* [in] */ IRunnable* cb);

    CARAPI SetExclusiveCheckable(
        /* [in] */ Boolean exclusive);

    CARAPI IsExclusiveCheckable(
        /* [out] */ Boolean* checkable);

    CARAPI ShouldShowIcon(
        /* [out] */ Boolean* show);

    CARAPI ActionFormatChanged();

    CARAPI IsActionButton(
        /* [out] */ Boolean* action);

    CARAPI RequestsActionButton(
        /* [out] */ Boolean* result);

    CARAPI RequiresActionButton(
        /* [out] */ Boolean* result);

    CARAPI SetIsActionButton(
        /* [in] */ Boolean isActionButton);

    CARAPI ShowsTextAsAction(
        /* [out] */ Boolean* shows);

    CARAPI HasCollapsibleActionView(
        /* [out] */ Boolean* result);

    CARAPI SetActionViewExpanded(
        /* [in] */ Boolean isExpanded);

    CARAPI GetItemId(
        /* [out] */ Int32* id);

    CARAPI GetGroupId(
        /* [out] */ Int32* id);

    CARAPI GetOrder(
        /* [out] */ Int32* order);

    CARAPI SetTitle(
        /* [in] */ ICharSequence* title);

    CARAPI SetTitle(
        /* [in] */ Int32 title);

    CARAPI GetTitle(
        /* [out] */ ICharSequence** title);

    CARAPI SetTitleCondensed(
        /* [in] */ ICharSequence* title);

    CARAPI GetTitleCondensed(
        /* [out] */ ICharSequence** title);

    CARAPI SetIcon(
        /* [in] */ IDrawable* icon);

    CARAPI SetIcon(
        /* [in] */ Int32 iconRes);

    CARAPI GetIcon(
        /* [out] */ IDrawable** icon);

    CARAPI SetIntent(
        /* [in] */ IIntent* intent);

    CARAPI GetIntent(
        /* [out] */ IIntent** intent);

    CARAPI SetShortcut(
        /* [in] */ Char32 numericChar,
        /* [in] */ Char32 alphaChar);

    CARAPI SetNumericShortcut(
        /* [in] */ Char32 numericChar);

    CARAPI GetNumericShortcut(
        /* [out] */ Char32* numericChar);

    CARAPI SetAlphabeticShortcut(
        /* [in] */ Char32 alphaChar);

    CARAPI GetAlphabeticShortcut(
        /* [out] */ Char32* alphaChar);

    CARAPI SetCheckable(
        /* [in] */ Boolean checkable);

    CARAPI IsCheckable(
        /* [out] */ Boolean* checkable);

    CARAPI SetChecked(
        /* [in] */ Boolean checked);

    CARAPI IsChecked(
        /* [out] */ Boolean* checked);

    CARAPI SetVisible(
        /* [in] */ Boolean visible);

    CARAPI IsVisible(
        /* [out] */ Boolean* visible);

    CARAPI SetEnabled(
        /* [in] */ Boolean enabled);

    CARAPI IsEnabled(
        /* [out] */ Boolean* enabled);

    CARAPI HasSubMenu(
        /* [out] */ Boolean* hasSubMenu);

    CARAPI GetSubMenu(
        /* [out] */ ISubMenu** subMenu);

    CARAPI SetOnMenuItemClickListener(
        /* [in] */ IOnMenuItemClickListener* menuItemClickListener);

    CARAPI GetMenuInfo(
        /* [out] */ IContextMenuInfo** menuInfo);

    CARAPI SetShowAsAction(
        /* [in] */ Int32 actionEnum);

    CARAPI SetShowAsActionFlags(
        /* [in] */ Int32 actionEnum);

    CARAPI SetActionView(
        /* [in] */ IView* view);

    CARAPI SetActionView(
        /* [in] */ Int32 resId);

    CARAPI GetActionView(
        /* [out] */ IView** view);

    CARAPI SetActionProvider(
        /* [in] */ IActionProvider* actionProvider);

    CARAPI GetActionProvider(
        /* [out] */ IActionProvider** provider);

    CARAPI ExpandActionView(
        /* [out] */ Boolean* expanded);

    CARAPI CollapseActionView(
        /* [out] */ Boolean* collapsed);

    CARAPI IsActionViewExpanded(
        /* [out] */ Boolean* expanded);

    CARAPI SetOnActionExpandListener(
        /* [in] */ IOnActionExpandListener* listener);

    CARAPI GetShortcut(
        /* [out] */ Char32* c);

    CARAPI GetShortcutLabel(
        /* [out] */ String* label);

    CARAPI ShouldShowShortcut(
        /* [out] */ Boolean* result);

    CARAPI SetSubMenu(
        /* [in] */ ISubMenuBuilder* subMenu);

    CARAPI GetTitleForItemView(
        /* [in] */ IMenuItemView* itemView,
        /* [out] */ ICharSequence** title);

    CARAPI SetCheckedInt(
        /* [in] */ Boolean checked);

    CARAPI SetVisibleInt(
        /* [in] */ Boolean show,
        /* [out] */ Boolean* isVisible);

    CARAPI SetMenuInfo(
        /* [in] */ IContextMenuInfo* menuInfo);
};

} // namespace Menu
} // namespace View
} // namepsace Droid
} // namespace Elastos

#endif
