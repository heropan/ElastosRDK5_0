
#include "elastos/droid/webkit/FindActionModeCallback.h"
#include "elastos/droid/webkit/CWebViewClassic.h"
#include "elastos/droid/text/Selection.h"
#include "elastos/droid/view/LayoutInflater.h"
#include "elastos/droid/R.h"

using Elastos::Core::CInteger32;
using Elastos::Core::IInteger32;
using Elastos::Core::CStringWrapper;
using Elastos::Droid::Text::Selection;
using Elastos::Droid::Text::EIID_ITextWatcher;
using Elastos::Droid::Os::IBinder;
using Elastos::Droid::View::EIID_IActionModeCallback;
using Elastos::Droid::View::EIID_IViewOnClickListener;
using Elastos::Droid::View::ILayoutInflater;
using Elastos::Droid::View::LayoutInflater;
using Elastos::Droid::View::IMenuInflater;

namespace Elastos {
namespace Droid {
namespace Webkit {

//===============================================================
//              FindActionModeCallback::NoAction
//===============================================================

CAR_INTERFACE_IMPL(FindActionModeCallback::NoAction, IActionModeCallback);

ECode FindActionModeCallback::NoAction::OnCreateActionMode(
    /* [in] */ IActionMode* mode,
    /* [in] */ IMenu* menu,
    /* [out] */ Boolean* result)
{
    if (result) *result = FALSE;
    return NOERROR;
}

ECode FindActionModeCallback::NoAction::OnPrepareActionMode(
    /* [in] */ IActionMode* mode,
    /* [in] */ IMenu* menu,
    /* [out] */ Boolean* result)
{
    if (result) *result = FALSE;
    return NOERROR;
}

ECode FindActionModeCallback::NoAction::OnActionItemClicked(
    /* [in] */ IActionMode* mode,
    /* [in] */ IMenuItem* item,
    /* [out] */ Boolean* result)
{
    if (result) *result = FALSE;
    return NOERROR;
}

ECode FindActionModeCallback::NoAction::OnDestroyActionMode(
    /* [in] */ IActionMode* mode)
{
    return NOERROR;
}

//===============================================================
//                   FindActionModeCallback
//===============================================================

FindActionModeCallback::FindActionModeCallback(
    /* [in] */ IContext* context)
    : mMatchesFound(FALSE)
    , mNumberOfMatches(0)
    , mActiveMatchIndex(0)
{
    AutoPtr<ILayoutInflater> inflate;
    LayoutInflater::From(context, (ILayoutInflater**)&inflate);
    inflate->Inflate(R::layout::webview_find, NULL, (IView**)&mCustomView);
    mCustomView->FindViewById(R::id::edit, (IView**)&mEditText);
    AutoPtr<NoAction> action = new NoAction();
    mEditText->SetCustomSelectionActionModeCallback(action);
    mEditText->SetOnClickListener(this);
    SetText(String(""));
    mCustomView->FindViewById(R::id::matches, (IView**)&mMatches);
    context->GetSystemService(IContext::INPUT_METHOD_SERVICE, (IInterface**)&mInput);
    context->GetResources((IResources**)&mResources);
}

UInt32 FindActionModeCallback::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 FindActionModeCallback::Release()
{
    return ElRefBase::Release();
}

ECode FindActionModeCallback::GetInterfaceID(
    /* [in] */ IInterface *Object,
    /* [out] */ InterfaceID *IID)
{
    VALIDATE_NOT_NULL(IID);

    if (Object == (IInterface*)(IActionModeCallback*)this) {
        *IID = EIID_IActionModeCallback;
    }
    else if (Object == (IInterface*)(ITextWatcher*)this) {
        *IID = EIID_ITextWatcher;
    }
    else if (Object == (IInterface*)(IViewOnClickListener*)this) {
        *IID = EIID_IViewOnClickListener;
    }
    else {
        return E_INVALID_ARGUMENT;
    }

    return NOERROR;
}

PInterface FindActionModeCallback::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IActionModeCallback) {
        return (IActionModeCallback*)this;
    }
    else if (riid == EIID_ITextWatcher) {
        return (ITextWatcher*)this;
    }
    else if (riid == EIID_IViewOnClickListener) {
        return (IViewOnClickListener*)this;
    }

    return NULL;
}

void FindActionModeCallback::Finish()
{
    mActionMode->Finish();
}

/*
 * Place text in the text field so it can be searched for.  Need to press
 * the find next or find previous button to find all of the matches.
 */
void FindActionModeCallback::SetText(
    /* [in] */ const String& text)
{
    AutoPtr<ICharSequence> textWrapper;
    CStringWrapper::New(text, (ICharSequence**)&textWrapper);
    mEditText->SetText(textWrapper);
    AutoPtr<ISpannable> span;
    mEditText->GetText((ICharSequence**)&span);
    Int32 length;
    span->GetLength(&length);
    // Ideally, we would like to set the selection to the whole field,
    // but this brings up the Text selection CAB, which dismisses this
    // one.
    Selection::SetSelection(span, length, length);
    // Necessary each time we set the text, so that this will watch
    // changes to it.
    assert(0);
//    span->SetSpan(this, 0, length, ISpannable::SPAN_INCLUSIVE_INCLUSIVE);
    mMatchesFound = FALSE;
}

/*
 * Set the WebView to search.  Must be non null, and set before calling
 * startActionMode.
 */
void FindActionModeCallback::SetWebView(
    /* [in] */ CWebViewClassic* webView)
{
    if (NULL == webView) {
        //throw new AssertionError("WebView supplied to "
        //        + "FindActionModeCallback cannot be null");
        assert(0);
    }
    mWebView = webView;
}

/*
 * Highlight all the instances of the string from mEditText in mWebView.
 */
void FindActionModeCallback::FindAll()
{
    if (mWebView == NULL) {
        //throw new AssertionError(
        //        "No WebView for FindActionModeCallback::findAll");
        assert(0);
    }

    AutoPtr<ICharSequence> find;
    mEditText->GetText((ICharSequence**)&find);
    Int32 length = 0;
    find->GetLength(&length);
    if (0 == length) {
        mWebView->ClearMatches();
        mMatches->SetVisibility(IView::GONE);
        mMatchesFound = FALSE;
        Int32 all;
        mWebView->FindAll(String(NULL), &all);
    }
    else {
        mMatchesFound = TRUE;
        mMatches->SetVisibility(IView::INVISIBLE);
        mNumberOfMatches = 0;
        String str;
        find->ToString(&str);
        mWebView->FindAllAsync(str);
    }
}

void FindActionModeCallback::ShowSoftInput()
{
    AutoPtr<IView> view;
    mEditText->GetRootView((IView**)&view);
    mInput->StartGettingWindowFocus(view);
    mInput->FocusIn(mEditText);
    mInput->ShowSoftInput(mEditText, 0, NULL);
}

void FindActionModeCallback::UpdateMatchCount(
    /* [in] */ Int32 matchIndex,
    /* [in] */ Int32 matchCount,
    /* [in] */ Boolean isEmptyFind)
{
    if (!isEmptyFind) {
        mNumberOfMatches = matchCount;
        mActiveMatchIndex = matchIndex;
        UpdateMatchesString();
    }
    else {
        mMatches->SetVisibility(IView::INVISIBLE);
        mNumberOfMatches = 0;
    }
}

// OnClickListener implementation

ECode FindActionModeCallback::OnClick(
    /* [in] */ IView* v)
{
    FindNext(TRUE);
    return NOERROR;
}

// ActionMode.Callback implementation

ECode FindActionModeCallback::OnCreateActionMode(
    /* [in] */ IActionMode* mode,
    /* [in] */ IMenu* menu,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(mode);
    VALIDATE_NOT_NULL(menu);

    Boolean bFlag = FALSE;
    mode->IsUiFocusable(&bFlag);
    if (!bFlag) {
        // If the action mode we're running in is not focusable the user
        // will not be able to type into the find on page field. This
        // should only come up when we're running in a dialog which is
        // already less than ideal; disable the option for now.
        return FALSE;
    }

    mode->SetCustomView(mCustomView);
    AutoPtr<IMenuInflater> menuInflater;
    mode->GetMenuInflater((IMenuInflater**)&menuInflater);
    menuInflater->Inflate(R::menu::webview_find, menu);
    mActionMode = mode;
    AutoPtr<IEditable> edit;
    mEditText->GetText((ICharSequence**)&edit);
    Int32 length;
    edit->GetLength(&length);
    Selection::SetSelection(edit, length);
    mMatches->SetVisibility(IView::GONE);
    mMatchesFound = FALSE;
    AutoPtr<ICharSequence> text;
    CStringWrapper::New(String("0"), (ICharSequence**)&text);
    mMatches->SetText(text);
    mEditText->RequestFocus(NULL);

    if (result) {
        *result = TRUE;
    }

    return NOERROR;
}

ECode FindActionModeCallback::OnDestroyActionMode(
    /* [in] */ IActionMode* mode)
{
    mActionMode = NULL;
    mWebView->NotifyFindDialogDismissed();
    AutoPtr<IBinder> binder;
//    mWebView->GetWebView()->GetWindowToken((IBinder**)&binder);
    mInput->HideSoftInputFromWindow(binder, 0, NULL);

    return NOERROR;
}

ECode FindActionModeCallback::OnPrepareActionMode(
    /* [in] */ IActionMode* mode,
    /* [in] */ IMenu* menu,
    /* [out] */ Boolean* result)
{
    if (result) {
        *result = FALSE;
    }

    return NOERROR;
}

//@Override
ECode FindActionModeCallback::OnActionItemClicked(
    /* [in] */ IActionMode* mode,
    /* [in] */ IMenuItem* item,
    /* [out] */ Boolean* result)
{
    if (mWebView == NULL) {
        //throw new AssertionError(
        //        "No WebView for FindActionModeCallback::onActionItemClicked");
        assert(0);
    }

    AutoPtr<IBinder> binder;
    assert(0);
//    mWebView->GetWebView()->GetWindowToken((IBinder**)&binder);
    mInput->HideSoftInputFromWindow(binder, 0, NULL);
    Int32 id;
    item->GetItemId(&id);
    switch(id) {
        case R::id::find_prev:
            FindNext(FALSE);
            break;
        case R::id::find_next:
            FindNext(TRUE);
            break;
        default:
            if (result) *result = FALSE;
            return NOERROR;
    }

    if (result) *result = TRUE;

    return NOERROR;
}

// TextWatcher implementation

ECode FindActionModeCallback::BeforeTextChanged(
    /* [in] */ ICharSequence* s,
    /* [in] */ Int32 start,
    /* [in] */ Int32 count,
    /* [in] */ Int32 after)
{
    // Does nothing.  Needed to implement TextWatcher.
    return NOERROR;
}

ECode FindActionModeCallback::OnTextChanged(
    /* [in] */ ICharSequence* s,
    /* [in] */ Int32 start,
    /* [in] */ Int32 before,
    /* [in] */ Int32 count)
{
    FindAll();
    return NOERROR;
}

ECode FindActionModeCallback::AfterTextChanged(
    /* [in] */ IEditable* s)
{
    // Does nothing.  Needed to implement TextWatcher.
    return NOERROR;
}

Int32 FindActionModeCallback::GetActionModeGlobalBottom()
{
    if (mActionMode == NULL) {
        return 0;
    }

    AutoPtr<IView> view;
    mCustomView->GetParent((IViewParent**)&view);
    if (view == NULL) {
        view = mCustomView;
    }

    view->GetGlobalVisibleRect(mGlobalVisibleRect, mGlobalVisibleOffset, NULL);

    Int32 bottom;
    mGlobalVisibleRect->GetBottom(&bottom);

    return NOERROR;
}

/*
 * Move the highlight to the next match.
 * @param next If true, find the next match further down in the document.
 *             If false, find the previous match, up in the document.
 */
void FindActionModeCallback::FindNext(
    /* [in] */ Boolean next)
{
    if (mWebView == NULL) {
        //throw new AssertionError(
        //        "No WebView for FindActionModeCallback::findNext");
        assert(0);
    }

    if (!mMatchesFound) {
        FindAll();
        return;
    }

    if (0 == mNumberOfMatches) {
        // There are no matches, so moving to the next match will not do
        // anything.
        return;
    }

    mWebView->FindNext(next);
    UpdateMatchesString();
}

/*
 * Update the string which tells the user how many matches were found, and
 * which match is currently highlighted.
 */
void FindActionModeCallback::UpdateMatchesString()
{
    if (mNumberOfMatches == 0) {
        mMatches->SetText(R::string::no_matches);
    }
    else {
        AutoPtr< ArrayOf<IInterface*> > formatArgs = ArrayOf<IInterface*>::Alloc(2);
        AutoPtr<IInteger32> value1, value2;
        CInteger32::New(mActiveMatchIndex + 1, (IInteger32**)&value1);
        CInteger32::New(mNumberOfMatches, (IInteger32**)&value2);
        (*formatArgs)[0] = value1;
        (*formatArgs)[1] = value2;
        String str;
        mResources->GetQuantityString(
            R::plurals::matches_found, mNumberOfMatches,
            formatArgs, &str);
        AutoPtr<ICharSequence> strCS;
        CStringWrapper::New(str, (ICharSequence**)&strCS);
        mMatches->SetText(strCS);
    }

    mMatches->SetVisibility(IView::VISIBLE);
}

} // namespace Webkit
} // namepsace Droid
} // namespace Elastos
