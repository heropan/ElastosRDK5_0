
#include "view/menu/ActionMenuItemView.h"
#include "view/menu/MenuItemImpl.h"
#include "R.h"
#include "text/TextUtils.h"
#include <elastos/core/Math.h>
#include "widget/CToastHelper.h"

using Elastos::Core::CStringWrapper;
using Elastos::Droid::R;
using Elastos::Droid::Text::TextUtils;
using Elastos::Droid::Widget::IToast;
using Elastos::Droid::Widget::IToastHelper;
using Elastos::Droid::Widget::CToastHelper;

namespace Elastos {
namespace Droid {
namespace View {
namespace Menu {

String ActionMenuItemView::TAG = String("ActionMenuItemView");
const Int32 ActionMenuItemView::MAX_ICON_SIZE = 32; // dp

ActionMenuItemView::ActionMenuItemView()
    : mAllowTextWithIcon(FALSE)
    , mExpandedFormat(FALSE)
    , mMinWidth(0)
    , mSavedPaddingLeft(0)
    , mMaxIconSize(0)
{
}

ActionMenuItemView::ActionMenuItemView(
    /* [in] */ IContext* context)
    : mAllowTextWithIcon(FALSE)
    , mExpandedFormat(FALSE)
    , mMinWidth(0)
    , mSavedPaddingLeft(0)
    , mMaxIconSize(0)
{
    Init(context, NULL);
}

ActionMenuItemView::ActionMenuItemView(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
    : mAllowTextWithIcon(FALSE)
    , mExpandedFormat(FALSE)
    , mMinWidth(0)
    , mSavedPaddingLeft(0)
    , mMaxIconSize(0)
{
    Init(context, attrs, 0);
}

ActionMenuItemView::ActionMenuItemView(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
    : mAllowTextWithIcon(FALSE)
    , mExpandedFormat(FALSE)
    , mMinWidth(0)
    , mSavedPaddingLeft(0)
    , mMaxIconSize(0)
{
    Init(context, attrs, defStyle);
}

void ActionMenuItemView::OnConfigurationChanged(
    /* [in] */ IConfiguration* newConfig)
{
    TextView::OnConfigurationChanged(newConfig);

    AutoPtr<IResources> res;
    GetContext()->GetResources((IResources**)&res);
    res->GetBoolean(R::bool_::config_allowActionMenuItemTextWithIcon, &mAllowTextWithIcon);
    UpdateTextButtonVisibility();
}

ECode ActionMenuItemView::SetPadding(
    /* [in] */ Int32 l,
    /* [in] */ Int32 t,
    /* [in] */ Int32 r,
    /* [in] */ Int32 b)
{
    mSavedPaddingLeft = l;
    return TextView::SetPadding(l, t, r, b);
}

ECode ActionMenuItemView::GetItemData(
    /* [out] */ IMenuItemImpl** itemData)
{
    assert(itemData != NULL);
    *itemData = mItemData;
    INTERFACE_ADDREF(*itemData);

    return NOERROR;
}

ECode ActionMenuItemView::Initialize(
    /* [in] */ IMenuItemImpl* itemData,
    /* [in] */ Int32 menuType)
{
    mItemData = itemData;

    AutoPtr<IDrawable> icon;
    itemData->GetIcon((IDrawable**)&icon);
    SetIcon(icon);

    AutoPtr<ICharSequence> title;
    itemData->GetTitleForItemView((IMenuItemView*)this->Probe(EIID_IMenuItemView), (ICharSequence**)&title);
    SetTitle(title); // Title only takes effect if there is no icon

    Int32 id = 0;
    itemData->GetItemId(&id);
    SetId(id);

    Boolean visible = FALSE;
    itemData->IsVisible(&visible);
    SetVisibility(visible ? IView::VISIBLE : IView::GONE);

    Boolean enabled = FALSE;
    itemData->IsEnabled(&enabled);
    SetEnabled(enabled);

    return NOERROR;
}

ECode ActionMenuItemView::OnClick(
    /* [in] */ IView* v)
{
    if (mItemInvoker != NULL) {
        Boolean state = FALSE;
        return mItemInvoker->InvokeItem(mItemData, &state);
    }

    return NOERROR;
}

ECode ActionMenuItemView::SetItemInvoker(
    /* [in] */ IMenuBuilderItemInvoker* invoker)
{
    mItemInvoker = invoker;
    return NOERROR;
}

ECode ActionMenuItemView::PrefersCondensedTitle(
    /* [out] */ Boolean* supported)
{
    assert(supported != NULL);
    *supported = TRUE;
    return NOERROR;
}

ECode ActionMenuItemView::SetCheckable(
    /* [in] */ Boolean checkable)
{
    // TODO Support checkable action items
    return NOERROR;
}

ECode ActionMenuItemView::SetChecked(
    /* [in] */ Boolean checked)
{
    // TODO Support checkable action items
    return NOERROR;
}

ECode ActionMenuItemView::SetExpandedFormat(
    /* [in] */ Boolean expandedFormat)
{
    if (mExpandedFormat != expandedFormat) {
        mExpandedFormat = expandedFormat;
        if (mItemData != NULL) {
            return mItemData->ActionFormatChanged();
        }
    }

    return NOERROR;
}

void ActionMenuItemView::UpdateTextButtonVisibility()
{
    Boolean visible = !TextUtils::IsEmpty(mTitle);

    Boolean tmp = FALSE;
    visible &= mIcon == NULL ||
            ((mItemData->ShowsTextAsAction(&tmp), tmp) && (mAllowTextWithIcon || mExpandedFormat));

    SetText(visible ? mTitle : NULL);
}

ECode ActionMenuItemView::SetIcon(
    /* [in] */ IDrawable* icon)
{
    mIcon = icon;
    if (icon != NULL) {
        Int32 width = 0;
        icon->GetIntrinsicWidth(&width);
        Int32 height = 0;
        icon->GetIntrinsicHeight(&height);

        if (width > mMaxIconSize) {
            Float scale = (Float) mMaxIconSize / width;
            width = mMaxIconSize;
            height *= scale;
        }
        if (height > mMaxIconSize) {
            Float scale = (Float) mMaxIconSize / height;
            height = mMaxIconSize;
            width *= scale;
        }

        icon->SetBounds(0, 0, width, height);
    }

    SetCompoundDrawables(icon, NULL, NULL, NULL);

    UpdateTextButtonVisibility();

    return NOERROR;
}

Boolean ActionMenuItemView::HasText()
{
    return !TextUtils::IsEmpty(GetText());
}

ECode ActionMenuItemView::SetShortcut(
    /* [in] */ Boolean showShortcut,
    /* [in] */ Char32 shortcutKey)
{
    // Action buttons don't show text for shortcut keys.
    return NOERROR;
}

ECode ActionMenuItemView::SetTitle(
    /* [in] */ ICharSequence* title)
{
    mTitle = title;

    SetContentDescription(mTitle);
    UpdateTextButtonVisibility();

    return NOERROR;
}

Boolean ActionMenuItemView::DispatchPopulateAccessibilityEvent(
    /* [in] */ IAccessibilityEvent* event)
{
    OnPopulateAccessibilityEvent(event);
    return TRUE;
}

ECode ActionMenuItemView::OnPopulateAccessibilityEvent(
    /* [in] */ IAccessibilityEvent* event)
{
    TextView::OnPopulateAccessibilityEvent(event);
    AutoPtr<ICharSequence> cdesc = GetContentDescription();
    if (!TextUtils::IsEmpty(cdesc)) {
        AutoPtr<IObjectContainer> texts;
        event->GetText((IObjectContainer**)&texts);
        assert(texts != NULL);
        texts->Add(cdesc);
    }

    return NOERROR;
}

Boolean ActionMenuItemView::DispatchHoverEvent(
    /* [in] */ IMotionEvent* event)
{
    // Don't allow children to hover; we want this to be treated as a single component.
    return OnHoverEvent(event);
}

ECode ActionMenuItemView::ShowsIcon(
    /* [out] */ Boolean* shows)
{
    assert(shows != NULL);
    *shows = TRUE;
    return NOERROR;
}

ECode ActionMenuItemView::NeedsDividerBefore(
    /* [out] */ Boolean* need)
{
    assert(need != NULL);
    AutoPtr<IDrawable> icon;
    *need = HasText() && (mItemData->GetIcon((IDrawable**)&icon), icon) == NULL;
    return NOERROR;
}

ECode ActionMenuItemView::NeedsDividerAfter(
    /* [out] */ Boolean* need)
{
    assert(need != NULL);
    *need = HasText();
    return NOERROR;
}

ECode ActionMenuItemView::OnLongClick(
    /* [in] */ IView* v,
    /* [out] */ Boolean* result)
{
    assert(result != NULL);
    if (HasText()) {
        // Don't show the cheat sheet for items that already show text.
        *result = FALSE;
        return NOERROR;
    }

    Int32 screenPosX = 0, screenPosY = 0;
    AutoPtr<IRect> displayFrame;
    CRect::New((IRect**)&displayFrame);
    GetLocationOnScreen(&screenPosX, &screenPosY);
    GetWindowVisibleDisplayFrame(displayFrame);

    AutoPtr<IContext> context = GetContext();
    Int32 width = GetWidth();
    Int32 height = GetHeight();
    Int32 midy = screenPosY + height / 2;

    AutoPtr<IResources> res;
    context->GetResources((IResources**)&res);
    assert(res != NULL);

    AutoPtr<IDisplayMetrics> metrics;
    res->GetDisplayMetrics((IDisplayMetrics**)&metrics);
    assert(metrics != NULL);

    Int32 screenWidth = 0;
    metrics->GetWidthPixels(&screenWidth);

    AutoPtr<ICharSequence> title;
    mItemData->GetTitle((ICharSequence**)&title);


    AutoPtr<IToast> cheatSheet;
    AutoPtr<IToastHelper> helper;
    CToastHelper::AcquireSingleton((IToastHelper**)&helper);

    helper->MakeText(context, title, IToast::LENGTH_SHORT, (IToast**)&cheatSheet);

    Int32 fHeight = 0;
    if (midy < (displayFrame->GetHeight(&fHeight), fHeight)) {
        // Show along the top; follow action buttons
        cheatSheet->SetGravity(IGravity::TOP | IGravity::END,
                screenWidth - screenPosX - width / 2, height);
    } else {
        // Show along the bottom center
        cheatSheet->SetGravity(IGravity::BOTTOM | IGravity::CENTER_HORIZONTAL, 0, height);
    }

    cheatSheet->Show();

    *result = TRUE;
    return NOERROR;
}

void ActionMenuItemView::OnMeasure(
    /* [in] */ Int32 widthMeasureSpec,
    /* [in] */ Int32 heightMeasureSpec)
{
    if (MeasureSpec::GetMode(heightMeasureSpec) == MeasureSpec::AT_MOST) {
        // Fill all available height.
        heightMeasureSpec = MeasureSpec::MakeMeasureSpec(
                MeasureSpec::GetSize(heightMeasureSpec), MeasureSpec::EXACTLY);
    }

    Boolean textVisible = HasText();
    if (textVisible && mSavedPaddingLeft >= 0) {
        TextView::SetPadding(mSavedPaddingLeft, GetPaddingTop(),
                GetPaddingRight(), GetPaddingBottom());
    }

    TextView::OnMeasure(widthMeasureSpec, heightMeasureSpec);

    const Int32 widthMode = MeasureSpec::GetMode(widthMeasureSpec);
    const Int32 widthSize = MeasureSpec::GetSize(widthMeasureSpec);
    const Int32 oldMeasuredWidth = GetMeasuredWidth();
    const Int32 targetWidth = widthMode == MeasureSpec::AT_MOST ? Elastos::Core::Math::Min(widthSize, mMinWidth)
            : mMinWidth;

    if (widthMode != MeasureSpec::EXACTLY && mMinWidth > 0 && oldMeasuredWidth < targetWidth) {
        // Remeasure at exactly the minimum width.
        TextView::OnMeasure(MeasureSpec::MakeMeasureSpec(targetWidth, MeasureSpec::EXACTLY),
                heightMeasureSpec);
    }

    if (!textVisible && mIcon != NULL) {
        // TextView won't center compound drawables in both dimensions without
        // a little coercion. Pad in to center the icon after we've measured.
        const Int32 w = GetMeasuredWidth();

        AutoPtr<IRect> bounds;
        mIcon->GetBounds((IRect**)&bounds);
        assert(bounds != NULL);
        Int32 dw = 0;
        bounds->GetWidth(&dw);

        TextView::SetPadding((w - dw) / 2, GetPaddingTop(), GetPaddingRight(), GetPaddingBottom());
    }
}

ECode ActionMenuItemView::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    TextView::Init(context, attrs, defStyle);

    AutoPtr<IResources> res;
    context->GetResources((IResources**)&res);
    res->GetBoolean(R::bool_::config_allowActionMenuItemTextWithIcon, &mAllowTextWithIcon);

    AutoPtr<ArrayOf<Int32> > attrIds = ArrayOf<Int32>::Alloc(
        const_cast<Int32 *>(R::styleable::ActionMenuItemView),
        ARRAY_SIZE(R::styleable::ActionMenuItemView));

    AutoPtr<ITypedArray> a;
    context->ObtainStyledAttributes(attrs, attrIds, 0, 0, (ITypedArray**)&a);
    a->GetDimensionPixelSize(R::styleable::ActionMenuItemView_minWidth, 0, &mMinWidth);
    a->Recycle();

    Float density = 0.f;

    AutoPtr<IDisplayMetrics> metrics;
    res->GetDisplayMetrics((IDisplayMetrics**)&metrics);
    metrics->GetDensity(&density);
    mMaxIconSize = (Int32) (MAX_ICON_SIZE * density + 0.5f);

    SetOnClickListener((IViewOnClickListener*)this->Probe(EIID_IViewOnClickListener));
    SetOnLongClickListener((IViewOnLongClickListener*)this->Probe(EIID_IViewOnLongClickListener));

    mSavedPaddingLeft = -1;
    return NOERROR;
}

} // namespace Menu
} // namespace View
} // namespace Droid
} // namespace Elastos
