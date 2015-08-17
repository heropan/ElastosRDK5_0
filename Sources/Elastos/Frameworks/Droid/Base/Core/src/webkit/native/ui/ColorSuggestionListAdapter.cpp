
namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Ui {

const Int32 ColorSuggestionListAdapter::COLORS_PER_ROW;

ColorSuggestionListAdapter::ColorSuggestionListAdapter(
    /* in */ IContext* context,
    /* in */ ArrayOf< AutoPtr< ColorSuggestion > >* suggestions)
    : mContext(context)
    , mSuggestions(suggestions)
    , mListener(NULL)
{
}

ECode ColorSuggestionListAdapter::SetOnColorSuggestionClickListener(
    /* in */ OnColorSuggestionClickListener* listener)
{
    VALIDATE_NOT_NULL(listener);
    mListener = listener;
    return NOERROR;
}

//@Override
ECode ColorSuggestionListAdapter::OnClick(
    /* in */ IView* v)
{
    VALIDATE_NOT_NULL(v);
    if (NULL == mListener) {
        return NOERROR;
    }
    AutoPtr<ColorSuggestion> suggestion = (ColorSuggestion*) v->GetTag();
    if (NULL == suggestion) {
        return NOERROR;
    }
    mListener->OnColorSuggestionClick(suggestion);
    return NOERROR;
}

//@Override
AutoPtr<IView> ColorSuggestionListAdapter::GetView(
    /* in */ Int32 position,
    /* in */ IView* convertView,
    /* in */ IViewGroup* parent)
{
    AutoPtr<ILinearLayout> layout;
    AutoPtr<ILinearLayout> maybelinear = (ILinearLayout*)convertView->Probe(EIID_ILinearLayout);
    if (NULL != convertView && NULL != maybelinear) {
        layout = maybelinear;
    }
    else {
        CLinearLayout::New(mContext, (ILinearLayout**)&layout);
        AutoPtr<IViewGroupLayoutParams> params;
        CAbsListView::New(AbsListView::LayoutParams::MATCH_PARENT, AbsListView::LayoutParams::WRAP_CONTENT, (IViewGroupLayoutParams**)&params);
        layout->SetLayoutParams(params);
        layout->SetOrientation(LinearLayout::HORIZONTAL);
        layout->SetBackgroundColor(IColor::WHITE);

        Int32 buttonHeight;
        AutoPtr<IResources> resource;
        CResources::New((IResources**)&resource);
        mContext->GetResources(&resource);
        resource->GetDimensionPixelOffset(R::dimen::color_button_height, &buttonHeight);

        for (Int32 i = 0; i < COLORS_PER_ROW; ++i) {
            AutoPtr<IView> button;
            CView::New(mContext, (IView**)&button);

            AutoPtr<ILayoutParams> layoutParams;
            CLinearLayout::LayoutParams::New(0, buttonHeight, 1f, (ILayoutParams**)&layoutParams);

            CApiCompatibilityUtils::SetMarginStart(layoutParams, -1);
            if (i == COLORS_PER_ROW - 1) {
                CApiCompatibilityUtils::etMarginEnd(layoutParams, -1);
            }
            button->SetLayoutParams(layoutParams);
            button->SetBackgroundResource(R::drawable::color_button_background);
            layout->AddView(button);
        }
    }
    for (Int32 i = 0; i < COLORS_PER_ROW; ++i) {
        AutoPtr<IView> child;
        layout->GetChildAt(i, (IView**)&child);
        SetUpColorButton(child, position * COLORS_PER_ROW + i);
    }
    return layout;
}

//@Override
Int64 ColorSuggestionListAdapter::GetItemId(
    /* in */ Int32 position)
{
    return position;
}

//@Override
AutoPtr<Object> ColorSuggestionListAdapter::GetItem(
    /* in */ Int32 position)
{
    return NULL;
}

//@Override
Int32 ColorSuggestionListAdapter::GetCount()
{
    return (mSuggestions->GetLength() + COLORS_PER_ROW - 1) / COLORS_PER_ROW;
}

ECode ColorSuggestionListAdapter::SetUpColorButton(
    /* in */ IView* button,
    /* in */ Int32 index)
{
    VALIDATE_NOT_NULL(button);
    if (index >= mSuggestions->GetLength()) {
        button->SetTag(NULL);
        button->SetContentDescription(NULL);
        button->SetVisibility(IView::INVISIBLE);
        return NOERROR;
    }
    button->SetTag((*mSuggestions)[index]);
    button->SetVisibility(IView::VISIBLE);
    AutoPtr<ColorSuggestion> suggestion = (*mSuggestions)[index];
    AutoPtr<ILayerDrawable> layers;
    button->GetBackground((ILayerDrawable**)&layers);

    AutoPtr<IGradientDrawable> swatch;
    layers->FindDrawableByLayerId(R::id::color_button_swatch, (IGradientDrawable**)&swatch);
    swatch->SetColor(suggestion->mColor);
    String description = suggestion->mLabel;
    if (CTextUtils::IsEmpty(description)) {
        description = String::Format(String("#%06X"), (0xFFFFFF & suggestion->mColor));
    }
    button->SetContentDescription(description);
    button->SetOnClickListener(this);
    return NOERROR;
}

} // namespace Ui
} // namespace Webkit
} // namespace Droid
} // namespace Elastos
