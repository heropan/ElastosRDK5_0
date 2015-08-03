
#include "inputmethodservice/CExtractEditLayout.h"
#include "view/menu/CMenuBuilder.h"
//#include "view/CMenuInflater.h"
#include "R.h"

using Elastos::Droid::R;
//using Elastos::Droid::View::CMenuInflater;
using Elastos::Droid::View::EIID_IViewOnClickListener;
using Elastos::Droid::View::EIID_IActionMode;
using Elastos::Droid::View::EIID_View;
using Elastos::Droid::View::EIID_ViewGroup;
using Elastos::Droid::View::Menu::EIID_IMenuBuilderCallback;
using Elastos::Droid::View::Menu::CMenuBuilder;

namespace Elastos {
namespace Droid {
namespace InputMethodService {

CAR_INTERFACE_IMPL(CExtractEditLayout::ExtractActionMode, ActionMode, IMenuBuilderCallback);
CExtractEditLayout::ExtractActionMode::ExtractActionMode(
    /* [in] */ IActionModeCallback* cb,
    /* [in] */ CExtractEditLayout* host)
    : mHost(host)
{
    CMenuBuilder::New(mHost->GetContext(), (IMenuBuilder**)&mMenu);
    mMenu->SetCallback(this);
    mCallback = cb;
}

ECode CExtractEditLayout::ExtractActionMode::SetTitle(
    /* [in] */ ICharSequence* title)
{
    // Title will not be shown.
    return NOERROR;
}

ECode CExtractEditLayout::ExtractActionMode::SetTitle(
    /* [in] */ Int32 resId)
{
    // Title will not be shown.
    return NOERROR;
}

ECode CExtractEditLayout::ExtractActionMode::SetSubtitle(
    /* [in] */ ICharSequence* subtitle)
{
    // Subtitle will not be shown.
    return NOERROR;
}

ECode CExtractEditLayout::ExtractActionMode::SetSubtitle(
    /* [in] */ Int32 resId)
{
    // Subtitle will not be shown.
    return NOERROR;
}

ECode CExtractEditLayout::ExtractActionMode::IsTitleOptional(
    /* [out] */ Boolean* result)
{
    assert(result != NULL);
    *result = TRUE;
    // Not only is it optional, it will *never* be shown.
    return NOERROR;
}

ECode CExtractEditLayout::ExtractActionMode::SetCustomView(
    /* [in] */ IView* view)
{
    // Custom view is not supported here.
    return NOERROR;
}

ECode CExtractEditLayout::ExtractActionMode::Invalidate()
{
    mMenu->StopDispatchingItemsChanged();
    // try {
    Boolean tmp = FALSE;
    mCallback->OnPrepareActionMode(this, mMenu, &tmp);
    // } finally {
    return mMenu->StartDispatchingItemsChanged();
    // }
}

Boolean CExtractEditLayout::ExtractActionMode::DispatchOnCreate()
{
    mMenu->StopDispatchingItemsChanged();
    // try {
    Boolean ret = FALSE;
    mCallback->OnCreateActionMode(this, mMenu, &ret);
    // } finally {
    mMenu->StartDispatchingItemsChanged();
    // }
    return ret;
}

ECode CExtractEditLayout::ExtractActionMode::Finish()
{
    if (mHost->mActionMode.Get() != this) {
        // Not the active action mode - no-op
        return NOERROR;
    }

    mCallback->OnDestroyActionMode(this);
    mCallback = NULL;

    mHost->mExtractActionButton->SetVisibility(IView::VISIBLE);
    mHost->mEditButton->SetVisibility(IView::INVISIBLE);

    mHost->SendAccessibilityEvent(IAccessibilityEvent::TYPE_WINDOW_STATE_CHANGED);

    mHost->mActionMode = NULL;
    return NOERROR;
}

ECode CExtractEditLayout::ExtractActionMode::GetMenu(
    /* [out] */ IMenu** menu)
{
    assert(menu != NULL);
    *menu = mMenu;
    REFCOUNT_ADD(*menu);

    return NOERROR;
}

ECode CExtractEditLayout::ExtractActionMode::GetTitle(
    /* [out] */ ICharSequence** title)
{
    assert(title != NULL);
    *title = NULL;
    return NOERROR;
}

ECode CExtractEditLayout::ExtractActionMode::GetSubtitle(
    /* [out] */ ICharSequence** subtitle)
{
    assert(subtitle != NULL);
    *subtitle = NULL;
    return NOERROR;
}

ECode CExtractEditLayout::ExtractActionMode::GetCustomView(
    /* [out] */ IView** view)
{
    assert(view != NULL);
    *view = NULL;
    return NOERROR;
}

ECode CExtractEditLayout::ExtractActionMode::GetMenuInflater(
    /* [out] */ IMenuInflater** inflater)
{
    assert(inflater != NULL);
    assert(0);
    //return CMenuInflater::New(mHost->GetContext(), inflater);
    return NOERROR;
}

ECode CExtractEditLayout::ExtractActionMode::OnMenuItemSelected(
    /* [in] */ IMenuBuilder* menu,
    /* [in] */ IMenuItem* item,
    /* [out] */ Boolean* result)
{
    assert(result != NULL);

    if (mCallback != NULL) {
        return mCallback->OnActionItemClicked(this, item, result);
    }

    *result = FALSE;
    return NOERROR;
}

ECode CExtractEditLayout::ExtractActionMode::OnMenuModeChange(
    /* [in] */ IMenuBuilder* menu)
{
    return NOERROR;
}

ECode CExtractEditLayout::ExtractActionMode::SetTag(
    /* [in] */ IInterface* tag)
{
    return ActionMode::SetTag(tag);
}

ECode CExtractEditLayout::ExtractActionMode::GetTag(
    /* [out] */ IInterface** tag)
{
    return ActionMode::GetTag(tag);
}

ECode CExtractEditLayout::ExtractActionMode::SetTitleOptionalHint(
    /* [in] */ Boolean titleOptional)
{
    return ActionMode::SetTitleOptionalHint(titleOptional);
}

ECode CExtractEditLayout::ExtractActionMode::GetTitleOptionalHint(
    /* [out] */ Boolean* titleOptionalHint)
{
    return ActionMode::GetTitleOptionalHint(titleOptionalHint);
}

ECode CExtractEditLayout::ExtractActionMode::IsUiFocusable(
    /* [out] */ Boolean* isUiFocusable)
{
    return ActionMode::IsUiFocusable(isUiFocusable);
}

CAR_INTERFACE_IMPL(CExtractEditLayout::_OnClickListener, Object, IViewOnClickListener);
CExtractEditLayout::_OnClickListener::_OnClickListener(
    /* [in] */ CExtractEditLayout* host)
    : mHost(host)
{
}

ECode CExtractEditLayout::_OnClickListener::OnClick(
    /* [in] */ IView* clicked)
{
    if (mHost->mActionMode != NULL) {
        assert(0);
        //TODo
        //new MenuPopupHelper(mHost->GetContext(), mHost->mActionMode->mMenu, clicked)->Show();
    }

    return NOERROR;
}

AutoPtr<IActionMode> CExtractEditLayout::StartActionModeForChild(
    /* [in] */ IView* sourceView,
    /* [in] */ IActionModeCallback* cb)
{
    AutoPtr<ExtractActionMode> mode = new ExtractActionMode(cb, this);
    if (mode->DispatchOnCreate()) {
        mode->Invalidate();
        mExtractActionButton->SetVisibility(IView::INVISIBLE);
        mEditButton->SetVisibility(IView::VISIBLE);
        mActionMode = mode;
        SendAccessibilityEvent(IAccessibilityEvent::TYPE_WINDOW_STATE_CHANGED);
        return mode;
    }

    return NULL;
}

ECode CExtractEditLayout::OnFinishInflate()
{
    LinearLayout::OnFinishInflate();
    mExtractActionButton = IButton::Probe(FindViewById(R::id::inputExtractAction));
    mEditButton = IButton::Probe(FindViewById(R::id::inputExtractEditButton));
    AutoPtr<_OnClickListener> listener = new _OnClickListener(this);
    return mEditButton->SetOnClickListener(listener);
}

CAR_OBJECT_IMPL(CExtractEditLayout);
CAR_INTERFACE_IMPL(CExtractEditLayout, LinearLayout, IExtractEditLayout);
PInterface CExtractEditLayout::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    }
    else if (riid == EIID_ViewGroup) {
        return reinterpret_cast<PInterface>((ViewGroup*)this);
    }
    else if (riid == EIID_IExtractEditLayout) {
        return reinterpret_cast<PInterface>((IExtractEditLayout*)this);
    }

    return LinearLayout::Probe(riid);
}

ECode CExtractEditLayout::constructor(
    /* [in] */ IContext* ctx)
{
    return LinearLayout::constructor(ctx);
}

ECode CExtractEditLayout::constructor(
    /* [in] */ IContext* ctx,
    /* [in] */ IAttributeSet* attrs)
{
    return LinearLayout::constructor(ctx, attrs);
}

ECode CExtractEditLayout::IsActionModeStarted(
    /* [out] */ Boolean* started)
{
    assert(started != NULL);
    *started = mActionMode != NULL;
    return NOERROR;
}

ECode CExtractEditLayout::FinishActionMode()
{
    if (mActionMode != NULL) {
        return mActionMode->Finish();
    }

    return NOERROR;
}

} // namespace InputMethodService
} // namespace Droid
} // namespace Elastos
