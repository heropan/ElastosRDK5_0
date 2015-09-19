
#include "widget/AbsListViewLayoutParams.h"

namespace Elastos {
namespace Droid {
namespace Widget {

AbsListViewLayoutParams::AbsListViewLayoutParams()
    : mViewType(0)
    , mRecycledHeaderFooter(FALSE)
    , mForceAdd(FALSE)
    , mScrappedFromPosition(0)
    , mItemId(-1)
{}

AbsListViewLayoutParams::AbsListViewLayoutParams(
    /* [in] */ IContext* c,
    /* [in] */ IAttributeSet* attrs)
    : mViewType(0)
    , mRecycledHeaderFooter(FALSE)
    , mForceAdd(FALSE)
    , mScrappedFromPosition(0)
    , mItemId(-1)
{
    ASSERT_SUCCEEDED(Init(c, attrs));
}

AbsListViewLayoutParams::AbsListViewLayoutParams(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
    : ViewGroupLayoutParams(width, height)
    , mViewType(0)
    , mRecycledHeaderFooter(FALSE)
    , mForceAdd(FALSE)
    , mScrappedFromPosition(0)
    , mItemId(-1)
{}

AbsListViewLayoutParams::AbsListViewLayoutParams(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Int32 viewType)
    : ViewGroupLayoutParams(width, height)
    , mViewType(viewType)
    , mRecycledHeaderFooter(FALSE)
    , mForceAdd(FALSE)
    , mScrappedFromPosition(0)
    , mItemId(-1)
{
}

AbsListViewLayoutParams::AbsListViewLayoutParams(
    /* [in] */ ViewGroupLayoutParams* source)
    : ViewGroupLayoutParams(source)
    , mViewType(0)
    , mRecycledHeaderFooter(FALSE)
    , mForceAdd(FALSE)
{}

ECode AbsListViewLayoutParams::Init(
    /* [in] */ IContext* c,
    /* [in] */ IAttributeSet* attrs)
{
    return ViewGroupLayoutParams::Init(c, attrs);
}

ECode AbsListViewLayoutParams::SetItemId(
    /* [in] */ Int32 itemId)
{
    mItemId = itemId;
    return NOERROR;
}

ECode AbsListViewLayoutParams::SetScrappedFromPosition(
    /* [in] */ Int32 scrappedFromPosition)
{
    mScrappedFromPosition = scrappedFromPosition;
    return NOERROR;
}

ECode AbsListViewLayoutParams::SetForceAdd(
    /* [in] */ Boolean force)
{
    mForceAdd = force;
    return NOERROR;
}

ECode AbsListViewLayoutParams::SetRecycledHeaderFooter(
    /* [in] */ Boolean footer)
{
    mRecycledHeaderFooter = footer;
    return NOERROR;
}

ECode AbsListViewLayoutParams::SetViewType(
    /* [in] */ Int32 type)
{
    mViewType = type;
    return NOERROR;
}


ECode AbsListViewLayoutParams::GetItemId(
    /* [out] */ Int32* itemId)
{
    *itemId = mItemId;
    return NOERROR;
}

ECode AbsListViewLayoutParams::GetScrappedFromPosition(
    /* [out] */ Int32* scrappedFromPosition)
{
    *scrappedFromPosition = mScrappedFromPosition;
    return NOERROR;
}

ECode AbsListViewLayoutParams::GetForceAdd(
    /* [out] */ Boolean* force)
{
    *force = mForceAdd;
    return NOERROR;
}

ECode AbsListViewLayoutParams::GetRecycledHeaderFooter(
    /* [out] */ Boolean* footer)
{
    *footer = mRecycledHeaderFooter;
    return NOERROR;
}

ECode AbsListViewLayoutParams::GetViewType(
    /* [in] */ Int32* type)
{
    *type = mViewType;
    return NOERROR;
}

ECode AbsListViewLayoutParams::Init(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    return ViewGroupLayoutParams::Init(width, height);
}

ECode AbsListViewLayoutParams::Init(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Int32 viewType)
{
    ViewGroupLayoutParams::Init(width, height);
    mViewType = viewType;

    return NOERROR;
}

ECode AbsListViewLayoutParams::Init(
    /* [in] */ IViewGroupLayoutParams* source)
{
    return ViewGroupLayoutParams::Init(source);
}

}// namespace Widget
}// namespace Droid
}// namespace Elastos
