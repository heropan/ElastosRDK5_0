#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/widget/LinearLayoutLayoutParams.h"
#include "elastos/droid/R.h"

namespace Elastos {
namespace Droid {
namespace Widget {


LinearLayoutLayoutParams::LinearLayoutLayoutParams()
    : mGravity(-1)
{}

/**
 * {@inheritDoc}
 */
LinearLayoutLayoutParams::LinearLayoutLayoutParams(
    /* [in] */ IContext* c,
    /* [in] */ IAttributeSet* attrs)
    : mWeight(0)
    , mGravity(-1)
{
    ASSERT_SUCCEEDED(Init(c, attrs));
}

/**
 * {@inheritDoc}
 */
LinearLayoutLayoutParams::LinearLayoutLayoutParams(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
    : ViewGroupMarginLayoutParams(width, height)
    , mWeight(0)
    , mGravity(-1)
{}

/**
 * Creates a new set of layout parameters with the specified width, height
 * and weight.
 *
 * @param width the width, either {@link #MATCH_PARENT},
 *        {@link #WRAP_CONTENT} or a fixed size in pixels
 * @param height the height, either {@link #MATCH_PARENT},
 *        {@link #WRAP_CONTENT} or a fixed size in pixels
 * @param weight the weight
 */
LinearLayoutLayoutParams::LinearLayoutLayoutParams(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Float weight)
    : ViewGroupMarginLayoutParams(width, height)
    , mWeight(weight)
    , mGravity(-1)
{}

/**
 * {@inheritDoc}
 */
LinearLayoutLayoutParams::LinearLayoutLayoutParams(
    /* [in] */ ViewGroupLayoutParams* p)
    : ViewGroupMarginLayoutParams(p)
    , mWeight(0)
    , mGravity(-1)
{}

/**
 * {@inheritDoc}
 */
LinearLayoutLayoutParams::LinearLayoutLayoutParams(
    /* [in] */ ViewGroupMarginLayoutParams* source)
    : ViewGroupMarginLayoutParams(source)
    , mWeight(0)
    , mGravity(-1)
{}

ECode LinearLayoutLayoutParams::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    FAIL_RETURN(ViewGroupMarginLayoutParams::Init(context, attrs));

    AutoPtr<ArrayOf<Int32> > attrIds = ArrayOf<Int32>::Alloc(
            const_cast<Int32 *>(R::styleable::LinearLayout_Layout),
            ARRAY_SIZE(R::styleable::LinearLayout_Layout));
    AutoPtr<ITypedArray> a;
    ASSERT_SUCCEEDED(context->ObtainStyledAttributes(
            attrs, attrIds, (ITypedArray**)&a));

    a->GetFloat(R::styleable::LinearLayout_Layout_layout_weight,
            0, &mWeight);
    a->GetInt32(R::styleable::LinearLayout_Layout_layout_gravity,
            -1, &mGravity);

    a->Recycle();
    return NOERROR;
}

ECode LinearLayoutLayoutParams::Init(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    ViewGroupMarginLayoutParams::Init(width, height);
    mWeight = 0;
    return NOERROR;
}

ECode LinearLayoutLayoutParams::Init(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Float weight)
{
    ViewGroupMarginLayoutParams::Init(width, height);
    mWeight = weight;
    return NOERROR;
}

ECode LinearLayoutLayoutParams::Init(
    /* [in] */ IViewGroupLayoutParams* source)
{
    ViewGroupMarginLayoutParams::Init(source);
    return NOERROR;
}

ECode LinearLayoutLayoutParams::Init(
    /* [in] */ IViewGroupMarginLayoutParams* source)
{
    ViewGroupMarginLayoutParams::Init(source);
    return NOERROR;
}

ECode LinearLayoutLayoutParams::SetWeight(
    /* [in] */ Float weight)
{
    mWeight = weight;
    return NOERROR;
}

Float LinearLayoutLayoutParams::GetWeight()
{
    return mWeight;
}

ECode LinearLayoutLayoutParams::SetGravity(
    /* [in] */ Int32 gravity)
{
    mGravity = gravity;
    return NOERROR;
}

Int32 LinearLayoutLayoutParams::GetGravity()
{
    return mGravity;
}

} // namespace Widget
} // namespace Droid
} // namespace Elastos
