
#ifndef  __MENUPOPUPHELPER_H__
#define  __MENUPOPUPHELPER_H__

#include "widget/BaseAdapter.h"
#include "widget/AdapterMacro.h"

using Elastos::Droid::Content::IContext;
using Elastos::Droid::View::IViewOnKeyListener;
using Elastos::Droid::View::EIID_IViewOnKeyListener;
using Elastos::Droid::View::IOnGlobalLayoutListener;
using Elastos::Droid::View::EIID_IOnGlobalLayoutListener;
using Elastos::Droid::View::IViewOnAttachStateChangeListener;
using Elastos::Droid::View::EIID_IViewOnAttachStateChangeListener;
using Elastos::Droid::View::Menu::IMenuPresenter;
using Elastos::Droid::View::Menu::EIID_IMenuPresenter;
using Elastos::Droid::Widget::IPopupWindowOnDismissListener;
using Elastos::Droid::Widget::EIID_IPopupWindowOnDismissListener;
using Elastos::Droid::Widget::IAdapterViewOnItemClickListener;
using Elastos::Droid::Widget::EIID_IAdapterViewOnItemClickListener;
using Elastos::Droid::Widget::BaseAdapter;
using Elastos::Droid::Widget::IBaseAdapter;
using Elastos::Droid::Widget::IAdapterView;
using Elastos::Droid::Widget::IListAdapter;
using Elastos::Droid::Widget::IListPopupWindow;
using Elastos::Droid::Widget::ISpinnerAdapter;

namespace Elastos {
namespace Droid {
namespace View {
namespace Menu {

/**
 * Presents a menu as a small, simple popup anchored to another view.
 * @hide
 */
class MenuPopupHelper
{
private:
    class _MenuAdapter : public BaseAdapter
    {
    public:
        _MenuAdapter(
            /* [in] */ IMenuBuilder* menu,
            /* [in] */ MenuPopupHelper* host);

        CARAPI_(Int32) GetCount();

        CARAPI_(AutoPtr<IInterface>) GetItem(
            /* [in] */ Int32 position);

        CARAPI_(Int64) GetItemId(
            /* [in] */ Int32 position);

        CARAPI_(AutoPtr<IView>) GetView(
            /* [in] */ Int32 position,
            /* [in] */ IView* convertView,
            /* [in] */ IViewGroup* parent);

        // /@Override
        CARAPI NotifyDataSetChanged();

        CARAPI NotifyDataSetInvalidated();

    protected:
        CARAPI_(void) FindExpandedIndex();

    protected:
        // AdapterMenu usually holds adapter, we use weak-reference here.
        AutoPtr<IWeakReference> mWeakAdapterMenu;
        // AutoPtr<IMenuBuilder> mAdapterMenu;
        Int32 mExpandedIndex;
        MenuPopupHelper*  mHost;

        friend class MenuPopupHelper;
    };

    class MenuAdapter
        : public ElRefBase
        , public _MenuAdapter
        , public IMenuAdapter
        , public ISpinnerAdapter
    {
    public:
        IADAPTER_METHODS_DECL()

        IBASEADAPTER_METHODS_DECL()

        ILISTADAPTER_METHODS_DECL()

        ISPINNERADAPTER_METHODS_DECL()

        MenuAdapter(
            /* [in] */ IMenuBuilder* menu,
            /* [in] */ MenuPopupHelper* host);

        CAR_INTERFACE_DECL()
    };

    class MyListener
        : public IAdapterViewOnItemClickListener
        , public IPopupWindowOnDismissListener
        , public ElRefBase
    {
    public:
        CAR_INTERFACE_DECL()

        MyListener(
            /* [in] */ MenuPopupHelper* owner)
            : mOwner(owner)
        {}

        CARAPI OnItemClick(
            /* [in] */ IAdapterView* parent,
            /* [in] */ IView* v,
            /* [in] */ Int32 position,
            /* [in] */ Int64 id);

        CARAPI OnDismiss();

    private:
        MenuPopupHelper* mOwner;
    };

public:
    MenuPopupHelper(
        /* [in] */ IContext* context,
        /* [in] */ IMenuBuilder* menu);

    MenuPopupHelper(
        /* [in] */ IContext* context,
        /* [in] */ IMenuBuilder* menu,
        /* [in] */ IView* anchorView);

    MenuPopupHelper(
        /* [in] */ IContext* context,
        /* [in] */ IMenuBuilder* menu,
        /* [in] */ IView* anchorView,
        /* [in] */ Boolean overflowOnly);

    virtual CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid) = 0;

    CARAPI SetAnchorView(
        /* [in] */ IView* anchor);

    CARAPI SetForceShowIcon(
        /* [in] */ Boolean forceShow);

    CARAPI Show();

    CARAPI_(Boolean) TryShow();

    CARAPI Dismiss();

    CARAPI OnDismiss();

    CARAPI_(Boolean) IsShowing();

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

    //@Override
    CARAPI OnGlobalLayout();

    //@Override
    CARAPI OnViewAttachedToWindow(
        /* [in] */ IView* v);

    //@Override
    CARAPI OnViewDetachedFromWindow(
        /* [in] */ IView* v);

    //@Override
    CARAPI InitForMenu(
        /* [in] */ IContext* context,
        /* [in] */ IMenuBuilder* menu);

    //@Override
    CARAPI GetMenuView(
        /* [in] */ IViewGroup* root,
        /* [out] */ IMenuView** view);

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
    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ IMenuBuilder* menu,
        /* [in] */ IView* anchorView = NULL,
        /* [in] */ Boolean overflowOnly = FALSE);

    MenuPopupHelper();
private:

    CARAPI_(Int32) MeasureContentWidth(
        /* [in] */ IListAdapter* adapter);

private:
    static String TAG;

    static const Int32 ITEM_LAYOUT;

    AutoPtr<IContext> mContext;
    AutoPtr<ILayoutInflater> mInflater;
    AutoPtr<IListPopupWindow> mPopup;
    AutoPtr<IMenuBuilder> mMenu;
    Int32 mPopupMaxWidth;
    AutoPtr<IView> mAnchorView;
    Boolean mOverflowOnly;
    AutoPtr<IViewTreeObserver> mTreeObserver;

    AutoPtr<MenuAdapter> mAdapter;
    AutoPtr<IMenuPresenterCallback> mPresenterCallback;
    Boolean mForceShowIcon;
    AutoPtr<IViewGroup> mMeasureParent;
};

} // namespace Menu
} // namespace View
} // namespace Droid
} // namespace Elastos

#endif  //__MENUPOPUPHELPER_H__
