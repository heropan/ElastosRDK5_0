
#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/view/menu/CSubMenuBuilder.h"
#include <elastos/core/StringUtils.h>

using Elastos::Core::StringUtils;

namespace Elastos {
namespace Droid {
namespace View {
namespace Menu {

ECode CSubMenuBuilder::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IMenuBuilder* parentMenu,
    /* [in] */ IMenuItemImpl* item)
{
    SubMenuBuilderBase::Init(context, parentMenu, item);
    return NOERROR;
}

PInterface CSubMenuBuilder::Probe(
    /* [in] */ REIID riid)
{
    if (EIID_MenuBuilderBase == riid) {
        return reinterpret_cast<PInterface>((MenuBuilderBase *)this);
    }

    return _CSubMenuBuilder::Probe(riid);
}

IMENU_METHODS_IMPL(CSubMenuBuilder, SubMenuBuilderBase, SubMenuBuilderBase);
IMENUBUILDER_METHODS_IMPL(CSubMenuBuilder, SubMenuBuilderBase, SubMenuBuilderBase);

ECode CSubMenuBuilder::SetIcon(
    /* [in] */ IDrawable* icon)
{
    SubMenuBuilderBase::SetIcon(icon);
    return NOERROR;
}

ECode CSubMenuBuilder::SetIcon(
    /* [in] */ Int32 iconRes)
{
    SubMenuBuilderBase::SetIcon(iconRes);
    return NOERROR;
}

ECode CSubMenuBuilder::SetHeaderIcon(
    /* [in] */ IDrawable* icon)
{
    SubMenuBuilderBase::SetHeaderIcon(icon);
    return NOERROR;
}

ECode CSubMenuBuilder::SetHeaderIcon(
    /* [in] */ Int32 iconRes)
{
    SubMenuBuilderBase::SetHeaderIcon(iconRes);
    return NOERROR;
}

ECode CSubMenuBuilder::SetHeaderTitle(
    /* [in] */ ICharSequence* title)
{
    SubMenuBuilderBase::SetHeaderTitle(title);
    return NOERROR;
}

ECode CSubMenuBuilder::SetHeaderTitle(
    /* [in] */ Int32 titleRes)
{
    SubMenuBuilderBase::SetHeaderTitle(titleRes);
    return NOERROR;
}

ECode CSubMenuBuilder::SetHeaderView(
    /* [in] */ IView* view)
{
    SubMenuBuilderBase::SetHeaderView(view);
    return NOERROR;
}

ECode CSubMenuBuilder::GetItem(
    /* [out] */ IMenuItem** menuItem)
{
    VALIDATE_NOT_NULL(menuItem);

    AutoPtr<IMenuItem> _menuItem = SubMenuBuilderBase::GetItem();
    *menuItem = _menuItem;
    REFCOUNT_ADD(*menuItem);
    return NOERROR;
}

ECode CSubMenuBuilder::GetParentMenu(
    /* [out] */ IMenu** menu)
{
    VALIDATE_NOT_NULL(menu);

    AutoPtr<IMenu> _menu = SubMenuBuilderBase::GetParentMenu();
    *menu = _menu;
    REFCOUNT_ADD(*menu);
    return NOERROR;
}

} // namespace Menu
} // namespace View
} // namespace Droid
} // namespace Elastos
