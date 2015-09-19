#include "ext/frameworkext.h"
#include "widget/CShareActionProvider.h"

namespace Elastos{
namespace Droid{
namespace Widget{

PInterface CShareActionProvider::Probe(
    /* [in] */ REIID riid)
{
    return _CShareActionProvider::Probe(riid);
}

ECode CShareActionProvider::constructor(
    /* [in] */ IContext* context)
{
    return ShareActionProvider::Init(context);
}

ECode CShareActionProvider::SetOnShareTargetSelectedListener(
    /* [in] */ IOnShareTargetSelectedListener* listener)
{
    return ShareActionProvider::SetOnShareTargetSelectedListener(listener);
}

ECode CShareActionProvider::SetShareHistoryFileName(
    /* [in] */ const String& shareHistoryFile)
{
    return ShareActionProvider::SetShareHistoryFileName(shareHistoryFile);
}

ECode CShareActionProvider::SetShareIntent(
    /* [in] */ IIntent* shareIntent)
{
    return ShareActionProvider::SetShareIntent(shareIntent);
}

ECode CShareActionProvider::OnCreateActionView(
    /* [out] */ IView** view)
{
    VALIDATE_NOT_NULL(view);
    AutoPtr<IView> v = ShareActionProvider::OnCreateActionView();
    *view = v;
    REFCOUNT_ADD(*view);
    return NOERROR;
}

ECode CShareActionProvider::OnCreateActionView(
    /* [in] */ IMenuItem* forItem,
    /* [out] */ IView** view)
{
    VALIDATE_NOT_NULL(view);
    AutoPtr<IView> v = ShareActionProvider::OnCreateActionView(forItem);
    *view = v;
    REFCOUNT_ADD(*view);
    return NOERROR;
}

ECode CShareActionProvider::OverridesItemVisibility(
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);
    *res = ShareActionProvider::OverridesItemVisibility();
    return NOERROR;
}

ECode CShareActionProvider::IsVisible(
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);
    *res = ShareActionProvider::IsVisible();
    return NOERROR;
}

ECode CShareActionProvider::RefreshVisibility()
{
    return ShareActionProvider::RefreshVisibility();
}

ECode CShareActionProvider::OnPerformDefaultAction(
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);
    *res = ShareActionProvider::OnPerformDefaultAction();
    return NOERROR;
}

ECode CShareActionProvider::HasSubMenu(
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);
    *res = ShareActionProvider::HasSubMenu();
    return NOERROR;
}

ECode CShareActionProvider::OnPrepareSubMenu(
    /* [in] */ ISubMenu* submenu)
{
    return ShareActionProvider::OnPrepareSubMenu(submenu);
}

ECode CShareActionProvider::SubUiVisibilityChanged(
    /* [in] */ Boolean isVisible)
{
    return ShareActionProvider::SubUiVisibilityChanged(isVisible);
}

ECode CShareActionProvider::SetSubUiVisibilityListener(
    /* [in] */ ISubUiVisibilityListener* listener)
{
    return ShareActionProvider::SetSubUiVisibilityListener(listener);
}

ECode CShareActionProvider::SetVisibilityListener(
    /* [in] */ IVisibilityListener* listener)
{
    return ShareActionProvider::SetVisibilityListener(listener);
}

}// namespace Widget
}// namespace Droid
}// namespace Elastos