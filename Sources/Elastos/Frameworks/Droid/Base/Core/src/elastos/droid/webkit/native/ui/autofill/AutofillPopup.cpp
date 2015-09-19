// wuweizuo automatic build .cpp file from .java file.

#include "AutofillPopup.h"

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Ui {
namespace Autofill {

//=====================================================================
//                            AutofillPopup
//=====================================================================
const Int32 AutofillPopup::ITEM_ID_AUTOCOMPLETE_ENTRY;
const Int32 AutofillPopup::ITEM_ID_PASSWORD_ENTRY;
const Int32 AutofillPopup::ITEM_ID_SEPARATOR_ENTRY;
const Int32 AutofillPopup::ITEM_ID_DATA_LIST_ENTRY;

AutofillPopup::AutofillPopup(
    /* [in] */ IContext* context,
    /* [in] */ ViewAndroidDelegate* viewAndroidDelegate,
    /* [in] */ AutofillPopupDelegate* autofillCallback)
{
    // ==================before translated======================
    // super(context, viewAndroidDelegate);
    // mContext = context;
    // mAutofillCallback = autofillCallback;
    //
    // setOnItemClickListener(this);
}

ECode AutofillPopup::FilterAndShow(
    /* [in] */ ArrayOf<AutofillSuggestion>* suggestions)
{
    VALIDATE_NOT_NULL(suggestions);
    // ==================before translated======================
    // mSuggestions = new ArrayList<AutofillSuggestion>(Arrays.asList(suggestions));
    // // Remove the AutofillSuggestions with IDs that are not supported by Android
    // ArrayList<DropdownItem> cleanedData = new ArrayList<DropdownItem>();
    // HashSet<Integer> separators = new HashSet<Integer>();
    // for (int i = 0; i < suggestions.length; i++) {
    //     int itemId = suggestions[i].mUniqueId;
    //     if (itemId > 0 || itemId == ITEM_ID_AUTOCOMPLETE_ENTRY ||
    //             itemId == ITEM_ID_PASSWORD_ENTRY || itemId == ITEM_ID_DATA_LIST_ENTRY) {
    //         cleanedData.add(suggestions[i]);
    //     } else if (itemId == ITEM_ID_SEPARATOR_ENTRY) {
    //         separators.add(cleanedData.size());
    //     }
    // }
    // setAdapter(new DropdownAdapter(mContext, cleanedData, separators));
    // show();
    assert(0);
    return NOERROR;
}

ECode AutofillPopup::Dismiss()
{
    // ==================before translated======================
    // mAutofillCallback.requestHide();
    assert(0);
    return NOERROR;
}

ECode AutofillPopup::Hide()
{
    // ==================before translated======================
    // super.dismiss();
    assert(0);
    return NOERROR;
}

ECode AutofillPopup::OnItemClick(
    /* [in] */ IAdapterView* parent,
    /* [in] */ IView* view,
    /* [in] */ Int32 position,
    /* [in] */ Int64 id)
{
    VALIDATE_NOT_NULL(parent);
    VALIDATE_NOT_NULL(view);
    // ==================before translated======================
    // DropdownAdapter adapter = (DropdownAdapter) parent.getAdapter();
    // int listIndex = mSuggestions.indexOf(adapter.getItem(position));
    // assert listIndex > -1;
    // mAutofillCallback.suggestionSelected(listIndex);
    assert(0);
    return NOERROR;
}

} // namespace Autofill
} // namespace Ui
} // namespace Webkit
} // namespace Droid
} // namespace Elastos


