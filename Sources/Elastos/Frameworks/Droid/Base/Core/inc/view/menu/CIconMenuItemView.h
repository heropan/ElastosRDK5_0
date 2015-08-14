
#ifndef __ELASTOS_DROID_VIEW_MENU_CICONMENIITEMVIEW_H__
#define __ELASTOS_DROID_VIEW_MENU_CICONMENIITEMVIEW_H__


#include "_CIconMenuItemView.h"
#include "view/menu/IconMenuItemView.h"



namespace Elastos {
namespace Droid {
namespace View {
namespace Menu {

CarClass(CIconMenuItemView), public IconMenuItemView
{
public:
    using IconMenuItemView::Initialize;

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI_(PInterface) Probe(
        /* [in]  */ REIID riid);

    IVIEW_METHODS_DECL()
    ITEXTVIEW_METHODS_DECL()
    IDRAWABLECALLBACK_METHODS_DECL()
    IKEYEVENTCALLBACK_METHODS_DECL()
    IACCESSIBILITYEVENTSOURCE_METHODS_DECL()

    CARAPI OnPreDraw(
        /* [out] */ Boolean* result);

    CARAPI SetItemInvoker(
        /* [in] */ IMenuBuilderItemInvoker* itemInvoker);

    //from IMenuItemView
    CARAPI Initialize(
        /* [in] */ IMenuItemImpl* itemData,
        /* [in] */ Int32 menuType);

    CARAPI SetItemData(
        /* [in] */ IMenuItemImpl* data);

    CARAPI GetItemData(
        /* [out] */ IMenuItemImpl** itemData);

    CARAPI SetTitle(
        /* [in] */ ICharSequence* title);

    CARAPI SetEnabled(
        /* [in] */ Boolean enabled);

    CARAPI SetCheckable(
        /* [in] */ Boolean checkable);

    CARAPI SetChecked(
        /* [in] */ Boolean checked);

    CARAPI SetShortcut(
        /* [in] */ Boolean showShortcut,
        /* [in] */ Char32 shortcutKey);

    CARAPI SetIcon(
        /* [in] */ IDrawable* icon);

    CARAPI GetIcon(
        /* [out] */ IDrawable** icon);

    CARAPI PrefersCondensedTitle(
        /* [out] */ Boolean* supported);

    CARAPI ShowsIcon(
        /* [out] */ Boolean* show);
};

} // namespace Menu
} // namespace View
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_VIEW_MENU_CICONMENIITEMVIEW_H__
