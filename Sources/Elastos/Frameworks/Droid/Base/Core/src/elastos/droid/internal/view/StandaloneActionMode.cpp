
#include "elastos/droid/internal/view/CMenuInflater.h"
#include "elastos/droid/internal/view/StandaloneActionMode.h"
#include "elastos/droid/internal/view/menu/CMenuBuilder.h"
#include "elastos/droid/internal/view/menu/CMenuPopupHelper.h"

using Elastos::Core::CString;
using Elastos::Droid::View::Accessibility::IAccessibilityEvent;
using Elastos::Droid::View::Accessibility::IAccessibilityEventSource;
using Elastos::Droid::View::EIID_IActionMode;
using Elastos::Droid::View::IMenuInflater;
using Elastos::Droid::View::Menu::CMenuBuilder;
using Elastos::Droid::View::Menu::CMenuPopupHelper;
using Elastos::Droid::View::Menu::IMenuPopupHelper;

namespace Elastos {
namespace Droid {
namespace Internal {
namespace View {

StandaloneActionMode::StandaloneActionMode(
    /* [in] */ IContext* context,
    /* [in] */ IActionBarContextView* view,
    /* [in] */ IActionModeCallback* callback,
    /* [in] */ Boolean isFocusable)
    : mContext(context)
    , mContextView(view)
    , mCallback(callback)
    , mFocusable(isFocusable)
{
    AutoPtr<IMenuBuilder> menuBuilder;
    CMenuBuilder::New(context, (IMenuBuilder**)&menuBuilder);
    menuBuilder->SetDefaultShowAsAction(IMenuItem::SHOW_AS_ACTION_IF_ROOM,
            (IMenuBuilder**)&mMenu);
    mMenu->SetCallback(this);
}

CAR_INTERFACE_IMPL(StandaloneActionMode, IActionMode);

ECode StandaloneActionMode::SetTitle(
    /* [in] */ ICharSequence* title)
{
    return mContextView->SetTitle(title);
}

ECode StandaloneActionMode::SetSubtitle(
    /* [in] */ ICharSequence* subtitle)
{
    return mContextView->SetSubtitle(subtitle);
}

ECode StandaloneActionMode::SetTitle(
    /* [in] */ Int32 resId)
{
    String str;
    mContext->GetString(resId, &str);
    AutoPtr<ICharSequence> title;
    CString::New(str, (ICharSequence**)&title);
    return SetTitle(title);
}

ECode StandaloneActionMode::SetSubtitle(
    /* [in] */ Int32 resId)
{
    String str;
    mContext->GetString(resId, &str);
    AutoPtr<ICharSequence> subTitle;
    CString::New(str, (ICharSequence**)&subTitle);
    return SetSubtitle(subTitle);
}

ECode StandaloneActionMode::SetTitleOptionalHint(
    /* [in] */ Boolean titleOptional)
{
    ActionMode::SetTitleOptionalHint(titleOptional);
    return mContextView->SetTitleOptional(titleOptional);
}

ECode StandaloneActionMode::IsTitleOptional(
    /* [out] */ Boolean* isTitleOptional)
{
    VALIDATE_NOT_NULL(isTitleOptional);
    return mContextView->IsTitleOptional(isTitleOptional);
}

ECode StandaloneActionMode::SetCustomView(
    /* [in] */ IView* view)
{
    mContextView->SetCustomView(view);
    mCustomView = NULL;
    if (view != NULL) {
        assert(IWeakReferenceSource::Probe(view) != NULL);
        IWeakReferenceSource::Probe(view)->GetWeakReference((IWeakReference**)&mCustomView);
    }
    return NOERROR;
}

ECode StandaloneActionMode::Invalidate()
{
    Boolean isPrepare = FALSE;
    return mCallback->OnPrepareActionMode(this, mMenu, &isPrepare);
}

ECode StandaloneActionMode::Finish()
{
    if (mFinished) {
        return NOERROR;
    }
    mFinished = TRUE;

    AutoPtr<IAccessibilityEventSource> eventSource = IAccessibilityEventSource::Probe(mContextView);
    eventSource->SendAccessibilityEvent(IAccessibilityEvent::TYPE_WINDOW_STATE_CHANGED);
    return mCallback->OnDestroyActionMode(this);
}

ECode StandaloneActionMode::GetMenu(
    /* [out] */ IMenu** menu)
{
    VALIDATE_NOT_NULL(menu);
    *menu = mMenu;
    REFCOUNT_ADD(*menu);
    return NOERROR;
}

ECode StandaloneActionMode::GetTitle(
    /* [out] */ ICharSequence** title)
{
    VALIDATE_NOT_NULL(title);
    return mContextView->GetTitle(title);
}

ECode StandaloneActionMode::GetSubtitle(
    /* [out] */ ICharSequence** subtitle)
{
    VALIDATE_NOT_NULL(subtitle);
    return mContextView->GetSubtitle(subtitle);
}

ECode StandaloneActionMode::GetCustomView(
    /* [out] */ IView** view)
{
    VALIDATE_NOT_NULL(view);
    *view = NULL;
    if (mCustomView != NULL) {
        mCustomView->Resolve(EIID_IView, (IInterface**)view);
    }
    return NOERROR;
}

ECode StandaloneActionMode::GetMenuInflater(
    /* [out] */ IMenuInflater** menuInflater)
{
    VALIDATE_NOT_NULL(menuInflater);
    return CMenuInflater::New(mContext, menuInflater);
}

ECode StandaloneActionMode::OnMenuItemSelected(
    /* [in] */ IMenuBuilder* menu,
    /* [in] */ IMenuItem* item,
    /* [out] */ Boolean* state)
{
    VALIDATE_NOT_NULL(state);
    return mCallback->OnActionItemClicked(this, item, state);
}

void StandaloneActionMode::OnCloseMenu(
    /* [in] */ IMenuBuilder* menu,
    /* [in] */ Boolean allMenusAreClosing)
{
}

Boolean StandaloneActionMode::OnSubMenuSelected(
    /* [in] */ ISubMenuBuilder* subMenu)
{
    Boolean isVisible = FALSE;
    if (!(subMenu->HasVisibleItems(&isVisible), isVisible)) {
        return TRUE;
    }
    AutoPtr<IMenuPopupHelper> menuPopupHelper;
    CMenuPopupHelper::New(mContext, subMenu, (IMenuPopupHelper**)&menuPopupHelper);
    menuPopupHelper->Show();
    return TRUE;
}

void StandaloneActionMode::OnCloseSubMenu(
    /* [in] */ ISubMenuBuilder* menu)
{
}

ECode StandaloneActionMode::OnMenuModeChange(
    /* [in] */ IMenuBuilder* menu)
{
    Invalidate();
    Boolean isShow = FALSE;
    return mContextView->ShowOverflowMenu(&isShow);
}

Boolean StandaloneActionMode::IsUiFocusable()
{
    return mFocusable;
}

} // namespace View
} // namespace Internal
} // namespace Droid
} // namespace Elastos