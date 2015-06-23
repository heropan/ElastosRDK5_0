
#include "view/menu/ListMenuItemView.h"
#include "view/LayoutInflater.h"
#include "R.h"

using Elastos::Core::CStringWrapper;
using Elastos::Droid::R;
using Elastos::Droid::Widget::ICompoundButton;
using Elastos::Droid::Widget::ICheckable;

namespace Elastos {
namespace Droid {
namespace View {
namespace Menu {

const String ListMenuItemView::TAG = String("ListMenuItemView");

ListMenuItemView::ListMenuItemView()
    : mTextAppearance(0)
    , mPreserveIconSpacing(FALSE)
    , mMenuType(0)
    , mForceShowIcon(FALSE)
{}

ListMenuItemView::ListMenuItemView(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
    : LinearLayout(context, attrs)
    , mTextAppearance(0)
    , mPreserveIconSpacing(FALSE)
    , mMenuType(0)
    , mForceShowIcon(FALSE)
{
    ASSERT_SUCCEEDED(InitFromAttributes(context, attrs, defStyle));
}

ECode ListMenuItemView::OnFinishInflate()
{
    LinearLayout::OnFinishInflate();

    SetBackgroundDrawable(mBackground);

    AutoPtr<IView> view = FindViewById(R::id::title);
    mTitleView = ITextView::Probe(view);
    if (mTextAppearance != -1) {
        mTitleView->SetTextAppearance(mTextAppearanceContext, mTextAppearance);
    }

    view = FindViewById(R::id::shortcut);
    mShortcutView = ITextView::Probe(view);
    return NOERROR;
}

ECode ListMenuItemView::Initialize(
    /* [in] */ IMenuItemImpl* itemData,
    /* [in] */ Int32 menuType)
{
    mItemData = itemData;
    mMenuType = menuType;

    Boolean visible = FALSE;
    mItemData->IsVisible(&visible);
    SetVisibility(visible ? IView::VISIBLE : IView::GONE);

    AutoPtr<ICharSequence> title;
    mItemData->GetTitleForItemView((IMenuItemView*)this->Probe(EIID_IMenuItemView), (ICharSequence**)&title);
    SetTitle(title);
    Boolean isCheckable = FALSE;
    mItemData->IsCheckable(&isCheckable);
    SetCheckable(isCheckable);
    Boolean mssc = FALSE;
    mItemData->ShouldShowShortcut(&mssc);
    Char32 mgsc;
    mItemData->GetShortcut(&mgsc);
    SetShortcut(mssc, mgsc);
    AutoPtr<IDrawable> icon;
    mItemData->GetIcon((IDrawable**)&icon);
    SetIcon(icon);
    Boolean isEnabled = FALSE;
    mItemData->IsEnabled(&isEnabled);
    SetEnabled(isEnabled);

    return NOERROR;
}

ECode ListMenuItemView::SetForceShowIcon(
    /* [in] */ Boolean forceShow)
{
    mPreserveIconSpacing = mForceShowIcon = forceShow;
    return NOERROR;
}

ECode ListMenuItemView::SetTitle(
    /* [in] */ ICharSequence* title)
{
    if (title != NULL) {
        mTitleView->SetText(title);

        Int32 visibility;
        mTitleView->GetVisibility(&visibility);
        if (visibility != IView::VISIBLE) {
            mTitleView->SetVisibility(IView::VISIBLE);
        }
    }
    else {
        Int32 visibility;
        mTitleView->GetVisibility(&visibility);
        if (visibility != IView::GONE) {
            mTitleView->SetVisibility(IView::GONE);
        }
    }

    return NOERROR;
}

AutoPtr<IMenuItemImpl> ListMenuItemView::GetItemData()
{
    return mItemData;
}

ECode ListMenuItemView::SetCheckable(
    /* [in] */ Boolean checkable)
{
    if (!checkable && mRadioButton == NULL && mCheckBox == NULL) {
        return NOERROR;
    }

    // Depending on whether its exclusive check or not, the checkbox or
    // radio button will be the one in use (and the other will be otherCompoundButton)
    AutoPtr<ICompoundButton> compoundButton;
    AutoPtr<ICompoundButton> otherCompoundButton;

    Boolean check = FALSE;
    mItemData->IsExclusiveCheckable(&check);
    if (check) {
        if (mRadioButton == NULL) {
            InsertRadioButton();
        }
        compoundButton = mRadioButton;
        otherCompoundButton = mCheckBox;
    }
    else {
        if (mCheckBox == NULL) {
            InsertCheckBox();
        }
        compoundButton = mCheckBox;
        otherCompoundButton = mRadioButton;
    }

    if (checkable) {
        mItemData->IsChecked(&check);
        ICheckable::Probe(compoundButton)->SetChecked(check);

        Int32 newVisibility = checkable ? IView::VISIBLE : IView::GONE;

        Int32 tmpVisibility;
        compoundButton->GetVisibility(&tmpVisibility);
        if (tmpVisibility != newVisibility) {
            compoundButton->SetVisibility(newVisibility);
        }

        // Make sure the other compound button isn't visible
        if (otherCompoundButton != NULL) {
            otherCompoundButton->GetVisibility(&tmpVisibility);
            if (tmpVisibility != IView::GONE) {
                otherCompoundButton->SetVisibility(IView::GONE);
            }
        }
    }
    else {
        if (mCheckBox != NULL) {
            mCheckBox->SetVisibility(IView::GONE);
        }

        if (mRadioButton != NULL) {
            mRadioButton->SetVisibility(IView::GONE);
        }
    }

    return NOERROR;
}

ECode ListMenuItemView::SetChecked(
    /* [in] */ Boolean checked)
{
    AutoPtr<ICompoundButton> compoundButton;

    Boolean isExclusiveCheckable = FALSE;
    mItemData->IsExclusiveCheckable(&isExclusiveCheckable);
    if (isExclusiveCheckable) {
        if (mRadioButton == NULL) {
            InsertRadioButton();
        }
        compoundButton = mRadioButton;
    }
    else {
        if (mCheckBox == NULL) {
            InsertCheckBox();
        }
        compoundButton = mCheckBox;
    }

    ICheckable::Probe(compoundButton)->SetChecked(checked);

    return NOERROR;
}

ECode ListMenuItemView::SetShortcut(
    /* [in] */ Boolean showShortcut,
    /* [in] */ Char32 shortcutKey)
{
    Boolean res = FALSE;
    mItemData->ShouldShowShortcut(&res);
    Int32 newVisibility = (showShortcut && res) ? IView::VISIBLE : IView::GONE;

    if (newVisibility == IView::VISIBLE) {
        String label;
        mItemData->GetShortcutLabel(&label);
        AutoPtr<ICharSequence> cs;
        CStringWrapper::New(label, (ICharSequence**)&cs);
        mShortcutView->SetText(cs);
    }

    Int32 visibility;
    mShortcutView->GetVisibility(&visibility);
    if (visibility != newVisibility) {
        mShortcutView->SetVisibility(newVisibility);
    }

    return NOERROR;
}

ECode ListMenuItemView::SetIcon(
    /* [in] */ IDrawable* icon)
{
    Boolean show = FALSE;
    mItemData->ShouldShowIcon(&show);
    Boolean showIcon = show || mForceShowIcon;
    if (!showIcon && !mPreserveIconSpacing) {
        return NOERROR;
    }

    if (mIconView == NULL && icon == NULL && !mPreserveIconSpacing) {
        return NOERROR;
    }

    if (mIconView == NULL) {
        InsertIconView();
    }

    if (icon != NULL || mPreserveIconSpacing) {
        mIconView->SetImageDrawable(showIcon ? icon : NULL);

        Int32 visibility;
        mIconView->GetVisibility(&visibility);
        if (visibility != IView::VISIBLE) {
            mIconView->SetVisibility(IView::VISIBLE);
        }
    }
    else {
        mIconView->SetVisibility(IView::GONE);
    }

    return NOERROR;
}

void ListMenuItemView::OnMeasure(
    /* [in] */ Int32 widthMeasureSpec,
    /* [in] */ Int32 heightMeasureSpec)
{
    if (mIconView != NULL && mPreserveIconSpacing) {
        // Enforce minimum icon spacing
        AutoPtr<IViewGroupLayoutParams> lp = GetLayoutParams();
        AutoPtr<IViewGroupLayoutParams> iconLp;
        mIconView->GetLayoutParams((IViewGroupLayoutParams**)&iconLp);

        Int32 height = 0, width = 0;
        lp->GetHeight(&height);
        iconLp->GetWidth(&width);
        if (height > 0 && width <= 0) {
            iconLp->SetWidth(height);
        }
    }

    LinearLayout::OnMeasure(widthMeasureSpec, heightMeasureSpec);
}

void ListMenuItemView::InsertIconView()
{
    AutoPtr<ILayoutInflater> inflater = GetInflater();
    mIconView = NULL;
    inflater->Inflate(R::layout::list_menu_item_icon,
            (IViewGroup*)this->Probe(EIID_IViewGroup), FALSE, (IView**)&mIconView);
    AddView(mIconView, 0);
}

void ListMenuItemView::InsertRadioButton()
{
    AutoPtr<ILayoutInflater> inflater = GetInflater();
    mRadioButton = NULL;
    inflater->Inflate(R::layout::list_menu_item_radio,
            (IViewGroup*)this->Probe(EIID_IViewGroup), FALSE, (IView**)&mRadioButton);
    AddView(mRadioButton);
}

void ListMenuItemView::InsertCheckBox()
{
    AutoPtr<ILayoutInflater> inflater = GetInflater();
    mCheckBox = NULL;
    inflater->Inflate(R::layout::list_menu_item_checkbox,
            (IViewGroup*)this->Probe(EIID_IViewGroup), FALSE, (IView**)&mCheckBox);
    AddView(mCheckBox);
}

Boolean ListMenuItemView::PrefersCondensedTitle()
{
    return FALSE;
}

Boolean ListMenuItemView::ShowsIcon()
{
    return mForceShowIcon;
}

AutoPtr<ILayoutInflater> ListMenuItemView::GetInflater()
{
    if (mInflater == NULL) {
        LayoutInflater::From(mContext, (ILayoutInflater**)&mInflater);
    }

    return mInflater;
}

ECode ListMenuItemView::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    FAIL_RETURN(LinearLayout::Init(context, attrs));

    return InitFromAttributes(context, attrs, defStyle);
}

ECode ListMenuItemView::InitFromAttributes(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    AutoPtr<ArrayOf<Int32> > attrIds = ArrayOf<Int32>::Alloc(
        const_cast<Int32 *>(R::styleable::MenuView),
        ARRAY_SIZE(R::styleable::MenuView));
    AutoPtr<ITypedArray> a;
    context->ObtainStyledAttributes(attrs, attrIds, defStyle, 0, (ITypedArray**)&a);

    a->GetDrawable(R::styleable::MenuView_itemBackground, (IDrawable**)&mBackground);
    a->GetResourceId(R::styleable::MenuView_itemTextAppearance, -1, &mTextAppearance);
    a->GetBoolean(R::styleable::MenuView_preserveIconSpacing, FALSE, &mPreserveIconSpacing);
    mTextAppearanceContext = context;

    a->Recycle();
    return NOERROR;
}

} // namespace Menu
} // namespace View
} // namespace Droid
} // namespace Elastos
