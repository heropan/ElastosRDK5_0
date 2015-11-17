
#ifndef __ELASTOS_DROID_INTERNAL_VIEW_MENU_ListMenuItemView_H__
#define __ELASTOS_DROID_INTERNAL_VIEW_MENU_ListMenuItemView_H__

#include "elastos/droid/internal/view/menu/MenuItemImpl.h"
#include "elastos/droid/widget/LinearLayout.h"

using Elastos::Droid::Widget::LinearLayout;
using Elastos::Droid::Widget::IImageView;
using Elastos::Droid::Widget::IRadioButton;
using Elastos::Droid::Widget::ITextView;
using Elastos::Droid::Widget::ICheckBox;

namespace Elastos {
namespace Droid {
namespace Internal {
namespace View {
namespace Menu {

class ListMenuItemView : public LinearLayout
{
public:
    ListMenuItemView(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle = 0);

    virtual CARAPI Initialize(
        /* [in] */ IMenuItemImpl* itemData,
        /* [in] */ Int32 menuType);

    virtual CARAPI SetForceShowIcon(
        /* [in] */ Boolean forceShow);

    virtual CARAPI SetTitle(
        /* [in] */ ICharSequence* title);

    virtual CARAPI_(AutoPtr<IMenuItemImpl>) GetItemData();

    virtual CARAPI SetCheckable(
        /* [in] */ Boolean checkable);

    virtual CARAPI SetChecked(
        /* [in] */ Boolean checked);

    virtual CARAPI SetShortcut(
        /* [in] */ Boolean showShortcut,
        /* [in] */ Char32 shortcutKey);

    virtual CARAPI SetIcon(
        /* [in] */ IDrawable* icon);

    virtual CARAPI_(Boolean) PrefersCondensedTitle();

    virtual CARAPI_(Boolean) ShowsIcon();

protected:
    ListMenuItemView();

    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle = 0);

    //Override
    CARAPI OnFinishInflate();

    CARAPI_(void) OnMeasure(
        /* [in] */ Int32 widthMeasureSpec,
        /* [in] */ Int32 heightMeasureSpec);

private:
    CARAPI_(void) InsertIconView();

    CARAPI_(void) InsertRadioButton();

    CARAPI_(void) InsertCheckBox();

    CARAPI InitFromAttributes(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle);

    CARAPI_(AutoPtr<ILayoutInflater>) GetInflater();

private:
    static const String TAG;

    AutoPtr<IMenuItemImpl> mItemData;
    AutoPtr<IImageView> mIconView;
    AutoPtr<IRadioButton> mRadioButton;
    AutoPtr<ITextView> mTitleView;
    AutoPtr<ICheckBox> mCheckBox;
    AutoPtr<ITextView> mShortcutView;

    AutoPtr<IDrawable> mBackground;
    Int32 mTextAppearance;
    AutoPtr<IContext> mTextAppearanceContext;
    Boolean mPreserveIconSpacing;

    Int32 mMenuType;

    AutoPtr<ILayoutInflater> mInflater;

    Boolean mForceShowIcon;

};

} // namespace Menu
} // namespace View
} // namespace Internal
} // namespace Droid
} // namespace Elastos

#endif  //__ELASTOS_DROID_INTERNAL_VIEW_MENU_ListMenuItemView_H__
