
namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Content {
namespace Browser {
namespace Input {

//==================================================================
//    SelectPopupDropdown::InnerAdapterViewOnItemClickListener
//==================================================================

SelectPopupDropdown::InnerAdapterViewOnItemClickListener::InnerAdapterViewOnItemClickListener(
    /* [in] */ SelectPopupDropdown* owner)
    : mOwner(owner)
{
}

ECode SelectPopupDropdown::InnerAdapterViewOnItemClickListener::OnItemClick(
    /* [in] */ IAdapterView* parent,
    /* [in] */ IView* view,
    /* [in] */ Int32 position,
    /* [in] */ Int64 id)
{
    AutoPtr< ArrayOf<Int32> > selectedIndices = ArrayOf<Int32>::Alloc(1);
    (*selectedIndices)[0] = position;
    mContentViewCore->SelectPopupMenuItems(selectedIndices);
    mAlreadySelectedItems = TRUE;
    Hide();

    return NOERROR;
}

//==================================================================
//     SelectPopupDropdown::InnerPopupWindowOnDismissListener
//==================================================================

SelectPopupDropdown::InnerPopupWindowOnDismissListener::InnerPopupWindowOnDismissListener(
    /* [in] */ SelectPopupDropdown* owner)
    : mOwner(owner)
{
}

ECode SelectPopupDropdown::InnerPopupWindowOnDismissListener::OnDismiss()
{
    if (!mAlreadySelectedItems) {
        mContentViewCore->SelectPopupMenuItems(NULL);
    }

    return NOERROR;
}

//==================================================================
//                     SelectPopupDropdown
//==================================================================

SelectPopupDropdown::SelectPopupDropdown(
    /* [in] */ ContentViewCore* contentViewCore,
    /* [in] */ List<SelectPopupItem> items,
    /* [in] */ IRect* bounds,
    /* [in] */ ArrayOf<Int32>* selected)
    : mInitialSelection(-1)
    , mAlreadySelectedItems(FALSE)
{
    mContentViewCore = contentViewCore;
    mContext = mContentViewCore->GetContext();
    mDropdownPopupWindow = new DropdownPopupWindow(mContext,
            mContentViewCore->GetViewAndroidDelegate());
    AutoPtr<IAdapterViewOnItemClickListener> clickListener =  new InnerAdapterViewOnItemClickListener(this);
    mDropdownPopupWindow->SetOnItemClickListener(clickListener);
    if (selected->GetLength() > 0) {
        mInitialSelection = (*selected)[0];
    }
    AutoPtr< ArrayOf<DropdownItem> > dropdownItems = items.toArray(new DropdownItem[items.size()]);
    AutoPtr<DropdownAdapter> adapter = new DropdownAdapter(mContext, dropdownItems, NULL);
    mDropdownPopupWindow->SetAdapter(adapter);
    AutoPtr<RenderCoordinates> renderCoordinates = mContentViewCore->GetRenderCoordinates();
    Int32 left;
    bounds->GetLeft(&left);
    Float anchorX = renderCoordinates->FromPixToDip(
            renderCoordinates->FromLocalCssToPix(left));
    Int32 top;
    bounds->GetTop(&top);
    Float anchorY = renderCoordinates->FromPixToDip(
            renderCoordinates->FromLocalCssToPix(top));
    Int32 right;
    bounds->GetRight(&right);
    Float anchorWidth = renderCoordinates->FromPixToDip(
            renderCoordinates->FromLocalCssToPix(right)) - anchorX;
    Int32 bottom;
    bounds->GetBottom(&bottom);
    Float anchorHeight = renderCoordinates->FromPixToDip(
            renderCoordinates->FromLocalCssToPix(bottom)) - anchorY;
    mDropdownPopupWindow->SetAnchorRect(anchorX, anchorY, anchorWidth, anchorHeight);
    AutoPtr<IPopupWindowOnDismissListener> dismissListener = new InnerPopupWindowOnDismissListener(this);
    mDropdownPopupWindow->SetOnDismissListener(dismissListener);
}

//@Override
void SelectPopupDropdown::Show()
{
    mDropdownPopupWindow->Show();
    if (mInitialSelection >= 0) {
        mDropdownPopupWindow->GetListView()->SetSelection(mInitialSelection);
    }
}

//@Override
void SelectPopupDropdown::Hide()
{
    mDropdownPopupWindow->Dismiss();
}

} // namespace Input
} // namespace Browser
} // namespace Content
} // namespace Webkit
} // namespace Droid
} // namespace Elastos
