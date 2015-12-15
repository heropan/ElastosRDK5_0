
#include "Elastos.Droid.Content.h"
#include "elastos/droid/webkit/native/ui/autofill/AutofillPopup.h"
#include "elastos/utility/Arrays.h"

using Elastos::Droid::Widget::EIID_IAdapterViewOnItemClickListener;
using Elastos::Utility::Arrays;

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

CAR_INTERFACE_IMPL(AutofillPopup, DropdownPopupWindow, IAdapterViewOnItemClickListener);

AutofillPopup::AutofillPopup(
    /* [in] */ IContext* context,
    /* [in] */ ViewElastosDelegate* viewElastosDelegate,
    /* [in] */ AutofillPopupDelegate* autofillCallback)
    : DropdownPopupWindow(context, viewElastosDelegate)
    , mContext(context)
    , mAutofillCallback(autofillCallback)
{
    // ==================before translated======================
    // super(context, viewAndroidDelegate);
    // mContext = context;
    // mAutofillCallback = autofillCallback;
    //
    // setOnItemClickListener(this);

    assert(0);
    //SetOnItemClickListener(this);
}

ECode AutofillPopup::FilterAndShow(
    /* [in] */ ArrayOf<IInterface*>* suggestions)
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

    assert(0);/*
    AutoPtr<IList> tmpSuggestion;
    Arrays<AutofillSuggestion*>::AsList(suggestions, (IList**)&tmpSuggestion);

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
    Show();*/
    return NOERROR;
}

ECode AutofillPopup::Dismiss()
{
    // ==================before translated======================
    // mAutofillCallback.requestHide();

    mAutofillCallback->RequestHide();
    return NOERROR;
}

ECode AutofillPopup::Hide()
{
    // ==================before translated======================
    // super.dismiss();

    Dismiss();
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
    //AutoPtr<DropdownAdapter> adapter = GetAdapter();
    //int listIndex = mSuggestions->IndexOf(adapter.getItem(position));
    //assert listIndex > -1;
    //mAutofillCallback->SuggestionSelected(listIndex);
    return NOERROR;
}

} // namespace Autofill
} // namespace Ui
} // namespace Webkit
} // namespace Droid
} // namespace Elastos


