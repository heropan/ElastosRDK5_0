
#include "elastos/droid/text/TextUtils.h"
#include "elastos/droid/webkit/native/base/ApiCompatibilityUtils.h"
#include "elastos/droid/webkit/native/ui/DropdownAdapter.h"
#include "elastos/droid/webkit/native/ui/DropdownDividerDrawable.h"
#include "elastos/droid/webkit/native/ui/DropdownItem.h"

using Elastos::Droid::Content::Res::IResources;
using Elastos::Droid::Graphics::Drawable::IDrawable;
using Elastos::Droid::Graphics::IColor;
using Elastos::Droid::Graphics::ITypeface;
using Elastos::Droid::Text::TextUtils;
using Elastos::Droid::View::ILayoutInflater;
using Elastos::Droid::View::IViewGroupLayoutParams;
using Elastos::Droid::Webkit::Base::ApiCompatibilityUtils;
using Elastos::Droid::Webkit::Ui::DropdownDividerDrawable;
using Elastos::Droid::Webkit::Ui::DropdownItem;
using Elastos::Core::CInteger32;
using Elastos::Core::CString;
using Elastos::Core::ICharSequence;
using Elastos::Core::IInteger32;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Ui {

//=====================================================================
//                           DropdownAdapter
//=====================================================================
DropdownAdapter::DropdownAdapter(
    /* [in] */ IContext* context,
    /* [in] */ IList* items,
    /* [in] */ ISet* separators)
    //: ArrayAdapter<DropdownItem*>(context, -1/*R::layout::dropdown_item*/, items)
    : mContext(context)
    , mSeparators(separators)
    , mAreAllItemsEnabled(FALSE)
{
    // ==================before translated======================
    // super(context, R.layout.dropdown_item, items);
    // mSeparators = separators;
    // mContext = context;
    // mAreAllItemsEnabled = checkAreAllItemsEnabled();

    assert(0);
    //mAreAllItemsEnabled = CheckAreAllItemsEnabled();
}

DropdownAdapter::DropdownAdapter(
    /* [in] */ IContext* context,
    /* [in] */ ArrayOf<DropdownItem*>* items,
    /* [in] */ ISet* separators)
    //: ArrayAdapter<DropdownItem*>(context, -1/*R::layout::dropdown_item*/, items)
    : mContext(context)
    , mSeparators(separators)
    , mAreAllItemsEnabled(FALSE)
{
    // ==================before translated======================
    // super(context, R.layout.dropdown_item, items);
    // mSeparators = separators;
    // mContext = context;
    // mAreAllItemsEnabled = checkAreAllItemsEnabled();

    assert(0);
    //mAreAllItemsEnabled = checkAreAllItemsEnabled();
}

AutoPtr<IView> DropdownAdapter::GetView(
    /* [in] */ Int32 position,
    /* [in] */ IView* convertView,
    /* [in] */ IViewGroup* parent)
{
    // ==================before translated======================
    // View layout = convertView;
    // if (convertView == null) {
    //     LayoutInflater inflater =
    //             (LayoutInflater) mContext.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
    //     layout = inflater.inflate(R.layout.dropdown_item, null);
    //     ApiCompatibilityUtils.setBackgroundForView(layout, new DropdownDividerDrawable());
    // }
    //
    // DropdownItem item = getItem(position);
    //
    // TextView labelView = (TextView) layout.findViewById(R.id.dropdown_label);
    // labelView.setText(item.getLabel());
    //
    // labelView.setEnabled(item.isEnabled());
    // if (item.isGroupHeader()) {
    //     labelView.setTypeface(null, Typeface.BOLD);
    // } else {
    //     labelView.setTypeface(null, Typeface.NORMAL);
    // }
    //
    // DropdownDividerDrawable divider = (DropdownDividerDrawable) layout.getBackground();
    // int height = mContext.getResources().getDimensionPixelSize(R.dimen.dropdown_item_height);
    // if (position == 0) {
    //     divider.setColor(Color.TRANSPARENT);
    // } else {
    //     int dividerHeight = mContext.getResources().getDimensionPixelSize(
    //             R.dimen.dropdown_item_divider_height);
    //     height += dividerHeight;
    //     divider.setHeight(dividerHeight);
    //     if (mSeparators != null && mSeparators.contains(position)) {
    //         divider.setColor(mContext.getResources().getColor(
    //                          R.color.dropdown_dark_divider_color));
    //     } else {
    //         divider.setColor(mContext.getResources().getColor(
    //                          R.color.dropdown_divider_color));
    //     }
    // }
    // layout.setLayoutParams(new LayoutParams(LayoutParams.MATCH_PARENT, height));
    //
    // TextView sublabelView = (TextView) layout.findViewById(R.id.dropdown_sublabel);
    // CharSequence sublabel = item.getSublabel();
    // if (TextUtils.isEmpty(sublabel)) {
    //     sublabelView.setVisibility(View.GONE);
    // } else {
    //     sublabelView.setText(sublabel);
    //     sublabelView.setVisibility(View.VISIBLE);
    // }
    //
    // return layout;

    assert(0);
    AutoPtr<IView> layout = convertView;
    if (NULL == convertView) {
        AutoPtr<IInterface> interfaceTmp;
        mContext->GetSystemService(IContext::LAYOUT_INFLATER_SERVICE, (IInterface**)&interfaceTmp);
        ILayoutInflater* inflater = ILayoutInflater::Probe(interfaceTmp);

        inflater->Inflate(-1/*R::layout::dropdown_item*/, NULL, (IView**)&layout);
        AutoPtr<DropdownDividerDrawable> drawable = new DropdownDividerDrawable();
        AutoPtr<IDrawable> drawableTmp = (IDrawable*)drawable;
        ApiCompatibilityUtils::SetBackgroundForView(layout, drawableTmp);
    }

    AutoPtr<DropdownItem> item;// = GetItem(position);
    AutoPtr<IView> viewTmp;
    layout->FindViewById(-1/*R::id::dropdown_label*/, (IView**)&viewTmp);
    ITextView* labelView = ITextView::Probe(viewTmp);

    String label = item->GetLabel();
    AutoPtr<ICharSequence> charSequence;
    CString::New(label, (ICharSequence**)&charSequence);
    labelView->SetText(charSequence);
    viewTmp->SetEnabled(item->IsEnabled());
    if (item->IsGroupHeader()) {
        labelView->SetTypeface(NULL, ITypeface::BOLD);
    }
    else {
        labelView->SetTypeface(NULL, ITypeface::NORMAL);
    }

    AutoPtr<IDrawable> drawableTmp;
    layout->GetBackground((IDrawable**)&drawableTmp);
    IObject* objectTmp = IObject::Probe(drawableTmp);
    AutoPtr<DropdownDividerDrawable> divider = (DropdownDividerDrawable*)objectTmp;
    Int32 height = 0;
    AutoPtr<IResources> resources;
    mContext->GetResources((IResources**)&resources);
    resources->GetDimensionPixelSize(-1/*R::dimen::dropdown_item_height*/, &height);
    if (position == 0) {
        divider->SetColor(IColor::TRANSPARENT);
    }
    else {
        Int32 dividerHeight = 0;
        AutoPtr<IResources> resources;
        mContext->GetResources((IResources**)&resources);
        resources->GetDimensionPixelSize(-1/*R::dimen::dropdown_item_divider_height*/, &dividerHeight);
        height += dividerHeight;
        divider->SetHeight(dividerHeight);

        AutoPtr<IInteger32> positionTmp;
        CInteger32::New(position, (IInteger32**)&positionTmp);
        Boolean contain = FALSE;

        AutoPtr<IInterface> interfaceTmp = (IInterface*)positionTmp;
        mSeparators->Contains(interfaceTmp, &contain);

        Int32 color = 0;
        AutoPtr<IResources> res;
        mContext->GetResources((IResources**)&res);
        if (NULL != mSeparators && contain) {
            res->GetColor(-1/*R::color::dropdown_dark_divider_color*/, &color);
            divider->SetColor(color);
        }
        else {
            res->GetColor(-1/*R::color::dropdown_divider_color*/, &color);
            divider->SetColor(color);
        }
    }

    //AutoPtr<ILayoutParams> params;
    //CLayoutParams::New(ILayoutParams::MATCH_PARENT, height, (ILayoutParams**)&params);
    IViewGroupLayoutParams* layoutParamsTmp;// = IViewGroupLayoutParams::Probe(params);
    layout->SetLayoutParams(layoutParamsTmp);

    AutoPtr<IView> viewTmp1;
    layout->FindViewById(-1/*R::id::dropdown_sublabel*/, (IView**)&viewTmp1);
    ITextView* sublabelView = ITextView::Probe(viewTmp1);

    String subLabelTmp = item->GetSublabel();
    AutoPtr<ICharSequence> sublabel;
    CString::New(subLabelTmp, (ICharSequence**)&sublabel);
    if (TextUtils::IsEmpty(sublabel)) {
        viewTmp1->SetVisibility(IView::GONE);
    }
    else {
        sublabelView->SetText(sublabel);
        viewTmp1->SetVisibility(IView::VISIBLE);
    }

    return layout;
}

Boolean DropdownAdapter::AreAllItemsEnabled()
{
    // ==================before translated======================
    // return mAreAllItemsEnabled;

    return mAreAllItemsEnabled;
}

Boolean DropdownAdapter::IsEnabled(
    /* [in] */ Int32 position)
{
    // ==================before translated======================
    // if (position < 0 || position >= getCount()) return false;
    // DropdownItem item = getItem(position);
    // return item.isEnabled() && !item.isGroupHeader();

    assert(0);
    if (position < 0 || position >= -1/*GetCount()*/)
        return FALSE;
    AutoPtr<DropdownItem> item;// = GetItem(position);
    return item->IsEnabled() && !item->IsGroupHeader();
}

Boolean DropdownAdapter::CheckAreAllItemsEnabled()
{
    // ==================before translated======================
    // for (int i = 0; i < getCount(); i++) {
    //     DropdownItem item = getItem(i);
    //     if (item.isEnabled() && !item.isGroupHeader()) {
    //         return false;
    //     }
    // }
    // return true;

    assert(0);
    for (Int32 i = 0; i < -1/*GetCount()*/; ++i) {
        AutoPtr<DropdownItem> item;// = GetItem(i);
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


