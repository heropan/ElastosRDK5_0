
#include "view/menu/ActionMenuPresenter.h"
#include "view/menu/MenuItemImpl.h"
#include "view/menu/ActionMenuView.h"
#include "view/menu/CActionMenuView.h"
#include "view/menu/SubMenuBuilderBase.h"
#include "view/menu/CActionBarPolicy.h"
#include "view/menu/CActionMenuPresenterSavedState.h"
#include "view/ActionProvider.h"
#include "view/SoundEffectConstants.h"
#include "R.h"

using Elastos::Droid::R;
using Elastos::Core::EIID_IRunnable;
using Elastos::Droid::Graphics::Drawable::EIID_IDrawableCallback;
using Elastos::Droid::Graphics::Drawable::IDrawableCallback;
using Elastos::Droid::View::Accessibility::EIID_IAccessibilityEvent;
using Elastos::Droid::Widget::EIID_IImageView;
using Elastos::Droid::Widget::EIID_IImageButton;

namespace Elastos {
namespace Droid {
namespace View {
namespace Menu {

String ActionMenuPresenter::TAG("ActionMenuPresenter");

ActionMenuPresenter::_OverflowMenuButton::_OverflowMenuButton(
    /* [in] */ ActionMenuPresenter* host)
    : mHost(host)
{}

Boolean ActionMenuPresenter::_OverflowMenuButton::PerformClick()
{
    if (ImageButton::PerformClick()) {
        return TRUE;
    }

    PlaySoundEffect(SoundEffectConstants::CLICK);
    mHost->ShowOverflowMenu();
    return TRUE;
}

IVIEW_METHODS_IMPL(ActionMenuPresenter::OverflowMenuButton, ActionMenuPresenter::_OverflowMenuButton);

IDRAWABLECALLBACK_METHODS_IMPL(ActionMenuPresenter::OverflowMenuButton, ActionMenuPresenter::_OverflowMenuButton);

IKEYEVENTCALLBACK_METHODS_IMPL(ActionMenuPresenter::OverflowMenuButton, ActionMenuPresenter::_OverflowMenuButton);

IACCESSIBILITYEVENTSOURCE_METHODS_IMPL(ActionMenuPresenter::OverflowMenuButton, ActionMenuPresenter::_OverflowMenuButton);

ActionMenuPresenter::OverflowMenuButton::OverflowMenuButton(
    /* [in] */ IContext* context,
    /* [in] */ ActionMenuPresenter* host)
    : _OverflowMenuButton(host)
{
    ImageButton::Init(context, NULL, R::attr::actionOverflowButtonStyle);

    SetClickable(TRUE);
    SetFocusable(TRUE);
    SetVisibility(VISIBLE);
    SetEnabled(TRUE);
}

void ActionMenuPresenter::OverflowMenuButton::OnMeasure(
    /* [in] */ Int32 widthMeasureSpec,
    /* [in] */ Int32 heightMeasureSpec)
{
    if (MeasureSpec::GetMode(heightMeasureSpec) == MeasureSpec::AT_MOST) {
        // Fill available height
        heightMeasureSpec = MeasureSpec::MakeMeasureSpec(
                MeasureSpec::GetSize(heightMeasureSpec), MeasureSpec::EXACTLY);
    }

    ImageButton::OnMeasure(widthMeasureSpec, heightMeasureSpec);
}

PInterface ActionMenuPresenter::OverflowMenuButton::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)(IImageButton*)this;
    }
    else if (riid == EIID_IView) {
        return (IView*)this;
    }
    else if (riid == EIID_IImageView) {
        return (IImageButton*)this;
    }
    else if (riid == EIID_IImageButton) {
        return (IImageButton*)this;
    }
    else if (riid == EIID_IDrawableCallback) {
        return (IDrawableCallback*)this;
    }
    else if (riid == EIID_IKeyEventCallback) {
        return (IKeyEventCallback*)this;
    }
    else if (riid == EIID_IAccessibilityEvent) {
        return (IAccessibilityEvent*)this;
    }
    else if (riid == EIID_IActionMenuChildView) {
        return (IActionMenuChildView*)this;
    }
    else if (riid == EIID_IWeakReferenceSource) {
        return (IWeakReferenceSource*)this;
    }
    else if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)(ImageButton*)this);
    }

    return NULL;
}

UInt32 ActionMenuPresenter::OverflowMenuButton::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 ActionMenuPresenter::OverflowMenuButton::Release()
{
    return ElRefBase::Release();
}

ECode ActionMenuPresenter::OverflowMenuButton::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

ECode ActionMenuPresenter::OverflowMenuButton::GetWeakReference(
    /* [out] */ IWeakReference** weakReference)
{
    VALIDATE_NOT_NULL(weakReference)
    *weakReference = new WeakReferenceImpl(Probe(EIID_IInterface), CreateWeak(this));
    REFCOUNT_ADD(*weakReference)
    return NOERROR;
}

ECode ActionMenuPresenter::OverflowMenuButton::NeedsDividerBefore(
    /* [out] */ Boolean* need)
{
    assert(need != NULL);
    *need = FALSE;
    return NOERROR;
}

ECode ActionMenuPresenter::OverflowMenuButton::NeedsDividerAfter(
    /* [out] */ Boolean* need)
{
    assert(need != NULL);
    *need = FALSE;
    return NOERROR;
}

ECode ActionMenuPresenter::OverflowMenuButton::GetAdjustViewBounds(
        /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);
    *res = _OverflowMenuButton::GetAdjustViewBounds();
    return NOERROR;
}

ECode ActionMenuPresenter::OverflowMenuButton::SetAdjustViewBounds(
        /* [in] */ Boolean adjustViewBounds)
{
    return _OverflowMenuButton::SetAdjustViewBounds(adjustViewBounds);
}

ECode ActionMenuPresenter::OverflowMenuButton::GetMaxWidth(
        /* [out] */ Int32* width)
{
    VALIDATE_NOT_NULL(width);
    *width = _OverflowMenuButton::GetMaxWidth();
    return NOERROR;
}

ECode ActionMenuPresenter::OverflowMenuButton::SetMaxWidth(
        /* [in] */ Int32 maxWidth)
{
    return _OverflowMenuButton::SetMaxWidth(maxWidth);
}

ECode ActionMenuPresenter::OverflowMenuButton::GetMaxHeight(
        /* [out] */ Int32* height)
{
    VALIDATE_NOT_NULL(height);
    *height = _OverflowMenuButton::GetMaxHeight();
    return NOERROR;
}

ECode ActionMenuPresenter::OverflowMenuButton::SetMaxHeight(
        /* [in] */ Int32 maxHeight)
{
    return _OverflowMenuButton::SetMaxHeight(maxHeight);
}

ECode ActionMenuPresenter::OverflowMenuButton::GetDrawable(
        /* [out] */ IDrawable** drawable)
{
    VALIDATE_NOT_NULL(drawable);
    AutoPtr<IDrawable> dTemp = _OverflowMenuButton::GetDrawable();
    *drawable = dTemp;
    REFCOUNT_ADD(*drawable);
    return NOERROR;
}

ECode ActionMenuPresenter::OverflowMenuButton::SetImageResource(
        /* [in] */ Int32 resId)
{
    return _OverflowMenuButton::SetImageResource(resId);
}

ECode ActionMenuPresenter::OverflowMenuButton::SetImageURI(
        /* [in] */ IUri* uri)
{
    return _OverflowMenuButton::SetImageURI(uri);
}

ECode ActionMenuPresenter::OverflowMenuButton::SetImageDrawable(
        /* [in] */ IDrawable* drawable)
{
    return _OverflowMenuButton::SetImageDrawable(drawable);
}

ECode ActionMenuPresenter::OverflowMenuButton::SetImageBitmap(
        /* [in] */ IBitmap* bm)
{
    return _OverflowMenuButton::SetImageBitmap(bm);
}

ECode ActionMenuPresenter::OverflowMenuButton::SetImageState(
        /* [in] */ ArrayOf<Int32>* state,
        /* [in] */ Boolean mg)
{
    return _OverflowMenuButton::SetImageState(state, mg);
}

ECode ActionMenuPresenter::OverflowMenuButton::SetImageLevel(
        /* [in] */ Int32 level)
{
    return _OverflowMenuButton::SetImageLevel(level);
}

 ECode ActionMenuPresenter::OverflowMenuButton::SetScaleType(
        /* [in] */ ImageViewScaleType scaleType)
 {
    return _OverflowMenuButton::SetScaleType(scaleType);
 }

ECode ActionMenuPresenter::OverflowMenuButton::GetScaleType(
        /* [out] */ ImageViewScaleType* scaleType)
{
    VALIDATE_NOT_NULL(scaleType);
    *scaleType = _OverflowMenuButton::GetScaleType();
    return NOERROR;
}

ECode ActionMenuPresenter::OverflowMenuButton::GetImageMatrix(
        /* [out] */ IMatrix** matrix)
{
    VALIDATE_NOT_NULL(matrix);
    AutoPtr<IMatrix> mTemp = _OverflowMenuButton::GetImageMatrix();
    *matrix = mTemp;
    REFCOUNT_ADD(*matrix);
    return NOERROR;
}

ECode ActionMenuPresenter::OverflowMenuButton::SetImageMatrix(
        /* [in] */ IMatrix* matrix)
{
    return _OverflowMenuButton::SetImageMatrix(matrix);
}

ECode ActionMenuPresenter::OverflowMenuButton::GetCropToPadding(
        /* [out] */ Boolean* padding)
{
    VALIDATE_NOT_NULL(padding);
    *padding = _OverflowMenuButton::GetCropToPadding();
    return NOERROR;
}

ECode ActionMenuPresenter::OverflowMenuButton::SetCropToPadding(
        /* [in] */ Boolean cropToPadding)
{
    return _OverflowMenuButton::SetCropToPadding(cropToPadding);
}

ECode ActionMenuPresenter::OverflowMenuButton::SetBaseline(
        /* [in] */ Int32 baseline)
{
    return _OverflowMenuButton::SetBaseline(baseline);
}

ECode ActionMenuPresenter::OverflowMenuButton::SetBaselineAlignBottom(
        /* [in] */ Boolean aligned)
{
    return _OverflowMenuButton::SetBaselineAlignBottom(aligned);
}

ECode ActionMenuPresenter::OverflowMenuButton::GetBaselineAlignBottom(
        /* [out] */ Boolean* aligned)
{
    VALIDATE_NOT_NULL(aligned);
    *aligned = _OverflowMenuButton::GetBaselineAlignBottom();
    return NOERROR;
}

ECode ActionMenuPresenter::OverflowMenuButton::SetColorFilter(
        /* [in] */ Int32 color)
{
    return _OverflowMenuButton::SetColorFilter(color);
}

ECode ActionMenuPresenter::OverflowMenuButton::SetColorFilter(
        /* [in] */ Int32 color,
        /* [in] */ PorterDuffMode mode)
{
    return _OverflowMenuButton::SetColorFilter(color, mode);
}

ECode ActionMenuPresenter::OverflowMenuButton::ClearColorFilter()
{
    return _OverflowMenuButton::ClearColorFilter();
}

ECode ActionMenuPresenter::OverflowMenuButton::GetColorFilter(
        /* [out] */ IColorFilter** filter)
{
    VALIDATE_NOT_NULL(filter);
    AutoPtr<IColorFilter> fTemp = _OverflowMenuButton::GetColorFilter();
    *filter = fTemp;
    REFCOUNT_ADD(*filter);
    return NOERROR;
}

ECode ActionMenuPresenter::OverflowMenuButton::SetColorFilter(
        /* [in] */ IColorFilter* cf)
{
    return _OverflowMenuButton::SetColorFilter(cf);
}

ECode ActionMenuPresenter::OverflowMenuButton::GetImageAlpha(
        /* [out] */ Int32* alpha)
{
    VALIDATE_NOT_NULL(alpha);
    *alpha = _OverflowMenuButton::GetImageAlpha();
    return NOERROR;
}

ECode ActionMenuPresenter::OverflowMenuButton::SetImageAlpha(
        /* [in] */ Int32 alpha)
{
    return _OverflowMenuButton::SetImageAlpha(alpha);
}

ECode ActionMenuPresenter::OverflowMenuButton::SetAlpha(
        /* [in] */ Int32 alpha)
{
    return _OverflowMenuButton::SetAlpha(alpha);
}

ActionMenuPresenter::OverflowPopup::OverflowPopup(
    /* [in] */ IContext* context,
    /* [in] */ IMenuBuilder* menu,
    /* [in] */ IView* anchorView,
    /* [in] */ Boolean overflowOnly,
    /* [in] */ ActionMenuPresenter* host)
    : mHost(host)
{
    MenuPopupHelper::Init(context, menu, anchorView, overflowOnly);
    SetCallback(mHost->mPopupPresenterCallback);
}

ECode ActionMenuPresenter::OverflowPopup::GetWeakReference(
    /* [out] */ IWeakReference** weakReference)
{
    VALIDATE_NOT_NULL(weakReference)
    *weakReference = new WeakReferenceImpl(THIS_PROBE(IInterface), CreateWeak(this));
    REFCOUNT_ADD(*weakReference)
    return NOERROR;
}

PInterface ActionMenuPresenter::OverflowPopup::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)(IMenuPopupHelper*)this;
    }
    else if (riid == EIID_IMenuPopupHelper) {
        return (IMenuPopupHelper*)this;
    }
    else if (riid == EIID_IAdapterViewOnItemClickListener) {
        return (IAdapterViewOnItemClickListener*)this;
    }
    else if (riid == EIID_IViewOnKeyListener) {
        return (IViewOnKeyListener*)this;
    }
    else if (riid == EIID_IOnGlobalLayoutListener) {
        return (IOnGlobalLayoutListener*)this;
    }
    else if (riid == EIID_IPopupWindowOnDismissListener) {
        return (IPopupWindowOnDismissListener*)this;
    }
    else if (riid == EIID_IViewOnAttachStateChangeListener) {
        return (IViewOnAttachStateChangeListener*)this;
    }
    else if (riid == EIID_IWeakReferenceSource) {
        return (IWeakReferenceSource*)this;
    }
    else if (riid == EIID_IMenuPresenter) {
        return (IMenuPresenter*)this;
    }
    return NULL;
}

UInt32 ActionMenuPresenter::OverflowPopup::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 ActionMenuPresenter::OverflowPopup::Release()
{
    return ElRefBase::Release();
}

ECode ActionMenuPresenter::OverflowPopup::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

ECode ActionMenuPresenter::OverflowPopup::OnDismiss()
{
    MenuPopupHelper::OnDismiss();
    mHost->mMenu->Close();
    mHost->mOverflowPopup = NULL;
    return NOERROR;
}

ECode ActionMenuPresenter::OverflowPopup::SetAnchorView(
    /* [in] */ IView* anchor)
{
    return MenuPopupHelper::SetAnchorView(anchor);
}

ECode ActionMenuPresenter::OverflowPopup::SetForceShowIcon(
    /* [in] */ Boolean forceShow)
{
    return MenuPopupHelper::SetForceShowIcon(forceShow);
}

ECode ActionMenuPresenter::OverflowPopup::Show()
{
    return MenuPopupHelper::Show();
}

ECode ActionMenuPresenter::OverflowPopup::TryShow(
    /* [out] */ Boolean* rst)
{
    VALIDATE_NOT_NULL(rst);
    *rst = MenuPopupHelper::TryShow();
    return NOERROR;
}

ECode ActionMenuPresenter::OverflowPopup::Dismiss()
{
    return MenuPopupHelper::Dismiss();
}

ECode ActionMenuPresenter::OverflowPopup::IsShowing(
    /* [out] */ Boolean* rst)
{
    VALIDATE_NOT_NULL(rst);
    *rst = MenuPopupHelper::IsShowing();
    return NOERROR;
}

ECode ActionMenuPresenter::OverflowPopup::OnItemClick(
    /* [in] */ IAdapterView* parent,
    /* [in] */ IView* view,
    /* [in] */ Int32 position,
    /* [in] */ Int64 id)
{
    return MenuPopupHelper::OnItemClick(parent, view, position, id);
}

ECode ActionMenuPresenter::OverflowPopup::OnKey(
    /* [in] */ IView* v,
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return MenuPopupHelper::OnKey(v, keyCode, event, result);
}

ECode ActionMenuPresenter::OverflowPopup::OnGlobalLayout()
{
    return MenuPopupHelper::OnGlobalLayout();
}

ECode ActionMenuPresenter::OverflowPopup::OnViewAttachedToWindow(
            /* [in] */ IView* v)
{
    return MenuPopupHelper::OnViewAttachedToWindow(v);
}

ECode ActionMenuPresenter::OverflowPopup::OnViewDetachedFromWindow(
    /* [in] */ IView* v)
{
    return MenuPopupHelper::OnViewDetachedFromWindow(v);
}

ECode ActionMenuPresenter::OverflowPopup::InitForMenu(
    /* [in] */ IContext* context,
    /* [in] */ IMenuBuilder* menu)
{
    return MenuPopupHelper::InitForMenu(context, menu);
}

ECode ActionMenuPresenter::OverflowPopup::GetMenuView(
    /* [in] */ IViewGroup* root,
    /* [out] */ IMenuView** view)
{
    return MenuPopupHelper::GetMenuView(root, view);
}

ECode ActionMenuPresenter::OverflowPopup::UpdateMenuView(
    /* [in] */ Boolean cleared)
{
    return MenuPopupHelper::UpdateMenuView(cleared);
}

ECode ActionMenuPresenter::OverflowPopup::SetCallback(
    /* [in] */ IMenuPresenterCallback* cb)
{
    return MenuPopupHelper::SetCallback(cb);
}

ECode ActionMenuPresenter::OverflowPopup::OnSubMenuSelected(
    /* [in] */ ISubMenuBuilder* subMenu,
    /* [out] */ Boolean* handled)
{
    VALIDATE_NOT_NULL(handled);
    return MenuPopupHelper::OnSubMenuSelected(subMenu, handled);
}

ECode ActionMenuPresenter::OverflowPopup::OnCloseMenu(
    /* [in] */ IMenuBuilder* menu,
    /* [in] */ Boolean allMenusAreClosing)
{
    return MenuPopupHelper::OnCloseMenu(menu, allMenusAreClosing);
}

ECode ActionMenuPresenter::OverflowPopup::FlagActionItems(
    /* [out] */ Boolean* shown)
{
    VALIDATE_NOT_NULL(shown);
    return MenuPopupHelper::FlagActionItems(shown);
}

ECode ActionMenuPresenter::OverflowPopup::ExpandItemActionView(
    /* [in] */ IMenuBuilder* menu,
    /* [in] */ IMenuItemImpl* item,
    /* [out] */ Boolean* expanded)
{
    VALIDATE_NOT_NULL(expanded);
    return MenuPopupHelper::ExpandItemActionView(menu, item, expanded);
}

ECode ActionMenuPresenter::OverflowPopup::CollapseItemActionView(
    /* [in] */ IMenuBuilder* menu,
    /* [in] */ IMenuItemImpl* item,
    /* [out] */ Boolean* collapsed)
{
    VALIDATE_NOT_NULL(collapsed);
    return MenuPopupHelper::CollapseItemActionView(menu, item, collapsed);
}

ECode ActionMenuPresenter::OverflowPopup::GetId(
    /* [out] */ Int32* id)
{
    VALIDATE_NOT_NULL(id);
    return MenuPopupHelper::GetId(id);
}

ECode ActionMenuPresenter::OverflowPopup::OnSaveInstanceState(
    /* [out] */ IParcelable** pa)
{
    VALIDATE_NOT_NULL(pa);
    return MenuPopupHelper::OnSaveInstanceState(pa);
}

ECode ActionMenuPresenter::OverflowPopup::OnRestoreInstanceState(
    /* [in] */ IParcelable* state)
{
    return MenuPopupHelper::OnRestoreInstanceState(state);
}

ActionMenuPresenter::ActionButtonSubmenu::ActionButtonSubmenu(
    /* [in] */ IContext* context,
    /* [in] */ ISubMenuBuilder* subMenu,
    /* [in] */ ActionMenuPresenter* host)
    : MenuPopupHelper(context, subMenu)
    , mHost(host)
{
    mSubMenu = subMenu;

    AutoPtr<IMenuItemImpl> item;
    AutoPtr<ISubMenu> sm = ISubMenu::Probe(subMenu);
    sm->GetItem((IMenuItem**)&item);

    Boolean tmp = FALSE;
    if (!(item->IsActionButton(&tmp), tmp)) {
        // Give a reasonable anchor to nested submenus.
        SetAnchorView(mHost->mOverflowButton == NULL ? IView::Probe(mHost->mMenuView) : mHost->mOverflowButton.Get());
    }

    SetCallback(mHost->mPopupPresenterCallback);

    Boolean preserveIconSpacing = FALSE;
    Int32 count = 0;
    subMenu->GetSize(&count);
    for (Int32 i = 0; i < count; i++) {
        AutoPtr<IMenuItem> childItem;
        subMenu->GetItem(i, (IMenuItem**)&childItem);

        AutoPtr<IDrawable> icon;
        if ((childItem->IsVisible(&tmp), tmp) && (childItem->GetIcon((IDrawable**)&icon), icon) != NULL) {
            preserveIconSpacing = TRUE;
            break;
        }
    }
    SetForceShowIcon(preserveIconSpacing);
}

ECode ActionMenuPresenter::ActionButtonSubmenu::GetWeakReference(
    /* [out] */ IWeakReference** weakReference)
{
    VALIDATE_NOT_NULL(weakReference)
    *weakReference = new WeakReferenceImpl(THIS_PROBE(IInterface), CreateWeak(this));
    REFCOUNT_ADD(*weakReference)
    return NOERROR;
}

PInterface ActionMenuPresenter::ActionButtonSubmenu::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)(IMenuPopupHelper*)this;
    }
    else if (riid == EIID_IMenuPopupHelper) {
        return (IMenuPopupHelper*)this;
    }
    else if (riid == EIID_IAdapterViewOnItemClickListener) {
        return (IAdapterViewOnItemClickListener*)this;
    }
    else if (riid == EIID_IViewOnKeyListener) {
        return (IViewOnKeyListener*)this;
    }
    else if (riid == EIID_IOnGlobalLayoutListener) {
        return (IOnGlobalLayoutListener*)this;
    }
    else if (riid == EIID_IPopupWindowOnDismissListener) {
        return (IPopupWindowOnDismissListener*)this;
    }
    else if (riid == EIID_IViewOnAttachStateChangeListener) {
        return (IViewOnAttachStateChangeListener*)this;
    }
    else if (riid == EIID_IWeakReferenceSource) {
        return (IWeakReferenceSource*)this;
    }
    else if (riid == EIID_IMenuPresenter) {
        return (IMenuPresenter*)this;
    }
    return NULL;
}

UInt32 ActionMenuPresenter::ActionButtonSubmenu::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 ActionMenuPresenter::ActionButtonSubmenu::Release()
{
    return ElRefBase::Release();
}

ECode ActionMenuPresenter::ActionButtonSubmenu::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

ECode ActionMenuPresenter::ActionButtonSubmenu::OnDismiss()
{
    MenuPopupHelper::OnDismiss();
    mHost->mActionButtonPopup = NULL;
    mHost->mOpenSubMenuId = 0;
    return NOERROR;
}

ECode ActionMenuPresenter::ActionButtonSubmenu::SetAnchorView(
    /* [in] */ IView* anchor)
{
    return MenuPopupHelper::SetAnchorView(anchor);
}

ECode ActionMenuPresenter::ActionButtonSubmenu::SetForceShowIcon(
    /* [in] */ Boolean forceShow)
{
    return MenuPopupHelper::SetForceShowIcon(forceShow);
}

ECode ActionMenuPresenter::ActionButtonSubmenu::Show()
{
    return MenuPopupHelper::Show();
}

ECode ActionMenuPresenter::ActionButtonSubmenu::TryShow(
    /* [out] */ Boolean* rst)
{
    VALIDATE_NOT_NULL(rst);
    *rst = MenuPopupHelper::TryShow();
    return NOERROR;
}

ECode ActionMenuPresenter::ActionButtonSubmenu::Dismiss()
{
    return MenuPopupHelper::Dismiss();
}

ECode ActionMenuPresenter::ActionButtonSubmenu::IsShowing(
    /* [out] */ Boolean* rst)
{
    VALIDATE_NOT_NULL(rst);
    *rst = MenuPopupHelper::IsShowing();
    return NOERROR;
}

ECode ActionMenuPresenter::ActionButtonSubmenu::OnItemClick(
    /* [in] */ IAdapterView* parent,
    /* [in] */ IView* view,
    /* [in] */ Int32 position,
    /* [in] */ Int64 id)
{
    return MenuPopupHelper::OnItemClick(parent, view, position, id);
}

ECode ActionMenuPresenter::ActionButtonSubmenu::OnKey(
    /* [in] */ IView* v,
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return MenuPopupHelper::OnKey(v, keyCode, event, result);
}

ECode ActionMenuPresenter::ActionButtonSubmenu::OnGlobalLayout()
{
    return MenuPopupHelper::OnGlobalLayout();
}

ECode ActionMenuPresenter::ActionButtonSubmenu::OnViewAttachedToWindow(
    /* [in] */ IView* v)
{
    return MenuPopupHelper::OnViewAttachedToWindow(v);
}

ECode ActionMenuPresenter::ActionButtonSubmenu::OnViewDetachedFromWindow(
    /* [in] */ IView* v)
{
    return MenuPopupHelper::OnViewDetachedFromWindow(v);
}

ECode ActionMenuPresenter::ActionButtonSubmenu::InitForMenu(
    /* [in] */ IContext* context,
    /* [in] */ IMenuBuilder* menu)
{
    return MenuPopupHelper::InitForMenu(context, menu);
}

ECode ActionMenuPresenter::ActionButtonSubmenu::GetMenuView(
    /* [in] */ IViewGroup* root,
    /* [out] */ IMenuView** view)
{
    return MenuPopupHelper::GetMenuView(root, view);
}

ECode ActionMenuPresenter::ActionButtonSubmenu::UpdateMenuView(
    /* [in] */ Boolean cleared)
{
    return MenuPopupHelper::UpdateMenuView(cleared);
}

ECode ActionMenuPresenter::ActionButtonSubmenu::SetCallback(
    /* [in] */ IMenuPresenterCallback* cb)
{
    return MenuPopupHelper::SetCallback(cb);
}

ECode ActionMenuPresenter::ActionButtonSubmenu::OnSubMenuSelected(
    /* [in] */ ISubMenuBuilder* subMenu,
    /* [out] */ Boolean* handled)
{
    VALIDATE_NOT_NULL(handled);
    return MenuPopupHelper::OnSubMenuSelected(subMenu, handled);
}

ECode ActionMenuPresenter::ActionButtonSubmenu::OnCloseMenu(
    /* [in] */ IMenuBuilder* menu,
    /* [in] */ Boolean allMenusAreClosing)
{
    return MenuPopupHelper::OnCloseMenu(menu, allMenusAreClosing);
}

ECode ActionMenuPresenter::ActionButtonSubmenu::FlagActionItems(
    /* [out] */ Boolean* shown)
{
    VALIDATE_NOT_NULL(shown);
    return MenuPopupHelper::FlagActionItems(shown);
}

ECode ActionMenuPresenter::ActionButtonSubmenu::ExpandItemActionView(
    /* [in] */ IMenuBuilder* menu,
    /* [in] */ IMenuItemImpl* item,
    /* [out] */ Boolean* expanded)
{
    VALIDATE_NOT_NULL(expanded);
    return MenuPopupHelper::ExpandItemActionView(menu, item, expanded);
}

ECode ActionMenuPresenter::ActionButtonSubmenu::CollapseItemActionView(
    /* [in] */ IMenuBuilder* menu,
    /* [in] */ IMenuItemImpl* item,
    /* [out] */ Boolean* collapsed)
{
    VALIDATE_NOT_NULL(collapsed);
    return MenuPopupHelper::CollapseItemActionView(menu, item, collapsed);
}

ECode ActionMenuPresenter::ActionButtonSubmenu::GetId(
    /* [out] */ Int32* id)
{
    VALIDATE_NOT_NULL(id);
    return MenuPopupHelper::GetId(id);
}

ECode ActionMenuPresenter::ActionButtonSubmenu::OnSaveInstanceState(
    /* [out] */ IParcelable** pa)
{
    VALIDATE_NOT_NULL(pa);
    return MenuPopupHelper::OnSaveInstanceState(pa);
}

ECode ActionMenuPresenter::ActionButtonSubmenu::OnRestoreInstanceState(
    /* [in] */ IParcelable* state)
{
    return MenuPopupHelper::OnRestoreInstanceState(state);
}

ActionMenuPresenter::PopupPresenterCallback::PopupPresenterCallback(
    /* [in] */ ActionMenuPresenter* host)
    : mHost(host)
{}

CAR_INTERFACE_IMPL(ActionMenuPresenter::PopupPresenterCallback, IMenuPresenterCallback)

ECode ActionMenuPresenter::PopupPresenterCallback::OnOpenSubMenu(
    /* [in] */ IMenuBuilder* subMenu,
    /* [out] */ Boolean* result)
{
    assert(result != NULL);
    *result = FALSE;
    if (subMenu == NULL) {
        return NOERROR;
    }

    AutoPtr<IMenuItem> item;
    AutoPtr<ISubMenu> sm = ISubMenu::Probe(subMenu);
    sm->GetItem((IMenuItem**)&item);
    item->GetItemId(&(mHost->mOpenSubMenuId));
    return NOERROR;
}

ECode ActionMenuPresenter::PopupPresenterCallback::OnCloseMenu(
    /* [in] */ IMenuBuilder* menu,
    /* [in] */ Boolean allMenusAreClosing)
{
    if (ISubMenuBuilder::Probe(menu) != NULL) {
        AutoPtr<IMenuBuilder> root;
        ISubMenuBuilder::Probe(menu)->GetRootMenu((IMenuBuilder**)&root);
        root->Close(FALSE);
    }

    return NOERROR;
}

CAR_INTERFACE_IMPL(ActionMenuPresenter::OpenOverflowRunnable, IRunnable)

ActionMenuPresenter::OpenOverflowRunnable::OpenOverflowRunnable(
    /* [in] */ OverflowPopup* popup,
    /* [in] */ ActionMenuPresenter* host)
    : mPopup(popup)
    , mHost(host)
{}

ECode ActionMenuPresenter::OpenOverflowRunnable::Run()
{
    mHost->mMenu->ChangeMenuMode();
    AutoPtr<IView> menuView = IView::Probe(mHost->mMenuView);

    AutoPtr<IBinder> token;
    Boolean tryShow = FALSE;
    if (menuView != NULL && (menuView->GetWindowToken((IBinder**)&token), token) != NULL && (mPopup->TryShow(&tryShow), tryShow)) {
        mHost->mOverflowPopup = mPopup;
    }

    mHost->mPostedOpenRunnable = NULL;

    return NOERROR;
}

ActionMenuPresenter::ActionMenuPresenter()
{
    mPopupPresenterCallback = new PopupPresenterCallback(this);
}

ActionMenuPresenter::ActionMenuPresenter(
    /* [in] */ IContext* context)
{
    BaseMenuPresenter::Init(context, R::layout::action_menu_layout,
            R::layout::action_menu_item_layout);

    mPopupPresenterCallback = new PopupPresenterCallback(this);
}

//@Override
ECode ActionMenuPresenter::InitForMenu(
    /* [in] */ IContext* context,
    /* [in] */ IMenuBuilder* menu)
{
    BaseMenuPresenter::InitForMenu(context, menu);

    AutoPtr<IResources> res;
    context->GetResources((IResources**)&res);


    AutoPtr<IActionBarPolicy> abp;
    CActionBarPolicy::New(context, (IActionBarPolicy**)&abp);
    if (!mReserveOverflowSet) {
        abp->ShowsOverflowMenuButton(&mReserveOverflow);
    }

    if (!mWidthLimitSet) {
        abp->GetEmbeddedMenuWidthLimit(&mWidthLimit);
    }

    // Measure for initial configuration
    if (!mMaxItemsSet) {
        abp->GetMaxActionButtons(&mMaxItems);
    }

    Int32 width = mWidthLimit;
    if (mReserveOverflow) {
        if (mOverflowButton == NULL) {
            mOverflowButton = new OverflowMenuButton(mSystemContext, this);
            const Int32 spec = View::MeasureSpec::MakeMeasureSpec(0, View::MeasureSpec::UNSPECIFIED);
            mOverflowButton->Measure(spec, spec);
        }

        Int32 v = 0;
        mOverflowButton->GetMeasuredWidth(&v);
        width -= v;
    } else {
        mOverflowButton = NULL;
    }

    mActionItemWidthLimit = width;

    AutoPtr<IDisplayMetrics> metrics;
    res->GetDisplayMetrics((IDisplayMetrics**)&metrics);
    Float density = 0.f;
    metrics->GetDensity(&density);
    mMinCellSize = (Int32) (ActionMenuView::MIN_CELL_SIZE * density);

    // Drop a scrap view as it may no longer reflect the proper context/config.
    mScrapActionButtonView = NULL;
    return NOERROR;
}

ECode ActionMenuPresenter::OnConfigurationChanged(
    /* [in] */ IConfiguration* newConfig)
{
    if (!mMaxItemsSet) {
        AutoPtr<IResources> res;
        mContext->GetResources((IResources**)&res);
        res->GetInteger(R::integer::max_action_buttons, &mMaxItems);
    }

    if (mMenu != NULL) {
        MenuBuilderBase* base = reinterpret_cast<MenuBuilderBase*>(mMenu->Probe(EIID_MenuBuilderBase));
        assert(base != NULL);
        base->OnItemsChanged(TRUE);
    }

    return NOERROR;
}

void ActionMenuPresenter::SetWidthLimit(
    /* [in] */ Int32 width,
    /* [in] */ Boolean strict)
{
    mWidthLimit = width;
    mStrictWidthLimit = strict;
    mWidthLimitSet = TRUE;
}

void ActionMenuPresenter::SetReserveOverflow(
    /* [in] */ Boolean reserveOverflow)
{
    mReserveOverflow = reserveOverflow;
    mReserveOverflowSet = TRUE;
}

void ActionMenuPresenter::SetItemLimit(
    /* [in] */ Int32 itemCount)
{
    mMaxItems = itemCount;
    mMaxItemsSet = TRUE;
}

void ActionMenuPresenter::SetExpandedActionViewsExclusive(
    /* [in] */ Boolean isExclusive)
{
    mExpandedActionViewsExclusive = isExclusive;
}

ECode ActionMenuPresenter::GetMenuView(
    /* [in] */ IViewGroup* root,
    /* [out] */ IMenuView** view)
{
    assert(view != NULL);
    BaseMenuPresenter::GetMenuView(root, view);
    AutoPtr<IActionMenuView> temp = IActionMenuView::Probe(*view);
    temp->SetPresenter((IActionMenuPresenter*)this->Probe(EIID_IActionMenuPresenter));
    return NOERROR;
}

ECode ActionMenuPresenter::GetItemView(
    /* [in] */ IMenuItemImpl* item,
    /* [in] */ IView* convertView,
    /* [in] */ IViewGroup* parent,
    /* [out] */ IView** view)
{
    assert(view != NULL && item != NULL);
    AutoPtr<IView> actionView;
    item->GetActionView((IView**)&actionView);

    Boolean has = FALSE;
    if (actionView == NULL || (item->HasCollapsibleActionView(&has), has)) {
        if (IActionMenuItemView::Probe(convertView) == NULL) {
            convertView = NULL;
        }

        actionView = BaseMenuPresenter::GetItemView(item, convertView, parent);
    }

    Boolean expanded = FALSE;
    actionView->SetVisibility((item->IsActionViewExpanded(&expanded), expanded) ? IView::GONE : IView::VISIBLE);

    ActionMenuView* menuParent = reinterpret_cast<ActionMenuView*>(parent->Probe(EIID_ActionMenuView));
    AutoPtr<IViewGroupLayoutParams> lp;
    actionView->GetLayoutParams((IViewGroupLayoutParams**)&lp);
    if (!menuParent->CheckLayoutParams(lp)) {
        actionView->SetLayoutParams(menuParent->GenerateLayoutParams(lp));
    }

    *view = actionView;
    REFCOUNT_ADD(*view);

    return NOERROR;
}

void ActionMenuPresenter::BindItemView(
    /* [in] */ IMenuItemImpl* item,
    /* [in] */ IMenuItemView* itemView)
{
    itemView->Initialize(item, 0);

    AutoPtr<IMenuBuilderItemInvoker> menuView = IMenuBuilderItemInvoker::Probe(mMenuView);
    AutoPtr<IActionMenuItemView> actionItemView = IActionMenuItemView::Probe(itemView);
    actionItemView->SetItemInvoker(menuView);
}

Boolean ActionMenuPresenter::ShouldIncludeItem(
    /* [in] */ Int32 childIndex,
    /* [in] */ IMenuItemImpl* item)
{
    assert(item != NULL);
    Boolean tmp = FALSE;
    return (item->IsActionButton(&tmp), tmp);
}

ECode ActionMenuPresenter::UpdateMenuView(
    /* [in] */ Boolean cleared)
{
    BaseMenuPresenter::UpdateMenuView(cleared);
    AutoPtr< List<AutoPtr<IMenuItemImpl> > > nonActionItems;
    MenuBuilderBase* base = NULL;
    if (mMenu != NULL) {
        base = reinterpret_cast<MenuBuilderBase*>(mMenu->Probe(EIID_MenuBuilderBase));
        assert(base != NULL);

        AutoPtr< List<AutoPtr<IMenuItemImpl> > > actionItems = base->GetActionItems();
        List<AutoPtr<IMenuItemImpl> >::Iterator it = actionItems->Begin();
        for (; it != actionItems->End(); it++) {
            AutoPtr<IActionProvider> provider;
            (*it)->GetActionProvider((IActionProvider**)&provider);
            if (provider != NULL) {
                provider->SetSubUiVisibilityListener(
                    (ISubUiVisibilityListener*)(this->Probe(EIID_ISubUiVisibilityListener)));
            }
        }
        nonActionItems = base->GetNonActionItems();
    }

    Boolean hasOverflow = FALSE;
    if (mReserveOverflow && nonActionItems != NULL) {
        Int32 count = nonActionItems->GetSize();
        if (count == 1) {
            Boolean tmp = FALSE;
            hasOverflow = !((*nonActionItems)[0]->IsActionViewExpanded(&tmp), tmp);
        } else {
            hasOverflow = count > 0;
        }
    }

    AutoPtr<IViewGroup> parent;
    mOverflowButton->GetParent((IViewParent**)&parent);
    if (hasOverflow) {
        if (mOverflowButton == NULL) {
            mOverflowButton = new OverflowMenuButton(mSystemContext, this);
        }

        if (parent.Get() != IViewGroup::Probe(mMenuView)) {
            if (parent != NULL) {
                assert(IViewManager::Probe(parent) != NULL);
                IViewManager::Probe(parent)->RemoveView(mOverflowButton);
            }

            AutoPtr<IActionMenuView> menuView = IActionMenuView::Probe(mMenuView);
            AutoPtr<IViewGroupLayoutParams> lp;
            menuView->GenerateOverflowButtonLayoutParams((IViewGroupLayoutParams**)&lp);
            menuView->AddView(mOverflowButton, lp);
        }
    } else if (mOverflowButton != NULL && parent.Get() == IViewGroup::Probe(mMenuView)) {
        assert(IViewManager::Probe(parent) != NULL);
        IViewManager::Probe(parent)->RemoveView(mOverflowButton);
    }

    AutoPtr<IActionMenuView> menuView = IActionMenuView::Probe(mMenuView);
    menuView->SetOverflowReserved(mReserveOverflow);

    return NOERROR;
}

Boolean ActionMenuPresenter::FilterLeftoverView(
    /* [in] */ IViewGroup* parent,
    /* [in] */ Int32 childIndex)
{
    assert(parent != NULL);
    AutoPtr<IView> child;
    parent->GetChildAt(childIndex, (IView**)&child);
    if (child == mOverflowButton) return FALSE;
    return BaseMenuPresenter::FilterLeftoverView(parent, childIndex);
}

ECode ActionMenuPresenter::OnSubMenuSelected(
    /* [in] */ ISubMenuBuilder* subMenu,
    /* [out] */ Boolean* result)
{
    assert(result != NULL && subMenu != NULL);

    Boolean tmp = FALSE;
    if (!(subMenu->HasVisibleItems(&tmp), tmp)) {
        *result = FALSE;
        return NOERROR;
    }

    AutoPtr<ISubMenuBuilder> topSubMenu = subMenu;
    AutoPtr<IMenu> parentMenu;
    while ((topSubMenu->GetParentMenu((IMenu**)&parentMenu), parentMenu) != mMenu) {
        topSubMenu = ISubMenuBuilder::Probe(parentMenu);
        parentMenu = NULL;
    }

    AutoPtr<IMenuItem> item;
    (ISubMenu::Probe(topSubMenu))->GetItem((IMenuItem**)&item);
    AutoPtr<IView> anchor = FindViewForItem(item);
    if (anchor == NULL) {
        if (mOverflowButton == NULL) {
            *result = FALSE;
            return NOERROR;
        }

        anchor = mOverflowButton;
    }

    item = NULL;
    (ISubMenu::Probe(subMenu))->GetItem((IMenuItem**)&item);
    assert(item != NULL);

    item->GetItemId(&mOpenSubMenuId);
    mActionButtonPopup = new ActionButtonSubmenu(mContext, subMenu, this);
    mActionButtonPopup->SetAnchorView(anchor);
    mActionButtonPopup->Show();

    BaseMenuPresenter::OnSubMenuSelected(subMenu, &tmp);

    *result = TRUE;
    return NOERROR;
}

AutoPtr<IView> ActionMenuPresenter::FindViewForItem(
    /* [in] */ IMenuItem* item)
{
    AutoPtr<IViewGroup> parent = IViewGroup::Probe(mMenuView);
    if (parent == NULL) return NULL;

    Int32 count = 0;
    parent->GetChildCount(&count);
    for (Int32 i = 0; i < count; i++) {
        AutoPtr<IView> child;
        parent->GetChildAt(i, (IView**)&child);


        if (IMenuItemView::Probe(child) != NULL)
        {
            AutoPtr<IMenuItemImpl> itemTemp;
            IMenuItemView::Probe(child)->GetItemData((IMenuItemImpl**)&itemTemp);
            AutoPtr<IMenuItem> tmp = itemTemp;
            if(tmp.Get() == item)
            {
                return child;
            }
        }
    }

    return NULL;
}

Boolean ActionMenuPresenter::ShowOverflowMenu()
{
    MenuBuilderBase* base = reinterpret_cast<MenuBuilderBase*>(mMenu->Probe(EIID_MenuBuilderBase));
    if (mReserveOverflow && !IsOverflowMenuShowing() && mMenu != NULL && mMenuView != NULL &&
            mPostedOpenRunnable == NULL && !base->GetNonActionItems()->IsEmpty()) {
        AutoPtr<OverflowPopup> popup = new OverflowPopup(mContext, mMenu, mOverflowButton, TRUE, this);
        mPostedOpenRunnable = new OpenOverflowRunnable(popup, this);
        // Post this for later; we might still need a layout for the anchor to be right.
        VIEW_PROBE(mMenuView)->Post(mPostedOpenRunnable);

        // ActionMenuPresenter uses null as a callback argument here
        // to indicate overflow is opening.
        Boolean tmp = FALSE;
        BaseMenuPresenter::OnSubMenuSelected(NULL, &tmp);

        return TRUE;
    }

    return FALSE;
}

Boolean ActionMenuPresenter::HideOverflowMenu()
{
    if (mPostedOpenRunnable != NULL && mMenuView != NULL) {
        VIEW_PROBE(mMenuView)->RemoveCallbacks(mPostedOpenRunnable);
        mPostedOpenRunnable = NULL;
        return TRUE;
    }

    if (mOverflowPopup != NULL) {
        mOverflowPopup->Dismiss();
        return TRUE;
    }

    return FALSE;
}

Boolean ActionMenuPresenter::DismissPopupMenus()
{
    Boolean result = HideOverflowMenu();
    result |= HideSubMenus();
    return result;
}

Boolean ActionMenuPresenter::HideSubMenus()
{
    if (mActionButtonPopup != NULL) {
        mActionButtonPopup->Dismiss();
        return TRUE;
    }
    return FALSE;
}

Boolean ActionMenuPresenter::IsOverflowMenuShowing()
{
    Boolean isShowing;
    return mOverflowPopup != NULL && (mOverflowPopup->IsShowing(&isShowing), isShowing);
}

Boolean ActionMenuPresenter::IsOverflowReserved()
{
    return mReserveOverflow;
}

ECode ActionMenuPresenter::FlagActionItems(
    /* [out] */ Boolean* result)
{
    assert(result != NULL);

    MenuBuilderBase* base = reinterpret_cast<MenuBuilderBase*>(mMenu->Probe(EIID_MenuBuilderBase));
    AutoPtr< List<AutoPtr<IMenuItemImpl> > > visibleItems = base->GetVisibleItems();
    List<AutoPtr<IMenuItemImpl> >::Iterator it = visibleItems->Begin();
    Int32 maxActions = mMaxItems;
    Int32 widthLimit = mActionItemWidthLimit;
    const Int32 querySpec = View::MeasureSpec::MakeMeasureSpec(0, View::MeasureSpec::UNSPECIFIED);
    AutoPtr<IViewGroup> parent = IViewGroup::Probe(mMenuView);

    Int32 requiredItems = 0;
    Int32 requestedItems = 0;
    Int32 firstActionWidth = 0;
    Boolean hasOverflow = FALSE;
    for (; it != visibleItems->End(); it++) {
        AutoPtr<IMenuItemImpl> item = *it;

        Boolean tmp = FALSE;
        if (item->RequiresActionButton(&tmp), tmp) {
            requiredItems++;
        } else if (item->RequestsActionButton(&tmp), tmp) {
            requestedItems++;
        } else {
            hasOverflow = TRUE;
        }

        if (mExpandedActionViewsExclusive && (item->IsActionViewExpanded(&tmp), tmp)) {
            // Overflow everything if we have an expanded action view and we're
            // space constrained.
            maxActions = 0;
        }
    }

    // Reserve a spot for the overflow item if needed.
    if (mReserveOverflow &&
            (hasOverflow || requiredItems + requestedItems > maxActions)) {
        maxActions--;
    }
    maxActions -= requiredItems;

    mActionButtonGroups.Clear();

    Int32 cellSize = 0;
    Int32 cellsRemaining = 0;
    if (mStrictWidthLimit) {
        cellsRemaining = widthLimit / mMinCellSize;
        const Int32 cellSizeRemaining = widthLimit % mMinCellSize;
        cellSize = mMinCellSize + cellSizeRemaining / cellsRemaining;
    }

    // Flag as many more requested items as will fit.
    it = visibleItems->Begin();
    for (; it != visibleItems->End(); it++) {
        AutoPtr<IMenuItemImpl> item = *it;

        Boolean tmp = FALSE;
        if (item->RequiresActionButton(&tmp), tmp) {
            AutoPtr<IView> v;
            GetItemView(item, mScrapActionButtonView, parent, (IView**)&v);
            if (mScrapActionButtonView == NULL) {
                mScrapActionButtonView = v;
            }

            if (mStrictWidthLimit) {
                cellsRemaining -= ActionMenuView::MeasureChildForCells(v,
                        cellSize, cellsRemaining, querySpec, 0);
            } else {
                v->Measure(querySpec, querySpec);
            }

            Int32 measuredWidth = 0;
            v->GetMeasuredWidth(&measuredWidth);

            widthLimit -= measuredWidth;
            if (firstActionWidth == 0) {
                firstActionWidth = measuredWidth;
            }

            Int32 groupId = 0;
            item->GetGroupId(&groupId);
            if (groupId != 0) {
                mActionButtonGroups[groupId] = TRUE;
            }

            item->SetIsActionButton(TRUE);
        } else if (item->RequestsActionButton(&tmp), tmp) {
            // Items in a group with other items that already have an action slot
            // can break the max actions rule, but not the width limit.
            Int32 groupId = 0;
            item->GetGroupId(&groupId);
            Boolean inGroup = mActionButtonGroups[groupId];
            Boolean isAction = (maxActions > 0 || inGroup) && widthLimit > 0 &&
                    (!mStrictWidthLimit || cellsRemaining > 0);

            if (isAction) {
                AutoPtr<IView> v;
                GetItemView(item, mScrapActionButtonView, parent, (IView**)&v);
                if (mScrapActionButtonView == NULL) {
                    mScrapActionButtonView = v;
                }

                if (mStrictWidthLimit) {
                    const Int32 cells = ActionMenuView::MeasureChildForCells(v,
                            cellSize, cellsRemaining, querySpec, 0);
                    cellsRemaining -= cells;
                    if (cells == 0) {
                        isAction = FALSE;
                    }
                } else {
                    v->Measure(querySpec, querySpec);
                }

                Int32 measuredWidth = 0;
                v->GetMeasuredWidth(&measuredWidth);
                widthLimit -= measuredWidth;
                if (firstActionWidth == 0) {
                    firstActionWidth = measuredWidth;
                }

                if (mStrictWidthLimit) {
                    isAction &= widthLimit >= 0;
                } else {
                    // Did this push the entire first item past the limit?
                    isAction &= widthLimit + firstActionWidth > 0;
                }
            }

            if (isAction && groupId != 0) {
                mActionButtonGroups[groupId] = TRUE;
            } else if (inGroup) {
                // We broke the width limit. Demote the whole group, they all overflow now.
                mActionButtonGroups[groupId] = FALSE;
                List<AutoPtr<IMenuItemImpl> >::Iterator it2 = visibleItems->Begin();
                for (; it2 != it; it2++) {
                    AutoPtr<IMenuItemImpl> areYouMyGroupie = *it2;
                    Int32 id = 0;
                    if ((areYouMyGroupie->GetGroupId(&id), id) == groupId) {
                        // Give back the action slot
                        Boolean tmp = FALSE;
                        if ((areYouMyGroupie->IsActionButton(&tmp), tmp)) maxActions++;

                        areYouMyGroupie->SetIsActionButton(FALSE);
                    }
                }
            }

            if (isAction) maxActions--;

            item->SetIsActionButton(isAction);
        } else {
            // Neither requires nor requests an action button.
            item->SetIsActionButton(FALSE);
        }
    }

    *result = TRUE;
    return NOERROR;
}

ECode ActionMenuPresenter::OnCloseMenu(
    /* [in] */ IMenuBuilder* menu,
    /* [in] */ Boolean allMenusAreClosing)
{
    DismissPopupMenus();
    BaseMenuPresenter::OnCloseMenu(menu, allMenusAreClosing);
    return NOERROR;
}

ECode ActionMenuPresenter::OnSaveInstanceState(
    /* [out] */ IParcelable** parcel)
{
    AutoPtr<IActionMenuPresenterSavedState> state;
    CActionMenuPresenterSavedState::New((IActionMenuPresenterSavedState**)&state);
    state->SetOpenSubMenuId(mOpenSubMenuId);
    *parcel = IParcelable::Probe(state);
    REFCOUNT_ADD(*parcel);
    return NOERROR;
}

ECode ActionMenuPresenter::OnRestoreInstanceState(
    /* [in] */ IParcelable* state)
{
    AutoPtr<IActionMenuPresenterSavedState> saved = IActionMenuPresenterSavedState::Probe(state);
    Int32 openSubMenuId;
    saved->GetOpenSubMenuId(&openSubMenuId);
    if (openSubMenuId > 0) {
        AutoPtr<IMenuItem> item;
        mMenu->FindItem(openSubMenuId, (IMenuItem**)&item);
        if (item != NULL) {
            AutoPtr<ISubMenu> mTemp;
            item->GetSubMenu((ISubMenu**)&mTemp);
            AutoPtr<ISubMenuBuilder> subMenu = ISubMenuBuilder::Probe(mTemp);
            Boolean temp;
            OnSubMenuSelected(subMenu, &temp);
        }
    }
    return NOERROR;
}

ECode ActionMenuPresenter::OnSubUiVisibilityChanged(
    /* [in] */ Boolean isVisible)
{
    if (isVisible) {
        // Not a submenu, but treat it like one.
        Boolean tmp = FALSE;
        BaseMenuPresenter::OnSubMenuSelected(NULL, &tmp);
    } else {
        MenuBuilderBase* base = reinterpret_cast<MenuBuilderBase*>(mMenu->Probe(EIID_MenuBuilderBase));;
        assert(base != NULL);
        base->Close(FALSE);
    }

    return NOERROR;
}

} // namespace Menu
} // namespace View
} // namespace Droid
} // namespace Elastos
