
#include "webkit/SelectActionModeCallback.h"
#include "webkit/CWebViewClassic.h"
//#include "provider/CBrowser.h"
#include "content/CIntent.h"
#include "R.h"

using Elastos::Core::CStringWrapper;
using Elastos::Droid::App::EIID_IActivity;
using Elastos::Droid::App::ISearchManager;
using Elastos::Droid::Content::CIntent;
using Elastos::Droid::Content::IClipboardManager;
using Elastos::Droid::View::EIID_IActionModeCallback;
using Elastos::Droid::View::IMenuInflater;

namespace Elastos {
namespace Droid {
namespace Webkit {

SelectActionModeCallback::SelectActionModeCallback()
    : mIsTextSelected(TRUE)
{
}

CAR_INTERFACE_IMPL(SelectActionModeCallback, IActionModeCallback);

void SelectActionModeCallback::SetWebView(
    /* [in] */ CWebViewClassic* webView)
{
    mWebView = webView;
}

void SelectActionModeCallback::SetTextSelected(
    /* [in] */ Boolean isTextSelected)
{
    mIsTextSelected = isTextSelected;
}

void SelectActionModeCallback::Finish()
{
    // It is possible that onCreateActionMode was never called, in the case
    // where there is no ActionBar, for example.
    if (mActionMode != NULL) {
        mActionMode->Finish();
    }
}

// ActionMode.Callback implementation

ECode SelectActionModeCallback::OnCreateActionMode(
    /* [in] */ IActionMode* mode,
    /* [in] */ IMenu* menu,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(mode);
    VALIDATE_NOT_NULL(menu);

    AutoPtr<IMenuInflater> menuInflater;
    mode->GetMenuInflater((IMenuInflater**)&menuInflater);
    menuInflater->Inflate(R::menu::webview_copy, menu);

    AutoPtr<IContext> context = mWebView->GetContext();
    String str;
    context->GetString(R::string::textSelectionCABTitle, &str);
    AutoPtr<ICharSequence> strCS;
    CStringWrapper::New(str, (ICharSequence**)&strCS);
    mode->SetTitle(strCS);
    mode->SetTitleOptionalHint(TRUE);

    // If the action mode UI we're running in isn't capable of taking window focus
    // the user won't be able to type into the find on page UI. Disable this functionality.
    // (Note that this should only happen in floating dialog windows.)
    // This can be removed once we can handle multiple focusable windows at a time
    // in a better way.
    AutoPtr<IClipboardManager> cm;
    context->GetSystemService(IContext::CLIPBOARD_SERVICE,
        (IInterface**)&cm);

    Boolean isFocusable;
    mode->IsUiFocusable(&isFocusable);

    Boolean isEditable = mWebView->FocusCandidateIsEditableText();
    Boolean bFlag = FALSE;
    cm->HasPrimaryClip(&bFlag);
    Boolean canPaste = isEditable && bFlag && isFocusable;
    Boolean canFind = !isEditable && isFocusable;
    Boolean canCut = isEditable && mIsTextSelected && isFocusable;
    Boolean canCopy = mIsTextSelected;
    Boolean canWebSearch = mIsTextSelected;
    SetMenuVisibility(menu, canFind, R::id::find);
    SetMenuVisibility(menu, canPaste, R::id::paste);
    SetMenuVisibility(menu, canCut, R::id::cut);
    SetMenuVisibility(menu, canCopy, R::id::copy);
    SetMenuVisibility(menu, canWebSearch, R::id::websearch);
    mActionMode = mode;

    if (result) *result = TRUE;

    return NOERROR;
}

ECode SelectActionModeCallback::OnPrepareActionMode(
    /* [in] */ IActionMode* mode,
    /* [in] */ IMenu* menu,
    /* [out] */ Boolean* result)
{
    if (result) *result = TRUE;
    return NOERROR;
}

ECode SelectActionModeCallback::OnActionItemClicked(
    /* [in] */ IActionMode* mode,
    /* [in] */ IMenuItem* item,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(mode);
    VALIDATE_NOT_NULL(item);
    VALIDATE_NOT_NULL(result);

    Int32 id;
    item->GetItemId(&id);
    switch(id) {
        case R::id::cut:
            mWebView->CutSelection();
            mode->Finish();
            break;

        case R::id::copy: {
            Boolean bFlag;
            mWebView->CopySelection(&bFlag);
            mode->Finish();
        }
            break;

        case R::id::paste:
            mWebView->PasteFromClipboard();
            mode->Finish();
            break;

        case R::id::share: {
                String selection = mWebView->GetSelection();
                AutoPtr<IContext> context = mWebView->GetContext();
                assert(0);
    //            AutoPtr<IBrowser> browser;
    //            CBrowser::AcquireSingleton((IBrowser**)&browser);
    //            browser->SendString(context, selection);
                mode->Finish();
            }
            break;

        case R::id::select_all:
            mWebView->SelectAll();
            break;

        case R::id::find: {
                String sel = mWebView->GetSelection();
                mode->Finish();
                Boolean bFlag;
                mWebView->ShowFindDialog(sel, FALSE, &bFlag);
            }
            break;

        case R::id::websearch: {
                mode->Finish();
                AutoPtr<IIntent> i;
                CIntent::New(IIntent::ACTION_WEB_SEARCH, (IIntent**)&i);
                i->PutBooleanExtra(ISearchManager::EXTRA_NEW_SEARCH, TRUE);
                i->PutStringExtra(ISearchManager::QUERY, mWebView->GetSelection());
                AutoPtr<IContext> context = mWebView->GetContext();
                if (context->Probe(EIID_IActivity) == NULL) {
                    i->AddFlags(IIntent::FLAG_ACTIVITY_NEW_TASK);
                }
                context->StartActivity(i);
            }
            break;

        default:
            *result = FALSE;
            return NOERROR;
    }

    *result = TRUE;

    return NOERROR;
}

ECode SelectActionModeCallback::OnDestroyActionMode(
    /* [in] */ IActionMode* mode)
{
    mWebView->SelectionDone();
    return NOERROR;
}

void SelectActionModeCallback::SetMenuVisibility(
    /* [in] */ IMenu* menu,
    /* [in] */ Boolean visible,
    /* [in] */ Int32 resourceId)
{
    AutoPtr<IMenuItem> item;
    menu->FindItem(resourceId, (IMenuItem**)&item);
    if (item != NULL) {
        item->SetVisible(visible);
    }
}

} // namespace Webkit
} // namepsace Droid
} // namespace Elastos