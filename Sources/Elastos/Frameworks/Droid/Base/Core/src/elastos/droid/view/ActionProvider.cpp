
#include "elastos/droid/view/ActionProvider.h"

namespace Elastos {
namespace Droid {
namespace View {

CAR_INTERFACE_IMPL(ActionProvider, Object, IActionProvider)

ECode ActionProvider::OnCreateActionView(
        /* [out] */ IView** view)
{
    VALIDATE_NOT_NULL(view)
    AutoPtr<IView> rst = OnCreateActionView();
    *view = rst;
    REFCOUNT_ADD(*view)
    return NOERROR;
}

ECode ActionProvider::OnCreateActionView(
    /* [in] */ IMenuItem* forItem,
    /* [out] */ IView** view)
{
    VALIDATE_NOT_NULL(view)
    AutoPtr<IView> rst = OnCreateActionView(forItem);
    *view = rst;
    REFCOUNT_ADD(*view)
    return NOERROR;
}

ECode ActionProvider::OverridesItemVisibility(
    /* [out] */ Boolean* visibility)
{
    VALIDATE_NOT_NULL(visibility)
    *visibility = OverridesItemVisibility();
    return NOERROR;
}

ECode ActionProvider::IsVisible(
    /* [out] */ Boolean* visible)
{
    VALIDATE_NOT_NULL(visible)
    *visible = IsVisible();
    return NOERROR;
}

ECode ActionProvider::OnPerformDefaultAction(
    /* [out] */ Boolean* rst)
{
    VALIDATE_NOT_NULL(rst)
    *rst = OnPerformDefaultAction();
    return NOERROR;
}

ECode ActionProvider::HasSubMenu(
    /* [out] */ Boolean* rst)
{
    VALIDATE_NOT_NULL(rst)
    *rst = HasSubMenu();
    return NOERROR;
}

AutoPtr<IView> ActionProvider::OnCreateActionView(
    /* [in] */ IMenuItem* forItem)
{
    return OnCreateActionView();
}

Boolean ActionProvider::OverridesItemVisibility()
{
    return FALSE;
}

Boolean ActionProvider::IsVisible()
{
    return TRUE;
}

ECode ActionProvider::RefreshVisibility()
{
    if (mVisibilityListener != NULL && OverridesItemVisibility()) {
        mVisibilityListener->OnActionProviderVisibilityChanged(IsVisible());
    }
    return NOERROR;
}

Boolean ActionProvider::OnPerformDefaultAction()
{
    return FALSE;
}

Boolean ActionProvider::HasSubMenu()
{
    return FALSE;
}

ECode ActionProvider::OnPrepareSubMenu(
    /* [in] */ ISubMenu* subMenu)
{
    return NOERROR;
}

ECode ActionProvider::SubUiVisibilityChanged(
    /* [in] */ Boolean isVisible)
{
    if (mSubUiVisibilityListener != NULL) {
        mSubUiVisibilityListener->OnSubUiVisibilityChanged(isVisible);
    }
    return NOERROR;
}

ECode ActionProvider::SetSubUiVisibilityListener(
    /* [in] */ ISubUiVisibilityListener* listener)
{
    mSubUiVisibilityListener = listener;
    return NOERROR;
}

ECode ActionProvider::SetVisibilityListener(
    /* [in] */ IVisibilityListener* listener)
{
    if (mVisibilityListener != NULL) {
        // Log.w(TAG, "setVisibilityListener: Setting a new ActionProvider.VisibilityListener " +
        //         "when one is already set. Are you reusing this " + getClass().getSimpleName() +
        //         " instance while it is still in use somewhere else?");
    }
    mVisibilityListener = listener;
    return NOERROR;
}


} // namespace View
} // namespace Droid
} // namespace Elastos
