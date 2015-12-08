#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/widget/TableRowLayoutParams.h"
#include "elastos/droid/R.h"

using Elastos::Droid::R;

namespace Elastos {
namespace Droid {
namespace Widget {

// {cd3e5b3e-2721-41f2-8793-bcc39e03482f}
extern "C" const InterfaceID EIID_TableRowLayoutParams =
{ 0xcd3e5b3e, 0x2721, 0x41f2, { 0x87, 0x93, 0xbc, 0xc3, 0x9e, 0x03, 0x48, 0x2f} };

const Int32 TableRowLayoutParams::LOCATION;
const Int32 TableRowLayoutParams::LOCATION_NEXT;

/**
 * {@inheritDoc}
 */
TableRowLayoutParams::TableRowLayoutParams(
    /* [in] */ IContext* c,
    /* [in] */ IAttributeSet* attrs)
    : LinearLayout::LayoutParams(c, attrs)
    , mColumn(0)
    , mSpan(0)
{
    ASSERT_SUCCEEDED(InitFromAttributes(c, attrs));
}

ECode TableRowLayoutParams::InitFromAttributes(
    /* [in] */ IContext* c,
    /* [in] */ IAttributeSet* attrs)
{
    AutoPtr<ArrayOf<Int32> > attrIds = ArrayOf<Int32>::Alloc(
            const_cast<Int32 *>(R::styleable::TableRow_Cell),
            ARRAY_SIZE(R::styleable::TableRow_Cell));
    AutoPtr<ITypedArray> a;
    c->ObtainStyledAttributes(
            attrs, attrIds, (ITypedArray**)&a);

    a->GetInt32(R::styleable::TableRow_Cell_layout_column,
            -1, &mColumn);
    a->GetInt32(R::styleable::TableRow_Cell_layout_span,
            1, &mSpan);
    if (mSpan <= 1) {
        mSpan = 1;
    }

    a->Recycle();

    return NOERROR;
}

/**
 * <p>Sets the child width and the child height.</p>
 *
 * @param w the desired width
 * @param h the desired height
 */
TableRowLayoutParams::TableRowLayoutParams(
    /* [in] */ Int32 w,
    /* [in] */ Int32 h)
    : LinearLayout::LayoutParams(w, h)
    , mColumn(-1)
    , mSpan(1)
{
}

/**
 * <p>Sets the child width, height and weight.</p>
 *
 * @param w the desired width
 * @param h the desired height
 * @param initWeight the desired weight
 */
TableRowLayoutParams::TableRowLayoutParams(
    /* [in] */ Int32 w,
    /* [in] */ Int32 h,
    /* [in] */ Float initWeight)
    : LinearLayout::LayoutParams(w, h, initWeight)
    , mColumn(-1)
    , mSpan(1)
{
}

/**
 * <p>Sets the child width to {@link android.view.ViewGroup.LayoutParams}
 * and the child height to
 * {@link android.view.ViewGroup.LayoutParams#WRAP_CONTENT}.</p>
 */
TableRowLayoutParams::TableRowLayoutParams()
    : LinearLayout::LayoutParams(
            IViewGroupLayoutParams::MATCH_PARENT,
            IViewGroupLayoutParams::WRAP_CONTENT)
    , mColumn(-1)
    , mSpan(1)
{
}

/**
 * <p>Puts the view in the specified column.</p>
 *
 * <p>Sets the child width to {@link android.view.ViewGroup.LayoutParams#MATCH_PARENT}
 * and the child height to
 * {@link android.view.ViewGroup.LayoutParams#WRAP_CONTENT}.</p>
 *
 * @param column the column index for the view
 */
TableRowLayoutParams::TableRowLayoutParams(
    /* [in] */ Int32 column)
    : LinearLayout::LayoutParams(
        IViewGroupLayoutParams::MATCH_PARENT,
        IViewGroupLayoutParams::WRAP_CONTENT)
    , mColumn(column)
    , mSpan(1)
{
}

/**
 * {@inheritDoc}
 */
TableRowLayoutParams::TableRowLayoutParams(
    /* [in] */ LayoutParams* p)
    : LinearLayout::LayoutParams(p)
    , mColumn(-1)
    , mSpan(1)
{
}

/**
 * {@inheritDoc}
 */
TableRowLayoutParams::TableRowLayoutParams(
    /* [in] */ ViewGroup::MarginLayoutParams* source)
    : LinearLayout::LayoutParams(source)
    , mColumn(-1)
    , mSpan(1)
{
}

ECode TableRowLayoutParams::SetColumn(
    /* [in] */ Int32 column)
{
    mColumn = column;
    return NOERROR;
}

Int32 TableRowLayoutParams::GetColumn()
{
    return mColumn;
}

ECode TableRowLayoutParams::SetSpan(
    /* [in] */ Int32 span)
{
    mSpan = span;
    return NOERROR;
}

Int32 TableRowLayoutParams::GetSpan()
{
    return mSpan;
}

ECode TableRowLayoutParams::SetBaseAttributes(
    /* [in] */ ITypedArray* a,
    /* [in] */ Int32 widthAttr,
    /* [in] */ Int32 heightAttr)
{
    // We don't want to force users to specify a layout_width
    Boolean res;
    a->HasValue(widthAttr, &res);

    if (res) {
        FAIL_RETURN(a->GetLayoutDimension(widthAttr, String("layout_width"), &mWidth));
    }
    else {
        mWidth = IViewGroupLayoutParams::MATCH_PARENT;
    }

    // We don't want to force users to specify a layout_height
    a->HasValue(heightAttr, &res);
    if (res) {
        FAIL_RETURN(a->GetLayoutDimension(heightAttr, String("layout_height"), &mHeight));
    }
    else {
        mHeight = IViewGroupLayoutParams::WRAP_CONTENT;
    }
    return NOERROR;
}

ECode TableRowLayoutParams::Init(
    /* [in] */ IContext* c,
    /* [in] */ IAttributeSet* attrs)
{
    FAIL_RETURN(LinearLayout::LayoutParams::Init(c, attrs));
    return InitFromAttributes(c, attrs);
}

ECode TableRowLayoutParams::Init(
    /* [in] */ Int32 w,
    /* [in] */ Int32 h)
{
    FAIL_RETURN(LinearLayout::LayoutParams::Init(w, h));
    mColumn = -1;
    mSpan = 1;
    return NOERROR;
}

ECode TableRowLayoutParams::Init(
    /* [in] */ Int32 w,
    /* [in] */ Int32 h,
    /* [in] */ Float initWeight)
{
    FAIL_RETURN(LinearLayout::LayoutParams::Init(w, h, initWeight));
    mColumn = -1;
    mSpan = 1;
    return NOERROR;
}

ECode TableRowLayoutParams::Init()
{
    FAIL_RETURN(LinearLayout::LayoutParams::Init(
            IViewGroupLayoutParams::MATCH_PARENT,
            IViewGroupLayoutParams::WRAP_CONTENT));
    mColumn = -1;
    mSpan = 1;
    return NOERROR;
}

ECode TableRowLayoutParams::Init(
    /* [in] */ Int32 column)
{
    FAIL_RETURN(LinearLayout::LayoutParams::Init(
            IViewGroupLayoutParams::MATCH_PARENT,
            IViewGroupLayoutParams::WRAP_CONTENT));
    mColumn = column;
    mSpan = 1;
    return NOERROR;
}

ECode TableRowLayoutParams::Init(
    /* [in] */ IViewGroupLayoutParams* p)
{
    FAIL_RETURN(LinearLayout::LayoutParams::Init(p));
    mColumn = -1;
    mSpan = 1;
    return NOERROR;
}

ECode TableRowLayoutParams::Init(
    /* [in] */ IViewGroupMarginLayoutParams* source)
{
    FAIL_RETURN(LinearLayout::LayoutParams::Init(source));
    mColumn = -1;
    mSpan = 1;
    return NOERROR;
}

} // namespace Widget
} // namespace Droid
} // namespace Elastos
