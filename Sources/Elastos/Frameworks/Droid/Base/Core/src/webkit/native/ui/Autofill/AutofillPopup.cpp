
namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Ui {

//===============================================================
//                  AutofillPopup
//===============================================================
AutofillPopup::AutofillPopup(
	/* in */ IContext* context,
	/* in */ ViewAndroidDelegate* viewAndroidDelegate,
	/* in */ AutofillPopupDelegate* autofillCallback)
	: DropdownPopupWindow(context, viewAndroidDelegate)
	, mContext(context)
	, mAutofillCallback(autofillCallback)
{
	SetOnItemClickListener(this);
}

/**
 * Filters the Autofill suggestions to the ones that we support and shows the popup.
 * @param suggestions Autofill suggestion data.
 */
ECode AutofillPopup::FilterAndShow(
    /* in */ ArrayOf<AutofillSuggestion*>* suggestions)
{
	VALIDATE_NOT_NULL(suggestions);
	// question: init
    mSuggestions = new List< AutoPtr<AutofillSuggestion> >(suggestions);
    // Remove the AutofillSuggestions with IDs that are not supported by Android
    AutoPtr< ArrayOf< AutoPtr<DropdownItem> > > cleanedData = ArrayOf< AutoPtr<DropdownItem> >::Alloc(1);
    AutoPtr<IHashSet> separators;
    CHashSet::New((IHashSet**)&separators);

    for (Int32 i = 0; i < suggestions->GetLength(); ++i) {
        Int32 itemId = (*suggestions)[i]->mUniqueId;
        if (itemId > 0 || itemId == ITEM_ID_AUTOCOMPLETE_ENTRY ||
                itemId == ITEM_ID_PASSWORD_ENTRY || itemId == ITEM_ID_DATA_LIST_ENTRY) {
            cleanedData->Append(suggestions[i]);
        }
        else if (itemId == ITEM_ID_SEPARATOR_ENTRY) {
            separators->Append(cleanedData->GetLength());
        }
    }

    AutoPtr<DropdownAdapter> dropDownAdapter = new DropdownAdapter(mContext, cleanedData, separators);
    SetAdapter(dropDownAdapter);
    Show();
    return NOERROR;
}

/**
 * Overrides the default dismiss behavior to request the controller to dismiss the view.
 */
//@Override
ECode AutofillPopup::Dismiss()
{
    mAutofillCallback->RequestHide();
    return NOERROR;
}

/**
 * Hides the popup.
 */
ECode AutofillPopup::Hide()
{
    Dismiss();
    return NOERROR;
}

} // namespace Ui
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

