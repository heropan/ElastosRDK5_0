#include "ext/frameworkext.h"
#include "widget/CListPopupWindow.h"

namespace Elastos{
namespace Droid{
namespace Widget{

PInterface CListPopupWindow::Probe(
    /* [in] */ REIID riid)
{
    return _CListPopupWindow::Probe(riid);
}

ECode CListPopupWindow::constructor(
    /* [in] */ IContext* context)
{
    return ListPopupWindow::Init(context);
}

ECode CListPopupWindow::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return ListPopupWindow::Init(context, attrs);
}

ECode CListPopupWindow::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    return ListPopupWindow::Init(context, attrs, defStyle);
}

ECode CListPopupWindow::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyleAttr,
    /* [in] */ Int32 defStyleRes)
{
    return ListPopupWindow::Init(context, attrs, defStyleAttr, defStyleRes);
}

ECode CListPopupWindow::SetAdapter(
    /* [in] */ IListAdapter* adapter)
{
    return ListPopupWindow::SetAdapter(adapter);
}

ECode CListPopupWindow::SetPromptPosition(
    /* [in] */ Int32 position)
{
    return ListPopupWindow::SetPromptPosition(position);
}

ECode CListPopupWindow::GetPromptPosition(
    /* [out] */ Int32* position)
{
    VALIDATE_NOT_NULL(position);
    *position = ListPopupWindow::GetPromptPosition();
    return NOERROR;
}

ECode CListPopupWindow::SetModal(
    /* [in] */ Boolean modal)
{
    return ListPopupWindow::SetModal(modal);
}

ECode CListPopupWindow::IsModal(
    /* [out] */ Boolean* modal)
{
    VALIDATE_NOT_NULL(modal);
    *modal = ListPopupWindow::IsModal();
    return NOERROR;
}

ECode CListPopupWindow::SetForceIgnoreOutsideTouch(
    /* [in] */ Boolean forceIgnoreOutsideTouch)
{
    return ListPopupWindow::SetForceIgnoreOutsideTouch(forceIgnoreOutsideTouch);
}

ECode CListPopupWindow::SetDropDownAlwaysVisible(
    /* [in] */ Boolean dropDownAlwaysVisible)
{
    return ListPopupWindow::SetDropDownAlwaysVisible(dropDownAlwaysVisible);
}

ECode CListPopupWindow::IsDropDownAlwaysVisible(
    /* [out] */ Boolean* visible)
{
    VALIDATE_NOT_NULL(visible);
    *visible = ListPopupWindow::IsDropDownAlwaysVisible();
    return NOERROR;
}

ECode CListPopupWindow::SetSoftInputMode(
    /* [in] */ Int32 mode)
{
    return ListPopupWindow::SetSoftInputMode(mode);
}

ECode CListPopupWindow::GetSoftInputMode(
    /* [out] */ Int32* mode)
{
    VALIDATE_NOT_NULL(mode);
    *mode = ListPopupWindow::GetSoftInputMode();
    return NOERROR;
}

ECode CListPopupWindow::SetListSelector(
    /* [in] */ IDrawable* selector)
{
    return ListPopupWindow::SetListSelector(selector);
}

ECode CListPopupWindow::GetBackground(
    /* [out] */ IDrawable** d)
{
    VALIDATE_NOT_NULL(d);
    AutoPtr<IDrawable> drawable = ListPopupWindow::GetBackground();
    *d = drawable;
    REFCOUNT_ADD(*d);
    return NOERROR;
}

ECode CListPopupWindow::SetBackgroundDrawable(
    /* [in] */ IDrawable* d)
{
    return ListPopupWindow::SetBackgroundDrawable(d);
}

ECode CListPopupWindow::SetAnimationStyle(
    /* [in] */ Int32 animationStyle)
{
    return ListPopupWindow::SetAnimationStyle(animationStyle);
}

ECode CListPopupWindow::GetAnimationStyle(
    /* [out] */ Int32* style)
{
    VALIDATE_NOT_NULL(style);
    *style = ListPopupWindow::GetAnimationStyle();
    return NOERROR;
}

ECode CListPopupWindow::GetAnchorView(
    /* [out] */ IView** view)
{
    VALIDATE_NOT_NULL(view);
    AutoPtr<IView> v = ListPopupWindow::GetAnchorView();
    *view = v;
    REFCOUNT_ADD(*view);
    return NOERROR;
}

ECode CListPopupWindow::SetAnchorView(
    /* [in] */ IView* anchor)
{
    return ListPopupWindow::SetAnchorView(anchor);
}

ECode CListPopupWindow::GetHorizontalOffset(
    /* [out] */ Int32* offset)
{
    VALIDATE_NOT_NULL(offset);
    *offset = ListPopupWindow::GetHorizontalOffset();
    return NOERROR;
}

ECode CListPopupWindow::SetHorizontalOffset(
    /* [in] */ Int32 offset)
{
    return ListPopupWindow::SetHorizontalOffset(offset);
}

ECode CListPopupWindow::GetVerticalOffset(
    /* [out] */ Int32* offset)
{
    VALIDATE_NOT_NULL(offset);
    *offset = ListPopupWindow::GetVerticalOffset();
    return NOERROR;
}

ECode CListPopupWindow::SetVerticalOffset(
    /* [in] */ Int32 offset)
{
    return ListPopupWindow::SetVerticalOffset(offset);
}

ECode CListPopupWindow::GetWidth(
    /* [out] */ Int32* width)
{
    VALIDATE_NOT_NULL(width);
    *width = ListPopupWindow::GetWidth();
    return NOERROR;
}

ECode CListPopupWindow::SetWidth(
    /* [in] */ Int32 width)
{
    return ListPopupWindow::SetWidth(width);
}

ECode CListPopupWindow::SetContentWidth(
    /* [in] */ Int32 width)
{
    return ListPopupWindow::SetContentWidth(width);
}

ECode CListPopupWindow::GetHeight(
    /* [out] */ Int32* height)
{
    VALIDATE_NOT_NULL(height);
    *height = ListPopupWindow::GetHeight();
    return NOERROR;
}

ECode CListPopupWindow::SetHeight(
    /* [in] */ Int32 height)
{
    return ListPopupWindow::SetHeight(height);
}

ECode CListPopupWindow::SetOnItemClickListener(
    /* [in] */ IAdapterViewOnItemClickListener* clickListener)
{
    return ListPopupWindow::SetOnItemClickListener(clickListener);
}

ECode CListPopupWindow::SetOnItemSelectedListener(
    /* [in] */ IAdapterViewOnItemSelectedListener* selectedListener)
{
    return ListPopupWindow::SetOnItemSelectedListener(selectedListener);
}

ECode CListPopupWindow::SetPromptView(
    /* [in] */ IView* prompt)
{
    return ListPopupWindow::SetPromptView(prompt);
}

ECode CListPopupWindow::PostShow()
{
    return ListPopupWindow::PostShow();
}

ECode CListPopupWindow::Show()
{
    return ListPopupWindow::Show();
}

ECode CListPopupWindow::Dismiss()
{
    return ListPopupWindow::Dismiss();
}

ECode CListPopupWindow::SetOnDismissListener(
    /* [in] */ IPopupWindowOnDismissListener* listener)
{
    return ListPopupWindow::SetOnDismissListener(listener);
}

ECode CListPopupWindow::SetInputMethodMode(
    /* [in] */ Int32 mode)
{
    return ListPopupWindow::SetInputMethodMode(mode);
}

ECode CListPopupWindow::GetInputMethodMode(
    /* [out] */ Int32* mode)
{
    VALIDATE_NOT_NULL(mode);
    *mode = ListPopupWindow::GetInputMethodMode();
    return NOERROR;
}

ECode CListPopupWindow::SetSelection(
    /* [in] */ Int32 position)
{
    return ListPopupWindow::SetSelection(position);
}

ECode CListPopupWindow::ClearListSelection()
{
    return ListPopupWindow::ClearListSelection();
}

ECode CListPopupWindow::IsShowing(
    /* [out] */ Boolean* showing)
{
    VALIDATE_NOT_NULL(showing);
    *showing = ListPopupWindow::IsShowing();
    return NOERROR;
}

ECode CListPopupWindow::IsInputMethodNotNeeded(
    /* [out] */ Boolean* needed)
{
    VALIDATE_NOT_NULL(needed);
    *needed = ListPopupWindow::IsInputMethodNotNeeded();
    return NOERROR;
}

ECode CListPopupWindow::PerformItemClick(
    /* [in] */ Int32 position,
    /* [out] */ Boolean* click)
{
    VALIDATE_NOT_NULL(click);
    *click = ListPopupWindow::PerformItemClick(position);
    return NOERROR;
}

ECode CListPopupWindow::GetSelectedItem(
    /* [out] */ IInterface** item)
{
    VALIDATE_NOT_NULL(item);
    AutoPtr<IInterface> temp = ListPopupWindow::GetSelectedItem();
    *item = temp;
    REFCOUNT_ADD(*item);
    return NOERROR;
}

ECode CListPopupWindow::GetSelectedItemPosition(
    /* [out] */ Int32* position)
{
    VALIDATE_NOT_NULL(position);
    *position = ListPopupWindow::GetSelectedItemPosition();
    return NOERROR;
}

ECode CListPopupWindow::GetSelectedItemId(
    /* [out] */ Int64* id)
{
    VALIDATE_NOT_NULL(id);
    *id = ListPopupWindow::GetSelectedItemId();
    return NOERROR;
}

ECode CListPopupWindow::GetSelectedView(
    /* [out] */ IView** view)
{
    VALIDATE_NOT_NULL(view);
    AutoPtr<IView> v = ListPopupWindow::GetSelectedView();
    *view = v;
    REFCOUNT_ADD(*view);
    return NOERROR;
}

ECode CListPopupWindow::GetListView(
    /* [out] */ IListView** view)
{
    VALIDATE_NOT_NULL(view);
    AutoPtr<IListView> v = ListPopupWindow::GetListView();
    *view = v;
    REFCOUNT_ADD(*view);
    return NOERROR;
}

ECode CListPopupWindow::SetListItemExpandMax(
    /* [in] */ Int32 max)
{
    return ListPopupWindow::SetListItemExpandMax(max);
}

ECode CListPopupWindow::OnKeyDown(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);
    *res = ListPopupWindow::OnKeyDown(keyCode, event);
    return NOERROR;
}

ECode CListPopupWindow::OnKeyUp(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);
    *res = ListPopupWindow::OnKeyUp(keyCode, event);
    return NOERROR;
}

ECode CListPopupWindow::OnKeyPreIme(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);
    *res = ListPopupWindow::OnKeyPreIme(keyCode, event);
    return NOERROR;
}

}// namespace Widget
}// namespace Droid
}// namespace Elastos
