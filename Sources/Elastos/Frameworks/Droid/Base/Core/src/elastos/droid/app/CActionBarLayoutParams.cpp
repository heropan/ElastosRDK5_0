
#include "elastos/droid/app/CActionBarLayoutParams.h"
#include "elastos/droid/R.h"

using Elastos::Droid::R;
using Elastos::Droid::Content::Res::ITypedArray;
using Elastos::Droid::View::IGravity;

namespace Elastos {
namespace Droid {
namespace App {

CActionBarLayoutParams::CActionBarLayoutParams()
    : mGravity(0)
{}

ECode CActionBarLayoutParams::constructor(
        /* [in] */ IContext* ctx,
        /* [in] */ IAttributeSet* attrs)
{
    ViewGroupMarginLayoutParams::Init(ctx, attrs);

    AutoPtr<ArrayOf<Int32> > attrIds = ArrayOf<Int32>::Alloc(
        const_cast<Int32 *>(R::styleable::ActionBar_LayoutParams),
        ARRAY_SIZE(R::styleable::ActionBar_LayoutParams));

    AutoPtr<ITypedArray> a;
    ctx->ObtainStyledAttributes(attrs, attrIds, (ITypedArray**)&a);
    a->GetInt32(R::styleable::ActionBar_LayoutParams_layout_gravity, IGravity::NO_GRAVITY, &mGravity);
    a->Recycle();
    return NOERROR;
}

ECode CActionBarLayoutParams::constructor(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height)
{
    ViewGroupMarginLayoutParams::Init(width, height);
    mGravity = IGravity::CENTER_VERTICAL | IGravity::START;
    return NOERROR;
}

ECode CActionBarLayoutParams::constructor(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ Int32 gravity)
{
    ViewGroupMarginLayoutParams::Init(width, height);
    mGravity = gravity;
    return NOERROR;
}

ECode CActionBarLayoutParams::constructor(
        /* [in] */ Int32 gravity)
{
    ViewGroupMarginLayoutParams::Init(IViewGroupLayoutParams::WRAP_CONTENT, IViewGroupLayoutParams::MATCH_PARENT);
    mGravity = gravity;
    return NOERROR;
}

ECode CActionBarLayoutParams::constructor(
        /* [in] */ IActionBarLayoutParams* ablp)
{
    ViewGroupMarginLayoutParams::Init(ablp);
    ablp->GetGravity(&mGravity);
    return NOERROR;
}

ECode CActionBarLayoutParams::constructor(
        /* [in] */ IViewGroupLayoutParams* vglp)
{
    ViewGroupMarginLayoutParams::Init(vglp);
    return NOERROR;
}

ECode CActionBarLayoutParams::GetGravity(
        /* [out] */ Int32* gravity)
{
    *gravity = mGravity;
    return NOERROR;
}

ECode CActionBarLayoutParams::SetGravity(
        /* [in] */ Int32 gravity)
{
    mGravity = gravity;
    return NOERROR;
}

ECode CActionBarLayoutParams::GetLeftMargin(
        /* [out] */ Int32* leftMargin)
{
    return ViewGroupMarginLayoutParams::GetLeftMargin(leftMargin);
}

ECode CActionBarLayoutParams::SetLeftMargin(
        /* [in] */ Int32 leftMargin)
{
    return ViewGroupMarginLayoutParams::SetLeftMargin(leftMargin);
}

ECode CActionBarLayoutParams::GetTopMargin(
        /* [out] */ Int32* topMargin)
{
    return ViewGroupMarginLayoutParams::GetTopMargin(topMargin);
}

ECode CActionBarLayoutParams::SetTopMargin(
        /* [in] */ Int32 topMargin)
{
    return ViewGroupMarginLayoutParams::SetTopMargin(topMargin);
}

ECode CActionBarLayoutParams::GetRightMargin(
        /* [out] */ Int32* rightMargin)
{
    return ViewGroupMarginLayoutParams::GetRightMargin(rightMargin);
}

ECode CActionBarLayoutParams::SetRightMargin(
        /* [in] */ Int32 rightMargin)
{
    return ViewGroupMarginLayoutParams::SetRightMargin(rightMargin);
}

ECode CActionBarLayoutParams::GetBottomMargin(
        /* [out] */ Int32* bottomMargin)
{
    return ViewGroupMarginLayoutParams::GetBottomMargin(bottomMargin);
}

ECode CActionBarLayoutParams::SetBottomMargin(
        /* [in] */ Int32 bottomMargin)
{
    return ViewGroupMarginLayoutParams::SetBottomMargin(bottomMargin);
}

ECode CActionBarLayoutParams::SetMargins(
        /* [in] */ Int32 left,
        /* [in] */ Int32 top,
        /* [in] */ Int32 right,
        /* [in] */ Int32 bottom)
{
    return ViewGroupMarginLayoutParams::SetMargins(left, top, right, bottom);
}

ECode CActionBarLayoutParams::GetMargins(
        /* [out] */ Int32* left,
        /* [out] */ Int32* top,
        /* [out] */ Int32* right,
        /* [out] */ Int32* bottom)
{
    return ViewGroupMarginLayoutParams::GetMargins(left, top, right, bottom);
}

ECode CActionBarLayoutParams::SetMarginsRelative(
        /* [in] */ Int32 start,
        /* [in] */ Int32 top,
        /* [in] */ Int32 end,
        /* [in] */ Int32 bottom)
{
    return ViewGroupMarginLayoutParams::SetMarginsRelative(start, top, end, bottom);
}

ECode CActionBarLayoutParams::SetMarginStart(
        /* [in] */ Int32 start)
{
    return ViewGroupMarginLayoutParams::SetMarginStart(start);
}

ECode CActionBarLayoutParams::GetMarginStart(
        /* [out] */ Int32* startMargin)
{
    *startMargin = ViewGroupMarginLayoutParams::GetMarginStart();
    return NOERROR;
}

ECode CActionBarLayoutParams::SetMarginEnd(
        /* [in] */ Int32 end)
{
    return ViewGroupMarginLayoutParams::SetMarginEnd(end);
}

ECode CActionBarLayoutParams::GetMarginEnd(
        /* [out] */ Int32* endMargin)
{
    *endMargin = ViewGroupMarginLayoutParams::GetMarginEnd();
    return NOERROR;
}

ECode CActionBarLayoutParams::IsMarginRelative(
        /* [out] */ Boolean* set)
{
    *set = ViewGroupMarginLayoutParams::IsMarginRelative();
    return NOERROR;
}

ECode CActionBarLayoutParams::SetLayoutDirection(
        /* [in] */ Int32 layoutDirection)
{
    return ViewGroupMarginLayoutParams::SetLayoutDirection(layoutDirection);
}

ECode CActionBarLayoutParams::GetLayoutDirection(
        /* [out] */ Int32* layoutDirection)
{
    *layoutDirection = ViewGroupMarginLayoutParams::GetLayoutDirection();
    return NOERROR;
}

ECode CActionBarLayoutParams::IsLayoutRtl(
        /* [out] */ Boolean* rtl)
{
    *rtl = ViewGroupMarginLayoutParams::IsLayoutRtl();
    return NOERROR;
}

ECode CActionBarLayoutParams::SetWidth(
        /* [in] */ Int32 width)
{
    return ViewGroupMarginLayoutParams::SetWidth(width);
}

ECode CActionBarLayoutParams::SetHeight(
        /* [in] */ Int32 height)
{
    return ViewGroupMarginLayoutParams::SetHeight(height);
}

ECode CActionBarLayoutParams::GetWidth(
        /* [out] */ Int32* width)
{
    *width = ViewGroupMarginLayoutParams::GetWidth();
    return NOERROR;
}

ECode CActionBarLayoutParams::GetHeight(
        /* [out] */ Int32* height)
{
    *height = ViewGroupMarginLayoutParams::GetHeight();
    return NOERROR;
}

ECode CActionBarLayoutParams::SetLayoutAnimationParameters(
        /* [in] */ IAnimationParameters* ap)
{
    return ViewGroupMarginLayoutParams::SetLayoutAnimationParameters(ap);
}

ECode CActionBarLayoutParams::GetLayoutAnimationParameters(
        /* [out] */ IAnimationParameters** ap)
{
    return ViewGroupMarginLayoutParams::GetLayoutAnimationParameters(ap);
}

ECode CActionBarLayoutParams::ResolveLayoutDirection(
        /* [in] */ Int32 layoutDirection)
{
    return ViewGroupMarginLayoutParams::ResolveLayoutDirection(layoutDirection);
}

ECode CActionBarLayoutParams::OnDebugDraw(
        /* [in] */ IView* view,
        /* [in] */ ICanvas* canvas)
{
    return ViewGroupMarginLayoutParams::OnDebugDraw(view, canvas);
}

} // namespace App
} // namespace Droid
} // namespace Elastos
