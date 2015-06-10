
namespace Elastos {
namespace Droid {
namespace Webkit {
namespace AndroidWebview {

//===============================================================
//         AwAutofillClient::InnerAutofillPopupDelegate
//===============================================================

AwAutofillClient::InnerAutofillPopupDelegate::InnerAutofillPopupDelegate(
    /* [in] */ AwAutofillClient* owner)
    : mOwner(owner)
{
}

//@Override
void AwAutofillClient::InnerAutofillPopupDelegate::RequestHide()
{
}

//@Override
void AwAutofillClient::InnerAutofillPopupDelegate::SuggestionSelected(
    /* [in] */ Int32 listIndex)
{
    mOwner->NativeSuggestionSelected(mNativeAwAutofillClient, listIndex);
}

//===============================================================
//                      AwAutofillClient
//===============================================================

AwAutofillClient::AwAutofillClient(
    /* [in] */ Int64 nativeAwAutofillClient)
    : mNativeAwAutofillClient(nativeAwAutofillClient)
{
}

//@CalledByNative
AutoPtr<AwAutofillClient> AwAutofillClient::Create(
    /* [in] */ Int64 nativeClient)
{
    AutoPtr<AwAutofillClient> client = new AwAutofillClient(nativeClient);
    return client;
}

void AwAutofillClient::Init(
    /* [in] */ ContentViewCore* contentViewCore)
{
    mContentViewCore = contentViewCore;
    mContainerView = contentViewCore->GetContainerView();
}

//@CalledByNative
void AwAutofillClient::ShowAutofillPopup(
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ Float width,
    /* [in] */ Float height,
    /* [in] */ ArrayOf<AutofillSuggestion>* suggestions)
{
    if (mContentViewCore == NULL) return;

    if (mAutofillPopup == NULL) {
        AutoPtr<AutofillPopupDelegate> delegate = new InnerAutofillPopupDelegate(this);
        mAutofillPopup = new AutofillPopup(
            mContentViewCore->GetContext(),
            mContentViewCore->GetViewAndroidDelegate(),
            delegate);
    }
    mAutofillPopup->SetAnchorRect(x, y, width, height);
    mAutofillPopup->FilterAndShow(suggestions);
}

//@CalledByNative
void AwAutofillClient::HideAutofillPopup()
{
    if (mAutofillPopup == NULL)
        return;

    mAutofillPopup->Hide();
    mAutofillPopup = NULL;
}

//@CalledByNative
AutoPtr< ArrayOf<AutofillSuggestion> > AwAutofillClient::CreateAutofillSuggestionArray(
    /* [in] */ Int32 size)
{
    AutoPtr< ArrayOf<AutofillSuggestion> > array = ArrayOf<AutofillSuggestion>::Alloc(size);
    return array;
}

/**
 * @param array AutofillSuggestion array that should get a new suggestion added.
 * @param index Index in the array where to place a new suggestion.
 * @param name Name of the suggestion.
 * @param label Label of the suggestion.
 * @param uniqueId Unique suggestion id.
 */
//@CalledByNative
void AwAutofillClient::AddToAutofillSuggestionArray(
    /* [in] */ ArrayOf<AutofillSuggestion>* array,
    /* [in] */ Int32 index,
    /* [in] */ String name,
    /* [in] */ String label,
    /* [in] */ Int32 uniqueId)
{
    (*array)[index] = new AutofillSuggestion(name, label, uniqueId);
}

void AwAutofillClient::NativeSuggestionSelected(
    /* [in] */ Int64 nativeAwAutofillClient,
    /* [in] */ Int32 position)
{
}

} // namespace AndroidWebview
} // namespace Webkit
} // namespace Droid
} // namespace Elastos