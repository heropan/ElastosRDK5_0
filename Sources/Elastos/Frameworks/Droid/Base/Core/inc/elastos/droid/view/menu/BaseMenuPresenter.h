
#ifndef __ELASTOS_DROID_VIEW_MENU_BASEMENUPRESENTER_H__
#define  __ELASTOS_DROID_VIEW_MENU_BASEMENUPRESENTER_H__

#include "elastos/droid/ext/frameworkext.h"

using Elastos::Droid::Content::IContext;
using Elastos::Droid::View::IViewGroup;
using Elastos::Droid::View::IView;
using Elastos::Droid::View::Menu::IMenuBuilder;
using Elastos::Droid::View::Menu::IMenuPresenter;
using Elastos::Droid::View::Menu::IMenuView;
using Elastos::Droid::View::Menu::IMenuPresenterCallback;
using Elastos::Droid::View::Menu::IMenuItemView;

namespace Elastos {
namespace Droid {
namespace View {
namespace Menu {


/**
 * Base class for MenuPresenters that have a consistent container view and item
 * views. Behaves similarly to an AdapterView in that existing item views will
 * be reused if possible when items change.
 */
class BaseMenuPresenter
{
public:

    virtual CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid) = 0;

    BaseMenuPresenter();

    /**
     * Construct a new BaseMenuPresenter.
     *
     * @param context Context for generating system-supplied views
     * @param menuLayoutRes Layout resource ID for the menu container view
     * @param itemLayoutRes Layout resource ID for a single item view
     */
    BaseMenuPresenter(
        /* [in] */ IContext* context,
        /* [in] */ Int32 menuLayoutRes,
        /* [in] */ Int32 itemLayoutRes);

    //@Override
    virtual CARAPI InitForMenu(
        /* [in] */ IContext* context,
        /* [in] */ IMenuBuilder* menu);

    //@Override
    virtual CARAPI GetMenuView(
        /* [in] */ IViewGroup* root,
        /* [out] */ IMenuView** view);

    /**
     * Reuses item views when it can
     */
    virtual CARAPI UpdateMenuView(
        /* [in] */ Boolean cleared);

    virtual CARAPI SetCallback(
        /* [in] */ IMenuPresenterCallback* cb);

    /**
     * Create a new item view that can be re-bound to other item data later.
     *
     * @return The new item view
     */
    virtual CARAPI_(AutoPtr<IMenuItemView>) CreateItemView(
        /* [in] */ IViewGroup* parent);

    /**
     * Prepare an item view for use. See AdapterView for the basic idea at work here.
     * This may require creating a new item view, but well-behaved implementations will
     * re-use the view passed as convertView if present. The returned view will be populated
     * with data from the item parameter.
     *
     * @param item Item to present
     * @param convertView Existing view to reuse
     * @param parent Intended parent view - use for inflation.
     * @return View that presents the requested menu item
     */
    virtual CARAPI_(AutoPtr<IView>) GetItemView(
        /* [in] */ IMenuItemImpl* item,
        /* [in] */ IView* convertView,
        /* [in] */ IViewGroup* parent);

    /**
     * Filter item by child index and item data.
     *
     * @param childIndex Indended presentation index of this item
     * @param item Item to present
     * @return true if this item should be included in this menu presentation; false otherwise
     */
    virtual CARAPI_(Boolean) ShouldIncludeItem(
        /* [in] */ Int32 childIndex,
        /* [in] */ IMenuItemImpl* item);

    virtual CARAPI OnCloseMenu(
        /* [in] */ IMenuBuilder* menu,
        /* [in] */ Boolean allMenusAreClosing);

    virtual CARAPI OnSubMenuSelected(
        /* [in] */ ISubMenuBuilder* menu,
        /* [out] */ Boolean* result);

    virtual CARAPI FlagActionItems(
        /* [out] */ Boolean* result);

    virtual CARAPI ExpandItemActionView(
        /* [in] */ IMenuBuilder* menu,
        /* [in] */ IMenuItemImpl* item,
        /* [out] */ Boolean* result);

    virtual CARAPI CollapseItemActionView(
        /* [in] */ IMenuBuilder* menu,
        /* [in] */ IMenuItemImpl* item,
        /* [out] */ Boolean* result);

    virtual CARAPI GetId(
        /* [out] */ Int32* id);

    virtual CARAPI SetId(
        /* [in] */ Int32 id);

protected:
    /**
     * Add an item view at the given index.
     *
     * @param itemView View to add
     * @param childIndex Index within the parent to insert at
     */
    virtual CARAPI_(void) AddItemView(
        /* [in] */ IView* itemView,
        /* [in] */ Int32 childIndex);

    /**
     * Filter the child view at index and remove it if appropriate.
     * @param parent Parent to filter from
     * @param childIndex Index to filter
     * @return true if the child view at index was removed
     */
    virtual CARAPI_(Boolean) FilterLeftoverView(
        /* [in] */ IViewGroup* parent,
        /* [in] */ Int32 childIndex);

    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ Int32 menuLayoutRes,
        /* [in] */ Int32 itemLayoutRes);

protected:
    AutoPtr<IContext> mSystemContext;
    AutoPtr<IContext> mContext;
    AutoPtr<IMenuBuilder> mMenu;
    AutoPtr<ILayoutInflater> mSystemInflater;
    AutoPtr<ILayoutInflater> mInflater;
    AutoPtr<IMenuView> mMenuView;

private:
    AutoPtr<IMenuPresenterCallback> mCallback;

    Int32 mMenuLayoutRes;
    Int32 mItemLayoutRes;

    Int32 mId;
};

} // namespace Menu
} // namespace View
} // namespace Droid
} // namespace Elastos

#endif  //__ELASTOS_DROID_VIEW_MENU_BASEMENUPRESENTER_H__
