
#include "graphics/CRect.h"
#include "view/menu/IconMenuItemView.h"
#include "view/CGravity.h"
#include "text/Layout.h"
#include "view/SoundEffectConstants.h"
#include "view/menu/CIconMenuView.h"
#include "view/menu/CIconMenuViewLayoutParams.h"
#include "R.h"
#include "text/Layout.h"

using Elastos::Core::CStringWrapper;
using Elastos::Droid::R;
using Elastos::Droid::Text::Layout;
using Elastos::Droid::View::SoundEffectConstants;

namespace Elastos {
namespace Droid {
namespace View {
namespace Menu {

// 860cad72-891e-4b30-af21-5fa5b77f9887
extern "C" const InterfaceID EIID_IconMenuItemView =
        { 0x860cad72, 0x891e, 0x4b30, { 0xaf, 0x21, 0x5f, 0xa5, 0xb7, 0x7f, 0x98, 0x87 } };

const Int32 IconMenuItemView::NO_ALPHA = 0xFF;
String IconMenuItemView::sPrependShortcutLabel;

IconMenuItemView::IconMenuItemView()
    : mTextAppearance(0)
    , mDisabledAlpha(0.f)
    , mShortcutCaptionMode(FALSE)
{
    CRect::New((IRect**)&mPositionIconAvailable);
    CRect::New((IRect**)&mPositionIconOutput);
}

ECode IconMenuItemView::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    FAIL_RETURN(TextView::Init(context, attrs));

    if (sPrependShortcutLabel.IsNull()) {
        /*
         * Views should only be constructed from the UI thread, so no
         * synchronization needed
         */
        GetResources()->GetString(R::string::prepend_shortcut_label, &sPrependShortcutLabel);
    }

    AutoPtr<ArrayOf<Int32> > attrIds = ArrayOf<Int32>::Alloc(
        const_cast<Int32 *>(R::styleable::MenuView),
        ARRAY_SIZE(R::styleable::MenuView));
    AutoPtr<ITypedArray> a;
    context->ObtainStyledAttributes(attrs, attrIds, defStyle, 0, (ITypedArray**)&a);

    a->GetFloat(R::styleable::MenuView_itemIconDisabledAlpha, 0.8f, &mDisabledAlpha);
    a->GetResourceId(R::styleable::MenuView_itemTextAppearance, -1, &mTextAppearance);
    mTextAppearanceContext = context;

    a->Recycle();

    return NOERROR;
}

/**
 * Initializes with the provided title and icon
 * @param title The title of this item
 * @param icon The icon of this item
 */
void IconMenuItemView::Initialize(
    /* [in] */ ICharSequence* title,
    /* [in] */ IDrawable* icon)
{
    SetClickable(TRUE);
    SetFocusable(TRUE);

    if (mTextAppearance != -1) {
        SetTextAppearance(mTextAppearanceContext, mTextAppearance);
    }

    SetTitle(title);
    SetIcon(icon);
}

ECode IconMenuItemView::Initialize(
    /* [in] */ IMenuItemImpl* itemData,
    /* [in] */ Int32 menuType)
{
    AutoPtr<ICharSequence> str;
    mItemData->GetTitleForItemView((IMenuItemView*)this->Probe(EIID_IMenuItemView), (ICharSequence**)&str);
    AutoPtr<IDrawable> icon;
    mItemData->GetIcon((IDrawable**)&icon);
    Initialize(str, icon);

    Boolean isVisible;
    mItemData->IsVisible(&isVisible);
    SetVisibility(isVisible ? IView::VISIBLE : IView::GONE);
    Boolean isEnabled;
    mItemData->IsEnabled(&isEnabled);
    SetEnabled(isEnabled);

    return NOERROR;
}

ECode IconMenuItemView::SetItemData(
    /* [in] */ IMenuItemImpl* data)
{
    mItemData = data;
    return NOERROR;
}

ECode IconMenuItemView::GetIcon(
    /* [out] */ IDrawable** icon)
{
    *icon = mIcon;
    REFCOUNT_ADD(*icon);
    return NOERROR;
}

Boolean IconMenuItemView::PerformClick()
{
    // Let the view's click listener have top priority (the More button relies on this)
    if (TextView::PerformClick()) {
        return TRUE;
    }

    Boolean result;
    if (mItemInvoker != NULL && (mItemInvoker->InvokeItem(mItemData, &result), result)) {
        PlaySoundEffect(SoundEffectConstants::CLICK);
        return TRUE;
    }
    else {
        return FALSE;
    }
}

ECode IconMenuItemView::SetTitle(
    /* [in] */ ICharSequence* title)
{
    if (mShortcutCaptionMode) {
        /*
         * Don't set the title directly since it will replace the
         * shortcut+title being shown. Instead, re-set the shortcut caption
         * mode so the new title is shown.
         */
        SetCaptionMode(TRUE);
    }
    else if (title != NULL) {
        SetText(title);
    }

    return NOERROR;
}

void IconMenuItemView::SetCaptionMode(
    /* [in] */ Boolean shortcut)
{
    /*
     * If there is no item model, don't do any of the below (for example,
     * the 'More' item doesn't have a model)
     */
    if (mItemData == NULL) {
        return;
    }
    Boolean res = FALSE;
    mItemData->ShouldShowShortcut(&res);
    mShortcutCaptionMode = shortcut && res;

    AutoPtr<ICharSequence> text;
    mItemData->GetTitleForItemView((IMenuItemView*)this->Probe(EIID_IMenuItemView), (ICharSequence**)&text);

    if (mShortcutCaptionMode) {
        if (mShortcutCaption.IsNull()) {
            mItemData->GetShortcutLabel(&mShortcutCaption);
        }

        text = NULL;
        CStringWrapper::New(mShortcutCaption, (ICharSequence**)&text);
    }

    SetText(text);
}

ECode IconMenuItemView::SetIcon(
        /* [in] */ IDrawable* icon)
{
    mIcon = icon;

    if (icon != NULL) {
        /* Set the bounds of the icon since setCompoundDrawables needs it. */
        Int32 width, height;
        icon->GetIntrinsicWidth(&width);
        icon->GetIntrinsicHeight(&height);
        icon->SetBounds(0, 0, width, height);

        // Set the compound drawables
        SetCompoundDrawables(NULL, icon, NULL, NULL);

        // When there is an icon, make sure the text is at the bottom
        SetGravity(IGravity::BOTTOM | IGravity::CENTER_HORIZONTAL);

        /*
         * Request a layout to reposition the icon. The positioning of icon
         * depends on this TextView's line bounds, which is only available
         * after a layout.
         */
        RequestLayout();
    }
    else {
        SetCompoundDrawables(NULL, NULL, NULL, NULL);

        // When there is no icon, make sure the text is centered vertically
        SetGravity(IGravity::CENTER_VERTICAL | IGravity::CENTER_HORIZONTAL);
    }

    return NOERROR;
}

ECode IconMenuItemView::SetItemInvoker(
        /* [in] */ IMenuBuilderItemInvoker* itemInvoker)
{
    mItemInvoker = itemInvoker;

    return NOERROR;
}

AutoPtr<IMenuItemImpl> IconMenuItemView::GetItemData()
{
    return mItemData;
}

//@override
ECode IconMenuItemView::SetVisibility(
    /* [in] */ Int32 v)
{
    TextView::SetVisibility(v);

    if (mIconMenuView != NULL) {
        // On visibility change, mark the IconMenuView to refresh itself eventually
        ((CIconMenuView*)mIconMenuView.Get())->MarkStaleChildren();
    }

    return NOERROR;
}

void IconMenuItemView::SetIconMenuView(
    /* [in] */ IIconMenuView* iconMenuView)
{
    mIconMenuView = iconMenuView;;
}

//@override
ECode IconMenuItemView::DrawableStateChanged()
{
    TextView::DrawableStateChanged();

    if (mItemData != NULL && mIcon != NULL) {
        // When disabled, the not-focused state and the pressed state should
        // drop alpha on the icon
        Boolean isEnabled;
        mItemData->IsEnabled(&isEnabled);
        Boolean isInAlphaState = !isEnabled && (IsPressed() || !IsFocused());
        mIcon->SetAlpha(isInAlphaState ? (Int32)(mDisabledAlpha * NO_ALPHA) : NO_ALPHA);
    }

    return NOERROR;
}

//@override
void IconMenuItemView::OnLayout(
    /* [in] */ Boolean changed,
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom)
{
    TextView::OnLayout(changed, left, top, right, bottom);

    PositionIcon();
}

void IconMenuItemView::OnTextChanged(
    /* [in] */ ICharSequence* text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 before,
    /* [in] */ Int32 after)
{
    TextView::OnTextChanged(text, start, before, after);
    // our layout params depend on the length of the text
    SetLayoutParams(GetTextAppropriateLayoutParams());
}

/**
 * @return layout params appropriate for this view.  If layout params already exist, it will
 *         augment them to be appropriate to the current text size.
 */
AutoPtr<IIconMenuViewLayoutParams> IconMenuItemView::GetTextAppropriateLayoutParams()
{
    AutoPtr<IIconMenuViewLayoutParams> lp;

    AutoPtr<IViewGroupLayoutParams> _lp = GetLayoutParams();
    if (_lp == NULL) {
        // Default layout parameters
        CIconMenuViewLayoutParams::New(IViewGroupLayoutParams::MATCH_PARENT,
                IViewGroupLayoutParams::MATCH_PARENT, (IIconMenuViewLayoutParams**)&lp);
    }
    else {
        lp = IIconMenuViewLayoutParams::Probe(_lp);
    }

    // Set the desired width of item
    lp->SetDesiredWidth((Int32)Layout::GetDesiredWidth(GetText(), GetPaint()));

    return lp;
}

void IconMenuItemView::PositionIcon()
{
    if (mIcon == NULL) {
        return;
    }

    // We reuse the output rectangle as a temp rect
    IRect* tmpRect = mPositionIconOutput;
    GetLineBounds(0, tmpRect);

    mPositionIconAvailable->Set(0, 0, GetWidth(), ((CRect*)tmpRect)->mTop);
    const Int32 layoutDirection = GetLayoutDirection();

    Int32 iconWidth, iconHeight;
    mIcon->GetIntrinsicWidth(&iconWidth);
    mIcon->GetIntrinsicHeight(&iconHeight);

    AutoPtr<IGravity> gravity;
    CGravity::AcquireSingleton((IGravity**)&gravity);
    gravity->Apply(IGravity::CENTER_VERTICAL | IGravity::START,
        iconWidth, iconHeight, mPositionIconAvailable, mPositionIconOutput, layoutDirection);
    mIcon->SetBounds(mPositionIconOutput);
}

ECode IconMenuItemView::SetCheckable(
    /* [in] */ Boolean checkable)
{
    return NOERROR;
}

ECode IconMenuItemView::SetChecked(
    /* [in] */ Boolean checked)
{
    return NOERROR;
}

ECode IconMenuItemView::SetShortcut(
    /* [in] */ Boolean showShortcut,
    /* [in] */ Char32 shortcutKey)
{
    if (mShortcutCaptionMode) {
        /*
         * Shortcut has changed and we're showing it right now, need to
         * update (clear the old one first).
         */
        mShortcutCaption = NULL;
        SetCaptionMode(TRUE);
    }

    return NOERROR;
}

Boolean IconMenuItemView::PrefersCondensedTitle()
{
    return TRUE;
}

Boolean IconMenuItemView::ShowsIcon()
{
    return TRUE;
}

} // namespace Menu
} // namespace View
} // namespace Droid
} // namespace Elastos
