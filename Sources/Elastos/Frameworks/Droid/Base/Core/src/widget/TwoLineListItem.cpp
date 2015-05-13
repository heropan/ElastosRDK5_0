
#include "widget/TwoLineListItem.h"

using Elastos::Core::CStringWrapper;

namespace Elastos {
namespace Droid {
namespace Widget {

TwoLineListItem::TwoLineListItem()
{

}

TwoLineListItem::TwoLineListItem(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    Init(context, attrs, defStyle);
}

ECode TwoLineListItem::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    RelativeLayout::Init(context, attrs, defStyle);

    AutoPtr<ArrayOf<Int32> > attrIds = ArrayOf<Int32>::Alloc(
            const_cast<Int32 *>(R::styleable::TwoLineListItem),
            ARRAY_SIZE(R::styleable::TwoLineListItem));
    AutoPtr<ITypedArray> a;
    FAIL_RETURN(context->ObtainStyledAttributesEx3(attrs, attrIds, defStyle, 0, (ITypedArray**)&a));

    a->Recycle();

    return NOERROR;
}

ECode TwoLineListItem::OnFinishInflate()
{
    RelativeLayout::OnFinishInflate();
    mText1 = ITextView::Probe(FindViewById(R::id::text1));
    mText2 = ITextView::Probe(FindViewById(R::id::text2));
    return NOERROR;
}

AutoPtr<ITextView> TwoLineListItem::GetText1()
{
    return mText1;
}

AutoPtr<ITextView> TwoLineListItem::GetText2()
{
    return mText2;
}

ECode TwoLineListItem::OnInitializeAccessibilityEvent(
    /* [in] */ IAccessibilityEvent* event)
{
    RelativeLayout::OnInitializeAccessibilityEvent(event);
    AutoPtr<ICharSequence> seq;
    FAIL_RETURN(CStringWrapper::New(String("CTwoLineListItem"), (ICharSequence**)&seq));
    event->SetClassName(seq);
    return NOERROR;
}

ECode TwoLineListItem::OnInitializeAccessibilityNodeInfo(
    /* [in] */ IAccessibilityNodeInfo* info)
{
    RelativeLayout::OnInitializeAccessibilityNodeInfo(info);
    AutoPtr<ICharSequence> seq;
    FAIL_RETURN(CStringWrapper::New(String("CTwoLineListItem"), (ICharSequence**)&seq));
    info->SetClassName(seq);
    return NOERROR;
}

} // namespace Widget
} // namespace Droid
} // namespace Elastos
