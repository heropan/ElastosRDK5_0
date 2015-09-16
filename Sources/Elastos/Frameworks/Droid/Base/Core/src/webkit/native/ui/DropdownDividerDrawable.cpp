// wuweizuo automatic build .cpp file from .java file.

#include "DropdownDividerDrawable.h"

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Ui {

//=====================================================================
//                       DropdownDividerDrawable
//=====================================================================
DropdownDividerDrawable::DropdownDividerDrawable()
{
    // ==================before translated======================
    // mPaint = new Paint();
    // mDividerRect = new Rect();
}

ECode DropdownDividerDrawable::Draw(
    /* [in] */ ICanvas* canvas)
{
    VALIDATE_NOT_NULL(canvas);
    // ==================before translated======================
    // canvas.drawRect(mDividerRect, mPaint);
    assert(0);
    return NOERROR;
}

ECode DropdownDividerDrawable::OnBoundsChange(
    /* [in] */ IRect* bounds)
{
    VALIDATE_NOT_NULL(bounds);
    // ==================before translated======================
    // mDividerRect.set(0, 0, bounds.width(), mDividerRect.height());
    assert(0);
    return NOERROR;
}

ECode DropdownDividerDrawable::SetHeight(
    /* [in] */ Int32 height)
{
    // ==================before translated======================
    // mDividerRect.set(0, 0, mDividerRect.right, height);
    assert(0);
    return NOERROR;
}

ECode DropdownDividerDrawable::SetColor(
    /* [in] */ Int32 color)
{
    // ==================before translated======================
    // mPaint.setColor(color);
    assert(0);
    return NOERROR;
}

ECode DropdownDividerDrawable::SetAlpha(
    /* [in] */ Int32 alpha)
{
    assert(0);
    return NOERROR;
}

ECode DropdownDividerDrawable::SetColorFilter(
    /* [in] */ IColorFilter* cf)
{
    VALIDATE_NOT_NULL(cf);
    assert(0);
    return NOERROR;
}

Int32 DropdownDividerDrawable::GetOpacity()
{
    // ==================before translated======================
    // return PixelFormat.OPAQUE;
    assert(0);
    return 0;
}

} // namespace Ui
} // namespace Webkit
} // namespace Droid
} // namespace Elastos


