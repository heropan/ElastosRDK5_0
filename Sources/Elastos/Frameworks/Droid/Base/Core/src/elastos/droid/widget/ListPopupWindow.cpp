#include <elastos/core/Math.h>
//#include <elastos/utility/logging/Logger.h>
#include "elastos/droid/os/CHandler.h"
#include "elastos/droid/text/TextUtils.h"
#include "elastos/droid/utility/CDisplayMetrics.h"
#include "elastos/droid/widget/ListPopupWindow.h"
#include "elastos/droid/widget/CPopupWindow.h"
#include "elastos/droid/widget/CLinearLayout.h"
#include "elastos/droid/widget/ListView.h"

//using Elastos::Utility::Logging::Logger;
using Elastos::Utility::ILocale;
using Elastos::Droid::Os::IHandler;
using Elastos::Droid::Os::CHandler;
using Elastos::Droid::Text::TextUtils;
using Elastos::Droid::Utility::CDisplayMetrics;

namespace Elastos{
namespace Droid{
namespace Widget{

const Int32 ListPopupWindow::EXPAND_LIST_TIMEOUT;

CAR_INTERFACE_IMPL(ListPopupWindow::PopupDataSetObserver, IDataSetObserver)
CAR_INTERFACE_IMPL(ListPopupWindow::PopupTouchInterceptor, IViewOnTouchListener)
CAR_INTERFACE_IMPL(ListPopupWindow::PopupScrollListener, IAbsListViewOnScrollListener)
CAR_INTERFACE_IMPL(ListPopupWindow::ListOnItemSelectedListener, IAdapterViewOnItemSelectedListener)
ILISTVIEW_METHODS_IMPL(ListPopupWindow::DropDownListView, ListPopupWindow::_DropDownListView)
IVIEW_METHODS_IMPL(ListPopupWindow::DropDownListView, ListPopupWindow::_DropDownListView)
IVIEWGROUP_METHODS_IMPL(ListPopupWindow::DropDownListView, ListPopupWindow::_DropDownListView)
IVIEWPARENT_METHODS_IMPL(ListPopupWindow::DropDownListView, ListPopupWindow::_DropDownListView)
IVIEWMANAGER_METHODS_IMPL(ListPopupWindow::DropDownListView, ListPopupWindow::_DropDownListView)
IDRAWABLECALLBACK_METHODS_IMPL(ListPopupWindow::DropDownListView, ListPopupWindow::_DropDownListView)
IKEYEVENTCALLBACK_METHODS_IMPL(ListPopupWindow::DropDownListView, ListPopupWindow::_DropDownListView)
IACCESSIBILITYEVENTSOURCE_METHODS_IMPL(ListPopupWindow::DropDownListView, ListPopupWindow::_DropDownListView)
IADAPTERVIEW_METHODS_IMPL(ListPopupWindow::DropDownListView, ListPopupWindow::_DropDownListView)
IABSLISTVIEW_METHODS_IMPL(ListPopupWindow::DropDownListView, ListPopupWindow::_DropDownListView)
IABSLISTVIEW_INTERFACE_METHODS_IMPL(ListPopupWindow::DropDownListView, ListPopupWindow::_DropDownListView)

ListPopupWindow::ListPopupWindow()
    : mDropDownHeight(IViewGroupLayoutParams::WRAP_CONTENT)
    , mDropDownWidth(IViewGroupLayoutParams::WRAP_CONTENT)
    , mDropDownHorizontalOffset(0)
    , mDropDownVerticalOffset(0)
    , mDropDownVerticalOffsetSet(FALSE)
    , mDropDownAlwaysVisible(FALSE)
    , mForceIgnoreOutsideTouch(FALSE)
    , mListItemExpandMaximun(Elastos::Core::Math::INT32_MAX_VALUE)
    , mPromptPosition(IListPopupWindow::POSITION_PROMPT_ABOVE)
    , mModal(FALSE)
    , mLayoutDirection(0)
{
    mResizePopupRunnable = new ResizePopupRunnable(this);
    mTouchInterceptor = new PopupTouchInterceptor(this);
    mScrollListener = new PopupScrollListener(this);
    mHideSelector = new ListSelectorHider(this);
    CHandler::New((IHandler**)&mHandler);
    CRect::New((IRect**)&mTempRect);
}

ListPopupWindow::ListPopupWindow(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle,
    /* [in] */ Int32 defStyleRes)
    : mDropDownHeight(IViewGroupLayoutParams::WRAP_CONTENT)
    , mDropDownWidth(IViewGroupLayoutParams::WRAP_CONTENT)
    , mDropDownHorizontalOffset(0)
    , mDropDownVerticalOffset(0)
    , mDropDownVerticalOffsetSet(FALSE)
    , mDropDownAlwaysVisible(FALSE)
    , mForceIgnoreOutsideTouch(FALSE)
    , mListItemExpandMaximun(Elastos::Core::Math::INT32_MAX_VALUE)
    , mPromptPosition(IListPopupWindow::POSITION_PROMPT_ABOVE)
    , mModal(FALSE)
    , mLayoutDirection(0)
{
    mResizePopupRunnable = new ResizePopupRunnable(this);
    mTouchInterceptor = new PopupTouchInterceptor(this);
    mScrollListener = new PopupScrollListener(this);
    mHideSelector = new ListSelectorHider(this);
    CHandler::New((IHandler**)&mHandler);
    CRect::New((IRect**)&mTempRect);
    ListPopupWindow::Init(context, attrs, defStyle, defStyleRes);
}

ECode ListPopupWindow::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle,
    /* [in] */ Int32 defStyleRes)
{
    mContext = context;
    CPopupWindow::New(context, attrs, defStyle, defStyleRes, (IPopupWindow**)&mPopup);
    mPopup->SetInputMethodMode(IPopupWindow::INPUT_METHOD_NEEDED);
    AutoPtr<IResources> resources;
    mContext->GetResources((IResources**)&resources);
    AutoPtr<IConfiguration> config;
    resources->GetConfiguration((IConfiguration**)&config);
    AutoPtr<ILocale> locale;
    config->GetLocale((ILocale**)&locale);
    mLayoutDirection = TextUtils::GetLayoutDirectionFromLocale(locale);
    return NOERROR;
}

ECode ListPopupWindow::SetAdapter(
    /* [in] */ IListAdapter* adapter)
{
    if (NULL == mObserver) {
        mObserver = new PopupDataSetObserver(this);
    }
    else if (NULL != mAdapter) {
        mAdapter->UnregisterDataSetObserver(mObserver);
    }

    mAdapter = adapter;
    if (mAdapter != NULL) {
        mAdapter->RegisterDataSetObserver(mObserver);
    }

    if (mDropDownList != NULL) {
        AutoPtr<IAdapter> temp = IAdapter::Probe(mAdapter.Get());
        mDropDownList->SetAdapter(temp);
    }
    return NOERROR;
}

ECode ListPopupWindow::SetPromptPosition(
    /* [in] */ Int32 position)
{
    mPromptPosition = position;
    return NOERROR;
}

Int32 ListPopupWindow::GetPromptPosition()
{
    return mPromptPosition;
}

ECode ListPopupWindow::SetModal(
    /* [in] */ Boolean modal)
{
    mModal = TRUE;
    return mPopup->SetFocusable(modal);
}

Boolean ListPopupWindow::IsModal()
{
    return mModal;
}

ECode ListPopupWindow::SetForceIgnoreOutsideTouch(
    /* [in] */ Boolean forceIgnoreOutsideTouch)
{
    mForceIgnoreOutsideTouch = forceIgnoreOutsideTouch;
    return NOERROR;
}

ECode ListPopupWindow::SetDropDownAlwaysVisible(
    /* [in] */ Boolean dropDownAlwaysVisible)
{
    mDropDownAlwaysVisible = dropDownAlwaysVisible;
    return NOERROR;
}

Boolean ListPopupWindow::IsDropDownAlwaysVisible()
{
    return mDropDownAlwaysVisible;
}

ECode ListPopupWindow::SetSoftInputMode(
    /* [in] */ Int32 mode)
{
    return mPopup->SetSoftInputMode(mode);
}

Int32 ListPopupWindow::GetSoftInputMode()
{
    Int32 mode = 0;
    mPopup->GetSoftInputMode(&mode);
    return mode;
}

ECode ListPopupWindow::SetListSelector(
    /* [in] */ IDrawable* selector)
{
    mDropDownListHighlight = selector;
    return NOERROR;
}

AutoPtr<IDrawable> ListPopupWindow::GetBackground()
{
    AutoPtr<IDrawable> drawable;
    mPopup->GetBackground((IDrawable**)&drawable);
    return drawable;
}

ECode ListPopupWindow::SetBackgroundDrawable(
    /* [in] */ IDrawable* d)
{
    return mPopup->SetBackgroundDrawable(d);
}

ECode ListPopupWindow::SetAnimationStyle(
    /* [in] */ Int32 animationStyle)
{
    return mPopup->SetAnimationStyle(animationStyle);
}

Int32 ListPopupWindow::GetAnimationStyle()
{
    Int32 style = 0;
    mPopup->GetAnimationStyle(&style);
    return style;
}

AutoPtr<IView> ListPopupWindow::GetAnchorView()
{
    return mDropDownAnchorview;
}

ECode ListPopupWindow::SetAnchorView(
    /* [in] */ IView* anchor)
{
    mDropDownAnchorview = anchor;
    return NOERROR;
}

Int32 ListPopupWindow::GetHorizontalOffset()
{
    return mDropDownHorizontalOffset;
}

ECode ListPopupWindow::SetHorizontalOffset(
    /* [in] */ Int32 offset)
{
    mDropDownHorizontalOffset = offset;
    return NOERROR;
}

Int32 ListPopupWindow::GetVerticalOffset()
{
    if(!mDropDownVerticalOffsetSet) {
        return 0;
    }
    return mDropDownVerticalOffset;
}

ECode ListPopupWindow::SetVerticalOffset(
    /* [in] */ Int32 offset)
{
    mDropDownVerticalOffset = offset;
    mDropDownVerticalOffsetSet = TRUE;
    return NOERROR;
}

Int32 ListPopupWindow::GetWidth()
{
    return mDropDownWidth;
}

ECode ListPopupWindow::SetWidth(
    /* [in] */ Int32 width)
{
    mDropDownWidth = width;
    return NOERROR;
}

ECode ListPopupWindow::SetContentWidth(
    /* [in] */ Int32 width)
{
    AutoPtr<IDrawable> popupBackground;
    mPopup->GetBackground((IDrawable**)&popupBackground);
    if (popupBackground) {
        Boolean res = FALSE;
        popupBackground->GetPadding(mTempRect, &res);
        CRect* rect = (CRect*)mTempRect.Get();
        mDropDownWidth = rect->mLeft + rect->mRight +width;
    } else {
        SetWidth(width);
    }

    return NOERROR;
}

Int32 ListPopupWindow::GetHeight()
{
    return mDropDownHeight;
}

ECode ListPopupWindow::SetHeight(
    /* [in] */ Int32 height)
{
    mDropDownHeight = height;
    return NOERROR;
}

ECode ListPopupWindow::SetOnItemClickListener(
    /* [in] */ IAdapterViewOnItemClickListener* clickListener)
{
    mItemClickListener = clickListener;
    return NOERROR;
}

ECode ListPopupWindow::SetOnItemSelectedListener(
    /* [in] */ IAdapterViewOnItemSelectedListener* selectedListener)
{
    mItemSelectedListener = selectedListener;
    return NOERROR;
}

ECode ListPopupWindow::SetPromptView(
    /* [in] */ IView* prompt)
{
    Boolean showing = IsShowing();
    if (showing) {
        RemovePromptView();
    }
    mPromptView = prompt;
    if (showing) {
        Show();
    }

    return NOERROR;
}

ECode ListPopupWindow::PostShow()
{
    Boolean res = FALSE;
    return mHandler->Post(mShowDropDownRunnable, &res);
}

ECode ListPopupWindow::Show()
{
    Int32 height = BuildDropDown();
    Int32 widthSpec = 0;
    Int32 heightSpec = 0;

    Boolean noInputMethod = IsInputMethodNotNeeded();
    mPopup->SetAllowScrollingAnchorParent(!noInputMethod);
    Boolean isShowing = FALSE;
    mPopup->IsShowing(&isShowing);
    if (isShowing) {
        if (mDropDownWidth == IViewGroupLayoutParams::MATCH_PARENT) {
            // The call to PopupWindow's update method below can accept -1 for any
            // value you do not want to update.
            widthSpec = -1;
        } else if (mDropDownWidth == IViewGroupLayoutParams::WRAP_CONTENT) {
            GetAnchorView()->GetWidth(&widthSpec);
        } else {
            widthSpec = mDropDownWidth;
        }

        if (mDropDownHeight == IViewGroupLayoutParams::MATCH_PARENT) {
            heightSpec = noInputMethod ? height : IViewGroupLayoutParams::MATCH_PARENT;
            if (noInputMethod) {
                mPopup->SetWindowLayoutMode(
                    mDropDownWidth == IViewGroupLayoutParams::MATCH_PARENT ?
                        IViewGroupLayoutParams::MATCH_PARENT : 0, 0);
            } else {
                mPopup->SetWindowLayoutMode(
                    mDropDownWidth == IViewGroupLayoutParams::MATCH_PARENT ?
                        IViewGroupLayoutParams::MATCH_PARENT : 0,
                            IViewGroupLayoutParams::MATCH_PARENT);
            }
        } else if (mDropDownHeight == IViewGroupLayoutParams::WRAP_CONTENT) {
            heightSpec = height;
        } else {
            heightSpec = mDropDownHeight;
        }

        mPopup->SetOutsideTouchable(!mForceIgnoreOutsideTouch && ! mDropDownAlwaysVisible);

        mPopup->Update(GetAnchorView(), mDropDownHorizontalOffset,
            mDropDownVerticalOffset, widthSpec, heightSpec);
    } else {
        if (mDropDownWidth == IViewGroupLayoutParams::MATCH_PARENT) {
            widthSpec = IViewGroupLayoutParams::MATCH_PARENT;
        } else {
            if (mDropDownWidth == IViewGroupLayoutParams::WRAP_CONTENT) {
                Int32 getWidth = 0;
                GetAnchorView()->GetWidth(&getWidth);
                mPopup->SetWidth(getWidth);
            } else {
                mPopup->SetWidth(mDropDownWidth);
            }
        }

        if (mDropDownHeight == IViewGroupLayoutParams::MATCH_PARENT) {
            heightSpec = IViewGroupLayoutParams::MATCH_PARENT;
        } else {
            if (mDropDownHeight == IViewGroupLayoutParams::WRAP_CONTENT) {
                mPopup->SetHeight(height);
            } else {
                mPopup->SetHeight(mDropDownHeight);
            }
        }

        mPopup->SetWindowLayoutMode(widthSpec, heightSpec);
        mPopup->SetClipToScreenEnabled(TRUE);

        // use outside touchable to dismiss drop down when touching outside of it, so
        // only set this if the dropdown is not always visible
        mPopup->SetOutsideTouchable(!mForceIgnoreOutsideTouch && !mDropDownAlwaysVisible);
        mPopup->SetTouchInterceptor(mTouchInterceptor);
        mPopup->ShowAsDropDown(GetAnchorView(),
            mDropDownHorizontalOffset, mDropDownVerticalOffset);
        mDropDownList->SetSelection(IListView::INVALID_POSITION);

        Boolean touchMode;
        mDropDownList->IsInTouchMode(&touchMode);
        if(!mModal || touchMode) {
            ClearListSelection();
        }

        if(!mModal) {
            Boolean result = FALSE;
            mHandler->Post(mHideSelector, &result);
        }
    }

    return NOERROR;
}

ECode ListPopupWindow::Dismiss()
{
    mPopup->Dismiss();
    RemovePromptView();
    AutoPtr<IView> v;
    mPopup->SetContentView(v);
    mDropDownList = NULL;
    mHandler->RemoveCallbacks(mResizePopupRunnable);
    return NOERROR;
}

ECode ListPopupWindow::SetOnDismissListener(
    /* [in] */ IPopupWindowOnDismissListener* listener)
{
    mPopup->SetOnDismissListener(listener);
    return NOERROR;
}

ECode ListPopupWindow::SetInputMethodMode(
    /* [in] */ Int32 mode)
{
    return mPopup->SetInputMethodMode(mode);
}

Int32 ListPopupWindow::GetInputMethodMode()
{
    Int32 mode = 0;
    mPopup->GetInputMethodMode(&mode);
    return mode;
}

ECode ListPopupWindow::SetSelection(
    /* [in] */ Int32 position)
{
    AutoPtr<DropDownListView> list = mDropDownList;
    if (IsShowing() && list) {
        list->mListSelectionHidden = FALSE;
        list->SetSelection(position);
        Int32 mode = 0;
        list->GetChoiceMode(&mode);
        if (mode != IAbsListView::CHOICE_MODE_NONE) {
            list->SetItemChecked(position, TRUE);
        }
    }
    return NOERROR;
}

ECode ListPopupWindow::ClearListSelection()
{
    AutoPtr<DropDownListView> list = mDropDownList;
    if (list != NULL) {
        list->mListSelectionHidden = TRUE;
        list->HideSelector();
        list->RequestLayout();
    }

    return NOERROR;
}

Boolean ListPopupWindow::IsShowing()
{
    Boolean res = FALSE;
    mPopup->IsShowing(&res);
    return res;
}

Boolean ListPopupWindow::IsInputMethodNotNeeded()
{
    Int32 mode = 0;
    mPopup->GetInputMethodMode(&mode);
    return mode == IPopupWindow::INPUT_METHOD_NOT_NEEDED;
}

Boolean ListPopupWindow::PerformItemClick(
    /* [in] */ Int32 position)
{
    if (IsShowing()) {
        if (mItemClickListener != NULL) {
            AutoPtr<DropDownListView> list = mDropDownList;
            Int32 visiblePosition = 0;
            list->GetFirstVisiblePosition(&visiblePosition);
            AutoPtr<IView> child;
            list->GetChildAt(position - visiblePosition, (IView**)&child);
            AutoPtr<IAdapter> adapter;
            list->GetAdapter((IAdapter**)&adapter);
            Int64 id = 0;
            adapter->GetItemId(position, &id);
            mItemClickListener->OnItemClick(list, child, position, id);
        }
        return TRUE;
    }
    return FALSE;
}

AutoPtr<IInterface> ListPopupWindow::GetSelectedItem()
{
    if(!IsShowing()) {
        return NULL;
    }
    AutoPtr<IInterface> rst;
    mDropDownList->GetSelectedItem((IInterface**)&rst);
    return rst;
}

Int32 ListPopupWindow::GetSelectedItemPosition()
{
    if(!IsShowing()) {
        return IListView::INVALID_POSITION;
    }
    Int32 rst;
    mDropDownList->GetSelectedItemPosition(&rst);
    return rst;
}

Int64 ListPopupWindow::GetSelectedItemId()
{
    if(!IsShowing()) {
        return IListView::INVALID_POSITION;
    }
    Int64 rst;
    mDropDownList->GetSelectedItemId(&rst);
    return rst;
}

AutoPtr<IView> ListPopupWindow::GetSelectedView()
{
    if(!IsShowing()) {
        return NULL;
    }
    AutoPtr<IView> rst;
    mDropDownList->GetSelectedView((IView**)&rst);
    return rst;
}

AutoPtr<IListView> ListPopupWindow::GetListView()
{
    return mDropDownList;
}

Boolean ListPopupWindow::OnKeyDown(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event)
{
    if (IsShowing()) {
        Int32 itemPosition = 0;
        mDropDownList->GetSelectedItemPosition(&itemPosition);
        if (keyCode != IKeyEvent::KEYCODE_SPACE &&
            (itemPosition >= 0
                || (keyCode != IKeyEvent::KEYCODE_ENTER
                    && keyCode != IKeyEvent::KEYCODE_DPAD_CENTER))) {
            Int32 curIndex = 0;
            mDropDownList->GetSelectedItemPosition(&curIndex);
            Boolean consumed = FALSE;

            Boolean aboveAnchor = FALSE;
            mPopup->IsAboveAnchor(&aboveAnchor);
            Boolean below = !aboveAnchor;

            AutoPtr<IListAdapter> adapter = mAdapter;
            Boolean allEnabled = FALSE;

            Int32 firstItem = Elastos::Core::Math::INT32_MAX_VALUE;
            Int32 lastItem = Elastos::Core::Math::INT32_MIN_VALUE;

            if (adapter != NULL) {
                adapter->AreAllItemsEnabled(&allEnabled);
                firstItem = allEnabled ? 0 :
                    mDropDownList->LookForSelectablePosition(0, TRUE);
                Int32 count = 0;
                adapter->GetCount(&count);
                lastItem = allEnabled ? count - 1 :
                    mDropDownList->LookForSelectablePosition(count - 1, FALSE);
            }

            if((below && IKeyEvent::KEYCODE_DPAD_UP && curIndex <= firstItem) ||
                (!below && IKeyEvent::KEYCODE_DPAD_DOWN && curIndex >= lastItem)) {
                ClearListSelection();
                mPopup->SetInputMethodMode(IPopupWindow::INPUT_METHOD_NEEDED);
                Show();
                return TRUE;
            } else {
                mDropDownList->mListSelectionHidden = FALSE;
            }

            mDropDownList->OnKeyDown(keyCode, event, &consumed);
            if (consumed) {
                mPopup->SetInputMethodMode(IPopupWindow::INPUT_METHOD_NEEDED);
                Boolean res = FALSE;
                mDropDownList->RequestFocusFromTouch(&res);
                Show();

                switch(keyCode) {
                    case IKeyEvent::KEYCODE_ENTER:
                    case IKeyEvent::KEYCODE_DPAD_CENTER:
                    case IKeyEvent::KEYCODE_DPAD_DOWN:
                    case IKeyEvent::KEYCODE_DPAD_UP:
                        return TRUE;
                }
            } else {
                if (below && keyCode == IKeyEvent::KEYCODE_DPAD_DOWN) {
                    if (curIndex == lastItem) {
                        return TRUE;
                    }
                } else if(!below && keyCode == IKeyEvent::KEYCODE_DPAD_UP
                        && curIndex == firstItem) {
                    return TRUE;
                }
            }
        }
    }
    return FALSE;
}

Boolean ListPopupWindow::OnKeyUp(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event)
{
    if (IsShowing()) {
        Int32 itemPosition;
        mDropDownList->GetSelectedItemPosition(&itemPosition);
        if (itemPosition >= 0) {
        Boolean consumed = FALSE;
        mDropDownList->OnKeyUp(keyCode, event, &consumed);
        if (consumed) {
            switch (keyCode) {
                case IKeyEvent::KEYCODE_ENTER:
                case IKeyEvent::KEYCODE_DPAD_CENTER:
                    Dismiss();
                    break;
            }
        }
        return consumed;
    }
    }
    return FALSE;
}

Boolean ListPopupWindow::OnKeyPreIme(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event)
{
    if (keyCode == IKeyEvent::KEYCODE_BACK && IsShowing()) {
        AutoPtr<IView> anchorView = mDropDownAnchorview;
        Int32 action = 0;
        event->GetAction(&action);
        Int32 repeatCount = 0;
        event->GetRepeatCount(&repeatCount);
        if (action == IKeyEvent::ACTION_DOWN && repeatCount == 0) {
            AutoPtr<IDispatcherState> state;
            anchorView->GetKeyDispatcherState((IDispatcherState**)&state);
            if (state != NULL) {
                //state->StartTracking(event, this->Probe(EIID_IInterface));
            }
            return TRUE;
        } else if (action == IKeyEvent::ACTION_UP) {
            AutoPtr<IDispatcherState> state;
            anchorView->GetKeyDispatcherState((IDispatcherState**)&state);
            if (state != NULL) {
                state->HandleUpEvent(event);
            }
            Boolean isTracking = FALSE;
            event->IsTracking(&isTracking);
            Boolean isCanceled = FALSE;
            event->IsCanceled(&isCanceled);
            if (isTracking && !isCanceled) {
                Dismiss();
                return TRUE;
            }
        }
    }
    return FALSE;
}

ECode ListPopupWindow::RemovePromptView()
{
    if (mPromptView != NULL) {
        AutoPtr<IViewParent> parent;
        mPromptView->GetParent((IViewParent**)&parent);
        AutoPtr<IViewGroup> group = IViewGroup::Probe(parent);
        if (group) {
            group->RemoveViewInLayout(mPromptView);
        }
    }
    return NOERROR;
}

ECode ListPopupWindow::SetListItemExpandMax(
    /* [in] */ Int32 max)
{
    mListItemExpandMaximun = max;
    return NOERROR;
}

Int32 ListPopupWindow::BuildDropDown()
{
    AutoPtr<IViewGroup> dropDownView;
    Int32 otherHeights = 0;
    if (mDropDownList == NULL) {
        AutoPtr<IContext> context = mContext;

        mShowDropDownRunnable = new ShowDropDownRunnable(this);

        mDropDownList = DropDownListView::GetNewInstance(context, !mModal, this);
        if (mDropDownListHighlight != NULL) {
            mDropDownList->SetSelector(mDropDownListHighlight);
        }

        AutoPtr<IAdapter> temp = IAdapter::Probe(mAdapter);
        mDropDownList->SetAdapter(temp);
        mDropDownList->SetOnItemClickListener(mItemClickListener);
        mDropDownList->SetFocusable(TRUE);
        mDropDownList->SetFocusableInTouchMode(TRUE);

        AutoPtr<ListOnItemSelectedListener> listener = new ListOnItemSelectedListener(this);
        mDropDownList->SetOnItemSelectedListener(listener);
        mDropDownList->SetOnScrollListener(mScrollListener);

        if (mItemSelectedListener != NULL) {
            mDropDownList->SetOnItemSelectedListener(mItemSelectedListener);
        }
        dropDownView = mDropDownList;

        AutoPtr<IView> hintView = mPromptView;

        if (hintView) {
            AutoPtr<ILinearLayout> hintcontainer;
            FAIL_RETURN(CLinearLayout::New(context, (ILinearLayout**)&hintcontainer));
            hintcontainer->SetOrientation(ILinearLayout::VERTICAL);
            AutoPtr<ILinearLayoutLayoutParams> hintParams;
            FAIL_RETURN(CLinearLayoutLayoutParams::New(IViewGroupLayoutParams::MATCH_PARENT,
                0, (Int32)1.0f, (ILinearLayoutLayoutParams**)&hintParams));

            switch (mPromptPosition) {
            case IListPopupWindow::POSITION_PROMPT_BELOW:
                hintcontainer->AddView(dropDownView, hintParams);
                hintcontainer->AddView(hintView);
                break;
            case IListPopupWindow::POSITION_PROMPT_ABOVE:
                hintcontainer->AddView(hintView);
                hintcontainer->AddView(dropDownView, hintParams);
                break;
            default:
                //log.e();
                break;
            }

            Int32 widthSpec = View::MeasureSpec::MakeMeasureSpec(mDropDownWidth,
                View::MeasureSpec::AT_MOST);
            Int32 heightSpec = View::MeasureSpec::UNSPECIFIED;

            hintView->Measure(widthSpec, heightSpec);
            AutoPtr<IViewGroupLayoutParams> pms;
            hintView->GetLayoutParams((IViewGroupLayoutParams**)&pms);
            hintParams = ILinearLayoutLayoutParams::Probe(pms);

            Int32 measureHeight = 0;
            hintView->GetMeasuredHeight(&measureHeight);
            Int32 topMargin = 0;
            hintParams->GetTopMargin(&topMargin);
            Int32 bottomMargin = 0;
            hintParams->GetBottomMargin(&bottomMargin);
            otherHeights = measureHeight + topMargin + bottomMargin;

            dropDownView = hintcontainer;
        }

        mPopup->SetContentView(dropDownView);
    }
    else {
        AutoPtr<IView> view;
        mPopup->GetContentView((IView**)&view);
        dropDownView = IViewGroup::Probe(view);
        view = mPromptView;
        if (view) {
            AutoPtr<IViewGroupLayoutParams> pms;
            view->GetLayoutParams((IViewGroupLayoutParams**)&pms);
            AutoPtr<ILinearLayoutLayoutParams> hintParams = ILinearLayoutLayoutParams::Probe(pms);

            Int32 measureHeight = 0;
            view->GetMeasuredHeight(&measureHeight);
            Int32 topMargin = 0;
            hintParams->GetTopMargin(&topMargin);
            Int32 bottomMargin = 0;
            hintParams->GetBottomMargin(&bottomMargin);
            otherHeights = measureHeight + topMargin + bottomMargin;
        }
    }

    Int32 padding = 0;
    AutoPtr<IDrawable> background;
    mPopup->GetBackground((IDrawable**)&background);
    if (background) {
        Boolean paddingRes = FALSE;
        background->GetPadding(mTempRect, &paddingRes);
        AutoPtr<CRect> cTempRect = (CRect*)mTempRect.Get();
        padding = cTempRect->mTop + cTempRect->mBottom;

        if(!mDropDownVerticalOffsetSet) {
            mDropDownVerticalOffset = -cTempRect->mTop;
        }
    }
    else {
        mTempRect->SetEmpty();
    }

    Int32 ipmMode = 0;
    mPopup->GetInputMethodMode(&ipmMode);
    Boolean ignoreBottomDecorations = ipmMode == IPopupWindow::INPUT_METHOD_NOT_NEEDED;
    Int32 maxHeight = 0;
    mPopup->GetMaxAvailableHeight(GetAnchorView(),
        mDropDownVerticalOffset, ignoreBottomDecorations, &maxHeight);

    if (mDropDownAlwaysVisible || mDropDownHeight == IViewGroupLayoutParams::MATCH_PARENT) {
        return maxHeight + padding;
    }

    Int32 childWidthSpec = 0;
    AutoPtr<IResources> res;
    mContext->GetResources((IResources**)&res);
    AutoPtr<IDisplayMetrics> disMetrics;
    res->GetDisplayMetrics((IDisplayMetrics**)&disMetrics);
    AutoPtr<CDisplayMetrics> cDisMetrics = (CDisplayMetrics*)disMetrics.Get();
    AutoPtr<CRect> cTempRect = (CRect*)mTempRect.Get();
    switch (mDropDownWidth) {
        case IViewGroupLayoutParams::WRAP_CONTENT:
            childWidthSpec = View::MeasureSpec::MakeMeasureSpec(
                cDisMetrics->mWidthPixels - (cTempRect->mLeft + cTempRect->mRight),
                View::MeasureSpec::AT_MOST);
            break;
        case IViewGroupLayoutParams::MATCH_PARENT:
            childWidthSpec = View::MeasureSpec::MakeMeasureSpec(
                cDisMetrics->mWidthPixels - (cTempRect->mLeft + cTempRect->mRight),
                View::MeasureSpec::EXACTLY);
            break;
        default:
            childWidthSpec = View::MeasureSpec::MakeMeasureSpec(
                mDropDownWidth, View::MeasureSpec::EXACTLY);
            break;
    }

    Int32 listContent = mDropDownList->MeasureHeightOfChildren(
        childWidthSpec, 0, ListView::NO_POSITION, maxHeight - otherHeights, -1);
    if (listContent > 0) otherHeights += padding;

    return listContent + otherHeights;
}

//===================================================================================
//                   ListPopupWindow::DropDownListView
//===================================================================================

AutoPtr<ListPopupWindow::DropDownListView> ListPopupWindow::DropDownListView::GetNewInstance(
    /* [in] */ IContext* context,
    /* [in] */ Boolean hijackFocus,
    /* [in] */ ListPopupWindow* host)
{
    AutoPtr<DropDownListView> instance = new DropDownListView();
    instance->Init(context, hijackFocus, host);
    return instance;
}

ECode ListPopupWindow::DropDownListView::Init(
    /* [in] */ IContext* context,
    /* [in] */ Boolean hijackFocus,
    /* [in] */ ListPopupWindow* host)
{
    ListView::Init(context, NULL, R::attr::dropDownListViewStyle);
    mHost = host;
    mHijackFocus = hijackFocus;
    SetCacheColorHint(0);
    return NOERROR;
}

ListPopupWindow::DropDownListView::DropDownListView()
{
}

ListPopupWindow::_DropDownListView::_DropDownListView()
{
}

Boolean ListPopupWindow::_DropDownListView::IsInTouchMode()
{
    return (mHijackFocus && mListSelectionHidden) || ListView::IsInTouchMode();
}

Boolean ListPopupWindow::_DropDownListView::HasWindowFocus()
{
    return mHijackFocus || ListView::HasWindowFocus();
}

Boolean ListPopupWindow::_DropDownListView::IsFocused()
{
    return mHijackFocus || ListView::IsFocused();
}

Boolean ListPopupWindow::_DropDownListView::HasFocus()
{
    return mHijackFocus || ListView::HasFocus();
}

AutoPtr<IAnimation> ListPopupWindow::_DropDownListView::GetAnimation()
{
    return ListView::GetAnimation();
}

AutoPtr<IView> ListPopupWindow::_DropDownListView::ObtainView(
    /* [in] */ Int32 position,
    /* [in] */ ArrayOf<Boolean>* isScrap)
{
    AutoPtr<IView> view = ListView::ObtainView(position, isScrap);
    AutoPtr<ITextView> tv = ITextView::Probe(view);
    if (tv) {
        tv->SetHorizontallyScrolling(TRUE);
    }
    return view;
}

PInterface ListPopupWindow::DropDownListView::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)(IListView*)this;
    } else if (riid == EIID_IListView) {
        return (IListView*)this;
    } else if (riid == EIID_IAbsListView) {
        return (IAbsListView*)(IListView*)this;
    } else if (riid == EIID_IAdapterView) {
        return (IAdapterView*)(IListView*)this;
    } else if (riid == EIID_IViewGroup) {
        return (IViewGroup*)(IListView*)this;
    } else if (riid == EIID_IView) {
        return (IView*)(IListView*)this;
    } else if (riid == EIID_IViewParent) {
        return (IViewParent*)this;
    } else if (riid == EIID_IViewManager) {
        return (IViewManager*)this;
    } else if (riid == EIID_IDrawableCallback) {
        return (IDrawableCallback*)this;
    } else if (riid == EIID_IKeyEventCallback) {
        return (IKeyEventCallback*)this;
    } else if (riid == EIID_IAccessibilityEventSource) {
        return (IAccessibilityEventSource*)this;
    } else if (riid == EIID_IWeakReferenceSource) {
        return (IWeakReferenceSource*)this;
    } else if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)(_DropDownListView*)this);
    } else if (riid == EIID_ViewGroup) {
        return reinterpret_cast<PInterface>((ViewGroup*)(_DropDownListView*)this);
    }
    return NULL;
}

UInt32 ListPopupWindow::DropDownListView::AddRef()
{
    return _DropDownListView::AddRef();
}

UInt32 ListPopupWindow::DropDownListView::Release()
{
    return _DropDownListView::Release();
}

ECode ListPopupWindow::DropDownListView::GetInterfaceID(
    /* [in] */ IInterface *object,
    /* [out] */ InterfaceID *pIID)
{
    if (object == (IListView*)this) {
        *pIID = EIID_IListView;
    } else if (object == (IViewParent*)this) {
        *pIID = EIID_IViewParent;
    } else if (object == (IViewManager*)this) {
        *pIID = EIID_IViewManager;
    } else if (object == (IDrawableCallback*)this) {
        *pIID = EIID_IDrawableCallback;
    } else if (object == (IKeyEventCallback*)this) {
        *pIID = EIID_IKeyEventCallback;
    } else if (object == (IAccessibilityEventSource*)this) {
        *pIID = EIID_IAccessibilityEventSource;
    } else if (object == reinterpret_cast<PInterface>((View*)this)) {
        *pIID = EIID_View;
    } else if (object == reinterpret_cast<PInterface>((ViewGroup*)this)) {
        *pIID = EIID_ViewGroup;
    } else {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return NOERROR;
}

ECode ListPopupWindow::DropDownListView::GetWeakReference(
    /* [out] */ IWeakReference** weakReference)
{
    VALIDATE_NOT_NULL(weakReference)
    *weakReference = new WeakReferenceImpl(Probe(EIID_IInterface), CreateWeak(this));
    REFCOUNT_ADD(*weakReference)
    return NOERROR;
}

//===================================================================================
//                   ListPopupWindow::PopupDataSetObserver
//===================================================================================

ListPopupWindow::PopupDataSetObserver::PopupDataSetObserver(
    /* [in] */ ListPopupWindow* host)
                : mHost(host)
{}

ECode ListPopupWindow::PopupDataSetObserver::OnChanged()
{
    if (mHost->IsShowing()) {
        mHost->Show();
    }
    return NOERROR;
}

ECode ListPopupWindow::PopupDataSetObserver::OnInvalidated()
{
    mHost->Dismiss();
    return NOERROR;
}

//===================================================================================
//                   ListPopupWindow::ListSelectorHider
//===================================================================================

ListPopupWindow::ListSelectorHider::ListSelectorHider(
    /* [in] */ ListPopupWindow* host)
                : mHost(host)
{}

ECode ListPopupWindow::ListSelectorHider::Run()
{
    mHost->ClearListSelection();
    return NOERROR;
}

//===================================================================================
//                   ListPopupWindow::ResizePopupRunnable
//===================================================================================

ListPopupWindow::ResizePopupRunnable::ResizePopupRunnable(
    /* [in] */ ListPopupWindow* host)
                : mHost(host)
{}

ECode ListPopupWindow::ResizePopupRunnable::Run()
{
    Int32 count = 0;
    mHost->mDropDownList->GetCount(&count);
    Int32 childCount = 0;
    mHost->mDropDownList->GetChildCount(&childCount);
    if (mHost->mDropDownList && count > childCount
        && childCount <= mHost->mListItemExpandMaximun) {
        mHost->mPopup->SetInputMethodMode(IPopupWindow::INPUT_METHOD_NOT_NEEDED);
        mHost->Show();
    }
    return NOERROR;
}

//===================================================================================
//                   ListPopupWindow::PopupTouchInterceptor
//===================================================================================

ListPopupWindow::PopupTouchInterceptor::PopupTouchInterceptor(
    /* [in] */ ListPopupWindow* host)
                : mHost(host)
{}

ECode ListPopupWindow::PopupTouchInterceptor::OnTouch(
    /* [in] */ IView* v,
    /* [in] */ IMotionEvent* event,
    /* [out] */ Boolean* result)
{
    Int32 action = 0;
    event->GetAction(&action);
    Float fx = 0;
    event->GetX(&fx);
    Int32 x = (Int32)fx;
    Float fy = 0;
    event->GetY(&fy);
    Int32 y = (Int32)fy;
    Int32 width = 0;
    mHost->mPopup->GetWidth(&width);
    Int32 height = 0;
    mHost->mPopup->GetHeight(&height);
    Boolean isShowing = FALSE;
    mHost->mPopup->IsShowing(&isShowing);
    if (action == IMotionEvent::ACTION_DOWN && mHost->mPopup && isShowing &&
        (x >= 0 && x < width && y >= 0 && y < height)) {
        Boolean result;
        mHost->mHandler->PostDelayed(mHost->mResizePopupRunnable,
            EXPAND_LIST_TIMEOUT, &result);
    }
    else if (action == IMotionEvent::ACTION_UP) {
        mHost->mHandler->RemoveCallbacks(mHost->mResizePopupRunnable);
    }
    *result = FALSE;
    return NOERROR;
}

//===================================================================================
//                   ListPopupWindow::PopupScrollListener
//===================================================================================

ListPopupWindow::PopupScrollListener::PopupScrollListener(
    /* [in] */ ListPopupWindow* host)
                : mHost(host)
{}

ECode ListPopupWindow::PopupScrollListener::OnScroll(
    /* [in] */ IAbsListView* view,
    /* [in] */ Int32 firstVisibleItem,
    /* [in] */ Int32 visibleItemCount,
    /* [in] */ Int32 totalItemCount)
{
    return NOERROR;
}

ECode ListPopupWindow::PopupScrollListener::OnScrollStateChanged(
    /* [in] */ IAbsListView* view,
    /* [in] */ Int32 scrollState)
{
    AutoPtr<IView> v;
    mHost->mPopup->GetContentView((IView**)&v);
    if (scrollState == IAbsListViewOnScrollListener::SCROLL_STATE_TOUCH_SCROLL
        && mHost->IsInputMethodNotNeeded() && v) {
        mHost->mHandler->RemoveCallbacks(mHost->mResizePopupRunnable);
        mHost->mResizePopupRunnable->Run();
    }
    return NOERROR;
}

//===================================================================================
//                   ListPopupWindow::ShowDropDownRunnable
//===================================================================================

ListPopupWindow::ShowDropDownRunnable::ShowDropDownRunnable(
    /* [in] */ ListPopupWindow* host) :
        mHost(host)
{

}

ECode ListPopupWindow::ShowDropDownRunnable::Run()
{
    AutoPtr<IView> view = mHost->GetAnchorView();
    AutoPtr<IBinder> binder;
    view->GetWindowToken((IBinder**)&binder);
    if (view && binder) {
        mHost->Show();
    }
    return NOERROR;
}

//===================================================================================
//                   ListPopupWindow::ListOnItemSelectedListener
//===================================================================================

ListPopupWindow::ListOnItemSelectedListener::ListOnItemSelectedListener(
    /* [in] */ ListPopupWindow* host)
                : mHost(host)
{

}

ECode ListPopupWindow::ListOnItemSelectedListener::OnItemSelected(
    /* [in] */ IAdapterView* parent,
    /* [in] */ IView* view,
    /* [in] */ Int32 position,
    /* [in] */ Int64 id)
{
    if (position != -1) {
        AutoPtr<DropDownListView> list = mHost->mDropDownList;
        if (list) {
            list->mListSelectionHidden = FALSE;
        }
    }
    return NOERROR;
}

ECode ListPopupWindow::ListOnItemSelectedListener::OnNothingSelected(
    /* [in] */ IAdapterView* parent)
{
    return NOERROR;
}


} // namespace Widget
} // namespace Droid
} // namespace Elastos
