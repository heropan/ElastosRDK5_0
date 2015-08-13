
namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Ui {

AutoPtr<IContext> DropdownAdapter::mContext;
AutoPtr< ISet<IInteger32> > DropdownAdapter::mSeparators;
Boolean DropdownAdapter::mAreAllItemsEnabled;

DropdownAdapter::DropdownAdapter(
    /* in */ IContext* context,
    /* in */ IList< AutoPtr<DropdownItem> >* items,
    /* in */ ISet<IInteger32>* separators)
    : ArrayAdapter<DropdownItem*>(context, R::layout::dropdown_item, items)
    , mSeparators(separators)
    , mContext(context)
    , mAreAllItemsEnabled(FALSE)
{
    mAreAllItemsEnabled = CheckAreAllItemsEnabled();
}

DropdownAdapter::DropdownAdapter(
    /* in */ IContext* context,
    /* in */ ArrayOf< AutoPtr<DropdownItem> >* items,
    /* in */ ISet<IInteger32>* separators)
    : ArrayAdapter<DropdownItem*>(context, R::layout::dropdown_item, items)
    , mSeparators(separators)
    , mContext(context)
{
    mAreAllItemsEnabled = CheckAreAllItemsEnabled();
}

//@Override
AutoPtr<IView> DropdownAdapter::GetView(
    /* in */ Int32 position,
    /* in */ IView* convertView,
    /* in */ IViewGroup* parent)
{
    AutoPtr<IView> layout = convertView;
    if (NULL == convertView) {
        AutoPtr<ILayoutInflater> inflater;
		mContext->GetSystemService(IContext::LAYOUT_INFLATER_SERVICE, (ILayoutInflater**)&inflater);
        inflater->Inflate(R::layout::dropdown_item, NULL, (IView**)&layout);
        AutoPtr<DropdownDividerDrawable> drawable = new DropdownDividerDrawable();
        ApiCompatibilityUtils::SetBackgroundForView(layout, drawable);
    }

    AutoPtr<DropdownItem> item = GetItem(position);
    AutoPtr<ITextView> labelView;
    layout->FindViewById(R::id::dropdown_label, (ITextView**)&labelView);
    labelView->SetText(item->GetLabel());
    labelView->SetEnabled(item->IsEnabled());
    if (item->IsGroupHeader()) {
        labelView->SetTypeface(NULL, ITypeface::BOLD);
    }
    else {
        labelView->SetTypeface(NULL, ITypeface::NORMAL);
    }

    AutoPtr<DropdownDividerDrawable> divider = layout->GetBackground();
    Int32 height = 0;
    AutoPtr<IResources> resources;
    CResources::New((IResources**)&resources);
    mContext->GetResources((IResources**)&resources);
    resources->GetDimensionPixelSize(R::dimen::dropdown_item_height, &height);
    if (position == 0) {
        divider->SetColor(IColor::TRANSPARENT);
    }
    else {
        Int32 dividerHeight;
        AutoPtr<IResources> resources;
    	CResources::New((IResources**)&resources);
    	mContext->GetResources((IResources**)&resources);
    	resources->GetDimensionPixelSize(R::dimen::dropdown_item_divider_height, &dividerHeight);
        height += dividerHeight;
        divider->SetHeight(dividerHeight);

        Boolean contain = FALSE;
        mSeparators->Contains(position, &contain);

		Int32 color;
		AutoPtr<IResources> res;
		CResources::New((IResources**)&res);
		mContext->GetResources((IResources**)&res);
        if (NULL != mSeparators && contain) {
	    	res->GetColor(R::color::dropdown_dark_divider_color, &color);
            divider->SetColor(color);
        }
        else {
            res->GetColor(R::color::dropdown_divider_color, &color);
            divider->SetColor(color);
        }
    }

    AutoPtr<ILayoutParams> params;
    CLayoutParams::New(ILayoutParams::MATCH_PARENT, height, (ILayoutParams**)&params);
    layout->SetLayoutParams(params);

    AutoPtr<ITextView> sublabelView;
    layout->FindViewById(R::id::dropdown_sublabel, &sublabelView);
    AutoPtr<ICharSequence> sublabel = item->GetSublabel();
    if (ITextUtils:IsEmpty(sublabel)) {
        sublabelView->SetVisibility(IView::GONE);
    }
    else {
        sublabelView->SetText(sublabel);
        sublabelView->SetVisibility(IView::VISIBLE);
    }

    return layout;
}

//@Override
Boolean DropdownAdapter::AreAllItemsEnabled()
{
    return mAreAllItemsEnabled;
}

//@Override
Boolean DropdownAdapter::IsEnabled(
    /* in */ Int32 position)
{
    if (position < 0 || position >= GetCount())
    	return FALSE;
    AutoPtr<DropdownItem> item = GetItem(position);
    return item->IsEnabled() && !item->IsGroupHeader();
}

Boolean DropdownAdapter::CheckAreAllItemsEnabled()
{
    for (Int32 i = 0; i < GetCount(); ++i) {
        AutoPtr<DropdownItem> item = GetItem(i);
        if (item->IsEnabled() && !item->IsGroupHeader()) {
            return FALSE;
        }
    }
    return TRUE;
}

} // namespace Ui
} // namespace Webkit
} // namespace Droid
} // namespace Elastos
