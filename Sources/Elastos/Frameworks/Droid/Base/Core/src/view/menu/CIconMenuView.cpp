
#include "ext/frameworkext.h"
#include "view/menu/CIconMenuView.h"

namespace Elastos {
namespace Droid {
namespace View {
namespace Menu {

IVIEW_METHODS_IMPL(CIconMenuView, IconMenuView);
IVIEWGROUP_METHODS_IMPL(CIconMenuView, IconMenuView);
IDRAWABLECALLBACK_METHODS_IMPL(CIconMenuView, IconMenuView);
IKEYEVENTCALLBACK_METHODS_IMPL(CIconMenuView, IconMenuView);
IACCESSIBILITYEVENTSOURCE_METHODS_IMPL(CIconMenuView, IconMenuView);
IVIEWPARENT_METHODS_IMPL(CIconMenuView, IconMenuView);
IVIEWMANAGER_METHODS_IMPL(CIconMenuView, IconMenuView);

ECode CIconMenuView::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return IconMenuView::Init(context, attrs);
}

PInterface CIconMenuView::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    }
    else if (riid == EIID_ViewGroup) {
        return reinterpret_cast<PInterface>((ViewGroup*)this);
    }
    return _CIconMenuView::Probe(riid);
}

ECode CIconMenuView::GetLayout(
    /* [out, callee] */ ArrayOf<Int32>** layout)
{
    VALIDATE_NOT_NULL(layout);

    return IconMenuView::GetLayout(layout);
}

ECode CIconMenuView::GetLayoutNumRows(
    /* [out] */ Int32* num)
{
    VALIDATE_NOT_NULL(num);

    *num = IconMenuView::GetLayoutNumRows();
    return NOERROR;
}

ECode CIconMenuView::Initialize(
    /* [in] */ IMenuBuilder* menu)
{
    return IconMenuView::Initialize(menu);
}

ECode CIconMenuView::GetWindowAnimations(
    /* [out] */ Int32* animations)
{
    VALIDATE_NOT_NULL(animations);

    *animations = IconMenuView::GetWindowAnimations();
    return NOERROR;
}

ECode CIconMenuView::InvokeItem(
    /* [in] */ IMenuItemImpl* item,
    /* [out] */ Boolean* state)
{
    VALIDATE_NOT_NULL(state);

    *state = IconMenuView::InvokeItem(item);
    return NOERROR;
}

ECode CIconMenuView::Run()
{
    return IconMenuView::Run();
}

} // namespace Menu
} // namespace View
} // namespace Droid
} // namespace Elastos
