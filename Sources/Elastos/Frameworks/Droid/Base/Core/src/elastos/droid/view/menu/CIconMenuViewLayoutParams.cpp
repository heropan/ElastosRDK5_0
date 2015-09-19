
#include "ext/frameworkext.h"
#include "view/menu/CIconMenuViewLayoutParams.h"

namespace Elastos {
namespace Droid {
namespace View {
namespace Menu {

IVIEWGROUPLP_METHODS_IMPL(CIconMenuViewLayoutParams, IconMenuViewLayoutParams);
IVIEWGROUPMARGINLP_METHODS_IMPL(CIconMenuViewLayoutParams, IconMenuViewLayoutParams);

ECode CIconMenuViewLayoutParams::constructor(
    /* [in] */ IContext* c,
    /* [in] */ IAttributeSet* attrs)
{
    return IconMenuViewLayoutParams::Init(c, attrs);
}

ECode CIconMenuViewLayoutParams::constructor(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    return IconMenuViewLayoutParams::Init(width, height);
}

ECode CIconMenuViewLayoutParams::SetLeft(
    /* [in] */ Int32 left)
{
    mLeft = left;
    return NOERROR;
}

ECode CIconMenuViewLayoutParams::GetLeft(
    /* [out] */ Int32* left)
{
    *left = mLeft;
    return NOERROR;
}

ECode CIconMenuViewLayoutParams::SetTop(
    /* [in] */ Int32 top)
{
    mTop = top;
    return NOERROR;
}

ECode CIconMenuViewLayoutParams::GetTop(
    /* [out] */ Int32* top)
{
    *top = mTop;
    return NOERROR;
}

ECode CIconMenuViewLayoutParams::SetRight(
    /* [in] */ Int32 right)
{
    mRight = right;
    return NOERROR;
}

ECode CIconMenuViewLayoutParams::GetRight(
    /* [out] */ Int32* right)
{
    *right = mRight;
    return NOERROR;
}

ECode CIconMenuViewLayoutParams::SetBottom(
    /* [in] */ Int32 bottom)
{
    mBottom = bottom;
    return NOERROR;
}

ECode CIconMenuViewLayoutParams::GetBottom(
    /* [out] */ Int32* bottom)
{
    *bottom = mBottom;
    return NOERROR;
}

ECode CIconMenuViewLayoutParams::SetDesiredWidth(
    /* [in] */ Int32 desiredWidth)
{
    mDesiredWidth = desiredWidth;
    return NOERROR;
}

ECode CIconMenuViewLayoutParams::GetDesiredWidth(
    /* [out] */ Int32* desiredWidth)
{
    *desiredWidth = mDesiredWidth;
    return NOERROR;
}

ECode CIconMenuViewLayoutParams::SetMaxNumItemsOnRow(
    /* [in] */ Int32 maxNumItemsOnRow)
{
    mMaxNumItemsOnRow = maxNumItemsOnRow;
    return NOERROR;
}

ECode CIconMenuViewLayoutParams::GetMaxNumItemsOnRow(
    /* [out] */ Int32* maxNumItemsOnRow)
{
    *maxNumItemsOnRow = mMaxNumItemsOnRow;
    return NOERROR;
}

} // namespace Menu
} // namespace View
} // namespace Droid
} // namespace Elastos
