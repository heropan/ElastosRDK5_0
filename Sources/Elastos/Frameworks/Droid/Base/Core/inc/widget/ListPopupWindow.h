#ifndef __LISTPOPUPWINDOW_H__
#define __LISTPOPUPWINDOW_H__

#include "ext/frameworkext.h"
#include "widget/ListView.h"




#include "os/Runnable.h"

using Elastos::Droid::Os::Runnable;
using Elastos::Droid::Widget::IListView;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Database::IDataSetObserver;
using Elastos::Droid::View::IView;
using Elastos::Droid::View::IViewOnHoverListener;
using Elastos::Droid::View::IViewOnTouchListener;
using Elastos::Droid::View::IViewGroupLayoutParams;
using Elastos::Droid::View::EIID_IViewOnTouchListener;
using Elastos::Droid::View::EIID_View;
using Elastos::Droid::View::EIID_IView;
using Elastos::Droid::View::IViewGroup;
using Elastos::Droid::View::EIID_IViewGroup;
using Elastos::Droid::View::EIID_ViewGroup;
using Elastos::Droid::View::IViewParent;
using Elastos::Droid::View::EIID_IViewParent;
using Elastos::Droid::View::IViewManager;
using Elastos::Droid::View::EIID_IViewManager;
using Elastos::Droid::View::IKeyEventCallback;
using Elastos::Droid::View::EIID_IKeyEventCallback;
using Elastos::Droid::View::Accessibility::IAccessibilityEventSource;
using Elastos::Droid::View::Accessibility::EIID_IAccessibilityEventSource;
using Elastos::Droid::Graphics::Drawable::IDrawableCallback;
using Elastos::Droid::Graphics::Drawable::EIID_IDrawableCallback;

namespace Elastos{
namespace Droid{
namespace Widget{

class Spinner;

class ListPopupWindow
{
private:
    class _DropDownListView
        : public ListView
        , public ElRefBase
    {
    public:
        CARAPI_(Boolean) IsInTouchMode();

        CARAPI_(Boolean) HasWindowFocus();

        CARAPI_(Boolean) IsFocused();

        CARAPI_(Boolean) HasFocus();

        virtual CARAPI_(AutoPtr<IAnimation>) GetAnimation();
    protected:
        _DropDownListView();

        CARAPI_(AutoPtr<IView>) ObtainView(
            /* [in] */ Int32 position,
            /* [in] */ ArrayOf<Boolean>* isScrap);

    protected:
        Boolean mListSelectionHidden;
        Boolean mHijackFocus;
        ListPopupWindow* mHost;
    };

    class DropDownListView
        : public _DropDownListView
        , public IListView
        , public ITextWatcher
        , public IOnGlobalLayoutListener
        , public IFilterListener
        , public IOnTouchModeChangeListener
        , public IRemoteAdapterConnectionCallback
        , public IViewParent
        , public IViewManager
        , public IDrawableCallback
        , public IKeyEventCallback
        , public IAccessibilityEventSource
        , public IWeakReferenceSource
    {
    friend class ListPopupWindow;
    private:

        DropDownListView();

        CARAPI Init(
            /* [in] */ IContext* context,
            /* [in] */ Boolean hijackFocus,
            /* [in] */ ListPopupWindow* host);
    public:

        IVIEW_METHODS_DECL()
        IVIEWGROUP_METHODS_DECL()
        IVIEWPARENT_METHODS_DECL()
        IVIEWMANAGER_METHODS_DECL()
        IDRAWABLECALLBACK_METHODS_DECL()
        IKEYEVENTCALLBACK_METHODS_DECL()
        IACCESSIBILITYEVENTSOURCE_METHODS_DECL()
        IADAPTERVIEW_METHODS_DECL()
        ILISTVIEW_METHODS_DECL()
        IABSLISTVIEW_METHODS_DECL()
        IABSLISTVIEW_INTERFACE_METHODS_DECL()

        CARAPI_(PInterface) Probe(
            /* [in] */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        CARAPI GetWeakReference(
            /* [out] */ IWeakReference** weakReference);

        static AutoPtr<ListPopupWindow::DropDownListView> GetNewInstance(
            /* [in] */ IContext* context,
            /* [in] */ Boolean hijackFocus,
            /* [in] */ ListPopupWindow* host);
    };

    class PopupDataSetObserver
        : public ElRefBase
        , public IDataSetObserver
    {
    public:
        PopupDataSetObserver(
            /* [in] */ ListPopupWindow* host);

        CAR_INTERFACE_DECL()

        CARAPI OnChanged();

        CARAPI OnInvalidated();

    private:
        ListPopupWindow* mHost;
    };

    class ListSelectorHider
        : public Runnable
    {
    public:

        ListSelectorHider(
            /* [in] */ ListPopupWindow* host);

        CARAPI Run();

    private:
        ListPopupWindow* mHost;
    };

    class ResizePopupRunnable
        : public Runnable
    {
    public:
        ResizePopupRunnable(
            /* [in] */ ListPopupWindow* host);

        CARAPI Run();

    private:
        ListPopupWindow* mHost;
    };

    class PopupTouchInterceptor
        : public ElRefBase
        , public IViewOnTouchListener
    {
    public:
        PopupTouchInterceptor(
            /* [in] */ ListPopupWindow* host);

        CAR_INTERFACE_DECL()

        CARAPI OnTouch(
        /* [in] */ IView* v,
        /* [in] */ IMotionEvent* event,
        /* [out] */ Boolean* result);

    private:
        ListPopupWindow* mHost;
    };

    class PopupScrollListener
        : public ElRefBase
        , public IAbsListViewOnScrollListener
    {
    public:
        PopupScrollListener(
            /* [in] */ ListPopupWindow* host);

        CAR_INTERFACE_DECL()

        CARAPI OnScroll(
        /* [in] */ IAbsListView* view,
        /* [in] */ Int32 firstVisibleItem,
        /* [in] */ Int32 visibleItemCount,
        /* [in] */ Int32 totalItemCount);

        CARAPI OnScrollStateChanged(
        /* [in] */ IAbsListView* view,
        /* [in] */ Int32 scrollState);
    private:
        ListPopupWindow* mHost;
    };

    class ShowDropDownRunnable
        : public Runnable
    {
    public:
        ShowDropDownRunnable(
            /* [in] */ ListPopupWindow* host);

        CARAPI Run();

    private:
        ListPopupWindow* mHost;
    };

    class ListOnItemSelectedListener
        : public ElRefBase
        , public IAdapterViewOnItemSelectedListener
    {
    public:
        ListOnItemSelectedListener(
            /* [in] */ ListPopupWindow* host);

        CAR_INTERFACE_DECL()

        CARAPI OnItemSelected(
            /* [in] */ IAdapterView* parent,
            /* [in] */ IView* view,
            /* [in] */ Int32 position,
            /* [in] */ Int64 id);

        CARAPI OnNothingSelected(
            /* [in] */ IAdapterView* parent);

    private:
        ListPopupWindow* mHost;
    };


public:

    ListPopupWindow(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs = NULL,
        /* [in] */ Int32 defStyleAttr = R::attr::listPopupWindowStyle,
        /* [in] */ Int32 defStyleRes = 0);

    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs = NULL,
        /* [in] */ Int32 defStyleAttr = R::attr::listPopupWindowStyle,
        /* [in] */ Int32 defStyleRes = 0);

    virtual CARAPI SetAdapter(
        /* [in] */ IListAdapter* adapter);

    virtual CARAPI SetPromptPosition(
        /* [in] */ Int32 position);

    virtual CARAPI_(Int32) GetPromptPosition();

    virtual CARAPI SetModal(
        /* [in] */ Boolean modal);

    virtual CARAPI_(Boolean) IsModal();

    virtual CARAPI SetForceIgnoreOutsideTouch(
        /* [in] */ Boolean forceIgnoreOutsideTouch);

    virtual CARAPI SetDropDownAlwaysVisible(
        /* [in] */ Boolean dropDownAlwaysVisible);

    virtual CARAPI_(Boolean) IsDropDownAlwaysVisible();

    virtual CARAPI SetSoftInputMode(
        /* [in] */ Int32 mode);

    virtual CARAPI_(Int32) GetSoftInputMode();

    virtual CARAPI SetListSelector(
        /* [in] */ IDrawable* selector);

    virtual CARAPI_(AutoPtr<IDrawable>) GetBackground();

    virtual CARAPI SetBackgroundDrawable(
        /* [in] */ IDrawable* d);

    virtual CARAPI SetAnimationStyle(
        /* [in] */ Int32 animationStyle);

    virtual CARAPI_(Int32) GetAnimationStyle();

    virtual CARAPI_(AutoPtr<IView>) GetAnchorView();

    virtual CARAPI SetAnchorView(
        /* [in] */ IView* anchor);

    virtual CARAPI_(Int32) GetHorizontalOffset();

    virtual CARAPI SetHorizontalOffset(
        /* [in] */ Int32 offset);

    virtual CARAPI_(Int32) GetVerticalOffset();

    virtual CARAPI SetVerticalOffset(
        /* [in] */ Int32 offset);

    virtual CARAPI_(Int32) GetWidth();

    virtual CARAPI SetWidth(
        /* [in] */ Int32 width);

    virtual CARAPI SetContentWidth(
        /* [in] */ Int32 width);

    virtual CARAPI_(Int32) GetHeight();

    virtual CARAPI SetHeight(
        /* [in] */ Int32 height);

    virtual CARAPI SetOnItemClickListener(
        /* [in] */ IAdapterViewOnItemClickListener* clickListener);

    virtual CARAPI SetOnItemSelectedListener(
        /* [in] */ IAdapterViewOnItemSelectedListener* selectedListener);

    virtual CARAPI SetPromptView(
        /* [in] */ IView* prompt);

    virtual CARAPI PostShow();

    virtual CARAPI Show();

    virtual CARAPI Dismiss();

    virtual CARAPI SetOnDismissListener(
        /* [in] */ IPopupWindowOnDismissListener* listener);

    virtual CARAPI SetInputMethodMode(
        /* [in] */ Int32 mode);

    virtual CARAPI_(Int32) GetInputMethodMode();

    virtual CARAPI SetSelection(
        /* [in] */ Int32 position);

    virtual CARAPI ClearListSelection();

    virtual CARAPI_(Boolean) IsShowing();

    virtual CARAPI_(Boolean) IsInputMethodNotNeeded();

    virtual CARAPI_(Boolean) PerformItemClick(
        /* [in] */ Int32 position);

    virtual CARAPI_(AutoPtr<IInterface>) GetSelectedItem();

    virtual CARAPI_(Int32) GetSelectedItemPosition();

    virtual CARAPI_(Int64) GetSelectedItemId();

    virtual CARAPI_(AutoPtr<IView>) GetSelectedView();

    virtual CARAPI_(AutoPtr<IListView>) GetListView();

    virtual CARAPI_(Boolean) OnKeyDown(
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event);

    virtual CARAPI_(Boolean) OnKeyUp(
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event);

    virtual CARAPI_(Boolean) OnKeyPreIme(
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event);

    CARAPI SetListItemExpandMax(
        /* [in] */ Int32 max);

protected:
    ListPopupWindow();

private:
    CARAPI RemovePromptView();

    CARAPI_(Int32) BuildDropDown();

private:
    AutoPtr<IContext> mContext;
    AutoPtr<IPopupWindow> mPopup;
    AutoPtr<IListAdapter> mAdapter;
    AutoPtr<DropDownListView> mDropDownList;
    Int32 mDropDownHeight;
    Int32 mDropDownWidth;
    Int32 mDropDownHorizontalOffset;
    Int32 mDropDownVerticalOffset;
    Boolean mDropDownVerticalOffsetSet;
    Boolean mDropDownAlwaysVisible;
    Boolean mForceIgnoreOutsideTouch;
    Int32 mListItemExpandMaximun;
    AutoPtr<IView> mPromptView;
    Int32 mPromptPosition;
    AutoPtr<IDataSetObserver> mObserver;
    AutoPtr<IView> mDropDownAnchorview;
    AutoPtr<IDrawable> mDropDownListHighlight;
    AutoPtr<IAdapterViewOnItemClickListener> mItemClickListener;
    AutoPtr<IAdapterViewOnItemSelectedListener> mItemSelectedListener;
    AutoPtr<ResizePopupRunnable> mResizePopupRunnable;
    AutoPtr<PopupTouchInterceptor> mTouchInterceptor;
    AutoPtr<PopupScrollListener> mScrollListener;
    AutoPtr<ListSelectorHider> mHideSelector;
    AutoPtr<IRunnable> mShowDropDownRunnable;
    AutoPtr<IHandler> mHandler;
    AutoPtr<IRect> mTempRect;
    Boolean mModal;
    Int32 mLayoutDirection;

private:
    const static Int32 EXPAND_LIST_TIMEOUT = 250;
    friend class Spinner;
};

}// namespace Widget
}// namespace Droid
}// namespace Elastos

#endif //__LISTPOPUPWINDOW_H__
