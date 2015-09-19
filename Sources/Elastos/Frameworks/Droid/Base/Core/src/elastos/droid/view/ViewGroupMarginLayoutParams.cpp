
#include "view/ViewGroupMarginLayoutParams.h"
#include "ext/frameworkdef.h"
#include "R.h"

namespace Elastos {
namespace Droid {
namespace View {

Int32 ViewGroupMarginLayoutParams::LAYOUT_DIRECTION_UNDEFINED = -1;

ViewGroupMarginLayoutParams::ViewGroupMarginLayoutParams()
    : mLeftMargin(0)
    , mTopMargin(0)
    , mRightMargin(0)
    , mBottomMargin(0)
    , mStartMargin(IViewGroupMarginLayoutParams::DEFAULT_RELATIVE)
    , mEndMargin(IViewGroupMarginLayoutParams::DEFAULT_RELATIVE)
    , mInitialLeftMargin(0)
    , mInitialRightMargin(0)
    , mLayoutDirection(LAYOUT_DIRECTION_UNDEFINED)
{}

ViewGroupMarginLayoutParams::ViewGroupMarginLayoutParams(
    /* [in] */ IContext* c,
    /* [in] */ IAttributeSet* attrs)
    : mLeftMargin(0)
    , mTopMargin(0)
    , mRightMargin(0)
    , mBottomMargin(0)
    , mStartMargin(IViewGroupMarginLayoutParams::DEFAULT_RELATIVE)
    , mEndMargin(IViewGroupMarginLayoutParams::DEFAULT_RELATIVE)
    , mInitialLeftMargin(0)
    , mInitialRightMargin(0)
    , mLayoutDirection(LAYOUT_DIRECTION_UNDEFINED)
{
    ASSERT_SUCCEEDED(Init(c, attrs));
}

/**
 * {@inheritDoc}
 */
ViewGroupMarginLayoutParams::ViewGroupMarginLayoutParams(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
    : ViewGroupLayoutParams(width, height)
    , mLeftMargin(0)
    , mTopMargin(0)
    , mRightMargin(0)
    , mBottomMargin(0)
    , mStartMargin(IViewGroupMarginLayoutParams::DEFAULT_RELATIVE)
    , mEndMargin(IViewGroupMarginLayoutParams::DEFAULT_RELATIVE)
    , mInitialLeftMargin(0)
    , mInitialRightMargin(0)
    , mLayoutDirection(LAYOUT_DIRECTION_UNDEFINED)
{}

/**
 * Copy constructor. Clones the width, height and margin values of the source.
 *
 * @param source The layout params to copy from.
 */
ViewGroupMarginLayoutParams::ViewGroupMarginLayoutParams(
    /* [in] */ ViewGroupMarginLayoutParams* source)
    : ViewGroupLayoutParams(source->mWidth, source->mHeight)
    , mLeftMargin(source->mLeftMargin)
    , mTopMargin(source->mTopMargin)
    , mRightMargin(source->mRightMargin)
    , mBottomMargin(source->mBottomMargin)
    , mStartMargin(source->mStartMargin)
    , mEndMargin(source->mEndMargin)
    , mInitialLeftMargin(source->mLeftMargin)
    , mInitialRightMargin(source->mRightMargin)
{
    SetLayoutDirection(source->mLayoutDirection);
}

/**
 * {@inheritDoc}
 */
ViewGroupMarginLayoutParams::ViewGroupMarginLayoutParams(
    /* [in] */ ViewGroupLayoutParams* source)
    : ViewGroupLayoutParams(source)
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
    mInitialLeftMargin = left;
    mInitialRightMargin = right;
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
    mInitialLeftMargin = 0;
    mInitialRightMargin = 0;
    return NOERROR;
}

ECode ViewGroupMarginLayoutParams::SetMarginStart(
    /* [in] */ Int32 start)
{
    mStartMargin = start;
    return NOERROR;
}

Int32 ViewGroupMarginLayoutParams::GetMarginStart()
{
    if (mStartMargin != IViewGroupMarginLayoutParams::DEFAULT_RELATIVE)
        return mStartMargin;
    switch(mLayoutDirection) {
        case IView::LAYOUT_DIRECTION_RTL:
            return mRightMargin;
        case IView::LAYOUT_DIRECTION_LTR:
        default:
            return mLeftMargin;
    }
}

ECode ViewGroupMarginLayoutParams::SetMarginEnd(
    /* [in] */ Int32 end)
{
    mEndMargin = end;
    return NOERROR;
}

Int32 ViewGroupMarginLayoutParams::GetMarginEnd()
{
    if (mEndMargin != IViewGroupMarginLayoutParams::DEFAULT_RELATIVE)
        return mEndMargin;
    switch(mLayoutDirection) {
        case IView::LAYOUT_DIRECTION_RTL:
            return mLeftMargin;
        case IView::LAYOUT_DIRECTION_LTR:
        default:
            return mRightMargin;
    }
}

Boolean ViewGroupMarginLayoutParams::IsMarginRelative()
{
    return (mStartMargin != IViewGroupMarginLayoutParams::DEFAULT_RELATIVE)
        || (mEndMargin != IViewGroupMarginLayoutParams::DEFAULT_RELATIVE);
}

ECode ViewGroupMarginLayoutParams::SetLayoutDirection(
    /* [in] */ Int32 layoutDirection)
{
    if (layoutDirection != IView::LAYOUT_DIRECTION_LTR &&
            layoutDirection != IView::LAYOUT_DIRECTION_RTL) {
        return NOERROR;
    }
    mLayoutDirection = layoutDirection;
    return NOERROR;
}

Int32 ViewGroupMarginLayoutParams::GetLayoutDirection()
{
    return mLayoutDirection;
}

ECode ViewGroupMarginLayoutParams::ResolveLayoutDirection(
    /* [in] */ Int32 layoutDirection)
{
    SetLayoutDirection(layoutDirection);

    if (!IsMarginRelative()) {
        return NOERROR;
    }

    switch(layoutDirection) {
        case IView::LAYOUT_DIRECTION_RTL:
            mLeftMargin = (mEndMargin > IViewGroupMarginLayoutParams::DEFAULT_RELATIVE)
                ? mEndMargin : mInitialLeftMargin;
            mRightMargin = (mStartMargin > IViewGroupMarginLayoutParams::DEFAULT_RELATIVE)
                ? mStartMargin : mInitialRightMargin;
            break;
        case IView::LAYOUT_DIRECTION_LTR:
        default:
            mLeftMargin = (mStartMargin > IViewGroupMarginLayoutParams::DEFAULT_RELATIVE)
                ? mStartMargin : mInitialLeftMargin;
            mRightMargin = (mEndMargin > IViewGroupMarginLayoutParams::DEFAULT_RELATIVE)
                ? mEndMargin : mInitialRightMargin;
            break;
    }

    return NOERROR;
}

Boolean ViewGroupMarginLayoutParams::IsLayoutRtl()
{
    return (mLayoutDirection == IView::LAYOUT_DIRECTION_RTL);
}

ECode ViewGroupMarginLayoutParams::OnDebugDraw(
    /* [in] */ IView* view,
    /* [in] */ ICanvas* canvas)
{
    Int32 l, t, r, b;
    view->GetLeft(&l);
    view->GetTop(&t);
    view->GetRight(&r);
    view->GetBottom(&b);

    // ViewGroup::DrawRect(
    //     canvas, l - mLeftMargin, t - mTopMargin,
    //     r + mRightMargin, b + mBottomMargin, Color::MAGENTA);

    return NOERROR;
}

ECode ViewGroupMarginLayoutParams::Init(
    /* [in] */ IContext* c,
    /* [in] */ IAttributeSet* attrs)
{
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
                0, &mLeftMargin);
        a->GetDimensionPixelSize(
                R::styleable::ViewGroup_MarginLayout_layout_marginTop,
                0, &mTopMargin);
        a->GetDimensionPixelSize(
                R::styleable::ViewGroup_MarginLayout_layout_marginRight,
                0, &mRightMargin);
        a->GetDimensionPixelSize(
                R::styleable::ViewGroup_MarginLayout_layout_marginBottom,
                0, &mBottomMargin);

        a->GetDimensionPixelSize(
                R::styleable::ViewGroup_MarginLayout_layout_marginStart,
                IViewGroupMarginLayoutParams::DEFAULT_RELATIVE, &mStartMargin);
        a->GetDimensionPixelSize(
                R::styleable::ViewGroup_MarginLayout_layout_marginEnd,
                IViewGroupMarginLayoutParams::DEFAULT_RELATIVE, &mEndMargin);
    }

    mInitialLeftMargin = mLeftMargin;
    mInitialRightMargin = mRightMargin;

    a->Recycle();
    return NOERROR;
}

ECode ViewGroupMarginLayoutParams::Init(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    return ViewGroupLayoutParams::Init(width, height);
}

ECode ViewGroupMarginLayoutParams::Init(
    /* [in] */ IViewGroupLayoutParams* source)
{
    return ViewGroupLayoutParams::Init(source);
}

ECode ViewGroupMarginLayoutParams::Init(
    /* [in] */ IViewGroupMarginLayoutParams* source)
{
    source->GetWidth(&mWidth);
    source->GetHeight(&mHeight);
    source->GetMargins(&mLeftMargin, &mTopMargin, &mRightMargin, &mBottomMargin);
    source->GetMarginStart(&mStartMargin);
    source->GetMarginEnd(&mEndMargin);
    mInitialLeftMargin = mLeftMargin;
    mInitialRightMargin = mRightMargin;

    Int32 layoutDirection = 0;
    source->GetLayoutDirection(&layoutDirection);
    return SetLayoutDirection(layoutDirection);
}

} // namespace View
} // namespace Droid
} // namespace Elastos
