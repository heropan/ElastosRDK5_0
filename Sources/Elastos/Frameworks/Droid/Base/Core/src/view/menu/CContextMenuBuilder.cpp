
#include "ext/frameworkext.h"
#include "view/menu/CContextMenuBuilder.h"

namespace Elastos {
namespace Droid {
namespace View {
namespace Menu {

ECode CContextMenuBuilder::constructor(
    /* [in] */ IContext* context)
{
    ContextMenuBuilderBase::Init(context);
    return NOERROR;
}

PInterface CContextMenuBuilder::Probe(
    /* [in]  */ REIID riid)
{
    if (EIID_MenuBuilderBase == riid) {
        return reinterpret_cast<PInterface>((MenuBuilderBase *)this);
    }
    return _CContextMenuBuilder::Probe(riid);
}

IMENU_METHODS_IMPL(CContextMenuBuilder, ContextMenuBuilderBase, ContextMenuBuilderBase);
IMENUBUILDER_METHODS_IMPL(CContextMenuBuilder, ContextMenuBuilderBase, ContextMenuBuilderBase);

ECode CContextMenuBuilder::SetHeaderIconEx(
    /* [in] */ IDrawable* icon)
{
    return ContextMenuBuilderBase::SetHeaderIcon(icon);
}

ECode CContextMenuBuilder::SetHeaderIcon(
    /* [in] */ Int32 iconRes)
{
    return ContextMenuBuilderBase::SetHeaderIcon(iconRes);
}

ECode CContextMenuBuilder::SetHeaderTitleEx(
    /* [in] */ ICharSequence* title)
{
    return ContextMenuBuilderBase::SetHeaderTitle(title);
}

ECode CContextMenuBuilder::SetHeaderTitle(
    /* [in] */ Int32 titleRes)
{
    return ContextMenuBuilderBase::SetHeaderTitle(titleRes);
}

ECode CContextMenuBuilder::SetHeaderView(
    /* [in] */ IView* view)
{
    return ContextMenuBuilderBase::SetHeaderView(view);
}

ECode CContextMenuBuilder::ClearHeader()
{
    return ContextMenuBuilderBase::ClearHeader();
}

CARAPI CContextMenuBuilder::Show(
    /* [in] */ IView* originalView,
    /* [in] */ IBinder* token,
    /* [out] */ IMenuDialogHelper** res)
{
    VALIDATE_NOT_NULL(res);
    AutoPtr<IMenuDialogHelper> temp = ContextMenuBuilderBase::Show(originalView, token);
    *res = temp;
    INTERFACE_ADDREF(*res);
    return NOERROR;
}

} // namespace Menu
} // namespace View
} // namespace Droid
} // namespace Elastos
