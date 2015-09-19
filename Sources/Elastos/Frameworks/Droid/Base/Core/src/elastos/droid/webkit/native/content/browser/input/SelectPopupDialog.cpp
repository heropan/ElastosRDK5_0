
namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Content {
namespace Browser {
namespace Input {

//==================================================================
//      SelectPopupDialog::OkDialogInterfaceOnClickListener
//==================================================================

SelectPopupDialog::OkDialogInterfaceOnClickListener::OkDialogInterfaceOnClickListener(
    /* [in] */ SelectPopupDialog* owner,
    /* [in] */ IListView* listView)
    : mOwner(owner)
    , mListView(listView)
{
}

ECode SelectPopupDialog::OkDialogInterfaceOnClickListener::OnClick(
    /* [in] */ IDialogInterface* dialog,
    /* [in] */ Int32 which)
{
    mOwner->mContentViewCore->SelectPopupMenuItems(mOwner->GetSelectedIndices(mListView));
    return NOERROR;
}

//==================================================================
//     SelectPopupDialog::CancelDialogInterfaceOnClickListener
//==================================================================

SelectPopupDialog::CancelDialogInterfaceOnClickListener::CancelDialogInterfaceOnClickListener(
    /* [in] */ SelectPopupDialog* owner)
    : mOwner(owner)
{
}

ECode SelectPopupDialog::CancelDialogInterfaceOnClickListener::OnClick(
    /* [in] */ IDialogInterface* dialog,
    /* [in] */ Int32 which)
{
    mOwner->mContentViewCore->SelectPopupMenuItems(NULL);
    return NOERROR;
}

//==================================================================
//     SelectPopupDialog::InnerAdapterViewOnItemClickListener
//==================================================================

SelectPopupDialog::InnerAdapterViewOnItemClickListener::InnerAdapterViewOnItemClickListener(
    /* [in] */ SelectPopupDialog* owner,
    /* [in] */ IListView* listView)
    : mOwner(owner)
    , mListView(listView)
{
}

ECode SelectPopupDialog::InnerAdapterViewOnItemClickListener::OnItemClick(
    /* [in] */ IAdapterView* parent,
    /* [in] */ IView* v,
    /* [in] */ Int32 position,
    /* [in] */ Int64 id)
{
    mOwner->mContentViewCore->SelectPopupMenuItems(mOwner->GetSelectedIndices(mListView));
    mOwner->mListBoxPopup->Dismiss();
    return NOERROR;
}

//==================================================================
//     SelectPopupDialog::InnerDialogInterfaceOnCancelListener
//==================================================================

SelectPopupDialog::InnerDialogInterfaceOnCancelListener::InnerDialogInterfaceOnCancelListener(
    /* [in] */ SelectPopupDialog* owner)
    : mOwner(owner)
{
}

ECode SelectPopupDialog::InnerDialogInterfaceOnCancelListener::OnCancel(
    /* [in] */ IDialogInterface* dialog)
{
    mOwner->mContentViewCore->SelectPopupMenuItems(NULL);
    return NOERROR;
}

//==================================================================
//                      SelectPopupDialog
//==================================================================

static AutoPtr< ArrayOf<Int32> > SELECT_DIALOG_ATTRS_Init()
{
    AutoPtr< ArrayOf<Int32> > array = ArrayOf<Int32>::Alloc(2);
    (*array)[0] = R::attr::select_dialog_multichoice;
    (*array)[1] = R::attr::select_dialog_singlechoice;
    return array;
}

const AutoPtr< ArrayOf<Int32> > SelectPopupDialog::SELECT_DIALOG_ATTRS = SELECT_DIALOG_ATTRS_Init();

SelectPopupDialog::SelectPopupDialog(
    /* [in] */ ContentViewCore* contentViewCore,
    /* [in] */ List<SelectPopupItem> items,
    /* [in] */ Boolean multiple,
    /* [in] */ ArrayOf<Int32>* selected)
    : mContentViewCore(contentViewCore)
    , mContext(mContentViewCore->GetContext())
{
    AutoPtr<IListView> listView;
    CListView::New(mContext, (IListView**)&listView);
    listView->SetCacheColorHint(0);
    AutoPtr<IAlertDialogBuilder> b;
    CAlertDialogBuilder::New(mContext, (IAlertDialogBuilder**)&b);
    b->SetView(listView);
    b->SetCancelable(TRUE);
    b->SetInverseBackgroundForced(TRUE);

    if (multiple) {
        AutoPtr<IDialogInterfaceOnClickListener> okListener =  new OkDialogInterfaceOnClickListener(this, listView);
        b->SetPositiveButton(android::R::string::ok, okListener);
        AutoPtr<IDialogInterfaceOnClickListener> cancelListener = new CancelDialogInterfaceOnClickListener(this);
        b->SetNegativeButton(android::R::string::cancel, cancelListener);
    }

    b->Create((IAlertDialog**)&mListBoxPopup);
    const AutoPtr<SelectPopupAdapter> adapter = new SelectPopupAdapter(
            mContext, GetSelectDialogLayout(multiple), items);
    listView->SetAdapter(adapter);
    listView->SetFocusableInTouchMode(TRUE);

    if (multiple) {
        listView->SetChoiceMode(IListView::CHOICE_MODE_MULTIPLE);
        for (Int32 i = 0; i < selected->GetLength(); ++i) {
            listView->SetItemChecked((*selected)[i], TRUE);
        }
    }
    else {
        listView->SetChoiceMode(IListView::CHOICE_MODE_SINGLE);
        AutoPtr<IAdapterViewOnItemClickListener> listener = new InnerAdapterViewOnItemClickListener(this, listView);
        listView->SetOnItemClickListener(listener);
        if (selected->GetLength() > 0) {
            listView->SetSelection((*selected)[0]);
            listView->SetItemChecked((*selected)[0], TRUE);
        }
    }

    AutoPtr<IDialogInterfaceOnCancelListener> listener = new InnerDialogInterfaceOnCancelListener(this);
    mListBoxPopup->SetOnCancelListener(this);
}

Int32 SelectPopupDialog::GetSelectDialogLayout(
    /* [in] */ Boolean isMultiChoice)
{
    Int32 resourceId;
    AutoPtr<ITypedArray> styledAttributes;
    mContext->ObtainStyledAttributes(
            R::style::SelectPopupDialog, SELECT_DIALOG_ATTRS,
            (ITypedArray**)&styledAttributes);
    styledAttributes->GetResourceId(isMultiChoice ? 0 : 1, 0, &resourceId);
    styledAttributes->Recycle();

    return resourceId;
}

AutoPtr< ArrayOf<Int32> > SelectPopupDialog::GetSelectedIndices(
    /* [in] */ IListView* listView)
{
    AutoPtr<ISparseBooleanArray> sparseArray;
    listView->GetCheckedItemPositions((ISparseBooleanArray**)&sparseArray);
    Int32 selectedCount = 0;
    Int32 size;
    sparseArray->Size(&size);
    for (Int32 i = 0; i < size; ++i) {
        Boolean bFlag;
        sparseArray->ValueAt(i, &bFlag);
        if (bFlag) {
            selectedCount++;
        }
    }

    AutoPtr< ArrayOf<Int32> > indices = ArrayOf<Int32>::Alloc(selectedCount);
    sparseArray->Size(&size);
    for (Int32 i = 0, j = 0; i < size; ++i) {
        Boolean bFlag;
        sparseArray->ValueAt(i, &bFlag);
        if (bFlag) {
            Int32 value;
            sparseArray->KeyAt(i, &value);
            (*indices)[j++] = value;
        }
    }

    return indices;
}

//@Override
void SelectPopupDialog::Show()
{
    mListBoxPopup->Show();
}

//@Override
void SelectPopupDialog::Hide()
{
    mListBoxPopup->Cancel();
}

} // namespace Input
} // namespace Browser
} // namespace Content
} // namespace Webkit
} // namespace Droid
} // namespace Elastos
