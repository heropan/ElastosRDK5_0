
#include "ext/frameworkext.h"
#include "view/menu/CIconMenuItemView.h"

using Elastos::Droid::Widget::EIID_TextView;

namespace Elastos {
namespace Droid {
namespace View {
namespace Menu {

IVIEW_METHODS_IMPL(CIconMenuItemView, IconMenuItemView);
ITEXTVIEW_METHODS_IMPL(CIconMenuItemView, IconMenuItemView);
IDRAWABLECALLBACK_METHODS_IMPL(CIconMenuItemView, IconMenuItemView);
IKEYEVENTCALLBACK_METHODS_IMPL(CIconMenuItemView, IconMenuItemView);
IACCESSIBILITYEVENTSOURCE_METHODS_IMPL(CIconMenuItemView, IconMenuItemView);

ECode CIconMenuItemView::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    return IconMenuItemView::Init(context, attrs, defStyle);
}

ECode CIconMenuItemView::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return IconMenuItemView::Init(context, attrs);
}

PInterface CIconMenuItemView::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    }
    else if (riid == EIID_TextView) {
        return reinterpret_cast<PInterface>((TextView*)this);
    }
    else if (riid == EIID_IconMenuItemView) {
        return reinterpret_cast<PInterface>((IconMenuItemView*)this);
    }
    return _CIconMenuItemView::Probe(riid);
}

ECode CIconMenuItemView::OnPreDraw(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    *result = IconMenuItemView::OnPreDraw();
    return NOERROR;
}

ECode CIconMenuItemView::SetItemInvoker(
    /* [in] */ IMenuBuilderItemInvoker* itemInvoker)
{
    return IconMenuItemView::SetItemInvoker(itemInvoker);
}

//from IMenuItemView
ECode CIconMenuItemView::Initialize(
    /* [in] */ IMenuItemImpl* itemData,
    /* [in] */ Int32 menuType)
{
    return IconMenuItemView::Initialize(itemData, menuType);
}

ECode CIconMenuItemView::SetItemData(
    /* [in] */ IMenuItemImpl* data)
{
    return IconMenuItemView::SetItemData(data);
}

ECode CIconMenuItemView::GetItemData(
    /* [out] */ IMenuItemImpl** itemData)
{
    VALIDATE_NOT_NULL(itemData);

    AutoPtr<IMenuItemImpl> item = IconMenuItemView::GetItemData();
    *itemData = item;
    INTERFACE_ADDREF(*itemData);
    return NOERROR;
}

ECode CIconMenuItemView::SetTitle(
    /* [in] */ ICharSequence* title)
{
    return IconMenuItemView::SetTitle(title);
}

ECode CIconMenuItemView::SetEnabledEx(
    /* [in] */ Boolean enabled)
{
    return IconMenuItemView::SetEnabled(enabled);
}

ECode CIconMenuItemView::SetCheckable(
    /* [in] */ Boolean checkable)
{
    return IconMenuItemView::SetCheckable(checkable);
}

ECode CIconMenuItemView::SetChecked(
    /* [in] */ Boolean checked)
{
    return IconMenuItemView::SetChecked(checked);
}

ECode CIconMenuItemView::SetShortcut(
    /* [in] */ Boolean showShortcut,
    /* [in] */ Char32 shortcutKey)
{
    return IconMenuItemView::SetShortcut(showShortcut, shortcutKey);
}

ECode CIconMenuItemView::SetIcon(
    /* [in] */ IDrawable* icon)
{
    return IconMenuItemView::SetIcon(icon);
}

ECode CIconMenuItemView::GetIcon(
    /* [out] */ IDrawable** icon)
{
    return IconMenuItemView::GetIcon(icon);
}


ECode CIconMenuItemView::PrefersCondensedTitle(
    /* [out] */ Boolean* supported)
{
    VALIDATE_NOT_NULL(supported);

    *supported = IconMenuItemView::PrefersCondensedTitle();
    return NOERROR;
}

ECode CIconMenuItemView::ShowsIcon(
    /* [out] */ Boolean* show)
{
    VALIDATE_NOT_NULL(show);

    *show = IconMenuItemView::ShowsIcon();
    return NOERROR;
}

} // namespace Menu
} // namespace View
} // namespace Droid
} // namespace Elastos
