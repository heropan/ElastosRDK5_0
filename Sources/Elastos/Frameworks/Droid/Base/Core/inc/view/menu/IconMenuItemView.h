
#ifndef __ELASTOS_DROID_VIEW_MENU_ICONMENIITEMVIEW_H__
#define __ELASTOS_DROID_VIEW_MENU_ICONMENIITEMVIEW_H__


#include "ext/frameworkext.h"
#include "view/menu/MenuItemImpl.h"
#include "widget/TextView.h"

using Elastos::Droid::Widget::TextView;

namespace Elastos {
namespace Droid {
namespace View {
namespace Menu {

extern "C" const InterfaceID EIID_IconMenuItemView;

/**
 * The item view for each item in the {@link IconMenuView}.
 */
class IconMenuItemView : public TextView
{
public:
    IconMenuItemView();

    /**
     * Initializes with the provided title and icon
     * @param title The title of this item
     * @param icon The icon of this item
     */
    CARAPI_(void) Initialize(
        /* [in] */ ICharSequence* title,
        /* [in] */ IDrawable* icon);

    CARAPI Initialize(
        /* [in] */ IMenuItemImpl* itemData,
        /* [in] */ Int32 menuType);

    CARAPI SetItemData(
        /* [in] */ IMenuItemImpl* data);

    //@override
    CARAPI_(Boolean) PerformClick();

    CARAPI SetTitle(
        /* [in] */ ICharSequence* title);

    CARAPI_(void) SetCaptionMode(
        /* [in] */ Boolean shortcut);

    CARAPI SetIcon(
        /* [in] */ IDrawable* icon);

    CARAPI SetItemInvoker(
        /* [in] */ IMenuBuilderItemInvoker* itemInvoker);

    //@override
    CARAPI SetVisibility(
        /* [in] */ Int32 v);

    CARAPI_(AutoPtr<IMenuItemImpl>) GetItemData();

    CARAPI_(void) SetIconMenuView(
        /* [in] */ IIconMenuView* iconMenuView);

    /**
     * @return layout params appropriate for this view.  If layout params already exist, it will
     *         augment them to be appropriate to the current text size.
     */
    CARAPI_(AutoPtr<IIconMenuViewLayoutParams>) GetTextAppropriateLayoutParams();

    CARAPI SetCheckable(
        /* [in] */ Boolean checkable);

    CARAPI SetChecked(
        /* [in] */ Boolean checked);

    CARAPI SetShortcut(
        /* [in] */ Boolean showShortcut,
        /* [in] */ Char32 shortcutKey);

    CARAPI_(Boolean) PrefersCondensedTitle();

    CARAPI_(Boolean) ShowsIcon();

    CARAPI GetIcon(
        /* [out] */ IDrawable** icon);

protected:
    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle = 0);

    //@Override
    CARAPI DrawableStateChanged();

    //@Override
    CARAPI_(void) OnLayout(
        /* [in] */ Boolean changed,
        /* [in] */ Int32 left,
        /* [in] */ Int32 top,
        /* [in] */ Int32 right,
        /* [in] */ Int32 bottom);

    //@Override
    CARAPI_(void) OnTextChanged(
        /* [in] */ ICharSequence* text,
        /* [in] */ Int32 start,
        /* [in] */ Int32 before,
        /* [in] */ Int32 after);

private:
    /**
     * Positions the icon vertically (horizontal centering is taken care of by
     * the TextView's gravity).
     */
    CARAPI_(void) PositionIcon();

private:
    static const Int32 NO_ALPHA;
    static String sPrependShortcutLabel;

    AutoPtr<IIconMenuView> mIconMenuView;

    AutoPtr<IMenuBuilderItemInvoker> mItemInvoker;
    AutoPtr<IMenuItemImpl> mItemData;

    AutoPtr<IDrawable> mIcon;

    Int32 mTextAppearance;
    AutoPtr<IContext> mTextAppearanceContext;

    Float mDisabledAlpha;

    AutoPtr<IRect> mPositionIconAvailable;
    AutoPtr<IRect> mPositionIconOutput;

    Boolean mShortcutCaptionMode;
    String mShortcutCaption;
};

} // namespace Menu
} // namespace View
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_VIEW_MENU_ICONMENIITEMVIEW_H__
