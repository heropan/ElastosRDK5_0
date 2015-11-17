
#ifndef __ELASTOS_DROID_INTERNAL_VIEW_MENU_CACTIONMENUITEMVIEW_H__
#define __ELASTOS_DROID_INTERNAL_VIEW_MENU_CACTIONMENUITEMVIEW_H__

#include "_Elastos_Droid_Internal_View_Menu_CActionMenuItemView.h"
#include "elastos/droid/internal/view/menu/ActionMenuItemView.h"

namespace Elastos {
namespace Droid {
namespace Internal {
namespace View {
namespace Menu {

CarClass(CActionMenuItemView), public ActionMenuItemView
{
public:
    IVIEW_METHODS_DECL()

    IDRAWABLECALLBACK_METHODS_DECL()

    IKEYEVENTCALLBACK_METHODS_DECL()

    IACCESSIBILITYEVENTSOURCE_METHODS_DECL()

    ITEXTVIEW_METHODS_DECL()

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI OnPreDraw(
        /* [out] */ Boolean* result);

    CARAPI constructor(
        /* [in] */ IContext* ctx);

    CARAPI constructor(
        /* [in] */ IContext* ctx,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle);

    CARAPI constructor(
        /* [in] */ IContext* ctx,
        /* [in] */ IAttributeSet* attrs);

    CARAPI GetItemData(
        /* [out] */ IMenuItemImpl** itemData);

    CARAPI Initialize(
        /* [in] */ IMenuItemImpl* itemData,
        /* [in] */ Int32 menuType);

    CARAPI SetTitle(
        /* [in] */ ICharSequence* title);

    CARAPI SetCheckable(
        /* [in] */ Boolean checkable);

    CARAPI SetChecked(
        /* [in] */ Boolean checked);

    CARAPI SetShortcut(
        /* [in] */ Boolean showShortcut,
        /* [in] */ Char32 shortcutKey);

    CARAPI PrefersCondensedTitle(
        /* [out] */ Boolean* supported);

    CARAPI ShowsIcon(
        /* [out] */ Boolean* shows);

    CARAPI SetEnabled(
        /* [in] */ Boolean enabled);

    CARAPI NeedsDividerBefore(
        /* [out] */ Boolean* need);

    CARAPI NeedsDividerAfter(
        /* [out] */ Boolean* need);

    CARAPI OnLongClick(
        /* [in] */ IView* v,
        /* [out] */ Boolean* result);

    CARAPI OnClick(
        /* [in] */ IView* v);

    CARAPI SetItemInvoker(
        /* [in] */ IMenuBuilderItemInvoker* invoker);

    CARAPI SetExpandedFormat(
        /* [in] */ Boolean expandedFormat);

    CARAPI SetIcon(
        /* [in] */ IDrawable* icon);

    CARAPI HasText(
        /* [out] */ Boolean* rst);
};

} // namespace Menu
} // namespace View
} // namespace Internal
} // namespace Droid
} // namespace Elastos

#endif  //__ELASTOS_DROID_INTERNAL_VIEW_MENU_CACTIONMENUITEMVIEW_H__
