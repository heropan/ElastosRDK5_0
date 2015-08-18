#ifndef __ELASTOS_DROID_WIDGET_CLISTPOPUPWINDOW_H__
#define __ELASTOS_DROID_WIDGET_CLISTPOPUPWINDOW_H__

#include "_Elastos_Droid_Widget_CListPopupWindow.h"
#include "widget/ListPopupWindow.h"

/*using Elastos::Droid::Content::IContext;
using Elastos::Droid::Graphics::Drawable::IDrawable;
using Elastos::Droid::View::IView;
using Elastos::Droid::View::IKeyEvent;
using Elastos::Droid::Utility::IAttributeSet;*/

namespace Elastos{
namespace Droid{
namespace Widget{

CarClass(CListPopupWindow), public ListPopupWindow
{
public:
    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI constructor(
        /* [in] */ IContext* context);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyleAttr,
        /* [in] */ Int32 defStyleRes);

public:
    CARAPI SetAdapter(
        /* [in] */ IListAdapter* adapter);

    CARAPI SetPromptPosition(
        /* [in] */ Int32 position);

    CARAPI GetPromptPosition(
        /* [out] */ Int32* position);

    CARAPI SetModal(
        /* [in] */ Boolean modal);

    CARAPI IsModal(
        /* [out] */ Boolean* modal);

    CARAPI SetForceIgnoreOutsideTouch(
        /* [in] */ Boolean forceIgnoreOutsideTouch);

    CARAPI SetDropDownAlwaysVisible(
        /* [in] */ Boolean dropDownAlwaysVisible);

    CARAPI IsDropDownAlwaysVisible(
        /* [out] */ Boolean* visible);

    CARAPI SetSoftInputMode(
        /* [in] */ Int32 mode);

    CARAPI GetSoftInputMode(
        /* [out] */ Int32* mode);

    CARAPI SetListSelector(
        /* [in] */ IDrawable* selector);

    CARAPI GetBackground(
        /* [out] */ IDrawable** d);

    CARAPI SetBackgroundDrawable(
        /* [in] */ IDrawable* d);

    CARAPI SetAnimationStyle(
        /* [in] */ Int32 animationStyle);

    CARAPI GetAnimationStyle(
        /* [out] */ Int32* style);

    CARAPI GetAnchorView(
        /* [out] */ IView** view);

    CARAPI SetAnchorView(
        /* [in] */ IView* anchor);

    CARAPI GetHorizontalOffset(
        /* [out] */ Int32* offset);

    CARAPI SetHorizontalOffset(
        /* [in] */ Int32 offset);

    CARAPI GetVerticalOffset(
        /* [out] */ Int32* offset);

    CARAPI SetVerticalOffset(
        /* [in] */ Int32 offset);

    CARAPI GetWidth(
        /* [out] */ Int32* width);

    CARAPI SetWidth(
        /* [in] */ Int32 width);

    CARAPI SetContentWidth(
        /* [in] */ Int32 width);

    CARAPI GetHeight(
        /* [out] */ Int32* height);

    CARAPI SetHeight(
        /* [in] */ Int32 height);

    CARAPI SetOnItemClickListener(
        /* [in] */ IAdapterViewOnItemClickListener* clickListener);

    CARAPI SetOnItemSelectedListener(
            /* [in] */ IAdapterViewOnItemSelectedListener* selectedListener);

    CARAPI SetPromptView(
        /* [in] */ IView* prompt);

    CARAPI PostShow();

    CARAPI Show();

    CARAPI Dismiss();

    CARAPI SetOnDismissListener(
        /* [in] */ IPopupWindowOnDismissListener* listener);

    CARAPI SetInputMethodMode(
        /* [in] */ Int32 mode);

    CARAPI GetInputMethodMode(
        /* [out] */ Int32* mode);

    CARAPI SetSelection(
        /* [in] */ Int32 position);

    CARAPI ClearListSelection();

    CARAPI IsShowing(
        /* [out] */ Boolean* showing);

    CARAPI IsInputMethodNotNeeded(
        /* [out] */ Boolean* needed);

    CARAPI PerformItemClick(
        /* [in] */ Int32 position,
        /* [out] */ Boolean* click);

    CARAPI GetSelectedItem(
        /* [out] */ IInterface** item);

    CARAPI GetSelectedItemPosition(
        /* [out] */ Int32* position);

    CARAPI GetSelectedItemId(
        /* [out] */ Int64* id);

    CARAPI GetSelectedView(
        /* [out] */ IView** view);

    CARAPI GetListView(
        /* [out] */ IListView** view);

    CARAPI SetListItemExpandMax(
        /* [in] */ Int32 max);

    CARAPI OnKeyDown(
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event,
        /* [out] */ Boolean* res);

    CARAPI OnKeyUp(
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event,
        /* [out] */ Boolean* res);

    CARAPI OnKeyPreIme(
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event,
        /* [out] */ Boolean* res);

};

}// namespace Widget
}// namespace Droid
}// namespace Elastos

#endif //__ELASTOS_DROID_WIDGET_CLISTPOPUPWINDOW_H__
