
#include "elastos/droid/view/ViewGroupMarginLayoutParams.h"
#include "elastos/droid/ext/frameworkdef.h"
#include "elastos/droid/R.h"
#include "elastos/droid/os/Build.h"
#include <elastos/core/Math.h>

using Elastos::Droid::Content::Pm::IApplicationInfo;
using Elastos::Droid::Os::Build;

namespace Elastos {
namespace Droid {
namespace View {

CAR_INTERFACE_IMPL(ViewGroupMarginLayoutParams, ViewGroupLayoutParams, IViewGroupMarginLayoutParams)

const Int32 ViewGroupMarginLayoutParams::DEFAULT_MARGIN_RELATIVE = Elastos::Core::Math::INT32_MIN_VALUE;
const Int32 ViewGroupMarginLayoutParams::LAYOUT_DIRECTION_MASK = 0x00000003;
const Int32 ViewGroupMarginLayoutParams::LEFT_MARGIN_UNDEFINED_MASK = 0x00000004;
const Int32 ViewGroupMarginLayoutParams::RIGHT_MARGIN_UNDEFINED_MASK = 0x00000008;
const Int32 ViewGroupMarginLayoutParams::RTL_COMPATIBILITY_MODE_MASK = 0x00000010;
const Int32 ViewGroupMarginLayoutParams::NEED_RESOLUTION_MASK = 0x00000020;
const Int32 ViewGroupMarginLayoutParams::DEFAULT_MARGIN_RESOLVED = 0;
const Int32 ViewGroupMarginLayoutParams::UNDEFINED_MARGIN = IViewGroupMarginLayoutParams::DEFAULT_MARGIN_RELATIVE;

ViewGroupMarginLayoutParams::ViewGroupMarginLayoutParams()
    : mLeftMargin(0)
    , mTopMargin(0)
    , mRightMargin(0)
    , mBottomMargin(0)
    , mStartMargin(IViewGroupMarginLayoutParams::DEFAULT_MARGIN_RELATIVE)
    , mEndMargin(IViewGroupMarginLayoutParams::DEFAULT_MARGIN_RELATIVE)
    , mMarginFlags(0)
{}

ECode ViewGroupMarginLayoutParams::GetLeftMargin(
    /* [out] */ Int32* leftMargin)
{
    VALIDATE_NOT_NULL(leftMargin);
    *leftMargin = mLeftMargin;
    return NOERROR;
}

ECode ViewGroupMarginLayoutParams::SetLeftMargin(
    /* [in] */ Int32 leftMargin)
{
    mLeftMargin = leftMargin;
    return NOERROR;
}

ECode ViewGroupMarginLayoutParams::GetTopMargin(
    /* [out] */ Int32* topMargin)
{
    VALIDATE_NOT_NULL(topMargin);
    *topMargin = mTopMargin;
    return NOERROR;
}

ECode ViewGroupMarginLayoutParams::SetTopMargin(
    /* [in] */ Int32 topMargin)
{
    mTopMargin = topMargin;
    return NOERROR;
}

ECode ViewGroupMarginLayoutParams::GetRightMargin(
    /* [out] */ Int32* rightMargin)
{
    VALIDATE_NOT_NULL(rightMargin);
    *rightMargin = mRightMargin;
    return NOERROR;
}

ECode ViewGroupMarginLayoutParams::SetRightMargin(
    /* [in] */ Int32 rightMargin)
{
    mRightMargin = rightMargin;
    return NOERROR;
}

ECode ViewGroupMarginLayoutParams::GetBottomMargin(
    /* [out] */ Int32* bottomMargin)
{
    VALIDATE_NOT_NULL(bottomMargin);
    *bottomMargin = mBottomMargin;
    return NOERROR;
}

ECode ViewGroupMarginLayoutParams::SetBottomMargin(
    /* [in] */ Int32 bottomMargin)
{
    mBottomMargin = bottomMargin;
    return NOERROR;
}

ECode ViewGroupMarginLayoutParams::SetMargins(
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom)
{
    mLeftMargin = left;
    mTopMargin = top;
    mRightMargin = right;
    mBottomMargin = bottom;
    mMarginFlags &= ~LEFT_MARGIN_UNDEFINED_MASK;
    mMarginFlags &= ~RIGHT_MARGIN_UNDEFINED_MASK;
    Boolean isMarginRelative;
    IsMarginRelative(&isMarginRelative);
    if (isMarginRelative) {
        mMarginFlags |= NEED_RESOLUTION_MASK;
    } else {
        mMarginFlags &= ~NEED_RESOLUTION_MASK;
    }
    return NOERROR;
}

ECode ViewGroupMarginLayoutParams::GetMargins(
    /* [out] */ Int32* left,
    /* [out] */ Int32* top,
    /* [out] */ Int32* right,
    /* [out] */ Int32* bottom)
{
    assert(left && top && right && bottom);
    *left = mLeftMargin;
    *top = mTopMargin;
    *right = mRightMargin;
    *bottom = mBottomMargin;

    return NOERROR;
}

ECode ViewGroupMarginLayoutParams::SetMarginsRelative(
    /* [in] */ Int32 start,
    /* [in] */ Int32 top,
    /* [in] */ Int32 end,
    /* [in] */ Int32 bottom)
{
    mStartMargin = start;
    mTopMargin = top;
    mEndMargin = end;
    mBottomMargin = bottom;
    mMarginFlags |= NEED_RESOLUTION_MASK;
    return NOERROR;
}

ECode ViewGroupMarginLayoutParams::SetMarginStart(
    /* [in] */ Int32 start)
{
    mStartMargin = start;
    mMarginFlags |= NEED_RESOLUTION_MASK;
    return NOERROR;
}

ECode ViewGroupMarginLayoutParams::GetMarginStart(
    /* [out] */ Int32* start)
{
    VALIDATE_NOT_NULL(start)
    if (mStartMargin != IViewGroupMarginLayoutParams::DEFAULT_MARGIN_RELATIVE) {
        *start = mStartMargin;
        return NOERROR;
    }
    if ((mMarginFlags & NEED_RESOLUTION_MASK) == NEED_RESOLUTION_MASK) {
        DoResolveMargins();
    }
    switch(mMarginFlags & LAYOUT_DIRECTION_MASK) {
        case IView::LAYOUT_DIRECTION_RTL:
            *start = mRightMargin;
            return NOERROR;
        case IView::LAYOUT_DIRECTION_LTR:
        default:
            *start = mLeftMargin;
            return NOERROR;
    }
}

ECode ViewGroupMarginLayoutParams::SetMarginEnd(
    /* [in] */ Int32 end)
{
    mEndMargin = end;
    mMarginFlags |= NEED_RESOLUTION_MASK;
    return NOERROR;
}

ECode ViewGroupMarginLayoutParams::GetMarginEnd(
    /* [out] */ Int32* end)
{
    VALIDATE_NOT_NULL(end)
    if (mEndMargin != IViewGroupMarginLayoutParams::DEFAULT_MARGIN_RELATIVE) {
        *end = mEndMargin;
        return NOERROR;
    }
    if ((mMarginFlags & NEED_RESOLUTION_MASK) == NEED_RESOLUTION_MASK) {
        DoResolveMargins();
    }
    switch(mMarginFlags & LAYOUT_DIRECTION_MASK) {
        case IView::LAYOUT_DIRECTION_RTL:
            *end = mLeftMargin;
            return NOERROR;
        case IView::LAYOUT_DIRECTION_LTR:
        default:
            *end = mRightMargin;
            return NOERROR;
    }
}

ECode ViewGroupMarginLayoutParams::IsMarginRelative(
    /* [out] */ Boolean* set)
{
    VALIDATE_NOT_NULL(set)
    *set = (mStartMargin != IViewGroupMarginLayoutParams::DEFAULT_MARGIN_RELATIVE)
        || (mEndMargin != IViewGroupMarginLayoutParams::DEFAULT_MARGIN_RELATIVE);
    return NOERROR;
}

ECode ViewGroupMarginLayoutParams::SetLayoutDirection(
    /* [in] */ Int32 layoutDirection)
{
    if (layoutDirection != IView::LAYOUT_DIRECTION_LTR &&
            layoutDirection != IView::LAYOUT_DIRECTION_RTL) {
        return NOERROR;
    }
    if (layoutDirection != (mMarginFlags & LAYOUT_DIRECTION_MASK)) {
        mMarginFlags &= ~LAYOUT_DIRECTION_MASK;
        mMarginFlags |= (layoutDirection & LAYOUT_DIRECTION_MASK);
        Boolean isMarginRelative;
        IsMarginRelative(&isMarginRelative);
        if (isMarginRelative) {
            mMarginFlags |= NEED_RESOLUTION_MASK;
        } else {
            mMarginFlags &= ~NEED_RESOLUTION_MASK;
        }
    }
    return NOERROR;
}

ECode ViewGroupMarginLayoutParams::GetLayoutDirection(
    /* [out] */ Int32* layoutDirection)
{
    VALIDATE_NOT_NULL(layoutDirection)
    *layoutDirection = (mMarginFlags & LAYOUT_DIRECTION_MASK);
    return NOERROR;
}

ECode ViewGroupMarginLayoutParams::GetMarginFlags(
    /* [out] */ Byte* leftMargin)
{
    VALIDATE_NOT_NULL(leftMargin);
    *leftMargin = mLeftMargin;
    return NOERROR;
}

ECode ViewGroupMarginLayoutParams::SetMarginFlags(
    /* [in] */ Byte leftMargin)
{
    mLeftMargin = leftMargin;
    return NOERROR;
}

ECode ViewGroupMarginLayoutParams::ResolveLayoutDirection(
    /* [in] */ Int32 layoutDirection)
{
    SetLayoutDirection(layoutDirection);

    // No relative margin or pre JB-MR1 case or no need to resolve, just dont do anything
    // Will use the left and right margins if no relative margin is defined.
    Boolean isMarginRelative;
    IsMarginRelative(&isMarginRelative);
    if (!isMarginRelative ||
            (mMarginFlags & NEED_RESOLUTION_MASK) != NEED_RESOLUTION_MASK) return NOERROR;

    // Proceed with resolution
    DoResolveMargins();

    return NOERROR;
}

void ViewGroupMarginLayoutParams::DoResolveMargins()
{
    if ((mMarginFlags & RTL_COMPATIBILITY_MODE_MASK) == RTL_COMPATIBILITY_MODE_MASK) {
        // if left or right margins are not defined and if we have some start or end margin
        // defined then use those start and end margins.
        if ((mMarginFlags & LEFT_MARGIN_UNDEFINED_MASK) == LEFT_MARGIN_UNDEFINED_MASK
                && mStartMargin > DEFAULT_MARGIN_RELATIVE) {
            mLeftMargin = mStartMargin;
        }
        if ((mMarginFlags & RIGHT_MARGIN_UNDEFINED_MASK) == RIGHT_MARGIN_UNDEFINED_MASK
                && mEndMargin > DEFAULT_MARGIN_RELATIVE) {
            mRightMargin = mEndMargin;
        }
    } else {
        // We have some relative margins (either the start one or the end one or both). So use
        // them and override what has been defined for left and right margins. If either start
        // or end margin is not defined, just set it to default "0".
        switch(mMarginFlags & LAYOUT_DIRECTION_MASK) {
            case IView::LAYOUT_DIRECTION_RTL:
                mLeftMargin = (mEndMargin > DEFAULT_MARGIN_RELATIVE) ?
                        mEndMargin : DEFAULT_MARGIN_RESOLVED;
                mRightMargin = (mStartMargin > DEFAULT_MARGIN_RELATIVE) ?
                        mStartMargin : DEFAULT_MARGIN_RESOLVED;
                break;
            case IView::LAYOUT_DIRECTION_LTR:
            default:
                mLeftMargin = (mStartMargin > DEFAULT_MARGIN_RELATIVE) ?
                        mStartMargin : DEFAULT_MARGIN_RESOLVED;
                mRightMargin = (mEndMargin > DEFAULT_MARGIN_RELATIVE) ?
                        mEndMargin : DEFAULT_MARGIN_RESOLVED;
                break;
        }
    }
    mMarginFlags &= ~NEED_RESOLUTION_MASK;
}

ECode ViewGroupMarginLayoutParams::IsLayoutRtl(
    /* [out] */ Boolean* rtl)
{
    VALIDATE_NOT_NULL(rtl)
    *rtl = ((mMarginFlags & LAYOUT_DIRECTION_MASK) == IView::LAYOUT_DIRECTION_RTL);
    return NOERROR;
}

ECode ViewGroupMarginLayoutParams::OnDebugDraw(
    /* [in] */ IView* view,
    /* [in] */ ICanvas* canvas)
{
    /*AutoPtr<IInsets> oi = IsLayoutModeOptical(view.mParent) ? view.getOpticalInsets() : Insets.NONE;

    fillDifference(canvas,
            view.getLeft()   + oi.left,
            view.getTop()    + oi.top,
            view.getRight()  - oi.right,
            view.getBottom() - oi.bottom,
            leftMargin,
            topMargin,
            rightMargin,
            bottomMargin,
            paint);*/

    return NOERROR;
}

ECode ViewGroupMarginLayoutParams::constructor(
    /* [in] */ IContext* c,
    /* [in] */ IAttributeSet* attrs)
{
    ViewGroupLayoutParams::constructor(c, attrs);
    AutoPtr<ArrayOf<Int32> > attrIds = ArrayOf<Int32>::Alloc(
        const_cast<Int32 *>(R::styleable::ViewGroup_MarginLayout),
        ARRAY_SIZE(R::styleable::ViewGroup_MarginLayout));
    AutoPtr<ITypedArray> a;
    ASSERT_SUCCEEDED(c->ObtainStyledAttributes(attrs, attrIds, (ITypedArray**)&a));

    ECode ec = SetBaseAttributes(a,
                R::styleable::ViewGroup_MarginLayout_layout_width,
                R::styleable::ViewGroup_MarginLayout_layout_height);

    if (FAILED(ec)) {
        a->Recycle();
        return ec;
    }

    Int32 margin;
    a->GetDimensionPixelSize(
            R::styleable::ViewGroup_MarginLayout_layout_margin,
            -1, &margin);
    if (margin >= 0) {
        mLeftMargin = margin;
        mTopMargin = margin;
        mRightMargin= margin;
        mBottomMargin = margin;
    }
    else {
        a->GetDimensionPixelSize(
                R::styleable::ViewGroup_MarginLayout_layout_marginLeft,
                UNDEFINED_MARGIN, &mLeftMargin);
        if (mLeftMargin == UNDEFINED_MARGIN) {
            mMarginFlags |= LEFT_MARGIN_UNDEFINED_MASK;
            mLeftMargin = DEFAULT_MARGIN_RESOLVED;
        }
        a->GetDimensionPixelSize(
                R::styleable::ViewGroup_MarginLayout_layout_marginTop,
                DEFAULT_MARGIN_RESOLVED, &mTopMargin);
        a->GetDimensionPixelSize(
                R::styleable::ViewGroup_MarginLayout_layout_marginRight,
                UNDEFINED_MARGIN, &mRightMargin);
        if (mRightMargin == UNDEFINED_MARGIN) {
            mMarginFlags |= RIGHT_MARGIN_UNDEFINED_MASK;
            mRightMargin = DEFAULT_MARGIN_RESOLVED;
        }
        a->GetDimensionPixelSize(
                R::styleable::ViewGroup_MarginLayout_layout_marginBottom,
                DEFAULT_MARGIN_RESOLVED, &mBottomMargin);

        a->GetDimensionPixelSize(
                R::styleable::ViewGroup_MarginLayout_layout_marginStart,
                IViewGroupMarginLayoutParams::DEFAULT_MARGIN_RELATIVE, &mStartMargin);
        a->GetDimensionPixelSize(
                R::styleable::ViewGroup_MarginLayout_layout_marginEnd,
                IViewGroupMarginLayoutParams::DEFAULT_MARGIN_RELATIVE, &mEndMargin);

        Boolean isMarginRelative;
        IsMarginRelative(&isMarginRelative);
        if (isMarginRelative) {
           mMarginFlags |= NEED_RESOLUTION_MASK;
        }
    }

    AutoPtr<IApplicationInfo> info;
    c->GetApplicationInfo((IApplicationInfo**)&info);
    Boolean hasRtlSupport;
    info->HasRtlSupport(&hasRtlSupport);
    Int32 targetSdkVersion;
    info->GetTargetSdkVersion(&targetSdkVersion);
    if (targetSdkVersion < Build::VERSION_CODES::JELLY_BEAN_MR1 || !hasRtlSupport) {
        mMarginFlags |= RTL_COMPATIBILITY_MODE_MASK;
    }

    // Layout direction is LTR by default
    mMarginFlags |= IView::LAYOUT_DIRECTION_LTR;

    a->Recycle();
    return NOERROR;
}

ECode ViewGroupMarginLayoutParams::constructor(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    ViewGroupLayoutParams::constructor(width, height);
    mMarginFlags |= LEFT_MARGIN_UNDEFINED_MASK;
    mMarginFlags |= RIGHT_MARGIN_UNDEFINED_MASK;

    mMarginFlags &= ~NEED_RESOLUTION_MASK;
    mMarginFlags &= ~RTL_COMPATIBILITY_MODE_MASK;
    return NOERROR;
}

ECode ViewGroupMarginLayoutParams::constructor(
    /* [in] */ IViewGroupLayoutParams* source)
{
    ViewGroupLayoutParams::constructor(source);
    mMarginFlags |= LEFT_MARGIN_UNDEFINED_MASK;
    mMarginFlags |= RIGHT_MARGIN_UNDEFINED_MASK;

    mMarginFlags &= ~NEED_RESOLUTION_MASK;
    mMarginFlags &= ~RTL_COMPATIBILITY_MODE_MASK;
    return NOERROR;
}

ECode ViewGroupMarginLayoutParams::constructor(
    /* [in] */ IViewGroupMarginLayoutParams* source)
{
    IViewGroupLayoutParams::Probe(source)->GetWidth(&mWidth);
    IViewGroupLayoutParams::Probe(source)->GetHeight(&mHeight);
    source->GetMargins(&mLeftMargin, &mTopMargin, &mRightMargin, &mBottomMargin);
    source->GetMarginStart(&mStartMargin);
    source->GetMarginEnd(&mEndMargin);

    source->GetMarginFlags(&mMarginFlags);
    return NOERROR;
}

ECode ViewGroupMarginLayoutParams::CopyMarginsFrom(
    /* [in] */ IViewGroupMarginLayoutParams* source)
{
    source->GetLeftMargin(&mLeftMargin);
    source->GetTopMargin(&mTopMargin);
    source->GetRightMargin(&mRightMargin);
    source->GetBottomMargin(&mBottomMargin);
    source->GetMarginStart(&mStartMargin);
    source->GetMarginEnd(&mEndMargin);
    source->GetMarginFlags(&mMarginFlags);
    return NOERROR;
}

} // namespace View
} // namespace Droid
} // namespace Elastos
