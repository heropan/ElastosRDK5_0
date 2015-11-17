
#ifndef __ELASTOS_DROID_INTERNAL_VIEW_MENU_LISTMENUPRESENTER_H__
#define __ELASTOS_DROID_INTERNAL_VIEW_MENU_LISTMENUPRESENTER_H__

#include "elastos/droid/widget/BaseAdapter.h"

using Elastos::Droid::Content::IContext;
using Elastos::Droid::Os::IBundle;
using Elastos::Droid::View::Menu::IMenuPresenter;
using Elastos::Droid::View::Menu::IExpandedMenuView;
using Elastos::Droid::Widget::IAdapterViewOnItemClickListener;
using Elastos::Droid::Widget::IBaseAdapter;
using Elastos::Droid::Widget::BaseAdapter;
using Elastos::Droid::Widget::IListAdapter;
using Elastos::Droid::Widget::IAdapterView;
using Elastos::Droid::Widget::ISpinnerAdapter;

namespace Elastos {
namespace Droid {
namespace Internal {
namespace View {
namespace Menu {

class ListMenuPresenter
{
private:
    class _MenuAdapter
        : public ElRefBase
        , public BaseAdapter
    {
    public:
        _MenuAdapter(
            /* [in] */ ListMenuPresenter* host);

        virtual CARAPI_(AutoPtr<IInterface>) GetItem(
            /* [in] */ Int32 position);

        CARAPI_(Int32) GetCount();

        CARAPI_(Int64) GetItemId(
            /* [in] */ Int32 position);

        CARAPI_(AutoPtr<IView>) GetView(
            /* [in] */ Int32 position,
            /* [in] */ IView* convertView,
            /* [in] */ IViewGroup* parent);

        CARAPI_(void) FindExpandedIndex();

        //@Override
        CARAPI NotifyDataSetChanged();

        CARAPI NotifyDataSetInvalidated();

    protected:
        Int32 mExpandedIndex;
        ListMenuPresenter*  mHost;
    };

    class MenuAdapter
        : public _MenuAdapter
        , public IBaseAdapter
        , public ISpinnerAdapter
    {
    public:
        IADAPTER_METHODS_DECL()
        IBASEADAPTER_METHODS_DECL()
        ILISTADAPTER_METHODS_DECL()
        ISPINNERADAPTER_METHODS_DECL()

        MenuAdapter(
            /* [in] */ ListMenuPresenter* host);

        CARAPI_(PInterface) Probe(
            /* [in] */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);
    };

    class OnItemClickListener
        : public IAdapterViewOnItemClickListener
        , public ElRefBase
    {
    public:
        CAR_INTERFACE_DECL()

        OnItemClickListener(
            /* [in] */ ListMenuPresenter* owner)
            : mOwner(owner)
        {}

        CARAPI OnItemClick(
            /* [in] */ IAdapterView* parent,
            /* [in] */ IView* v,
            /* [in] */ Int32 position,
            /* [in] */ Int64 id);

    private:
        ListMenuPresenter* mOwner;
    };

public:

    virtual CARAPI_(PInterface) Probe(
        /* [in]  */ REIID riid) = 0;
    /**
     * Construct a new ListMenuPresenter.
     * @param context Context to use for theming. This will supersede the context provided
     *                to initForMenu when this presenter is added.
     * @param itemLayoutRes Layout resource for individual item views.
     */
    ListMenuPresenter(
        /* [in] */ IContext* context,
        /* [in] */ Int32 itemLayoutRes);

    /**
     * Construct a new ListMenuPresenter.
     * @param itemLayoutRes Layout resource for individual item views.
     * @param themeRes Resource ID of a theme to use for views.
     */
    ListMenuPresenter(
        /* [in] */ Int32 itemLayoutRes,
        /* [in] */ Int32 themeRes);

    //@Override
    CARAPI InitForMenu(
        /* [in] */ IContext* context,
        /* [in] */ IMenuBuilder* menu);

    //@Override
    CARAPI GetMenuView(
        /* [in] */ IViewGroup* root,
        /* [out] */ IMenuView** view);

    /**
     * Call this instead of getMenuView if you want to manage your own ListView.
     * For proper operation, the ListView hosting this adapter should add
     * this presenter as an OnItemClickListener.
     *
     * @return A ListAdapter containing the items in the menu.
     */
    CARAPI_(AutoPtr<IListAdapter>) GetAdapter();

    //@Override
    CARAPI UpdateMenuView(
        /* [in] */ Boolean cleared);

    //@Override
    CARAPI SetCallback(
        /* [in] */ IMenuPresenterCallback* cb);

    //@Override
    CARAPI OnSubMenuSelected(
        /* [in] */ ISubMenuBuilder* subMenu,
        /* [out] */ Boolean* selected);

    //@Override
    CARAPI OnCloseMenu(
        /* [in] */ IMenuBuilder* menu,
        /* [in] */ Boolean allMenusAreClosing);

    CARAPI SetItemIndexOffset(
        /* [in] */ Int32 offset);

    //@Override
    CARAPI OnItemClick(
        /* [in] */ IAdapterView* parent,
        /* [in] */ IView* view,
        /* [in] */ Int32 position,
        /* [in] */ Int64 id);

    //@Override
    CARAPI FlagActionItems(
        /* [out] */ Boolean* result);

    CARAPI ExpandItemActionView(
        /* [in] */ IMenuBuilder* menu,
        /* [in] */ IMenuItemImpl* item,
        /* [out] */ Boolean* result);

    CARAPI CollapseItemActionView(
        /* [in] */ IMenuBuilder* menu,
        /* [in] */ IMenuItemImpl* item,
        /* [out] */ Boolean* result);

    CARAPI SaveHierarchyState(
        /* [in] */ IBundle* outState);

    CARAPI RestoreHierarchyState(
        /* [in] */ IBundle* inState);

    CARAPI SetId(
        /* [in] */ Int32 id);

    //@Override
    CARAPI GetId(
        /* [out] */ Int32* id);

    //@Override
    CARAPI OnSaveInstanceState(
        /* [out] */ IParcelable** parcel);

    //@Override
    CARAPI OnRestoreInstanceState(
        /* [in] */ IParcelable* state);

protected:
    CARAPI_(Int32) GetItemIndexOffset();

    ListMenuPresenter();

    CARAPI Init(
        /* [in] */ Int32 itemLayoutRes,
        /* [in] */ Int32 themeRes);

    CARAPI Init(
        /* [in] */ IContext* ctx,
        /* [in] */ Int32 itemLayoutRes);

private:
    static String TAG;

    AutoPtr<IContext> mContext;
    AutoPtr<ILayoutInflater> mInflater;
    AutoPtr<IMenuBuilder> mMenu;

    AutoPtr<IExpandedMenuView> mMenuView;

    Int32 mItemIndexOffset;
    Int32 mThemeRes;
    Int32 mItemLayoutRes;

    AutoPtr<IMenuPresenterCallback> mCallback;
    AutoPtr<MenuAdapter> mAdapter;

    Int32 mId;

    static String VIEWS_TAG;

};

} // namespace Menu
} // namespace View
} // namespace Internal
} // namespace Droid
} // namespace Elastos

#endif  //__ELASTOS_DROID_INTERNAL_VIEW_MENU_LISTMENUPRESENTER_H__
