#include "ext/frameworkext.h"
#include "widget/RelativeLayoutLayoutParams.h"
#include <R.h>

using Elastos::Droid::View::IView;

namespace Elastos {
namespace Droid {
namespace Widget {

RelativeLayoutLayoutParams::RelativeLayoutLayoutParams()
    : mLeft(0)
    , mTop(0)
    , mRight(0)
    , mBottom(0)
    , mStart(IViewGroupMarginLayoutParams::DEFAULT_RELATIVE)
    , mEnd(IViewGroupMarginLayoutParams::DEFAULT_RELATIVE)
    , mRulesChanged(0)
    , mAlignWithParent(FALSE)
{
    mRules = ArrayOf<Int32>::Alloc(IRelativeLayout::VERB_COUNT);
    mInitialRules = ArrayOf<Int32>::Alloc(IRelativeLayout::VERB_COUNT);
}

RelativeLayoutLayoutParams::RelativeLayoutLayoutParams(
    /* [in] */ IContext* c,
    /* [in] */ IAttributeSet* attrs)
    : ViewGroupMarginLayoutParams(c, attrs)
    , mLeft(0)
    , mTop(0)
    , mRight(0)
    , mBottom(0)
    , mStart(IViewGroupMarginLayoutParams::DEFAULT_RELATIVE)
    , mEnd(IViewGroupMarginLayoutParams::DEFAULT_RELATIVE)
    , mRulesChanged(0)
    , mAlignWithParent(FALSE)
{
    mRules = ArrayOf<Int32>::Alloc(IRelativeLayout::VERB_COUNT);
    mInitialRules = ArrayOf<Int32>::Alloc(IRelativeLayout::VERB_COUNT);
    ASSERT_SUCCEEDED(InitFromAttributes(c, attrs));
}

RelativeLayoutLayoutParams::RelativeLayoutLayoutParams(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
    : ViewGroupMarginLayoutParams(width, height)
    , mLeft(0)
    , mTop(0)
    , mRight(0)
    , mBottom(0)
    , mStart(IViewGroupMarginLayoutParams::DEFAULT_RELATIVE)
    , mEnd(IViewGroupMarginLayoutParams::DEFAULT_RELATIVE)
    , mRulesChanged(0)
    , mAlignWithParent(FALSE)
{
    mRules = ArrayOf<Int32>::Alloc(IRelativeLayout::VERB_COUNT);
    mInitialRules = ArrayOf<Int32>::Alloc(IRelativeLayout::VERB_COUNT);
}

RelativeLayoutLayoutParams::RelativeLayoutLayoutParams(
    /* [in] */ ViewGroupLayoutParams* source)
    : ViewGroupMarginLayoutParams(source)
    , mLeft(0)
    , mTop(0)
    , mRight(0)
    , mBottom(0)
    , mStart(IViewGroupMarginLayoutParams::DEFAULT_RELATIVE)
    , mEnd(IViewGroupMarginLayoutParams::DEFAULT_RELATIVE)
    , mRulesChanged(0)
    , mAlignWithParent(FALSE)
{
    mRules = ArrayOf<Int32>::Alloc(IRelativeLayout::VERB_COUNT);
    mInitialRules = ArrayOf<Int32>::Alloc(IRelativeLayout::VERB_COUNT);
}

RelativeLayoutLayoutParams::RelativeLayoutLayoutParams(
    /* [in] */ ViewGroupMarginLayoutParams* source)
    : ViewGroupMarginLayoutParams(source)
    , mLeft(0)
    , mTop(0)
    , mRight(0)
    , mBottom(0)
    , mStart(IViewGroupMarginLayoutParams::DEFAULT_RELATIVE)
    , mEnd(IViewGroupMarginLayoutParams::DEFAULT_RELATIVE)
    , mRulesChanged(0)
    , mAlignWithParent(FALSE)
{
    mRules = ArrayOf<Int32>::Alloc(IRelativeLayout::VERB_COUNT);
    mInitialRules = ArrayOf<Int32>::Alloc(IRelativeLayout::VERB_COUNT);
}

ECode RelativeLayoutLayoutParams::Init(
    /* [in] */ IContext* c,
    /* [in] */ IAttributeSet* attrs)
{
    ASSERT_SUCCEEDED(ViewGroupMarginLayoutParams::Init(c, attrs));
    ASSERT_SUCCEEDED(InitFromAttributes(c, attrs));
    return NOERROR;
}

ECode RelativeLayoutLayoutParams::Init(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    ASSERT_SUCCEEDED(ViewGroupMarginLayoutParams::Init(width, height));
    return NOERROR;
}

ECode RelativeLayoutLayoutParams::Init(
    /* [in] */ IViewGroupLayoutParams* source)
{
    ASSERT_SUCCEEDED(ViewGroupMarginLayoutParams::Init(source));
    return NOERROR;
}

ECode RelativeLayoutLayoutParams::Init(
    /* [in] */ IViewGroupMarginLayoutParams* source)
{
    ASSERT_SUCCEEDED(ViewGroupMarginLayoutParams::Init(source));
    return NOERROR;
}

ECode RelativeLayoutLayoutParams::InitFromAttributes(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    AutoPtr<ArrayOf<Int32> > attrIds = ArrayOf<Int32>::Alloc(
            const_cast<Int32 *>(R::styleable::RelativeLayout_Layout),
            ARRAY_SIZE(R::styleable::RelativeLayout_Layout));
    AutoPtr<ITypedArray> a;
    ASSERT_SUCCEEDED(context->ObtainStyledAttributesEx2(
            attrs, attrIds, (ITypedArray**)&a));

    Int32* rules = mRules->GetPayload();
    Int32* initialRules = mInitialRules->GetPayload();

    Int32 N;
    a->GetIndexCount(&N);
    for (Int32 i = 0; i < N; i++) {
        Int32 attr;
        a->GetIndex(i, &attr);
        Boolean val = FALSE;
        switch (attr) {
            case R::styleable::RelativeLayout_Layout_layout_alignWithParentIfMissing:
                a->GetBoolean(attr, FALSE, &mAlignWithParent);
                break;
            case R::styleable::RelativeLayout_Layout_layout_toLeftOf:
                a->GetResourceId(attr, 0, rules + IRelativeLayout::LEFT_OF);
                break;
            case R::styleable::RelativeLayout_Layout_layout_toRightOf:
                a->GetResourceId(attr, 0, rules + IRelativeLayout::RIGHT_OF);
                break;
            case R::styleable::RelativeLayout_Layout_layout_above:
                a->GetResourceId(attr, 0, rules + IRelativeLayout::ABOVE);
                break;
            case R::styleable::RelativeLayout_Layout_layout_below:
                a->GetResourceId(attr, 0, rules + IRelativeLayout::BELOW);
                break;
            case R::styleable::RelativeLayout_Layout_layout_alignBaseline:
                a->GetResourceId(attr, 0, rules + IRelativeLayout::ALIGN_BASELINE);
                break;
            case R::styleable::RelativeLayout_Layout_layout_alignLeft:
                a->GetResourceId(attr, 0, rules + IRelativeLayout::ALIGN_LEFT);
                break;
            case R::styleable::RelativeLayout_Layout_layout_alignTop:
                a->GetResourceId(attr, 0, rules + IRelativeLayout::ALIGN_TOP);
                break;
            case R::styleable::RelativeLayout_Layout_layout_alignRight:
                a->GetResourceId(attr, 0, rules + IRelativeLayout::ALIGN_RIGHT);
                break;
            case R::styleable::RelativeLayout_Layout_layout_alignBottom:
                a->GetResourceId(attr, 0, rules + IRelativeLayout::ALIGN_BOTTOM);
                break;
            case R::styleable::RelativeLayout_Layout_layout_alignParentLeft:
                a->GetBoolean(attr, FALSE, &val);
                *(rules + IRelativeLayout::ALIGN_PARENT_LEFT) = val ? 1 : 0;
                break;
            case R::styleable::RelativeLayout_Layout_layout_alignParentTop:
                a->GetBoolean(attr, FALSE, &val);
                *(rules + IRelativeLayout::ALIGN_PARENT_TOP) = val ? 1 : 0;
                break;
            case R::styleable::RelativeLayout_Layout_layout_alignParentRight:
                a->GetBoolean(attr, FALSE, &val);
                *(rules + IRelativeLayout::ALIGN_PARENT_RIGHT) = val ? 1 : 0;
                break;
            case R::styleable::RelativeLayout_Layout_layout_alignParentBottom:
                a->GetBoolean(attr, FALSE, &val);
                *(rules + IRelativeLayout::ALIGN_PARENT_BOTTOM) = val ? 1 : 0;
                break;
            case R::styleable::RelativeLayout_Layout_layout_centerInParent:
                a->GetBoolean(attr, FALSE, &val);
                *(rules + IRelativeLayout::CENTER_IN_PARENT) = val ? 1 : 0;
                break;
            case R::styleable::RelativeLayout_Layout_layout_centerHorizontal:
                a->GetBoolean(attr, FALSE, &val);
                *(rules + IRelativeLayout::CENTER_HORIZONTAL) = val ? 1 : 0;
                break;
            case R::styleable::RelativeLayout_Layout_layout_centerVertical:
                a->GetBoolean(attr, FALSE, &val);
                *(rules + IRelativeLayout::CENTER_VERTICAL) = val ? 1 : 0;
                break;
            case R::styleable::RelativeLayout_Layout_layout_toStartOf:
                a->GetResourceId(attr, 0, rules + IRelativeLayout::START_OF);
                break;
            case R::styleable::RelativeLayout_Layout_layout_toEndOf:
                a->GetResourceId(attr, 0, rules + IRelativeLayout::END_OF);
                break;
            case R::styleable::RelativeLayout_Layout_layout_alignStart:
                a->GetResourceId(attr, 0, rules + IRelativeLayout::END_OF);
                break;
            case R::styleable::RelativeLayout_Layout_layout_alignEnd:
                a->GetResourceId(attr, 0, rules + IRelativeLayout::ALIGN_END);
                break;
            case R::styleable::RelativeLayout_Layout_layout_alignParentStart:
                a->GetBoolean(attr, FALSE, &val);
                *(rules + IRelativeLayout::ALIGN_PARENT_START) = val ? 1 : 0;
                break;
            case R::styleable::RelativeLayout_Layout_layout_alignParentEnd:
                a->GetBoolean(attr, FALSE, &val);
                *(rules + IRelativeLayout::ALIGN_PARENT_END) = val ? 1 : 0;
                break;
        }
    }

    for (Int32 i = IRelativeLayout::LEFT_OF; i < IRelativeLayout::VERB_COUNT; i++) {
        initialRules[i] = rules[i];
    }

    a->Recycle();
    return NOERROR;
}

ECode RelativeLayoutLayoutParams::AddRule(
    /* [in] */ Int32 verb)
{
    if (verb < 0 || verb >= mRules->GetLength())
        return E_ILLEGAL_ARGUMENT_EXCEPTION;

    (*mRules)[verb] = IRelativeLayout::RelativeLayout_TRUE;
    (*mInitialRules)[verb] = IRelativeLayout::RelativeLayout_TRUE;
    mRulesChanged = TRUE;

    return NOERROR;
}

ECode RelativeLayoutLayoutParams::AddRule(
    /* [in] */ Int32 verb,
    /* [in] */ Int32 anchor)
{
    if (verb < 0 || verb >= mRules->GetLength())
        return E_ILLEGAL_ARGUMENT_EXCEPTION;

    (*mRules)[verb] = anchor;
    (*mInitialRules)[verb] = anchor;
    mRulesChanged = TRUE;
    return NOERROR;
}

ECode RelativeLayoutLayoutParams::RemoveRule(
    /* [in] */ Int32 verb)
{
    if (verb < 0 || verb >= mRules->GetLength())
        return E_ILLEGAL_ARGUMENT_EXCEPTION;

    (*mRules)[verb] = 0;
    (*mInitialRules)[verb] = 0;
    mRulesChanged = TRUE;
    return NOERROR;
}

Boolean RelativeLayoutLayoutParams::HasRelativeRules()
{
    return ((*mInitialRules)[IRelativeLayout::START_OF] != 0
            || (*mInitialRules)[IRelativeLayout::END_OF] != 0
            || (*mInitialRules)[IRelativeLayout::ALIGN_START] != 0
            || (*mInitialRules)[IRelativeLayout::ALIGN_END] != 0
            || (*mInitialRules)[IRelativeLayout::ALIGN_PARENT_START] != 0
            || (*mInitialRules)[IRelativeLayout::ALIGN_PARENT_END] != 0);
}

void RelativeLayoutLayoutParams::ResolveRules(
    /* [in] */ Int32 layoutDirection)
{
    Boolean isLayoutRtl = (layoutDirection == IView::LAYOUT_DIRECTION_RTL);
    // Reset to initial state
    mRules->Copy(mInitialRules);

    // Apply rules depending on direction
    if ((*mRules)[IRelativeLayout::ALIGN_START] != 0) {
        (*mRules)[isLayoutRtl ? IRelativeLayout::ALIGN_RIGHT : IRelativeLayout::ALIGN_LEFT]
                = (*mRules)[IRelativeLayout::ALIGN_START];
    }
    if ((*mRules)[IRelativeLayout::ALIGN_END] != 0) {
        (*mRules)[isLayoutRtl ? IRelativeLayout::ALIGN_LEFT : IRelativeLayout::ALIGN_RIGHT]
                = (*mRules)[IRelativeLayout::ALIGN_END];
    }
    if ((*mRules)[IRelativeLayout::START_OF] != 0) {
        (*mRules)[isLayoutRtl ? IRelativeLayout::RIGHT_OF : IRelativeLayout::LEFT_OF]
                = (*mRules)[IRelativeLayout::START_OF];
    }
    if ((*mRules)[IRelativeLayout::END_OF] != 0) {
        (*mRules)[isLayoutRtl ? IRelativeLayout::LEFT_OF : IRelativeLayout::RIGHT_OF]
                = (*mRules)[IRelativeLayout::END_OF];
    }
    if ((*mRules)[IRelativeLayout::ALIGN_PARENT_START] != 0) {
        (*mRules)[isLayoutRtl ? IRelativeLayout::ALIGN_PARENT_RIGHT : IRelativeLayout::ALIGN_PARENT_LEFT]
                = (*mRules)[IRelativeLayout::ALIGN_PARENT_START];
    }
    if ((*mRules)[IRelativeLayout::ALIGN_PARENT_END] != 0) {
        (*mRules)[isLayoutRtl ? IRelativeLayout::ALIGN_PARENT_LEFT : IRelativeLayout::ALIGN_PARENT_RIGHT]
                = (*mRules)[IRelativeLayout::ALIGN_PARENT_END];
    }
    mRulesChanged = FALSE;
}

AutoPtr<ArrayOf<Int32> > RelativeLayoutLayoutParams::GetRules(
    /* [in] */ Int32 layoutDirection)
{
    if (HasRelativeRules() &&
            (mRulesChanged || layoutDirection != GetLayoutDirection())) {
        ResolveRules(layoutDirection);
        if (layoutDirection != GetLayoutDirection()) {
            SetLayoutDirection(layoutDirection);
        }
    }
    return mRules;
}

AutoPtr<ArrayOf<Int32> > RelativeLayoutLayoutParams::GetRules()
{
    return mRules;
}

ECode RelativeLayoutLayoutParams::ResolveLayoutDirection(
    /* [in] */ Int32 layoutDirection)
{
    Boolean isLayoutRtl = IsLayoutRtl();
    if (isLayoutRtl) {
        if (mStart != IViewGroupMarginLayoutParams::DEFAULT_RELATIVE) mRight = mStart;
        if (mEnd != IViewGroupMarginLayoutParams::DEFAULT_RELATIVE) mLeft = mEnd;
    } else {
        if (mStart != IViewGroupMarginLayoutParams::DEFAULT_RELATIVE) mLeft = mStart;
        if (mEnd != IViewGroupMarginLayoutParams::DEFAULT_RELATIVE) mRight = mEnd;
    }

    if (HasRelativeRules() && layoutDirection != GetLayoutDirection()) {
        ResolveRules(layoutDirection);
    }
    // This will set the layout direction
    return ViewGroupMarginLayoutParams::ResolveLayoutDirection(layoutDirection);
}

ECode RelativeLayoutLayoutParams::SetAlignWithParent(
    /* [in] */ Boolean align)
{
    mAlignWithParent = align;
    return NOERROR;
}

Boolean RelativeLayoutLayoutParams::GetAlignWithParent()
{
    return mAlignWithParent;
}

} // namespace Widget
} // namespace Droid
} // namespace Elastos
