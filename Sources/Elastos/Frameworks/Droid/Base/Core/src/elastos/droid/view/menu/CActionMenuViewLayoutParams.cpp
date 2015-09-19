
#include "view/menu/CActionMenuViewLayoutParams.h"

using Elastos::Droid::View::IViewGroupMarginLayoutParams;

namespace Elastos {
namespace Droid {
namespace View {
namespace Menu {

IVIEWGROUPLP_METHODS_IMPL(CActionMenuViewLayoutParams, LinearLayoutLayoutParams)
IVIEWGROUPMARGINLP_METHODS_IMPL(CActionMenuViewLayoutParams, LinearLayoutLayoutParams)

CActionMenuViewLayoutParams::CActionMenuViewLayoutParams()
    : mIsOverflowButton(0)
    , mCellsUsed(0)
    , mExtraPixels(0)
    , mExpandable(0)
    , mPreventEdgeOffset(0)
    , mExpanded(0)
{}

ECode CActionMenuViewLayoutParams::constructor(
    /* [in] */ IContext* ctx,
    /* [in] */ IAttributeSet* attrs)
{
    LinearLayoutLayoutParams::Init(ctx, attrs);
    return NOERROR;
}

ECode CActionMenuViewLayoutParams::constructor(
    /* [in] */ IViewGroupLayoutParams* other)
{
    LinearLayoutLayoutParams::Init(other);
    return NOERROR;
}

ECode CActionMenuViewLayoutParams::constructor(
    /* [in] */ IActionMenuViewLayoutParams* actLp)
{
    IViewGroupMarginLayoutParams* temp = IViewGroupMarginLayoutParams::Probe(actLp);
    LinearLayoutLayoutParams::Init(temp);
    actLp->GetIsOverflowButton(&mIsOverflowButton);
    return NOERROR;
}

ECode CActionMenuViewLayoutParams::constructor(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    LinearLayoutLayoutParams::Init(width, height);
    mIsOverflowButton = FALSE;
    return NOERROR;
}

ECode CActionMenuViewLayoutParams::constructor(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Boolean isOverflowButton)
{
    LinearLayoutLayoutParams::Init(width, height);
    mIsOverflowButton = isOverflowButton;
    return NOERROR;
}

ECode CActionMenuViewLayoutParams::GetWeight(
        /* [out] */ Float* weight)
{
    *weight = LinearLayoutLayoutParams::GetWeight();
    return NOERROR;
}

ECode CActionMenuViewLayoutParams::SetWeight(
        /* [in] */ Float weight)
{
    return LinearLayoutLayoutParams::SetWeight(weight);
}

ECode CActionMenuViewLayoutParams::GetGravity(
        /* [out] */ Int32* gravity)
{
    *gravity = LinearLayoutLayoutParams::GetGravity();
    return NOERROR;
}

ECode CActionMenuViewLayoutParams::SetGravity(
        /* [in] */ Int32 gravity)
{
    return LinearLayoutLayoutParams::SetGravity(gravity);
}


ECode CActionMenuViewLayoutParams::SetIsOverflowButton(
    /* [in] */ Boolean isOverflow)
{
    mIsOverflowButton = isOverflow;
    return NOERROR;
}

ECode CActionMenuViewLayoutParams::GetIsOverflowButton(
    /* [out] */ Boolean* isOverflow)
{
    *isOverflow = mIsOverflowButton;
    return NOERROR;
}

ECode CActionMenuViewLayoutParams::SetCellsUsed(
    /* [in] */ Int32 cellsUsed)
{
    mCellsUsed = cellsUsed;
    return NOERROR;
}

ECode CActionMenuViewLayoutParams::GetCellsUsed(
    /* [out] */ Int32* cellsUsed)
{
    *cellsUsed = mCellsUsed;
    return NOERROR;
}

ECode CActionMenuViewLayoutParams::SetExtraPixels(
    /* [in] */ Int32 extraPixels)
{
    mExtraPixels = extraPixels;
    return NOERROR;
}

ECode CActionMenuViewLayoutParams::GetExtraPixels(
    /* [out] */ Int32* extraPixels)
{
    *extraPixels = mExtraPixels;
    return NOERROR;
}

ECode CActionMenuViewLayoutParams::SetExpandable(
    /* [in] */ Boolean expandable)
{
    mExpandable = expandable;
    return NOERROR;
}

ECode CActionMenuViewLayoutParams::GetExpandable(
    /* [out] */ Boolean* expandable)
{
    *expandable = mExpandable;
    return NOERROR;
}

ECode CActionMenuViewLayoutParams::SetPreventEdgeOffset(
    /* [in] */ Boolean preventEdgeOffset)
{
    mPreventEdgeOffset = preventEdgeOffset;
    return NOERROR;
}

ECode CActionMenuViewLayoutParams::GetPreventEdgeOffset(
    /* [out] */ Boolean* preventEdgeOffset)
{
    *preventEdgeOffset = mPreventEdgeOffset;
    return NOERROR;
}

ECode CActionMenuViewLayoutParams::SetExpanded(
    /* [in] */ Boolean expanded)
{
    mExpanded = expanded;
    return NOERROR;
}

ECode CActionMenuViewLayoutParams::GetExpanded(
    /* [out] */ Boolean* expanded)
{
    *expanded = mExpanded;
    return NOERROR;
}

} // namespace Menu
} // namespace View
} // namepsace Droid
} // namespace Elastos

