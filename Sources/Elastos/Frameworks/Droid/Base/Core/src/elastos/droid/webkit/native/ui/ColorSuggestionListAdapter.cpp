
#include "elastos/droid/webkit/native/ui/ColorSuggestionListAdapter.h"
//#include "elastos/droid/widget/CLinearLayout.h"
//#include "elastos/droid/view/CViewGroupLayoutParams.h"
//#include "elastos/droid/widget/CLinearLayoutLayoutParams.h"
#include "elastos/droid/text/TextUtils.h"
#include "elastos/droid/webkit/native/ui/ColorSuggestion.h"
#include "elastos/droid/webkit/native/base/ApiCompatibilityUtils.h"

using Elastos::Core::CString;
using Elastos::Droid::Widget::ILinearLayout;
//using Elastos::Droid::Widget::CLinearLayout;
using Elastos::Droid::View::IViewGroupLayoutParams;
//using Elastos::Droid::View::CViewGroupLayoutParams;
using Elastos::Droid::View::EIID_IViewOnClickListener;
using Elastos::Droid::View::IViewManager;
using Elastos::Droid::View::IViewGroupMarginLayoutParams;
using Elastos::Droid::Graphics::IColor;
using Elastos::Droid::Content::Res::IResources;
using Elastos::Droid::Widget::ILinearLayoutLayoutParams;
//using Elastos::Droid::Widget::CLinearLayoutLayoutParams;
using Elastos::Droid::Text::TextUtils;
using Elastos::Droid::Graphics::Drawable::IDrawable;
using Elastos::Droid::Graphics::Drawable::IGradientDrawable;
using Elastos::Droid::Graphics::Drawable::ILayerDrawable;
using Elastos::Droid::Webkit::Ui::ColorSuggestion;
using Elastos::Droid::Webkit::Base::ApiCompatibilityUtils;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Ui {

//=====================================================================
//                      ColorSuggestionListAdapter
//=====================================================================
const Int32 ColorSuggestionListAdapter::COLORS_PER_ROW;

CAR_INTERFACE_IMPL(ColorSuggestionListAdapter, Object, IViewOnClickListener)
//CAR_INTERFACE_IMPL(ColorSuggestionListAdapter, BaseAdapter, IViewOnClickListener)

ColorSuggestionListAdapter::ColorSuggestionListAdapter(
    /* [in] */ IContext* context,
    /* [in] */ ArrayOf<IInterface*>* suggestions)
    : mContext(context)
    , mSuggestions(suggestions)
{
    // ==================before translated======================
    // mContext = context;
    // mSuggestions = suggestions;
}

ECode ColorSuggestionListAdapter::SetOnColorSuggestionClickListener(
    /* [in] */ OnColorSuggestionClickListener* listener)
{
    VALIDATE_NOT_NULL(listener);
    // ==================before translated======================
    // mListener = listener;

    mListener = listener;
    return NOERROR;
}

ECode ColorSuggestionListAdapter::OnClick(
    /* [in] */ IView* v)
{
    VALIDATE_NOT_NULL(v);
    // ==================before translated======================
    // if (mListener == null) {
    //     return;
    // }
    // ColorSuggestion suggestion = (ColorSuggestion) v.getTag();
    // if (suggestion == null) {
    //     return;
    // }
    // mListener.onColorSuggestionClick(suggestion);

    if (NULL == mListener) {
        return NOERROR;
    }

    AutoPtr<IInterface> interfaceTmp;
    v->GetTag((IInterface**)&interfaceTmp);
    IObject* objectTmp = IObject::Probe(interfaceTmp);
    ColorSuggestion* suggestion = (ColorSuggestion*)objectTmp;
    if (NULL == suggestion) {
        return NOERROR;
    }

    mListener->OnColorSuggestionClick(suggestion);
    return NOERROR;
}

AutoPtr<IView> ColorSuggestionListAdapter::GetView(
    /* [in] */ Int32 position,
    /* [in] */ IView* convertView,
    /* [in] */ IViewGroup* parent)
{
    // ==================before translated======================
    // LinearLayout layout;
    // if (convertView != null && convertView instanceof LinearLayout) {
    //     layout = (LinearLayout) convertView;
    // } else {
    //     layout = new LinearLayout(mContext);
    //     layout.setLayoutParams(new AbsListView.LayoutParams(
    //             AbsListView.LayoutParams.MATCH_PARENT,
    //             AbsListView.LayoutParams.WRAP_CONTENT));
    //     layout.setOrientation(LinearLayout.HORIZONTAL);
    //     layout.setBackgroundColor(Color.WHITE);
    //     int buttonHeight =
    //         mContext.getResources().getDimensionPixelOffset(R.dimen.color_button_height);
    //     for (int i = 0; i < COLORS_PER_ROW; ++i) {
    //         View button = new View(mContext);
    //         LinearLayout.LayoutParams layoutParams =
    //                 new LinearLayout.LayoutParams(0, buttonHeight, 1f);
    //         ApiCompatibilityUtils.setMarginStart(layoutParams, -1);
    //         if (i == COLORS_PER_ROW - 1) {
    //             ApiCompatibilityUtils.setMarginEnd(layoutParams, -1);
    //         }
    //         button.setLayoutParams(layoutParams);
    //         button.setBackgroundResource(R.drawable.color_button_background);
    //         layout.addView(button);
    //     }
    // }
    // for (int i = 0; i < COLORS_PER_ROW; ++i) {
    //     setUpColorButton(layout.getChildAt(i), position * COLORS_PER_ROW + i);
    // }
    // return layout;

    assert(0);
    AutoPtr<ILinearLayout> layout;
    AutoPtr<ILinearLayout> maybelinear = ILinearLayout::Probe(convertView);
    if (NULL != convertView && NULL != maybelinear) {
        layout = maybelinear;
    }
    else {
        //CLinearLayout::New(mContext, (ILinearLayout**)&layout);
        AutoPtr<IViewGroupLayoutParams> params;
        //CViewGroupLayoutParams::New(IViewGroupLayoutParams::MATCH_PARENT, IViewGroupLayoutParams::WRAP_CONTENT, (IViewGroupLayoutParams**)&params);

        //layout->SetLayoutParams(params);
        layout->SetOrientation(ILinearLayout::HORIZONTAL);
        //layout->SetBackgroundColor(IColor::WHITE);

        Int32 buttonHeight = 0;
        AutoPtr<IResources> resource;
        mContext->GetResources((IResources**)&resource);
        resource->GetDimensionPixelOffset(-1/*R::dimen::color_button_height*/, &buttonHeight);

        AutoPtr<IViewManager> viewManager = IViewManager::Probe(layout);

        for (Int32 i = 0; i < COLORS_PER_ROW; ++i) {
            AutoPtr<IView> button;
            //CView::New(mContext, (IView**)&button);

            AutoPtr<ILinearLayoutLayoutParams> layoutParams;
            //CLinearLayoutLayoutParams::New(0, buttonHeight, 1f, (ILinearLayoutLayoutParams**)&layoutParams);

            AutoPtr<IViewGroupMarginLayoutParams> viewGroupParams = IViewGroupMarginLayoutParams::Probe(layoutParams);
            ApiCompatibilityUtils::SetMarginStart(viewGroupParams, -1);
            if (i == COLORS_PER_ROW - 1) {
                ApiCompatibilityUtils::SetMarginEnd(viewGroupParams, -1);
            }
            AutoPtr<IViewGroupLayoutParams> viewGroupLayoutParams = IViewGroupLayoutParams::Probe(layoutParams);
            button->SetLayoutParams(viewGroupLayoutParams);
            button->SetBackgroundResource(-1/*R::drawable::color_button_background*/);
            viewManager->AddView(button, NULL); // is addView(Parm1) or addView(Parm1, Param2)
        }
    }

    AutoPtr<IViewGroup> viewGroup = IViewGroup::Probe(layout);
    for (Int32 i = 0; i < COLORS_PER_ROW; ++i) {
        AutoPtr<IView> child;
        viewGroup->GetChildAt(i, (IView**)&child);
        SetUpColorButton(child, position * COLORS_PER_ROW + i);
    }

    AutoPtr<IView> result = IView::Probe(layout);
    return result;
}

Int64 ColorSuggestionListAdapter::GetItemId(
    /* [in] */ Int32 position)
{
    // ==================before translated======================
    // return position;

    return position;
}

AutoPtr<IInterface> ColorSuggestionListAdapter::GetItem(
    /* [in] */ Int32 position)
{
    // ==================before translated======================
    // return null;

    return NULL;
}

Int32 ColorSuggestionListAdapter::GetCount()
{
    // ==================before translated======================
    // return (mSuggestions.length + COLORS_PER_ROW - 1) / COLORS_PER_ROW;

    return (mSuggestions->GetLength() + COLORS_PER_ROW - 1) / COLORS_PER_ROW;
}

ECode ColorSuggestionListAdapter::SetUpColorButton(
    /* [in] */ IView* button,
    /* [in] */ Int32 index)
{
    VALIDATE_NOT_NULL(button);
    // ==================before translated======================
    // if (index >= mSuggestions.length) {
    //     button.setTag(null);
    //     button.setContentDescription(null);
    //     button.setVisibility(View.INVISIBLE);
    //     return;
    // }
    // button.setTag(mSuggestions[index]);
    // button.setVisibility(View.VISIBLE);
    // ColorSuggestion suggestion = mSuggestions[index];
    // LayerDrawable layers = (LayerDrawable) button.getBackground();
    // GradientDrawable swatch =
    //         (GradientDrawable) layers.findDrawableByLayerId(R.id.color_button_swatch);
    // swatch.setColor(suggestion.mColor);
    // String description = suggestion.mLabel;
    // if (TextUtils.isEmpty(description)) {
    //     description = String.format("#%06X", (0xFFFFFF & suggestion.mColor));
    // }
    // button.setContentDescription(description);
    // button.setOnClickListener(this);

    AutoPtr<IView> viewTmp = IView::Probe(button);
    if (index >= mSuggestions->GetLength()) {
        viewTmp->SetTag(NULL);
        viewTmp->SetContentDescription(NULL);
        button->SetVisibility(IView::INVISIBLE);
        return NOERROR;
    }

    AutoPtr<IInterface> interfaceTmp = (*mSuggestions)[index];
    viewTmp->SetTag(interfaceTmp);
    viewTmp->SetVisibility(IView::VISIBLE);

    AutoPtr<ColorSuggestion> suggestion = (ColorSuggestion*)(IObject::Probe((*mSuggestions)[index]));
    AutoPtr<IDrawable> layersTmp;
    viewTmp->GetBackground((IDrawable**)&layersTmp);
    AutoPtr<ILayerDrawable> layers = ILayerDrawable::Probe(layersTmp);

    AutoPtr<IDrawable> swatchTmp;
    layers->FindDrawableByLayerId(-1/*R::id::color_button_swatch*/, (IDrawable**)&swatchTmp);
    AutoPtr<IGradientDrawable> swatch = IGradientDrawable::Probe(swatchTmp);

    swatch->SetColor(suggestion->mColor);
    String description = suggestion->mLabel;
    if (TextUtils::IsEmpty(description)) {
        description.AppendFormat("#%06X", (0xFFFFFF & suggestion->mColor));
    }

    AutoPtr<ICharSequence> charSequence;
    CString::New(description, (ICharSequence**)&charSequence);
    viewTmp->SetContentDescription(charSequence);
    viewTmp->SetOnClickListener(this);
    return NOERROR;
}

} // namespace Ui
} // namespace Webkit
} // namespace Droid
} // namespace Elastos


