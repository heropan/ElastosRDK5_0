
#include "view/ActionProvider.h"

namespace Elastos {
namespace Droid {
namespace View {

PInterface ActionProvider::Probe(
    /* [in] */ REIID riid)
{
    if (EIID_IActionProvider == riid) {
        return (IActionProvider*)this;
    }

    return NULL;
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
