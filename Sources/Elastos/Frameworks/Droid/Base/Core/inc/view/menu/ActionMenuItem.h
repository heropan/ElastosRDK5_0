
#ifndef  __ACTIONMENUITEM_H__
#define  __ACTIONMENUITEM_H__

#include "ext/frameworkext.h"

using Elastos::Core::ICharSequence;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::IIntent;
using Elastos::Droid::Graphics::Drawable::IDrawable;
using Elastos::Droid::View::IMenuItem;


namespace Elastos {
namespace Droid {
namespace View {
namespace Menu {


class ActionMenuItem
{
public:
    ActionMenuItem(
        /* [in] */ IContext* context,
        /* [in] */ Int32 group,
        /* [in] */ Int32 id,
        /* [in] */ Int32 categoryOrder,
        /* [in] */ Int32 ordering,
        /* [in] */ ICharSequence* title);

    virtual CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid) = 0;

    CARAPI GetInterfaceID(
        /* [in] */ IInterface *pObject,
        /* [out] */ InterfaceID *pIID);

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

    CARAPI SetIconEx(
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

    CARAPI SetTitleEx(
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
    CARAPI SetActionViewEx(
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

protected:
    ActionMenuItem();

    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ Int32 group,
        /* [in] */ Int32 id,
        /* [in] */ Int32 categoryOrder,
        /* [in] */ Int32 ordering,
        /* [in] */ ICharSequence* title);

private:
    Int32 mId;
    Int32 mGroup;
    Int32 mCategoryOrder;
    Int32 mOrdering;

    AutoPtr<ICharSequence> mTitle;
    AutoPtr<ICharSequence> mTitleCondensed;
    AutoPtr<IIntent> mIntent;
    Char32 mShortcutNumericChar;
    Char32 mShortcutAlphabeticChar;

    AutoPtr<IDrawable> mIconDrawable;
    Int32 mIconResId;

    AutoPtr<IContext> mContext;

    AutoPtr<IOnMenuItemClickListener> mClickListener;

    static const Int32 NO_ICON;

    Int32 mFlags;
    static const Int32 CHECKABLE;
    static const Int32 CHECKED;
    static const Int32 EXCLUSIVE;
    static const Int32 HIDDEN;
    static const Int32 ENABLED;
};

} // namespace Menu
} // namespace View
} // namespace Droid
} // namespace Elastos

#endif //__ACTIONMENUITEM_H__
