
#include "elastos/droid/ext/frameworkdef.h"
#include "elastos/droid/widget/RadioGroupLayoutParams.h"

namespace Elastos {
namespace Droid {
namespace Widget {


RadioGroupLayoutParams::RadioGroupLayoutParams()
{}

/**
 * {@inheritDoc}
 */
RadioGroupLayoutParams::RadioGroupLayoutParams(
    /* [in] */ IContext* c,
    /* [in] */ IAttributeSet* attrs)
    : LinearLayoutLayoutParams(c, attrs)
{
}

/**
 * {@inheritDoc}
 */
RadioGroupLayoutParams::RadioGroupLayoutParams(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
    : LinearLayoutLayoutParams(width, height)
{}

/**
* {@inheritDoc}
*/
RadioGroupLayoutParams::RadioGroupLayoutParams(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Float weight)
    : LinearLayoutLayoutParams(width, height, weight)
{}

/**
 * {@inheritDoc}
 */
RadioGroupLayoutParams::RadioGroupLayoutParams(
    /* [in] */ ViewGroupLayoutParams* p)
    : LinearLayoutLayoutParams(p)
{}

/**
 * {@inheritDoc}
 */
RadioGroupLayoutParams::RadioGroupLayoutParams(
    /* [in] */ ViewGroupMarginLayoutParams* source)
    : LinearLayoutLayoutParams(source)
{}

/**
 * <p>Fixes the child's width to
 * {@link android.view.ViewGroup.LayoutParams#WRAP_CONTENT} and the child's
 * height to  {@link android.view.ViewGroup.LayoutParams#WRAP_CONTENT}
 * when not specified in the XML file.</p>
 *
 * @param a the styled attributes set
 * @param widthAttr the width attribute to fetch
 * @param heightAttr the height attribute to fetch
 */
ECode RadioGroupLayoutParams::SetBaseAttributes(
    /* [in] */ ITypedArray* a,
    /* [in] */ Int32 widthAttr,
    /* [in] */ Int32 heightAttr)
{
    Boolean res;
    a->HasValue(widthAttr, &res);
    if (res) {
        FAIL_RETURN(a->GetLayoutDimension(widthAttr, String("layout_width"), &mWidth));
    }
    else {
        mWidth = IViewGroupLayoutParams::WRAP_CONTENT;
    }

    a->HasValue(heightAttr, &res);
    if (res) {
        FAIL_RETURN(a->GetLayoutDimension(heightAttr, String("layout_height"), &mHeight));
    }
    else {
        mHeight = IViewGroupLayoutParams::WRAP_CONTENT;
    }

    return NOERROR;
}

} // namespace Widget
} // namespace Droid
} // namespace Elastos
