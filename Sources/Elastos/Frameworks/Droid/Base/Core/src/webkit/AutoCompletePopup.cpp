
#include "webkit/AutoCompletePopup.h"
#include "webkit/CWebViewClassic.h"
#include "widget/CAbsoluteLayoutLayoutParams.h"
#include "widget/CListPopupWindow.h"
#include <elastos/Logger.h>

using Elastos::Utility::Logging::Logger;
using Elastos::Droid::View::EIID_IView;
using Elastos::Droid::View::IDispatcherState;
using Elastos::Droid::Widget::CAbsoluteLayoutLayoutParams;
using Elastos::Droid::Widget::EIID_IAdapterViewOnItemClickListener;
using Elastos::Droid::Widget::EIID_IFilterListener;
using Elastos::Droid::Widget::EIID_IPopupWindowOnDismissListener;
using Elastos::Droid::Widget::IAdapterView;
using Elastos::Droid::Widget::IAbsoluteLayoutLayoutParams;
using Elastos::Droid::Widget::IFilterable;
using Elastos::Droid::Widget::IListView;
using Elastos::Droid::Widget::CListPopupWindow;

namespace Elastos {
namespace Droid {
namespace Webkit {

//===============================================================
//                    AutoCompletePopup::AnchorView
//===============================================================
IVIEW_METHODS_IMPL(AutoCompletePopup::AnchorView, View);

//because the AutoCompletePopup::AnchorView implement two parent class
//however one is Interface(IView), the other is not from Interface(View)
#ifndef CAR_INTERFACE_IMPL_1_WITH_1_NON
#define CAR_INTERFACE_IMPL_1_WITH_1_NON(ClassName, Interface1, NonInterface2) \
    PInterface ClassName::Probe( \
        /* [in] */ REIID riid) \
    { \
        if ( riid == EIID_IInterface) { \
            return (IInterface*)(Interface1 *)this; \
        } \
        else if ( riid == EIID_##Interface1 ) { \
            return (Interface1 *)this; \
        } \
        else if ( riid == EIID_##NonInterface2 ) { \
            return (PInterface)((NonInterface2 *)this); \
        } \
        return NULL; \
    } \
    UInt32 ClassName::AddRef() \
    { \
        return ElRefBase::AddRef(); \
    } \
    UInt32 ClassName::Release() \
    { \
        return ElRefBase::Release(); \
    } \
    ECode ClassName::GetInterfaceID( \
        /* [in] */ IInterface* object, \
        /* [out] */ InterfaceID* iid) \
    { \
        VALIDATE_NOT_NULL(iid); \
        if (object == (IInterface*)(Interface1 *)this) { \
            *iid = EIID_##Interface1; \
        } \
        else { \
            return E_ILLEGAL_ARGUMENT_EXCEPTION; \
        } \
        return NOERROR; \
    }
#endif
CAR_INTERFACE_IMPL_1_WITH_1_NON(AutoCompletePopup::AnchorView, IView, View);
#if 0
//===============================================================
//                    AutoCompletePopup::MyHandler
//===============================================================
CAR_INTERFACE_IMPL(AutoCompletePopup::MyHandler, IHandler);

IHANDLER_METHODS_IMPL(AutoCompletePopup::MyHandler, Handler);

ECode AutoCompletePopup::MyHandler::HandleMessage(
            /* [in] */ IMessage* msg)
{
    VALIDATE_NOT_NULL(msg);

    Int32 what;
    msg->GetWhat(&what);
    switch (what) {
    case AUTOFILL_FORM:
//        mOwner->mWebView->AutoFillForm(mQueryId);
        break;
    }
}
#endif

//===============================================================
//                        AutoCompletePopup
//===============================================================
const Int32 AutoCompletePopup::AUTOFILL_FORM;

AutoCompletePopup::AutoCompletePopup(
    /* [in] */ CWebViewClassic* webView,
    /* [in] */ CWebViewClassicWebViewInputConnection* inputConnection)
    : mInputConnection(inputConnection)
    , mWebView(webView)
{
//    ASSERT_SUCCEEDED(CHandler::New(IHandler**)&mHandler);
}

UInt32 AutoCompletePopup::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 AutoCompletePopup::Release()
{
    return ElRefBase::Release();
}

ECode AutoCompletePopup::GetInterfaceID(
    /* [in] */ IInterface *Object,
    /* [out] */ InterfaceID *IID)
{
    VALIDATE_NOT_NULL(IID);

    if (Object == (IInterface*)(IAdapterViewOnItemClickListener*)this) {
        *IID = EIID_IAdapterViewOnItemClickListener;
    }
    else if (Object == (IInterface*)(IFilterListener*)this) {
        *IID = EIID_IFilterListener;
    }
    else if (Object == (IInterface*)(IPopupWindowOnDismissListener*)this) {
        *IID = EIID_IPopupWindowOnDismissListener;
    }
    else {
        return E_INVALID_ARGUMENT;
    }

    return NOERROR;
}

PInterface AutoCompletePopup::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)(IAdapterViewOnItemClickListener*)this;
    }
    else if (riid == EIID_IAdapterViewOnItemClickListener) {
        return (IAdapterViewOnItemClickListener*)this;
    }
    else if (riid == EIID_IFilterListener) {
        return (IFilterListener*)this;
    }
    else if (riid == EIID_IPopupWindowOnDismissListener) {
        return (IPopupWindowOnDismissListener*)this;
    }

    return NULL;
}

Boolean AutoCompletePopup::OnKeyPreIme(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event)
{
    if (mPopup == NULL) {
        return FALSE;
    }
    Boolean bFlag = FALSE;
    if (keyCode == IKeyEvent::KEYCODE_BACK && (mPopup->IsShowing(&bFlag), bFlag)) {
        // special case for the back key, we do not even try to send it
        // to the drop down list but instead, consume it immediately
        Int32 action, count;
        event->GetAction(&action);
        event->GetRepeatCount(&count);
        if (action == IKeyEvent::ACTION_DOWN && count == 0) {
            AutoPtr<IDispatcherState> state;
            mAnchor->GetKeyDispatcherState((IDispatcherState**)&state);
            if (state != NULL) {
                state->StartTracking(event, this->Probe(EIID_IInterface));
            }
            return TRUE;
        }
        else if (action == IKeyEvent::ACTION_UP) {
            AutoPtr<IDispatcherState> state;
            mAnchor->GetKeyDispatcherState((IDispatcherState**)&state);
            if (state != NULL) {
                state->HandleUpEvent(event);
            }
            Boolean bFlag1, bFlag2;
            event->IsTracking(&bFlag1);
            event->IsCanceled(&bFlag2);
            if (bFlag1 && !bFlag2) {
                mPopup->Dismiss();
                return TRUE;
            }
        }
    }
    if (mPopup->IsShowing(&bFlag), bFlag) {
        mPopup->OnKeyPreIme(keyCode, event, &bFlag);
        return bFlag;
    }
    return FALSE;
}

void AutoCompletePopup::SetText(
    /* [in] */ ICharSequence* text)
{
    mText = text;
    if (mFilter != NULL) {
        mFilter->DoFilterEx(text, (IFilterListener*)this);
    }
}

void AutoCompletePopup::SetAutoFillQueryId(
    /* [in] */ Int32 queryId)
{
    mQueryId = queryId;
}

void AutoCompletePopup::ClearAdapter()
{
    mAdapter = NULL;
    mFilter = NULL;
    if (mPopup != NULL) {
        mPopup->Dismiss();
        mPopup->SetAdapter(NULL);
    }
}

void AutoCompletePopup::SetAdapter(
    /* [in] */ IInterface* adapter)
{
    assert(IListAdapter::Probe(adapter) != NULL);
    assert(IFilterable::Probe(adapter) != NULL);
    EnsurePopup();
    mPopup->SetAdapter((IListAdapter*)adapter);
    mAdapter = (IListAdapter*)adapter;
    if (adapter != NULL) {
        assert(0);
//        IFilterListener::Probe(adapter)->GetFilter((IFilter**)&mFilter);
        mFilter->DoFilterEx(mText, (IFilterListener*)this);
    } else {
        mFilter = NULL;
    }

    ResetRect();
}

void AutoCompletePopup::ResetRect()
{
    EnsurePopup();
    Int32 left, right, width;
    mWebView->mEditTextContentBounds->GetLeft(&left);
    mWebView->mEditTextContentBounds->GetRight(&right);
    left = mWebView->ContentToViewX(left);
    right = mWebView->ContentToViewX(right);
    width = right - left;
    mPopup->SetWidth(width);

    Int32 bottom, top, height;
    mWebView->mEditTextContentBounds->GetBottom(&bottom);
    mWebView->mEditTextContentBounds->GetTop(&top);
    bottom = mWebView->ContentToViewY(bottom);
    top = mWebView->ContentToViewY(top);
    height = bottom - top;

    AutoPtr<IAbsoluteLayoutLayoutParams> lp;
    mAnchor->GetLayoutParams((IViewGroupLayoutParams**)&lp);
    Boolean needsUpdate = FALSE;
    if (NULL == lp) {
        CAbsoluteLayoutLayoutParams::New(width, height, left, top, (IAbsoluteLayoutLayoutParams**)&lp);
    }
    else {
        Int32 x, y, _width, _height;
        lp->GetX(&x);
        lp->GetY(&y);
        lp->GetWidth(&_width);
        lp->GetHeight(&_height);
        if ((x != left) || (y != top) || (_width != width)
                || (_height != height)) {
            needsUpdate = TRUE;
            lp->SetX(left);
            lp->SetY(top);
            lp->SetWidth(width);
            lp->SetHeight(height);
        }
    }

    if (needsUpdate) {
        mAnchor->SetLayoutParams(lp);
    }

    Boolean bFlag = FALSE;
    if (mPopup->IsShowing(&bFlag), bFlag) {
        mPopup->Show(); // update its position
    }
}

// AdapterView.OnItemClickListener implementation
ECode AutoCompletePopup::OnItemClick(
    /* [in] */ IAdapterView* parent,
    /* [in] */ IView* view,
    /* [in] */ Int32 position,
    /* [in] */ Int64 id)
{
    if (mPopup == NULL) {
        return NOERROR;
    }
    if (id == 0 && position == 0 && mInputConnection->GetIsAutoFillable()) {

        mText = NULL;
        PushTextToInputConnection();
        // Blank out the text box while we wait for WebCore to fill the form.
        if (mIsAutoFillProfileSet) {
            // Call a webview method to tell WebCore to autofill the form.
            mWebView->AutoFillForm(mQueryId);
        }
        else {
            // There is no autofill profile setup yet and the user has
            // elected to try and set one up. Call through to the
            // embedder to action that.
            AutoPtr<IWebChromeClient> webChromeClient;
            mWebView->GetWebChromeClient((IWebChromeClient**)&webChromeClient);
            if (webChromeClient != NULL) {
                assert(0);
//                webChromeClient->SetupAutoFill(mHandler.obtainMessage(AUTOFILL_FORM));
            }
        }
    }
    else {
        AutoPtr<IInterface> selectedItem;
        if (position < 0) {
            mPopup->GetSelectedItem((IInterface**)&selectedItem);
        }
        else {
            mAdapter->GetItem(position, (IInterface**)&selectedItem);
        }

        if (selectedItem != NULL) {
            AutoPtr<ICharSequence> cs;
            mFilter->ConvertResultToString(selectedItem, (ICharSequence**)&cs);
            SetText(cs);
            PushTextToInputConnection();
        }
    }
    mPopup->Dismiss();

    return NOERROR;
}

void AutoCompletePopup::SetIsAutoFillProfileSet(
    /* [in] */ Boolean isAutoFillProfileSet)
{
    mIsAutoFillProfileSet = isAutoFillProfileSet;
}

void AutoCompletePopup::PushTextToInputConnection()
{
    AutoPtr<IEditable> oldText;
    mInputConnection->GetEditable((IEditable**)&oldText);
    Int32 length;
    oldText->GetLength(&length);
    Boolean result;
    mInputConnection->SetSelection(0, length, &result);
    mInputConnection->ReplaceSelection(mText);
    mText->GetLength(&length);
    mInputConnection->SetSelection(length, length, &result);
}

ECode AutoCompletePopup::OnFilterComplete(
    /* [in] */ Int32 count)
{
    EnsurePopup();
    Int32 length = -1;
    Boolean showDropDown = (count > 0) &&
            (mInputConnection->GetIsAutoFillable() || (mText->GetLength(&length), length > 0));

    if (showDropDown) {
        Boolean bFlag = FALSE;
        if (mPopup->IsShowing(&bFlag), !bFlag) {
            // Make sure the list does not obscure the IME when shown for the first time.
            mPopup->SetInputMethodMode(IListPopupWindow::INPUT_METHOD_NEEDED);
        }

        mPopup->Show();
        AutoPtr<IListView> lv;
        mPopup->GetListView((IListView**)&lv);
        lv->SetOverScrollMode(IView::OVER_SCROLL_ALWAYS);
    }
    else {
        mPopup->Dismiss();
    }
    return NOERROR;
}

ECode AutoCompletePopup::OnDismiss()
{
    AutoPtr<IWebView> webView;
    mWebView->GetWebView((IWebView**)&webView);
    //TODO below code need more check, not match with the origin java code
    return webView->RemoveViewInLayout(mAnchor);
}

void AutoCompletePopup::EnsurePopup()
{
    AutoPtr<IWebView> webView;
    mWebView->GetWebView((IWebView**)&webView);
    if (mPopup == NULL) {
        AutoPtr<IContext> context = mWebView->GetContext();
        CListPopupWindow::New(context, (IListPopupWindow**)&mPopup);
        mAnchor = (IView*)(new AnchorView(context));
        webView->AddView(mAnchor);
        mPopup->SetOnItemClickListener(this);
        mPopup->SetAnchorView(mAnchor);
        mPopup->SetPromptPosition(IListPopupWindow::POSITION_PROMPT_BELOW);
    }
    else {
        Int32 index = -1;
        webView->IndexOfChild(mAnchor, &index);
        if (index < 0) {
            webView->AddView(mAnchor);
        }
    }
}

} // namespace Webkit
} // namepsace Droid
} // namespace Elastos
