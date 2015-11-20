
#include "elastos/droid/widget/RelativeLayoutLayoutParams.h"
#include "elastos/droid/os/Build.h"
#include "elastos/droid/R.h"

using Elastos::Droid::Content::Pm::IApplicationInfo;
using Elastos::Droid::Os::Build;
using Elastos::Droid::View::IView;

namespace Elastos {
namespace Droid {
namespace Widget {

CAR_INTERFACE_IMPL(RelativeLayoutLayoutParams, ViewGroupMarginLayoutParams, IRelativeLayoutLayoutParams);
RelativeLayoutLayoutParams::RelativeLayoutLayoutParams()
    : mLeft(0)
    , mTop(0)
    , mRight(0)
    , mBottom(0)
    , mRulesChanged(FALSE)
    , mIsRtlCompatibilityMode(FALSE)
    , mAlignWithParent(FALSE)
{
    mRules = ArrayOf<Int32>::Alloc(IRelativeLayout::VERB_COUNT);
    mInitialRules = ArrayOf<Int32>::Alloc(IRelativeLayout::VERB_COUNT);
}

ECode RelativeLayoutLayoutParams::constructor(
    /* [in] */ IContext* c,
    /* [in] */ IAttributeSet* attrs)
{
    ASSERT_SUCCEEDED(ViewGroupMarginLayoutParams::constructor(c, attrs));
    ASSERT_SUCCEEDED(InitFromAttributes(c, attrs));
    return NOERROR;
}

ECode RelativeLayoutLayoutParams::constructor(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    ASSERT_SUCCEEDED(ViewGroupMarginLayoutParams::constructor(width, height));
    return NOERROR;
}

ECode RelativeLayoutLayoutParams::constructor(
    /* [in] */ IViewGroupLayoutParams* source)
{
    ASSERT_SUCCEEDED(ViewGroupMarginLayoutParams::constructor(source));
    return NOERROR;
}

ECode RelativeLayoutLayoutParams::constructor(
    /* [in] */ IViewGroupMarginLayoutParams* source)
{
    ASSERT_SUCCEEDED(ViewGroupMarginLayoutParams::constructor(source));
    return NOERROR;
}

ECode RelativeLayoutLayoutParams::constructor(
    /* [in] */ IRelativeLayoutLayoutParams* _source)
{
    ASSERT_SUCCEEDED(ViewGroupMarginLayoutParams::constructor(IViewGroupMarginLayoutParams::Probe(_source)));

    RelativeLayoutLayoutParams* source = (RelativeLayoutLayoutParams*)_source;
    mIsRtlCompatibilityMode = source->mIsRtlCompatibilityMode;
    mRulesChanged = source->mRulesChanged;
    mAlignWithParent = source->mAlignWithParent;

    mRules->Copy(IRelativeLayout::LEFT_OF, source->mRules, IRelativeLayout::LEFT_OF, IRelativeLayout::VERB_COUNT);
    mInitialRules->Copy(IRelativeLayout::LEFT_OF, source->mInitialRules, IRelativeLayout::LEFT_OF, IRelativeLayout::VERB_COUNT);
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
    ASSERT_SUCCEEDED(context->ObtainStyledAttributes(
            attrs, attrIds, (ITypedArray**)&a));

    AutoPtr<IApplicationInfo> info;
    context->GetApplicationInfo((IApplicationInfo**)&info);
    Int32 targetSdkVersion = 0;
    info->GetTargetSdkVersion(&targetSdkVersion);
    Boolean isSupport = FALSE;
    mIsRtlCompatibilityMode = (targetSdkVersion < Build::VERSION_CODES::JELLY_BEAN_MR1 ||
            (info->HasRtlSupport(&isSupport), !isSupport));

    Int32* rules = mRules->GetPayload();
    //noinspection MismatchedReadAndWriteOfArray
    // Int32* initialRules = mInitialRules->GetPayload();

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

    mRulesChanged = TRUE;
    mInitialRules->Copy(IRelativeLayout::LEFT_OF, mRules, IRelativeLayout::LEFT_OF, IRelativeLayout::VERB_COUNT);

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
    mRules->Copy(IRelativeLayout::LEFT_OF, mInitialRules, IRelativeLayout::LEFT_OF, IRelativeLayout::VERB_COUNT);

    // Apply rules depending on direction and if we are in RTL compatibility mode
    if (mIsRtlCompatibilityMode) {
        if ((*mRules)[IRelativeLayout::ALIGN_START] != 0) {
            if ((*mRules)[IRelativeLayout::ALIGN_LEFT] == 0) {
                // "left" rule is not defined but "start" rule is: use the "start" rule as
                // the "left" rule
                (*mRules)[IRelativeLayout::ALIGN_LEFT] = (*mRules)[IRelativeLayout::ALIGN_START];
            }
            (*mRules)[IRelativeLayout::ALIGN_START] = 0;
        }

        if ((*mRules)[IRelativeLayout::ALIGN_END] != 0) {
            if ((*mRules)[IRelativeLayout::ALIGN_RIGHT] == 0) {
                // "right" rule is not defined but "end" rule is: use the "end" rule as the
                // "right" rule
                (*mRules)[IRelativeLayout::ALIGN_RIGHT] = (*mRules)[IRelativeLayout::ALIGN_END];
            }
            (*mRules)[IRelativeLayout::ALIGN_END] = 0;
        }

        if ((*mRules)[IRelativeLayout::START_OF] != 0) {
            if ((*mRules)[IRelativeLayout::LEFT_OF] == 0) {
                // "left" rule is not defined but "start" rule is: use the "start" rule as
                // the "left" rule
                (*mRules)[IRelativeLayout::LEFT_OF] = (*mRules)[IRelativeLayout::START_OF];
            }
            (*mRules)[IRelativeLayout::START_OF] = 0;
        }

        if ((*mRules)[IRelativeLayout::END_OF] != 0) {
            if ((*mRules)[IRelativeLayout::RIGHT_OF] == 0) {
                // "right" rule is not defined but "end" rule is: use the "end" rule as the
                // "right" rule
                (*mRules)[IRelativeLayout::RIGHT_OF] = (*mRules)[IRelativeLayout::END_OF];
            }
            (*mRules)[IRelativeLayout::END_OF] = 0;
        }

        if ((*mRules)[IRelativeLayout::ALIGN_PARENT_START] != 0) {
            if ((*mRules)[IRelativeLayout::ALIGN_PARENT_LEFT] == 0) {
                // "left" rule is not defined but "start" rule is: use the "start" rule as
                // the "left" rule
                (*mRules)[IRelativeLayout::ALIGN_PARENT_LEFT] = (*mRules)[IRelativeLayout::ALIGN_PARENT_START];
            }
            (*mRules)[IRelativeLayout::ALIGN_PARENT_START] = 0;
        }

        if ((*mRules)[IRelativeLayout::ALIGN_PARENT_END] != 0) {
            if ((*mRules)[IRelativeLayout::ALIGN_PARENT_RIGHT] == 0) {
                // "right" rule is not defined but "end" rule is: use the "end" rule as the
                // "right" rule
                (*mRules)[IRelativeLayout::ALIGN_PARENT_RIGHT] = (*mRules)[IRelativeLayout::ALIGN_PARENT_END];
            }
            (*mRules)[IRelativeLayout::ALIGN_PARENT_END] = 0;
        }
    } else {
        // JB MR1+ case
        if (((*mRules)[IRelativeLayout::ALIGN_START] != 0 || (*mRules)[IRelativeLayout::ALIGN_END] != 0) &&
                ((*mRules)[IRelativeLayout::ALIGN_LEFT] != 0 || (*mRules)[IRelativeLayout::ALIGN_RIGHT] != 0)) {
            // "start"/"end" rules take precedence over "left"/"right" rules
            (*mRules)[IRelativeLayout::ALIGN_LEFT] = 0;
            (*mRules)[IRelativeLayout::ALIGN_RIGHT] = 0;
        }
        if ((*mRules)[IRelativeLayout::ALIGN_START] != 0) {
            // "start" rule resolved to "left" or "right" depending on the direction
            (*mRules)[isLayoutRtl ? IRelativeLayout::ALIGN_RIGHT : IRelativeLayout::ALIGN_LEFT] = (*mRules)[IRelativeLayout::ALIGN_START];
            (*mRules)[IRelativeLayout::ALIGN_START] = 0;
        }
        if ((*mRules)[IRelativeLayout::ALIGN_END] != 0) {
            // "end" rule resolved to "left" or "right" depending on the direction
            (*mRules)[isLayoutRtl ? IRelativeLayout::ALIGN_LEFT : IRelativeLayout::ALIGN_RIGHT] = (*mRules)[IRelativeLayout::ALIGN_END];
            (*mRules)[IRelativeLayout::ALIGN_END] = 0;
        }

        if (((*mRules)[IRelativeLayout::START_OF] != 0 || (*mRules)[IRelativeLayout::END_OF] != 0) &&
                ((*mRules)[IRelativeLayout::LEFT_OF] != 0 || (*mRules)[IRelativeLayout::RIGHT_OF] != 0)) {
            // "start"/"end" rules take precedence over "left"/"right" rules
            (*mRules)[IRelativeLayout::LEFT_OF] = 0;
            (*mRules)[IRelativeLayout::RIGHT_OF] = 0;
        }
        if ((*mRules)[IRelativeLayout::START_OF] != 0) {
            // "start" rule resolved to "left" or "right" depending on the direction
            (*mRules)[isLayoutRtl ? IRelativeLayout::RIGHT_OF : IRelativeLayout::LEFT_OF] = (*mRules)[IRelativeLayout::START_OF];
            (*mRules)[IRelativeLayout::START_OF] = 0;
        }
        if ((*mRules)[IRelativeLayout::END_OF] != 0) {
            // "end" rule resolved to "left" or "right" depending on the direction
            (*mRules)[isLayoutRtl ? IRelativeLayout::LEFT_OF : IRelativeLayout::RIGHT_OF] = (*mRules)[IRelativeLayout::END_OF];
            (*mRules)[IRelativeLayout::END_OF] = 0;
        }

        if (((*mRules)[IRelativeLayout::ALIGN_PARENT_START] != 0 || (*mRules)[IRelativeLayout::ALIGN_PARENT_END] != 0) &&
                ((*mRules)[IRelativeLayout::ALIGN_PARENT_LEFT] != 0 || (*mRules)[IRelativeLayout::ALIGN_PARENT_RIGHT] != 0)) {
            // "start"/"end" rules take precedence over "left"/"right" rules
            (*mRules)[IRelativeLayout::ALIGN_PARENT_LEFT] = 0;
            (*mRules)[IRelativeLayout::ALIGN_PARENT_RIGHT] = 0;
        }
        if ((*mRules)[IRelativeLayout::ALIGN_PARENT_START] != 0) {
            // "start" rule resolved to "left" or "right" depending on the direction
            (*mRules)[isLayoutRtl ? IRelativeLayout::ALIGN_PARENT_RIGHT : IRelativeLayout::ALIGN_PARENT_LEFT]
                    = (*mRules)[IRelativeLayout::ALIGN_PARENT_START];
            (*mRules)[IRelativeLayout::ALIGN_PARENT_START] = 0;
        }
        if ((*mRules)[IRelativeLayout::ALIGN_PARENT_END] != 0) {
            // "end" rule resolved to "left" or "right" depending on the direction
            (*mRules)[isLayoutRtl ? IRelativeLayout::ALIGN_PARENT_LEFT : IRelativeLayout::ALIGN_PARENT_RIGHT] = (*mRules)[IRelativeLayout::ALIGN_PARENT_END];
            (*mRules)[IRelativeLayout::ALIGN_PARENT_END] = 0;
        }
    }
    mRulesChanged = FALSE;
}

ECode RelativeLayoutLayoutParams::GetRules(
    /* [in] */ Int32 layoutDirection,
    /* [out, callee] */ ArrayOf<Int32>** rules)
{
    VALIDATE_NOT_NULL(rules);
    Int32 ld = 0;
    if (HasRelativeRules() &&
            (mRulesChanged || layoutDirection != (GetLayoutDirection(&ld), ld))) {
        ResolveRules(layoutDirection);
        if (layoutDirection != ld) {
            SetLayoutDirection(layoutDirection);
        }
    }
    *rules = mRules;
    REFCOUNT_ADD(*rules);
    return NOERROR;
}

ECode RelativeLayoutLayoutParams::GetRules(
    /* [out, callee] */ ArrayOf<Int32>** rules)
{
    VALIDATE_NOT_NULL(rules);
    *rules = mRules;
    REFCOUNT_ADD(*rules);
    return NOERROR;
}

ECode RelativeLayoutLayoutParams::ResolveLayoutDirection(
    /* [in] */ Int32 layoutDirection)
{
    Boolean isLayoutRtl = FALSE;
    IsLayoutRtl(&isLayoutRtl);

    Int32 ld = 0;
    if (HasRelativeRules() && layoutDirection != (GetLayoutDirection(&ld), ld)) {
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

ECode RelativeLayoutLayoutParams::GetAlignWithParent(
    /* [out] */ Boolean* align)
{
    VALIDATE_NOT_NULL(align);
    *align = mAlignWithParent;
    return NOERROR;
}

} // namespace Widget
} // namespace Droid
} // namespace Elastos
