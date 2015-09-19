
#include "view/menu/IconMenuViewLayoutParams.h"

namespace Elastos {
namespace Droid {
namespace View {
namespace Menu {

IconMenuViewLayoutParams::IconMenuViewLayoutParams()
    : mLeft(0)
    , mTop(0)
    , mRight(0)
    , mBottom(0)
    , mDesiredWidth(0)
    , mMaxNumItemsOnRow(0)
{}

IconMenuViewLayoutParams::IconMenuViewLayoutParams(
    /* [in] */ IContext* c,
    /* [in] */ IAttributeSet* attrs)
    : ViewGroupMarginLayoutParams(c, attrs)
    , mLeft(0)
    , mTop(0)
    , mRight(0)
    , mBottom(0)
    , mDesiredWidth(0)
    , mMaxNumItemsOnRow(0)
{}

IconMenuViewLayoutParams::IconMenuViewLayoutParams(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
    : ViewGroupMarginLayoutParams(width, height)
    , mLeft(0)
    , mTop(0)
    , mRight(0)
    , mBottom(0)
    , mDesiredWidth(0)
    , mMaxNumItemsOnRow(0)
{}

ECode IconMenuViewLayoutParams::Init(
    /* [in] */ IContext* c,
    /* [in] */ IAttributeSet* attrs)
{
    return ViewGroupMarginLayoutParams::Init(c, attrs);
}

ECode IconMenuViewLayoutParams::Init(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    return ViewGroupMarginLayoutParams::Init(width, height);
}

} // namespace Menu
} // namespace View
} // namespace Droid
} // namespace Elastos
