#include "ext/frameworkext.h"
#include "widget/FrameLayout.h"
#include <elastos/core/Math.h>
#include <R.h>
#include "view/Gravity.h"
#include "widget/CFrameLayoutLayoutParams.h"

using Elastos::Droid::Graphics::Drawable::IDrawableCallback;
using Elastos::Droid::Graphics::Drawable::EIID_IDrawableCallback;
using Elastos::Droid::View::Gravity;
using Elastos::Droid::View::IGravity;
using Elastos::Droid::View::View;
using Elastos::Core::CStringWrapper;

namespace Elastos{
namespace Droid{
namespace Widget{

static Int32 InitDefaultChildGravity()
{
    return IGravity::TOP | IGravity::START;
}
Int32 FrameLayout::DEFAULT_CHILD_GRAVITY = InitDefaultChildGravity();

FrameLayout::FrameLayout()
    : mForegroundInPadding(TRUE)
    , mMeasureAllChildren(FALSE)
    , mForegroundPaddingLeft(0)
    , mForegroundPaddingTop(0)
    , mForegroundPaddingRight(0)
    , mForegroundPaddingBottom(0)
    , mForegroundGravity(IGravity::FILL)
    , mForegroundBoundsChanged(FALSE)
{
    ASSERT_SUCCEEDED(CRect::NewByFriend((CRect**)&mSelfBounds));
    ASSERT_SUCCEEDED(CRect::NewByFriend((CRect**)&mOverlayBounds));
}

FrameLayout::FrameLayout(
    /* [in] */ IContext* context)
    : ViewGroup(context)
    , mForegroundInPadding(TRUE)
    , mMeasureAllChildren(FALSE)
    , mForegroundPaddingLeft(0)
    , mForegroundPaddingTop(0)
    , mForegroundPaddingRight(0)
    , mForegroundPaddingBottom(0)
    , mForegroundGravity(IGravity::FILL)
    , mForegroundBoundsChanged(FALSE)
{
    ASSERT_SUCCEEDED(CRect::NewByFriend((CRect**)&mSelfBounds));
    ASSERT_SUCCEEDED(CRect::NewByFriend((CRect**)&mOverlayBounds));
}

FrameLayout::FrameLayout(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
    : ViewGroup(context, attrs, 0)
    , mForegroundInPadding(TRUE)
    , mMeasureAllChildren(FALSE)
    , mForegroundPaddingLeft(0)
    , mForegroundPaddingTop(0)
    , mForegroundPaddingRight(0)
    , mForegroundPaddingBottom(0)
    , mForegroundGravity(IGravity::FILL)
    , mForegroundBoundsChanged(FALSE)
{
    ASSERT_SUCCEEDED(CRect::NewByFriend((CRect**)&mSelfBounds));
    ASSERT_SUCCEEDED(CRect::NewByFriend((CRect**)&mOverlayBounds));
    ASSERT_SUCCEEDED(InitFromAttributes(context, attrs, 0));
}

FrameLayout::FrameLayout(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
    : ViewGroup(context, attrs, defStyle)
    , mForegroundInPadding(TRUE)
    , mMeasureAllChildren(FALSE)
    , mForegroundPaddingLeft(0)
    , mForegroundPaddingTop(0)
    , mForegroundPaddingRight(0)
    , mForegroundPaddingBottom(0)
    , mForegroundGravity(IGravity::FILL)
    , mForegroundBoundsChanged(FALSE)
{
    ASSERT_SUCCEEDED(CRect::NewByFriend((CRect**)&mSelfBounds));
    ASSERT_SUCCEEDED(CRect::NewByFriend((CRect**)&mOverlayBounds));
    ASSERT_SUCCEEDED(InitFromAttributes(context, attrs, defStyle));
}

ECode FrameLayout::Init(
    /* [in] */ IContext* context)
{
    ASSERT_SUCCEEDED(ViewGroup::Init(context));
    return NOERROR;
}

ECode FrameLayout::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    ASSERT_SUCCEEDED(Init(context, attrs, 0));
    return NOERROR;
}

ECode FrameLayout::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    ASSERT_SUCCEEDED(ViewGroup::Init(context, attrs, defStyle));
    ASSERT_SUCCEEDED(InitFromAttributes(context, attrs, defStyle));
    return NOERROR;
}

ECode FrameLayout::InitFromAttributes(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    AutoPtr<ArrayOf<Int32> > attrIds = ArrayOf<Int32>::Alloc(
            const_cast<Int32 *>(R::styleable::FrameLayout),
            ARRAY_SIZE(R::styleable::FrameLayout));
    AutoPtr<ITypedArray> a;
    FAIL_RETURN(context->ObtainStyledAttributes(
            attrs, attrIds, defStyle, 0, (ITypedArray**)&a));

    a->GetInt32(R::styleable::FrameLayout_foregroundGravity,
            mForegroundGravity, &mForegroundGravity);

    AutoPtr<IDrawable> d;
    a->GetDrawable(R::styleable::FrameLayout_foreground,
            (IDrawable**)&d);
    if (d != NULL) {
        SetForeground(d);
    }

    Boolean value;
    a->GetBoolean(R::styleable::FrameLayout_measureAllChildren,
            FALSE, &value);
    if (value) {
        SetMeasureAllChildren(TRUE);
    }

    a->GetBoolean(R::styleable::FrameLayout_foregroundInsidePadding,
            TRUE, &mForegroundInPadding);

    a->Recycle();
    return NOERROR;
}

Int32 FrameLayout::GetForegroundGravity()
{
    return mForegroundGravity;
}

ECode FrameLayout::SetForegroundGravity(
    /* [in] */ Int32 foregroundGravity)
{
    if (mForegroundGravity != foregroundGravity) {
        if ((foregroundGravity & IGravity::RELATIVE_HORIZONTAL_GRAVITY_MASK) == 0) {
            foregroundGravity |= IGravity::START;
        }

        if ((foregroundGravity & IGravity::VERTICAL_GRAVITY_MASK) == 0) {
            foregroundGravity |= IGravity::TOP;
        }

        mForegroundGravity = foregroundGravity;

        if (mForegroundGravity == IGravity::FILL && mForeground != NULL) {
            AutoPtr<IRect> padding;
            CRect::New((IRect**)&padding);
            Boolean isPadding;
            mForeground->GetPadding(padding, &isPadding);
            if (isPadding) {
                mForegroundPaddingLeft = ((CRect*)padding.Get())->mLeft;
                mForegroundPaddingTop = ((CRect*)padding.Get())->mTop;
                mForegroundPaddingRight = ((CRect*)padding.Get())->mRight;
                mForegroundPaddingBottom = ((CRect*)padding.Get())->mBottom;
            }
        } else {
            mForegroundPaddingLeft = 0;
            mForegroundPaddingTop = 0;
            mForegroundPaddingRight = 0;
            mForegroundPaddingBottom = 0;
        }

        RequestLayout();
    }
    return NOERROR;
}

Boolean FrameLayout::VerifyDrawable(
    /* [in] */ IDrawable* who)
{
    return ViewGroup::VerifyDrawable(who) || (who == mForeground);
}

ECode FrameLayout::JumpDrawablesToCurrentState()
{
    FAIL_RETURN(ViewGroup::JumpDrawablesToCurrentState());
    if (mForeground != NULL)
        return mForeground->JumpToCurrentState();
    return NOERROR;
}

ECode FrameLayout::DrawableStateChanged()
{
    FAIL_RETURN(ViewGroup::DrawableStateChanged());
    if (mForeground != NULL) {
        Boolean isStateful;
        mForeground->IsStateful(&isStateful);
        if (isStateful) {
            AutoPtr<ArrayOf<Int32> > states = GetDrawableState();
            return mForeground->SetState(states, &isStateful);
        }
    }
    return NOERROR;
}

ECode FrameLayout::GenerateDefaultLayoutParams(
    /* [out] */ IViewGroupLayoutParams** params)
{
    VALIDATE_NOT_NULL(params);
    AutoPtr<IFrameLayoutLayoutParams> temp;
    CFrameLayoutLayoutParams::New(
        IViewGroupLayoutParams::MATCH_PARENT,
        IViewGroupLayoutParams::MATCH_PARENT,
        (IFrameLayoutLayoutParams**)&temp);
    *params = IViewGroupLayoutParams::Probe(temp);
    REFCOUNT_ADD(*params);
    return NOERROR;
}

ECode FrameLayout::SetForeground(
    /* [in] */ IDrawable* drawable)
{
    if (mForeground.Get() != drawable) {
        if (mForeground != NULL) {
            mForeground->SetCallback(NULL);
            UnscheduleDrawable(mForeground);
        }

        mForeground = drawable;
        mForegroundPaddingLeft = 0;
        mForegroundPaddingTop = 0;
        mForegroundPaddingRight = 0;
        mForegroundPaddingBottom = 0;

        if (drawable != NULL) {
            SetWillNotDraw(FALSE);
            IDrawableCallback* callback = (IDrawableCallback*)this->Probe(EIID_IDrawableCallback);
            drawable->SetCallback(callback);

            Boolean isStateful;
            drawable->IsStateful(&isStateful);
            if (isStateful) {
                AutoPtr<ArrayOf<Int32> > states = GetDrawableState();
                drawable->SetState(states, &isStateful);
            }
            if (mForegroundGravity == IGravity::FILL) {
                AutoPtr<IRect> padding;
                CRect::New((IRect**)&padding);
                Boolean isPadding;
                drawable->GetPadding(padding, &isPadding);
                if (isPadding) {
                    mForegroundPaddingLeft = ((CRect*)padding.Get())->mLeft;
                    mForegroundPaddingTop = ((CRect*)padding.Get())->mTop;
                    mForegroundPaddingRight = ((CRect*)padding.Get())->mRight;
                    mForegroundPaddingBottom = ((CRect*)padding.Get())->mBottom;
                }
            }
        } else {
            SetWillNotDraw(TRUE);
        }
        RequestLayout();
        Invalidate();
    }
    return NOERROR;
}

AutoPtr<IDrawable> FrameLayout::GetForeground()
{
    return mForeground;
}

Int32 FrameLayout::GetPaddingLeftWithForeground()
{
    return mForegroundInPadding ? Elastos::Core::Math::Max(mPaddingLeft, mForegroundPaddingLeft) :
        mPaddingLeft + mForegroundPaddingLeft;
}

Int32 FrameLayout::GetPaddingRightWithForeground()
{
    return mForegroundInPadding ? Elastos::Core::Math::Max(mPaddingRight, mForegroundPaddingRight) :
        mPaddingRight + mForegroundPaddingRight;
}

Int32 FrameLayout::GetPaddingTopWithForeground()
{
    return mForegroundInPadding ? Elastos::Core::Math::Max(mPaddingTop, mForegroundPaddingTop) :
        mPaddingTop + mForegroundPaddingTop;
}

Int32 FrameLayout::GetPaddingBottomWithForeground()
{
    return mForegroundInPadding ? Elastos::Core::Math::Max(mPaddingBottom, mForegroundPaddingBottom) :
        mPaddingBottom + mForegroundPaddingBottom;
}

void FrameLayout::OnMeasure(
    /* [in] */ Int32 widthMeasureSpec,
    /* [in] */ Int32 heightMeasureSpec)
{
    Int32 count = GetChildCount();

    Boolean measureMatchParentChildren =
        MeasureSpec::GetMode(widthMeasureSpec) != MeasureSpec::EXACTLY ||
        MeasureSpec::GetMode(heightMeasureSpec) != MeasureSpec::EXACTLY;
    mMatchParentChildren.Clear();

    Int32 maxHeight = 0;
    Int32 maxWidth = 0;
    Int32 childState = 0;
    Int32 visibility;

    for (Int32 i = 0; i < count; i++) {
        AutoPtr<IView>  child = GetChildAt(i);
        if (child == NULL) {
            continue;
        }

        child->GetVisibility(&visibility);
        if (mMeasureAllChildren || visibility != IView::GONE) {
            MeasureChildWithMargins(child, widthMeasureSpec, 0, heightMeasureSpec, 0);

            AutoPtr<IFrameLayoutLayoutParams> lp;
            child->GetLayoutParams((IViewGroupLayoutParams**)&lp);
            Int32 ml, mt, mr, mb;
            lp->GetMargins(&ml, &mt, &mr, &mb);

            Int32 childWidth, childHeight, state;
            child->GetMeasuredWidth(&childWidth);
            child->GetMeasuredHeight(&childHeight);
            child->GetMeasuredState(&state);

            maxWidth = Elastos::Core::Math::Max(maxWidth, childWidth + ml + mr);
            maxHeight = Elastos::Core::Math::Max(maxHeight, childHeight + mt + mb);
            childState = CombineMeasuredStates(childState, state);
            if (measureMatchParentChildren) {
                Int32 width, height;
                lp->GetWidth(&width);
                lp->GetHeight(&height);
                if (width == IViewGroupLayoutParams::MATCH_PARENT ||
                        height == IViewGroupLayoutParams::MATCH_PARENT) {
                    mMatchParentChildren.PushBack(child);
                }
            }
        }
    }

    // Account for padding too
    maxWidth += GetPaddingLeftWithForeground() + GetPaddingRightWithForeground();
    maxHeight += GetPaddingTopWithForeground() + GetPaddingBottomWithForeground();

    // Check against our minimum height and width
    maxHeight = Elastos::Core::Math::Max(maxHeight, GetSuggestedMinimumHeight());
    maxWidth = Elastos::Core::Math::Max(maxWidth, GetSuggestedMinimumWidth());

    // Check against our foreground's minimum height and width
    AutoPtr<IDrawable> drawable = GetForeground();
    if (drawable != NULL) {
        Int32 drawMinHeight, drawMinWidth;
        drawable->GetMinimumHeight(&drawMinHeight);
        drawable->GetMinimumWidth(&drawMinWidth);

        maxHeight = Elastos::Core::Math::Max(maxHeight, drawMinHeight);
        maxWidth = Elastos::Core::Math::Max(maxWidth, drawMinWidth);
    }

    SetMeasuredDimension(ResolveSizeAndState(maxWidth, widthMeasureSpec, childState),
        ResolveSizeAndState(maxHeight, heightMeasureSpec,
        childState << IView::MEASURED_HEIGHT_STATE_SHIFT));

    count = mMatchParentChildren.GetSize();
    if (count > 1) {
        for (Int32 i = 0; i < count; i++) {
            AutoPtr<IView> child = mMatchParentChildren[i];

            AutoPtr<IFrameLayoutLayoutParams> lp;
            child->GetLayoutParams((IViewGroupLayoutParams**)&lp);
            Int32 ml, mt, mr, mb;
            lp->GetMargins(&ml, &mt, &mr, &mb);
            Int32 width, height;
            lp->GetWidth(&width);
            lp->GetHeight(&height);

            Int32 childWidthMeasureSpec;
            Int32 childHeightMeasureSpec;

            if (width == IViewGroupLayoutParams::MATCH_PARENT) {
                childWidthMeasureSpec = MeasureSpec::MakeMeasureSpec(
                        GetMeasuredWidth() - GetPaddingLeftWithForeground()
                        - GetPaddingRightWithForeground() - ml - mr, MeasureSpec::EXACTLY);
            } else {
                childWidthMeasureSpec = GetChildMeasureSpec(widthMeasureSpec,
                    GetPaddingLeftWithForeground() + GetPaddingRightWithForeground()
                    + ml + mr, width);
            }

            if (height == IViewGroupLayoutParams::MATCH_PARENT) {
                childHeightMeasureSpec = MeasureSpec::MakeMeasureSpec(
                        GetMeasuredHeight() - GetPaddingTopWithForeground()
                        - GetPaddingBottomWithForeground() - mt - mb, MeasureSpec::EXACTLY);
            } else {
                childHeightMeasureSpec = GetChildMeasureSpec(heightMeasureSpec,
                    GetPaddingTopWithForeground() + GetPaddingBottomWithForeground()
                    + mt + mb,  height);
            }

            child->Measure(childWidthMeasureSpec, childHeightMeasureSpec);
        }
    }
}

void FrameLayout::OnLayout(
    /* [in] */ Boolean changed,
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom)
{
    const Int32 count = GetChildCount();

    Int32 parentLeft = GetPaddingLeftWithForeground();
    Int32 parentRight = right - left - GetPaddingRightWithForeground();

    Int32 parentTop = GetPaddingTopWithForeground();
    Int32 parentBottom = bottom - top - GetPaddingBottomWithForeground();

    mForegroundBoundsChanged = TRUE;
    Int32 visibility;

    for (Int32 i = 0; i < count; i++) {
        AutoPtr<IView>  child = GetChildAt(i);
        if (child == NULL) {
            continue;
        }

        child->GetVisibility(&visibility);
        if (visibility != IView::GONE) {
            AutoPtr<IFrameLayoutLayoutParams> lp;
            child->GetLayoutParams((IViewGroupLayoutParams**)&lp);
            Int32 ml, mt, mr, mb;
            lp->GetMargins(&ml, &mt, &mr, &mb);

            Int32 gravity;
            lp->GetGravity(&gravity);
            if (gravity == -1) {
                gravity = DEFAULT_CHILD_GRAVITY;
            }

            Int32 width, height;
            child->GetMeasuredWidth(&width);
            child->GetMeasuredHeight(&height);
            Int32 childLeft, childTop;

            Int32 layoutDirection = GetLayoutDirection();
            Int32 absoluteGravity = Gravity::GetAbsoluteGravity(gravity, layoutDirection);
            Int32 verticalGravity = gravity & IGravity::VERTICAL_GRAVITY_MASK;

            switch (absoluteGravity & IGravity::HORIZONTAL_GRAVITY_MASK) {
                    case IGravity::LEFT:
                        childLeft = parentLeft + ml;
                        break;
                    case IGravity::CENTER_HORIZONTAL:
                        childLeft = parentLeft + (parentRight - parentLeft - width) / 2 +
                            ml - mr;
                        break;
                    case IGravity::RIGHT:
                        childLeft = parentRight - width - mr;
                        break;
                    default:
                        childLeft = parentLeft + ml;
            }

            switch (verticalGravity) {
                    case IGravity::TOP:
                        childTop = parentTop + mt;
                        break;
                    case IGravity::CENTER_VERTICAL:
                        childTop = parentTop + (parentBottom - parentTop - height) / 2 +
                            mt - mb;
                        break;
                    case IGravity::BOTTOM:
                        childTop = parentBottom - height - mb;
                        break;
                    default:
                        childTop = parentTop + mt;
            }

            child->Layout(childLeft, childTop, childLeft + width, childTop + height);
        }
    }
}

void FrameLayout::OnSizeChanged(
    /* [in] */ Int32 w,
    /* [in] */ Int32 h,
    /* [in] */ Int32 oldw,
    /* [in] */ Int32 oldh)
{
    ViewGroup::OnSizeChanged(w, h, oldw, oldh);
    mForegroundBoundsChanged = TRUE;
}

ECode FrameLayout::Draw(
    /* [in] */ ICanvas* canvas)
{
    ViewGroup::Draw(canvas);

    if (mForeground != NULL) {
        AutoPtr<IDrawable> foreground = mForeground;

        if (mForegroundBoundsChanged) {
            mForegroundBoundsChanged = FALSE;
            AutoPtr<IRect> selfBounds = (IRect*)mSelfBounds.Get();
            AutoPtr<IRect> overlayBounds = (IRect*)mOverlayBounds.Get();

            Int32 w = mRight - mLeft;
            Int32 h = mBottom - mTop;

            if (mForegroundInPadding) {
                selfBounds->Set(0, 0, w, h);
            } else {
                selfBounds->Set(mPaddingLeft, mPaddingTop, w - mPaddingRight,
                        h - mPaddingBottom);
            }

            Int32 layoutDirection = GetLayoutDirection();

            Int32 iWidth, iHeight;
            foreground->GetIntrinsicWidth(&iWidth);
            foreground->GetIntrinsicHeight(&iHeight);
            Gravity::Apply(mForegroundGravity, iWidth, iHeight,
                selfBounds, overlayBounds,
                layoutDirection);
            foreground->SetBounds(overlayBounds);
        }

        foreground->Draw(canvas);
    }

    return NOERROR;
}

Boolean FrameLayout::GatherTransparentRegion(
    /* [in] */ IRegion* region)
{
    Boolean opaque = ViewGroup::GatherTransparentRegion(region);
    if (region != NULL && mForeground != NULL) {
        ApplyDrawableToTransparentRegion(mForeground, region);
    }
    return opaque;
}

ECode FrameLayout::SetMeasureAllChildren(
    /* [in] */ Boolean measureAll)
{
    mMeasureAllChildren = measureAll;
    return NOERROR;
}

Boolean FrameLayout::GetConsiderGoneChildrenWhenMeasuring()
{
    return GetMeasureAllChildren();
}

Boolean FrameLayout::GetMeasureAllChildren()
{
    return mMeasureAllChildren;
}

ECode FrameLayout::GenerateLayoutParams(
    /* [in] */ IAttributeSet* attrs,
    /* [out] */ IViewGroupLayoutParams** params)
{
    VALIDATE_NOT_NULL(params);
    AutoPtr<IFrameLayoutLayoutParams> lp;
    FAIL_RETURN(CFrameLayoutLayoutParams::New(GetContext(), attrs, (IFrameLayoutLayoutParams**)&lp));
    *params = IViewGroupLayoutParams::Probe(lp);
    REFCOUNT_ADD(*params);
    return NOERROR;
}

//@Override
Boolean FrameLayout::ShouldDelayChildPressedState()
{
    return FALSE;
}

Boolean FrameLayout::CheckLayoutParams(
    /* [in] */ IViewGroupLayoutParams* p)
{
    return p && (p->Probe(EIID_IFrameLayoutLayoutParams) != NULL);
}

AutoPtr<IViewGroupLayoutParams> FrameLayout::GenerateLayoutParams(
    /* [Int32] */ IViewGroupLayoutParams* p)
{
    assert(p);
    AutoPtr<IFrameLayoutLayoutParams> temp;
    ASSERT_SUCCEEDED(CFrameLayoutLayoutParams::New(p,
        (IFrameLayoutLayoutParams**)&temp));
    AutoPtr<IViewGroupLayoutParams> params = IViewGroupLayoutParams::Probe(temp);
    return params;
}

ECode FrameLayout::OnInitializeAccessibilityEvent(
    /* [in] */ IAccessibilityEvent* event)
{
    ViewGroup::OnInitializeAccessibilityEvent(event);
    AutoPtr<ICharSequence> seq;
    CStringWrapper::New(String("CFrameLayout"), (ICharSequence**)&seq);
    event->SetClassName(seq);
    return NOERROR;
}

ECode FrameLayout::OnInitializeAccessibilityNodeInfo(
    /* [in] */ IAccessibilityNodeInfo* info)
{
    ViewGroup::OnInitializeAccessibilityNodeInfo(info);
    AutoPtr<ICharSequence> seq;
    CStringWrapper::New(String("CFrameLayout"), (ICharSequence**)&seq);
    info->SetClassName(seq);
    return NOERROR;
}

}// namespace Widget
}// namespace Droid
}// namespace Elastos
