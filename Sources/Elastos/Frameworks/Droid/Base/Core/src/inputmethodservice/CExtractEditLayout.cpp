
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


ExtractEditLayout::ExtractActionMode::ExtractActionMode(
    /* [in] */ IActionModeCallback* cb,
    /* [in] */ ExtractEditLayout* host)
    : mHost(host)
{
    CMenuBuilder::New(mHost->GetContext(), (IMenuBuilder**)&mMenu);
    mMenu->SetCallback(this);
    mCallback = cb;
}

PInterface ExtractEditLayout::ExtractActionMode::Probe(
    /* [in] */ REIID riid)
{
    if (EIID_IActionMode == riid) {
        return (IActionMode*)this;
    }
    else if (EIID_IMenuBuilderCallback == riid) {
        return (IMenuBuilderCallback*)this;
    }

    return NULL;
}

UInt32 ExtractEditLayout::ExtractActionMode::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 ExtractEditLayout::ExtractActionMode::Release()
{
    return ElRefBase::Release();
}

ECode ExtractEditLayout::ExtractActionMode::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID);

    if (pObject == (IInterface*)(IActionMode*)this) {
        *pIID = EIID_IActionMode;
    }
    else if (pObject == (IInterface*)(IMenuBuilderCallback*)this) {
        *pIID = EIID_IMenuBuilderCallback;
    }
    else {
        return E_INVALID_ARGUMENT;
    }

    return NOERROR;
}

ECode ExtractEditLayout::ExtractActionMode::SetTitle(
    /* [in] */ ICharSequence* title)
{
    // Title will not be shown.
    return NOERROR;
}

ECode ExtractEditLayout::ExtractActionMode::SetTitleEx(
    /* [in] */ Int32 resId)
{
    // Title will not be shown.
    return NOERROR;
}

ECode ExtractEditLayout::ExtractActionMode::SetSubtitle(
    /* [in] */ ICharSequence* subtitle)
{
    // Subtitle will not be shown.
    return NOERROR;
}

ECode ExtractEditLayout::ExtractActionMode::SetSubtitleEx(
    /* [in] */ Int32 resId)
{
    // Subtitle will not be shown.
    return NOERROR;
}

ECode ExtractEditLayout::ExtractActionMode::IsTitleOptional(
    /* [out] */ Boolean* result)
{
    assert(result != NULL);
    *result = TRUE;
    // Not only is it optional, it will *never* be shown.
    return NOERROR;
}

ECode ExtractEditLayout::ExtractActionMode::SetCustomView(
    /* [in] */ IView* view)
{
    // Custom view is not supported here.
    return NOERROR;
}

ECode ExtractEditLayout::ExtractActionMode::Invalidate()
{
    mMenu->StopDispatchingItemsChanged();
    // try {
    Boolean tmp = FALSE;
    mCallback->OnPrepareActionMode(this, mMenu, &tmp);
    // } finally {
    return mMenu->StartDispatchingItemsChanged();
    // }
}

Boolean ExtractEditLayout::ExtractActionMode::DispatchOnCreate()
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

ECode ExtractEditLayout::ExtractActionMode::Finish()
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

ECode ExtractEditLayout::ExtractActionMode::GetMenu(
    /* [out] */ IMenu** menu)
{
    assert(menu != NULL);
    *menu = mMenu;
    INTERFACE_ADDREF(*menu);

    return NOERROR;
}

ECode ExtractEditLayout::ExtractActionMode::GetTitle(
    /* [out] */ ICharSequence** title)
{
    assert(title != NULL);
    *title = NULL;
    return NOERROR;
}

ECode ExtractEditLayout::ExtractActionMode::GetSubtitle(
    /* [out] */ ICharSequence** subtitle)
{
    assert(subtitle != NULL);
    *subtitle = NULL;
    return NOERROR;
}

ECode ExtractEditLayout::ExtractActionMode::GetCustomView(
    /* [out] */ IView** view)
{
    assert(view != NULL);
    *view = NULL;
    return NOERROR;
}

ECode ExtractEditLayout::ExtractActionMode::GetMenuInflater(
    /* [out] */ IMenuInflater** inflater)
{
    assert(inflater != NULL);
    assert(0);
    //return CMenuInflater::New(mHost->GetContext(), inflater);
    return NOERROR;
}

ECode ExtractEditLayout::ExtractActionMode::OnMenuItemSelected(
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

ECode ExtractEditLayout::ExtractActionMode::OnMenuModeChange(
    /* [in] */ IMenuBuilder* menu)
{
    return NOERROR;
}

ECode ExtractEditLayout::ExtractActionMode::SetTag(
    /* [in] */ IInterface* tag)
{
    return ActionMode::SetTag(tag);
}

ECode ExtractEditLayout::ExtractActionMode::GetTag(
    /* [out] */ IInterface** tag)
{
    return ActionMode::GetTag(tag);
}

ECode ExtractEditLayout::ExtractActionMode::SetTitleOptionalHint(
    /* [in] */ Boolean titleOptional)
{
    return ActionMode::SetTitleOptionalHint(titleOptional);
}

ECode ExtractEditLayout::ExtractActionMode::GetTitleOptionalHint(
    /* [out] */ Boolean* titleOptionalHint)
{
    return ActionMode::GetTitleOptionalHint(titleOptionalHint);
}

ECode ExtractEditLayout::ExtractActionMode::IsUiFocusable(
    /* [out] */ Boolean* isUiFocusable)
{
    return ActionMode::IsUiFocusable(isUiFocusable);
}

ExtractEditLayout::_OnClickListener::_OnClickListener(
    /* [in] */ ExtractEditLayout* host)
    : mHost(host)
{

}

PInterface ExtractEditLayout::_OnClickListener::Probe(
    /* [in] */ REIID riid)
{
    if (EIID_IViewOnClickListener == riid) {
        return (IViewOnClickListener*)this;
    }

    return NULL;
}

UInt32 ExtractEditLayout::_OnClickListener::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 ExtractEditLayout::_OnClickListener::Release()
{
    return ElRefBase::Release();
}

ECode ExtractEditLayout::_OnClickListener::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID);

    if (pObject == (IInterface*)(IViewOnClickListener*)this) {
        *pIID = EIID_IViewOnClickListener;
    }
    else {
        return E_INVALID_ARGUMENT;
    }

    return NOERROR;
}

ECode ExtractEditLayout::_OnClickListener::OnClick(
    /* [in] */ IView* clicked)
{
    if (mHost->mActionMode != NULL) {
        assert(0);
        //TODo
        //new MenuPopupHelper(mHost->GetContext(), mHost->mActionMode->mMenu, clicked)->Show();
    }

    return NOERROR;
}

AutoPtr<IActionMode> ExtractEditLayout::StartActionModeForChild(
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

ECode ExtractEditLayout::OnFinishInflate()
{
    LinearLayout::OnFinishInflate();
    mExtractActionButton = IButton::Probe(FindViewById(R::id::inputExtractAction));
    mEditButton = IButton::Probe(FindViewById(R::id::inputExtractEditButton));
    AutoPtr<_OnClickListener> listener = new _OnClickListener(this);
    return mEditButton->SetOnClickListener(listener);
}



IVIEW_METHODS_IMPL(CExtractEditLayout, ExtractEditLayout);
IVIEWGROUP_METHODS_IMPL(CExtractEditLayout, ExtractEditLayout);
IVIEWPARENT_METHODS_IMPL(CExtractEditLayout, ExtractEditLayout);
IVIEWMANAGER_METHODS_IMPL(CExtractEditLayout, ExtractEditLayout);
IDRAWABLECALLBACK_METHODS_IMPL(CExtractEditLayout, ExtractEditLayout);
IKEYEVENTCALLBACK_METHODS_IMPL(CExtractEditLayout, ExtractEditLayout);
IACCESSIBILITYEVENTSOURCE_METHODS_IMPL(CExtractEditLayout, ExtractEditLayout);

PInterface CExtractEditLayout::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    }
    else if (riid == EIID_ViewGroup) {
        return reinterpret_cast<PInterface>((ViewGroup*)this);
    }

    return _CExtractEditLayout::Probe(riid);
}

ECode CExtractEditLayout::constructor(
    /* [in] */ IContext* ctx)
{
    return ExtractEditLayout::Init(ctx);
}

ECode CExtractEditLayout::constructor(
    /* [in] */ IContext* ctx,
    /* [in] */ IAttributeSet* attrs)
{
    return ExtractEditLayout::Init(ctx, attrs);
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

ECode CExtractEditLayout::SetShowDividers(
    /* [in] */ Int32 showDividers)
{
    return ExtractEditLayout::SetShowDividers(showDividers);
}

ECode CExtractEditLayout::GetShowDividers(
    /* [out] */ Int32* showDividers)
{
    VALIDATE_NOT_NULL(showDividers);
    *showDividers = ExtractEditLayout::GetShowDividers();
    return NOERROR;
}

ECode CExtractEditLayout::GetDividerDrawable(
    /* [out] */ IDrawable** divider)
{
    VALIDATE_NOT_NULL(divider);
    AutoPtr<IDrawable> d = ExtractEditLayout::GetDividerDrawable();
    *divider = d;
    INTERFACE_ADDREF(*divider);
    return NOERROR;
}

ECode CExtractEditLayout::SetDividerDrawable(
    /* [in] */ IDrawable* divider)
{
    return ExtractEditLayout::SetDividerDrawable(divider);
}

ECode CExtractEditLayout::SetDividerPadding(
    /* [in] */ Int32 padding)
{
    return ExtractEditLayout::SetDividerPadding(padding);
}

ECode CExtractEditLayout::GetDividerPadding(
    /* [out] */ Int32* padding)
{
    VALIDATE_NOT_NULL(padding);
    *padding = ExtractEditLayout::GetDividerPadding();
    return NOERROR;
}

ECode CExtractEditLayout::GetDividerWidth(
    /* [out] */ Int32* width)
{
    VALIDATE_NOT_NULL(width);
    *width = ExtractEditLayout::GetDividerWidth();
    return NOERROR;
}

ECode CExtractEditLayout::IsBaselineAligned(
    /* [out] */ Boolean* baselineAligned)
{
    VALIDATE_NOT_NULL(baselineAligned);
    *baselineAligned = ExtractEditLayout::IsBaselineAligned();
    return NOERROR;
}

ECode CExtractEditLayout::SetBaselineAligned(
    /* [in] */ Boolean baselineAligned)
{
    return ExtractEditLayout::SetBaselineAligned(baselineAligned);
}

ECode CExtractEditLayout::IsMeasureWithLargestChildEnabled(
    /* [out] */ Boolean* enabled)
{
    VALIDATE_NOT_NULL(enabled);
    *enabled = ExtractEditLayout::IsMeasureWithLargestChildEnabled();
    return NOERROR;
}

ECode CExtractEditLayout::SetMeasureWithLargestChildEnabled(
    /* [in] */ Boolean enabled)
{
    return ExtractEditLayout::SetMeasureWithLargestChildEnabled(enabled);
}

ECode CExtractEditLayout::GetBaselineAlignedChildIndex(
    /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index);
    *index = ExtractEditLayout::GetBaselineAlignedChildIndex();
    return NOERROR;
}

ECode CExtractEditLayout::SetBaselineAlignedChildIndex(
    /* [in] */ Int32 index)
{
    return ExtractEditLayout::SetBaselineAlignedChildIndex(index);
}

ECode CExtractEditLayout::GetWeightSum(
    /* [out] */ Float* weightSum)
{
    VALIDATE_NOT_NULL(weightSum);
    *weightSum = ExtractEditLayout::GetWeightSum();
    return NOERROR;
}

ECode CExtractEditLayout::SetWeightSum(
    /* [in] */ Float weightSum)
{
    return ExtractEditLayout::SetWeightSum(weightSum);
}

ECode CExtractEditLayout::SetOrientation(
    /* [in] */ Int32 orientation)
{
    return ExtractEditLayout::SetOrientation(orientation);
}

ECode CExtractEditLayout::GetOrientation(
    /* [out] */ Int32* orientation)
{
    VALIDATE_NOT_NULL(orientation);
    *orientation = ExtractEditLayout::GetOrientation();
    return NOERROR;
}

ECode CExtractEditLayout::SetGravity(
    /* [in] */ Int32 gravity)
{
    return ExtractEditLayout::SetGravity(gravity);
}

ECode CExtractEditLayout::SetHorizontalGravity(
    /* [in] */ Int32 horizontalGravity)
{
    return ExtractEditLayout::SetHorizontalGravity(horizontalGravity);
}

ECode CExtractEditLayout::SetVerticalGravity(
    /* [in] */ Int32 verticalGravity)
{
    return ExtractEditLayout::SetVerticalGravity(verticalGravity);
}

} // namespace InputMethodService
} // namespace Droid
} // namespace Elastos
