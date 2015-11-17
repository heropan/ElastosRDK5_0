#ifndef __ELASTOS_DROID_INTERNAL_VIEW_MENU_ACTIONMENUITEMVIEW_H__
#define __ELASTOS_DROID_INTERNAL_VIEW_MENU_ACTIONMENUITEMVIEW_H__

#if 0
#include "elastos/droid/widget/TextView.h"
#else
#include "elastos/droid/view/View.h"
using Elastos::Droid::View::IMotionEvent;
using Elastos::Droid::View::IView;
#endif

using Elastos::Droid::View::IViewOnClickListener;
using Elastos::Droid::View::IViewOnLongClickListener;
using Elastos::Droid::Widget::IActionMenuChildView;
using Elastos::Droid::Widget::IListPopupWindow;
// using Elastos::Droid::Widget::TextView;

namespace Elastos {
namespace Droid {
namespace Internal {
namespace View {
namespace Menu {

class ActionMenuItemView
#if 0
    : public TextView
#else
    : public Elastos::Droid::View::View
#endif
    , public IActionMenuItemView
    , public IMenuItemView
    , public IViewOnClickListener
    , public IViewOnLongClickListener
    , public IActionMenuChildView
{
private:
    class MyListener
        : public Object
        , public IViewOnClickListener
        , public IViewOnLongClickListener
    {
    public:
        MyListener(
            /* [in] */ ActionMenuItemView* owner);

        CAR_INTERFACE_DECL()

        CARAPI OnClick(
            /* [in] */ IView* v);

        CARAPI OnLongClick(
            /* [in] */ IView* v,
            /* [out] */ Boolean* result);

    private:
        ActionMenuItemView* mOwner;
    };

    class ActionMenuItemForwardingListener
    #if 0
        : public ForwardingListener
    #endif
    {
    public:
        ActionMenuItemForwardingListener(
            /* [in] */ ActionMenuItemView* owner);

        // @Override
        CARAPI GetPopup(
            /* [out] */ IListPopupWindow** window);

    protected:
        // @Override
        CARAPI_(Boolean) OnForwardingStarted();

        // @Override
        CARAPI_(Boolean) OnForwardingStopped();

    private:
        ActionMenuItemView* mOwner;
    };

public:
    ActionMenuItemView();

    CAR_INTERFACE_DECL()

    CARAPI constructor(
        /* [in] */ IContext* context);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyleAttr);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyleAttr,
        /* [in] */ Int32 defStyleRes);

    //@Override
    CARAPI_(void) OnConfigurationChanged(
        /* [in] */ IConfiguration* newConfig);

    //@Override
    CARAPI SetPadding(
        /* [in] */ Int32 l,
        /* [in] */ Int32 t,
        /* [in] */ Int32 r,
        /* [in] */ Int32 b);

    CARAPI GetItemData(
        /* [out] */ IMenuItemImpl** itemData);

    CARAPI Initialize(
        /* [in] */ IMenuItemImpl* itemData,
        /* [in] */ Int32 menuType);

    // @Override
    CARAPI OnTouchEvent(
        /* [in] */ IMotionEvent* e,
        /* [out] */ Boolean* result);

    CARAPI OnClick(
        /* [in] */ IView* v);

    CARAPI SetItemInvoker(
        /* [in] */ IMenuBuilderItemInvoker* invoker);

    CARAPI SetPopupCallback(
        /* [in] */ IPopupCallback* popupCallback);

    CARAPI PrefersCondensedTitle(
        /* [out] */ Boolean* supported);

    CARAPI SetCheckable(
        /* [in] */ Boolean checkable);

    CARAPI SetChecked(
        /* [in] */ Boolean checked);

    CARAPI SetExpandedFormat(
        /* [in] */ Boolean expandedFormat);

    CARAPI SetIcon(
        /* [in] */ IDrawable* icon);

    CARAPI HasText(
        /* [out] */ Boolean* result);

    CARAPI SetShortcut(
        /* [in] */ Boolean showShortcut,
        /* [in] */ Char32 shortcutKey);

    CARAPI SetTitle(
        /* [in] */ ICharSequence* title);

    //@Override
    CARAPI DispatchPopulateAccessibilityEvent(
        /* [in] */ IAccessibilityEvent* event,
        /* [out] */ Boolean* result);

    //@Override
    CARAPI OnPopulateAccessibilityEvent(
        /* [in] */ IAccessibilityEvent* event);

    //@Override
    CARAPI DispatchHoverEvent(
        /* [in] */ IMotionEvent* event,
        /* [out] */ Boolean* result);

    CARAPI ShowsIcon(
        /* [out] */ Boolean* shows);

    CARAPI NeedsDividerBefore(
        /* [out] */ Boolean* need);

    CARAPI NeedsDividerAfter(
        /* [out] */ Boolean* need);

    //@Override
    CARAPI OnLongClick(
        /* [in] */ IView* v,
        /* [out] */ Boolean* result);

    //@Override
    CARAPI SetEnabled(
        /* [in] */ Boolean enabled);

protected:
    CARAPI Init(
        /* [in] */ IContext* ctx,
        /* [in] */ IAttributeSet* attrs = NULL,
        /* [in] */ Int32 defStyle = 0);

    //@Override
    CARAPI_(void) OnMeasure(
        /* [in] */ Int32 widthMeasureSpec,
        /* [in] */ Int32 heightMeasureSpec);

private:
    CARAPI_(void) UpdateTextButtonVisibility();

private:
    static String TAG;

    AutoPtr<IMenuItemImpl> mItemData;
    AutoPtr<ICharSequence> mTitle;
    AutoPtr<IDrawable> mIcon;
    AutoPtr<IMenuBuilderItemInvoker> mItemInvoker;
    // TODO:
    // AutoPtr<IForwardingListener> mForwardingListener;
    AutoPtr<IPopupCallback> mPopupCallback;

    Boolean mAllowTextWithIcon;
    Boolean mExpandedFormat;
    Int32 mMinWidth;
    Int32 mSavedPaddingLeft;

    static const Int32 MAX_ICON_SIZE; // dp
    Int32 mMaxIconSize;
};

} // namespace Menu
} // namespace View
} // namespace Internal
} // namespace Droid
} // namespace Elastos

#endif  //__ELASTOS_DROID_INTERNAL_VIEW_MENU_ACTIONMENUITEMVIEW_H__
