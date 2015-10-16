
#ifndef __ELASTOS_DROID_VIEW_MENU_ACTIONMENUPRESENTER_H__
#define  __ELASTOS_DROID_VIEW_MENU_ACTIONMENUPRESENTER_H__

#include "elastos/droid/widget/ImageButton.h"

#include "elastos/droid/view/menu/MenuPopupHelper.h"
#include "elastos/droid/view/menu/BaseMenuPresenter.h"
#include <elastos/utility/etl/HashMap.h>


using Elastos::Utility::Etl::HashMap;
using Elastos::Droid::View::IKeyEventCallback;
using Elastos::Droid::View::ISubUiVisibilityListener;
using Elastos::Droid::View::Accessibility::IAccessibilityEventSource;
using Elastos::Droid::View::Menu::IActionMenuChildView;
using Elastos::Droid::View::Menu::MenuPopupHelper;
using Elastos::Droid::View::Menu::IActionMenuChildView;
using Elastos::Droid::Widget::ImageButton;
using Elastos::Droid::Widget::IImageButton;
using Elastos::Droid::Widget::ImageViewScaleType;
using Elastos::Droid::Widget::IAdapterViewOnItemClickListener;
using Elastos::Droid::Widget::EIID_IAdapterViewOnItemClickListener;
using Elastos::Droid::Widget::IPopupWindowOnDismissListener;
using Elastos::Droid::Widget::EIID_IPopupWindowOnDismissListener;
using Elastos::Droid::Graphics::Drawable::IDrawableCallback;
using Elastos::Droid::View::IViewOnKeyListener;
using Elastos::Droid::View::EIID_IViewOnKeyListener;
using Elastos::Droid::View::IOnGlobalLayoutListener;
using Elastos::Droid::View::EIID_IOnGlobalLayoutListener;
using Elastos::Droid::View::IViewOnAttachStateChangeListener;
using Elastos::Droid::View::EIID_IViewOnAttachStateChangeListener;

namespace Elastos {
namespace Droid {
namespace View {
namespace Menu {

class ActionMenuPresenter : public BaseMenuPresenter
{
private:
    class _OverflowMenuButton
        : public ImageButton
    {
    public:
        _OverflowMenuButton(
            /* [in] */ ActionMenuPresenter* host);

        //@Override
        CARAPI_(Boolean) PerformClick();


    protected:
        ActionMenuPresenter*  mHost;
    };

    class OverflowMenuButton
        : public _OverflowMenuButton
        , public IImageButton
        , public IDrawableCallback
        , public IKeyEventCallback
        , public IAccessibilityEventSource
        , public IActionMenuChildView
        , public IWeakReferenceSource
        , public ElRefBase
    {
    public:
        IVIEW_METHODS_DECL()

        IDRAWABLECALLBACK_METHODS_DECL()

        IKEYEVENTCALLBACK_METHODS_DECL()

        IACCESSIBILITYEVENTSOURCE_METHODS_DECL()

        OverflowMenuButton(
            /* [in] */ IContext* context,
            /* [in] */ ActionMenuPresenter* host);

        CARAPI_(PInterface) Probe(
            /* [in] */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        CARAPI GetWeakReference(
            /* [out] */ IWeakReference** weakReference);

        CARAPI NeedsDividerBefore(
            /* [out] */ Boolean* need);

        CARAPI NeedsDividerAfter(
            /* [out] */ Boolean* need);

        CARAPI GetAdjustViewBounds(
            /* [out] */ Boolean* res);

        CARAPI SetAdjustViewBounds(
            /* [in] */ Boolean adjustViewBounds);

        CARAPI GetMaxWidth(
            /* [out] */ Int32* width);

        CARAPI SetMaxWidth(
            /* [in] */ Int32 maxWidth);

        CARAPI GetMaxHeight(
            /* [out] */ Int32* height);

        CARAPI SetMaxHeight(
            /* [in] */ Int32 maxHeight);

        CARAPI GetDrawable(
            /* [out] */ IDrawable** drawable);

        CARAPI SetImageResource(
            /* [in] */ Int32 resId);

        CARAPI SetImageURI(
            /* [in] */ IUri* uri);

        CARAPI SetImageDrawable(
            /* [in] */ IDrawable* drawable);

        CARAPI SetImageBitmap(
            /* [in] */ IBitmap* bm);

        CARAPI SetImageState(
            /* [in] */ ArrayOf<Int32>* state,
            /* [in] */ Boolean mg);

        CARAPI SetImageLevel(
            /* [in] */ Int32 level);

        CARAPI SetScaleType(
            /* [in] */ ImageViewScaleType scaleType);

        CARAPI GetScaleType(
            /* [out] */ ImageViewScaleType* scaleType);

        CARAPI GetImageMatrix(
            /* [out] */ IMatrix** matrix);

        CARAPI SetImageMatrix(
            /* [in] */ IMatrix* matrix);

        CARAPI GetCropToPadding(
            /* [out] */ Boolean* padding);

        CARAPI SetCropToPadding(
            /* [in] */ Boolean cropToPadding);

        CARAPI SetBaseline(
            /* [in] */ Int32 baseline);

        CARAPI SetBaselineAlignBottom(
            /* [in] */ Boolean aligned);

        CARAPI GetBaselineAlignBottom(
            /* [out] */ Boolean* aligned);

        CARAPI SetColorFilter(
            /* [in] */ Int32 color);

        CARAPI SetColorFilter(
            /* [in] */ Int32 color,
            /* [in] */ PorterDuffMode mode);

        CARAPI ClearColorFilter();

        CARAPI GetColorFilter(
            /* [out] */ IColorFilter** filter);

        CARAPI SetColorFilter(
            /* [in] */ IColorFilter* cf);

        CARAPI GetImageAlpha(
            /* [out] */ Int32* alpha);

        CARAPI SetImageAlpha(
            /* [in] */ Int32 alpha);

        CARAPI SetAlpha(
            /* [in] */ Int32 alpha);
    protected:
        CARAPI_(void) OnMeasure(
            /* [in] */ Int32 widthMeasureSpec,
            /* [in] */ Int32 heightMeasureSpec);
    };

    class OverflowPopup
        : public MenuPopupHelper
        , public IMenuPopupHelper
        , public IAdapterViewOnItemClickListener
        , public IViewOnKeyListener
        , public IOnGlobalLayoutListener
        , public IPopupWindowOnDismissListener
        , public IViewOnAttachStateChangeListener
        , public IMenuPresenter
        , public IWeakReferenceSource
        , public ElRefBase
    {
    public:
        OverflowPopup(
            /* [in] */ IContext* context,
            /* [in] */ IMenuBuilder* menu,
            /* [in] */ IView* anchorView,
            /* [in] */ Boolean overflowOnly,
            /* [in] */ ActionMenuPresenter* host);

        CAR_INTERFACE_DECL()

        CARAPI GetWeakReference(
            /* [out] */ IWeakReference** weakReference);

        CARAPI OnDismiss();

        CARAPI SetAnchorView(
            /* [in] */ IView* anchor);

        CARAPI SetForceShowIcon(
            /* [in] */ Boolean forceShow);

        CARAPI TryShow(
            /* [out] */ Boolean* rst);

        CARAPI Show();

        CARAPI Dismiss();

        CARAPI IsShowing(
            /* [out] */ Boolean* rst);

        CARAPI OnItemClick(
            /* [in] */ IAdapterView* parent,
            /* [in] */ IView* view,
            /* [in] */ Int32 position,
            /* [in] */ Int64 id);

        CARAPI OnKey(
            /* [in] */ IView* v,
            /* [in] */ Int32 keyCode,
            /* [in] */ IKeyEvent* event,
            /* [out] */ Boolean* result);

        CARAPI OnGlobalLayout();

        CARAPI OnViewAttachedToWindow(
                    /* [in] */ IView* v);

        CARAPI OnViewDetachedFromWindow(
            /* [in] */ IView* v);

        CARAPI InitForMenu(
            /* [in] */ IContext* context,
            /* [in] */ IMenuBuilder* menu);

        CARAPI GetMenuView(
            /* [in] */ IViewGroup* root,
            /* [out] */ IMenuView** view);

        CARAPI UpdateMenuView(
            /* [in] */ Boolean cleared);

        CARAPI SetCallback(
            /* [in] */ IMenuPresenterCallback* cb);

        CARAPI OnSubMenuSelected(
            /* [in] */ ISubMenuBuilder* subMenu,
            /* [out] */ Boolean* handled);

        CARAPI OnCloseMenu(
            /* [in] */ IMenuBuilder* menu,
            /* [in] */ Boolean allMenusAreClosing);

        CARAPI FlagActionItems(
            /* [out] */ Boolean* shown);

        CARAPI ExpandItemActionView(
            /* [in] */ IMenuBuilder* menu,
            /* [in] */ IMenuItemImpl* item,
            /* [out] */ Boolean* expanded);

        CARAPI CollapseItemActionView(
            /* [in] */ IMenuBuilder* menu,
            /* [in] */ IMenuItemImpl* item,
            /* [out] */ Boolean* collapsed);

        CARAPI GetId(
            /* [out] */ Int32* id);

        CARAPI OnSaveInstanceState(
            /* [out] */ IParcelable** pa);

        CARAPI OnRestoreInstanceState(
            /* [in] */ IParcelable* state);

    private:
        ActionMenuPresenter*  mHost;
    };

    class ActionButtonSubmenu
        : public MenuPopupHelper
        , public IMenuPopupHelper
        , public IAdapterViewOnItemClickListener
        , public IViewOnKeyListener
        , public IOnGlobalLayoutListener
        , public IPopupWindowOnDismissListener
        , public IViewOnAttachStateChangeListener
        , public IMenuPresenter
        , public IWeakReferenceSource
        , public ElRefBase
    {
    public:
        ActionButtonSubmenu(
            /* [in] */ IContext* context,
            /* [in] */ ISubMenuBuilder* subMenu,
            /* [in] */ ActionMenuPresenter* host);

        CAR_INTERFACE_DECL()

        CARAPI GetWeakReference(
            /* [out] */ IWeakReference** weakReference);

        CARAPI OnDismiss();

        CARAPI SetAnchorView(
            /* [in] */ IView* anchor);

        CARAPI SetForceShowIcon(
            /* [in] */ Boolean forceShow);

        CARAPI Show();

        CARAPI TryShow(
            /* [out] */ Boolean* rst);

        CARAPI Dismiss();

        CARAPI IsShowing(
            /* [out] */ Boolean* rst);

        CARAPI OnItemClick(
            /* [in] */ IAdapterView* parent,
            /* [in] */ IView* view,
            /* [in] */ Int32 position,
            /* [in] */ Int64 id);

        CARAPI OnKey(
            /* [in] */ IView* v,
            /* [in] */ Int32 keyCode,
            /* [in] */ IKeyEvent* event,
            /* [out] */ Boolean* result);

        CARAPI OnGlobalLayout();

        CARAPI OnViewAttachedToWindow(
                    /* [in] */ IView* v);

        CARAPI OnViewDetachedFromWindow(
            /* [in] */ IView* v);

        CARAPI InitForMenu(
            /* [in] */ IContext* context,
            /* [in] */ IMenuBuilder* menu);

        CARAPI GetMenuView(
            /* [in] */ IViewGroup* root,
            /* [out] */ IMenuView** view);

        CARAPI UpdateMenuView(
            /* [in] */ Boolean cleared);

        CARAPI SetCallback(
            /* [in] */ IMenuPresenterCallback* cb);

        CARAPI OnSubMenuSelected(
            /* [in] */ ISubMenuBuilder* subMenu,
            /* [out] */ Boolean* handled);

        CARAPI OnCloseMenu(
            /* [in] */ IMenuBuilder* menu,
            /* [in] */ Boolean allMenusAreClosing);

        CARAPI FlagActionItems(
            /* [out] */ Boolean* shown);

        CARAPI ExpandItemActionView(
            /* [in] */ IMenuBuilder* menu,
            /* [in] */ IMenuItemImpl* item,
            /* [out] */ Boolean* expanded);

        CARAPI CollapseItemActionView(
            /* [in] */ IMenuBuilder* menu,
            /* [in] */ IMenuItemImpl* item,
            /* [out] */ Boolean* collapsed);

        CARAPI GetId(
            /* [out] */ Int32* id);

        CARAPI OnSaveInstanceState(
            /* [out] */ IParcelable** pa);

        CARAPI OnRestoreInstanceState(
            /* [in] */ IParcelable* state);
    private:
        AutoPtr<ISubMenuBuilder> mSubMenu;
        ActionMenuPresenter*  mHost;
    };

    class PopupPresenterCallback
        : public ElRefBase
        , public IMenuPresenterCallback
    {
    public:
        CAR_INTERFACE_DECL()

        PopupPresenterCallback(
            /* [in] */ ActionMenuPresenter* host);

        //@Override
        CARAPI OnOpenSubMenu(
            /* [in] */ IMenuBuilder* subMenu,
            /* [out] */ Boolean* result);

        //@Override
        CARAPI OnCloseMenu(
            /* [in] */ IMenuBuilder* menu,
            /* [in] */ Boolean allMenusAreClosing);

    private:
        ActionMenuPresenter*  mHost;
    };

    class OpenOverflowRunnable
        : public ElRefBase
        , public IRunnable
    {
    public:
        CAR_INTERFACE_DECL()

        OpenOverflowRunnable(
            /* [in] */ OverflowPopup* popup,
            /* [in] */ ActionMenuPresenter* host);

        CARAPI Run();

    private:
        AutoPtr<OverflowPopup> mPopup;
        ActionMenuPresenter* mHost;
    };

public:
    ActionMenuPresenter(
        /* [in] */ IContext* context);

    //@Override
    CARAPI InitForMenu(
        /* [in] */ IContext* context,
        /* [in] */ IMenuBuilder* menu);

    CARAPI OnConfigurationChanged(
        /* [in] */ IConfiguration* newConfig);

    CARAPI_(void) SetWidthLimit(
        /* [in] */ Int32 width,
        /* [in] */ Boolean strict);

    CARAPI_(void) SetReserveOverflow(
        /* [in] */ Boolean reserveOverflow);

    CARAPI_(void) SetItemLimit(
        /* [in] */ Int32 itemCount);

    CARAPI_(void) SetExpandedActionViewsExclusive(
        /* [in] */ Boolean isExclusive);

    //@Override
    CARAPI GetMenuView(
        /* [in] */ IViewGroup* root,
        /* [out] */ IMenuView** view);

    //@Override
    CARAPI GetItemView(
        /* [in] */ IMenuItemImpl* item,
        /* [in] */ IView* convertView,
        /* [in] */ IViewGroup* parent,
        /* [out] */ IView** view);

    //@Override
    CARAPI_(void) BindItemView(
        /* [in] */ IMenuItemImpl* item,
        /* [in] */ IMenuItemView* itemView);

    //@Override
    CARAPI_(Boolean) ShouldIncludeItem(
        /* [in] */ Int32 childIndex,
        /* [in] */ IMenuItemImpl* item);

    //@Override
    CARAPI UpdateMenuView(
        /* [in] */ Boolean cleared);

    //@Override
    CARAPI_(Boolean) FilterLeftoverView(
        /* [in] */ IViewGroup* parent,
        /* [in] */ Int32 childIndex);

    CARAPI OnSubMenuSelected(
        /* [in] */ ISubMenuBuilder* subMenu,
        /* [out] */ Boolean* result);

    /**
     * Display the overflow menu if one is present.
     * @return TRUE if the overflow menu was shown, FALSE otherwise.
     */
    CARAPI_(Boolean) ShowOverflowMenu();

    /**
     * Hide the overflow menu if it is currently showing.
     *
     * @return TRUE if the overflow menu was hidden, FALSE otherwise.
     */
    CARAPI_(Boolean) HideOverflowMenu();

    /**
     * Dismiss all popup menus - overflow and submenus.
     * @return TRUE if popups were dismissed, FALSE otherwise. (This can be because none were open.)
     */
    CARAPI_(Boolean) DismissPopupMenus();

    /**
     * Dismiss all submenu popups.
     *
     * @return TRUE if popups were dismissed, FALSE otherwise. (This can be because none were open.)
     */
    CARAPI_(Boolean) HideSubMenus();

    /**
     * @return TRUE if the overflow menu is currently showing
     */
    CARAPI_(Boolean) IsOverflowMenuShowing();

    /**
     * @return TRUE if space has been reserved in the action menu for an overflow item.
     */
    CARAPI_(Boolean) IsOverflowReserved();

    CARAPI FlagActionItems(
        /* [out] */ Boolean* result);

    //@Override
    CARAPI OnCloseMenu(
        /* [in] */ IMenuBuilder* menu,
        /* [in] */ Boolean allMenusAreClosing);

    //@Override
    CARAPI OnSaveInstanceState(
        /* [out] */ IParcelable** parcel);

    //@Override
    CARAPI OnRestoreInstanceState(
        /* [in] */ IParcelable* state);

    //@Override
    CARAPI OnSubUiVisibilityChanged(
        /* [in] */ Boolean isVisible);

protected:
    ActionMenuPresenter();

private:
    CARAPI_(AutoPtr<IView>) FindViewForItem(
        /* [in] */ IMenuItem* item);

private:
    static String TAG;

    AutoPtr<IView> mOverflowButton;
    Boolean mReserveOverflow;
    Boolean mReserveOverflowSet;
    Int32 mWidthLimit;
    Int32 mActionItemWidthLimit;
    Int32 mMaxItems;
    Boolean mMaxItemsSet;
    Boolean mStrictWidthLimit;
    Boolean mWidthLimitSet;
    Boolean mExpandedActionViewsExclusive;

    Int32 mMinCellSize;

    // Group IDs that have been added as actions - used temporarily, allocated here for reuse.
    HashMap<Int32, Boolean> mActionButtonGroups;

    AutoPtr<IView> mScrapActionButtonView;

    AutoPtr<OverflowPopup> mOverflowPopup;
    AutoPtr<ActionButtonSubmenu> mActionButtonPopup;

    AutoPtr<OpenOverflowRunnable> mPostedOpenRunnable;

    AutoPtr<PopupPresenterCallback> mPopupPresenterCallback;
    Int32 mOpenSubMenuId;
};

} // namespace Menu
} // namespace View
} // namespace Droid
} // namespace Elastos

#endif  //__ELASTOS_DROID_VIEW_MENU_ACTIONMENUPRESENTER_H__
