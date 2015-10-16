#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/widget/FrameLayoutLayoutParams.h"
#include "elastos/droid/R.h"

namespace Elastos{
namespace Droid{
namespace Widget{


FrameLayoutLayoutParams::FrameLayoutLayoutParams()
    : mGravity(-1)
{}

/**
 * {@inheritDoc}
 */
FrameLayoutLayoutParams::FrameLayoutLayoutParams(
    /* [in] */ IContext* c,
    /* [in] */ IAttributeSet* attrs)
    : mGravity(-1)
{
    ASSERT_SUCCEEDED(Init(c, attrs));
}

/**
 * {@inheritDoc}
 */
FrameLayoutLayoutParams::FrameLayoutLayoutParams(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
    : ViewGroupMarginLayoutParams(width, height)
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
 * @param gravity the gravity
 *
 * @see android.view.Gravity
 */
FrameLayoutLayoutParams::FrameLayoutLayoutParams(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Int32 gravity)
    : ViewGroupMarginLayoutParams(width, height)
    , mGravity(gravity)
{}

/**
 * {@inheritDoc}
 */
FrameLayoutLayoutParams::FrameLayoutLayoutParams(
    /* [in] */ ViewGroupLayoutParams* source)
    : ViewGroupMarginLayoutParams(source)
    , mGravity(-1)
{}

/**
 * {@inheritDoc}
 */
FrameLayoutLayoutParams::FrameLayoutLayoutParams(
    /* [in] */ ViewGroupMarginLayoutParams* source)
    : ViewGroupMarginLayoutParams(source)
    , mGravity(-1)
{}


ECode FrameLayoutLayoutParams::Init(
    /* [in] */ IContext* c,
    /* [in] */ IAttributeSet* attrs)
{
    FAIL_RETURN(ViewGroupMarginLayoutParams::Init(c, attrs));

    AutoPtr<ArrayOf<Int32> > attrIds = ArrayOf<Int32>::Alloc(
            const_cast<Int32 *>(R::styleable::FrameLayout_Layout),
            ARRAY_SIZE(R::styleable::FrameLayout_Layout));
    AutoPtr<ITypedArray> a;
    ASSERT_SUCCEEDED(c->ObtainStyledAttributes(attrs, attrIds, (ITypedArray**)&a));
    a->GetInt32(R::styleable::FrameLayout_Layout_layout_gravity, -1, &mGravity);

    a->Recycle();
    return NOERROR;
}

ECode FrameLayoutLayoutParams::Init(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    return ViewGroupMarginLayoutParams::Init(width, height);
}

ECode FrameLayoutLayoutParams::Init(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Int32 gravity)
{
    ViewGroupMarginLayoutParams::Init(width, height);
    mGravity = gravity;
    return NOERROR;
}

ECode FrameLayoutLayoutParams::Init(
    /* [in] */ IViewGroupLayoutParams* source)
{
    return ViewGroupMarginLayoutParams::Init(source);
}

ECode FrameLayoutLayoutParams::Init(
    /* [in] */ IViewGroupMarginLayoutParams* source)
{
    return ViewGroupMarginLayoutParams::Init(source);
}

Int32 FrameLayoutLayoutParams::GetGravity()
{
    return mGravity;
}

ECode FrameLayoutLayoutParams::SetGravity(
    /* [in] */ Int32 gravity)
{
    mGravity = gravity;
    return NOERROR;
}

}// namespace Widget
}// namespace Droid
}// namespace Elastos
