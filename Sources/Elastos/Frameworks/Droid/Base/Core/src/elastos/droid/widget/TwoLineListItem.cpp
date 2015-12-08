
#include "elastos/droid/widget/TwoLineListItem.h"
#include "elastos/droid/R.h"

using Elastos::Droid::R;
using Elastos::Droid::View::Accessibility::IAccessibilityRecord;
using Elastos::Core::CString;

namespace Elastos {
namespace Droid {
namespace Widget {

CAR_INTERFACE_IMPL(TwoLineListItem, RelativeLayout, ITwoLineListItem);
ECode TwoLineListItem::constructor(
    /* [in] */ IContext* context)
{
    return constructor(context, NULL, 0);
}

ECode TwoLineListItem::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return constructor(context, attrs, 0);
}

ECode TwoLineListItem::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyleAttr)
{
    return constructor(context, attrs, defStyleAttr, 0);
}

ECode TwoLineListItem::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyleAttr,
    /* [in] */ Int32 defStyleRes)
{
    RelativeLayout::constructor(context, attrs, defStyleAttr, defStyleRes);

    AutoPtr<ArrayOf<Int32> > attrIds = ArrayOf<Int32>::Alloc(
            const_cast<Int32 *>(R::styleable::TwoLineListItem),
            ARRAY_SIZE(R::styleable::TwoLineListItem));
    AutoPtr<ITypedArray> a;
    FAIL_RETURN(context->ObtainStyledAttributes(attrs, attrIds, defStyleAttr, defStyleRes, (ITypedArray**)&a));

    a->Recycle();

    return NOERROR;
}

ECode TwoLineListItem::OnFinishInflate()
{
    RelativeLayout::OnFinishInflate();
    AutoPtr<IView> view;
    FindViewById(R::id::text1, (IView**)&view);
    mText1 = ITextView::Probe(view);
    view = NULL;
    FindViewById(R::id::text2, (IView**)&view);
    mText2 = ITextView::Probe(view);
    return NOERROR;
}

ECode TwoLineListItem::GetText1(
    /* [out] */ ITextView** textView)
{
    VALIDATE_NOT_NULL(textView);
    *textView = mText1;
    REFCOUNT_ADD(*textView);
    return NOERROR;
}

ECode TwoLineListItem::GetText2(
    /* [out] */ ITextView** textView)
{
    VALIDATE_NOT_NULL(textView);
    *textView = mText2;
    REFCOUNT_ADD(*textView);
    return NOERROR;
}

ECode TwoLineListItem::OnInitializeAccessibilityEvent(
    /* [in] */ IAccessibilityEvent* event)
{
    RelativeLayout::OnInitializeAccessibilityEvent(event);
    AutoPtr<ICharSequence> seq;
    FAIL_RETURN(CString::New(String("CTwoLineListItem"), (ICharSequence**)&seq));
    IAccessibilityRecord::Probe(event)->SetClassName(seq);
    return NOERROR;
}

ECode TwoLineListItem::OnInitializeAccessibilityNodeInfo(
    /* [in] */ IAccessibilityNodeInfo* info)
{
    RelativeLayout::OnInitializeAccessibilityNodeInfo(info);
    AutoPtr<ICharSequence> seq;
    FAIL_RETURN(CString::New(String("CTwoLineListItem"), (ICharSequence**)&seq));
    info->SetClassName(seq);
    return NOERROR;
}

} // namespace Widget
} // namespace Droid
} // namespace Elastos
